/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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
 * @file eim_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "eim_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name :EIM_InitChannel
 * Description   :This function disables all channels and clears check-bit
 * and data masks of all the channels.
 *
 *END**************************************************************************/
void EIM_InitChannel(EIM_Type * const base)
{
    uint8_t channel;

    for(channel = 0; channel < EIM_EICHDn_COUNT; channel++)
    {
        EIM_EnableChannelCmd(base, channel, false);
        EIM_SetCheckBitMask(base, channel, 0U);
        EIM_SetDataMask(base, channel, 0U);
    #if defined(FEATURE_EIM_DATAMASK1)
        EIM_SetDataMask1(base, channel, 0U);
    #endif
    }
}

/*******************************************************************************
* EOF
******************************************************************************/


