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
 * @file lin_j2602_proto.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012: Repeated include file
 * All the library headers are already guarded using macro.
 */

#ifndef LIN_J2602_PROTO_H
#define LIN_J2602_PROTO_H

#include "lin.h"

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Update date word status.
 *
 * @param[in] iii Interface name
 * @return void
 */
void lin_update_word_status_j2602(l_ifc_handle iii);

#if (SUPPORT_SLAVE_MODE == 1U)
#if (SUPPORT_PROTOCOL_J2602 == 1U)
/*!
 * @brief The function is to perform the update of error signal in J2602 system.
 *
 * @param[in] iii Interface name
 * @param[in] event_id Event id
 * @return void
 */
void lin_update_status_byte(l_ifc_handle iii,
                            lin_lld_event_id_t event_id);

#endif /* End (SUPPORT_PROTOCOL_J2602 == 1U)*/
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */
#endif /* ifndef LIN_J2602_PROTO_H */
/*! @} */
/******************************************************************************/
/* EOF */
/******************************************************************************/
