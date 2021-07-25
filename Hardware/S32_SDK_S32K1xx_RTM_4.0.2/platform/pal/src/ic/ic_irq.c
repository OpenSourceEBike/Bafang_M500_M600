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

/*!
 * @file ic_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Function not defined with external linkage.
 * The functions are not defined static because they are referenced in .s startup files.
 */

#include "ic_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(IC_PAL_OVER_EMIOS))

#if (IC_PAL_INSTANCE_MAX > 0U)
/* The interrupt function for channel 0, 1 on EMIOS0 */
void EMIOS0_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 0U) == true)
    {
        IC_Emios_IrqHandler(0U, 0U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 1U) == true)
    {
        IC_Emios_IrqHandler(0U, 1U);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS0 */
void EMIOS0_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 2U) == true)
    {
        IC_Emios_IrqHandler(0U, 2U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 3U) == true)
    {
        IC_Emios_IrqHandler(0U, 3U);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS0 */
void EMIOS0_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 4U) == true)
    {
        IC_Emios_IrqHandler(0U, 4U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 5U) == true)
    {
        IC_Emios_IrqHandler(0U, 5U);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS0 */
void EMIOS0_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 6U) == true)
    {
        IC_Emios_IrqHandler(0U, 6U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 7U) == true)
    {
        IC_Emios_IrqHandler(0U, 7U);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS0 */
void EMIOS0_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 8U) == true)
    {
        IC_Emios_IrqHandler(0U, 8U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 9U) == true)
    {
        IC_Emios_IrqHandler(0U, 9U);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS0 */
void EMIOS0_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 10U) == true)
    {
        IC_Emios_IrqHandler(0U, 10U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 11U) == true)
    {
        IC_Emios_IrqHandler(0U, 11U);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS0 */
void EMIOS0_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 12U) == true)
    {
        IC_Emios_IrqHandler(0U, 12U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 13U) == true)
    {
        IC_Emios_IrqHandler(0U, 13U);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS0 */
void EMIOS0_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 14U) == true)
    {
        IC_Emios_IrqHandler(0U, 14U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 15U) == true)
    {
        IC_Emios_IrqHandler(0U, 15U);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS0 */
void EMIOS0_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 16U) == true)
    {
        IC_Emios_IrqHandler(0U, 16U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 17U) == true)
    {
        IC_Emios_IrqHandler(0U, 17U);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS0 */
void EMIOS0_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 18U) == true)
    {
        IC_Emios_IrqHandler(0U, 18U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 19U) == true)
    {
        IC_Emios_IrqHandler(0U, 19U);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS0 */
void EMIOS0_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 20U) == true)
    {
        IC_Emios_IrqHandler(0U, 20U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 21U) == true)
    {
        IC_Emios_IrqHandler(0U, 21U);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS0 */
void EMIOS0_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 22U) == true)
    {
        IC_Emios_IrqHandler(0U, 22U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 23U) == true)
    {
        IC_Emios_IrqHandler(0U, 23U);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS0 */
void EMIOS0_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 24U) == true)
    {
        IC_Emios_IrqHandler(0U, 24U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 25U) == true)
    {
        IC_Emios_IrqHandler(0U, 25U);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS0 */
void EMIOS0_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 26U) == true)
    {
        IC_Emios_IrqHandler(0U, 26U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 27U) == true)
    {
        IC_Emios_IrqHandler(0U, 27U);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS0 */
void EMIOS0_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 28U) == true)
    {
        IC_Emios_IrqHandler(0U, 28U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 29U) == true)
    {
        IC_Emios_IrqHandler(0U, 29U);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS0 */
void EMIOS0_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 30U) == true)
    {
        IC_Emios_IrqHandler(0U, 30U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(0U, 31U) == true)
    {
        IC_Emios_IrqHandler(0U, 31U);
    }
}
#endif /* End of (IC_PAL_INSTANCE_MAX > 0U) */

