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
 * @file ftm_ic_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 */

#ifndef FTM_IC_DRIVER_H
#define FTM_IC_DRIVER_H

#include "ftm_common.h"

/*!
 * @addtogroup ftm_ic_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Selects mode operation in the input capture
 *
 * Implements : ftm_input_op_mode_t_Class
 */
typedef enum
{
    FTM_EDGE_DETECT         = 0U,    /*!< FTM edge detect */
    FTM_SIGNAL_MEASUREMENT  = 1U,    /*!< FTM signal measurement */
    FTM_NO_OPERATION        = 2U     /*!< FTM no operation */
} ftm_input_op_mode_t;

/*!
 * @brief FlexTimer input capture measurement type for dual edge input capture
 *
 * Implements : ftm_signal_measurement_mode_t_Class
 */
typedef enum
{
    FTM_NO_MEASUREMENT                  = 0x00U,    /*!< No measurement */
    FTM_RISING_EDGE_PERIOD_MEASUREMENT  = 0x01U,    /*!< Period measurement between two consecutive rising edges */
    FTM_FALLING_EDGE_PERIOD_MEASUREMENT = 0x02U,    /*!< Period measurement between two consecutive falling edges */
    FTM_PERIOD_ON_MEASUREMENT           = 0x03U,    /*!< The time measurement taken for the pulse to remain ON or HIGH state */
    FTM_PERIOD_OFF_MEASUREMENT          = 0x04U     /*!< The time measurement taken for the pulse to remain OFF or LOW state */
} ftm_signal_measurement_mode_t;

/*!
 * @brief FlexTimer input capture edge mode as rising edge or falling edge
 *
 * Implements : ftm_edge_alignment_mode_t_Class
 */
typedef enum
{
    FTM_NO_PIN_CONTROL          = 0x00U,       /*!< No trigger */
    FTM_RISING_EDGE             = 0x01U,       /*!< Rising edge trigger */
    FTM_FALLING_EDGE            = 0x02U,       /*!< Falling edge trigger */
    FTM_BOTH_EDGES              = 0x03U        /*!< Rising and falling edge trigger */
} ftm_edge_alignment_mode_t;

/*!
 * @brief The measurement type for input capture mode
 * Implements : ftm_ic_op_mode_t_Class
 */
typedef enum
{
    FTM_DISABLE_OPERATION           = 0x00U, /*!< Have no operation */
    FTM_TIMESTAMP_RISING_EDGE       = 0x01U, /*!< Rising edge trigger */
    FTM_TIMESTAMP_FALLING_EDGE      = 0x02U, /*!< Falling edge trigger */
    FTM_TIMESTAMP_BOTH_EDGES        = 0x03U, /*!< Rising and falling edge trigger */
    FTM_MEASURE_RISING_EDGE_PERIOD  = 0x04U, /*!< Period measurement between two consecutive rising edges */
    FTM_MEASURE_FALLING_EDGE_PERIOD = 0x05U, /*!< Period measurement between two consecutive falling edges */
    FTM_MEASURE_PULSE_HIGH          = 0x06U, /*!< The time measurement taken for the pulse to remain ON or HIGH state */
    FTM_MEASURE_PULSE_LOW           = 0x07U  /*!< The time measurement taken for the pulse to remain OFF or LOW state */
} ftm_ic_op_mode_t;

/*!
 * @brief FlexTimer driver Input capture parameters for each channel
 *
 * Implements : ftm_input_ch_param_t_Class
 */
typedef struct
{
    uint8_t hwChannelId;                                /*!< Physical hardware channel ID*/
    ftm_input_op_mode_t inputMode;                      /*!< FlexTimer module mode of operation  */
    ftm_edge_alignment_mode_t edgeAlignement;           /*!< Edge alignment Mode for signal measurement*/
    ftm_signal_measurement_mode_t measurementType;      /*!< Measurement Mode for signal measurement*/
    uint16_t filterValue;                               /*!< Filter Value */
    bool filterEn;                                      /*!< Input capture filter state */
    bool continuousModeEn;                              /*!< Continuous measurement state */
    void * channelsCallbacksParams;                     /*!< The parameters of callback functions for channels events */
    ic_callback_t channelsCallbacks;                    /*!< The callback function for channels events */
} ftm_input_ch_param_t;

/*!
 * @brief FlexTimer driver input capture parameters
 *
 * Implements : ftm_input_param_t_Class
 */
typedef struct
{
    uint8_t nNumChannels;                    /*!< Number of input capture channel used */
    uint16_t nMaxCountValue;                 /*!< Maximum counter value. Minimum value is 0 for this mode */
    ftm_input_ch_param_t * inputChConfig;    /*!< Input capture channels configuration */
} ftm_input_param_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief This function configures the channel in the Input Capture mode for either getting
 * time-stamps on edge detection or on signal measurement. When the edge specified in the captureMode
 * argument occurs on the channel and then the FTM counter is captured into the CnV register.
 * The user have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed as 0. The filter feature.
 * is available only on channels 0,1,2,3.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the input capture channel.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_InitInputCapture(uint32_t instance,
                                  const ftm_input_param_t * param);

/*!
 * @brief  Disables input capture mode and clears FTM timer configuration
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] param Configuration of the output compare channel.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_DeinitInputCapture(uint32_t instance,
                                    const ftm_input_param_t * param);

/*!
 * @brief  This function is used to calculate the measurement and/or time stamps values
 * which are read from the C(n, n+1)V registers and stored to the static buffers.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] channel  For getting the time stamp of the last edge (in normal input capture) this
 *                    parameter represents the channel number.
 *                    For getting the last measured value (in dual edge input capture) this parameter
 *                    is the lowest channel number of the pair (EX: 0, 2, 4, 6).
 * @return value   The measured value
 */
uint16_t FTM_DRV_GetInputCaptureMeasurement(uint32_t instance,
                                            uint8_t channel);

/*!
 * @brief  Starts new single-shot signal measurement of the given channel.
 *
 * @param [in] instance The FTM peripheral instance number.
 * @param [in] channel Configuration of the output compare channel.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_StartNewSignalMeasurement(uint32_t instance,
                                           uint8_t channel);
/*!
 * @brief Set mode operation for channel in the input capture mode
 *
 * This function will change the channel mode at run time or
 * when stopping channel. The channel mode is selected in the ftm_ic_op_mode_t
 * enumeration type.
 *
 * @param[in] instance The input capture instance number.
 * @param[in] channel The channel number.
 * @param[in] inputMode The channel operation mode.
 * @param[in] enableContinuousCapture Enable/disable the continuous capture mode.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 */
status_t FTM_IC_DRV_SetChannelMode(uint32_t instance,
                                   uint8_t channel,
                                   ftm_ic_op_mode_t inputMode,
                                   bool enableContinuousCapture);

#if defined(__cplusplus)
}
#endif

/*! @}*/

/*! @}*/ /* End of addtogroup ftm_ic_driver */

#endif /* FTM_IC_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
