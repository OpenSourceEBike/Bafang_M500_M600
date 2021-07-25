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
 * @file timing_pal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * The types are defined here to be used by other drivers or applications.
 *
 *
 */

#ifndef TIMING_PAL_H
#define TIMING_PAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "timing_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*!
 * @defgroup timing_pal TIMING PAL
 * @ingroup timing_pal
 * @addtogroup timing_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Type options available for tick resolution
 *
 * Implements : timer_resolution_type_t_Class
 */
typedef enum
{
    TIMER_RESOLUTION_TYPE_NANOSECOND,   /*!< Tick resolution is nanosecond  */
    TIMER_RESOLUTION_TYPE_MICROSECOND,  /*!< Tick resolution is microsecond */
    TIMER_RESOLUTION_TYPE_MILISECOND    /*!< Tick resolution is millisecond  */
} timer_resolution_type_t;

/*!
 * @brief Type options available for timer channel notification
 *
 * Implements : timer_chan_type_t_Class
 */
typedef enum
{
    TIMER_CHAN_TYPE_CONTINUOUS, /*!< Timer channel creates continuous notification */
    TIMER_CHAN_TYPE_ONESHOT     /*!< Timer channel creates one-shot notification   */
} timer_chan_type_t;

/*!
 * @brief Structure to configure the channel timer notification
 *
 * This structure holds the configuration settings for the timer channel notification
 * Implements : timer_chan_config_t_Class
 */
typedef struct
{
    uint8_t channel;            /*!< Channel number */
    timer_chan_type_t chanType; /*!< Continuous or One-shot */
    timer_callback_t callback;  /*!< Callback function called on notification */
    void * callbackParam;       /*!< Callback parameter pointer*/
} timer_chan_config_t;

/*!
 * @brief Timer configuration structure
 *
 * This structure holds the configuration settings for the timer
 * Implements : timer_config_t_Class
 */
typedef struct
{
    const timer_chan_config_t * chanConfigArray; /*!< Channel configuration array */
    uint8_t numChan;                             /*!< Number of elements in chanConfigArray */
    void * extension;                            /*!< IP specific configuration structure */
} timer_config_t;


/*
 * The extension member of timer_config_t structure should be:
 * - NULL if timer is LPIT or PIT
 * - extension_lptmr_for_timer_t if timer is LPTMR
 * - extension_stm_for_timer_t if timer is STM
 * - extension_ftm_for_timer_t if timer is FTM
 */

#if (defined(TIMING_OVER_LPTMR))
/*!
 * @brief Defines the extension structure for the timer over LPTMR
 *
 * Part of LPTMR configuration structure
 * Implements : extension_lptmr_for_timer_t_Class
 */
typedef struct
{
    lptmr_clocksource_t clockSelect; /*!< LPTMR clock source selection */
    lptmr_prescaler_t prescaler;     /*!< Prescaler Selection */
    bool bypassPrescaler;            /*!< Enable/Disable prescaler bypass */
} extension_lptmr_for_timer_t;
#endif

#if (defined(TIMING_OVER_FTM))
/*!
 * @brief Defines the extension structure for the timer over FTM
 *
 * Part of FTM configuration structure
 * Implements : extension_ftm_for_timer_t_Class
 */
typedef struct
{
    ftm_clock_source_t clockSelect;  /*!< FTM clock source selection */
    ftm_clock_ps_t prescaler;        /*!< Prescaler Selection */
    uint16_t finalValue;             /*!< The final value of FTM counter */
} extension_ftm_for_timer_t;
#endif

#if (defined(TIMING_OVER_STM))
/*!
 * @brief Defines the extension structure for the timer over STM
 *
 * Part of STM configuration structure
 * Implements : extension_stm_for_timer_t_Class
 */
typedef struct
{
#if FEATURE_STM_HAS_CLOCK_SELECTION
    stm_clock_source_t clockSelect;  /*!< STM clock source selection */
#endif
    uint8_t prescaler;               /*!< Prescaler Selection */
} extension_stm_for_timer_t;
#endif

/*!
 * @brief Initialize the timer instance and timer channels with value from input configuration structure
 *
 * This function initializes clock source, prescaler of the timer instance(except LPIT, PIT), the final
 * value of counter (only FTM). This function also setups notification type and callback function of timer channel.
 * The timer instance number and its configuration structure shall be passed as arguments.
 * Timer channels do not start counting by default after calling this function.
 * The function TIMING_StartChannel must be called to start the timer channel counting.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] config The pointer to configuration structure
 * @return Operation status
 *         - STATUS_SUCCESS: Operation was successful
 *         - STATUS_ERROR  : Operation was fail if the timer instance is out of range
 *                           For example: Timing over LPIT but the instance is not LPIT instance(TIMING_OVER_LPIT0_INSTANCE)
 *         - STATUS_ERROR  : Operation was fail if the FTM instance has been initialized
 */
status_t TIMING_Init(const timing_instance_t * const instance,
                     const timer_config_t * const config);

