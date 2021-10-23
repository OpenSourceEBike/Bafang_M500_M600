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

#ifndef ERM_DRIVER_H
#define ERM_DRIVER_H

/*! @file erm_driver.h */

#include "device_registers.h"
#include "status.h"

/*!
 * @defgroup erm_driver ERM Driver
 * @ingroup erm
 * @brief Error Reporting Module Peripheral Driver.
 * @details This section describes the programming interface of the ERM driver.
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief ERM types of ECC events
 * Implements : erm_ecc_event_t_Class
 */
typedef enum
{
    ERM_EVENT_NONE              = 0U,   /*!< None events */
    ERM_EVENT_SINGLE_BIT        = 1U,   /*!< Single-bit correction ECC events */
    ERM_EVENT_NON_CORRECTABLE   = 2U    /*!< Non-correctable ECC events */
} erm_ecc_event_t;

/*!
 * @brief ERM interrupt notification configuration structure
 * Implements : erm_interrupt_config_t_Class
 */
typedef struct
{
    bool enableSingleCorrection;    /*!< Enable Single Correction Interrupt Notification */
    bool enableNonCorrectable;      /*!< Enable Non-Correctable Interrupt Notification */
} erm_interrupt_config_t;

/*!
 * @brief ERM user configuration structure
 * Implements : erm_user_config_t_Class
 */
typedef struct
{
    uint8_t channel;                                /*!< The channel assignments */
    const erm_interrupt_config_t * interruptCfg;    /*!< Interrupt configuration */
} erm_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name ERM DRIVER API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ERM module
 *
 * This function initializes ERM driver based on user configuration input,
 * channelCnt takes values between 1 and the maximum channel count supported by the hardware
 *
 * @param[in] instance The ERM instance number
 * @param[in] channelCnt The number of channels
 * @param[in] userConfigArr The pointer to the array of ERM user configure structure
 */
void ERM_DRV_Init(uint32_t instance,
                  uint8_t channelCnt,
                  const erm_user_config_t * userConfigArr);

/*!
 * @brief Sets the default configuration
 *
 * This function sets the default configuration
 *
 * @param[in] instance The ERM instance number
 */
void ERM_DRV_Deinit(uint32_t instance);

/*!
 * @brief Sets interrupt notification
 *
 * This function sets interrupt notification based on interrupt notification configuration input
 *
 * @param[in] instance The ERM instance number
 * @param[in] channel The configured memory channel
 * @param[in] interruptCfg The ERM interrupt configuration structure
 */
void ERM_DRV_SetInterruptConfig(uint32_t instance,
                                uint8_t channel,
                                erm_interrupt_config_t interruptCfg);

/*!
 * @brief Gets interrupt notification
 *
 * This function gets the current interrupt configuration of the available events
 * (which interrupts are enabled/disabled)
 *
 * @param[in] instance The ERM instance number
 * @param[in] channel The examined memory channel
 * @param[out] interruptPtr The pointer to the ERM interrupt configuration structure
 */
void ERM_DRV_GetInterruptConfig(uint32_t instance,
                                uint8_t channel,
                                erm_interrupt_config_t * const interruptPtr);

/*!
 * @brief Clears error event and the corresponding interrupt notification
 *
 * This function clears the record of an event. If the corresponding interrupt is enabled,
 * the interrupt notification will be cleared
 *
 * @param[in] instance The ERM instance number
 * @param[in] channel The configured memory channel
 * @param[in] eccEvent The types of ECC events
 */
void ERM_DRV_ClearEvent(uint32_t instance,
                        uint8_t channel,
                        erm_ecc_event_t eccEvent);

/*!
 * @brief Gets the address of the last ECC event in Memory n and ECC event
 *
 * This function gets the address of the last ECC event in Memory n and the types of the event
 *
 * @param[in] instance The ERM instance number
 * @param[in] channel The examined memory channel
 * @param[out] addressPtr The pointer to address of the last ECC event in Memory n with ECC event
 * @return The last occurred ECC event
 */
erm_ecc_event_t ERM_DRV_GetErrorDetail(uint32_t instance,
                                       uint8_t channel,
                                       uint32_t * addressPtr);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* ERM_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
