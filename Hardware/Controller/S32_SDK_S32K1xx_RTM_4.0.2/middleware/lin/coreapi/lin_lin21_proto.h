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
 * @file lin_lin21_proto.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012: Repeated include file
 * All the library headers are already guarded using macro.
 */

#ifndef LIN_LIN21_PROTO_H
#define LIN_LIN21_PROTO_H

/*!
 * @addtogroup LIN21_core_api_group
 * @{
 */

#include "lin.h"

/*******************************************************************************
 * API
 ******************************************************************************/
#if (SUPPORT_MASTER_MODE == 1U)
/*!
 * @brief Switch to collision resolve table.
 *
 * @param[in] iii Interface name
 * @param[in] pid PID to process
 * @return void
 */
void lin_collision_resolve(l_ifc_handle iii,
                           l_u8 pid);

#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*!
 * @brief Update node status flags.
 *
 * @param[in] iii Interface name
 * @param[in] event_id Event id
 * @return void
 */
void lin_update_word_status_lin21(l_ifc_handle iii,
                                  lin_lld_event_id_t event_id);

#if (SUPPORT_SLAVE_MODE == 1U)
/*!
 * @brief Update error signal.
 *
 * @param[in] iii Interface name
 * @param[in] frm_id Frame index
 * @return void
 */
void lin_update_err_signal(l_ifc_handle iii,
                           l_u8 frm_id);

/*!
 * @brief This function packs signals associated with event trigger frame into buffer.
 *
 * @param[in] iii Interface name
 * @param[in] pid PID to process
 * @return void
 */
void lin_make_res_evnt_frame(l_ifc_handle iii,
                             l_u8 pid);

#endif /* End (SUPPORT_SLAVE_MODE == 1U) */

/*!
 * @brief The function updates the receive flags associated with signals/frames
 *  in case receive an event trigger frame.
 *
 * @param[in] iii Interface name
 * @param[in] pid PID to process
 * @return void
 */
void lin_update_rx_evnt_frame(l_ifc_handle iii,
                              l_u8 pid);

#endif /* LIN_LIN21_PROTO_H */

/*! @} */
/******************************************************************************/
/* EOF */
/******************************************************************************/
