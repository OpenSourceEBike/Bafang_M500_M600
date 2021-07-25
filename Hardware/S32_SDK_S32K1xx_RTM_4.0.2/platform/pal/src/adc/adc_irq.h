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

#ifndef ADC_IRQ_H
#define ADC_IRQ_H

#include "device_registers.h"
#include "adc_pal_mapping.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined (ADC_PAL_S32K1xx)

void ADC_S32K1xx_IrqHandler(const uint32_t instIdx);

#endif /* defined(ADC_PAL_S32K1xx) */


#if defined (ADC_PAL_MPC574xC_G_R)

void ADC_MPC574xC_G_R_HwTrigIrqHandler(const uint32_t instIdx,
                                       const uint8_t adcIdx);

#endif /* defined(ADC_PAL_MPC574xC_G_R) */

#if defined (ADC_PAL_SAR_CTU)

void ADC_SAR_CTU_HwTrigIrqHandler(const uint32_t instIdx,
                                  const uint8_t fifoIdx);

#endif /* defined(ADC_PAL_SAR_CTU) */

#if (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

void ADC_SAR_CTU_SwTrigIrqHandler(const uint32_t instIdx,
                                  const uint8_t adcIdx);

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */

#endif /* ADC_IRQ_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
