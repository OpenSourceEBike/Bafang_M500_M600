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

#ifndef PCC_HW_ACCESS_H
#define PCC_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file pcc_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to implement enumeration entries as macro.
 * Enumeration entries (macros) are referenced from application code.
 */

/*!
 * @ingroup pcc_hw_access
 * @defgroup pcc_hw_access
 * @{
 */


/*! @brief Clock name mappings
 *         Constant array storing the mappings between clock names and peripheral clock control indexes.
 *         If there is no peripheral clock control index for a clock name, then the corresponding value is
 *         PCC_INVALID_INDEX.
 */
extern const uint16_t clockNameMappings[CLOCK_NAME_COUNT];

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/


/*!
* @brief Sets SOSC control register
*
* @param[in] base        pcc base pointer
* @param[in] monitorMode clock monitor enablement
* @param[in] clockGate    control register can be written or not
*/
static inline void PCC_SetPeripheralClockControl(PCC_Type* base, clock_names_t clockName, bool clockGate, uint32_t clockSource, uint32_t divider, uint32_t multiplier)
{
   /* Configure the peripheral clock source, the fractional clock divider and the clock gate */
   uint32_t value =  PCC_PCCn_PCS(clockSource)              |
                     PCC_PCCn_FRAC(multiplier)              |
                     PCC_PCCn_PCD(divider)                  |
                     PCC_PCCn_CGC(clockGate ? 1UL : 0UL );

   base->PCCn[clockNameMappings[clockName]] = value;
}

/*!
* @brief Enables/disables the clock for a given peripheral.
* For example, to enable the ADC0 clock, use like this:
* @code
*  PCC_SetClockMode(PCC, PCC_ADC0_CLOCK, true);
* @endcode
*
* @param[in] base        pcc base pointer
* @param[in] clockName   is the name of the peripheral clock
* must be one of the following values (see the clock_names_t type from S32K144_clock_names.h)
*    PCC_DMA0_CLOCK
*    PCC_MPU0_CLOCK
*    ...
*    PCC_LPUART3_CLOCK
* @param[in] isClockEnabled  is the value of the command that enables/disables the clock
*/
static inline void PCC_SetClockMode(PCC_Type* const base,
                                       const clock_names_t clockName,
                                       const bool isClockEnabled)
{
   if (isClockEnabled)
   {
       base->PCCn[clockNameMappings[clockName]] |= PCC_PCCn_CGC(1UL);
   }
   else
   {
       base->PCCn[clockNameMappings[clockName]] &= (uint32_t)(~(PCC_PCCn_CGC_MASK));
   }
}



/*!
* @brief Gets the clock gate control mode.
*
* @param[in] base        pcc base pointer
* @param[in] clockName   is the name of the peripheral clock
* must be one of the following values (see the clock_names_t type from S32K144_clock_names.h)
*    PCC_DMA0_CLOCK
*    PCC_MPU0_CLOCK
*    ...
*    PCC_LPUART3_CLOCK
* @return  the clock gate control mode
*        - false : Clock is disabled
*        - true : Clock is enabled
*/
static inline bool PCC_GetClockMode(const PCC_Type* const base,
                                       const clock_names_t clockName)
{
   uint32_t regValue = (uint32_t)base->PCCn[clockNameMappings[clockName]];
   regValue = (regValue & PCC_PCCn_CGC_MASK) >> PCC_PCCn_CGC_SHIFT;
   return (regValue == 0U) ? false : true;
}

/*!
* @brief Gets the selection of a clock source for a specific peripheral
*
* @param[in] base        pcc base pointer
* @param[in] clockName   is the name of the peripheral clock
* must be one of the following values (see the clock_names_t type from S32K144_clock_names.h)
*    PCC_DMA0_CLOCK
*    PCC_MPU0_CLOCK
*    ...
*    PCC_LPUART3_CLOCK
* @return  the clock source
*/
static inline uint32_t PCC_GetClockSourceSel(const PCC_Type* const base,
                                                                 const clock_names_t clockName)
{
   return ((base->PCCn[clockNameMappings[clockName]] & PCC_PCCn_PCS_MASK) >> PCC_PCCn_PCS_SHIFT);
}

/*!
* @brief Gets the selection of the fractional value for a specific peripheral
*
* @param[in] base        pcc base pointer
* @param[in] clockName   is the name of the peripheral clock
* must be one of the following values (see the clock_names_t type from S32K144_clock_names.h)
*    PCC_DMA0_CLOCK
*    PCC_MPU0_CLOCK
*    ...
*    PCC_LPUART3_CLOCK
* @return  the fractional value
*        - PCC_MULTPCCnLY_BY_ONE : Fractional value is zero
*        - PCC_MULTPCCnLY_BY_TWO : Fractional value is one
*/
static inline uint32_t PCC_GetFracValueSel(const PCC_Type* const base,
                                               const clock_names_t clockName)
{
   return ((base->PCCn[clockNameMappings[clockName]] & PCC_PCCn_FRAC_MASK) >> PCC_PCCn_FRAC_SHIFT);
}

/*!
* @brief Gets the selection of the divider value for a specific peripheral
*
* @param[in] base        pcc base pointer
* @param[in] clockName   is the name of the peripheral clock
* must be one of the following values (see the clock_names_t type from S32K144_clock_names.h)
*    PCC_DMA0_CLOCK
*    PCC_MPU0_CLOCK
*    ...
*    PCC_LPUART3_CLOCK
* @return  the divider value
*        - PCC_DIVIDE_BY_ONE   : Divide by 1
*        - PCC_DIVIDE_BY_TWO   : Divide by 2
*        - PCC_DIVIDE_BY_THREE : Divide by 3
*        - PCC_DIVIDE_BY_FOUR  : Divide by 4
*        - PCC_DIVIDE_BY_FIVE  : Divide by 5
*        - PCC_DIVIDE_BY_SIX   : Divide by 6
*        - PCC_DIVIDE_BY_SEVEN : Divide by 7
*        - PCC_DIVIDE_BY_EIGTH : Divide by 8
*/
static inline uint32_t PCC_GetDividerSel(const PCC_Type* const base,
                                             const clock_names_t clockName)
{
   return ((base->PCCn[clockNameMappings[clockName]] & PCC_PCCn_PCD_MASK) >> PCC_PCCn_PCD_SHIFT);
}





#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* PCC_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
