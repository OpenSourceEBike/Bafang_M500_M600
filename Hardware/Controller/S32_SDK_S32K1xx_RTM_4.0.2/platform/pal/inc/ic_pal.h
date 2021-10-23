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
 * @file ic_pal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * flash_swap_callback_t can be used in user code.
 *
 */

#ifndef IC_PAL_H
#define IC_PAL_H

#include <stddef.h>
#include "ic_pal_mapping.h"
#include "ic_pal_cfg.h"
#include "status.h"
#include "callbacks.h"


/*! @file ic_pal.h */

/*!
 * @defgroup ic_pal IC PAL
 * @ingroup ic_pal
 * @addtogroup ic_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (defined(IC_PAL_OVER_EMIOS))
/*!
 * @brief Select either one of the counter buses or the internal counter to be used by
 * the Unified Channel.
 * Implements : ic_bus_select_t_Class
 */
typedef enum
{
    IC_BUS_SEL_A        = 0x00U,    /*!< Global counter bus A */
    IC_BUS_SEL_B        = 0x01U,    /*!< Local counter bus B */
    IC_BUS_SEL_C        = 0x02U,    /*!< Local counter bus C */
    IC_BUS_SEL_D        = 0x03U,    /*!< Local counter bus D */
    IC_BUS_SEL_E        = 0x04U,    /*!< Local counter bus E */
    IC_BUS_SEL_F        = 0x05U,    /*!< Global counter bus F */
    IC_BUS_SEL_INTERNAL = 0x06U     /*!< Internal counter bus */
} ic_bus_select_t;

/*!
 * @brief Internal pre-scaler factor selection for the clock source.
 * Implements : ic_internal_ps_t_Class
 */
typedef enum
{
    IC_CLOCK_DIVID_BY_1  = 0x00U,    /*!< Divide by 1 */
    IC_CLOCK_DIVID_BY_2  = 0x01U,    /*!< Divide by 2 */
    IC_CLOCK_DIVID_BY_3  = 0x02U,    /*!< Divide by 3 */
    IC_CLOCK_DIVID_BY_4  = 0x03U,    /*!< Divide by 4 */
    IC_CLOCK_DIVID_BY_5  = 0x04U,    /*!< Divide by 5 */
    IC_CLOCK_DIVID_BY_6  = 0x05U,    /*!< Divide by 6 */
    IC_CLOCK_DIVID_BY_7  = 0x06U,    /*!< Divide by 7 */
    IC_CLOCK_DIVID_BY_8  = 0x07U,    /*!< Divide by 8 */
    IC_CLOCK_DIVID_BY_9  = 0x08U,    /*!< Divide by 9 */
    IC_CLOCK_DIVID_BY_10 = 0x09U,    /*!< Divide by 10 */
    IC_CLOCK_DIVID_BY_11 = 0x0AU,    /*!< Divide by 11 */
    IC_CLOCK_DIVID_BY_12 = 0x0BU,    /*!< Divide by 12 */
    IC_CLOCK_DIVID_BY_13 = 0x0CU,    /*!< Divide by 13 */
    IC_CLOCK_DIVID_BY_14 = 0x0DU,    /*!< Divide by 14 */
    IC_CLOCK_DIVID_BY_15 = 0x0EU,    /*!< Divide by 15 */
    IC_CLOCK_DIVID_BY_16 = 0x0FU     /*!< Divide by 16 */
} ic_internal_ps_t;
#endif

#if (defined(IC_PAL_OVER_ETIMER) || defined(IC_PAL_OVER_FLEXPWM))
/*!
 * @brief Input source options.
 *
 * Each 16-bit counter or timer channel contains a prescaler to IP Bus clock.
 *
 * Implements : ic_source_prescaler_t_Class
 */
typedef enum
{
    IC_CLOCK_DIVID_1   = 0x18U,    /*!< Use Bus Clock divided by 1 as input source  */
    IC_CLOCK_DIVID_2   = 0x19U,    /*!< Use Bus Clock divided by 2 as input source  */
    IC_CLOCK_DIVID_4   = 0x1AU,    /*!< Use Bus Clock divided by 4 as input source  */
    IC_CLOCK_DIVID_8   = 0x1BU,    /*!< Use Bus Clock divided by 8 as input source  */
    IC_CLOCK_DIVID_16  = 0x1CU,    /*!< Use Bus Clock divided by 16 as input source  */
    IC_CLOCK_DIVID_32  = 0x1DU,    /*!< Use Bus Clock divided by 32 as input source  */
    IC_CLOCK_DIVID_64  = 0x1EU,    /*!< Use Bus Clock divided by 64 as input source  */
    IC_CLOCK_DIVID_128 = 0x1FU,    /*!< Use Bus Clock divided by 128 as input source  */
} ic_source_prescaler_t;
#endif

/*!
 * @brief The measurement type for input capture mode
 * Implements : ic_option_mode_t_Class
 */
