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
 * @file ic_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type
 * This is needed for the conversion between generic IC types to driver types.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially enum<i>'
 * to 'essentially enum<i>'
 * This is needed for the conversion between generic IC types to the driver types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 *(different essential type categories).
 * This is required by the conversion of unsigned long integer into unsigned integer.
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
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 */

#include "ic_pal.h"
#include "interrupt_manager.h"
#include "ic_irq.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Define state structures for IC PAL */
static ic_pal_state_t g_icPalStatePtr[NUMBER_OF_IC_PAL_INSTANCES];

/* IC state-instance matching */
static uint32_t icStateInstanceMapping[NUMBER_OF_IC_PAL_INSTANCES];
/* IC available resources array */
static bool icStateIsAllocated[NUMBER_OF_IC_PAL_INSTANCES];
/* The type of instance in the IC array */
static ic_inst_type_t icStateInstType[NUMBER_OF_IC_PAL_INSTANCES];

#if (defined(IC_PAL_OVER_FTM))
/* The FTM state structures */
static ftm_state_t g_ftmState[NUMBER_OF_IC_PAL_INSTANCES];
/* The filter clock prescaler is divided by 4 */
#define FTM_FILTER_PRESCALER_DIVIDE_BY_4    (3U)
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : icAllocateState
 * Description   : Allocates one of the available state structure.
 *
 *END**************************************************************************/
