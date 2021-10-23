/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * @file lin_common.c
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lin_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : BIT
 * Description   : Return bit B in byte A
 * This is not a public API as it is called by other API functions.
 *
 *END**************************************************************************/
static inline uint8_t BIT(uint8_t A,
                          uint8_t B)
{
    return (uint8_t)((A >> B) & 0x01U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_ProcessParity
 * Description   : Makes or checks parity bits. If action is checking parity, the function
 * returns ID value if parity bits are correct or 0xFF if parity bits are incorrect. If action
 * is making parity bits, then from input value of ID, the function returns PID.
 * This is not a public API as it is called by other API functions.
 *
 * Implements    : LIN_DRV_ProcessParity_Activity
 *END**************************************************************************/
uint8_t LIN_DRV_ProcessParity(uint8_t PID,
                              uint8_t typeAction)
{
    uint8_t parity;
    uint8_t retVal;

    parity = (uint8_t)(((0xFFU & (BIT(PID, 0U) ^ BIT(PID, 1U) ^ BIT(PID, 2U) ^ BIT(PID, 4U))) << 6U) |
                       ((0xFFU ^ (BIT(PID, 1U) ^ BIT(PID, 3U) ^ BIT(PID, 4U) ^ BIT(PID, 5U))) << 7U));

    /* Check if action is checking parity bits */
    if (CHECK_PARITY == typeAction)
    {
        /* If parity bits are incorrect */
        if ((PID & 0xC0U) != parity)
        {
            /* Return 0xFF if parity bits are incorrect */
            retVal = 0xFFU;
        }
        /* If parity bits are correct */
        else
        {
            /* Return ID if parity bits are correct */
            retVal = (uint8_t)(PID & 0x3FU);
        }
    }
    /* If action is making parity bits */
    else
    {
        /* Return PID in case of making parity bits */
        retVal = (uint8_t)(PID | parity);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_MakeChecksumByte
 * Description   : Makes the checksum byte for a frame. For PID of identifiers,
 * if PID is 0x3C (ID 0x3C) or 0x7D (ID 0x3D) or 0xFE (ID 0x3E) or 0xBF (ID 0x3F)
 * apply classic checksum and apply enhanced checksum for other PID.
 * In case user want to calculate classic checksum please set PID to zero.
 *
 * Implements    : LIN_DRV_MakeChecksumByte_Activity
 *END**************************************************************************/
uint8_t LIN_DRV_MakeChecksumByte(const uint8_t * buffer,
                                 uint8_t sizeBuffer,
                                 uint8_t PID)
{
    uint8_t length;
    uint16_t checksum = 0U;

    /* For PID is 0x3C (ID 0x3C) or 0x7D (ID 0x3D) or 0xFE (ID 0x3E) or 0xBF (ID 0x3F)
     * apply classic checksum and apply enhanced checksum for other PID */
    if ((0x3CU != PID) && (0x7DU != PID) && (0xFEU != PID) && (0xBFU != PID))
    {
        /* For PID other than 0x3C, 0x7D, 0xFE and 0xBF: Add PID in checksum calculation */
        checksum = PID;
    }
    else
    {
        /* For 0x3C, 0x7D, 0xFE and 0xBF: Do not add PID in checksum calculation */
        checksum = 0U;
    }

    for (length = sizeBuffer; 0U < length; length--)
    {
        checksum += *buffer;
        buffer++;
        /* Deal with the carry */
        if (checksum > 0xFFU)
        {
            checksum -= 0xFFU;
        }
    }

    /* Return reversed checksum */
    return (uint8_t)(~checksum);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
