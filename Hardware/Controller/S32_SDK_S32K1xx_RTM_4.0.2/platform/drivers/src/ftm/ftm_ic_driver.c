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
 * @file ftm_ic_driver.c
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

#include "ftm_ic_driver.h"
#include "ftm_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
static void FTM_DRV_InputCaptureHandler(uint32_t instance,
                                        uint8_t channelPair);

static void FTM_DRV_IrqHandler(uint32_t instance,
                               uint8_t channelPair);

static status_t FTM_DRV_InitMeasurement(uint32_t instance,
                                        uint8_t channel,
                                        bool continuousModeEn,
                                        ftm_signal_measurement_mode_t measurementType);

#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
void FTM0_Ch0_7_IRQHandler(void);
void FTM1_Ch0_7_IRQHandler(void);
#endif

#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 4U)
void FTM0_Ch0_Ch1_IRQHandler(void);

void FTM0_Ch2_Ch3_IRQHandler(void);

void FTM0_Ch4_Ch5_IRQHandler(void);

void FTM0_Ch6_Ch7_IRQHandler(void);

void FTM1_Ch0_Ch1_IRQHandler(void);

void FTM1_Ch2_Ch3_IRQHandler(void);

void FTM1_Ch4_Ch5_IRQHandler(void);

void FTM1_Ch6_Ch7_IRQHandler(void);

#if (FTM_INSTANCE_COUNT > 2U)
void FTM2_Ch0_Ch1_IRQHandler(void);

void FTM2_Ch2_Ch3_IRQHandler(void);

void FTM2_Ch4_Ch5_IRQHandler(void);

void FTM2_Ch6_Ch7_IRQHandler(void);

void FTM3_Ch0_Ch1_IRQHandler(void);

void FTM3_Ch2_Ch3_IRQHandler(void);

void FTM3_Ch4_Ch5_IRQHandler(void);

void FTM3_Ch6_Ch7_IRQHandler(void);
#endif

#if (FTM_INSTANCE_COUNT > 4U)
void FTM4_Ch0_Ch1_IRQHandler(void);

void FTM4_Ch2_Ch3_IRQHandler(void);

void FTM4_Ch4_Ch5_IRQHandler(void);

void FTM4_Ch6_Ch7_IRQHandler(void);

void FTM5_Ch0_Ch1_IRQHandler(void);

void FTM5_Ch2_Ch3_IRQHandler(void);

void FTM5_Ch4_Ch5_IRQHandler(void);

void FTM5_Ch6_Ch7_IRQHandler(void);
#endif

#if (FTM_INSTANCE_COUNT > 6U)
void FTM6_Ch0_Ch1_IRQHandler(void);

void FTM6_Ch2_Ch3_IRQHandler(void);

void FTM6_Ch4_Ch5_IRQHandler(void);

void FTM6_Ch6_Ch7_IRQHandler(void);

void FTM7_Ch0_Ch1_IRQHandler(void);

void FTM7_Ch2_Ch3_IRQHandler(void);

void FTM7_Ch4_Ch5_IRQHandler(void);

void FTM7_Ch6_Ch7_IRQHandler(void);
#endif
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_InitMeasurement
 * Description   : This function is used to initialize the measurement mode in the
 * input capture mode. It is called in the FTM_DRV_InitInputCapture function.
 *
 *END**************************************************************************/
