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
 * @file lin.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Functions are APIs, so they shall not be made static.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type .
 * The conversion is needed to restore original type of pointer to lin_state_t. Before callback function,
 * the input pointer is a pointer to lin_state_t.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'linState' could be declared as pointing to const
 * This function must be of lin_callback_t type and this type is defined in lin_driver.h, part of LIN Driver
 */

#include "lin.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Global variables */
/*  lin_tl_descriptor_t */
lin_tl_descriptor_t g_lin_tl_descriptor_array[LIN_NUM_OF_IFCS];     /*!< Global array for storing transport configuration for each interface */
lin_protocol_state_t g_lin_protocol_state_array[LIN_NUM_OF_IFCS];   /*!< Global array for storing the protocol state for each interface */
#if (SUPPORT_MASTER_MODE == 1U)
lin_master_data_t g_lin_master_data_array[LIN_NUM_OF_MASTER_IFCS];  /*!< Global array for storing the master interfaces configurations */
#endif /* End (SUPPORT_MASTER_MODE == 1U) */
/* Static variables */
static lin_state_t s_lin_state_array[LIN_NUM_OF_IFCS];              /*!< Global array for storing the status of all available LIN interfaces */
static volatile l_u16 s_lin_max_header_timeout[LIN_NUM_OF_IFCS];  /*!< Global array for storing the frame header timeout for each interface */
static l_u16 s_lin_max_frame_res_timeout_val[LIN_NUM_OF_IFCS][8]; /*!< Global 2-dimensions array for storing the response timeout for all frame sizes for each interface */
static void CallbackHandler(uint32_t instance,
                            void * linState);
static l_bool s_baudrate_adjusted_flg[LIN_NUM_OF_IFCS];

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : lin_process_parity
 * Description   : Makes or checks parity bits. If action is checking parity, the function
 * returns ID value if parity bits are correct or 0xFF if parity bits are incorrect. If action
 * is making parity bits, then from input value of ID, the function returns PID.
 *
 * Implements    : lin_process_parity_Activity
 *END**************************************************************************/
