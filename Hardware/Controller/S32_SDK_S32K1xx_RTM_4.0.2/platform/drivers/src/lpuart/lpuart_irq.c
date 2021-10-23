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

/*!
 * @file lpuart_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Function not defined with external linkage.
 * The functions are not defined static because they are referenced in .s startup files.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variable is used in driver c file, so it must remain global.
 */

#include "lpuart_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (LPUART_INSTANCE_COUNT > 0U)
/* Implementation of LPUART0 handler named in startup code. */
void LPUART0_IrqHandler(void)
{
    LPUART_DRV_IRQHandler(0);
}
#endif

#if (LPUART_INSTANCE_COUNT > 1U)
/* Implementation of LPUART1 handler named in startup code. */
void LPUART1_IrqHandler(void)
{
    LPUART_DRV_IRQHandler(1);
}
#endif

#if (LPUART_INSTANCE_COUNT > 2U)
/* Implementation of LPUART2 handler named in startup code. */
void LPUART2_IrqHandler(void)
{
    LPUART_DRV_IRQHandler(2);
}
#endif

#if (LPUART_INSTANCE_COUNT > 3U)
/* Implementation of LPUART3 handler named in startup code. */
void LPUART3_IrqHandler(void)
{
    LPUART_DRV_IRQHandler(3);
}
#endif

/* Array storing references to LPUART irq handlers */
isr_t g_lpuartIsr[LPUART_INSTANCE_COUNT] =
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LPUART0_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 1U)
    LPUART1_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 2U)
    LPUART2_IrqHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 3U)
    LPUART3_IrqHandler,
#endif
};

/*******************************************************************************
 * EOF
 ******************************************************************************/
