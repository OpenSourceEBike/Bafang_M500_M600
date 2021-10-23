/*
 * Copyright 2017-2018,2020 NXP
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

#ifndef PHY_TJA110X_H
#define PHY_TJA110X_H

#include "phy.h"

/*!
 * @brief PHY driver for TJA110x series
 * 
 * ## Supported devices ##
 *
 * This driver supports TJA1110, TJA1101, TJA1102, TJA1102S devices.
 * It reuses functions of \ref phy_generic
 *
 * ## Unsupported API ##
 *
 * The following functions are not supported by this driver:
 *  - PHY_EnableAutoneg
 *  - PHY_DisableAutoneg
 *  - PHY_RestartAutoneg
 * 
 * Note: Wakeup functionality is not supported on TJA1100
 *
 * ## Device specific configuration options ##
 *
 * Device specific configuration options are represented by the <b>phy_tja110x_config_t</b> structure and include:
 *  - Local wake-up forwarding
 *  - Remote wake-up forwarding
 *  - MII output driver strength reduced
 *  - CLK_IN_OUT output driver strength reduced
 *  - XTAL and CLK_IN_OUT output remain active until device switched to Sleep mode via SMI
 *  - Local wake configuration: ratiometric input threshold
 *  - Local wake-up timer
 *  - Clock mode
 * 
 * Note: Device specific configuration options are not available for TJA1100
 *
 * @ingroup phy_driver
 * @addtogroup phy_tja110x TJA110x PHY
 * @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

/*!
 * @brief TJA110x PHY local wakeup pin timer
 */
typedef enum
{
    PHY_TJA110X_LOCALWUTIME_SHORTEST = 3,  /**< shortest (10 us to 40 us) */
    PHY_TJA110X_LOCALWUTIME_SHORT    = 2,  /**< short (100 us to 200 us) */
    PHY_TJA110X_LOCALWUTIME_LONG     = 1,  /**< long (250 us to 500 us) */
    PHY_TJA110X_LOCALWUTIME_LONGEST  = 0   /**< longest (10 ms to 20 ms) */
} phy_tja110x_localWuTime_t;

/*!
 * @brief TJA110x PHY clk mode
 */
typedef enum
{
    PHY_TJA110X_CLKMODE_50MHZ_EXT_REFCLK       = 3,  /**< shortest (10 us to 40 us) */
    PHY_TJA110X_CLKMODE_25MHZ_EXT_CLK          = 2,  /**< short (100 us to 200 us) */
    PHY_TJA110X_CLKMODE_XTAL_WITH_25MHZ_OUTPUT = 1,  /**< long (250 us to 500 us) */
    PHY_TJA110X_CLKMODE_XTAL_NO_OUTPUT         = 0   /**< longest (10 ms to 20 ms) */
} phy_tja110x_clkMode_t;

/*!
 * @brief TJA110x PHY extended configuration structure
 */
typedef struct
{
	bool fwdPhyLocal;                 /**< local wake-up forwarding */
	bool fwdPhyRemote;                /**< remote wake-up forwarding */
	bool localWakeupPhy;              /**< local wake-up */
	bool remoteWakeupPhy;             /**< remote wake-up */
	bool reducedMiiDriveStrength;     /**< MII output driver strength reduced */
	bool reducedClkDriveStrength;     /**< CLK_IN_OUT output driver strength reduced */
	bool clkHold;                     /**< XTAL and CLK_IN_OUT output remain active until device switched to Sleep mode via SMI */
	bool wakeInRatiometricThreshold;  /**< local wake configuration: ratiometric input threshold */
	phy_tja110x_localWuTime_t localWuTimer;  /**< local wake-up timer */
phy_tja110x_clkMode_t clkMode;  /**< clock mode */

} phy_tja110x_config_t;

/*!
 * @brief TJA110x PHY driver structure.
 */
extern phy_driver_t PHY_driver_tja110x;

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* PHY_TJA110X_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/

