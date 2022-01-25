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

#ifndef PWM_PAL_H
#define PWM_PAL_H

#include "status.h"
#include <stdint.h>
#include <stdbool.h>
#include "callbacks.h"
#include "pwm_pal_mapping.h"
#include "pwm_pal_cfg.h"
/* Include PD files */
/* Include PD files for FTM. */
#if (defined (PWM_OVER_FTM))
    #include "ftm_pwm_driver.h"
    #include "ftm_common.h"
#endif

/* Include PD files for EMIOS. */
#if (defined (PWM_OVER_EMIOS))
    #include "emios_common.h"
    #include "emios_mc_driver.h"
    #include "emios_pwm_driver.h"
    #include "emios_hw_access.h"
#endif

/* Include PD files for ETIMER. */
#if (defined (PWM_OVER_ETIMER))
    #include "etimer_driver.h"
#endif

/*! @file pwm_pal.h */

/*!
 * @ingroup pwm_pal
 * @addtogroup pwm_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the channel types
 * Implements : pwm_channel_type_t_Class
 */
typedef enum
{
    PWM_EDGE_ALIGNED   = 0, /*!< Counter used by this type of channel is in up counting mode and the edge is aligned to PWM period */
    PWM_CENTER_ALIGNED = 1, /*!< Counter used by this type of channel is in up-down counting mode and the duty is inserted in center of PWM period */
} pwm_channel_type_t;

/*!
 * @brief Defines the polarity of pwm channels
 * Implements : pwm_polarity_t_Class
 */
typedef enum
{
    PWM_ACTIVE_HIGH = 0, /*!< Polarity is active high */
    PWM_ACTIVE_LOW  = 1  /*!< Polarity is active low */
} pwm_polarity_t;

/*!
 * @brief Defines the polarity of complementary pwm channels relative to main channel
 * Implements : pwm_complementarty_mode_t_Class
 */
typedef enum
{
    PWM_DUPLICATED = 0,   /*!< Complementary channel is the same as main channel */
    PWM_INVERTED   = 1    /*!< Complementary channel is inverted relative to main channel */
} pwm_complementarty_mode_t;

#if defined PWM_OVER_FTM

/*!
 * @brief This structure is specific for platforms where FTM is available.
 * Implements : pwm_ftm_timebase_t_Class
 */
typedef struct
{
    ftm_clock_source_t sourceClock;         /*!< Clock source for FTM timebase */
    ftm_clock_ps_t     prescaler;           /*!< Prescaler for FTM timebase */
    ftm_deadtime_ps_t  deadtimePrescaler;   /*!< Prescaler for FTM dead-time insertion */
} pwm_ftm_timebase_t;
#endif

#if defined PWM_OVER_EMIOS
/*!
 * @brief This enumeration contains all clock buses available for EMIOS.
 * Implements : pwm_emios_timebase_name_t_Class
 */
typedef enum
{
    BUS_B = 0U,
    BUS_C = 8U,
    BUS_D = 16U,
    BUS_F = 22U,
    BUS_A = 23U,
    BUS_E = 24U
} pwm_emios_timebase_name_t;

/*!
 * @brief This structure is specific for platforms where EMIOS is available and it contains the
 * configuration of a single timebase.
 * Implements : pwm_emios_timebase_t_Class
 */
typedef struct
{
    pwm_emios_timebase_name_t name;                 /*!< Timebase name */
    emios_clock_internal_ps_t internalPrescaler;    /*!< Internal prescaler,  pre-scale channel clock by internalPrescaler +1 */
} pwm_emios_timebase_t;
#endif

#if (defined (PWM_OVER_ETIMER))

/*!
 * @brief This structure is specific for platforms where ETIMER is available and it contains the
 * configuration of a single timebase.
 * Implements : pwm_etimer_timebase_t_Class
 */
typedef struct
{
    bool                         halfClkPeriod;     /*!< Automatically half the input clock period, effectively double the running frequency  */
    etimer_input_config_t        pwmClkSrc;         /*!< Clock source of PWM channel    */
    bool                         enableOutSig;      /*!< Enable the PWM signal on the output pin  */
} pwm_etimer_timebase_t;

