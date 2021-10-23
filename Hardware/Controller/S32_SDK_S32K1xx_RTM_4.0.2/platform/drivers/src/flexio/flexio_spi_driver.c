/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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

#include "flexio_spi_driver.h"
#include "flexio_hw_access.h"
#include "flexio_common.h"
#include "clock_manager.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * These are very simple macros used for accessing the hardware resources (shifters and timers) 
 * allocated for each driver instance. They help make the code easy to understand.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type 
 * and a pointer to a different object type.
 * This is needed for callbacks from other modules, which cannot know the actual argument type (flexio_common, dma).
 * Also used in multi-byte transfers for ensuring efficient and endianess-independent data read/write.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for working with the dma. Addresses are configured as uint32_t in the dma driver.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for callbacks from other modules, which cannot know the actual argument type (flexio_common, dma)
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long.
 * This is required for working with the dma. Addresses are configured as uint32_t in the dma driver.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code 
 * structure and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'master' could be declared as pointing to const
 * This is a pointer to the driver context structure which is for internal use only, and the application
 * must make no assumption about the content of this structure. Therefore it is irrelevant for the application
 * whether the structure is changed in the function or not. The fact that in a particular implementation of some 
 * functions there is no write in the context structure is an implementation detail and there is no reason to 
 * propagate it in the interface. That would compromise the stability of the interface, if this implementation 
 * detail is changed in later releases or on other platforms.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

    /* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE 1
#define DIVIDER_MAX_VALUE 0xFF

    /* Shifters/Timers used for SPI simulation. The parameter x represents the 
       resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)     (x)
#define RX_SHIFTER(x)     (uint8_t)((x) + 1U)
#define SCK_TIMER(x)      (x)
#define SS_TIMER(x)       (uint8_t)((x) + 1U)

    /* Dummy data to send when user provides no data */
