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
 * @file mpu_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros defined are used to define features for each driver, so this might be reported
 * when the analysis is made only on one driver.
 *
 */

#ifndef MPU_PAL_MAPPING_H
#define MPU_PAL_MAPPING_H

#include "device_registers.h"
#include "mpu_pal_cfg.h"

/* Include PD files */
#if defined(MPU_OVER_MPU)
    #include "mpu_driver.h"
#elif defined(MPU_OVER_SMPU)
    #include "smpu_driver.h"
#endif

/*!
 * @ingroup mpu_pal_code
 * @{
 */

/*!
 * @brief Enumeration with the types of peripherals supported by MPU PAL
 *
 * This enumeration contains the types of peripherals supported by MPU PAL.
 * Implements : mpu_inst_type_t_Class
 */
typedef enum
{
#if defined(MPU_INSTANCE_COUNT)
    MPU_INST_TYPE_MPU    = 0U,
#endif /* defined(MPU_INSTANCE_COUNT)  */

#if defined(SMPU_INSTANCE_COUNT)
    MPU_INST_TYPE_SMPU   = 1U,
#endif /* defined(SMPU_INSTANCE_COUNT) */
} mpu_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : mpu_instance_t_Class
 */
typedef struct
{
    mpu_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
} mpu_instance_t;

#endif /* MPU_PAL_MAPPING_H */

/*! @} */
