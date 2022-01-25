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

#ifndef CSEC_HW_ACCESS_H
#define CSEC_HW_ACCESS_H

#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5,
 * Impermissible cast; cannot cast from 'essentially unsigned' to 'essentially enum<i>'.
 * All possible values are covered by the enumeration, direct casting is used to optimize code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3,
 * Expression assigned to a narrower or different essential type.
 * All possible values are covered by the enumeration, direct casting is used to optimize code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#include <stddef.h>
#include "status.h"
#include "csec_driver.h"

/*!
 * @brief Specifies how the data is transferred to/from the CSE.
 * There are two use cases. One is to copy all data and the command function call
 * method and the other is a pointer and function call method.
 *
 * Implements : csec_func_format_t_Class
 */
typedef enum {
    CSEC_FUNC_FORMAT_COPY,
    CSEC_FUNC_FORMAT_ADDR
} csec_func_format_t;

 /*! @brief Represents the result of the execution of a command. Provides one bit
for each error code as per SHE specification. */
#define CSEC_NO_ERROR            0x1U
#define CSEC_SEQUENCE_ERROR      0x2U
#define CSEC_KEY_NOT_AVAILABLE   0x4U
#define CSEC_KEY_INVALID         0x8U
#define CSEC_KEY_EMPTY           0x10U
#define CSEC_NO_SECURE_BOOT      0x20U
#define CSEC_KEY_WRITE_PROTECTED 0x40U
#define CSEC_KEY_UPDATE_ERROR    0x80U
#define CSEC_RNG_SEED            0x100U
#define CSEC_NO_DEBUGGING        0x200U
#define CSEC_MEMORY_FAILURE      0x400U
#define CSEC_GENERAL_ERROR       0x800U

#define CSEC_UPPER_HALF_MASK    (0xFFFF0000U)
#define CSEC_UPPER_HALF_SHIFT   (0x10U)
#define CSEC_LOWER_HALF_MASK    (0xFFFFU)
#define CSEC_LOWER_HALF_SHIFT   (0U)

/*! @brief Converts the FTFM flash module registers access */
#if FEATURE_CSEC_HAS_FTFM_MODULE
#define FTFC					(FTFM)
#define FTFC_FSTAT_CCIF_MASK	FTFM_FSTAT_CCIF_MASK
#define FTFC_FCNFG_CCIE_MASK	FTFM_FCNFG_CCIE_MASK
#define FTFC_FCNFG_CCIE			FTFM_FCNFG_CCIE
#endif
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Writes the command header to CSE_PRAM.
 *
 * This function writes the command header to CSE_PRAM, triggering the CSEc
 * operation.
 *
 * @param[in] funcId The ID of the operation to be started.
 * @param[in] funcFormat Specifies how the data is transferred to/from the CSE.
 * @param[in] callSeq Specifies if the information is the first or a following
 * function call.
 * @param[in] keyId Specify the KeyID to be used to implement the requested
 * cryptographic operation.
 */
static inline void CSEC_WriteCommandHeader(csec_cmd_t funcId,
                                       csec_func_format_t funcFormat,
                                       csec_call_sequence_t callSeq,
                                       csec_key_id_t keyId)
{
    CSE_PRAM->RAMn[0].DATA_32 =
        CSE_PRAM_RAMn_DATA_32_BYTE_0(funcId) |
        CSE_PRAM_RAMn_DATA_32_BYTE_1(funcFormat) |
        CSE_PRAM_RAMn_DATA_32_BYTE_2(callSeq) |
        CSE_PRAM_RAMn_DATA_32_BYTE_3(keyId);
}

/*!
 * @brief Writes command bytes to CSE_PRAM.
 *
 * This function writes command bytes to CSE_PRAM, at a 32-bit aligned offset.
 *
 * @param[in] offset The offset (in bytes) at which the bytes shall be written.
 * @param[in] bytes The buffer containing the bytes to be written.
 * @param[in] numBytes The number of bytes to be written.
 */
void CSEC_WriteCommandBytes(uint8_t offset, const uint8_t *bytes, uint8_t numBytes);

/*!
 * @brief Writes a command half word to CSE_PRAM.
 *
 * This function writes a command half word to CSE_PRAM, at a 16-bit aligned
 * offset.
 *
 * @param[in] offset The offset (in bytes) at which the half word shall be
 * written.
 * @param[in] halfWord The half word to be written.
 */
void CSEC_WriteCommandHalfWord(uint8_t offset, uint16_t halfWord);

/*!
 * @brief Writes a command byte to CSE_PRAM.
 *
 * This function writes a command byte to CSE_PRAM.
 *
 * @param[in] offset The offset (in bytes) at which the byte shall be written.
 * @param[in] byte The byte to be written.
 */
void CSEC_WriteCommandByte(uint8_t offset, uint8_t byte);

/*!
 * @brief Writes command words to CSE_PRAM.
 *
 * This function writes command words to CSE_PRAM, at a 32-bit aligned offset.
 *
 * @param[in] offset The offset (in bytes) at which the words shall be written.
 * @param[in] words The buffer containing the words to be written.
 * @param[in] numWords The number of words to be written.
 */
