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
 * @file timing_pal.c
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
 * Violates MISRA 2012 Required Rule 10.1, Unpermitted operand to operator.
 * This is required to get the right returned status from the
 * initialization function.
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

#include <stddef.h>
#include "timing_pal.h"
#include "timing_irq.h"
#include "device_registers.h"
#include "interrupt_manager.h"
#include "clock_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Runtime state of the Timer channel
 *
 * This structure is used by the driver for its internal logic
 * The application should make no assumptions about the content of this structure
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint32_t period;            /*!< To save timer channel period */
    uint32_t chanStartVal;      /*!< To save the moment that timer channel start new period */
    timer_chan_type_t chanType; /*!< To save timer channel notification type */
    timer_callback_t callback;  /*!< To save callback for channels notification */
    void * callbackParam;       /*!< To save callback parameter pointer.*/
    bool enableNotification;    /*!< To save enable channels notification */
    bool periodExpired;         /*!< If the channel type is one-shot, this flag is used to check whether period is expired */
/*! @endcond */
} timer_chan_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(TIMING_OVER_LPIT))
    /* Table to save LPIT channel runtime state */
    static timer_chan_state_t s_lpitState[LPIT_INSTANCE_COUNT][LPIT_TMR_COUNT];
    /* Table to save LPIT IRQ number */
    static const IRQn_Type s_lpitIrq[LPIT_INSTANCE_COUNT][LPIT_TMR_COUNT] = {LPIT_IRQS};
    /* The maximum value of compare register */
    #define LPIT_COMPARE_MAX (LPIT_TMR_TVAL_TMR_VAL_MASK)
#endif

#if (defined(TIMING_OVER_LPTMR))
    /* The maximum value of LPTMR channel number */
    #define LPTMR_TMR_COUNT LPTMR_IRQS_CH_COUNT
    /* Table to save LPTMR channel runtime state */
    static timer_chan_state_t s_lptmrState[LPTMR_INSTANCE_COUNT][LPTMR_TMR_COUNT];
    /* Table to save LPTMR IRQ number */
    static const IRQn_Type s_lptmrIrq[LPTMR_INSTANCE_COUNT][LPTMR_TMR_COUNT]  = {LPTMR_IRQS};
    /* Table to save LPTMR clock source name */
    static lptmr_clocksource_t s_lptmrClockSource[LPTMR_INSTANCE_COUNT];
    /* Table to save LPTMR prescaler */
    static lptmr_prescaler_t s_lptmrPrescaler[LPTMR_INSTANCE_COUNT];
    /* Table to save LPTMR bypass prescaler enable */
    static bool s_lptmrBypassPrescaler[LPTMR_INSTANCE_COUNT];
    /* The maximum value of compare register */
    #define LPTMR_COMPARE_MAX (LPTMR_CMR_COMPARE_MASK)
#endif

#if (defined(TIMING_OVER_FTM))
    /*! @brief Table of base addresses for FTM instances. */
    FTM_Type * const ftmBase[FTM_INSTANCE_COUNT] = FTM_BASE_PTRS;
    /* Table to save FTM IRQ number */
    static const IRQn_Type s_ftmIrq[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT] = FTM_IRQS;
    /* Table to save FTM channel runtime state */
    static timer_chan_state_t s_ftmState[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT];
    /* Table to save FTM channel running status */
    bool g_ftmChannelRunning[FTM_INSTANCE_COUNT][FTM_CONTROLS_COUNT];
    /* The maximum value of compare register */
    #define FTM_COMPARE_MAX (FTM_CNT_COUNT_MASK)
#endif

#if (defined(TIMING_OVER_PIT))
    /* Table to save PIT channel runtime state */
    static timer_chan_state_t s_pitState[PIT_INSTANCE_COUNT][PIT_TIMER_COUNT];
    /* Table to save PIT IRQ number */
    static const IRQn_Type s_pitIrq[PIT_INSTANCE_COUNT][PIT_IRQS_CH_COUNT] = PIT_IRQS;
    /* Table to save PIT clock source name */
    static clock_names_t s_pitClockName[PIT_INSTANCE_COUNT] = PIT_CLOCK_NAMES;
    /* The maximum value of compare register */
    #define PIT_COMPARE_MAX (PIT_LDVAL_TSV_MASK)
#endif

#if (defined(TIMING_OVER_STM))
    /* Table to save STM channel runtime state */
    static timer_chan_state_t s_stmState[STM_INSTANCE_COUNT][STM_CHANNEL_COUNT];
    /* Table to save STM IRQ number */
    static const IRQn_Type s_stmIrq[STM_INSTANCE_COUNT][STM_CHANNEL_COUNT] = STM_IRQS;
#if FEATURE_STM_HAS_CLOCK_SELECTION
    /* Table to save STM clock source name */
    static stm_clock_source_t s_stmClockSource[STM_INSTANCE_COUNT];
#endif
    /* Table to save STM prescaler */
    static uint8_t s_stmPrescaler[STM_INSTANCE_COUNT];
    /* The maximum value of compare register */
    #define STM_COMPARE_MAX (STM_CMP_CMP_MASK)
#endif

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

#if (defined(TIMING_OVER_LPIT))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InitLpit
 * Description   : This function initializes TIMING over LPIT.
 *END**************************************************************************/
static status_t TIMING_InitLpit(uint32_t instance,
                                const timer_config_t * config)
{
    DEV_ASSERT(instance < LPIT_INSTANCE_COUNT);

    status_t status = STATUS_SUCCESS;
    lpit_user_config_t lpitConfig;
    lpit_user_channel_config_t channelConfig;
    timer_chan_state_t * channelState;
    uint32_t chanIndex;
    uint8_t index;

    /* Set global structure */
    lpitConfig.enableRunInDebug = true;
    lpitConfig.enableRunInDoze = true;
    /* Set channel configuration structure */
    channelConfig.timerMode = LPIT_PERIODIC_COUNTER;
    channelConfig.periodUnits = LPIT_PERIOD_UNITS_COUNTS;
    channelConfig.period = 0U;
    channelConfig.triggerSource = LPIT_TRIGGER_SOURCE_EXTERNAL;
    channelConfig.triggerSelect = 0U;
    channelConfig.enableReloadOnTrigger = false;
    channelConfig.enableStopOnInterrupt = false;
    channelConfig.enableStartOnTrigger = false;
    channelConfig.chainChannel = false;
    channelConfig.isInterruptEnabled = true;
    /* Initialize LPIT instance */
    LPIT_DRV_Init(instance, &lpitConfig);
    /* Initialize LPIT channels */
    for (index = 0U; index < config->numChan; index++)
    {
        chanIndex = config->chanConfigArray[index].channel;
        channelState = &s_lpitState[instance][chanIndex];
        /* Initialize LPIT channels */
        status = LPIT_DRV_InitChannel(instance, chanIndex, &channelConfig);
        /* Save runtime state structure of timer channel */
        channelState->chanType = config->chanConfigArray[index].chanType;
        channelState->callback = config->chanConfigArray[index].callback;
        channelState->callbackParam = config->chanConfigArray[index].callbackParam;
        channelState->enableNotification = false;
        /* Enable LPIT interrupt */
        INT_SYS_EnableIRQ(s_lpitIrq[instance][chanIndex]);
    }

    return status;
}
#endif

#if (defined(TIMING_OVER_LPTMR))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InitLptmr
 * Description   : This function initializes TIMING over LPTMR.
 *END**************************************************************************/
