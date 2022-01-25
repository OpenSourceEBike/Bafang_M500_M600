/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2021 NXP
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
 * @file lin_diagnostic_service.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Functions are APIs, so they shall not be made static.
 */

#include "lin_commontl_api.h"
#include "lin_diagnostic_service.h"

#if (1U == SUPPORT_TRANSPORT_LAYER)
#if (1U == SUPPORT_DIAG_SERVICE)
#if (1U == SUPPORT_SLAVE_MODE)
/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
#if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))

static void lin_assign_nad(l_ifc_handle iii);

static void lin_condittional_change_nad(l_ifc_handle iii);

#if (1U == SUPPORT_PROTOCOL_21)
static void lin_diagservice_assign_frame_id_range(l_ifc_handle iii);
#endif /* (1U == SUPPORT_PROTOCOL_21) */

#endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

static void lin_diagservice_read_by_identifier(l_ifc_handle iii);

static void lin_handle_read_by_id_user_define(l_ifc_handle iii,
                                              l_u8 id);

static void ld_make_slave_response_pdu(l_ifc_handle iii,
                                       l_u8 sid,
                                       l_u8 res_type,
                                       l_u8 error_code);

static void lin_make_res_read_by_id(l_ifc_handle iii,
                                    l_u8 sid,
                                    l_u8 error_code,
                                    l_u8 * pdu_data);

#if ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20))
static l_bool ld_change_msg_id(l_ifc_handle iii,
                               l_u8 dnn,
                               l_u8 frame_id_change);

static void lin_diagservice_assign_frame_id(l_ifc_handle iii);

#if (1U == SUPPORT_PROTOCOL_J2602)
static void lin_diagservice_target_reset(l_ifc_handle iii);
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */

#endif /* ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20)) */

#endif /* (1U == SUPPORT_SLAVE_MODE) */
/*******************************************************************************
 * Code
 ******************************************************************************/
#if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
#if (1U == SUPPORT_MASTER_MODE)
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_is_ready
 * Description   : This call returns the status of the last requested configuration service
 *
 * Implements    : ld_is_ready_Activity
 *END**************************************************************************/
l_u8 ld_is_ready(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 ret_val;
    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        ret_val = (l_u8)(g_lin_tl_descriptor_array[iii].service_status);
    }
    else
    {
        ret_val = 0xFF;
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_check_response
 * Description   : This call returns the result of the last node configuration service, in the
 * parameters RSID and error_code. A value in RSID is always returned but not always in the
 * error_code. Default values for RSID and error_code is 0 (zero).
 *
 * Implements    : ld_check_response_Activity
 *END**************************************************************************/
void ld_check_response(l_ifc_handle iii,
                       l_u8 * const RSID,
                       l_u8 * const error_code)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(RSID != NULL);
    DEV_ASSERT(error_code != NULL);
    const lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        /* Get last response service identifier */
        *RSID = tl_desc_ptr->last_RSID;
        /* Set error_code to the default value - zero */
        *error_code = 0U;
        /* Get the error code of the last config service if it is negative response */
        if (LD_NEGATIVE == (tl_desc_ptr->last_cfg_result))
        {
            *error_code = tl_desc_ptr->ld_error_code;
        }
    }
}

#if (1U == SUPPORT_PROTOCOL_21)
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_assign_frame_id_range
 * Description   : This function assigns the protected identifier of up to four frames
 *
 * Implements    : ld_assign_frame_id_range_Activity
 *END**************************************************************************/
void ld_assign_frame_id_range(l_ifc_handle iii,
                              l_u8 NAD,
                              l_u8 start_index,
                              const l_u8 * const PIDs)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(PIDs != NULL);
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u8 buff[6U];

    /* Check if this interface is a LIN Master */
    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            /* pack data into a single frame */
            /* Buffer[0] SID of Service Assign Frame ID Range: 0xB7 */
            buff[0U] = SERVICE_ASSIGN_FRAME_ID_RANGE;
            buff[1U] = start_index;
            buff[2U] = PIDs[0U];
            buff[3U] = PIDs[1U];
            buff[4U] = PIDs[2U];
            buff[5U] = PIDs[3U];

            /* put data into TX_QUEUE */
            ld_send_message(iii, 6U, NAD, buff);

            /* set node config status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        } /* End of checking service status */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_save_configuration
 * Description   : This function is for Master nodes only. This call will make a
 * save configuration request to a specific slave node with the given
 * NAD, or to all slave nodes if NAD is set to broadcast.
 *
 * Implements    : ld_save_configuration_Activity
 *END**************************************************************************/
void ld_save_configuration(l_ifc_handle iii,
                           l_u8 NAD)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 data[6U];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    /* Check if this interface is a LIN Master */
    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        /* check service is busy? */
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            data[0U] = SERVICE_SAVE_CONFIGURATION;
            data[1U] = 0xFFU;
            data[2U] = 0xFFU;
            data[3U] = 0xFFU;
            data[4U] = 0xFFU;
            data[5U] = 0xFFU;

            /* put data into TX_QUEUE */
            ld_send_message(iii, PCI_SAVE_CONFIGURATION, NAD, data);

            /* set node config status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        } /* End of checking service status */
    }
}
#endif /* (1U == SUPPORT_PROTOCOL_21) */

#endif /* (1U == SUPPORT_MASTER_MODE) */

#if (1U == SUPPORT_SLAVE_MODE)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_condittional_change_nad
 * Description   : Process Conditional Change NAD request
 *
 * Implements    : lin_condittional_change_nad_Activity
 *END**************************************************************************/