#define FLEXIO_SPI_DUMMYDATA (0xFFFFFFFFU)

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterComputeBaudRateDivider(uint32_t baudRate, 
                                                        uint16_t *divider,
                                                        uint32_t inputClock)
{
    int32_t tmpDiv;

    /* Compute divider: ((input_clock / baud_rate) / 2) - 1. Round to nearest integer */
    tmpDiv = (((int32_t)inputClock + (int32_t)baudRate) / (2 * (int32_t)baudRate)) - 1;
    /* Enforce upper/lower limits */
    if (tmpDiv < DIVIDER_MIN_VALUE)
    {
        tmpDiv = DIVIDER_MIN_VALUE;
    }
    if (tmpDiv > DIVIDER_MAX_VALUE)
    {
        tmpDiv = DIVIDER_MAX_VALUE;
    }
    
    *divider = (uint16_t)tmpDiv;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterConfigure
 * Description   : configures the FLEXIO module as SPI master
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterConfigure(const flexio_spi_master_state_t *master, 
                                           const flexio_spi_master_user_config_t * userConfigPtr,
                                           uint32_t inputClock)
{
    FLEXIO_Type *baseAddr;
    flexio_pin_polarity_t clockPolarity;        /* Polarity of clock signal */
    flexio_timer_polarity_t txShifterPolarity;  /* Polarity of MOSI shifter */
    flexio_timer_polarity_t rxShifterPolarity;  /* Polarity of MISO shifter */
    uint16_t divider;
    flexio_shifter_stop_t stopBit;
    flexio_shifter_start_t startBit;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;
    /* Compute divider.*/
    FLEXIO_SPI_DRV_MasterComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    /* add number of bits in the upper 8 bits of the divider. Formula is: TIMCMP[15:8] = (number of bits x 2) - 1 */
    divider += (uint16_t)((((uint16_t)(userConfigPtr->transferSize) * 8U * 2U) - 1U) << 8U);

    if (userConfigPtr->clockPolarity == 0U)
    {
        /* CPOL = 0 */
        clockPolarity = FLEXIO_PIN_POLARITY_HIGH;
    }
    else
    {
        /* CPOL = 1 */
        clockPolarity = FLEXIO_PIN_POLARITY_LOW;
    }

    if (userConfigPtr->clockPhase == 0U)
    {
        /* CPHA = 0 */
        txShifterPolarity = FLEXIO_TIMER_POLARITY_NEGEDGE;
        rxShifterPolarity = FLEXIO_TIMER_POLARITY_POSEDGE;
        stopBit = FLEXIO_SHIFTER_STOP_BIT_DISABLED;
        startBit = FLEXIO_SHIFTER_START_BIT_DISABLED;
    }
    else
    {
        /* CPHA = 1 */
        txShifterPolarity = FLEXIO_TIMER_POLARITY_POSEDGE;
        rxShifterPolarity = FLEXIO_TIMER_POLARITY_NEGEDGE;
        stopBit = FLEXIO_SHIFTER_STOP_BIT_0;
        startBit = FLEXIO_SHIFTER_START_BIT_DISABLED_SH;
    }

    /* Configure Tx shifter (MOSI) */
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->mosiPin,
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_OUTPUT,
                                 SCK_TIMER(resourceIndex),
                                 txShifterPolarity);
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                startBit,
                                stopBit,
                                FLEXIO_SHIFTER_SOURCE_PIN);

    /* Configure Rx shifter (MISO) */
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->misoPin,
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 SCK_TIMER(resourceIndex),
                                 rxShifterPolarity);
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);

    /* Configure sck timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), divider); /* set baud rate, and number of bits */
    FLEXIO_SetTimerConfig(baseAddr,
                              SCK_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_TIM_DIS,
                              FLEXIO_TIMER_ENABLE_TRG_HIGH,
                              FLEXIO_TIMER_DISABLE_TIM_CMP,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR,
                              FLEXIO_TIMER_INITOUT_ZERO);
    FLEXIO_SetTimerControl(baseAddr,
                               SCK_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U), /* trigger on tx shifter status flag */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->sckPin,                     /* output on clock pin */
                               clockPolarity,                      /* used configured polarity */
                               FLEXIO_PIN_CONFIG_OUTPUT,
                               FLEXIO_TIMER_MODE_DISABLED);

    /* Configure SS timer */
    FLEXIO_SetTimerCompare(baseAddr, SS_TIMER(resourceIndex), 0xFFFFU);
    FLEXIO_SetTimerConfig(baseAddr,
                              SS_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_TIM_ENABLE,    /* enable when SCK timer is enabled */
                              FLEXIO_TIMER_DISABLE_TIM_DISABLE,  /* disable when SCK timer is disabled */
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR,
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               SS_TIMER(resourceIndex),
                               0U,                 /* trigger not used, using defaults */
                               FLEXIO_TRIGGER_POLARITY_HIGH,
                               FLEXIO_TRIGGER_SOURCE_EXTERNAL,
                               userConfigPtr->ssPin,
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_OUTPUT,
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterEndTransfer
 * Description   : end a transfer
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterEndTransfer(flexio_spi_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Disable transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Disable interrupts for Rx and Tx shifters */
            FLEXIO_SetShifterInterrupt(baseAddr, 
                                (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                false);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, 
                                (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                false);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Stop DMA channels */
            (void)EDMA_DRV_StopChannel(master->txDMAChannel);
            (void)EDMA_DRV_StopChannel(master->rxDMAChannel);
            /* Disable FlexIO DMA requests for both shifters */
            FLEXIO_SetShifterDMARequest(baseAddr, 
                     (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                     false);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    master->driverIdle = true;
    master->txRemainingBytes = 0U;
    master->rxRemainingBytes = 0U;

    /* Signal transfer end for blocking transfers */
    if (master->blocking == true)
    {
        (void)OSIF_SemaPost(&(master->idleSemaphore));
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterEnableTransfer(flexio_spi_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[master->flexioCommon.instance];

    /* enable timers and shifters */
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_TRANSMIT);
    FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_RECEIVE);
    if (master->master)
    {
        FLEXIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), FLEXIO_TIMER_MODE_8BIT_BAUD);
        FLEXIO_SetTimerMode(baseAddr, SS_TIMER(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
    }
    else
    {
        FLEXIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterStopTransfer
 * Description   : Forcefully stops the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterStopTransfer(flexio_spi_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[master->flexioCommon.instance];

    /* disable and re-enable timers and shifters to reset them */
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    FLEXIO_SetTimerMode(baseAddr, SS_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);

    /* clear any leftover error flags */
    FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

    /* end the transfer */
    FLEXIO_SPI_DRV_MasterEndTransfer(master);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t FLEXIO_SPI_DRV_MasterWaitTransferEnd(flexio_spi_master_state_t *master, uint32_t timeout)
{
    status_t osifError = STATUS_SUCCESS;

    DEV_ASSERT(master != NULL);

    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Wait for transfer to be completed by the IRQ */
            osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_SPI_DRV_MasterGetStatus() to do the transfer */
            while (FLEXIO_SPI_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY)
            {
            }
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    /* blocking transfer is over */
    master->blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* abort current transfer */
        master->status = STATUS_TIMEOUT;
        FLEXIO_SPI_DRV_MasterStopTransfer(master);
    }

    return master->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterEndDmaTransfer
 * Description   : end a DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterEndDmaTransfer(void *stateStruct, edma_chn_status_t status)
{
    flexio_spi_master_state_t *master;

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_spi_master_state_t *)stateStruct;

    /* Record success if there was no error */
    if (status == EDMA_CHN_ERROR)
    {
        master->status = STATUS_ERROR;
    }
    else
    {
        master->status = STATUS_SUCCESS;
    }
    FLEXIO_SPI_DRV_MasterStopTransfer(master);
    /* Call callback to announce the event to the user */
    if (master->callback != NULL)
    {
        master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_SlaveConfigure
 * Description   : configures the FLEXIO module as SPI slave
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_SlaveConfigure(const flexio_spi_slave_state_t *slave,
                                          const flexio_spi_slave_user_config_t * userConfigPtr)
{
    FLEXIO_Type *baseAddr;
    flexio_pin_polarity_t clockPolarity;        /* Polarity of clock signal */
    flexio_timer_polarity_t txShifterPolarity;  /* Polarity of MISO shifter */
    flexio_timer_polarity_t rxShifterPolarity;  /* Polarity of MOSI shifter */
    flexio_shifter_start_t startBit;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[slave->flexioCommon.instance];
    resourceIndex = slave->flexioCommon.resourceIndex;

    if (userConfigPtr->clockPolarity == 0U)
    {
        /* CPOL = 0 */
        clockPolarity = FLEXIO_PIN_POLARITY_HIGH;
    }
    else
    {
        /* CPOL = 1 */
        clockPolarity = FLEXIO_PIN_POLARITY_LOW;
    }

    if (userConfigPtr->clockPhase == 0U)
    {
        /* CPHA = 0 */
        txShifterPolarity = FLEXIO_TIMER_POLARITY_NEGEDGE;
        rxShifterPolarity = FLEXIO_TIMER_POLARITY_POSEDGE;
        startBit = FLEXIO_SHIFTER_START_BIT_DISABLED;
    }
    else
    {
        /* CPHA = 1 */
        txShifterPolarity = FLEXIO_TIMER_POLARITY_POSEDGE;
        rxShifterPolarity = FLEXIO_TIMER_POLARITY_NEGEDGE;
        startBit = FLEXIO_SHIFTER_START_BIT_DISABLED_SH;
    }

    /* Configure Slave Tx shifter (MISO) */
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->misoPin,
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_OUTPUT,
                                 SCK_TIMER(resourceIndex),
                                 txShifterPolarity);
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                startBit,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);

    /* Configure Slave Rx shifter (MOSI) */
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->mosiPin,
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 SCK_TIMER(resourceIndex),
                                 rxShifterPolarity);
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);

    /* Configure sck timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), 0xFFFFU);
    FLEXIO_SetTimerConfig(baseAddr,
                              SCK_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_TRG_POSEDGE,        /* enable on SS pin rising edge */
                              FLEXIO_TIMER_DISABLE_TRG,               /* disable on SS pin falling edge */
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_PIN_SHIFT_PIN,   /* decrement on input pin - SCK */
                              FLEXIO_TIMER_INITOUT_ZERO);
    FLEXIO_SetTimerControl(baseAddr,
                               SCK_TIMER(resourceIndex),
                               (uint8_t)(userConfigPtr->ssPin << 1U), /* trigger on SS pin edge */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->sckPin,                 /* use SCK pin as input pin */
                               clockPolarity,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_ReadData
 * Description   : reads data received by the module
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_ReadData(flexio_spi_master_state_t *master)
{
    const FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Read data from shifter buffer */
    if (master->bitOrder == FLEXIO_SPI_TRANSFER_LSB_FIRST)
    {
        /* For data size < 4 bytes our data is in the upper part of the buffer and must be shifted */
        data = FLEXIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_RW_MODE_NORMAL);
        data >>= (32U - (8U * (uint32_t)(master->transferSize)));
    }
    else
    {
        data = FLEXIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);
    }

    if ((master->rxRemainingBytes > 0U) && (master->rxData != NULL))
    {
        switch (master->transferSize)
        {
            case FLEXIO_SPI_TRANSFER_1BYTE:
                *(uint8_t *)master->rxData = (uint8_t)data;
                break;
            case FLEXIO_SPI_TRANSFER_2BYTE:
                *(uint16_t *)master->rxData = (uint16_t)data;
                break;
            case FLEXIO_SPI_TRANSFER_4BYTE:
                *(uint32_t *)master->rxData = (uint32_t)data;
                break;
            default:
                /* Not possible */
                break;
        }
        /* Update rx buffer pointer and remaining bytes count */
        master->rxData = &master->rxData[(master->transferSize)];
        master->rxRemainingBytes -= (uint32_t)(master->transferSize);
    }
    else
    {
        /* No data to receive, just ignore the read data */
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_WriteData
 * Description   : writes data to be transmitted by the module
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_WriteData(flexio_spi_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint32_t data = FLEXIO_SPI_DUMMYDATA;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    if (master->txRemainingBytes == 0U)
    {
        /* Done transmitting */
        return;
    }

    if ((master->txRemainingBytes > 0U) && (master->txData != NULL))
    {
        /* Read data from user buffer */
        switch (master->transferSize)
        {
            case FLEXIO_SPI_TRANSFER_1BYTE:
                data = (uint32_t)(*(const uint8_t *)master->txData);
                break;
            case FLEXIO_SPI_TRANSFER_2BYTE:
                data = (uint32_t)(*(const uint16_t *)master->txData);
                break;
            case FLEXIO_SPI_TRANSFER_4BYTE:
                data = (uint32_t)(*(const uint32_t *)master->txData);
                break;
            default:
                /* Not possible */
                break;
        }
        /* Update tx buffer pointer and remaining bytes count */
        master->txData = &master->txData[master->transferSize];
        master->txRemainingBytes -= (uint32_t)(master->transferSize);
        /* Write data to shifter buffer */
        if (master->bitOrder == FLEXIO_SPI_TRANSFER_LSB_FIRST)
        {
            FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, FLEXIO_SHIFTER_RW_MODE_NORMAL);
        }
        else
        {
            /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
            data <<= 32U - (8U * (uint32_t)(master->transferSize));
            FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);
        }
    }
    else
    {
        /* Nothing to send, write dummy data in buffer */
        FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SPI_DUMMYDATA, FLEXIO_SHIFTER_RW_MODE_NORMAL);
    }
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterCheckStatus
 * Description   : Check status of SPI master transfer. This function can be 
 *                 called either in an interrupt routine or directly in polling 
 *                 mode to advance the SPI transfer.
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterCheckStatus(void *stateStruct)
{
    flexio_spi_master_state_t *master;
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_spi_master_state_t *)stateStruct;
    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check for errors */
    if (FLEXIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex)))
    {
        master->status = STATUS_SPI_TX_UNDERRUN;
        /* Force the transfer to stop */
        FLEXIO_SPI_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    if (FLEXIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        master->status = STATUS_SPI_RX_OVERRUN;
        /* Force the transfer to stop */
        FLEXIO_SPI_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check if data was received */
    if (FLEXIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        FLEXIO_SPI_DRV_ReadData(master);
    }
    /* Check if transmitter needs more data */
    if (FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)))
    {
        FLEXIO_SPI_DRV_WriteData(master);
        if (master->txRemainingBytes == 0U)
        {
            /* No more data to transmit, disable tx interrupts */
            FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        }
    }

    /* Check there is any data left */
    if ((master->txRemainingBytes == 0U) && (master->rxRemainingBytes == 0U))
    {
        /* Record success if there was no error */
        if (master->status == STATUS_BUSY)
        {
            master->status = STATUS_SUCCESS;
        }
        /* End transfer */
        FLEXIO_SPI_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for DMA tx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_SPI_DRV_MasterComputeTxRegAddr(const flexio_spi_master_state_t *master)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    shifter = TX_SHIFTER(master->flexioCommon.resourceIndex);
    if (master->bitOrder == FLEXIO_SPI_TRANSFER_LSB_FIRST)
    {
        addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter]));
    }
    else
    {
        addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - (uint32_t)master->transferSize);
    }
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for DMA rx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_SPI_DRV_MasterComputeRxRegAddr(const flexio_spi_master_state_t *master)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    shifter = RX_SHIFTER(master->flexioCommon.resourceIndex);
    if (master->bitOrder == FLEXIO_SPI_TRANSFER_LSB_FIRST)
    {
        addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter])) + (sizeof(uint32_t) - (uint32_t)master->transferSize);
    }
    else
    {
        addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter]));
    }
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterStartDmaTransfer
 * Description   : Starts a DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_SPI_DRV_MasterStartDmaTransfer(flexio_spi_master_state_t *master)
{
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    FLEXIO_Type *baseAddr;
    uint32_t addr;
    /* Table to map flexio_spi transfer sizes to EDMA transfer sizes */
    static const edma_transfer_size_t dmaTransferSize[FLEXIO_SPI_TRANSFER_4BYTE] = 
            {EDMA_TRANSFER_SIZE_1B, EDMA_TRANSFER_SIZE_2B, EDMA_TRANSFER_SIZE_4B, EDMA_TRANSFER_SIZE_4B};


    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    if (master->txData != NULL)
    {
        addr = (uint32_t)(master->txData);
    }
    else
    {
        /* if there is no data to transmit, use dummy data as source for DMA transfer */
        master->dummyDmaData = FLEXIO_SPI_DUMMYDATA;
        addr = (uint32_t)(&(master->dummyDmaData));
    }

    /* Configure the transfer control descriptor for the Tx channel */
    (void)EDMA_DRV_ConfigMultiBlockTransfer(master->txDMAChannel, 
                                            EDMA_TRANSFER_MEM2PERIPH, 
                                            addr,
                                            FLEXIO_SPI_DRV_MasterComputeTxRegAddr(master), 
                                            dmaTransferSize[(uint32_t)master->transferSize - 1U], 
                                            (uint32_t)master->transferSize,
                                            master->txRemainingBytes / (uint32_t)master->transferSize,
                                            true);
    if (master->txData == NULL)
    {
        /* if there is no data to transmit, don't increment source offset */
        EDMA_DRV_SetSrcOffset(master->txDMAChannel, 0);
    }

    if (master->rxData != NULL)
    {
        addr = (uint32_t)(master->rxData);
    }
    else
    {
        /* if there is no data to receive, use dummy data as destination for DMA transfer */
        addr = (uint32_t)(&(master->dummyDmaData));
    }

    /* Configure the transfer control descriptor for the Rx channel */
    (void)EDMA_DRV_ConfigMultiBlockTransfer(master->rxDMAChannel, 
                                            EDMA_TRANSFER_PERIPH2MEM, 
                                            FLEXIO_SPI_DRV_MasterComputeRxRegAddr(master), 
                                            addr,
                                            dmaTransferSize[(uint32_t)master->transferSize - 1U], 
                                            (uint32_t)master->transferSize,
                                            master->rxRemainingBytes / (uint32_t)master->transferSize,
                                            true);
    if (master->rxData == NULL)
    {
        /* if there is no data to receive, don't increment destination offset */
        EDMA_DRV_SetDestOffset(master->rxDMAChannel, 0);
    }
    /* Setup callback for DMA transfer end */
    (void)EDMA_DRV_InstallCallback(master->rxDMAChannel,
                                   (edma_callback_t)(FLEXIO_SPI_DRV_MasterEndDmaTransfer),
                                   (void*)(master));

    /* Start both DMA channels */
    (void)EDMA_DRV_StartChannel(master->txDMAChannel);
    (void)EDMA_DRV_StartChannel(master->rxDMAChannel);

    /* Enable FlexIO DMA requests for both shifters */
    FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
}


