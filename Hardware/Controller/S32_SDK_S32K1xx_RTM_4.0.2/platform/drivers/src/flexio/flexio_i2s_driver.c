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

#include "flexio_i2s_driver.h"
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
 * Violates MISRA 2012 Required Rule 5.1, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
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
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
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
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code
 * structure and better readability.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

    /* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE  1
#define DIVIDER_MAX_VALUE  0xFF

    /* Shifters/Timers used for I2S simulation The parameter x represents the
       resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)     (x)
#define RX_SHIFTER(x)     (uint8_t)((x) + 1U)
#define SCK_TIMER(x)      (x)
#define WS_TIMER(x)       (uint8_t)((x) + 1U)
#define WS_TIMER_RISING_BLK(x)       (uint8_t)((x) + 2U)

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_ComputeByteWidth
 * Description   : Computes the baud rate divider for a target baud rate
 *
 *END**************************************************************************/
static uint8_t FLEXIO_I2S_DRV_ComputeByteWidth(uint8_t bitsWidth)
{
    uint8_t byteWidth;

    if (bitsWidth <= 8U)
    {
        byteWidth = 1U;
    }
    else if (bitsWidth <= 16U)
    {
        byteWidth = 2U;
    }
    else
    {
        byteWidth = 4U;
    }

    return byteWidth;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterComputeBaudRateDivider(uint32_t baudRate,
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
 * Function Name : FLEXIO_I2S_DRV_MasterConfigure
 * Description   : configures the FLEXIO module as I2S master
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterConfigure(flexio_i2s_master_state_t *master,
                                           const flexio_i2s_master_user_config_t * userConfigPtr,
                                           uint32_t inputClock)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Compute divider. */
    FLEXIO_I2S_DRV_MasterComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitsWidth;

    /* Configure tx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED_SH,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->txPin,             /* output on tx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,       /* disable for now, will enable on transmit */
                                 SCK_TIMER(resourceIndex),         /* use clock timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->rxPin,                    /* output to rx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 SCK_TIMER(resourceIndex),     /* use control timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCK timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((uint16_t)(((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    FLEXIO_SetTimerConfig(baseAddr,
                              SCK_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_TRG_HIGH,         /* enable when Tx data is available */
                              FLEXIO_TIMER_DISABLE_NEVER,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR, /* decrement on FlexIO clock */
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               SCK_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),  /* trigger on tx shifter status flag */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->sckPin,                 /* output on SCK pin */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_OUTPUT,              /* enable output */
                               FLEXIO_TIMER_MODE_DISABLED);

    /* Configure WS timer */

    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((bits * ((divider + 1U) * 2U)) - 1U));
    FLEXIO_SetTimerConfig(baseAddr,
                              WS_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_TIM_ENABLE,       /* enable when SCK timer is enabled */
                              FLEXIO_TIMER_DISABLE_NEVER,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR, /* decrement on FlexIO clock */
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               WS_TIMER(resourceIndex),
                               0U,                                  /* trigger not used */
                               FLEXIO_TRIGGER_POLARITY_HIGH,
                               FLEXIO_TRIGGER_SOURCE_EXTERNAL,
                               userConfigPtr->wsPin,                /* output on WS pin */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_OUTPUT,            /* enable output */
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_SlaveConfigure
 * Description   : configures the FLEXIO module as I2S slave
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_SlaveConfigure(const flexio_i2s_slave_state_t *slave,
                                          const flexio_i2s_slave_user_config_t * userConfigPtr)
{
    FLEXIO_Type *baseAddr;
    uint16_t bits;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[slave->flexioCommon.instance];
    resourceIndex = slave->flexioCommon.resourceIndex;
    bits = userConfigPtr->bitsWidth;


    /* Configure tx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->txPin,             /* output on tx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,       /* disable for now, will enable on transmit */
                                 WS_TIMER(resourceIndex),          /* use clock timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_DISABLED,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->rxPin,                    /* output to rx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 WS_TIMER(resourceIndex),     /* use control timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCK timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((uint16_t)(bits << 2U) - 3U));
    FLEXIO_SetTimerConfig(baseAddr,
                              SCK_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_PIN_POSEDGE,      /* enable on WS positive edge */
                              FLEXIO_TIMER_DISABLE_TIM_CMP,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_TRG_SHIFT_TRG, /* decrement on SCK input  */
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               SCK_TIMER(resourceIndex),
                               (uint8_t)(userConfigPtr->sckPin << 1U), /* trigger on SCK pin */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->wsPin,                 /* use WS input pin */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);

    /* Configure WS_TIMER_RISING_BLK timer */
    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), (uint16_t)((uint16_t)(bits << 2U) - 4U));
    FLEXIO_SetTimerConfig(baseAddr,
                              WS_TIMER_RISING_BLK(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH,
                              FLEXIO_TIMER_DISABLE_TRG,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_PIN_SHIFT_PIN,
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               WS_TIMER_RISING_BLK(resourceIndex),
                               (uint8_t)((uint8_t)(SCK_TIMER(resourceIndex) << 2U) + 3U), /* SCK timer trigger output */
                               FLEXIO_TRIGGER_POLARITY_HIGH,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->sckPin,               /* SCK input pin */
                               FLEXIO_PIN_POLARITY_HIGH,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);

    /* Configure WS timer */
    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((uint16_t)(bits << 1U) - 1U));
    FLEXIO_SetTimerConfig(baseAddr,
                              WS_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_DISABLED,
                              FLEXIO_TIMER_STOP_BIT_DISABLED,
                              FLEXIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH, /* enable at first falling edge after first rising edge of bclk */
                              FLEXIO_TIMER_DISABLE_TIM_CMP_TRG_LOW,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_PIN_SHIFT_PIN,
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               WS_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(WS_TIMER_RISING_BLK(resourceIndex) << 2U) + 3U), /* WS_TIMER_RISING_BLK timer trigger output */
                               FLEXIO_TRIGGER_POLARITY_HIGH,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               userConfigPtr->sckPin,               /* SCK input pin */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_DISABLED,          /* enable output */
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterEndTransfer
 * Description   : End the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterEndTransfer(flexio_i2s_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

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
            /* Disable FlexIO Tx and Rx DMA requests */
            FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    master->driverIdle = true;
    master->txData = NULL;
    master->rxData = NULL;
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
 * Function Name : FLEXIO_I2S_DRV_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterEnableTransfer(flexio_i2s_master_state_t *master)
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
    }
    else
    {
        FLEXIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
        FLEXIO_SetTimerMode(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
    }
    FLEXIO_SetTimerMode(baseAddr, WS_TIMER(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterStopTransfer
 * Description   : Stops the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterStopTransfer(flexio_i2s_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[master->flexioCommon.instance];

    /* disable timers and shifters */
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    if (!master->master)
    {
        FLEXIO_SetTimerMode(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    }
    FLEXIO_SetTimerMode(baseAddr, WS_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    /* Disable pin output */
    FLEXIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_PIN_CONFIG_DISABLED);

    /* clear any leftover error flags */
    FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

    /* end the transfer */
    FLEXIO_I2S_DRV_MasterEndTransfer(master);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t FLEXIO_I2S_DRV_MasterWaitTransferEnd(flexio_i2s_master_state_t *master, uint32_t timeout)
{
    status_t osifError = STATUS_SUCCESS;

    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Wait for transfer to be completed by the IRQ */
            osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_I2S_DRV_MasterGetStatus() to do the transfer */
            while (FLEXIO_I2S_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY) {}
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
        FLEXIO_I2S_DRV_MasterStopTransfer(master);
    }

    return master->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_ReadData
 * Description   : reads data received by the module
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_ReadData(flexio_i2s_master_state_t *master)
{
    const FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Read data from shifter buffer */
    data = FLEXIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);

    if (master->rxRemainingBytes > 0U)
    {
        if (master->rxData != NULL)
        {
            switch (master->byteWidth)
            {
            case 1U:
                *(uint8_t *)master->rxData = (uint8_t)data;
                break;
            case 2U:
                *(uint16_t *)master->rxData = (uint16_t)data;
                break;
            default:
                *(uint32_t *)master->rxData = (uint32_t)data;
                break;
            }
            /* Update rx buffer pointer */
            master->rxData = &master->rxData[master->byteWidth];
        }
        /* Update remaining bytes count even if buffer is null */
        master->rxRemainingBytes -= (uint32_t)(master->byteWidth);
    }
    else
    {
        /* No data to receive, just ignore the read data */
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_WriteData
 * Description   : writes data to be transmitted by the module
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_WriteData(flexio_i2s_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    if (master->txRemainingBytes == 0U)
    {
        /* Done transmitting */
        return;
    }

    /* Read data from user buffer and update tx buffer pointer and remaining bytes count */
    switch (master->byteWidth)
    {
    case 1U:
        data = (uint32_t)(*(const uint8_t *)master->txData);
        break;
    case 2U:
        data = (uint32_t)(*(const uint16_t *)master->txData);
        break;
    default:
        data = (uint32_t)(*(const uint32_t *)master->txData);
        break;
    }
    master->txData = &master->txData[master->byteWidth];
    master->txRemainingBytes -= (uint32_t)(master->byteWidth);

    /* Write data to shifter buffer */
    /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
    data <<= 32U - (uint32_t)(master->bitsWidth);
    FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterCheckStatus
 * Description   : Check status of the I2S transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the I2S transfer.
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterCheckStatus(void *stateStruct)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    flexio_i2s_master_state_t *master;

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_i2s_master_state_t *)stateStruct;
    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check for errors */
    if (master->txData != NULL)
    {
        if(FLEXIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex)))
        {
            master->status = STATUS_I2S_TX_UNDERRUN;
            FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
            /* Continue processing events */
        }
    }
    if (master->rxData != NULL)
    {
        if (FLEXIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex)))
        {
            master->status = STATUS_I2S_RX_OVERRUN;
            FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
            /* Continue processing events */
        }
    }
    /* Check if data was received */
    if (FLEXIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        FLEXIO_I2S_DRV_ReadData(master);
        if ((master->rxData != NULL) && (master->rxRemainingBytes == 0U))
        {
            /* Out of rx space, call callback to allow user to provide a new buffer */
            if (master->callback != NULL)
            {
                master->callback(I2S_EVENT_RX_FULL, master->callbackParam);
            }
        }
    }
    /* Check if transmitter needs more data */
    if (master->txData != NULL)
    {
        if (FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)))
        {
            FLEXIO_I2S_DRV_WriteData(master);
            if (master->txRemainingBytes == 0U)
            {
                master->txData = NULL;
                /* Out of data, call callback to allow user to provide a new buffer */
                if (master->callback != NULL)
                {
                    master->callback(I2S_EVENT_TX_EMPTY, master->callbackParam);
                }
                if ((master->txRemainingBytes == 0U) && (master->driverType == FLEXIO_DRIVER_TYPE_INTERRUPTS))
                {
                    /* Still no more data to transmit, transmission will stop */
                    /* disable tx interrupts */
                    FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                    FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                    /* Enable rx interrupt to signal end of transfer */
                    FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
                }
            }
        }
    }

    /* Check if transfer is over */
    if (master->rxRemainingBytes == 0U)
    {
        /* Record success if there was no error */
        if (master->status == STATUS_BUSY)
        {
            master->status = STATUS_SUCCESS;
        }
        /* End transfer */
        FLEXIO_I2S_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterEndDmaTxTransfer
 * Description   : function called at the end of a DMA Tx transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterEndDmaTxTransfer(void *stateStruct, edma_chn_status_t status)
{
    flexio_i2s_master_state_t *master;
    uint8_t dmaChn;

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_i2s_master_state_t *)stateStruct;

    /* Call callback to allow user to provide a new buffer */
    if (status == EDMA_CHN_ERROR)
    {
        /* DMA error, stop transfer */
        master->status = STATUS_ERROR;
        /* End transfer */
        FLEXIO_I2S_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    else
    {
        if (master->callback != NULL)
        {
            master->callback(I2S_EVENT_TX_EMPTY, master->callbackParam);
        }
    }
    if (master->txRemainingBytes == 0U)
    {
        /* No more data to transmit, transmission will stop */
        master->txData = NULL;
    }
    else
    {
        /* There is more data to transfer, restart DMA channel */
        /* Update buffer address and size */
        dmaChn = master->txDMAChannel;
        EDMA_DRV_SetSrcAddr(dmaChn, (uint32_t)(master->txData));
        EDMA_DRV_SetMajorLoopIterationCount(dmaChn, master->txRemainingBytes / master->byteWidth);
        /* Now that this tx is set up, clear remaining bytes count */
        master->txRemainingBytes = 0U;
        /* Start the channel */
        (void)EDMA_DRV_StartChannel(dmaChn);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterEndDmaRxTransfer
 * Description   : function called at the end of a DMA Tx transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterEndDmaRxTransfer(void *stateStruct, edma_chn_status_t status)
{
    flexio_i2s_master_state_t *master;
    uint8_t dmaChn;
    uint32_t addr;

    DEV_ASSERT(stateStruct != NULL);

    (void)status;
    master = (flexio_i2s_master_state_t *)stateStruct;

    /* Check if this was a reception */
    if (master->rxData != NULL)
    {
        if (status == EDMA_CHN_ERROR)
        {
            /* DMA error, stop transfer */
            master->status = STATUS_ERROR;
        }
        else
        {
            /* Call callback to allow user to provide a new buffer */
            if(master->callback != NULL)
            {
                master->callback(I2S_EVENT_RX_FULL, master->callbackParam);
            }
        }
    
    }
    if (master->rxRemainingBytes == 0U)
    {
        /* No more data, end transfer */
        if (master->status == STATUS_BUSY)
        {
            master->status = STATUS_SUCCESS;
        }
        /* End transfer */
        FLEXIO_I2S_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
    else
    {
        /* There is more data to transfer, restart DMA channel */
        /* Update buffer address and size */
        dmaChn = master->rxDMAChannel;
        if (master->rxData != NULL)
        {
            addr = (uint32_t)(master->rxData);
        }
        else
        {
            /* if there is no data to receive, use dummy data as destination for DMA transfer */
            addr = (uint32_t)(&(master->dummyDmaData));
        }
        EDMA_DRV_SetDestAddr(dmaChn, addr);
        EDMA_DRV_SetMajorLoopIterationCount(dmaChn, master->rxRemainingBytes / master->byteWidth);
        /* Now that this rx is set up, clear remaining bytes count */
        master->rxRemainingBytes = 0U;
        /* Start the channel */
        (void)EDMA_DRV_StartChannel(dmaChn);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for DMA tx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_I2S_DRV_MasterComputeTxRegAddr(const flexio_i2s_master_state_t *master)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    shifter = TX_SHIFTER(master->flexioCommon.resourceIndex);
    addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - master->byteWidth);
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for DMA rx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_I2S_DRV_MasterComputeRxRegAddr(const flexio_i2s_master_state_t *master)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    shifter = RX_SHIFTER(master->flexioCommon.resourceIndex);
    addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter]));
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterStartDmaTransfer
 * Description   : Starts a DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2S_DRV_MasterStartDmaTransfer(flexio_i2s_master_state_t *master)
{
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    FLEXIO_Type *baseAddr;
    uint32_t addr;
    uint8_t requestMask = 0U;

    /* Table to map flexio_i2s transfer sizes to EDMA transfer sizes */
    static const edma_transfer_size_t dmaTransferSize[4U] =
        {EDMA_TRANSFER_SIZE_1B, EDMA_TRANSFER_SIZE_2B, EDMA_TRANSFER_SIZE_4B, EDMA_TRANSFER_SIZE_4B};

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Configure Tx channel if this is a transmission */
    if (master->txData != NULL)
    {
        /* Configure the transfer control descriptor for the previously allocated channel */
        (void)EDMA_DRV_ConfigMultiBlockTransfer(master->txDMAChannel,
                                                EDMA_TRANSFER_MEM2PERIPH,
                                                (uint32_t)(master->txData),
                                                FLEXIO_I2S_DRV_MasterComputeTxRegAddr(master),
                                                dmaTransferSize[master->byteWidth - 1U],
                                                master->byteWidth,
                                                master->txRemainingBytes / master->byteWidth,
                                                true);
        /* Now that this tx is set up, clear remaining bytes count */
        master->txRemainingBytes = 0U;

        /* Setup callback for DMA tx transfer end */
        (void)EDMA_DRV_InstallCallback(master->txDMAChannel,
                                       (edma_callback_t)(FLEXIO_I2S_DRV_MasterEndDmaTxTransfer),
                                       (void*)(master));
        /* Start tx DMA channel */
        (void)EDMA_DRV_StartChannel(master->txDMAChannel);
        requestMask = (uint8_t)(1U << TX_SHIFTER(resourceIndex));
    }
    /* Configure Rx channel; if this is a transmission we still use Rx for the "end transfer" event */
    if (master->rxData != NULL)
    {
        addr = (uint32_t)(master->rxData);
    }
    else
    {
        /* if there is no data to receive, use dummy data as destination for DMA transfer */
        addr = (uint32_t)(&(master->dummyDmaData));
    }
    /* Configure the transfer control descriptor for the previously allocated channel */
    (void)EDMA_DRV_ConfigMultiBlockTransfer(master->rxDMAChannel,
                                            EDMA_TRANSFER_PERIPH2MEM,
                                            FLEXIO_I2S_DRV_MasterComputeRxRegAddr(master),
                                            addr,
                                            dmaTransferSize[master->byteWidth - 1U],
                                            master->byteWidth,
                                            master->rxRemainingBytes / master->byteWidth,
                                            true);
    if (master->rxData == NULL)
    {
        /* if there is no data to receive, don't increment destination offset */
        EDMA_DRV_SetDestOffset(master->rxDMAChannel, 0);
    }
    /* Now that this rx is set up, clear remaining bytes count */
    master->rxRemainingBytes = 0U;

    /* Setup callback for DMA rx transfer end */
    (void)EDMA_DRV_InstallCallback(master->rxDMAChannel,
                                   (edma_callback_t)(FLEXIO_I2S_DRV_MasterEndDmaRxTransfer),
                                   (void*)(master));
    /* Start rx DMA channel */
    (void)EDMA_DRV_StartChannel(master->rxDMAChannel);
    requestMask |= (uint8_t)(1U << RX_SHIFTER(resourceIndex));

    /* Enable FlexIO DMA requests */
    FLEXIO_SetShifterDMARequest(baseAddr, requestMask, true);
}

/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterInit
 * Description   : Initialize the FLEXIO_I2S master mode driver
 * Implements : FLEXIO_I2S_DRV_MasterInit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterInit(uint32_t instance,
                                          const flexio_i2s_master_user_config_t * userConfigPtr,
                                          flexio_i2s_master_state_t * master)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(instance < FLEXIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    DEV_ASSERT(g_flexioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need two shifters/timers */
    master->flexioCommon.resourceCount = 2U;
    /* Common FlexIO driver initialization */
    retCode = FLEXIO_DRV_InitDriver(instance, (flexio_common_state_t *)master);
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
    master->bitsWidth = userConfigPtr->bitsWidth;
    master->byteWidth = FLEXIO_I2S_DRV_ComputeByteWidth(userConfigPtr->bitsWidth);
    master->driverIdle = true;
    master->callback = userConfigPtr->callback;
    master->callbackParam = userConfigPtr->callbackParam;
    master->blocking = false;
    master->txData = NULL;
    master->txRemainingBytes = 0U;
    master->rxData = NULL;
    master->rxRemainingBytes = 0U;
    master->master = true;
    master->status = STATUS_SUCCESS;

    /* Configure device for I2S mode */
    FLEXIO_I2S_DRV_MasterConfigure(master, userConfigPtr, inputClock);

    /* Set up transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            master->flexioCommon.isr = FLEXIO_I2S_DRV_MasterCheckStatus;
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_I2S_DRV_MasterGetStatus() will handle the transfer */
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
 * Function Name : FLEXIO_I2S_DRV_MasterDeinit
 * Description   : De-initialize the FLEXIO_I2S master mode driver
 * Implements : FLEXIO_I2S_DRV_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterDeinit(flexio_i2s_master_state_t *master)
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
    return FLEXIO_DRV_DeinitDriver((flexio_common_state_t *)master);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterSetConfig
 * Description   : Set the baud rate and bit width for any subsequent I2S transfer
 * Implements : FLEXIO_I2S_DRV_MasterSetConfig_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterSetConfig(flexio_i2s_master_state_t * master,
                                               uint32_t baudRate,
                                               uint8_t bitsWidth)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(bitsWidth <= 32U);
    /* for DMA transfers bitsWidth must 8, 16, or 32 */
    DEV_ASSERT(!((master->driverType == FLEXIO_DRIVER_TYPE_DMA) &&
                 (bitsWidth != 8U) && (bitsWidth != 16U) && (bitsWidth != 32U)
                ));

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
    FLEXIO_I2S_DRV_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure SCK timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((((uint16_t)((uint16_t)bitsWidth << 1U) - 1U) << 8U) + divider));
    /* Configure WS timer */
    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((bitsWidth * ((divider + 1U) * 2U)) - 1U));

    master->bitsWidth = bitsWidth;
    master->byteWidth = FLEXIO_I2S_DRV_ComputeByteWidth(bitsWidth);

    (void)clkErr;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 * Implements : FLEXIO_I2S_DRV_MasterGetBaudRate_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterGetBaudRate(flexio_i2s_master_state_t *master, uint32_t *baudRate)
{
    const FLEXIO_Type *baseAddr;
    uint32_t inputClock;
    uint16_t divider;
    uint16_t timerCmp;
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
    *baudRate = (inputClock + (uint32_t)divider + 1U) / (2U * ((uint32_t)divider + 1U));

    (void)clkErr;
    return STATUS_SUCCESS;
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterSendData
 * Description   : Perform a non-blocking send transaction on the I2S bus
 * Implements : FLEXIO_I2S_DRV_MasterSendData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterSendData(flexio_i2s_master_state_t *master,
                                              const uint8_t * txBuff,
                                              uint32_t txSize)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);
    DEV_ASSERT((txSize % (uint32_t)(master->byteWidth)) == 0U);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    master->txData = txBuff;
    master->txRemainingBytes = txSize;
    /* also count received data, it tells us when tx is actually completed */
    master->rxRemainingBytes = txSize;
    master->rxData = NULL;
    master->status = STATUS_BUSY;
    master->driverIdle = false;

    /* Enable pin output */
    FLEXIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_PIN_CONFIG_OUTPUT);
    /* Enable timers and shifters */
    FLEXIO_I2S_DRV_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Enable interrupts for Tx shifter */
            FLEXIO_SetShifterInterrupt(baseAddr,
                                 (uint8_t)(1U << TX_SHIFTER(resourceIndex)),
                                 true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr,
                                 (uint8_t)(1U << TX_SHIFTER(resourceIndex)),
                                 true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_I2S_DRV_MasterCheckStatus once to send the first byte */
            FLEXIO_I2S_DRV_MasterCheckStatus(master);
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_I2S_DRV_MasterStartDmaTransfer(master);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2S bus
 * Implements : FLEXIO_I2S_DRV_MasterSendDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterSendDataBlocking(flexio_i2s_master_state_t *master,
                                                          const uint8_t * txBuff,
                                                          uint32_t txSize,
                                                          uint32_t timeout)
{
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
    /* Call FLEXIO_I2S_DRV_MasterSendData to start transfer */
    (void)FLEXIO_I2S_DRV_MasterSendData(master, txBuff, txSize);

    /* Wait for transfer to end */
    return FLEXIO_I2S_DRV_MasterWaitTransferEnd(master, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2S bus
 * Implements : FLEXIO_I2S_DRV_MasterReceiveData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterReceiveData(flexio_i2s_master_state_t *master,
                                                     uint8_t * rxBuff,
                                                     uint32_t rxSize)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);
    DEV_ASSERT((rxSize % (uint32_t)(master->byteWidth)) == 0U);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    master->rxData = rxBuff;
    master->rxRemainingBytes = rxSize;
    master->txData = NULL;
    master->txRemainingBytes = 0U;
    master->status = STATUS_BUSY;
    master->driverIdle = false;

    /* Enable timers and shifters */
    FLEXIO_I2S_DRV_MasterEnableTransfer(master);

    /* Enable transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Enable interrupts for Rx shifter */
            FLEXIO_SetShifterInterrupt(baseAddr,
                                 (uint8_t)(1U << RX_SHIFTER(resourceIndex)),
                                 true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr,
                                 (uint8_t)(1U << RX_SHIFTER(resourceIndex)),
                                 true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_I2S_DRV_MasterCheckStatus once to send the first byte */
            FLEXIO_I2S_DRV_MasterCheckStatus(master);
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_I2S_DRV_MasterStartDmaTransfer(master);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    /* For master we need to send a dummy char to start the clock.
       For slave just put a 0 in the buffer to keep the tx line clear while receiving. */
    FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), 0x0, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2S bus
 * Implements : FLEXIO_I2S_DRV_MasterReceiveDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterReceiveDataBlocking(flexio_i2s_master_state_t *master,
                                                        uint8_t * rxBuff,
                                                        uint32_t rxSize,
                                                        uint32_t timeout)
{
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
    /* Call FLEXIO_I2S_DRV_MasterReceiveData to start transfer */
    (void)FLEXIO_I2S_DRV_MasterReceiveData(master, rxBuff, rxSize);

    /* Wait for transfer to end */
    return FLEXIO_I2S_DRV_MasterWaitTransferEnd(master, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking I2S master transaction
 * Implements : FLEXIO_I2S_DRV_MasterTransferAbort_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterTransferAbort(flexio_i2s_master_state_t *master)
{
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (master->driverIdle)
    {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_I2S_ABORTED;
    FLEXIO_I2S_DRV_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking I2S master transaction
 * Implements : FLEXIO_I2S_DRV_MasterGetStatus_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterGetStatus(flexio_i2s_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    DEV_ASSERT(master != NULL);

    /* Use rxRemainingBytes even for transmit; byte is not transmitted
    until rx shifter reports a receive event */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle)
    {
        switch(master->driverType)
        {
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the I2S transfer here */
            FLEXIO_I2S_DRV_MasterCheckStatus(master);
            break;

        case FLEXIO_DRIVER_TYPE_DMA:
            /* In DMA mode just update the remaining count.
            DO NOT write master->rxRemainingBytes directly !!! */
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

    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }
    else
    {
        return master->status;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterSetRxBuffer
 * Description   : Provide a buffer for receiving data.
 * Implements : FLEXIO_I2S_DRV_MasterSetRxBuffer_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterSetRxBuffer(flexio_i2s_master_state_t *master,
                                                 uint8_t * rxBuff,
                                                 uint32_t rxSize)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    master->rxData = rxBuff;
    master->rxRemainingBytes = rxSize;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterSetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 * Implements : FLEXIO_I2S_DRV_MasterSetTxBuffer_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_MasterSetTxBuffer(flexio_i2s_master_state_t *master,
                                          const uint8_t * txBuff,
                                          uint32_t txSize)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    master->txData = txBuff;
    master->txRemainingBytes = txSize;
    /* for transmit we also count received bytes for end condition */
    master->rxRemainingBytes += txSize;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_SlaveInit
 * Description   : Initialize the FLEXIO_I2S slave mode driver
 * Implements : FLEXIO_I2S_DRV_SlaveInit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_SlaveInit(uint32_t instance,
                                          const flexio_i2s_slave_user_config_t * userConfigPtr,
                                          flexio_i2s_slave_state_t * slave)
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
    slave->flexioCommon.resourceCount = 3U;
    /* Common FlexIO driver initialization */
    retCode = FLEXIO_DRV_InitDriver(instance, (flexio_common_state_t *)slave);
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
    /* Initialize driver-specific context structure */
    slave->driverType = userConfigPtr->driverType;
    slave->bitsWidth = userConfigPtr->bitsWidth;
    slave->byteWidth = FLEXIO_I2S_DRV_ComputeByteWidth(userConfigPtr->bitsWidth);
    slave->driverIdle = true;
    slave->callback = userConfigPtr->callback;
    slave->callbackParam = userConfigPtr->callbackParam;
    slave->blocking = false;
    slave->txData = NULL;
    slave->txRemainingBytes = 0U;
    slave->rxData = NULL;
    slave->rxRemainingBytes = 0U;
    slave->master = false;
    slave->status = STATUS_SUCCESS;

    /* Configure device for I2S mode */
    FLEXIO_I2S_DRV_SlaveConfigure(slave, userConfigPtr);

    /* Set up transfer engine */
    switch (slave->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            slave->flexioCommon.isr = FLEXIO_I2S_DRV_MasterCheckStatus;
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_I2S_DRV_MasterGetStatus() will handle the transfer */
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
 * Function Name : FLEXIO_I2S_DRV_SlaveSetConfig
 * Description   : Set the baud rate and bit width for any subsequent I2S transfer
 * Implements : FLEXIO_I2S_DRV_SlaveSetConfig_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2S_DRV_SlaveSetConfig(flexio_i2s_slave_state_t * slave,
                                               uint8_t bitsWidth)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(slave != NULL);
    DEV_ASSERT(bitsWidth <= 32U);
    /* for DMA transfers bitsWidth must 8, 16, or 32 */
    DEV_ASSERT(!((slave->driverType == FLEXIO_DRIVER_TYPE_DMA) &&
                 (bitsWidth != 8U) && (bitsWidth != 16U) && (bitsWidth != 32U)
                ));

    baseAddr = g_flexioBase[slave->flexioCommon.instance];
    resourceIndex = slave->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!slave->driverIdle)
    {
        return STATUS_BUSY;
    }

    slave->bitsWidth = bitsWidth;
    slave->byteWidth = FLEXIO_I2S_DRV_ComputeByteWidth(bitsWidth);

    /* Configure SCK timer */
    FLEXIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((uint16_t)((uint16_t)bitsWidth << 2U) - 3U));
    /* Configure WS_TIMER_RISING_BLK timer */
    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), (uint16_t)((uint16_t)((uint16_t)bitsWidth << 2U) - 4U));
    /* Configure WS timer */
    FLEXIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((uint16_t)((uint16_t)bitsWidth << 1U) - 1U));

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_MasterGetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_I2S master
 * Implements : FLEXIO_I2S_DRV_MasterGetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_I2S_DRV_MasterGetDefaultConfig(flexio_i2s_master_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate = 256000U;   /* 8 kHz sampling rate, 16 bits per sample */
    userConfigPtr->bitsWidth = 16U;
    userConfigPtr->txPin = 0U;
    userConfigPtr->rxPin = 1U;
    userConfigPtr->sckPin = 2U;
    userConfigPtr->wsPin = 3U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel = 255U;
    userConfigPtr->txDMAChannel = 255U;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2S_DRV_SlaveGetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_I2S slave
 * Implements : FLEXIO_I2S_DRV_SlaveGetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_I2S_DRV_SlaveGetDefaultConfig(flexio_i2s_slave_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->bitsWidth = 16U;
    userConfigPtr->txPin = 0U;
    userConfigPtr->rxPin = 1U;
    userConfigPtr->sckPin = 2U;
    userConfigPtr->wsPin = 3U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel = 255U;
    userConfigPtr->txDMAChannel = 255U;
}


/*!
 * @brief De-initialize the FLEXIO_I2S slave mode driver
 *
 * This function de-initializes the FLEXIO_I2S driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveDeinit_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveDeinit(flexio_i2s_slave_state_t * slave)
{
    return FLEXIO_I2S_DRV_MasterDeinit(slave);
}


/*!
 * @brief Perform a non-blocking send transaction on the I2S bus
 *
 * This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_SlaveGetStatus() to check the progress of the transmission.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveSendData_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveSendData(flexio_i2s_slave_state_t * slave,
                                                    const uint8_t * txBuff,
                                                    uint32_t txSize)
{
    return FLEXIO_I2S_DRV_MasterSendData(slave, txBuff, txSize);
}



/*!
 * @brief Perform a blocking send transaction on the I2S bus
 *
 * This function sends a block of data, and
 * only returns when the transmission is complete.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveSendDataBlocking_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveSendDataBlocking(flexio_i2s_slave_state_t * slave,
                                                            const uint8_t * txBuff,
                                                            uint32_t txSize,
                                                            uint32_t timeout)
{
    return FLEXIO_I2S_DRV_MasterSendDataBlocking(slave, txBuff, txSize, timeout);
}


/*!
 * @brief Perform a non-blocking receive transaction on the I2S bus
 *
 * This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_SlaveGetStatus() to check the progress of the reception.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveReceiveData_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveReceiveData(flexio_i2s_slave_state_t * slave,
                                                       uint8_t * rxBuff,
                                                       uint32_t rxSize)
{
    return FLEXIO_I2S_DRV_MasterReceiveData(slave, rxBuff, rxSize);
}


/*!
 * @brief Perform a blocking receive transaction on the I2S bus
 *
 * This function receives a block of data and only returns when the reception is complete.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveReceiveDataBlocking_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveReceiveDataBlocking(flexio_i2s_slave_state_t * slave,
                                                               uint8_t * rxBuff,
                                                               uint32_t rxSize,
                                                               uint32_t timeout)
{
    return FLEXIO_I2S_DRV_MasterReceiveDataBlocking(slave, rxBuff, rxSize, timeout);
}


/*!
 * @brief Aborts a non-blocking I2S slave transaction
 *
 * This function aborts a non-blocking I2S transfer.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveTransferAbort_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveTransferAbort(flexio_i2s_slave_state_t * slave)
{
    return FLEXIO_I2S_DRV_MasterTransferAbort(slave);
}


/*!
 * @brief Get the status of the current non-blocking I2S slave transaction
 *
 * This function returns the current status of a non-blocking I2S slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveGetStatus_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveGetStatus(flexio_i2s_slave_state_t * slave, uint32_t *bytesRemaining)
{
    return FLEXIO_I2S_DRV_MasterGetStatus(slave, bytesRemaining);
}


/*!
 * @brief Provide a buffer for receiving data
 *
 * This function can be used to provide a driver with a new buffer for receiving data.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveSetRxBuffer_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveSetRxBuffer(flexio_i2s_slave_state_t *slave,
                                                       uint8_t * rxBuff,
                                                       uint32_t rxSize)
{
    return FLEXIO_I2S_DRV_MasterSetRxBuffer(slave, rxBuff, rxSize);
}


/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function can be used to provide a driver with a new buffer for transmitting data.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the buffer containing transmit data
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 * Implements : FLEXIO_I2S_DRV_SlaveSetTxBuffer_Activity
 */
status_t FLEXIO_I2S_DRV_SlaveSetTxBuffer(flexio_i2s_slave_state_t *slave,
                                                       const uint8_t * txBuff,
                                                       uint32_t txSize)
{
    return FLEXIO_I2S_DRV_MasterSetTxBuffer(slave, txBuff, txSize);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
