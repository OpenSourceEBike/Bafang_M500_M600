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
 * @file eim_driver.h
 */

#ifndef EIM_DRIVER_H
#define EIM_DRIVER_H

#include <stddef.h>
#include "device_registers.h"


/*!
 * @defgroup eim_drv EIM Driver
 * @ingroup eim
 * @brief Error Injection Module Peripheral Driver.@n
 * EIM PD provides a set of high-level APIs/services to configure the
 * Error Injection Module (EIM) module.
 * @addtogroup eim_drv
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The value default of EIM check-bit mask  */
#define EIM_CHECKBITMASK_DEFAULT    (0x01U)
/*! @brief The value default of EIM data mask  */
#define EIM_DATAMASK_DEFAULT        (0x00U)

/*!
 * @brief EIM channel configuration structure
 *
 * This structure holds the configuration settings for the EIM channel
 * Implements : eim_user_channel_config_t_Class
 */
typedef struct
{
    uint8_t channel;      /*!< EIM channel number                                          */
    uint8_t checkBitMask; /*!< Specifies whether the corresponding bit of the check-bit bus
                               from the target RAM should be inverted or remain unmodified */
    uint32_t dataMask;    /*!< Specifies whether the corresponding bit of the read data bus
                               from the target RAM should be inverted or remain unmodified */
#if defined(FEATURE_EIM_DATAMASK1)
    uint32_t dataMask1;   /*!< Specifies whether the corresponding bit of the read data bus
                               from the target RAM should be inverted or remain unmodified */
#endif
    bool enable;          /*!< true : EIM channel operation is enabled
                               false : EIM channel operation is disabled                   */
} eim_user_channel_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name EIM Driver API
 * @{
 */

#if defined(__cplusplus)
extern "C"
{
#endif

/*!
 * @brief Initializes the EIM module.
 *
 * This function configures for EIM channels.
 * The EIM channel configuration structure array and number of configured channels
 * shall be passed as arguments.
 * This function should be called before calling any other EIM driver function.
 *
 * This is an example demonstrating how to define a EIM channel configuration structure array:
 @code
 eim_user_channel_config_t channelConfigArr[] =
 {
 {
 .channel = 0x0U,
 .checkBitMask = 0x12U,
 .dataMask = 0x01234567U,
 .enable = true
 },
 {
 .channel = 0x1U,
 .checkBitMask = 0x22U,
 .dataMask = 0x01234444U,
 .enable = false
 }
 };
 @endcode
 *
 * @param[in] instance EIM module instance number.
 * @param[in] channelCnt Number of configured channels
 * @param[in] channelConfigArr EIM channel configuration structure array
 */
void EIM_DRV_Init(uint32_t instance,
                  uint8_t channelCnt,
                  const eim_user_channel_config_t *channelConfigArr);

/*!
 * @brief De-initializes the EIM module.
 *
 * This function sets all registers to reset value and disables EIM module.
 * In order to use the EIM module again, EIM_DRV_Init must be called.
 *
 * @param[in] instance EIM module instance number
 */
void EIM_DRV_Deinit(uint32_t instance);

/*!
 * @brief Configures the EIM channel.
 *
 * This function configures check-bit mask, data mask and
 * operation status(enable/disable) for EIM channel.
 * The EIM channel configuration structure shall be
 * passed as arguments.
 *
 * This is an example demonstrating how to define a EIM channel configuration structure:
   @code
   eim_user_channel_config_t eimTestInit = {
        .channel = 0x1U,
        .checkBitMask = 0x25U,
        .dataMask = 0x11101100U,
        .enable = true
   };
   @endcode
 *
 * @param[in] instance EIM module instance number
 * @param[in] userChannelConfig Pointer to EIM channel configuration structure
 */
void EIM_DRV_ConfigChannel(uint32_t instance,
                           const eim_user_channel_config_t *userChannelConfig);

/*!
 * @brief Gets the EIM channel configuration.
 *
 * This function gets check bit mask, data mask and operation status of EIM channel.
 *
 * @param[in] instance EIM module instance number
 * @param[in] channel EIM channel number
 * @param[out] channelConfig Pointer to EIM channel configuration structure
 */
void EIM_DRV_GetChannelConfig(uint32_t instance,
                              uint8_t channel,
                              eim_user_channel_config_t *channelConfig);

/*!
 * @brief Gets the EIM channel configuration default.
 *
 * This function gets check bit mask, data mask and operation status default of EIM channel.
 *
 * @param[in] channel EIM channel number
 * @param[out] channelConfig Pointer to EIM channel configuration structure default
 */
void EIM_DRV_GetDefaultConfig(uint8_t channel,
                              eim_user_channel_config_t *channelConfig);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* EIM_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
