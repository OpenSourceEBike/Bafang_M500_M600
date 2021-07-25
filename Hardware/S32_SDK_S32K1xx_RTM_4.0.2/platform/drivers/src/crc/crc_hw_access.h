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

#ifndef CRC_HW_ACCESS_H
#define CRC_HW_ACCESS_H

/*! @file crc_hw_access.h */

#include "crc_driver.h"

/*!

 * @brief Cyclic Redundancy Check Hardware Access.
 *
 * This section describes the programming interface of the CRC HW ACCESS.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name CRC HW ACCESS API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the CRC module
 *
 * This function initializes the module to default configuration
 * (Initial checksum: 0U,
 * Default polynomial: 0x1021U,
 * Type of read transpose: CRC_TRANSPOSE_NONE,
 * Type of write transpose: CRC_TRANSPOSE_NONE,
 * No complement of checksum read,
 * 32-bit CRC)
 *
 * @param[in] base The CRC peripheral base address
 */
void CRC_Init(CRC_Type * const base);

/*!
 * @brief Returns the current result of the CRC calculation
 *
 * This function returns the current result of the CRC calculation
 *
 * @param[in] base The CRC peripheral base address
 * @return Result of CRC calculation
 */
uint32_t CRC_GetCrcResult(const CRC_Type * const base);

/*!
 * @brief Sets seed value for CRC computation
 *
 * This function sets the 32 bits of CRC data register
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] value New seed data for CRC computation
 */
void CRC_SetSeedReg(CRC_Type * const base,
		            uint32_t seedValue);

/*!
 * @brief Gets the current CRC result
 *
 * This function gets the current CRC result from the data register
 *
 * @param[in] base The CRC peripheral base address
 * @return Returns the current CRC result
 */
static inline uint32_t CRC_GetDataReg(const CRC_Type * const base)
{
    return base->DATAu.DATA;
}

/*!
 * @brief Sets the 32 bits of CRC data register
 *
 * This function sets the 32 bits of CRC data register
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void CRC_SetDataReg(CRC_Type * const base,
                                  uint32_t value)
{
    base->DATAu.DATA = value;
}

/*!
 * @brief Gets the upper 16 bits of the current CRC result
 *
 * This function gets the upper 16 bits of the current CRC result from the data register
 *
 * @param[in] base The CRC peripheral base address
 * @return Returns the upper 16 bits of the current CRC result
 */
static inline uint16_t CRC_GetDataHReg(const CRC_Type * const base)
{
    return base->DATAu.DATA_16.H;
}

/*!
 * @brief Gets the lower 16 bits of the current CRC result
 *
 * This function gets the lower 16 bits of the current CRC result from the data register
 *
 * @param[in] base The CRC peripheral base address
 * @return Returns the lower 16 bits of the current CRC result
 */
static inline uint16_t CRC_GetDataLReg(const CRC_Type * const base)
{
    return base->DATAu.DATA_16.L;
}

/*!
 * @brief Sets the lower 16 bits of CRC data register
 *
 * This function sets the lower 16 bits of CRC data register
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void CRC_SetDataLReg(CRC_Type * const base,
                                   uint16_t value)
{
    base->DATAu.DATA_16.L = value;
}

/*!
 * @brief Sets the Low Lower Byte - LL
 *
 * This function sets the Low Lower Byte - LL of CRC data register
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] value New data for CRC computation
 */
static inline void CRC_SetDataLLReg(CRC_Type * const base,
                                    uint8_t value)
{
    base->DATAu.DATA_8.LL = value;
}

/*!
 * @brief Gets the polynomial register value
 *
 * This function gets the polynomial register value
 *
 * @param[in] base The CRC peripheral base address
 * @return Returns the polynomial register value
 */
static inline uint32_t CRC_GetPolyReg(const CRC_Type * const base)
{
    return base->GPOLY;
}

/*!
 * @brief Sets the polynomial register
 *
 * This function sets the polynomial register
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] value Polynomial value
 */
static inline void CRC_SetPolyReg(CRC_Type * const base,
                                  uint32_t value)
{
    base->GPOLY = value;
}

/*!
 * @brief Sets the CRC_DATA register mode
 *
 * This function sets the CRC_DATA register mode
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] enable Enable CRC data register to use for seed value
 *            -true: use CRC data register for seed values.
 *            -false: use CRC data register for data values.
 */
static inline void CRC_SetSeedOrDataMode(CRC_Type * const base,
                                         bool enable)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_WAS_MASK);
    ctrlTemp |= CRC_CTRL_WAS(enable ? 1UL : 0UL);
    base->CTRL = ctrlTemp;
}

/*!
 * @brief Gets complement read of CRC data register
 *
 * This function gets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data
 *
 * @param[in] base The CRC peripheral base address
 * @return Complement read
 *         -true: Invert or complement the read value of the CRC Data register.
 *         -false: No XOR on reading.
 */
