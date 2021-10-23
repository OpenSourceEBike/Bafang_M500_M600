/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
 * @file lin_commontl_proto.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 */
#include "lin_commontl_proto.h"
#include "lin_common_api.h"
#include "lin_commontl_api.h"
#if (SUPPORT_TRANSPORT_LAYER == 1U)

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
static void lin_tl_handler_error(l_ifc_handle iii,
                                 l_u8 pid);

static void lin_tl_get_pdu(l_ifc_handle iii);

static void lin_tl_handler_rx_completed(l_ifc_handle iii);

static void lin_tl_handler_tx_completed(l_ifc_handle iii);

static void lin_process_single_frame(l_ifc_handle iii);

static void lin_process_first_frame(l_ifc_handle iii);

static void lin_process_consecutive_frame(l_ifc_handle iii);

#if (SUPPORT_MASTER_MODE == 1U)
static void lin_tl_get_response(l_ifc_handle iii);

static void lin_tl_sch_switch(l_ifc_handle iii);

static void lin_tl_handle_interleave_mode(l_ifc_handle iii);
#endif
#if (SUPPORT_SLAVE_MODE == 1U)
static void lin_tl_attach_service(l_ifc_handle iii);
#endif
static void lin_tl_handle_timeout(l_ifc_handle iii);
static void lin_process_pdu(l_ifc_handle iii,
                            const lin_tl_pdu_data_t * pdu);

static void lin_tl_make_master_request_pdu(l_ifc_handle iii);

static void tl_receive_message(l_ifc_handle iii);


/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_callback_handler
 * Description   : Transport layer callback to process the specified event
 *
 * Implements    : lin_tl_callback_handler_Activity
 *END**************************************************************************/
