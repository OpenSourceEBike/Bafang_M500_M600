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
 * @file wdg_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a different essential type.
 * The cast is required expression assigned to a narrower or different essential type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast;
 * cannot cast from 'essentially enum<i>' to 'essentially enum<i>'.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, The cast performed between a pointer to
 * object type and a pointer to a different object type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to pointer to other type
 * This is required for working with the common initialize function in initialize function.
 *
 */

#include "wdg_pal.h"
#include "stddef.h"

/* Include PD files */
#if defined(WDG_OVER_WDOG)
    #include "wdog_driver.h"
#endif

#if defined(WDG_OVER_EWM)
    #include "ewm_driver.h"
#endif

#if defined(WDG_OVER_SWT)
    #include "swt_driver.h"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @name Watchdog Peripheral Abstraction Layer
 * @{
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Init
 * Description   : Initialize the WDG PAL
 *
 * Implements    : WDG_Init_Activity
 *END**************************************************************************/
status_t WDG_Init(const wdg_instance_t * const instance,
                  const wdg_config_t * configPtr)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(configPtr != NULL);

    uint32_t windowValue = 0U;
    status_t status = STATUS_UNSUPPORTED;

    /* Calculate window value */
    if (true == configPtr->winEnable)
    {
        DEV_ASSERT(configPtr->percentWindow <= 100U);

        windowValue = (uint32_t)(((uint64_t)configPtr->percentWindow * configPtr->timeoutValue) / 100U);
    }

    /* Define WDG PAL over WDOG */
#if defined (WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Define structures for WDG PAL over WDOG */
        wdog_user_config_t wdogConfig =
        {
            .clkSource        = (wdog_clk_source_t)configPtr->clkSource,
            .opMode           =
            {
                .wait         = configPtr->opMode.wait,
                .stop         = configPtr->opMode.stop,
                .debug        = configPtr->opMode.debug
            },
            .updateEnable     = true,
            .intEnable        = configPtr->intEnable,
            .winEnable        = configPtr->winEnable,
            .windowValue      = (uint16_t)((configPtr->timeoutValue - windowValue) & 0xFFFFU),
            .timeoutValue     = (uint16_t)(configPtr->timeoutValue & 0xFFFFU),
            .prescalerEnable  = configPtr->prescalerEnable
        };

        /* Initialize WDOG instance */
        status = WDOG_DRV_Init(instance->instIdx, &wdogConfig);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over EWM */
#if defined (WDG_OVER_EWM)
    if (WDG_INST_TYPE_EWM == instance->instType)
    {
        /* Define structures for WDG PAL over EWM */
        ewm_init_config_t ewmConfig;

        /* Set value for EWM structure */
        ewmConfig.interruptEnable = configPtr->intEnable;

        /* Set value for input pin asserts and clock prescaler */
        if (NULL == configPtr->extension)
        {
            ewmConfig.assertLogic = EWM_IN_ASSERT_ON_LOGIC_ZERO;
            ewmConfig.prescaler   = 0xFFU;
        }
        else
        {
            ewmConfig.assertLogic = (ewm_in_assert_logic_t)((extension_ewm_for_wdg_t*)(configPtr->extension))->assertLogic;
            ewmConfig.prescaler   = ((extension_ewm_for_wdg_t*)(configPtr->extension))->prescalerValue;
        }

        /* Set value for compare low register */
        if (true == configPtr->winEnable)
        {
            ewmConfig.compareLow  = (uint8_t)((configPtr->timeoutValue - windowValue) & 0xFFU);
        }
        else
        {
            ewmConfig.compareLow  = FEATURE_EWM_CMPL_MIN_VALUE;
        }

        /* Set value for compare high register */
        if (configPtr->timeoutValue > FEATURE_EWM_CMPH_MAX_VALUE)
        {
            ewmConfig.compareHigh = FEATURE_EWM_CMPH_MAX_VALUE;
        }
        else
        {
            ewmConfig.compareHigh = (uint8_t)(configPtr->timeoutValue & 0xFFU);
        }

        /* Initialize EWM instance */
        status = EWM_DRV_Init(instance->instIdx, &ewmConfig);
    }
#endif /* defined (WDG_OVER_EWM) */

    /* Define WDG PAL over SWT */
#if defined (WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Define structures for WDG PAL over SWT */
        swt_user_config_t swtConfig =
        {
            .mapConfig    = 0xFFU,
            .invalidReset = false,
            .winEnable    = configPtr->winEnable,
            .intEnable    = configPtr->intEnable,
            .stop         = configPtr->opMode.stop,
            .debug        = configPtr->opMode.debug,
            .serviceMode  = SWT_FS_SEQ_MODE,
            .lockConfig   = SWT_UNLOCK,
            .timeoutValue = configPtr->timeoutValue,
            .windowValue  = windowValue,
            .initKey      = 0U
        };

        /* Initialize SWT instance */
        status = SWT_DRV_Init(instance->instIdx, &swtConfig);
    }
#endif /* defined (WDG_OVER_SWT) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_GetDefaultConfig
 * Description   : Gets default configuration of the WDG PAL
 *
 * Implements    : WDG_GetDefaultConfig_Activity
 *END**************************************************************************/
