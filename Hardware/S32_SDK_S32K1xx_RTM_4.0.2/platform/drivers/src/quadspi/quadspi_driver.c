/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

#include "quadspi_driver.h"
#include "quadspi_hw_access.h"
#include "edma_driver.h"
#include "osif.h"
#include "interrupt_manager.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for working with the dma. Addresses are configured as uint32_t in the dma driver.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long.
 * This is required for working with the dma. Addresses are configured as uint32_t in the dma driver.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type 
 * and a pointer to a different object type.
 * This is needed for callbacks from other modules, which cannot know the actual argument type.
 * Also needed for 32-bit reads from user buffer, per device FIFO record size.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for callbacks from other modules, which cannot know the actual argument type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code 
 * structure and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 13.5, side effects on right hand of logical operator, ''||''
 * QSPI_DRV_GetRxBufFill is a function without side effects.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, local macro not referenced
 * Those are register manipulation macros for platform-specific bit-fields, declared in the same
 * style as in the SoC header file. They could be needed for future enhancements.
 *
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/* Minimum Tx buffer fill needed to allow Tx operation to start */
#define QSPI_TX_MIN_BUF_FILL  4U
/* Wrap-around value for timeout */
#define QSPI_TIMEOUT_WRAP  0xFFFFFFFFU

/* Operations codes for state->operation field */
#define QSPI_OP_READ_DMA      0U
#define QSPI_OP_READ_INT      1U
#define QSPI_OP_WRITE         2U

/* Mask of QuadSPI IP-related error flags */
#define QSPI_ERR_FLAGS_MASK    (QuadSPI_FR_TBUF_MASK | \
                                QuadSPI_FR_ILLINE_MASK | \
                                QuadSPI_FR_RBOF_MASK | \
                                QuadSPI_FR_IPAEF_MASK | \
                                QuadSPI_FR_IPIEF_MASK | \
                                QuadSPI_FR_IPGEF_MASK)


/* Table of base addresses for QuadSPI instances. */
QuadSPI_Type * const g_qspiBase[QuadSPI_INSTANCE_COUNT] = QuadSPI_BASE_PTRS;

/* Pointer to runtime state structure.*/
static qspi_state_t * g_qspiStatePtr[QuadSPI_INSTANCE_COUNT] = {NULL};

/* Table to of QSPI interrupt sources. */
static const IRQn_Type g_qspiIrqId[QuadSPI_INSTANCE_COUNT] = FEATURE_QSPI_IRQS;

/* Declaration of QSPI_IRQHandler. Needed just to avoid MISRA violation. */
void QSPI_IRQHandler(void);

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

 
 /*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_Timeout
 * Description   : Checks for timeout condition
 *
 *END**************************************************************************/