typedef enum
{
    IC_DISABLE_OPERATION           = 0x00U,    /*!< Have no operation */
    IC_TIMESTAMP_RISING_EDGE       = 0x01U,    /*!< Rising edge trigger */
    IC_TIMESTAMP_FALLING_EDGE      = 0x02U,    /*!< Falling edge trigger */
    IC_TIMESTAMP_BOTH_EDGES        = 0x03U,    /*!< Rising and falling edge trigger */
    IC_MEASURE_RISING_EDGE_PERIOD  = 0x04U,    /*!< Period measurement between two consecutive rising edges */
    IC_MEASURE_FALLING_EDGE_PERIOD = 0x05U,    /*!< Period measurement between two consecutive falling edges */
    IC_MEASURE_PULSE_HIGH          = 0x06U,    /*!< The time measurement taken for the pulse to remain ON or HIGH state */
    IC_MEASURE_PULSE_LOW           = 0x07U     /*!< The time measurement taken for the pulse to remain OFF or LOW state */
} ic_option_mode_t;

/*!
 * @brief The configuration structure of input capture parameters for each channel
 *
 * Implements : ic_input_ch_param_t_Class
 */
typedef struct
{
    uint8_t hwChannelId;                  /*!< Physical hardware channel ID */
    ic_option_mode_t inputCaptureMode;    /*!< Input capture mode of operation */
    bool filterEn;                        /*!< Input capture filter state */
    uint16_t filterValue;                 /*!< Filter Value */
    void * channelExtension;              /*!< The IP specific configuration structure for channel */
    void * channelCallbackParams;         /*!< The parameter of callback application for channels event */
    ic_callback_t channelCallbacks;       /*!< The callback function for channels event */
} ic_input_ch_param_t;

/*!
 * @brief Defines the configuration structures are used in the input capture mode
 *
 * Implements : ic_config_t_Class
 */
typedef struct
{
    uint8_t nNumChannels;                         /*!< Number of input capture channel used */
    const ic_input_ch_param_t * inputChConfig;    /*!< Input capture channels configuration */
    void * extension;                             /*!< IP specific configuration structure */
} ic_config_t;

#if (defined(IC_PAL_OVER_FTM))
/*!
 * @brief Defines the extension structure for the channel configuration over FTM
 *
 * Part of FTM channel configuration structure
 * Implements : channel_extension_ftm_for_ic_t_Class
 */
typedef struct
{
    bool continuousModeEn;    /*!< Continuous measurement state */
} channel_extension_ftm_for_ic_t;

/*!
 * @brief Defines the extension structure for the input capture mode over FTM
 *
 * Part of FTM configuration structure
 * Implements : extension_ftm_for_ic_t_Class
 */
typedef struct
{
    ftm_clock_source_t ftmClockSource;    /*!< Select clock source for FTM */
    ftm_clock_ps_t ftmPrescaler;          /*!< Register pre-scaler options available in the
                                           *   ftm_clock_ps_t enumeration */
} extension_ftm_for_ic_t;
#endif

#if (defined(IC_PAL_OVER_EMIOS))
/*!
 * @brief Defines the extension structure for the channel configuration over EMIOS
 *
 * Part of EMIOS channel configuration structure
 * Implements : channel_extension_emios_for_ic_t_Class
 */
typedef struct
{
    ic_bus_select_t timebase;      /*!< Counter bus selected */
    ic_internal_ps_t prescaler;    /*!< Internal prescaler, pre-scale channel clock by internalPrescaler + 1 */
} channel_extension_emios_for_ic_t;

/*!
 * @brief Defines the extension structure for the input capture mode over EMIOS
 *
 * Part of EMIOS configuration structure
 * Implements : extension_emios_for_ic_t_Class
 */
typedef struct
{
    uint16_t clkDivVal;            /*!< Select the clock divider value for the global prescaler in range (1-256) */
    bool enableGlobalPrescaler;    /*!< Enable global prescaler or disable */
    bool enableGlobalTimeBase;     /*!< Enable global timebase or disable */
} extension_emios_for_ic_t;
#endif

#if (defined(IC_PAL_OVER_ETIMER))
/*!
 * @brief Defines the extension structure for the channel configuration over ETIMER
 *
 * Part of ETIMER channel configuration structure
 * Implements : channel_extension_etimer_for_ic_t_Class
 */
typedef struct
{
    ic_source_prescaler_t prescaler;    /*!< IP Bus clock divide by prescaler */
    bool continuousModeEn;              /*!< Continuous measurement state */
    uint8_t rate;                       /*!< The rate at which the input is samples.
                                             Represents the sampling period (in peripheral clock cycles) of the eTimer input signal. */
} channel_extension_etimer_for_ic_t;
#endif