lin_tl_callback_return_t lin_tl_callback_handler(l_ifc_handle iii,
                                                 lin_tl_event_id_t tl_event_id,
                                                 l_u8 id)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    lin_tl_callback_return_t ret_val = TL_ACTION_NONE;

    switch (tl_event_id)
    {
        case TL_RECEIVE_MESSAGE:
            /* If ld_receive_message is called and first frame in multi PDU is not transmitted */
            if ((tl_desc_ptr->rx_msg_status == LD_IN_PROGRESS) && (tl_desc_ptr->FF_pdu_received == false))
            {
              /* Activate function tl_receive_message() in process_pdu() function to return data to data pointer of ld_receive_message function */
              tl_desc_ptr->ld_return_data = true;
            }
            break;
        case TL_MAKE_RES_DATA:
            lin_tl_make_master_request_pdu(iii);
            break;
        case TL_SLAVE_GET_ACTION:
            if ((tl_desc_ptr->tl_tx_queue.queue_current_size > 0U) && (tl_desc_ptr->service_status != LD_SERVICE_ERROR))
            {
                lin_tl_get_pdu(iii);
            }
            else
            {
                ret_val = TL_ACTION_ID_IGNORE;
            }
            break;
        case TL_TX_COMPLETED:
            lin_tl_handler_tx_completed(iii);
            break;
        case TL_RX_COMPLETED:
            lin_tl_handler_rx_completed(iii);
            break;
        case TL_ERROR:
            lin_tl_handler_error(iii, id);
            break;
        case TL_TIMEOUT_SERVICE:
            lin_tl_handle_timeout(iii);
            break;
        #if (SUPPORT_MASTER_MODE == 1U)
        case TL_HANDLER_INTERLEAVE_MODE:
            lin_tl_handle_interleave_mode(iii);
            break;
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
        default:
            /* Do nothing */
            break;
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_handle_timeout
 * Description   : This function handle when N_CR, N_AS or DIAGNOSTIC INTERLEAVE time out occur
 *
 * Implements    : lin_tl_handle_timeout_Activity
 *END**************************************************************************/
static void lin_tl_handle_timeout(l_ifc_handle iii)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    #if (SUPPORT_MASTER_MODE == 1U)
    const lin_master_data_t * master_data_ptr;
    const lin_schedule_t * active_schedule_struct_ptr;
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */

    if (LD_CHECK_N_CR_TIMEOUT == tl_desc_ptr->check_timeout_type)
    {
        if (!(tl_desc_ptr->check_timeout-- > 0U))
        {
            /* Update status of transport layer */
            prot_state_ptr->diagnostic_mode = DIAG_NONE;
            tl_desc_ptr->service_status = LD_SERVICE_ERROR;
            tl_desc_ptr->rx_msg_status = LD_N_CR_TIMEOUT;
            tl_desc_ptr->ld_return_data = false;
            tl_desc_ptr->FF_pdu_received = false;
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->diag_state = LD_DIAG_IDLE;
        }
    }

    if (LD_CHECK_N_AS_TIMEOUT == tl_desc_ptr->check_timeout_type)
    {
        if (!(tl_desc_ptr->check_timeout-- > 0U))
        {
            /* update status of transport layer */
            prot_state_ptr->diagnostic_mode = DIAG_NONE;
            tl_desc_ptr->service_status = LD_SERVICE_ERROR;
            tl_desc_ptr->tx_msg_status = LD_N_AS_TIMEOUT;
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->diag_state = LD_DIAG_IDLE;
        }
    }
    #if (SUPPORT_MASTER_MODE == 1U)
    /* process diagnostic interleaved mode */
    if (prot_state_ptr->diagnostic_mode == DIAG_INTERLEAVE_MODE)
    {
        /* To be misra compliant */
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        tl_desc_ptr->diag_interleave_state = DIAG_NO_RESPONSE;
        tl_desc_ptr->interleave_timeout_counter++;
        if (INTERLEAVE_MAX_TIMEOUT < tl_desc_ptr->interleave_timeout_counter)
        {
            active_schedule_struct_ptr = (const lin_schedule_t *)&g_lin_protocol_user_cfg_array[iii].schedule_tbl[master_data_ptr->active_schedule_id];
            /* Switch back to normal table if active schedule table is diagnostic table */
            if (LIN_SCH_TBL_DIAG == active_schedule_struct_ptr->sch_tbl_type)
            {
                /* Switch to previous normal schedule table*/
                lin_tl_sch_switch(iii);
            }
            /* inform service error */
            tl_desc_ptr->service_status = LD_SERVICE_ERROR;
            tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
            prot_state_ptr->diagnostic_mode = DIAG_NONE;
            tl_desc_ptr->interleave_timeout_counter = 0U;
        }
    }
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_single_frame
 * Description   : Process single frame.
 *
 * Implements    : lin_process_single_frame_Activity
 *END**************************************************************************/
static void lin_process_single_frame(l_ifc_handle iii)
{
    l_u8 length;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    const l_u8 * data = prot_state_ptr->response_buffer_ptr;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];

    #if (SUPPORT_MASTER_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_master_data_t * master_data_ptr;
    if (prot_user_config_ptr->function == (bool)LIN_MASTER)
    {
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
    }
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */

    length = (l_u8)(data[1] & 0x0FU);
    /* check length of SF. If not valid, ignore this PDU */
    if (length <= 6U)
    {
        #if (SUPPORT_MASTER_MODE == 1U)
        if ((bool)LIN_MASTER == prot_user_config_ptr->function)
        {
            /* reset queue header and queue tail */
            tl_desc_ptr->tl_rx_queue.queue_header = 0;
            tl_desc_ptr->tl_rx_queue.queue_tail = 0;
            tl_desc_ptr->tl_rx_queue.queue_current_size = 0;
            tl_desc_ptr->tl_rx_queue.queue_status = LD_NO_DATA;
            /* put to rx queue */
            tl_put_raw(iii, data, &(tl_desc_ptr->tl_rx_queue), RECEIVING);

            /* process diagnostic interleaved mode */
            if (prot_state_ptr->diagnostic_mode == DIAG_INTERLEAVE_MODE)
            {
                tl_desc_ptr->diag_interleave_state = DIAG_NOT_START;
                tl_desc_ptr->interleave_timeout_counter = 0;
                prot_state_ptr->diagnostic_mode = DIAG_NONE;
            }

            /* set status is IDLE to receive new message */
            tl_desc_ptr->diag_state = LD_DIAG_IDLE;
            tl_desc_ptr->service_status = LD_SERVICE_IDLE;
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;

            /* process response */
            lin_tl_get_response(iii);

            /* switch to normal table */
            /* To be misra compliant */
            master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
            master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0;
        }
        else
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
        {
        #if (SUPPORT_SLAVE_MODE == 1U)
            /* Slave interface */
            /* put PDU to rx queue */
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->service_status = LD_REQUEST_FINISHED;
            /* reset queue header and queue tail */
            tl_desc_ptr->tl_rx_queue.queue_header = 0;
            tl_desc_ptr->tl_rx_queue.queue_tail = 0;
            tl_desc_ptr->tl_rx_queue.queue_current_size = 0;
            tl_desc_ptr->tl_rx_queue.queue_status = LD_NO_DATA;

            if (data[0] == LD_FUNCTIONAL_NAD)
            {
                tl_desc_ptr->diag_state  = LD_DIAG_RX_FUNCTIONAL;
            }
            else
            {
                tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
            }

            /* put to rx queue */
            tl_put_raw(iii, data, &(tl_desc_ptr->tl_rx_queue), RECEIVING);
            tl_desc_ptr->frame_counter = 1;
            tl_desc_ptr->num_of_pdu = 1;
            lin_tl_attach_service(iii);
        #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        }

        /* If ld_receive_message() function is called before SF pdu to be sent */
        if(tl_desc_ptr->ld_return_data)
        {
            tl_receive_message(iii);
        }
        tl_desc_ptr->FF_pdu_received = false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_first_frame
 * Description   : Process first frame.
 *
 * Implements    : lin_process_first_frame_Activity
 *END**************************************************************************/
static void lin_process_first_frame(l_ifc_handle iii)
{
    l_u16 length;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const l_u8 * data = g_lin_protocol_state_array[iii].response_buffer_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    /* for both MASTER and SLAVE */
    length = (l_u16)((((l_u16)data[1] & 0x0FU) << 8U) + (l_u16)data[2]);
    /* check length of FF. If not valid, ignore this PDU */
    if ((length >= 7U) && (length <= prot_user_config_ptr->max_message_length))
    {
        #if (SUPPORT_SLAVE_MODE == 1U)
        if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
        {
            /* For misra compliant rule 9.1 */
            tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                /* Set check N_Cr timeout */
                tl_desc_ptr->check_timeout = g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].N_Cr_timeout;
                tl_desc_ptr->check_timeout_type = LD_CHECK_N_CR_TIMEOUT;
            }
        }
        #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        /* put PDU to rx queue */
        /* reset queue header and queue tail */
        tl_desc_ptr->tl_rx_queue.queue_header = 0;
        tl_desc_ptr->tl_rx_queue.queue_tail = 0;
        tl_desc_ptr->tl_rx_queue.queue_current_size = 0;
        tl_desc_ptr->tl_rx_queue.queue_status = LD_NO_DATA;
        tl_put_raw(iii, data, &(tl_desc_ptr->tl_rx_queue), RECEIVING);

        /* calculate number of PDU for this message */
        if (((length - 5U) % 6U) == 0U)
        {
            tl_desc_ptr->num_of_pdu = (l_u8)((length - 5U) / 6U);
        }
        else
        {
            tl_desc_ptr->num_of_pdu = (l_u8)(((length - 5U) / 6U) + 1U);
        }

        /* set frame counter = 1 */
        tl_desc_ptr->frame_counter = 1U;
        tl_desc_ptr->service_status = LD_SERVICE_BUSY;

        /* Mark FF pdu is sent */
        tl_desc_ptr->FF_pdu_received = true;
        tl_desc_ptr->interleave_timeout_counter = 0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_consecutive_frame
 * Description   : Process consecutive frame.
 *
 * Implements    : lin_process_consecutive_frame_Activity
 *END**************************************************************************/
static void lin_process_consecutive_frame(l_ifc_handle iii)
{
    l_u8 tmp_frame_counter;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const l_u8 * data = g_lin_protocol_state_array[iii].response_buffer_ptr;

    #if (SUPPORT_MASTER_MODE == 1U)
    /* Only for MASTER */
    lin_master_data_t * master_data_ptr;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    if ((bool)LIN_MASTER == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* To be misra compliant */
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        /* get frame counter of this PDU */
        tmp_frame_counter = (l_u8)(data[1] & 0x0FU);
        /* Reset interleave timeout */
        tl_desc_ptr->interleave_timeout_counter = 0;
        /* Check valid frame counter */
        if (tmp_frame_counter == tl_desc_ptr->frame_counter)
        {
            /* increase frame counter */
            tl_desc_ptr->frame_counter++;
            if (tl_desc_ptr->frame_counter > 15U)
            {
                tl_desc_ptr->frame_counter = 0U;
            }

            /* decrease number of PDU to check message is complete */
            tl_desc_ptr->num_of_pdu--;
            /* put PDU to rx queue */
            tl_put_raw(iii, prot_state_ptr->response_buffer_ptr, &(tl_desc_ptr->tl_rx_queue), RECEIVING);
        }
        else
        {
            /* abort this message */
            if(tl_desc_ptr->ld_return_data)
            {
               tl_desc_ptr->ld_return_data = false;
            }
            tl_desc_ptr->FF_pdu_received = false;
            tl_desc_ptr->rx_msg_status = LD_WRONG_SN;
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            prot_state_ptr->diagnostic_mode = DIAG_NONE;
            /* switch to normal table */
            master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
            master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0U;
        }

        if (0U == tl_desc_ptr->num_of_pdu)
        {
            /* message is received completely */
            /* set status is IDLE to receive new message */
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->service_status = LD_SERVICE_IDLE;

            tl_desc_ptr->diag_state = LD_DIAG_IDLE;
            /* process diagnostic interleaved mode */
            if (prot_state_ptr->diagnostic_mode == DIAG_INTERLEAVE_MODE)
            {
                tl_desc_ptr->diag_interleave_state = DIAG_NOT_START;
                prot_state_ptr->diagnostic_mode = DIAG_NONE;
            }

            /* call function to process this response */
            lin_tl_get_response(iii);
            /* switch to normal table */
            master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
            master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0U;

            /* If ld_receive_message() function is called before FF pdu to be sent */
            if(tl_desc_ptr->ld_return_data)
            {
                tl_receive_message(iii);
            }
            tl_desc_ptr->FF_pdu_received = false;
        }
    }
    else
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */
    {
        #if (SUPPORT_SLAVE_MODE == 1U)
        /* for SLAVE */
        /* For misra compliant rule 9.1 */

        if (g_lin_protocol_user_cfg_array[iii].protocol_version == LIN_PROTOCOL_21)
        {
            /* Set check N_Cr timeout */
            tl_desc_ptr->check_timeout = g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].N_Cr_timeout;
            tl_desc_ptr->check_timeout_type = LD_CHECK_N_CR_TIMEOUT;
        }
        /* get frame counter of this PDU */
        tmp_frame_counter = (l_u8)(data[1] & 0x0FU);
        /* Check valid frame counter */
        if (tmp_frame_counter == tl_desc_ptr->frame_counter)
        {
            /* increase frame counter */
            tl_desc_ptr->frame_counter++;
            if (tl_desc_ptr->frame_counter > 15U)
            {
                tl_desc_ptr->frame_counter = 0U;
            }

            /* decrease number of PDU to check message is complete */
            tl_desc_ptr->num_of_pdu--;
            /* put PDU to rx queue */
            tl_put_raw(iii, data, &(tl_desc_ptr->tl_rx_queue), RECEIVING);
            tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
        }
        else
        {
            /* abort this message */
            if(tl_desc_ptr->ld_return_data)
            {
               tl_desc_ptr->ld_return_data = false;
            }
            tl_desc_ptr->FF_pdu_received = false;
            tl_desc_ptr->rx_msg_status = LD_WRONG_SN;
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
        }

        if (0U == tl_desc_ptr->num_of_pdu)
        {
            /* message is received completely */
            /* set status is IDLE to receive new message */
            tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_desc_ptr->service_status = LD_REQUEST_FINISHED;

            lin_tl_attach_service(iii);
            /* If ld_receive_message() function is called before FF pdu to be sent */
            if(tl_desc_ptr->ld_return_data)
            {
                tl_receive_message(iii);
            }
            tl_desc_ptr->FF_pdu_received = false;
        }
        #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_pdu
 * Description   : Process packet data unit
 *
 * Implements    : lin_process_pdu_Activity
 *END**************************************************************************/
static void lin_process_pdu(l_ifc_handle iii,
                            const lin_tl_pdu_data_t * pdu)
{
    l_u8 pci_type;
#if (SUPPORT_SLAVE_MODE == 1U)
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    if ((bool)LIN_SLAVE == g_lin_protocol_user_cfg_array[iii].function)
    {
        /* clear tx queue */
        tl_desc_ptr->tl_tx_queue.queue_status = LD_QUEUE_EMPTY;
        tl_desc_ptr->tl_tx_queue.queue_current_size = 0U;
        tl_desc_ptr->tl_tx_queue.queue_header = tl_desc_ptr->tl_tx_queue.queue_tail;
    }
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

    /* get PCI type */
    pci_type = (l_u8)(((*pdu)[1] & 0xF0U) >> 4U);
    switch (pci_type)
    {
        case PCI_SF:
            lin_process_single_frame(iii);
            break;
        case PCI_FF:
            lin_process_first_frame(iii);
            break;
        case PCI_CF:
            lin_process_consecutive_frame(iii);
            break;
        default:
            /* ignore this PDU */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_handler_error
 * Description   : This function handles the diagnostic service and control error in TL
 * This function is implemented for Slave and Master.
 *
 * Implements    : lin_tl_handler_error_Activity
 *END**************************************************************************/
static void lin_tl_handler_error(l_ifc_handle iii,
                                 l_u8 pid)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    #if (SUPPORT_MASTER_MODE == 1U)
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    lin_master_data_t * master_data_ptr;

    /* for master */
    if ((bool)LIN_MASTER == prot_user_config_ptr->function)
    {
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        /* Master request */
        if (0x3CU == pid)
        {
            /* update status of message and queue */
            tl_desc_ptr->tx_msg_status = LD_FAILED;
            /* For LIN 2.1 and above*/
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                tl_desc_ptr->tl_tx_queue.queue_status = LD_TRANSMIT_ERROR;
            }
            /* For J2602*/
            else
            {
                tl_desc_ptr->tl_tx_queue.queue_status = LD_TRANSFER_ERROR;
            }
        }
        else
        {
            /* Slave Response */
            /* update status of message and queue */
            tl_desc_ptr->rx_msg_status = LD_FAILED;

            /* For LIN 2.1 and above*/
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                tl_desc_ptr->tl_rx_queue.queue_status = LD_RECEIVE_ERROR;
            }
            /* For J2602*/
            else
            {
                tl_desc_ptr->tl_rx_queue.queue_status = LD_TRANSFER_ERROR;
            }
        } /* end of (0x3C == pid) */

        tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
        tl_desc_ptr->service_status = LD_SERVICE_ERROR;
        if (DIAG_INTERLEAVE_MODE == prot_state_ptr->diagnostic_mode)
        {
            /* switch to normal table */
            master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
            master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0U;

            if(tl_desc_ptr->diag_state == LD_DIAG_TX_INTERLEAVED)
            {
                tl_desc_ptr->diag_state  = LD_DIAG_TX_PHY;
            }
            else if(tl_desc_ptr->diag_state == LD_DIAG_RX_INTERLEAVED)
            {
                tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
            }
            else
            {
                /* clear queue */
                tl_desc_ptr->tl_tx_queue.queue_current_size = 0U;
                tl_desc_ptr->tl_tx_queue.queue_header = tl_desc_ptr->tl_tx_queue.queue_tail;
                tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
            }
            master_data_ptr->send_slave_res_flg = (bool)1U;
            master_data_ptr->send_functional_request_flg = (bool)0U;
        }

        prot_state_ptr->diagnostic_mode = DIAG_NONE;
    }
    else
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */
    {
        /* for slave */
        /* Master request */
        if (0x3CU == pid)
        {
            /* update status of message and queue */
            tl_desc_ptr->rx_msg_status = LD_FAILED;
            /* clear queue */
            tl_desc_ptr->tl_tx_queue.queue_current_size = 0U;
            tl_desc_ptr->tl_tx_queue.queue_header = tl_desc_ptr->tl_tx_queue.queue_tail;

            /* For LIN 2.1 and above*/
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                tl_desc_ptr->tl_rx_queue.queue_status = LD_RECEIVE_ERROR;
            }
            /* For J2602*/
            else
            {
                tl_desc_ptr->tl_rx_queue.queue_status = LD_TRANSFER_ERROR;
            }
        }
        else
        {
            /* Slave Response */
            /* update status of message and queue */
            tl_desc_ptr->tx_msg_status = LD_FAILED;
            /* For LIN 2.1 and above*/
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                tl_desc_ptr->tl_tx_queue.queue_status = LD_TRANSMIT_ERROR;
            }
            /* For J2602*/
            else
            {
                tl_desc_ptr->tl_tx_queue.queue_status = LD_TRANSFER_ERROR;
            }
        } /* end of (0x3C == pid) */

        tl_desc_ptr->service_status = LD_SERVICE_ERROR;
        tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_make_master_request_pdu
 * Description   : This function will be called on Master task in LIN protocol.
 * It create a pointer to PDU data which available for Master request command,
 * correspond to Diag frame to send
 * This function is implemented for Master
 *
 * Implements    : lin_tl_make_master_request_pdu_Activity
 *END**************************************************************************/
static void lin_tl_make_master_request_pdu(l_ifc_handle iii)
{
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];

    tl_get_raw(iii, prot_state_ptr->response_buffer_ptr, &(g_lin_tl_descriptor_array[iii].tl_tx_queue), TRANSMITTING);
    prot_state_ptr->response_length = 8U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_get_pdu
 * Description   : This function copies the current PDU to Response Buffer in Low-level
 * This function is implemented for Slave and Master.
 *
 * Implements    : lin_tl_get_pdu_Activity
 *END**************************************************************************/
static void lin_tl_get_pdu(l_ifc_handle iii)
{
    /* Get data from tx queue */
    tl_get_raw(iii, g_lin_protocol_state_array[iii].response_buffer_ptr, &(g_lin_tl_descriptor_array[iii].tl_tx_queue), TRANSMITTING);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_handler_rx_completed
 * Description   : This function handles the diag service and control event and data in TL
 * This function is implemented for Slave and Master.
 *
 * Implements    : lin_tl_handler_rx_completed_Activity
 *END**************************************************************************/
static void lin_tl_handler_rx_completed(l_ifc_handle iii)
{
    l_u8 pci_type;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    const lin_tl_pdu_data_t * lin_tl_pdu = (const lin_tl_pdu_data_t *)prot_state_ptr->response_buffer_ptr;
    #if (SUPPORT_SLAVE_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    bool invalid_NAD = false;
    if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
    {
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
        invalid_NAD = ((LD_BROADCAST != (*lin_tl_pdu)[0]) && (LD_FUNCTIONAL_NAD != (*lin_tl_pdu)[0]) &&
                       (((*node_attr_ptr->configured_NAD_ptr != (*lin_tl_pdu)[0]) && (SERVICE_ASSIGN_NAD != (*lin_tl_pdu)[2])) ||
                       ((node_attr_ptr->initial_NAD != (*lin_tl_pdu)[0]) && (SERVICE_ASSIGN_NAD == (*lin_tl_pdu)[2]))));
    }
    #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
    pci_type = (l_u8)(((*lin_tl_pdu)[1] & 0xF0U) >> 4U);
    /* check NAD whether or not belongs to this slave node */
    #if (SUPPORT_SLAVE_MODE == 1U)
    if (((bool)LIN_SLAVE == prot_user_config_ptr->function) && (invalid_NAD || (pci_type > PCI_CF)))
    {
        tl_desc_ptr->tl_tx_queue.queue_current_size = 0U;
        tl_desc_ptr->tl_tx_queue.queue_header = tl_desc_ptr->tl_tx_queue.queue_tail;
    }
    else
    #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
    {
        /* ignore Functional request */
        if (!(((tl_desc_ptr->diag_state  == LD_DIAG_RX_PHY) || (tl_desc_ptr->diag_state  == LD_DIAG_TX_PHY)) &&
              ((*lin_tl_pdu)[0] == LD_FUNCTIONAL_NAD)))
        {
            /* check error */
            if (!((tl_desc_ptr->service_status == LD_SERVICE_ERROR) && (pci_type == PCI_CF)))
            {
                /* process received PDU */
                lin_process_pdu(iii, lin_tl_pdu);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_handler_tx_completed
 * Description   : This function process when interface send diagnostic frame successfully
 *
 * Implements    : lin_tl_handler_tx_completed_Activity
 *END**************************************************************************/
static void lin_tl_handler_tx_completed(l_ifc_handle iii)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    #if (SUPPORT_MASTER_MODE == 1U)
    lin_master_data_t * master_data_ptr;
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */

    /* Reset interleave timeout */
    tl_desc_ptr->interleave_timeout_counter = 0;
    /* Check message is sent completely */
    if (0U == tl_desc_ptr->tl_tx_queue.queue_current_size)
    {
        tl_desc_ptr->check_timeout_type = LD_NO_CHECK_TIMEOUT;
        tl_desc_ptr->tx_msg_status = LD_COMPLETED;
        tl_desc_ptr->service_status = LD_REQUEST_FINISHED;
        if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
        {
            tl_desc_ptr->diag_state = LD_DIAG_IDLE;
        }
        #if (SUPPORT_MASTER_MODE == 1U)
        else
        {
            master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            switch (tl_desc_ptr->diag_state)
            {
                case LD_DIAG_TX_PHY:
                    if (master_data_ptr->send_slave_res_flg == (bool)1U)
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
                    }
                    else
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
                        master_data_ptr->send_slave_res_flg = (bool)1U;
                        master_data_ptr->send_functional_request_flg = (bool)0U;
                    }
                    break;
                case LD_DIAG_TX_FUNCTIONAL:
                    tl_desc_ptr->diag_state  = LD_DIAG_IDLE;
                    master_data_ptr->send_slave_res_flg = (bool)1U;
                    master_data_ptr->send_functional_request_flg = (bool)0U;
                    break;
                case LD_DIAG_RX_INTERLEAVED:
                    tl_desc_ptr->diag_state  = LD_DIAG_RX_PHY;
                    master_data_ptr->send_slave_res_flg = (bool)1U;
                    master_data_ptr->send_functional_request_flg = (bool)0U;
                    break;
                default:
                    /* DO nothing */
                    break;
            }
        }
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
    }
    else
    {
        #if (SUPPORT_SLAVE_MODE == 1U)
        if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
        {
            if(prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21)
            {
                tl_desc_ptr->check_timeout = g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].N_As_timeout;
                tl_desc_ptr->check_timeout_type = LD_CHECK_N_AS_TIMEOUT;
            }
        }
        #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        #if (SUPPORT_MASTER_MODE == 1U)
            #if (SUPPORT_SLAVE_MODE == 1U)
        else
            #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        {
            if(tl_desc_ptr->diag_state == LD_DIAG_TX_INTERLEAVED)
            {
                master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
                master_data_ptr->send_functional_request_flg = (bool)0U;
                master_data_ptr->send_slave_res_flg = (bool)1U;
                tl_desc_ptr->diag_state  = LD_DIAG_TX_PHY;
            }
        }
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
    }
    #if (SUPPORT_MASTER_MODE == 1U)
    /* For LIN Master switch back to normal schedule from Master Request Schedule */
    if (prot_user_config_ptr->function == (bool)LIN_MASTER)
    {
        /* Switch to previous normal schedule table*/
        lin_tl_sch_switch(iii);
    }
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */
}

#if (SUPPORT_SLAVE_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_attach_service
 * Description   : This function makes a connection to Diag service class that support for LIN function
 * This function is implemented for Slave only.
 *
 * Implements    : lin_tl_attach_service_Activity
 *END**************************************************************************/
static void lin_tl_attach_service(l_ifc_handle iii)
{
    l_u8 sid;
    l_u8 frame_type;
    l_u8 rx_queue_current_status;
    const lin_transport_layer_queue_t * rx_queue;
    rx_queue = &(g_lin_tl_descriptor_array[iii].tl_rx_queue);

    /* Get rx queue current status */
    rx_queue_current_status = ld_raw_rx_status(iii);
    /* Check whether RX Queue contains data */
    if (((l_u8)LD_DATA_AVAILABLE == rx_queue_current_status) || ((l_u8)LD_QUEUE_FULL == rx_queue_current_status))
    {
        /* get frame type */
        frame_type = (l_u8)((rx_queue->tl_pdu_ptr[rx_queue->queue_header][1U] & 0xF0U) >> 4U);
        /* Get SID information */
        if (frame_type == PCI_FF)
        {
            sid = rx_queue->tl_pdu_ptr[rx_queue->queue_header][3U];
        }
        else
        {
            sid = rx_queue->tl_pdu_ptr[rx_queue->queue_header][2U];
        }

        /* Callback to diagnostic layer to process SID */
        DIAG_SERVICE_CALLBACK_HANDLER(iii, sid);
    }
}

#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

#if (SUPPORT_MASTER_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_get_response
 * Description   : This function checks result of service, it is called when the Slave Response Frame received
 *  This function is implemented for Master only.
 *
 * Implements    : lin_tl_get_response_Activity
 *END**************************************************************************/
static void lin_tl_get_response(l_ifc_handle iii)
{
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
    lin_tl_pdu_data_t lin_tl_pdu;
    l_u8 sci = 0U;
    l_u8 i;
    const lin_transport_layer_queue_t * rx_queue = &(tl_desc_ptr->tl_rx_queue);
    l_u8 frame_type;

    for (i = 0; i < 8U; i++)
    {
        lin_tl_pdu[i] = rx_queue->tl_pdu_ptr[rx_queue->queue_header][i];
    }

    /* get frame type */
    frame_type = (l_u8)(((lin_tl_pdu)[1] & 0xF0U) >> 4U);
    /* Set configuration service status */
    tl_desc_ptr->service_status = LD_SERVICE_IDLE;
    if (frame_type == PCI_SF)
    {
        sci = (lin_tl_pdu)[2];
    }
    else
    {
        if (frame_type == PCI_FF)
        {
            sci = (lin_tl_pdu)[3];
        }
    }

    /* Update last RSID */
    tl_desc_ptr->last_RSID = sci;
    /* Get SID information */
    if (RES_NEGATIVE == sci)
    {
        /* set response configuration status to negative response */
        tl_desc_ptr->last_cfg_result = LD_NEGATIVE;
        /* update error code */
        tl_desc_ptr->ld_error_code = (lin_tl_pdu)[4];

        /* switch to normal schedule table */
        master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;

        tl_desc_ptr->service_status = LD_SERVICE_IDLE;
    }
    else
    {
        sci = (l_u8)(sci - 0x40U);
        /* Check the result of service */
        switch (sci)
        {
            case SERVICE_READ_BY_IDENTIFY:
                /* Assign data received from slave to product_id_ptr */
                ((l_u8 *)tl_desc_ptr->product_id_ptr)[0] = (lin_tl_pdu)[4];
                ((l_u8 *)tl_desc_ptr->product_id_ptr)[1] = (lin_tl_pdu)[3];
                ((l_u8 *)tl_desc_ptr->product_id_ptr)[2] = (lin_tl_pdu)[6];
                ((l_u8 *)tl_desc_ptr->product_id_ptr)[3] = (lin_tl_pdu)[5];
                ((l_u8 *)tl_desc_ptr->product_id_ptr)[4] = (lin_tl_pdu)[7];

                break;
            case SERVICE_TARGET_RESET:
            case SERVICE_ASSIGN_FRAME_ID:
            case SERVICE_ASSIGN_FRAME_ID_RANGE:
            case SERVICE_ASSIGN_NAD:
            case SERVICE_CONDITIONAL_CHANGE_NAD:
            case SERVICE_SAVE_CONFIGURATION:
                break;
            /* check diagnostic service is belong to class II or class III */
            default:
                /* do nothing */
                break;
        }

        /* Set the last configuration result */
        tl_desc_ptr->last_cfg_result = LD_SUCCESS;
        tl_desc_ptr->service_status = LD_SERVICE_IDLE;
        /* switch to normal schedule table */
        master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_sch_switch
 * Description   : This function switch value of active schedule table and previous schedule table
 *                 and reset entry of active schedule table
 *  This function is implemented for Master only.
 *
 * Implements    : lin_tl_sch_switch_Activity
 *END**************************************************************************/
static void lin_tl_sch_switch(l_ifc_handle iii)
{
    l_u8 temp;
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];

    /* Switch to previous normal schedule table*/
    temp = master_data_ptr->previous_schedule_id;
    /* current schedule reaches its next schedule entry point */
    master_data_ptr->previous_schedule_id = master_data_ptr->active_schedule_id;
    /* Insert new schedule at that point of the current schedule */
    master_data_ptr->active_schedule_id = temp;
    /* Assign the start entry of new schedule table by argument entry */
    master_data_ptr->schedule_start_entry_ptr[temp] = 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_handle_interleave_mode
 * Description   : This function switch value of active schedule table and previous schedule table
 *                 and change diagnostic state of Master
 *  This function is implemented for Master only.
 *
 * Implements    : lin_tl_handle_interleave_mode_Activity
 *END**************************************************************************/
static void lin_tl_handle_interleave_mode(l_ifc_handle iii)
{
    l_u8 * active_schedule_id;
    l_u8 * previous_schedule_id;
    lin_master_data_t * master_data_ptr;
    lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    const lin_transport_layer_queue_t * tl_queue;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    /* Get transmit queue */
    tl_queue = &(tl_desc_ptr->tl_tx_queue);

    if (tl_queue->queue_current_size > 0U)
    {
        prot_state_ptr->diagnostic_mode = DIAG_INTERLEAVE_MODE;
    }

    if (DIAG_INTERLEAVE_MODE == prot_state_ptr->diagnostic_mode)
    {
        /* To be misra compliant */
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        active_schedule_id   = &master_data_ptr->active_schedule_id;
        previous_schedule_id = &master_data_ptr->previous_schedule_id;
        *previous_schedule_id = *active_schedule_id;

        if (tl_queue->queue_current_size > 0U)
        {
            switch (tl_desc_ptr->diag_state)
            {
                case LD_DIAG_IDLE:
                    if (master_data_ptr->send_functional_request_flg == (bool)0U)
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_TX_PHY;
                    }
                    else
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_TX_FUNCTIONAL;
                    }
                    break;
                case LD_DIAG_TX_PHY:
                    if (master_data_ptr->send_functional_request_flg == (bool)1U)
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_TX_INTERLEAVED;
                    }
                    break;
                case LD_DIAG_RX_PHY:
                    if (master_data_ptr->send_functional_request_flg == (bool)1U)
                    {
                        tl_desc_ptr->diag_state  = LD_DIAG_RX_INTERLEAVED;
                    }
                    break;
                default:
                    /* DO nothing */
                    break;
            }
        }


        if ((tl_desc_ptr->diag_state == LD_DIAG_TX_PHY) || (tl_desc_ptr->diag_state == LD_DIAG_TX_INTERLEAVED) ||
            (tl_desc_ptr->diag_state == LD_DIAG_RX_INTERLEAVED) || (tl_desc_ptr->diag_state == LD_DIAG_TX_FUNCTIONAL))
        {
            *active_schedule_id = (l_u8)(prot_user_config_ptr->schedule_start + 2U);
        }
        else
        {
            if (tl_desc_ptr->diag_state == LD_DIAG_RX_PHY)
            {
                *active_schedule_id = (l_u8)(prot_user_config_ptr->schedule_start + 3U);
            }
        }
    }
}
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_tl_package_pdu
 * Description   : Pack the information specified by data and length into one or multiple diagnostic pdus.
 *
 * Implements    : lin_tl_package_pdu_Activity
 *END**************************************************************************/
void lin_tl_package_pdu(l_ifc_handle iii,
                        l_u16 length,
                        l_u8 message_size,
                        l_u8 NAD,
                        const l_u8 * const data)
{
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_tl_pdu_data_t pdu;
    l_u8 i;
    l_u16 data_index = 0U;
    l_u16 tmp_length = length;
    l_u16 frame_counter = 0U;

    /* package data */
    if (message_size == 1U)
    {
        /* package single frame */
        /* ____________________________________________ */
        /* | NAD | PCI | SID | D1 | D2 | D3 | D4 | D5 | */
        /* |_____|_____|_____|____|____|____|____|____| */
        if ((bool)LIN_MASTER == prot_user_config_ptr->function)
        {
            pdu[0] = NAD;
        }
    #if (SUPPORT_SLAVE_MODE == 1U)
        else
        {
            pdu[0] = *(g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].configured_NAD_ptr);
        }
    #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        pdu[1] = (l_u8)length;
        pdu[2] = data[0]; /* SID / RSID */
        for (i = 1U; i < length; i++)
        {
            pdu[i + 2U] = data[i]; /* used data */
        }

        for (i = (l_u8)length; i < 6U; i++)
        {
            pdu[i + 2U] = 0xFFU; /* unused data */
        }

        ld_put_raw(iii, pdu);
    }
    else
    {
        /* package first frame */
        /* ____________________________________________ */
        /* | NAD | PCI | LEN |SID | D2 | D3 | D4 | D5 | */
        /* |_____|_____|_____|____|____|____|____|____| */
        if ((bool)LIN_MASTER == prot_user_config_ptr->function)
        {
            pdu[0] = NAD;
        }
    #if (SUPPORT_SLAVE_MODE == 1U)
        else
        {
            pdu[0] = *(g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].configured_NAD_ptr);
        }
    #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        pdu[1] = (l_u8)(((length / 256U) & 0x0FU) | 0x10U); /* PCI */
        pdu[2] = (l_u8)(length % 256U); /* length */
        pdu[3] = data[0]; /* SID / RSID */
        for (i = 1U; i < 5U; i++)
        {
            /* data */
            pdu[i + 3U] = data[i];
        }

        data_index += 5U;
        tmp_length -= 5U;
        ld_put_raw(iii, pdu);

        /* package consecutive frame */
        /* ___________________________________________ */
        /* | NAD | PCI | D1 | D2 | D3 | D4 | D5 | D6 | */
        /* |_____|_____|____|____|____|____|____|____| */
        message_size--;
        if ((bool)LIN_MASTER == prot_user_config_ptr->function)
        {
            pdu[0] = NAD;
        }
    #if (SUPPORT_SLAVE_MODE == 1U)
        else
        {
            pdu[0] = *(g_lin_node_attribute_array[g_lin_protocol_user_cfg_array[iii].slave_ifc_handle].configured_NAD_ptr);
        }
    #endif /* End (SUPPORT_SLAVE_MODE == 1U) */
        while (message_size > 0U)
        {
            frame_counter++;
            pdu[1] = (l_u8)(0x20U | (frame_counter & 0x0FU));
            if (tmp_length < 6U)
            {
                /* last PDU */
                /* used data */
                for (i = 0U; i < tmp_length; i++)
                {
                    pdu[i + 2U] = data[data_index];
                    data_index++;
                }

                /* unused data */
                for (i = (l_u8)tmp_length; i < 6U; i++)
                {
                    pdu[i + 2U] = 0xFFU;
                }
            }
            else
            {
                for (i = 2U; i < 8U; i++)
                {
                    pdu[i] = data[data_index];
                    data_index++;
                }

                tmp_length -= 6U;
            } /* end of (tmp < 6) */

            message_size--;
            ld_put_raw(iii, pdu);
        } /* (message_size > 0) */
    } /* (length <= 6) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : tl_put_raw
 * Description   : Copy the specified raw data to the specified queue
 *
 * Implements    : tl_put_raw_Activity
 *END**************************************************************************/
void tl_put_raw(l_ifc_handle iii,
                const l_u8 * const data,
                lin_transport_layer_queue_t * queue,
                l_u8 direction)
{
    lin_tl_pdu_data_t * queue_data;
    l_u8 i;

#if (SUPPORT_MASTER_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_master_data_t * master_data_ptr;
#else
    (void)iii;
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

    /* Get transmit queue */
    queue_data = queue->tl_pdu_ptr;

    /* Use a critical section to make sure queue is updated completely before we handle any IRQs */
    DISABLE_INTERRUPTS();
    /* check status of queue */
    if ((queue->queue_current_size < queue->queue_max_size) && ((data[0] != LD_FUNCTIONAL_NAD) || (queue_data[queue->queue_header][0] != LD_FUNCTIONAL_NAD)))
    {
        /* process queue tail */
        if ((LD_QUEUE_EMPTY == queue->queue_status) || (LD_NO_DATA == queue->queue_status))
        {
            /* Set status of queue */
            if (TRANSMITTING == direction)
            {
                queue->queue_status = LD_QUEUE_AVAILABLE;
            }
            else
            {
                queue->queue_status = LD_DATA_AVAILABLE;
            }
        }
        else
        {
            if ((data[0] == LD_FUNCTIONAL_NAD) && (TRANSMITTING == direction))
            {
                if (0U == queue->queue_header)
                {
                    queue->queue_header = queue->queue_max_size;
                }
                queue->queue_header--;
            }
            else
            {
                queue->queue_tail++;
                /* Check if queue size equal to queue tail */
                if (queue->queue_max_size == queue->queue_tail)
                {
                    queue->queue_tail = 0U;
                }
            }
        }

        #if (SUPPORT_MASTER_MODE == 1U)
        if ((data[0] == LD_FUNCTIONAL_NAD) && (TRANSMITTING == direction) && (prot_user_config_ptr->function == (bool)LIN_MASTER))
        {
            master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            for (i = 0U; i < 8U; i++)
            {
                queue_data[queue->queue_header][i] = data[i];
            }

            /* Set Send Functional Request Flag */
            master_data_ptr->send_functional_request_flg = (bool)1U;
            /* Clear Send slave response flag */
            master_data_ptr->send_slave_res_flg = (bool)0U;
        }
        else
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
        {
            /* copy data to queue */
            for (i = 0U; i < 8U; i++)
            {
                queue_data[queue->queue_tail][i] = data[i];
            }
        }

        /* update queue status */
        queue->queue_current_size++;
        if (queue->queue_current_size >= queue->queue_max_size)
        {
            queue->queue_status = LD_QUEUE_FULL;
        }
    }
    ENABLE_INTERRUPTS();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : tl_get_raw
 * Description   : Get the raw data from the specified queue
 *
 * Implements    : tl_get_raw_Activity
 *END**************************************************************************/
void tl_get_raw(l_ifc_handle iii,
                l_u8 * const data,
                lin_transport_layer_queue_t * queue,
                l_u8 direction)
{
    const lin_transport_layer_queue_t * tl_queue;
    const lin_tl_pdu_data_t * queue_data;
    l_u8 i;
    l_u16 queue_header;
    tl_queue = queue;
    /* Get queue */
    queue_data = (const lin_tl_pdu_data_t *)tl_queue->tl_pdu_ptr;

    /* Check queue status (is the queue contains data? if yes: get data from queue and put them to buffer) */
    if (0U != queue->queue_current_size)
    {
        /* get data from head queue */
        queue_header = tl_queue->queue_header;
        for (i = 0U; i < 8U; i++)
        {
            data[i] = queue_data[queue_header][i];
        }

        /* update queue status */
        if (0U == (--queue->queue_current_size))
        {
            if (TRANSMITTING == direction)
            {
                queue->queue_status = LD_QUEUE_EMPTY;
            }
            else
            {
                queue->queue_status = LD_NO_DATA;
            }
        }
        else
        {
            if (TRANSMITTING == direction)
            {
                queue->queue_status = LD_QUEUE_AVAILABLE;
            }
            else
            {
                queue->queue_status = LD_DATA_AVAILABLE;
            }
            /* change queue header */
            queue->queue_header++;
            if (queue->queue_header == queue->queue_max_size)
            {
                queue->queue_header = 0U;
            }
        }
    }
    (void)iii;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : tl_receive_message
 * Description   : Get data from rx_queue and store it in data pointer of
 *                 ld_receive_message() function
 *
 * Implements    : tl_receive_message_Activity
 *END**************************************************************************/
static void tl_receive_message(l_ifc_handle iii)
{
  lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_tl_pdu_data_t pdu;
    l_u16 i;
    l_u16 data_index = 0U;
    l_u16 tmp_length = 0U;
    l_u8 PCI_type;
    l_u8* data = tl_desc_ptr->receive_message_ptr;
    l_u8* NAD = tl_desc_ptr->receive_NAD_ptr;
    l_u16* length = tl_desc_ptr->receive_message_length_ptr;

    if (LD_NO_DATA != tl_desc_ptr->tl_rx_queue.queue_status)
    {
        /* Message is received completely */
        /* get data from receive queue */
        /*
        * @violates MISRA 2012 Required Rule 1.3 Required
        */
        ld_get_raw(iii, pdu);
        /* Analyze data */
        if ((bool)LIN_MASTER == prot_user_config_ptr->function)
        {
            *NAD = pdu[0];
        }

        /* Check type of pdu */
        PCI_type = (l_u8)((pdu[1] & 0xF0U) >> 4U);
        switch (PCI_type)
        {
            /* Single frame */
            case PCI_SF:
                tmp_length = (l_u16)((l_u16)pdu[1] & 0x000FU);
                i = *length;
                *length = tmp_length;
                if (i < tmp_length)
                {
                    tmp_length = i;
                }

                data[0] = pdu[2];
                for (i = 1U; i < tmp_length; i++)
                {
                    data[i] = pdu[i + 2U];
                }
                break;
            /* Multi frame */
            case PCI_FF:
                /* First frame */
                tmp_length = (l_u16)((((l_u16)pdu[1] & 0x000FU) << 8) + (l_u16)pdu[2]);
                i = *length;
                *length = tmp_length;
                if (i < tmp_length)
                {
                    tmp_length = i;
                }
                data[0] = pdu[3];
                for (i = 1U; i < 5U; i++)
                {
                    data[i] = pdu[i + 3U];
                }

                tmp_length -= 5U;
                data_index += 5U;

                /* Consecutive frame */
                while (tmp_length > 6U)
                {
                    /* get PDU */
                    /*
                    * @violates MISRA 2012 Required Rule 1.3 Required
                    */
                    ld_get_raw(iii, pdu);
                    for (i = 2U; i < 8U; i++)
                    {
                        data[data_index] = pdu[i];
                        data_index++;
                    }

                    tmp_length -= 6U;
                }

                /* Last frame */
                if (tmp_length > 0U)
                {
                    /* get PDU */
                    /*
                    * @violates MISRA 2012 Required Rule 1.3 Required
                    */
                    ld_get_raw(iii, pdu);
                    for (i = 0U; i < tmp_length; i++)
                    {
                        data[data_index] = pdu[i + 2U];
                        data_index++;
                    }
                }
                break;
            default:
                /* do nothing */
                break;
        } /* end of switch */
        tl_desc_ptr->ld_return_data = false;
        tl_desc_ptr->rx_msg_status = LD_COMPLETED;
    }
}

#endif /*end (SUPPORT_TRANSPORT_LAYER == 1U) */
/*******************************************************************************
 * EOF
 ******************************************************************************/
