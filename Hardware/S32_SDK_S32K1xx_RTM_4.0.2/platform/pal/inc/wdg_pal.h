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
 * @file wdg_pal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Type used only in some modules of the SDK.
 *
 */

#ifndef WDG_PAL_H
#define WDG_PAL_H

#include "status.h"
#include "wdg_pal_mapping.h"
#include "wdg_pal_cfg.h"

/*!
 * @defgroup wdg_pal_code WDG PAL
 * @ingroup wdg_pal
 * @brief Watchdog Peripheral Abstraction Layer.
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Clock sources for the WDG PAL.
 * Implements : wdg_clock_source_t_Class
 */
typedef enum
{
    WDG_PAL_BUS_CLOCK     = 0x00U, /*!< Bus clock */
    WDG_PAL_LPO_CLOCK     = 0x01U, /*!< LPO clock */
    WDG_PAL_SOSC_CLOCK    = 0x02U, /*!< SOSC clock */
    WDG_PAL_SIRC_CLOCK    = 0x03U  /*!< SIRC clock */
} wdg_clock_source_t;

/*!
 * @brief WDG PAL option mode configuration structure
 * Implements : wdg_option_mode_t_Class
 */
typedef struct
{
    bool    wait;  /*!< Wait mode */
    bool    stop;  /*!< Stop mode */
    bool    debug; /*!< Debug mode */
} wdg_option_mode_t;

#if defined(WDG_OVER_EWM)
/*!
 * @brief WDG PAL input pin configuration
 * Configures if the input pin is enabled and when is asserted
 * Implements : wdg_in_assert_logic_t_Class
 */
typedef enum
{
    WDG_IN_ASSERT_DISABLED      = 0x00U, /*!< Input pin disabled                    */
    WDG_IN_ASSERT_ON_LOGIC_ZERO = 0x01U, /*!< Input pin asserts EWM when on logic 0 */
    WDG_IN_ASSERT_ON_LOGIC_ONE  = 0x02U  /*!< Input pin asserts EWM when on logic 1 */
} wdg_in_assert_logic_t;

/*!
 * @brief The extension structure for the EWM over WDOG peripheral
 * Implements : extension_ewm_for_wdg_t_Class
 */
typedef struct
{
    wdg_in_assert_logic_t    assertLogic;    /*!< Assert logic for EWM input pin */
    uint8_t                  prescalerValue; /*!< EWM clock prescaler */
} extension_ewm_for_wdg_t;
#endif /* defined(WDG_OVER_EWM) */

/*!
 * @brief WDG PAL configuration structure
 * Implements : wdg_config_t_Class
 */
typedef struct
{
    wdg_clock_source_t    clkSource;        /*!< The clock source of the WDOG */
    wdg_option_mode_t     opMode;           /*!< The modes in which the WDOG is functional */
    uint32_t              timeoutValue;     /*!< The timeout value */
    uint8_t               percentWindow;    /*!< The window value compares to timeout value. Maximum value is 100 */
    bool                  intEnable;        /*!< If true, an interrupt request is generated before reset */
    bool                  winEnable;        /*!< If true, window mode is enabled */
    bool                  prescalerEnable;  /*!< If true, prescaler is enabled( default prescaler = 256) */
#if defined(WDG_OVER_EWM)
    void                  *extension;       /*!< This field will be add extra settings to EWM's configuration */
#endif
} wdg_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name WDG PAL API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the WDG PAL.
 *
 * This function initializes the WDG instance by user configuration
 *
 * @param[in] instance The name of the instance.
 * @param[in] configPtr Pointer to the WDG PAL user configuration structure
 * @return operation status
 *        - STATUS_SUCCESS : Operation was successful.
 *        - STATUS_ERROR   : Operation failed. Possible causes: previous
 *        clock source or the one specified in the configuration structure is
 *        disabled; WDG PAL configuration updates are not allowed.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_Init(const wdg_instance_t * const instance,
                  const wdg_config_t * configPtr);

/*!
 * @brief Gets default configuration of the WDG PAL.
 *
 * This function gets the default configuration of the WDG PAL.
 *
 * @param[out] configures the default configuration
 */
void WDG_GetDefaultConfig(wdg_config_t * const config);

/*!
 * @brief Refreshes the WDG PAL counter.
 *
 * This function resets the WDG PAL counter
 *
 * @param[in] instance The name of the instance.
 */
void WDG_Refresh(const wdg_instance_t * const instance);

/*!
 * @brief De-initializes the WDG PAL
 *
 * This function resets all configuration to default and disable the WDG PAL instance.
 *
 * @param[in] instance  The name of the instance.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_Deinit(const wdg_instance_t * const instance);

/*!
 * @brief Set interrupt for WDG PAL.
 *
 * This function enables/disables the WDG PAL timeout interrupt and sets a function to be
 * called when a timeout interrupt is received, before reset.
 *
 * @param[in] instance The name of the instance.
 * @param[in] enable
 *            - true  : Enable interrupt
 *            - false : Disable interrupt
 * @return operation status
 *        - STATUS_SUCCESS    : Operation was successful.
 *        - STATUS_ERROR      : Operation failed. Possible causes: failed to
 *          WDG PAL configuration updates not allowed.
 *        - STATUS_UNSUPPORTED: Operation was unsupported.
 */
status_t WDG_SetInt(const wdg_instance_t * const instance,
                    bool enable);

/*!
 * @brief Sets the value of the WDG PAL timeout.
 *
 * This function sets the value of the WDG PAL timeout.
 *
 * @param[in] instance The name of the instance.
 * @param[in] value The value of the WDG PAL timeout.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_SetTimeout(const wdg_instance_t * const instance,
                        uint32_t value);

/*!
 * @brief Set window mode and window value of the WDG PAL.
 *
 * This function set window mode, window value is set when window mode enabled.
 *
 * @param[in] instance The name of the instance.
 * @param[in] enable
 *            - true  : Enable window mode
 *            - false : Disable window mode
 * @param[in] value The value of the WDG PAL window.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_SetWindow(const wdg_instance_t * const instance,
                       bool enable,
                       uint32_t value);

/*!
 * @brief Gets the value of the WDG PAL counter.
 *
 * This function gets counter of WDG PAL module.
 * Note that: Counter will be reset to timeout value if WDG PAL uses SWT.
 * The counter will continue to run if WDG PAL uses WDOG.
 *
 * @param[in] instance The name of the instance.
 * @param[out] value Pointer to the counter value
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to SWT was lock by hard lock.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_GetCounter(const wdg_instance_t * const instance,
                        uint32_t * value);

/*!
 * @brief Clears the Timeout Interrupt Flag.
 *
 * This function clears the Timeout Interrupt Flag.
 *
 * @param[in] instance The name of the instance.
 */
void WDG_ClearIntFlag(const wdg_instance_t * const instance);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* WDG_PAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
