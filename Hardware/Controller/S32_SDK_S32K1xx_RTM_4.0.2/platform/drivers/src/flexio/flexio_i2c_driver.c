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

#include "flexio_i2c_driver.h"
#include "flexio_hw_access.h"
#include "flexio_common.h"
#include "clock_manager.h"
#include "interrupt_manager.h"

/**
 * @page misra_violations MISRA-C:2012 violations
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
 * This is needed for callbacks from other modules, which cannot know the actual argument type (flexio_common, dma)
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
 * The driver uses a common function for transmit and receive, so the same pointer is used either for the constant 
 * tx buffer, or for the non-constant rx buffer. The information about transfer direction is kept in another variable
 * and the driver will not write in the tx buffer.
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
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 13.5, side effects on right hand of logical operator, ''||''
 * EDMA_DRV_GetChannelStatus is a function without side effects.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.4, Mismatched essential type categories for binary operator.
 * Function is defined for usage by application code.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

    /* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE  1
#define DIVIDER_MAX_VALUE  0xFF

    /* Shifters/Timers used for I2C simulation The parameter x represents the 
       resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)     (x)
#define RX_SHIFTER(x)     (uint8_t)((x) + 1U)
#define SCL_TIMER(x)      (x)
#define CONTROL_TIMER(x)  (uint8_t)((x) + 1U)

/*******************************************************************************
 * Private Functions
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterComputeBaudRateDivider(uint32_t baudRate, 
                                                        uint16_t *divider,
                                                        uint32_t inputClock)
{
    int32_t tmpDiv;

    /* Compute divider: ((input_clock / baud_rate) / 2) - 1 - 1. The extra -1 is from the 
       timer reset setting used for clock stretching. Round to nearest integer */
    tmpDiv = (((int32_t)inputClock + (int32_t)baudRate) / (2 * (int32_t)baudRate)) - 2;
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
 * Function Name : FLEXIO_I2C_DRV_MasterConfigure
 * Description   : configures the FLEXIO module as I2C master
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterConfigure(const flexio_i2c_master_state_t *master, uint32_t inputClock, uint32_t baudRate)
{
    FLEXIO_Type *baseAddr;
    uint16_t divider;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Compute divider.*/
    FLEXIO_I2C_DRV_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure tx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                TX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_0,
                                FLEXIO_SHIFTER_STOP_BIT_1,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    /* Shifter disabled and pin enabled causes the pin to be held low. 
       So disable pin too, will be enabled at transfer time.  */
    FLEXIO_SetShifterControl(baseAddr,
                                 TX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 master->sdaPin,                    /* output on SDA pin */
                                 FLEXIO_PIN_POLARITY_LOW,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 CONTROL_TIMER(resourceIndex),                     /* use control timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    FLEXIO_SetShifterConfig(baseAddr,
                                RX_SHIFTER(resourceIndex),
                                FLEXIO_SHIFTER_START_BIT_DISABLED,
                                FLEXIO_SHIFTER_STOP_BIT_0,
                                FLEXIO_SHIFTER_SOURCE_PIN);
    FLEXIO_SetShifterControl(baseAddr,
                                 RX_SHIFTER(resourceIndex),
                                 FLEXIO_SHIFTER_MODE_DISABLED,
                                 master->sdaPin,                    /* input from SDA pin */       
                                 FLEXIO_PIN_POLARITY_HIGH,
                                 FLEXIO_PIN_CONFIG_DISABLED,
                                 CONTROL_TIMER(resourceIndex),                     /* use control timer to drive the shifter */
                                 FLEXIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCL timer */
    FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), divider);
    FLEXIO_SetTimerConfig(baseAddr, 
                              SCL_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_TIM_DIS,
                              FLEXIO_TIMER_ENABLE_TRG_HIGH,         /* enable when Tx data is available */
                              FLEXIO_TIMER_DISABLE_NEVER,
                              FLEXIO_TIMER_RESET_PIN_OUT,           /* reset if output equals pin (for clock stretching) */
                              FLEXIO_TIMER_DECREMENT_CLK_SHIFT_TMR, /* decrement on FlexIO clock */
                              FLEXIO_TIMER_INITOUT_ZERO);
    FLEXIO_SetTimerControl(baseAddr,
                               SCL_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U), /* trigger on tx shifter status flag */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               master->sclPin,                  /* output on SCL pin */
                               FLEXIO_PIN_POLARITY_HIGH,
                               FLEXIO_PIN_CONFIG_OPEN_DRAIN,    /* enable output */
                               FLEXIO_TIMER_MODE_DISABLED);

    /* Configure control timer for shifters */
    FLEXIO_SetTimerCompare(baseAddr, CONTROL_TIMER(resourceIndex), 0x000FU);
    FLEXIO_SetTimerConfig(baseAddr,
                              CONTROL_TIMER(resourceIndex),
                              FLEXIO_TIMER_START_BIT_ENABLED,
                              FLEXIO_TIMER_STOP_BIT_TIM_CMP,
                              FLEXIO_TIMER_ENABLE_TIM_ENABLE,       /* enable on SCL timer enable */
                              FLEXIO_TIMER_DISABLE_TIM_DISABLE,     /* disable on SCL timer disable */
                              FLEXIO_TIMER_RESET_NEVER,
                              FLEXIO_TIMER_DECREMENT_PIN_SHIFT_PIN, /* decrement on SCL pin input */
                              FLEXIO_TIMER_INITOUT_ONE);
    FLEXIO_SetTimerControl(baseAddr,
                               CONTROL_TIMER(resourceIndex),
                               (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U), /* trigger on tx shifter status flag */
                               FLEXIO_TRIGGER_POLARITY_LOW,
                               FLEXIO_TRIGGER_SOURCE_INTERNAL,
                               master->sclPin,                      /* use SCL pin as input */
                               FLEXIO_PIN_POLARITY_LOW,
                               FLEXIO_PIN_CONFIG_DISABLED,
                               FLEXIO_TIMER_MODE_DISABLED);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterSetBytesNo
 * Description   : configures the number of SCL clocks needed for the entire transmission
 *
 *END**************************************************************************/

