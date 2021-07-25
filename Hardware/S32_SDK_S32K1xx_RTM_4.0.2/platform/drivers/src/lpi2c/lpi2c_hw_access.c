/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

 /*!
 * @lpi2c_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */
 
#include "lpi2c_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_Init
 * Description   : This function initializes the LPI2C module to a known
 * state (use software reset bit to reset the module).
 *
 * Implements : LPI2C_Init_Activity
 *END**************************************************************************/
void LPI2C_Init(LPI2C_Type *baseAddr)
{
    /*
     * The MCR[RST] will reset all master logic and registers to their default state,
     * except for the MCR itself. The SCR[RST] will reset all slave logic and registers
     * to their default state, except for the SCR itself. So we must reset the control
     * registers manually;
     */
    LPI2C_Set_MasterSoftwareReset(baseAddr, true);
    LPI2C_Set_SlaveSoftwareReset(baseAddr, true);

    baseAddr->MCR = 0x0U;
    baseAddr->SCR = 0x0U;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