static uint8_t icAllocateState(bool * isAllocated,
                               uint32_t * instanceMapping,
                               const ic_instance_t * const instance)
{
    uint8_t i;

    /* Allocate one of the IC state structure for this instance */
    for (i = 0U;i < NUMBER_OF_IC_PAL_INSTANCES;i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance->instIdx;
            isAllocated[i] = true;
            icStateInstType[i] = instance->instType;
            break;
        }
    }

    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : icFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void icFreeState(bool * isAllocated,
                        const uint32_t * instanceMapping,
                        const ic_instance_t * const instance)
{
    uint8_t i;

    /* Allocate one of the IC state structure for this instance */
    for (i = 0U;i < NUMBER_OF_IC_PAL_INSTANCES;i++)
    {
        if ((instanceMapping[i] == instance->instIdx) && (icStateInstType[i] == instance->instType))
        {
            isAllocated[i] = false;
            break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FindIcState
 * Description   : Search the state structure of the instance for each IP
 *
 *END**************************************************************************/
static uint8_t FindIcState(const ic_instance_t * const instance)
{
    uint8_t i;

    for (i = 0U;i < NUMBER_OF_IC_PAL_INSTANCES;i++)
    {
        if ((icStateInstanceMapping[i] == instance->instIdx) && (icStateInstType[i] == instance->instType))
        {
            break;
        }
    }

    return i;
}

#if (defined(IC_PAL_OVER_FTM))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_FtmInit
 * Description   : This function initializes IC_PAL over FTM.
 *
 *END**************************************************************************/
static status_t IC_FtmInit(uint32_t instance,
                           const ic_config_t * configPtr)
{
    DEV_ASSERT(instance < IC_PAL_INSTANCE_MAX);
    ic_pal_state_t * icState;
    ic_instance_t instancePtr;
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;
    /* Table of base addresses for FTM instances. */
    static FTM_Type * const s_ftmBase[FTM_INSTANCE_COUNT] = FTM_BASE_PTRS;
    FTM_Type * base;
    base = s_ftmBase[instance];

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_FTM;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    uint8_t channel = 0U;
    ftm_pwm_sync_t sync;
    ftm_user_config_t info;
    ftm_state_t * ftmState = &g_ftmState[index];
    ftm_input_param_t param;
    ftm_input_ch_param_t channelConfig[FEATURE_FTM_CHANNEL_COUNT];

    /* Configure the synchronous to default */
    sync.softwareSync = true;
    sync.hardwareSync0 = false;
    sync.hardwareSync1 = false;
    sync.hardwareSync2 = false;
    sync.maxLoadingPoint = true;
    sync.minLoadingPoint = false;
    sync.inverterSync = FTM_SYSTEM_CLOCK;
    sync.outRegSync = FTM_SYSTEM_CLOCK;
    sync.maskRegSync = FTM_SYSTEM_CLOCK;
    sync.initCounterSync = FTM_SYSTEM_CLOCK;
    sync.autoClearTrigger = false;
    sync.syncPoint = FTM_UPDATE_NOW;

    /* Get some information from user configuration */
    info.syncMethod = sync;
    info.ftmMode = FTM_MODE_INPUT_CAPTURE;
    info.ftmPrescaler = ((extension_ftm_for_ic_t *)(configPtr->extension))->ftmPrescaler;
    info.ftmClockSource = ((extension_ftm_for_ic_t *)(configPtr->extension))->ftmClockSource;
    info.BDMMode = FTM_BDM_MODE_11;
    info.isTofIsrEnabled = false;
    info.enableInitializationTrigger = false;

    /* Initializes the FTM driver */
    status = FTM_DRV_Init(instance,
                          &info,
                          ftmState);
    DEV_ASSERT(STATUS_SUCCESS == status);

    param.nNumChannels = configPtr->nNumChannels;
    param.nMaxCountValue = MAX_COUNTER_VALUE;

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        /* Get the hardware channel ID */
        channelConfig[index].hwChannelId = configPtr->inputChConfig[index].hwChannelId;
        channel = channelConfig[index].hwChannelId;

        /* Check the type of measurement */
        switch (configPtr->inputChConfig[index].inputCaptureMode)
        {
            case IC_TIMESTAMP_RISING_EDGE:
                channelConfig[index].edgeAlignement = FTM_RISING_EDGE;
                break;
            case IC_TIMESTAMP_FALLING_EDGE:
                channelConfig[index].edgeAlignement = FTM_FALLING_EDGE;
                break;
            case IC_TIMESTAMP_BOTH_EDGES:
                channelConfig[index].edgeAlignement = FTM_BOTH_EDGES;
                break;
            case IC_MEASURE_RISING_EDGE_PERIOD:
                channelConfig[index].measurementType = FTM_RISING_EDGE_PERIOD_MEASUREMENT;
                break;
            case IC_MEASURE_FALLING_EDGE_PERIOD:
                channelConfig[index].measurementType = FTM_FALLING_EDGE_PERIOD_MEASUREMENT;
                break;
            case IC_MEASURE_PULSE_HIGH:
                channelConfig[index].measurementType = FTM_PERIOD_ON_MEASUREMENT;
                break;
            case IC_MEASURE_PULSE_LOW:
                channelConfig[index].measurementType = FTM_PERIOD_OFF_MEASUREMENT;
                break;
            default:
                /* Do nothing */
                break;
        }

        /* Check the input capture operation mode */
        if ((configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_RISING_EDGE) || \
            (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_FALLING_EDGE) || \
            (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_BOTH_EDGES))
        {
            channelConfig[index].inputMode = FTM_EDGE_DETECT;
            channelConfig[index].measurementType = FTM_NO_MEASUREMENT;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_DISABLE_OPERATION)
        {
            channelConfig[index].inputMode = FTM_NO_OPERATION;
            channelConfig[index].edgeAlignement = FTM_NO_PIN_CONTROL;
            channelConfig[index].measurementType = FTM_NO_MEASUREMENT;
        }
        else
        {
            /* Check the channel ID need to even number in the measurement mode */
            DEV_ASSERT((channel % 2U) == 0U);
            channelConfig[index].inputMode = FTM_SIGNAL_MEASUREMENT;
            channelConfig[index].edgeAlignement = FTM_NO_PIN_CONTROL;
        }

        /* Set channels configuration from user */
        channelConfig[index].filterEn = configPtr->inputChConfig[index].filterEn;
        channelConfig[index].filterValue = configPtr->inputChConfig[index].filterValue;
        channelConfig[index].continuousModeEn = ((channel_extension_ftm_for_ic_t *)(configPtr->inputChConfig[index].channelExtension))->continuousModeEn;
        channelConfig[index].channelsCallbacksParams = configPtr->inputChConfig[index].channelCallbackParams;
        channelConfig[index].channelsCallbacks = configPtr->inputChConfig[index].channelCallbacks;

        if ((channelConfig[index].filterEn == true) && (channel < CHAN4_IDX))
        {
            /* Only sets the FTM filter clock is divided by 4 */
            FTM_DRV_SetClockFilterPs(base, FTM_FILTER_PRESCALER_DIVIDE_BY_4);
        }

        /* Store some needed information into state structure */
        icState->channelConfigArray[index] = configPtr->inputChConfig[index].hwChannelId;
        icState->icChannelType[channel] = configPtr->inputChConfig[index].inputCaptureMode;
        icState->enableContinuousMode[channel] = channelConfig[index].continuousModeEn;
        icState->enableNotification[channel] = true;
        icState->channelCallbackParams[channel] = channelConfig[index].channelsCallbacksParams;
        icState->channelCallbacks[channel] = channelConfig[index].channelsCallbacks;
    }

    param.inputChConfig = channelConfig;
    /* Configure channels in input capture mode */
    status = FTM_DRV_InitInputCapture(instance,
                                      &param);

    return status;
}
#endif

#if (defined(IC_PAL_OVER_EMIOS))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_EmiosInit
 * Description   : This function initializes IC_PAL over EMIOS.
 *
 *END**************************************************************************/
static status_t IC_EmiosInit(uint32_t instance,
                             const ic_config_t * configPtr)
{
    DEV_ASSERT(instance < IC_PAL_INSTANCE_MAX);
    ic_pal_state_t * icState;
    ic_instance_t instancePtr;
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;
    static const IRQn_Type emiosIrq[eMIOS_INSTANCE_COUNT][(eMIOS_UC_COUNT / 2U)] = eMIOS_IRQS;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_EMIOS;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    emios_common_param_t commonParam;
    uint8_t channel = 0U;
    uint8_t optionMode = 0U;
    emios_bus_select_t timeBase = EMIOS_BUS_SEL_A;
    emios_mc_mode_param_t mcParam;
    emios_input_capture_param_t icParam;
    emios_gpio_mode_param_t gpioParam;

    /* Get common parameters from user */
    commonParam.allowDebugMode = false;
    commonParam.lowPowerMode = false;
    commonParam.clkDivVal = ((extension_emios_for_ic_t *)(configPtr->extension))->clkDivVal;
    commonParam.enableGlobalPrescaler = ((extension_emios_for_ic_t *)(configPtr->extension))->enableGlobalPrescaler;
    commonParam.enableGlobalTimeBase = ((extension_emios_for_ic_t *)(configPtr->extension))->enableGlobalTimeBase;

    /* Initialize the global for a eMIOS group */
    EMIOS_DRV_InitGlobal((uint8_t)instance,
                          &commonParam);

    /* Get the information from user configuration */
    mcParam.mode = EMIOS_MODE_MCB_UP_COUNTER_INT_CLK;
    mcParam.period = MAX_COUNTER_VALUE;
    mcParam.filterInput = EMIOS_INPUT_FILTER_BYPASS;
    mcParam.filterEn = false;
    mcParam.triggerMode = EMIOS_TRIGGER_EDGE_ANY;

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        mcParam.internalPrescaler = (emios_clock_internal_ps_t)((channel_extension_emios_for_ic_t *)(configPtr->inputChConfig[index].channelExtension))->prescaler;
        mcParam.internalPrescalerEn = true;

        switch (((channel_extension_emios_for_ic_t *)(configPtr->inputChConfig[index].channelExtension))->timebase)
        {
            case IC_BUS_SEL_A:
                /* Set channel to use as a time base */
                channel = 23U;
                timeBase = EMIOS_BUS_SEL_A;
                break;
            case IC_BUS_SEL_B:
                /* Set channel to use as a time base */
                channel = 0U;
                timeBase = EMIOS_BUS_SEL_BCDE;
                break;
            case IC_BUS_SEL_C:
                /* Set channel to use as a time base */
                channel = 8U;
                timeBase = EMIOS_BUS_SEL_BCDE;
                break;
            case IC_BUS_SEL_D:
                /* Set channel to use as a time base */
                channel = 16U;
                timeBase = EMIOS_BUS_SEL_BCDE;
                break;
            case IC_BUS_SEL_E:
                /* Set channel to use as a time base */
                channel = 24U;
                timeBase = EMIOS_BUS_SEL_BCDE;
                break;
            case IC_BUS_SEL_F:
                /* Set channel to use as a time base */
                channel = 22U;
                timeBase = EMIOS_BUS_SEL_F;
                break;
            case IC_BUS_SEL_INTERNAL:
                /* Set channel to use as a time base */
                channel = configPtr->inputChConfig[index].hwChannelId;
                timeBase = EMIOS_BUS_SEL_INTERNAL;
                break;
            default:
                /* Do nothing */
                break;
        }

        /* Initialize the counter mode */
        status = EMIOS_DRV_MC_InitCounterMode((uint8_t)instance,
                                              channel,
                                              &mcParam);

        channel = configPtr->inputChConfig[index].hwChannelId;
        icParam.mode = EMIOS_MODE_IC;
        icParam.timebase = timeBase;
        icParam.filterInput = (emios_input_filter_t)(configPtr->inputChConfig[index].filterValue);
        icParam.filterEn = configPtr->inputChConfig[index].filterEn;

        if (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_RISING_EDGE)
        {
            icParam.inputCaptureMode = EMIOS_CAPTURE_TRIGGER_EDGE_RISING;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_FALLING_EDGE)
        {
            icParam.inputCaptureMode = EMIOS_CAPTURE_TRIGGER_EDGE_FALLING;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_DISABLE_OPERATION)
        {
            /* Set default configure for an input pin */
            gpioParam.mode = EMIOS_MODE_GPIO_INPUT;
            gpioParam.filterEn = false;
            gpioParam.filterInput = EMIOS_INPUT_FILTER_BYPASS;
            gpioParam.triggerMode = EMIOS_TRIGGER_EDGE_ANY;

            /* Disable operation on the channel input */
            EMIOS_DRV_InitGpioMode((uint8_t)instance,
                                   channel,
                                   &gpioParam);
        }
        else
        {
            optionMode = (uint8_t)configPtr->inputChConfig[index].inputCaptureMode - 1U;
            icParam.inputCaptureMode = (emios_input_capture_mode_t)optionMode;
        }

        if (configPtr->inputChConfig[index].inputCaptureMode != IC_DISABLE_OPERATION)
        {
            /* Initialize the input capture mode for each channel */
            status = EMIOS_DRV_IC_InitInputCaptureMode((uint8_t)instance,
                                                       channel,
                                                       &icParam);
        }

        EMIOS_DRV_ChannelEnableInterruptRequest((uint8_t)instance, channel);
        /* Enable EMIOS interrupt */
        INT_SYS_EnableIRQ(emiosIrq[instance][(channel>>1U)]);

        /* Store some needed information into state structure */
        icState->channelConfigArray[index] = channel;
        icState->icChannelType[channel] = configPtr->inputChConfig[index].inputCaptureMode;
        icState->timeBaseSelection[channel] = icParam.timebase;
        icState->filterEn[channel] = icParam.filterEn;
        icState->filterInput[channel] = icParam.filterInput;
        icState->enableNotification[channel] = true;
        icState->channelCallbackParams[channel] = configPtr->inputChConfig[index].channelCallbackParams;
        icState->channelCallbacks[channel] = configPtr->inputChConfig[index].channelCallbacks;
    }

    /* Enable the global eMIOS to start counter */
    EMIOS_DRV_EnableGlobalEmios((uint8_t)instance);

    return status;
}
#endif

#if (defined(IC_PAL_OVER_ETIMER))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_EtimerInit
 * Description   : This function initializes the input capture mode over ETIMER.
 *
 *END**************************************************************************/
static status_t IC_EtimerInit(uint32_t instance,
                              const ic_config_t * configPtr)
{
    DEV_ASSERT(instance < IC_PAL_ETIMER_INSTANCE_COUNT);
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;
    uint8_t i = 0U;
    uint16_t channel = 0U;
    uint16_t maskChannel = 0U;
    etimer_cptmode_t capCtr[ETIMER_NUM_COMPARE_REGISTERS];
    ic_pal_state_t * icState;
    ic_instance_t instancePtr;
    static const IRQn_Type etimerIrqId[ETIMER_INSTANCE_COUNT][ETIMER_CH_MAX_IRQS] = ETIMER_IRQS;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_ETIMER;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    etimer_user_channel_config_t userChannelConfig;

    /* Initialize the ETIMER module */
    ETIMER_DRV_Init((uint16_t)instance);

    userChannelConfig.coreSettings = ETIMER_COUNTER_NORMAL;
    userChannelConfig.outputPin.enable = false;
    userChannelConfig.outputPin.polarity = ETIMER_POLARITY_POSITIVE;
    userChannelConfig.countDirection = ETIMER_COUNT_UP;
    userChannelConfig.compareOutputControl = ETIMER_OUTMODE_SOFTWARE;
    userChannelConfig.compareMode = ETIMER_CMPMODE_COMP1_UP_COMP2_UP;
    userChannelConfig.primaryInput.polarity = ETIMER_POLARITY_POSITIVE;
    userChannelConfig.secondaryInput.polarity = ETIMER_POLARITY_POSITIVE;

    for (index = 0U; index < ETIMER_NUM_COMPARE_REGISTERS; index++)
    {
        userChannelConfig.compareValues[index] = 0U;
        userChannelConfig.compareLoading[index] = ETIMER_CLC_NEVER;
    }

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = configPtr->inputChConfig[index].hwChannelId;
        maskChannel |= (uint16_t)(1U << channel);

        if (((channel_extension_etimer_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->continuousModeEn == true)
        {
            userChannelConfig.timerMode = ETIMER_MODE_INPUT_CAPTURE;
        }
        else
        {
            userChannelConfig.timerMode = ETIMER_MODE_INPUT_CAPTURE_ONESHOT;
        }

        if (configPtr->inputChConfig[index].filterEn)
        {
            userChannelConfig.inputFilter.samples = (etimer_filt_cnt_t)configPtr->inputChConfig[index].filterValue;
            userChannelConfig.inputFilter.rate = ((channel_extension_etimer_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->rate;
        }

        userChannelConfig.primaryInput.source = (etimer_input_source_t)(((channel_extension_etimer_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->prescaler);;
        userChannelConfig.secondaryInput.source = (etimer_input_source_t)channel;
        capCtr[0] = ETIMER_CPTMODE_DISABLED;
        capCtr[1] = ETIMER_CPTMODE_DISABLED;
        userChannelConfig.captureWords = 1U;
        userChannelConfig.interruptEnableMask = ETIMER_CH_IRQ_SOURCE_ICF1IE;

        /* Check the type of measurement */
        if (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_RISING_EDGE)
        {
            capCtr[0] = ETIMER_CPTMODE_RISING;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_FALLING_EDGE)
        {
            capCtr[0] = ETIMER_CPTMODE_FALLING;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_TIMESTAMP_BOTH_EDGES)
        {
            capCtr[0] = ETIMER_CPTMODE_BOTH;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_RISING_EDGE_PERIOD)
        {
            capCtr[0] = ETIMER_CPTMODE_RISING;
            userChannelConfig.captureWords = 2U;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_FALLING_EDGE_PERIOD)
        {
            capCtr[0] = ETIMER_CPTMODE_FALLING;
            userChannelConfig.captureWords = 2U;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_PULSE_HIGH)
        {
            capCtr[0] = ETIMER_CPTMODE_RISING;
            capCtr[1] = ETIMER_CPTMODE_FALLING;
            userChannelConfig.interruptEnableMask = ETIMER_CH_IRQ_SOURCE_ICF2IE;
        }
        else if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_PULSE_LOW)
        {
            capCtr[0] = ETIMER_CPTMODE_FALLING;
            capCtr[1] = ETIMER_CPTMODE_RISING;
            userChannelConfig.interruptEnableMask = ETIMER_CH_IRQ_SOURCE_ICF2IE;
        }
        else
        {
            /* Nothing to do */
        }

        if (((channel_extension_etimer_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->continuousModeEn == false)
        {
            userChannelConfig.captureWords = 0U;
            if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_RISING_EDGE_PERIOD)
            {
                capCtr[0] = ETIMER_CPTMODE_RISING;
                capCtr[1] = ETIMER_CPTMODE_RISING;
                userChannelConfig.interruptEnableMask = ETIMER_CH_IRQ_SOURCE_ICF2IE;
            }
            else if (configPtr->inputChConfig[index].inputCaptureMode == IC_MEASURE_FALLING_EDGE_PERIOD)
            {
                capCtr[0] = ETIMER_CPTMODE_FALLING;
                capCtr[1] = ETIMER_CPTMODE_FALLING;
                userChannelConfig.interruptEnableMask = ETIMER_CH_IRQ_SOURCE_ICF2IE;
            }
            else
            {
                /* Nothing to do */
            }
        }

        for (i = 0U; i < ETIMER_NUM_COMPARE_REGISTERS; i++)
        {
            userChannelConfig.captureControl[i] = capCtr[i];
        }

        /* Initialize the channel at the input capture mode */
        ETIMER_DRV_InitChannel((uint16_t)instance, channel, &userChannelConfig);
        /* Enable the input capture */
        ETIMER_DRV_StartCapture((uint16_t)instance, channel);
        /* Enable interrupt on channel */
        INT_SYS_EnableIRQ(etimerIrqId[instance][channel]);

        /* Store some needed information into state structure */
        icState->channelConfigArray[index] = channel;
        icState->icChannelType[channel] = configPtr->inputChConfig[index].inputCaptureMode;
        icState->enableNotification[channel] = true;
        icState->channelCallbackParams[channel] = configPtr->inputChConfig[index].channelCallbackParams;
        icState->channelCallbacks[channel] = configPtr->inputChConfig[index].channelCallbacks;
        icState->enableContinuousMode[channel] = ((channel_extension_etimer_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->continuousModeEn;
    }

    /* Start the channel counting */
    ETIMER_DRV_StartTimerChannels((uint16_t)instance, maskChannel);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Etimer_SetChannelMode
 * Description   : This function will change the mode operation in the input capture.
 *
 *END**************************************************************************/
static void IC_Etimer_SetChannelMode(uint32_t instance,
                                     uint8_t channel,
                                     ic_option_mode_t channelMode)
{
    DEV_ASSERT(instance < ETIMER_INSTANCE_COUNT);
    DEV_ASSERT(channel < ETIMER_CH_COUNT);
    /* Table of base addresses for ETIMER instances */
    static ETIMER_Type * const s_etimerBase[ETIMER_INSTANCE_COUNT] = ETIMER_BASE_PTRS;
    ETIMER_Type * base;
    base = s_etimerBase[instance];
    uint8_t captureWords = 1U;
    etimer_cptmode_t captureControl[2];
    captureControl[0] = ETIMER_CPTMODE_DISABLED;
    captureControl[1] = ETIMER_CPTMODE_DISABLED;
    base->CH[channel].INTDMA &= ~(ETIMER_CH_IRQ_FLAGS_ICF1 | ETIMER_CH_IRQ_FLAGS_ICF2);

    switch (channelMode)
    {
        case IC_TIMESTAMP_RISING_EDGE:
            captureControl[0] = ETIMER_CPTMODE_RISING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF1;
            break;
        case IC_TIMESTAMP_FALLING_EDGE:
            captureControl[0] = ETIMER_CPTMODE_FALLING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF1;
            break;
        case IC_TIMESTAMP_BOTH_EDGES:
            captureControl[0] = ETIMER_CPTMODE_BOTH;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF1;
            break;
        case IC_MEASURE_RISING_EDGE_PERIOD:
            captureControl[0] = ETIMER_CPTMODE_RISING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF1;
            captureWords = 2U;
            break;
        case IC_MEASURE_FALLING_EDGE_PERIOD:
            captureControl[0] = ETIMER_CPTMODE_FALLING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF1;
            captureWords = 2U;
            break;
        case IC_MEASURE_PULSE_HIGH:
            captureControl[0] = ETIMER_CPTMODE_RISING;
            captureControl[1] = ETIMER_CPTMODE_FALLING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF2;
            break;
        case IC_MEASURE_PULSE_LOW:
            captureControl[0] = ETIMER_CPTMODE_FALLING;
            captureControl[1] = ETIMER_CPTMODE_RISING;
            base->CH[channel].INTDMA |= ETIMER_CH_IRQ_FLAGS_ICF2;
            break;
        default:
            /* nothing to do */
            break;
    }

    /* Capture counter value using external inputs */
    base->CH[channel].CCCTRL &= ((~ETIMER_CCCTRL_CFWM_MASK) & (~ETIMER_CCCTRL_CPT1MODE_MASK) & (~ETIMER_CCCTRL_CPT2MODE_MASK));
    base->CH[channel].CCCTRL |= ETIMER_CCCTRL_CFWM(captureWords) | ETIMER_CCCTRL_CPT1MODE(captureControl[0]) | ETIMER_CCCTRL_CPT2MODE(captureControl[1]);
}
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_FlexPwmInit
 * Description   : This function initializes the input capture mode over FlexPWM.
 *
 *END**************************************************************************/
static status_t IC_FlexPwmInit(uint32_t instance,
                              const ic_config_t * configPtr)
{
    DEV_ASSERT(instance < IC_PAL_FLEXPWM_INSTANCE_COUNT);
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;
    uint16_t channel = 0U;
    uint16_t subModuleMask = 0U;
    uint16_t interruptMask = 0U;
    ic_pal_state_t * icState;
    ic_instance_t instancePtr;
    flexpwm_module_t subModule;
    flexpwm_input_capture_config_t inputCapture;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_FLEXPWM;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    /* Set default configuration */
    flexpwm_module_setup_t moduleSetupParams = {
        .cntrInitSel = InitSrcLocalSync,
        .clkSrc = ClkSrcPwmPeriphClk,
        .prescaler = PwmDividedBy1,
        .clockFreq = 0U,
        .chnlPairOper = FlexPwmComplementary,
        .complementarySourceSel = FlexPwmComplementarySource23,
        .reloadLogic = FlexPwmReloadImmediate,
        .reloadFreq = PwmLoadEvery1Oportunity,
        .forceTrig = ForceOutputLocalForce
    };

    flexpwm_module_signal_setup_t signalParams = {
        .pwmPeriod = 0U,
        .flexpwmType = FlexPwmCentreAligned,
        .pwmAPulseWidth = 0U,
        .pwmBPulseWidth = 0U,
        .pwmAShift = 0U,
        .pwmBShift = 0U,
        .pwmAOuten = false,
        .pwmBOuten = false,
        .pwmXOuten = false,
        .pwmAPolarity = false,
        .pwmBPolarity = false,
        .pwmXPolarity = false,
        .isPWMAFaultProtectEnabled = false,
        .isPWMBFaultProtectEnabled = false,
        .isPWMXFaultProtectEnabled = false
    };

    inputCapture.inputSelect = FLEXPWM_RAW_PWM_INPUT;
    inputCapture.edgeCompareVal = 0U;
    inputCapture.edgeCntEn = FLEXPWM_CAPTURE_EDGE_CNT_DISABLED;
    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = configPtr->inputChConfig[index].hwChannelId;
        subModuleMask |= (uint16_t)(1U << channel);
        subModule = (flexpwm_module_t)channel;

        moduleSetupParams.prescaler = ((channel_extension_flexpwm_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->prescaler;
        /* Initialize the channel at the output compare mode */
        FLEXPWM_DRV_SetupPwm(instance,
                             subModule,
                             &moduleSetupParams,
                             &signalParams);

        inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_EDGE_DISABLED;
        inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_EDGE_DISABLED;
        inputCapture.watermark = 1U;
        interruptMask = (uint16_t)FLEXPWM_CAPTURE_X0_INT_ENABLE;

        if (((channel_extension_flexpwm_for_ic_t *)configPtr->inputChConfig[index].channelExtension)->continuousModeEn == true)
        {
            inputCapture.oneShot = FLEXPWM_CAPTURE_FREE_RUNNIG;
        }
        else
        {
            inputCapture.oneShot = FLEXPWM_CAPTURE_ONE_SHOT;
        }

        /* Check the type of measurement */
        switch (configPtr->inputChConfig[index].inputCaptureMode)
        {
            case IC_TIMESTAMP_RISING_EDGE:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
            }
                break;
            case IC_TIMESTAMP_FALLING_EDGE:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
            }
                break;
            case IC_TIMESTAMP_BOTH_EDGES:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_EITHER_EDGE;
            }
                break;
            case IC_MEASURE_RISING_EDGE_PERIOD:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
                inputCapture.watermark = 2U;
            }
                break;
            case IC_MEASURE_FALLING_EDGE_PERIOD:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
                inputCapture.watermark = 2U;
            }
                break;
            case IC_MEASURE_PULSE_HIGH:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
                inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_FALLING_EDGE;
                interruptMask |= (uint16_t)FLEXPWM_CAPTURE_X1_INT_ENABLE;
            }
                break;
            case IC_MEASURE_PULSE_LOW:
            {
                inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
                inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_RISING_EDGE;
                interruptMask |= (uint16_t)FLEXPWM_CAPTURE_X1_INT_ENABLE;
            }
                break;
            default:
                /* Nothing to do */
                break;
        }

        /* Configures the input capture circuit */
        FLEXPWM_DRV_ConfigureInputCapture(instance,
                                          subModule,
                                          &inputCapture);

        /* Starts the input capture event */
        FLEXPWM_DRV_StartInputCapture(instance,
                                      subModule);

        /* Enable interrupt on channel */
        FLEXPWM_DRV_EnableModuleInterrupts(instance,
                                           subModule,
                                           interruptMask);

        /* Store some needed information into state structure */
        icState->channelConfigArray[index] = channel;
        icState->icChannelType[channel] = configPtr->inputChConfig[index].inputCaptureMode;
        icState->enableNotification[channel] = true;
        icState->channelCallbackParams[channel] = configPtr->inputChConfig[index].channelCallbackParams;
        icState->channelCallbacks[channel] = configPtr->inputChConfig[index].channelCallbacks;
    }

    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = (configPtr->inputChConfig[index].hwChannelId);
        subModule = (flexpwm_module_t)channel;
        /* Start the PWM sub-module counters */
        FLEXPWM_DRV_CounterStart(instance, subModule);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_FlexPwm_SetChannelMode
 * Description   : This function will change the mode operation in the input capture.
 *
 *END**************************************************************************/
static void IC_FlexPwm_SetChannelMode(uint32_t instance,
                                      uint8_t channel,
                                      ic_option_mode_t channelMode)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);
    DEV_ASSERT(channel < FlexPWM_SUB_COUNT);
    uint16_t interruptMask = (uint16_t)FLEXPWM_CAPTURE_X0_INT_ENABLE;
    flexpwm_input_capture_config_t inputCapture;

    inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_EDGE_DISABLED;
    inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_EDGE_DISABLED;
    inputCapture.watermark = 1U;
    FLEXPWM_DRV_DisableModuleInterrupts(instance,
                                        (flexpwm_module_t)channel,
                                        (interruptMask | (uint16_t)FLEXPWM_CAPTURE_X1_INT_ENABLE));

    switch (channelMode)
    {
        case IC_TIMESTAMP_RISING_EDGE:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
            break;
        case IC_TIMESTAMP_FALLING_EDGE:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
            break;
        case IC_TIMESTAMP_BOTH_EDGES:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_EITHER_EDGE;
            break;
        case IC_MEASURE_RISING_EDGE_PERIOD:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
            inputCapture.watermark = 2U;
            break;
        case IC_MEASURE_FALLING_EDGE_PERIOD:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
            inputCapture.watermark = 2U;
            break;
        case IC_MEASURE_PULSE_HIGH:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_RISING_EDGE;
            inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_FALLING_EDGE;
            interruptMask |= (uint16_t)FLEXPWM_CAPTURE_X1_INT_ENABLE;
            break;
        case IC_MEASURE_PULSE_LOW:
            inputCapture.edgeSelect0 = FLEXPWM_CAPTURE_FALLING_EDGE;
            inputCapture.edgeSelect1 = FLEXPWM_CAPTURE_RISING_EDGE;
            interruptMask |= (uint16_t)FLEXPWM_CAPTURE_X1_INT_ENABLE;
            break;
        default:
            /* nothing to do */
            break;
    }

    /* Configures the input capture circuit */
    FLEXPWM_DRV_ConfigureInputCapture(instance,
                                      (flexpwm_module_t)channel,
                                      &inputCapture);

    /* Enable interrupt on channel */
    FLEXPWM_DRV_EnableModuleInterrupts(instance,
                                       (flexpwm_module_t)channel,
                                       interruptMask);
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Init
 * Description   : This function will initialize the IC PAL instance, including the
 * other platform specific HW units used together in the input capture mode. This
 * function configures a group of channels in instance to detect or measure the
 * input signal.
 *
 * Implements    : IC_Init_Activity
 *END**************************************************************************/
status_t IC_Init(const ic_instance_t * const instance,
                 const ic_config_t * configPtr)
{
    DEV_ASSERT(configPtr != NULL);
    DEV_ASSERT(instance != NULL);
    ic_pal_state_t * icState;
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;

    /* Allocate one of the IC state structure for this instance */
    index = icAllocateState(icStateIsAllocated, icStateInstanceMapping, instance);
    icState = &g_icPalStatePtr[index];
    DEV_ASSERT(icState->nNumChannels == 0x0U);

    /* Initialize the internal context to default value */
    icState->nNumChannels = configPtr->nNumChannels;
    for (index = 0U; index < IC_PAL_NUM_OF_CHANNEL_MAX; index++)
    {
        icState->channelConfigArray[index] = 0U;
        icState->icChannelType[index] = IC_DISABLE_OPERATION;
        icState->enableNotification[index] = false;
        icState->channelCallbackParams[index] = NULL;
        icState->channelCallbacks[index] = NULL;
        #if (defined(IC_PAL_OVER_FTM) || defined(IC_PAL_OVER_ETIMER))
        icState->enableContinuousMode[index] = false;
        #endif
        #if (defined(IC_PAL_OVER_ETIMER) || defined(IC_PAL_OVER_FLEXPWM))
        icState->measurementResults[index] = 0U;
        #endif
    }

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    status = IC_FtmInit(instance->instIdx, configPtr);
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    status = IC_EmiosInit(instance->instIdx, configPtr);
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        status = IC_EtimerInit(instance->instIdx, configPtr);
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        status = IC_FlexPwmInit(instance->instIdx, configPtr);
    }
    else
#endif
    {
        /* Nothing to do */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Deinit
 * Description   : This function will disable the input capture mode. The driver
 * can't be used again until reinitialized. The context structure is no longer
 * needed by the driver and can be freed after calling this function.
 *
 * Implements    : IC_Deinit_Activity
 *END**************************************************************************/
status_t IC_Deinit(const ic_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);
    uint8_t index = 0U;
    ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    index = FindIcState(instance);
    icState = &g_icPalStatePtr[index];

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    /* Disable the input capture over FTM */
    (void)FTM_DRV_Deinit(instance->instIdx);
    ftm_state_t * ftmState = &g_ftmState[index];
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    uint8_t channel;

    for (index = 0U; index < icState->nNumChannels; index++)
    {
        channel = icState->channelConfigArray[index];
        /* Disable channels in the input capture over EMIOS */
        EMIOS_DRV_DeInitChannel((uint8_t)instance->instIdx,
                                channel);
    }

    /* Disable the global of EMIOS */
    EMIOS_DRV_DisableGlobalEmios((uint8_t)instance->instIdx);
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_ETIMER);
    uint8_t channel;
    uint16_t captureRegister = 0U;
    uint16_t captureResultBuffer[2] = {0U, 0U};
    uint8_t captureWordsToRead = 1U;
    uint16_t returnCode = 0U;

    for (index = 0U; index < icState->nNumChannels; index++)
    {
        channel = icState->channelConfigArray[index];
        for (captureRegister = 0U; captureRegister < 2U; captureRegister++)
        {
            /* Returns how many captured words are stored in the hardware FIFO */
            captureWordsToRead = (uint8_t)ETIMER_DRV_GetCaptureWords((uint16_t)instance->instIdx,
                                                                     (uint16_t)channel,
                                                                     captureRegister);
            if (captureWordsToRead > 0U)
            {
                /* Reads the values are already stored in the hardware FIFO */
                returnCode = ETIMER_DRV_GetCaptureValue((uint16_t)instance->instIdx,
                                                        (uint16_t)channel,
                                                        captureRegister,
                                                        captureResultBuffer,
                                                        captureWordsToRead);
                (void)returnCode;
            }
        }
    }

    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        /* Disable the input capture over ETIMER */
        ETIMER_DRV_Deinit((uint16_t)instance->instIdx);
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        /* Disable the input capture over FLEXPWM */
        FLEXPWM_DRV_Deinit(instance->instIdx);
    }
    else
#endif
    {
        /* Nothing to do */
    }

    /* De-Initialize the internal context to default value */
    icState->nNumChannels = 0U;
    for (index = 0U; index < IC_PAL_NUM_OF_CHANNEL_MAX; index++)
    {
        icState->channelConfigArray[index] = 0U;
        icState->icChannelType[index] = IC_DISABLE_OPERATION;
        icState->enableNotification[index] = false;
        #if (defined(IC_PAL_OVER_FTM))
        icState->enableContinuousMode[index] = false;
        ftmState->enableNotification[index] = false;
        #endif
        #if (defined(IC_PAL_OVER_ETIMER) || defined(IC_PAL_OVER_FLEXPWM))
        icState->measurementResults[index] = 0U;
        icState->enableContinuousMode[index] = false;
        #endif
    }

    /* De-Allocate the instance which is not used */
    icFreeState(icStateIsAllocated, icStateInstanceMapping, instance);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_StartChannel
 * Description   : This function start channel counting.
 *
 * Implements    : IC_StartChannel_Activity
 *END**************************************************************************/
void IC_StartChannel(const ic_instance_t * const instance,
                     uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    uint8_t index = 0U;
    const ic_pal_state_t * icState;
    status_t status = STATUS_SUCCESS;

    /* Allocate one of the IC state structure for this instance */
    index = FindIcState(instance);
    icState = &g_icPalStatePtr[index];

    /* Re-start the channel mode */
    status = IC_SetChannelMode(instance,
                               channel,
                               icState->icChannelType[channel]);
    (void)status;
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    /* Enable the channel clock */
    EMIOS_DRV_ChannelEnableClk((uint8_t)instance->instIdx,
                               channel);
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
        ETIMER_DRV_StartCapture((uint16_t)instance->instIdx,
                                (uint16_t)channel);
        ETIMER_DRV_StartTimerChannels((uint16_t)instance->instIdx,
                                      (uint16_t)(1U << channel));
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
        /* Start the input capture */
        FLEXPWM_DRV_StartInputCapture(instance->instIdx,
                                      (flexpwm_module_t)channel);
        /* Start the channel counting */
        FLEXPWM_DRV_CounterStart(instance->instIdx,
                                 (flexpwm_module_t)channel);
    }
    else
#endif
    {
        /* Nothing to do */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_StopChannel
 * Description   : This function will stop channel counting.
 *
 * Implements    : IC_StopChannel_Activity
 *END**************************************************************************/
void IC_StopChannel(const ic_instance_t * const instance,
                    uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    /* Disable pin not used for FTM */
    (void)FTM_DRV_SetOutputlevel(instance->instIdx,
                                 channel,
                                 0x0U);
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    /* Disable individual channel by stopping its respective clock*/
    EMIOS_DRV_ChannelDisableClk((uint8_t)instance->instIdx,
                                channel);
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
        ETIMER_DRV_StopCapture((uint16_t)instance->instIdx,
                               (uint16_t)channel);
        ETIMER_DRV_StopTimerChannels((uint16_t)instance->instIdx,
                                     (uint16_t)(1U << channel));
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
        /* Stop the input capture */
        FLEXPWM_DRV_StopInputCapture(instance->instIdx,
                                     (flexpwm_module_t)channel);
        /* Stop the channel counting */
        FLEXPWM_DRV_CounterStop(instance->instIdx,
                                (flexpwm_module_t)channel);
    }
    else
#endif
    {
        /* Nothing to do */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_SetChannelMode
 * Description   : This function is used to change the channel mode at run time or
 * when stopping channel. The channel mode is selected in the ic_option_mode_t
 * enumeration type.
 *
 * Implements    : IC_SetChannelMode_Activity
 *END**************************************************************************/
status_t IC_SetChannelMode(const ic_instance_t * const instance,
                           uint8_t channel,
                           ic_option_mode_t channelMode)
{
    DEV_ASSERT(instance != NULL);
    uint8_t index = 0U;
    ic_pal_state_t * icState;
    status_t status = STATUS_SUCCESS;

    /* Allocate one of the IC state structure for this instance */
    index = FindIcState(instance);
    icState = &g_icPalStatePtr[index];

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    bool contModeEnable;
    index = (uint8_t)channelMode;
    ftm_ic_op_mode_t inputMode = (ftm_ic_op_mode_t)index;

    if (true == icState->enableContinuousMode[channel])
    {
        contModeEnable = true;
    }
    else
    {
        contModeEnable = false;
    }

    /* Set operation mode for channel input */
    status = FTM_IC_DRV_SetChannelMode(instance->instIdx,
                                       channel,
                                       inputMode,
                                       contModeEnable);
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    emios_input_capture_param_t icParam;
    emios_gpio_mode_param_t gpioParam;
    uint8_t optionMode = 0U;

    if (channelMode == IC_DISABLE_OPERATION)
    {
        /* Set default configure for an input pin */
        gpioParam.mode = EMIOS_MODE_GPIO_INPUT;
        gpioParam.filterEn = false;
        gpioParam.filterInput = EMIOS_INPUT_FILTER_BYPASS;
        gpioParam.triggerMode = EMIOS_TRIGGER_EDGE_ANY;

        /* Disable operation on the channel input */
        EMIOS_DRV_InitGpioMode((uint8_t)instance->instIdx,
                               channel,
                               &gpioParam);
    }
    else
    {
        icParam.mode = EMIOS_MODE_IC;
        icParam.timebase = icState->timeBaseSelection[channel];
        icParam.filterInput = icState->filterInput[channel];
        icParam.filterEn = icState->filterEn[channel];

        if (channelMode == IC_TIMESTAMP_RISING_EDGE)
        {
            icParam.inputCaptureMode = EMIOS_CAPTURE_TRIGGER_EDGE_RISING;
        }
        else if (channelMode == IC_TIMESTAMP_FALLING_EDGE)
        {
            icParam.inputCaptureMode = EMIOS_CAPTURE_TRIGGER_EDGE_FALLING;
        }
        else
        {
            optionMode = (uint8_t)(channelMode) - 1U;
            icParam.inputCaptureMode = (emios_input_capture_mode_t)optionMode;
        }

        /* Initialize the input capture mode for each channel */
        status = EMIOS_DRV_IC_InitInputCaptureMode((uint8_t)instance->instIdx,
                                                   channel,
                                                   &icParam);
    }
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
        IC_Etimer_SetChannelMode(instance->instIdx,
                                 channel,
                                 channelMode);
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
        IC_FlexPwm_SetChannelMode(instance->instIdx,
                                  channel,
                                  channelMode);
    }
    else
#endif
    {
        /* Nothing to do */
    }

    /* Update the channel mode */
    icState->icChannelType[channel] = channelMode;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_GetMeasurement
 * Description   : This function will get the value of measured signal in ticks.
 *
 * Implements    : IC_GetMeasurement_Activity
 *END**************************************************************************/
uint16_t IC_GetMeasurement(const ic_instance_t * const instance,
                           uint8_t channel)
{
    DEV_ASSERT(instance != NULL);
    uint16_t value = 0U;

#if (defined(IC_PAL_OVER_FTM))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_FTM);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    /* Get the measured value over the FTM */
    value = FTM_DRV_GetInputCaptureMeasurement(instance->instIdx,
                                               channel);
#endif

#if (defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instType == IC_INST_TYPE_EMIOS);
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    status_t status = STATUS_SUCCESS;
    uint32_t retValue = 0U;

    /* Get the measured value over the EMIOS */
    status = EMIOS_DRV_IC_GetLastMeasurement((uint8_t)instance->instIdx,
                                             channel,
                                             &retValue);
    (void) status;
    value = (uint16_t)retValue;
#endif

#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
        uint8_t index = 0U;
        const ic_pal_state_t * icState;

        /* Allocate one of the IC state structure for this instance */
        index = FindIcState(instance);
        icState = &g_icPalStatePtr[index];
        /* Get the measured value */
        value = icState->measurementResults[channel];
    }
    else
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
        uint8_t index = 0U;
        const ic_pal_state_t * icState;

        /* Allocate one of the IC state structure for this instance */
        index = FindIcState(instance);
        icState = &g_icPalStatePtr[index];
        /* Get the measured value */
        value = icState->measurementResults[channel];
    }
    else
#endif
    {
        /* Nothing to do */
    }

    return value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_EnableNotification
 * Description   : This function enables channel notification.
 *
 * Implements    : IC_EnableNotification_Activity
 *END**************************************************************************/
void IC_EnableNotification(const ic_instance_t * const instance,
                           uint8_t channel)
{
    DEV_ASSERT(instance != NULL);
#if (defined(IC_PAL_OVER_FTM) || defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
#endif
#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
    }
    else
#endif
#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
    }
    else
#endif
    {
        /* Nothing to do */
    }

    uint8_t index = 0U;
    ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    index = FindIcState(instance);
    icState = &g_icPalStatePtr[index];

    /* Enable notification */
    icState->enableNotification[channel] = true;

#if (defined(IC_PAL_OVER_FTM))
    ftm_state_t * ftmState = ftmStatePtr[instance->instIdx];
    ftmState->enableNotification[channel] = true;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : IC_DisableNotification
 * Description   : This function disables channel notification.
 *
 * Implements    : IC_DisableNotification_Activity
 *END**************************************************************************/
void IC_DisableNotification(const ic_instance_t * const instance,
                            uint8_t channel)
{
    DEV_ASSERT(instance != NULL);
#if (defined(IC_PAL_OVER_FTM) || defined(IC_PAL_OVER_EMIOS))
    DEV_ASSERT(instance->instIdx < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
#endif
#if (defined(IC_PAL_OVER_ETIMER))
    if (instance->instType == IC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_ETIMER_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
    }
    else
#endif
#if (defined(IC_PAL_OVER_FLEXPWM))
    if (instance->instType == IC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < IC_PAL_FLEXPWM_INSTANCE_COUNT);
        DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
    }
    else
#endif
    {
        /* Nothing to do */
    }
    uint8_t index = 0U;
    ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    index = FindIcState(instance);
    icState = &g_icPalStatePtr[index];

    /* Disable notification */
    icState->enableNotification[channel] = false;

#if (defined(IC_PAL_OVER_FTM))
    ftm_state_t * ftmState = ftmStatePtr[instance->instIdx];
    ftmState->enableNotification[channel] = false;
#endif
}

#if (defined (IC_PAL_OVER_EMIOS))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Emios_IrqHandler
 * Description   : Interrupt handler for IC PAL over EMIOS.
 * This is not a public API as it is called by IRQ whenever an interrupt occurs.
 *
 *END**************************************************************************/
void IC_Emios_IrqHandler(uint32_t instance, uint8_t channel)
{
    DEV_ASSERT(instance < IC_PAL_INSTANCE_MAX);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX);
    uint8_t index = 0U;
    ic_instance_t instancePtr;
    const ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_EMIOS;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    if ((icState->channelCallbacks[channel] != NULL) && (icState->enableNotification[channel] == true))
    {
        /* Call to callback function */
        (icState->channelCallbacks[channel])(IC_EVENT_MEASUREMENT_COMPLETE, icState->channelCallbackParams[channel]);
    }


    /* Clear interrupt flag */
    EMIOS_DRV_ClrFlagState(instance, channel);
}
#endif

#if (defined (IC_PAL_OVER_ETIMER))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Etimer_IrqHandler
 * Description   : Interrupt handler for IC PAL over ETIMER.
 * This is not a public API as it is called by IRQ whenever an interrupt occurs.
 *
 *END**************************************************************************/
void IC_Etimer_IrqHandler(uint32_t instance, uint8_t channel)
{
    DEV_ASSERT(instance < IC_PAL_ETIMER_INSTANCE_COUNT);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER);
    uint8_t index = 0U;
    ic_instance_t instancePtr;
    ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_ETIMER;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    uint16_t value = 0U;
    uint16_t captureRegister = 0U;
    uint16_t captureResultBuffer1[2] = {0U, 0U};
    uint16_t captureResultBuffer2[2] = {0U, 0U};
    uint8_t captureWordsToRead = 1U;
    uint16_t returnCode = 0U;

    /* Returns how many captured words are stored in the hardware FIFO */
    captureWordsToRead = (uint8_t)ETIMER_DRV_GetCaptureWords((uint16_t)instance,
                                                             (uint16_t)channel,
                                                             captureRegister);
    /* Reads the values are already stored in the hardware FIFO */
    returnCode = ETIMER_DRV_GetCaptureValue((uint16_t)instance,
                                            (uint16_t)channel,
                                            captureRegister,
                                            captureResultBuffer1,
                                            captureWordsToRead);

    if (returnCode == 0U)
    {
        value = 0U;
    }
    else if (returnCode == 1U)
    {
        value = captureResultBuffer1[0];
    }
    else
    {
        if (captureResultBuffer1[1] >= captureResultBuffer1[0])
        {
            value = captureResultBuffer1[1] - captureResultBuffer1[0];
        }
        else
        {
            value = captureResultBuffer1[1] + (ETIMER_CH_MAX_TIMER_COUNT - captureResultBuffer1[0]);
        }
    }

    if ((icState->icChannelType[channel] == IC_MEASURE_PULSE_HIGH) || \
        (icState->icChannelType[channel] == IC_MEASURE_PULSE_LOW) || \
        ((icState->enableContinuousMode[channel] == false) && ((icState->icChannelType[channel] == IC_MEASURE_RISING_EDGE_PERIOD) || (icState->icChannelType[channel] == IC_MEASURE_FALLING_EDGE_PERIOD))))
    {
        captureRegister = 1U;
        /* Returns how many captured words are stored in the hardware FIFO */
        captureWordsToRead = (uint8_t)ETIMER_DRV_GetCaptureWords((uint16_t)instance,
                                                                 (uint16_t)channel,
                                                                 captureRegister);
        /* Reads the values are already stored in the hardware FIFO */
        returnCode = ETIMER_DRV_GetCaptureValue((uint16_t)instance,
                                                (uint16_t)channel,
                                                captureRegister,
                                                captureResultBuffer2,
                                                captureWordsToRead);

        if (captureResultBuffer2[0] >= captureResultBuffer1[0])
        {
            value = captureResultBuffer2[0] - captureResultBuffer1[0];
        }
        else
        {
            value = captureResultBuffer2[0] + (ETIMER_CH_MAX_TIMER_COUNT - captureResultBuffer1[0]);
        }
    }

    if (icState->icChannelType[channel] == IC_DISABLE_OPERATION)
    {
        value = 0U;
    }

    (void)returnCode;

    /* Get the time stamp of the event */
    icState->measurementResults[channel] = value;

    if ((icState->channelCallbacks[channel] != NULL) && (icState->enableNotification[channel] == true))
    {
        /* Call to callback function */
        (icState->channelCallbacks[channel])(IC_EVENT_MEASUREMENT_COMPLETE, icState->channelCallbackParams[channel]);
    }

    /* Clear interrupt flag */
    ETIMER_DRV_ClearInterruptStatus((uint16_t)instance,
                                    (ETIMER_CH_IRQ_FLAGS_ICF1 | ETIMER_CH_IRQ_FLAGS_ICF2),
                                    channel);
}
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
/*FUNCTION**********************************************************************
 *
 * Function Name : IC_Flexpwm_IrqHandler
 * Description   : Interrupt handler for IC PAL over FLEXPWM.
 * This is not a public API as it is called by IRQ whenever an interrupt occurs.
 *
 *END**************************************************************************/
void IC_Flexpwm_IrqHandler(uint32_t instance, uint8_t channel)
{
    DEV_ASSERT(instance < IC_PAL_FLEXPWM_INSTANCE_COUNT);
    DEV_ASSERT(channel < IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM);
    uint8_t index = 0U;
    ic_instance_t instancePtr;
    ic_pal_state_t * icState;

    /* Allocate one of the IC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = IC_INST_TYPE_FLEXPWM;
    index = FindIcState(&instancePtr);
    icState = &g_icPalStatePtr[index];

    uint16_t value = 0U;
    uint16_t captureResultBuffer[2] = {0U, 0U};

    switch (icState->icChannelType[channel])
    {
        case IC_TIMESTAMP_RISING_EDGE:
        case IC_TIMESTAMP_FALLING_EDGE:
        case IC_TIMESTAMP_BOTH_EDGES:
            value = FLEXPWM_DRV_GetCaptureValue(instance, (flexpwm_module_t)channel, FLEXPWM_INPUT_CAPTURE_COUNTER_0);
            break;
        case IC_MEASURE_RISING_EDGE_PERIOD:
        case IC_MEASURE_FALLING_EDGE_PERIOD:
            captureResultBuffer[0] = FLEXPWM_DRV_GetCaptureValue(instance, (flexpwm_module_t)channel, FLEXPWM_INPUT_CAPTURE_COUNTER_0);
            captureResultBuffer[1] = FLEXPWM_DRV_GetCaptureValue(instance, (flexpwm_module_t)channel, FLEXPWM_INPUT_CAPTURE_COUNTER_0);
            if (captureResultBuffer[1] >= captureResultBuffer[0])
            {
                value = captureResultBuffer[1] - captureResultBuffer[0];
            }
            else
            {
                value = captureResultBuffer[0] + (MAX_COUNTER_VALUE - captureResultBuffer[1]);
            }
            break;
        case IC_MEASURE_PULSE_HIGH:
        case IC_MEASURE_PULSE_LOW:
            captureResultBuffer[0] = FLEXPWM_DRV_GetCaptureValue(instance, (flexpwm_module_t)channel, FLEXPWM_INPUT_CAPTURE_COUNTER_0);
            captureResultBuffer[1] = FLEXPWM_DRV_GetCaptureValue(instance, (flexpwm_module_t)channel, FLEXPWM_INPUT_CAPTURE_COUNTER_1);
            if (captureResultBuffer[1] >= captureResultBuffer[0])
            {
                value = captureResultBuffer[1] - captureResultBuffer[0];
            }
            else
            {
                value = captureResultBuffer[0] + (MAX_COUNTER_VALUE - captureResultBuffer[1]);
            }
            break;
        default:
            /* nothing to do */
            break;
    }

    /* Get the time stamp of the event */
    icState->measurementResults[channel] = value;

    if ((icState->channelCallbacks[channel] != NULL) && (icState->enableNotification[channel] == true))
    {
        /* Call to callback function */
        (icState->channelCallbacks[channel])(IC_EVENT_MEASUREMENT_COMPLETE, icState->channelCallbackParams[channel]);
    }

    /* Clear interrupt flag */
    FLEXPWM_DRV_ClearInterruptFlag(instance,
                                   (flexpwm_module_t)channel,
                                   (FLEXPWM_CFX0_INT_FLAG | FLEXPWM_CFX1_INT_FLAG));
}
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