static inline bool CRC_GetFXorMode(const CRC_Type * const base)
{
    return ((base->CTRL & CRC_CTRL_FXOR_MASK) >> CRC_CTRL_FXOR_SHIFT) != 0U;
}

/*!
 * @brief Sets complement read of CRC data register
 *
 * This function sets complement read of CRC data register.
 * Some CRC protocols require the final checksum to be XORed with 0xFFFFFFFF
 * or 0xFFFF. Complement mode enables "on the fly" complementing of read data
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] enable Enable or disable complementing of read data
 */
static inline void CRC_SetFXorMode(CRC_Type * const base,
                                   bool enable)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_FXOR_MASK);
    ctrlTemp |= CRC_CTRL_FXOR(enable ? 1UL : 0UL);
    base->CTRL = ctrlTemp;
}

/*!
 * @brief Gets the CRC protocol width
 *
 * This function gets the CRC protocol width
 *
 * @param[in] base The CRC peripheral base address
 * @return CRC protocol width
 *         - CRC_BITS_16: 16-bit CRC protocol.
 *         - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline crc_bit_width_t CRC_GetProtocolWidth(const CRC_Type * const base)
{
    crc_bit_width_t retVal = CRC_BITS_16;

    if (((base->CTRL & CRC_CTRL_TCRC_MASK) >> CRC_CTRL_TCRC_SHIFT) != 0U)
    {
        retVal = CRC_BITS_32;
    }

    return retVal;
}

/*!
 * @brief Sets the CRC protocol width
 *
 * This function sets the CRC protocol width
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] width The CRC protocol width
 *            - CRC_BITS_16: 16-bit CRC protocol.
 *            - CRC_BITS_32: 32-bit CRC protocol.
 */
static inline void CRC_SetProtocolWidth(CRC_Type * const base,
                                        crc_bit_width_t width)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TCRC_MASK);
    ctrlTemp |= CRC_CTRL_TCRC(width);
    base->CTRL = ctrlTemp;
}

/*!
 * @brief Gets the CRC transpose type for writes
 *
 * This function gets the CRC transpose type for writes
 *
 * @param[in] base The CRC peripheral base address
 * @return CRC input transpose type for writes
 */
static inline crc_transpose_t CRC_GetWriteTranspose(const CRC_Type * const base)
{
    crc_transpose_t type;
    uint32_t temp = (base->CTRL & CRC_CTRL_TOT_MASK) >> CRC_CTRL_TOT_SHIFT;

    switch (temp)
    {
        case 1U:
            type = CRC_TRANSPOSE_BITS;
            break;
        case 2U:
            type = CRC_TRANSPOSE_BITS_AND_BYTES;
            break;
        case 3U:
            type = CRC_TRANSPOSE_BYTES;
            break;
        default:
            type = CRC_TRANSPOSE_NONE;
            break;
    }

    return type;
}

/*!
 * @brief Sets the CRC transpose type for writes
 *
 * This function sets the CRC transpose type for writes
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] transp The CRC input transpose type
 */
static inline void CRC_SetWriteTranspose(CRC_Type * const base,
                                         crc_transpose_t transp)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TOT_MASK);
    ctrlTemp |= CRC_CTRL_TOT(transp);
    base->CTRL = ctrlTemp;
}

/*!
 * @brief Gets the CRC transpose type for reads
 *
 * This function gets the CRC transpose type for reads
 *
 * @param[in] base The CRC peripheral base address
 * @return CRC output transpose type
 */
static inline crc_transpose_t CRC_GetReadTranspose(const CRC_Type * const base)
{
    crc_transpose_t type;
    uint32_t temp = (base->CTRL & CRC_CTRL_TOTR_MASK) >> CRC_CTRL_TOTR_SHIFT;

    switch (temp)
    {
        case 1U:
            type = CRC_TRANSPOSE_BITS;
            break;
        case 2U:
            type = CRC_TRANSPOSE_BITS_AND_BYTES;
            break;
        case 3U:
            type = CRC_TRANSPOSE_BYTES;
            break;
        default:
            type = CRC_TRANSPOSE_NONE;
            break;
    }

    return type;
}

/*!
 * @brief Sets the CRC transpose type for reads
 *
 * This function sets the CRC transpose type for reads
 *
 * @param[in] base The CRC peripheral base address
 * @param[in] transp The CRC output transpose type
 */
static inline void CRC_SetReadTranspose(CRC_Type * const base,
                                        crc_transpose_t transp)
{
    uint32_t ctrlTemp = base->CTRL;

    ctrlTemp &= ~(CRC_CTRL_TOTR_MASK);
    ctrlTemp |= CRC_CTRL_TOTR(transp);
    base->CTRL = ctrlTemp;
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* CRC_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
