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

#ifndef PMC_HW_ACCESS_H
#define PMC_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file pmc_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to implement enumeration entries as macro.
 * Enumeration entries (macros) are referenced from application code.
 */

/*!
 * @ingroup pmc_hw_access
 * @defgroup pmc_hw_access
 * @{
 */


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/


/*!
 * @brief Enables/Disables the Low Power Oscillator.
 *
 * This function  enables/disables the Low Power Oscillator.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] enable    enable/disable the Low Power Oscillator.
 */
static inline void PMC_SetLpoMode(PMC_Type* const baseAddr, const bool enable)
{
    uint8_t regValue = baseAddr->REGSC;
    regValue &= (uint8_t)(~(PMC_REGSC_LPODIS_MASK));
    regValue |= (uint8_t)PMC_REGSC_LPODIS(enable?0U:1U);
    baseAddr->REGSC = regValue;
}

/*!
 * @brief Gets the Low Power Oscillator status.
 *
 * This function gets the Low Power Oscillator status.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @return value LPO status
 *               false - LPO is disabled
 *               true - LPO is enabled
 */
static inline bool PMC_GetLpoMode(const PMC_Type * const baseAddr)
{
    uint8_t regValue = baseAddr->REGSC;
    regValue = (uint8_t)((regValue & PMC_REGSC_LPODIS_MASK) >> PMC_REGSC_LPODIS_SHIFT);
    return (regValue == 0U) ? true : false;
}


/*!
 * @brief Low Power Oscillator Trimming Value
 *
 * This function sets the trimming value for the low power oscillator
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] value     Trimming value
 */
static inline void PMC_SetLpoTrimValue(PMC_Type* const baseAddr, const int8_t decimalValue)
{
    int8_t decValue = decimalValue;
    uint8_t lpotrim, trimval, regValue;

    if (decValue < 0)
    {
        lpotrim = ((uint8_t)1U) << (PMC_LPOTRIM_LPOTRIM_WIDTH);
        decValue = (int8_t)(decValue + (int8_t)(lpotrim));
    }
    trimval = (uint8_t)decValue;

    DEV_ASSERT(trimval <= (1U << PMC_LPOTRIM_LPOTRIM_WIDTH));

    regValue = baseAddr->LPOTRIM;
    regValue &= (uint8_t)(~(PMC_LPOTRIM_LPOTRIM_MASK));
    regValue |= (uint8_t)PMC_LPOTRIM_LPOTRIM(trimval);
    baseAddr->LPOTRIM = regValue;
}


#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* PMC_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