static void FLEXIO_I2C_DRV_MasterSetBytesNo(FLEXIO_Type *baseAddr, flexio_i2c_master_state_t *master)
{
    uint16_t timerCmp;
    uint32_t edgeNo;        /* total number of clock edges */
    uint32_t counter;       /* edge counter per timer reload */
    uint32_t reloads;       /* number of timer reloads */
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    /* Compute number of SCL edges, including address */
    edgeNo = (master->txRemainingBytes * 18U) + 2U;
    reloads = (uint32_t)((edgeNo + 255U) / 256U);
    counter = (uint32_t)((uint32_t)(edgeNo + (reloads - 1)) / (uint32_t)reloads); /* round up */

    /* Set number of ticks in high part of timer compare register */
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0x00FFU) | (uint16_t)(((counter - 1U) & 0xFFU) << 8U));
    FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);

    /* Store reload information */
    master->eventCount =(uint16_t) reloads;
    master->lastReload = (uint8_t)(edgeNo - ((reloads - 1U) * counter) - 1U);
    /* Handle no reload case */
    if (reloads == 1U)
    {
        FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_TIM_CMP);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterSendAddress
 * Description   : send address byte
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterSendAddress(FLEXIO_Type *baseAddr, const flexio_i2c_master_state_t *master)
{
    uint8_t addrByte;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    /* Address byte: slave 7-bit address + D = 0(transmit) or 1 (receive) */
    addrByte = (uint8_t)((uint8_t)(master->slaveAddress << 1U) + (uint8_t)(master->receive ? 1U : 0U));
    FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), (uint32_t)addrByte << 24U, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_ReadData
 * Description   : Handles data reception
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_ReadData(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Read data from rx shifter */
    data = (uint8_t)FLEXIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);

    if (master->addrReceived == false)
    {
        /* This was the address byte */
        master->addrReceived = true;
        if (master->receive == true)
        {
            /* Send ACK from now on */
            FLEXIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_STOP_BIT_0);
        }
    }
    else
    {
        master->rxRemainingBytes--;
        if (master->receive == true)
        {
            /* Put data in user buffer */
            *(master->rxData) = data;
            master->rxData++;
        }
    }
    if ((master->receive == true) && (master->rxRemainingBytes == 1U))
    {
        /* Send NACK for last byte */
        FLEXIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_STOP_BIT_1);
        /* Also instruct rx shifter to expect NACK */
        FLEXIO_SetShifterStopBit(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_STOP_BIT_1);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_WriteData
 * Description   : Handles data transmission
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_WriteData(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint32_t data;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* If txRemainingBytes == 0 the transmission is over */
    if (master->txRemainingBytes == 0U) 
    {
        return;
    }

    master->txRemainingBytes--;

    if (master->txRemainingBytes == 0U)
    {
        /* Done transmitting */
        if (master->sendStop == true)
        {
            /* Transmit stop condition */
            data = 0x0;
        }
        else
        {
            /* Do not transmit stop condition */
            data = 0xFFU;
        }
    }
    else if (master->receive == true)
    {
        /* Transmit 0xFF to leave the line free while receiving */
        data = 0xFFU;
    }
    else
    {
        /* Read data from user buffer */
        data =  *(master->txData);
        master->txData++;
    }

    /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
    data <<= 24U;
    FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterEndTransfer
 * Description   : End the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterEndTransfer(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Restore Rx stop bit, in case it was changed by a receive */
    FLEXIO_SetShifterStopBit(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_STOP_BIT_0);
    /* Restore Tx stop bit, in case it was changed by a receive */
    FLEXIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_STOP_BIT_1);
    /* Clear Rx status in case there is a character left in the buffer */
    FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

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
            /* Disable interrupt for SCL timer */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), false);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Disable error interrupt for Rx shifter */
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), false);
            /* Disable interrupt for SCL timer */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), false);
            /* Stop DMA channels */
            (void)EDMA_DRV_StopChannel(master->txDMAChannel);
            (void)EDMA_DRV_StopChannel(master->rxDMAChannel);
            /* Disable FlexIO DMA requests for both shifters */
            FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    master->driverIdle = true;

    /* Signal transfer end for blocking transfers */
    if (master->blocking == true)
    {
        (void)OSIF_SemaPost(&(master->idleSemaphore));
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterEnableTransfer(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[master->flexioCommon.instance];

    /* enable timers and shifters */
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_TRANSMIT);
    FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_RECEIVE);
    FLEXIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_MODE_8BIT_BAUD);
    FLEXIO_SetTimerMode(baseAddr, CONTROL_TIMER(resourceIndex), FLEXIO_TIMER_MODE_16BIT);
    /* enable Tx pin */
    FLEXIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_PIN_CONFIG_OPEN_DRAIN);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterStopTransfer
 * Description   : Forcefully stops the current transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterStopTransfer(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    baseAddr = g_flexioBase[master->flexioCommon.instance];

    /* disable Tx pin */
    FLEXIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_PIN_CONFIG_DISABLED);
    /* disable and re-enable timers and shifters to reset them */
    FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_DISABLED);
    FLEXIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);
    FLEXIO_SetTimerMode(baseAddr, CONTROL_TIMER(resourceIndex), FLEXIO_TIMER_MODE_DISABLED);

    /* clear any leftover error flags */
    FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    FLEXIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* Clear timer status */
    FLEXIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));

    /* end the transfer */
    FLEXIO_I2C_DRV_MasterEndTransfer(master);
	
    /* Check receive overflow */
    if((master->rxRemainingBytes != 0U) && (master->status == STATUS_SUCCESS) && (master->receive == true) && (master->driverType != FLEXIO_DRIVER_TYPE_DMA))
    {
    	master->status = STATUS_I2C_RX_OVERRUN;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t FLEXIO_I2C_DRV_MasterWaitTransferEnd(flexio_i2c_master_state_t *master, uint32_t timeout)
{
    status_t osifError = STATUS_SUCCESS;

    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Wait for transfer to be completed by the IRQ */
            osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Call FLEXIO_I2C_DRV_MasterGetStatus() to do the transfer */
            while (FLEXIO_I2C_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY)
            {
            }
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Wait for transfer to be completed by the DMA IRQ */
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
        FLEXIO_I2C_DRV_MasterStopTransfer(master);
    }

    return master->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterCheckNack
 * Description   : Checks if the current Rx shifter error is NACK or RX_OVERFLOW
 *                 
 *         If there is a Tx event active it is an indication that module was not 
 *         serviced for a long time - chances are this is an overflow.
 *         It is not certain, and it is even possible to have both NACK and overflow,
 *         but there is no way to tell, so we chose the safe option (if it is an 
 *         overflow and we abort the transfer we may block the I2C bus).
 *                 
 *
 *END**************************************************************************/
static inline bool FLEXIO_I2C_DRV_MasterCheckNack(const FLEXIO_Type *baseAddr, const flexio_i2c_master_state_t *master)
{
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->flexioCommon.resourceIndex;
    return !(FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)));
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterBusBusy
 * Description   : Check status of the I2C bus.
 *                 If either SDA or SCL is low, the bus is busy.
 *
 *END**************************************************************************/