void CSEC_WriteCommandWords(uint8_t offset, const uint32_t *words, uint8_t numWords);

/*!
 * @brief Reads command bytes from CSE_PRAM.
 *
 * This function reads command bytes from CSE_PRAM, from a 32-bit aligned offset.
 *
 * @param[in] offset The offset (in bytes) from which the bytes shall be read.
 * @param[out] bytes The buffer containing the bytes read.
 * @param[in] numBytes The number of bytes to be read.
 */
void CSEC_ReadCommandBytes(uint8_t offset, uint8_t *bytes, uint8_t numBytes);

/*!
 * @brief Reads a command byte from CSE_PRAM.
 *
 * This function reads a command byte from CSE_PRAM.
 *
 * @param[in] offset The offset (in bytes) from which the byte shall be read.
 * @return The byte read.
 */
uint8_t CSEC_ReadCommandByte(uint8_t offset);

/*!
 * @brief Reads a command word from CSE_PRAM.
 *
 * This function reads a command word from CSE_PRAM, from a 32-bit aligned offset.
 *
 * @param[in] offset The offset (in bytes) from which the word shall be read.
 * @return The word read.
 */
static inline uint32_t CSEC_ReadCommandWord(uint8_t offset)
{
    return CSE_PRAM->RAMn[offset >> 2U].DATA_32;
}

/*!
 * @brief Waits for the completion of a CSEc command.
 *
 * This function waits for the completion of a CSEc command.
 */
static inline void CSEC_WaitCommandCompletion(void)
{
    while ((FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK) == 0U)
    {
        /* Wait until the CCIF flag is set */
    }
}

/*!
 * @brief Reads the error bits from PRAM.
 *
 * This function reads the error bits reported after running a CSEc command.
 *
 * @return Error Code after command execution.
 */
static inline status_t CSEC_ReadErrorBits(void)
{
	uint32_t errWord;
    uint16_t errBits;
    status_t stat;

    errWord = CSEC_ReadCommandWord(FEATURE_CSEC_ERROR_BITS_OFFSET);
    errBits = (uint16_t)((errWord & CSEC_UPPER_HALF_MASK) >> CSEC_UPPER_HALF_SHIFT);

    switch (errBits)
    {
    case CSEC_NO_ERROR:
        stat = STATUS_SUCCESS;
        break;
    case CSEC_SEQUENCE_ERROR:
        stat = STATUS_SEC_SEQUENCE_ERROR;
        break;
    case CSEC_KEY_NOT_AVAILABLE:
        stat = STATUS_SEC_KEY_NOT_AVAILABLE;
        break;
    case CSEC_KEY_INVALID:
        stat = STATUS_SEC_KEY_INVALID;
        break;
    case CSEC_KEY_EMPTY:
        stat = STATUS_SEC_KEY_EMPTY;
        break;
    case CSEC_NO_SECURE_BOOT:
        stat = STATUS_SEC_NO_SECURE_BOOT;
        break;
    case CSEC_KEY_WRITE_PROTECTED:
        stat = STATUS_SEC_KEY_WRITE_PROTECTED;
        break;
    case CSEC_KEY_UPDATE_ERROR:
        stat = STATUS_SEC_KEY_UPDATE_ERROR;
        break;
    case CSEC_RNG_SEED:
        stat = STATUS_SEC_RNG_SEED;
        break;
    case CSEC_NO_DEBUGGING:
        stat = STATUS_SEC_NO_DEBUGGING;
        break;
    case CSEC_MEMORY_FAILURE:
        stat = STATUS_SEC_MEMORY_FAILURE;
        break;
    case CSEC_GENERAL_ERROR:
    default:
        stat = STATUS_ERROR;
        break;
    }

    return stat;
}

/*!
 * @brief Writes the command header to CSE_PRAM and waits for completion.
 *
 * This function writes the header of a command and waits for completion.
 * The function is always located in RAM, and is used for CSEc commands using
 * pointer methods, in order to allow the MGATE to read from FLASH without
 * causing a read collision.
 *
 * @param[in] funcId The ID of the operation to be started.
 * @param[in] funcFormat Specifies how the data is transferred to/from the CSE.
 * @param[in] callSeq Specifies if the information is the first or a following
 * function call.
 * @param[in] keyId Specify the KeyID to be used to implement the requested
 * cryptographic operation.
 */
START_FUNCTION_DECLARATION_RAMSECTION
void CSEC_WriteCmdAndWait(csec_cmd_t funcId,
        csec_func_format_t funcFormat,
        csec_call_sequence_t callSeq,
        csec_key_id_t keyId)
END_FUNCTION_DECLARATION_RAMSECTION

/*!
 * @brief Enables/Disables the command completion interrupt.
 *
 * This function enables/disables the command completion interrupt.
 *
 * @param[in] enable Enable/Disable the command completion interrupt.
 */
static inline void CSEC_SetInterrupt(bool enable)
{
    uint8_t tmp = FTFC->FCNFG;

    FTFC->FCNFG = (uint8_t)((tmp & ~FTFC_FCNFG_CCIE_MASK) | FTFC_FCNFG_CCIE(enable ? 1U : 0U));
}

#if defined(__cplusplus)
}
#endif

#endif /* CSEC_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
