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
 * @file ftm_oc_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "ftm_oc_driver.h"
#include "ftm_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_InitOutputCompare
 * Description   : Configures the FTM to generate timed pulses
 * When the FTM counter matches the value of compareVal argument (this is
 * written into CnV register), the channel output is changed based on what is specified
 * in the compareMode argument.
 *
 * Implements    : FTM_DRV_InitOutputCompare_Activity
 *END**************************************************************************/
status_t FTM_DRV_InitOutputCompare(uint32_t instance,
                                   const ftm_output_cmp_param_t * param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    DEV_ASSERT(param->maxCountValue > 0U);
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t index = 0U;
    uint8_t hwChannel = 0U;
    uint8_t chnlPairNum = 0U;
    ftm_state_t * state = ftmStatePtr[instance];
    status_t retStatus = STATUS_SUCCESS;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
        FTM_DRV_SetCpwms(ftmBase, false);
        /* Clear the overflow flag */
        FTM_DRV_ClearTimerOverflow(ftmBase);
        FTM_DRV_SetCounterInitVal(ftmBase, 0U);
        FTM_DRV_SetMod(ftmBase, param->maxCountValue);
        FTM_DRV_SetCounter(ftmBase, 0U);
        FTM_DRV_SetQuadDecoderCmd(ftmBase, false);
        /* Use FTM as counter, disable all the channels */
        for (index = 0U; index < param->nNumOutputChannels; index++)
        {
            DEV_ASSERT(param->maxCountValue >= param->outputChannelConfig[index].comparedValue);
            hwChannel = param->outputChannelConfig[index].hwChannelId;
            chnlPairNum =  (uint8_t)(hwChannel >> 1U);
            FTM_DRV_SetDualChnMofCombineCmd(ftmBase, chnlPairNum, false);
            FTM_DRV_SetDualChnCombineCmd(ftmBase, chnlPairNum, false);
            FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, chnlPairNum, false);
            /* Set Channel Output mode */
            FTM_DRV_SetChnEdgeLevel(ftmBase, hwChannel, (uint8_t)(param->outputChannelConfig[index].chMode));
            /* Enter counter mode for all configured channels */
            FTM_DRV_SetChnMSnBAMode(ftmBase, hwChannel, 1U);
            /* Write initial count value for all channels */
            FTM_DRV_SetChnCountVal(ftmBase, hwChannel, param->outputChannelConfig[index].comparedValue);
            /* Enable channel output */
            FTM_DRV_EnablePwmChannelOutputs(ftmBase, hwChannel);
            /* Enable the generation a trigger on chip module */
            FTM_DRV_SetChnTriggerCmd(ftmBase, hwChannel, param->outputChannelConfig[index].enableExternalTrigger);
        }

        /* Set software trigger */
        FTM_DRV_SetSoftwareTriggerCmd(ftmBase, true);
        state->ftmMode = param->mode;
        /* Set clock source to start the counter */
        FTM_DRV_SetClockSource(ftmBase, state->ftmClockSource);
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_DeinitOutputCompare
 * Description   : Disables compare match output control and clears FTM timer configuration
 *
 * Implements    : FTM_DRV_DeinitOutputCompare_Activity
 *END**************************************************************************/
status_t FTM_DRV_DeinitOutputCompare(uint32_t instance,
                                     const ftm_output_cmp_param_t * param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t index = 0U;
    uint8_t hwChannel = 0U;
    ftm_state_t * state = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Stop the FTM counter */
    FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
    /* Clear the overflow flag */
    FTM_DRV_ClearTimerOverflow(ftmBase);
    FTM_DRV_SetCounterInitVal(ftmBase, 0U);
    for (index = 0U; index < param->nNumOutputChannels; index++)
    {
        hwChannel = param->outputChannelConfig[index].hwChannelId;
        /* Disable Channel Output mode */
        FTM_DRV_SetChnEdgeLevel(ftmBase, hwChannel, (uint8_t)0U);
        /* Write initial count value for all channels to 0xFFFF */
        FTM_DRV_SetChnCountVal(ftmBase, hwChannel, 0U);
        /* Disable channel output */
        FTM_DRV_DisablePwmChannelOutputs(ftmBase, hwChannel);
    }

    /* Clear out the registers */
    FTM_DRV_SetMod(ftmBase, 0U);
    FTM_DRV_SetCounter(ftmBase, 0U);
    state->ftmMode = FTM_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_UpdateOutputCompareChannel
 * Description   : Sets the next compare match value on the given channel starting
 *                 from the current counter value.
 *
 * Implements    : FTM_DRV_UpdateOutputCompareChannel_Activity
 *END**************************************************************************/
status_t FTM_DRV_UpdateOutputCompareChannel(uint32_t instance,
                                            uint8_t channel,
                                            uint16_t nextComparematchValue,
                                            ftm_output_compare_update_t update,
                                            bool softwareTrigger)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
    uint16_t counterValue = FTM_DRV_GetCounter(g_ftmBase[instance]);
    uint16_t compareValue = 0U;
    uint16_t maxCounterValue = FTM_DRV_GetMod(g_ftmBase[instance]);
    DEV_ASSERT(nextComparematchValue <= maxCounterValue);
    FTM_Type * ftmBase = g_ftmBase[instance];

    if (update == FTM_RELATIVE_VALUE)
    {
        /* Configure channel compare register */
        if (nextComparematchValue > (maxCounterValue - counterValue))
        {
            compareValue = (uint16_t)(nextComparematchValue - (maxCounterValue - counterValue));
        }
        else
        {
            compareValue = (uint16_t)(counterValue + nextComparematchValue);
        }
    }
    else
    {
        compareValue = nextComparematchValue;
    }

    /* Set CnV value and use software trigger for sync */
    FTM_DRV_SetChnCountVal(g_ftmBase[instance], channel, compareValue);
    FTM_DRV_SetSoftwareTriggerCmd(ftmBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*******************************************************************************
* EOF
******************************************************************************/
