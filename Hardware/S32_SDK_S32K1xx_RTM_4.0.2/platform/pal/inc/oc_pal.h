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

#ifndef OC_PAL_H
#define OC_PAL_H

#include "oc_pal_mapping.h"
#include <stddef.h>
#include "status.h"
#include "callbacks.h"

/*! @file oc_pal.h */

/*!
 * @defgroup oc_pal OC PAL
 * @ingroup oc_pal
 * @addtogroup oc_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (defined(OC_PAL_OVER_EMIOS))
/*!
 * @brief Select either one of the counter buses or the internal counter to be used by
 * the Unified Channel.
 * Implements : oc_bus_select_t_Class
 */
typedef enum
{
    OC_BUS_SEL_A        = 0x00U,    /*!< Global counter bus A */
    OC_BUS_SEL_B        = 0x01U,    /*!< Local counter bus B */
    OC_BUS_SEL_C        = 0x02U,    /*!< Local counter bus C */
    OC_BUS_SEL_D        = 0x03U,    /*!< Local counter bus D */
    OC_BUS_SEL_E        = 0x04U,    /*!< Local counter bus E */
    OC_BUS_SEL_F        = 0x05U,    /*!< Global counter bus F */
    OC_BUS_SEL_INTERNAL = 0x06U     /*!< Internal counter bus */
} oc_bus_select_t;

#endif


/*!
 * @brief The type of comparison for output compare mode
 * Implements : oc_option_mode_t_Class
 */
typedef enum
{
    OC_DISABLE_OUTPUT  = 0x00U,    /*!< No action on output pin */
    OC_TOGGLE_ON_MATCH = 0x01U,    /*!< Toggle on match */
    OC_CLEAR_ON_MATCH  = 0x02U,    /*!< Clear on match */
    OC_SET_ON_MATCH    = 0x03U     /*!< Set on match */
} oc_option_mode_t;

/*!
 * @brief The type of update on the channel match
 * Implements : oc_option_update_t_Class
 */
typedef enum
{
    OC_RELATIVE_VALUE = 0x00U,    /*!< Next compared value is relative to current value */
    OC_ABSOLUTE_VALUE = 0x01U     /*!< Next compared value is absolute */
} oc_option_update_t;

/*!
 * @brief The configuration structure of output compare parameters for each channel
 *
 * Implements : oc_output_ch_param_t_Class
 */
typedef struct
{
    uint8_t hwChannelId;            /*!< Physical hardware channel ID */
    oc_option_mode_t chMode;        /*!< Channel output mode*/
    uint16_t comparedValue;         /*!< The compared value */
    void * channelExtension;        /*!< The IP specific configuration structure for channel */
    void * channelCallbackParams;   /*!< The parameter of callback application for channels event */
    oc_callback_t channelCallbacks; /*!< The callback function for channels event */
} oc_output_ch_param_t;

/*!
 * @brief Defines the configuration structures are used in the output compare mode
 *
 * Implements : oc_config_t_Class
 */
typedef struct
{
    uint8_t nNumChannels;                           /*!< Number of output compare channel used */
    const oc_output_ch_param_t * outputChConfig;    /*!< Output compare channels configuration */
    void * extension;                               /*!< IP specific configuration structure */
} oc_config_t;

#if (defined(OC_PAL_OVER_FTM))
/*!
 * @brief Defines the extension structure for the output compare mode over FTM
 *
 * Part of FTM configuration structure
 * Implements : extension_ftm_for_oc_t_Class
 */
typedef struct
{
    uint16_t maxCountValue;               /*!< Maximum count value in ticks */
    ftm_clock_source_t ftmClockSource;    /*!< Select clock source for FTM */
    ftm_clock_ps_t ftmPrescaler;          /*!< Register pre-scaler options available in the
                                           *   ftm_clock_ps_t enumeration */
} extension_ftm_for_oc_t;
#endif

#if (defined(OC_PAL_OVER_EMIOS))
/*!
 * @brief Defines the extension structure for the channel configuration over EMIOS
 *
 * Part of EMIOS channel configuration structure
 * Implements : channel_extension_emios_for_oc_t_Class
 */
typedef struct
{
    oc_bus_select_t timebase;      /*!< Counter bus selected */
    uint32_t period;               /*!< If up mode period = A1, period = 2(A1) with MC up/down mode,
                                    *   period = 2(A1) -2 with MCB up/down mode */
    emios_clock_internal_ps_t prescaler;    /*!< Internal prescaler, pre-scale channel clock by internalPrescaler + 1 */
} channel_extension_emios_for_oc_t;

/*!
 * @brief Defines the extension structure for the output compare mode over EMIOS
 *
 * Part of eMIOS configuration structure
 * Implements : extension_emios_for_oc_t_Class
 */
typedef struct
{
    uint16_t clkDivVal;            /*!< Select the clock divider value for the global prescaler in range (1-256) */
    bool enableGlobalPrescaler;    /*!< Enable global prescaler or disable */
    bool enableGlobalTimeBase;     /*!< Enable global timebase or disable */
} extension_emios_for_oc_t;
#endif

