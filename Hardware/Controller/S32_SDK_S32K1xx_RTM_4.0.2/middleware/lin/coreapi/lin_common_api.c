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
 * @file lin_common_api.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Functions are APIs, so they shall not be made static.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 */

#include "lin_common_api.h"
#include "lin_common_proto.h"
#include "lin_lin21_proto.h"
#include "lin_j2602_proto.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/
#if (SUPPORT_MASTER_MODE == 1U)
static void lin_master_update_signal(l_ifc_handle iii);

static void lin_master_update_frame_before_transmit(l_ifc_handle iii, l_u8 cur_id, l_u8 frame_index);
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : l_sys_init
 * Description   : This function performs the initialization of the LIN core.
 * The implementation of this function can be replaced by user if needed.
 *
 * Implements    : l_sys_init_Activity
 *END**************************************************************************/
l_bool l_sys_init(void)
{
    return (l_bool)0;
}

#if (SUPPORT_MASTER_MODE == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : l_sch_set
 * Description   : This function sets up the next schedule to be followed by the
 * l_sch_tick function for a certain interface iii. The new schedule will be activated as soon as the current schedule reaches
 * its next schedule entry point. The entry defines the starting entry point in the new schedule table. The value should
 * be in the range 0 to N if the schedule table has N entries
 *
 * Implements    : l_sch_set_Activity
 *END**************************************************************************/
