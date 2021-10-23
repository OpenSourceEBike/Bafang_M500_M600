/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
** * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

/*!
 * @file lin_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declarations
 * This exception is being added for MASTER and SLAVE defines that are not used by LIN Stack
 * code.
 */

#ifndef LIN_DRIVER_H
#define LIN_DRIVER_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "status.h"
#include "interrupt_manager.h"
#include "osif.h"

/*!
 * @addtogroup lin_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SLAVE  0U
#define MASTER 1U
#define MAKE_PARITY 0U
#define CHECK_PARITY 1U

/* Table to save ISR pointers for LPUART instances */
extern isr_t g_linLpuartIsrs[LPUART_INSTANCE_COUNT];

#ifdef LPUART_ERR_IRQS
/* Table to save ERR ISR pointers for LPUART instances */
extern isr_t g_linLpuartErrIsrs[LPUART_INSTANCE_COUNT];
#endif

/*!
 * @brief Callback function to get time interval in nanoseconds
 * Implements : lin_timer_get_time_interval_t_Class
 */
typedef uint32_t (* lin_timer_get_time_interval_t) (uint32_t *nanoSeconds);

/*!
 * @brief LIN hardware configuration structure
 * Implements : lin_user_config_t_Class
 */
typedef struct {
    uint32_t baudRate;                        /*!< baudrate of LIN Hardware Interface to configure */
    bool nodeFunction;                        /*!< Node function as Master or Slave */
    bool autobaudEnable;                      /*!< Enable Autobaud feature */
    lin_timer_get_time_interval_t timerGetTimeIntervalCallback;    /*!< Callback function to get time interval in nanoseconds */
    uint8_t *classicPID;                      /*!< List of PIDs use classic checksum */
    uint8_t numOfClassicPID;                  /*!< Number of PIDs use classic checksum */
} lin_user_config_t;

/*!
 * @brief Defines types for an enumerating event related to an Identifier.
 * Implements : lin_event_id_t_Class
 */
typedef enum {
    LIN_NO_EVENT             = 0x00U,    /*!< No event yet */
    LIN_WAKEUP_SIGNAL        = 0x01U,    /*!< Received a wakeup signal */
    LIN_BAUDRATE_ADJUSTED    = 0x02U,    /*!< Indicate that baudrate was adjusted to Master's baudrate */
    LIN_RECV_BREAK_FIELD_OK  = 0x03U,    /*!< Indicate that correct Break Field was received */
    LIN_SYNC_OK              = 0x04U,    /*!< Sync byte is correct */
    LIN_SYNC_ERROR           = 0x05U,    /*!< Sync byte is incorrect */
    LIN_PID_OK               = 0x06U,    /*!< PID correct */
    LIN_PID_ERROR            = 0x07U,    /*!< PID incorrect */
    LIN_FRAME_ERROR          = 0x08U,    /*!< Framing Error */
    LIN_READBACK_ERROR       = 0x09U,    /*!< Readback data is incorrect */
    LIN_CHECKSUM_ERROR       = 0x0AU,    /*!< Checksum byte is incorrect */
    LIN_TX_COMPLETED         = 0x0BU,    /*!< Sending data completed */
    LIN_RX_COMPLETED         = 0x0CU,    /*!< Receiving data completed */
    LIN_RX_OVERRUN           = 0x0DU     /*!< RX overrun flag */
} lin_event_id_t;

/*!
 * @brief Define type for an enumerating LIN Node state.
 * Implements : lin_node_state_t_Class
 */
typedef enum {
    LIN_NODE_STATE_UNINIT                 = 0x00U,    /*!< Uninitialized state */
    LIN_NODE_STATE_SLEEP_MODE             = 0x01U,    /*!< Sleep mode state */
    LIN_NODE_STATE_IDLE                   = 0x02U,    /*!< Idle state */
    LIN_NODE_STATE_SEND_BREAK_FIELD       = 0x03U,    /*!< Send break field state */
    LIN_NODE_STATE_RECV_SYNC              = 0x04U,    /*!< Receive the synchronization byte state */
    LIN_NODE_STATE_SEND_PID               = 0x05U,    /*!< Send PID state */
    LIN_NODE_STATE_RECV_PID               = 0x06U,    /*!< Receive PID state */
    LIN_NODE_STATE_RECV_DATA              = 0x07U,    /*!< Receive data state */
    LIN_NODE_STATE_RECV_DATA_COMPLETED    = 0x08U,    /*!< Receive data completed state */
    LIN_NODE_STATE_SEND_DATA              = 0x09U,    /*!< Send data state */
    LIN_NODE_STATE_SEND_DATA_COMPLETED    = 0x0AU     /*!< Send data completed state */
} lin_node_state_t;

