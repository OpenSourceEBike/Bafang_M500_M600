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


#include "lin_lin21_proto.h"
#include "lin_common_proto.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (SUPPORT_MASTER_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_collision_resolve
 * Description   : Switch to collision resolve table.
 *
 * Implements    : lin_collision_resolve_Activity
 *END**************************************************************************/
void lin_collision_resolve(l_ifc_handle iii,
                           l_u8 pid)
{
    const lin_associate_frame_t * ptr;
    l_u8 frame_index;
    l_u8 active_schedule_id;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
    frame_index = lin_get_frame_index(iii, pid);
    /* Check MASTER function */
    if ((frame_index != 0xFFU) && ((bool)LIN_MASTER == prot_user_config_ptr->function))
    {
        /* Find collision resolver table */
        /* Swap schedule table */
        /* Get active_schedule_id */
        active_schedule_id = master_data_ptr->active_schedule_id;
        master_data_ptr->previous_schedule_id = active_schedule_id;

        /* Set collision resolver table to active schedule */
        ptr = prot_user_config_ptr->frame_tbl_ptr[frame_index].frame_data_ptr;

        master_data_ptr->active_schedule_id = ptr->coll_resolv_schd;

        master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0U;
    }
}
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_word_status_lin21
 * Description   : Update node status flags.
 *
 * Implements    : lin_update_word_status_lin21_Activity
 *END**************************************************************************/
void lin_update_word_status_lin21(l_ifc_handle iii,
                                  lin_lld_event_id_t event_id)
{
    l_u8 err_in_res;
    l_u8 suc_in_tras;
    l_u8 save_config;
    l_u8 overrun_flag;
    lin_word_status_str_t * word_status_ptr;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (SUPPORT_MASTER_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_master_data_t * master_data_ptr;
    l_u8 active_schedule_id;
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

    err_in_res = prot_state_ptr->error_in_response;
    prot_state_ptr->error_in_response = 0;
    suc_in_tras = prot_state_ptr->successful_transfer;
    /* use of ternary operator below is for being MISRA compliant */
    save_config = (l_u8)(prot_state_ptr->save_config_flg ? 1U : 0U);
    overrun_flag = prot_state_ptr->overrun_flg;

    word_status_ptr = &prot_state_ptr->word_status;
    word_status_ptr->overrun = overrun_flag;

    /* values for all processed frames */
    word_status_ptr->error_in_res |= err_in_res;
    word_status_ptr->successful_transfer |= suc_in_tras;
    word_status_ptr->last_pid = prot_state_ptr->last_pid;

    if(prot_state_ptr->go_to_sleep_flg == true)
    {
        /* Set go to sleep flag for word status*/
        word_status_ptr->go_to_sleep_flg = 1;
    }

    if(LIN_LLD_BUS_ACTIVITY_TIMEOUT != event_id)
    {
        /* Set bus activity timeout */
        word_status_ptr->bus_activity = 1U;
    }

#if (SUPPORT_MASTER_MODE == 1U)
    if ((bool)LIN_MASTER == prot_user_config_ptr->function)
    {
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        /* If is Master node, value always equal 0 */
        word_status_ptr->save_config_flg = 0U;


        active_schedule_id = master_data_ptr->active_schedule_id;
        /* If collision schedule table is executing, set event_trigger_collision_flag bit field*/
        if (prot_user_config_ptr->schedule_tbl[active_schedule_id].sch_tbl_type == LIN_SCH_TBL_COLL_RESOLV)
        {
            word_status_ptr->event_trigger_collision_flg = 1;
        }
        else
        {
            word_status_ptr->event_trigger_collision_flg = 0;
        }
    }
    else
#endif /* End (SUPPORT_MASTER_MODE == 1U) */
    {
        /* For Slave node - using ternary operator is a workaround for MISRA */
        word_status_ptr->event_trigger_collision_flg = 0U;
        /* Updated save configuration value to word status */
        word_status_ptr->save_config_flg = save_config;
        word_status_ptr->reserved = 0U;
    }
}

#if (SUPPORT_SLAVE_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_err_signal
 * Description   : Updates error in response flag for frames different than Event Frames
 *
 * Implements    : lin_update_err_signal_Activity
 *END**************************************************************************/
void lin_update_err_signal(l_ifc_handle iii,
                           l_u8 frm_id)
{
    l_u8 i;
    const l_u16* byte_offset_temp_ptr;
    const l_u8* bit_offset_temp_ptr;
    l_u8 err_in_res = g_lin_protocol_state_array[iii].error_in_response;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    const lin_node_attribute_t * node_attr_ptr;
    node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];

    if (frm_id < LIN_NUM_OF_FRMS)
    {
        /* Set error signal equal to error in response */
        if (LIN_FRM_EVNT != prot_user_config_ptr->frame_tbl_ptr[frm_id].frm_type)
        {
            for (i = 0; i < node_attr_ptr->num_frame_have_esignal; i++)
            {
                byte_offset_temp_ptr = &node_attr_ptr->response_error_byte_offset_ptr[i];
                bit_offset_temp_ptr = &node_attr_ptr->response_error_bit_offset_ptr[i];
                g_lin_frame_data_buffer[*byte_offset_temp_ptr] = (l_u8)((g_lin_frame_data_buffer[*byte_offset_temp_ptr] & (~(1U << (*bit_offset_temp_ptr)))) |
                                                        ((l_u8)(err_in_res << (*bit_offset_temp_ptr))));
            }
        }
    }
}

#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_rx_evnt_frame
 * Description   : The function updates the receive flags associated with signals/frames
 * in case receive an event trigger frame.
 *
 * Implements    : lin_update_rx_evnt_frame_Activity
 *END**************************************************************************/
void lin_update_rx_evnt_frame(l_ifc_handle iii,
                              l_u8 pid)
{
    const lin_associate_frame_t * ptr;
    l_u8 frame_index, i, associate_id;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    /* Get frame index of the event trigger frame */
    frame_index = lin_get_frame_index(iii, pid);

    if (0xFFU != frame_index) /* This PID doesn't belong to this node */
    {
        /* calculate PID that associated with ETF */
        associate_id = lin_process_parity(g_lin_protocol_state_array[iii].response_buffer_ptr[0], CHECK_PARITY);
        ptr = prot_user_config_ptr->frame_tbl_ptr[frame_index].frame_data_ptr;

        for (i = 0U; i < ptr->num_of_associated_uncond_frames; i++)
        {
            /* First byte of response for event trigger is the PID of the associate frame */
            if (prot_user_config_ptr->list_identifiers_RAM_ptr[((l_u8)ptr->associated_uncond_frame_ptr[i] - prot_user_config_ptr->frame_start) + 1U] == associate_id)
            {
                /* Update unconditional frame */
                lin_update_uncd_frame(iii, associate_id);
                break;
            }
        }
    }
}

#if (SUPPORT_SLAVE_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_make_res_evnt_frame
 * Description   : This function packs signals associated with event trigger frame into buffer.
 *
 * Implements    : lin_make_res_evnt_frame_Activity
 *END**************************************************************************/
void lin_make_res_evnt_frame(l_ifc_handle iii,
                             l_u8 pid)
{
    /* Create response */
    lin_make_uncd_frame(iii, pid);
    /* Set associate PID */
    g_lin_protocol_state_array[iii].response_buffer_ptr[0] = lin_process_parity(pid, MAKE_PARITY);
}

#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