void l_sch_set(l_ifc_handle iii,
               l_schedule_handle schedule_iii,
               l_u8 entry)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    DEV_ASSERT((l_u8)schedule_iii < LIN_NUM_OF_SCHD_TBL);
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    /* Check if the interface is LIN Master */
    if (prot_user_config_ptr->function == (bool)LIN_MASTER)
    {
        /* Check if input entry is in range from 0 to N = (Number of frame slots) */
        if (entry <= prot_user_config_ptr->schedule_tbl[schedule_iii].num_slots)
        {
            l_u8 tmp_entry = entry;
            lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
            /* save the new schedule which is activated after the */
            /* current schedule reaches its next schedule entry point */
            master_data_ptr->previous_schedule_id = master_data_ptr->active_schedule_id;
            /* Insert new schedule at that point of the current schedule */
            master_data_ptr->active_schedule_id = (l_u8)schedule_iii;

            /* if entry is 0 or 1 the new schedule table will be started from the beginning. */
            if (tmp_entry == 0U)
            {
                /* Set entry equal to 1 */
                tmp_entry = 1U;
            }

            /* Assign the start entry of new schedule table by argument entry */
            master_data_ptr->schedule_start_entry_ptr[schedule_iii] = (l_u8)(tmp_entry - 1U);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_master_update_signal
 * Description   : This function handles update signal for master node.
 * This function is implemented for Master.
 *
 * Implements    : lin_master_update_signal_Activity
 *END**************************************************************************/
static void lin_master_update_signal(l_ifc_handle iii)
{
    l_u8 i;
    l_u8 size;
    l_u16 offset;
    lin_master_data_t * master_data_ptr;
    master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];

    if (master_data_ptr->frm_offset != 0xFFFFU)
    {
        /* Copy data from master_data_buffer to g_lin_frame_data_buffer */
        /* Do this to satisfy Requirement: The master node updates its
         * received signals periodically at the time base start */
        size = master_data_ptr->frm_size;
        offset = master_data_ptr->frm_offset;

        /* Check if size and offset are valid */
        if ((size <= 8U) && ((size + offset) <= LIN_FRAME_BUF_SIZE))
        {
            /* Copy data from master_data_buffer to g_lin_frame_data_buffer */
            for (i = 0U; i < size; i++)
            {
                g_lin_frame_data_buffer[offset + i] = master_data_ptr->master_data_buffer[i];
            }
        }

        master_data_ptr->frm_offset = 0xFFFFU;
    }

    if (master_data_ptr->flag_offset != 0xFFU)
    {
        /* Do this to satisfy Requirement: The master node updates its
         * received signals periodically at the time base start */
        size = master_data_ptr->flag_size;
        offset = master_data_ptr->flag_offset;

        /* Check if size and offset are valid */
        if ((size <= LIN_FLAG_BUF_SIZE) && ((size + offset) <= LIN_FLAG_BUF_SIZE))
        {
            /* Update flag */
            for (i = 0U; i < size; i++)
            {
                g_lin_flag_handle_tbl[offset + i] = 0xFF;
            }

            master_data_ptr->flag_offset = 0xFF;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_master_update_frame_before_transmit
 * Description   : This function update signal in transmitted frame of master.
 * This function is implemented for Master.
 *
 * Implements    : lin_master_update_frame_before_transmit_Activity
 *END**************************************************************************/
static void lin_master_update_frame_before_transmit(l_ifc_handle iii, l_u8 cur_id, l_u8 frame_index)
{
    l_u8 i;
    l_u8 size;
    l_u16 offset;
    l_u8 active_schedule_id;
    l_u8 flag;
    const l_u8 * current_entry;
    const lin_protocol_state_t * prot_state_ptr;
    const lin_schedule_data_t * sch_data_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr;
    const lin_schedule_t * active_schedule_struct_ptr;
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];

    if (0x3CU == cur_id)
    {
        active_schedule_id = master_data_ptr->active_schedule_id;
        active_schedule_struct_ptr = (const lin_schedule_t *)&(g_lin_protocol_user_cfg_array[iii].schedule_tbl[active_schedule_id]);
        if (LIN_SCH_TBL_DIAG != active_schedule_struct_ptr->sch_tbl_type)
        {
            current_entry = (l_u8 *)&master_data_ptr->schedule_start_entry_ptr[active_schedule_id];
            prot_state_ptr = &g_lin_protocol_state_array[iii];
            sch_data_ptr = &active_schedule_struct_ptr->ptr_sch_data_ptr[*current_entry];
            /* Update transmitted data in case diagnostic frame is not added in diagnostic schedule table */
            for (i = 0U; i < 8U; i++)
            {
                prot_state_ptr->response_buffer_ptr[i] = sch_data_ptr->tl_queue_data[i];
            }
        }
    }
    else
    {
        prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
        /* Update buffer data if master is publisher and not transmit diagnostic request */
        if (prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_response == LIN_RES_PUB)
        {
            size = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_len;
            offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_offset;
            flag = g_lin_frame_updating_flag_tbl[frame_index];
            /* Update data */
            for (i = 0U; i < size; i++)
            {
                if((flag & (1U << i)) != 0U)
                {
                    master_data_ptr->master_data_buffer[i] = g_buffer_backup_data[i];
                }
                else
                {
                    master_data_ptr->master_data_buffer[i] = g_lin_frame_data_buffer[offset];
                }
                offset++;
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_sch_tick
 * Description   : This function follows a schedule. When a frame becomes due, its
 * transmission is initiated. When the end of the current schedule is reached,
 * this function starts again at the beginning of the schedule.
 * This API is for Master interface only.
 *
 * Implements    : l_sch_tick_Activity
 *END**************************************************************************/
l_u8 l_sch_tick(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    const lin_schedule_t * active_schedule_struct_ptr;
    l_u8 * current_entry;
    l_u8 frame_index;
    l_u8 cur_id;
    l_u8 active_schedule_id;
    l_u8 retVal = 0U;

    const lin_tl_descriptor_t * tl_desc_ptr = &g_lin_tl_descriptor_array[iii];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];

    lin_master_data_t * master_data_ptr;

    /* Check if the interface is LIN Master */
    if (prot_user_config_ptr->function == (bool)LIN_MASTER)
    {
        master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];

        lin_master_update_signal(iii);

        /* Get active_schedule_id */
        active_schedule_id = master_data_ptr->active_schedule_id;
        cur_id = prot_state_ptr->current_id;

        /* process diagnostic interleaved mode */
        if ((tl_desc_ptr->diag_interleave_state == DIAG_NO_RESPONSE) && (prot_state_ptr->diagnostic_mode == DIAG_INTERLEAVE_MODE) &&
            (cur_id == 0x3DU))
        {
            /* go back normal schedule table */
            active_schedule_id = master_data_ptr->previous_schedule_id;
            master_data_ptr->active_schedule_id = active_schedule_id;
            master_data_ptr->schedule_start_entry_ptr[active_schedule_id] = 0U;
        }

        active_schedule_struct_ptr = (const lin_schedule_t *)&prot_user_config_ptr->schedule_tbl[active_schedule_id];
        current_entry = (l_u8 *)&master_data_ptr->schedule_start_entry_ptr[active_schedule_id];

        if (LIN_SCH_TBL_NULL != active_schedule_struct_ptr->sch_tbl_type)
        {
            /* Check if next_transmit is 0 */
            if (0U == prot_state_ptr->next_transmit_tick)
            {
                /* Check protocol and event trigger collision flag */
                if ((prot_user_config_ptr->protocol_version != LIN_PROTOCOL_J2602) && (master_data_ptr->event_trigger_collision_flg == true))
                {
                    /* Call collision resolver */
                    lin_collision_resolve(iii, cur_id);
                    /* Update active schedule table */
                    active_schedule_id = master_data_ptr->active_schedule_id;
                    active_schedule_struct_ptr = (const lin_schedule_t *)&prot_user_config_ptr->schedule_tbl[active_schedule_id];
                    /* Re-calculate current entry due to change table to collision */
                    current_entry = (l_u8 *)&master_data_ptr->schedule_start_entry_ptr[active_schedule_id];
                    master_data_ptr->event_trigger_collision_flg = false;
                }

                /* Set new transmit tick */
                prot_state_ptr->next_transmit_tick = active_schedule_struct_ptr->ptr_sch_data_ptr[*current_entry].delay_integer;
                /* Get frame index to send */
                frame_index = (l_u8)active_schedule_struct_ptr->ptr_sch_data_ptr[*current_entry].frm_id;

                if ((frame_index - prot_user_config_ptr->frame_start) < prot_user_config_ptr->number_of_configurable_frames)
                {
                    cur_id = prot_user_config_ptr->list_identifiers_RAM_ptr[(frame_index - prot_user_config_ptr->frame_start) + 1U];
                }

                if (LIN_FRM_SPRDC == prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_type)
                {
                    /* Sporadic frame */
                    cur_id = lin_check_sporadic_update(iii, frame_index);
                    frame_index = lin_get_frame_index(iii, cur_id);
                }

                if ((0xFFU != cur_id) && (frame_index < LIN_NUM_OF_FRMS))
                {
                    /* The latest point master can update value of signal in transmitted frame */
                    lin_master_update_frame_before_transmit(iii, cur_id, frame_index);

                    /* Start transmit header */
                    (void)lin_lld_tx_header(iii, cur_id);
                }
                /* increase start entry */
                 *current_entry = (l_u8)(*current_entry + 1U);
                /* Check if it is the last entry */
                if (*current_entry >= active_schedule_struct_ptr->num_slots)
                {
                    /* switch schedule table */
                    lin_switch_sch_table(iii);
                }
            }

            /* Decrease next_transmit_tick */
            prot_state_ptr->next_transmit_tick -= 1U;

            /* if the next call of l_sch_tick will start the transmission of the frame in the next schedule table entry */
            /* The return value will in this case be the next schedule table entry's number */
            /* counted from the beginning of the schedule table) in the schedule table */
            /* The return value will be in range 1 to N if the schedule table has N entries */
            if (0U == prot_state_ptr->next_transmit_tick)
            {
                /* The return value will be in range 1 to N if the schedule table has N entries */
                retVal = (l_u8)(*current_entry + 1U);
            }
        } /* End (LIN_SCH_TBL_NULL != active_schedule_struct_ptr->sch_tbl_type) */

        /* In the master node the status word is updated in the l_sch_tick function*/
        switch (prot_user_config_ptr->protocol_version)
        {
            case LIN_PROTOCOL_20:
            case LIN_PROTOCOL_21:
                lin_update_word_status_lin21(iii, LIN_LLD_PID_OK);
                break;
            case LIN_PROTOCOL_J2602:
                lin_update_word_status_j2602(iii);
                break;
            default:
                /*do nothing*/
                break;
        }

        prot_state_ptr->current_id = cur_id;
    } /* END (prot_user_config_ptr->function == LIN_MASTER) */

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_ifc_goto_sleep
 * Description   : Request slave nodes on the cluster connected to the interface to enter
 * bus sleep mode by issuing one go to sleep command.
 *
 * Implements    : l_ifc_goto_sleep_Activity
 *END**************************************************************************/
void l_ifc_goto_sleep(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_master_data_t * master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];

    if (prot_user_config_ptr->function == (bool)LIN_MASTER)
    {
        /* Set active schedule as GOTO_SLEEP_SCHEDULE */
        /* current schedule reaches its next schedule entry point */
        master_data_ptr->previous_schedule_id = master_data_ptr->active_schedule_id;
        /* Insert new schedule at that point of the current schedule */
        master_data_ptr->active_schedule_id = (l_u8)(prot_user_config_ptr->schedule_start + 1U);
        /* Assign the start entry of new schedule table by argument entry */
        master_data_ptr->schedule_start_entry_ptr[prot_user_config_ptr->schedule_start + 1U] = 0;
    }
}

#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : l_ifc_init
 * Description   : Initialize the interface specified by name, i.e. sets up internal
 * functions such as the baud rate
 *
 * Implements    : l_ifc_init_Activity
 *END**************************************************************************/
l_bool l_ifc_init(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (SUPPORT_MASTER_MODE == 1U)
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    static l_u8 lin_schedule_start_entry[LIN_NUM_OF_SCHD_TBL];
    lin_master_data_t * master_data_ptr;
#endif /* End (SUPPORT_MASTER_MODE == 1U) */
    l_bool ret_val;
    prot_state_ptr->diagnostic_mode = DIAG_NONE;

    /* Call Low level initialization function */
    ret_val = lin_lld_init(iii);
    /* Check if low level initialization is successful */
    if (ret_val == (l_bool)LIN_LLD_OK)
    {
    #if (SUPPORT_MASTER_MODE == 1U)
        if (prot_user_config_ptr->function == (bool)LIN_MASTER)
        {
            master_data_ptr = &g_lin_master_data_array[prot_user_config_ptr->master_ifc_handle];
            /* Init flag offset and frame offset for Master Frame Data Buffer */
            master_data_ptr->flag_offset = 0xFFU;
            master_data_ptr->frm_offset  = 0xFFFFU;
            master_data_ptr->flag_size = 0x00U;
            master_data_ptr->frm_size  = 0x00U;
            master_data_ptr->schedule_start_entry_ptr = &lin_schedule_start_entry[0U];
            master_data_ptr->send_slave_res_flg = (bool)1;
            master_data_ptr->send_functional_request_flg = (bool)0;
            /* For LIN Master, set NULL schedule table after initialization*/
            /* current schedule reaches its next schedule entry point */
            master_data_ptr->previous_schedule_id = master_data_ptr->active_schedule_id;
            /* Insert new schedule at that point of the current schedule */
            master_data_ptr->active_schedule_id = prot_user_config_ptr->schedule_start;
            /* Assign the start entry of new schedule table by argument entry */
            master_data_ptr->schedule_start_entry_ptr[prot_user_config_ptr->schedule_start] = 0;
        }
    #endif /* End (SUPPORT_MASTER_MODE == 1U) */
    }

    return ret_val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_ifc_wake_up
 * Description   : Transmit the wake up signal.
 *
 * Implements    : l_ifc_wake_up_Activity
 *END**************************************************************************/
void l_ifc_wake_up(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    /* Send wakeup signal */
    (void)lin_lld_tx_wake_up(iii);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_ifc_read_status
 * Description   : This function will return the status of the previous communication.
 *
 * Implements    : l_ifc_read_status_Activity
 *END**************************************************************************/
l_u16 l_ifc_read_status(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u16 tmp_word_status = 0U;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    tmp_word_status = (l_u16)(prot_state_ptr->word_status.last_pid << 8);
    if (prot_state_ptr->word_status.save_config_flg == 1U)
    {
        tmp_word_status |= SAVE_CONFIG_SET;
    }

    if (prot_state_ptr->word_status.event_trigger_collision_flg == 1U)
    {
        tmp_word_status |= EVENT_TRIGGER_COLLISION_SET;
    }

    if (prot_state_ptr->word_status.bus_activity == 1U)
    {
        tmp_word_status |= BUS_ACTIVITY_SET;
    }

    if (prot_state_ptr->word_status.go_to_sleep_flg == 1U)
    {
        tmp_word_status |= GO_TO_SLEEP_SET;
    }

    if (prot_state_ptr->word_status.overrun == 1U)
    {
        tmp_word_status |= OVERRUN;
    }

    if (prot_state_ptr->word_status.successful_transfer == 1U)
    {
        tmp_word_status |= SUCCESSFULL_TRANSFER;
    }

    if (prot_state_ptr->word_status.error_in_res == 1U)
    {
        tmp_word_status |= ERROR_IN_RESPONSE;
    }

    /* Read-reset call; meaning that after the call has returned,
     * the status word is set to 0 */
    /* Clear word status */
    prot_state_ptr->word_status.last_pid = 0x00U;
    prot_state_ptr->word_status.reserved = 0U;
    prot_state_ptr->word_status.save_config_flg = 0U;
    prot_state_ptr->word_status.event_trigger_collision_flg = 0U;
    prot_state_ptr->word_status.bus_activity = 0U;
    prot_state_ptr->word_status.go_to_sleep_flg = 0U;
    prot_state_ptr->word_status.overrun = 0U;
    prot_state_ptr->word_status.successful_transfer = 0U;
    prot_state_ptr->word_status.error_in_res = 0U;
    /* Clear save configuration flag value */
    prot_state_ptr->save_config_flg = (bool)0U;
    /* reset word status flags in g_lin_protocol_state_array for new session*/
    prot_state_ptr->go_to_sleep_flg = false;
    prot_state_ptr->overrun_flg = 0U;
    prot_state_ptr->successful_transfer = 0U;

    /* Reset number of processed frames to 0 */
    prot_state_ptr->num_of_processed_frame = 0U;

    return tmp_word_status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_sys_irq_disable
 * Description   : Disable LIN related IRQ.
 *
 * Implements    : l_sys_irq_disable_Activity
 *END**************************************************************************/
l_u16 l_sys_irq_disable(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    return lin_lld_int_disable(iii);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : l_sys_irq_restore
 * Description   : Enable LIN related IRQ.
 *
 * Implements    : l_sys_irq_restore_Activity
 *END**************************************************************************/
void l_sys_irq_restore(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    (void)lin_lld_int_enable(iii);
}

#if (SUPPORT_TRANSPORT_LAYER != 1U)
/* This function is use when user not use transport layer */
lin_tl_callback_return_t lin_tl_callback_handler(l_ifc_handle iii,
                                                 lin_tl_event_id_t tl_event_id,
                                                 l_u8 id)
{
    UNUSED(iii);
    UNUSED(tl_event_id);
    UNUSED(id);

    return TL_ACTION_NONE;
}

#endif /* if (SUPPORT_TRANSPORT_LAYER != 1U) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
