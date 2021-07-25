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
 * @file pwm_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.7, Symbol not referenced
 * These symbols are referenced only for some platforms.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variable must be global because it can't be allocated on stack.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, essential type
 * Expression assigned to a narrower or different essential type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.1, Unpermitted operand to operator.
 * This is required to get the right returned status from the
 * initialization function.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.7, other operand.
 * Composite expression with smaller essential type than other operand.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 */

#include "pwm_pal.h"

/* Include ETIMER special functions. */
#if (defined (PWM_OVER_ETIMER))
/* nothing here */
#endif

/* Define state structures for FTM */
#if (defined(PWM_OVER_FTM))
    /*! @brief FTM state structures */
    static ftm_state_t FtmState[NO_OF_FTM_INSTS_FOR_PWM];
    /*! @brief FTM state-instance matching */
    static uint32_t FtmStateInstanceMapping[NO_OF_FTM_INSTS_FOR_PWM];
    /*! @brief FTM  available resources table */
    static bool FtmStateIsAllocated[NO_OF_FTM_INSTS_FOR_PWM];
    /*! @brief Store the flag of channel is used in combined PWM signal */
    static bool pwmPalCombChnFlag[FTM_INSTANCE_COUNT][FEATURE_FTM_CHANNEL_COUNT];


/*FUNCTION**********************************************************************
 *
 * Function Name : PwmAllocateState
 * Description   : Allocates one of the available state structure.
 *
 *END**************************************************************************/
static uint8_t PwmAllocateState(bool* isAllocated, uint32_t* instanceMapping, const pwm_instance_t * const instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the FTM state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance->instIdx;
            isAllocated[i] = true;
            break;
        }
    }
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PwmFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void PwmFreeState(bool* isAllocated, const uint32_t* instanceMapping, const pwm_instance_t * const instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the FTM state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (instanceMapping[i] == instance->instIdx)
        {
            isAllocated[i] = false;
            break;
        }
    }
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : PWM_Init
 * Description   : Initialize PWM channels based on config parameter.
 *
 * Implements    : PWM_Init_Activity
 *END**************************************************************************/