static status_t TIMING_InitLptmr(uint32_t instance,
                                 const timer_config_t * config)
{
    DEV_ASSERT(instance < LPTMR_INSTANCE_COUNT);

    status_t status = STATUS_SUCCESS;
    lptmr_config_t lptmrConfig;
    timer_chan_state_t * channelState;
    uint8_t chanIndex;
    uint8_t index;

    /* Set lptmr structure */
    lptmrConfig.dmaRequest = false;
    lptmrConfig.interruptEnable = true;
    lptmrConfig.freeRun = false;
    lptmrConfig.workMode = LPTMR_WORKMODE_TIMER;
    lptmrConfig.clockSelect = ((extension_lptmr_for_timer_t*)(config->extension))->clockSelect;
    lptmrConfig.prescaler = ((extension_lptmr_for_timer_t*)(config->extension))->prescaler;
    lptmrConfig.bypassPrescaler = ((extension_lptmr_for_timer_t*)(config->extension))->bypassPrescaler;
    lptmrConfig.counterUnits = LPTMR_COUNTER_UNITS_TICKS;
    lptmrConfig.pinSelect = LPTMR_PINSELECT_TRGMUX;
    lptmrConfig.pinPolarity = LPTMR_PINPOLARITY_RISING;
    lptmrConfig.compareValue = 0U;
    /* Save LPTMR clock source name */
    s_lptmrClockSource[instance] = ((extension_lptmr_for_timer_t*)(config->extension))->clockSelect;
    /* Save LPTMR prescaler */
    s_lptmrPrescaler[instance] = ((extension_lptmr_for_timer_t*)(config->extension))->prescaler;
    /* Save LPTMR bypass enable */
    s_lptmrBypassPrescaler[instance] = ((extension_lptmr_for_timer_t*)(config->extension))->bypassPrescaler;
    /* Initialize LPTMR instance */
    LPTMR_DRV_Init(instance, &lptmrConfig, false);

    for (index = 0U; index < config->numChan; index++)
    {
        chanIndex = config->chanConfigArray[index].channel;
        DEV_ASSERT(chanIndex < LPTMR_TMR_COUNT);
        channelState = &s_lptmrState[instance][chanIndex];
        /* Save runtime state structure of timer channel */
        channelState->chanType = config->chanConfigArray[index].chanType;
        channelState->callback = config->chanConfigArray[index].callback;
        channelState->callbackParam = config->chanConfigArray[index].callbackParam;
        channelState->enableNotification = false;
        /* Enable LPTMR interrupt */
        INT_SYS_EnableIRQ(s_lptmrIrq[instance][chanIndex]);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_GetLptmrFreq
 * Description   : This function gets LPTMR clock frequency.
 *END**************************************************************************/
static void TIMING_GetLptmrFreq(uint32_t instance, uint32_t * frequency, uint64_t * prescaler)
{
    status_t clkErr;
    clock_names_t inputClockName = SIRCDIV2_CLK;

    switch(s_lptmrClockSource[instance])
    {
    case LPTMR_CLOCKSOURCE_SIRCDIV2:
        inputClockName = SIRCDIV2_CLK;
        break;
    case LPTMR_CLOCKSOURCE_1KHZ_LPO:
        inputClockName = SIM_LPO_1K_CLK;
        break;
    case LPTMR_CLOCKSOURCE_RTC:
        inputClockName = SIM_RTCCLK_CLK;
        break;
    case LPTMR_CLOCKSOURCE_PCC:
        inputClockName = LPTMR0_CLK;
        break;
    default:
        /* Invalid clock source */
        DEV_ASSERT(false);
        break;
    }
    /* Gets current functional clock frequency of LPTMR instance */
    clkErr = CLOCK_SYS_GetFreq(inputClockName, frequency);
    /* Checks the functional clock module is available */
    (void)clkErr;
    DEV_ASSERT(clkErr == STATUS_SUCCESS);
    DEV_ASSERT(*frequency > 0U);

    if (!s_lptmrBypassPrescaler[instance])
    {
        uint32_t tmp = 1UL << ((uint8_t)s_lptmrPrescaler[instance] + 1U);
        *prescaler = (uint64_t)(tmp);
    }
}
#endif

#if (defined(TIMING_OVER_FTM))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InitFtm
 * Description   : This function initializes TIMING over FTM.
 *END**************************************************************************/
static status_t TIMING_InitFtm(uint32_t instance,
                               const timer_config_t * config)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);

    status_t status = STATUS_SUCCESS;
    static ftm_state_t ftmState[FTM_INSTANCE_COUNT];
    ftm_user_config_t ftmConfig;
    ftm_output_cmp_param_t outputCmpConfig;
    ftm_output_cmp_ch_param_t chnConfigArray[FTM_CONTROLS_COUNT];
    timer_chan_state_t * channelState;
    uint8_t chanIndex;
    uint8_t index;

    /* Set ftm structure */
    ftmConfig.syncMethod.softwareSync = true;
    ftmConfig.syncMethod.hardwareSync0 = false;
    ftmConfig.syncMethod.hardwareSync1 = false;
    ftmConfig.syncMethod.hardwareSync2 = false;
    ftmConfig.syncMethod.maxLoadingPoint = true;
    ftmConfig.syncMethod.minLoadingPoint = false;
    ftmConfig.syncMethod.inverterSync = FTM_SYSTEM_CLOCK;
    ftmConfig.syncMethod.outRegSync = FTM_SYSTEM_CLOCK;
    ftmConfig.syncMethod.maskRegSync = FTM_SYSTEM_CLOCK;
    ftmConfig.syncMethod.initCounterSync = FTM_SYSTEM_CLOCK;
    ftmConfig.syncMethod.autoClearTrigger = false;
    ftmConfig.syncMethod.syncPoint = FTM_UPDATE_NOW;
    ftmConfig.ftmMode = FTM_MODE_OUTPUT_COMPARE;
    ftmConfig.ftmPrescaler = ((extension_ftm_for_timer_t*)(config->extension))->prescaler;
    ftmConfig.ftmClockSource = ((extension_ftm_for_timer_t*)(config->extension))->clockSelect;
    ftmConfig.BDMMode = FTM_BDM_MODE_00;
    ftmConfig.isTofIsrEnabled = false;
    ftmConfig.enableInitializationTrigger = false;

    /* Initialize running status of FTM channels */
    for (index = 0U; index < FTM_CONTROLS_COUNT; index++)
    {
        g_ftmChannelRunning[instance][index] = false;
    }
    /* Set output compare configuration structure */
    for (index = 0U; index < config->numChan; index++)
    {
        chnConfigArray[index].hwChannelId = config->chanConfigArray[index].channel;
        chnConfigArray[index].chMode = FTM_DISABLE_OUTPUT;
        chnConfigArray[index].comparedValue = 0U;
        chnConfigArray[index].enableExternalTrigger = false;
    }
    outputCmpConfig.nNumOutputChannels = config->numChan;
    outputCmpConfig.mode = FTM_MODE_OUTPUT_COMPARE;
    outputCmpConfig.maxCountValue = ((extension_ftm_for_timer_t*)(config->extension))->finalValue;
    outputCmpConfig.outputChannelConfig = chnConfigArray;

    /* Initialize FTM instance */
    status = FTM_DRV_Init(instance, &ftmConfig, &ftmState[instance]);
    status |= FTM_DRV_InitOutputCompare(instance, &outputCmpConfig);

    if (status == STATUS_SUCCESS)
    {
        for (index = 0U; index < config->numChan; index++)
        {
            chanIndex = config->chanConfigArray[index].channel;
            channelState = &s_ftmState[instance][chanIndex];
            /* Save runtime state structure of timer channel */
            channelState->chanType = config->chanConfigArray[index].chanType;
            channelState->callback = config->chanConfigArray[index].callback;
            channelState->callbackParam = config->chanConfigArray[index].callbackParam;
            channelState->enableNotification = false;
            /* Install FTM irq handler */
            INT_SYS_InstallHandler(s_ftmIrq[instance][chanIndex],
                                   s_timingOverFtmIsr[instance][chanIndex], (isr_t*)0);
            /* Enable FTM interrupt */
            INT_SYS_EnableIRQ(s_ftmIrq[instance][chanIndex]);
        }
    }
    else
    {
        status = STATUS_ERROR;
    }

    return status;
}
#endif