static void lin_condittional_change_nad(l_ifc_handle iii)
{
    l_u8 id;
    l_u8 byte;
    l_u8 mask;
    l_u8 invert;
    l_bool give_positive_flg = (bool)0U;
    const lin_node_attribute_t * node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
    const lin_transport_layer_queue_t * rx_queue;
    lin_product_id_t product_id = node_attr_ptr->product_id;
    lin_serial_number_t serial_number = node_attr_ptr->serial_number;

    /* Get receive queue */
    rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);
    id = rx_queue->tl_pdu_ptr[rx_queue->queue_header][3];
    byte = rx_queue->tl_pdu_ptr[rx_queue->queue_header][4];
    mask = rx_queue->tl_pdu_ptr[rx_queue->queue_header][5];
    invert = rx_queue->tl_pdu_ptr[rx_queue->queue_header][6];

    switch (id)
    {
        case LIN_PRODUCT_ID:
            if ((byte > 0U) && (byte < 6U))
            {
                /* Byte 1: Supplier ID LSB; Byte 2: Supplier ID MSB */
                if ((byte > 0U) && (byte < 3U))
                {
                    byte = (l_u8)(product_id.supplier_id >> ((byte - 1U) * 8U));
                }
                /* Byte 3: Function ID LSB; Byte 4: Function ID MSB */
                else if ((byte > 2U) && (byte < 5U))
                {
                    byte = (l_u8)(product_id.function_id >> ((byte - 3U) * 8U));
                }
                /* Byte 5: Variant */
                else
                {
                    byte = product_id.variant;
                }

                /* Do a bitwise XOR with Invert and Do a bitwise AND with Mask */
                byte = (l_u8)((byte ^ invert) & mask);
                if (byte == 0U)
                {
                    give_positive_flg = (l_bool)1U;
                }
            }

            break;
        case LIN_SERIAL_NUMBER:
            if ((byte > 0U) && (byte < 5U))
            {
                switch (byte)
                {
                    case 1U:
                        byte = serial_number.serial_0;
                        break;
                    case 2U:
                        byte = serial_number.serial_1;
                        break;
                    case 3U:
                        byte = serial_number.serial_2;
                        break;
                    case 4U:
                        byte = serial_number.serial_3;
                        break;
                    default:
                        /* Do nothing */
                        break;
                }

                /* Do a bitwise XOR with Invert and Do a bitwise AND with Mask */
                byte = (l_u8)((byte ^ invert) & mask);
                if (byte == 0U)
                {
                    give_positive_flg = (l_bool)1U;
                }
            }

            break;
        default:
            /* Do nothing */
            break;
    }

    if (give_positive_flg == (bool)1U)
    {
        /* Make response PDU before change configuration NAD */
        ld_make_slave_response_pdu(iii, SERVICE_CONDITIONAL_CHANGE_NAD, POSITIVE, 0U);
        /* If the final result is zero then change the NAD to New NAD */
        *node_attr_ptr->configured_NAD_ptr = rx_queue->tl_pdu_ptr[rx_queue->queue_header][7U];
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_assign_nad
 * Description   : Process lin assign nad request
 *
 * Implements    : lin_assign_nad_Activity
 *END**************************************************************************/
static void lin_assign_nad(l_ifc_handle iii)
{
    lin_tl_pdu_data_t lin_tl_pdu;
    l_u16 supid;
    l_u16 fid;
    lin_product_id_t product_id = g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].product_id;
    const lin_transport_layer_queue_t * rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);
    l_u8 i;
    for (i = 0U; i < 8U; i++)
    {
        lin_tl_pdu[i] = rx_queue->tl_pdu_ptr[rx_queue->queue_header][i];
    }

    /* Get supplier and function identification in request */
    supid = (l_u16)(((l_u16)lin_tl_pdu[4U]) << 8U);
    supid = (l_u16)(supid | ((l_u16)lin_tl_pdu[3]));

    fid = (l_u16)(((l_u16)lin_tl_pdu[6U]) << 8U);
    fid = (l_u16)(fid | ((l_u16)lin_tl_pdu[5]));

    /* Check Supplier ID and Function ID */
    if (((supid == product_id.supplier_id) || (supid == LD_ANY_SUPPLIER)) &&
        ((fid == product_id.function_id) || (fid == LD_ANY_FUNCTION)))
    {
        ld_make_slave_response_pdu(iii, SERVICE_ASSIGN_NAD, POSITIVE, 0U);
    }
}

#if (1U == SUPPORT_PROTOCOL_21)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_diagservice_assign_frame_id_range
 * Description   : This function to process assign frame id range request, and also prepare its response data.
 *                 This function is only for Slave Node.
 *
 * Implements    : lin_diagservice_assign_frame_id_range_Activity
 *END**************************************************************************/
static void lin_diagservice_assign_frame_id_range(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 j;
    l_u8 i;
    l_u8 start_index;
    l_u8 cfg_frame_num = 0U;
    lin_tl_pdu_data_t lin_tl_pdu;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_transport_layer_queue_t * rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);
    l_u8 storePID = 1U;
    for (i = 0U; i < 8U; i++)
    {
        lin_tl_pdu[i] = rx_queue->tl_pdu_ptr[rx_queue->queue_header][i];
    }

    /* Get start index in request */
    start_index = lin_tl_pdu[3U];

    /* Find the number of configurable frames plus 2, including 0x3C and 0x3D */
    i = 1U;
    while (0xFFFFU != (prot_user_config_ptr->list_identifiers_ROM_ptr)[i++])
    {
        cfg_frame_num++;
    }

    /* Calculate number of configurable frames */
    cfg_frame_num = (l_u8)(cfg_frame_num - 3U);

    i = 4U;
    /* Check request validity */
    for (j = start_index; j < (start_index + 4U); j++)
    {
        if ((0xFFU != lin_tl_pdu[i++]) && (j > cfg_frame_num))
        {
            ld_make_slave_response_pdu(iii, SERVICE_ASSIGN_FRAME_ID_RANGE, NEGATIVE, GENERAL_REJECT);
            storePID = 0U;
            break;
        }
    }

    if (storePID == 1U)
    {
        /* Store PIDs */
        for (i = 4U; i < 8U; i++)
        {
            switch (lin_tl_pdu[i])
            {
                case 0x00U:
                    /* Unassign frame */
                    start_index++;
                    (prot_user_config_ptr->list_identifiers_RAM_ptr)[start_index] = 0xFFU;
                    break;
                case 0xFFU:
                    /* keep the previous assigned value of this frame */
                    break;
                default:
                    /* Calculate frame ID and Assign ID to frame */
                    start_index++;
                    (prot_user_config_ptr->list_identifiers_RAM_ptr)[start_index] = lin_process_parity(lin_tl_pdu[i], CHECK_PARITY);
                    break;
            }
        } /* End of for statement */

        ld_make_slave_response_pdu(iii, SERVICE_ASSIGN_FRAME_ID_RANGE, POSITIVE, 0U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_read_configuration
 * Description   : This function copies current configuration in a reserved area.
 *
 * Implements    : ld_read_configuration_Activity
 *END**************************************************************************/
l_u8 ld_read_configuration(l_ifc_handle iii,
                           l_u8 * const data,
                           l_u8 * const length)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);
    DEV_ASSERT(length != NULL);

    l_u8 i, temp;
    const lin_node_attribute_t * node_attr_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    /* Set the default returned value to LD_READ_OK */
    l_u8 retval = (l_u8)LD_READ_OK;
    /** Set the expected length value to
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u8 expected_length;

    expected_length = (l_u8)(prot_user_config_ptr->number_of_configurable_frames - 1U);
    /* Check if slave node */
    if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
    {
        temp = *length;
        /* Get node attribute */
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
        if (temp < expected_length)
        {
            /* The 'data' size is not enough to store NAD+PIDs */
            retval = (l_u8)LD_LENGTH_TOO_SHORT;
        }
        else
        {
            /* The 'data' size is enough to store NAD+PIDs, so proceed to store them */
            /* Copy actual NAD to 'data' */
            data[0U] = *node_attr_ptr->configured_NAD_ptr;

            /* Copy protected IDs to 'data' */
            for (i = 1U; i < expected_length; i++)
            {
                data[i] = lin_process_parity(prot_user_config_ptr->list_identifiers_RAM_ptr[i], MAKE_PARITY);
            }

            /* Set the length parameter to the actual size of the configuration */
            *length = expected_length;
        }
    }
    else
    {
        retval = 0xFFU;
    }

    return retval;
} /* End ld_read_configuration() */

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_set_configuration
 * Description   : This function configures slave node according to data.
 *
 * Implements    : ld_set_configuration_Activity
 *END**************************************************************************/
