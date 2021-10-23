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

#ifndef LIN_DIAGNOSTIC_SERVICE_H
#define LIN_DIAGNOSTIC_SERVICE_H

#include "lin_types.h"
#include "lin_cfg.h"

#if (1U == SUPPORT_TRANSPORT_LAYER)
#if (1U == SUPPORT_DIAG_SERVICE)

/*!
 * @addtogroup diagnostic_group
 * @{
 */
/*******************************************************************************
 * API
 ******************************************************************************/
#if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
#if (1U == SUPPORT_MASTER_MODE)
/*!
 * @addtogroup diag_node_configuration_group
 * @{
 */
/*!
 * @brief This call returns the status of the last requested configuration service
 *
 * @param[in] iii lin interface handle
 * @return LD_SERVICE_BUSY Service is ongoing.
 * @return LD_REQUEST_FINISHED The configuration request has been completed.
 *                             This is a intermediate status between the configuration request
 *                             and configuration response.
 * @return LD_SERVICE_IDLE The configuration request/response combination has been completed,
 *                         i.e. the response is valid and may be analyzed. Also, this value is
 *                         returned if no request has yet been called.
 * @return LD_SERVICE_ERROR The configuration request or response experienced an error. Error
 *                          here means error on the bus, and not a negative configuration
 *                          response from the slave node.
 */
l_u8 ld_is_ready(l_ifc_handle iii);


/*!
 * @brief This call returns the result of the last node configuration service, in the parameters
 *        RSID and error_code. A value in RSID is always returned but not always in the
 *        error_code. Default values for RSID and error_code is 0 (zero).
 *
 * For slave interfaces ld_check_response shall do nothing
 * @param[in] iii lin interface handle
 * @param[out] RSID buffer for saving the response ID
 * @param[out] error_code buffer for saving the error code
 *
 * @details
 *  This API is available for master interfaces only
 */
void ld_check_response(l_ifc_handle iii,
                       l_u8 * const RSID,
                       l_u8 * const error_code);

#if (1U == SUPPORT_PROTOCOL_21)
/*!
 * @brief This function assigns the protected identifier of up to four frames
 *
 * @param[in] iii lin interface handle
 * @param[in] NAD Node address value of the target node
 * @param[in] start_index specifies which is the first frame to assign a PID
 * @param[in] PIDs list of protected identifier
 * @return void
 *
 * @details
 *  This API is available for master interfaces only
 */
void ld_assign_frame_id_range(l_ifc_handle iii,
                              l_u8 NAD,
                              l_u8 start_index,
                              const l_u8 * const PIDs);

/*!
 * @brief This function to issue a save configuration request to a slave node.
 *
 * @param[in] iii Interface name
 * @param[in] NAD Node address of target
 * @return void
 *
 * @details
 *  This function is available for master nodes only. This function is available for all
 *  diagnostic classes and only for LIN2.1 and above.
 *  This function is called to send a save configuration request to a specific slave node
 *  with the given NAD, or to all slave nodes if NAD is set to broadcast
 *  This function is implemented for Master Only.
 */
void ld_save_configuration(l_ifc_handle iii,
                           l_u8 NAD);
#endif /* (1U == SUPPORT_PROTOCOL_21) */
#endif /* (1U == SUPPORT_MASTER_NODE) */

#if ((1U == SUPPORT_SLAVE_MODE) && (1U == SUPPORT_PROTOCOL_21))
/*!
 * @brief This function copies current configuration in a reserved area.
 *
 * @param[in] iii Lin interface handle
 * @param[out] data Data area to save configuration,
 * @param[out] length Length of data area (1 + n, NAD + PIDs)
 * @return LD_READ_OK If the service was successful.
 * @return LD_LENGTH_TOO_SHORT If the configuration size is greater than the length. It
 *         means that the data area does not contain a valid configuration.
 *
 * @details
 *  This function is implemented Slave Only.
 */
l_u8 ld_read_configuration(l_ifc_handle iii,
                           l_u8 * const data,
                           l_u8 * const length);


/*!
 * @brief This function configures slave node according to data.
 *
 * @param[in] iii Lin interface handle
 * @param[in] data Structure containing the NAD and all the n PIDs for the frames of the specified NAD,
 * @param[in] length Length of data area (1 + n, NAD + PIDs)
 * @return LD_SET_OK If the service was successful
 * @return LD_LENGTH_NOT_CORRECT If the required size of the configuration
 *         is not equal to the given length.
 * @return LD_DATA_ERROR The set of configuration could not be made.
 *
 * @details
 *  This function is implemented Slave Only.
 */
l_u8 ld_set_configuration(l_ifc_handle iii,
                          const l_u8 * const data,
                          l_u16 length);
#endif /* ((1U == SUPPORT_SLAVE_MODE) && (1U == SUPPORT_PROTOCOL_21)) */
/*! @} */

