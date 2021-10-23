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

#ifndef TIMING_IRQ_H
#define TIMING_IRQ_H

#include "device_registers.h"
#include "timing_pal_mapping.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (defined (TIMING_OVER_FTM))
/*! @brief Table of base addresses for FTM instances. */
extern FTM_Type * const ftmBase[FTM_INSTANCE_COUNT];
/* Table to save FTM channel running status */
extern bool g_ftmChannelRunning[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT];
#endif /* TIMING_OVER_FTM */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (defined (TIMING_OVER_LPIT))
void TIMING_Lpit_IrqHandler(uint32_t instance, uint8_t channel);
#endif

#if (defined (TIMING_OVER_LPTMR))
void TIMING_Lptmr_IrqHandler(uint32_t instance, uint8_t channel);
#endif

#if (defined (TIMING_OVER_FTM))
void TIMING_Ftm_IrqHandler(uint32_t instance, uint8_t channel);
#endif

#if (defined (TIMING_OVER_PIT))
void TIMING_Pit_IrqHandler(uint32_t instance, uint8_t channel);
#endif

#if (defined (TIMING_OVER_STM))
void TIMING_Stm_IrqHandler(uint32_t instance, uint8_t channel);
#endif

/*******************************************************************************
 *  Default interrupt handlers signatures
 ******************************************************************************/

/* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))

#if (FEATURE_LPIT_HAS_NUM_IRQS_CHANS == 1U)
void LPIT0_IRQHandler(void);
#else
void LPIT0_Ch0_IRQHandler(void);
void LPIT0_Ch1_IRQHandler(void);
void LPIT0_Ch2_IRQHandler(void);
void LPIT0_Ch3_IRQHandler(void);
#endif /* FEATURE_LPIT_HAS_NUM_IRQS_CHANS == 1U */

#endif /* TIMING_OVER_LPIT */

/* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))

void LPTMR0_IRQHandler(void);

#endif /* TIMING_OVER_LPTMR */

/* Define TIMING PAL over PIT */
#if (defined(TIMING_OVER_PIT))

#if (PIT_INSTANCE_COUNT > 1U)
#define PIT_Ch0_IRQHandler PIT0_Ch0_IRQHandler
#define PIT_Ch1_IRQHandler PIT0_Ch1_IRQHandler
#define PIT_Ch2_IRQHandler PIT0_Ch2_IRQHandler
#define PIT_Ch3_IRQHandler PIT0_Ch3_IRQHandler
#if (PIT_TIMER_COUNT > 4U)
#define PIT_Ch4_IRQHandler PIT0_Ch4_IRQHandler
#define PIT_Ch5_IRQHandler PIT0_Ch5_IRQHandler
#define PIT_Ch6_IRQHandler PIT0_Ch6_IRQHandler
#define PIT_Ch7_IRQHandler PIT0_Ch7_IRQHandler
#define PIT_Ch8_IRQHandler PIT0_Ch8_IRQHandler
#define PIT_Ch9_IRQHandler PIT0_Ch9_IRQHandler
#define PIT_Ch10_IRQHandler PIT0_Ch10_IRQHandler
#define PIT_Ch11_IRQHandler PIT0_Ch11_IRQHandler
#define PIT_Ch12_IRQHandler PIT0_Ch12_IRQHandler
#define PIT_Ch13_IRQHandler PIT0_Ch13_IRQHandler
#define PIT_Ch14_IRQHandler PIT0_Ch14_IRQHandler
#define PIT_Ch15_IRQHandler PIT0_Ch15_IRQHandler
#endif /* PIT_TIMER_COUNT > 4U */
#endif

#if (PIT_INSTANCE_COUNT > 0U)
void PIT_Ch0_IRQHandler(void);
void PIT_Ch1_IRQHandler(void);
void PIT_Ch2_IRQHandler(void);
void PIT_Ch3_IRQHandler(void);
#if (PIT_TIMER_COUNT > 4U)
void PIT_Ch4_IRQHandler(void);
void PIT_Ch5_IRQHandler(void);
void PIT_Ch6_IRQHandler(void);
void PIT_Ch7_IRQHandler(void);
void PIT_Ch8_IRQHandler(void);
void PIT_Ch9_IRQHandler(void);
void PIT_Ch10_IRQHandler(void);
void PIT_Ch11_IRQHandler(void);
void PIT_Ch13_IRQHandler(void);
void PIT_Ch14_IRQHandler(void);
void PIT_Ch15_IRQHandler(void);
#endif /* PIT_TIMER_COUNT > 4U */
#endif /* PIT_INSTANCE_COUNT > 0U */

