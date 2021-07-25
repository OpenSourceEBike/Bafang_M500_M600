/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
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
 * @file edma_irq.h
 */

#include "edma_hw_access.h"

/*! @brief DMA channel interrupt handler, implemented in driver c file. */
void EDMA_DRV_IRQHandler(uint8_t virtualChannel);
#ifdef FEATURE_DMA_HAS_ERROR_IRQ
/*! @brief DMA error interrupt handler, implemented in driver c file. */
void EDMA_DRV_ErrorIRQHandler(uint8_t virtualChannel);
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
