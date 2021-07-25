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
 * @file lptmr_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 */

#include "lptmr_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_Init
 * Description   : This function configures all registers of the LPTMR instance to reset value.
 *
 *END**************************************************************************/
void LPTMR_Init(LPTMR_Type* const base)
{
    DEV_ASSERT(base != NULL);

    /* First, disable the module so we can write the registers */
    uint32_t tmp = base->CSR;
    tmp &= ~(LPTMR_CSR_TEN_MASK | LPTMR_CSR_TCF_MASK);
    tmp |= LPTMR_CSR_TEN(0u);
    base->CSR = tmp;

    base->CSR = LPTMR_CSR_TEN(0u) | \
                LPTMR_CSR_TMS(0u) | \
                LPTMR_CSR_TFC(0u) | \
                LPTMR_CSR_TPP(0u) | \
                LPTMR_CSR_TPS(0u) | \
                LPTMR_CSR_TIE(0u) | \
                LPTMR_CSR_TCF(0u) | \
                LPTMR_CSR_TDRE(0u);

    base->PSR = LPTMR_PSR_PCS(0u) | \
                LPTMR_PSR_PBYP(0u) | \
                LPTMR_PSR_PRESCALE(0u);

    base->CMR = LPTMR_CMR_COMPARE(0u);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