static inline bool FLEXIO_I2C_DRV_MasterBusBusy(const FLEXIO_Type *baseAddr, const flexio_i2c_master_state_t *master)
{
    uint8_t pinMask = (uint8_t)((1U << master->sdaPin) | (1U << master->sclPin));
    if ((FLEXIO_GetPinData(baseAddr) & pinMask) == pinMask)
    {
        /* both pins are high, bus is not busy */
        return false;
    }
    else
    {
        /* bus is busy */
        return true;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterCheckStatus
 * Description   : Check status of the I2C transfer. This function can be 
 *                 called either in an interrupt routine or directly in polling 
 *                 mode to advance the I2C transfer.
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterCheckStatus(void *stateStruct)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    flexio_i2c_master_state_t *master;
    uint16_t timerCmp;

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_i2c_master_state_t *)stateStruct;
    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check for errors */
    if (FLEXIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex)))
    {
        master->status = STATUS_I2C_TX_UNDERRUN;
        FLEXIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
        /* don't stop the transfer, continue processing events */
    }
    if (FLEXIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        /* Device limitation: not possible to tell the difference between NACK and receive overflow */
        if (FLEXIO_I2C_DRV_MasterCheckNack(baseAddr, master))
        {
            master->status = STATUS_I2C_RECEIVED_NACK;
            /* Force the transfer to stop */
            FLEXIO_I2C_DRV_MasterStopTransfer(master);
            /* Call callback to announce the event to the user */
            if (master->callback != NULL)
            {
                master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
            return;
        }
        else
        {
            master->status = STATUS_I2C_RX_OVERRUN;
            FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
            /* don't stop the transfer, continue processing events */
        }
    }
    /* Check if data was received */
    if (FLEXIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        FLEXIO_I2C_DRV_ReadData(master);
    }
    /* Check if transmitter needs more data */
    if (FLEXIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)))
    {
        FLEXIO_I2C_DRV_WriteData(master);
        if (master->txRemainingBytes == 0U) 
        {
            /* Done transmitting, disable Tx interrupt */
            FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        }
    }
    /* Check if the transfer is over */
    if (FLEXIO_GetTimerStatus(baseAddr, SCL_TIMER(resourceIndex)))
    {
        /* Deinit when when generate nine clock */
        if(master->driverIdle == true)
        {
            /* Clear timer status */
            FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_NEVER);
            FLEXIO_I2C_DRV_MasterStopTransfer(master);
        }
		/* General interrupt behavior */
        else
        {
            master->eventCount--;
            /* Clear timer status */
            FLEXIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));
            if (master->eventCount == 2U)
            {
                /* Adjust number of ticks in high part of timer compare register  for the last reload */
                timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
                timerCmp = (uint16_t)((uint16_t)((uint32_t)timerCmp & 0x00FFU) | (uint16_t)(((uint32_t)(master->lastReload) & 0xFFU) << 8U));
                FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
            }
            if (master->eventCount == 1U)
            {
                /* Timer will disable on the next countdown complete */
                FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_TIM_CMP);
            }

            if (master->eventCount == 0U)
            {
                FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_NEVER);

                /* Record success if there was no error */
                if (master->status == STATUS_BUSY)
                {
                    master->status = STATUS_SUCCESS;
                }
                /* End transfer. In case of race condition between Tx_shifter and timer_end events, 
                   it is possible for the clock to be restarted. So we use forced stop to avoid this. */
                FLEXIO_I2C_DRV_MasterStopTransfer(master);
                /* Call callback to announce the event to the user */
                if (master->callback != NULL)
                {
                    master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
                }
            }
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for DMA tx transfer
 *
 *END**************************************************************************/