#if (defined(IC_PAL_OVER_FLEXPWM))
/*!
 * @brief Defines the extension structure for the channel configuration over FLEXPWM
 *
 * Part of ETIMER channel configuration structure
 * Implements : channel_extension_flexpwm_for_ic_t_Class
 */
typedef struct
{
    flexpwm_clock_ps_t prescaler;      /*!< IP Bus clock divide by prescaler */
    bool continuousModeEn;             /*!< Continuous measurement state */
    uint8_t rate;                      /*!< The rate at which the input is samples.
                                            Represents the sampling period (in peripheral clock cycles) of the eTimer input signal. */
} channel_extension_flexpwm_for_ic_t;
#endif

/*!
 * @brief The internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the IC_Init() function, then
 * it cannot be freed until the driver is de-initialized using IC_Deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint8_t nNumChannels;                                            /*!< Number of input capture channel used */
    ic_option_mode_t icChannelType[IC_PAL_NUM_OF_CHANNEL_MAX];       /*!< Input capture mode of operation */
    uint8_t channelConfigArray[IC_PAL_NUM_OF_CHANNEL_MAX];           /*!< Store the hardware channel IDs are used in capture mode */
    bool enableContinuousMode[IC_PAL_NUM_OF_CHANNEL_MAX];            /*!< To save channels enable the continuous mode */
#if (defined(IC_PAL_OVER_EMIOS))
    emios_bus_select_t timeBaseSelection[IC_PAL_NUM_OF_CHANNEL_MAX]; /*!< To save channels are used which timebase */
    emios_input_filter_t filterInput[IC_PAL_NUM_OF_CHANNEL_MAX];     /*!< Filter Value, ignore if not select external clock mode */
    bool filterEn[IC_PAL_NUM_OF_CHANNEL_MAX];                        /*!< Input capture filter state, ignore if not select external clock mode */
#endif
#if (defined(IC_PAL_OVER_ETIMER) || defined(IC_PAL_OVER_FLEXPWM))
    uint16_t measurementResults[IC_PAL_NUM_OF_CHANNEL_MAX];          /*!< This field is used only to store value of edges time stamps */
#endif
    bool enableNotification[IC_PAL_NUM_OF_CHANNEL_MAX];              /*!< To save channels enable the notification on the callback application */
    void * channelCallbackParams[IC_PAL_NUM_OF_CHANNEL_MAX];         /*!< To save the parameter of callback application */
    ic_callback_t channelCallbacks[IC_PAL_NUM_OF_CHANNEL_MAX];       /*!< To save the callback function */
/*! @endcond */
} ic_pal_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the input capture mode
 *
 * This function will initialize the IC PAL instance, including the
 * other platform specific HW units used together in the input capture mode. This
 * function configures a group of channels in instance to detect or measure the
 * input signal.
 * @Note: If the filter input is enabled on the channel 0,1,2 or 3 over FTM. The filter pre-scaler
 * will be configured to divide by 4. The maximum frequency for the channel input to be detected
 * correctly is FTM input clock divided by 16.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] configPtr The pointer to configuration structure.
 * @return Operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t IC_Init(const ic_instance_t * const instance,
                 const ic_config_t * configPtr);

/*!
 * @brief De-initialize a input capture instance
 *
 * This function will disable the input capture mode. The driver
 * can't be used again until reinitialized. The context structure is no longer
 * needed by the driver and can be freed after calling this function.
 *
 * @param[in] instance The pointer to instance number structure.
 * @return Operation status
 *         - STATUS_SUCCESS: Operation was successful
 */
status_t IC_Deinit(const ic_instance_t * const instance);

/*!
 * @brief Start the counter
 *
 * This function start channel counting.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 */
void IC_StartChannel(const ic_instance_t * const instance,
                     uint8_t channel);

/*!
 * @brief Stop the counter
 *
 * This function stop channel counting.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 */
void IC_StopChannel(const ic_instance_t * const instance,
                    uint8_t channel);

/*!
 * @brief Get the measured value
 *
 * This function will get the value of measured signal in ticks.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 * @return The last value of measured signal in ticks.
 */
status_t IC_SetChannelMode(const ic_instance_t * const instance,
                           uint8_t channel,
                           ic_option_mode_t channelMode);

/*!
 * @brief Get the measured value
 *
 * This function will get the value of measured signal in ticks.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 * @return The last value of measured signal in ticks.
 */
uint16_t IC_GetMeasurement(const ic_instance_t * const instance,
                           uint8_t channel);

/*!
 * @brief Enable channel notifications
 *
 * This function enables channel notification.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 */
void IC_EnableNotification(const ic_instance_t * const instance,
                           uint8_t channel);

/*!
 * @brief Disable channel notifications
 *
 * This function disables channel notification.
 *
 * @param[in] instance The pointer to instance number structure.
 * @param[in] channel The channel number.
 */
void IC_DisableNotification(const ic_instance_t * const instance,
                            uint8_t channel);

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* IC_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
