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
 * @file ftm_qd_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 */

#ifndef FTM_QD_DRIVER_H
#define FTM_QD_DRIVER_H

#include "ftm_common.h"

/*!
 * @addtogroup ftm_qd_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief FlexTimer quadrature decode modes, phase encode or count and direction mode
 *
 * Implements : ftm_quad_decode_mode_t_Class
 */
typedef enum
{
    FTM_QUAD_PHASE_ENCODE   = 0x00U,    /*!< Phase encoding mode                 */
    FTM_QUAD_COUNT_AND_DIR  = 0x01U     /*!< Counter and direction encoding mode */
} ftm_quad_decode_mode_t;

/*!
 * @brief FlexTimer quadrature phase polarities, normal or inverted polarity
 *
 * Implements : ftm_quad_phase_polarity_t_Class
 */
typedef enum
{
    FTM_QUAD_PHASE_NORMAL = 0x00U,  /*!< Phase input signal is not inverted before identifying
                                     *   the rising and falling edges of this signal */
    FTM_QUAD_PHASE_INVERT = 0x01U   /*!< Phase input signal is inverted before identifying
                                     *   the rising and falling edges of this signal */
} ftm_quad_phase_polarity_t;

/*!
 * @brief FlexTimer quadrature decoder channel parameters
 *
 * Implements : ftm_phase_params_t_Class
 */
typedef struct
{
    bool phaseInputFilter;                          /*!< false: disable phase filter,
                                                     *   true: enable phase filter */
    uint8_t phaseFilterVal;                         /*!< Filter value (if input filter is enabled)*/
    ftm_quad_phase_polarity_t phasePolarity;        /*!< Phase polarity */
} ftm_phase_params_t;

/*!
 * @brief FTM quadrature configure structure
 *
 * Implements : ftm_quad_decode_config_t_Class
 */
typedef struct
{
    ftm_quad_decode_mode_t mode;        /*!< FTM_QUAD_PHASE_ENCODE or FTM_QUAD_COUNT_AND_DIR */
    uint16_t initialVal;                /*!< Initial counter value*/
    uint16_t maxVal;                    /*!< Maximum counter value*/
    ftm_phase_params_t phaseAConfig;    /*!< Configuration for the input phase a */
    ftm_phase_params_t phaseBConfig;    /*!< Configuration for the input phase b */
} ftm_quad_decode_config_t;

/*!
 * @brief FTM quadrature state(counter value and flags)
 *
 * Implements : ftm_quad_decoder_state_t_Class
 */
typedef struct
{
    uint16_t counter;           /*!< Counter value */
    bool overflowFlag;          /*!< True if overflow occurred,
                                 *   False if overflow doesn't occurred */
    bool overflowDirection;     /*!< False if overflow occurred at minimum value,
                                 *   True if overflow occurred at maximum value */
    bool counterDirection;      /*!< False FTM counter is decreasing,
                                 *   True FTM counter is increasing */
} ftm_quad_decoder_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures the quadrature mode and starts measurement
 *
 * @param [in] instance Instance number of the FTM module.
 * @param [in] config   Configuration structure(quadrature decode mode, polarity for both phases,
 *                      initial and maximum value for the counter, filter configuration).
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_QuadDecodeStart(uint32_t instance,
                                 const ftm_quad_decode_config_t * config);

/*!
 * @brief De-activates the quadrature decode mode.
 *
 * @param [in] instance Instance number of the FTM module.
 * @return success
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t FTM_DRV_QuadDecodeStop(uint32_t instance);

/*!
 * @brief Return the current quadrature decoder state (counter value, overflow flag and
 * overflow direction)
 *
 * @param [in] instance Instance number of the FTM module.
 * @return The current state of quadrature decoder
 */
ftm_quad_decoder_state_t FTM_DRV_QuadGetState(uint32_t instance);

/*!
 * @brief This function will get the default configuration values
 *        in the structure which is used as a common use-case.
 * @param[out] config Pointer to the structure in which the
 *                    configuration will be saved.
 * @return None
 */
void FTM_QD_DRV_GetDefaultConfig(ftm_quad_decode_config_t * const config);

#if defined(__cplusplus)
}
#endif

/*! @}*/

/*! @}*/ /* End of addtogroup ftm_qd_driver */

#endif /* FTM_QD_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
