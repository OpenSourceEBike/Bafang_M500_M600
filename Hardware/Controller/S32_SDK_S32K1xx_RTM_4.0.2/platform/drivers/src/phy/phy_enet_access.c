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
 * @file phy_enet_access.c
 */
 
#include "phy_hw_access.h"
#include "enet_driver.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HW_WriteCl22
 * Description   : Writes a register of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_HW_WriteCl22(uint8_t instance, uint8_t phyAddr, uint8_t phyReg, uint16_t data, uint32_t timeoutMs)
{
    return ENET_DRV_MDIOWrite(instance, phyAddr, phyReg, data, timeoutMs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HW_ReadCl22
 * Description   : Reads a register of the specified PHY device.
 *
 *END**************************************************************************/
status_t PHY_HW_ReadCl22(uint8_t instance, uint8_t phyAddr, uint8_t phyReg, uint16_t *data, uint32_t timeoutMs)
{
    return ENET_DRV_MDIORead(instance, phyAddr, phyReg, data, timeoutMs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HW_WriteCl45
 * Description   : Writes a register of the specified MMD in a PHY device.
 *
 *END**************************************************************************/
status_t PHY_HW_WriteCl45(uint8_t instance, uint8_t phyAddr, uint8_t mmd, uint16_t phyReg, uint16_t data, uint32_t timeoutMs)
{
    return ENET_DRV_MDIOWriteMMD(instance, phyAddr, mmd, phyReg, data, timeoutMs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HW_ReadCl45
 * Description   : Reads a register of the specified MMD in a PHY device.
 *
 *END**************************************************************************/
status_t PHY_HW_ReadCl45(uint8_t instance, uint8_t phyAddr, uint8_t mmd, uint16_t phyReg, uint16_t *data, uint32_t timeoutMs)
{
    return ENET_DRV_MDIOReadMMD(instance, phyAddr, mmd, phyReg, data, timeoutMs);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