#if (IC_PAL_INSTANCE_MAX > 1U)
/* The interrupt function for channel 0, 1 on EMIOS1 */
void EMIOS1_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 0U) == true)
    {
        IC_Emios_IrqHandler(1U, 0U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 1U) == true)
    {
        IC_Emios_IrqHandler(1U, 1U);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS1 */
void EMIOS1_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 2U) == true)
    {
        IC_Emios_IrqHandler(1U, 2U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 3U) == true)
    {
        IC_Emios_IrqHandler(1U, 3U);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS1 */
void EMIOS1_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 4U) == true)
    {
        IC_Emios_IrqHandler(1U, 4U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 5U) == true)
    {
        IC_Emios_IrqHandler(1U, 5U);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS1 */
void EMIOS1_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 6U) == true)
    {
        IC_Emios_IrqHandler(1U, 6U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 7U) == true)
    {
        IC_Emios_IrqHandler(1U, 7U);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS1 */
void EMIOS1_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 8U) == true)
    {
        IC_Emios_IrqHandler(1U, 8U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 9U) == true)
    {
        IC_Emios_IrqHandler(1U, 9U);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS1 */
void EMIOS1_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 10U) == true)
    {
        IC_Emios_IrqHandler(1U, 10U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 11U) == true)
    {
        IC_Emios_IrqHandler(1U, 11U);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS1 */
void EMIOS1_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 12U) == true)
    {
        IC_Emios_IrqHandler(1U, 12U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 13U) == true)
    {
        IC_Emios_IrqHandler(1U, 13U);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS1 */
void EMIOS1_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 14U) == true)
    {
        IC_Emios_IrqHandler(1U, 14U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 15U) == true)
    {
        IC_Emios_IrqHandler(1U, 15U);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS1 */
void EMIOS1_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 16U) == true)
    {
        IC_Emios_IrqHandler(1U, 16U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 17U) == true)
    {
        IC_Emios_IrqHandler(1U, 17U);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS1 */
void EMIOS1_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 18U) == true)
    {
        IC_Emios_IrqHandler(1U, 18U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 19U) == true)
    {
        IC_Emios_IrqHandler(1U, 19U);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS1 */
void EMIOS1_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 20U) == true)
    {
        IC_Emios_IrqHandler(1U, 20U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 21U) == true)
    {
        IC_Emios_IrqHandler(1U, 21U);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS1 */
void EMIOS1_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 22U) == true)
    {
        IC_Emios_IrqHandler(1U, 22U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 23U) == true)
    {
        IC_Emios_IrqHandler(1U, 23U);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS1 */
void EMIOS1_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 24U) == true)
    {
        IC_Emios_IrqHandler(1U, 24U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 25U) == true)
    {
        IC_Emios_IrqHandler(1U, 25U);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS1 */
void EMIOS1_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 26U) == true)
    {
        IC_Emios_IrqHandler(1U, 26U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 27U) == true)
    {
        IC_Emios_IrqHandler(1U, 27U);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS1 */
void EMIOS1_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 28U) == true)
    {
        IC_Emios_IrqHandler(1U, 28U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 29U) == true)
    {
        IC_Emios_IrqHandler(1U, 29U);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS1 */
void EMIOS1_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 30U) == true)
    {
        IC_Emios_IrqHandler(1U, 30U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(1U, 31U) == true)
    {
        IC_Emios_IrqHandler(1U, 31U);
    }
}
#endif  /* End of (IC_PAL_INSTANCE_MAX > 1U) */

#if (IC_PAL_INSTANCE_MAX > 2U)
/* The interrupt function for channel 0, 1 on EMIOS2 */
void EMIOS2_00_01_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 0U) == true)
    {
        IC_Emios_IrqHandler(2U, 0U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 1U) == true)
    {
        IC_Emios_IrqHandler(2U, 1U);
    }
}

/* The interrupt function for channel 2, 3 on EMIOS2 */
void EMIOS2_02_03_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 2U) == true)
    {
        IC_Emios_IrqHandler(2U, 2U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 3U) == true)
    {
        IC_Emios_IrqHandler(2U, 3U);
    }
}

/* The interrupt function for channel 4, 5 on EMIOS2 */
void EMIOS2_04_05_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 4U) == true)
    {
        IC_Emios_IrqHandler(2U, 4U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 5U) == true)
    {
        IC_Emios_IrqHandler(2U, 5U);
    }
}

/* The interrupt function for channel 6, 7 on EMIOS2 */
void EMIOS2_06_07_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 6U) == true)
    {
        IC_Emios_IrqHandler(2U, 6U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 7U) == true)
    {
        IC_Emios_IrqHandler(2U, 7U);
    }
}

/* The interrupt function for channel 8, 9 on EMIOS2 */
void EMIOS2_08_09_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 8U) == true)
    {
        IC_Emios_IrqHandler(2U, 8U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 9U) == true)
    {
        IC_Emios_IrqHandler(2U, 9U);
    }
}

/* The interrupt function for channel 10, 11 on EMIOS2 */
void EMIOS2_10_11_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 10U) == true)
    {
        IC_Emios_IrqHandler(2U, 10U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 11U) == true)
    {
        IC_Emios_IrqHandler(2U, 11U);
    }
}

/* The interrupt function for channel 12, 13 on EMIOS2 */
void EMIOS2_12_13_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 12U) == true)
    {
        IC_Emios_IrqHandler(2U, 12U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 13U) == true)
    {
        IC_Emios_IrqHandler(2U, 13U);
    }
}

/* The interrupt function for channel 14, 15 on EMIOS2 */
void EMIOS2_14_15_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 14U) == true)
    {
        IC_Emios_IrqHandler(2U, 14U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 15U) == true)
    {
        IC_Emios_IrqHandler(2U, 15U);
    }
}

/* The interrupt function for channel 16, 17 on EMIOS2 */
void EMIOS2_16_17_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 16U) == true)
    {
        IC_Emios_IrqHandler(2U, 16U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 17U) == true)
    {
        IC_Emios_IrqHandler(2U, 17U);
    }
}