/*!
 * @brief De-initialize a timer instance
 *
 * This function de-initializes timer instance.
 * In order to use the timer instance again, TIMING_Init must be called.
 *
 * @param[in] instance The pointer to timer instance number structure
 */
void TIMING_Deinit(const timing_instance_t * const instance);

/*!
 * @brief Starts the timer channel counting.
 *
 * This function starts channel counting with a new period in ticks.
 * Note that:
 * - If the timer is PIT or LPIT, to abort the current timer channel period and start a timer channel period with a
 *   new value, the timer channel must be stopped and started again.
 * - If the timer is FTM, this function start channel by enable channel interrupt generation.
 * - LPTMR and FTM is 16 bit timer, so the input period must be smaller than 65535.
 * - LPTMR and FTM is 16 bit timer, so the input period must be smaller than 65535.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 * @param[in] periodTicks The input period in ticks
 */
void TIMING_StartChannel(const timing_instance_t * const instance,
                         const uint8_t channel,
                         const uint32_t periodTicks);

/*!
 * @brief Stop the timer channel counting.
 *
 * This function stop channel counting. Note that if the timer is FTM, this function stop channel by disable channel interrupt
 * generation.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 */
void TIMING_StopChannel(const timing_instance_t * const instance,
                        const uint8_t channel);

/*!
 * @brief Get elapsed ticks
 *
 * This function gets elapsed time of the current period by ticks. The elapsed time by nanosecond, microsecond or
 * millisecond is the result of this function multiplies by the result of the TIMING_GetResolution function.
 * Note that:
 * If the timer channel type is continuous, this function may not return value of the period at the moment period is timeout depending on timer frequency,
 * optimizations, etc. The behavior occurs if the execution time of the function is significant relative to timer tick duration.
 * If the timer channel type is one-shot, this function can be used to check whether the current period is timeout, in this case
 * if the returned value is bigger or equal than the period, the current period is timeout or overflowed.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 * @return Number of ticks elapsed of the current period
 */
uint32_t TIMING_GetElapsed(const timing_instance_t * const instance,
                           const uint8_t channel);

/*!
 * @brief Get remaining ticks
 *
 * This function gets remaining time of the current period by ticks. The remaining time by nanosecond, microsecond or
 * millisecond is the result of this function multiplies by the result of the TIMING_GetResolution function.
 * Note that:
 * If the timer channel type is continuous, this function may not return 0 at the moment period is timeout depending on timer frequency,
 * optimizations, etc. The behavior occurs if the execution time of the function is significant relative to timer tick duration.
 * If the timer channel type is one-shot, this function can be used to check whether the current period is timeout, in this case
 * if the returned value is 0, the current period is timeout or overflowed.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 * @return Number of ticks remaining of the current period
 */
uint32_t TIMING_GetRemaining(const timing_instance_t * const instance,
                             const uint8_t channel);

/*!
 * @brief Enable channel notifications
 *
 * This function enables channel notification.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 */
void TIMING_EnableNotification(const timing_instance_t * const instance,
                               const uint8_t channel);

/*!
 * @brief Disable channel notifications
 *
 * This function disables channel notification.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] channel The channel number
 */
void TIMING_DisableNotification(const timing_instance_t * const instance,
                                const uint8_t channel);

/*!
 * @brief Get tick resolution
 *
 * This function gets tick resolution in engineering units (nanosecond, microsecond or millisecond). The result of this function is used to
 * calculate period, remaining time or elapsed time in engineering units.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] type Resolution type
 * @param[out] resolution The pointer to resolution in engineering units
 * @return Operation status
 *         - STATUS_SUCCESS: Operation was successful
 *         - STATUS_ERROR  : The timer frequency is not fit to resolution type
 */
status_t TIMING_GetResolution(const timing_instance_t * const instance,
                              const timer_resolution_type_t type,
                              uint64_t * const resolution);

/*!
 * @brief Get max period in engineering units
 *
 * This function gets max period in engineering units.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] type Resolution type
 * @param[out] maxPeriod The pointer to max period in engineering units
 * @return Operation status
 *         - STATUS_SUCCESS: Operation was successful
 *         - STATUS_ERROR  : The timer frequency is not fit to resolution type
 */
status_t TIMING_GetMaxPeriod(const timing_instance_t * const instance,
                             const timer_resolution_type_t type,
                             uint64_t * const maxPeriod);

/*!
 * @brief Installs callback function for the timer channel.
 *
 * This function installs new callback function and callback parameter for the timer channel event.
 * This function allows changing the callback function and parameter while the timer channel is running.
 * If the provided callback function parameter is NULL, it is equivalent to removing the callback.
 *
 * @param[in] instance The pointer to timer instance number structure
 * @param[in] callback The new callback function for timer channel
 * @param[in] callbackParam The new callback parameter pointer
 */
void TIMING_InstallCallback(const timing_instance_t * const instance,
                           const uint8_t channel,
                           const timer_callback_t callback,
                           void * const callbackParam);

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* TIMING_PAL_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
