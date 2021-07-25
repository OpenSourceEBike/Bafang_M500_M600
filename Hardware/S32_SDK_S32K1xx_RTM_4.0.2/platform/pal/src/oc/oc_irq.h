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

#ifndef OC_IRQ_H
#define OC_IRQ_H

#include "oc_pal_mapping.h"
#include "interrupt_manager.h"


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void OC_IrqHandler(uint32_t instance, uint8_t channel, oc_inst_type_t instType);

/*******************************************************************************
 *  Default interrupt handlers signatures
 ******************************************************************************/

/* Define OC PAL over FTM */
#if (defined(OC_PAL_OVER_FTM))

#if (OC_PAL_INSTANCE_MAX_NUM > 0U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM0_Ch0_7_IrqHandler(void);
#else
void FTM0_Ch0_Ch1_IrqHandler(void);

void FTM0_Ch2_Ch3_IrqHandler(void);

void FTM0_Ch4_Ch5_IrqHandler(void);

void FTM0_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* OC_PAL_INSTANCE_MAX_NUM > 0U */

#if (OC_PAL_INSTANCE_MAX_NUM > 1U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM1_Ch0_7_IrqHandler(void);
#else
void FTM1_Ch0_Ch1_IrqHandler(void);

void FTM1_Ch2_Ch3_IrqHandler(void);

void FTM1_Ch4_Ch5_IrqHandler(void);

void FTM1_Ch6_Ch7_IrqHandler(void);
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* OC_PAL_INSTANCE_MAX_NUM > 1U */

#if (OC_PAL_INSTANCE_MAX_NUM > 2U)
void FTM2_Ch0_Ch1_IrqHandler(void);

void FTM2_Ch2_Ch3_IrqHandler(void);

void FTM2_Ch4_Ch5_IrqHandler(void);

void FTM2_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 2U */

#if (OC_PAL_INSTANCE_MAX_NUM > 3U)
void FTM3_Ch0_Ch1_IrqHandler(void);

void FTM3_Ch2_Ch3_IrqHandler(void);

void FTM3_Ch4_Ch5_IrqHandler(void);

void FTM3_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 3U */

#if (OC_PAL_INSTANCE_MAX_NUM > 4U)
void FTM4_Ch0_Ch1_IrqHandler(void);

void FTM4_Ch2_Ch3_IrqHandler(void);

void FTM4_Ch4_Ch5_IrqHandler(void);

void FTM4_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 4U */

#if (OC_PAL_INSTANCE_MAX_NUM > 5U)
void FTM5_Ch0_Ch1_IrqHandler(void);

void FTM5_Ch2_Ch3_IrqHandler(void);

void FTM5_Ch4_Ch5_IrqHandler(void);

void FTM5_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 5U */

#if (OC_PAL_INSTANCE_MAX_NUM > 6U)
void FTM6_Ch0_Ch1_IrqHandler(void);

void FTM6_Ch2_Ch3_IrqHandler(void);

void FTM6_Ch4_Ch5_IrqHandler(void);

void FTM6_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 6U */

#if (OC_PAL_INSTANCE_MAX_NUM > 7U)
void FTM7_Ch0_Ch1_IrqHandler(void);

void FTM7_Ch2_Ch3_IrqHandler(void);

void FTM7_Ch4_Ch5_IrqHandler(void);

void FTM7_Ch6_Ch7_IrqHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 7U */
/* Array storing references to OC PAL over FTM irq handlers */
extern const isr_t s_ocOverFtmIsr[OC_PAL_INSTANCE_MAX_NUM][FTM_CONTROLS_COUNT];

#endif /* OC_PAL_OVER_FTM */

#if (defined(OC_PAL_OVER_EMIOS))

#if (OC_PAL_INSTANCE_MAX_NUM > 0U)
void EMIOS0_00_01_IRQHandler(void);
void EMIOS0_02_03_IRQHandler(void);
void EMIOS0_04_05_IRQHandler(void);
void EMIOS0_06_07_IRQHandler(void);
void EMIOS0_08_09_IRQHandler(void);
void EMIOS0_10_11_IRQHandler(void);
void EMIOS0_12_13_IRQHandler(void);
void EMIOS0_14_15_IRQHandler(void);
void EMIOS0_16_17_IRQHandler(void);
void EMIOS0_18_19_IRQHandler(void);
void EMIOS0_20_21_IRQHandler(void);
void EMIOS0_22_23_IRQHandler(void);
void EMIOS0_24_25_IRQHandler(void);
void EMIOS0_26_27_IRQHandler(void);
void EMIOS0_28_29_IRQHandler(void);
void EMIOS0_30_31_IRQHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 0U */

