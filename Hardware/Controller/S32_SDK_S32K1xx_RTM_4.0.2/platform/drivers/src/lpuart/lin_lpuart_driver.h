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
 * @file lin_lpuart_driver.h
 */

#ifndef LIN_LPUART_DRIVER_H
#define LIN_LPUART_DRIVER_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lpuart_hw_access.h"
#include "clock_manager.h"
#include "lin_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Table of base addresses for LPUART instances. */
extern LPUART_Type * const g_linLpuartBase[LPUART_INSTANCE_COUNT];

/*! @brief Table to save LPUART IRQ enumeration numbers defined in the device header file */
extern const IRQn_Type g_linLpuartRxTxIrqId[LPUART_INSTANCE_COUNT];

#ifdef LPUART_ERR_IRQS
/*! @brief Table to save LPUART_ERR IRQ enumeration numbers defined in the device header file */
extern const IRQn_Type g_linLpuartErrIrqId[LPUART_INSTANCE_COUNT];
#endif

/*! @brief Table to save LPUART state structure pointers */
extern lin_state_t * g_linStatePtr[LPUART_INSTANCE_COUNT];

/*! @brief Table to save LIN user config structure pointers */
extern lin_user_config_t * g_linUserconfigPtr[LPUART_INSTANCE_COUNT];

/* Accept Master baudrate deviation from the slave baudrate to be 2% */
#define AUTOBAUD_BAUDRATE_TOLERANCE (uint32_t)2U
#define BIT_RATE_TOLERANCE_UNSYNC   (uint32_t)14U
/* calculate range of one bit time with baudrate 19200 by formula {1000000/19200*(100 + AUTOBAUD_BAUDRATE_TOLERANCE))/100} */
#define BIT_DURATION_MAX_19200      (uint32_t)(100000U * (100U + AUTOBAUD_BAUDRATE_TOLERANCE) / 192U)
#define BIT_DURATION_MIN_19200      (uint32_t)(100000U * (100U - AUTOBAUD_BAUDRATE_TOLERANCE) / 192U)
/* calculate range of one bit time with baudrate 14400 by formula {1000000/14400*(100 + AUTOBAUD_BAUDRATE_TOLERANCE))/100} */
#define BIT_DURATION_MAX_14400      (uint32_t)(100000U * (100U + AUTOBAUD_BAUDRATE_TOLERANCE) / 144U)
#define BIT_DURATION_MIN_14400      (uint32_t)(100000U * (100U - AUTOBAUD_BAUDRATE_TOLERANCE) / 144U)
/* calculate range of one bit time with baudrate 9600 by formula {1000000/9600*(100 + AUTOBAUD_BAUDRATE_TOLERANCE))/100} */
#define BIT_DURATION_MAX_9600       (uint32_t)(100000U * (100U + AUTOBAUD_BAUDRATE_TOLERANCE) / 96U)
#define BIT_DURATION_MIN_9600       (uint32_t)(100000U * (100U - AUTOBAUD_BAUDRATE_TOLERANCE) / 96U)
/* calculate range of one bit time with baudrate 4800 by formula {1000000/4800*(100 + AUTOBAUD_BAUDRATE_TOLERANCE))/100} */
#define BIT_DURATION_MAX_4800       (uint32_t)(100000U * (100U + AUTOBAUD_BAUDRATE_TOLERANCE) / 48U)
#define BIT_DURATION_MIN_4800       (uint32_t)(100000U * (100U - AUTOBAUD_BAUDRATE_TOLERANCE) / 48U)
/* calculate range of one bit time with baudrate 2400 by formula {1000000/2400*(100 + AUTOBAUD_BAUDRATE_TOLERANCE))/100} */
#define BIT_DURATION_MAX_2400       (uint32_t)(100000U * (100U + AUTOBAUD_BAUDRATE_TOLERANCE) / 24U)
#define BIT_DURATION_MIN_2400       (uint32_t)(100000U * (100U - AUTOBAUD_BAUDRATE_TOLERANCE) / 24U)

