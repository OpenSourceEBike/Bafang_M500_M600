/*
 * Copyright 2020 NXP
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
 * @file phy_shared.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "phy.h"
#include "phy_shared.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BASIC_STATUS_ADDR       1U

#define BASIC_STATUS_LINK_FLAG              0x0004U
#define BASIC_STATUS_AUTONEG_ABILITY_FLAG   0x0008U
#define BASIC_STATUS_EXTENDED_STATUS_FLAG   0x0100U
#define BASIC_STATUS_100BASET2HD_FLAG       0x0200U
#define BASIC_STATUS_100BASET2FD_FLAG       0x0400U
#define BASIC_STATUS_AUTONEG_COMPLETE_FLAG  0x0020U
#define BASIC_STATUS_REMOTE_FAULT_FLAG      0x0010U

#define ID_1_OCTET0_MASK  0xFC00U  /**< Bits belonging to the octet 0 of OUI */
#define ID_1_OCTET1_MASK  0x03FCU  /**< Bits belonging to the octet 1 of OUI */
#define ID_1_OCTET2_MASK  0x0003U  /**< Bits belonging to the octet 2 of OUI */

#define ID_1_OCTET0_SHIFT 10U
#define ID_1_OCTET1_SHIFT 2U
#define ID_1_OCTET2_SHIFT 6U      /**< Left Shift to reach correct position in OUI byte */

#define ID_2_OCTET2_MASK  0xFC00U  /**< Bits belonging to the third octet of OUI */
#define ID_2_OCTET2_SHIFT 10U

#define ID_2_TYPE_MASK  0x03F0U
#define ID_2_TYPE_SHIFT 4U
#define ID_2_REV_MASK   0x000FU


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint8_t swap(const uint8_t byte);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SHARED_ReadBasicStatus
 * Description   : Reads the basic status register
 *
 *END**************************************************************************/
status_t PHY_SHARED_ReadBasicStatus(uint8_t phy, phy_basicStatus_t *basicStatus)
{
    uint16_t reg;
    status_t status;

    status = PHY_Read(phy, BASIC_STATUS_ADDR, &reg);
    if (status == STATUS_SUCCESS)
    {
        basicStatus->link = ((reg & BASIC_STATUS_LINK_FLAG) != 0U) ? (true) : (false);
        basicStatus->autonegAbility = ((reg & BASIC_STATUS_AUTONEG_ABILITY_FLAG) != 0U) ? (true) : (false);
        basicStatus->extendedStatus = ((reg & BASIC_STATUS_EXTENDED_STATUS_FLAG) != 0U) ? (true) : (false);
        basicStatus->baseT2Hd= ((reg & BASIC_STATUS_100BASET2HD_FLAG) != 0U) ? (true) : (false);
        basicStatus->baseT2Fd = ((reg & BASIC_STATUS_100BASET2FD_FLAG) != 0U) ? (true) : (false);
        basicStatus->autonegComplete = ((reg & BASIC_STATUS_AUTONEG_COMPLETE_FLAG) != 0U) ? (true) : (false);
        basicStatus->remoteFault = ((reg & BASIC_STATUS_REMOTE_FAULT_FLAG) != 0U) ? (true) : (false);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SHARED_ExtractPhyID
 * Description   : Extract PHY ID from two PHY ID registers
 *
 *END**************************************************************************/
void PHY_SHARED_ExtractPhyID(const uint16_t id1, const uint16_t id2, phy_id_t* id)
{
    uint8_t ouiOctet0 = (uint8_t) ((id1 & (uint16_t) ID_1_OCTET0_MASK) >> ID_1_OCTET0_SHIFT);
    uint8_t ouiOctet1 = (uint8_t) ((id1 & (uint16_t) ID_1_OCTET1_MASK) >> ID_1_OCTET1_SHIFT);
    uint8_t ouiOctet2 = (uint8_t) (id1 & (uint16_t) ID_1_OCTET2_MASK) << ID_1_OCTET2_SHIFT;

    ouiOctet2 |= (uint8_t) ((id2 & (uint16_t) ID_2_OCTET2_MASK) >> ID_2_OCTET2_SHIFT);

    ouiOctet0 = swap(ouiOctet0);
    ouiOctet1 = swap(ouiOctet1);
    ouiOctet2 = swap(ouiOctet2);

    id->oui = (((uint32_t) ouiOctet0) << 16U) | (((uint32_t) ouiOctet1) << 8U) | ((uint32_t) ouiOctet2);

    id->typeNo     = (uint8_t)((id2 & ID_2_TYPE_MASK) >> ID_2_TYPE_SHIFT);
    id->revisionNo = (uint8_t)(id2 & ID_2_REV_MASK);

    return;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SHARED_ReturnNotSupported
 * Description   : Unsupported framework functions return STATUS_UNSUPPORTED.
 *
 *END**************************************************************************/
status_t PHY_SHARED_ReturnNotSupported(uint8_t phy)
{
    (void)(phy);
    return STATUS_UNSUPPORTED;
}

static uint8_t swap(const uint8_t byte)
{
    uint8_t swappedByte = byte;
    uint8_t temp;

    temp = swappedByte & (uint8_t) 0x0FU;
    temp = (uint8_t) (((uint16_t) temp) << 4U);
    swappedByte = ((swappedByte & (uint8_t) 0xF0U) >> 4U) | temp;

    temp = swappedByte & (uint8_t) 0x33U;
    temp = (uint8_t) (((uint16_t) temp) << 2U);
    swappedByte = ((swappedByte & (uint8_t) 0xCCU) >> 2U) | temp;

    temp = swappedByte & (uint8_t) 0x55U;
    temp = (uint8_t) (((uint16_t) temp) << 1U);
    swappedByte = ((swappedByte & (uint8_t) 0xAAU) >> 1U) | temp;

    return swappedByte;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
