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

#ifndef PHY_SHARED_H
#define PHY_SHARED_H

/*!
 * @file phy_shared.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros define global parameters which can be used by application code.
 *
 */

#include <stdint.h>
#include "phy.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MMDs */
#define MMD1_PMA_PMD  1U
#define MMD3_PCS      3U
#define MMD30_VS1     30U
#define MMD31_VS2     31U

/* Private Data Format */
/* This section defines the meaning of the fields within the private section of the driver configuration */

#define PRIVATE_ANEG_CAPABLE  0x00000001U    /**< Flag that defines if a device is capable of auto-negotiation */
#define PRIVATE_ANEG_MSCTRL   0x00000002U    /**< Flag that defines if a device is using master/slave settings during auto-negotiation (i.e. if register 9 is available) */
#define PRIVATE_ANEG_ENABLED  0x00000004U    /**< Flag that defines if auto-negotiation is enabled */
#define PRIVATE_1000BASET     0x00000008U    /**< Flag that defines if a device is 1000BASE-T capable */
#define PRIVATE_TJA1100       0x00001000U    /**< Flag that defines that a device is a TJA1100 --> limited wake/sleep support */
#define PRIVATE_ACTIVE        0x20000000U    /**< Flag that defines that a PHY is powered on and operational */
#define PRIVATE_ANEG_PENDING  0x40000000U    /**< Flag that defines if a completion of auto-negotiation is expected */
#define PRIVATE_LINKUP        0x80000000U    /**< Flag that defines if the link is up */

/*!
 * @brief Structure containing the basic status information
 */
typedef struct
{
    bool link;            /**< Link Status */
    bool autonegAbility;  /**< Auto-negotiation ability */
    bool extendedStatus;  /**< Extended Status */
    bool baseT2Hd;        /**< 100BASE-T2 Half-Duplex */
    bool baseT2Fd;        /**< 100BASE-T2 Full-Duplex */
    bool autonegComplete; /**< Auto-negotiation complete */
    bool remoteFault;     /**< Remote fault */
} phy_basicStatus_t;

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * @brief Reads the basic status register
 *
 * @param[in] phy Index of the phy to read from
 * @param[out] basicStatus basic status structure
 */
status_t PHY_SHARED_ReadBasicStatus(uint8_t phy, phy_basicStatus_t *basicStatus);

/*!
 * @brief Extracts a PHY ID from the two IEEE registers containing the ID
 *
 * @param[in] id1 First PHY ID register value
 * @param[in] id2 Second PHY ID register value
 * @param[out] id Structure containing the extracted ID
 */
void PHY_SHARED_ExtractPhyID(const uint16_t id1, const uint16_t id2, phy_id_t* id);

/*!
 * @brief Return status as not support for unsupported feature
 *
 * @param[in] phy The device number. Will be ignored, used to match prototype.
 */
status_t PHY_SHARED_ReturnNotSupported(uint8_t phy);

#ifdef __cplusplus
}
#endif



#endif /* PHY_SHARED_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