#if (1U == SUPPORT_DIAG_CLASS_II)
/* Diagnostic class II services, they can be used in class III */
/*!
 * @brief This function reads data by identifier, Diagnostic Class II service (0x22).
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] number_of_id number id that send in this request
 * @param[in] list_of_id list of id that send in this request
 * @return void
 *
 * @details
 *  This function is for Master node only.
 */
void diag_read_data_by_identifier(l_ifc_handle iii,
                                  const l_u8 NAD,
                                  const l_u8 number_of_id,
                                  const l_u16 * const list_of_id);

/*!
 * @brief Write Data by Identifier for a specified node - Diagnostic Class II service (0x2E)
 *
 * @param[in] iii Lin interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] data_length Data length of frame
 * @param[in] data Buffer for the data to be transmitted
 * @return void
 *
 * @details
 *  This function is for Master node only.
 */
void diag_write_data_by_identifier(l_ifc_handle iii,
                                   const l_u8 NAD,
                                   l_u16 data_length,
                                   const l_u8 * const data);

/* END diagnostic class II services */
#endif /* (1U == SUPPORT_DIAG_CLASS_II) */

/* Diagnostic class III services, these services only belong to diagnostic class III */
#if (1U == SUPPORT_DIAG_CLASS_III)
/* Session control */
/*!
 * @brief This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x10: Session control.
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] session_type is sub function of diagnostic session master request
 * @return void
 */
void diag_session_control(l_ifc_handle iii,
                          const l_u8 NAD,
                          const l_u8 session_type);

/* Fault memory */
/*!
 * @brief This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x19: Fault memory read.
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] data_length Data length of frame
 * @param[in] data Buffer for the data to be transmitted
 * @return void
 */
void diag_fault_memory_read(l_ifc_handle iii,
                            const l_u8 NAD,
                            l_u16 data_length,
                            const l_u8 * const data);

/* Fault memory clear */
/*!
 * @brief This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x14: Fault memory clear.
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] groupOfDTC contain 3 byte will be transmit follow
 *            (byte 0: HighByte, byte 1: Middle Byte, byte 2: Low Byte)
 *            to be transmitted
 * @return void
 */
void diag_fault_memory_clear(l_ifc_handle iii,
                             const l_u8 NAD,
                             const l_u8 * const groupOfDTC);

/* Input/Output control service */
/*!
 * @brief This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x2F: Input/Output control service.
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Node address value of the destination node for the transmission
 * @param[in] data_length Data length of frame
 * @param[in] data Buffer for the data to be transmitted
 * @return void
 */
void diag_IO_control(l_ifc_handle iii,
                     const l_u8 NAD,
                     l_u16 data_length,
                     const l_u8 * const data);

#endif /* (1U == SUPPORT_DIAG_CLASS_III) */

#endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

/*!
 * @addtogroup diag_node_configuration_group
 * @{
 */
/*!
 * @brief This call assigns the NAD (node diagnostic address) of all slave nodes
 *  that matches the initial_NAD, the supplier ID and the function ID.
 *  Master node only.
 *
 * @param[in] iii LIN interface handle
 * @param[in] initial_NAD Initial node address  of the target node
 * @param[in] supplier_id Supplier ID of the target node
 * @param[in] function_id Function identifier of the target node
 * @param[in] new_NAD New node address
 * @return void
 *
 * @details
 *  This call assigns the NAD (node diagnostic address) of all slave nodes
 *  that matches the initial_NAD, the supplier ID and the function ID.
 *  The new NAD of the slave node will be new_NAD.
 *  This function is used for master node only.
 */
void ld_assign_NAD(l_ifc_handle iii,
                   l_u8 initial_NAD,
                   l_u16 supplier_id,
                   l_u16 function_id,
                   l_u8 new_NAD);

/*!
 * @brief This call changes the NAD
 *  if the node properties fulfill the test specified by id, byte, mask and invert.
 *  Master node only.
 *
 * @param[in] iii :LIN interface handle
 * @param[in] NAD Current NAD value of the target node
 * @param[in] id Property ID of the target node
 * @param[in] byte Byte location of property value to be read from the target node
 * @param[in] mask Value for masking the read property byte
 * @param[in] invert Value for excluding the read property byte
 * @param[in] new_NAD New NAD value to be assigned when the condition is met
 * @return void
 *
 * @details
 *  This call changes the NAD if the node properties fulfill the test
 *  specified by id, byte, mask and invert.
 */
void ld_conditional_change_NAD(l_ifc_handle iii,
                               l_u8 NAD,
                               l_u8 id,
                               l_u8 byte_data,
                               l_u8 mask,
                               l_u8 invert,
                               l_u8 new_NAD);

/*! @} */

/*!
 * @addtogroup diag_node_identification_group
 * @{
 */
/*!
 * @brief The call requests the slave node selected with the NAD
 *  to return the property associated with the id parameter.
 *  Master node only.
 *
 * @param[in] iii LIN interface handle
 * @param[in] NAD Value of the target node
 * @param[in] supplier_id Supplier ID of the target node
 * @param[in] function_id Function ID of the target node
 * @param[in] id ID of the target node
 * @param[out] data Buffer for saving the data read from the node
 * @return void
 *
 * @details
 *  The call requests the slave node selected with the NAD to return the
 *  property associated with the id parameter.
 */
