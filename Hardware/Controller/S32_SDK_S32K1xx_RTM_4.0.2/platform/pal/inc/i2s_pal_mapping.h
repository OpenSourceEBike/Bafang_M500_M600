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
 * @file i2s_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef I2S_PAL_MAPPING_H
#define I2S_PAL_MAPPING_H
#include "device_registers.h"
#include "i2s_pal_cfg.h"

#ifdef I2S_OVER_SAI
    #include "sai_driver.h"
#endif /* I2S_OVER_SAI */

#ifdef I2S_OVER_FLEXIO
    #include "flexio_i2s_driver.h"
#endif /* I2S_OVER_FLEXIO */

/*!
 * @brief Enumeration with the types of peripherals supported by Timing PAL
 *
 * This enumeration contains the types of peripherals supported by Timing PAL.
 * Implements : i2s_inst_type_t_Class
 */
typedef enum{
#ifdef I2S_OVER_SAI
    I2S_INST_TYPE_SAI    = 0u,
#endif /* I2S_OVER_SAI  */

#ifdef I2S_OVER_FLEXIO
    I2S_INST_TYPE_FLEXIO   = 1u
#endif /* I2S_OVER_FLEXIO */
} i2s_inst_type_t;


/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 */
typedef struct{
    i2s_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
} i2s_instance_t;
#endif /* I2S_PAL_MAPPING_H */