l_u8 lin_process_parity(l_u8 pid,
                        l_u8 typeAction)
{
    return LIN_DRV_ProcessParity(pid, typeAction);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_init
 * Description   : This function initializes a LIN hardware instance for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers, initialize the module to user defined settings and
 * default settings, configure the IRQ state structure and enable the module-level
 * interrupt to the core, and enable the LIN hardware module transmitter and receiver.
 *
 * Implements    : lin_lld_init_Activity
 *END**************************************************************************/
l_bool lin_lld_init(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    l_u8 index;
    status_t retVal = STATUS_ERROR;
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    static l_u8 lin_lld_response_buffer[LIN_NUM_OF_IFCS][10];

    prot_state_ptr->response_buffer_ptr = lin_lld_response_buffer[iii];
    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Initialize time out values */
    /* If autobaud is enabled, set frame response timeout array and header timeout */
    /* to maximum values in case the  Master baudrate is 2400 */
    if (prot_user_config_ptr->lin_user_config_ptr->autobaudEnable)
    {
        s_lin_max_header_timeout[iii] = lin_calc_max_header_timeout_cnt(2400);

        for (index = 1U; index < 9U; index++)
        {
            s_lin_max_frame_res_timeout_val[iii][index - 1U] = lin_calc_max_res_timeout_cnt(2400, index);
        }
    }
    /* If autobaud is disabled, set frame response timeout array and header timeout */
    /* to values corresponding to node's configured baudrate */
    else
    {
        s_lin_max_header_timeout[iii] = lin_calc_max_header_timeout_cnt(prot_user_config_ptr->lin_user_config_ptr->baudRate);
        for (index = 1U; index < 9U; index++)
        {
            s_lin_max_frame_res_timeout_val[iii][index - 1U] = lin_calc_max_res_timeout_cnt(prot_user_config_ptr->lin_user_config_ptr->baudRate, index);
        }
    }

    /* Set frame timeout count */
    prot_state_ptr->frame_timeout_cnt = s_lin_max_frame_res_timeout_val[iii][7U];
    /* Set idle timeout count */
    prot_state_ptr->idle_timeout_cnt = prot_user_config_ptr->max_idle_timeout_cnt;

    /* Initialize the LIN hardware interface */
    retVal = LIN_DRV_Init(lin_hardware_instance, prot_user_config_ptr->lin_user_config_ptr, &s_lin_state_array[iii]);

    if (retVal == STATUS_SUCCESS)
    {
        /* Install callback function */
        (void)LIN_DRV_InstallCallback(lin_hardware_instance, CallbackHandler);
        /* Clear s_baudrate_adjusted_flg[iii] */
        s_baudrate_adjusted_flg[iii] = false;
    }

    return (retVal != STATUS_SUCCESS) ? true : false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_deinit
 * Description   : This function shuts down the LIN hardware interface by disabling interrupts and
 *                 transmitter/receiver.
 *
 * Implements    : lin_lld_deinit_Activity
 *END**************************************************************************/
void lin_lld_deinit(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* De-initialize the LIN hardware interface */
    LIN_DRV_Deinit(lin_hardware_instance);

}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_get_state
 * Description   : This function returns current node's state.
 *
 * Implements    : lin_lld_get_state_Activity
 *END**************************************************************************/
lin_node_state_t lin_lld_get_state(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    lin_node_state_t retVal;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Get current node's state */
    retVal = LIN_DRV_GetCurrentNodeState(lin_hardware_instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_tx_header
 * Description   : This function sends frame header for the input ID.
 *
 * Implements    : lin_lld_tx_header_Activity
 *END**************************************************************************/
l_u8 lin_lld_tx_header(l_ifc_handle iii,
                       l_u8 id)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Set correct frame timeout */
    prot_state_ptr->frame_timeout_cnt = (l_u16)(s_lin_max_frame_res_timeout_val[iii][7] + s_lin_max_header_timeout[iii]);

    /* Send frame header */
    retVal = LIN_DRV_MasterSendHeader(lin_hardware_instance, id);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_tx_wake_up
 * Description   : This function sends a wakeup signal.
 *
 * Implements    : lin_lld_tx_wake_up_Activity
 *END**************************************************************************/
l_u8 lin_lld_tx_wake_up(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Send a wakeup signal */
    retVal = LIN_DRV_SendWakeupSignal(lin_hardware_instance);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_int_enable
 * Description   : This function enables LIN hardware interrupt of an interface.
 *
 * Implements    : lin_lld_int_enable_Activity
 *END**************************************************************************/
l_u8 lin_lld_int_enable(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Call function LIN_DRV_DisableIRQ to enable interrupts */
    retVal = LIN_DRV_EnableIRQ(lin_hardware_instance);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_int_disable
 * Description   : This function disables LIN hardware interrupt of an interface.
 *
 * Implements    : lin_lld_int_disable_Activity
 *END**************************************************************************/
l_u8 lin_lld_int_disable(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Call function LIN_DRV_DisableIRQ to disable interrupts */
    retVal = LIN_DRV_DisableIRQ(lin_hardware_instance);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_ignore_response
 * Description   : This function terminates an on-going data transmission/reception.
 *
 * Implements    : lin_lld_ignore_response_Activity
 *END**************************************************************************/
l_u8 lin_lld_ignore_response(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Abort frame data transferring */
    retVal = LIN_DRV_AbortTransferData(lin_hardware_instance);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_set_low_power_mode
 * Description   : This function puts current node to sleep mode.
 *
 * Implements    : lin_lld_set_low_power_mode_Activity
 *END**************************************************************************/
l_u8 lin_lld_set_low_power_mode(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Put current node to sleep mode */
    retVal = LIN_DRV_GoToSleepMode(lin_hardware_instance);

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_set_response
 * Description   : This function sends frame data that is contained in g_lin_protocol_state_array[iii].response_buffer_ptr.
 *
 * Implements    : lin_lld_set_response_Activity
 *END**************************************************************************/
l_u8 lin_lld_set_response(l_ifc_handle iii,
                          l_u8 response_length)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;
    const l_u8 * buffer;

    /* Check if response_length is from 1 to 8 */
    if ((response_length <= 8U) && (response_length != 0U))
    {
        buffer = g_lin_protocol_state_array[iii].response_buffer_ptr;
        /* Get the LIN hardware instance number */
        lin_hardware_instance = g_lin_virtual_ifc[iii];

        /* Put response length into buffer */
        g_lin_protocol_state_array[iii].response_length = response_length;

        /* Set correct timeout value */
        LIN_DRV_SetTimeoutCounter(lin_hardware_instance, s_lin_max_frame_res_timeout_val[iii][response_length - 1U]);

        /* Start sending data from the buffer */
        retVal = LIN_DRV_SendFrameData(lin_hardware_instance, &buffer[0U], response_length);
    }

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_rx_response
 * Description   : This function receives frame data into the buffer g_lin_protocol_state_array[iii].response_buffer_ptr.
 *
 * Implements    : lin_lld_rx_response_Activity
 *END**************************************************************************/
l_u8 lin_lld_rx_response(l_ifc_handle iii,
                         l_u8 response_length)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);

    l_u32 lin_hardware_instance;
    status_t retVal = STATUS_ERROR;

    /* Get the LIN hardware instance number */
    lin_hardware_instance = g_lin_virtual_ifc[iii];

    /* Check if response_length is from 1 to 8 */
    if ((response_length <= 8U) && (response_length != 0U))
    {
        /* Set correct timeout value */
        LIN_DRV_SetTimeoutCounter(lin_hardware_instance, s_lin_max_frame_res_timeout_val[iii][response_length - 1U]);

        /* Start receiving frame data into the buffer */
        retVal = LIN_DRV_ReceiveFrameData(lin_hardware_instance, g_lin_protocol_state_array[iii].response_buffer_ptr, response_length);
    }

    return (retVal == STATUS_SUCCESS) ? LIN_LLD_OK : LIN_LLD_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : lin_lld_timeout_service
 * Description   : This is a part of Timer Interrupt Handler.
 *
 * Implements    : lin_lld_timeout_service_Activity
 *END**************************************************************************/
void lin_lld_timeout_service(l_ifc_handle iii)
{
    DEV_ASSERT((l_u8)iii < LIN_NUM_OF_IFCS);
    l_u32 lin_hardware_instance = g_lin_virtual_ifc[iii];
    lin_state_t linState = s_lin_state_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
    l_u8 index;

    LIN_DRV_TimeoutService(lin_hardware_instance);
    /* Callback to transport layer to handle  N_CR, N_AS or diagnostic interleave timeout */
    (void)LIN_TL_CALLBACK_HANDLER(iii, TL_TIMEOUT_SERVICE, 0);

    switch (linState.currentNodeState)
    {
        case LIN_NODE_STATE_IDLE:
            if (!(prot_state_ptr->idle_timeout_cnt-- > 0U))
            {
                /* Callback to handle timeout */
                CALLBACK_HANDLER(iii, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFFU);

                /* Put current node to Idle state, reset idle timeout count */
                prot_state_ptr->idle_timeout_cnt = g_lin_protocol_user_cfg_array[iii].max_idle_timeout_cnt;

                /* Put current node to sleep mode */
                (void)LIN_DRV_GoToSleepMode(lin_hardware_instance);
            }
            /* Re-calculate s_lin_max_header_timeout and s_lin_max_frame_res_timeout_val */
            if (s_baudrate_adjusted_flg[iii])
            {
                /* Clear baudrate adjusted flag */
                s_baudrate_adjusted_flg[iii] = false;
                s_lin_max_header_timeout[iii] = lin_calc_max_header_timeout_cnt(g_lin_protocol_user_cfg_array->lin_user_config_ptr->baudRate);
                for (index = 1U; index < 9U; index++)
                {
                    s_lin_max_frame_res_timeout_val[iii][index - 1U] = lin_calc_max_res_timeout_cnt(g_lin_protocol_user_cfg_array->lin_user_config_ptr->baudRate, index);
                }
            }
            break;
        case LIN_NODE_STATE_SEND_BREAK_FIELD:
        case LIN_NODE_STATE_RECV_SYNC:
        case LIN_NODE_STATE_RECV_PID:
        case LIN_NODE_STATE_SEND_PID:
            /* timeout send has occurred - change state of the node and inform core */
            if (!(prot_state_ptr->frame_timeout_cnt-- > 0U))
            {
                /* Go to idle state */
                (void)LIN_DRV_GotoIdleState(lin_hardware_instance);

                /* Reset frame count timeout */
                prot_state_ptr->frame_timeout_cnt = s_lin_max_frame_res_timeout_val[iii][7U];
            }
            break;
        case LIN_NODE_STATE_SEND_DATA:
            /* timeout send has occurred - change state of the node and inform core */
            if (!(prot_state_ptr->frame_timeout_cnt-- > 0U))
            {
                /* Abort frame data transferring */
                (void)LIN_DRV_AbortTransferData(lin_hardware_instance);

                /* Reset frame count timeout */
                prot_state_ptr->frame_timeout_cnt = s_lin_max_frame_res_timeout_val[iii][7U];
            }
            break;
        case LIN_NODE_STATE_UNINIT:
        case LIN_NODE_STATE_SLEEP_MODE:
        case LIN_NODE_STATE_RECV_DATA:
        case LIN_NODE_STATE_RECV_DATA_COMPLETED:
        case LIN_NODE_STATE_SEND_DATA_COMPLETED:
        default:
            /* do nothing */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CallbackHandler
 * Description   : This is the callback function for LIN Driver.
 *
 * Implements    : CallbackHandler_Activity
 *END**************************************************************************/
static void CallbackHandler(uint32_t instance,
                            void * linState)
{
    const lin_state_t * linCurrentState = linState;
    lin_lld_event_id_t event_id;
    l_u8 id = linCurrentState->currentId;
    l_u8 bytesRemaining = 0U;
    l_ifc_handle iii = g_lin_hardware_ifc[instance];
    const lin_protocol_user_config_t * prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];
    lin_protocol_state_t * prot_state_ptr = &g_lin_protocol_state_array[iii];
#if (SUPPORT_MASTER_MODE == 1U)
    lin_master_data_t * master_data_ptr;
#endif /* End (SUPPORT_MASTER_MODE == 1U) */

    switch (linCurrentState->currentEventId)
    {
        case LIN_PID_OK:
            event_id = LIN_LLD_PID_OK;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_PID_ERROR:
            event_id = LIN_LLD_PID_ERR;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_TX_COMPLETED:
            event_id = LIN_LLD_TX_COMPLETED;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_RX_COMPLETED:
            event_id = LIN_LLD_RX_COMPLETED;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_CHECKSUM_ERROR:
            event_id = LIN_LLD_CHECKSUM_ERR;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_READBACK_ERROR:
            event_id = LIN_LLD_READBACK_ERR;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_FRAME_ERROR:
            event_id = LIN_LLD_FRAME_ERR;
            CALLBACK_HANDLER(iii, (event_id), (id));
            break;
        case LIN_RECV_BREAK_FIELD_OK:
            prot_state_ptr->frame_timeout_cnt = (l_u16)(s_lin_max_frame_res_timeout_val[iii][7U] + s_lin_max_header_timeout[iii]);
            break;
        case LIN_SYNC_ERROR:
        #if (SUPPORT_PROTOCOL_J2602 == 1U)
            if (prot_user_config_ptr->protocol_version == LIN_PROTOCOL_J2602)
            {
                event_id = LIN_LLD_READBACK_ERR;
                CALLBACK_HANDLER(iii, (event_id), (id));
            }
        #endif /* End of (SUPPORT_PROTOCOL_J2602 == 1U) */

            prot_state_ptr->frame_timeout_cnt = s_lin_max_frame_res_timeout_val[iii][7U];
            break;
        case LIN_BAUDRATE_ADJUSTED:
            /* Set s_baudrate_adjusted_flg[iii] */
            s_baudrate_adjusted_flg[iii] = true;
            break;
        case LIN_NO_EVENT:
            if (linCurrentState->timeoutCounterFlag == (bool)1U)
            {
        #if (SUPPORT_MASTER_MODE == 1U)
                if ((prot_state_ptr->diagnostic_mode == DIAG_INTERLEAVE_MODE) && (prot_state_ptr->current_id == 0x3DU))
                {
                    master_data_ptr = &g_lin_master_data_array[g_lin_protocol_user_cfg_array[iii].master_ifc_handle];
                    master_data_ptr->active_schedule_id = master_data_ptr->previous_schedule_id;
                    master_data_ptr->schedule_start_entry_ptr[master_data_ptr->active_schedule_id] = 0U;
                }
                else
        #endif /* End (SUPPORT_MASTER_MODE == 1U) */
                {
                    (void)LIN_DRV_GetReceiveStatus(instance, &bytesRemaining);
                    /* Received part of data but not completed */
                    if (linCurrentState->rxSize > bytesRemaining)
                    {
                        event_id = LIN_LLD_NODATA_TIMEOUT;
                        CALLBACK_HANDLER(iii, (event_id), (id));
                    }
                }
            }
            break;
        default:
            /* do nothing */
            break;
    }

    prot_state_ptr->idle_timeout_cnt = prot_user_config_ptr->max_idle_timeout_cnt;
    /*
    * @violates MISRA 2012 Advisory Rule 8.13 Advisory
    */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
