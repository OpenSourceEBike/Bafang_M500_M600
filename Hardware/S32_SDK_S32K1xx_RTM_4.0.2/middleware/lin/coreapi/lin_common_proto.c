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


#include "lin_common_api.h"
#include "lin_common_proto.h"
#include "lin_lin21_proto.h"
#include "lin_j2602_proto.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
/*
 * @brief Handle the received ID and decide the next action of Stack.
 *
 * @param iii Interface name
 * @param id ID to process
 * @return void
 */
static void lin_process_id(l_ifc_handle iii,
                           l_u8 id);

static void lin_update_rx(l_ifc_handle iii,
                          l_u8 id);

static void lin_update_tx(l_ifc_handle iii,
                          l_u8 id);

static void lin_handle_error(l_ifc_handle iii,
                             lin_lld_event_id_t event_id,
                             l_u8 id);

static void lin_bus_activity_timeout(l_ifc_handle iii);

static void lin_update_tx_flags(l_ifc_handle iii,
                                l_u8 frm_id);

#if ((1U == SUPPORT_SLAVE_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13)))
static l_u8 lin_process_evnt_frame(l_ifc_handle iii,
                                   l_u8 id);
#endif /* ((1U == SUPPORT_SLAVE_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13))) */

static l_u8 lin_process_diag_frame(l_ifc_handle iii,
                                   l_u8 id);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_pid_resp_callback_handler
 * Description   : Callback handler for LIN low level events.
 *
 * Implements    : lin_pid_resp_callback_handler_Activity
 *END**************************************************************************/
