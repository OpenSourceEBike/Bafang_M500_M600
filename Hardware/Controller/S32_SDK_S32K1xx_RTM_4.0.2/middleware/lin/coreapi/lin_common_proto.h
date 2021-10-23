/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2021 NXP
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


#ifndef LIN_COMMON_PROTO_H
#define LIN_COMMON_PROTO_H

#include "lin_types.h"
#include "lin_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if (SUPPORT_MASTER_MODE == 1U)
/*
 * @brief Check to see if the current schedule table is needed to change
 *  or not. Used in tick callback function.
 *
 * @param[in] iii Interface name
 * @return void
 */
void lin_switch_sch_table(l_ifc_handle iii);

/*
 * @brief Checks if the associated flags to a sporadic frame have been updated
 *  and a transmission is needed
 *
 * @param[in] iii Interface name
 * @param[in] frm_id Index of frame
 * @return l_u8
 */
l_u8 lin_check_sporadic_update(l_ifc_handle iii,
                               l_u8 frm_id);

#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*
 * @brief This function gets signals associated with unconditional frame
 * from buffer.
 *
 * @param[in] iii Interface name
 * @param[in] id ID to process
 * @return void
 */
void lin_make_uncd_frame(l_ifc_handle iii,
                         l_u8 id);

/*
 * @brief This function packs signals associated with unconditional frame
 *  into buffer.
 *
 * @param[in] iii Interface name
 * @param[in] id ID to process
 * @return void
 */
void lin_update_uncd_frame(l_ifc_handle iii,
                           l_u8 id);

/*
 * @brief This function returns the index of a frame in frame list
 * or 0xFF if not found.
 *
 * @param[in] iii Interface name
 * @param[in] id ID to process
 * @return l_u8
 */
l_u8 lin_get_frame_index(l_ifc_handle iii,
                         l_u8 id);

#endif /* LIN_COMMON_PROTO_H */

/******************************************************************************/
/* EOF */
/******************************************************************************/
