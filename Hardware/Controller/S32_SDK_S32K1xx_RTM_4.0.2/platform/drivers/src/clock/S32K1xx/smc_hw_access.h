/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#ifndef SMC_HW_ACCESS_H
#define SMC_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file smc_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to implement enumeration entries as macro.
 * Enumeration entries (macros) are referenced from application code.
 */

/*!
 * @ingroup smc_hw_access
 * @defgroup smc_hw_access
 * @{
 */


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/


/*!
 * @brief Gets the current running power mode.
 *
 * This function  returns the current running power mode.
 *
 * @param[in] baseAddr  Base address for current SMC instance.
 * @return stat  Current power mode stat
 */
static inline uint32_t SMC_GetCurrentRunningMode(const SMC_Type* const baseAddr)
{
    return (baseAddr->PMSTAT & SMC_PMSTAT_PMSTAT_MASK) >> SMC_PMSTAT_PMSTAT_SHIFT;
}


#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* SMC_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
