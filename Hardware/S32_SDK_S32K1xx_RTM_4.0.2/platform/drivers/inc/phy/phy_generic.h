/*
 * Copyright 2017-2020 NXP
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

#ifndef PHY_GENERIC_H
#define PHY_GENERIC_H

#include "phy.h"

/*!
 * @brief Generic PHY driver for 10/100/1000 PHYs following IEEE 802.3-Clause 22
 * 
 * ## Supported devices ##
 *
 * This driver supports generic devices, supporting Clause 22 IEEE standardized registers.
 * This includes typical 10/100/1000 PHYs like 100BASE-TX or 1000BASE-T
 *
 * ## Unsupported API ##
 *
 * The following functions are not supported by this driver:
 *  - PHY_SleepAbortReject
 *  - PHY_Sleep
 *  - PHY_Wakeup
 *  - PHY_SelectTestMode
 *  - PHY_EnableInterrupts
 *  - PHY_HandleInterrupt
 *
 * ## Device specific configuration options ##
 *
 * N/A
 *
 * @ingroup phy_driver
 * @addtogroup phy_generic Generic PHY
 * @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

/*!
 * @brief Generic PHY driver structure.
 */
extern phy_driver_t PHY_driver_generic;

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* PHY_GENERIC_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/