#endif

/*!
 * @brief This structure includes the configuration for each channel
 * Implements : pwm_channel_t_Class
 */
typedef struct
{
    uint8_t                     channel;                        /*!< Channel number */
    pwm_channel_type_t          channelType;                    /*!< Channel waveform type */
    uint32_t                    period;                         /*!< Period of the PWM signal in ticks */
    uint32_t                    duty;                           /*!< Duty cycle in ticks */
    pwm_polarity_t              polarity;                       /*!< Channel polarity */
    bool                        insertDeadtime;                 /*!< Enable/disable dead-time insertion. This  feature is available only if complementary mode is enabled */
    uint8_t                     deadtime;                       /*!< Dead-time value in ticks*/
    bool                        enableComplementaryChannel;     /*!< Enable a complementary channel. This option can take control over other channel than the channel configured in this structure. */
    pwm_complementarty_mode_t   complementaryChannelPolarity;   /*!< Configure the polarity of the complementary channel relative to the main channel */
    void*                       timebase;                       /*!< This field is platform specific and it's used to configure the clocking tree for different time-bases.
                                                                     If FTM is use this field must be filled by a pointer to pwm_ftm_timebase_t */
} pwm_channel_t;

/*!
 * @brief This structure is the configuration for initialization of PWM channels.
 * Implements : pwm_global_config_t_Class
 */
typedef struct
{
    pwm_channel_t*  pwmChannels;          /*!< Pointer to channels configurations */
    uint8_t         numberOfPwmChannels;  /*!< Number of channels which are configured */
} pwm_global_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize PWM channels based on config parameter.
 *
 * @param[in] instance The name of the instance
 * @param[in] config The configuration structure used to initialize PWM modules
 * @return    Error or success status returned by API
 */
 status_t PWM_Init(const pwm_instance_t * const instance, const pwm_global_config_t* config);

 /*!
 * @brief Update duty cycle. The measurement unit for duty is clock ticks.
 *
 * @param[in] instance The name of the instance
 * @param[in] channel The channel which is update
 * @param[in] duty The duty cycle measured in ticks
 * @return    Error or success status returned by API
 */
 status_t PWM_UpdateDuty(const pwm_instance_t * const instance, uint8_t channel, uint32_t duty);

/*!
 * @brief  Update period for specific a specific channel. This function changes period for
 * all channels which shares the timebase with targeted channel.
 *
 * @param[in] instance The name of the instance
 * @param[in] channel The channel which is update
 * @param[in] period The period measured in ticks
 * @return    Error or success status returned by API
 */
 status_t PWM_UpdatePeriod(const pwm_instance_t * const instance, uint8_t channel, uint32_t period);

 /*!
 * @brief This function change the output value for some channels. channelsMask select
 * which channels will be overwrite, each bit filed representing one channel: 1 - channel is controlled
 * by channelsValues, 0 - channel is controlled by pwm. channelsValues select output values to be write on corresponding
 * channel.
 * For PWM_PAL over FTM, when enable complementary channels, if this function is used to force output of complementary channels(n and n+1)
 * with value is high, the output of channel n is going to be high and the output of channel n+1 is going to be low.
 * Please refer to Software ouput control behavior table in the reference manual to get more detail.
 *
 * @param[in] instance The name of the instance
 * @param[in] channelsMask The name mask used to select which channel is overwrite
 * @param[in] channelsValues The name overwrite values for all channels
 * @return    Error or success status returned by API
 */
 status_t PWM_OverwriteOutputChannels(const pwm_instance_t * const instance, uint32_t channelsMask, uint32_t channelsValues);

 /*!
 * @brief De-Initialize PWM instance.
 *
 * @param[in] instance The name of the instance
 * @return    Error or success status returned by API
 */
 status_t PWM_Deinit(const pwm_instance_t * const instance);

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* PWM_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
