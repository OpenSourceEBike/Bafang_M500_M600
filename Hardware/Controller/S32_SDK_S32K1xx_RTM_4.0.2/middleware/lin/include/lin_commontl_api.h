/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
 * @file lin_commontl_api.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declarations.
 * The macro can be used by some configuration.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, A function should be used in preference to a function-like
 * macro where they are interchangealbe.
 * The macro is defined for callback functions or use for skeleton code.
 * This way is more efficient.
 */

#ifndef LIN_COMMONTL_API_H
#define LIN_COMMONTL_API_H

/*!
 * @addtogroup commontl_api_group
 * @{
 */

#include "lin.h"
#include "lin_common_api.h"
#if (SUPPORT_TRANSPORT_LAYER == 1U)

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Node configuration */
#define LD_READ_OK                      0x33U       /*!< Read OK */
#define LD_LENGTH_TOO_SHORT             0x34U       /*!< Length too short */

#define LD_DATA_ERROR                   0x43U       /*!< Data error */
#define LD_LENGTH_NOT_CORRECT           0x44U       /*!< Length not correct */
#define LD_SET_OK                       0x45U       /*!< Set OK */

/* For J2602 */
#define SERVICE_TARGET_RESET            0xB5U       /*!< Target reset service */
#define RES_POSITIVE                    0x40U       /*!< Positive response */

/* Identifiers of node read by identifier service */
#define LIN_PRODUCT_ID                  0x00U       /*!< Node product identifier */
#define LIN_SERIAL_NUMBER               0x01U       /*!< Serial number */

/* Define wildcards */
#define LD_BROADCAST                    0x7FU       /*!< Broadcast NAD */
#define LD_FUNCTIONAL_NAD               0x7EU       /*!< Functional NAD */
#define LD_ANY_SUPPLIER                 0x7FFFU     /*!< Supplier */
#define LD_ANY_FUNCTION                 0xFFFFU     /*!< Function */
#define LD_ANY_MESSAGE                  0xFFFFU     /*!< Message */

/* For negative response */
#define RES_NEGATIVE                    0x7FU       /*!< Negative response */
#define GENERAL_REJECT                  0x10U       /*!< Error code raised when request for service not supported comes */
#define SERVICE_NOT_SUPPORTED           0x11U       /*!< Error code in negative response for not supported service */
#define SUBFUNCTION_NOT_SUPPORTED       0x12U       /*!< Error code in negative response for not supported sub function */

/* Response type from Slave */
#define NEGATIVE                        0U          /*!< Negative response */
#define POSITIVE                        1U          /*!< Positive response */

#define TRANSMITTING                    0U          /*!< Transmitting */
#define RECEIVING                       1U          /*!< Receiving */

#if (SUPPORT_SLAVE_MODE == 1U)
extern void lin_diag_service_callback(l_ifc_handle iii,
                                      l_u8 sid);

#define DIAG_SERVICE_CALLBACK_HANDLER(iii, sid) lin_diag_service_callback((iii), (sid))
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

/* Initialized transport layer */
/*!
 * @addtogroup initialization_group
 * @{
 */
/*!
 * @brief Initialize or reinitialize the raw and cooked layers.
 *
 * @param[in] iii Interface name
 * @return void
 *
 * @details
 *  Initialize or reinitialize the raw and cooked layers  on the interface iii.
 *  All the transport layer buffers will be initialized.
 */
void ld_init(l_ifc_handle iii);

/*! @} */

/* Transport layer RAW APIs */
/*!
 * @addtogroup raw_api_group
 * @{
 */
/*!
 * @brief Queue the transmission of 8 bytes of data in one frame.
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data to be transmitted
 * @return void
 *
 * @details
 *  Queue the transmission of 8 bytes of data in one frame
 *  The data is sent in the next suitable frame.
 */
void ld_put_raw(l_ifc_handle iii,
                const l_u8 * const data);

/*!
 * @brief Copy the oldest received diagnostic frame data to the memory
 *  specified by data.
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data to be transmitted
 * @return void
 *
 * @details
 *  Copy the oldest received diagnostic frame data to the memory
 *  specified by data.
 *  The data returned is received from master request frame for slave node
 *  and the slave response frame for master node.
 */
void ld_get_raw(l_ifc_handle iii,
                l_u8 * const data);

/*!
 * @brief Get the status of the raw frame transmission function.
 *
 * @param[in] iii Interface name
 * @return l_u8
 *
 * @details
 *  Get the status of the raw frame transmission function:
 *  This function is available for < br / >
 *     LD_QUEUE_EMPTY    : The transmit queue is empty. In case previous calls to < br / >
 *                         ld_put_raw, all frames in the queue have been < br / >
 *                         transmitted. < br / >
 *     LD_QUEUE_AVAILABLE: The transmit queue contains entries, but is not full. < br / >
 *                         (For LIN2.1 and above only).
 *     LD_QUEUE_FULL     : The transmit queue is full and can not accept further < br / >
 *                         frames. < br / >
 *     LD_TRANSMIT_ERROR :  (For LIN2.1 and above only) LIN protocol errors
 *                        occurred during  the transfer; initialize and redo the transfer.
 *    LD_TRANSFER_ERROR: (For LIN2.0 and J2602 only) LIN protocol errors
 *                        occurred during  the transfer; initialize and redo the transfer.
 */
