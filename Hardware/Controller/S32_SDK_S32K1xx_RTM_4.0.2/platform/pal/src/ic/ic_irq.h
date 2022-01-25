/*
 * Copyright 2018-2020 NXP
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

#ifndef IC_IRQ_H
#define IC_IRQ_H

#include "device_registers.h"
#include "ic_pal_mapping.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (defined (IC_PAL_OVER_EMIOS))
void IC_Emios_IrqHandler(uint32_t instance,
                         uint8_t channel);
#endif

#if (defined (IC_PAL_OVER_ETIMER))
void IC_Etimer_IrqHandler(uint32_t instance,
                          uint8_t channel);
#endif

#if (defined (IC_PAL_OVER_FLEXPWM))
void IC_Flexpwm_IrqHandler(uint32_t instance,
                           uint8_t channel);
#endif
/*******************************************************************************
 *  Default interrupt handlers signatures
 ******************************************************************************/
#if (defined(IC_PAL_OVER_EMIOS))

#if (IC_PAL_INSTANCE_MAX > 0U)
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
#endif

#if (IC_PAL_INSTANCE_MAX > 1U)
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
#endif

#if (IC_PAL_INSTANCE_MAX > 2U)
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
#endif

#endif /* defined(IC_PAL_OVER_EMIOS) */

#if (defined(IC_PAL_OVER_ETIMER))

#if (IC_PAL_ETIMER_INSTANCE_COUNT > 0U)
void ETIMER0_TC0IR_IRQHandler(void);
void ETIMER0_TC1IR_IRQHandler(void);
void ETIMER0_TC2IR_IRQHandler(void);
void ETIMER0_TC3IR_IRQHandler(void);
void ETIMER0_TC4IR_IRQHandler(void);
void ETIMER0_TC5IR_IRQHandler(void);
void ETIMER0_TC6IR_IRQHandler(void);
#endif

#if (IC_PAL_ETIMER_INSTANCE_COUNT > 1U)
void ETIMER1_TC0IR_IRQHandler(void);
void ETIMER1_TC1IR_IRQHandler(void);
void ETIMER1_TC2IR_IRQHandler(void);
void ETIMER1_TC3IR_IRQHandler(void);
void ETIMER1_TC4IR_IRQHandler(void);
void ETIMER1_TC5IR_IRQHandler(void);
void ETIMER1_TC6IR_IRQHandler(void);
void ETIMER1_TC7IR_IRQHandler(void);
#endif

#if (IC_PAL_ETIMER_INSTANCE_COUNT > 2U)
void ETIMER2_TC0IR_IRQHandler(void);
void ETIMER2_TC1IR_IRQHandler(void);
void ETIMER2_TC2IR_IRQHandler(void);
void ETIMER2_TC3IR_IRQHandler(void);
void ETIMER2_TC4IR_IRQHandler(void);
void ETIMER2_TC5IR_IRQHandler(void);
#endif

#endif /* defined(IC_PAL_OVER_ETIMER) */

#if (defined(IC_PAL_OVER_FLEXPWM))

#if (IC_PAL_FLEXPWM_INSTANCE_COUNT > 0U)
void FlexPWM_0_CAF0_IRQHandler(void);
void FlexPWM_0_CAF1_IRQHandler(void);
void FlexPWM_0_CAF2_IRQHandler(void);
void FlexPWM_0_CAF3_IRQHandler(void);
#endif

#if (IC_PAL_FLEXPWM_INSTANCE_COUNT > 1U)
void FlexPWM_1_CAF0_IRQHandler(void);
void FlexPWM_1_CAF1_IRQHandler(void);
void FlexPWM_1_CAF2_IRQHandler(void);
void FlexPWM_1_CAF3_IRQHandler(void);
#endif

#endif /* defined(IC_PAL_OVER_FLEXPWM) */

#endif  /* IC_IRQ_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
