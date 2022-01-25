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
 * @file can_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef CAN_PAL_MAPPING_H
#define CAN_PAL_MAPPING_H

#include "device_registers.h"
#include "can_pal_cfg.h"

/* Include PD files */
#if (defined(CAN_OVER_FLEXCAN))
    #include "flexcan_driver.h"
#endif

/*!
 * @brief Enumeration with the types of peripherals supported by CAN PAL
 *
 * This enumeration contains the types of peripherals supported by CAN PAL.
 * Implements : can_inst_type_t_Class
 */
typedef enum {
#if defined(CAN_INSTANCE_COUNT)
    CAN_INST_TYPE_FLEXCAN = 0u,
#endif /* defined(CAN_INSTANCE_COUNT) */
} can_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : can_instance_t_Class
 */
typedef struct {
    can_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
} can_instance_t;

#endif /* CAN_PAL_MAPPING_H */
