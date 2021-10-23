/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
 * @file lin_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
 * visible when an object or function with external linkage is defined.
 * Folder structure has only C source file for interrupt routine.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The interrupt handlers defined here are installed at runtime and called when the
 * interrupt events occur.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lin_driver.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (LPUART_INSTANCE_COUNT > 0U)
void LIN_LPUART0_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(0U);
}

#ifdef LPUART_ERR_IRQS
void LIN_LPUART0_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(0U);
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 0U) */

#if (LPUART_INSTANCE_COUNT > 1U)
void LIN_LPUART1_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(1U);
}

#ifdef LPUART_ERR_IRQS
void LIN_LPUART1_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(1U);
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 1U) */

#if (LPUART_INSTANCE_COUNT > 2U)
void LIN_LPUART2_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(2U);
}

#ifdef LPUART_ERR_IRQS
void LIN_LPUART2_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(2U);
}
#endif
#endif /* if (LPUART_INSTANCE_COUNT > 2U) */
/* ISR array for Lin LPUART driver */
isr_t g_linLpuartIsrs[LPUART_INSTANCE_COUNT] =
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART0_RxTx_IRQHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 1U)
    LIN_LPUART1_RxTx_IRQHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 2U)
    LIN_LPUART2_RxTx_IRQHandler,
#endif
};

#ifdef LPUART_ERR_IRQS
isr_t g_linLpuartErrIsrs[LPUART_INSTANCE_COUNT] =
{
#if (LPUART_INSTANCE_COUNT > 0U)
    LIN_LPUART0_ERR_IRQHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 1U)
    LIN_LPUART1_ERR_IRQHandler,
#endif
#if (LPUART_INSTANCE_COUNT > 2U)
    LIN_LPUART2_ERR_IRQHandler,
#endif
};
#endif
/******************************************************************************/
/* EOF */
/******************************************************************************/