l_u8 ld_set_configuration(l_ifc_handle iii,
                          const l_u8 * const data,
                          l_u16 length)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    l_u8 i;
    /* Set the default returned value to LD_DATA_ERROR */
    l_u8 retval = LD_DATA_ERROR;
    const lin_node_attribute_t * node_attr_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    /** Set the expected length value to
     * EXP = NN + NF, where :
     * NN = the number of NAD.
     * NF = the number of configurable frames;
     * Moreover:
     * Not taken PID's diagnostics frame: 3C, 3D
     */
    l_u16 expected_length;

    expected_length = (l_u16)((l_u16)prot_user_config_ptr->number_of_configurable_frames - 1U);

    /* Check if slave node */
    if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
    {
        /* Get node attribute */
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
        if (length < expected_length)
        {
            /* The 'data' size is not enough to contain NAD+PIDs */
            retval = LD_LENGTH_NOT_CORRECT;
        }
        else
        {
            /* The 'data' size is enough to contain NAD+PIDs, so proceed to read from 'data' */
            /* Read actual NAD from 'data' */
            *node_attr_ptr->configured_NAD_ptr = data[0];

            /* Copy protected IDs in 'data' to RAM configuration */
            for (i = 1U; i < expected_length; ++i)
            {
                prot_user_config_ptr->list_identifiers_RAM_ptr[i] = lin_process_parity(data[i], CHECK_PARITY);
            }

            /* No error, return OK */
            retval = LD_SET_OK;
        }
    }
    else
    {
        retval = 0xFFU;
    }

    return retval;
} /* End ld_set_configuration() */
#endif /* (1U == SUPPORT_PROTOCOL_21) */

#endif /* (1U == SUPPORT_SLAVE_MODE) */

/* diagnostic services class II */
#if (1U == SUPPORT_DIAG_CLASS_II)
/*FUNCTION**********************************************************************
 *
 * Function Name : diag_read_data_by_identifier
 * Description   : This function reads data by identifier, Diagnostic Class II service (0x22).
 *
 * Implements    : diag_read_data_by_identifier_Activity
 *END**************************************************************************/