#if (defined(TIMING_OVER_PIT))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InitPit
 * Description   : This function initializes TIMING over PIT.
 *END**************************************************************************/
static status_t TIMING_InitPit(uint32_t instance,
                               const timer_config_t * config)
{
    DEV_ASSERT(instance < PIT_INSTANCE_COUNT);

    status_t status = STATUS_SUCCESS;
    pit_config_t pitConfig;
    pit_channel_config_t channelConfig;
    timer_chan_state_t * channelState;
    uint8_t chanIndex;
    uint8_t index;

    /* Set global structure */
#if FEATURE_PIT_HAS_RTI_CHANNEL
    pitConfig.enableRTITimer = false;
#endif
    pitConfig.enableStandardTimers = true;
    pitConfig.stopRunInDebug = true;
    /* Set channel configuration structure */
    channelConfig.hwChannel = 0U;
    channelConfig.periodUnit = PIT_PERIOD_UNITS_COUNTS;
    channelConfig.period = 0U;
    channelConfig.enableChain = false;
    channelConfig.enableInterrupt = true;

    /* Initialize PIT instance */
    PIT_DRV_Init(instance, &pitConfig);

    for (index = 0U; index < config->numChan; index++)
    {
        chanIndex = config->chanConfigArray[index].channel;
        channelState = &s_pitState[instance][chanIndex];
        /* Initialize PIT channels */
        channelConfig.hwChannel = chanIndex;
        status = PIT_DRV_InitChannel(instance, &channelConfig);
        if (status != STATUS_SUCCESS)
        {
            break;
        }
        /* Save runtime state structure of timer channel */
        channelState->chanType = config->chanConfigArray[index].chanType;
        channelState->callback = config->chanConfigArray[index].callback;
        channelState->callbackParam = config->chanConfigArray[index].callbackParam;
        channelState->enableNotification = false;
        /* Enable PIT interrupt */
        INT_SYS_EnableIRQ(s_pitIrq[instance][chanIndex]);
    }

    return status;
}
#endif

#if (defined(TIMING_OVER_STM))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InitStm
 * Description   : This function initializes TIMING over STM.
 *END**************************************************************************/
static status_t TIMING_InitStm(uint32_t instance,
                               const timer_config_t * config)
{
    DEV_ASSERT(instance < STM_INSTANCE_COUNT);

    status_t status = STATUS_SUCCESS;
    stm_config_t stmConfig;
    timer_chan_state_t * channelState;
    uint8_t chanIndex;
    uint8_t index;

    /* Set stm structure */
#if FEATURE_STM_HAS_CLOCK_SELECTION
    stmConfig.clockSource = ((extension_stm_for_timer_t*)(config->extension))->clockSelect;
    /* Save LPTMR clock source name */
    s_stmClockSource[instance] = ((extension_stm_for_timer_t*)(config->extension))->clockSelect;
#endif
    stmConfig.clockPrescaler = ((extension_stm_for_timer_t*)(config->extension))->prescaler;
    stmConfig.stopInDebugMode = true;
    stmConfig.startValue = 0U;
    /* Save LPTMR prescaler */
    s_stmPrescaler[instance] = ((extension_stm_for_timer_t*)(config->extension))->prescaler;
    /* Initialize STM instance */
    STM_DRV_Init(instance, &stmConfig);

    for (index = 0U; index < config->numChan; index++)
    {
        chanIndex = config->chanConfigArray[index].channel;
        channelState = &s_stmState[instance][chanIndex];
        /* Save runtime state structure of timer channel */
        channelState->chanType = config->chanConfigArray[index].chanType;
        channelState->callback = config->chanConfigArray[index].callback;
        channelState->callbackParam = config->chanConfigArray[index].callbackParam;
        channelState->enableNotification = false;
        /* Enable STM interrupt */
        INT_SYS_EnableIRQ(s_stmIrq[instance][chanIndex]);
    }

    return status;
}
#endif

/*******************************************************************************
 * FUCNTION FUNCTION
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Init
 * Description   : Initialize timer instance.
 * This function initializes clock source, prescaler of the timer instance(except LPIT, PIT), the final
 * value of counter (only FTM). This function also setups notification type and callback function of timer channel.
 * The timer instance number and its configuration structure shall be passed as arguments.
 * Timer channels do not start counting by default after calling this function.
 * The function TIMING_StartChannel must be called to start the timer channel counting.
 *
 * Implements    : TIMING_Init_Activity
 *END**************************************************************************/
status_t TIMING_Init(const timing_instance_t * const instance,
                     const timer_config_t * const config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);
    status_t status = STATUS_ERROR;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        /* Initialize TIMING over LPIT */
        status = TIMING_InitLpit(instance->instIdx, config);
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT(config->extension != NULL);

        /* Initialize TIMING over LPTMR */
        status = TIMING_InitLptmr(instance->instIdx, config);
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT(config->extension != NULL);

        /* Initialize TIMING over FTM */
        status = TIMING_InitFtm(instance->instIdx, config);
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        /* Initialize TIMING over LPIT */
        status = TIMING_InitPit(instance->instIdx, config);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT(config->extension != NULL);

        /* Initialize TIMING over STM */
        status = TIMING_InitStm(instance->instIdx, config);
    }
    else
