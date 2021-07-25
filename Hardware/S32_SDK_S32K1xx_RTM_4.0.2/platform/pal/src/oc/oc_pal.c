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
 * @file oc_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
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
 * Violates MISRA 2012 Advisory Rule 8.9, could define variable
 * at block scope.
 * Those variables are state structures, required to be global.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'ocState' could be declared as pointing to const.
 * This is a pointer to the driver context structure which is for internal use only, and the application
 * must make no assumption about the content of this structure. Therefore it is irrelevant for the application
 * whether the structure is changed in the function or not. The fact that in a particular implementation of some
 * functions there is no write in the context structure is an implementation detail and there is no reason to
 * propagate it in the interface. That would compromise the stability of the interface, if this implementation
 * detail is changed in later releases or on other platforms.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially Boolean' type to 'essentially unsigned'.This is required by the
 * conversion of a bit-field of a bool type into a bit-field of a register type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 *(different essential type categories).
 * This is required by the conversion of unsigned long integer into unsigned integer.

 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
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
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 */

#include "oc_pal.h"
#include "oc_pal_cfg.h"
#include "interrupt_manager.h"
#include "oc_irq.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief The internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the OC_Init() function, then
 * it cannot be freed until the driver is de-initialized using OC_Deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint8_t nNumChannels;                                      /*!< Number of output compare channel used */
    uint8_t channelConfigArray[OC_PAL_CHANNEL_MAX_NUM];     /*!< Store the hardware channel IDs are used output compare mode */
    oc_option_mode_t ocChannelMode[OC_PAL_CHANNEL_MAX_NUM]; /*!< Output compare mode of operation */
    void * channelCallbackParams[OC_PAL_CHANNEL_MAX_NUM];   /*!< To save the parameter of callback application */
    oc_callback_t channelCallbacks[OC_PAL_CHANNEL_MAX_NUM]; /*!< To save the callback function */
    bool enableNotification[OC_PAL_CHANNEL_MAX_NUM];        /*!< To save channels enable the notification on the callback application */
/*! @endcond */
} oc_pal_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Pointer to runtime state structures */
static oc_pal_state_t g_ocPalStatePtr[NUMBER_OF_OC_PAL_INSTANCES];

/* OC state-instance matching */
static uint32_t ocStateInstanceMapping[NUMBER_OF_OC_PAL_INSTANCES];
/* OC  available resources table */
static bool ocStateIsAllocated[NUMBER_OF_OC_PAL_INSTANCES];
/* The type of instance in the OC array */
static oc_inst_type_t ocStateInstType[NUMBER_OF_OC_PAL_INSTANCES];

#if (defined(OC_PAL_OVER_FTM))
/* The FTM state structures */
static ftm_state_t g_ocFtmState[NUMBER_OF_OC_PAL_INSTANCES];
#elif (defined(OC_PAL_OVER_EMIOS))
static void OC_EmiosWaitOutputOff(uint32_t instance, uint8_t channel);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ocAllocateState
 * Description   : Allocates one of the available state structure.
 *
 *END**************************************************************************/