/*!
 * @brief LIN Driver callback function type
 * Implements : lin_callback_t_Class
 */
typedef void (* lin_callback_t)(uint32_t instance, void * linState);

/*!
 * @brief Runtime state of the LIN driver.
 *
 * Note that the caller provides memory for the driver state structures during
 * initialization because the driver does not statically allocate memory.
 * Implements : lin_state_t_Class
 */
typedef struct {
    const uint8_t * txBuff;                     /*!< The buffer of data being sent. */
    uint8_t * rxBuff;                           /*!< The buffer of received data. */
    uint8_t cntByte;                            /*!< To count number of bytes already transmitted or received. */
    volatile uint8_t txSize;                    /*!< The remaining number of bytes to be transmitted. */
    volatile uint8_t rxSize;                    /*!< The remaining number of bytes to be received. */
    uint8_t checkSum;                           /*!< Checksum byte. */
    volatile bool isTxBusy;                     /*!< True if the LIN interface is transmitting frame data. */
    volatile bool isRxBusy;                     /*!< True if the LIN interface is receiving frame data. */
    volatile bool isBusBusy;                    /*!< True if there are data, frame headers being transferred on bus */
    volatile bool isTxBlocking;                 /*!< True if transmit is blocking transaction. */
    volatile bool isRxBlocking;                 /*!< True if receive is blocking transaction. */
    lin_callback_t Callback;                    /*!< Callback function to invoke after receiving a byte or transmitting a byte. */
    uint8_t currentId;                          /*!< Current ID */
    uint8_t currentPid;                         /*!< Current PID */
    volatile lin_event_id_t currentEventId;     /*!< Current ID Event */
    volatile lin_node_state_t currentNodeState; /*!< Current Node state */
    volatile uint32_t timeoutCounter;           /*!< Value of the timeout counter */
    volatile bool timeoutCounterFlag;           /*!< Timeout counter flag */
    volatile bool baudrateEvalEnable;           /*!< Baudrate Evaluation Process Enable */
    volatile uint8_t fallingEdgeInterruptCount; /*!< Falling Edge count of a sync byte */
    uint32_t linSourceClockFreq;                /*!< Frequency of the source clock for LIN */
    semaphore_t txCompleted;                    /*!< Used to wait for LIN interface ISR to complete transmission.*/
    semaphore_t rxCompleted;                    /*!< Used to wait for LIN interface ISR to complete reception*/
} lin_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name LIN DRIVER
 * @{
 */

/*!
 * @brief Initializes an instance LIN Hardware Interface for LIN Network.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must select the LIN Hardware Interface clock source in the application to initialize the LIN Hardware Interface.
 *
 * @param instance LIN Hardware Interface instance number
 * @param linUserConfig user configuration structure of type #lin_user_config_t
 * @param linCurrentState pointer to the LIN Hardware Interface driver state structure
 * @return operation status:
 *         - STATUS_SUCCESS : Operation was successful.
 *         - STATUS_ERROR   : Operation failed due to semaphores initialize error.
 */
status_t LIN_DRV_Init(uint32_t instance,
                      lin_user_config_t * linUserConfig,
                      lin_state_t * linCurrentState);

/*!
 * @brief Shuts down the LIN Hardware Interface by disabling interrupts and transmitter/receiver.
 *
 * @param instance LIN Hardware Interface instance number
 * @return void
 */
void LIN_DRV_Deinit(uint32_t instance);

/*!
 * @brief Initializes the LIN user configuration structure with default values.
 *
 * This function initializes a configuration structure received from the application
 * with default values.
 * Note: Users shall assign measurement callback function pointer that is timerGetTimeIntervalCallback
 * for linUserConfig. Users can see detail in doxygen.
 *
 * @param[in] isMaster Node function:
                - true if node is MASTER
                - false if node is SLAVE
 * @param[out] linUserConfig the default configuration
 * @return void
 */
void LIN_DRV_GetDefaultConfig(bool isMaster,
                              lin_user_config_t * linUserConfig);

/*!
 * @brief Installs callback function that is used for LIN_DRV_IRQHandler.
 *
 * @note After a callback is installed, it bypasses part of the LIN Hardware Interface IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance LIN Hardware Interface instance number.
 * @param function the LIN receive callback function.
 * @return Former LIN callback function pointer.
 */
lin_callback_t LIN_DRV_InstallCallback(uint32_t instance,
                                       lin_callback_t function);

/*!
 * @brief Sends Frame data out through the LIN Hardware Interface using blocking method.
 *  This function will calculate the checksum byte and send it with the frame data.
 *  Blocking means that the function does not return until the transmission is complete.
 *  This function checks if txSize is in range from 1 to 8.
 *  If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR
 *  if node's current state is in SLEEP mode. This function checks if the isBusBusy
 *  is false, if not it will return LIN_BUS_BUSY. The function does not return
 *  until the transmission is complete. If the transmission is successful, it
 *  will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 * @param instance LIN Hardware Interface instance number
 * @param txBuff source buffer containing 8-bit data chars to send
 * @param txSize the number of bytes to send
 * @param timeoutMSec timeout value in milliseconds
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_TIMEOUT : The transmission isn't successful.
 */
status_t LIN_DRV_SendFrameDataBlocking(uint32_t instance,
                                       const uint8_t * txBuff,
                                       uint8_t txSize,
                                       uint32_t timeoutMSec);

/*!
 * @brief Sends frame data out through the LIN Hardware Interface using non-blocking method.
 *  This enables an a-sync method for transmitting data.
 *  Non-blocking  means that the function returns immediately.
 *  The application has to get the transmit status to know when the transmit is complete.
 *  This function will calculate the checksum byte and send it with the frame data. The
 *  function will return immediately after calling this function. If txSize is equal to 0
 *  or greater than 8 or node's current state is in SLEEP mode then the function will
 *  return STATUS_ERROR. If isBusBusy is currently true then the function will
 *  return LIN_BUS_BUSY.
 * @note If users use LIN_DRV_TimeoutService in a timer interrupt handler, then before using this function,
 * users have to set timeout counter to an appropriate value by using LIN_DRV_SetTimeoutCounter(instance, timeoutValue).
 * The timeout value should be big enough to complete the transmission. Timeout in real time is (timeoutValue) * (time
 * period that LIN_DRV_TimeoutService is called). For example, if LIN_DRV_TimeoutService is called in an timer
 * interrupt with period of 500 micro seconds, then timeout in real time is
 * timeoutValue * 500 micro seconds.
 * @param instance LIN Hardware Interface instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize  the number of bytes to send
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : Operation failed due to isBusBusy is currently true.
 *         - STATUS_ERROR   : Operation failed due to txSize is equal to 0 or greater than 8
 *                            or node's current state is in SLEEP mode
 */
status_t LIN_DRV_SendFrameData(uint32_t instance,
                               const uint8_t * txBuff,
                               uint8_t txSize);

/*!
 * @brief Get status of an on-going non-blocking transmission
 *  While sending frame data using non-blocking method, users can
 *  use this function to get status of that transmission.
 *  The bytesRemaining shows number of bytes that still needed to transmit.
 *
 * @param instance LIN Hardware Interface instance number
 * @param bytesRemaining Number of bytes still needed to transmit
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : The transmission is sending
 *         - STATUS_TIMEOUT : Operation failed due to timeout has occurred.
 */
status_t LIN_DRV_GetTransmitStatus(uint32_t instance,
                                   uint8_t * bytesRemaining);

/*!
 * @brief Receives frame data through the LIN Hardware Interface using blocking method.
 *  This function receives data from LPUART module using blocking
 *  method, the function does not return until the receive is complete. The interrupt
 *  handler LIN_LPUART_DRV_IRQHandler will check the checksum byte. If the checksum
 *  is correct, it will receive the frame data. If the checksum is incorrect, this
 *  function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 *  also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 *  This function also returns STATUS_ERROR if node's current state is in SLEEP mode.
 *  This function checks if the isBusBusy is false, if not it will return
 *  LIN_BUS_BUSY.
 *
 * @param instance LIN Hardware Interface instance number
 * @param rxBuff  buffer containing 8-bit received data
 * @param rxSize the number of bytes to receive
 * @param timeoutMSec timeout value in milliseconds
 * @return operation status:
 *         - STATUS_SUCCESS : The receives frame data is successful.
 *         - STATUS_TIMEOUT : The checksum is incorrect.
 *         - STATUS_BUSY    : Bus busy flag is true.
 *         - STATUS_ERROR   : Operation failed due is equal to 0 or greater than 8 or
 *                            node's current state is in SLEEP mode
 */
status_t LIN_DRV_ReceiveFrameDataBlocking(uint32_t instance,
                                          uint8_t * rxBuff,
                                          uint8_t rxSize,
                                          uint32_t timeoutMSec);

/*!
 * @brief Receives frame data through the LIN Hardware Interface using non-blocking method.
 *  This function will check the checksum byte. If the checksum is correct, it
 *  will receive it with the frame data.
 *  Non-blocking  means that the function returns immediately.
 *  The application has to get the receive status to know when the reception is complete.
 *  The interrupt handler LIN_LPUART_DRV_IRQHandler will check the checksum byte. If the
 *  checksum is correct, it will receive the frame data. If the checksum is incorrect, this
 *  function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function also
 *  check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR. This function
 *  also returns STATUS_ERROR if node's current state is in SLEEP mode. This function checks
 *  if the isBusBusy is false, if not it will return LIN_BUS_BUSY.
 * @note If users use LIN_DRV_TimeoutService in a timer interrupt handler, then before using this function,
 * users have to set timeout counter to an appropriate value by using LIN_DRV_SetTimeoutCounter(instance, timeoutValue).
 * The timeout value should be big enough to complete the reception. Timeout in real time is (timeoutValue) * (time period
 * that LIN_DRV_TimeoutService is called). For example, if LIN_DRV_TimeoutService is called in an timer
 * interrupt with period of 500 micro seconds, then timeout in real time is
 * timeoutValue * 500 micro seconds.
 *
 * @param instance LIN Hardware Interface instance number
 * @param rxBuff  buffer containing 8-bit received data
 * @param rxSize the number of bytes to receive
 * @return operation status:
 *         - STATUS_SUCCESS : The receives frame data is successful.
 *         - STATUS_TIMEOUT : The checksum is incorrect.
 *         - STATUS_BUSY    : Bus busy flag is true.
 *         - STATUS_ERROR   : Operation failed due is equal to 0 or greater than 8 or
 *                            node's current state is in SLEEP mode
 */
status_t LIN_DRV_ReceiveFrameData(uint32_t instance,
                                  uint8_t * rxBuff,
                                  uint8_t rxSize);

/*!
 * @brief Aborts an on-going non-blocking transmission/reception.
 *  While performing a non-blocking transferring data, users can call this function
 *  to terminate immediately the transferring.
 *
 * @param instance LIN Hardware Interface instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_DRV_AbortTransferData(uint32_t instance);

/*!
 * @brief Get status of an on-going non-blocking reception.
 *  This function returns whether the data reception is complete.
 *  When performing non-blocking transmit, the user can call this function to
 *  ascertain the state of the current receive progress:
 *  in progress (STATUS_BUSY) or timeout (STATUS_TIMEOUT) or complete (STATUS_SUCCESS).
 *  In addition, if the reception is still in progress, the user can obtain the
 *  number of bytes that still needed to receive.
 *
 * @param instance LIN Hardware Interface instance number
 * @param bytesRemaining Number of bytes still needed to receive
 * @return operation status:
 *         - STATUS_SUCCESS : The reception is complete.
 *         - STATUS_TIMEOUT : The reception isn't complete.
 *         - STATUS_BUSY    : The reception is on going
 */
status_t LIN_DRV_GetReceiveStatus(uint32_t instance,
                                  uint8_t * bytesRemaining);

/*!
 * @brief Puts current LIN node to sleep mode
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE
 *
 * @param instance LIN Hardware Interface instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_DRV_GoToSleepMode(uint32_t instance);

/*!
 * @brief Puts current LIN node to Idle state
 * This function changes current node state to LIN_NODE_STATE_IDLE
 *
 * @param instance LIN Hardware Interface instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_DRV_GotoIdleState(uint32_t instance);

/*!
 * @brief Sends a wakeup signal through the LIN Hardware Interface
 *
 * @param instance LIN Hardware Interface instance number
 * @return operation status:
 *         - STATUS_SUCCESS : Bus busy flag is false.
 *         - STATUS_BUSY    : Bus busy flag is true.
 */
status_t LIN_DRV_SendWakeupSignal(uint32_t instance);

/*!
 * @brief Get the current LIN node state
 *
 * @param instance LIN Hardware Interface instance number
 * @return current LIN node state
 */
lin_node_state_t LIN_DRV_GetCurrentNodeState (uint32_t instance);

/*!
 * @brief Callback function for Timer Interrupt Handler
 * Users may use (optional, not required) LIN_DRV_TimeoutService to check if timeout has occurred during non-blocking frame data
 * transmission and reception. User may initialize a timer (for example FTM) in Output Compare Mode
 * with period of 500 micro seconds (recommended). In timer IRQ handler, call this function.
 *
 * @param instance LIN Hardware Interface instance number
 * @return void
 */
void LIN_DRV_TimeoutService(uint32_t instance);

/*!
 * @brief Set Value for Timeout Counter that is used in LIN_DRV_TimeoutService
 *
 * @param instance LIN Hardware Interface instance number
 * @param timeoutValue  Timeout Value to be set
 * @return void
 */
void LIN_DRV_SetTimeoutCounter(uint32_t instance,
                               uint32_t timeoutValue);

/*!
 * @brief Sends frame header out through the LIN Hardware Interface using a non-blocking method.
 * This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR.
 *
 * @param instance LIN Hardware Interface instance number
 * @param id  Frame Identifier
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : Bus busy flag is true.
 *         - STATUS_ERROR   : The interface isn't Master or id isn't in range from 0 to 0x3F
 *                            or node's current state is in SLEEP mode.
 */
status_t LIN_DRV_MasterSendHeader(uint32_t instance,
                                  uint8_t id);

/*!
 * @brief Enables LIN hardware interrupts.
 *
 * @param instance LIN Hardware Interface instance number.
 * @return function always return STATUS_SUCCESS.
 */
status_t LIN_DRV_EnableIRQ(uint32_t instance);

/*!
 * @brief Disables LIN hardware interrupts.
 *
 * @param instance LIN Hardware Interface instance number
 * @return function always return STATUS_SUCCESS.
 */
status_t LIN_DRV_DisableIRQ(uint32_t instance);

/*!
 * @brief Interrupt handler for LIN Hardware Interface.
 *
 * @param instance LIN Hardware Interface instance number
 * @return void
 */
void LIN_DRV_IRQHandler(uint32_t instance);


/*!
 * @brief Makes or checks parity bits. If action is checking parity, the function
 * returns ID value if parity bits are correct or 0xFF if parity bits are incorrect. If action
 * is making parity bits, then from input value of ID, the function returns PID.
 * This is not a public API as it is called by other API functions.
 *
 * @param PID PID byte in case of checking parity bits or ID byte in case of making parity bits.
 * @param typeAction: 1 for Checking parity bits, 0 for making parity bits
 * @return Value has 8 bit:
 *         - 0xFF : Parity bits are incorrect,
 *         - ID   : Checking parity bits are correct.
 *         - PID  : typeAction is making parity bits.
 */
uint8_t LIN_DRV_ProcessParity(uint8_t PID,
                              uint8_t typeAction);

/*!
 * @brief Makes the checksum byte for a frame. For PID of identifiers,
 * if PID is 0x3C (ID 0x3C) or 0x7D (ID 0x3D) or 0xFE (ID 0x3E) or 0xBF (ID 0x3F)
 * apply classic checksum and apply enhanced checksum for other PID.
 * In case user want to calculate classic checksum please set PID to zero.
 *
 * @param buffer Pointer to Tx buffer
 * @param sizeBuffer Number of bytes that are contained in the buffer.
 * @param PID Protected Identifier byte.
 * @return the checksum byte.
 */
uint8_t LIN_DRV_MakeChecksumByte(const uint8_t *buffer,
                                 uint8_t sizeBuffer,
                                 uint8_t PID);

/*!
 * @brief Captures time interval to capture baudrate automatically
 * when enable autobaud feature.
 * This function should only be used in Slave.
 * The timer should be in input capture mode of both rising and falling edges.
 * The timer input capture pin should be externally connected to RXD pin.
 *
 * @param instance LIN Hardware Interface instance number
 * @return operation status
 *        - STATUS_SUCCESS: Operation was successful.
 *        - STATUS_BUSY:    Operation is running.
 *        - STATUS_ERROR:   Operation failed due to break char incorrect,
 *                          wakeup signal incorrect
 *                          or calculate baudrate failed.
 */
status_t LIN_DRV_AutoBaudCapture(uint32_t instance);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* LIN_DRIVER_H */
/******************************************************************************/
/* EOF */
/******************************************************************************/