/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterInit
 * Description   : Initialize the FLEXIO_SPI master mode driver
 *
 * Implements : FLEXIO_SPI_DRV_MasterInit_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterInit(uint32_t instance,
                                          const flexio_spi_master_user_config_t * userConfigPtr,
                                          flexio_spi_master_state_t * master)
{
    status_t retCode;
    uint32_t inputClock;
    status_t clkErr;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(instance < FLEXIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    DEV_ASSERT(g_flexioDeviceStatePtr[instance] != NULL);
    DEV_ASSERT((userConfigPtr->transferSize == FLEXIO_SPI_TRANSFER_1BYTE) ||
               (userConfigPtr->transferSize == FLEXIO_SPI_TRANSFER_2BYTE) ||
               (userConfigPtr->transferSize == FLEXIO_SPI_TRANSFER_4BYTE));

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need two shifters/timers */
    master->flexioCommon.resourceCount = 2U;
    /* Common FlexIO driver initialization */
    retCode = FLEXIO_DRV_InitDriver(instance, &(master->flexioCommon));
    if (retCode != STATUS_SUCCESS)
    {   /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        osifError = OSIF_SemaCreate(&(master->idleSemaphore), 0U);
        DEV_ASSERT(osifError == STATUS_SUCCESS);
    }

    /* Initialize driver-specific context structure */
    master->driverType = userConfigPtr->driverType;
    master->bitOrder = userConfigPtr->bitOrder;
    master->transferSize = userConfigPtr->transferSize;
    master->callback = userConfigPtr->callback;
    master->callbackParam = userConfigPtr->callbackParam;
    master->blocking = false;
    master->driverIdle = true;
    master->master = true;
    master->status = STATUS_SUCCESS;

    /* Configure device for SPI mode */
    FLEXIO_SPI_DRV_MasterConfigure(master, userConfigPtr, inputClock);

    /* Set up transfer engine */
    switch (userConfigPtr->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            master->flexioCommon.isr = FLEXIO_SPI_DRV_MasterCheckStatus;
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_SPI_DRV_MasterGetStatus() will handle the transfer */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Store DMA channel numbers */
            master->rxDMAChannel = userConfigPtr->rxDMAChannel;
            master->txDMAChannel = userConfigPtr->txDMAChannel;
            /* Configure DMA request sources */
            dmaReqTx = g_flexioDMASrc[instance][TX_SHIFTER(master->flexioCommon.resourceIndex)];
            dmaReqRx = g_flexioDMASrc[instance][RX_SHIFTER(master->flexioCommon.resourceIndex)];
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    (void)clkErr;
    (void)osifError;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterDeinit
 * Description   : De-initialize the FLEXIO_SPI master mode driver
 *
 * Implements : FLEXIO_SPI_DRV_MasterDeinit_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterDeinit(flexio_spi_master_state_t *master)
{
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (master->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        (void)OSIF_SemaDestroy(&(master->idleSemaphore));
    }

    return FLEXIO_DRV_DeinitDriver(&(master->flexioCommon));
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterSetBaudRate
 * Description   : Set the baud rate for any subsequent SPI communication
 *
 * Implements : FLEXIO_SPI_DRV_MasterSetBaudRate_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterSetBaudRate(flexio_spi_master_state_t *master, uint32_t baudRate)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint16_t timerCmp;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }
    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[master->flexioCommon.instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Compute divider */
    FLEXIO_SPI_DRV_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure timer divider in the lower 8 bits of TIMCMP[CMP] */
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCK_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0xFF00U) | divider);
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), timerCmp);

    (void)clkErr;
    return STATUS_SUCCESS;
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 *
 * Implements : FLEXIO_SPI_DRV_MasterGetBaudRate_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterGetBaudRate(flexio_spi_master_state_t *master, uint32_t *baudRate)
{
    const FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint16_t timerCmp;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baudRate != NULL);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[master->flexioCommon.instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Get the currently configured divider */
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCK_TIMER(resourceIndex));
    divider = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 1)). Round to nearest integer */
    *baudRate = (inputClock + divider + 1U) / (2U * ((uint32_t)divider + 1U));
    
    (void)clkErr;
    return STATUS_SUCCESS;
}




