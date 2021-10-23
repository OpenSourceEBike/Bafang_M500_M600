/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * @file lin_j2602_proto.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Some functions are part of API and are called by user's application and can't be made static
 */

#include "lin_j2602_proto.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_word_status_j2602
 * Description   : Update date word status.
 *
 * Implements    : lin_update_word_status_j2602_Activity
 *END**************************************************************************/
void lin_update_word_status_j2602(l_ifc_handle iii)
{
    l_u8 err_in_res;
    l_u8 suc_in_tras;
    l_u8 overrun_flg;
    lin_word_status_str_t * word_status_ptr;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];

    err_in_res = prot_state_ptr->error_in_response;
    prot_state_ptr->error_in_response = 0;
    suc_in_tras = prot_state_ptr->successful_transfer;
    overrun_flg = prot_state_ptr->overrun_flg;

    word_status_ptr = &prot_state_ptr->word_status;

    /* values for all processed frames */
    word_status_ptr->error_in_res |= err_in_res;
    word_status_ptr->successful_transfer |= suc_in_tras;


    word_status_ptr->overrun = overrun_flg;
    word_status_ptr->last_pid = prot_state_ptr->last_pid;

    if(prot_state_ptr->go_to_sleep_flg == true)
    {
        /* Set go to sleep flag for word status*/
        word_status_ptr->go_to_sleep_flg = 1;
    }
}

#if (SUPPORT_SLAVE_MODE == 1U)
#if (SUPPORT_PROTOCOL_J2602 == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_update_status_byte
 * Description   : The function is to perform the update of error signal in J2602 system.
 *
 * Implements    : lin_update_status_byte_Activity
 *END**************************************************************************/
void lin_update_status_byte(l_ifc_handle iii,
                            lin_lld_event_id_t event_id)
{
    l_u8 i;
    l_u8 signal_data;
    l_u16* byte_offset_temp_ptr;
    l_u8* bit_offset_temp_ptr;
    l_u8 set_error_signal = 0U;
    const lin_node_attribute_t * node_attr_ptr;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    node_attr_ptr = &g_lin_node_attribute_array[prot_user_config_ptr->slave_ifc_handle];
    signal_data = 0;
    switch (event_id)
    {
        /* Parity error */
        case LIN_LLD_PID_ERR:
            /* Set ID Parity Error 0x07 */
            signal_data = 0x07U;
            set_error_signal = 1;
            break;
        /* Byte Field Framing Error */
        case LIN_LLD_FRAME_ERR:
            /* Set Byte Field Framing Error 0x06 */
            signal_data = 0x06U;
            set_error_signal = 1;
            break;
        /* Checksum error */
        case LIN_LLD_CHECKSUM_ERR:
            /* Set Checksum error 0x05 */
            signal_data = 0x05U;
            set_error_signal = 1;
            break;
        /* Data error */
        case LIN_LLD_READBACK_ERR:
            /* Set Data error 0x04 */
            signal_data = 0x04U;
            set_error_signal = 1;
            break;
        default:
            /* do nothing */
            break;
    }

    if (set_error_signal == 1U)
    {
        for (i = 0; i < node_attr_ptr->num_frame_have_esignal; i++)
        {
            /* Get pointer to Byte and bit offset values in each frame that contains the error signal */
            byte_offset_temp_ptr = node_attr_ptr->response_error_byte_offset_ptr + i;
            bit_offset_temp_ptr = node_attr_ptr->response_error_bit_offset_ptr + i;
            /* Set error signal equal to error in response */
            g_lin_frame_data_buffer[*byte_offset_temp_ptr] = (l_u8)((g_lin_frame_data_buffer[*byte_offset_temp_ptr] & (~(0x07U << (*bit_offset_temp_ptr)))) |
                                                        (signal_data << (*bit_offset_temp_ptr)));
        }
    }
}

#endif /* End (SUPPORT_PROTOCOL_J2602 == 1U) */
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