/* calculate range of two bit time with baudrate 19200 */
#define TWO_BIT_DURATION_MAX_19200 (2U * BIT_DURATION_MAX_19200)
#define TWO_BIT_DURATION_MIN_19200 (2U * BIT_DURATION_MIN_19200)
/* calculate range of two bit time with baudrate 14400 */
#define TWO_BIT_DURATION_MAX_14400 (2U * BIT_DURATION_MAX_14400)
#define TWO_BIT_DURATION_MIN_14400 (2U * BIT_DURATION_MIN_14400)
/* calculate range of two bit time with baudrate 9600 */
#define TWO_BIT_DURATION_MAX_9600  (2U * BIT_DURATION_MAX_9600)
#define TWO_BIT_DURATION_MIN_9600  (2U * BIT_DURATION_MIN_9600)
/* calculate range of two bit time with baudrate 4800 */
#define TWO_BIT_DURATION_MAX_4800  (2U * BIT_DURATION_MAX_4800)
#define TWO_BIT_DURATION_MIN_4800  (2U * BIT_DURATION_MIN_4800)
/* calculate range of two bit time with baudrate 2400 */
#define TWO_BIT_DURATION_MAX_2400  (2U * BIT_DURATION_MAX_2400)
#define TWO_BIT_DURATION_MIN_2400  (2U * BIT_DURATION_MIN_2400)

/* calculate range of 13 bit time minimum with baudrate 19200 for autobaud feature */
#define AUTOBAUD_BREAK_TIME_MIN    (13U * BIT_DURATION_MIN_19200)

/*******************************************************************************
 * LPUART API
 ******************************************************************************/
/*!
 * @brief Returns whether the receive data is inverted or not.
 *
 * This function returns the polarity of the receive data.
 *
 * @param base LPUART base pointer.
 * @return Rx data polarity; true: inverted, false: not inverted.
 */
static inline bool LPUART_GetRxDataPolarity(const LPUART_Type * base)
{
    return (((base->STAT >> LPUART_STAT_RXINV_SHIFT) & 1U) > 0U);
}

/*!
 * @brief Sets whether the recevie data is inverted or not.
 *
 * This function sets the polarity of the receive data.
 *
 * @param base LPUART base pointer.
 * @param polarity  Rx Data polarity; true: inverted, false: not inverted.
 */
static inline void LPUART_SetRxDataPolarity(LPUART_Type * base, bool polarity)
{
    base->STAT = (base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK & ~LPUART_STAT_RXINV_MASK)) | \
                 ((polarity ? 1UL : 0UL) << LPUART_STAT_RXINV_SHIFT);
}

/*!
 * @brief  LPUART break character transmit length configuration
 *
 * This function configures the break char length.
 * In some LPUART instances, the user should disable the transmitter before calling
 * this function. Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 * @param base LPUART base pointer
 * @param length   LPUART break character length setting: 0 - minimum 10-bit times (default),
 *                   1 - minimum 13-bit times
 */
static inline void LPUART_SetBreakCharTransmitLength(LPUART_Type * base,
                                                         lpuart_break_char_length_t length)
{
    base->STAT = (base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK & ~LPUART_STAT_BRK13_MASK)) | \
                 ((uint32_t)length << LPUART_STAT_BRK13_SHIFT);
}

/*!
 * @brief  LPUART break character detect length configuration
 *
 * This function sets the LPUART detectable break character length.
 *
 *
 * @param base LPUART base pointer
 * @param length  LPUART break character length setting: 0 - minimum 10-bit times (default),
 *                  1 - minimum 13-bit times
 */
static inline void LPUART_SetBreakCharDetectLength(LPUART_Type * base,
                                                       lpuart_break_char_length_t length)
{
    base->STAT = (base->STAT & (~FEATURE_LPUART_STAT_REG_FLAGS_MASK & ~LPUART_STAT_LBKDE_MASK)) | \
                 ((uint32_t)length << LPUART_STAT_LBKDE_SHIFT);
}