void lin_pid_resp_callback_handler(l_ifc_handle iii,
                                   const lin_lld_event_id_t event_id,
                                   l_u8 id)
{
    switch (event_id)
    {
        case LIN_LLD_PID_OK:
            lin_process_id(iii, id);
            break;
        case LIN_LLD_TX_COMPLETED:
            lin_update_tx(iii, id);
            break;
        case LIN_LLD_RX_COMPLETED:
            lin_update_rx(iii, id);
            break;
        case LIN_LLD_BUS_ACTIVITY_TIMEOUT:
            lin_bus_activity_timeout(iii);
            break;
        case LIN_LLD_PID_ERR:
        case LIN_LLD_FRAME_ERR:
        case LIN_LLD_CHECKSUM_ERR:
        case LIN_LLD_READBACK_ERR:
        case LIN_LLD_NODATA_TIMEOUT:
            lin_handle_error(iii, event_id, id);
            break;
        default:
            /* do nothing */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_id
 * Description   : Handle the received ID and decide the next action of Stack.
 *
 * Implements    : lin_process_id_Activity
 *END**************************************************************************/
static void lin_process_id(l_ifc_handle iii,
                           l_u8 id)
{
    l_u8 action = 0U;
    const lin_frame_t * lin_frame_ptr;
    l_u8 frame_index;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];

    frame_index = lin_get_frame_index(iii, id);
    lin_frame_ptr = &(prot_user_config_ptr->frame_tbl_ptr[frame_index]);
    if (0xFFU != frame_index)
    {
        action = 1U;
        prot_state_ptr->last_pid = lin_process_parity(id,MAKE_PARITY);
        /* PID belongs to this node, then check type of frame */
        switch (lin_frame_ptr->frm_type)
        {
            /* Unconditional frame */
            case LIN_FRM_UNCD:
                if (LIN_RES_PUB == lin_frame_ptr->frm_response)
                {
                    lin_make_uncd_frame(iii, id);
                    /* Set response */
                    action = 2U;
                }
                break;
            #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13))
            /* Event trigger frame */
            case LIN_FRM_EVNT:
            #if (1U == SUPPORT_SLAVE_MODE)
                if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
                {
                    action = lin_process_evnt_frame(iii,id);
                }
            #endif /* (1U == SUPPORT_SLAVE_MODE) */
                break;
            #endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13)) */

            /* Diagnostic frame */
            case LIN_FRM_DIAG:
                if(LIN_PROTOCOL_13 != prot_user_config_ptr->protocol_version)
                {
                    action = lin_process_diag_frame(iii,id);
                }
                else
                {
                    action = 0U;
                }
                break;
            default:
                /* do nothing */
                break;
        }
    }

    switch (action)
    {
        case 1U:
            /* Receive response */
            (void)lin_lld_rx_response(iii, lin_frame_ptr->frm_len);
            break;
        case 2U:
            /* Set response */
            (void)lin_lld_set_response(iii, lin_frame_ptr->frm_len);
            break;
        default:
            /* ignore response */
            (void)lin_lld_ignore_response(iii);
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_rx
 * Description   : This function update signal, status, and flag after received response.
 *
 * Implements    : lin_update_rx_Activity
 *END**************************************************************************/
static void lin_update_rx(l_ifc_handle iii,
                          l_u8 id)
{
    l_u8 frame_index, flag_size, i, associate_id;
    l_u16 flag_offset;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (1U == SUPPORT_MASTER_MODE)
    lin_master_data_t * master_data_ptr;
#endif /* (1U == SUPPORT_MASTER_MODE) */

    /* Set successful transfer */
    prot_state_ptr->successful_transfer = 1U;
    /* if two or more frames are processed since the previous call to l_ifc_read_status, set overrun flag to 1 */
    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        prot_state_ptr->num_of_processed_frame++;
    }

    if ((prot_state_ptr->num_of_processed_frame > 1U) && (prot_state_ptr->overrun_flg == 0U))
    {
        prot_state_ptr->overrun_flg = 1U;
    }

    frame_index = lin_get_frame_index(iii, id);
    if (frame_index < LIN_NUM_OF_FRMS)
    {
        /* This PID doesn't belong to this node */
        /* PID belongs to this node, then check type of frame */
        switch (prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_type)
        {
            /* Unconditional frame */
            case LIN_FRM_UNCD:
                lin_update_uncd_frame(iii, id);
                break;
            /* Event trigger frame */
            case LIN_FRM_EVNT:
                lin_update_rx_evnt_frame(iii, id);
                /* Recalculate frame_index by updating associate PID */
                associate_id = lin_process_parity(prot_state_ptr->response_buffer_ptr[0], CHECK_PARITY);
                frame_index = lin_get_frame_index(iii, associate_id);
                break;
            /* Diagnostic frame */
            case LIN_FRM_DIAG:
                /* If goto sleep signal */
                if (0U == prot_state_ptr->response_buffer_ptr[0])
                {
                    /* In a slave node, set go to sleep flag in state structure */
                    prot_state_ptr->go_to_sleep_flg = true;
                    (void)lin_lld_set_low_power_mode(iii);
                }
                else
                {
                    /* Call transport layer callback to process TL_RX_COMPLETED event*/
                    (void)LIN_TL_CALLBACK_HANDLER(iii, TL_RX_COMPLETED, id);
                }

                break;
            default:
                /* do nothing */
                break;
        }

        if (frame_index < LIN_NUM_OF_FRMS)
        {
            /* Update rx frame flag */
            g_lin_frame_flag_handle_tbl[frame_index] = true;

            if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
            {
                /* Update rx signal flags */
                flag_offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_offset;
                flag_size = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_size;
                for (i = 0U; i < flag_size; i++)
                {
                    g_lin_flag_handle_tbl[flag_offset] = 0xFFU;
                    flag_offset++;
                }
            }
        #if (1U == SUPPORT_MASTER_MODE)
            else /* If master node */
            {
                master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
                master_data_ptr->flag_offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_offset;
                master_data_ptr->flag_size = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_size;
            }
        #endif /* (1U == SUPPORT_MASTER_MODE) */
        }

    #if (1U == SUPPORT_SLAVE_MODE)
        /* update word status */
        if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
        {
            if ((prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21) || (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_20))
            {
                lin_update_word_status_lin21(iii, LIN_LLD_RX_COMPLETED);
            }
            else
            {
                if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602)
                {
                    lin_update_word_status_j2602(iii);
                }
            }
        }
    #endif /* (1U == SUPPORT_SLAVE_MODE) */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_tx
 * Description   : This function update status and flags after transmit response
 *
 * Implements    : lin_update_tx_Activity
 *END**************************************************************************/
static void lin_update_tx(l_ifc_handle iii,
                          l_u8 id)
{
    l_u8 frame_index;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (1U  == SUPPORT_MASTER_MODE)
    const lin_master_data_t * master_data_ptr;
#endif /* (1U  == SUPPORT_MASTER_MODE) */
#if (1U == SUPPORT_SLAVE_MODE)
    const lin_node_attribute_t * node_attr_ptr;
    l_u8 i;
#if (1U == SUPPORT_PROTOCOL_J2602)
    l_u8* bit_offset_temp_ptr;
    l_u16* byte_offset_temp_ptr;
#endif /* ((1U == SUPPORT_PROTOCOL_J2602) */
#endif /* (1U == SUPPORT_SLAVE_MODE) */

    /* Set successful transfer */
    prot_state_ptr->successful_transfer = 1U;
    /* if two or more frames are processed since the previous call to l_ifc_read_status, set overrun flag to 1 */
    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        prot_state_ptr->num_of_processed_frame++;
    }

    if ((prot_state_ptr->num_of_processed_frame > 1U) && (prot_state_ptr->overrun_flg == 0U))
    {
        prot_state_ptr->overrun_flg = 1U;
    }

#if (1U == SUPPORT_MASTER_MODE)
    /* If the Master node sent go to sleep command successfully, then set go to sleep flag*/
    if (((bool)LIN_MASTER == prot_user_config_ptr->function) && (0x3CU == id))
    {
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
        /* At this point, the schedule table is LIN_SCH_TBL_NULL */
        if (prot_user_config_ptr->schedule_tbl[master_data_ptr->active_schedule_id].sch_tbl_type == LIN_SCH_TBL_NULL)
        {
            /* Set go to sleep flag in state structure */
            prot_state_ptr->go_to_sleep_flg = true;
            (void)lin_lld_set_low_power_mode(iii);
        }
    }
#endif /* (1U == SUPPORT_MASTER_MODE) */
    /* Find frame index by pid */
    frame_index = lin_get_frame_index(iii, id);
    if ((((bool)LIN_MASTER == prot_user_config_ptr->function) && (0x3CU == id) && (DIAG_INTERLEAVE_MODE == prot_state_ptr->diagnostic_mode)) ||
        (((bool)LIN_SLAVE == prot_user_config_ptr->function) && (0x3DU == id)))
    {
        /* Call transport layer callback to process TL_TX_COMPLETED event*/
        (void)LIN_TL_CALLBACK_HANDLER(iii, TL_TX_COMPLETED, id);
    }
    if (frame_index < LIN_NUM_OF_FRMS)
    {
        /* Update signal flag */
        lin_update_tx_flags(iii, frame_index);
        /* Update tx frame flag */
        g_lin_frame_flag_handle_tbl[frame_index] = true;
    }

#if (1U == SUPPORT_SLAVE_MODE)
    /* Update word status */
    if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
    {
        node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
    #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
        if ((prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21) || (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_20))
        {
            for (i = 0U; i < node_attr_ptr->num_frame_have_esignal; i++)
            {
                /* Check if frame contain error signal */
                if (id == node_attr_ptr->resp_err_frm_id_ptr[i])
                {
                    /* Clear error signal in frame data buffer */
                    prot_state_ptr->error_in_response = 0U;
                    lin_update_err_signal(iii, frame_index);
                    break;
                }
            }
            lin_update_word_status_lin21(iii, LIN_LLD_TX_COMPLETED);
        }
    #endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */
    #if (1U == SUPPORT_PROTOCOL_J2602)
        if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602)
        {
            /* This status byte which contains error signal shall be transmitted as the first byte of every slave transmission */
            if (0x3DU != id)
            {
                /* Clear error signal in frame data buffer */
                prot_state_ptr->error_in_response = 0U;
                for (i = 0; i < node_attr_ptr->num_frame_have_esignal; i++)
                {
                    byte_offset_temp_ptr = node_attr_ptr->response_error_byte_offset_ptr + i;
                    bit_offset_temp_ptr = node_attr_ptr->response_error_bit_offset_ptr + i;
                    /* Clear error signal in frame data buffer */
                    g_lin_frame_data_buffer[*byte_offset_temp_ptr] = (l_u8)(g_lin_frame_data_buffer[*byte_offset_temp_ptr] & (~(0x07U << (*bit_offset_temp_ptr))));
                }
            }
            lin_update_word_status_j2602(iii);
        }
    #endif /* (1U == SUPPORT_PROTOCOL_J2602) */
    }
#endif /* (1U == SUPPORT_SLAVE_MODE) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_handle_error
 * Description   : This function handle error raised by low-level
 *
 * Implements    : lin_handle_error_Activity
 *END**************************************************************************/
static void lin_handle_error(l_ifc_handle iii,
                             lin_lld_event_id_t event_id,
                             l_u8 id)
{
#if ((1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_SLAVE_MODE))
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
#endif /* ((1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_SLAVE_MODE)) */
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];

#if ((1U == SUPPORT_MASTER_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)))
    lin_master_data_t * master_data_ptr;
#endif /* ((1U == SUPPORT_MASTER_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))) */

#if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
    l_u8 frame_index;
    frame_index = lin_get_frame_index(iii, id);
#endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

    /* if two or more frames are processed since the previous call to l_ifc_read_status, set overrun flag to 1 */
    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        prot_state_ptr->num_of_processed_frame++;
    }

    if ((prot_state_ptr->num_of_processed_frame > 1U) && (prot_state_ptr->overrun_flg == 0U))
    {
        prot_state_ptr->overrun_flg = 1U;
    }

    switch (event_id)
    {
        /* PID error */
        case LIN_LLD_PID_ERR:
            /* do nothing here */
            break;
        /* Frame error */
        case LIN_LLD_FRAME_ERR:
        case LIN_LLD_CHECKSUM_ERR:
        case LIN_LLD_READBACK_ERR:
        case LIN_LLD_NODATA_TIMEOUT:
        #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
            /* Check if frame type is event trigger */
            /* Find the corresponding frame */

            if (LIN_FRM_EVNT == prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_type)
            {
            #if (1U == SUPPORT_MASTER_MODE)
                if (prot_user_config_ptr->function == (bool)LIN_MASTER)
                {
                    master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
                    master_data_ptr->event_trigger_collision_flg  = true;
                }
            #endif /* (1U == SUPPORT_MASTER_MODE) */
            }
            else
        #endif  /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */
            {
                /* Set response error */
                prot_state_ptr->error_in_response = 1U;
                /* TL support */
                if ((0x3CU == id) || (0x3DU == id))
                {
                    /* Call transport layer callback to handle TL_ERROR */
                    (void)LIN_TL_CALLBACK_HANDLER(iii, TL_ERROR, id);
                }
            }
            break;
        default:
            /* do nothing */
            break;
    }

#if (1U == SUPPORT_SLAVE_MODE)
    /* Update word status */
    if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
    {
    #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
        if ((prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21) || (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_20))
        {
            if (prot_state_ptr->error_in_response == 1U)
            {
                lin_update_err_signal(iii, frame_index);
            }
            lin_update_word_status_lin21(iii, event_id);
        }
    #endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */
    #if (1U == SUPPORT_PROTOCOL_J2602)
        if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602)
        {
            lin_update_status_byte(iii, event_id);
            lin_update_word_status_j2602(iii);
        }
    #endif /* (1U == SUPPORT_PROTOCOL_J2602) */
    }