static uint8_t ocAllocateState(bool * isAllocated,
                               uint32_t * instanceMapping,
                               const oc_instance_t * const instance,
                               uint8_t numberOfInstances)
{
    uint8_t i;

    /* Allocate one of the OC state structure for this instance */
    for (i = 0U; i < numberOfInstances; i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance->instIdx;
            isAllocated[i] = true;
            ocStateInstType[i] = instance->instType;
            break;
        }
    }

    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ocFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void ocFreeState(bool * isAllocated,
                        uint32_t * instanceMapping,
                        const oc_instance_t * const instance,
                        uint8_t numberOfInstances)
{
    uint8_t i;

    /* Allocate one of the OC state structure for this instance */
    for (i = 0U; i < numberOfInstances; i++)
    {
        if ((instanceMapping[i] == instance->instIdx) && (ocStateInstType[i] == instance->instType))
        {
            isAllocated[i] = false;
            break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FindOcState
 * Description   : Search the state structure of the instance for each IP
 *
 *END**************************************************************************/
static uint8_t FindOcState(const oc_instance_t * const instance)
{
    uint8_t i;

    for (i = 0U; i < NUMBER_OF_OC_PAL_INSTANCES; i++)
    {
        if ((ocStateInstanceMapping[i] == instance->instIdx) && (ocStateInstType[i] == instance->instType))
        {
            break;
        }
    }

    return i;
}

 #if (defined(OC_PAL_OVER_FTM))
/*FUNCTION**********************************************************************
 *
 * Function Name : OC_InitFtm
 * Description   : This function initializes output compare over FTM.
 *END**************************************************************************/
 static status_t OC_InitFtm(uint32_t instance,
                            const oc_config_t * configPtr)
{
    status_t status = STATUS_SUCCESS;
    static oc_pal_state_t * ocState;
    oc_instance_t instancePtr;
    uint8_t indexInstance = 0U;
    uint8_t index = 0U;
    uint8_t channel = 0U;

    /* Allocate one of the OC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = OC_INST_TYPE_FTM;
    indexInstance = ocAllocateState(ocStateIsAllocated, ocStateInstanceMapping, &instancePtr, NUMBER_OF_OC_PAL_INSTANCES);
    ocState = &g_ocPalStatePtr[indexInstance];
    DEV_ASSERT(ocState->nNumChannels == 0x0U);

    /* Initialize the internal context to default value */
    ocState->nNumChannels = configPtr->nNumChannels;
    for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM; index++)
    {
        ocState->channelConfigArray[index] = 0U;
        ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
        ocState->enableNotification[index] = false;
        ocState->channelCallbackParams[index] = NULL;
        ocState->channelCallbacks[index] = NULL;
    }

    ftm_pwm_sync_t sync;
    ftm_user_config_t info;
    ftm_state_t * ftmState = &g_ocFtmState[indexInstance];
    ftm_output_cmp_param_t param;
    ftm_output_cmp_ch_param_t channelConfig[FEATURE_FTM_CHANNEL_COUNT];
    static const IRQn_Type ftmIrq[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT] = FTM_IRQS;

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
    info.ftmMode = FTM_MODE_OUTPUT_COMPARE;
    info.ftmPrescaler = ((extension_ftm_for_oc_t *)(configPtr->extension))->ftmPrescaler;
    info.ftmClockSource = ((extension_ftm_for_oc_t *)(configPtr->extension))->ftmClockSource;
    info.BDMMode = FTM_BDM_MODE_11;
    info.isTofIsrEnabled = false;
    info.enableInitializationTrigger = false;

    /* Initializes the FTM driver */
    status = FTM_DRV_Init((uint32_t)instance,
                          &info, ftmState);
    param.nNumOutputChannels = configPtr->nNumChannels;
    param.mode = FTM_MODE_OUTPUT_COMPARE;
    param.maxCountValue = ((extension_ftm_for_oc_t *)(configPtr->extension))->maxCountValue;

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        /* Get the hardware channel ID */
        channelConfig[index].hwChannelId = configPtr->outputChConfig[index].hwChannelId;
        channel = channelConfig[index].hwChannelId;
        /* Set channels configuration from user */
        channelConfig[index].chMode = (ftm_output_compare_mode_t)(configPtr->outputChConfig[index].chMode);
        channelConfig[index].comparedValue = configPtr->outputChConfig[index].comparedValue;
        channelConfig[index].enableExternalTrigger = false;
    }

    param.outputChannelConfig = channelConfig;

    if(STATUS_SUCCESS == status)
    {
        /* Configure channels in output compare mode */
        status = FTM_DRV_InitOutputCompare((uint32_t)instance,
                                          &param);

        if (status == STATUS_SUCCESS)
        {
            for (index = 0U; index < configPtr->nNumChannels; index++)
            {
                /* Get the hardware channel ID */
                channel = configPtr->outputChConfig[index].hwChannelId;
                /* Store some needed information into state structure */
                ocState->channelConfigArray[index] = configPtr->outputChConfig[index].hwChannelId;
                ocState->ocChannelMode[channel] = configPtr->outputChConfig[index].chMode;
                ocState->enableNotification[channel] = true;
                ocState->channelCallbackParams[channel] = configPtr->outputChConfig[index].channelCallbackParams;
                ocState->channelCallbacks[channel] = configPtr->outputChConfig[index].channelCallbacks;
                /* Install FTM irq handler */
                INT_SYS_InstallHandler(ftmIrq[instance][channel],
                                       s_ocOverFtmIsr[instance][channel], (isr_t*)0);
                /* Enable FTM interrupt */
                INT_SYS_EnableIRQ(ftmIrq[instance][channel]);
                status = FTM_DRV_EnableInterrupts((uint32_t)instance, (uint32_t)(1U << channel));
            }
        }
    }
    return status;
}
#endif

#if (defined(OC_PAL_OVER_EMIOS))
/*FUNCTION**********************************************************************
 *
 * Function Name : OC_InitEMIOS
 * Description   : This function initializes output compare over EMIOS.
 * This one supports all mode except the disable output mode.
 *END**************************************************************************/
 static status_t OC_InitEMIOS(uint32_t instance,
                              const oc_config_t * configPtr)
{
    status_t status = STATUS_SUCCESS;
    static oc_pal_state_t * ocState;
    oc_instance_t instancePtr;
    uint8_t indexInstance = 0U;
    uint8_t index = 0U;
    uint8_t channel = 0U;

    emios_common_param_t commonParam;
    emios_mc_mode_param_t mcParam;
    emios_oc_param_t ocParam;
    emios_bus_select_t timeBaseSelection = EMIOS_BUS_SEL_BCDE;
    static const IRQn_Type emiosIrq[eMIOS_INSTANCE_COUNT][(eMIOS_UC_COUNT / 2U)] = eMIOS_IRQS;

    /* Allocate one of the OC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = OC_INST_TYPE_EMIOS;
    indexInstance = ocAllocateState(ocStateIsAllocated, ocStateInstanceMapping, &instancePtr, NUMBER_OF_OC_PAL_INSTANCES);
    ocState = &g_ocPalStatePtr[indexInstance];
    DEV_ASSERT(ocState->nNumChannels == 0x0U);

    /* Initialize the internal context to default value */
    ocState->nNumChannels = configPtr->nNumChannels;
    for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM; index++)
    {
        ocState->channelConfigArray[index] = 0U;
        ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
        ocState->enableNotification[index] = false;
        ocState->channelCallbackParams[index] = NULL;
        ocState->channelCallbacks[index] = NULL;
    }

    /* Get common parameters from user */
    commonParam.allowDebugMode = false;
    commonParam.lowPowerMode = false;
    commonParam.clkDivVal = ((extension_emios_for_oc_t *)(configPtr->extension))->clkDivVal;
    commonParam.enableGlobalPrescaler = ((extension_emios_for_oc_t *)(configPtr->extension))->enableGlobalPrescaler;
    commonParam.enableGlobalTimeBase = ((extension_emios_for_oc_t *)(configPtr->extension))->enableGlobalTimeBase;

    /* Initialize the global for a eMIOS group */
    EMIOS_DRV_InitGlobal((uint8_t)instance,
                          &commonParam);

    /* Get the information from user configuration */
    mcParam.mode = EMIOS_MODE_MCB_UP_COUNTER_INT_CLK;
    mcParam.filterInput = EMIOS_INPUT_FILTER_BYPASS;
    mcParam.filterEn = false;
    mcParam.triggerMode = EMIOS_TRIGGER_EDGE_ANY;

    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        mcParam.period = ((channel_extension_emios_for_oc_t *)(configPtr->outputChConfig[index].channelExtension))->period;
        mcParam.internalPrescaler = ((channel_extension_emios_for_oc_t *)(configPtr->outputChConfig[index].channelExtension))->prescaler;
        mcParam.internalPrescalerEn = true;

        switch (((channel_extension_emios_for_oc_t *)(configPtr->outputChConfig[index].channelExtension))->timebase)
        {
            case OC_BUS_SEL_A:
                /* Set channel to use as a time base */
                channel = 23U;
                timeBaseSelection = EMIOS_BUS_SEL_A;
                break;
            case OC_BUS_SEL_B:
                /* Set channel to use as a time base */
                channel = 0U;
                timeBaseSelection = EMIOS_BUS_SEL_BCDE;
                break;
            case OC_BUS_SEL_C:
                /* Set channel to use as a time base */
                channel = 8U;
                timeBaseSelection = EMIOS_BUS_SEL_BCDE;
                break;
            case OC_BUS_SEL_D:
                /* Set channel to use as a time base */
                channel = 16U;
                timeBaseSelection = EMIOS_BUS_SEL_BCDE;
                break;
            case OC_BUS_SEL_E:
                /* Set channel to use as a time base */
                channel = 24U;
                timeBaseSelection = EMIOS_BUS_SEL_BCDE;
                break;
            case OC_BUS_SEL_F:
                /* Set channel to use as a time base */
                channel = 22U;
                timeBaseSelection = EMIOS_BUS_SEL_F;
                break;
            case OC_BUS_SEL_INTERNAL:
                /* Set channel to use as a time base */
                channel = configPtr->outputChConfig[index].hwChannelId;
                timeBaseSelection = EMIOS_BUS_SEL_INTERNAL;
                break;
            default:
                /* Do nothing */
                break;
        }

        /* Initialize the counter mode */
        status = EMIOS_DRV_MC_InitCounterMode((uint8_t)instance,
                                              channel,
                                              &mcParam);
        if(STATUS_SUCCESS != status)
        {
            break;
        }
        channel = configPtr->outputChConfig[index].hwChannelId;
        ocParam.mode = EMIOS_MODE_SAOC;
        ocParam.timebase = timeBaseSelection;
        ocParam.matchLeadingEdgeValue = (uint32_t)(configPtr->outputChConfig[index].comparedValue);
        ocParam.matchTrailingEdgeValue = 0U;

        /* Check the operation mode of channel output */
        if (OC_CLEAR_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            ocParam.outputActiveMode = EMIOS_OUTPUT_ACTIVE_LOW;
        }
        else if (OC_SET_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            ocParam.outputActiveMode = EMIOS_OUTPUT_ACTIVE_HIGH;
        }
        else
        {
            ocParam.outputActiveMode = EMIOS_OUTPUT_ACTIVE_TOGGLE;
        }

        /* Initialize the output compare mode for each channel */
        status = EMIOS_DRV_OC_InitOutputCompareMode((uint8_t)instance,
                                                    channel,
                                                    &ocParam);

        /* Enable EMIOS interrupt */
        EMIOS_DRV_ChannelEnableInterruptRequest((uint8_t)instance, channel);
        INT_SYS_EnableIRQ(emiosIrq[instance][(channel>>1U)]);

        /* Store some needed information into state structure */
        ocState->channelConfigArray[index] = channel;
        ocState->ocChannelMode[channel] = configPtr->outputChConfig[index].chMode;
        ocState->enableNotification[channel] = true;
        ocState->channelCallbackParams[channel] = configPtr->outputChConfig[index].channelCallbackParams;
        ocState->channelCallbacks[channel] = configPtr->outputChConfig[index].channelCallbacks;
    }

    /* Enable the global eMIOS to start counter */
    EMIOS_DRV_EnableGlobalEmios((uint8_t)instance);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_EmiosWaitOff
 * Description   : Interrupt handler for OC PAL.
 * This is not a public API as it is called by IRQ whenever an interrupt occurs.
 *
 *END**************************************************************************/
