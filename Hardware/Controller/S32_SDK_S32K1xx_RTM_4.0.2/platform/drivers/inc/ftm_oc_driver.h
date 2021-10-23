/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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
 * @file ftm_oc_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 */

#ifndef FTM_OC_DRIVER_H
#define FTM_OC_DRIVER_H

#include "ftm_common.h"

/*!
 * @addtogroup ftm_oc_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief FlexTimer Mode configuration for output compare mode
 *
 * Implements : ftm_output_compare_mode_t_Class
 */
typedef enum
{
    FTM_DISABLE_OUTPUT  = 0x00U,    /*!< No action on output pin */
    FTM_TOGGLE_ON_MATCH = 0x01U,    /*!< Toggle on match */
    FTM_CLEAR_ON_MATCH  = 0x02U,    /*!< Clear on match */
    FTM_SET_ON_MATCH    = 0x03U     /*!< Set on match */
} ftm_output_compare_mode_t;

/*!
 * @brief FlexTimer input capture type of the next output compare value
 *
 * Implements : ftm_output_compare_update_t_Class
 */
typedef enum
{
    FTM_RELATIVE_VALUE = 0x00U,     /*!< Next compared value is relative to current value */
    FTM_ABSOLUTE_VALUE = 0x01U      /*!< Next compared value is absolute */
} ftm_output_compare_update_t;

/*!
 * @brief FlexTimer driver PWM parameters each channel in the output compare mode
 *
 * Implements : ftm_output_cmp_ch_param_t_Class
 */
typedef struct
{
    uint8_t hwChannelId;                        /*!< Physical hardware channel ID*/
    ftm_output_compare_mode_t chMode;           /*!< Channel output mode*/
    uint16_t comparedValue;                     /*!< The compared value */
    bool enableExternalTrigger;                 /*!< true: enable the generation of a trigger is used for on-chip modules
                                                 *   false: disable the generation of a trigger */
} ftm_output_cmp_ch_param_t;

/*!
 * @brief FlexTimer driver PWM parameters which is configured for the list of channels
 *
 * Implements : ftm_output_cmp_param_t_Class
 */
typedef struct
{
    uint8_t nNumOutputChannels;                             /*!< Number of output compare channels */
    ftm_config_mode_t mode;                                 /*!< FlexTimer PWM operation mode */
    uint16_t maxCountValue;                                 /*!< Maximum count value in ticks */
    ftm_output_cmp_ch_param_t * outputChannelConfig;        /*!< Output compare channels configuration */
} ftm_output_cmp_param_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures the FTM to generate timed pulses (Output compare mode).
 *
 * When the FTM counter matches the value of CnV, the channel output is changed based on what is
 * specified in the mode argument. The signal period can be modified using
 * param->maxCountValue. After this function when the max counter value and CnV are equal.
 * FTM_DRV_UpdateOutputCompareChannel function can be used to change CnV value.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param configuration of the output compare channels
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_InitOutputCompare(uint32_t instance,
                                   const ftm_output_cmp_param_t * param);

/*!
 * @brief  Disables compare match output control and clears FTM timer configuration
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the output compare channel
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_DeinitOutputCompare(uint32_t instance,
                                     const ftm_output_cmp_param_t * param);

/*!
 * @brief Sets the next compare match value based on the current counter value
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] channel Configuration of the output compare channel
 * @param [in] nextComparematchValue Timer value in ticks until the next compare match event should appear
 * @param [in] update
 *        - FTM_RELATIVE_VALUE : nextComparemantchValue will be added to current counter value
 *        - FTM_ABSOLUTE_VALUE : nextComparemantchValue will be written in counter register as it is
 * @param [in] softwareTrigger This parameter will be true if software trigger sync is enabled and
 * the user want to generate a software trigger (the value from buffer will be moved to register immediate or
 * at next loading point depending on the sync configuration). Otherwise this parameter must be false
 * and the next compared value will be stored in buffer until a trigger signal will be received.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_UpdateOutputCompareChannel(uint32_t instance,
                                            uint8_t channel,
                                            uint16_t nextComparematchValue,
                                            ftm_output_compare_update_t update,
                                            bool softwareTrigger);

#if defined(__cplusplus)
}
#endif

/*! @}*/

/*! @}*/ /* End of addtogroup ftm_oc_driver */

#endif /* FTM_OC_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
