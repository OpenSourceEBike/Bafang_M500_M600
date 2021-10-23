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

#ifndef PHY_H
#define PHY_H

#include <stdint.h>
#include <stdbool.h>
#include "status.h"

#include "device_registers.h"
#include "floats.h"

/* */
/* */
/* */

/*!
 * @defgroup phy_driver PHY Framework
 * @brief PHY Driver containing API for operating PHYs managed in the PHY framework
 * @ingroup phy
 * @addtogroup phy_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Link speed.
 *
 * Implements : phy_speed_t_Class
 */
typedef enum
{
    PHY_SPEED_1_GBPS   = 2,  /**< 1 Gbps */
    PHY_SPEED_100_MBPS = 1,  /**< 100 Mbps */
    PHY_SPEED_10_MBPS  = 0,  /**< 10 Mbps */
    PHY_SPEED_AUTONEG  = -1, /**< Auto-negotiation */
    PHY_SPEED_RESERVED = -2  /**< Reserved */
} phy_speed_t;

/*!
 * @brief PHY state
 *
 * Implements : phy_state_t_Class
 */
typedef enum
{
    PHY_STATE_LINKTRAINING = 7,  /**< PHY is in link training mode */
    PHY_STATE_DISABLED     = 6,  /**< PHY disabled */
    PHY_STATE_SILENT       = 5,  /**< PHY in silent mode */
    PHY_STATE_SLEEPREQ     = 4,  /**< PHY in sleep request state */
    PHY_STATE_SLEEP        = 3,  /**< PHY in low power sleep mode */
    PHY_STATE_ACTIVE       = 2,  /**< PHY active/normal mode */
    PHY_STATE_OFF          = 1,  /**< PHY switched off/standby/power down */
    PHY_STATE_UNKNOWN      = 0   /**< PHY state unknown, e.g. during state transition */
} phy_state_t;


/*!
 * @brief PHY role.
 *
 * Implements : phy_role_t_Class
 */
typedef enum
{
    PHY_ROLE_AUTO   = 2,  /**< Automatic configuration, i.e. use the default of the device. Could be pin strapping or auto-negotiation of master/slave parameter. */
    PHY_ROLE_MASTER = 1,  /**< Master */
    PHY_ROLE_SLAVE  = 0   /**< Slave */
} phy_role_t;

/*!
 * @brief PHY location.
 *
 * Implements : phy_location_t_Class
 */
typedef enum
{
    PHY_LOCATION_EXTERNAL = 1,  /**< External PHY */
    PHY_LOCATION_INTERNAL = 0   /**< Internal PHY */
} phy_location_t;

/*!
 * @brief Loopback modes.
 *
 * Implements : phy_loopback_t_Class
 */
typedef enum
{
    PHY_LOOPBACK_REMOTE   = 3,  /**< Remote loopback */
    PHY_LOOPBACK_EXTERNAL = 2,  /**< External loopback */
    PHY_LOOPBACK_INTERNAL = 1,  /**< Internal loopback */
    PHY_LOOPBACK_NONE     = 0   /**< No loopback */
} phy_loopback_t;

/*!
 * @brief PHY ID.
 *
 * Implements : phy_id_t_Class
 */
typedef struct
{
    uint32_t oui;         /**< 24-bit Organizationally Unique Identifier (OUI) */
    uint8_t  typeNo;      /**< 6-bit manufacturer type number */
    uint8_t  revisionNo;  /**< Four-bit manufacturer revision number */
} phy_id_t;

/*!
 * @brief PHY Delay.
 *
 * Implements : phy_delay_t_Class
 */
typedef struct
{
    uint32_t miiToMdiMax;  /**< Maximum propagation delay from MII to MDI */
    uint32_t miiToMdiMin;  /**< Minimum propagation delay from MII to MDI */
    uint32_t mdiToMiiMax;  /**< Maximum propagation delay from MDI to MII */
    uint32_t mdiToMiiMin;  /**< Minimum propagation delay from MDI to MII */
} phy_delay_t;

/*!
 * @brief PHY configuration structure.
 *
 * Implements : phy_config_t_Class
 */
