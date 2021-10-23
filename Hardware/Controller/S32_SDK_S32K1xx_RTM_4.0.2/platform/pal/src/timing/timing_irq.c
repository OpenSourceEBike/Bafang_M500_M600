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

/*!
 * @file timing_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * These are symbols weak symbols defined in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Function not defined with external linkage.
 * The functions are not defined static because they are referenced in .s startup files.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variable is used in driver c file, so it must remain global.
 */

#include "timing_irq.h"
#include "device_registers.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Define interrupt handler for Timing instance */

/* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))

#if (FEATURE_LPIT_HAS_NUM_IRQS_CHANS == 1U)
void LPIT0_IRQHandler(void)
{
    uint32_t chanIntFlags = LPIT_DRV_GetInterruptFlagTimerChannels(0U, 0xFU);
    bool chan0IntFlag = ((chanIntFlags & LPIT_MSR_TIF0_MASK) != 0U) ? true : false;
    bool chan1IntFlag = ((chanIntFlags & LPIT_MSR_TIF1_MASK) != 0U) ? true : false;
    bool chan2IntFlag = ((chanIntFlags & LPIT_MSR_TIF2_MASK) != 0U) ? true : false;
    bool chan3IntFlag = ((chanIntFlags & LPIT_MSR_TIF3_MASK) != 0U) ? true : false;

    if (chan0IntFlag)
    {
        TIMING_Lpit_IrqHandler(0U, 0U);
    }

    if (chan1IntFlag)
    {
        TIMING_Lpit_IrqHandler(0U, 1U);
    }

    if (chan2IntFlag)
    {
        TIMING_Lpit_IrqHandler(0U, 2U);
    }

    if (chan3IntFlag)
    {
        TIMING_Lpit_IrqHandler(0U, 3U);
    }
}
#else
void LPIT0_Ch0_IRQHandler(void)
{
    TIMING_Lpit_IrqHandler(0U, 0U);
}

void LPIT0_Ch1_IRQHandler(void)
{
    TIMING_Lpit_IrqHandler(0U, 1U);
}

void LPIT0_Ch2_IRQHandler(void)
{
    TIMING_Lpit_IrqHandler(0U, 2U);
}

void LPIT0_Ch3_IRQHandler(void)
{
    TIMING_Lpit_IrqHandler(0U, 3U);
}
#endif /* FEATURE_LPIT_HAS_NUM_IRQS_CHANS == 1U */

#endif /* TIMING_OVER_LPIT */

/* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))

void LPTMR0_IRQHandler(void)
{
    TIMING_Lptmr_IrqHandler(0U, 0U);
}

#endif /* TIMING_OVER_LPTMR */

/* Define TIMING PAL over PIT */
#if (defined(TIMING_OVER_PIT))

#if (PIT_INSTANCE_COUNT > 0U)
void PIT_Ch0_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 0U);
}

void PIT_Ch1_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 1U);
}

void PIT_Ch2_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 2U);
}

void PIT_Ch3_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 3U);
}
#if (PIT_TIMER_COUNT > 4U)
void PIT_Ch4_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 4U);
}

void PIT_Ch5_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 5U);
}

void PIT_Ch6_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 6U);
}

void PIT_Ch7_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 7U);
}

void PIT_Ch8_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 8U);
}

void PIT_Ch9_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 9U);
}

void PIT_Ch10_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 10U);
}

void PIT_Ch11_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 11U);
}

void PIT_Ch12_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 12U);
}

void PIT_Ch13_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 13U);
}

void PIT_Ch14_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 14U);
}

void PIT_Ch15_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(0U, 15U);
}
#endif /* PIT_TIMER_COUNT > 4U */
#endif /* PIT_INSTANCE_COUNT > 0U */

#if (PIT_INSTANCE_COUNT > 1U)
void PIT1_Ch0_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(1U, 0U);
}