static inline uint32_t FLEXIO_I2C_DRV_MasterComputeTxRegAddr(const flexio_i2c_master_state_t *master)
{
    uint32_t addr;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    shifter = TX_SHIFTER(master->flexioCommon.resourceIndex);
    addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - 1U);
    return addr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for DMA rx transfer
 *
 *END**************************************************************************/
static inline uint32_t FLEXIO_I2C_DRV_MasterComputeRxRegAddr(const flexio_i2c_master_state_t *master)
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
 * Function Name : FLEXIO_I2C_DRV_MasterDmaBlockImmediate
 * Description   : Configures a DMA transfer to trigger immediately
 *
 *END**************************************************************************/
static inline void FLEXIO_I2C_DRV_MasterDmaBlockImmediate(edma_software_tcd_t *stcdBase, uint8_t blockNo)
{
    /* Set the START bit for this TCD to 1 */
    stcdBase[blockNo].CSR |= (uint16_t)DMA_TCD_CSR_START(1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterDmaBlockTerminate
 * Description   : Configures a DMA transfer to disable the DMArequuest upon transfer completion
 *
 *END**************************************************************************/
static inline void FLEXIO_I2C_DRV_MasterDmaBlockTerminate(edma_software_tcd_t *stcdBase, uint8_t blockNo)
{
    /* Set the DREQ bit for this TCD to 1 */
    stcdBase[blockNo].CSR |= (uint16_t)DMA_TCD_CSR_DREQ(1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterDmaConfigTx
 * Description   : Configures DMA transfer for Tx
 *
 *END**************************************************************************/
static inline void FLEXIO_I2C_DRV_MasterDmaConfigTx(flexio_i2c_master_state_t *master, 
                                                        edma_software_tcd_t *stcdBase)
{
    edma_scatter_gather_list_t srcList[2U];
    edma_scatter_gather_list_t destList[2U];
    uint32_t addr;

    /* Configure Tx chain */
    /* First block: transmit data */
    if (master->receive == false)
    {
        addr = (uint32_t)(master->txData);
    }
    else
    {
        /* if receiving, send 0xFF to keep the line clear */
        master->dummyDmaIdle = 0xFFU;
        addr = (uint32_t)(&(master->dummyDmaIdle));
    }
    srcList[0U].address = addr;
    srcList[0U].length = master->rxRemainingBytes;
    srcList[0U].type = EDMA_TRANSFER_MEM2PERIPH;
    destList[0U].address = FLEXIO_I2C_DRV_MasterComputeTxRegAddr(master);
    destList[0U].length = master->rxRemainingBytes;
    destList[0U].type = EDMA_TRANSFER_MEM2PERIPH;

    /* Second block: transmit stop/repeated start */
    if (master->sendStop)
    {
        master->dummyDmaStop = 0U;
    }
    else
    {
        master->dummyDmaStop = 0xFFU;
    }
    srcList[1U].address = (uint32_t)(&(master->dummyDmaStop));
    srcList[1U].length = 1U;
    srcList[1U].type = EDMA_TRANSFER_MEM2PERIPH;
    destList[1U].address = FLEXIO_I2C_DRV_MasterComputeTxRegAddr(master);
    destList[1U].length = 1U;
    destList[1U].type = EDMA_TRANSFER_MEM2PERIPH;

    /* use 2 STCDs for tx, transfer size: 1 byte, 1 byte per DMA request */
    (void)EDMA_DRV_ConfigScatterGatherTransfer(master->txDMAChannel, stcdBase, EDMA_TRANSFER_SIZE_1B, 
                                               1U, srcList, destList, 2U);

    if (master->receive == true)
    {
        /* if there is no data to transmit, don't increment source offset */
        EDMA_DRV_SetSrcOffset(master->txDMAChannel, 0);
    }
    /* set DREQ bit for last block */
    FLEXIO_I2C_DRV_MasterDmaBlockTerminate(stcdBase, 0U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterDmaConfigRx
 * Description   : Configures DMA transfer for Rx
 *
 *END**************************************************************************/
static inline void FLEXIO_I2C_DRV_MasterDmaConfigRx(flexio_i2c_master_state_t *master, 
                                                    edma_software_tcd_t *stcdBase)
{
    edma_scatter_gather_list_t srcList[6U];
    edma_scatter_gather_list_t destList[6U];
    uint32_t tmp;
    uint8_t blockCnt = 0U;
    const FLEXIO_Type *baseAddr;
    uint8_t shifter;
    uint8_t dmaChn;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    dmaChn = master->rxDMAChannel;

    /* Configure Rx chain */
    if (master->receive == false)
    {
        /* when transmitting we don't need scatter-gather for receive, just move read data to dummy location */
        (void)EDMA_DRV_ConfigMultiBlockTransfer(dmaChn, 
                                                EDMA_TRANSFER_PERIPH2MEM, 
                                                FLEXIO_I2C_DRV_MasterComputeRxRegAddr(master), 
                                                (uint32_t)(&(master->dummyDmaReceive)),
                                                EDMA_TRANSFER_SIZE_1B, 
                                                1U, master->rxRemainingBytes + 1U, true);
        /* no data to receive, don't increment destination offset */
        EDMA_DRV_SetDestOffset(dmaChn, 0);
    }
    else
    {
        /* First block: receive address byte (dummy read) */
        srcList[blockCnt].address = FLEXIO_I2C_DRV_MasterComputeRxRegAddr(master);
        srcList[blockCnt].length = 1U;
        srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(master->dummyDmaReceive));
        destList[blockCnt].length = 1U;
        destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        blockCnt++;
        /* When receiving just 1 byte skip the "middle" part */
        if (master->rxRemainingBytes > 1U)
        {
            /* Second block: set tx shifter stop bit to 0 (transmit ACK) */
            shifter = TX_SHIFTER(master->flexioCommon.resourceIndex);
            tmp = baseAddr->SHIFTCFG[shifter];
            tmp &= ~(FLEXIO_SHIFTCFG_SSTOP_MASK);
            tmp |= FLEXIO_SHIFTCFG_SSTOP(FLEXIO_SHIFTER_STOP_BIT_0);
            master->dmaReceiveTxStop0 = (uint8_t)(tmp & 0xFFU);
            srcList[blockCnt].address = (uint32_t)(&(master->dmaReceiveTxStop0));
            srcList[blockCnt].length = 1U;
            srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
            destList[blockCnt].length = 1U;
            destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            blockCnt++;
            /* Third block: receive all but the last data byte */
            srcList[blockCnt].address = FLEXIO_I2C_DRV_MasterComputeRxRegAddr(master);
            srcList[blockCnt].length = master->rxRemainingBytes - 1U;
            srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(master->rxData);
            destList[blockCnt].length = master->rxRemainingBytes - 1U;
            destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            blockCnt++;
            /* Fourth block: set tx shifter stop bit to 1 (transmit NACK for last byte) */
            tmp = baseAddr->SHIFTCFG[shifter];
            tmp &= ~(FLEXIO_SHIFTCFG_SSTOP_MASK);
            tmp |= FLEXIO_SHIFTCFG_SSTOP(FLEXIO_SHIFTER_STOP_BIT_1);
            master->dmaReceiveTxStop1 = (uint8_t)(tmp & 0xFFU);
            srcList[blockCnt].address = (uint32_t)(&(master->dmaReceiveTxStop1));
            srcList[blockCnt].length = 1U;
            srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
            destList[blockCnt].length = 1U;
            destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
            blockCnt++;
        }
        /* Fifth block: set rx shifter stop bit to 1 (expect NACK) */
        shifter = RX_SHIFTER(master->flexioCommon.resourceIndex);
        tmp = baseAddr->SHIFTCFG[shifter];
        tmp &= ~(FLEXIO_SHIFTCFG_SSTOP_MASK);
        tmp |= FLEXIO_SHIFTCFG_SSTOP(FLEXIO_SHIFTER_STOP_BIT_1);
        master->dmaReceiveRxStop1 = (uint8_t)(tmp & 0xFFU);
        srcList[blockCnt].address = (uint32_t)(&(master->dmaReceiveRxStop1));
        srcList[blockCnt].length = 1U;
        srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
        destList[blockCnt].length = 1U;
        destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        blockCnt++;
        /* Sixth block: receive last byte */
        srcList[blockCnt].address = FLEXIO_I2C_DRV_MasterComputeRxRegAddr(master);
        srcList[blockCnt].length = 1U;
        srcList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(master->rxData[master->rxRemainingBytes - 1U]));
        destList[blockCnt].length = 1U;
        destList[blockCnt].type = EDMA_TRANSFER_PERIPH2MEM;
        blockCnt++;

        /* use blockCnt (3 or 6) STCDs for rx, transfer size: 1 byte, 1 byte per DMA request */
        (void)EDMA_DRV_ConfigScatterGatherTransfer(dmaChn, 
                                                   stcdBase, 
                                                   EDMA_TRANSFER_SIZE_1B, 1U, srcList, destList, blockCnt);

        /* set all config transfers to SHIFTCFG registers to trigger immediately */
        FLEXIO_I2C_DRV_MasterDmaBlockImmediate(stcdBase, 0U);
        if (master->rxRemainingBytes > 1U)
        {
            FLEXIO_I2C_DRV_MasterDmaBlockImmediate(stcdBase, 2U);
            FLEXIO_I2C_DRV_MasterDmaBlockImmediate(stcdBase, 3U);
        }
        /* set DREQ bit for last block */
        FLEXIO_I2C_DRV_MasterDmaBlockTerminate(stcdBase, (uint8_t)(blockCnt - 2U));
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterStartDmaTransfer
 * Description   : Starts a DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterStartDmaTransfer(flexio_i2c_master_state_t *master)
{
    uint32_t alignedStcd;
    edma_software_tcd_t *stcdBase;

    /* get aligned address to use for software TCDs */
    alignedStcd = STCD_ADDR(master->stcd);
    stcdBase = (edma_software_tcd_t *)(alignedStcd);

    /* Configure Tx and Rx chains */
    FLEXIO_I2C_DRV_MasterDmaConfigTx(master, stcdBase);
    FLEXIO_I2C_DRV_MasterDmaConfigRx(master, &stcdBase[FLEXIO_I2C_DMA_TX_CHAIN_LENGTH]);

    /* Start both DMA channels */
    (void)EDMA_DRV_StartChannel(master->txDMAChannel);
    (void)EDMA_DRV_StartChannel(master->rxDMAChannel);
}



/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterEndDmaTransfer
 * Description   : Starts a DMA transfer
 *
 *END**************************************************************************/
static void FLEXIO_I2C_DRV_MasterEndDmaTransfer(void *stateStruct)
{
    flexio_i2c_master_state_t *master;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    FLEXIO_Type *baseAddr;
    uint16_t timerCmp;

    DEV_ASSERT(stateStruct != NULL);

    master = (flexio_i2c_master_state_t *)stateStruct;
    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check for DMA transfer errors */
    if ((EDMA_DRV_GetChannelStatus(master->txDMAChannel) == EDMA_CHN_ERROR) ||
        (EDMA_DRV_GetChannelStatus(master->rxDMAChannel) == EDMA_CHN_ERROR))
    {
        master->status = STATUS_ERROR;
        /* Force the transfer to stop */
        FLEXIO_I2C_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check for NACK */
    if (FLEXIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex)))
    {
        FLEXIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
        master->status = STATUS_I2C_RECEIVED_NACK;
        /* Force the transfer to stop */
        FLEXIO_I2C_DRV_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL)
        {
            master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check if the transfer is over */
    if (FLEXIO_GetTimerStatus(baseAddr, SCL_TIMER(resourceIndex)))
    {
        master->eventCount--;
        /* Clear timer status */
        FLEXIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));
        if (master->eventCount == 2U)
        {
            /* Adjust number of ticks in high part of timer compare register  for the last reload */
            timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
            timerCmp = (uint16_t)((uint32_t)timerCmp & 0x00FFU) | (uint16_t)(((uint32_t)(master->lastReload) & 0xFFU) << 8U);
            FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
        }
        if (master->eventCount == 1U)
        {
            /* Timer will disable on the next countdown complete */
            FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_TIM_CMP);
        }

        if (master->eventCount == 0U)
        {
            FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_NEVER);
            /* Record success if there was no error */
            if (master->status == STATUS_BUSY)
            {
                master->status = STATUS_SUCCESS;
            }
            /* End transfer */
            FLEXIO_I2C_DRV_MasterStopTransfer(master);
            /* Call callback to announce the event to the user */
            if (master->callback != NULL)
            {
                master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterStartTransfer
 * Description   : Perform a send or receive transaction on the I2C bus
 *
 *END**************************************************************************/
static status_t FLEXIO_I2C_DRV_MasterStartTransfer(flexio_i2c_master_state_t *master,
                                                              uint32_t size,
                                                              bool sendStop,
                                                              bool receive)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */

    DEV_ASSERT(master != NULL);

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check if bus is busy */
    if (FLEXIO_I2C_DRV_MasterBusBusy(baseAddr, master))
    {
        return STATUS_I2C_BUS_BUSY;
    }

    /* Tx - one extra byte for stop condition */
    master->txRemainingBytes = size + 1U;
    master->rxRemainingBytes = size;
    master->status = STATUS_BUSY;
    master->driverIdle = false;
    master->sendStop = sendStop;
    master->receive = receive;
    master->addrReceived = false;

    /* Configure device for requested number of bytes, keeping the existing baud rate */
    FLEXIO_I2C_DRV_MasterSetBytesNo(baseAddr, master);
    /* Enable timers and shifters */
    FLEXIO_I2C_DRV_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            /* Send address to start transfer */
            FLEXIO_I2C_DRV_MasterSendAddress(baseAddr, master);
            /* Enable interrupt for Tx and Rx shifters */
            FLEXIO_SetShifterInterrupt(baseAddr, 
                                 (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                 true);
            FLEXIO_SetShifterErrorInterrupt(baseAddr, 
                                 (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))),
                                 true);
            /* Enable interrupt for SCL timer */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Send address to start transfer */
            FLEXIO_I2C_DRV_MasterSendAddress(baseAddr, master);
            /* Nothing to do here, FLEXIO_I2C_DRV_MasterGetStatus() will handle the transfer */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            FLEXIO_I2C_DRV_MasterStartDmaTransfer(master);
            /* Enable error interrupt for Rx shifter - for NACK detection */
            FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
            /* Enable interrupt for SCL timer - for end of transfer detection */
            FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);
            /*Disable system interrupt*/
            INT_SYS_DisableIRQGlobal();
            /* Send address to start transfer */
            FLEXIO_I2C_DRV_MasterSendAddress(baseAddr, master);
            /* Enable FlexIO DMA requests for both shifters */
            FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
            /*Enable system interrupt*/
            INT_SYS_EnableIRQGlobal();
            break;
        default: 
            /* Impossible type - do nothing */
            break;
    }

    return STATUS_SUCCESS;
}

/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterInit
 * Description   : Initialize the FLEXIO_I2C master mode driver
 * Implements : FLEXIO_I2C_DRV_MasterInit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterInit(uint32_t instance,
                                          const flexio_i2c_master_user_config_t * userConfigPtr,
                                          flexio_i2c_master_state_t * master)
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
    master->slaveAddress = userConfigPtr->slaveAddress;
    master->sdaPin = userConfigPtr->sdaPin;
    master->sclPin = userConfigPtr->sclPin;
    master->callback = userConfigPtr->callback;
    master->callbackParam = userConfigPtr->callbackParam;
    master->blocking = false;
    master->driverIdle = true;
    master->status = STATUS_SUCCESS;

    /* Configure device for I2C mode */
    FLEXIO_I2C_DRV_MasterConfigure(master, inputClock, userConfigPtr->baudRate);

    /* Set up transfer engine */
    switch (master->driverType)
    {
        case FLEXIO_DRIVER_TYPE_INTERRUPTS:
            master->flexioCommon.isr = FLEXIO_I2C_DRV_MasterCheckStatus;
            break;
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* Nothing to do here, FLEXIO_I2C_DRV_MasterGetStatus() will handle the transfer */
            break;
        case FLEXIO_DRIVER_TYPE_DMA:
            /* Store DMA channel numbers */
            master->txDMAChannel = userConfigPtr->txDMAChannel;
            master->rxDMAChannel = userConfigPtr->rxDMAChannel;
            /* Configure DMA request sources */
            dmaReqTx = g_flexioDMASrc[instance][TX_SHIFTER(master->flexioCommon.resourceIndex)];
            dmaReqRx = g_flexioDMASrc[instance][RX_SHIFTER(master->flexioCommon.resourceIndex)];
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
            (void)EDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
            /* For DMA transfers we use timer interrupts to signal transfer end */
            master->flexioCommon.isr = FLEXIO_I2C_DRV_MasterEndDmaTransfer;
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
 * Function Name : FLEXIO_I2C_DRV_MasterDeinit
 * Description   : De-initialize the FLEXIO_I2C master mode driver
 * Implements : FLEXIO_I2C_DRV_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterDeinit(flexio_i2c_master_state_t *master)
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
 * Function Name : FLEXIO_I2C_DRV_MasterSetBaudRate
 * Description   : Set the baud rate for any subsequent I2C communication
 * Implements : FLEXIO_I2C_DRV_MasterSetBaudRate_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterSetBaudRate(flexio_i2c_master_state_t *master, uint32_t baudRate)
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
    FLEXIO_I2C_DRV_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure timer divider in the lower 8 bits of TIMCMP[CMP] */
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0xFF00U) | divider);
    FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);

    (void)clkErr;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 * Implements : FLEXIO_I2C_DRV_MasterGetBaudRate_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterGetBaudRate(flexio_i2c_master_state_t *master, uint32_t *baudRate)
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
    timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    divider = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 2)). Round to nearest integer */
    *baudRate = (inputClock + divider + 2U) / (2U * ((uint32_t)divider + 2U));

    (void)clkErr;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterSetSlaveAddr
 * Description   : Set the slave address for any subsequent I2C communication
 * Implements : FLEXIO_I2C_DRV_MasterSetSlaveAddr_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterSetSlaveAddr(flexio_i2c_master_state_t *master, uint16_t address)
{
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    master->slaveAddress = address;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterSendData
 * Description   : Perform a non-blocking send transaction on the I2C bus
 * Implements : FLEXIO_I2C_DRV_MasterSendData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterSendData(flexio_i2c_master_state_t *master,
                                              const uint8_t * txBuff,
                                              uint32_t txSize,
                                              bool sendStop)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Initialize transfer data */
    master->txData = txBuff;
    /* Start the transfer */
    return FLEXIO_I2C_DRV_MasterStartTransfer(master, txSize, sendStop, false);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2C bus
 * Implements : FLEXIO_I2C_DRV_MasterSendDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterSendDataBlocking(flexio_i2c_master_state_t *master,
                                                          const uint8_t * txBuff,
                                                          uint32_t txSize,
                                                          bool sendStop,
                                                          uint32_t timeout)
{
    status_t status;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking */
    if (master->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OSIF_SemaWait(&(master->idleSemaphore), 0);
    }
    /* Initialize transfer data */
    master->txData = txBuff;
    /* Start the transfer */
    status = FLEXIO_I2C_DRV_MasterStartTransfer(master, txSize, sendStop, false);
    if (status != STATUS_SUCCESS)
    {
        /* Transfer could not be started */
        master->blocking = false;
        return status; 
    }

    /* Wait for transfer to end */
    return FLEXIO_I2C_DRV_MasterWaitTransferEnd(master, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2C bus
 * Implements : FLEXIO_I2C_DRV_MasterReceiveData_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterReceiveData(flexio_i2c_master_state_t *master,
                                                     uint8_t * rxBuff,
                                                     uint32_t rxSize,
                                                     bool sendStop)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Initialize transfer data */
    master->rxData = rxBuff;
    /* Start the transfer */
    return FLEXIO_I2C_DRV_MasterStartTransfer(master, rxSize, sendStop, true);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2C bus
 * Implements : FLEXIO_I2C_DRV_MasterReceiveDataBlocking_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterReceiveDataBlocking(flexio_i2c_master_state_t *master,
                                                        uint8_t * rxBuff,
                                                        uint32_t rxSize,
                                                        bool sendStop,
                                                        uint32_t timeout)
{
    status_t status;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle)
    {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking */
    if (master->driverType != FLEXIO_DRIVER_TYPE_POLLING)
    {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OSIF_SemaWait(&(master->idleSemaphore), 0);
    }
    /* Initialize transfer data */
    master->rxData = rxBuff;
    /* Start the transfer */
    status = FLEXIO_I2C_DRV_MasterStartTransfer(master, rxSize, sendStop, true);
    if (status != STATUS_SUCCESS)
    {
        /* Transfer could not be started */
        master->blocking = false;
        return status; 
    }

    /* Wait for transfer to end */
    return FLEXIO_I2C_DRV_MasterWaitTransferEnd(master, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking I2C master transaction
 * Implements : FLEXIO_I2C_DRV_MasterTransferAbort_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterTransferAbort(flexio_i2c_master_state_t *master)
{
    DEV_ASSERT(master != NULL);

    /* Warning: an ongoing transfer can't be aborted safely due to device limitation; 
       there is no way to know the exact stage of the transfer, and if we disable the module 
       during the ACK bit (transmit) or during a 0 data bit (receive) the slave will hold 
       the SDA line low forever and block the I2C bus. NACK reception is the only exception, 
       as there is no slave to hold the line low. Therefore this function should only be used 
       in extreme circumstances, and the application must have a way to reset the I2C slave
    */
    
    /* Check if driver is busy */
    if (master->driverIdle)
    {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_I2C_ABORTED;
    FLEXIO_I2C_DRV_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking I2C master transaction
 * Implements : FLEXIO_I2C_DRV_MasterGetStatus_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_MasterGetStatus(flexio_i2c_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    DEV_ASSERT(master != NULL);

    /* Use rxRemainingBytes even for transmit; byte is not transmitted
       until rx shifter confirms the ACK */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle)
    {
        switch(master->driverType)
        {
        case FLEXIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the I2C transfer here */
            FLEXIO_I2C_DRV_MasterCheckStatus(master);
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
 * Function Name : FLEXIO_I2C_DRV_GetDefaultConfig
 * Description   : Returns default configuration structure for FLEXIO_I2C
 * Implements : FLEXIO_I2C_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
void FLEXIO_I2C_DRV_GetDefaultConfig(flexio_i2c_master_user_config_t * userConfigPtr)
{
	DEV_ASSERT(userConfigPtr != NULL);

    userConfigPtr->slaveAddress = 32U;
    userConfigPtr->driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate = 100000U;
    userConfigPtr->sdaPin = 0U;
    userConfigPtr->sclPin = 1U;
    userConfigPtr->callback = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel = 255U;
    userConfigPtr->txDMAChannel = 255U;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_GenerateNineClock
 * Description   : Generate nine clock on SCL line to free SDA line
 * Implements : FLEXIO_I2C_DRV_GenerateNineClock_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_GenerateNineClock(flexio_i2c_master_state_t *master)
{
  FLEXIO_Type *baseAddr;
  uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
  uint16_t timerCmp;
  status_t status;

  DEV_ASSERT(master != NULL);

  baseAddr = g_flexioBase[master->flexioCommon.instance];
  resourceIndex = master->flexioCommon.resourceIndex;

  if (master->driverIdle == true)
  {
      /* Set number of ticks in high part of timer compare register */
      timerCmp = FLEXIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
      /* Set ticks in high part of timer compare register to generated nine clock*/
      timerCmp = (uint16_t)((timerCmp & 0x00FFU) | (uint16_t)(((16U) & 0xFFU) << 8U));
      FLEXIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
      /* Disable timer on Timer compare */
      FLEXIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_DISABLE_TIM_CMP);
      FLEXIO_SetTimerStop(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_STOP_BIT_DISABLED);
      /* Enable timers and shifters */
      FLEXIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), FLEXIO_SHIFTER_MODE_TRANSMIT);
      FLEXIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), FLEXIO_TIMER_MODE_8BIT_BAUD);
      /* Enable Interrupt SLC timer */
      FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);

      /* Write any value to triger timer */
      FLEXIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), (uint32_t)0x00U, FLEXIO_SHIFTER_RW_MODE_BIT_SWAP);

      status = STATUS_SUCCESS;
  }
  else
  {
	  status = STATUS_BUSY;
  }

  return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_StatusGenerateNineClock
 * Description   : Check status of SCL timer be disable or not.
 * Implements : FLEXIO_I2C_DRV_StatusGenerateNineClock_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_I2C_DRV_StatusGenerateNineClock(flexio_i2c_master_state_t *master)
{
    FLEXIO_Type *baseAddr;
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
    uint32_t interruptMask;
    uint32_t tmp;
    status_t status;

    baseAddr = g_flexioBase[master->flexioCommon.instance];
    resourceIndex = master->flexioCommon.resourceIndex;

    /* Check Enable SCL timer */
    interruptMask = (1UL << (uint32_t)SCL_TIMER(resourceIndex));
    tmp = FLEXIO_GetAllTimerInterrupt(baseAddr);
    if((tmp & interruptMask) != 0U)
    {
        /* The generation nine clock have not done yet*/
		status = STATUS_BUSY;
    }
    else
    {
        /* The generation nine clock is done*/
        status = STATUS_SUCCESS;
    }
	
	return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_I2C_DRV_GetBusStatus
 * Description   : Check status of the SDA line.
 *                 If either SDA or SCL is low, the bus is busy.
 * Implements : FLEXIO_I2C_DRV_GetBusStatus_Activity
 *
 *END**************************************************************************/
bool FLEXIO_I2C_DRV_GetBusStatus(const flexio_i2c_master_state_t *master, bool sdaLine)
{

    FLEXIO_Type *baseAddr;
    uint8_t pinMask;

    baseAddr = g_flexioBase[master->flexioCommon.instance];

	/* Select Pin */
    if(sdaLine)
    {
        /* Select SDA line */
        pinMask = (uint8_t)((1U << master->sdaPin));
    }
    else
    {
        /* Select SCL line */
        pinMask = (uint8_t)((1U << master->sclPin));
    }

    /* Check Pin */
    if((FLEXIO_GetPinData(baseAddr) & pinMask) == pinMask)
    {
        /* SDA/SCL line be high */
        return true;
    }
    else
    {
        /* SDA/SCL line be low */
        return false;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