status_t PWM_Init(const pwm_instance_t * const instance, const pwm_global_config_t* config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;

    #if (defined(PWM_OVER_FTM))
    if(instance->instType == PWM_INST_TYPE_FTM)
    {
        /* Declare internal variables */
        uint8_t channel;
        uint8_t channelID;
        uint8_t index;
        uint32_t frequency;
        uint32_t dutyPercent;
        DEV_ASSERT(instance->instIdx < FTM_INSTANCE_COUNT);
        /* Declare the configuration structure for FTM */
        ftm_pwm_param_t ftmPwmConfig;
        ftm_independent_ch_param_t pwmIndependentChannelConfig[FEATURE_FTM_CHANNEL_COUNT];
        ftm_combined_ch_param_t pwmCombinedChannelConfig[FEATURE_FTM_CHANNEL_COUNT >> 1U];
        ftm_user_config_t ftmGlobalConfig;
        /* Fault control feature is not supported, but this structure is filled with values which disable fault control */
        ftm_pwm_fault_param_t faultConfig =
        {
            false, /* Output pin state on fault */
            false, /* PWM fault interrupt state */
            0U, /* Fault filter value */
            FTM_FAULT_CONTROL_DISABLED,  /* Fault mode */
            {
                {
                    false, /* Fault channel state (Enabled/Disabled) */
                    false, /* Fault channel filter state (Enabled/Disabled) */
                    FTM_POLARITY_LOW, /* Fault channel state (Enabled/Disabled) */
                },
                {
                    false, /* Fault channel state (Enabled/Disabled) */
                    false, /* Fault channel filter state (Enabled/Disabled) */
                    FTM_POLARITY_LOW, /* Fault channel state (Enabled/Disabled) */
                },
                {
                    false, /* Fault channel state (Enabled/Disabled) */
                    false, /* Fault channel filter state (Enabled/Disabled) */
                    FTM_POLARITY_LOW, /* Fault channel state (Enabled/Disabled) */
                },
                {
                    false, /* Fault channel state (Enabled/Disabled) */
                    false, /* Fault channel filter state (Enabled/Disabled) */
                    FTM_POLARITY_LOW, /* Fault channel state (Enabled/Disabled) */
                }
            }
        };

        ftmPwmConfig.pwmCombinedChannelConfig = pwmCombinedChannelConfig;
        ftmPwmConfig.pwmIndependentChannelConfig = pwmIndependentChannelConfig;
        ftmPwmConfig.faultConfig = &faultConfig;

        /* Because FTM has only one timebase first channel is used to configure FTM clocking*/
        ftmGlobalConfig.ftmClockSource = ((pwm_ftm_timebase_t*)(config->pwmChannels[0].timebase))->sourceClock;
        ftmGlobalConfig.ftmPrescaler = ((pwm_ftm_timebase_t*)(config->pwmChannels[0].timebase))->prescaler;
        ftmPwmConfig.deadTimePrescaler = ((pwm_ftm_timebase_t*)(config->pwmChannels[0].timebase))->deadtimePrescaler;

        /* Configure FTM mode according to first channel setup.
        * All PWM channels must be PWM_EDGE_ALIGNED/PWM_Shifted or PWM_CENTER_ALIGNED */
        if ((config->pwmChannels[0].channelType) == PWM_CENTER_ALIGNED)
        {
            ftmPwmConfig.mode = FTM_MODE_CEN_ALIGNED_PWM;
        }
        else
        {
            ftmPwmConfig.mode = FTM_MODE_EDGE_ALIGNED_PWM;
        }
        ftmGlobalConfig.ftmMode =  ftmPwmConfig.mode;

        /* The synchronization for duty, period and phase shift will be update when
        * signal period is done. Only overwrite function shall take effect immediate.
        */
        ftmGlobalConfig.syncMethod.softwareSync = true;
        ftmGlobalConfig.syncMethod.hardwareSync1 = false;
        ftmGlobalConfig.syncMethod.hardwareSync2 = false;
        ftmGlobalConfig.syncMethod.hardwareSync0 = false;
        ftmGlobalConfig.syncMethod.autoClearTrigger = false;
        ftmGlobalConfig.syncMethod.maskRegSync = FTM_SYSTEM_CLOCK;
        ftmGlobalConfig.syncMethod.initCounterSync = FTM_PWM_SYNC;
        ftmGlobalConfig.syncMethod.inverterSync = FTM_PWM_SYNC;
        ftmGlobalConfig.syncMethod.outRegSync = FTM_SYSTEM_CLOCK;
        ftmGlobalConfig.syncMethod.maxLoadingPoint = true;
        ftmGlobalConfig.syncMethod.minLoadingPoint = false;
        ftmGlobalConfig.syncMethod.syncPoint = FTM_WAIT_LOADING_POINTS;
        ftmGlobalConfig.isTofIsrEnabled = false;
        ftmGlobalConfig.BDMMode = FTM_BDM_MODE_00;

        /* Configure FTM channels */
        ftmPwmConfig.nNumCombinedPwmChannels = 0;
        ftmPwmConfig.nNumIndependentPwmChannels = 0;

        /* Configure dead-time insertion. For them the dead time configuration is available for all channels. */
        ftmPwmConfig.deadTimePrescaler = ((pwm_ftm_timebase_t*)(config->pwmChannels[0].timebase))->deadtimePrescaler;
        ftmPwmConfig.deadTimeValue = config->pwmChannels[0].deadtime;

        for (channel = 0; channel < config->numberOfPwmChannels; channel++)
        {
            if (config->pwmChannels[channel].enableComplementaryChannel == false)
            {
                /* Configure duty cycle in percents */
                dutyPercent = (FTM_MAX_DUTY_CYCLE * config->pwmChannels[channel].duty) / config->pwmChannels[channel].period;
                pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].uDutyCyclePercent = (uint16_t)dutyPercent;

                /* Configure channel number */
                pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].hwChannelId = config->pwmChannels[channel].channel;

                /* Configure channel polarity */
                if (config->pwmChannels[channel].polarity == PWM_ACTIVE_HIGH)
                {
                    pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].polarity = FTM_POLARITY_HIGH;
                    pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].safeState = FTM_LOW_STATE;
                }
                else
                {
                    pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].polarity = FTM_POLARITY_LOW;
                    pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].safeState = FTM_LOW_STATE;
                }

                /* Configure default value for fail safe value. */
                pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].enableExternalTrigger = false;
                pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].deadTime = false;
                pwmIndependentChannelConfig[ftmPwmConfig.nNumIndependentPwmChannels].enableSecondChannelOutput = false;

                ftmPwmConfig.nNumIndependentPwmChannels++;
            }
            else
            {
                /* Configure duty cycle in percents */
                dutyPercent = (FTM_MAX_DUTY_CYCLE * config->pwmChannels[channel].duty) / config->pwmChannels[channel].period;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].firstEdge = 0;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].secondEdge = (uint16_t)dutyPercent;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].enableModifiedCombine = false;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].enableSecondChannelOutput = true;

                /* Configure channel number */
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].hwChannelId = config->pwmChannels[channel].channel;

                if (config->pwmChannels[channel].polarity == PWM_ACTIVE_HIGH)
                {
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].mainChannelPolarity = FTM_POLARITY_HIGH;
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].mainChannelSafeState = FTM_LOW_STATE;
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].secondChannelSafeState = FTM_LOW_STATE;

                }
                else
                {
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].mainChannelPolarity = FTM_POLARITY_LOW;
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].mainChannelSafeState = FTM_LOW_STATE;
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].secondChannelSafeState = FTM_LOW_STATE;
                }

                if (config->pwmChannels[channel].complementaryChannelPolarity == PWM_DUPLICATED)
                {
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].secondChannelPolarity = FTM_MAIN_DUPLICATED;
                }
                else
                {
                    pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].secondChannelPolarity = FTM_MAIN_INVERTED;
                }
                /* Configure default value for fail safe value. */
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].enableExternalTrigger = false;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].deadTime = config->pwmChannels[channel].insertDeadtime;
                pwmCombinedChannelConfig[ftmPwmConfig.nNumCombinedPwmChannels].enableExternalTriggerOnNextChn = false;
                ftmPwmConfig.nNumCombinedPwmChannels++;

                /* Set the flag as true when it is configured in combined channel */
                channelID = config->pwmChannels[channel].channel;
                pwmPalCombChnFlag[instance->instIdx][channelID] = true;
            }
        }
        /* Initialize FTM */
        index = PwmAllocateState(FtmStateIsAllocated, FtmStateInstanceMapping, instance, NO_OF_FTM_INSTS_FOR_PWM);
        /* No permit if all elements in PWM_PAL available resources table are chosen */
        DEV_ASSERT(index < NO_OF_FTM_INSTS_FOR_PWM);
        (void)FTM_DRV_Init(instance->instIdx, &ftmGlobalConfig, (ftm_state_t*)(&FtmState[index]));
        /* Configure PWM frequency */
        frequency = FTM_DRV_GetFrequency(instance->instIdx);
        ftmPwmConfig.uFrequencyHZ = frequency / config->pwmChannels[0].period;
        /* Start PWM signal generation */
        status = FTM_DRV_InitPwm(instance->instIdx, &ftmPwmConfig);
    }
    else
    #endif

    #if (defined(PWM_OVER_EMIOS))
    if (instance->instType == PWM_INST_TYPE_EMIOS)
    {
        /* Configure the default configuration for eMIOS */
        emios_common_param_t pwmGlobalConfig =
        {
            false, /*!< If true, all channel in eMIOS group can enter debug mode */
            false, /*!< Low power mode or normal mode */
            1U, /*!< Select the clock divider value for the global prescaler in range (1-256) */
            false, /*!< Enable or disable global prescaler */
            false /*!< Enable or disable global timebase */
        };

        /* Initialize global configuration of the EMIOS. */
        EMIOS_DRV_InitGlobal((uint8_t)instance->instIdx, &pwmGlobalConfig);

        /* Default timebase setup which will be changed accroding to config structure */
        emios_mc_mode_param_t pwmTimebaseConfig =
        {
            EMIOS_MODE_MCB_UP_COUNTER_EXT_CLK,
            10UL, /*!< Period value */
            EMIOS_CLOCK_DIVID_BY_1, /*!< Internal prescaler value */
            false, /*!< Enable internal prescaler */
            EMIOS_INPUT_FILTER_BYPASS, /*!< Filter value, ignore if not select external clock mode */
            false, /*!< Input capture filter state, ignore if not select external clock mode */
            EMIOS_TRIGGER_EDGE_FALLING /*!< Input signal trigger mode, ignore if not select external clock mode */
        };

        emios_pwm_param_t pwmChannelConfig=
        {
            EMIOS_MODE_OPWMCB_TRAIL_EDGE_DEADTIME_FLAGX1,
            EMIOS_CLOCK_DIVID_BY_1, /*!< Internal prescaler value */
            true, /*!< Internal prescaler is enabled */
            EMIOS_NEGATIVE_PULSE, /*!< Output active value, active low or high level */
            10000UL, /*!< Period count */
            5000UL, /*!< Duty cycle count */
            EMIOS_BUS_SEL_BCDE, /*!< Counter bus selected */
            1UL, /*!< Ideal duty cycle value using to compare with the selected time base */
            0UL, /*!< The dead time value and is compared against the internal counter */
            0UL /*!< Trigger Event placement */
        };
        /* Configure all timebases */
        uint8_t i;
        for (i = 0; i < config->numberOfPwmChannels; i++)
        {
            /* Configure timebase counter mode. */
            if(config->pwmChannels[i].channelType == PWM_EDGE_ALIGNED)
            {
                pwmTimebaseConfig.mode = EMIOS_MODE_MCB_UP_COUNTER_INT_CLK;
                pwmChannelConfig.mode = EMIOS_MODE_OPWMB_FLAGX1;
                /* Configure timebase period. */
                pwmTimebaseConfig.period = config->pwmChannels[i].period;
            }
            else
            {
                pwmTimebaseConfig.mode = EMIOS_MODE_MCB_UPDOWN_CNT_FLAGX1_INT_CLK;
                pwmChannelConfig.mode = EMIOS_MODE_OPWMCB_TRAIL_EDGE_DEADTIME_FLAGX1;
                /* Configure timebase period. */
                pwmTimebaseConfig.period = config->pwmChannels[i].period << 1U;
            }


            /* Configure timebase divider. */

            pwmTimebaseConfig.internalPrescalerEn = true;
            pwmTimebaseConfig.internalPrescaler = ((pwm_emios_timebase_t*)(config->pwmChannels[i].timebase))->internalPrescaler;

            /* Configure timebase */
            if(eMIOS[instance->instIdx]->UC[((pwm_emios_timebase_t*)(config->pwmChannels[i].timebase))->name].C == 0UL)
            {
                (void)EMIOS_DRV_MC_InitCounterMode((uint8_t)instance->instIdx, (uint8_t)((pwm_emios_timebase_t*)(config->pwmChannels[i].timebase))->name, &pwmTimebaseConfig);
            }
            /* Select timebase for current channel */
            if (((pwm_emios_timebase_t*)(config->pwmChannels[i].timebase))->name == BUS_A)
            {
                pwmChannelConfig.timebase = EMIOS_BUS_SEL_A;
            }
            else if (((pwm_emios_timebase_t*)(config->pwmChannels[i].timebase))->name == BUS_F)
            {
                pwmChannelConfig.timebase = EMIOS_BUS_SEL_F;
            }
            else
            {
                pwmChannelConfig.timebase = EMIOS_BUS_SEL_BCDE;
            }

            /* Select channel polarity */
            if (config->pwmChannels[i].polarity == PWM_ACTIVE_HIGH)
            {
                pwmChannelConfig.outputActiveMode = EMIOS_POSITIVE_PULSE;
            }
            else
            {
                pwmChannelConfig.outputActiveMode = EMIOS_NEGATIVE_PULSE;
            }

            /* Configure duty */
            pwmChannelConfig.idealDutyCycle = config->pwmChannels[i].duty;
            pwmChannelConfig.dutyCycleCount = config->pwmChannels[i].duty;

            /* Configure channel */
            (void)EMIOS_DRV_PWM_InitMode((uint8_t)(instance->instIdx), config->pwmChannels[i].channel, &pwmChannelConfig);
        }
        EMIOS_DRV_EnableGlobalEmios((uint8_t)(instance->instIdx));
        status = STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined (PWM_OVER_ETIMER))
    if (instance->instType == PWM_INST_TYPE_ETIMER)
    {
        /* intial sanity check */
        DEV_ASSERT(config->numberOfPwmChannels < ETIMER_CH_COUNT);

        /* default configuration for ETIMER for PWM mode */
        etimer_user_channel_config_t etimerPwmPalCfg = {
           .timerMode = ETIMER_MODE_VARIABLE_PWM,
           .coreSettings = ETIMER_COUNTER_NORMAL,
           .inputFilter =
           {
               .samples = ETIMER_FILT_CNT_3,
               .rate = 0,
           },
           .primaryInput =
           {
               .source = ETIMER_IN_SRC_CLK_DIV_32,
               .polarity = ETIMER_POLARITY_POSITIVE,
           },
           .secondaryInput =
           {
               .source = ETIMER_IN_SRC_CNT0_IN,
               .polarity = ETIMER_POLARITY_POSITIVE,
           },
           .outputPin =
           {
               .enable = true,
               .polarity = ETIMER_POLARITY_POSITIVE,
           },
           .countDirection = ETIMER_COUNT_UP,
           .compareValues =
           {
               32767,
               65535,
           },
           .compareLoading =
           {
               ETIMER_CLC_FROM_CMPLD1_WHEN_COMP2,
               ETIMER_CLC_FROM_CMPLD2_WHEN_COMP2,
           },
           .compareOutputControl = ETIMER_OUTMODE_SOFTWARE,
           .compareMode = ETIMER_CMPMODE_COMP1_UP_COMP2_UP,
           .captureControl =
           {
               ETIMER_CPTMODE_DISABLED,
               ETIMER_CPTMODE_DISABLED,
           },
           .captureWords = 0,
           .interruptEnableMask = 0,
        };

        /* initialize ETIMERx peripheral */
        ETIMER_DRV_Init(instance->instIdx);
        /* Configure all timebases */
        uint8_t i;
        uint16_t enmask=0;
        for (i = 0; i < config->numberOfPwmChannels; i++)
        {
            /* sanity checks */
            DEV_ASSERT(config->pwmChannels[i].channelType == PWM_EDGE_ALIGNED);
            DEV_ASSERT(config->pwmChannels[i].enableComplementaryChannel == false);
            DEV_ASSERT(config->pwmChannels[i].insertDeadtime == false);
            DEV_ASSERT(config->pwmChannels[i].period >= config->pwmChannels[i].duty);
            DEV_ASSERT(config->pwmChannels[i].duty <= 0xFFFFu);
            DEV_ASSERT((config->pwmChannels[i].period - config->pwmChannels[i].duty) <= 0xFFFFu);
            /* double frequency ? */
            if( (((pwm_etimer_timebase_t *)(config->pwmChannels[i].timebase))->halfClkPeriod) == true )
            {
                etimerPwmPalCfg.coreSettings = ETIMER_COUNTER_BOTHEDGE;
            }
            else
            {
                etimerPwmPalCfg.coreSettings = ETIMER_COUNTER_NORMAL;
            }
            /* clock source */
            etimerPwmPalCfg.primaryInput.polarity = ((pwm_etimer_timebase_t *)(config->pwmChannels[i].timebase))->pwmClkSrc.polarity;
            etimerPwmPalCfg.primaryInput.source = ((pwm_etimer_timebase_t *)(config->pwmChannels[i].timebase))->pwmClkSrc.source;
            /* enable output ? */
            if( (((pwm_etimer_timebase_t *)(config->pwmChannels[i].timebase))->enableOutSig) == true )
            {
                etimerPwmPalCfg.outputPin.enable = true;
            }
            else
            {
                etimerPwmPalCfg.outputPin.enable = false;
            }
            /* output polarity */
            /* eTimer ETIMER_POLARITY_POSITIVE has the first output the low period, then the high
             * it is opposite of the PWM PAL ACTIVE_HIGH and ACTIVE_LOW concepts
             */
            if(config->pwmChannels[i].polarity == PWM_ACTIVE_HIGH)
            {
                etimerPwmPalCfg.outputPin.polarity = ETIMER_POLARITY_NEGATIVE;
            }
            else
            {
                etimerPwmPalCfg.outputPin.polarity = ETIMER_POLARITY_POSITIVE;
            }
            /* COMP1 and COMP2 */
            uint16_t ton = config->pwmChannels[i].duty;
            if (ton == 0u)
            {
                /* duty 0% is not supported */
                ton = 1u;
            }
            else if (ton == config->pwmChannels[i].period)
            {
                /* duty 100% is not supported */
                ton = config->pwmChannels[i].period - 1u;
            }
            else
            {
                /* ton does not need any adjustment */
            }
            uint16_t toff = config->pwmChannels[i].period - ton;
            etimerPwmPalCfg.compareValues[0] = ton - 1u;
            etimerPwmPalCfg.compareValues[1] = toff - 1u;
            /* setup ETIMERx channels */
            ETIMER_DRV_InitChannel(instance->instIdx, config->pwmChannels[i].channel, &etimerPwmPalCfg);
            /* mask the enable bit */
            enmask|=(1UL<<(config->pwmChannels[i].channel));
        }
        /* start channel operation */
        ETIMER_DRV_StartTimerChannels(instance->instIdx, enmask);
        /* all ok */
        status = STATUS_SUCCESS;
    }
    else
    #endif
    {
        DEV_ASSERT(false);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PWM_UpdateDuty
 * Description   : Update duty cycle. The measurement unit for duty is clock ticks.
 * Implements    : PWM_UpdateDuty_Activity
 *
 *END**************************************************************************/
status_t PWM_UpdateDuty(const pwm_instance_t * const instance, uint8_t channel, uint32_t duty)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;

    #if (defined(PWM_OVER_FTM))
    if (instance->instType == PWM_INST_TYPE_FTM)
    {
        DEV_ASSERT(instance->instIdx < FTM_INSTANCE_COUNT);
        if (pwmPalCombChnFlag[instance->instIdx][channel] == true)
        {
            DEV_ASSERT(((uint32_t)channel % (uint32_t)2U) == (uint32_t)0U);
            status = FTM_DRV_UpdatePwmChannel(instance->instIdx, channel, FTM_PWM_UPDATE_IN_TICKS, 0, (uint16_t)duty, true);
        }
        else
        {
            status = FTM_DRV_UpdatePwmChannel(instance->instIdx, channel, FTM_PWM_UPDATE_IN_TICKS, (uint16_t)duty, 0, true);
        }
    }
    else
    #endif

    #if (defined(PWM_OVER_EMIOS))
    if (instance->instType == PWM_INST_TYPE_EMIOS)
    {
        if ((eMIOS[instance->instIdx]->UC[channel].C & eMIOS_C_MODE_MASK) == (uint32_t)EMIOS_MODE_OPWMB_FLAGX1)
        {
            eMIOS[instance->instIdx]->UC[channel].B = duty;
        }
        else
        {
            eMIOS[instance->instIdx]->UC[channel].A = duty;
        }
        status = STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined (PWM_OVER_ETIMER))
    if (instance->instType == PWM_INST_TYPE_ETIMER)
    {
        /* read current compare values */
        uint16_t comp1;
        uint16_t comp2;
        ETIMER_DRV_GetCompareThresholdBuffered(instance->instIdx, channel, &comp1, &comp2);

        /* the duty cycle must be lower or equal the period */
        uint16_t ton = comp1 + 1u;
        uint16_t toff = comp2 + 1u;
        uint16_t period = ton + toff;
        if(duty <= period)
        {
            if (duty == 0u)
            {
                ton = 1u;
            }
            else if (period == duty)
            {
                ton = period - 1u;
            }
            else
            {
                ton = duty;
            }
            toff = period - ton;
            comp1 = ton - 1u;
            comp2 = toff - 1u;
            /* set it */
            ETIMER_DRV_SetCompareThresholdBuffered(instance->instIdx, channel, comp1, comp2);
            /* all ok */
            status = STATUS_SUCCESS;
        }
        else
        {
            /* duty is higher than period */
            DEV_ASSERT(false);
        }
    }
    else
    #endif
    {
        DEV_ASSERT(false);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PWM_UpdatePeriod
 * Description   : Update period for specific a specific channel. This function changes period for
 * all channels which shares the timebase with targeted channel.
 * Implements    : PWM_UpdatePeriod_Activity
 *
 *END**************************************************************************/
status_t PWM_UpdatePeriod(const pwm_instance_t * const instance, uint8_t channel, uint32_t period)
{

    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;

    #if (defined(PWM_OVER_FTM))
    if (instance->instType == PWM_INST_TYPE_FTM)
    {
        (void)FTM_DRV_UpdatePwmPeriod(instance->instIdx, FTM_PWM_UPDATE_IN_TICKS, (uint16_t)period, true);
        (void)channel;
        status = STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined(PWM_OVER_EMIOS))
    if (instance->instType == PWM_INST_TYPE_EMIOS)
    {
        switch (((eMIOS[instance->instIdx]->UC[channel].C) & eMIOS_C_BSL_MASK) >> eMIOS_C_BSL_SHIFT)
        {
            case 0U:
                eMIOS[instance->instIdx]->UC[BUS_A].A = period;
                break;
            case 1U:
                if(channel <=7U)
                {
                    eMIOS[instance->instIdx]->UC[BUS_B].A = period;
                }
                else if (channel <= 15U)
                {
                    eMIOS[instance->instIdx]->UC[BUS_C].A = period;
                }
                else if (channel <= 23U)
                {
                    eMIOS[instance->instIdx]->UC[BUS_D].A = period;
                }
                else if (channel <= 31U)
                {
                    eMIOS[instance->instIdx]->UC[BUS_E].A = period;
                }
                else
                {
                    /* Noting to do */
                }
                break;
            case 2U:
                eMIOS[instance->instIdx]->UC[BUS_F].A = period;
                break;
            default: /* Noting to do */
                break;
        }
        status = STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined (PWM_OVER_ETIMER))
    if (instance->instType == PWM_INST_TYPE_ETIMER)
    {
        /* read current compare values */
        uint16_t comp1;
        ETIMER_DRV_GetCompareThresholdBuffered(instance->instIdx, channel, &comp1, NULL);
        uint16_t ton = comp1 + 1u;
        /* the period must be higher than the duty (Ton)
         * because the eTimer cannot generate a 100% duty signal
         */
        if(period > ton)
        {
            /* set it */
            DEV_ASSERT((period - ton) <= 0xFFFFu); /* check if the Toff length fits in 16 bits */
            uint16_t toff = period - ton;
            uint16_t comp2 = toff - 1u;
            /* set it */
            ETIMER_DRV_SetCompareThresholdBuffered(instance->instIdx, channel, comp1, comp2);
            /* all ok */
            status = STATUS_SUCCESS;
        }
        else
        {
            /* duty is higher than period */
            DEV_ASSERT(false);
        }
    }
    else
    #endif
    {
        DEV_ASSERT(false);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PWM_OverwriteOutputChannels
 * Description   : This function change the output value for some channels. channelsMask select
 * which channels will be overwrite, each bit filed representing one channel: 1 - channel is controlled
 * by channelsValues, 0 - channel is controlled by pwm. channelsValues select output values to be write on corresponding
 * channel.
 * For PWM_PAL over FTM, when enable complementary channels, if this function is used to force output of complementary channels(n and n+1)
 * with value is high, the output of channel n is going to be high and the output of channel n+1 is going to be low.
 * Please refer to Software ouput control behavior table in the reference manual to get more details.
 * Implements    : PWM_OverwriteOutputChannels_Activity
 *
 *END**************************************************************************/
status_t PWM_OverwriteOutputChannels(const pwm_instance_t * const instance, uint32_t channelsMask, uint32_t channelsValues)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;

    #if (defined(PWM_OVER_FTM))
    if (instance->instType == PWM_INST_TYPE_FTM)
    {
        (void)FTM_DRV_SetAllChnSoftwareOutputControl(instance->instIdx, (uint8_t)channelsMask, (uint8_t)channelsValues, true);
        status =  STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined(PWM_OVER_EMIOS))
    if (instance->instType == PWM_INST_TYPE_EMIOS)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    #endif

    #if (defined (PWM_OVER_ETIMER))
    if (instance->instType == PWM_INST_TYPE_ETIMER)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    #endif
    {
        DEV_ASSERT(false);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PWM_Deinit
 * Description   : Uninitialised PWM instance.
 * Implements    : PWM_Deinit_Activity
 *
 *END**************************************************************************/
status_t PWM_Deinit(const pwm_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;

    #if (defined(PWM_OVER_FTM))
    if (instance->instType == PWM_INST_TYPE_FTM)
    {
        DEV_ASSERT(instance->instIdx < FTM_INSTANCE_COUNT);
        (void)FTM_DRV_DeinitPwm(instance->instIdx);
        (void)FTM_DRV_Deinit(instance->instIdx);
        PwmFreeState(FtmStateIsAllocated, FtmStateInstanceMapping, instance, NO_OF_FTM_INSTS_FOR_PWM);
        uint8_t index;
        for (index = 0U; index < FEATURE_FTM_CHANNEL_COUNT; index++)
        {
            pwmPalCombChnFlag[instance->instIdx][index] = false;
        }

        status = STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined(PWM_OVER_EMIOS))
    if (instance->instType == PWM_INST_TYPE_EMIOS)
    {
        uint8_t index;
        for (index = 0U; index < 32U; index++)
        {
            /* Disable channels in the output compare over EMIOS */
            EMIOS_DRV_DeInitChannel((uint8_t)instance->instIdx,
                                  index);
        }
        status =  STATUS_SUCCESS;
    }
    else
    #endif

    #if (defined (PWM_OVER_ETIMER))
    if (instance->instType == PWM_INST_TYPE_ETIMER)
    {
        /* deinitialize ETIMERx peripheral */
        ETIMER_DRV_Deinit(instance->instIdx);
        status =  STATUS_SUCCESS;
    }
    else
    #endif

    {
        DEV_ASSERT(false);
    }

    return status;
}