void PIT1_Ch1_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(1U, 1U);
}

void PIT1_Ch2_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(1U, 2U);
}

void PIT1_Ch3_IRQHandler(void)
{
    TIMING_Pit_IrqHandler(1U, 3U);
}
#endif /* PIT_INSTANCE_COUNT > 1U */


#endif /* TIMING_OVER_PIT */

/* Define TIMING PAL over FTM */
#if (defined(TIMING_OVER_FTM))

#if (FTM_INSTANCE_COUNT > 0U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM0_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[0];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[0][0])
    {
        TIMING_Ftm_IrqHandler(0U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[0][1])
    {
        TIMING_Ftm_IrqHandler(0U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[0][2])
    {
        TIMING_Ftm_IrqHandler(0U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[0][3])
    {
        TIMING_Ftm_IrqHandler(0U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[0][4])
    {
        TIMING_Ftm_IrqHandler(0U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[0][5])
    {
        TIMING_Ftm_IrqHandler(0U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[0][6])
    {
        TIMING_Ftm_IrqHandler(0U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[0][7])
    {
        TIMING_Ftm_IrqHandler(0U, 7U);
    }
}
#else
void FTM0_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[0];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[0][0])
    {
        TIMING_Ftm_IrqHandler(0U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[0][1])
    {
        TIMING_Ftm_IrqHandler(0U, 1U);
    }
}

void FTM0_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[0];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[0][2])
    {
        TIMING_Ftm_IrqHandler(0U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[0][3])
    {
        TIMING_Ftm_IrqHandler(0U, 3U);
    }
}

void FTM0_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[0];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[0][4])
    {
        TIMING_Ftm_IrqHandler(0U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[0][5])
    {
        TIMING_Ftm_IrqHandler(0U, 5U);
    }
}

void FTM0_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[0];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[0][6])
    {
        TIMING_Ftm_IrqHandler(0U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[0][7])
    {
        TIMING_Ftm_IrqHandler(0U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 0U */

#if (FTM_INSTANCE_COUNT > 1U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM1_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[1];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[1][0])
    {
        TIMING_Ftm_IrqHandler(1U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[1][1])
    {
        TIMING_Ftm_IrqHandler(1U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[1][2])
    {
        TIMING_Ftm_IrqHandler(1U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[1][3])
    {
        TIMING_Ftm_IrqHandler(1U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[1][4])
    {
        TIMING_Ftm_IrqHandler(1U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[1][5])
    {
        TIMING_Ftm_IrqHandler(1U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[1][6])
    {
        TIMING_Ftm_IrqHandler(1U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[1][7])
    {
        TIMING_Ftm_IrqHandler(1U, 7U);
    }
}
#else
void FTM1_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[1];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[1][0])
    {
        TIMING_Ftm_IrqHandler(1U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[1][1])
    {
        TIMING_Ftm_IrqHandler(1U, 1U);
    }
}

void FTM1_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[1];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[1][2])
    {
        TIMING_Ftm_IrqHandler(1U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[1][3])
    {
        TIMING_Ftm_IrqHandler(1U, 3U);
    }
}

void FTM1_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[1];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[1][4])
    {
        TIMING_Ftm_IrqHandler(1U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[1][5])
    {
        TIMING_Ftm_IrqHandler(1U, 5U);
    }
}

void FTM1_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[1];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[1][6])
    {
        TIMING_Ftm_IrqHandler(1U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[1][7])
    {
        TIMING_Ftm_IrqHandler(1U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 1U */

#if (FTM_INSTANCE_COUNT > 2U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM2_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[2];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[2][0])
    {
        TIMING_Ftm_IrqHandler(2U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[2][1])
    {
        TIMING_Ftm_IrqHandler(2U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[2][2])
    {
        TIMING_Ftm_IrqHandler(2U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[2][3])
    {
        TIMING_Ftm_IrqHandler(2U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[2][4])
    {
        TIMING_Ftm_IrqHandler(2U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[2][5])
    {
        TIMING_Ftm_IrqHandler(2U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[2][6])
    {
        TIMING_Ftm_IrqHandler(2U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[2][7])
    {
        TIMING_Ftm_IrqHandler(2U, 7U);
    }
}
#else
void FTM2_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[2];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[2][0])
    {
        TIMING_Ftm_IrqHandler(2U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[2][1])
    {
        TIMING_Ftm_IrqHandler(2U, 1U);
    }
}

void FTM2_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[2];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[2][2])
    {
        TIMING_Ftm_IrqHandler(2U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[2][3])
    {
        TIMING_Ftm_IrqHandler(2U, 3U);
    }
}

void FTM2_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[2];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[2][4])
    {
        TIMING_Ftm_IrqHandler(2U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[2][5])
    {
        TIMING_Ftm_IrqHandler(2U, 5U);
    }
}

void FTM2_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[2];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[2][6])
    {
        TIMING_Ftm_IrqHandler(2U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[2][7])
    {
        TIMING_Ftm_IrqHandler(2U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 2U */

#if (FTM_INSTANCE_COUNT > 3U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM3_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[3];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[3][0])
    {
        TIMING_Ftm_IrqHandler(3U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[3][1])
    {
        TIMING_Ftm_IrqHandler(3U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[3][2])
    {
        TIMING_Ftm_IrqHandler(3U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[3][3])
    {
        TIMING_Ftm_IrqHandler(3U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[3][4])
    {
        TIMING_Ftm_IrqHandler(3U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[3][5])
    {
        TIMING_Ftm_IrqHandler(3U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[3][6])
    {
        TIMING_Ftm_IrqHandler(3U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[3][7])
    {
        TIMING_Ftm_IrqHandler(3U, 7U);
    }
}
#else
void FTM3_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[3];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[3][0])
    {
        TIMING_Ftm_IrqHandler(3U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[3][1])
    {
        TIMING_Ftm_IrqHandler(3U, 1U);
    }
}

void FTM3_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[3];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[3][2])
    {
        TIMING_Ftm_IrqHandler(3U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[3][3])
    {
        TIMING_Ftm_IrqHandler(3U, 3U);
    }
}

void FTM3_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[3];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[3][4])
    {
        TIMING_Ftm_IrqHandler(3U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[3][5])
    {
        TIMING_Ftm_IrqHandler(3U, 5U);
    }
}

void FTM3_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[3];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[3][6])
    {
        TIMING_Ftm_IrqHandler(3U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[3][7])
    {
        TIMING_Ftm_IrqHandler(3U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 3U */

#if (FTM_INSTANCE_COUNT > 4U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM4_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[4];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[4][0])
    {
        TIMING_Ftm_IrqHandler(4U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[4][1])
    {
        TIMING_Ftm_IrqHandler(4U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[4][2])
    {
        TIMING_Ftm_IrqHandler(4U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[4][3])
    {
        TIMING_Ftm_IrqHandler(4U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[4][4])
    {
        TIMING_Ftm_IrqHandler(4U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[4][5])
    {
        TIMING_Ftm_IrqHandler(4U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[4][6])
    {
        TIMING_Ftm_IrqHandler(4U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[4][7])
    {
        TIMING_Ftm_IrqHandler(4U, 7U);
    }
}
#else
void FTM4_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[4];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[4][0])
    {
        TIMING_Ftm_IrqHandler(4U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[4][1])
    {
        TIMING_Ftm_IrqHandler(4U, 1U);
    }
}

void FTM4_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[4];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[4][2])
    {
        TIMING_Ftm_IrqHandler(4U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[4][3])
    {
        TIMING_Ftm_IrqHandler(4U, 3U);
    }
}

void FTM4_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[4];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[4][4])
    {
        TIMING_Ftm_IrqHandler(4U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[4][5])
    {
        TIMING_Ftm_IrqHandler(4U, 5U);
    }
}

void FTM4_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[4];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[4][6])
    {
        TIMING_Ftm_IrqHandler(4U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[4][7])
    {
        TIMING_Ftm_IrqHandler(4U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 4U */

#if (FTM_INSTANCE_COUNT > 5U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM5_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[5];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[5][0])
    {
        TIMING_Ftm_IrqHandler(5U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[5][1])
    {
        TIMING_Ftm_IrqHandler(5U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[5][2])
    {
        TIMING_Ftm_IrqHandler(5U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[5][3])
    {
        TIMING_Ftm_IrqHandler(5U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[5][4])
    {
        TIMING_Ftm_IrqHandler(5U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[5][5])
    {
        TIMING_Ftm_IrqHandler(5U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[5][6])
    {
        TIMING_Ftm_IrqHandler(5U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[5][7])
    {
        TIMING_Ftm_IrqHandler(5U, 7U);
    }
}
#else
void FTM5_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[5];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[5][0])
    {
        TIMING_Ftm_IrqHandler(5U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[5][1])
    {
        TIMING_Ftm_IrqHandler(5U, 1U);
    }
}

void FTM5_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[5];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[5][2])
    {
        TIMING_Ftm_IrqHandler(5U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[5][3])
    {
        TIMING_Ftm_IrqHandler(5U, 3U);
    }
}

void FTM5_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[5];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[5][4])
    {
        TIMING_Ftm_IrqHandler(5U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[5][5])
    {
        TIMING_Ftm_IrqHandler(5U, 5U);
    }
}

void FTM5_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[5];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[5][6])
    {
        TIMING_Ftm_IrqHandler(5U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[5][7])
    {
        TIMING_Ftm_IrqHandler(5U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 5U */

#if (FTM_INSTANCE_COUNT > 6U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM6_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[6];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[6][0])
    {
        TIMING_Ftm_IrqHandler(6U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[6][1])
    {
        TIMING_Ftm_IrqHandler(6U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[6][2])
    {
        TIMING_Ftm_IrqHandler(6U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[6][3])
    {
        TIMING_Ftm_IrqHandler(6U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[6][4])
    {
        TIMING_Ftm_IrqHandler(6U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[6][5])
    {
        TIMING_Ftm_IrqHandler(6U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[6][6])
    {
        TIMING_Ftm_IrqHandler(6U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[6][7])
    {
        TIMING_Ftm_IrqHandler(6U, 7U);
    }
}
#else
void FTM6_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[6];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[6][0])
    {
        TIMING_Ftm_IrqHandler(6U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[6][1])
    {
        TIMING_Ftm_IrqHandler(6U, 1U);
    }
}

void FTM6_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[6];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[6][2])
    {
        TIMING_Ftm_IrqHandler(6U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[6][3])
    {
        TIMING_Ftm_IrqHandler(6U, 3U);
    }
}

void FTM6_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[6];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[6][4])
    {
        TIMING_Ftm_IrqHandler(6U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[6][5])
    {
        TIMING_Ftm_IrqHandler(6U, 5U);
    }
}

void FTM6_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[6];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[6][6])
    {
        TIMING_Ftm_IrqHandler(6U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[6][7])
    {
        TIMING_Ftm_IrqHandler(6U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 6U */

#if (FTM_INSTANCE_COUNT > 7U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM7_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[7];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan0IntFlag && g_ftmChannelRunning[7][0])
    {
        TIMING_Ftm_IrqHandler(7U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[7][1])
    {
        TIMING_Ftm_IrqHandler(7U, 1U);
    }

    if (chan2IntFlag && g_ftmChannelRunning[7][2])
    {
        TIMING_Ftm_IrqHandler(7U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[7][3])
    {
        TIMING_Ftm_IrqHandler(7U, 3U);
    }

    if (chan4IntFlag && g_ftmChannelRunning[7][4])
    {
        TIMING_Ftm_IrqHandler(7U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[7][5])
    {
        TIMING_Ftm_IrqHandler(7U, 5U);
    }

    if (chan6IntFlag && g_ftmChannelRunning[7][6])
    {
        TIMING_Ftm_IrqHandler(7U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[7][7])
    {
        TIMING_Ftm_IrqHandler(7U, 7U);
    }
}
#else
void FTM7_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[7];
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);

    if (chan0IntFlag && g_ftmChannelRunning[7][0])
    {
        TIMING_Ftm_IrqHandler(7U, 0U);
    }

    if (chan1IntFlag && g_ftmChannelRunning[7][1])
    {
        TIMING_Ftm_IrqHandler(7U, 1U);
    }
}

void FTM7_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[7];
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);

    if (chan2IntFlag && g_ftmChannelRunning[7][2])
    {
        TIMING_Ftm_IrqHandler(7U, 2U);
    }

    if (chan3IntFlag && g_ftmChannelRunning[7][3])
    {
        TIMING_Ftm_IrqHandler(7U, 3U);
    }
}

void FTM7_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[7];
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);

    if (chan4IntFlag && g_ftmChannelRunning[7][4])
    {
        TIMING_Ftm_IrqHandler(7U, 4U);
    }

    if (chan5IntFlag && g_ftmChannelRunning[7][5])
    {
        TIMING_Ftm_IrqHandler(7U, 5U);
    }
}

void FTM7_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = ftmBase[7];
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);

    if (chan6IntFlag && g_ftmChannelRunning[7][6])
    {
        TIMING_Ftm_IrqHandler(7U, 6U);
    }

    if (chan7IntFlag && g_ftmChannelRunning[7][7])
    {
        TIMING_Ftm_IrqHandler(7U, 7U);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 7U */

/* Array storing references to TIMING over FTM irq handlers */
const isr_t s_timingOverFtmIsr[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT] =
{
#if (FTM_INSTANCE_COUNT > 0U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler,
     FTM0_Ch0_7_IrqHandler},
#else
    {FTM0_Ch0_Ch1_IrqHandler,
     FTM0_Ch0_Ch1_IrqHandler,
     FTM0_Ch2_Ch3_IrqHandler,
     FTM0_Ch2_Ch3_IrqHandler,
     FTM0_Ch4_Ch5_IrqHandler,
     FTM0_Ch4_Ch5_IrqHandler,
     FTM0_Ch6_Ch7_IrqHandler,
     FTM0_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 0U */

#if (FTM_INSTANCE_COUNT > 1U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler,
     FTM1_Ch0_7_IrqHandler},
#else
    {FTM1_Ch0_Ch1_IrqHandler,
     FTM1_Ch0_Ch1_IrqHandler,
     FTM1_Ch2_Ch3_IrqHandler,
     FTM1_Ch2_Ch3_IrqHandler,
     FTM1_Ch4_Ch5_IrqHandler,
     FTM1_Ch4_Ch5_IrqHandler,
     FTM1_Ch6_Ch7_IrqHandler,
     FTM1_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 1U */

#if (FTM_INSTANCE_COUNT > 2U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler,
     FTM2_Ch0_7_IrqHandler},
#else
    {FTM2_Ch0_Ch1_IrqHandler,
     FTM2_Ch0_Ch1_IrqHandler,
     FTM2_Ch2_Ch3_IrqHandler,
     FTM2_Ch2_Ch3_IrqHandler,
     FTM2_Ch4_Ch5_IrqHandler,
     FTM2_Ch4_Ch5_IrqHandler,
     FTM2_Ch6_Ch7_IrqHandler,
     FTM2_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 2U */

#if (FTM_INSTANCE_COUNT > 3U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler,
     FTM3_Ch0_7_IrqHandler},
#else
    {FTM3_Ch0_Ch1_IrqHandler,
     FTM3_Ch0_Ch1_IrqHandler,
     FTM3_Ch2_Ch3_IrqHandler,
     FTM3_Ch2_Ch3_IrqHandler,
     FTM3_Ch4_Ch5_IrqHandler,
     FTM3_Ch4_Ch5_IrqHandler,
     FTM3_Ch6_Ch7_IrqHandler,
     FTM3_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 3U */

#if (FTM_INSTANCE_COUNT > 4U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler,
     FTM4_Ch0_7_IrqHandler},
#else
    {FTM4_Ch0_Ch1_IrqHandler,
     FTM4_Ch0_Ch1_IrqHandler,
     FTM4_Ch2_Ch3_IrqHandler,
     FTM4_Ch2_Ch3_IrqHandler,
     FTM4_Ch4_Ch5_IrqHandler,
     FTM4_Ch4_Ch5_IrqHandler,
     FTM4_Ch6_Ch7_IrqHandler,
     FTM4_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 4U */

#if (FTM_INSTANCE_COUNT > 5U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler,
     FTM5_Ch0_7_IrqHandler},
#else
    {FTM5_Ch0_Ch1_IrqHandler,
     FTM5_Ch0_Ch1_IrqHandler,
     FTM5_Ch2_Ch3_IrqHandler,
     FTM5_Ch2_Ch3_IrqHandler,
     FTM5_Ch4_Ch5_IrqHandler,
     FTM5_Ch4_Ch5_IrqHandler,
     FTM5_Ch6_Ch7_IrqHandler,
     FTM5_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 5U */

#if (FTM_INSTANCE_COUNT > 6U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler,
     FTM6_Ch0_7_IrqHandler},
#else
    {FTM6_Ch0_Ch1_IrqHandler,
     FTM6_Ch0_Ch1_IrqHandler,
     FTM6_Ch2_Ch3_IrqHandler,
     FTM6_Ch2_Ch3_IrqHandler,
     FTM6_Ch4_Ch5_IrqHandler,
     FTM6_Ch4_Ch5_IrqHandler,
     FTM6_Ch6_Ch7_IrqHandler,
     FTM6_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 6U */

#if (FTM_INSTANCE_COUNT > 7U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
    {FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler,
     FTM7_Ch0_7_IrqHandler},
#else
    {FTM7_Ch0_Ch1_IrqHandler,
     FTM7_Ch0_Ch1_IrqHandler,
     FTM7_Ch2_Ch3_IrqHandler,
     FTM7_Ch2_Ch3_IrqHandler,
     FTM7_Ch4_Ch5_IrqHandler,
     FTM7_Ch4_Ch5_IrqHandler,
     FTM7_Ch6_Ch7_IrqHandler,
     FTM7_Ch6_Ch7_IrqHandler},
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* FTM_INSTANCE_COUNT > 7U */
};

#endif /* TIMING_OVER_FTM */

/* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))

#ifdef STM_0
void STM0_Ch0_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(0U, 0U);
}

void STM0_Ch1_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(0U, 1U);
}

void STM0_Ch2_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(0U, 2U);
}

void STM0_Ch3_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(0U, 3U);
}
#endif

#ifdef STM_1
void STM1_Ch0_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(1U, 0U);
}

void STM1_Ch1_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(1U, 1U);
}

void STM1_Ch2_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(1U, 2U);
}

void STM1_Ch3_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(1U, 3U);
}
#endif

#ifdef STM_2
void STM2_Ch0_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(2U, 0U);
}

void STM2_Ch1_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(2U, 1U);
}

void STM2_Ch2_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(2U, 2U);
}

void STM2_Ch3_IRQHandler(void)
{
    TIMING_Stm_IrqHandler(2U, 3U);
}
#endif

#endif /* TIMING_OVER_STM */

/*******************************************************************************
 * EOF
 ******************************************************************************/