static bool QSPI_DRV_Timeout(uint32_t startTime, uint32_t timeout)
{
    uint32_t currentTime;
    bool retVal;

    currentTime = OSIF_GetMilliseconds();
    if (currentTime >= startTime)
    {
        retVal = ((currentTime - startTime) > timeout)?true:false;
    }
    else
    {
        /* wrap around */
        retVal = ((QSPI_TIMEOUT_WRAP - startTime + currentTime) > timeout)?true:false;
    }
    return retVal;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ProcessData
 * Description   : Processes read data
 *
 *END**************************************************************************/
static inline status_t QSPI_DRV_ProcessData(uint32_t value, qspi_state_t * state)
{
    status_t status = STATUS_SUCCESS;

    if (state->data != NULL)
    {
        /* Normal read */
        *((uint32_t *)(state->data)) = value;
        state->data = &(state->data[4U]);
    }
    else if (state->roData != NULL)
    {
        /* Verify */
        if (*((const uint32_t *)(state->roData)) != value)
        {
            status = STATUS_ERROR;
        }
        state->roData = &(state->roData[4U]);
    }
    else
    {
        /* Blank check */
        if (value != 0xFFFFFFFFU)
        {
            status = STATUS_ERROR;
        }
    }
    state->size -= 4U;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ProcessLastData
 * Description   : Processes last few data bytes (less than 4)
 *
 *END**************************************************************************/
static inline status_t QSPI_DRV_ProcessLastData(uint32_t value, qspi_state_t * state)
{
    uint8_t cnt;
    status_t status = STATUS_SUCCESS;
    uint32_t dataSize;

    dataSize = state->size;
    if (state->data != NULL)
    {
        /* Normal read */
        for (cnt = 0U; cnt < dataSize; cnt++)
        {
            *state->data = (uint8_t)(value & 0xFFU);
            value >>= 8U;
            state->data++;
        }
    }
    else if (state->roData != NULL)
    {
        /* Verify */
        for (cnt = 0U; cnt < dataSize; cnt++)
        {
            if (*(state->roData) != (value & 0xFFU))
            {
                status = STATUS_ERROR;
            }
            value >>= 8U;
            state->roData++;
        }
    }
    else
    {
        /* Blank check */
        if ((~value & (((uint32_t)1U << (dataSize * 8U)) - 1U)) != 0U)
        {
            status = STATUS_ERROR;
        }
    }
    state->size = 0U;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ProcessSyncData
 * Description   : Processes data bytes received in SYNC mode
 *
 *END**************************************************************************/
static inline status_t QSPI_DRV_ProcessSyncData(uint32_t value, qspi_state_t * state)
{
    status_t status;
    /* check received word */
    if (state->size >= 4U)
    {
        status = QSPI_DRV_ProcessData(value, state);
    }
    else
    {
        status = QSPI_DRV_ProcessLastData(value, state);
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_FillTxBuf
 * Description   : Fill Tx buffer with the specified number of 4-byte entries
 *
 *END**************************************************************************/
static void QSPI_DRV_FillTxBuf(QuadSPI_Type *baseAddr, qspi_state_t * state, uint16_t entries)
{
    uint32_t data;
    uint8_t entriesCnt = 0U;
    uint8_t byteCnt;

    while ((entriesCnt < entries) && (state->size > 0U))
    {
        if (state->size < 4U)
        {
            /* Processes last few data bytes (less than 4) */
            data = 0U;
            for (byteCnt = 0U; byteCnt < state->size; byteCnt++)
            {
                data += ((uint32_t)(*(state->roData)) << (8U * byteCnt));
                state->roData++;
            }
            state->size = 0U;
        }
        else
        {
            data = *(const uint32_t *)(state->roData);
            state->size -= 4U;
            state->roData = &(state->roData[4U]);
        }
        
        QSPI_DRV_WriteTxData(baseAddr, data);
        entriesCnt++;
    }

    return;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ErrorCheck
 * Description   : Checks if there were errors during IP command execution
 *
 *END**************************************************************************/
static inline status_t QSPI_DRV_ErrorCheck(QuadSPI_Type *baseAddr)
{
    if ((baseAddr->FR & QSPI_ERR_FLAGS_MASK) != 0U)
    {
        /* clear error flags */
        baseAddr->FR = QSPI_ERR_FLAGS_MASK;
        return STATUS_ERROR;
    }
    return STATUS_SUCCESS;

}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_PadTxBuf
 * Description   : Pad Tx buffer up to a minimum number of entries required 
 *                 by the device for transmission to start
 *
 *END**************************************************************************/
static inline void QSPI_DRV_PadTxBuf(QuadSPI_Type *baseAddr)
{
    uint32_t bufFill;

    bufFill = QSPI_DRV_GetTxBufFill(baseAddr);
    while ((bufFill < QSPI_TX_MIN_BUF_FILL) || ((bufFill & 3U) != 0U))
    {
        QSPI_DRV_WriteTxData(baseAddr, 0xFFFFFFFFU);
        bufFill++;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_SwReset
 * Description   : Resets the QuadSPI device
 *
 *END**************************************************************************/
static void QSPI_DRV_SwReset(QuadSPI_Type *baseAddr)
{
    /* Software reset AHB domain and Serial Flash domain at the same time. */
    QSPI_DRV_SwResetOn(baseAddr);
    /* Disable QuadSPI module before de-asserting the reset bits. */
    QSPI_DRV_Disable(baseAddr);
    /* De-asset Software reset AHB domain and Serial Flash domain bits. */
    QSPI_DRV_SwResetOff(baseAddr);
    /* Re-enable QuadSPI module after reset. */
    QSPI_DRV_Enable(baseAddr);
}


#if (FEATURE_QSPI_CHIP_OPTIONS == 1)
/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ConfigureChipOptions
 * Description   : Configures chip-specific QuadSPI options
 *
 *END**************************************************************************/
static void QSPI_DRV_ConfigureChipOptions(QuadSPI_Type *baseAddr, const qspi_user_config_t * userConfigPtr)
{
    /* always enable pads input buffers */
    uint8_t option = QSPI_MCR_SCLKCFG_INPUT_EN;
    uint32_t chipOption = 0U;

    /* configure MCR options */
    if (userConfigPtr->clock_src == QSPI_CLK_SRC_FIRC_DIV1)
    {
        /* configure clock source selection */
        option |= QSPI_MCR_SCLKCFG_CLK_SRC;
    }
    if (userConfigPtr->readMode == QSPI_READ_MODE_EXTERNAL_DQS)
    {
        /* configure external DQS mode */
        option |= QSPI_MCR_SCLKCFG_EXT_DQS;
    }
    if (userConfigPtr->readMode == QSPI_READ_MODE_LOOPBACK_DQS)
    {
        /* Select loopback clock for both sides and save an if statement for checking the side */
        option |= (uint8_t)(QSPI_MCR_SCLKCFG_DQS_SEL_A | QSPI_MCR_SCLKCFG_DQS_SEL_B);
    }
    if (userConfigPtr->dqsInvert == true)
    {
        option |= (uint8_t)(QSPI_MCR_SCLKCFG_DQS_INV_B | QSPI_MCR_SCLKCFG_DQS_INV_A);
    }
    QSPI_DRV_SetClockOptions(baseAddr, option);

    /* configure SOCCR options */
        /* first disable divider before configuring it */
    QSPI_DRV_SetChipOptions(baseAddr, QuadSPI_SOCCR_PDD_MASK);
    chipOption |= QuadSPI_SOCCR_PD((uint32_t)userConfigPtr->divider - 1U);
    chipOption |= ((uint32_t)userConfigPtr->dqsDelay << QuadSPI_SOCCR_DSQ_DEL_A) + 
                  ((uint32_t)userConfigPtr->dqsDelay << QuadSPI_SOCCR_DSQ_DEL_B);
        /* write configuration, keep divider disabled */
    QSPI_DRV_SetChipOptions(baseAddr, chipOption | QuadSPI_SOCCR_PDD_MASK);
        /* enable divider */
    QSPI_DRV_SetChipOptions(baseAddr, chipOption);
}
#endif  /* FEATURE_QSPI_CHIP_OPTIONS == 1 */


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IRQHandler
 * Description   : Configures chip-specific QuadSPI options
 *
 *END**************************************************************************/
static void QSPI_DRV_IRQHandler(uint32_t instance)
{
    QuadSPI_Type *baseAddr;
    qspi_state_t * state;
    uint8_t cnt;
    uint32_t size;
    status_t status = STATUS_SUCCESS;

    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];

    if (state->operation == QSPI_OP_READ_INT)
    {
        /* For interrupt transfer read the received data from the device Rx buffer */
        size = state->size >> 2U;
        for (cnt = 0U; cnt < size; cnt++)
        {
            status = QSPI_DRV_ProcessData(baseAddr->RBDR[cnt], state);
            if (status != STATUS_SUCCESS)
            {
                break;
            }
        }
        if ((state->size > 0U) && (status == STATUS_SUCCESS))
        {
            /* check incomplete word */
            status = QSPI_DRV_ProcessLastData(baseAddr->RBDR[size], state);
        }
        state->status = status;
        /* Reset Rx queue */
        QSPI_DRV_ClearRxBuf(baseAddr);
    }

    /* No need to check the transfer end event, interrupt is only used for this event */
    QSPI_DRV_DisableInt(baseAddr, QuadSPI_RSER_TFIE_MASK);
    /* For write and interrupt read operations call callback to announce the end event to the user */
    /* For DMA read operations this will be done from DMA callback */
    if (state->operation != QSPI_OP_READ_DMA)
    {
        state->driverBusy = false;
        if (state->callback != NULL)
        {
            state->callback(instance, state->callbackParam);
        }
    }
}


#if (QuadSPI_INSTANCE_COUNT > 0U)
/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_IRQHandler
 * Description   : Implementation of QSPI interrupt handler named in start-up code
 *
 *END**************************************************************************/
void QSPI_IRQHandler(void)
{
    QSPI_DRV_IRQHandler(0U);
}
#endif


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_EndDmaTransfer
 * Description   : Callback that handles the end of a DMA transfer
 *
 *END**************************************************************************/
static void QSPI_DRV_EndDmaTransfer(void *stateStruct, edma_chn_status_t status)
{
    qspi_state_t * state;
    QuadSPI_Type *baseAddr;

    state = (qspi_state_t *)stateStruct;
    baseAddr = g_qspiBase[state->instance];

    /* Record error if there was any */
    if (status == EDMA_CHN_ERROR)
    {
        state->status = STATUS_ERROR;
    }
    (void)EDMA_DRV_StopChannel(state->dmaChannel);
    QSPI_DRV_DisableDmaReq(baseAddr);
    /* For read operations call callback to announce the end event to the user */
    /* For write operations this will be done from interrupt handler */
    if (state->operation == QSPI_OP_READ_DMA)
    {
        state->driverBusy = false;
        if (state->callback != NULL)
        {
            state->callback(state->instance, state->callbackParam);
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ConfigDmaRx
 * Description   : Sets up an Rx DMA transfer
 *
 *END**************************************************************************/
static void QSPI_DRV_ConfigDmaRx(uint32_t instance, qspi_state_t * state)
{
    QuadSPI_Type *baseAddr;
    /* Table of Rx DMA requests. */
    static const uint8_t g_qspiDmaRxSrc[QuadSPI_INSTANCE_COUNT] = FEATURE_QSPI_DMA_RX_REQ;

#if (FEATURE_QSPI_FIXED_DMA_CHANNELS == 1)
    /* Get DMA channel number for Rx */
    state->dmaChannel = g_qspiDmaRxSrc[instance];
#endif
    baseAddr = g_qspiBase[instance];
    /* Set the DMA request source */
    (void)EDMA_DRV_SetChannelRequestAndTrigger(state->dmaChannel, g_qspiDmaRxSrc[instance], false);

    /* Configure the transfer control descriptor for the Rx channel */
    (void)EDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel, 
                                            EDMA_TRANSFER_PERIPH2MEM, 
                                            QSPI_DRV_GetRxDataAddr(baseAddr), 
                                            (uint32_t)(state->data),
                                            EDMA_TRANSFER_SIZE_4B, 
                                            4U, state->size >> 2U, true);

    /* Setup callback for DMA transfer end */
    (void)EDMA_DRV_InstallCallback(state->dmaChannel,
                                   (edma_callback_t)(QSPI_DRV_EndDmaTransfer),
                                   (void*)(state));

    /* Start DMA channel */
    (void)EDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable Rx DMA requests in QSPI */
    QSPI_DRV_EnableRxDmaReq(baseAddr);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_ConfigDmaTx
 * Description   : Sets up a Tx DMA transfer
 *
 *END**************************************************************************/
static void QSPI_DRV_ConfigDmaTx(uint32_t instance, qspi_state_t * state)
{
    QuadSPI_Type *baseAddr;
    /* Table of Tx DMA requests. */
    static const uint8_t g_qspiDmaTxSrc[QuadSPI_INSTANCE_COUNT] = FEATURE_QSPI_DMA_TX_REQ;

#if (FEATURE_QSPI_FIXED_DMA_CHANNELS == 1)
    /* Get DMA channel number for Tx */
    state->dmaChannel = g_qspiDmaTxSrc[instance];
#endif
    baseAddr = g_qspiBase[instance];
    /* Set the DMA request source */
    (void)EDMA_DRV_SetChannelRequestAndTrigger(state->dmaChannel, g_qspiDmaTxSrc[instance], false);

    /* Configure the transfer control descriptor for the Tx channel */
    (void)EDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel, 
                                            EDMA_TRANSFER_MEM2PERIPH, 
                                            (uint32_t)(state->roData),
                                            QSPI_DRV_GetTxDataAddr(baseAddr), 
                                            EDMA_TRANSFER_SIZE_4B, 
                                            4U, state->size >> 2U, true);

    /* Setup callback for DMA transfer end */
    (void)EDMA_DRV_InstallCallback(state->dmaChannel,
                                   (edma_callback_t)(QSPI_DRV_EndDmaTransfer),
                                   (void*)(state));

    /* Start DMA channel */
    (void)EDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable Tx DMA requests in QSPI */
    QSPI_DRV_EnableTxDmaReq(baseAddr);
}


/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_Init
 * Description   : Initializes the qspi driver
 * Implements : QSPI_DRV_Init_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_Init(uint32_t instance,
                       const qspi_user_config_t * userConfigPtr,
                       qspi_state_t * state)
{
    QuadSPI_Type *baseAddr;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    DEV_ASSERT(g_qspiStatePtr[instance] == NULL);

    baseAddr = g_qspiBase[instance];
    g_qspiStatePtr[instance] = state;

    /* Initialize driver status structure */
    state->dmaSupport = userConfigPtr->dmaSupport;
#if (FEATURE_QSPI_FIXED_DMA_CHANNELS == 0)
    state->dmaChannel = userConfigPtr->dmaChannel;
#endif
    state->callback = userConfigPtr->callback;
    state->callbackParam = userConfigPtr->callbackParam;
    state->instance = instance;
    state->driverBusy = false;
    state->status = STATUS_SUCCESS;

    /* Apply configuration settings */
    QSPI_DRV_SetMemMap(baseAddr, userConfigPtr->side, userConfigPtr->memSize);
    QSPI_DRV_SetAddrOptions(baseAddr, userConfigPtr->columnAddr, userConfigPtr->wordAddresable);
    QSPI_DRV_SetRxCfg(baseAddr, userConfigPtr->sampleDelay, userConfigPtr->samplePhase);
    QSPI_DRV_SetCsHoldTime(baseAddr, userConfigPtr->csHoldTime);
    QSPI_DRV_SetCsSetupTime(baseAddr, userConfigPtr->csSetupTime);
    /* Unused side lines are "no matter" so just repeat idle settings on both sides */
    QSPI_DRV_SetIdleLineValues(baseAddr, userConfigPtr->io2IdleValue, userConfigPtr->io3IdleValue, 
                                         userConfigPtr->io2IdleValue, userConfigPtr->io3IdleValue);
    QSPI_DRV_SetEndianess(baseAddr, userConfigPtr->endianess);
    QSPI_DRV_SetDozeMode(baseAddr, userConfigPtr->dozeMode);
    QSPI_DRV_SetRxBufReadout(baseAddr, QSPI_RX_READOUT_IP);
    /* Watermarks are used in DMA mode */
    QSPI_DRV_SetTxWatermark(baseAddr, 2U);
    QSPI_DRV_SetRxWatermark(baseAddr, 1U);

    if (userConfigPtr->dataRate == QSPI_DATE_RATE_SDR)
    {
        /* Check for unsupported modes */
        DEV_ASSERT(((userConfigPtr->readMode == QSPI_READ_MODE_INTERNAL_SAMPLING) && 
                    (userConfigPtr->side == QSPI_FLASH_SIDE_B)) || 
                   ((userConfigPtr->readMode == QSPI_READ_MODE_INTERNAL_SAMPLING) && 
                    (userConfigPtr->side == QSPI_FLASH_SIDE_A)) || 
                   ((userConfigPtr->readMode == QSPI_READ_MODE_INTERNAL_DQS) && 
                    (userConfigPtr->side == QSPI_FLASH_SIDE_A)) || 
                   ((userConfigPtr->readMode == QSPI_READ_MODE_LOOPBACK_DQS) && 
                    (userConfigPtr->side == QSPI_FLASH_SIDE_A)));
        QSPI_DDR_Disable(baseAddr);
        if (userConfigPtr->readMode == QSPI_READ_MODE_INTERNAL_SAMPLING)
        {
            QSPI_DQS_Disable(baseAddr);
        }
        else
        {
            QSPI_DQS_Enable(baseAddr);
        }
    }
    else  /* QSPI_DATE_RATE_DDR */
    {
        /* Check for unsupported modes */
        DEV_ASSERT(userConfigPtr->readMode == QSPI_READ_MODE_EXTERNAL_DQS);
        QSPI_DDR_Enable(baseAddr);
        QSPI_DQS_Enable(baseAddr);
    }

    /* Configure chip-specific options */
#if (FEATURE_QSPI_CHIP_OPTIONS == 1)
    QSPI_DRV_ConfigureChipOptions(baseAddr, userConfigPtr);
#endif

    /* Enable QSPI interrupt. */
    INT_SYS_EnableIRQ(g_qspiIrqId[instance]);

    /* Enable QuadSPI module */
    QSPI_DRV_Enable(baseAddr);
    /* Reset serial flash and AHB domains */
    QSPI_DRV_SwReset(baseAddr);

    /* Initialize OSIF timer to use OSIF_GetMilliseconds() later */
    OSIF_TimeDelay(0);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_Deinit
 * Description   : De-initialize the qspi driver
 * Implements : QSPI_DRV_Deinit_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_Deinit(uint32_t instance)
{
    QuadSPI_Type *baseAddr;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    baseAddr = g_qspiBase[instance];

    /* Disable QuadSPI module */
    QSPI_DRV_Disable(baseAddr);

    /* Enable QSPI interrupt. */
    INT_SYS_DisableIRQ(g_qspiIrqId[instance]);

    g_qspiStatePtr[instance] = NULL;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_AhbSetup
 * Description   : Sets up AHB accesses to the serial flash
 * Implements : QSPI_DRV_AhbSetup_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_AhbSetup(uint32_t instance, const qspi_ahb_config_t *config)
{
    QuadSPI_Type *baseAddr;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    DEV_ASSERT((config->sizes[0U] & 7U) == 0U);
    DEV_ASSERT(((uint32_t)config->sizes[0U] + 
                (uint32_t)config->sizes[1U] + 
                (uint32_t)config->sizes[2U] + 
                (uint32_t)config->sizes[3U]) <= FEATURE_QSPI_AHB_BUF_SIZE);

    baseAddr = g_qspiBase[instance];

    /* configure AHB transfer sizes to match the buffer sizes */
    QSPI_DRV_SetAhbBuf0(baseAddr, config->sizes[0U], config->masters[0U], config->highPriority);
    QSPI_DRV_SetAhbBuf1(baseAddr, config->sizes[1U], config->masters[1U]);
    QSPI_DRV_SetAhbBuf2(baseAddr, config->sizes[2U], config->masters[2U]);
    QSPI_DRV_SetAhbBuf3(baseAddr, config->sizes[3U], config->masters[3U], config->allMasters);

    QSPI_DRV_SetAhbBuf0Ind(baseAddr, (uint32_t)config->sizes[0U]);
    QSPI_DRV_SetAhbBuf1Ind(baseAddr, (uint32_t)config->sizes[0U] + (uint32_t)config->sizes[1U]);
    QSPI_DRV_SetAhbBuf2Ind(baseAddr, (uint32_t)config->sizes[0U] + (uint32_t)config->sizes[1U] + (uint32_t)config->sizes[2U]);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IpCommand
 * Description   : Launches a simple IP command
 * Implements : QSPI_DRV_IpCommand_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_IpCommand(uint32_t instance, uint8_t lut, uint32_t timeout)
{
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    QuadSPI_Type *baseAddr;
    qspi_state_t * state;
    status_t status = STATUS_SUCCESS;
    uint32_t startTime;

    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];
    state->status = STATUS_SUCCESS;
    /* Set dummy address */
    QSPI_DRV_SetIpAddr(baseAddr, 0U);
    /* Trigger IP command with specified sequence and dummy size */
    QSPI_DRV_IpTrigger(baseAddr, lut, 1U);
    /* Wait for command to complete */
    startTime = OSIF_GetMilliseconds();
    do
    {
        status = QSPI_DRV_IpGetStatus(instance);
    }
    while (!QSPI_DRV_Timeout(startTime, timeout) && (status == STATUS_BUSY));
    if (status == STATUS_BUSY)
    {
        status = STATUS_TIMEOUT;
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IpRead
 * Description   : Launches an IP read command
 * Implements : QSPI_DRV_IpRead_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_IpRead(uint32_t instance, 
                         uint8_t lut, 
                         uint32_t addr, 
                         uint8_t * dataRead,
                         const uint8_t * dataCmp,
                         uint32_t size,
                         qspi_transfer_type_t transferType,
                         uint32_t timeout)
{
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    /* For interrupt mode check that the data can fit in the Rx FIFO */
    DEV_ASSERT((transferType != QSPI_TRANSFER_TYPE_ASYNC_INT) || 
               (size <= (QuadSPI_RBDR_COUNT * sizeof(uint32_t))));
    status_t status = STATUS_SUCCESS;
    status_t errors = STATUS_SUCCESS;
    QuadSPI_Type *baseAddr;
    qspi_state_t * state;
    uint32_t startTime;

    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];
    /* Check DMA support if DMA transfer is requested */
    DEV_ASSERT((transferType != QSPI_TRANSFER_TYPE_ASYNC_DMA) || 
               (state->dmaSupport == true));

    state->status = STATUS_SUCCESS;
    state->data = dataRead;
    state->size = size;
    state->roData = dataCmp;
    if (transferType != QSPI_TRANSFER_TYPE_SYNC)
    {
        state->driverBusy = true;
        if (transferType == QSPI_TRANSFER_TYPE_ASYNC_DMA)
        {
            state->operation = QSPI_OP_READ_DMA;
            /* Prepare DMA transfer */
            QSPI_DRV_ConfigDmaRx(instance, state);
        }
        else
        {
            /* clear leftover flags from previous transfers */
            QSPI_DRV_ClearIntFlag(baseAddr, QuadSPI_FR_TFF_MASK);
            /* enable end of transfer interrupt for asynchronous transfers */
            QSPI_DRV_EnableInt(baseAddr, QuadSPI_RSER_TFIE_MASK);
            /* read will be done in interrupt */
            state->operation = QSPI_OP_READ_INT;
        }
    }
    /* Set read address */
    QSPI_DRV_SetIpAddr(baseAddr, addr);
    /* Trigger IP command with specified sequence and size */
    QSPI_DRV_IpTrigger(baseAddr, lut, (uint16_t)size);
    if (transferType == QSPI_TRANSFER_TYPE_SYNC)
    {
        /* Wait for command to be completed */
        startTime = OSIF_GetMilliseconds();
        while (QSPI_DRV_GetBusyStatus(baseAddr) || (QSPI_DRV_GetRxBufFill(baseAddr) != 0U))
        {
            if (QSPI_DRV_GetRxDataEvent(baseAddr))
            {
                if (status == STATUS_SUCCESS)
                {
                    /* check received word */
                    status = QSPI_DRV_ProcessSyncData(baseAddr->RBDR[0U], state);
                }
                QSPI_DRV_RxPop(baseAddr);
            }
            if (QSPI_DRV_Timeout(startTime, timeout))
            {
                status = STATUS_TIMEOUT;
                break;
            }
        }
        /* Reset Rx queue */
        QSPI_DRV_ClearRxBuf(baseAddr);
        /* Check for errors reported by the QuadSPI */
        errors = QSPI_DRV_ErrorCheck(baseAddr);
        if (status == STATUS_SUCCESS)
        {
            status = errors;
        }
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IpWrite
 * Description   : Launches an IP write command
 * Implements : QSPI_DRV_IpWrite_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_IpWrite(uint32_t instance, 
                          uint8_t lut, 
                          uint32_t addr, 
                          const uint8_t * data, 
                          uint32_t size,
                          qspi_transfer_type_t transferType,
                          uint32_t timeout)
{
    QuadSPI_Type *baseAddr;
    qspi_state_t * state;
    status_t status = STATUS_SUCCESS;
    status_t errors = STATUS_SUCCESS;
    uint32_t startTime;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    /* For interrupt mode check that the data can fit in the Tx FIFO */
    DEV_ASSERT((transferType != QSPI_TRANSFER_TYPE_ASYNC_INT) || 
               (size <= FEATURE_QSPI_TX_BUF_SIZE));
    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];
    /* Check DMA support if DMA transfer is requested */
    DEV_ASSERT((transferType != QSPI_TRANSFER_TYPE_ASYNC_DMA) || 
               (state->dmaSupport == true));

    state->operation = QSPI_OP_WRITE;
    state->status = STATUS_SUCCESS;
    state->roData = data;
    state->size = size;

    /* Reset AHB buffers to force re-read from memory after write operation */
    QSPI_DRV_SwReset(baseAddr);

    /* Set write address */
    QSPI_DRV_SetIpAddr(baseAddr, addr);
    /* Fill Tx FIFO with initial data */
    QSPI_DRV_ClearTxBuf(baseAddr);

    if ((transferType == QSPI_TRANSFER_TYPE_ASYNC_DMA) && (state->size > FEATURE_QSPI_TX_BUF_SIZE))
    {
        /* Fill Tx buffer with minimum required number of bytes; DMA will do the rest */
        QSPI_DRV_FillTxBuf(baseAddr, state, (uint16_t)((FEATURE_QSPI_TX_BUF_SIZE >> 2U) - 1U));
        /* Prepare DMA transfer */
        QSPI_DRV_ConfigDmaTx(instance, state);
    }
    else
    {
        /* Fill Tx buffer */
        QSPI_DRV_FillTxBuf(baseAddr, state, FEATURE_QSPI_TX_BUF_SIZE >> 2U);
        /* If not enough Tx data, pad Tx buffer up to 16 bytes */
        QSPI_DRV_PadTxBuf(baseAddr);
    }
    if (transferType != QSPI_TRANSFER_TYPE_SYNC)
    {
        state->driverBusy = true;
        /* clear leftover flags from previous transfers */
        QSPI_DRV_ClearIntFlag(baseAddr, QuadSPI_FR_TFF_MASK);
        /* enable end of transfer interrupt for asynchronous transfers */
        QSPI_DRV_EnableInt(baseAddr, QuadSPI_RSER_TFIE_MASK);
    }


    /* Trigger IP command with specified sequence and size */
    QSPI_DRV_IpTrigger(baseAddr, lut, (uint16_t)size);
    if (transferType == QSPI_TRANSFER_TYPE_SYNC)
    {
        /* Wait for command to be completed */
        startTime = OSIF_GetMilliseconds();
        while (QSPI_DRV_GetBusyStatus(baseAddr))
        {
            /* Keep feeding the Tx FIFO */
            if (QSPI_DRV_GetTxWatermarkAvailable(baseAddr))
            {
                QSPI_DRV_FillTxBuf(baseAddr, state, 1U);
            }
            if (QSPI_DRV_Timeout(startTime, timeout))
            {
                status = STATUS_TIMEOUT;
                break;
            }
        }
        /* Check for errors reported by the QuadSPI */
        errors = QSPI_DRV_ErrorCheck(baseAddr);
        if (status == STATUS_SUCCESS)
        {
            status = errors;
        }
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IpErase
 * Description   : Launches an IP erase command
 * Implements : QSPI_DRV_IpErase_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_IpErase(uint32_t instance, 
                          uint8_t lut, 
                          uint32_t addr)
{
    QuadSPI_Type *baseAddr;
    qspi_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];

    state->status = STATUS_SUCCESS;
    /* Reset AHB buffers to force re-read from memory after erase operation */
    QSPI_DRV_SwReset(baseAddr);
    /* Set erase address */
    QSPI_DRV_SetIpAddr(baseAddr, addr);
    /* Trigger IP command with specified sequence and dummy size */
    QSPI_DRV_IpTrigger(baseAddr, lut, 1U);
    /* Do not wait for command to complete */
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_IpGetStatus
 * Description   : Checks the status of the currently running IP command
 * Implements : QSPI_DRV_IpGetStatus_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_IpGetStatus(uint32_t instance)
{
    qspi_state_t * state;
    QuadSPI_Type *baseAddr;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    state = g_qspiStatePtr[instance];

    if (QSPI_DRV_GetBusyStatus(baseAddr) || state->driverBusy)
    {
        return STATUS_BUSY;
    }
    if (state->status == STATUS_SUCCESS)
    {
        /* Check for errors reported by the QuadSPI */
        state->status = QSPI_DRV_ErrorCheck(baseAddr);
    }
    return state->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : QSPI_DRV_GetDefaultConfig
 * Description   : Returns default configuration structure for QuadSPI
 * Implements : QSPI_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
status_t QSPI_DRV_GetDefaultConfig(qspi_user_config_t * userConfigPtr)
{
    userConfigPtr->dataRate = QSPI_DATE_RATE_SDR;   /* single data rate */
    userConfigPtr->dmaSupport = false;              /* no DMA support by default */
#if (FEATURE_QSPI_FIXED_DMA_CHANNELS == 0)
    userConfigPtr->dmaChannel = 0U;
#endif
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->readMode = QSPI_READ_MODE_INTERNAL_SAMPLING;
    userConfigPtr->side = QSPI_FLASH_SIDE_A;        /* serial memory connected to side A */
    userConfigPtr->memSize = 0x7FFFFFU;             /* Size: 8 MB */
    userConfigPtr->csHoldTime = 1U;
    userConfigPtr->csSetupTime = 1U;
    userConfigPtr->columnAddr = 0U;
    userConfigPtr->wordAddresable = false;
    userConfigPtr->sampleDelay = QSPI_SAMPLE_DELAY_1;
    userConfigPtr->samplePhase = QSPI_SAMPLE_PHASE_NON_INVERTED;
    userConfigPtr->endianess = QSPI_END_64BIT_LE;       /* little endian */
    userConfigPtr->clock_src = QSPI_CLK_SRC_FIRC_DIV1;  /* use FIRC as clock source */
    userConfigPtr->divider = 1U;
	userConfigPtr->io2IdleValue = 1U;
    userConfigPtr->io3IdleValue = 1U;

    return STATUS_SUCCESS;
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