typedef struct
{
    uint8_t type;             /**< type of driver used for this PHY instance. Will refer to an index in ::gap_phyDrivers */
    uint32_t addr;            /**< SMI/MDIO Address for external PHY, Base Address for integrated PHY */
    phy_location_t location;  /**< Internal: PHY integrated with host device and addressable via memory mapped IO. External: PHY outside of device and addressable via MDIO/SMI */
    bool clause45;            /**< For MDIO/SMI managed devices: If true, access is performed using IEEE 802.3 Clause 45 MDIO frames. If false, using IEEE 802.3 Clause 22 MDIO frames */
    phy_speed_t speed;        /**< Initial speed setting for the PHY */
    phy_role_t role;          /**< Role of the PHY, e.g. Master or Slave */
    uint8_t enetInstance;     /**< The instance number of the ENET module connected to this PHY */
    void (*linkUpEventCB)(uint8_t phy);        /**< Callback for the link up event */
    void (*linkDownEventCB)(uint8_t phy);      /**< Callback for the link down event */
    void (*sleepRequestCB)(uint8_t phy);       /**< Callback for the sleep request event */
    void (*autonegCompleteCB)(uint8_t p_phy);  /**< Callback for the auto-negotiation complete event */
    void (*autonegFailedCB)(uint8_t p_phy);    /**< Callback for the auto-negotiation failed event */
    bool manualDelay;         /**< Delay values are configured statically */
    phy_delay_t delay;        /**< Delay values */
    uint32_t privateData;     /**< Private data which can be used by the driver */
    void *extendedConfig;     /**< Extended configuration (PHY specific) */
} phy_config_t;

/*!
 * @brief Structure containing the APIs of a driver compatible with this PHY framework.
 *
 * Implements : phy_driver_t_Class
 */
typedef struct
{
    status_t (*init)(uint8_t phy);
    status_t (*reset)(uint8_t phy);
    status_t (*mainFunction)(uint8_t phy);
    status_t (*getId)(uint8_t phy, phy_id_t* id);
    status_t (*getState)(uint8_t phy, phy_state_t* state);
    status_t (*getLinkStatus)(uint8_t phy, bool* linkUp);
    status_t (*getLinkSpeed)(uint8_t phy, phy_speed_t* speed);
    status_t (*getDelay)(uint8_t phy, phy_delay_t *delay);
    status_t (*suspend)(uint8_t phy);
    status_t (*resume)(uint8_t phy);
    status_t (*sleepAbortReject)(uint8_t phy);
    status_t (*sleep)(uint8_t phy);
    status_t (*wakeup)(uint8_t phy);
    status_t (*setRole)(uint8_t phy, phy_role_t role);
    status_t (*setLoopback)(uint8_t phy, phy_loopback_t loopbackMode);
    status_t (*selectTestMode)(uint8_t phy, uint8_t testModeIndex);
    status_t (*enableInterrupts)(uint8_t phy);
    status_t (*handleInterrupt)(uint8_t phy);
    status_t (*enableAutoneg)(uint8_t phy);
    status_t (*disableAutoneg)(uint8_t phy);
    status_t (*restartAutoneg)(uint8_t phy);
    status_t (*write)(uint8_t phy, uint8_t registerAddr, uint16_t value);
    status_t (*read)(uint8_t phy, uint8_t registerAddr, uint16_t* value);
    status_t (*rmw)(uint8_t phy, uint8_t registerAddr, uint16_t value, uint16_t mask);
    status_t (*writeMMD)(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value);
    status_t (*readMMD)(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t *value);
    status_t (*rmwMMD)(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value, uint16_t mask);
} phy_driver_t;