#endif /* (1U == SUPPORT_SLAVE_MODE) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_bus_activity_timeout
 * Description   : This function set bus activity timeout
 *
 * Implements    : lin_bus_activity_timeout_Activity
 *END**************************************************************************/
static void lin_bus_activity_timeout(l_ifc_handle iii)
{
    /* Set goto sleep mode */
#if (1U == SUPPORT_SLAVE_MODE)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
    {
    #if ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20))
        if ((prot_user_config_ptr->protocol_version == LIN_PROTOCOL_21) || (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_20))
        {
            lin_update_word_status_lin21(iii, LIN_LLD_BUS_ACTIVITY_TIMEOUT);
        }
    #endif /* ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20)) */

    #if (1U == SUPPORT_PROTOCOL_J2602)
        if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602)
        {
            lin_update_word_status_j2602(iii);
        }

    #endif /* (1U == SUPPORT_PROTOCOL_J2602) */
    }

#endif /* (1U == SUPPORT_SLAVE_MODE) */

    /* Change to low power mode */
    (void)lin_lld_set_low_power_mode(iii);
}

#if (1U == SUPPORT_MASTER_MODE)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_switch_sch_table
 * Description   : Check to see if the current schedule table is needed to change
 * or not. Used in tick callback function.
 *
 * Implements    : lin_switch_sch_table_Activity
 *END**************************************************************************/