/* The interrupt function for channel 18, 19 on EMIOS2 */
void EMIOS2_18_19_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 18U) == true)
    {
        IC_Emios_IrqHandler(2U, 18U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 19U) == true)
    {
        IC_Emios_IrqHandler(2U, 19U);
    }
}

/* The interrupt function for channel 20, 21 on EMIOS2 */
void EMIOS2_20_21_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 20U) == true)
    {
        IC_Emios_IrqHandler(2U, 20U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 21U) == true)
    {
        IC_Emios_IrqHandler(2U, 21U);
    }
}

/* The interrupt function for channel 22, 23 on EMIOS2 */
void EMIOS2_22_23_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 22U) == true)
    {
        IC_Emios_IrqHandler(2U, 22U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 23U) == true)
    {
        IC_Emios_IrqHandler(2U, 23U);
    }
}

/* The interrupt function for channel 24, 25 on EMIOS2 */
void EMIOS2_24_25_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 24U) == true)
    {
        IC_Emios_IrqHandler(2U, 24U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 25U) == true)
    {
        IC_Emios_IrqHandler(2U, 25U);
    }
}

/* The interrupt function for channel 26, 27 on EMIOS2 */
void EMIOS2_26_27_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 26U) == true)
    {
        IC_Emios_IrqHandler(2U, 26U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 27U) == true)
    {
        IC_Emios_IrqHandler(2U, 27U);
    }
}

/* The interrupt function for channel 28, 29 on EMIOS2 */
void EMIOS2_28_29_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 28U) == true)
    {
        IC_Emios_IrqHandler(2U, 28U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 29U) == true)
    {
        IC_Emios_IrqHandler(2U, 29U);
    }
}

/* The interrupt function for channel 30, 31 on EMIOS2 */
void EMIOS2_30_31_IRQHandler(void)
{
    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 30U) == true)
    {
        IC_Emios_IrqHandler(2U, 30U);
    }

    /* Check the FLAG bit is set or not */
    if (EMIOS_DRV_ReadFlagState(2U, 31U) == true)
    {
        IC_Emios_IrqHandler(2U, 31U);
    }
}
#endif  /* End of (IC_PAL_INSTANCE_MAX > 2U) */

#endif /* End of defined(IC_PAL_OVER_EMIOS) */


#if (defined(IC_PAL_OVER_ETIMER))
#if (IC_PAL_ETIMER_INSTANCE_COUNT > 0U)
void ETIMER0_TC0IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 0U);
}

void ETIMER0_TC1IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 1U);
}

void ETIMER0_TC2IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 2U);
}

void ETIMER0_TC3IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 3U);
}

void ETIMER0_TC4IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 4U);
}

void ETIMER0_TC5IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 5U);
}

void ETIMER0_TC6IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(0U, 6U);
}
#endif

#if (IC_PAL_ETIMER_INSTANCE_COUNT > 1U)
void ETIMER1_TC0IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 0U);
}

void ETIMER1_TC1IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 1U);
}

void ETIMER1_TC2IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 2U);
}

void ETIMER1_TC3IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 3U);
}

void ETIMER1_TC4IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 4U);
}

void ETIMER1_TC5IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 5U);
}

void ETIMER1_TC6IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 6U);
}

void ETIMER1_TC7IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(1U, 7U);
}
#endif

#if (IC_PAL_ETIMER_INSTANCE_COUNT > 2U)
void ETIMER2_TC0IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 0U);
}

void ETIMER2_TC1IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 1U);
}

void ETIMER2_TC2IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 2U);
}

void ETIMER2_TC3IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 3U);
}

void ETIMER2_TC4IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 4U);
}

void ETIMER2_TC5IR_IRQHandler(void)
{
    IC_Etimer_IrqHandler(2U, 5U);
}
#endif
#endif /* defined(IC_PAL_OVER_ETIMER) */

#if (defined(IC_PAL_OVER_FLEXPWM))

#if (IC_PAL_FLEXPWM_INSTANCE_COUNT > 0U)
void FlexPWM_0_CAF0_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(0U, 0U);
}

void FlexPWM_0_CAF1_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(0U, 1U);
}

void FlexPWM_0_CAF2_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(0U, 2U);
}

void FlexPWM_0_CAF3_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(0U, 3U);
}
#endif

#if (IC_PAL_FLEXPWM_INSTANCE_COUNT > 1U)
void FlexPWM_1_CAF0_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(1U, 0U);
}

void FlexPWM_1_CAF1_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(1U, 1U);
}

void FlexPWM_1_CAF2_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(1U, 2U);
}

void FlexPWM_1_CAF3_IRQHandler(void)
{
    IC_Flexpwm_IrqHandler(1U, 3U);
}
#endif

#endif /* defined(IC_PAL_OVER_FLEXPWM) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