/*!
 * @brief  LPUART transmit sends break character configuration.
 *
 * This function sets break character transmission in queue mode.
 *
 *
 * @param base LPUART base pointer
 */

static inline void LPUART_QueueBreakField(LPUART_Type * base)
{
    base->DATA = LPUART_DATA_FRETSC(1U);
}

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes an LIN_LPUART instance for LIN Network.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must select the LIN_LPUART clock source in the application to initialize the LIN_LPUART.
 * This function initializes a LPUART instance for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers, initialize the module to user defined settings and
 * default settings, set break field length to be 13 bit times minimum, enable
 * the break detect interrupt, Rx complete interrupt, frame error detect interrupt,
 * and enable the LPUART module transmitter and receiver
 *
 * @param instance LIN_LPUART instance number
 * @param linUserConfig user configuration structure of type #lin_user_config_t
 * @param linCurrentState pointer to the LIN_LPUART driver state structure
 * @return operation status:
 *         - STATUS_SUCCESS : Operation was successful.
 *         - STATUS_ERROR   : Operation failed due to semaphores initialize error.
 */
status_t LIN_LPUART_DRV_Init(uint32_t instance,
                             lin_user_config_t * linUserConfig,
                             lin_state_t * linCurrentState);

/*!
 * @brief Shuts down the LIN_LPUART by disabling interrupts and transmitter/receiver.
 *
 * @param instance LIN_LPUART instance number
 * @return void
 */
void LIN_LPUART_DRV_Deinit(uint32_t instance);

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
void LIN_LPUART_DRV_GetDefaultConfig(bool isMaster,
                                     lin_user_config_t * linUserConfig);

/*!
 * @brief Installs callback function that is used for LIN_LPUART_DRV_IRQHandler.
 *
 * @note After a callback is installed, it bypasses part of the LIN_LPUART IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance The LIN_LPUART instance number.
 * @param function The LIN_LPUART receive callback function.
 * @return Former LIN callback function pointer.
 */
lin_callback_t LIN_LPUART_DRV_InstallCallback(uint32_t instance,
                                              lin_callback_t function);

/*!
 * @brief Sends Frame data out through the LIN_LPUART module using blocking method.
 *  This function will calculate the checksum byte and send it with the frame data.
 *  Blocking means that the function does not return until the transmission is complete.
 *  This function checks if txSize is in range from 1 to 8.
 *  If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR
 *  if node's current state is in SLEEP mode. This function checks if the isBusBusy
 *  is false, if not it will return STATUS_BUSY. The function does not return
 *  until the transmission is complete. If the transmission is successful, it
 *  will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 * @param instance LIN_LPUART instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize the number of bytes to send
 * @param timeoutMSec timeout value in milliseconds
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_TIMEOUT : The transmission isn't successful.
 */
status_t LIN_LPUART_DRV_SendFrameDataBlocking(uint32_t instance,
                                              const uint8_t * txBuff,
                                              uint8_t txSize,
                                              uint32_t timeoutMSec);

/*!
 * @brief Sends frame data out through the LIN_LPUART module using non-blocking method.
 *  This enables an a-sync method for transmitting data.
 *  Non-blocking  means that the function returns immediately.
 *  The application has to get the transmit status to know when the transmit is complete.
 *  This function will calculate the checksum byte and send it with the frame data.
 *  If txSize is equal to 0 or greater than 8 or node's current state is in SLEEP mode
 *  then the function will return STATUS_ERROR. If isBusBusy is currently true then the
 *  function will return STATUS_BUSY.
 *
 * @param instance LIN_LPUART instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize  the number of bytes to send
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : Operation failed due to isBusBusy is currently true.
 *         - STATUS_ERROR   : Operation failed due to txSize is equal to 0 or greater than 8
 *                            or node's current state is in SLEEP mode
 */
status_t LIN_LPUART_DRV_SendFrameData(uint32_t instance,
                                      const uint8_t * txBuff,
                                      uint8_t txSize);

