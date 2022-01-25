/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * @file ftm_qd_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbols are declared in the driver common file as external; they are needed
 * at driver initialization to install the correct interrupt handler, but are not
 * a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "ftm_qd_driver.h"
#include "ftm_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_QuadDecodeStart
 * Description   : Configures the parameters needed and activates quadrature
 * decode mode.
 *
 * Implements    : FTM_DRV_QuadDecodeStart_Activity
 *END**************************************************************************/
status_t FTM_DRV_QuadDecodeStart(uint32_t instance,
                                 const ftm_quad_decode_config_t * config)
{
    DEV_ASSERT((instance == 1U) || (instance == 2U));
    DEV_ASSERT(config != NULL);
    FTM_Type * ftmBase = g_ftmBase[instance];
    ftm_state_t * state = ftmStatePtr[instance];
    status_t retStatus = STATUS_SUCCESS;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        /* Disable Quadrature Decoder */
        FTM_DRV_SetQuadDecoderCmd(ftmBase, false);
        FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
        /* Configure Quadrature Decoder */
        /* Set decoder mode Speed and direction or Phase A and Phase B encoding */
        FTM_DRV_SetQuadMode(ftmBase, (uint8_t)config->mode);
        /* Set filter state for Phase A (enable/disable) */
        FTM_DRV_SetQuadPhaseAFilterCmd(ftmBase, config->phaseAConfig.phaseInputFilter);
        /* Set Phase A filter value if phase filter is enabled */
        if (config->phaseAConfig.phaseInputFilter)
        {
            FTM_DRV_SetChnInputCaptureFilter(ftmBase, CHAN0_IDX, config->phaseAConfig.phaseFilterVal);
        }

        /* Set filter state for Phase B (enable/disable) */
        FTM_DRV_SetQuadPhaseBFilterCmd(ftmBase, config->phaseBConfig.phaseInputFilter);
        /* Set Phase B filter value if phase filter is enabled */
        if (config->phaseBConfig.phaseInputFilter)
        {
            FTM_DRV_SetChnInputCaptureFilter(ftmBase, CHAN1_IDX, config->phaseBConfig.phaseFilterVal);
        }

        /* Set polarity for Phase A and Phase B */
        FTM_DRV_SetQuadPhaseAPolarity(ftmBase, (uint8_t)config->phaseAConfig.phasePolarity);
        FTM_DRV_SetQuadPhaseBPolarity(ftmBase, (uint8_t)config->phaseBConfig.phasePolarity);
        /* Configure counter (initial value and maximum value) */
        FTM_DRV_SetCounterInitVal(ftmBase, config->initialVal);
        FTM_DRV_SetMod(ftmBase, config->maxVal);
        FTM_DRV_SetCounter(ftmBase, config->initialVal);
        /* Enable Quadrature Decoder */
        FTM_DRV_SetQuadDecoderCmd(ftmBase, true);
        state->ftmMode = FTM_MODE_QUADRATURE_DECODER;
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_QuadDecodeStop
 * Description   : De-activates quadrature decoder mode.
 *
 * Implements    : FTM_DRV_QuadDecodeStop_Activity
 *END**************************************************************************/
status_t FTM_DRV_QuadDecodeStop(uint32_t instance)
{
    DEV_ASSERT((instance == 1U) || (instance == 2U));
    FTM_Type * ftmBase = g_ftmBase[instance];
    ftm_state_t * state = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Disable Quadrature decoder */
    FTM_DRV_SetQuadDecoderCmd(ftmBase, false);
    state->ftmMode = FTM_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_QuadGetState
 * Description   : Return the current quadrature decoder state
 * (counter value, overflow flag and overflow direction)
 *
 * Implements    : FTM_DRV_QuadGetState_Activity
 *END**************************************************************************/
ftm_quad_decoder_state_t FTM_DRV_QuadGetState(uint32_t instance)
{
    DEV_ASSERT((instance == 1U) || (instance == 2U));
    FTM_Type const * ftmBase = g_ftmBase[instance];
    ftm_quad_decoder_state_t state;

    state.counterDirection = FTM_DRV_GetQuadDir(ftmBase);
    state.overflowDirection = FTM_DRV_GetQuadTimerOverflowDir(ftmBase);
    state.overflowFlag = FTM_DRV_HasTimerOverflowed(ftmBase);
    state.counter = FTM_DRV_GetCounter(ftmBase);

    return state;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_QD_DRV_GetDefaultConfig
 * Description   : This function will get the default configuration values
 * in the structure which is used as a common use-case.
 * Return        : None
 * Implements    : FTM_QD_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void FTM_QD_DRV_GetDefaultConfig(ftm_quad_decode_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->mode                           = FTM_QUAD_COUNT_AND_DIR;
    config->initialVal                     = 0U;
    config->maxVal                         = 65535U;
    config->phaseAConfig.phaseInputFilter = false;
    config->phaseAConfig.phaseFilterVal   = 0U;
    config->phaseAConfig.phasePolarity    = FTM_QUAD_PHASE_NORMAL;
    config->phaseBConfig.phaseInputFilter = false;
    config->phaseBConfig.phaseFilterVal   = 0U;
    config->phaseBConfig.phasePolarity    = FTM_QUAD_PHASE_NORMAL;
}

/*******************************************************************************
* EOF
******************************************************************************/