static void OC_EmiosWaitOutputOff(uint32_t instance, uint8_t channel)
{
    uint16_t i = 0U;

    for (i = 0U;i < OC_PAL_MAX_COUNTER_VALUE; i++)
    {
        if (EMIOS_DRV_GetCurrentOutputPin(instance,channel) == false)
        {
            break;
        }
    }
}
#endif

#if (defined(OC_PAL_OVER_ETIMER))
/*FUNCTION**********************************************************************
 *
 * Function Name : OC_EtimerInit
 * Description   : This function initializes the output compare mode over ETIMER.
 *
 *END**************************************************************************/
static status_t OC_EtimerInit(uint32_t etimerInstance,
                              const oc_config_t * configPtr)
{
    uint8_t index = 0U;
    uint16_t channel = 0U;
    uint16_t maskChannel = 0U;
    oc_pal_state_t * ocState;
    oc_instance_t instancePtr;

    /* Allocate one of the OC state structure for this instance */
    instancePtr.instIdx = etimerInstance;
    instancePtr.instType = OC_INST_TYPE_ETIMER;
    index = ocAllocateState(ocStateIsAllocated, ocStateInstanceMapping, &instancePtr, OC_ETIMER_INSTANCE_NUM);
    ocState = &g_ocPalStatePtr[index];
    DEV_ASSERT(ocState->nNumChannels == 0x0U);

    /* Initialize the internal context to default value */
    ocState->nNumChannels = configPtr->nNumChannels;
    for (index = 0U; index < OC_ETIMER_CHANNEL_MAX_NUM; index++)
    {
        ocState->channelConfigArray[index] = 0U;
        ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
        ocState->enableNotification[index] = false;
        ocState->channelCallbackParams[index] = NULL;
        ocState->channelCallbacks[index] = NULL;
    }

    /* Initialize the ETIMER module */
    ETIMER_DRV_Init(etimerInstance);

    /* default configuration */
    etimer_user_channel_config_t userChannelConfig = {
       .timerMode = ETIMER_MODE_OUTPUT_COMPARE,
       .coreSettings = ETIMER_COUNTER_NORMAL,
       .inputFilter =
       {
           .samples = ETIMER_FILT_CNT_3,
           .rate = 100,
       },
       .primaryInput =
       {
           .source = ETIMER_IN_SRC_CLK_DIV_128,
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
           0U,
           0U,
       },
       .compareLoading =
       {
           ETIMER_CLC_FROM_CMPLD1_WHEN_COMP1,
           ETIMER_CLC_NEVER,
       },
       .compareOutputControl = ETIMER_OUTMODE_SOFTWARE,
       .compareMode = ETIMER_CMPMODE_COMP1_UP_COMP2_DOWN,
       .captureControl =
       {
           ETIMER_CPTMODE_DISABLED,
           ETIMER_CPTMODE_DISABLED,
       },
       .captureWords = 0U,
       .interruptEnableMask = (ETIMER_CH_IRQ_SOURCE_TCF1IE|0U),
    };

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = (uint16_t)(configPtr->outputChConfig[index].hwChannelId);
        maskChannel |= (uint16_t)(1U << channel);

        /* setup clocking */
        userChannelConfig.primaryInput.source = ((channel_extension_etimer_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->primaryInput.source;
        userChannelConfig.primaryInput.polarity = ((channel_extension_etimer_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->primaryInput.polarity;

        /* output pin */
        userChannelConfig.outputPin.polarity = ((channel_extension_etimer_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->outputPin.polarity;
        userChannelConfig.outputPin.enable = ((channel_extension_etimer_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->outputPin.enable;

        /* set compare threshold */
        userChannelConfig.compareValues[0] = configPtr->outputChConfig[index].comparedValue;

        /* Check the operation mode of channel output */
        if (OC_CLEAR_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            userChannelConfig.compareOutputControl = ETIMER_OUTMODE_COMPARE_CLEAR;
        }
        else if (OC_SET_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            userChannelConfig.compareOutputControl = ETIMER_OUTMODE_COMPARE_SET;
        }
        else if (OC_TOGGLE_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            userChannelConfig.compareOutputControl = ETIMER_OUTMODE_COMPARE_TOGGLE;
        }
        else
        {
            userChannelConfig.compareOutputControl = ETIMER_OUTMODE_SOFTWARE;
            maskChannel &= ~((uint16_t)(1U << channel));
            /* disable the output pin, go to tri-state mode */
            ETIMER_DRV_OutputPinDisable((uint16_t)etimerInstance, channel);
        }
        /* Initialize the channel at the output compare mode */
        ETIMER_DRV_InitChannel((uint16_t)etimerInstance, channel, &userChannelConfig);
        /* Set counter reload */
        ETIMER_DRV_ReloadOnCompare((uint16_t)etimerInstance, channel, 0, true);

        /* Enable ETIMER channel interrupt and install an ISR */
        IRQn_Type etimer_ch_irq_id = ETIMER_DRV_GetInterruptNumber((uint16_t)etimerInstance,(etimer_irq_vector_t)channel);
        INT_SYS_EnableIRQ(etimer_ch_irq_id);

        /* Store some needed information into state structure */
        ocState->channelConfigArray[index] = channel;
        ocState->ocChannelMode[channel] = configPtr->outputChConfig[index].chMode;
        ocState->enableNotification[channel] = true;
        ocState->channelCallbackParams[channel] = configPtr->outputChConfig[index].channelCallbackParams;
        ocState->channelCallbacks[channel] = configPtr->outputChConfig[index].channelCallbacks;
    }

    /* Start the channel counting */
    ETIMER_DRV_StartTimerChannels((uint16_t)etimerInstance, maskChannel);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_ETIMER_SetOutputMode
 * Description   : This function will configure channel mode of output signal.
 *
 *END**************************************************************************/
static status_t OC_ETIMER_SetOutputMode(uint32_t instance,
                                        uint8_t channel,
                                        oc_option_mode_t channelMode)
{
    switch (channelMode)
    {
        case OC_TOGGLE_ON_MATCH:
            ETIMER_DRV_SetOutputFunction(instance, channel, ETIMER_OUTMODE_COMPARE_TOGGLE);
            ETIMER_DRV_OutputPinEnable(instance, channel);
            break;
        case OC_CLEAR_ON_MATCH:
            ETIMER_DRV_SetOutputFunction(instance, channel, ETIMER_OUTMODE_COMPARE_CLEAR);
            ETIMER_DRV_OutputPinEnable(instance, channel);
            break;
        case OC_SET_ON_MATCH:
            ETIMER_DRV_SetOutputFunction(instance, channel, ETIMER_OUTMODE_COMPARE_SET);
            ETIMER_DRV_OutputPinEnable(instance, channel);
            break;
        default:
            /* disable the output pin, go to tri-state mode */
            ETIMER_DRV_OutputPinDisable(instance, channel);
            break;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_ETIMER_ForceOutput
 * Description   : This function will force the output signal to specified value.
 *
 *END**************************************************************************/
static status_t OC_ETIMER_ForceOutput(uint32_t instance,
                                      uint8_t channel,
                                      bool outputValue)
{
    ETIMER_DRV_ForceOutputLogicLevel((uint16_t) instance, (uint16_t) channel,outputValue);
    return STATUS_SUCCESS;
}
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FLEXPWM_DRV_SetCountVal23
 * Description   : This function will update the PWM23 signal pulse width.
 *END**************************************************************************/
static void OC_FLEXPWM_DRV_SetCountVal23(uint32_t instance,
                                  uint8_t channel,
                                  uint16_t countValue2,
                                  uint16_t countValue3)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);
    FLEXPWM_DRV_SetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL2, countValue2);
    FLEXPWM_DRV_SetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL3, countValue3);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FLEXPWM_DRV_SetCountVal45
 * Description   : This function will update the PWM45 signal pulse width.
 *END**************************************************************************/
static void OC_FLEXPWM_DRV_SetCountVal45(uint32_t instance,
                                  uint8_t channel,
                                  uint16_t countValue4,
                                  uint16_t countValue5)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);

    FLEXPWM_DRV_SetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL4, countValue4);
    FLEXPWM_DRV_SetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL5, countValue5);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FLEXPWM_DRV_SetOutputLevel
 * Description   : This function is used to force signal output
 *END**************************************************************************/
static void OC_FLEXPWM_DRV_SetOutputLevel(uint32_t instance,
                                          uint8_t channel,
                                          uint8_t level)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);
    switch(channel)
    {
        case 0U:
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL23_0_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT23_0_SHIFT, level);
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL45_0_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT45_0_SHIFT, level);
            break;
        case 1U:
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL23_1_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT23_1_SHIFT, level);
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL45_1_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT45_1_SHIFT, level);
            break;
        case 2U:
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL23_2_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT23_2_SHIFT, level);
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL45_2_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT45_2_SHIFT, level);
            break;
        case 3U:
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL23_3_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT23_3_SHIFT, level);
            FLEXPWM_DRV_SelectDeadtimeSource(instance, FLEXPWM_DEADTIME_SEL45_3_SOURCE,
                                             FLEXPWM_DEADTIME_SW_PWM_SIGNAL);
            FLEXPWM_DRV_SelectSwCtrlOutput(instance, FlexPWM_SWCOUT_OUT45_3_SHIFT, level);
            break;
        default:
            /* Don't nothing !*/
            break;
    }

   /* Force the channel output which sets by software */
   FLEXPWM_DRV_ForcePwmOutput(instance, (flexpwm_module_t)channel,
                              true, true,
                              ForceOutputLocalForce);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FlexPwmInit
 * Description   : This function initializes the output compare mode over FLEXPWM.
 *
 *END**************************************************************************/