/*!
 * @brief Get status of an on-going non-blocking transmission
 *  While sending frame data using non-blocking method, users can
 *  use this function to get status of that transmission.
 *  This function return STATUS_BUSY while sending, or STATUS_TIMEOUT
 *  if timeout has occurred, or return STATUS_SUCCESS when the transmission is complete.
 *  The bytesRemaining shows number of bytes that still needed to transmit.
 *
 * @param instance LIN_LPUART instance number
 * @param bytesRemaining  Number of bytes still needed to transmit
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : The transmission is sending
 *         - STATUS_TIMEOUT : Operation failed due to timeout has occurred.
 */
status_t LIN_LPUART_DRV_GetTransmitStatus(uint32_t instance,
                                          uint8_t * bytesRemaining);

/*!
 * @brief Receives frame data through the LIN_LPUART module using blocking method.
 *  The function does not return until the receive is complete. The interrupt
 *  handler LIN_LPUART_DRV_IRQHandler will check the checksum byte. If the checksum
 *  is correct, it will receive the frame data. If the checksum is incorrect, this
 *  function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 *  also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 *  This function also returns STATUS_ERROR if node's current state is in SLEEP mode.
 *  This function checks if the isBusBusy is false, if not it will return
 *  STATUS_BUSY.
 *
 * @param instance LIN_LPUART instance number
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
status_t LIN_LPUART_DRV_RecvFrmDataBlocking(uint32_t instance,
                                            uint8_t * rxBuff,
                                            uint8_t rxSize,
                                            uint32_t timeoutMSec);

/*!
 * @brief Receives frame data through the LIN_LPUART module using non-blocking method.
 *  This function returns immediately after initiating the receive function.
 *  The application has to get the receive status to see when the receive is complete.
 *  The interrupt handler LIN_LPUART_DRV_IRQHandler will check the checksum byte.
 *  If the checksum is correct, it will receive the frame data.
 *  If the checksum is incorrect, this function will return STATUS_TIMEOUT and data in
 *  rxBuff might be wrong. This function also check if rxSize is in range from 1 to 8.
 *  If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR if
 *  node's current state is in SLEEP mode. This function checks if the isBusBusy is
 *  false, if not it will return STATUS_BUSY.
 *
 * @param instance LIN_LPUART instance number
 * @param rxBuff  buffer containing 8-bit received data
 * @param rxSize the number of bytes to receive
 * @return operation status:
 *         - STATUS_SUCCESS : The receives frame data is successful.
 *         - STATUS_TIMEOUT : The checksum is incorrect.
 *         - STATUS_BUSY    : Bus busy flag is true.
 *         - STATUS_ERROR   : Operation failed due is equal to 0 or greater than 8 or
 *                            node's current state is in SLEEP mode
 */
status_t LIN_LPUART_DRV_RecvFrmData(uint32_t instance,
                                    uint8_t * rxBuff,
                                    uint8_t rxSize);

/*!
 * @brief Aborts an on-going non-blocking transmission/reception.
 *  While performing a non-blocking transferring data, users can call this function
 *  to terminate immediately the transferring.
 *
 * @param instance LIN_LPUART instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_LPUART_DRV_AbortTransferData(uint32_t instance);

/*!
 * @brief Get status of an on-going non-blocking reception
 *  While receiving frame data using non-blocking method, users can
 *  use this function to get status of that receiving.
 *  This function return the current event ID, STATUS_BUSY while receiving
 *  and return STATUS_SUCCESS, or timeout (STATUS_TIMEOUT) when the reception is complete.
 *  The bytesRemaining shows number of bytes that still needed to receive.
 *
 * @param instance LIN_LPUART instance number
 * @param bytesRemaining  Number of bytes still needed to receive
 * @return operation status:
 *         - STATUS_SUCCESS : The reception is complete.
 *         - STATUS_TIMEOUT : The reception isn't complete.
 *         - STATUS_BUSY    : The reception is on going
 */
status_t LIN_LPUART_DRV_GetReceiveStatus(uint32_t instance,
                                         uint8_t * bytesRemaining);