void lin_switch_sch_table(l_ifc_handle iii)
{
    l_u8 * active_schedule_id;
    const l_u8 * previous_schedule_id;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
    /* Get current configuration */
    active_schedule_id   = &master_data_ptr->active_schedule_id;
    previous_schedule_id = &master_data_ptr->previous_schedule_id;

    /* Get transmit queue */
    switch (prot_user_config_ptr->schedule_tbl[master_data_ptr->active_schedule_id].sch_tbl_type)
    {
        /* Collision */
        case LIN_SCH_TBL_COLL_RESOLV:
            /* Set active table equal to previous table */
            *active_schedule_id = *previous_schedule_id;
            break;
        /* Diagnostic schedule table */
        case LIN_SCH_TBL_DIAG:
            /* Set start entry of active schedule table to 0 */
            master_data_ptr->schedule_start_entry_ptr[*active_schedule_id] = 0U;
            break;
        /* Normal schedule table */
        case LIN_SCH_TBL_NORM:
            (void)LIN_TL_CALLBACK_HANDLER(iii, TL_HANDLER_INTERLEAVE_MODE, 0);
            /* Set start entry of active schedule table to 0 */
            master_data_ptr->schedule_start_entry_ptr[*active_schedule_id] = 0U;
            break;
        /* Goto sleep schedule table */
        case LIN_SCH_TBL_GO_TO_SLEEP:
            /* Switch to NULL_SCHEDULE_TABLE */
            /* In the lin_sch_table, the NULL_SCHEDULE_TABLE for interface  iii */
            /* is located at shedule_start */
            *active_schedule_id = prot_user_config_ptr->schedule_start;
            master_data_ptr->schedule_start_entry_ptr[*active_schedule_id] = 0U;
            break;
        case LIN_SCH_TBL_NULL:
        default:
            /* do nothing */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_check_sporadic_update
 * Description   : Checks if the associated flags to a sporadic frame have been updated
 * and a transmission is needed
 *
 * Implements    : lin_check_sporadic_update_Activity
 *END**************************************************************************/
l_u8 lin_check_sporadic_update(l_ifc_handle iii,
                               l_u8 frm_id)
{
    l_u16 flag_offset = 0U;
    l_u8 flag_size = 0U;
    l_u8 i = 0U;
    l_u8 j;
    l_u8 associate_frame_offset = 0U;
    const lin_associate_frame_t * ptr;
    l_u8 retVal = 0xFFU;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    ptr = (const lin_associate_frame_t *)(prot_user_config_ptr->frame_tbl_ptr[frm_id].frame_data_ptr);

    for (j = 0U; j < ptr->num_of_associated_uncond_frames; j++)
    {
        associate_frame_offset = (l_u8)ptr->associated_uncond_frame_ptr[j];
        flag_offset = prot_user_config_ptr->frame_tbl_ptr[associate_frame_offset].flag_offset;
        flag_size = prot_user_config_ptr->frame_tbl_ptr[associate_frame_offset].flag_size;
        /* Update transmit flags */
        for (i = 0U; i < flag_size; i++)
        {
            if (g_lin_flag_handle_tbl[flag_offset++] != 0xFFU)
            {
                retVal = prot_user_config_ptr->list_identifiers_RAM_ptr[((l_u8)ptr->associated_uncond_frame_ptr[j] - prot_user_config_ptr->frame_start) + 1U];
                break;
            }
        }

        if (retVal != 0xFFU)
        {
            break;
        }
    }

    /* There's no updated frame */
    return retVal;
}

#endif /* (1U == SUPPORT_MASTER_MODE) */

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_tx_flags
 * Description   : This function updates transmit flags
 *
 * Implements    : lin_update_tx_flags_Activity
 *END**************************************************************************/
static void lin_update_tx_flags(l_ifc_handle iii,
                                l_u8 frm_id)
{
    l_u16 flag_offset;
    l_u8 flag_size;
    l_u8 i;
    l_u8 associate_frame_offset;
    const lin_frame_t * lin_frame_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_frame_ptr = &(prot_user_config_ptr->frame_tbl_ptr[frm_id]);

    if (LIN_FRM_UNCD == lin_frame_ptr->frm_type)
    {
        flag_offset = lin_frame_ptr->flag_offset;
        flag_size = lin_frame_ptr->flag_size;
        /* Update transmit flags */
        for (i = 0U; i < flag_size; i++)
        {
            g_lin_flag_handle_tbl[flag_offset] = 0xFFU;
            flag_offset++;
        }
    }

    /* check event trigger frame and clear flag */
    if (LIN_FRM_EVNT == prot_user_config_ptr->frame_tbl_ptr[frm_id].frm_type)
    {
        associate_frame_offset = (l_u8)(*((lin_frame_ptr->frame_data_ptr)->associated_uncond_frame_ptr));
        flag_offset = prot_user_config_ptr->frame_tbl_ptr[associate_frame_offset].flag_offset;
        flag_size = prot_user_config_ptr->frame_tbl_ptr[associate_frame_offset].flag_size;
        /* Update transmit flags */
        for (i = 0U; i < flag_size; i++)
        {
            g_lin_flag_handle_tbl[flag_offset] = 0xFFU;
            flag_offset++;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_get_frame_index
 * Description   : This function returns the index of a frame in frame list
 * or 0xFF if not found.
 *
 * Implements    : lin_get_frame_index_Activity
 *END**************************************************************************/
l_u8 lin_get_frame_index(l_ifc_handle iii,
                         l_u8 id)
{
    l_u8 i;
    l_u8 retVal = 0xFFU;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    for (i = prot_user_config_ptr->number_of_configurable_frames; i > 0U; i--)
    {
        if (prot_user_config_ptr->list_identifiers_RAM_ptr[i] == id)
        {
            retVal = (l_u8)(i + prot_user_config_ptr->frame_start - 1U);
            break;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_make_uncd_frame
 * Description   : This function gets signals associated with unconditional frame
 * from buffer.
 *
 * Implements    : lin_make_uncd_frame_Activity
 *END**************************************************************************/
void lin_make_uncd_frame(l_ifc_handle iii,
                         l_u8 id)
{
    l_u8 frame_index;
    l_u8 * response_buffer_ptr;
    l_u8 i;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (1U == SUPPORT_SLAVE_MODE)
    l_u16 frame_byte_offset;
    l_u8 flag;
#endif /* (1U == SUPPORT_SLAVE_MODE) */
#if (1U == SUPPORT_MASTER_MODE)
    const lin_master_data_t * master_data_ptr;
#endif /* (1U == SUPPORT_MASTER_MODE) */
    frame_index = lin_get_frame_index(iii, id);

    if (0xFFU != frame_index)
    {
        response_buffer_ptr = prot_state_ptr->response_buffer_ptr;

        /* Set frame length */
        prot_state_ptr->response_length = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_len;

    #if (1U == SUPPORT_SLAVE_MODE)
        frame_byte_offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_offset;

        if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
        {
            flag = g_lin_frame_updating_flag_tbl[frame_index];

            /* Get data from LIN frame buffer */
            for (i = 0U; i < prot_state_ptr->response_length; i++)
            {
                if((flag & (1U << i)) != 0U)
                {
                    response_buffer_ptr[i] = g_buffer_backup_data[i];
                }
                else
                {
                    response_buffer_ptr[i] = g_lin_frame_data_buffer[frame_byte_offset + i];
                }
            }
        }
        else
    #endif /* (1U == SUPPORT_SLAVE_MODE) */
        {
        #if (1U == SUPPORT_MASTER_MODE)
            master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            /* For Master node, get data from master data buffer */
            for (i = 0U; i < prot_state_ptr->response_length; i++)
            {
                response_buffer_ptr[i] = master_data_ptr->master_data_buffer[i];
            }
        #endif /* (1U == SUPPORT_MASTER_MODE) */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_uncd_frame
 * Description   : This function packs signals associated with unconditional frame
 * into buffer.
 *
 * Implements    : lin_update_uncd_frame_Activity
 *END**************************************************************************/
void lin_update_uncd_frame(l_ifc_handle iii,
                           l_u8 id)
{
    l_u8 frame_index;
    const l_u8 * response_buffer_ptr;
    l_u16 frame_byte_offset;
    l_u8 i;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (1U == SUPPORT_MASTER_MODE)
    lin_master_data_t * master_data_ptr;
#endif /* (1U == SUPPORT_MASTER_MODE) */
    frame_index = lin_get_frame_index(iii, id);

    if (0xFFU != frame_index)
    {
        response_buffer_ptr = prot_state_ptr->response_buffer_ptr;

        /* Set frame length */
        prot_state_ptr->response_length = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_len;
        frame_byte_offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_offset;

    #if (1U == SUPPORT_SLAVE_MODE)
        if ((bool)LIN_SLAVE == prot_user_config_ptr->function)
        {
            for (i = 0U; i < prot_state_ptr->response_length; i++)
            {
                g_lin_frame_data_buffer[frame_byte_offset + i] = response_buffer_ptr[i];
            }
        }
        else
    #endif /* (1U == SUPPORT_SLAVE_MODE) */
        {
        #if (1U == SUPPORT_MASTER_MODE)
            /* If master node save frame */
            master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            master_data_ptr->frm_offset = frame_byte_offset;
            master_data_ptr->frm_size = prot_state_ptr->response_length;
            for (i = 0U; i < prot_state_ptr->response_length; i++)
            {
                master_data_ptr->master_data_buffer[i] = response_buffer_ptr[i];
            }
        #endif /* (1U == SUPPORT_MASTER_MODE) */
        }
    }
}

#if ((1U == SUPPORT_SLAVE_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13)))
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_evnt_frame
 * Description   : This function packs signals associated with event trigger frame
 * into buffer if they have been updated and return the corresponding action.
 *
 * Implements    : lin_process_evnt_frame_Activity
 *END**************************************************************************/
static l_u8 lin_process_evnt_frame(l_ifc_handle iii,
                                   l_u8 id)
{
    l_u8 action = 0U;
    const lin_frame_t * lin_frame_ptr;
    l_u8 frame_index;

    l_u8 volatile associate_frame;
    l_u16 flag_offset;
    l_u8 flag_size;
    l_u8 i;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    frame_index = lin_get_frame_index(iii, id);
    lin_frame_ptr = &(prot_user_config_ptr->frame_tbl_ptr[frame_index]);

    /* For each slave node, get the only one associated unconditional frame */
    associate_frame = (l_u8)(*((lin_frame_ptr->frame_data_ptr)->associated_uncond_frame_ptr));
    flag_offset = prot_user_config_ptr->frame_tbl_ptr[associate_frame].flag_offset;
    flag_size = prot_user_config_ptr->frame_tbl_ptr[associate_frame].flag_size;
    /* Update transmit flags */
    for (i = 0U; i < flag_size; i++)
    {
        if (g_lin_flag_handle_tbl[flag_offset++] != 0xFFU)
        {
            /* Frame is updated */
            lin_make_res_evnt_frame(iii, prot_user_config_ptr->list_identifiers_RAM_ptr[1U + associate_frame - prot_user_config_ptr->frame_start]);
            /* Set response */
            action = 2U;
            break;
        }
    }

    return action;
}
#endif /* ((1U == SUPPORT_SLAVE_MODE) && ((1U == SUPPORT_PROTOCOL_21) || (1U == SUPPORT_PROTOCOL_20) || (1U == SUPPORT_PROTOCOL_13))) */

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_diag_frame
 * Description   : This function packs signals associated with diagnostic frame
 * into buffer and return the corresponding action.
 *
 * Implements    : lin_process_diag_frame_Activity
 *END**************************************************************************/
static l_u8 lin_process_diag_frame(l_ifc_handle iii,
                                   l_u8 id)
{
    l_u8 action = 0U;
    const lin_frame_t * lin_frame_ptr;
    l_u8 frame_index;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    frame_index = lin_get_frame_index(iii, id);
    lin_frame_ptr = &(prot_user_config_ptr->frame_tbl_ptr[frame_index]);

    if (LIN_RES_PUB == lin_frame_ptr->frm_response)
    {
        action = 2U;
        if (0x3CU == id)
        {
            if (DIAG_INTERLEAVE_MODE == g_lin_protocol_state_array[iii].diagnostic_mode)
            {
                (void)LIN_TL_CALLBACK_HANDLER(iii, TL_MAKE_RES_DATA, id);
            }
        }
        else
        {
            if (LIN_TL_CALLBACK_HANDLER(iii, TL_SLAVE_GET_ACTION, id) == TL_ACTION_ID_IGNORE)
            {
                action = 0U;
            }
        }
    }
    else
    {
        (void)LIN_TL_CALLBACK_HANDLER(iii, TL_RECEIVE_MESSAGE, id);
        /* Rx response */
        action = 1U;
    }

    return action;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