#if (PIT_INSTANCE_COUNT > 1U)
void PIT1_Ch0_IRQHandler(void);
void PIT1_Ch1_IRQHandler(void);
void PIT1_Ch2_IRQHandler(void);
void PIT1_Ch3_IRQHandler(void);
#endif /* PIT_INSTANCE_COUNT > 1U */

#endif /* TIMING_OVER_PIT */

/* Define TIMING PAL over FTM */
#if (defined(TIMING_OVER_FTM))

#if (FTM_INSTANCE_COUNT > 0U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM0_Ch0_7_IrqHandler(void);
#else
void FTM0_Ch0_Ch1_IrqHandler(void);

void FTM0_Ch2_Ch3_IrqHandler(void);

void FTM0_Ch4_Ch5_IrqHandler(void);

void FTM0_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 0U */

#if (FTM_INSTANCE_COUNT > 1U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM1_Ch0_7_IrqHandler(void);
#else
void FTM1_Ch0_Ch1_IrqHandler(void);

void FTM1_Ch2_Ch3_IrqHandler(void);

void FTM1_Ch4_Ch5_IrqHandler(void);

void FTM1_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 1U */

#if (FTM_INSTANCE_COUNT > 2U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM2_Ch0_7_IrqHandler(void);
#else
void FTM2_Ch0_Ch1_IrqHandler(void);

void FTM2_Ch2_Ch3_IrqHandler(void);

void FTM2_Ch4_Ch5_IrqHandler(void);

void FTM2_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 2U */

#if (FTM_INSTANCE_COUNT > 3U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM3_Ch0_7_IrqHandler(void);
#else
void FTM3_Ch0_Ch1_IrqHandler(void);

void FTM3_Ch2_Ch3_IrqHandler(void);

void FTM3_Ch4_Ch5_IrqHandler(void);

void FTM3_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 3U */

#if (FTM_INSTANCE_COUNT > 4U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM4_Ch0_7_IrqHandler(void);
#else
void FTM4_Ch0_Ch1_IrqHandler(void);

void FTM4_Ch2_Ch3_IrqHandler(void);

void FTM4_Ch4_Ch5_IrqHandler(void);

void FTM4_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 4U */

#if (FTM_INSTANCE_COUNT > 5U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM5_Ch0_7_IrqHandler(void);
#else
void FTM5_Ch0_Ch1_IrqHandler(void);

void FTM5_Ch2_Ch3_IrqHandler(void);

void FTM5_Ch4_Ch5_IrqHandler(void);

void FTM5_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 5U */

#if (FTM_INSTANCE_COUNT > 6U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM6_Ch0_7_IrqHandler(void);
#else
void FTM6_Ch0_Ch1_IrqHandler(void);

void FTM6_Ch2_Ch3_IrqHandler(void);

void FTM6_Ch4_Ch5_IrqHandler(void);

void FTM6_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 6U */

#if (FTM_INSTANCE_COUNT > 7U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM7_Ch0_7_IrqHandler(void);
#else
void FTM7_Ch0_Ch1_IrqHandler(void);

void FTM7_Ch2_Ch3_IrqHandler(void);

void FTM7_Ch4_Ch5_IrqHandler(void);

void FTM7_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 7U */
/* Array storing references to TIMING over FTM irq handlers */
extern const isr_t s_timingOverFtmIsr[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT];

#endif /* TIMING_OVER_FTM */

/* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))

#ifdef STM_0
void STM0_Ch0_IRQHandler(void);
void STM0_Ch1_IRQHandler(void);
void STM0_Ch2_IRQHandler(void);
void STM0_Ch3_IRQHandler(void);
#endif

#ifdef STM_1
void STM1_Ch0_IRQHandler(void);
void STM1_Ch1_IRQHandler(void);
void STM1_Ch2_IRQHandler(void);
void STM1_Ch3_IRQHandler(void);
#endif

#ifdef STM_2
void STM2_Ch0_IRQHandler(void);
void STM2_Ch1_IRQHandler(void);
void STM2_Ch2_IRQHandler(void);
void STM2_Ch3_IRQHandler(void);
#endif

#endif /* TIMING_OVER_STM */

#endif /* TIMING_IRQ_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