/*!
 * @brief This function puts current node to sleep mode
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE
 *
 * @param instance LIN_LPUART instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_LPUART_DRV_GoToSleepMode(uint32_t instance);

/*!
 * @brief Puts current LIN node to Idle state
 * This function changes current node state to LIN_NODE_STATE_IDLE
 *
 * @param instance LIN_LPUART instance number
 * @return function always return STATUS_SUCCESS
 */
status_t LIN_LPUART_DRV_GotoIdleState(uint32_t instance);

/*!
 * @brief Sends a wakeup signal through the LIN_LPUART interface
 *
 * @param instance LIN_LPUART instance number
 * @return operation status:
 *         - STATUS_SUCCESS : Bus busy flag is false.
 *         - STATUS_BUSY    : Bus busy flag is true.
 */
status_t LIN_LPUART_DRV_SendWakeupSignal(uint32_t instance);

/*!
 * @brief Get the current LIN node state
 *
 * @param instance LIN_LPUART instance number
 * @return current LIN node state
 */
lin_node_state_t LIN_LPUART_DRV_GetCurrentNodeState(uint32_t instance);

/*!
 * @brief Callback function for Timer Interrupt Handler
 * Users shall initialize a timer (for example FTM) in Output compare mode
 * with period of 500 micro seconds. In timer IRQ handler, call this function.
 *
 * @param instance LIN_LPUART instance number
 * @return void
 */
void LIN_LPUART_DRV_TimeoutService(uint32_t instance);

/*!
 * @brief Set Value for Timeout Counter that is used in LIN_LPUART_DRV_TimeoutService
 *
 * @param instance LPUART instance number
 * @param timeoutValue  Timeout Value to be set
 * @return void
 */
void LIN_LPUART_DRV_SetTimeoutCounter(uint32_t instance,
                                      uint32_t timeoutValue);

/*!
 * @brief Sends frame header out through the LIN_LPUART module using a non-blocking method.
 * This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR.
 *
 * @param instance LIN_LPUART instance number.
 * @param id  Frame Identifier.
 * @return operation status:
 *         - STATUS_SUCCESS : The transmission is successful.
 *         - STATUS_BUSY    : Bus busy flag is true.
 *         - STATUS_ERROR   : The interface isn't Master or id isn't in range from 0 to 0x3F
 *                            or node's current state is in SLEEP mode.
 */
status_t LIN_LPUART_DRV_MasterSendHeader(uint32_t instance,
                                         uint8_t id);

/*!
 * @brief Enables LIN_LPUART hardware interrupts.
 *
 * @param instance LIN_LPUART instance number.
 * @return function always return STATUS_SUCCESS.
 */
status_t LIN_LPUART_DRV_EnableIRQ(uint32_t instance);

/*!
 * @brief Disables LIN_LPUART hardware interrupts.
 *
 * @param instance LIN_LPUART instance number.
 * @return function always return STATUS_SUCCESS.
 */
status_t LIN_LPUART_DRV_DisableIRQ(uint32_t instance);

/*!
 * @brief LIN_LPUART interrupt handler for RX_TX and Error interrupts.
 *
 * @param instance LIN_LPUART instance number
 * @return void
 */
void LIN_LPUART_DRV_IRQHandler(uint32_t instance);

/*!
 * @brief LIN_LPUART capture time interval to set baudrate automatically
 * when enable autobaud feature.
 * This function should only be used in Slave.
 * The timer should be in input capture mode of both rising and falling edges.
 * The timer input capture pin should be externally connected to RXD pin.
 *
 * @param instance LIN_LPUART instance number
 * @return operation status
 *        - STATUS_SUCCESS: Operation was successful.
 *        - STATUS_BUSY:    Operation is running.
 *        - STATUS_ERROR:   Operation failed due to break char incorrect,
 *                          wakeup signal incorrect
 *                          or calculate baudrate failed.
 */
status_t LIN_LPUART_DRV_AutoBaudCapture(uint32_t instance);

#if defined(__cplusplus)
}
#endif

#endif /* LIN_LPUART_DRIVER_H */
/******************************************************************************/
/* EOF */
/******************************************************************************/
