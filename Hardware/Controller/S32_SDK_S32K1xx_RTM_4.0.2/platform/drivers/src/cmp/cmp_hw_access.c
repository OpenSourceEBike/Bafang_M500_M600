/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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
 
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code. 
  */
  
#include "cmp_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_SetFunctionalMode
 * Description   : This function set CMP functional mode based on configuration
 * structure. If values for filter count or filter period are specific for selected mode
 * the value from input structure will be ignored.
 *   
 *   FUNCTIONAL MODE                 | FILTER COUNT  | FILTER PERIOD
 *   ----------------------------------------------------------------
 *   CMP_DISABLED                    |    IGNORED    |    IGNORED           
 *   CMP_CONTINUOUS                  |    IGNORED    |    IGNORED
 *   CMP_SAMPLED_NONFILTRED_INT_CLK  |    IGNORED    |    IGNORED
 *   CMP_SAMPLED_NONFILTRED_EXT_CLK  |    IGNORED    |     USED
 *   CMP_SAMPLED_FILTRED_INT_CLK     |     USED      |    IGNORED
 *   CMP_SAMPLED_FILTRED_EXT_CLK     |     USED      |     USED 
 *   CMP_WINDOWED                    |    IGNORED    |    IGNORED
 *   CMP_WINDOWED_RESAMPLED          |    IGNORED    |     USED
 *   CMP_WINDOWED_FILTRED            |     USED      |     USED 
 *
 *END**************************************************************************/
 void CMP_SetFunctionalMode(CMP_Type* baseAddr, cmp_mode_t mode, uint8_t filter_sample_count, uint8_t filter_sample_period)
 {
    uint32_t tmp = baseAddr->C0;
    tmp = tmp & ( ~(CMP_C0_SE_MASK & CMP_C0_FPR_MASK & CMP_C0_FILTER_CNT_MASK & CMP_C0_EN_MASK & CMP_C0_WE_MASK));
    baseAddr->C0 = tmp;
        switch(mode)
        {
        case CMP_DISABLED:
            break;
        case CMP_CONTINUOUS:
            tmp |= CMP_C0_EN(1U);
            break;
        case CMP_SAMPLED_NONFILTRED_EXT_CLK:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_SE(1U);
            tmp |= CMP_C0_FILTER_CNT(1U);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        case CMP_SAMPLED_NONFILTRED_INT_CLK:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_FILTER_CNT(1U);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        case CMP_SAMPLED_FILTRED_INT_CLK:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_FILTER_CNT(filter_sample_count);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        case CMP_SAMPLED_FILTRED_EXT_CLK:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_SE(1U);           
            tmp |= CMP_C0_FILTER_CNT(filter_sample_count);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        case CMP_WINDOWED:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_WE(1U);
            tmp |= CMP_C0_FILTER_CNT(0U);
            break;
        case CMP_WINDOWED_RESAMPLED:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_WE(1U);
            tmp |= CMP_C0_FILTER_CNT(1U);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        case CMP_WINDOWED_FILTRED:
            tmp |= CMP_C0_EN(1U);
            tmp |= CMP_C0_WE(1U);
            tmp |= CMP_C0_FILTER_CNT(filter_sample_count);
            tmp |= CMP_C0_FPR(filter_sample_period);
            break;
        default:
            /* Impossible case */
            break;
        }
        baseAddr->C0 = tmp;
 }

/* Function Name : CMP_GetFunctionalMode
 * Description   : This function return current CMP functional mode. CMP module cand be used in
 * one of this modes: CMP_DISABLED , CMP_CONTINUOUS, CMP_SAMPLED_NONFILTRED_INT_CLK, 
 * CMP_SAMPLED_NONFILTRED_EXT_CLK, CMP_SAMPLED_FILTRED_INT_CLK, CMP_SAMPLED_FILTRED_EXT_CLK,
 * CMP_WINDOWED, CMP_WINDOWED_RESAMPLED, CMP_WINDOWED_FILTRED
 *
 *END**************************************************************************/

 cmp_mode_t CMP_GetFunctionalMode(const CMP_Type* baseAddr)
{
    uint32_t state;
    uint32_t tmp = baseAddr->C0;
    cmp_mode_t mode = CMP_DISABLED;
    state = (((baseAddr->C0) & CMP_C0_EN_MASK) >> CMP_C0_EN_SHIFT) << 2U;
    state |= (((baseAddr->C0) & CMP_C0_WE_MASK) >> CMP_C0_WE_SHIFT) << 1U;
    state |= (((baseAddr->C0) & CMP_C0_SE_MASK) >> CMP_C0_SE_SHIFT);
    uint32_t filter_sample_period = (tmp & CMP_C0_FPR_MASK) >> CMP_C0_FPR_SHIFT;
    uint32_t filter_sample_count = (tmp & CMP_C0_FILTER_CNT_MASK) >> CMP_C0_FILTER_CNT_SHIFT;

    switch(state)
    {
    case 4: 
        if ((filter_sample_period == 0U) || (filter_sample_count == 0U))
        {
            mode = CMP_CONTINUOUS;
        }
        else if (filter_sample_count == 1U)
        {    
            mode = CMP_SAMPLED_NONFILTRED_INT_CLK;
        }
        else if (filter_sample_count > 1U)
        {
            mode = CMP_SAMPLED_FILTRED_INT_CLK;
        }
        else
        {
            /* Nothing */
        }
        break;
    case 5:
        if (filter_sample_count == 1U)
        {
            mode = CMP_SAMPLED_NONFILTRED_EXT_CLK;
        }
        else if (filter_sample_count > 1u)
        {        
            mode = CMP_SAMPLED_FILTRED_EXT_CLK;
        }
        else
        {
            /* Nothing */
        }
        break;
    case 6 :
        if ((filter_sample_period == 0U) || (filter_sample_count == 0U))
        {
            mode = CMP_WINDOWED;
        }
        else if ((filter_sample_count == 1U) && (filter_sample_period > 0U))
        {
            mode = CMP_WINDOWED_RESAMPLED;
        }
        else if ((filter_sample_count > 1U) && (filter_sample_period > 0U))
        {    
            mode = CMP_WINDOWED_FILTRED;
        }
        else
        {
            /*Nothing */
        }
        break;
    default:
        /* Impossible case */
        break;
    }

    if ((state & 0x4U) == 0U)
    {
        mode = CMP_DISABLED;
    }
	
    return mode;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
