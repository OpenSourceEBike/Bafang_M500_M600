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

#include "flexio_uart_driver.h"
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
 * Violates MISRA 2012 Required Rule 11.8, Attempt to cast away const/volatile from a pointer or reference.
 * Since the driver is not full duplex, it uses the same state structure member (data) to store the pointers
 * for transmit and receive. So the same pointer is used either for the constant
 * tx buffer, or for the non-constant rx buffer. The information about transfer direction is kept in another
 * state structure member (direction) and the driver will not write in the tx buffer.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter could be declared as pointing to const
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
#define DIVIDER_MIN_VALUE  0
#define DIVIDER_MAX_VALUE  0xFF

/* Shifters/Timers used for UART simulation The parameter x represents the
   resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)     (x)
#define RX_SHIFTER(x)     (x)
#define TX_TIMER(x)       (x)
#define RX_TIMER(x)       (x)

/*******************************************************************************
 * Private Functions
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_ComputeBaudRateDivider(uint32_t baudRate,
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
 * Function Name : FLEXIO_UART_DRV_Configure
 * Description   : configures the FLEXIO module for UART
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_ConfigureTx(flexio_uart_state_t *state,
                                        const flexio_uart_user_config_t * userConfigPtr,
                                        uint32_t inputClock)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Compute divider. */
    FLEXIO_UART_DRV_ComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitCount;

    /* Configure tx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_0,
                                FLEXIO_SHIFTER_STOP_BIT_1,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_TRANSMIT,
                                 userConfigPtr->dataPin,             /* Output on tx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_OUTPUT,
                                 TX_TIMER(resourceIndex),
                                 FLEXIO_TIMER_POLARITY_POSEDGE);

    /* Configure tx timer */
    FLEXIO_SetTimerCompare(baseAddr, TX_TIMER(resourceIndex), (uint16_t)((((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    FLEXIO_SetTimerConfig(baseAddr,
                              TX_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_TIM_DIS,
                              FLEXIO_TIMER_ENABLE_TRG_HIGH,         /* Enable when Tx data is available */
                              FLEXIO_TIMER_DISABLE_TIM_CMP,
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR, /* Decrement on FlexIO clock */
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               TX_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U), /* Trigger on tx shifter status flag */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               0U,                                     /* Pin unused */
                               FLEXIO_PIN_POLARITY_HIGH,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_Configure
 * Description   : configures the FLEXIO module for UART
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_ConfigureRx(flexio_uart_state_t *state,
                                        const flexio_uart_user_config_t * userConfigPtr,
                                        uint32_t inputClock)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Compute divider. */
    FLEXIO_UART_DRV_ComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitCount;

    /* Configure rx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_0,
                                FLEXIO_SHIFTER_STOP_BIT_1,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 userConfigPtr->dataPin,             /* Input from rx pin */
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 RX_TIMER(resourceIndex),
                                 FLEXIO_TIMER_POLARITY_NEGEDGE);

    /* Configure rx timer */
    FLEXIO_SetTimerCompare(baseAddr, RX_TIMER(resourceIndex), (uint16_t)((((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    FLEXIO_SetTimerConfig(baseAddr,
                              RX_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_TIM_DIS,
                              FLEXIO_TIMER_ENABLE_PIN_POSEDGE,         /* Enable when data is available */
                              FLEXIO_TIMER_DISABLE_TIM_CMP,
                              FLEXIO_TIMER_RESET_PIN_RISING,
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR,    /* Decrement on FlexIO clock */
                              FLEXIO_TIMER_INITOUT_ONE_RESET);
    FLEXIO_SetTimerControl(baseAddr,
                               RX_TIMER(resourceIndex),
                               0U,                                      /* Trigger unused */
                               FLEXIO_TRIGGER_POLARITY_HIGH,
                               FLEXIO_TRIGGER_SOURCE_EXTERNAL,
                               userConfigPtr->dataPin,                   /* Input from rx pin */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_EndTransfer
 * Description   : End the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_EndTransfer(flexio_uart_state_t *state)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Disable transfer engine */
    switch (state->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Disable interrupts for Rx / Tx shifter */
            FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
            /* Disable timer interrupt  */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), false);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* For Tx we need to disable timer interrupt */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), false);
            /* Stop DMA channels */
            (void)EDMA_DRV_StopChannel(state->dmaChannel);
            /* Disable the FlexIO DMA request */
            FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    state->remainingBytes = 0U;
    state->driverIdle = true;
    /* Signal transfer end for blocking transfers */
    if (state->blocking == true)
    {
        (void)OSIF_SemaPost(&(state->idleSemaphore));
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_EnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_EnableTransfer(flexio_uart_state_t *state)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = state->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[state->flexioCommon.instance];

    /* Enable timers and shifters */
    if (state->direction == FLEXIO_UART_DIRECTION_RX)
    {
        /* In rx mode, discard any leftover rx. data */
        FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
        FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_RECEIVE);
    }
    FLEXIO_SetTimerMode(baseAddr, TX_TIMER(resourceIndex), FLEXIO_TIMER_MODE_8BIT_BAUD);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_StopTransfer
 * Description   : Forcefully stops the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_StopTransfer(flexio_uart_state_t *state)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = state->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[state->flexioCommon.instance];

    /* Disable and re-enable timers and shifters to reset them */
    FLEXIO_SetTimerMode(baseAddr, TX_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);

    /* Clear any leftover error flags */
    FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
 
    /* End the transfer */
    FLEXIO_UART_DRV_EndTransfer(state);

    /* Re-enable shifter for Tx, to ensure correct idle state */
    if (state->direction == FLEXIO_UART_DIRECTION_TX)
    {
        /* In tx mode restore start bit in case it was changed for end of transmission detection */
        FLEXIO_SetShifterStartBit(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_START_BIT_0);
        FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_TRANSMIT);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_WaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t FLEXIO_UART_DRV_WaitTransferEnd(flexio_uart_state_t *state, uint32_t timeout)
{
    status_t osifError = STATUS_SUCCESS;

    switch (state->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Wait for transfer to be completed by the IRQ */
            osifError = OSIF_SemaWait(&(state->idleSemaphore), timeout);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_UART_DRV_GetStatus() to do the transfer */
            while (FLEXIO_UART_DRV_GetStatus(state, NULL) == STATUS_BUSY) {}
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Wait for transfer completion to be signaled by the DMA or IRQ */
            osifError = OSIF_SemaWait(&(state->idleSemaphore), timeout);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    /* Blocking transfer is over */
    state->blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* Abort current transfer */
        state->status = STATUS_TIMEOUT;
        FLEXIO_UART_DRV_StopTransfer(state);
    }

    return state->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ReadData
 * Description   : reads data received by the module
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_ReadData(flexio_uart_state_t *state)
{
    const FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state->remainingBytes > 0U);
    DEV_ASSERT(state->rxData != NULL);

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Read data from shifter buffer */
    data = FLEXIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_RW_MODE_NORMAL);
    data >>= 32U - (uint32_t)(state->bitCount);

    if (state->bitCount <= 8U)
    {
        *(uint8_t *)state->rxData = (uint8_t)data;
        /* Update rx buffer pointer and remaining bytes count */
        state->rxData ++;
        state->remainingBytes -= 1U;
    }
    else
    {
        /* For more than 8 bits per word 2 bytes are needed */
        *(uint16_t *)state->rxData = (uint16_t)data;
        /* Update rx buffer pointer and remaining bytes count */
        state->rxData = &state->rxData[2U];
        state->remainingBytes -= 2U;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_WriteData
 * Description   : writes data to be transmitted by the module
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_WriteData(flexio_uart_state_t *state)
{
    FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state->txData != NULL);

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Read data from user buffer and update tx buffer pointer and remaining bytes count */
    if (state->bitCount <= 8U)
    {
        data = (uint32_t)(*(uint8_t *)state->txData);
        state->txData++;
        state->remainingBytes -= 1U;
    }
    else
    {
        /* For more than 8 bits per word 2 bytes are needed */
        data = (uint32_t)(*(uint16_t *)state->txData);
        state->txData = &state->txData[2U];
        state->remainingBytes -= 2U;
    }

    FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, FLEXIO_SHIFTER_RW_MODE_NORMAL);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_CheckStatusTx
 * Description   : Check status of the UART transmission. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_CheckStatusTx(void *stateStruct)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    flexio_uart_state_t *state;

    DEV_ASSERT(stateStruct != NULL);

    state = (flexio_uart_state_t *)stateStruct;
    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* No need to check for Tx underflow since timer is controlled by the shifter status flag */
    /* Check for transfer end */
    if (state->remainingBytes == 0U)
    {
        if (FLEXIO_GetTimerStatus(baseAddr, TX_TIMER(resourceIndex)))
        {
            /* Clear timer status */
            FLEXIO_ClearTimerStatus(baseAddr, TX_TIMER(resourceIndex));
            state->txFlush--;
            if (state->txFlush == 0U)
            {
                /* Done flushing the Tx buffer, end transfer */
                /* Record success if there was no error */
                if (state->status == STATUS_BUSY)
                {
                    state->status = STATUS_SUCCESS;
                }
                FLEXIO_UART_DRV_StopTransfer(state);
                /* Call callback to announce the end transfer event to the user */
                if (state->callback != NULL)
                {
                    state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
                }
            }
            else if (FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)))
            {
                /* txFlush == 1, but last byte was already transferred from buffer to shifter. There is a
                   danger that the transmission is over and we end up never reporting the end event.
                   To avoid this, send one extra dummy byte */
                /* Set start bit to 1 and send an 0xFF byte, this way the line will appear idle */
                FLEXIO_SetShifterStartBit(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_START_BIT_1);
                FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), 0xFFFFFFFFUL, FLEXIO_SHIFTER_RW_MODE_NORMAL);
            }
            else
            {
                /* txFlush == 1, and last byte was not yet transferred from buffer to shifter.
                   No need to do anything, just wait for the next timer event. */
            }
        }
    }
    /* Check if transmitter needs more data */
    else if (FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)) && (state->remainingBytes > 0U))
    {
        FLEXIO_UART_DRV_WriteData(state);
        if (state->remainingBytes == 0U)
        {
            /* Out of data, call callback to allow user to provide a new buffer */
            if (state->callback != NULL)
            {
                state->callback(state, UART_EVENT_TX_EMPTY, state->callbackParam);
            }
        }
        if (state->remainingBytes == 0U)
        {
            /* No more data, transmission will stop after the last bytes are sent.
               The timer event will indicate when the send is complete */
            /* Clear any previous timer events */
            FLEXIO_ClearTimerStatus(baseAddr, TX_TIMER(resourceIndex));
            if (state->driverType == FLEXIO_DRIVER_TYPE_INTERRUPTS)
            {
                /* Transmission completed; disable interrupt */
                FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                /* Enable timer interrupt to ensure that transfer is completed */
                FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), true);
            }
        }
    }
    else
    {
        /* No relevant events - nothing to do */
    }

}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_CheckStatusRx
 * Description   : Check status of the UART reception. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_CheckStatusRx(void *stateStruct)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    flexio_uart_state_t *state;

    DEV_ASSERT(stateStruct != NULL);

    state = (flexio_uart_state_t *)stateStruct;
    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Check for errors */
    if (FLEXIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        state->status = STATUS_UART_RX_OVERRUN;
        FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
        state->remainingBytes = 0U;
        /* Continue processing events */
    }
    /* Check if data was received */
    else if (FLEXIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        FLEXIO_UART_DRV_ReadData(state);
        if (state->remainingBytes == 0U)
        {
            /* Out of data, call callback to allow user to provide a new buffer */
            if (state->callback != NULL)
            {
                state->callback(state, UART_EVENT_RX_FULL, state->callbackParam);
            }
        }
    }
    else
    {
        /* No events - nothing to do */
    }
    /* Check if transfer is over */
    if (state->remainingBytes == 0U)
    {
        /* Record success if there was no error */
        if (state->status == STATUS_BUSY)
        {
            state->status = STATUS_SUCCESS;
        }
        /* Discard any leftover rx. data */
        FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
        /* End transfer */
        FLEXIO_UART_DRV_StopTransfer(state);
        /* Call callback to announce the event to the user */
        if (state->callback != NULL)
        {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_CheckStatus
 * Description   : Check status of the UART transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_CheckStatus(void *stateStruct)
{
    const flexio_uart_state_t *state;

    DEV_ASSERT(stateStruct != NULL);

    state = (flexio_uart_state_t *)stateStruct;
    if (state->direction == FLEXIO_UART_DIRECTION_TX)
    {
        FLEXIO_UART_DRV_CheckStatusTx(stateStruct);
    }
    else
    {
        FLEXIO_UART_DRV_CheckStatusRx(stateStruct);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_EndDmaTxTransfer
 * Description   : function called at the end of a DMA Tx transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_EndDmaTxTransfer(void *stateStruct, edma_chn_status_t status)
{
    flexio_uart_state_t *state;
    uint8_t dmaChn;
    uint32_t byteCount;
    FLEXIO_Type *baseAddr;

    DEV_ASSERT(stateStruct != NULL);

    state = (flexio_uart_state_t *)stateStruct;
    baseAddr = g_flexioBase[state->flexioCommon.instance];

    if (status == EDMA_CHN_ERROR)
    {
        /* DMA error, stop transfer */
        state->status = STATUS_ERROR;
        FLEXIO_UART_DRV_StopTransfer(state);
        /* Call callback to announce the end transfer event to the user */
        if (state->callback != NULL)
        {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    }
    else
    {
        /* Call callback to allow user to provide a new buffer */
        if (state->callback != NULL)
        {
            state->callback(state, UART_EVENT_TX_EMPTY, state->callbackParam);
        }
        if (state->remainingBytes == 0U)
        {
            /* No more data to transmit, transmission will stop */
            /* Enable timer interrupt to let IRQ ensure that transfer is completed */
            FLEXIO_ClearTimerStatus(baseAddr, TX_TIMER(state->flexioCommon.resourceIndex));
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(state->flexioCommon.resourceIndex)), true);
        }
        else
        {
            /* There is more data to transfer, restart DMA channel */
            /* Update buffer address and size */
            dmaChn = state->dmaChannel;
            if (state->bitCount <= 8U)
            {
                byteCount = 1U;
            }
            else
            {
                byteCount = 2U;
            }
            EDMA_DRV_SetSrcAddr(dmaChn, (uint32_t)(state->txData));
            EDMA_DRV_SetMajorLoopIterationCount(dmaChn, state->remainingBytes / byteCount);
            /* Now that this tx is set up, clear remaining bytes count */
            state->remainingBytes = 0U;
            /* Start the channel */
            (void)EDMA_DRV_StartChannel(dmaChn);
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_EndDmaRxTransfer
 * Description   : function called at the end of a DMA Rx transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_EndDmaRxTransfer(void *stateStruct, edma_chn_status_t status)
{
    flexio_uart_state_t *state;
    uint8_t dmaChn;
    uint32_t byteCount;

    DEV_ASSERT(stateStruct != NULL);

    state = (flexio_uart_state_t *)stateStruct;

    if (status == EDMA_CHN_ERROR)
    {
        /* DMA error, stop transfer */
        state->status = STATUS_ERROR;
    }
    else
    {
        /* Call callback to allow user to provide a new buffer */
        if (state->callback != NULL)
        {
            state->callback(state, UART_EVENT_RX_FULL, state->callbackParam);
        }
    }
    if (state->remainingBytes == 0U)
    {
        /* No more data to transmit, reception will stop */
        if (state->status == STATUS_BUSY)
        {
            state->status = STATUS_SUCCESS;
        }
        FLEXIO_UART_DRV_StopTransfer(state);
        /* Call callback to announce the event to the user */
        if (state->callback != NULL)
        {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    }
    else
    {
        /* There is more data to transfer, restart DMA channel */
        /* Update buffer address and size */
        dmaChn = state->dmaChannel;
        if (state->bitCount <= 8U)
        {
            byteCount = 1U;
        }
        else
        {
            byteCount = 2U;
        }
        EDMA_DRV_SetDestAddr(dmaChn, (uint32_t)(state->rxData));
        EDMA_DRV_SetMajorLoopIterationCount(dmaChn, state->remainingBytes / byteCount);
        /* Now that this tx is set up, clear remaining bytes count */
        state->remainingBytes = 0U;
        /* Start the channel */
        (void)EDMA_DRV_StartChannel(dmaChn);
    }
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ComputeTxRegAddr
 * Description   : Computes the address of the register used for DMA tx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_UART_DRV_ComputeTxRegAddr(const flexio_uart_state_t *state)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    shifter = TX_SHIFTER(state->flexioCommon.resourceIndex);
    addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter]));
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ComputeRxRegAddr
 * Description   : Computes the address of the register used for DMA rx transfer
 *
 *END**************************************************************************/
static uint32_t FLEXIO_UART_DRV_ComputeRxRegAddr(const flexio_uart_state_t *state)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;
    uint32_t byteCount;

    if (state->bitCount <= 8U)
    {
        byteCount = 1U;
    }
    else
    {
        byteCount = 2U;
    }
    baseAddr = g_flexioBase[state->flexioCommon.instance];
    shifter = RX_SHIFTER(state->flexioCommon.resourceIndex);
    addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter])) + (sizeof(uint32_t) - byteCount);
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_StartTxDmaTransfer
 * Description   : Starts a Tx DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_StartTxDmaTransfer(flexio_uart_state_t *state)
{
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    FLEXIO_Type *baseAddr;
    edma_transfer_size_t dmaTransferSize;
    uint32_t byteCount;

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Configure the transfer control descriptor for the previously allocated channel */
    if (state->bitCount <= 8U)
    {
        dmaTransferSize = EDMA_TRANSFER_SIZE_1B;
        byteCount = 1U;
    }
    else
    {
        dmaTransferSize = EDMA_TRANSFER_SIZE_2B;
        byteCount = 2U;
    }
    (void)EDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel,
                                             EDMA_TRANSFER_MEM2PERIPH,
                                             (uint32_t)(state->txData),
                                             FLEXIO_UART_DRV_ComputeTxRegAddr(state),
                                             dmaTransferSize,
                                             byteCount,
                                             state->remainingBytes / byteCount,
                                             true);

    /* Now that this transfer is set up, clear remaining bytes count */
    state->remainingBytes = 0U;

    /* Setup callback for DMA tx transfer end */
    (void)EDMA_DRV_InstallCallback(state->dmaChannel,
                                   (edma_callback_t)(FLEXIO_UART_DRV_EndDmaTxTransfer),
                                   (void*)(state));
    /* Start tx DMA channel */
    (void)EDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable FlexIO DMA requests */
    FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_StartRxDmaTransfer
 * Description   : Starts an Rx DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_UART_DRV_StartRxDmaTransfer(flexio_uart_state_t *state)
{
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    FLEXIO_Type *baseAddr;
    edma_transfer_size_t dmaTransferSize;
    uint32_t byteCount;

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Configure the transfer control descriptor for the previously allocated channel */
    if (state->bitCount <= 8U)
    {
        dmaTransferSize = EDMA_TRANSFER_SIZE_1B;
        byteCount = 1U;
    }
    else
    {
        dmaTransferSize = EDMA_TRANSFER_SIZE_2B;
        byteCount = 2U;
    }
    (void)EDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel,
                                            EDMA_TRANSFER_PERIPH2MEM,
                                            FLEXIO_UART_DRV_ComputeRxRegAddr(state),
                                            (uint32_t)(state->rxData),
                                            dmaTransferSize,
                                            byteCount,
                                            state->remainingBytes / byteCount,
                                            true);

    /* Now that this transfer is set up, clear remaining bytes count */
    state->remainingBytes = 0U;

    /* Setup callback for DMA tx transfer end */
    (void)EDMA_DRV_InstallCallback(state->dmaChannel,
                                   (edma_callback_t)(FLEXIO_UART_DRV_EndDmaRxTransfer),
                                   (void*)(state));
    /* Start tx DMA channel */
    (void)EDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable FlexIO DMA requests */
    FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
}

/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_Init
 * Description   : Initialize the FLEXIO_UART driver
 * Implements : FLEXIO_UART_DRV_Init_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_Init(uint32_t instance,
                                     const flexio_uart_user_config_t * userConfigPtr,
                                     flexio_uart_state_t * state)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReq;

    DEV_ASSERT(state != NULL);
    DEV_ASSERT(instance < FLEXIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    DEV_ASSERT(g_flexioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);


    /* Instruct the resource allocator that we need one shifter/timer */
    state->flexioCommon.resourceCount = 1U;
    /* Common FlexIO driver initialization */
    retCode = FLEXIO_DRV_InitDriver(instance, (flexio_common_state_t *)state);
    if (retCode != STATUS_SUCCESS)
    {   /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        osifError = OSIF_SemaCreate(&(state->idleSemaphore), 0U);
        DEV_ASSERT(osifError == STATUS_SUCCESS);
    }

    /* Initialize driver-specific context structure */
    state->rxData = NULL;
    state->txData = NULL;
    state->remainingBytes = 0U;
    state->callback = userConfigPtr->callback;
    state->callbackParam = userConfigPtr->callbackParam;
    state->blocking = false;
    state->driverType = userConfigPtr->driverType;
    state->direction = userConfigPtr->direction;
    state->status = STATUS_SUCCESS;
    state->driverIdle = true;
    state->bitCount = userConfigPtr->bitCount;

    if (state->direction == FLEXIO_UART_DIRECTION_TX)
    {
        /* Configure device for UART Tx mode */
        FLEXIO_UART_DRV_ConfigureTx(state, userConfigPtr, inputClock);
    }
    else
    {
        /* Configure device for UART Rx mode */
        FLEXIO_UART_DRV_ConfigureRx(state, userConfigPtr, inputClock);
    }

    /* Set up transfer engine */
    switch (state->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            if (state->direction == FLEXIO_UART_DIRECTION_TX)
            {
                state->flexioCommon.isr = FLEXIO_UART_DRV_CheckStatusTx;
            }
            else
            {
                state->flexioCommon.isr = FLEXIO_UART_DRV_CheckStatusRx;
            }
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_UART_DRV_GetStatus() will handle the transfer */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Store DMA channel number */
            state->dmaChannel = userConfigPtr->dmaChannel;
            /* Configure DMA request source */
            dmaReq = g_flexioDMASrc[instance][TX_SHIFTER(state->flexioCommon.resourceIndex)];
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->dmaChannel, dmaReq, false);
            /* For Tx we will still need interrupt to signal end of transfer */
            if (state->direction == FLEXIO_UART_DIRECTION_TX)
            {
                state->flexioCommon.isr = FLEXIO_UART_DRV_CheckStatusTx;
            }
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
 * Function Name : FLEXIO_UART_DRV_Deinit
 * Description   : De-initialize the FLEXIO_UART driver
 * Implements : FLEXIO_UART_DRV_Deinit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_Deinit(flexio_uart_state_t *state)
{
    DEV_ASSERT(state != NULL);

    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (state->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        (void)OSIF_SemaDestroy(&(state->idleSemaphore));
    }

    return FLEXIO_DRV_DeinitDriver((flexio_common_state_t *)state);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_SetConfig
 * Description   : Set the baud rate and bit width for any subsequent UART transfer
 * Implements : FLEXIO_UART_DRV_SetConfig_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_SetConfig(flexio_uart_state_t * state,
                                          uint32_t baudRate,
                                          uint8_t bitCount)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state != NULL);
    DEV_ASSERT(baudRate > 0U);
    DEV_ASSERT(bitCount > 0U);
    DEV_ASSERT(bitCount <= 16U);
    /* For DMA transfers bitCount must 8 */
    DEV_ASSERT(!((state->driverType == FLEXIO_DRIVER_TYPE_DMA) && (bitCount != 8U)));

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }
    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[state->flexioCommon.instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Compute divider */
    FLEXIO_UART_DRV_ComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure tx/rx timer */
    FLEXIO_SetTimerCompare(baseAddr, TX_TIMER(resourceIndex), (uint16_t)((uint16_t)(((uint16_t)((uint16_t)bitCount << 1U) - 1U) << 8U) + divider));

    state->bitCount = bitCount;

    (void)clkErr;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_GetBaudRate
 * Description   : Get the currently configured baud rate
 * Implements : FLEXIO_UART_DRV_GetBaudRate_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_GetBaudRate(flexio_uart_state_t *state, uint32_t *baudRate)
{
    const FLEXIO_Type *baseAddr;
    uint32_t inputClock;
    uint16_t divider;
    uint16_t timerCmp;
    status_t clkErr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state != NULL);
    DEV_ASSERT(baudRate != NULL);

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_flexioClock[state->flexioCommon.instance], &inputClock);
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Get the currently configured divider */
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, TX_TIMER(resourceIndex));
    divider = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 1)). Round to nearest integer */
    *baudRate = (inputClock + (uint32_t)divider + 1U) / (2U * ((uint32_t)divider + 1U));

    (void)clkErr;
    return STATUS_SUCCESS;
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_SendData
 * Description   : Perform a non-blocking UART transmission
 * Implements : FLEXIO_UART_DRV_SendData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_SendData(flexio_uart_state_t *state,
                                  const uint8_t * txBuff,
                                  uint32_t txSize)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);
    DEV_ASSERT(state->direction == FLEXIO_UART_DIRECTION_TX);
    /* If 2 bytes per word are needed, then the size must be even */
    DEV_ASSERT((state->bitCount <= 8U) || ((txSize & 1U) == 0U));

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }

    state->txData = (uint8_t *)txBuff;
    state->remainingBytes = txSize;
    state->status = STATUS_BUSY;
    state->driverIdle = false;
    /* Number of bytes to flush after the last byte is copied in the tx shifter buffer */
    state->txFlush = (uint8_t)((txSize == 1U) ? 1U : 2U);

    /* Enable timers and shifters */
    FLEXIO_UART_DRV_EnableTransfer(state);
    /* Enable transfer engine */
    switch (state->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Enable interrupts for Tx shifter */
            FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_UART_DRV_CheckStatus once to send the first byte */
            FLEXIO_UART_DRV_CheckStatus(state);
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_UART_DRV_StartTxDmaTransfer(state);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_SendDataBlocking
 * Description   : Perform a blocking UART transmission
 * Implements : FLEXIO_UART_DRV_SendDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_SendDataBlocking(flexio_uart_state_t *state,
                                          const uint8_t * txBuff,
                                          uint32_t txSize,
                                          uint32_t timeout)
{
    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }
    /* Mark transfer as blocking */
    if (state->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        state->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OSIF_SemaWait(&(state->idleSemaphore), 0);
    }
    /* Call FLEXIO_UART_DRV_SendData to start transfer */
    /* We already did the busy check so no need to check return code of FLEXIO_UART_DRV_SendData */
    (void)FLEXIO_UART_DRV_SendData(state, txBuff, txSize);

    /* Wait for transfer to end */
    return FLEXIO_UART_DRV_WaitTransferEnd(state, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ReceiveData
 * Description   : Perform a non-blocking UART reception
 * Implements : FLEXIO_UART_DRV_ReceiveData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_ReceiveData(flexio_uart_state_t *state,
                                     uint8_t * rxBuff,
                                     uint32_t rxSize)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(state != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);
    DEV_ASSERT(state->direction == FLEXIO_UART_DIRECTION_RX);
    /* If 2 bytes per word are needed, then the size must be even */
    DEV_ASSERT((state->bitCount <= 8U) || ((rxSize & 1U) == 0U));

    baseAddr = g_flexioBase[state->flexioCommon.instance];
    resourceIndex = state->flexioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }

    state->rxData = rxBuff;
    state->remainingBytes = rxSize;
    state->status = STATUS_BUSY;
    state->driverIdle = false;

    /* Enable timers and shifters */
    FLEXIO_UART_DRV_EnableTransfer(state);
    /* Enable transfer engine */
    switch (state->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Enable interrupts for Rx shifter */
            FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_UART_DRV_CheckStatus once to send the first byte */
            FLEXIO_UART_DRV_CheckStatus(state);
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_UART_DRV_StartRxDmaTransfer(state);
            break;
        default:
            /* Impossible type - do nothing */
            break;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_ReceiveDataBlocking
 * Description   : Perform a blocking UART reception
 * Implements : FLEXIO_UART_DRV_ReceiveDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_ReceiveDataBlocking(flexio_uart_state_t *state,
                                             uint8_t * rxBuff,
                                             uint32_t rxSize,
                                             uint32_t timeout)
{
    /* Check if driver is busy */
    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }
    /* Mark transfer as blocking */
    if (state->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        state->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OSIF_SemaWait(&(state->idleSemaphore), 0);
    }
    /* Call FLEXIO_UART_DRV_ReceiveData to start transfer */
    /* We already did the busy check so no need to check return code of FLEXIO_UART_DRV_ReceiveData */
    (void)FLEXIO_UART_DRV_ReceiveData(state, rxBuff, rxSize);

    /* Wait for transfer to end */
    return FLEXIO_UART_DRV_WaitTransferEnd(state, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_TransferAbort
 * Description   : Aborts a non-blocking UART transfer
 * Implements : FLEXIO_UART_DRV_TransferAbort_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_TransferAbort(flexio_uart_state_t *state)
{
    DEV_ASSERT(state != NULL);

    /* Check if driver is busy */
    if (state->driverIdle)
    {
        return STATUS_SUCCESS;
    }

    state->status = STATUS_UART_ABORTED;
    FLEXIO_UART_DRV_StopTransfer(state);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_GetStatus
 * Description   : Get the status of the current non-blocking UART transaction
 * Implements : FLEXIO_UART_DRV_GetStatus_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_GetStatus(flexio_uart_state_t *state, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    DEV_ASSERT(state != NULL);

    /* Initialize with the actual remaining bytes count */
    remainingBytes = state->remainingBytes;

    if(!state->driverIdle)
    {
        switch(state->driverType)
        {
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the UART transfer here */
            FLEXIO_UART_DRV_CheckStatus(state);
            break;

        case FLEXIO_DRIVER_TYPE_DMA:
            /* In DMA mode just update the remaining count.
            DO NOT write state->remainingBytes directly !!! */
            remainingBytes = EDMA_DRV_GetRemainingMajorIterationsCount(state->dmaChannel);
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

    if (!state->driverIdle)
    {
        return STATUS_BUSY;
    }
    else
    {
        return state->status;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_SetRxBuffer
 * Description   : Provide a buffer for receiving data.
 * Implements : FLEXIO_UART_DRV_SetRxBuffer_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_SetRxBuffer(flexio_uart_state_t *state,
                                            uint8_t * rxBuff,
                                            uint32_t rxSize)
{
    DEV_ASSERT(state != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    state->rxData = rxBuff;
    state->remainingBytes = rxSize;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_SetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 * Implements : FLEXIO_UART_DRV_SetTxBuffer_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_UART_DRV_SetTxBuffer(flexio_uart_state_t *state,
                                    const uint8_t * txBuff,
                                    uint32_t txSize)
{
    DEV_ASSERT(state != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    state->txData = (uint8_t *)txBuff;
    state->remainingBytes = txSize;
    /* In case of continuous transmission there are always 2 bytes to flush */
    state->txFlush = 2;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_UART_DRV_GetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_UART
 * Implements : FLEXIO_UART_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_UART_DRV_GetDefaultConfig(flexio_uart_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate = 115200U;
    userConfigPtr->bitCount = 8U;
    userConfigPtr->direction = FLEXIO_UART_DIRECTION_TX;
    userConfigPtr->dataPin = 0U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->dmaChannel = 255U;
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