#endif
    {
        /* Do nothing */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Deinit
 * Description   : De-initialize timer instance.
 * This function de-initializes timer instance.
 * In order to use the timer instance again, TIMING_Init must be called.
 *
 * Implements    : TIMING_Deinit_Activity
 *END**************************************************************************/
void TIMING_Deinit(const timing_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        LPIT_DRV_Deinit(instance->instIdx);
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        LPTMR_DRV_Deinit(instance->instIdx);
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        status_t retVal;

        retVal = FTM_DRV_Deinit(instance->instIdx);
        (void)retVal;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        PIT_DRV_Deinit(instance->instIdx);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        STM_DRV_Deinit(instance->instIdx);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_StartChannel
 * Description   : Starts the timer channel counting.
 * This function starts channel counting with a new period in ticks.
 * Note that:
 * - If the timer is PIT or LPIT, if timer channel is running with a period, this function will not immediately restart
 *   timer channel with a new period, instead the new period will be loaded after the current period expires.
 *   In order to abort the current timer channel period and start timer channel with a new period, the timer channel
 *   must be stopped and started again with new period.
 *   Additional, if call this function with new period while timer channel is running, calling TIMING_GetElapsed and TIMING_GetRemaining
 *   may return unexpected result.
 * - If the timer is FTM, this function start channel by enable channel interrupt generation.
 * - LPTMR and FTM is 16 bit timer, so the input period must be smaller than 65535.
 * - LPTMR and FTM is 16 bit timer, so the input period must be smaller than 65535.
 *
 * Implements    : TIMING_StartChannel_Activity
 *END**************************************************************************/
void TIMING_StartChannel(const timing_instance_t * const instance,
                         const uint8_t channel,
                         const uint32_t periodTicks)
{
    DEV_ASSERT(instance != NULL);

    timer_chan_state_t * channelState;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        uint32_t channelMask = 1UL << channel;

        /* Set the channel compare value */
        LPIT_DRV_SetTimerPeriodByCount(instance->instIdx, channel, periodTicks);
        /* Start the channel counting */
        LPIT_DRV_StartTimerChannels(instance->instIdx, channelMask);

        channelState = &s_lpitState[instance->instIdx][channel];
        /* Reset period expired flag */
        channelState->periodExpired = false;
        /* Save the period of channel */
        channelState->period = periodTicks;
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT(periodTicks <= LPTMR_COMPARE_MAX);
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);
        uint32_t lptmrInstance = instance->instIdx;
        status_t retVal;

        /* Stop the channel counting */
        LPTMR_DRV_StopCounter(lptmrInstance);
        /* Set the channel compare value */
        retVal = LPTMR_DRV_SetCompareValueByCount(lptmrInstance, (uint16_t)periodTicks);
        (void)retVal;
        /* Start the channel counting */
        LPTMR_DRV_StartCounter(lptmrInstance);

        channelState = &s_lptmrState[lptmrInstance][channel];
        /* Reset period expired flag */
        channelState->periodExpired = false;
        /* Save the period of channel */
        channelState->period = periodTicks;
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        uint32_t ftmInstance = instance->instIdx;
        FTM_Type * const base = ftmBase[ftmInstance];
        uint32_t currentCounter = 0U;
        uint32_t nexCompareValue = 0U;
        uint32_t finalValue;
        status_t retVal;

        DEV_ASSERT(periodTicks <= FTM_DRV_GetMod(base));

        /* Clear the channel interrupt flag which may be set after executed initialization timing function or a previous channel match event */
        FTM_DRV_ClearChnEventStatus(base, channel);
        /* Get current counter*/
        currentCounter = FTM_DRV_GetCounter(base);
        /* Get the final value of counter */
        finalValue = FTM_DRV_GetMod(base);
        /* Calculate the next compare value of the channel */
        if ((finalValue - currentCounter) > periodTicks)
        {
            nexCompareValue = currentCounter + periodTicks;
        }
        else
        {
            nexCompareValue = periodTicks - (finalValue - currentCounter);
        }
        /* Update next compare value to the channel */
        retVal = FTM_DRV_UpdateOutputCompareChannel(ftmInstance, channel, (uint16_t)nexCompareValue, FTM_ABSOLUTE_VALUE, false);
        /* Enable the channel by enable interrupt generation */
        retVal = FTM_DRV_EnableInterrupts(ftmInstance, (1UL << channel));
        (void)retVal;
        /* Update channel running status */
        g_ftmChannelRunning[ftmInstance][channel] = true;
        /* Save the start value of channel at the moment the start channel function is called */
        channelState = &s_ftmState[ftmInstance][channel];
        /* Reset period expired flag */
        channelState->periodExpired = false;
        /* Save the channel start value */
        channelState->chanStartVal = currentCounter;
        /* Save the period of channel */
        channelState->period = periodTicks;
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        const uint32_t pitInstance = instance->instIdx;
        /* Set the channel compare value */
        PIT_DRV_SetTimerPeriodByCount(pitInstance, channel, periodTicks);
        /* Start the channel counting */
        PIT_DRV_StartChannel(pitInstance, channel);

        channelState = &s_pitState[pitInstance][channel];
        /* Save the period of channel */
        channelState->period = periodTicks;
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        uint32_t stmInstance = instance->instIdx;
        uint32_t currentCounter;
        uint32_t compareValue;

        /* Get current counter value */
        currentCounter = STM_DRV_GetCounterValue(stmInstance);
        /* Calculate the channel compare value */
        if ((STM_COMPARE_MAX - currentCounter) >= periodTicks)
        {
            /* The distance from current value to max of compare register is enough */
            compareValue = currentCounter + periodTicks;
        }
        else
        {
            /* The distance is not enough, calculates a new value for compare register */
            compareValue = periodTicks - (STM_COMPARE_MAX - currentCounter);
        }
        /* Configure channel compare value */
        STM_DRV_ConfigChannel(stmInstance, channel, compareValue);
        /* Start counter */
        STM_DRV_StartTimer(stmInstance);
        /* Save the start value of channel at the moment the start channel function is called */
        channelState = &s_stmState[stmInstance][channel];
        channelState->chanStartVal = currentCounter;
        /* Save the period of channel */
        channelState->period = periodTicks;
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_StopChannel
 * Description   : Stop the timer channel counting.
 * This function stop channel counting. Note that if the timer is FTM,
 * this function stop channel by disable channel interrupt generation.
 *
 * Implements    : TIMING_StopChannel_Activity
 *END**************************************************************************/
void TIMING_StopChannel(const timing_instance_t * const instance,
                        const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

    timer_chan_state_t * channelState;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        /* Stop the channel counting */
        LPIT_DRV_StopTimerChannels(instance->instIdx, (1UL << channel));

        channelState = &s_lpitState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);

        /* Stop the channel counting */
        LPTMR_DRV_StopCounter(instance->instIdx);

        channelState = &s_lptmrState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);
        /* Stop the channel by disable interrupt generation */
        FTM_DRV_DisableInterrupts(instance->instIdx, (1UL << channel));
        /* Update channel running status */
        g_ftmChannelRunning[instance->instIdx][channel] = false;
        channelState = &s_ftmState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        /* Stop the channel counting */
        PIT_DRV_StopChannel(instance->instIdx, channel);

        channelState = &s_pitState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        /* Stop the channel counting */
        STM_DRV_DisableChannel(instance->instIdx, channel);

         /* Save the start value of channel at the moment the start channel function is called */
        channelState = &s_stmState[instance->instIdx][channel];
        /* Enable notification */
        channelState->enableNotification = false;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_GetElapsed
 * Description   : Get elapsed ticks.
 * This function gets elapsed time since the last event by ticks. The elapsed time by nanosecond, microsecond or
 * millisecond is the result of this function multiplies by the result of the TIMING_GetResolution
 * function.
 * Note that:
 * If the timer channel type is continuous, this function may not return value of the period at the moment period is timeout depending on timer frequency,
 * optimizations, etc. The behavior occurs if the execution time of the function is significant relative to timer tick duration.
 * If the timer channel type is one-shot, this function can be used to check whether the current period is timeout, in this case
 * if the returned value is bigger or equal than the period, the current period is timeout or overflowed.
 *
 * Implements    : TIMING_GetElapsed_Activity
 *END**************************************************************************/
uint32_t TIMING_GetElapsed(const timing_instance_t * const instance,
                           const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

    uint32_t currentCounter = 0U;
    uint32_t timeElapsed = 0U;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        const timer_chan_state_t * lpitChannelState;
        uint32_t lpitChanPeriod = 0U;
        /* Get current channel counter value */
        currentCounter = LPIT_DRV_GetCurrentTimerCount(instance->instIdx, channel);

        lpitChannelState = &s_lpitState[instance->instIdx][channel];
        /* Get channel period */
        lpitChanPeriod = lpitChannelState->period;
        /* Calculate timer elapsed */
        timeElapsed = lpitChanPeriod - currentCounter;

        if ((lpitChannelState->chanType == TIMER_CHAN_TYPE_ONESHOT) && (lpitChannelState->periodExpired))
        {
            /* In the case channel type is one-shot and period is expired, set time elapsed to be period value */
            timeElapsed = lpitChanPeriod;
        }
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);
        const timer_chan_state_t * lptmrChannelState;
        (void)currentCounter;

        lptmrChannelState = &s_lptmrState[instance->instIdx][channel];
        /* Time elapsed is current counter value */
        timeElapsed = LPTMR_DRV_GetCounterValueByCount(instance->instIdx);

        if ((lptmrChannelState->chanType == TIMER_CHAN_TYPE_ONESHOT) && (lptmrChannelState->periodExpired))
        {
            /* In the case channel type is one-shot and period is expired, set time elapsed to be period value*/
            timeElapsed = lptmrChannelState->period;
        }
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);

        uint32_t ftmInstance = instance->instIdx;
        const FTM_Type * const base = ftmBase[ftmInstance];
        uint32_t ftmchanStartVal = 0U;
        uint32_t ftmChanPeriod = 0U;
        uint32_t finalValue = 0U;
        timer_chan_type_t ftmChanType;
        const timer_chan_state_t * ftmChannelState;

        /* Get the final value of counter */
        finalValue = FTM_DRV_GetMod(base);
        ftmChannelState = &s_ftmState[ftmInstance][channel];
        /* Get channel start value */
        ftmchanStartVal = ftmChannelState->chanStartVal;
        /* Get current FTM counter value */
        currentCounter = FTM_DRV_GetCounter(base);
        /* Get channel period */
        ftmChanPeriod = ftmChannelState->period;
        /* Get channel type */
        ftmChanType = ftmChannelState->chanType;
        /* Calculate time elapsed */
        if (currentCounter >= ftmchanStartVal)
        {
            /* In case the counter is smaller than the final value of counter */
            timeElapsed = currentCounter - ftmchanStartVal;
        }
        else
        {
            /* In case the counter is over the final value of counter */
            timeElapsed = (finalValue - ftmchanStartVal) + currentCounter;
        }

        if ((ftmChanPeriod < timeElapsed) && (ftmChanType == TIMER_CHAN_TYPE_CONTINUOUS))
        {
            /* If the timer channel type is continuous and the timeElapsed is bigger than the period, the counter overflows the current period.
               For this case, timeElapsed is the time elapsed of the next period, but not the current period */
            timeElapsed %= ftmChanPeriod;
        }

        if ((ftmChanType == TIMER_CHAN_TYPE_ONESHOT) && (ftmChannelState->periodExpired))
        {
            /* In the case channel type is one-shot and period is expired, set time elapsed to be period value */
            timeElapsed = ftmChanPeriod;
        }
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        uint32_t pitInstance = instance->instIdx;
        const timer_chan_state_t * pitChannelState;

        /* Get current channel counter value */
        currentCounter = PIT_DRV_GetCurrentTimerCount(pitInstance, channel);

        pitChannelState = &s_pitState[pitInstance][channel];
        /* Calculate time elapsed */
        timeElapsed = pitChannelState->period - currentCounter;
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT(channel < STM_CHANNEL_COUNT);

        uint32_t stmInstance = instance->instIdx;
        const timer_chan_state_t * stmChannelState;

        /* Get current counter value */
        currentCounter = STM_DRV_GetCounterValue(stmInstance);
        stmChannelState = &s_stmState[stmInstance][channel];
        /* Calculate time elapsed */
        if (currentCounter >= (stmChannelState->chanStartVal))
        {
            /* In case the counter is smaller than the final value of counter */
            timeElapsed = currentCounter - stmChannelState->chanStartVal;
        }
        else
        {
            /* In case the counter is over the final value of counter */
            timeElapsed = (STM_COMPARE_MAX - stmChannelState->chanStartVal) + currentCounter;
        }
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

    return timeElapsed;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_GetRemaining
 * Description   : Get remaining ticks.
 * This function gets remaining time to next event by ticks. The remaining time by nanosecond, microsecond or
 * millisecond is the result of this function multiplies by the result of the TIMING_GetResolution function.
 * Note that:
 * If the timer channel type is continuous, this function may not return 0 at the moment period is timeout depending on timer frequency,
 * optimizations, etc. The behavior occurs if the execution time of the function is significant relative to timer tick duration.
 * If the timer channel type is one-shot, this function can be used to check whether the current period is timeout, in this case
 * if the returned value is 0, the current period is timeout or overflowed.
 *
 * Implements    : TIMING_GetRemaining_Activity
 *END**************************************************************************/
