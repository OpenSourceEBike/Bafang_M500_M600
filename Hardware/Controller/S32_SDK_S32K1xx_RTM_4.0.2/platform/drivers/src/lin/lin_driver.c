/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/*!
 * @file lin_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lin_driver.h"
#if (LPUART_INSTANCE_COUNT > 0U)
    #include "lin_lpuart_driver.h"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_Init
 * Description   : This function initializes a LIN Hardware Interface for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers, ungate the clock to LIN Hardware Interface, initialize the
 * module to user defined settings and default settings, configure the IRQ state
 * structure and enable the module-level interrupt to the core, and enable the
 * LIN Hardware Interface transmitter and receiver.
 * The following is an example of how to set up the lin_state_t and the
 * lin_user_config_t parameters and how to call the LIN_DRV_Init function
 * by passing in these parameters:
 *    lin_user_config_t linUserConfig
 *    linUserConfig.baudRate = 9600
 *    linUserConfig.nodeFunction = SLAVE
 *    linUserConfig.autobaudEnable = true
 *    linUserConfig.timerGetTimeIntervalCallback = (lin_timer_get_time_interval_t) l_ifc_timerGetTimeIntervalCallbackHandler
 *    lin_state_t linState
 *    LIN_DRV_Init(instance, (lin_user_config_t *) &linUserConfig, (lin_state_t *) &linState)
 *
 * Implements    : LIN_DRV_Init_Activity
 *END**************************************************************************/
status_t LIN_DRV_Init(uint32_t instance,
                      lin_user_config_t * linUserConfig,
                      lin_state_t * linCurrentState)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_Init(instance, linUserConfig, linCurrentState);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_Deinit
 * Description   : This function shuts down the LIN Hardware Interface by disabling interrupts and
 *                 transmitter/receiver.
 *
 * Implements    : LIN_DRV_Deinit_Activity
 *END**************************************************************************/
void LIN_DRV_Deinit(uint32_t instance)
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART_DRV_Deinit(instance);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetDefaultConfig
 * Description   : This function initializes a configuration structure received
 * from the application with default values.
 *
 * Implements    : LIN_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void LIN_DRV_GetDefaultConfig(bool isMaster,
                              lin_user_config_t * linUserConfig)
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART_DRV_GetDefaultConfig(isMaster, linUserConfig);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_InstallCallback
 * Description   : This function installs the callback function that is used for LIN_DRV_IRQHandler.
 * Pass in Null pointer as callback will uninstall.
 *
 * Implements    : LIN_DRV_InstallCallback_Activity
 *END**************************************************************************/
