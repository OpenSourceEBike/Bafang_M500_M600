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

/*!
 * @ingroup phy
 * @addtogroup phy_generic Generic PHY
 * @{
 */

/*!
 * @file phy_generic.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "osif.h"
#include "phy.h"
#include "phy_generic.h"
#include "phy_shared.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define T_RST_MAX 500U  /**< Maximum reset time in ms according to IEEE */

/* IEEE Standardized Registers */

#define BASIC_CONTROL_ADDR      0U
#define ID_1_ADDR               2U
#define ID_2_ADDR               3U
#define AUTONEG_ADVERT_ADDR     4U
#define AUTONEG_LPABILITY_ADDR  5U
#define CONTROL_1000BASET_ADDR  9U
#define STATUS_1000BASET_ADDR  10U
#define EXTENDED_STATUS_ADDR   15U

#define BASIC_CONTROL_RESET_FLAG            0x8000U
#define BASIC_CONTROL_LOOPBACK_FLAG         0x4000U
#define BASIC_CONTROL_AUTONEG_FLAG          0x1000U
#define BASIC_CONTROL_PDOWN_FLAG            0x0800U
#define BASIC_CONTROL_RESTART_AUTONEG_FLAG  0x0200U
#define BASIC_CONTROL_SPEED_LSB_FLAG        0x2000U
#define BASIC_CONTROL_SPEED_MSB_FLAG        0x0040U

#define AUTONEG_100BASETXFD  0x0100U
#define AUTONEG_100BASETXHD  0x0080U
#define AUTONEG_10BASETFD    0x0040U
#define AUTONEG_10BASETHD    0x0020U

#define CONTROL_1000BASET_CONFIG_ENABLE_FLAG 0x1000U
#define CONTROL_1000BASET_CONFIG_VALUE_FLAG  0x0800U

#define AUTONEG_1000BASETFD    0x0200U
#define AUTONEG_1000BASETHD    0x0100U

#define EXTENDED_STATUS_1000BASETHD_FLAG     0x1000U
#define EXTENDED_STATUS_1000BASETFD_FLAG     0x2000U

#define CONTROL_STATUS_1000BASET_OFFSET 0x2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* PHY functions */
static status_t init(uint8_t phy);
static status_t reset(uint8_t phy);
static status_t mainFunction(uint8_t phy);
static status_t getId(uint8_t phy, phy_id_t* id);
static status_t getState(uint8_t phy, phy_state_t* state);
static status_t getLinkStatus(uint8_t phy, bool* linkUp);
static status_t getLinkSpeed(uint8_t phy, phy_speed_t* speed);
static status_t getDelay(uint8_t phy, phy_delay_t *delay);
static status_t suspend(uint8_t phy);
static status_t resume(uint8_t phy);
static status_t enableAutoneg(uint8_t phy);
static status_t disableAutoneg(uint8_t phy);
static status_t restartAutoneg(uint8_t phy);
static status_t setRole(uint8_t phy, phy_role_t role);
static status_t setLoopback(uint8_t phy, phy_loopback_t loopbackMode);

static status_t checkCapabilities(uint8_t phy);
static status_t returnNotSupportUint8Param(uint8_t phy, uint8_t parameter);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*!
 * @brief Generic PHY driver structure.
 */