void WDG_GetDefaultConfig(wdg_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->opMode.debug    = false;
    config->opMode.wait     = false;
    config->opMode.stop     = false;
    config->percentWindow   = 0U;
    config->intEnable       = false;
    config->winEnable       = false;
    config->prescalerEnable = false;

    /* Define WDG PAL over WDOG */
#if (defined(WDG_OVER_WDOG) || defined(WDG_OVER_EWM))
    config->clkSource       = WDG_PAL_LPO_CLOCK;
    config->timeoutValue    = FEATURE_WDOG_TO_RESET_VALUE;
#endif /* (defined (WDG_OVER_WDOG) || defined(WDG_OVER_EWM)) */

    /* Define WDG PAL over EWM */
#if (defined(WDG_OVER_EWM))
    config->extension       = NULL;
#endif /* defined (WDG_OVER_EWM) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    config->clkSource       = WDG_PAL_SIRC_CLOCK;
    config->timeoutValue    = FEATURE_SWT_TO_RESET_VALUE;
#endif /* defined (WDG_OVER_SWT) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Refresh
 * Description   : Refreshes the WDG PAL counter.
 *
 * Implements    : WDG_Refresh_Activity
 *END**************************************************************************/
void WDG_Refresh(const wdg_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Refresh counter over WDOG module */
        WDOG_DRV_Trigger(instance->instIdx);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over EWM */
#if defined(WDG_OVER_EWM)
    if (WDG_INST_TYPE_EWM == instance->instType)
    {
        /* Refresh counter over EWM module */
        EWM_DRV_Refresh(instance->instIdx);
    }
#endif /* defined (WDG_OVER_EWM) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Refresh counter over SWT module */
        SWT_DRV_Service(instance->instIdx);
    }
#endif /* defined (WDG_OVER_SWT) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Deinit
 * Description   : De-initializes the WDG PAL
 *
 * Implements    : WDG_Deinit_Activity
 *END**************************************************************************/
status_t WDG_Deinit(const wdg_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* De-initiation over WDOG module */
        status = WDOG_DRV_Deinit(instance->instIdx);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* De-initiation over SWT module */
        status = SWT_DRV_Deinit(instance->instIdx);
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameter is not used*/
    (void)instance;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetInt
 * Description   : Set interrupt for WDG PAL module
 *
 * Implements    : WDG_SetInt_Activity
 *END**************************************************************************/
status_t WDG_SetInt(const wdg_instance_t * const instance,
                    bool enable)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Set interrupt over WDOG module */
        status = WDOG_DRV_SetInt(instance->instIdx, enable);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define  PAL PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Set interrupt over SWT module */
        status = SWT_DRV_SetIntConfig(instance->instIdx, enable);
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameters are not used*/
    (void)instance;
    (void)enable;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetTimeout
 * Description   : Sets the value of the WDG PAL timeout.
 *
 * Implements    : WDG_SetTimeout_Activity
 *END**************************************************************************/
status_t WDG_SetTimeout(const wdg_instance_t * const instance,
                        uint32_t value)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Set timeout value over WDOG module */
        status = WDOG_DRV_SetTimeout(instance->instIdx, (uint16_t)value);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Set timeout value over SWT module */
        status = SWT_DRV_SetTimeoutValue(instance->instIdx, value);
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameters are not used*/
    (void)instance;
    (void)value;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetWindow
 * Description   : Set window mode and window value of the WDG PAL.
 *
 * Implements    : WDG_SetWindow_Activity
 *END**************************************************************************/
status_t WDG_SetWindow(const wdg_instance_t * const instance,
                       bool enable,
                       uint32_t value)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Set window value over WDOG module */
        status = WDOG_DRV_SetWindow(instance->instIdx, enable, (uint16_t)value);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Set window value over SWT module */
        status = SWT_DRV_SetWindowConfig(instance->instIdx, enable, value);
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameters are not used*/
    (void)instance;
    (void)enable;
    (void)value;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_GetCounter
 * Description   : Gets the value of the WDG PAL counter.
 * Note that: Counter will be reset to timeout value if WDG PAL uses SWT.
 * The counter will continue to run if WDG PAL uses WDOG.
 *
 * Implements    : WDG_GetCounter_Activity
 *END**************************************************************************/
status_t WDG_GetCounter(const wdg_instance_t * const instance,
                        uint32_t * value)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Get counter value over WDOG module */
        *value = WDOG_DRV_GetCounter(instance->instIdx);
        status = STATUS_SUCCESS;
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Disable SWT */
        status = SWT_DRV_StopTimer(instance->instIdx);
        /* Get counter value of SWT */
        if (STATUS_SUCCESS == status)
        {
            /* Get counter value over SWT module */
            (void)SWT_DRV_GetCounterValue(instance->instIdx, value);
            /* Enable SWT */
            (void)SWT_DRV_StartTimer(instance->instIdx);
        }
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameters are not used*/
    (void)instance;
    (void)value;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_ClearIntFlag
 * Description   : Clears the Timeout Interrupt Flag.
 *
 * Implements    : WDG_ClearIntFlag_Activity
 *END**************************************************************************/
void WDG_ClearIntFlag(const wdg_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (WDG_INST_TYPE_WDOG == instance->instType)
    {
        /* Clears interrupt flag over WDOG module */
        WDOG_DRV_ClearIntFlag(instance->instIdx);
    }
#endif /* defined (WDG_OVER_WDOG) */

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (WDG_INST_TYPE_SWT == instance->instType)
    {
        /* Clears interrupt flag over SWT module */
        SWT_DRV_ClearIntFlag(instance->instIdx);
    }
#endif /* defined (WDG_OVER_SWT) */

    /* Just eliminate warning if input parameter is not used*/
    (void)instance;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