void diag_read_data_by_identifier(l_ifc_handle iii,
                                  const l_u8 NAD,
                                  const l_u8 number_of_id,
                                  const l_u16 * const list_of_id)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(list_of_id != NULL);

    l_u8 buff[MAX_LENGTH_SERVICE];
    l_u8 i;
    l_u16 count = 0U;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u16 length = g_lin_protocol_user_cfg_array[iii].max_message_length;

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        count = (l_u16)((l_u16)number_of_id << 1U);
        if(count < length)
        {
            /* check whether service status is idle or not */
            if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
            {
                /* Buffer[0] SID of Service Read Data by Identifier: 0x22 */
                buff[0U] = SERVICE_READ_DATA_BY_IDENTIFY;
                /* Pack data */
                for(i = 0U; i < number_of_id; i++)
                {
                    count = (l_u16)(((l_u16)i + 1U) << 1U);
                    buff[count - 1U] = (l_u8)(list_of_id[i] >> 8U);
                    buff[count]      = (l_u8)(list_of_id[i] & 0xFFU);
                }

                /* send message to transport layer */
                ld_send_message(iii, count + 1U, NAD, buff);

                /* set service status to busy */
                tl_desc_ptr->service_status = LD_SERVICE_BUSY;
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : diag_write_data_by_identifier
 * Description   : Write Data by Identifier for a specified node - Diagnostic Class II service (0x2E)
 *
 * Implements    : diag_write_data_by_identifier_Activity
 *END**************************************************************************/
void diag_write_data_by_identifier(l_ifc_handle iii,
                                   const l_u8 NAD,
                                   l_u16 data_length,
                                   const l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    l_u8 buff[MAX_LENGTH_SERVICE];
    l_u8 i;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u16 length = g_lin_protocol_user_cfg_array[iii].max_message_length;

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* Check if length of data is less than length maximum */
        if (data_length < length)
        {
            /* check whether service status is idle or not */
            if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
            {

                /* pack data */
                /* Buffer[0] SID of Write Data by identifier: 0x2E */
                buff[0U] = SERVICE_WRITE_DATA_BY_IDENTIFY;

                for (i = 0U; i < data_length; i++)
                {
                    buff[i + 1U] = data[i];
                }

                ld_send_message(iii, data_length + 1U, NAD, buff);
                /* set service status to busy */
                tl_desc_ptr->service_status = LD_SERVICE_BUSY;
            }
        }
    }
}
#endif /* (1U == SUPPORT_DIAG_CLASS_II) */

/* Diagnostic services class III */
#if (1U == SUPPORT_DIAG_CLASS_III)
/*FUNCTION**********************************************************************
 *
 * Function Name : diag_session_control
 * Description   : This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x10: Session control.
 *
 * Implements    : diag_session_control_Activity
 *END**************************************************************************/
void diag_session_control(l_ifc_handle iii,
                          const l_u8 NAD,
                          const l_u8 session_type)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 buff[2];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* check whether service status is idle or not */
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            /* pack data */
            /* Buffer[0] SID of Session Control: 0x10 */
            buff[0U] = SERVICE_SESSION_CONTROL;
            buff[1U] = session_type;

            ld_send_message(iii, 2U, NAD, buff);
            /* set service status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : diag_fault_memory_read
 * Description   : This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x19: Fault memory read.
 *
 * Implements    : diag_fault_memory_read_Activity
 *END**************************************************************************/
void diag_fault_memory_read(l_ifc_handle iii,
                            const l_u8 NAD,
                            l_u16 data_length,
                            const l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    l_u8 buff[MAX_LENGTH_SERVICE];
    l_u8 i;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u16 length = g_lin_protocol_user_cfg_array[iii].max_message_length;

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* Check if length of data is lower than length maximum */
        if (data_length < length)
        {
            /* check whether service status is idle or not */
            if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
            {
                /* pack data */
                /* Buffer[0] SID of Service fault memory read: 0x19 */
                buff[0U] = SERVICE_FAULT_MEMORY_READ;

                for (i = 0U; i < data_length; i++)
                {
                    buff[i + 1U] = data[i];
                }

                ld_send_message(iii, data_length + 1U, NAD, buff);
                /* set service status to busy */
                tl_desc_ptr->service_status = LD_SERVICE_BUSY;
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : diag_fault_memory_clear
 * Description   : This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x14: Fault memory clear.
 *
 * Implements    : diag_fault_memory_clear_Activity
 *END**************************************************************************/
void diag_fault_memory_clear(l_ifc_handle iii,
                             const l_u8 NAD,
                             const l_u8 * const groupOfDTC)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(groupOfDTC != NULL);

    l_u8 buff[4U];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* check whether service status is idle or not */
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            /* pack data */
            /* Buffer[0] SID of Service fault memory clear: 0x14 */
            buff[0U] = SERIVCE_FAULT_MEMORY_CLEAR;
            buff[1U] = groupOfDTC[0U];
            buff[2U] = groupOfDTC[1U];
            buff[3U] = groupOfDTC[2U];

            ld_send_message(iii, 4U, NAD, buff);
            /* set service status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : diag_IO_control
 * Description   : This function is used for master node only. It will pack data
 * and send request to slave node with service ID = 0x2F: Input/Output control service.
 *
 * Implements    : diag_IO_control_Activity
 *END**************************************************************************/
void diag_IO_control(l_ifc_handle iii,
                     const l_u8 NAD,
                     l_u16 data_length,
                     const l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    l_u8 buff[MAX_LENGTH_SERVICE];
    l_u8 i;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u16 length = g_lin_protocol_user_cfg_array[iii].max_message_length;

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* Check if length of data is lower than length maximum */
        if (data_length < length)
        {
            /* check whether service status is idle or not */
            if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
            {
                /* pack data */
                /* Buffer[0] SID of IO Control by Identify: 0x2F */
                buff[0U] = SERVICE_IO_CONTROL_BY_IDENTIFY;

                for (i = 0U; i < data_length; i++)
                {
                    buff[i + 1U] = data[i];
                }

                ld_send_message(iii, data_length + 1U, NAD, buff);
                /* set service status to busy */
                tl_desc_ptr->service_status = LD_SERVICE_BUSY;
            }
        }
    }
}
#endif /* (1U == SUPPORT_DIAG_CLASS_III) */

#endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_assign_NAD
 * Description   : This call assigns the NAD (node diagnostic address) of all slave nodes
 *  that matches the initial_NAD, the supplier ID and the function ID.
 *
 * Implements    : ld_assign_NAD_Activity
 *END**************************************************************************/
void ld_assign_NAD(l_ifc_handle iii,
                   l_u8 initial_NAD,
                   l_u16 supplier_id,
                   l_u16 function_id,
                   l_u8 new_NAD)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 data[6U];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* check service is busy? */
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            /* data[0] SID of Service assign NAD: 0xB0 */
            data[0] = SERVICE_ASSIGN_NAD;
            data[1] = (l_u8)(supplier_id & 0x00FFU);
            data[2] = (l_u8)((supplier_id >> 8U) & 0x00FFU);
            data[3] = (l_u8)(function_id & 0x00FFU);
            data[4] = (l_u8)((function_id >> 8U) & 0x00FFU);
            data[5] = new_NAD;

            /* put data into TX_QUEUE */
            ld_send_message(iii, 6U, initial_NAD, data);

            /* set node config status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        } /* End of checking service status */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_conditional_change_NAD
 * Description   : This call changes the NAD
 *  if the node properties fulfill the test specified by id, byte, mask and invert.
 *
 * Implements    : ld_conditional_change_NAD_Activity
 *END**************************************************************************/
void ld_conditional_change_NAD(l_ifc_handle iii,
                               l_u8 NAD,
                               l_u8 id,
                               l_u8 byte_data,
                               l_u8 mask,
                               l_u8 invert,
                               l_u8 new_NAD)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 data[6];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    /* Check input parameters are in accepted range*/
    if ((id < 32U) && ((0U < byte_data) && (byte_data < 6U)))
    {
        if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
        {
            /* check service is busy? */
            if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
            {
                /* data[0] SID of Service Conditional change NAD: 0xB3 */
                data[0] = SERVICE_CONDITIONAL_CHANGE_NAD;
                data[1] = id;
                data[2] = byte_data;
                data[3] = mask;
                data[4] = invert;
                data[5] = new_NAD;

                /* put data into TX_QUEUE */
                ld_send_message(iii, 6U, NAD, data);

                /* set node config status to busy */
                tl_desc_ptr->service_status = LD_SERVICE_BUSY;
            } /* End of checking service status */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_read_by_id
 * Description   : The call requests the slave node selected with the NAD
 *  to return the property associated with the id parameter.
 *
 * Implements    : ld_read_by_id_Activity
 *END**************************************************************************/
void ld_read_by_id(l_ifc_handle iii,
                   l_u8 NAD,
                   l_u16 supplier_id,
                   l_u16 function_id,
                   l_u8 id,
                   lin_product_id_t * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    /* Multi frame support */
    l_u8 buff[6U];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* Check service is busy? */
        if (tl_desc_ptr->service_status != LD_SERVICE_BUSY)
        {
            /* Create data for Read by Identifier command */
            /* Buffer[0] SID of Service Read by identifier: 0xB2 */
            buff[0U] = SERVICE_READ_BY_IDENTIFY;
            buff[1U] = id;
            buff[2U] = (l_u8)(supplier_id & 0x00FFU);
            buff[3U] = (l_u8)((supplier_id >> 8U) & 0x00FFU);
            buff[4U] = (l_u8)(function_id & 0x00FFU);
            buff[5U] = (l_u8)((function_id >> 8U) & 0x00FFU);

            /* Store address of RAM data which contain response info */
            tl_desc_ptr->product_id_ptr = data;

            /* put data into TX_QUEUE */
            ld_send_message(iii, 6U, NAD, buff);

            /* set node config status to busy */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        } /* End of checking service status */
    }
}

#if (1U == SUPPORT_SLAVE_MODE)
/*FUNCTION**********************************************************************
 *
 * Function Name : diag_get_flag
 * Description   : This function will return flag of diagnostic service,
 *  if LIN node receive master request or slave response.
 *
 * Implements    : diag_get_flag_Activity
 *END**************************************************************************/
l_u8 diag_get_flag(l_ifc_handle iii,
                   l_u8 flag_order)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    const l_u8 * service_flag;
    l_u8 ret_val = 0xFFU;
    const lin_node_attribute_t * node_attr_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
    {
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
        service_flag = node_attr_ptr->service_flags_ptr;

        if (flag_order < node_attr_ptr->number_support_sid)
        {
            ret_val = (l_u8)service_flag[flag_order];
        }
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : diag_clear_flag
 * Description   : This function will clear flag of diagnostic service,
 *  if lin node receive master request or slave response.
 *
 * Implements    : diag_clear_flag_Activity
 *END**************************************************************************/
void diag_clear_flag(l_ifc_handle iii,
                     l_u8 flag_order)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    l_u8 * service_flag;
    const lin_node_attribute_t * node_attr_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
    {
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
        /* Get current configuration */
        service_flag = node_attr_ptr->service_flags_ptr;

        if (flag_order < node_attr_ptr->number_support_sid)
        {
            service_flag[flag_order] = 0U;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_diag_service_callback
 * Description   : Callback handler for dispatching diagnostic services
 *
 * Implements    : lin_diag_service_callback_Activity
 *END**************************************************************************/
void lin_diag_service_callback(l_ifc_handle iii,
                               l_u8 sid)
{
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
    l_bool sid_supported_flg = (bool)0U;
    l_u8 i;
    l_u8 pci_length;
    const l_u8 * service_supported_ptr;
    l_u8 * service_flag_ptr;
    /* Get support sid */
    service_supported_ptr = node_attr_ptr->service_supported_ptr;
    /* Get service flag */
    service_flag_ptr = node_attr_ptr->service_flags_ptr;
    /* Get request frame length */
    const lin_transport_layer_queue_t * rx_queue = &(tl_desc_ptr->tl_rx_queue);
    pci_length = (l_u8)(rx_queue->tl_pdu_ptr[rx_queue->queue_header][1U] & 0x0FU);

    for (i = 0U; i < node_attr_ptr->number_support_sid; i++)
    {
        if (service_supported_ptr[i] == sid)
        {
            service_flag_ptr[i] = 1U;
            sid_supported_flg = (bool)1U;
            break;
        }
    }

    if (sid_supported_flg == (bool)1U)
    {
        /* Check whether or not the Service is supported by the Slave node */
        switch (sid)
        {
            case SERVICE_READ_BY_IDENTIFY:    /* Mandatory for TL LIN 2.1 & 2.0, Optional for J2602 */
                if (6U == pci_length)
                {
                    lin_diagservice_read_by_identifier(iii);
                }
                break;

            #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
            case SERVICE_CONDITIONAL_CHANGE_NAD:
                if (((LIN_PROTOCOL_21 == prot_user_config_ptr->protocol_version) || (LIN_PROTOCOL_20 == prot_user_config_ptr->protocol_version)) && (6U == pci_length))
                {
                    lin_condittional_change_nad(iii);
                }
                break;
            case SERVICE_ASSIGN_NAD:
                if (((LIN_PROTOCOL_21 == prot_user_config_ptr->protocol_version) || (LIN_PROTOCOL_20 == prot_user_config_ptr->protocol_version)) && (6U == pci_length))
                {
                    lin_assign_nad(iii);
                }
                break;

            #if (1U == SUPPORT_PROTOCOL_21)
            case SERVICE_ASSIGN_FRAME_ID_RANGE:                  /* Mandatory for TL LIN 2.1 */
                if ((LIN_PROTOCOL_21 == prot_user_config_ptr->protocol_version) && (6U == pci_length))
                {
                    lin_diagservice_assign_frame_id_range(iii);
                }
                break;
            case SERVICE_SAVE_CONFIGURATION:
                if (1U == pci_length)
                {
                    /* Set save configuration flag */
                    g_lin_protocol_state_array[iii].save_config_flg = (l_bool)1U;
                    /* Response to master - RSID */
                    ld_make_slave_response_pdu(iii, SERVICE_SAVE_CONFIGURATION, POSITIVE, 0U);
                }
                break;
            #endif /* (1U == SUPPORT_PROTOCOL_21) */

            #endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

            #if ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20))
            case SERVICE_ASSIGN_FRAME_ID:
                if ((LIN_PROTOCOL_J2602 == prot_user_config_ptr->protocol_version || LIN_PROTOCOL_20 == prot_user_config_ptr->protocol_version) && (6U == pci_length))
                {
                    lin_diagservice_assign_frame_id(iii);
                }
                break;
            #if (1U == SUPPORT_PROTOCOL_J2602)
            case SERVICE_TARGET_RESET:
                if ((LIN_PROTOCOL_J2602 == prot_user_config_ptr->protocol_version) && (1U == pci_length))
                {
                    lin_diagservice_target_reset(iii);
                }
                break;
            #endif /* (1U == SUPPORT_PROTOCOL_J2602) */

            #endif /* ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20)) */

            default:
                /* do nothing */
                break;
        } /* end of switch */
    }
    else
    {
        ld_make_slave_response_pdu(iii, sid, NEGATIVE, SERVICE_NOT_SUPPORTED);
        /* clear queue */
        tl_desc_ptr->tl_rx_queue.queue_status = LD_NO_DATA;
        tl_desc_ptr->tl_rx_queue.queue_current_size = 0U;
        tl_desc_ptr->tl_rx_queue.queue_header = tl_desc_ptr->tl_rx_queue.queue_tail;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_diagservice_read_by_identifier
 * Description   : This function to process read by identifier request, and also prepare its response data.
 *                 Only for Slave Nodes.
 *
 * Implements    : lin_diagservice_read_by_identifier_Activity
 *END**************************************************************************/
static void lin_diagservice_read_by_identifier(l_ifc_handle iii)
{
    lin_product_id_t product_id;
    lin_tl_pdu_data_t lin_tl_pdu;
    l_u8 id;
    l_u16 supid;
    l_u16 fid;
    const lin_transport_layer_queue_t * rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);
    l_u8 i;
    for (i = 0U; i < 8U; i++)
    {
        lin_tl_pdu[i] = rx_queue->tl_pdu_ptr[rx_queue->queue_header][i];
    }

    /* Get the product identification */
    product_id = g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].product_id;

    /* Get supplier and function identification in request */
    supid = (l_u16)(((l_u16)(lin_tl_pdu[5U])) << 8U);
    supid = (l_u16)(supid | (l_u16)(lin_tl_pdu[4U]));

    fid = (l_u16)(((l_u16)(lin_tl_pdu[7U])) << 8U);
    fid = (l_u16)(fid | (l_u16)(lin_tl_pdu[6U]));


    /* Check Supplier ID and Function ID */
    if (((supid == product_id.supplier_id) || (supid == LD_ANY_SUPPLIER)) &&
        ((fid == product_id.function_id) || (fid == LD_ANY_FUNCTION)))
    {
        /* Get the identifier of request */
        id = lin_tl_pdu[3];

        switch (id)
        {
            case LIN_PRODUCT_ID:
                ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, POSITIVE, id);
                break;
            case LIN_SERIAL_NUMBER:
                ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, POSITIVE, id);
                break;
            default:
                lin_handle_read_by_id_user_define(iii, id);
                break;
        } /* end of switch */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_handle_read_by_id_user_define
 * Description   : This function to process read by identifier with ID is defined by user, and prepare its response data.
 *                 Only for Slave Nodes.
 *
 * Implements    : lin_handle_read_by_id_user_define_Activity
 *END**************************************************************************/
static void lin_handle_read_by_id_user_define(l_ifc_handle iii,
                                              l_u8 id)
{
    l_u8 i = 0U;
    /* For ID from 32 to 63, call user defined ld_read_by_id_callout */
    if ((id >= LIN_READ_USR_DEF_MIN) && (id <= LIN_READ_USR_DEF_MAX))
    {
        l_u8 data_callout[5U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
        l_u8 retval = ld_read_by_id_callout(iii, id, data_callout);
        /*If the User ID is supported, make positive response*/
        if (retval == LD_POSITIVE_RESPONSE)
        {
            while ((i < 5U) && (data_callout[i] == 0xFFU))
            {
                i++;
            }
            if (i < 5U)
            {
                ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, POSITIVE, id);
            }
            else
            {
                ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, NEGATIVE, SUBFUNCTION_NOT_SUPPORTED);
            }
        }
        /*If the User ID is not supported, make negative response*/
        else if (retval == LD_NEGATIVE_RESPONSE)
        {
            /* Make a negative slave response PDU */
            ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, NEGATIVE, SUBFUNCTION_NOT_SUPPORTED);
        }
        else
        {
            /*Do not answer*/
            g_lin_tl_descriptor_array[iii].service_status = LD_SERVICE_IDLE;
        }
    }
    /* For ID from 2 to 31 or 64-255, give negative response */
    else
    {
        /* Make a negative slave response PDU */
        ld_make_slave_response_pdu(iii, SERVICE_READ_BY_IDENTIFY, NEGATIVE, SUBFUNCTION_NOT_SUPPORTED);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_make_slave_response_pdu
 *
 * Description   : Creates PDUs for the transport layer for specified diagnostic service
 *                 and adds them to the transmit queue.
 *                 This function is implemented for Slave only.
 *
 * Implements    : ld_make_slave_response_pdu_Activity
 *END**************************************************************************/
static void ld_make_slave_response_pdu(l_ifc_handle iii,
                                       l_u8 sid,
                                       l_u8 res_type,
                                       l_u8 error_code)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
#if (1U == SUPPORT_PROTOCOL_J2602)
    const lin_product_id_t * ident;
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */
    l_u8 NAD;
    lin_tl_pdu_data_t lin_tl_pdu;
    const lin_transport_layer_queue_t * rx_queue;

    /* Get receive queue */
    rx_queue = &(tl_desc_ptr->tl_rx_queue);
    NAD = rx_queue->tl_pdu_ptr[rx_queue->queue_header][0U];
    if (NAD != LD_FUNCTIONAL_NAD)
    {
        /* Pack data to response PDU */
        if (sid == SERVICE_ASSIGN_NAD)
        {
            lin_tl_pdu[0U] = node_attr_ptr->initial_NAD;
        }
        else
        {
            lin_tl_pdu[0U] = *node_attr_ptr->configured_NAD_ptr;
        }
        lin_tl_pdu[1U] = 0x03U;        /* PCI */
        lin_tl_pdu[2U] = RES_NEGATIVE; /* SID */
        lin_tl_pdu[3U] = sid;          /* D0 */
        lin_tl_pdu[4U] = error_code;   /* D1 */
        lin_tl_pdu[5U] = 0xFFU;        /* D2 */
        lin_tl_pdu[6U] = 0xFFU;        /* D3 */
        lin_tl_pdu[7U] = 0xFFU;        /* D4 */

        if (POSITIVE == res_type)
        {
            switch (sid)
            {
                case SERVICE_READ_BY_IDENTIFY:
                    lin_make_res_read_by_id(iii, sid, error_code, lin_tl_pdu);
                    break;
                case SERVICE_ASSIGN_FRAME_ID:
                    lin_tl_pdu[0U] = *node_attr_ptr->configured_NAD_ptr;
                    lin_tl_pdu[1U] = 0x01U;        /* PCI */
                    lin_tl_pdu[2U] = 0xF1U;        /* SID */
                    lin_tl_pdu[3U] = 0xFFU;        /* D0 */
                    lin_tl_pdu[4U] = 0xFFU;        /* D1 */
                    break;
                case SERVICE_ASSIGN_NAD:
                    lin_tl_pdu[0U] = node_attr_ptr->initial_NAD;
                    *node_attr_ptr->configured_NAD_ptr = rx_queue->tl_pdu_ptr[rx_queue->queue_header][7];
                    /* PCI */
                    lin_tl_pdu[1U] = 0x01U;
                    /* RSID */
                    lin_tl_pdu[2U] = 0xF0U;
                    lin_tl_pdu[3U] = 0xFFU;
                    lin_tl_pdu[4U] = 0xFFU;
                    break;
                case SERVICE_CONDITIONAL_CHANGE_NAD:
                    /* PCI */
                    lin_tl_pdu[1U] = 0x01U;
                    /* RSID */
                    lin_tl_pdu[2U] = 0xF3U;
                    lin_tl_pdu[3U] = 0xFFU;
                    lin_tl_pdu[4U] = 0xFFU;
                    break;

                #if (1U == SUPPORT_PROTOCOL_21)
                case SERVICE_SAVE_CONFIGURATION:
                    /* PCI type */
                    lin_tl_pdu[1U] = PCI_RES_SAVE_CONFIGURATION;
                    /* SID */
                    lin_tl_pdu[2U] = (l_u8)(RES_POSITIVE + sid);
                    /* Data unused */
                    lin_tl_pdu[3U] = 0xFFU;
                    lin_tl_pdu[4U] = 0xFFU;
                    break;
                case SERVICE_ASSIGN_FRAME_ID_RANGE:  /* Mandatory for TL LIN 2.1 */
                        lin_tl_pdu[1U] = PCI_RES_ASSIGN_FRAME_ID_RANGE;
                        lin_tl_pdu[2U] = (l_u8)(RES_POSITIVE + sid);
                        lin_tl_pdu[3U] = 0xFFU;
                        lin_tl_pdu[4U] = 0xFFU;
                    break;
                #endif /* (1U == SUPPORT_PROTOCOL_21) */

                #if (1U == SUPPORT_PROTOCOL_J2602)
                case SERVICE_TARGET_RESET:
                    lin_tl_pdu[1U] = 0x06U;
                    lin_tl_pdu[2U] = (l_u8)(RES_POSITIVE + sid);
                    ident = (lin_product_id_t *)&node_attr_ptr->product_id;
                    lin_tl_pdu[3U] = (l_u8)(ident->supplier_id & 0xFFU);
                    lin_tl_pdu[4U] = (l_u8)(ident->supplier_id >> 8U);
                    lin_tl_pdu[5U] = (l_u8)(ident->function_id & 0xFFU);
                    lin_tl_pdu[6U] = (l_u8)(ident->function_id >> 8U);
                    lin_tl_pdu[7U] = ident->variant;
                    break;
                #endif /* (1U == SUPPORT_PROTOCOL_J2602) */

                default:
                    /* do nothing */
                    break;
            } /* end of switch statement */
        }

        /* Put lin_tl_pdu data into transmit queue*/
        ld_put_raw(iii, lin_tl_pdu);
        tl_desc_ptr->diag_state  = LD_DIAG_TX_PHY;
    }
    else
    {
        tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_make_res_read_by_id
 * Description   : Prepare PDUs for the transport layer for diagnostic service read by identifier
 *                 This function is implemented for Slave only.
 *
 * Implements    : lin_make_res_read_by_id_Activity
 *END**************************************************************************/
static void lin_make_res_read_by_id(l_ifc_handle iii,
                                    l_u8 sid,
                                    l_u8 error_code,
                                    l_u8 * pdu_data)
{
    const lin_node_attribute_t * node_attr_ptr;
    node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
    const lin_product_id_t * ident;
    const lin_serial_number_t * serial_number;
    l_u8 i = 0U;

    /* PCI type */
    pdu_data[1U] = PCI_RES_READ_BY_IDENTIFY;
    /* SID */
    pdu_data[2U] = (l_u8)(RES_POSITIVE + sid);
    if (error_code == LIN_PRODUCT_ID)
    {
        /* Get Identifier info */
        ident = (const lin_product_id_t *)(&node_attr_ptr->product_id);
        pdu_data[3U] = (l_u8)(ident->supplier_id & 0xFFU);
        pdu_data[4U] = (l_u8)(ident->supplier_id >> 8U);
        pdu_data[5U] = (l_u8)(ident->function_id & 0xFFU);
        pdu_data[6U] = (l_u8)(ident->function_id >> 8U);
        pdu_data[7U] = ident->variant;
    }
    else if (error_code == LIN_SERIAL_NUMBER)
    {
        serial_number = (const lin_serial_number_t *)(&node_attr_ptr->serial_number);
        pdu_data[3U] = serial_number->serial_0;
        pdu_data[4U] = serial_number->serial_1;
        pdu_data[5U] = serial_number->serial_2;
        pdu_data[6U] = serial_number->serial_3;
        /* PCI for Serial Number is 0x05 */
        pdu_data[1U] = 0x05U;
    }
    else
    {
        l_u8 data_callout[5U] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        (void)ld_read_by_id_callout(iii, error_code, data_callout);
        /* packing user defined pdu */
        pdu_data[3U] = data_callout[0U];
        pdu_data[4U] = data_callout[1U];
        pdu_data[5U] = data_callout[2U];
        pdu_data[6U] = data_callout[3U];
        pdu_data[7U] = data_callout[4U];
        /* Check for data values*/
        for (i = 5U; i > 0U; i--)
        {
            if (data_callout[i - 1U] != 0xFFU)
            {
                /* PCI: Data length is 1 (RSID) + all data exclude 0xFF */
                pdu_data[1] = (l_u8)(i + 1U);
                break;
            }
        }
    }
}
#endif /* (1U == SUPPORT_SLAVE_MODE) */

#if ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20))
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_assign_frame_id
 * Description   : This function assigns the protected identifier of up to four frames
 *  in the slave node with the addressed NAD (using for J2602 and LIN 2.0).
 *
 * Implements    : ld_assign_frame_id_Activity
 *END**************************************************************************/
void ld_assign_frame_id(l_ifc_handle iii,
                        l_u8 NAD,
                        l_u16 supplier_id,
                        l_u16 message_id,
                        l_u8 PID)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 data[6];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        /* check service is busy? */
        if (LD_SERVICE_BUSY != tl_desc_ptr->service_status)
        {
            /* SID of Service assign frame identifier: 0xB1 */
            data[0] = SERVICE_ASSIGN_FRAME_ID;
            data[1] = (l_u8)(supplier_id & 0x00FFU);
            data[2] = (l_u8)((supplier_id >> 8U) & 0x00FFU);
            data[3] = (l_u8)(message_id & 0x00FFU);
            data[4] = (l_u8)((message_id >> 8U) & 0x00FFU);
            data[5] = PID;

            /* put data into TX_QUEUE */
            ld_send_message(iii, 6U, NAD, data);

            /* Set state of service is BUSY */
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;
        }  /* End of checking service status */
    }
}

#if (1U == SUPPORT_PROTOCOL_J2602)
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_is_ready_j2602
 * Description   : Verifies a state of node setting (using for J2602 and LIN 2.0).
 *
 * Implements    : ld_is_ready_j2602_Activity
 *END**************************************************************************/
l_bool ld_is_ready_j2602(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_bool retVal = false;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        /* Check the service status */
        if (LD_SERVICE_BUSY != tl_desc_ptr->service_status)
        {
            retVal = true;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_check_response_j2602
 * Description   : Verifies the state of response (using for J2602 and LIN 2.0)
 *
 * Implements    : ld_check_response_j2602_Activity
 *END**************************************************************************/
l_u8 ld_check_response_j2602(l_ifc_handle iii,
                             l_u8 * const RSID,
                             l_u8 * const error_code)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u8 retval = 0xFFU;
    lin_last_cfg_result_t temp;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if (g_lin_protocol_user_cfg_array[iii].function == (bool)LIN_MASTER)
    {
        /* Get the status of the last service */
        temp = tl_desc_ptr->last_cfg_result;
        /* Check status of last configuration */
        switch (temp)
        {
            case LD_SUCCESS:
                *RSID = tl_desc_ptr->last_RSID;
                break;
            case LD_NEGATIVE:
                *RSID = tl_desc_ptr->last_RSID;
                *error_code = tl_desc_ptr->ld_error_code;
                break;
            default:
                /* do nothing */
                break;
        } /* end of switch */

        retval = (l_u8)temp;
    }

    return retval;
}
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */

#if (1U == SUPPORT_SLAVE_MODE)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_diagservice_assign_frame_id
 * Description   : Process assign frame id request, and also prepare its response data
 *                 and change protected identifier of frame have correct message
 *                 identifier. This function is only for Slave Nodes
 *
 * Implements    : lin_diagservice_assign_frame_id_Activity
 *END**************************************************************************/
static void lin_diagservice_assign_frame_id(l_ifc_handle iii)
{
    l_u8 i;
    l_u8 id;
    l_u16 supplier_id;
    l_u16 message_id;
    lin_tl_pdu_data_t lin_tl_pdu;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_transport_layer_queue_t * rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);
    l_u16 slave_supplier_id = g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle].product_id.supplier_id;

    /* Get data from queue */
    for (i = 0U; i < 8U; i++)
    {
        lin_tl_pdu[i] = rx_queue->tl_pdu_ptr[rx_queue->queue_header][i];
    }

    supplier_id = (l_u16)((l_u16)lin_tl_pdu[3U] | (l_u16)(lin_tl_pdu[4U] << 8U));
    message_id = (l_u16)((l_u16)lin_tl_pdu[5U] | (l_u16)(lin_tl_pdu[6U] << 8U));

    if (supplier_id == slave_supplier_id)
    {
        id = lin_process_parity(lin_tl_pdu[7], CHECK_PARITY);
        /* Checking id is correct */
        if (0xFFU != id)
        {
            for (i = (l_u8)(prot_user_config_ptr->number_of_configurable_frames - 2U); i > 0U; i--)
            {
                if (prot_user_config_ptr->list_identifiers_ROM_ptr[i] == message_id)
                {
                    prot_user_config_ptr->list_identifiers_RAM_ptr[i] = id;
                    /* Send positive response if assign successfully */
                    ld_make_slave_response_pdu(iii, SERVICE_ASSIGN_FRAME_ID, POSITIVE, 0U);
                    break;
                }
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_reconfig_msg_ID
 * Description   : This function reconfigures frame identifiers of a J2602 slave node
 *                 based on input dnn. This function is for slave nodes only.
 *
 * Implements    : ld_reconfig_msg_ID_Activity
 *END**************************************************************************/
l_bool ld_reconfig_msg_ID(l_ifc_handle iii,
                          l_u8 dnn)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    l_u8 i;
    l_bool ret_val = 1U;
    /* Get number of unconditional frames not calculate id 0x3C and 0x3D */
    l_u8 number_of_unconditional_frames = (l_u8)(prot_user_config_ptr->number_of_configurable_frames - 2U);
    i = number_of_unconditional_frames + prot_user_config_ptr->frame_start - 1U;

    /* Calculate number of unconditional frames by removing sporadic frame */
    for(; i >= 0; i--)
    {
        if(prot_user_config_ptr->frame_tbl_ptr[i].frm_type == LIN_FRM_UNCD)
        {
            break;
        }
        else
        {
            number_of_unconditional_frames--;
        }
    }

    /* Check if this interface is a LIN Slave and the protocol is J2602 */
    if ((prot_user_config_ptr->function == LIN_SLAVE) && (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602))
    {
        if (dnn <= 0xDU)
        {
            /* number of unconditional frames greater than 16 */
            if (number_of_unconditional_frames > 16U)
            {
                /* Only 0x60 is valid NAD */
                /* Do nothing */
            }
            /* number of unconditional frames is from 9 - 16 */
            else if (number_of_unconditional_frames > 8U)
            {
                /* Only NAD 0x60, 0x64, 0x68 are valid, 0x6C and 0x6D not valid */
                if ((dnn == 0U) || (dnn == 4U) || (dnn == 8U))
                {
                    ret_val = ld_change_msg_id(iii, dnn, 16U);
                }
            }
            /* number of unconditional frames is from 5 - 8 */
            else if (number_of_unconditional_frames > 4U)
            {
                /* Check to verify if dnn is 0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C */
                if ((dnn % 2U) == 0U)
                {
                    ret_val = ld_change_msg_id(iii, dnn, 8U);
                }
            }
            /* number of unconditional frames is from 1 - 4 */
            else
            {
                ret_val = ld_change_msg_id(iii, dnn, 4U);
            }
        }
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_change_msg_id
 * Description   : This function reconfigures frame identifiers of a J2602 slave node
 *                 based on input dnn and frame_id_change.
 *
 * Implements    : ld_change_msg_id_Activity
 *END**************************************************************************/
static l_bool ld_change_msg_id(l_ifc_handle iii,
                               l_u8 dnn,
                               l_u8 frame_id_change)
{
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    l_u8 number_of_configurable_frames = prot_user_config_ptr->number_of_configurable_frames;
    l_u8 i;
    l_u8 id_origin;
    /* If new DNN is greater than current DNN */
    for (i = number_of_configurable_frames; i > 0U; i--)
    {
        /* For non-broadcast frame identifiers  less than or equal to 0x37 */
        if (prot_user_config_ptr->list_identifiers_RAM_ptr[i] <= 0x37U)
        {
            /* get id with dnn equal 0 */
            id_origin = (l_u8)(prot_user_config_ptr->list_identifiers_RAM_ptr[i] % frame_id_change);
            prot_user_config_ptr->list_identifiers_RAM_ptr[i] = (l_u8)(id_origin + (l_u8)(dnn << 2));
        }
        /* For broad cast message ID */
        else
        {
            if ((dnn  >= 8U) &&
                ((prot_user_config_ptr->list_identifiers_RAM_ptr[i] == 0x38U) || (prot_user_config_ptr->list_identifiers_RAM_ptr[i] == 0x3AU)))
            {
                prot_user_config_ptr->list_identifiers_RAM_ptr[i] += 1U;
            }
            else if ((dnn  < 8U) &&
                     ((prot_user_config_ptr->list_identifiers_RAM_ptr[i] == 0x39U) || (prot_user_config_ptr->list_identifiers_RAM_ptr[i] == 0x3BU)))
            {
                prot_user_config_ptr->list_identifiers_RAM_ptr[i] -= 1U;
            }
        }
    }

    return (l_bool)0U;
}

#if (1U == SUPPORT_PROTOCOL_J2602)
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_assign_NAD_j2602
 * Description   : This function assigns NAD of a J2602 slave device based on input
 *                 dnn. NAD is (0x60 + dnn).
 *
 * Implements    : ld_assign_NAD_j2602_Activity
 *END**************************************************************************/
l_bool ld_assign_NAD_j2602(l_ifc_handle iii,
                           l_u8 dnn)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    l_bool ret_val = 1U;

    if ((prot_user_config_ptr->function == (bool)LIN_SLAVE) && (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602))
    {
        node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
        if (dnn <= 0xFU)
        {
            *(node_attr_ptr->configured_NAD_ptr) = (l_u8)(0x60U + dnn);
            ret_val = 0U;
        }
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_diagservice_target_reset
 * Description   : Process read by identifier request, and also prepare its response data
 *                 and Reset the Slave Node. This function is only for Slave Node
 *
 * Implements    : lin_diagservice_target_reset_Activity
 *END**************************************************************************/
static void lin_diagservice_target_reset(l_ifc_handle iii)
{
    const lin_node_attribute_t * node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
    l_u16* byte_offset_temp_ptr;
    l_u8* bit_offset_temp_ptr;
    l_u8 i;
    const lin_transport_layer_queue_t * rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);

    for (i = 0U; i < node_attr_ptr->num_frame_have_esignal; i++)
    {
        /* Set the reset flag within the J2602 Status Byte */
        byte_offset_temp_ptr = node_attr_ptr->response_error_byte_offset_ptr + i;
        bit_offset_temp_ptr = node_attr_ptr->response_error_bit_offset_ptr + i;
        /* Set error signal to 0x01 means "Reset" */
        g_lin_frame_data_buffer[*byte_offset_temp_ptr] = (l_u8)((g_lin_frame_data_buffer[*byte_offset_temp_ptr] & (~(0x07U << (*bit_offset_temp_ptr)))) |
                                                    (0x01U << (*bit_offset_temp_ptr)));
    }
    /* check if pdu[0] - NAD is different from LD_BROADCAST */
    if (LD_BROADCAST != rx_queue->tl_pdu_ptr[(rx_queue->queue_header)][0U])
    {
        ld_make_slave_response_pdu(iii, SERVICE_TARGET_RESET, POSITIVE, 0U);
    }
}
#endif /* (1U == SUPPORT_PROTOCOL_J2602) */

#endif /* (1U == SUPPORT_SLAVE_MODE) */

#endif /* ((1U == SUPPORT_PROTOCOL_J2602) || (1U == SUPPORT_PROTOCOL_20)) */

#endif /* (1U == SUPPORT_DIAG_SERVICE) */

#endif /* (1U == SUPPORT_TRANSPORT_LAYER) */
/*******************************************************************************
 * EOF
 ******************************************************************************/