phy_driver_t PHY_driver_generic =
{
    .init             = init,
    .reset            = reset,
    .mainFunction     = mainFunction,
    .getId            = getId,
    .getState         = getState,
    .getLinkStatus    = getLinkStatus,
    .getLinkSpeed     = getLinkSpeed,
    .getDelay         = getDelay,
    .suspend          = suspend,
    .resume           = resume,
    .sleepAbortReject = PHY_SHARED_ReturnNotSupported,
    .sleep            = PHY_SHARED_ReturnNotSupported,
    .wakeup           = PHY_SHARED_ReturnNotSupported,
    .setRole          = setRole,
    .setLoopback      = setLoopback,
    .selectTestMode   = returnNotSupportUint8Param,
    .enableInterrupts = PHY_SHARED_ReturnNotSupported,
    .handleInterrupt  = PHY_SHARED_ReturnNotSupported,
    .enableAutoneg    = enableAutoneg,
    .disableAutoneg   = disableAutoneg,
    .restartAutoneg   = restartAutoneg,
    .write            = NULL,
    .read             = NULL,
    .rmw              = NULL,
    .writeMMD         = NULL,
    .readMMD          = NULL,
    .rmwMMD           = NULL
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : init
 * Description   : Initializes the specified PHY device.
 *
 *END**************************************************************************/
static status_t init(uint8_t phy)
{
    status_t status;
    uint16_t basicCtrl;
    uint16_t mask;
    phy_speed_t speed = g_phyConfig[phy].speed;

    if (speed != PHY_SPEED_AUTONEG)
    {
        /* set fixed speed */
        mask = (uint16_t) (BASIC_CONTROL_AUTONEG_FLAG | BASIC_CONTROL_SPEED_MSB_FLAG | BASIC_CONTROL_SPEED_LSB_FLAG);
        if (speed == PHY_SPEED_100_MBPS)
        {
            basicCtrl = BASIC_CONTROL_SPEED_LSB_FLAG;
        }
        else if (speed == PHY_SPEED_1_GBPS)
        {
            basicCtrl = BASIC_CONTROL_SPEED_MSB_FLAG;
        }
        else
        {
            basicCtrl = 0;
        }
        status = PHY_RMW(phy, BASIC_CONTROL_ADDR, basicCtrl, mask);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    status = checkCapabilities(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (g_phyConfig[phy].role != PHY_ROLE_AUTO)
    {
        status = PHY_SetRole(phy, g_phyConfig[phy].role);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    g_phyConfig[phy].privateData = (g_phyConfig[phy].privateData | PRIVATE_ACTIVE);  /* assert active bit */
    if (g_phyConfig[phy].manualDelay == false)
    {
        /* delays unspecified for generic device. Store defaults */
        g_phyConfig[phy].delay.miiToMdiMax = 0xFFFFFFFFU;
        g_phyConfig[phy].delay.miiToMdiMin = 0;
        g_phyConfig[phy].delay.mdiToMiiMax = 0xFFFFFFFFU;
        g_phyConfig[phy].delay.mdiToMiiMin = 0;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : reset
 * Description   : Resets the specified PHY device.
 *
 *END**************************************************************************/
static status_t reset(uint8_t phy)
{
    status_t status;
    uint16_t basicCtrl;
    uint32_t startTime, currentTime;

    status = PHY_Write(phy, BASIC_CONTROL_ADDR, BASIC_CONTROL_RESET_FLAG);
    if (status == STATUS_SUCCESS)
    {
        startTime = OSIF_GetMilliseconds();
        do
        {
            status = PHY_Read(phy, BASIC_CONTROL_ADDR, &basicCtrl);
            if (((basicCtrl & BASIC_CONTROL_RESET_FLAG) == 0U) || (status != STATUS_SUCCESS))
            {
                break;
            }
            currentTime = OSIF_GetMilliseconds();
        }
        while (currentTime <= (uint32_t) (startTime + T_RST_MAX));
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mainFunction
 * Description   : Provides polling support, by handling specific events.
 *
 *END**************************************************************************/
static status_t mainFunction(uint8_t phy)
{
    status_t status;
    bool linkUp;
    phy_basicStatus_t basicStatus;

    /* Read basic status register */
    status = PHY_SHARED_ReadBasicStatus(phy, &basicStatus);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    linkUp = basicStatus.link;
    linkUp = ((basicStatus.remoteFault) == true) ? (false) : (linkUp);

    if (linkUp == false)
    {
        if ((g_phyConfig[phy].privateData & PRIVATE_LINKUP) != 0U)
        {
            /* Link down event */
            if (g_phyConfig[phy].linkDownEventCB != NULL)
            {
                g_phyConfig[phy].linkDownEventCB(phy);
            }
            g_phyConfig[phy].privateData = g_phyConfig[phy].privateData & ~PRIVATE_LINKUP;
        }
        /* read basic status again, link might be up because of latching function of link status bit */
        status = PHY_SHARED_ReadBasicStatus(phy, &basicStatus);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }
    if (linkUp == true)
    {
        if ((g_phyConfig[phy].privateData & PRIVATE_LINKUP) == 0U)
        {
            /* Link changed from down to up */
            if (g_phyConfig[phy].linkUpEventCB != NULL)
            {
                g_phyConfig[phy].linkUpEventCB(phy);
            }
            g_phyConfig[phy].privateData = g_phyConfig[phy].privateData | PRIVATE_LINKUP;
            /* if autoneg enabled, check for status */
            if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_ENABLED) != 0U)
            {
                /* start checking for completion of auto-negotiation */
                g_phyConfig[phy].privateData = g_phyConfig[phy].privateData | PRIVATE_ANEG_PENDING;
            }
        }
        if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_PENDING) != 0U)
        {
            if ((basicStatus.autonegComplete) == true)
            {
                /* autoneg complete*/
                if (g_phyConfig[phy].autonegCompleteCB != NULL)
                {
                    g_phyConfig[phy].autonegCompleteCB(phy);
                    g_phyConfig[phy].privateData = g_phyConfig[phy].privateData & ~PRIVATE_ANEG_PENDING;
                }
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : getId
 * Description   : Gets the ID of the specified PHY device.
 *
 *END**************************************************************************/
static status_t getId(uint8_t phy, phy_id_t* id)
{
    status_t status;
    uint16_t id1;
    uint16_t id2;

    /* Get PHY ID from device */
    status = PHY_Read(phy, ID_1_ADDR, &id1);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    status = PHY_Read(phy, ID_2_ADDR, &id2);
    if (status == STATUS_SUCCESS)
    {
        PHY_SHARED_ExtractPhyID(id1, id2, id);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : getState
 * Description   : Gets the current state of the specified PHY device
 *
 *END**************************************************************************/
static status_t getState(uint8_t phy, phy_state_t* state)
{
    status_t status;
    uint16_t basicCtrl;

    status = PHY_Read(phy, BASIC_CONTROL_ADDR, &basicCtrl);
    if (status == STATUS_SUCCESS)
    {
        *state = ((basicCtrl & BASIC_CONTROL_PDOWN_FLAG) != 0U) ? (PHY_STATE_OFF) : (PHY_STATE_ACTIVE);
    }


    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : suspend
 * Description   : Suspends the specified PHY device.
 *
 *END**************************************************************************/
static status_t suspend(uint8_t phy)
{
    g_phyConfig[phy].privateData &= ~PRIVATE_ACTIVE;  /* reset active bit */
    return PHY_RMW(phy, BASIC_CONTROL_ADDR, BASIC_CONTROL_PDOWN_FLAG, BASIC_CONTROL_PDOWN_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : resume
 * Description   : Resumes the specified PHY device.
 *
 *END**************************************************************************/
static status_t resume(uint8_t phy)
{
    g_phyConfig[phy].privateData = (g_phyConfig[phy].privateData | PRIVATE_ACTIVE);  /* assert active bit */
    return PHY_RMW(phy, BASIC_CONTROL_ADDR, 0, BASIC_CONTROL_PDOWN_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : enableAutoneg
 * Description   : Enables auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
static status_t enableAutoneg(uint8_t phy)
{
    status_t status;

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_CAPABLE) == 0U)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        status = PHY_RMW(phy, BASIC_CONTROL_ADDR, BASIC_CONTROL_AUTONEG_FLAG, BASIC_CONTROL_AUTONEG_FLAG);
        g_phyConfig[phy].privateData = (g_phyConfig[phy].privateData | PRIVATE_ANEG_ENABLED);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : disableAutoneg
 * Description   : Disables auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
static status_t disableAutoneg(uint8_t phy)
{
    status_t status;

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_CAPABLE) == 0U)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        status = PHY_RMW(phy, BASIC_CONTROL_ADDR, 0, BASIC_CONTROL_AUTONEG_FLAG);
        g_phyConfig[phy].privateData = (g_phyConfig[phy].privateData & ~PRIVATE_ANEG_ENABLED);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : restartAutoneg
 * Description   : Restarts auto-negotiation for the specified PHY device.
 *
 *END**************************************************************************/
static status_t restartAutoneg(uint8_t phy)
{
    status_t status;

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_CAPABLE) == 0U)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        status = PHY_RMW(phy, BASIC_CONTROL_ADDR, BASIC_CONTROL_RESTART_AUTONEG_FLAG, BASIC_CONTROL_RESTART_AUTONEG_FLAG);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : getLinkStatus
 * Description   : Gets the link status for the specified PHY device.
 *
 *END**************************************************************************/
static status_t getLinkStatus(uint8_t phy, bool* linkUp)
{
    status_t status;
    phy_basicStatus_t basicStatus;

    /* Read basic status register */
    status = PHY_SHARED_ReadBasicStatus(phy, &basicStatus);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    *linkUp = basicStatus.link;
    *linkUp = ((basicStatus.remoteFault) == true) ? (false) : (*linkUp);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : getLinkSpeed
 * Description   : Gets the link speed for the specified PHY device.
 *
 *END**************************************************************************/
static status_t getLinkSpeed(uint8_t phy, phy_speed_t* speed)
{
    status_t status;
    uint16_t reg;
    uint16_t autonegCapabilities;

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_ENABLED) == 0U)
    {
        /* speed in basic control register only valid when autoneg is disabled */
        status = PHY_Read(phy, BASIC_CONTROL_ADDR, &reg);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if ((reg & BASIC_CONTROL_SPEED_MSB_FLAG) != 0U)
        {
            *speed = PHY_SPEED_1_GBPS;
            if ((reg & BASIC_CONTROL_SPEED_LSB_FLAG) != 0U)
            {
                *speed = PHY_SPEED_RESERVED;
            }
        }
        else
        {
            *speed = PHY_SPEED_10_MBPS;
            if ((reg & BASIC_CONTROL_SPEED_LSB_FLAG) != 0U)
            {
                *speed = PHY_SPEED_100_MBPS;
            }
        }
    }
    else
    {
        /* auto negotation enabled - check advertised and link partner capabilities */
        *speed = PHY_SPEED_RESERVED;
        if ((g_phyConfig[phy].privateData & PRIVATE_1000BASET) != 0U)
        {
            status = PHY_Read(phy, CONTROL_1000BASET_ADDR, &autonegCapabilities);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = PHY_Read(phy, STATUS_1000BASET_ADDR, &reg);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            autonegCapabilities &= (uint16_t) (reg >> CONTROL_STATUS_1000BASET_OFFSET);
            if ((autonegCapabilities & (AUTONEG_1000BASETFD | AUTONEG_1000BASETHD)) != 0U)
            {
                *speed = PHY_SPEED_1_GBPS;
            }
        }
        if (*speed == PHY_SPEED_RESERVED)
        {  /* not gigabit, check if 10 or 100 */
            status = PHY_Read(phy, AUTONEG_ADVERT_ADDR, &autonegCapabilities);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = PHY_Read(phy, AUTONEG_LPABILITY_ADDR, &reg);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            autonegCapabilities &= reg;
            if ((autonegCapabilities & (AUTONEG_100BASETXFD | AUTONEG_100BASETXHD)) != 0U)
            {
                *speed = PHY_SPEED_100_MBPS;
            }
            else
            {
                if ((autonegCapabilities & (AUTONEG_10BASETFD | AUTONEG_10BASETHD)) != 0U)
                {
                    *speed = PHY_SPEED_10_MBPS;
                }
            }
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : getDelay
 * Description   : Gets the propagation delays for the specified PHY device.
 *
 *END**************************************************************************/
static status_t getDelay(uint8_t phy, phy_delay_t *delay)
{
    *delay = g_phyConfig[phy].delay;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : setRole
 * Description   : Sets the role of the specified PHY device.
 *
 *END**************************************************************************/
static status_t setRole(uint8_t phy, phy_role_t role)
{
    status_t status;
    uint16_t reg;

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_MSCTRL) != 0U)
    {
        if (role != PHY_ROLE_AUTO)
        {
            reg = CONTROL_1000BASET_CONFIG_ENABLE_FLAG;
            if (role == PHY_ROLE_MASTER)
            {
                reg |= CONTROL_1000BASET_CONFIG_VALUE_FLAG;
            }
            status = PHY_RMW(phy, CONTROL_1000BASET_ADDR, reg, (uint16_t) CONTROL_1000BASET_CONFIG_ENABLE_FLAG | (uint16_t) CONTROL_1000BASET_CONFIG_VALUE_FLAG);
        }
        else
        {
            status = PHY_RMW(phy, CONTROL_1000BASET_ADDR, 0, CONTROL_1000BASET_CONFIG_ENABLE_FLAG);  /* Deactivate manual configuration */
        }
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        g_phyConfig[phy].role = role;  /* Update role in local shadow */
    }
    else
    {
        /* No master/slave selection supported */
        status = STATUS_UNSUPPORTED;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : setLoopback
 * Description   : Sets the loopback mode for the specified PHY device.
 *
 *END**************************************************************************/
static status_t setLoopback(uint8_t phy, phy_loopback_t loopbackMode)
{
    status_t status;

    switch (loopbackMode)
    {
        case PHY_LOOPBACK_INTERNAL:
            /* Fallthrough */
            /* No differentiation between internal and external loopback for the generic PHY */
        case PHY_LOOPBACK_EXTERNAL:
            status = PHY_RMW(phy, BASIC_CONTROL_ADDR, BASIC_CONTROL_LOOPBACK_FLAG, BASIC_CONTROL_LOOPBACK_FLAG);
            break;
        case PHY_LOOPBACK_NONE:
            status = PHY_RMW(phy, BASIC_CONTROL_ADDR, 0, BASIC_CONTROL_LOOPBACK_FLAG);
            break;
        default:
            status = STATUS_UNSUPPORTED;
            break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : checkCapabilities
 * Description   : Checks the capabilities of the specified PHY device.
 *
 *END**************************************************************************/
static status_t checkCapabilities(uint8_t phy)
{
    status_t status;
    bool autonegMasterSlave = false;
    uint16_t reg;
    phy_basicStatus_t basicStatus;

    /* Read basic status register */
    status = PHY_SHARED_ReadBasicStatus(phy, &basicStatus);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Store Autoneg flag based on capability */
    g_phyConfig[phy].privateData = ((basicStatus.autonegAbility) == true) ?
                                    (g_phyConfig[phy].privateData | PRIVATE_ANEG_CAPABLE) :
                                    (g_phyConfig[phy].privateData & ~PRIVATE_ANEG_CAPABLE);

    if ((basicStatus.baseT2Hd || basicStatus.baseT2Fd) != false)
    {
        /* 100BASE-T2 support */
        autonegMasterSlave = true;
    }

    if (basicStatus.extendedStatus == true)
    {
        status = PHY_Read(phy, EXTENDED_STATUS_ADDR, &reg);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        if ((reg & (EXTENDED_STATUS_1000BASETFD_FLAG | EXTENDED_STATUS_1000BASETHD_FLAG)) != 0U)
        {
            /* 1000BASE-T support */
            autonegMasterSlave = true;
            g_phyConfig[phy].privateData |= PRIVATE_1000BASET;
        }
        else
        {
            g_phyConfig[phy].privateData &= ~PRIVATE_1000BASET;
        }
    }

    g_phyConfig[phy].privateData = (autonegMasterSlave == false) ?
                                    (g_phyConfig[phy].privateData & ~PRIVATE_ANEG_MSCTRL) :
                                    (g_phyConfig[phy].privateData | PRIVATE_ANEG_MSCTRL);

    if ((g_phyConfig[phy].privateData & PRIVATE_ANEG_CAPABLE) != 0U)
    {
        /* check default setting for auto-negotiation */
        status = PHY_Read(phy, BASIC_CONTROL_ADDR, &reg);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        g_phyConfig[phy].privateData = ((reg & BASIC_CONTROL_AUTONEG_FLAG) == 0U) ?
                                        (g_phyConfig[phy].privateData & ~PRIVATE_ANEG_ENABLED) :
                                        (g_phyConfig[phy].privateData | PRIVATE_ANEG_ENABLED);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : returnNotSupportUint8Param
 * Description   : Unsupported framework functions return STATUS_UNSUPPORTED.
 *
 *END**************************************************************************/
static status_t returnNotSupportUint8Param(uint8_t phy, uint8_t parameter)
{
    (void)(phy);
    (void)(parameter);
    return STATUS_UNSUPPORTED;
}

/*! @}*/

/*******************************************************************************
 * EOF
 ******************************************************************************/
