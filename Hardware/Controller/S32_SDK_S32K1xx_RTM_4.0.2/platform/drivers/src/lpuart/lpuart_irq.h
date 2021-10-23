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

#ifndef LPUART_IRQ_H__
#define LPUART_IRQ_H__

#include "device_registers.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void LPUART_DRV_IRQHandler(uint32_t instance);

/*******************************************************************************
 *  Default interrupt handlers signatures
 ******************************************************************************/

#if (LPUART_INSTANCE_COUNT > 0U)
/*! @brief LPUART0 interrupt handler. */
void LPUART0_IrqHandler(void);
#endif

#if (LPUART_INSTANCE_COUNT > 1U)
/*! @brief LPUART1 interrupt handler. */
void LPUART1_IrqHandler(void);
#endif

#if (LPUART_INSTANCE_COUNT > 2U)
/*! @brief LPUART2 interrupt handler. */
void LPUART2_IrqHandler(void);
#endif

#if (LPUART_INSTANCE_COUNT > 3U)
/*! @brief LPUART3 interrupt handler. */
void LPUART3_IrqHandler(void);
#endif

/*! Array storing references to LPUART irq handlers */
extern isr_t g_lpuartIsr[LPUART_INSTANCE_COUNT];

#endif /* LPUART_IRQ_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