/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterTransfer
 * Description   : Perform an SPI master non-blocking transaction
 *
 * Implements : FLEXIO_SPI_DRV_MasterTransfer_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterTransfer(flexio_spi_master_state_t *master,
                                              const uint8_t *txData,
                                              uint8_t *rxData,
                                              uint32_t dataSize)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);
    DEV_ASSERT((dataSize % (uint32_t)(master->transferSize)) == 0U);
    DEV_ASSERT((txData != NULL) || (rxData != NULL));

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }
    /* Initialize transfer data */
    master->txData = txData;
    master->rxData = rxData;
    master->txRemainingBytes = dataSize;
    master->rxRemainingBytes = dataSize;
    master->driverIdle = false;
    master->status = STATUS_BUSY;

    /* Enable timers and shifters */
    FLEXIO_SPI_DRV_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Enable interrupts for Rx and Tx shifters */
            FLEXIO_SetShifterInterrupt(baseAddr, 
                                 (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                 true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, 
                                 (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                 true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_SPI_DRV_MasterCheckStatus once to send the first byte */
            FLEXIO_SPI_DRV_MasterCheckStatus(master);
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_SPI_DRV_MasterStartDmaTransfer(master);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterTransferBlocking
 * Description   : Perform an SPI master blocking transaction
 *
 * Implements : FLEXIO_SPI_DRV_MasterTransferBlocking_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterTransferBlocking(flexio_spi_master_state_t *master,
                                                      const uint8_t *txData,
                                                      uint8_t *rxData,
                                                      uint32_t dataSize,
                                                      uint32_t timeout)
{
    status_t status;

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    if (master->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OSIF_SemaWait(&(master->idleSemaphore), 0);
    }
    status = FLEXIO_SPI_DRV_MasterTransfer(master, txData, rxData, dataSize);
    if (status != STATUS_SUCCESS)
    {
        /* Transfer could not be started */
        master->blocking = false;
        return status; 
    }

    /* Wait for transfer to end */
    return FLEXIO_SPI_DRV_MasterWaitTransferEnd(master, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking SPI master transaction
 *
 * Implements : FLEXIO_SPI_DRV_MasterTransferAbort_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterTransferAbort(flexio_spi_master_state_t *master)
{
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (master->driverIdle)
    {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_SPI_ABORTED;
    FLEXIO_SPI_DRV_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking SPI master transaction
 *
 * Implements : FLEXIO_SPI_DRV_MasterGetStatus_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_MasterGetStatus(flexio_spi_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    DEV_ASSERT(master != NULL);

    /* Initialize with the actual remaining bytes count */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle)
    {
        switch(master->driverType)
        {
            case FLEXIO_DRIVER_TYPE_POLLING:
                /* In polling mode advance the SPI transfer here */
                FLEXIO_SPI_DRV_MasterCheckStatus(master);
                break;

            case FLEXIO_DRIVER_TYPE_DMA:
                /* In DMA mode just update the remaining count.
                DO NOT write master->txRemainingBytes directly !!! */
                remainingBytes = EDMA_DRV_GetRemainingMajorIterationsCount(master->rxDMAChannel);
                break;

            default:
                /* Nothing */
                break;
        }

    }

    if (bytesRemaining != NULL)
    {
        *bytesRemaining = remainingBytes;
    }

    return master->status;
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_SlaveInit
 * Description   : Initialize the FLEXIO_SPI slave mode driver
 *
 * Implements : FLEXIO_SPI_DRV_SlaveInit_Activity
 *END**************************************************************************/
status_t FLEXIO_SPI_DRV_SlaveInit(uint32_t instance,
                                         const flexio_spi_slave_user_config_t * userConfigPtr,
                                         flexio_spi_slave_state_t * slave)
{
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    DEV_ASSERT(slave != NULL);
    DEV_ASSERT(instance < FLEXIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    DEV_ASSERT(g_flexioDeviceStatePtr[instance] != NULL);

    /* Instruct the resource allocator that we need two shifters/timers */
    slave->flexioCommon.resourceCount = 2U;
    /* Common FlexIO driver initialization */
    retCode = FLEXIO_DRV_InitDriver(instance, &(slave->flexioCommon));
    if (retCode != STATUS_SUCCESS)
    {   /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        osifError = OSIF_SemaCreate(&(slave->idleSemaphore), 0U);
        DEV_ASSERT(osifError == STATUS_SUCCESS);
    }

    /* Initialize driver context structure */
    slave->driverType = userConfigPtr->driverType;
    slave->bitOrder = userConfigPtr->bitOrder;
    slave->transferSize = userConfigPtr->transferSize;
    slave->callback = userConfigPtr->callback;
    slave->callbackParam = userConfigPtr->callbackParam;
    slave->blocking = false;
    slave->driverIdle = true;
    slave->master = false;
    slave->status = STATUS_SUCCESS;

    /* Configure device for SPI mode */
    FLEXIO_SPI_DRV_SlaveConfigure(slave, userConfigPtr);

    /* Set up transfer engine */
    switch (userConfigPtr->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            slave->flexioCommon.isr = FLEXIO_SPI_DRV_MasterCheckStatus;
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_SPI_DRV_MasterGetStatus() will handle the transfer */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Store DMA channel numbers */
            slave->rxDMAChannel = userConfigPtr->rxDMAChannel;
            slave->txDMAChannel = userConfigPtr->txDMAChannel;
            /* Configure DMA request sources */
            dmaReqTx = g_flexioDMASrc[instance][TX_SHIFTER(slave->flexioCommon.resourceIndex)];
            dmaReqRx = g_flexioDMASrc[instance][RX_SHIFTER(slave->flexioCommon.resourceIndex)];
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    (void)osifError;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_MasterGetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_SPI master
 * Implements : FLEXIO_SPI_DRV_MasterGetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_SPI_DRV_MasterGetDefaultConfig(flexio_spi_master_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->baudRate = 100000U;
    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->bitOrder = FLEXIO_SPI_TRANSFER_MSB_FIRST;
    userConfigPtr->transferSize = FLEXIO_SPI_TRANSFER_1BYTE;
    userConfigPtr->clockPolarity = 0U;
    userConfigPtr->clockPhase = 0U;
    userConfigPtr->mosiPin = 0U;
    userConfigPtr->misoPin = 1U;
    userConfigPtr->sckPin = 2U;
    userConfigPtr->ssPin = 3U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel = 255U;
    userConfigPtr->txDMAChannel = 255U;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_SPI_DRV_SlaveGetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_SPI slave
 * Implements : FLEXIO_SPI_DRV_SlaveGetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_SPI_DRV_SlaveGetDefaultConfig(flexio_spi_slave_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->bitOrder = FLEXIO_SPI_TRANSFER_MSB_FIRST;
    userConfigPtr->transferSize = FLEXIO_SPI_TRANSFER_1BYTE;
    userConfigPtr->clockPolarity = 0U;
    userConfigPtr->clockPhase = 0U;
    userConfigPtr->mosiPin = 0U;
    userConfigPtr->misoPin = 1U;
    userConfigPtr->sckPin = 2U;
    userConfigPtr->ssPin = 3U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel = 255U;
    userConfigPtr->txDMAChannel = 255U;
}


/*!
 * @brief De-initialize the FLEXIO_SPI slave mode driver
 *
 * This function de-initializes the FLEXIO_SPI driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @return    Error or success status returned by API
 * Implements : FLEXIO_SPI_DRV_SlaveDeinit_Activity
 */
status_t FLEXIO_SPI_DRV_SlaveDeinit(flexio_spi_slave_state_t *slave)
{
    return FLEXIO_SPI_DRV_MasterDeinit(slave);
}


/*!
 * @brief Perform a non-blocking SPI slave transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer 
 * and then returns, leaving the transfer to complete asynchronously).
 * FLEXIO_SPI_DRV_SlaveGetStatus() can be called to check the status of the transfer.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_SPI_DRV_SlaveTransfer_Activity
 */
status_t FLEXIO_SPI_DRV_SlaveTransfer(flexio_spi_slave_state_t *slave,
                                                    const uint8_t *txData,
                                                    uint8_t *rxData,
                                                    uint32_t dataSize)
{
    return FLEXIO_SPI_DRV_MasterTransfer(slave, txData, rxData, dataSize);
}


/*!
 * @brief Perform a blocking SPI slave transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is blocking, the function only returns when the transfer 
 * is complete.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 * Implements : FLEXIO_SPI_DRV_SlaveTransferBlocking_Activity
 */
status_t FLEXIO_SPI_DRV_SlaveTransferBlocking(flexio_spi_slave_state_t *slave,
                                                            const uint8_t *txData,
                                                            uint8_t *rxData,
                                                            uint32_t dataSize,
                                                            uint32_t timeout)
{
    return FLEXIO_SPI_DRV_MasterTransferBlocking(slave, txData, rxData, dataSize, timeout);
}


/*!
 * @brief Aborts a non-blocking SPI slave transaction
 *
 * This function aborts a non-blocking SPI transfer.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @return    Error or success status returned by API
 * Implements : FLEXIO_SPI_DRV_SlaveTransferAbort_Activity
 */
status_t FLEXIO_SPI_DRV_SlaveTransferAbort(flexio_spi_slave_state_t *slave)
{
    return FLEXIO_SPI_DRV_MasterTransferAbort(slave);
}


/*!
 * @brief Get the status of the current non-blocking SPI slave transaction
 *
 * This function returns the current status of a non-blocking SPI slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_SPI_DRV_SlaveGetStatus_Activity
 */
status_t FLEXIO_SPI_DRV_SlaveGetStatus(flexio_spi_slave_state_t *slave, uint32_t *bytesRemaining)
{
    return FLEXIO_SPI_DRV_MasterGetStatus(slave, bytesRemaining);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