static status_t OC_FlexPwmInit(uint32_t instance,
                               const oc_config_t * configPtr)
{
    status_t status = STATUS_SUCCESS;
    uint8_t index = 0U;
    uint8_t channel = 0U;
    uint16_t interruptMask = 0U;
    uint16_t countValue2 = 0U;
    uint16_t countValue3 = 0U;
    uint16_t countValue4 = 0U;
    uint16_t countValue5 = 0U;
    oc_pal_state_t * ocState;
    oc_instance_t instancePtr;
    flexpwm_module_t subModule;

    /* Allocate one of the OC state structure for this instance */
    instancePtr.instIdx = instance;
    instancePtr.instType = OC_INST_TYPE_FLEXPWM;
    index = ocAllocateState(ocStateIsAllocated, ocStateInstanceMapping, &instancePtr, OC_FLEXPWM_INSTANCE_NUM);
    ocState = &g_ocPalStatePtr[index];
    DEV_ASSERT(ocState->nNumChannels == 0x0U);

    /* Initialize the internal context to default value */
    ocState->nNumChannels = configPtr->nNumChannels;
    for (index = 0U; index < OC_FLEXPWM_CHANNEL_MAX_NUM; index++)
    {
        ocState->channelConfigArray[index] = 0U;
        ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
        ocState->enableNotification[index] = false;
        ocState->channelCallbackParams[index] = NULL;
        ocState->channelCallbacks[index] = NULL;
    }

    /* Set default configuration */
    flexpwm_module_setup_t moduleSetupParams = {
        .cntrInitSel = InitSrcLocalSync,
        .clkSrc = ClkSrcPwmPeriphClk,
        .prescaler = PwmDividedBy1,
        .clockFreq = 0U,
        .chnlPairOper = FlexPwmIndependent,
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
        .pwmAOuten = true,
        .pwmBOuten = true,
        .pwmXOuten = true,
        .pwmAPolarity = false,
        .pwmBPolarity = false,
        .pwmXPolarity = false,
        .isPWMAFaultProtectEnabled = false,
        .isPWMBFaultProtectEnabled = false,
        .isPWMXFaultProtectEnabled = false
    };

    /* Configure a list of channels which are used */
    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = configPtr->outputChConfig[index].hwChannelId;
        subModule = (flexpwm_module_t)channel;

        moduleSetupParams.clkSrc = ((channel_extension_flexpwm_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->clkSrc;
        moduleSetupParams.prescaler = ((channel_extension_flexpwm_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->clkPrescaler;
        signalParams.pwmPeriod = ((channel_extension_flexpwm_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->maxCountValue;
        DEV_ASSERT(signalParams.pwmPeriod < 0xFFFFU);
        /* Initialize the channel at the output compare mode */
        FLEXPWM_DRV_SetupPwm(instance,
                             subModule,
                             &moduleSetupParams,
                             &signalParams);

        /* Check the operation mode of channel output */
        if (OC_CLEAR_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            countValue2 = signalParams.pwmPeriod + 1U;
            countValue3 = configPtr->outputChConfig[index].comparedValue;
            interruptMask = (((uint16_t)FLEXPWM_CMP_VAL2_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL4_INT_ENABLE));
        }
        else if (OC_SET_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            countValue2 = configPtr->outputChConfig[index].comparedValue;
            countValue3 = signalParams.pwmPeriod + 1U;
            interruptMask = (((uint16_t)FLEXPWM_CMP_VAL3_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL5_INT_ENABLE));
        }
        else if (OC_TOGGLE_ON_MATCH == configPtr->outputChConfig[index].chMode)
        {
            countValue2 = configPtr->outputChConfig[index].comparedValue;
            countValue3 = countValue2 + (signalParams.pwmPeriod / 2U);
            interruptMask = ((uint16_t)(((uint16_t)FLEXPWM_CMP_VAL2_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL3_INT_ENABLE)) |
                             (uint16_t)(((uint16_t)FLEXPWM_CMP_VAL4_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL5_INT_ENABLE)));
        }
        else
        {
            DEV_ASSERT(false);
        }

        /* Update the count value */
        countValue4 = countValue2;
        countValue5 = countValue3;
        OC_FLEXPWM_DRV_SetCountVal23(instance, channel, countValue2, countValue3);
        OC_FLEXPWM_DRV_SetCountVal45(instance, channel, countValue4, countValue5);

        /* Enable FLEXPWM channel interrupt and install an ISR */
        FLEXPWM_DRV_EnableModuleInterrupts(instance,
                                           subModule,
                                           interruptMask);

        /* Store some needed information into state structure */
        ocState->channelConfigArray[index] = channel;
        ocState->ocChannelMode[channel] = configPtr->outputChConfig[index].chMode;
        ocState->enableNotification[channel] = true;
        ocState->channelCallbackParams[channel] = configPtr->outputChConfig[index].channelCallbackParams;
        ocState->channelCallbacks[channel] = configPtr->outputChConfig[index].channelCallbacks;
    }

    for (index = 0U; index < configPtr->nNumChannels; index++)
    {
        channel = (configPtr->outputChConfig[index].hwChannelId);
        subModule = (flexpwm_module_t)channel;
        if(((channel_extension_flexpwm_for_oc_t *)configPtr->outputChConfig[index].channelExtension)->enDebugMode == true)
        {
            FLEXPWM_DRV_DebugEnable(instance, subModule);
        }
        /* Start the channel counting */
        FLEXPWM_DRV_CounterStart(instance, subModule);
    }

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FLEXPWM_SetOutputMode
 * Description   : This function will configure channel mode of output signal.
 *
 *END**************************************************************************/
static status_t OC_FLEXPWM_SetOutputMode(uint32_t instance,
                                         uint8_t channel,
                                         oc_option_mode_t channelMode)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);
    oc_pal_state_t * ocState;
    oc_instance_t instancePtr;
    uint8_t index = 0U;
    uint16_t countValue1 = FLEXPWM_DRV_GetPeriod(instance, (flexpwm_module_t)channel);
    uint16_t countValue2 = FLEXPWM_DRV_GetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL2);
    uint16_t countValue3 = FLEXPWM_DRV_GetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL3);
    uint16_t countValue4 = 0U;
    uint16_t countValue5 = 0U;
    uint16_t interruptMask = (uint16_t)FLEXPWM_CMP_ALL_VAL_INT_ENABLE;
    instancePtr.instType = OC_INST_TYPE_FLEXPWM;
    instancePtr.instIdx = instance;
    index = FindOcState(&instancePtr);
    ocState = &g_ocPalStatePtr[index];
    oc_option_mode_t prvChMode = ocState->ocChannelMode[channel];

    /* Disable FLEXPWM channel interrupt and install an ISR */
    FLEXPWM_DRV_DisableModuleInterrupts(instance,
                                        (flexpwm_module_t)channel,
                                        interruptMask);
    switch (channelMode)
    {
        case OC_TOGGLE_ON_MATCH:
            /* Set the pin to toggle output signal */
            if(prvChMode == OC_CLEAR_ON_MATCH)
            {
                countValue2 = countValue3;
                countValue3 += (countValue1 / 2U);
            }
            else
            {
                countValue3 = countValue2 + (countValue1 / 2U);
            }
            interruptMask = ((uint16_t)(((uint16_t)FLEXPWM_CMP_VAL2_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL3_INT_ENABLE)) |
                             (uint16_t)(((uint16_t)FLEXPWM_CMP_VAL4_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL5_INT_ENABLE)));
            break;
        case OC_CLEAR_ON_MATCH:
            /* Set the output pin as high level */
            if(prvChMode != OC_CLEAR_ON_MATCH)
            {
                countValue3 = countValue2;
            }
            countValue2 =  countValue1 + 1U;
            interruptMask = (((uint16_t)FLEXPWM_CMP_VAL2_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL4_INT_ENABLE));
            break;
        case OC_SET_ON_MATCH:
            /* Set the output pin as low level */
            if(prvChMode == OC_CLEAR_ON_MATCH)
            {
                countValue2 = countValue3;
            }
            countValue3 = countValue1 + 1U;
            interruptMask = (((uint16_t)FLEXPWM_CMP_VAL3_INT_ENABLE) | ((uint16_t)FLEXPWM_CMP_VAL5_INT_ENABLE));
            break;
        default:
            /* Disable the output pin, go to tri-state mode */
            countValue2 = 0U;
            countValue3 = 0U;
            break;
    }
    /* Update the count value */
    countValue4 = countValue2;
    countValue5 = countValue3;
    OC_FLEXPWM_DRV_SetCountVal23(instance, channel, countValue2, countValue3);
    OC_FLEXPWM_DRV_SetCountVal45(instance, channel, countValue4, countValue5);

    /* Reload register value */
    FLEXPWM_DRV_LoadCommands(instance, (uint32_t)(1UL << channel));
    /* Enable FLEXPWM channel interrupt and install an ISR */
    FLEXPWM_DRV_EnableModuleInterrupts(instance,
                                       (flexpwm_module_t)channel,
                                       interruptMask);
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_FLEXPWM_DRV_UpdateOutputCompare
 * Description   : This function will update the output state.
 *
 *END**************************************************************************/
static status_t OC_FLEXPWM_DRV_UpdateOutputCompare(uint32_t instance,
                                                   uint8_t channel,
                                                   uint32_t nextCompareMatchValue,
                                                   oc_option_update_t typeOfupdate)
{
    DEV_ASSERT(instance < FlexPWM_INSTANCE_COUNT);
    uint16_t compareValue = 0U;
    uint16_t counterValue = FLEXPWM_DRV_GetCounterValue(instance, (flexpwm_module_t)channel);
    uint16_t countValue3 = FLEXPWM_DRV_GetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL3);
    uint16_t countValue5 = FLEXPWM_DRV_GetCmpRegValue(instance, (flexpwm_module_t)channel, FlexPwmVAL5);

    if (typeOfupdate == OC_RELATIVE_VALUE)
    {
        /* Configure channel compare register */
        if ((uint16_t)nextCompareMatchValue > (MAX_COUNTER_VALUE - counterValue))
        {
            compareValue = ((uint16_t)nextCompareMatchValue - (MAX_COUNTER_VALUE - counterValue));
        }
        else
        {
            compareValue = (counterValue + (uint16_t)nextCompareMatchValue);
        }
    }
    else
    {
        compareValue = (uint16_t)nextCompareMatchValue;
    }

    /* Update the output compare value over FLEXPWM */
    OC_FLEXPWM_DRV_SetCountVal23(instance, channel, compareValue, countValue3);
    OC_FLEXPWM_DRV_SetCountVal45(instance, channel, compareValue, countValue5);
    FLEXPWM_DRV_LoadCommands(instance, (uint32_t)(1UL << channel));

    return STATUS_SUCCESS;
}
#endif
/*FUNCTION**********************************************************************
 *
 * Function Name : OC_Init
 * Description   : This function will initialize the OC PAL instance, including the
 * other platform specific HW units used together in the output compare mode. This
 * function configures a group of channels in instance to generate timed pulses with
 * programmable position, polarity, duration, and frequency. The channel (n) output
 * can be set, cleared, or toggled, unsupport output disable mode when initialize.
 * Implements    : OC_Init_Activity
 *END**************************************************************************/
status_t OC_Init(const oc_instance_t * const instance,
                 const oc_config_t * const configPtr)
{
    DEV_ASSERT(configPtr != NULL);
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(configPtr->nNumChannels > 0U);
    status_t status = STATUS_SUCCESS;

#if (defined(OC_PAL_OVER_FTM))
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(configPtr->extension != NULL);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Initialize output compare over FTM */
        status = OC_InitFtm(instance->instIdx, configPtr);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(configPtr->extension != NULL);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Initialize output compare over EMIOS */
        status = OC_InitEMIOS(instance->instIdx, configPtr);
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        /* Initialize output compare over ETIMER */
        status = OC_EtimerInit(instance->instIdx, configPtr);
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if(instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Initialize output compare over FLEXPWM */
        status = OC_FlexPwmInit(instance->instIdx, configPtr);
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
 * Function Name : OC_Deinit
 * Description   : This function will disable the output compare mode. The driver
 * can't be used again until reinitialized. The context structure is no longer
 * needed by the driver and can be freed after calling this function.
 *
 * Implements    : OC_Deinit_Activity
 *END**************************************************************************/
status_t OC_Deinit(const oc_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint8_t index;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];

#if (defined(OC_PAL_OVER_FTM))
    if(instance-> instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Disable the output compare over FTM */
        status = FTM_DRV_Deinit(instance->instIdx);
        /* De-Initialize the internal context to default value */
        ocState->nNumChannels = 0U;
        for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM; index++)
        {
            ocState->channelConfigArray[index] = 0U;
            ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
            ocState->enableNotification[index] = false;
        }
        if (STATUS_SUCCESS == status)
        {
            /* De-Allocate the instance which is not used */
            ocFreeState(ocStateIsAllocated, ocStateInstanceMapping, instance, NUMBER_OF_OC_PAL_INSTANCES);
        }
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    uint8_t channel;
    if(instance-> instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        for (index = 0U; index < ocState->nNumChannels; index++)
        {
            channel = ocState->channelConfigArray[index];
            /* Disable channels in the output compare over EMIOS */
            EMIOS_DRV_DeInitChannel((uint8_t)instance->instIdx, channel);
        }
        /* De-Initialize the internal context to default value */
        ocState->nNumChannels = 0U;
        for (index = 0U; index < OC_PAL_CHANNEL_MAX_NUM; index++)
        {
            ocState->channelConfigArray[index] = 0U;
            ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
            ocState->enableNotification[index] = false;
        }
        /* De-Allocate the instance which is not used */
        ocFreeState(ocStateIsAllocated, ocStateInstanceMapping, instance, NUMBER_OF_OC_PAL_INSTANCES);
        status = STATUS_SUCCESS;
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        /* Disable the output compare over ETIMER */
        ETIMER_DRV_Deinit((uint16_t)instance->instIdx);
        /* De-Initialize the internal context to default value */
        ocState->nNumChannels = 0U;
        for (index = 0U; index < OC_ETIMER_CHANNEL_MAX_NUM; index++)
        {
            ocState->channelConfigArray[index] = 0U;
            ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
            ocState->enableNotification[index] = false;
        }
        /* De-Allocate the instance which is not used */
        ocFreeState(ocStateIsAllocated, ocStateInstanceMapping, instance, OC_ETIMER_INSTANCE_NUM);
        status = STATUS_SUCCESS;
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if(instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Disable the output compare over FLEXPWM */
        FLEXPWM_DRV_Deinit(instance->instIdx);
        /* De-Initialize the internal context to default value */
        ocState->nNumChannels = 0U;
        for (index = 0U; index < OC_FLEXPWM_CHANNEL_MAX_NUM; index++)
        {
            ocState->channelConfigArray[index] = 0U;
            ocState->ocChannelMode[index] = OC_DISABLE_OUTPUT;
            ocState->enableNotification[index] = false;
        }
        /* De-Allocate the instance which is not used */
        ocFreeState(ocStateIsAllocated, ocStateInstanceMapping, instance, OC_FLEXPWM_INSTANCE_NUM);
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
 * Function Name : OC_StartChannel
 * Description   : This function start channel counting.
 *
 * Implements    : OC_StartChannel_Activity
 *END**************************************************************************/
void OC_StartChannel(const oc_instance_t * const instance,
                     const uint8_t channel)
{
    status_t status = STATUS_SUCCESS;
    DEV_ASSERT(instance != NULL);

#if (defined(OC_PAL_OVER_FTM))
    uint8_t index;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];
    oc_option_mode_t channelMode = ocState->ocChannelMode[channel];
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Set the channel output mode */
        status = FTM_DRV_SetOutputlevel(instance->instIdx, channel,
                                        (uint8_t)channelMode);
        DEV_ASSERT(status == STATUS_SUCCESS);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    uint8_t index;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];
    oc_option_mode_t channelMode = ocState->ocChannelMode[channel];
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Enable the channel clock */
        EMIOS_DRV_ChannelEnableClk((uint8_t)instance->instIdx, channel);
        /* Restore state structure */
        EMIOS_DRV_SetOutputLevel((uint8_t)instance->instIdx, channel,
                                 ((((uint8_t)channelMode & 0x02U) == 0U) ? true : false),
                                 ((((uint8_t)channelMode & 0x01U) == 0U) ? false : true));
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        ETIMER_DRV_StartTimerChannels((uint16_t)(instance->instIdx),
                                      (uint16_t)(1U << channel));
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if (instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Start the channel counting */
        FLEXPWM_DRV_CounterStart(instance->instIdx,
                                 (flexpwm_module_t)channel);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
    (void)status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_StopChannel
 * Description   : This function will stop channel output.
 *
 * Implements    : OC_StopChannel_Activity
 *END**************************************************************************/
void OC_StopChannel(const oc_instance_t * const instance,
                    const uint8_t channel)
{
    status_t status = STATUS_SUCCESS;
    DEV_ASSERT(instance != NULL);

#if (defined(OC_PAL_OVER_FTM))
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Disable pin not used for FTM */
        status = FTM_DRV_SetOutputlevel(instance->instIdx, channel, 0x0U);
        DEV_ASSERT(status == STATUS_SUCCESS);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        EMIOS_DRV_SetOutputLevel((uint8_t)instance->instIdx, channel, false, false);
        /* Disable individual channel by stopping its respective clock*/
        EMIOS_DRV_ChannelDisableClk((uint8_t)instance->instIdx, channel);
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        ETIMER_DRV_StopTimerChannels((uint16_t)(instance->instIdx),
                                     (uint16_t)(1U << channel));
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if (instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Stop the channel counting */
        FLEXPWM_DRV_CounterStop(instance->instIdx,
                                (flexpwm_module_t)channel);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
    (void)status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_SetOutputState
 * Description   : This function is used to forces the output pin to a specified
 * value.
 *
 * Implements    : OC_SetOutputState_Activity
 *END**************************************************************************/
status_t OC_SetOutputState(const oc_instance_t * const instance,
                           const uint8_t channel, bool outputValue)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_SUCCESS;

#if (defined(OC_PAL_OVER_FTM))
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        uint8_t channelMask = (uint8_t)(1U << channel);
        /* Enable the software output control */
        status = FTM_DRV_SetSoftwareOutputChannelControl(instance->instIdx, channelMask, false);
        /* Set the value of channel output */
        status = FTM_DRV_SetSoftOutChnValue(instance->instIdx, outputValue ? channelMask : 0x00U, true);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Set the state on the output signal */
        EMIOS_DRV_SetOutputLevel((uint8_t)instance->instIdx, channel, false, outputValue);
        /* Force the channel output which sets by software */
        EMIOS_DRV_OC_ForceSingleActOutputCmpMatch((uint8_t)instance->instIdx, channel);
        (void)status;
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        /* Force the channel output which sets by software */
        status = OC_ETIMER_ForceOutput(instance->instIdx,
                                       channel,
                                       outputValue);
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if(instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Force the channel output which sets by software */
        OC_FLEXPWM_DRV_SetOutputLevel(instance->instIdx,
                                      (flexpwm_module_t)channel,
                                      outputValue);
        (void)status;
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
 * Function Name : OC_SetOutputAction
 * Description   : This function is used to sets the action executed on a compare
 * match value to set output pin, clear output pin, toggle output pin.
 *
 * Implements    : OC_SetOutputAction_Activity
 *END**************************************************************************/
status_t OC_SetOutputAction(const oc_instance_t * const instance,
                            const uint8_t channel,
                            oc_option_mode_t channelMode)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_SUCCESS;
    uint8_t index;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];

#if (defined(OC_PAL_OVER_FTM))
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Set the channel output mode */
        status = FTM_DRV_SetOutputlevel(instance->instIdx,
                                        channel,
                                        (uint8_t)channelMode);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Set the channel output mode */
        if(channelMode != OC_DISABLE_OUTPUT)
        {
            /* Enable output pin */
            EMIOS_DRV_OutputEnable(instance->instIdx, channel);
            /* Set channel mode */
            EMIOS_DRV_SetOutputLevel((uint8_t)instance->instIdx, channel,
                                     ((((uint8_t)channelMode & 0x02U) == 0U) ? true : false),
                                     ((((uint8_t)channelMode & 0x01U) == 0U) ? false : true));
        }
        else
        {
            EMIOS_DRV_SetOutputLevel((uint8_t)instance->instIdx, channel, false, false);
            OC_EmiosWaitOutputOff((uint8_t)instance->instIdx, channel);
            EMIOS_DRV_OutputDisable(instance->instIdx, channel);
        }
        status = STATUS_SUCCESS;
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        status = OC_ETIMER_SetOutputMode(instance->instIdx,
                                         channel, channelMode);
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if(instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Set the channel output mode */
        status = OC_FLEXPWM_SetOutputMode(instance->instIdx,
                                          channel,
                                          channelMode);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
    /* Update the channel mode */
    ocState->ocChannelMode[channel] = channelMode;
    (void)index;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_SetCompareValue
 * Description   : This function will update the compare value to change the
 * output signal in the output compare mode.
 *
 * Implements    : OC_SetCompareValue_Activity
 *END**************************************************************************/
status_t OC_SetCompareValue(const oc_instance_t * const instance,
                            const uint8_t channel,
                            uint32_t nextCompareMatchValue,
                            oc_option_update_t typeOfupdate)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_SUCCESS;

#if (defined(OC_PAL_OVER_FTM))
    if(instance->instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        /* Update the output compare value over FTM */
        status = FTM_DRV_UpdateOutputCompareChannel(instance->instIdx, channel,
                                                    (uint16_t)nextCompareMatchValue,
                                                    (ftm_output_compare_update_t)typeOfupdate,
                                                    true);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instance->instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_PAL_INSTANCE_MAX_NUM);
        uint32_t compareValue = 0U;
        uint32_t maxCounterValue = 0U;
        uint32_t counterValue = EMIOS_DRV_MC_CounterRead((uint8_t)instance->instIdx, channel);

        if (typeOfupdate == OC_RELATIVE_VALUE)
        {
            maxCounterValue = EMIOS_DRV_MC_GetCounterPeriod((uint8_t)instance->instIdx, channel);
            /* Configure channel compare register */
            if (nextCompareMatchValue > (maxCounterValue - counterValue))
            {
                compareValue = (nextCompareMatchValue - (maxCounterValue - counterValue));
            }
            else
            {
                compareValue = (counterValue + nextCompareMatchValue);
            }
        }
        else
        {
            compareValue = nextCompareMatchValue;
        }
        /* Update the output compare value over EMIOS */
        EMIOS_DRV_OC_SetSingleActOutputCmpMatch((uint8_t)instance->instIdx,
                                                channel, compareValue);
        status = STATUS_SUCCESS;
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instance->instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_ETIMER_INSTANCE_MAX_NUM);
        uint16_t compareValue = 0U;
        uint16_t counterValue = ETIMER_DRV_GetTimerTicks((uint16_t)instance->instIdx, (uint16_t)channel);

        if (typeOfupdate == OC_RELATIVE_VALUE)
        {
            /* Configure channel compare register */
            if ((uint16_t)nextCompareMatchValue > (ETIMER_CH_MAX_TIMER_COUNT - counterValue))
            {
                compareValue = ((uint16_t)nextCompareMatchValue - (ETIMER_CH_MAX_TIMER_COUNT - counterValue));
            }
            else
            {
                compareValue = (counterValue + (uint16_t)nextCompareMatchValue);
            }
        }
        else
        {
            compareValue = (uint16_t)nextCompareMatchValue;
        }

        /* Update the output compare value over ETIMER */
        ETIMER_DRV_SetCompareThresholdBuffered((uint16_t)instance->instIdx,
                                            channel,
                                            compareValue,
                                            0x0000UL);
    }
    else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if(instance->instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance->instIdx < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Update the output compare value over FLEXPWM */
        status = OC_FLEXPWM_DRV_UpdateOutputCompare(instance->instIdx,
                                                    channel,
                                                    nextCompareMatchValue,
                                                    typeOfupdate);
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
 * Function Name : OC_EnableNotification
 * Description   : This function enables channel notification.
 *
 * Implements    : OC_EnableNotification_Activity
 *END**************************************************************************/
void OC_EnableNotification(const oc_instance_t * const instance,
                           const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);
    #if (defined(OC_PAL_OVER_FTM))
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_EMIOS))
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_ETIMER))
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_FLEXPWM))
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
    #endif
    uint8_t index = 0U;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];
    (void)index;
    /* Enable notification */
    ocState->enableNotification[channel] = true;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_DisableNotification
 * Description   : This function disables channel notification.
 *
 * Implements    : OC_DisableNotification_Activity
 *END**************************************************************************/
void OC_DisableNotification(const oc_instance_t * const instance,
                            const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);
    #if (defined(OC_PAL_OVER_FTM))
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_EMIOS))
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_ETIMER))
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
    #endif
    #if (defined(OC_PAL_OVER_FLEXPWM))
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
    #endif
    uint8_t index = 0U;
    oc_pal_state_t * ocState;

    /* Allocate one of the OC state structure for this instance */
    index = FindOcState(instance);
    ocState = &g_ocPalStatePtr[index];
    (void)index;
    /* Disable notification */
    ocState->enableNotification[channel] = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OC_IrqHandler
 * Description   : Interrupt handler for OC PAL.
 * This is not a public API as it is called by IRQ whenever an interrupt occurs.
 *
 *END**************************************************************************/