#if (OC_PAL_INSTANCE_MAX_NUM > 1U)
void EMIOS1_00_01_IRQHandler(void);
void EMIOS1_02_03_IRQHandler(void);
void EMIOS1_04_05_IRQHandler(void);
void EMIOS1_06_07_IRQHandler(void);
void EMIOS1_08_09_IRQHandler(void);
void EMIOS1_10_11_IRQHandler(void);
void EMIOS1_12_13_IRQHandler(void);
void EMIOS1_14_15_IRQHandler(void);
void EMIOS1_16_17_IRQHandler(void);
void EMIOS1_18_19_IRQHandler(void);
void EMIOS1_20_21_IRQHandler(void);
void EMIOS1_22_23_IRQHandler(void);
void EMIOS1_24_25_IRQHandler(void);
void EMIOS1_26_27_IRQHandler(void);
void EMIOS1_28_29_IRQHandler(void);
void EMIOS1_30_31_IRQHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 1U */

#if (OC_PAL_INSTANCE_MAX_NUM > 2U)
void EMIOS2_00_01_IRQHandler(void);
void EMIOS2_02_03_IRQHandler(void);
void EMIOS2_04_05_IRQHandler(void);
void EMIOS2_06_07_IRQHandler(void);
void EMIOS2_08_09_IRQHandler(void);
void EMIOS2_10_11_IRQHandler(void);
void EMIOS2_12_13_IRQHandler(void);
void EMIOS2_14_15_IRQHandler(void);
void EMIOS2_16_17_IRQHandler(void);
void EMIOS2_18_19_IRQHandler(void);
void EMIOS2_20_21_IRQHandler(void);
void EMIOS2_22_23_IRQHandler(void);
void EMIOS2_24_25_IRQHandler(void);
void EMIOS2_26_27_IRQHandler(void);
void EMIOS2_28_29_IRQHandler(void);
void EMIOS2_30_31_IRQHandler(void);
#endif /* OC_PAL_INSTANCE_MAX_NUM > 2U */

#endif /* defined(OC_PAL_OVER_EMIOS) */

#if (defined(OC_PAL_OVER_ETIMER))

#if (OC_ETIMER_INSTANCE_MAX_NUM > 0U)
void ETIMER0_TC0IR_IRQHandler(void);
void ETIMER0_TC1IR_IRQHandler(void);
void ETIMER0_TC2IR_IRQHandler(void);
void ETIMER0_TC3IR_IRQHandler(void);
void ETIMER0_TC4IR_IRQHandler(void);
void ETIMER0_TC5IR_IRQHandler(void);
#endif /* OC_ETIMER_INSTANCE_MAX_NUM > 0U */

#if (OC_ETIMER_INSTANCE_MAX_NUM > 1U)
void ETIMER1_TC0IR_IRQHandler(void);
void ETIMER1_TC1IR_IRQHandler(void);
void ETIMER1_TC2IR_IRQHandler(void);
void ETIMER1_TC3IR_IRQHandler(void);
void ETIMER1_TC4IR_IRQHandler(void);
void ETIMER1_TC5IR_IRQHandler(void);
#endif /* OC_ETIMER_INSTANCE_MAX_NUM > 1U */

#if (OC_ETIMER_INSTANCE_MAX_NUM > 2U)
void ETIMER2_TC0IR_IRQHandler(void);
void ETIMER2_TC1IR_IRQHandler(void);
void ETIMER2_TC2IR_IRQHandler(void);
void ETIMER2_TC3IR_IRQHandler(void);
void ETIMER2_TC4IR_IRQHandler(void);
void ETIMER2_TC5IR_IRQHandler(void);
#endif /* OC_ETIMER_INSTANCE_MAX_NUM > 2U */

#endif /* defined(OC_PAL_OVER_ETIMER) */

#if (defined(OC_PAL_OVER_FLEXPWM))

#if (OC_FLEXPWM_INSTANCE_MAX_NUM > 0U)
void FlexPWM_0_COF0_IRQHandler(void);
void FlexPWM_0_COF1_IRQHandler(void);
void FlexPWM_0_COF2_IRQHandler(void);
void FlexPWM_0_COF3_IRQHandler(void);
#endif /* OC_FLEXPWM_INSTANCE_MAX_NUM > 0U */

#if (OC_FLEXPWM_INSTANCE_MAX_NUM > 1U)
void FlexPWM_1_COF0_IRQHandler(void);
void FlexPWM_1_COF1_IRQHandler(void);
void FlexPWM_1_COF2_IRQHandler(void);
void FlexPWM_1_COF3_IRQHandler(void);
#endif /* OC_FLEXPWM_INSTANCE_MAX_NUM > 1U */

#endif /* defined(OC_PAL_OVER_FLEXPWM) */
#endif /* OC_IRQ_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