static status_t FTM_DRV_InitMeasurement(uint32_t instance,
                                        uint8_t channel,
                                        bool continuousModeEn,
                                        ftm_signal_measurement_mode_t measurementType)
{
    DEV_ASSERT((channel % 2U) == 0U);
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t chnlPairNum = (uint8_t)(channel >> 1U);
    status_t retStatus = STATUS_SUCCESS;

    /* Enable the dual edge mode */
    FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, chnlPairNum, true);
    /* Enable dual edge input capture */
    FTM_DRV_SetDualChnDecapCmd(ftmBase, chnlPairNum, true);

    /* If continuous mode is set */
    if (true == continuousModeEn)
    {
        /* Set MSnA and MSnB bit */
        FTM_DRV_SetChnMSnBAMode(ftmBase, channel, 3U);
    }
    else
    {
        /* Clear MSnA and Set MSnB bit */
        FTM_DRV_SetChnMSnBAMode(ftmBase, channel, 2U);
    }

    /* Check If want to measure a pulse width or period of the signal */
    if ((FTM_PERIOD_ON_MEASUREMENT == measurementType) || (FTM_RISING_EDGE_PERIOD_MEASUREMENT== measurementType))
    {
        FTM_DRV_SetChnEdgeLevel(ftmBase, channel, 1U);
        if (FTM_PERIOD_ON_MEASUREMENT == measurementType)
        {
            /* Measure time between rising and falling edge - positive duty */
            FTM_DRV_SetChnEdgeLevel(ftmBase, (uint8_t)(channel + 1U), 2U);
        }
        else
        {
            /* If channel (n) is configured to capture falling edges (ELS(n)B:ELS(n)A = 0:1)
             * then channel (n+1) also captures falling edges (ELS(n+1)B:ELS(n+1)A = 0:1) */
            FTM_DRV_SetChnEdgeLevel(ftmBase, (uint8_t)(channel + 1U), 1U);
        }
    }
    else if ((FTM_PERIOD_OFF_MEASUREMENT == measurementType) || (FTM_FALLING_EDGE_PERIOD_MEASUREMENT == measurementType))
    {
        FTM_DRV_SetChnEdgeLevel(ftmBase, channel, 2U);
        if (FTM_PERIOD_OFF_MEASUREMENT == measurementType)
        {
            /* Measure time between falling and rising edge - negative duty */
            FTM_DRV_SetChnEdgeLevel(ftmBase, (uint8_t)(channel + 1U), 1U);
        }
        else
        {
            /* If channel (n) is configured to capture rising edges (ELS(n)B:ELS(n)A = 1:0) than
             * channel (n+1) is setup to capture also raising edges (ELS(n+1)B:ELS(n+1)A = 1:0) */
            FTM_DRV_SetChnEdgeLevel(ftmBase, (uint8_t)(channel + 1U), 2U);
        }
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    /* Enable the interrupt request for the channel which will indicate that the measurement is done. */
    FTM_DRV_EnableChnInt(ftmBase, (uint8_t)(channel + 1U));
    INT_SYS_EnableIRQ(g_ftmIrqId[instance][channel]);

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_InitInputCapture
 * Description   : Configures Channel Input Capture for either getting time-stamps on edge detection
 * or on signal measurement . When the edge specified in the captureMode
 * argument occurs on the channel the FTM counter is captured into the CnV register.
 * The user will have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed in is 0. The filter function
 * is available only on channels 0,1,2,3.
 *
 * Implements    : FTM_DRV_InitInputCapture_Activity
 *END**************************************************************************/
status_t FTM_DRV_InitInputCapture(uint32_t instance,
                                  const ftm_input_param_t * param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    DEV_ASSERT(param->nMaxCountValue > 0U);
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t chnlPairNum = 0U;
    uint8_t index = 0U;
    uint8_t hwChannel = 0U;
    bool contModeEn;
    ftm_signal_measurement_mode_t measurementType;
    ftm_state_t * state = ftmStatePtr[instance];
    status_t retStatus = STATUS_SUCCESS;

    if ((NULL != state) && ((FTM_MODE_NOT_INITIALIZED == state->ftmMode) || (FTM_MODE_EDGE_ALIGNED_PWM == state->ftmMode) || (FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == state->ftmMode)))
    {
        if (state->ftmMode == FTM_MODE_NOT_INITIALIZED)
        {
            FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
            FTM_DRV_SetCounterInitVal(ftmBase, 0U);
            FTM_DRV_SetMod(ftmBase, param->nMaxCountValue);
            FTM_DRV_SetCpwms(ftmBase, false);
        }
        else
        {
            /* In this case clock source is configured from PWM init. */
            state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE;
        }

        /* Get curent MOD value */
        state->ftmModValue = FTM_DRV_GetMod(ftmBase);

        for (index = 0U; index < param->nNumChannels; index++)
        {
            hwChannel = param->inputChConfig[index].hwChannelId;
            chnlPairNum =  (uint8_t)(hwChannel >> 1U);
            /* Save in state structure user define handlers */
            state->channelsCallbacksParams[hwChannel] =  param->inputChConfig[index].channelsCallbacksParams;
            state->channelsCallbacks[hwChannel] = param->inputChConfig[index].channelsCallbacks;
            state->enableNotification[hwChannel] = true;

            /* Enable filtering for input channels */
            if (hwChannel < CHAN4_IDX)
            {
                if (true == param->inputChConfig[index].filterEn)
                {
                    FTM_DRV_SetChnInputCaptureFilter(ftmBase, hwChannel, (uint8_t)param->inputChConfig[index].filterValue);
                }
                else
                {
                    FTM_DRV_SetChnInputCaptureFilter(ftmBase, hwChannel, 0U);
                }
            }

            if (FTM_EDGE_DETECT == param->inputChConfig[index].inputMode)
            {
                /* Disable the dual edge mode */
                FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, chnlPairNum, false);
                /* Set input capture mode */
                FTM_DRV_SetChnMSnBAMode(ftmBase, hwChannel, 0U);
                /* Set the event which will generate the interrupt */
                FTM_DRV_SetChnEdgeLevel(ftmBase, hwChannel, (uint8_t)param->inputChConfig[index].edgeAlignement);
                /* Enable interrupt request for the current channel */
                FTM_DRV_EnableChnInt(ftmBase, hwChannel);
                INT_SYS_EnableIRQ(g_ftmIrqId[instance][hwChannel]);
            }
            else if (FTM_SIGNAL_MEASUREMENT == param->inputChConfig[index].inputMode)
            {
                measurementType = param->inputChConfig[index].measurementType;
                contModeEn = param->inputChConfig[index].continuousModeEn;
                /* Configure the measurement type */
                retStatus = FTM_DRV_InitMeasurement(instance, hwChannel, contModeEn, measurementType);
            }
            else
            {
                /* Do nothing */
            }

            if (STATUS_SUCCESS != retStatus)
            {
                break;
            }
        }

        if (STATUS_SUCCESS == retStatus)
        {
            /* Update mode. */
            if (state->ftmMode == FTM_MODE_NOT_INITIALIZED)
            {
                state->ftmMode = FTM_MODE_INPUT_CAPTURE;
                /* Set clock source to start the counter. */
                FTM_DRV_SetClockSource(ftmBase, state->ftmClockSource);
            }
        }
        else
        {
            if (state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
            {
                state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM;
            }
        }
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_DeinitInputCapture
 * Description   : Disables Channel Input Capture
 *
 * Implements    : FTM_DRV_DeinitInputCapture_Activity
 *END**************************************************************************/
status_t FTM_DRV_DeinitInputCapture(uint32_t instance,
                                    const ftm_input_param_t * param)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(param != NULL);
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t chnlPairNum = 0U;
    uint8_t index = 0U;
    uint8_t hwChannel = 0U;
    ftm_state_t * state = ftmStatePtr[instance];
    DEV_ASSERT(state != NULL);

    /* Remain configuration for PWM */
    if (state->ftmMode != FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
    {
        /* FTM counter is disabled */
        FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
        FTM_DRV_SetCounterInitVal(ftmBase, 0U);
        FTM_DRV_SetMod(ftmBase, 0xFFFFU);
        FTM_DRV_SetCpwms(ftmBase, false);
    }

    for (index = 0U; index < param->nNumChannels; index++)
    {
        hwChannel = param->inputChConfig[index].hwChannelId;
        chnlPairNum =  (uint8_t)(hwChannel >> 1U);
        /* Disable filtering for input channels */
        if (hwChannel < CHAN4_IDX)
        {
            FTM_DRV_SetChnInputCaptureFilter(ftmBase, hwChannel, 0U);
        }

        FTM_DRV_SetDualChnCombineCmd(ftmBase, chnlPairNum, false);
        FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, chnlPairNum, false);
        FTM_DRV_SetChnEdgeLevel(ftmBase, hwChannel, (uint8_t)0U);
        FTM_DRV_DisableChnInt(ftmBase, hwChannel);
        FTM_DRV_ClearChnEventFlag(ftmBase, hwChannel);

        FTM_DRV_SetChnEdgeLevel(ftmBase, (hwChannel | 1U), (uint8_t)0U);
        FTM_DRV_DisableChnInt(ftmBase, (hwChannel | 1U));
        FTM_DRV_ClearChnEventFlag(ftmBase, (hwChannel | 1U));
    }

    if (state->ftmMode == FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE)
    {
        state->ftmMode = FTM_MODE_EDGE_ALIGNED_PWM;
    }
    else
    {
        state->ftmMode = FTM_MODE_NOT_INITIALIZED;
    }

    /* Clear Callbacks function from the state structure */
    for (index = 0U; index < FEATURE_FTM_CHANNEL_COUNT; index++)
    {
        state->channelsCallbacksParams[index] =  NULL;
        state->channelsCallbacks[index] = NULL;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_GetInputCaptureMeasurement
 * Description   : This function is used to calculate the measurement and/or time stamps values
 * which are read from the C(n, n+1)V registers and stored to the static buffers.
 *
 * Implements    : FTM_DRV_GetInputCaptureMeasurement_Activity
 *END**************************************************************************/
uint16_t FTM_DRV_GetInputCaptureMeasurement(uint32_t instance,
                                            uint8_t channel)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
    const ftm_state_t * state = ftmStatePtr[instance];
    uint16_t value = 0U;

    if (state != NULL)
    {
        value = state->measurementResults[channel];
    }

    return value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_StartNewSignalMeasurement
 * Description   : This function starts new Signal Measurements on a dual input compare channel
 * that is configured as single-shot measurement.
 *
 * Implements    : FTM_DRV_StartNewSignalMeasurement_Activity
 *END**************************************************************************/
status_t FTM_DRV_StartNewSignalMeasurement(uint32_t instance,
                                           uint8_t channel)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
    /* Clear CH(n)F and CH(n+1)F  flags and Set DECAP bit */
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t chnlPairNum = (uint8_t)(channel >> 1U);
    status_t retStatus = STATUS_SUCCESS;

    /* Get channel mode */
    if (FTM_FEATURE_INPUT_CAPTURE_SINGLE_SHOT == FTM_DRV_GetChnMode(ftmBase, channel))
    {
        if (FTM_DRV_GetDualEdgeCaptureBit(ftmBase, chnlPairNum))
        {
            /* Clear event flags for channel n and n + 1 */
            FTM_DRV_ClearChnEventFlag(ftmBase, (uint8_t)(channel + 1U));
            FTM_DRV_ClearChnEventFlag(ftmBase, channel);
            /* Set DECAP bit to start measurement */
            FTM_DRV_SetDualChnDecapCmd(ftmBase, chnlPairNum, true);
        }
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_IC_DRV_SetChannelMode
 * Description   : This function is used to change the channel mode at run time or
 * when stopping channel. The channel mode is selected in the ftm_ic_op_mode_t
 * enumeration type.
 *
 * Implements    : FTM_IC_DRV_SetChannelMode_Activity
 *END**************************************************************************/
status_t FTM_IC_DRV_SetChannelMode(uint32_t instance,
                                   uint8_t channel,
                                   ftm_ic_op_mode_t inputMode,
                                   bool enableContinuousCapture)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);

    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t level = 1U;
    uint8_t nextChnlevel = 1U;
    uint8_t channelPair =  (uint8_t)(channel >> 1U);

    if (inputMode == FTM_DISABLE_OPERATION)
    {
        /* Set the edge level to disable operation on the channel input */
        FTM_DRV_SetChnEdgeLevel(ftmBase, channel, 0x0U);
    }
    else if ((inputMode == FTM_TIMESTAMP_RISING_EDGE) ||
             (inputMode == FTM_TIMESTAMP_FALLING_EDGE) ||
             (inputMode == FTM_TIMESTAMP_BOTH_EDGES))
    {
        /* Disable the dual edge mode */
        FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, channelPair, false);
        /* Set input capture mode */
        FTM_DRV_SetChnMSnBAMode(ftmBase, channel, 0U);
        /* Set the event which will generate the interrupt */
        FTM_DRV_SetChnEdgeLevel(ftmBase, channel, (uint8_t)inputMode);
        /* Enable interrupt request for the current channel */
        FTM_DRV_EnableChnInt(ftmBase, channel);
    }
    else
    {
        /* Check the channel is even number */
        DEV_ASSERT((channel % 2U) == 0U);
        /* Enable the dual edge mode */
        FTM_DRV_SetDualEdgeCaptureCmd(ftmBase, channelPair, true);
        /* Enable dual edge input capture */
        FTM_DRV_SetDualChnDecapCmd(ftmBase, channelPair, true);
        /* If continuous mode is set*/
        if (true == enableContinuousCapture)
        {
            /* Set MSnA and MSnB bit*/
            FTM_DRV_SetChnMSnBAMode(ftmBase, channel, 3U);
        }
        else
        {
            /* Clear MSnA and Set MSnB bit*/
            FTM_DRV_SetChnMSnBAMode(ftmBase, channel, 2U);
        }

        /* Check If want to measure a pulse width or period of the signal */
        if ((FTM_MEASURE_PULSE_HIGH == inputMode) || (FTM_MEASURE_RISING_EDGE_PERIOD == inputMode))
        {
            if (FTM_MEASURE_PULSE_HIGH == inputMode)
            {
                /* Measure time between rising and falling edge - positive duty */
                nextChnlevel = 2U;
            }
        }
        else
        {
            level = 2U;
            if (FTM_MEASURE_FALLING_EDGE_PERIOD == inputMode)
            {
                /* If channel (n) is configured to capture rising edges (ELS(n)B:ELS(n)A = 1:0) than
                 * channel (n+1) is setup to capture also raising edges (ELS(n+1)B:ELS(n+1)A = 1:0) */
                nextChnlevel = 2U;
            }
        }

        FTM_DRV_SetChnEdgeLevel(ftmBase, channel, level);
        FTM_DRV_SetChnEdgeLevel(ftmBase, (uint8_t)(channel + 1U), nextChnlevel);

        /* Disable interrupt request for the current channel */
        FTM_DRV_DisableChnInt(ftmBase, channel);
        FTM_DRV_EnableChnInt(ftmBase, (uint8_t)(channel + 1U));
    }

    return STATUS_SUCCESS;
}

#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 1U)
/* Implementation of FTM0_Ch0_Ch7_IRQHandler master handler named in startup code. */
void FTM0_Ch0_7_IRQHandler(void)
{
    uint32_t statusFlag = FTM_DRV_GetStatusFlags(0U);
    uint32_t enabledInt = FTM_DRV_GetEnabledInterrupts(0U);
    uint32_t index = 0U;
    uint32_t shift = 0U;
    bool isIntOnChn = false;
    bool isIntOnNextChn = false;

    for (index = 0U; index < (uint32_t)(FEATURE_FTM_CHANNEL_COUNT / 2U); index++)
    {
        shift = index << 1U;
        isIntOnChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;
        shift++;
        isIntOnNextChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;

        if ((isIntOnChn == true) || (isIntOnNextChn == true))
        {
            FTM_DRV_IrqHandler(0U, (uint8_t)index);
        }
    }
}

/* Implementation of FTM1_Ch0_Ch7_IRQHandler master handler named in startup code. */
void FTM1_Ch0_7_IRQHandler(void)
{
    uint32_t statusFlag = FTM_DRV_GetStatusFlags(1U);
    uint32_t enabledInt = FTM_DRV_GetEnabledInterrupts(1U);
    uint32_t index = 0U;
    uint32_t shift = 0U;
    bool isIntOnChn = false;
    bool isIntOnNextChn = false;

    for (index = 0U; index < (uint32_t)(FEATURE_FTM_CHANNEL_COUNT / 2U); index++)
    {
        shift = index << 1U;
        isIntOnChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;
        shift++;
        isIntOnNextChn = (((statusFlag & enabledInt) & (0x1UL << shift)) != 0U) ? true : false;

        if ((isIntOnChn == true) || (isIntOnNextChn == true))
        {
            FTM_DRV_IrqHandler(1U, (uint8_t)index);
        }
    }
}
#endif

#if (FEATURE_FTM_HAS_NUM_IRQS_CHANS == 4U)
/* Implementation of FTM0_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM0_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(0U, 0U);
}

/* Implementation of FTM0_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM0_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(0U, 1U);
}

/* Implementation of FTM0_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM0_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(0U, 2U);
}

/* Implementation of FTM0_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM0_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(0U, 3U);
}

/* Implementation of FTM1_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM1_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(1U, 0U);
}

/* Implementation of FTM1_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM1_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(1U, 1U);
}

/* Implementation of FTM1_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM1_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(1U, 2U);
}

/* Implementation of FTM1_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM1_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(1U, 3U);
}

#if (FTM_INSTANCE_COUNT > 2U)
/* Implementation of FTM2_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM2_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(2U, 0U);
}

/* Implementation of FTM2_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM2_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(2U, 1U);
}

/* Implementation of FTM2_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM2_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(2U, 2U);
}

/* Implementation of FTM2_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM2_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(2U, 3U);
}

/* Implementation of FTM3_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM3_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(3U, 0U);
}

/* Implementation of FTM3_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM3_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(3U, 1U);
}

/* Implementation of FTM3_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM3_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(3U, 2U);
}

/* Implementation of FTM3_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM3_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(3U, 3U);
}
#endif

#if (FTM_INSTANCE_COUNT > 4U)
/* Implementation of FTM4_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM4_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(4U, 0U);
}

/* Implementation of FTM4_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM4_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(4U, 1U);
}

/* Implementation of FTM4_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM4_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(4U, 2U);
}

/* Implementation of FTM4_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM4_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(4U, 3U);
}

/* Implementation of FTM5_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM5_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(5U, 0U);
}

/* Implementation of FTM5_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM5_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(5U, 1U);
}

/* Implementation of FTM5_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM5_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(5U, 2U);
}

/* Implementation of FTM5_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM5_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(5U, 3U);
}
#endif

#if (FTM_INSTANCE_COUNT > 6U)
/* Implementation of FTM6_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM6_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(6U, 0U);
}

/* Implementation of FTM6_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM6_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(6U, 1U);
}

/* Implementation of FTM6_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM6_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(6U, 2U);
}

/* Implementation of FTM6_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM6_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(6U, 3U);
}

/* Implementation of FTM7_Ch0_Ch1_IRQHandler master handler named in startup code. */
void FTM7_Ch0_Ch1_IRQHandler(void)
{
    FTM_DRV_IrqHandler(7U, 0U);
}

/* Implementation of FTM7_Ch2_Ch3_IRQHandler master handler named in startup code. */
void FTM7_Ch2_Ch3_IRQHandler(void)
{
    FTM_DRV_IrqHandler(7U, 1U);
}

/* Implementation of FTM7_Ch4_Ch5_IRQHandler master handler named in startup code. */
void FTM7_Ch4_Ch5_IRQHandler(void)
{
    FTM_DRV_IrqHandler(7U, 2U);
}

/* Implementation of FTM7_Ch6_Ch7_IRQHandler master handler named in startup code. */
void FTM7_Ch6_Ch7_IRQHandler(void)
{
    FTM_DRV_IrqHandler(7U, 3U);
}
#endif
#endif

static void FTM_DRV_IrqHandler(uint32_t instance,
                               uint8_t channelPair)
{
    const ftm_state_t * state = ftmStatePtr[instance];
    switch (state->ftmMode)
    {
        case FTM_MODE_INPUT_CAPTURE:
        case FTM_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE:
            FTM_DRV_InputCaptureHandler(instance, channelPair);
            break;
        default:
            /* Nothing to do */
            break;
    }
}

static void FTM_DRV_InputCaptureHandler(uint32_t instance,
                                        uint8_t channelPair)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(channelPair < (FEATURE_FTM_CHANNEL_COUNT >> 1U));

    ftm_state_t * state = ftmStatePtr[instance];
    FTM_Type * ftmBase = g_ftmBase[instance];
    uint8_t channel = (uint8_t)(channelPair << 1U);

    /* Verify the mode for current pair of channels */
    if (FTM_DRV_GetDualEdgeCaptureBit(ftmBase, channelPair))
    {
        /* Dual edge input capture case */
        uint16_t first_event_time = FTM_DRV_GetChnCountVal(ftmBase, channel);
        uint16_t second_event_time = FTM_DRV_GetChnCountVal(ftmBase, (uint8_t)(channel + 1U));
        if (second_event_time <= first_event_time)
        {
            /* Measurement when overflow occurred */
            state->measurementResults[channel] = (uint16_t)(second_event_time + (state->ftmModValue - first_event_time));
        }
        else
        {
            /* Measurement when overflow doesn't occurred */
            state->measurementResults[channel] = (uint16_t)(second_event_time - first_event_time);
        }

        /* Clear flags for channels n and n+1 */
        FTM_DRV_ClearChnEventFlag(ftmBase, channel);
        FTM_DRV_ClearChnEventFlag(ftmBase, (uint8_t)(channel + 1U));
    }
    else
    {
        /* To get the channel interrupt source the both channels flag must be checked */
        if (false == FTM_DRV_HasChnEventOccurred(ftmBase, channel))
        {
            channel++;
        }

        /* Get the time stamp of the event */
        state->measurementResults[channel] = FTM_DRV_GetChnCountVal(ftmBase, channel);
        /* Clear the flag for C(n+1) channel */
        FTM_DRV_ClearChnEventFlag(ftmBase, channel);
    }

    /* Re-update MOD value */
    state->ftmModValue = FTM_DRV_GetMod(ftmBase);

    /* If the callback is defined to use it */
    if (((state->channelsCallbacks[channel]) != NULL) && (state->enableNotification[channel] == true))
    {
        state->channelsCallbacks[channel](IC_EVENT_MEASUREMENT_COMPLETE, state->channelsCallbacksParams[channel]);
    }
}

/*******************************************************************************
* EOF
******************************************************************************/
