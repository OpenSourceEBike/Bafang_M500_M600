/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "erm_hw_access.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_Init
 * Description   : This function initializes the module to default configuration,
 * the configuration register is initialized with interrupt notification disabled
 * for all channels and the status register events are cleared.
 *
 *END**************************************************************************/
void ERM_Init(ERM_Type * const base)
{
    base->CR0 = 0UL;
    /* Write 1 to clear flags */
    base->SR0 = 0xFFFFFFFFUL;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
