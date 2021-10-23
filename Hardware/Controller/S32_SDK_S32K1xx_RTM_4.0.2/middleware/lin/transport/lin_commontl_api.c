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
 * @file lin_commontl_api.c
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
#include "lin_commontl_proto.h"
#if (SUPPORT_TRANSPORT_LAYER == 1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ld_init
 * Description   : Initialize or reinitialize the raw and cooked layers  on the interface iii.
 * All the transport layer buffers will be initialized.
 *
 * Implements    : ld_init_Activity
 *END**************************************************************************/
void ld_init(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    lin_transport_layer_queue_t * tl_queue;
    static lin_product_id_t product_id_data[LIN_NUM_OF_IFCS];
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u16 length = g_lin_protocol_user_cfg_array[iii].max_message_length;
    l_u16 max_queue_size;

    /* Calculate max_queue_size from max_message_length */
    if (length <= 6U)
    {
        max_queue_size = 1U;
    }
    else
    {
        if (((length - 5U) % 6U) == 0U)
        {
            max_queue_size = (l_u16)(((length - 5U) / 6U) + 1U);
        }
        else
        {
            max_queue_size = (l_u16)(((length - 5U) / 6U) + 2U);
        }
    }

    /* Initial global variables */
    tl_desc_ptr->tl_tx_queue.tl_pdu_ptr = g_lin_protocol_user_cfg_array[iii].tl_tx_queue_data_ptr;
    tl_desc_ptr->tl_rx_queue.tl_pdu_ptr = g_lin_protocol_user_cfg_array[iii].tl_rx_queue_data_ptr;

    tl_desc_ptr->product_id_ptr = &product_id_data[iii];
    tl_desc_ptr->tl_tx_queue.queue_max_size = max_queue_size;
    tl_desc_ptr->tl_rx_queue.queue_max_size = max_queue_size;

    /* Initialize transmit queue */
    /* Get TL tx queue */
    tl_queue = &(tl_desc_ptr->tl_tx_queue);
    tl_queue->queue_header = 0;
    tl_queue->queue_tail = 0;
    tl_queue->queue_status = LD_QUEUE_EMPTY;
    tl_queue->queue_current_size = 0;
    /* Initialize receive queue */
    /* Get TL rx queue */
    tl_queue = &(tl_desc_ptr->tl_rx_queue);
    tl_queue->queue_header = 0;
    tl_queue->queue_tail = 0;
    tl_queue->queue_status = LD_NO_DATA;
    tl_queue->queue_current_size = 0;
    /* Initialize transmit message */
    tl_desc_ptr->rx_msg_status = LD_COMPLETED;
    /* Initialize receive message */
    tl_desc_ptr->tx_msg_status = LD_COMPLETED;

    tl_desc_ptr->last_cfg_result = LD_SUCCESS;
    tl_desc_ptr->last_RSID = 0;
    tl_desc_ptr->ld_error_code = 0;

    tl_desc_ptr->frame_counter = 0;
    tl_desc_ptr->num_of_pdu = 0;
    tl_desc_ptr->slave_resp_cnt = 0;

    tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
    tl_desc_ptr->check_timeout = 0;

    tl_desc_ptr->diag_state = LD_DIAG_IDLE;
    tl_desc_ptr->service_status = LD_SERVICE_IDLE;
    tl_desc_ptr->diag_interleave_state = DIAG_NOT_START;
    tl_desc_ptr->interleave_timeout_counter = 0;
    tl_desc_ptr->FF_pdu_received = false;
    tl_desc_ptr->ld_return_data = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_put_raw
 * Description   : Queue the transmission of 8 bytes of data in one frame.
 * The data is sent in the next suitable frame.
 *
 * Implements    : ld_put_raw_Activity
 *END**************************************************************************/
void ld_put_raw(l_ifc_handle iii,
                const l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);
    lin_transport_layer_queue_t * tl_queue;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

#if (SUPPORT_SLAVE_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    if ((prot_user_config_ptr->function == (bool)LIN_SLAVE) && (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21))
    {
        node_attr_ptr = &g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle];
        tl_desc_ptr->check_timeout = node_attr_ptr->N_As_timeout;
        tl_desc_ptr->check_timeout_type = LD_CHECK_N_AS_TIMEOUT;
    }
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

    /* Get transmit queue */
    tl_queue = &(tl_desc_ptr->tl_tx_queue);
    tl_put_raw(iii, data, tl_queue, TRANSMITTING);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_get_raw
 * Description   : Copy the oldest received diagnostic frame data to the memory
 * specified by data.
 *
 * Implements    : ld_get_raw_Activity
 *END**************************************************************************/