#if (defined(OC_PAL_OVER_ETIMER))
/*!
 * @brief Defines the extension structure for the channel configuration over ETIMER
 *
 * Part of ETIMER channel configuration structure
 * Implements : channel_extension_etimer_for_oc_t_Class
 */
typedef struct
{
    etimer_input_config_t primaryInput;     /*!< Primary input of the channel */
    etimer_output_config_t outputPin;       /*!< Output pin of the channel  */
} channel_extension_etimer_for_oc_t;
#endif

#if (defined(OC_PAL_OVER_FLEXPWM))
/*!
 * @brief Defines the extension structure for the channel configuration over FLEXPWM
 *
 * Part of FLEXPWM channel configuration structure
 * Implements : channel_extension_flexpwm_for_oc_t_Class
 */
typedef struct
{
    flexpwm_clock_src_t clkSrc;      /*!< Clock source for the counter. */
    flexpwm_clock_ps_t clkPrescaler;    /*!< Pre-scaler to divide down the clock. */
    bool enDebugMode;                /*!< Debug mode*/
    uint16_t maxCountValue;          /*!< Maximum count value in ticks. */
} channel_extension_flexpwm_for_oc_t;
#endif
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the output compare mode
 *
 * This function will initialize the OC PAL instance, including the
 * other platform specific HW units used together in the output compare mode. This
 * function configures a group of channels in instance to set, clear toggle the
 * output signal.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] configPtr The pointer to configuration structure.
 * @return Operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t OC_Init(const oc_instance_t * const instance,
                 const oc_config_t * const configPtr);

/*!
 * @brief De-initialize the output compare instance
 *
 * This function will disable the output compare mode. The driver
 * can't be used again until reinitialized. The context structure is no longer
 * needed by the driver and can be freed after calling this function.
 *
 * @param[in] instance The output compare instance number.
 * @return Operation status
 *         - STATUS_SUCCESS: Operation was successful
 */
status_t OC_Deinit(const oc_instance_t * const instance);

/*!
 * @brief Start the counter
 *
 * This function start channel counting.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] channel The channel number.
 */
void OC_StartChannel(const oc_instance_t * const instance,
                     const uint8_t channel);

/*!
 * @brief Stop the counter
 *
 * This function stop channel output.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] channel The channel number.
 */
void OC_StopChannel(const oc_instance_t * const instance,
                    const uint8_t channel);

/*!
 * @brief Control the channel output by software
 *
 * This function is used to forces the output pin to a specified
 * value.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] channel The channel number.
 * @param[in] outputValue The output value:
 *            - false : The software output control forces 0 to the channel output.
 *            - true : The software output control forces 1 to the channel output.
 * @return Operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t OC_SetOutputState(const oc_instance_t * const instance,
                           const uint8_t channel,
                           bool outputValue);

/*!
 * @brief Set the operation mode of channel output
 *
* This function will set the action executed on a compare
 * match value to set output pin, clear output pin, toggle output pin.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] channel The channel number.
 * @param[in] channelMode The channel mode in output compare:
 *            - OC_DISABLE_OUTPUT : No action on output pin
 *            - OC_TOGGLE_ON_MATCH : Toggle on match
 *            - OC_CLEAR_ON_MATCH : Clear on match
 *            - OC_SET_ON_MATCH : Set on match
 * @return Operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t OC_SetOutputAction(const oc_instance_t * const instance,
                            const uint8_t channel,
                            oc_option_mode_t channelMode);

/*!
 * @brief Update the match value on the channel
 *
 * This function will update the value of an output compare channel to
 * the counter matches to this value.
 *
 * @param[in] instance The output compare instance number.
 * @param[in] channel The channel number.
 * @param[in] nextCompareMatchValue The timer value in ticks until the next compare match event should be appeared.
 * @param[in] typeOfupdate The type of update:
 *        - OC_RELATIVE_VALUE : nextCompareMatchValue will be added to current counter value into the channel value register
 *        - OC_ABSOLUTE_VALUE : nextCompareMatchValue will be written into the channel value register
 * @return Operation status
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t OC_SetCompareValue(const oc_instance_t * const instance,
                            const uint8_t channel,
                            uint32_t nextCompareMatchValue,
                            oc_option_update_t typeOfupdate);

/*!
 * @brief Enable channel notifications
 *
 * This function enables channel notification.
 *
 * @param[in] instance The output compare instance number
 * @param[in] channel The channel number
 */
void OC_EnableNotification(const oc_instance_t * const instance,
                           const uint8_t channel);

/*!
 * @brief Disable channel notifications
 *
 * This function disables channel notification.
 *
 * @param[in] instance The output compare instance number
 * @param[in] channel The channel number
 */
void OC_DisableNotification(const oc_instance_t * const instance,
                            const uint8_t channel);

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* OC_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
