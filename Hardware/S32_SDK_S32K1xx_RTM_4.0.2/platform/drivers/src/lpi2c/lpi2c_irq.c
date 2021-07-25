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
 * @lpi2c_irq.c
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
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
 * visible when an object or function with external linkage is defined.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 */

#include "device_registers.h"
#include "lpi2c_driver.h"
#include "lpi2c_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined (S32K11x_SERIES)
/* Implementation of LPI2C0 master and slave handler named in startup code. */
void LPI2C0_Master_Slave_IRQHandler(void)
{
	LPI2C_DRV_ModuleIRQHandler(0);
}

#else
#if (LPI2C_INSTANCE_COUNT > 0u)
/* Implementation of LPI2C0 master handler named in startup code. */
void LPI2C0_Master_IRQHandler(void)
{
	LPI2C_DRV_MasterIRQHandler(0);
}

/* Implementation of LPI2C0 slave handler named in startup code. */
void LPI2C0_Slave_IRQHandler(void)
{
	LPI2C_DRV_SlaveIRQHandler(0);
}

#if(LPI2C_INSTANCE_COUNT == 2u)

/* Implementation of LPI2C1 master handler named in startup code. */
void LPI2C1_Master_IRQHandler(void)
{
	LPI2C_DRV_MasterIRQHandler(1);
}

/* Implementation of LPI2C1 slave handler named in startup code. */
void LPI2C1_Slave_IRQHandler(void)
{
	LPI2C_DRV_SlaveIRQHandler(1);
}

#endif

#endif

#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