lin_callback_t LIN_DRV_InstallCallback(uint32_t instance,
                                       lin_callback_t function)
{
    lin_callback_t retVal = NULL;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_InstallCallback(instance, function);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendFrameDataBlocking
 * Description   : This function sends data out through the LIN Hardware Interface using
 * blocking method. This function will calculate the checksum byte and send it with the
 * frame data. Blocking means that the function does not return until the transmission is
 * complete. This function checks if txSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also checks if the isBusBusy
 * is false, if not it will return STATUS_BUSY. The function does not return
 * until the transmission is complete. If the transmission is successful, it
 * will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 * Implements    : LIN_DRV_SendFrameDataBlocking_Activity
 *END**************************************************************************/
status_t LIN_DRV_SendFrameDataBlocking(uint32_t instance,
                                       const uint8_t * txBuff,
                                       uint8_t txSize,
                                       uint32_t timeoutMSec)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_SendFrameDataBlocking(instance, txBuff, txSize, timeoutMSec);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendFrameData
 * Description   : This function sends data out through the LIN Hardware Interface using
 * non-blocking method. This function will calculate the checksum byte and send it with the
 * frame data. The function will return immediately after calling this function. If txSize
 * is equal to 0 or greater than 8 then the function will return STATUS_ERROR. If isBusBusy is
 * currently true then the function will return STATUS_BUSY.
 *
 * Implements    : LIN_DRV_SendFrameData_Activity
 *END**************************************************************************/
status_t LIN_DRV_SendFrameData(uint32_t instance,
                               const uint8_t * txBuff,
                               uint8_t txSize)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_SendFrameData(instance, txBuff, txSize);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous transmission has
 * finished. When performing non-blocking transmit, the user can call this
 * function to ascertain the state of the current transmission:
 * in progress (or busy that STATUS_BUSY) or timeout (STATUS_TIMEOUT) or complete (success that is STATUS_SUCCESS).
 * In addition, if the transmission is still in progress, the user can obtain the number
 * of bytes that still needed to transmit.
 *
 * Implements    : LIN_DRV_GetTransmitStatus_Activity
 *END**************************************************************************/
status_t LIN_DRV_GetTransmitStatus(uint32_t instance,
                                   uint8_t * bytesRemaining)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_GetTransmitStatus(instance, bytesRemaining);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_ReceiveFrameDataBlocking
 * Description   : This function receives data from LIN Hardware Interface module using blocking
 * method. This function receives data from LPUART module using blocking
 * method, the function does not return until the receive is complete. The interrupt
 * handler LIN_LPUART_DRV_IRQHandler will check the checksum byte. If the checksum
 * is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 * also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 * This function also checks if the isBusBusy is false, if not it will return
 * STATUS_BUSY.
 *
 * Implements    : LIN_DRV_ReceiveFrameDataBlocking_Activity
 *END**************************************************************************/
status_t LIN_DRV_ReceiveFrameDataBlocking(uint32_t instance,
                                          uint8_t * rxBuff,
                                          uint8_t rxSize,
                                          uint32_t timeoutMSec)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_RecvFrmDataBlocking(instance, rxBuff, rxSize, timeoutMSec);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_ReceiveFrameData
 * Description   : This function receives data from LIN Hardware Interface using
 * non-blocking method. This function will check the checksum byte. If the checksum is
 * correct, it will receive it with the frame data.
 * Non-blocking  means that the function returns immediately.
 * The application has to get the receive status to know when the reception is complete.
 * The interrupt handler LIN_LPUART_DRV_IRQHandler will check the checksum byte. If the
 * checksum is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function also
 * check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR. This function
 * also checks if the isBusBusy is false, if not it will return STATUS_BUSY.
 *
 * Implements    : LIN_DRV_ReceiveFrameData_Activity
 *END**************************************************************************/
status_t LIN_DRV_ReceiveFrameData(uint32_t instance,
                                  uint8_t * rxBuff,
                                  uint8_t rxSize)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_RecvFrmData(instance, rxBuff, rxSize);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_AbortTransferData
 * Description   : Aborts an on-going non-blocking transmission/reception.
 * While performing a non-blocking transferring data, users can call this
 * function to terminate immediately the transferring.
 *
 * Implements    : LIN_DRV_AbortTransferData_Activity
 *END**************************************************************************/
status_t LIN_DRV_AbortTransferData(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_AbortTransferData(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetReceiveStatus
 * Description   : This function returns whether the data reception is complete.
 * When performing non-blocking transmit, the user can call this function to
 * ascertain the state of the current receive progress:
 * in progress (STATUS_BUSY) or timeout (STATUS_TIMEOUT) or complete (STATUS_SUCCESS).
 * In addition, if the reception is still in progress, the user can obtain the
 * number of bytes that still needed to receive.
 *
 * Implements    : LIN_DRV_GetReceiveStatus_Activity
 *END**************************************************************************/
status_t LIN_DRV_GetReceiveStatus(uint32_t instance,
                                  uint8_t * bytesRemaining)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_GetReceiveStatus(instance, bytesRemaining);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GoToSleepMode
 * Description   : This function puts current LIN node to sleep mode.
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE.
 *
 * Implements    : LIN_DRV_GoToSleepMode_Activity
 *END**************************************************************************/
status_t LIN_DRV_GoToSleepMode(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_GoToSleepMode(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GotoIdleState
 * Description   : This function puts current node to Idle state.
 *
 * Implements    : LIN_DRV_GoToIdleState_Activity
 *END**************************************************************************/
status_t LIN_DRV_GotoIdleState(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_GotoIdleState(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendWakeupSignal
 * Description   : This function sends a wakeup signal through the LPUART interface.
 *
 * Implements    : LIN_DRV_SendWakeupSignal_Activity
 *END**************************************************************************/
status_t LIN_DRV_SendWakeupSignal(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_SendWakeupSignal(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetCurrentNodeState
 * Description   : This function gets the current LIN node state.
 *
 * Implements    : LIN_DRV_GetCurrentNodeState_Activity
 *END**************************************************************************/
lin_node_state_t LIN_DRV_GetCurrentNodeState(uint32_t instance)
{
    lin_node_state_t retVal = LIN_NODE_STATE_UNINIT;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_GetCurrentNodeState(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_TimeoutService
 * Description   : This is callback function for Timer Interrupt Handler.
 * Users shall initialize a timer (for example FTM) in Output compare mode
 * with period of 500 micro seconds. In timer IRQ handler, call this function.
 *
 * Implements    : LIN_DRV_TimeoutService_Activity
 *END**************************************************************************/
void LIN_DRV_TimeoutService(uint32_t instance)
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART_DRV_TimeoutService(instance);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SetTimeoutCounter
 * Description   : This function sets value for timeout counter that is used in
 * LIN_DRV_TimeoutService
 *
 * Implements    : LIN_DRV_SetTimeoutCounter_Activity
 *END**************************************************************************/
void LIN_DRV_SetTimeoutCounter(uint32_t instance,
                               uint32_t timeoutValue)
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART_DRV_SetTimeoutCounter(instance, timeoutValue);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_MasterSendHeader
 * Description   : This function sends frame header out through the LIN Hardware Interface
 * using a non-blocking method. Non-blocking  means that the function returns
 * immediately. This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR.
 *
 * Implements    : LIN_DRV_MasterSendHeader_Activity
 *END**************************************************************************/
status_t LIN_DRV_MasterSendHeader(uint32_t instance,
                                  uint8_t id)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_MasterSendHeader(instance, id);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_EnableIRQ
 * Description   : This function enables LIN hardware interrupts.
 *
 * Implements    : LIN_DRV_EnableIRQ_Activity
 *END**************************************************************************/
status_t LIN_DRV_EnableIRQ(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_EnableIRQ(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_DisableIRQ
 * Description   : This function disables LIN hardware interrupts.
 *
 * Implements    : LIN_DRV_DisableIRQ_Activity
 *END**************************************************************************/
status_t LIN_DRV_DisableIRQ(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_DisableIRQ(instance);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_IRQHandler
 * Description   : Interrupt handler for LIN Hardware Interface.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 * Implements    : LIN_DRV_IRQHandler_Activity
 *END**************************************************************************/
void LIN_DRV_IRQHandler(uint32_t instance)
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART_DRV_IRQHandler(instance);
#endif
}

 /*FUNCTION**********************************************************************
  *
  * Function Name : LIN_DRV_AutoBaudCapture
  * Description   : This function capture bits time to detect break char, calculate
  * baudrate from sync bits and enable transceiver if autobaud successful.
  * This function should only be used in Slave.
  * The timer should be in mode input capture of both rising and falling edges.
  * The timer input capture pin should be externally connected to RXD pin.
  *
  * Implements    : LIN_DRV_AutoBaudCapture_Activity
  *END**************************************************************************/
status_t LIN_DRV_AutoBaudCapture(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

#if (LPUART_INSTANCE_COUNT > 0U)
    retVal = LIN_LPUART_DRV_AutoBaudCapture(instance);
#endif

    return retVal;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