uint32_t TIMING_GetRemaining(const timing_instance_t * const instance,
                             const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

    uint32_t timeElapsed = 0U;
    uint32_t timeRemain = 0U;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        uint32_t lpitInstance = instance->instIdx;
        const timer_chan_state_t * lpitChannelState;
        (void)timeElapsed;

        lpitChannelState = &s_lpitState[lpitInstance][channel];
        /* Get the remaining time */
        timeRemain = LPIT_DRV_GetCurrentTimerCount(instance->instIdx, channel);

        if ((lpitChannelState->chanType == TIMER_CHAN_TYPE_ONESHOT) && (lpitChannelState->periodExpired))
        {
            /* In the case channel type is one-shot and period is expired, set time remain to be 0 */
            timeRemain = 0U;
        }

    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);
        uint32_t lptmrInstance = instance->instIdx;
        const timer_chan_state_t * lptmrChannelState;

        /* Time elapsed is current counter value */
        timeElapsed = LPTMR_DRV_GetCounterValueByCount(lptmrInstance);

        lptmrChannelState = &s_lptmrState[lptmrInstance][channel];
        /* Calculate the remaining time */
        timeRemain = lptmrChannelState->period - timeElapsed;

        if ((lptmrChannelState->chanType == TIMER_CHAN_TYPE_ONESHOT) && (lptmrChannelState->periodExpired))
        {
            /* In the case channel type is one-shot and period is expired, set time remain to be 0 */
            timeRemain = 0U;
        }
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);

        uint32_t ftmInstance = instance->instIdx;
        const FTM_Type * const base = ftmBase[ftmInstance];
        uint32_t ftmCurrentCounter = 0U;
        uint32_t ftmchanStartVal = 0U;
        uint32_t ftmChanPeriod = 0U;
        uint32_t finalValue = 0U;
        const timer_chan_state_t * ftmChannelState;

        /* Get the final value of counter */
        finalValue = FTM_DRV_GetMod(base);
        ftmChannelState = &s_ftmState[ftmInstance][channel];
        /* Get channel start value */
        ftmchanStartVal = ftmChannelState->chanStartVal;
        /* Get current FTM counter value */
        ftmCurrentCounter = FTM_DRV_GetCounter(base);
        /* Get channel period */
        ftmChanPeriod = ftmChannelState->period;
        /* Calculate time elapsed */
        if (ftmCurrentCounter >= ftmchanStartVal)
        {
            /* In case the counter is smaller than the final value of counter */
            timeElapsed = ftmCurrentCounter - ftmchanStartVal;
        }
        else
        {
            /* In case the counter is over the final value of counter */
            timeElapsed = (finalValue - ftmchanStartVal) + ftmCurrentCounter;
        }
        /* Get the remaining time */
        if(ftmChanPeriod >= timeElapsed)
        {
            timeRemain = ftmChanPeriod - timeElapsed;
        }
        else
        {
            /* If the timer channel type is continuous and the timeElapsed is bigger than the period, the counter overflows the current period.
               For this case, timeRemain is the time remaining of the next period, but not the current period */
            timeRemain = ftmChanPeriod - (timeElapsed % ftmChanPeriod);
        }

        if ((ftmChannelState->chanType == TIMER_CHAN_TYPE_ONESHOT) && (ftmChannelState->periodExpired))
        {
            /* Set time remain to be 0 if the period is expired */
            timeRemain = 0U;
        }
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        (void)timeElapsed;

        /* Get the remaining time */
        timeRemain = PIT_DRV_GetCurrentTimerCount(instance->instIdx, channel);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT(channel < STM_CHANNEL_COUNT);

        uint32_t stmInstance = instance->instIdx;
        const timer_chan_state_t * stmChannelState;

        /* Get current counter value */
        uint32_t stmCurrentCounter = STM_DRV_GetCounterValue(stmInstance);
        stmChannelState = &s_stmState[stmInstance][channel];
        /* Calculate time elapsed */
        if ((stmCurrentCounter >= stmChannelState->chanStartVal))
        {
            /* In case the counter is smaller than the final value of counter */
            timeElapsed = stmCurrentCounter - stmChannelState->chanStartVal;
        }
        else
        {
            /* In case the counter is over the final value of counter */
            timeElapsed = (STM_COMPARE_MAX - stmChannelState->chanStartVal) + stmCurrentCounter;
        }
        /* Get the remaining time */
        timeRemain = stmChannelState->period - timeElapsed;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

    return timeRemain;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_EnableNotification
 * Description   : Enable channel notifications.
 * This function enables channel notification.
 *
 * Implements    : TIMING_EnableNotification_Activity
 *END**************************************************************************/