void OC_IrqHandler(uint32_t instance, uint8_t channel, oc_inst_type_t instType)
{
    uint8_t index = 0U;
    oc_pal_state_t * ocState;
#if (defined (OC_PAL_OVER_FTM))
    FTM_Type * const base = g_ftmBase[instance];
#endif
    oc_instance_t instancePtr;
    /* Allocate one of the OC state structure for this instance */
    instancePtr.instType = instType;
    instancePtr.instIdx = instance;
    index = FindOcState(&instancePtr);
    ocState = &g_ocPalStatePtr[index];

    if ((ocState->channelCallbacks[channel] != NULL) && (ocState->enableNotification[channel] == true))
    {
        /* Call to callback function */
        (ocState->channelCallbacks[channel])(OC_EVENT_GENERATION_OUTPUT_COMPLETE, ocState->channelCallbackParams[channel]);
    }
    (void)index;
#if (defined (OC_PAL_OVER_FTM))
    if(instType == OC_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance < OC_PAL_INSTANCE_MAX_NUM);
        /* Clear interrupt flag */
        FTM_DRV_ClearChnEventStatus(base, channel);
    }
    else
#endif

#if (defined(OC_PAL_OVER_EMIOS))
    if(instType == OC_INST_TYPE_EMIOS)
    {
        DEV_ASSERT(channel < OC_PAL_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance < OC_PAL_INSTANCE_MAX_NUM);
        /* Clear interrupt flag */
        EMIOS_DRV_ClrFlagState(instance, channel);
    }
    else
#endif

#if (defined(OC_PAL_OVER_ETIMER))
    if(instType == OC_INST_TYPE_ETIMER)
    {
        DEV_ASSERT(channel < OC_ETIMER_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance < OC_ETIMER_INSTANCE_MAX_NUM);
        /* Clear interrupt flag */
        ETIMER_DRV_ClearInterruptStatus((uint16_t)instance,
                                        (ETIMER_CH_IRQ_FLAGS_TCF | ETIMER_CH_IRQ_FLAGS_TCF1 | ETIMER_CH_IRQ_FLAGS_TCF2),
                                         channel);
     }
     else
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
    if (instType == OC_INST_TYPE_FLEXPWM)
    {
        DEV_ASSERT(channel < OC_FLEXPWM_CHANNEL_MAX_NUM);
        DEV_ASSERT(instance < OC_FLEXPWM_INSTANCE_MAX_NUM);
        /* Clear interrupt flag */
        FLEXPWM_DRV_ClearInterruptFlag(instance,
                                       (flexpwm_module_t)channel,
                                       FLEXPWM_CMPF_INT_FLAG);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