l_u8 ld_raw_tx_status(l_ifc_handle iii);

/*!
 * @brief Get the status of the raw frame receive function.
 *
 * @param[in] iii Interface name
 * @return l_u8
 *
 * @details
 *  Get the status of the raw frame receive function:
 *      LD_NO_DATA          The receive queue is empty.(For LIN2.1 and above only)
 *      LD_DATA_AVAILABLE   The receive queue contains data that can be read.
 *      LD_RECEIVE_ERROR    LIN protocol errors occurred during  the transfer;
 *                          initialize and redo the transfer.(For LIN2.1 and above only).
 *      LD_TRANSFER_ERROR: (For LIN2.0 and J2602 only) LIN protocol errors
 *                        occurred during  the transfer; initialize and redo the transfer.
 */
l_u8 ld_raw_rx_status(l_ifc_handle iii);

/*! @} */

/* Transport layer COOKED APIs */
/*!
 * @addtogroup cooked_api_group
 * @{
 */
/*!
 * @brief Pack the information specified by data and length into one or multiple diagnostic frames.
 *
 * @param[in] iii Lin interface handle
 * @param[in] length Length of data to send
 * @param[in] NAD Node address of slave node
 * @param[in] data Data to be sent
 * @return void
 *
 * @details
 *    Pack the information specified by data and length into one or multiple
 *    diagnostic frames. If the call is made in a master node application the frames
 *    are transmitted to the slave node  with the address NAD. If the  call is made in
 *    a slave node application the frames are transmitted to the master node with the
 *    address NAD. The parameter NAD is not used in slave nodes.
 */
void ld_send_message(l_ifc_handle iii,
                     l_u16 length,
                     l_u8 NAD,
                     const l_u8 * const data);

/*!
 * @brief Prepare the LIN diagnostic module to receive one message
 *  and store it in the buffer pointed to  by data.
 *
 * @param[in] iii Lin interface handle
 * @param[in] length Length of data to receive
 * @param[in] NAD Node address of slave node
 * @param[in] data Data to be sent
 * @return void
 *
 * @details
 *   Prepare the LIN diagnostic module to receive one message and store it in
 *    the buffer pointed to  by data. At the call, length shall specify the maximum length
 *    allowed. When the reception has completed, length is changed to the actual length
 *    and NAD to the NAD in the message.
 */
void ld_receive_message(l_ifc_handle iii,
                        l_u16 * const length,
                        l_u8 * const NAD,
                        l_u8 * const data);

/*!
 * @brief Get the status of the last made call to ld_send_message.
 *
 * @param[in] iii Lin interface handle
 * @return l_u8
 *
 * @details
 *    Get the status of the last made call to ld_send_message.
 *    The following values can be returned:
 *        LD_IN_PROGRESS:  The transmission is not yet completed.
 *        LD_COMPLETED:    The transmission has  completed successfully (and
 *                         you can issue a new ld_send_message call). This
 *                         value is also returned after  initialization of
 *                         the transport layer.
 *        LD_FAILED:       The transmission ended in an error. The data was
 *                         only partially sent. The transport layer shall
 *                         be reinitialized before processing  further messages.
 *                         To find out why a transmission has  failed, check
 *                         the status management function l_read_status.
 *                         For LIN2.0 and J2602 Users can make a new call to
 *                         ld_send_message.
 *                         For LIN2.1 and above, the transport layer shall be
 *                         reinitialized before processing further messages.
 *        LD_N_AS_TIMEOUT: The transmission failed because of a N_As timeout.
 *                         This applies for LIN2.1 and above only.
 */
l_u8 ld_tx_status(l_ifc_handle iii);

/*!
 * @brief Get the status of the last made call to ld_send_message.
 *
 * @param[in] iii Lin interface handle
 * @return l_u8
 *
 * @details
 *    The call returns the status of the last made call to ld_receive_message. < br / >
 *    The following values can be returned: < br / >
 *        LD_IN_PROGRESS: The reception is not yet completed. < br / >
 *        LD_COMPLETED:   The reception has  completed successfully and all < br / >
 *                        information (length, NAD, data) is available. (You can < br / >
 *                        also issue a new ld_receive_message call). This < br / >
 *                        value is also returned after  initialization of the < br / >
 *                        transport layer. < br / >
 *        LD_FAILED:      The reception ended in an error. The data was only < br / >
 *                        partially received and should not be trusted. Initialize < br / >
 *                        before processing further  transport layer messages. < br / >
 *                        For LIN2.0 and J2602 Users can make a new call to
 *                        ld_receive_message.
 *                        For LIN2.1 and above, the transport layer shall be
 *                        reinitialized before processing further messages.
 *                        To find out why a transmission has failed, check the
 *                        status management function l_ifc_read_status. *
 *        LD_N_CR_TIMEOUT The reception failed because of a N_Cr timeout
 *                        (For LIN2.1 and above only)< br / >
 *        LD_WRONG_SN     The reception failed because of an unexpected sequence number.
 *                        (For LIN2.1 and above only) <br/>
 */
l_u8 ld_rx_status(l_ifc_handle iii);

/*! @} */
#endif /*end (SUPPORT_TRANSPORT_LAYER == 1U) */
#endif /* LIN_COMMONTL_API_H */
/*! @} */

/*! @} */

/******************************************************************************/
/* EOF */
/******************************************************************************/
