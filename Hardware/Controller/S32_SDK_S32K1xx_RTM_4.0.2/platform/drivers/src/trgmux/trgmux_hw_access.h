/*
 * Copyright 2017-2020 NXP.
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

#ifndef TRGMUX_HW_ACCESS_H
#define TRGMUX_HW_ACCESS_H

/*! @file trgmux_hw_access.h */

#include "trgmux_driver.h"

/*!
 * trgmux_hw_access TRGMUX Hardware Access
 * @details This section describes the programming interface of the TRGMUX Hardware Access.
 * @{
 */

/*******************************************************************************
* Definitions
*******************************************************************************/

/*******************************************************************************
* API
*******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Restore the TRGMUX module to reset value.
 *
 * This function restores the TRGMUX module to reset value.
 *
 * @param[in] base   The TRGMUX peripheral base address
 * @return           Execution status:
 *   STATUS_SUCCESS
 *   STATUS_ERROR    If at least one of the target module register is locked.
 */
status_t TRGMUX_Init(TRGMUX_Type * const base);

/*!
 * @brief Configures a source trigger for a target module.
 *
 * This function configures a TRGMUX link between a source trigger and a target module,
 * if the requested target module is not locked.
 *
 * @param[in] base           The TRGMUX peripheral base address
 * @param[in] triggerSource  One of the values in the trgmux_trigger_source_t enumeration
 * @param[in] targetModule   One of the values in the trgmux_target_module_t enumeration
 */
void TRGMUX_SetTrigSourceForTargetModule(TRGMUX_Type * const base,
                                         const trgmux_trigger_source_t triggerSource,
                                         const trgmux_target_module_t targetModule);

/*!
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param[in] base         The TRGMUX peripheral base address
 * @param[in] targetModule One of the values in the trgmux_target_module_t enumeration
 * @return                 Enum value corresponding to the trigger source configured for the given target module
 */
trgmux_trigger_source_t TRGMUX_GetTrigSourceForTargetModule(const TRGMUX_Type * const base,
                                                            const trgmux_target_module_t targetModule);

/*!
 * @brief Lock the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can contain up to 4
 * SEL bitfields, meaning that these registers can be used to configure up to 4 target
 * modules independently. Because the LK bit is only one per register, the configuration
 * of all target modules referred from that register will be locked.
 *
 * @param[in] base         The TRGMUX peripheral base address
 * @param[in] targetModule One of the values in the trgmux_target_module_t enumeration
 */
void TRGMUX_SetLockForTargetModule(TRGMUX_Type * const base,
                                   const trgmux_target_module_t targetModule);

/*!
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register corresponding to
 * the selected target module.
 *
 * @param[in] base         The TRGMUX peripheral base address
 * @param[in] targetModule One of the values in the trgmux_target_module_t enumeration
 * @return                 true or false depending on the state of the LK bit
 */
bool TRGMUX_GetLockForTargetModule(const TRGMUX_Type * const base,
                                   const trgmux_target_module_t targetModule);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* TRGMUX_HW_ACCESS_H */
/*******************************************************************************
* EOF
*******************************************************************************/
