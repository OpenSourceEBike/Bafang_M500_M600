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

#ifndef CRC_DRIVER_H
#define CRC_DRIVER_H

/*! @file crc_driver.h */

#include <stddef.h>
#include "device_registers.h"
#include "status.h"

/*!
 * @defgroup crc_driver CRC Driver
 * @ingroup crc
 * @brief Cyclic Redundancy Check Peripheral Driver
 * @details This section describes the programming interface of the CRC driver.
 * @{
 */
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief CRC type of transpose of read write data
 * Implements : crc_transpose_t_Class
 */
typedef enum
{
    CRC_TRANSPOSE_NONE              = 0x00U,    /*!< No transpose */
    CRC_TRANSPOSE_BITS              = 0x01U,    /*!< Transpose bits in bytes */
    CRC_TRANSPOSE_BITS_AND_BYTES    = 0x02U,    /*!< Transpose bytes and bits in bytes */
    CRC_TRANSPOSE_BYTES             = 0x03U     /*!< Transpose bytes */
} crc_transpose_t;

#if defined(FEATURE_CRC_DRIVER_SOFT_POLYNOMIAL)
/*!
 * @brief CRC bit width
 * Implements : crc_bit_width_t_Class
 */
typedef enum
{
    CRC_BITS_16 = 0U,   /*!< Generate 16-bit CRC code */
    CRC_BITS_32 = 1U    /*!< Generate 32-bit CRC code */
} crc_bit_width_t;

#elif defined(FEATURE_CRC_DRIVER_HARD_POLYNOMIAL)
/*!
 * @brief CRC mode polynomial
 * Implements : crc_mode_polynomial_t_Class
 */
typedef enum
{
    CRC_BITS_16_CCITT   = 0U,   /*!< Generate 16-bit CRC code */
    CRC_BITS_32         = 1U,   /*!< Generate 32-bit CRC code */
    CRC_BITS_8          = 2U,   /*!< Generate 8-bit CRC code */
#if defined(FEATURE_CRC_BITS_8_H2F)
    CRC_BITS_8_H2F      = 3U    /*!< Generate 8-bit H2F CRC code */
#endif
} crc_mode_polynomial_t;
#endif /* FEATURE_CRC_DRIVER_SOFT_POLYNOMIAL */

/*!
 * @brief CRC configuration structure.
 * Implements : crc_user_config_t_Class
 */
typedef struct
{
#if defined(FEATURE_CRC_DRIVER_SOFT_POLYNOMIAL)
    crc_bit_width_t crcWidth;               /*!< Selects 16-bit or 32-bit CRC protocol. */
    uint32_t polynomial;                    /*!< CRC Polynomial, MSBit first.<br/>
                                             *   Example polynomial: 0x1021U = 1_0000_0010_0001 = x^12+x^5+1 */
    crc_transpose_t readTranspose;          /*!< Type of transpose when reading CRC result. */
#elif defined(FEATURE_CRC_DRIVER_HARD_POLYNOMIAL)
    crc_mode_polynomial_t polynomial;       /*!< Selects 8-bit, 8 bit H2F, 16-bit or 32-bit CRC protocol. */
    bool readTranspose;                     /*!< Swap on the CRC_OUTP content. */
#endif /* FEATURE_CRC_DRIVER_SOFT_POLYNOMIAL */
    crc_transpose_t writeTranspose;         /*!< Type of transpose when writing CRC input data. */
    bool complementChecksum;                /*!< True if the result shall be complement of the actual checksum. */
    uint32_t seed;                          /*!< Starting checksum value. */
} crc_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name CRC DRIVER API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the CRC module
 *
 * This function initializes CRC driver based on user configuration input.
 * The user must make sure that the clock is enabled
 *
 * @param[in] instance The CRC instance number
 * @param[in] userConfigPtr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t CRC_DRV_Init(uint32_t instance,
                      const crc_user_config_t * userConfigPtr);

/*!
 * @brief Sets the default configuration
 *
 * This function sets the default configuration
 *
 * @param[in] instance The CRC instance number
 * @return Execution status (success)
 */
status_t CRC_DRV_Deinit(uint32_t instance);

/*!
 * @brief Appends 32-bit data to the current CRC calculation and returns new result
 *
 * This function appends 32-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] newSeed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t CRC_DRV_GetCrc32(uint32_t instance,
                          uint32_t data,
                          bool newSeed,
                          uint32_t seed);

/*!
 * @brief Appends 16-bit data to the current CRC calculation and returns new result
 *
 * This function appends 16-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] newSeed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t CRC_DRV_GetCrc16(uint32_t instance,
                          uint16_t data,
                          bool newSeed,
                          uint32_t seed);

/*!
 * @brief Appends 8-bit data to the current CRC calculation and returns new result
 *
 * This function appends 8-bit data to the current CRC calculation
 * and returns new result. If the newSeed is true, seed set and
 * result are calculated from the seed new value (new CRC calculation)
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Input data for CRC calculation
 * @param[in] newSeed Sets new CRC calculation
 *            - true: New seed set and used for new calculation.
 *            - false: Seed argument ignored, continues old calculation.
 * @param[in] seed New seed if newSeed is true, else ignored
 * @return New CRC result
 */
uint32_t CRC_DRV_GetCrc8(uint32_t instance,
                         uint8_t data,
                         bool newSeed,
                         uint32_t seed);

/*!
 * @brief Appends a block of bytes to the current CRC calculation
 *
 * This function appends a block of bytes to the current CRC calculation
 *
 * @param[in] instance The CRC instance number
 * @param[in] data Data for current CRC calculation
 * @param[in] dataSize Length of data to be calculated
 */
void CRC_DRV_WriteData(uint32_t instance,
                       const uint8_t * data,
                       uint32_t dataSize);

/*!
 * @brief Returns the current result of the CRC calculation
 *
 * This function returns the current result of the CRC calculation
 *
 * @param[in] instance The CRC instance number
 * @return Result of CRC calculation
 */
uint32_t CRC_DRV_GetCrcResult(uint32_t instance);

/*!
 * @brief Configures the CRC module from a user configuration structure
 *
 * This function configures the CRC module from a user configuration structure
 *
 * @param[in] instance The CRC instance number
 * @param[in] userConfigPtr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t CRC_DRV_Configure(uint32_t instance,
                           const crc_user_config_t * userConfigPtr);

/*!
 * @brief Get configures of the CRC module currently
 *
 * This function Get configures of the CRC module currently
 *
 * @param[in] instance The CRC instance number
 * @param[out] userConfigPtr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t CRC_DRV_GetConfig(uint32_t instance,
                           crc_user_config_t * const userConfigPtr);

/*!
 * @brief Get default configures the CRC module for configuration structure
 *
 * This function Get default configures the CRC module for user configuration structure
 *
 * @param[out] userConfigPtr Pointer to structure of initialization
 * @return Execution status (success)
 */
status_t CRC_DRV_GetDefaultConfig(crc_user_config_t * const userConfigPtr);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* CRC_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