/*! @brief Pointer to the array of PHY configuration structures. */
extern phy_config_t* g_phyConfig;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * @name Driver initialization
 * @{
 */

/*!
 * @brief Initializes the PHY driver.
 *
 * This function initializes the internal state of the driver and enables the MDIO interface.
 * Important: PHY_FrameworkInit must be invoked prior to using the rest of the API.
 *
 * @param[in] phyConfig Array of PHY configuration structures.
 * @param[in] phyDrivers Array of pointers to the PHY drivers used by the application.
 */
status_t PHY_FrameworkInit(phy_config_t *phyConfig, phy_driver_t **phyDrivers);

/*! @} */

/*!
 * @name Basic PHY functionality
 * @{
 */

/*!
 * @brief Initializes the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Init(uint8_t phy);

/*!
 * @brief Resets the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Reset(uint8_t phy);

/*!
 * @brief Provides polling support, by handling specific events.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_MainFunction(uint8_t phy);

/*!
 * @brief Gets the ID of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[out] id The ID of the PHY.
 */
status_t PHY_GetId(uint8_t phy, phy_id_t *id);

/*!
 * @brief Gets the current state of the specified PHY device
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[out] state The current state of the PHY
 */
status_t PHY_GetState(uint8_t phy, phy_state_t* state);

/*!
 * @brief Gets the link status for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[out] linkUp The link status.
 */
status_t PHY_GetLinkStatus(uint8_t phy, bool *linkUp);

/*!
 * @brief Gets the link speed for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[out] speed The link speed.
 */
status_t PHY_GetLinkSpeed(uint8_t phy, phy_speed_t *speed);

/*!
 * @brief Gets the propagation delay within the PHY.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[out] delay The PHY propagation delays
 */
status_t PHY_GetDelay(uint8_t phy, phy_delay_t *delay);

/*!
 * @brief Suspends the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Suspend(uint8_t phy);

/*!
 * @brief Resumes the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Resume(uint8_t phy);

/*!
 * @brief Aborts an initiated sleep request or rejects an received sleep request.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_SleepAbortReject(uint8_t phy);

/*!
 * @brief Sends a sleep request to the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Sleep(uint8_t phy);

/*!
 * @brief Wakes up the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_Wakeup(uint8_t phy);

/*!
 * @brief Sets the role of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] role The role to be selected for the PHY.
 */
status_t PHY_SetRole(uint8_t phy, phy_role_t role);

/*!
 * @brief Sets the loopback mode for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] loopbackMode The loopback type to be configured.
 */
status_t PHY_SetLoopback(uint8_t phy, phy_loopback_t loopbackMode);

/*!
 * @brief Sets the test mode for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] testModeIndex Index of the test mode to be started. 0 means no test mode.
 */
status_t PHY_SelectTestMode(uint8_t phy, uint8_t testModeIndex);

/*!
 * @brief Enables interrupts for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_EnableInterrupts(uint8_t phy);

/*!
 * @brief Handles interrupts for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_HandleInterrupt(uint8_t phy);

/*! @} */

/*!
 * @name Auto-negotiation
 * @{
 */

/*!
 * @brief Enables auto-negotiation for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_EnableAutoneg(uint8_t phy);

/*!
 * @brief Disables auto-negotiation for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_DisableAutoneg(uint8_t phy);

/*!
 * @brief Restarts auto-negotiation for the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 */
status_t PHY_RestartAutoneg(uint8_t phy);

/*! @} */

/*!
 * @name Register Access
 * @{
 */

/*!
 * @brief Writes a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] registerAddr The address of the register to be written.
 * @param[in] value The value to be written.
 */
status_t PHY_Write(uint8_t phy, uint8_t registerAddr, uint16_t value);

/*!
 * @brief Reads a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] registerAddr The address of the register to be read.
 * @param[out] value The value of the register.
 */
status_t PHY_Read(uint8_t phy, uint8_t registerAddr, uint16_t *value);

/*!
 * @brief Reads and modifies a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] registerAddr The address of the register to be written.
 * @param[in] value The value to be written.
 * @param[in] mask The mask representing the bits which should be modified.
 */
status_t PHY_RMW(uint8_t phy, uint8_t registerAddr, uint16_t value, uint16_t mask);

/*!
 * @brief Writes a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] mmd The MMD index of the target register.
 * @param[in] registerAddr The address of the register to be written.
 * @param[in] value The value to be written.
 */
status_t PHY_WriteMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value);

/*!
 * @brief Reads a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] mmd The MMD index of the target register.
 * @param[in] registerAddr The address of the register to be read.
 * @param[out] value The value of the register.
 */
status_t PHY_ReadMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t *value);

/*!
 * @brief Reads and modifies a register of the specified PHY device.
 *
 * @param[in] phy The device number, representing the index in the array of PHY configurations.
 * @param[in] mmd The MMD index of the target register.
 * @param[in] registerAddr The address of the register to be written.
 * @param[in] value The value to be written.
 * @param[in] mask The mask representing the bits which should be modified.
 */
status_t PHY_RMWMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value, uint16_t mask);

/*! @} */

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* PHY_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