void ld_get_raw(l_ifc_handle iii,
                l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);
    lin_transport_layer_queue_t * tl_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);

    tl_get_raw(iii, data, tl_queue, RECEIVING);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_raw_tx_status
 * Description   : Get the status of the raw frame transmission function
 *
 * Implements    : ld_raw_tx_status_Activity
 *END**************************************************************************/
l_u8 ld_raw_tx_status(l_ifc_handle iii)
{

    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    return (l_u8)(g_lin_tl_descriptor_array[iii].tl_tx_queue.queue_status);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_raw_rx_status
 * Description   : Get the status of the raw frame receive function.
 *
 * Implements    : ld_raw_rx_status_Activity
 *END**************************************************************************/
l_u8 ld_raw_rx_status(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    return (l_u8)(g_lin_tl_descriptor_array[iii].tl_rx_queue.queue_status);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_send_message
 * Description   : Pack the information specified by data and length into one or multiple diagnostic frames.
 *
 * Implements    : ld_send_message_Activity
 *END**************************************************************************/
void ld_send_message(l_ifc_handle iii,
                     l_u16 length,
                     l_u8 NAD,
                     const l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);

    const lin_transport_layer_queue_t * tl_queue;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    l_u8 message_size;

    /* Get transmit queue */
    tl_queue = &(tl_desc_ptr->tl_tx_queue);
    /* check message status in queue */
    if (LD_COMPLETED == tl_desc_ptr->tx_msg_status)
    {
        /* calculate number of PDU message_size for this message */
        if (length <= 6U)
        {
            message_size = 1U;
        }
        else
        {
            if (((length - 5U) % 6U) == 0U)
            {
                message_size = (l_u8)(((length - 5U) / 6U) + 1U);
            }
            else
            {
                message_size = (l_u8)(((length - 5U) / 6U) + 2U);
            }
        }

        /* Check if the message can be put into queue  */
        if (message_size <= (tl_queue->queue_max_size - tl_queue->queue_current_size))
        {
            /* update information of message in queue */
            tl_desc_ptr->tx_msg_status = LD_IN_PROGRESS;
            tl_desc_ptr->service_status = LD_SERVICE_BUSY;

            lin_tl_package_pdu(iii, length, message_size, NAD, data);
        } /* end of check message size */
    } /* end of (LD_COMPLETED == tl_conf->tl_message_status) */
} /* end of ld_send_message */

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_receive_message
 * Description   : Prepare the LIN diagnostic module to receive one message
 * and store it in the buffer pointed to  by data.
 *
 * Implements    : ld_receive_message_Activity
 *END**************************************************************************/
void ld_receive_message(l_ifc_handle iii,
                        l_u16 * const length,
                        l_u8 * const NAD,
                        l_u8 * const data)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT(data != NULL);
    DEV_ASSERT(NAD != NULL);
    DEV_ASSERT(length != NULL);
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    if((tl_desc_ptr->rx_msg_status == LD_COMPLETED) && (*length != 0U))
    {
        /* set status of receive message */
        tl_desc_ptr->rx_msg_status = LD_IN_PROGRESS;

        /* Prepare to receive diagnostic frame */
        tl_desc_ptr->receive_message_ptr  = data;
        tl_desc_ptr->receive_message_length_ptr = length;
        tl_desc_ptr->receive_NAD_ptr = NAD;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_tx_status
 * Description   : Get the status of the last made call to ld_send_message.
 *
 * Implements    : ld_tx_status_Activity
 *END**************************************************************************/
l_u8 ld_tx_status(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    const lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    return (l_u8)(tl_desc_ptr->tx_msg_status);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ld_rx_status
 * Description   : Get the status of the last made call to  ld_receive_message.
 *
 * Implements    : ld_rx_status_Activity
 *END**************************************************************************/
l_u8 ld_rx_status(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    const lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    l_u8 ret_val = (l_u8)tl_desc_ptr->rx_msg_status;
    /* For protocol 2.0 and J2602, return LD_FAILED for LD_WRONG_SN */
    if ((tl_desc_ptr->rx_msg_status == LD_WRONG_SN) && (prot_user_config_ptr->protocol_version != LIN_PROTOCOL_21))
    {
        ret_val = (l_u8)LD_FAILED;
    }

    return ret_val;
}

#if (SUPPORT_DIAG_SERVICE != 1U)
/* This function is use when user not use diag service */
void lin_diag_service_callback(l_ifc_handle iii,
                               l_u8 sid)
{
    UNUSED(iii);
    UNUSED(sid);
}

#endif /* End (SUPPORT_DIAG_SERVICE != 1)*/
#endif /* End (SUPPORT_TRANSPORT_LAYER == 1U) */
/*******************************************************************************
 * EOF
 ******************************************************************************/
