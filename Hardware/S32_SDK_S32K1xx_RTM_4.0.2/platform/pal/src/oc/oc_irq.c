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
 * @file oc_irq.c
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

#include "oc_irq.h"


/*******************************************************************************
 * Code
 ******************************************************************************/

/* Define interrupt handler for the output compare instance */

/* Define OC PAL over FTM */
#if (defined(OC_PAL_OVER_FTM))

#if (OC_PAL_INSTANCE_MAX_NUM > 0U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM0_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[0];
    bool chanIntFlag;
    uint8_t index;

    for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM;index++)
    {
        chanIntFlag = FTM_DRV_GetChnEventStatus(base, index);
        if (chanIntFlag)
        {
            OC_IrqHandler(0U, index, OC_INST_TYPE_FTM);
        }
    }
}
#else
void FTM0_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[0];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(0U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(0U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(0U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM0_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[0];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(0U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(0U, 2U, OC_INST_TYPE_FTM);
    }
    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(0U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM0_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[0];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(0U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(0U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(0U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM0_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[0];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(0U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(0U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(0U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* OC_PAL_INSTANCE_MAX_NUM > 0U */

#if (OC_PAL_INSTANCE_MAX_NUM > 1U)
#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM1_Ch0_7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[1];
    bool chanIntFlag;
    uint8_t index;

    for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM;index++)
    {
        chanIntFlag = FTM_DRV_GetChnEventStatus(base, index);
        if (chanIntFlag)
        {
            OC_IrqHandler(1U, index, OC_INST_TYPE_FTM);
        }
    }
}
#else
void FTM1_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[1];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(1U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(1U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(1U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM1_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[1];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(1U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(1U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(1U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM1_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[1];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(1U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(1U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(1U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM1_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[1];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(1U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(1U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(1U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U */
#endif /* OC_PAL_INSTANCE_MAX_NUM > 1U */

#if (OC_PAL_INSTANCE_MAX_NUM > 2U)
void FTM2_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[2];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(2U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(2U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(2U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM2_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[2];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(2U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(2U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(2U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM2_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[2];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(2U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(2U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(2U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM2_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[2];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(2U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(2U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(2U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 2U */

#if (OC_PAL_INSTANCE_MAX_NUM > 3U)
void FTM3_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[3];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(3U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(3U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(3U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM3_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[3];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(3U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(3U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(3U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM3_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[3];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(3U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(3U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(3U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM3_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[3];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(3U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(3U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(3U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 3U */

#if (OC_PAL_INSTANCE_MAX_NUM > 4U)
void FTM4_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[4];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(4U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(4U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(4U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM4_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[4];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(4U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(4U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(4U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM4_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[4];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(4U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(4U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(4U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM4_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[4];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(4U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(4U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(4U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 4U */

#if (OC_PAL_INSTANCE_MAX_NUM > 5U)
void FTM5_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[5];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(5U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(5U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(5U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM5_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[5];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(5U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(5U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(5U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM5_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[5];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(5U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(5U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(5U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM5_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[5];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(5U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(5U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(5U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 5U */

#if (OC_PAL_INSTANCE_MAX_NUM > 6U)
void FTM6_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[6];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(6U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(6U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(6U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM6_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[6];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(6U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(6U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(6U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM6_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[6];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(6U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(6U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(6U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM6_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[6];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(6U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(6U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(6U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 6U */

#if (OC_PAL_INSTANCE_MAX_NUM > 7U)
void FTM7_Ch0_Ch1_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[7];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(7U);
    bool chan0IntFlag = FTM_DRV_GetChnEventStatus(base, 0U);
    bool chan1IntFlag = FTM_DRV_GetChnEventStatus(base, 1U);
    bool chan0EnabledInt = ((enabledInterrupts & (1UL << 0U)) != 0U) ? true : false;
    bool chan1EnabledInt = ((enabledInterrupts & (1UL << 1U)) != 0U) ? true : false;

    if (chan0EnabledInt && chan0IntFlag)
    {
        OC_IrqHandler(7U, 0U, OC_INST_TYPE_FTM);
    }

    if (chan1EnabledInt && chan1IntFlag)
    {
        OC_IrqHandler(7U, 1U, OC_INST_TYPE_FTM);
    }
}

void FTM7_Ch2_Ch3_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[7];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(7U);
    bool chan2IntFlag = FTM_DRV_GetChnEventStatus(base, 2U);
    bool chan3IntFlag = FTM_DRV_GetChnEventStatus(base, 3U);
    bool chan2EnabledInt = ((enabledInterrupts & (1UL << 2U)) != 0U) ? true : false;
    bool chan3EnabledInt = ((enabledInterrupts & (1UL << 3U)) != 0U) ? true : false;

    if (chan2EnabledInt && chan2IntFlag)
    {
        OC_IrqHandler(7U, 2U, OC_INST_TYPE_FTM);
    }

    if (chan3EnabledInt && chan3IntFlag)
    {
        OC_IrqHandler(7U, 3U, OC_INST_TYPE_FTM);
    }
}

void FTM7_Ch4_Ch5_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[7];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(7U);
    bool chan4IntFlag = FTM_DRV_GetChnEventStatus(base, 4U);
    bool chan5IntFlag = FTM_DRV_GetChnEventStatus(base, 5U);
    bool chan4EnabledInt = ((enabledInterrupts & (1UL << 4U)) != 0U) ? true : false;
    bool chan5EnabledInt = ((enabledInterrupts & (1UL << 5U)) != 0U) ? true : false;

    if (chan4EnabledInt && chan4IntFlag)
    {
        OC_IrqHandler(7U, 4U, OC_INST_TYPE_FTM);
    }

    if (chan5EnabledInt && chan5IntFlag)
    {
        OC_IrqHandler(7U, 5U, OC_INST_TYPE_FTM);
    }
}

void FTM7_Ch6_Ch7_IrqHandler(void)
{
    const FTM_Type * const base = g_ftmBase[7];
    uint32_t enabledInterrupts = FTM_DRV_GetEnabledInterrupts(7U);
    bool chan6IntFlag = FTM_DRV_GetChnEventStatus(base, 6U);
    bool chan7IntFlag = FTM_DRV_GetChnEventStatus(base, 7U);
    bool chan6EnabledInt = ((enabledInterrupts & (1UL << 6U)) != 0U) ? true : false;
    bool chan7EnabledInt = ((enabledInterrupts & (1UL << 7U)) != 0U) ? true : false;

    if (chan6EnabledInt && chan6IntFlag)
    {
        OC_IrqHandler(7U, 6U, OC_INST_TYPE_FTM);
    }

    if (chan7EnabledInt && chan7IntFlag)
    {
        OC_IrqHandler(7U, 7U, OC_INST_TYPE_FTM);
    }
}
#endif /* OC_PAL_INSTANCE_MAX_NUM > 7U */

/* Array storing references to OC PAL over FTM irq handlers */
const isr_t s_ocOverFtmIsr[OC_PAL_INSTANCE_MAX_NUM][FTM_CONTROLS_COUNT] =
{
#if (OC_PAL_INSTANCE_MAX_NUM > 0U)
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
#endif /* OC_PAL_INSTANCE_MAX_NUM > 0U */
#if (OC_PAL_INSTANCE_MAX_NUM > 1U)
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
#endif /* OC_PAL_INSTANCE_MAX_NUM > 1U */
#if (OC_PAL_INSTANCE_MAX_NUM > 2U)
    {FTM2_Ch0_Ch1_IrqHandler,
     FTM2_Ch0_Ch1_IrqHandler,
     FTM2_Ch2_Ch3_IrqHandler,
     FTM2_Ch2_Ch3_IrqHandler,
     FTM2_Ch4_Ch5_IrqHandler,
     FTM2_Ch4_Ch5_IrqHandler,
     FTM2_Ch6_Ch7_IrqHandler,
     FTM2_Ch6_Ch7_IrqHandler},
#endif
#if (OC_PAL_INSTANCE_MAX_NUM > 3U)
    {FTM3_Ch0_Ch1_IrqHandler,
     FTM3_Ch0_Ch1_IrqHandler,
     FTM3_Ch2_Ch3_IrqHandler,
     FTM3_Ch2_Ch3_IrqHandler,
     FTM3_Ch4_Ch5_IrqHandler,
     FTM3_Ch4_Ch5_IrqHandler,
     FTM3_Ch6_Ch7_IrqHandler,
     FTM3_Ch6_Ch7_IrqHandler},
#endif
#if (OC_PAL_INSTANCE_MAX_NUM > 4U)
    {FTM4_Ch0_Ch1_IrqHandler,
     FTM4_Ch0_Ch1_IrqHandler,
     FTM4_Ch2_Ch3_IrqHandler,
     FTM4_Ch2_Ch3_IrqHandler,
     FTM4_Ch4_Ch5_IrqHandler,
     FTM4_Ch4_Ch5_IrqHandler,
     FTM4_Ch6_Ch7_IrqHandler,
     FTM4_Ch6_Ch7_IrqHandler},
#endif
#if (OC_PAL_INSTANCE_MAX_NUM > 5U)
    {FTM5_Ch0_Ch1_IrqHandler,
     FTM5_Ch0_Ch1_IrqHandler,
     FTM5_Ch2_Ch3_IrqHandler,
     FTM5_Ch2_Ch3_IrqHandler,
     FTM5_Ch4_Ch5_IrqHandler,
     FTM5_Ch4_Ch5_IrqHandler,
     FTM5_Ch6_Ch7_IrqHandler,
     FTM5_Ch6_Ch7_IrqHandler},
#endif
#if (OC_PAL_INSTANCE_MAX_NUM > 6U)
    {FTM6_Ch0_Ch1_IrqHandler,
     FTM6_Ch0_Ch1_IrqHandler,
     FTM6_Ch2_Ch3_IrqHandler,
     FTM6_Ch2_Ch3_IrqHandler,
     FTM6_Ch4_Ch5_IrqHandler,
     FTM6_Ch4_Ch5_IrqHandler,
     FTM6_Ch6_Ch7_IrqHandler,
     FTM6_Ch6_Ch7_IrqHandler},
#endif
#if (OC_PAL_INSTANCE_MAX_NUM > 7U)
    {FTM7_Ch0_Ch1_IrqHandler,
     FTM7_Ch0_Ch1_IrqHandler,
     FTM7_Ch2_Ch3_IrqHandler,
     FTM7_Ch2_Ch3_IrqHandler,
     FTM7_Ch4_Ch5_IrqHandler,
     FTM7_Ch4_Ch5_IrqHandler,
     FTM7_Ch6_Ch7_IrqHandler,
     FTM7_Ch6_Ch7_IrqHandler}
#endif
};

#endif /* OC_PAL_OVER_FTM */

#if (defined(OC_PAL_OVER_EMIOS))

#if (OC_PAL_INSTANCE_MAX_NUM > 0U)
/* The interrupt function for channel 0, 1 on EMIOS0 */
void EMIOS0_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 0U) == true)
    {
        OC_IrqHandler(0U, 0U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 1U) == true)
    {
        OC_IrqHandler(0U, 1U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS0 */
void EMIOS0_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 2U) == true)
    {
        OC_IrqHandler(0U, 2U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 3U) == true)
    {
        OC_IrqHandler(0U, 3U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS0 */
void EMIOS0_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 4U) == true)
    {
        OC_IrqHandler(0U, 4U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 5U) == true)
    {
        OC_IrqHandler(0U, 5U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS0 */
void EMIOS0_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 6U) == true)
    {
        OC_IrqHandler(0U, 6U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 7U) == true)
    {
        OC_IrqHandler(0U, 7U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS0 */
void EMIOS0_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 8U) == true)
    {
        OC_IrqHandler(0U, 8U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 9U) == true)
    {
        OC_IrqHandler(0U, 9U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS0 */
void EMIOS0_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 10U) == true)
    {
        OC_IrqHandler(0U, 10U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 11U) == true)
    {
        OC_IrqHandler(0U, 11U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS0 */
void EMIOS0_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 12U) == true)
    {
        OC_IrqHandler(0U, 12U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 13U) == true)
    {
        OC_IrqHandler(0U, 13U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS0 */
void EMIOS0_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 14U) == true)
    {
        OC_IrqHandler(0U, 14U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 15U) == true)
    {
        OC_IrqHandler(0U, 15U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS0 */
void EMIOS0_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 16U) == true)
    {
        OC_IrqHandler(0U, 16U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 17U) == true)
    {
        OC_IrqHandler(0U, 17U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS0 */
void EMIOS0_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 18U) == true)
    {
        OC_IrqHandler(0U, 18U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 19U) == true)
    {
        OC_IrqHandler(0U, 19U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS0 */
void EMIOS0_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 20U) == true)
    {
        OC_IrqHandler(0U, 20U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 21U) == true)
    {
        OC_IrqHandler(0U, 21U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS0 */
void EMIOS0_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 22U) == true)
    {
        OC_IrqHandler(0U, 22U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 23U) == true)
    {
        OC_IrqHandler(0U, 23U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS0 */
void EMIOS0_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 24U) == true)
    {
        OC_IrqHandler(0U, 24U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 25U) == true)
    {
        OC_IrqHandler(0U, 25U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS0 */
void EMIOS0_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 26U) == true)
    {
        OC_IrqHandler(0U, 26U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 27U) == true)
    {
        OC_IrqHandler(0U, 27U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS0 */
void EMIOS0_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 28U) == true)
    {
        OC_IrqHandler(0U, 28U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 29U) == true)
    {
        OC_IrqHandler(0U, 29U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS0 */
void EMIOS0_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 30U) == true)
    {
        OC_IrqHandler(0U, 30U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 31U) == true)
    {
        OC_IrqHandler(0U, 31U, OC_INST_TYPE_EMIOS);
    }
}
#endif /* End of (OC_PAL_INSTANCE_MAX_NUM > 0U) */

#if (OC_PAL_INSTANCE_MAX_NUM > 1U)
/* The interrupt function for channel 0, 1 on EMIOS1 */
void EMIOS1_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 0U) == true)
    {
        OC_IrqHandler(1U, 0U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 1U) == true)
    {
        OC_IrqHandler(1U, 1U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS1 */
void EMIOS1_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 2U) == true)
    {
        OC_IrqHandler(1U, 2U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 3U) == true)
    {
        OC_IrqHandler(1U, 3U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS1 */
void EMIOS1_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 4U) == true)
    {
        OC_IrqHandler(1U, 4U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 5U) == true)
    {
        OC_IrqHandler(1U, 5U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS1 */
void EMIOS1_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 6U) == true)
    {
        OC_IrqHandler(1U, 6U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 7U) == true)
    {
        OC_IrqHandler(1U, 7U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS1 */
void EMIOS1_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 8U) == true)
    {
        OC_IrqHandler(1U, 8U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 9U) == true)
    {
        OC_IrqHandler(1U, 9U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS1 */
void EMIOS1_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 10U) == true)
    {
        OC_IrqHandler(1U, 10U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 11U) == true)
    {
        OC_IrqHandler(1U, 11U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS1 */
void EMIOS1_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 12U) == true)
    {
        OC_IrqHandler(1U, 12U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 13U) == true)
    {
        OC_IrqHandler(1U, 13U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS1 */
void EMIOS1_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 14U) == true)
    {
        OC_IrqHandler(1U, 14U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 15U) == true)
    {
        OC_IrqHandler(1U, 15U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS1 */
void EMIOS1_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 16U) == true)
    {
        OC_IrqHandler(1U, 16U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 17U) == true)
    {
        OC_IrqHandler(1U, 17U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS1 */
void EMIOS1_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 18U) == true)
    {
        OC_IrqHandler(1U, 18U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 19U) == true)
    {
        OC_IrqHandler(1U, 19U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS1 */
void EMIOS1_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 20U) == true)
    {
        OC_IrqHandler(1U, 20U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 21U) == true)
    {
        OC_IrqHandler(1U, 21U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS1 */
void EMIOS1_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 22U) == true)
    {
        OC_IrqHandler(1U, 22U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 23U) == true)
    {
        OC_IrqHandler(1U, 23U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS1 */
void EMIOS1_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 24U) == true)
    {
        OC_IrqHandler(1U, 24U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 25U) == true)
    {
        OC_IrqHandler(1U, 25U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS1 */
void EMIOS1_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 26U) == true)
    {
        OC_IrqHandler(1U, 26U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 27U) == true)
    {
        OC_IrqHandler(1U, 27U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS1 */
void EMIOS1_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 28U) == true)
    {
        OC_IrqHandler(1U, 28U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 29U) == true)
    {
        OC_IrqHandler(1U, 29U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS1 */
void EMIOS1_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 30U) == true)
    {
        OC_IrqHandler(1U, 30U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 31U) == true)
    {
        OC_IrqHandler(1U, 31U, OC_INST_TYPE_EMIOS);
    }
}
#endif  /* End of (OC_PAL_INSTANCE_MAX_NUM > 1U) */

#if (OC_PAL_INSTANCE_MAX_NUM > 2U)
/* The interrupt function for channel 0, 1 on EMIOS2 */
void EMIOS2_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 0U) == true)
    {
        OC_IrqHandler(2U, 0U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 1U) == true)
    {
        OC_IrqHandler(2U, 1U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS2 */
void EMIOS2_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 2U) == true)
    {
        OC_IrqHandler(2U, 2U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 3U) == true)
    {
        OC_IrqHandler(2U, 3U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS2 */
void EMIOS2_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 4U) == true)
    {
        OC_IrqHandler(2U, 4U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 5U) == true)
    {
        OC_IrqHandler(2U, 5U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS2 */
void EMIOS2_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 6U) == true)
    {
        OC_IrqHandler(2U, 6U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 7U) == true)
    {
        OC_IrqHandler(2U, 7U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS2 */
void EMIOS2_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 8U) == true)
    {
        OC_IrqHandler(2U, 8U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 9U) == true)
    {
        OC_IrqHandler(2U, 9U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS2 */
void EMIOS2_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 10U) == true)
    {
        OC_IrqHandler(2U, 10U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 11U) == true)
    {
        OC_IrqHandler(2U, 11U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS2 */
void EMIOS2_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 12U) == true)
    {
        OC_IrqHandler(2U, 12U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 13U) == true)
    {
        OC_IrqHandler(2U, 13U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS2 */
void EMIOS2_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 14U) == true)
    {
        OC_IrqHandler(2U, 14U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 15U) == true)
    {
        OC_IrqHandler(2U, 15U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS2 */
void EMIOS2_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 16U) == true)
    {
        OC_IrqHandler(2U, 16U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 17U) == true)
    {
        OC_IrqHandler(2U, 17U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS2 */
void EMIOS2_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 18U) == true)
    {
        OC_IrqHandler(2U, 18U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 19U) == true)
    {
        OC_IrqHandler(2U, 19U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS2 */
void EMIOS2_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 20U) == true)
    {
        OC_IrqHandler(2U, 20U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 21U) == true)
    {
        OC_IrqHandler(2U, 21U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS2 */
void EMIOS2_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 22U) == true)
    {
        OC_IrqHandler(2U, 22U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 23U) == true)
    {
        OC_IrqHandler(2U, 23U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS2 */
void EMIOS2_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 24U) == true)
    {
        OC_IrqHandler(2U, 24U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 25U) == true)
    {
        OC_IrqHandler(2U, 25U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS2 */
void EMIOS2_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 26U) == true)
    {
        OC_IrqHandler(2U, 26U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 27U) == true)
    {
        OC_IrqHandler(2U, 27U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS2 */
void EMIOS2_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 28U) == true)
    {
        OC_IrqHandler(2U, 28U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 29U) == true)
    {
        OC_IrqHandler(2U, 29U, OC_INST_TYPE_EMIOS);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS2 */
void EMIOS2_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 30U) == true)
    {
        OC_IrqHandler(2U, 30U, OC_INST_TYPE_EMIOS);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 31U) == true)
    {
        OC_IrqHandler(2U, 31U, OC_INST_TYPE_EMIOS);
    }
}
#endif  /* End of (OC_PAL_INSTANCE_MAX_NUM > 2U) */

#endif /* End of defined(OC_PAL_OVER_EMIOS) */


#if (defined(OC_PAL_OVER_ETIMER))
#if (OC_ETIMER_INSTANCE_MAX_NUM > 0U)
void ETIMER0_TC0IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 0U, OC_INST_TYPE_ETIMER);
}

void ETIMER0_TC1IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 1U, OC_INST_TYPE_ETIMER);
}

void ETIMER0_TC2IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 2U, OC_INST_TYPE_ETIMER);
}

void ETIMER0_TC3IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 3U, OC_INST_TYPE_ETIMER);
}

void ETIMER0_TC4IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 4U, OC_INST_TYPE_ETIMER);
}

void ETIMER0_TC5IR_IRQHandler(void)
{
    OC_IrqHandler(0U, 5U, OC_INST_TYPE_ETIMER);
}
#endif /* End of (OC_ETIMER_INSTANCE_MAX_NUM > 0U) */

#if (OC_ETIMER_INSTANCE_MAX_NUM > 1U)
void ETIMER1_TC0IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 0U, OC_INST_TYPE_ETIMER);
}

void ETIMER1_TC1IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 1U, OC_INST_TYPE_ETIMER);
}

void ETIMER1_TC2IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 2U, OC_INST_TYPE_ETIMER);
}

void ETIMER1_TC3IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 3U, OC_INST_TYPE_ETIMER);
}

void ETIMER1_TC4IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 4U, OC_INST_TYPE_ETIMER);
}

void ETIMER1_TC5IR_IRQHandler(void)
{
    OC_IrqHandler(1U, 5U, OC_INST_TYPE_ETIMER);
}
#endif /* End of (OC_ETIMER_INSTANCE_MAX_NUM > 1U) */

#if (OC_ETIMER_INSTANCE_MAX_NUM > 2U)
void ETIMER2_TC0IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 0U, OC_INST_TYPE_ETIMER);
}

void ETIMER2_TC1IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 1U, OC_INST_TYPE_ETIMER);
}

void ETIMER2_TC2IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 2U, OC_INST_TYPE_ETIMER);
}

void ETIMER2_TC3IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 3U, OC_INST_TYPE_ETIMER);
}

void ETIMER2_TC4IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 4U, OC_INST_TYPE_ETIMER);
}

void ETIMER2_TC5IR_IRQHandler(void)
{
    OC_IrqHandler(2U, 5U, OC_INST_TYPE_ETIMER);
}
#endif /* End of (OC_ETIMER_INSTANCE_MAX_NUM > 2U) */
#endif /* End of defined(OC_PAL_OVER_ETIMER) */

#if (defined(OC_PAL_OVER_FLEXPWM))

#if (OC_FLEXPWM_INSTANCE_MAX_NUM > 0U)
void FlexPWM_0_COF0_IRQHandler(void)
{
    OC_IrqHandler(0U, 0U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_0_COF1_IRQHandler(void)
{
    OC_IrqHandler(0U, 1U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_0_COF2_IRQHandler(void)
{
    OC_IrqHandler(0U, 2U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_0_COF3_IRQHandler(void)
{
    OC_IrqHandler(0U, 3U, OC_INST_TYPE_FLEXPWM);
}
#endif /* End of (OC_FLEXPWM_INSTANCE_MAX_NUM > 0U)*/

#if (OC_FLEXPWM_INSTANCE_MAX_NUM > 1U)
void FlexPWM_1_COF0_IRQHandler(void)
{
    OC_IrqHandler(1U, 0U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_1_COF1_IRQHandler(void)
{
    OC_IrqHandler(1U, 1U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_1_COF2_IRQHandler(void)
{
    OC_IrqHandler(1U, 2U, OC_INST_TYPE_FLEXPWM);
}

void FlexPWM_1_COF3_IRQHandler(void)
{
    OC_IrqHandler(1U, 3U, OC_INST_TYPE_FLEXPWM);
}
#endif /* End of (OC_FLEXPWM_INSTANCE_MAX_NUM > 1U)*/

#endif /* defined(OC_PAL_OVER_FLEXPWM) */
/*******************************************************************************
 * EOF
 ******************************************************************************/