void TIMING_EnableNotification(const timing_instance_t * const instance,
                               const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

    timer_chan_state_t * channelState;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        DEV_ASSERT((instance->instIdx) < LPIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPIT_TMR_COUNT);

        channelState = &s_lpitState[instance->instIdx][channel];
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT((instance->instIdx) < LPTMR_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);

        uint32_t lptmrInstance = instance->instIdx;

        channelState = &s_lptmrState[lptmrInstance][channel];
        /* Enable notification */
        channelState->enableNotification = true;

    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);

        uint32_t ftmInstance = instance->instIdx;

        channelState = &s_ftmState[ftmInstance][channel];
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        DEV_ASSERT((instance->instIdx) < PIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < PIT_TIMER_COUNT);

        uint32_t pitInstance = instance->instIdx;

        channelState = &s_pitState[pitInstance][channel];
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT((instance->instIdx) < STM_INSTANCE_COUNT);
        DEV_ASSERT(channel < STM_CHANNEL_COUNT);

        uint32_t stmInstance = instance->instIdx;

        channelState = &s_stmState[stmInstance][channel];
        /* Enable notification */
        channelState->enableNotification = true;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_DisableNotification
 * Description   : Disable channel notifications
 * This function disables channel notification.
 *
 * Implements    : TIMING_DisableNotification_Activity
 *END**************************************************************************/
void TIMING_DisableNotification(const timing_instance_t * const instance,
                                const uint8_t channel)
{
    DEV_ASSERT(instance != NULL);

    timer_chan_state_t * channelState;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        DEV_ASSERT((instance->instIdx) < LPIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPIT_TMR_COUNT);

        channelState = &s_lpitState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT((instance->instIdx) < LPTMR_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);

        channelState = &s_lptmrState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;

    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);

        channelState = &s_ftmState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        DEV_ASSERT((instance->instIdx) < PIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < PIT_TIMER_COUNT);

        channelState = &s_pitState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT((instance->instIdx) < STM_INSTANCE_COUNT);
        DEV_ASSERT(channel < STM_CHANNEL_COUNT);

        channelState = &s_stmState[instance->instIdx][channel];
        /* Disable notification */
        channelState->enableNotification = false;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_GetResolution
 * Description   : Get tick resolution
 * This function gets tick resolution in engineering units (nanosecond, microsecond or millisecond).
 * The result of this function is used to calculate period, remaining time or elapsed time in engineering units.
 *
 * Implements    : TIMING_GetResolution_Activity
 *END**************************************************************************/