void ld_read_by_id(l_ifc_handle iii,
                   l_u8 NAD,
                   l_u16 supplier_id,
                   l_u16 function_id,
                   l_u8 id,
                   lin_product_id_t * const data);


/*! @} */

#if (1U == SUPPORT_SLAVE_MODE)
/*!
 * @brief This function will return flag of diagnostic service,
 *  if LIN slave node receive master request of the diagnostic service
 *
 * @param[in] iii LIN interface handle
 * @param[in] flag_order Order of service flag
 * @return 1 if LIN Slave node receives master request of the diagnostic service, and the flag
 *           has not been cleared by diag_clear_flag
 * @return 0 default value
 * @return 0xFF if service is not supported
 */
l_u8 diag_get_flag(l_ifc_handle iii,
                   l_u8 flag_order);

/*!
 * @brief This function will clear flag of diagnostic service,
 *
 * @param[in] iii LIN interface handle
 * @param[in] flag_order Order of service flag
 * @return void
 */
void diag_clear_flag(l_ifc_handle iii,
                     l_u8 flag_order);

/*! @} */
#endif /* (1U == SUPPORT_SLAVE_MODE) */

#if ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20))
/*!
 * @brief This function assigns the protected identifier to a slave node
 *  with the address NAD and specified supplier id (using for J2602 and LIN 2.0).
 *  Master node only.
 *
 * @param[in] iii LIN interface handle
 * @param[in] initial_NAD Initial node address  of the target node
 * @param[in] supplier_id Supplier ID of the target node
 * @param[in] message_id Message ID of the target node
 * @param[in] PID Protected ID of the target node
 * @return void
 */
void ld_assign_frame_id(l_ifc_handle iii,
                        l_u8 NAD,
                        l_u16 supplier_id,
                        l_u16 message_id,
                        l_u8 PID);

#if (1U == SUPPORT_PROTOCOL_J2602)
/*!
 * @addtogroup j2602tl_node_configuration_group
 * @{
 */
/*!
 * @brief Verifies a state of node setting (using for J2602 and LIN 2.0).
 *
 * @param[in] iii LIN interface handle
 * @return l_bool
 */
l_bool ld_is_ready_j2602(l_ifc_handle iii);

/*!
 * @brief Verifies the state of response (using for J2602 and LIN 2.0)
 * Master node only.
 *
 * @param[in] iii LIN interface handle
 * @param[out] RSID buffer for saving the response ID
 * @param[out] error_code buffer for saving the error code
 * @return l_u8 status of the last service
 */
l_u8 ld_check_response_j2602(l_ifc_handle iii,
                             l_u8 * const RSID,
                             l_u8 * const error_code);
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */

#if (1U == SUPPORT_SLAVE_MODE)
/*!
 * @brief This function reconfigures frame identifiers of a J2602 slave node
 *              based on input dnn.
 *
 * @param[in] iii LIN interface handle
 * @param[in] dnn DNN of the device
 * @return l_bool: 0: successful: Frame Identifiers were reconfigured based on
 *                    input DNN according to NAD Message ID mapping table.
 * @return l_bool: 1: Unsuccesfull: for either one of the following reasons:
 *                    - The protocol of this interface is not J2602
 *                    - This device is a Master node in this interface
 *                    - The input DNN is greater than 0xD that is invalid
 *                    - The slave has more than 16 configurable frames
 *                    - The slave has 9-16 configurable frames, and dnn is 0xC or 0xD
 *                    - The slave has 5-8 configurable frames, and dnn is not 0x00, 0x2,
 *                      0x4, 0x6, 0x8, 0xA, 0xC.
 */
l_bool ld_reconfig_msg_ID(l_ifc_handle iii,
                          l_u8 dnn);

#if (1U == SUPPORT_PROTOCOL_J2602)
/*!
 * @brief This function assigns NAD of a J2602 slave device based on input
 *              DNN that is Device Node Number. NAD is (0x60+ DNN).
 *
 * @param[in] iii LIN interface handle
 * @param[in] dnn DNN of the device
 * @return l_bool: 0: successful: New Configured NAD is 0x60 + DNN
 * @return l_bool: 1: Unsuccesfull: for either one of the following reasons:
 *                    - The protocol of this interface is not J2602
 *                    - This device is a Master node in this interface
 *                    - The input DNN is greater than 0xD that is invalid
 */
l_bool ld_assign_NAD_j2602(l_ifc_handle iii,
                           l_u8 dnn);
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */
/*! @} */
#endif /* (1U == SUPPORT_SLAVE_MODE) */

#endif /* ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20)) */

#endif /* (1U == SUPPORT_DIAG_SERVICE) */

#endif /* (1U == SUPPORT_TRANSPORT_LAYER) */

#endif /* LIN_DIAGNOSTIC_SERVICE_H */
/******************************************************************************/
/* EOF */
/******************************************************************************/
