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

#ifndef ADC_PAL_MAPPING_H
#define ADC_PAL_MAPPING_H

#include "adc_pal_cfg.h"
#include "device_registers.h"

/* Include peripheral drivers */
#if defined (ADC_PAL_S32K1xx)

#include "adc_driver.h"
#include "adc_hw_access.h"
#include "trgmux_driver.h"
#include "pdb_driver.h"

#endif /* defined(ADC_PAL_S32K1xx) */

#if defined (ADC_PAL_MPC574xC_G_R)

#include "adc_c55_driver.h"
#include "bctu_driver.h"

#endif /* defined(ADC_PAL_MPC574xC_G_R) */

#if defined (ADC_PAL_SAR_CTU)

#include "adc_c55_driver.h"
#include "ctu_driver.h"

#endif /* defined(ADC_PAL_SAR_CTU) */


/*!
 * @brief Enumeration with the types of ADC peripherals supported by ADC PAL
 *
 * This enumeration contains the types of ADC peripherals supported by ADC PAL.
 * Implements : adc_inst_type_t_Class
 */
typedef enum
{
#if (defined (PDB_INSTANCE_COUNT) && defined (TRGMUX_INSTANCE_COUNT))
    ADC_INST_TYPE_ADC_S32K1xx      = 0u,
#endif /* (defined(PDB_INSTANCE_COUNT) && defined(TRGMUX_INSTANCE_COUNT)) */

#if defined (BCTU_INSTANCE_COUNT)
    ADC_INST_TYPE_ADC_MPC574xC_G_R = 1u,
#endif /* defined(BCTU_INSTANCE_COUNT) */

#if defined (CTU_INSTANCE_COUNT)
    ADC_INST_TYPE_ADC_SAR_CTU      = 2u
#endif /* defined(CTU_INSTANCE_COUNT) */
} adc_inst_type_t;


/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : adc_instance_t_Class
 */
typedef struct
{
    adc_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral (for ADC PAL the triggering peripheral) over which the PAL is used */
} adc_instance_t;


#endif /* ADC_PAL_MAPPING_H */
