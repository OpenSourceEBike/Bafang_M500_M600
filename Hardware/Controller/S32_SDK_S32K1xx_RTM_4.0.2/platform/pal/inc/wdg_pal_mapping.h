/*
 * Copyright 2017-2020 NXP
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
 * @file wdg_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros defined are used to define features for each driver, so this might be reported
 * when the analysis is made only on one driver.
 *
 */

#ifndef WDG_PAL_MAPPING_H
#define WDG_PAL_MAPPING_H

#include "device_registers.h"

/*!
 * @ingroup wdg_pal_code
 */

/*!
 * @brief Enumeration with the types of peripherals supported by Watchdog PAL
 *
 * This enumeration contains the types of peripherals supported by Watchdog PAL.
 * Implements : wdg_inst_type_t_Class
 */
typedef enum
{
#if defined(WDOG_INSTANCE_COUNT)
    WDG_INST_TYPE_WDOG    = 0U,
#endif /* defined(WDOG_INSTANCE_COUNT) */

#if defined(EWM_INSTANCE_COUNT)
    WDG_INST_TYPE_EWM     = 1U,
#endif /* defined(EWM_INSTANCE_COUNT) */

#if defined(SWT_INSTANCE_COUNT)
    WDG_INST_TYPE_SWT     = 2U
#endif /* defined(SWT_INSTANCE_COUNT) */
} wdg_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : wdg_instance_t_Class
 */
typedef struct{
    wdg_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t        instIdx;     /*!< Instance index of the peripheral over which the PAL is used */
} wdg_instance_t;

#endif /* WDG_PAL_MAPPING_H */