status_t TIMING_GetResolution(const timing_instance_t * const instance,
                              const timer_resolution_type_t type,
                              uint64_t * const resolution)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(resolution != NULL);

    status_t status = STATUS_SUCCESS;
    uint32_t clkFrequency = 0U;
    uint64_t prescaler = 1U;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        static const clock_names_t lpitClockName[LPIT_INSTANCE_COUNT] = {LPIT0_CLK};
        status_t clkErr;

        DEV_ASSERT((instance->instIdx) < LPIT_INSTANCE_COUNT);
        /* Gets current functional clock frequency of LPIT instance */
        clkErr = CLOCK_SYS_GetFreq(lpitClockName[instance->instIdx], &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT((instance->instIdx) < LPTMR_INSTANCE_COUNT);
        /* Get lptmr clock frequency */
        TIMING_GetLptmrFreq(instance->instIdx, &clkFrequency, &prescaler);
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);

        uint32_t ftmInstance = instance->instIdx;

        /* Gets current functional clock frequency of FTM instance */
        clkFrequency = FTM_DRV_GetFrequency(ftmInstance);
        DEV_ASSERT(clkFrequency > 0U);
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        DEV_ASSERT((instance->instIdx) < PIT_INSTANCE_COUNT);

        status_t clkErr;
        uint32_t pitInstance = instance->instIdx;
        /* Gets current functional clock frequency of PIT instance */
        clkErr = CLOCK_SYS_GetFreq(s_pitClockName[pitInstance], &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT((instance->instIdx) < STM_INSTANCE_COUNT);

        uint32_t stmInstance = instance->instIdx;
        status_t clkErr;
        clock_names_t inputClockName = CLOCK_NAME_COUNT;

#if FEATURE_STM_HAS_CLOCK_SELECTION
        switch(s_stmClockSource[stmInstance])
        {
        case STM_CLOCK_SYSTEM:
            inputClockName = FS80_CLK;
            break;
        case STM_CLOCK_FXOSC:
            inputClockName = FXOSC_CLK;
            break;
        default:
            /* Invalid clock source */
            DEV_ASSERT(false);
            break;
        }
#else
        inputClockName = PBRIDGEx_CLK;
#endif
        /* Gets current functional clock frequency of STM instance */
        clkErr = CLOCK_SYS_GetFreq(inputClockName, &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
        prescaler = (uint64_t)s_stmPrescaler[stmInstance] + 1UL;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

    if (type == TIMER_RESOLUTION_TYPE_NANOSECOND)
    {
        /* For better precision, add half of the frequency before the division */
        *resolution = ((1000000000U * prescaler) + ((uint64_t)clkFrequency >> 1U)) / clkFrequency;
    }
    else if (type == TIMER_RESOLUTION_TYPE_MICROSECOND)
    {
        /* For better precision, add half of the frequency before the division */
        *resolution = ((1000000U * prescaler) + ((uint64_t)clkFrequency >> 1U)) / clkFrequency;
    }
    else
    {
        /* For better precision, add half of the frequency before the division */
        *resolution = ((1000U * prescaler) + ((uint64_t)clkFrequency >> 1U)) / clkFrequency;
    }

    if(*resolution == 0U)
    {
        status = STATUS_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_GetMaxPeriod
 * Description   : Get max period in engineering units
 * This function get max period in engineering units.
 *
 * Implements    : TIMING_GetMaxPeriod_Activity
 *END**************************************************************************/
status_t TIMING_GetMaxPeriod(const timing_instance_t * const instance,
                             const timer_resolution_type_t type,
                             uint64_t * const maxPeriod)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(maxPeriod != NULL);

    status_t status = STATUS_SUCCESS;
    uint32_t clkFrequency = 0U;
    uint64_t prescaler = 1U;
    uint64_t maxCountValue = 0U;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        static const clock_names_t lpitClockName[LPIT_INSTANCE_COUNT] = {LPIT0_CLK};
        status_t clkErr;

        DEV_ASSERT((instance->instIdx) < LPIT_INSTANCE_COUNT);
        /* Set max count value of LPIT */
        maxCountValue = (uint64_t)LPIT_COMPARE_MAX + 1UL;
        /* Gets current functional clock frequency of LPIT instance */
        clkErr = CLOCK_SYS_GetFreq(lpitClockName[instance->instIdx], &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT((instance->instIdx) < LPTMR_INSTANCE_COUNT);
        /* Set max count value of LPTMR */
        maxCountValue = LPTMR_COMPARE_MAX + 1UL;
        /* Get lptmr clock frequency */
        TIMING_GetLptmrFreq(instance->instIdx, &clkFrequency, &prescaler);
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        uint32_t ftmInstance = instance->instIdx;

        /* Set max count value of FTM */
        maxCountValue = FTM_COMPARE_MAX + 1UL;
        /* Gets current functional clock frequency of FTM instance */
        clkFrequency = FTM_DRV_GetFrequency(ftmInstance);
        DEV_ASSERT(clkFrequency > 0U);
        status = STATUS_SUCCESS;
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        DEV_ASSERT((instance->instIdx) < PIT_INSTANCE_COUNT);

        status_t clkErr;
        uint32_t pitInstance = instance->instIdx;

        /* Set max count value of PIT */
        maxCountValue = (uint64_t)PIT_COMPARE_MAX + 1UL;
        /* Gets current functional clock frequency of PIT instance */
        clkErr = CLOCK_SYS_GetFreq(s_pitClockName[pitInstance], &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT((instance->instIdx) < STM_INSTANCE_COUNT);

        status_t clkErr;
        uint32_t stmInstance = instance->instIdx;
        clock_names_t inputClockName = CLOCK_NAME_COUNT;

        /* Set max count value of STM */
        maxCountValue = (uint64_t)STM_COMPARE_MAX + 1UL;

#if FEATURE_STM_HAS_CLOCK_SELECTION
        /* Select name of clock source*/
        switch(s_stmClockSource[stmInstance])
        {
        case STM_CLOCK_SYSTEM:
            inputClockName = FS80_CLK;
            break;
        case STM_CLOCK_FXOSC:
            inputClockName = FXOSC_CLK;
            break;
        default:
            /* Invalid clock source */
            DEV_ASSERT(false);
            break;
        }
#else
        inputClockName = PBRIDGEx_CLK;
#endif
        /* Gets current functional clock frequency of STM instance */
        clkErr = CLOCK_SYS_GetFreq(inputClockName, &clkFrequency);
        /* Checks the functional clock module is available */
        (void)clkErr;
        DEV_ASSERT(clkErr == STATUS_SUCCESS);
        DEV_ASSERT(clkFrequency > 0U);
        prescaler = (uint64_t)s_stmPrescaler[stmInstance] + 1UL;
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

    if (type == TIMER_RESOLUTION_TYPE_NANOSECOND)
    {
        /* For better precision, add half of the frequency before the division.
           To avoid overflow, divided the clock frequency before multiples by the max count value */
        *maxPeriod = (((1000000000U * prescaler) + ((uint64_t)clkFrequency >> 1U)) / clkFrequency) * maxCountValue;
    }
    else if (type == TIMER_RESOLUTION_TYPE_MICROSECOND)
    {
        /* For better precision, add half of the frequency before the division. */
        *maxPeriod = ((1000000U * prescaler * maxCountValue) + ((uint64_t)clkFrequency >> 1U)) / clkFrequency;
    }
    else
    {   /* For better precision, add half of the frequency before the division. */
        *maxPeriod = ((1000U * prescaler * maxCountValue) +  ((uint64_t)clkFrequency >> 1U)) / clkFrequency;
    }

    if(*maxPeriod == 0U)
    {
        status = STATUS_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_InstallCallback
 * Description   : Install new callback function and callback parameter for timer channel.
 *
 * Implements    : TIMING_InstallCallback_Activity
 *END**************************************************************************/
void TIMING_InstallCallback(const timing_instance_t * const instance,
                           const uint8_t channel,
                           const timer_callback_t callback,
                           void * const callbackParam)
{
    DEV_ASSERT(instance != NULL);

    timer_chan_state_t * channelState;

    /* Define TIMING PAL over LPIT */
#if (defined (TIMING_OVER_LPIT))
    if (instance->instType == TIMING_INST_TYPE_LPIT)
    {
        DEV_ASSERT((instance->instIdx) < LPIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPIT_TMR_COUNT);

        channelState = &s_lpitState[instance->instIdx][channel];

        /* Disable interrupt generating to avoid timer event appears just between
           the write of the new function address and the new parameter */
        INT_SYS_DisableIRQ(s_lpitIrq[instance->instIdx][channel]);
        /* update new callback function */
        channelState->callback = callback;
        /* Update new callback parameter */
        channelState->callbackParam = callbackParam;
        /* Enable interrupt after the updating of callback and parameter*/
        INT_SYS_EnableIRQ(s_lpitIrq[instance->instIdx][channel]);
    }
    else
#endif

    /* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
    if (instance->instType == TIMING_INST_TYPE_LPTMR)
    {
        DEV_ASSERT((instance->instIdx) < LPTMR_INSTANCE_COUNT);
        DEV_ASSERT(channel < LPTMR_TMR_COUNT);

        channelState = &s_lptmrState[instance->instIdx][channel];

        /* Disable interrupt generating to avoid timer event appears just between
           the write of the new function address and the new parameter */
        INT_SYS_DisableIRQ(s_lptmrIrq[instance->instIdx][channel]);
        /* update new callback function */
        channelState->callback = callback;
        /* Update new callback parameter */
        channelState->callbackParam = callbackParam;
        /* Enable interrupt after the updating of callback and parameter*/
        INT_SYS_EnableIRQ(s_lptmrIrq[instance->instIdx][channel]);
    }
    else
#endif

    /* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
    if (instance->instType == TIMING_INST_TYPE_FTM)
    {
        DEV_ASSERT((instance->instIdx) < FTM_INSTANCE_COUNT);
        DEV_ASSERT(channel < FTM_CONTROLS_COUNT);

        channelState = &s_ftmState[instance->instIdx][channel];

        /* Disable interrupt generating to avoid timer event appears just between
           the write of the new function address and the new parameter */
        INT_SYS_DisableIRQ(s_ftmIrq[instance->instIdx][channel]);
        /* update new callback function */
        channelState->callback = callback;
        /* Update new callback parameter */
        channelState->callbackParam = callbackParam;
        /* Enable interrupt after the updating of callback and parameter*/
        INT_SYS_EnableIRQ(s_ftmIrq[instance->instIdx][channel]);
    }
    else
#endif

    /* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
    if (instance->instType == TIMING_INST_TYPE_PIT)
    {
        DEV_ASSERT((instance->instIdx) < PIT_INSTANCE_COUNT);
        DEV_ASSERT(channel < PIT_TIMER_COUNT);

        channelState = &s_pitState[instance->instIdx][channel];

        /* Disable interrupt generating to avoid timer event appears just between
           the write of the new function address and the new parameter */
        INT_SYS_DisableIRQ(s_pitIrq[instance->instIdx][channel]);
        /* update new callback function */
        channelState->callback = callback;
        /* Update new callback parameter */
        channelState->callbackParam = callbackParam;
        /* Enable interrupt after the updating of callback and parameter*/
        INT_SYS_EnableIRQ(s_pitIrq[instance->instIdx][channel]);
    }
    else
#endif

    /* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
    if (instance->instType == TIMING_INST_TYPE_STM)
    {
        DEV_ASSERT((instance->instIdx) < STM_INSTANCE_COUNT);
        DEV_ASSERT(channel < STM_CHANNEL_COUNT);

        channelState = &s_stmState[instance->instIdx][channel];

        /* Disable interrupt generating to avoid timer event appears just between
           the write of the new function address and the new parameter */
        INT_SYS_DisableIRQ(s_stmIrq[instance->instIdx][channel]);
        /* update new callback function */
        channelState->callback = callback;
        /* Update new callback parameter */
        channelState->callbackParam = callbackParam;
        /* Enable interrupt after the updating of callback and parameter*/
        INT_SYS_EnableIRQ(s_stmIrq[instance->instIdx][channel]);
    }
    else
#endif
    {
        DEV_ASSERT(false);
    }

}

#if (defined (TIMING_OVER_LPIT))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Lpit_IrqHandler
 * Description   : Interrupt handler for TIMING over LPIT.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void TIMING_Lpit_IrqHandler(uint32_t instance, uint8_t channel)
{
    timer_chan_state_t * channelState = &s_lpitState[instance][channel];

    LPIT_DRV_ClearInterruptFlagTimerChannels(instance, (1UL << channel));

    if (channelState->chanType == TIMER_CHAN_TYPE_ONESHOT)
    {
        uint32_t channelMask = 1UL << channel;
        /* Stop the channel counting */
        LPIT_DRV_StopTimerChannels(instance, channelMask);
        /* Set the flag to notify the period is expired*/
        channelState->periodExpired = true;
    }

    if ((channelState->callback != NULL) && (channelState->enableNotification))
    {
        /* Call to callback function */
        (channelState->callback)(channelState->callbackParam);
    }

}
#endif

/* Define TIMING PAL over LPTMR */
#if (defined (TIMING_OVER_LPTMR))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Lptmr_IrqHandler
 * Description   : Interrupt handler for TIMING over LPTMR.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void TIMING_Lptmr_IrqHandler(uint32_t instance, uint8_t channel)
{
    timer_chan_state_t * channelState = &s_lptmrState[instance][channel];

    LPTMR_DRV_ClearCompareFlag(instance);

    if (channelState->chanType == TIMER_CHAN_TYPE_ONESHOT)
    {
        /* Stop the channel counting */
        LPTMR_DRV_StopCounter(instance);
        /* Set the flag to notify the period is expired*/
        channelState->periodExpired = true;
    }

    if ((channelState->callback != NULL) && (channelState->enableNotification))
    {
        /* Call to callback function */
        (channelState->callback)(channelState->callbackParam);
    }

}
#endif

/* Define TIMING PAL over FTM */
#if (defined (TIMING_OVER_FTM))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Ftm_IrqHandler
 * Description   : Interrupt handler for TIMING over FTM.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void TIMING_Ftm_IrqHandler(uint32_t instance, uint8_t channel)
{
    FTM_Type * const base = ftmBase[instance];
    timer_chan_state_t * channelState = &s_ftmState[instance][channel];
    /* Clear interrupt flag */
    FTM_DRV_ClearChnEventStatus(base, channel);

    /* Check channel type */
    if (channelState->chanType == TIMER_CHAN_TYPE_ONESHOT)
    {
        /* Stop the channel by disable interrupt generation */
        FTM_DRV_DisableInterrupts(instance, (1UL << channel));
        /* Update channel running status */
        g_ftmChannelRunning[instance][channel] = false;
        /* Set the flag to notify the period is expired*/
        channelState->periodExpired = true;
    }
    else
    {
        status_t status;
        uint32_t currentCmpValue = 0U;
        uint32_t currentPeriod = channelState->period;
        uint32_t nexCompareValue = 0U;
        uint32_t finalValue;

        /* Get the final value of counter */
        finalValue = FTM_DRV_GetMod(base);
        /* Get current counter value */
        currentCmpValue = FTM_DRV_GetCounter(base);
        /* Calculate the next compare value of the channel */
        if ((finalValue - currentCmpValue) > currentPeriod)
        {
            nexCompareValue = currentCmpValue + currentPeriod;
        }
        else
        {
            nexCompareValue = currentPeriod - (finalValue - currentCmpValue);
        }
        /* Update next compare value to the channel */
        status = FTM_DRV_UpdateOutputCompareChannel(instance, channel, (uint16_t)nexCompareValue, FTM_ABSOLUTE_VALUE, false);
        (void)status;
        /* Save the start value of channel at the moment new period is started */
        channelState->chanStartVal = currentCmpValue;
    }
    if ((channelState->callback != NULL) && (channelState->enableNotification))
    {
        /* Call to callback function */
        (channelState->callback)(channelState->callbackParam);
    }

}
#endif

/* Define TIMING PAL over PIT */
#if (defined (TIMING_OVER_PIT))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Pit_IrqHandler
 * Description   : Interrupt handler for TIMING over PIT.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void TIMING_Pit_IrqHandler(uint32_t instance, uint8_t channel)
{
    const timer_chan_state_t * channelState = &s_pitState[instance][channel];

    if ((channelState->callback != NULL) && (channelState->enableNotification))
    {
        /* Call to callback function */
        (channelState->callback)(channelState->callbackParam);
        /* Check notification type */
        if (channelState->chanType == TIMER_CHAN_TYPE_ONESHOT)
        {
            /* Stop the channel counting */
            PIT_DRV_StopChannel(instance, channel);
        }
    }
    PIT_DRV_ClearStatusFlags(instance, channel);
}
#endif

/* Define TIMING PAL over STM */
#if (defined (TIMING_OVER_STM))
/*FUNCTION**********************************************************************
 *
 * Function Name : TIMING_Stm_IrqHandler
 * Description   : Interrupt handler for TIMING over STM.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void TIMING_Stm_IrqHandler(uint32_t instance, uint8_t channel)
{
#ifndef STM_0
    uint32_t stmInstance = instance - 1U;
#else
    uint32_t stmInstance = instance;
#endif
    uint32_t currentCounter = 0U;
    timer_chan_state_t * channelState = &s_stmState[stmInstance][channel];

    /* Get current counter value */
    currentCounter = STM_DRV_GetCounterValue(stmInstance);

    if ((channelState->callback != NULL) && (channelState->enableNotification))
    {
        /* Call to callback function */
        (channelState->callback)(channelState->callbackParam);
        /* Save the start value of channel at the moment new period is started */
        channelState->chanStartVal = currentCounter;
    }
    /* Check notification type */
    if (channelState->chanType == TIMER_CHAN_TYPE_ONESHOT)
    {
        /* Stop the channel counting */
        STM_DRV_DisableChannel(stmInstance, channel);
    }
    else
    {
        /* Update next compare value of the channel*/
        STM_DRV_IncrementTicks(stmInstance, channel, channelState->period);
    }
    STM_DRV_ClearStatusFlags(stmInstance, channel);
}
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
