/*
 * Copyright 2017-2018,2020-2021 NXP
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
 * @addtogroup phy_tja110x TJA110x PHY
 * @{
 */

/*!
 * @file phy_tja110x.c
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
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * This structure is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to pointer
 * to other type
 * This is intended functionality to have a driver specific configuration data structure.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, cast performed between a pointer to object
 * type and a pointer to a different object type
 * This is intended functionality to have a driver specific configuration data structure.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "phy.h"
#include "phy_tja110x.h"
#include "phy_generic.h"
#include "phy_shared.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Registers */

#define OUI                0x0006ECU /**< Expected OUI across all devices covered by this driver */
#define TJA1102P0_TYPE_NO  0x8U
#define TJA1100_TYPE_NO    0x4U

#define BASIC_CTRL_ADDR     0U
#define EXTENDED_CTRL_ADDR  17U
#define CONFIG1_ADDR        18U
#define CONFIG2_ADDR        19U
#define IRQ_STATUS_ADDR     21U
#define IRQ_ENABLE_ADDR     22U
#define COMM_STATUS_ADDR    23U
#define GENERAL_STATUS_ADDR 24U
#define COMMON_CONFIG_ADDR  27U

#define BASIC_CTRL_LOOPBACK_FLAG             0x4000U

#define EXTENDED_CTRL_LINK_CTRL_FLAG         0x8000U
#define EXTENDED_CTRL_LOOPBACK_MODE_MASK     0x0018U
#define EXTENDED_CTRL_LOOPBACK_MODE_INTERNAL 0x0000U
#define EXTENDED_CTRL_LOOPBACK_MODE_EXTERNAL 0x0008U
#define EXTENDED_CTRL_LOOPBACK_MODE_REMOTE   0x0018U
#define EXTENDED_CTRL_POWER_MODE_MASK        0x7800U
#define EXTENDED_CTRL_POWER_MODE_SLEEP_REQ   0x5800U
#define EXTENDED_CTRL_POWER_MODE_NORMAL_REQ  0x1800U
#define EXTENDED_CTRL_POWER_MODE_STANDBY     0x6000U
#define EXTENDED_CTRL_POWER_MODE_SLEEP       0x5000U
#define EXTENDED_CTRL_POWER_MODE_SILENT      0x4800U
#define EXTENDED_CTRL_TEST_MODE_MASK         0x01C0U
#define EXTENDED_CTRL_WAKE_REQ_FLAG          0x0001U
#define EXTENDED_CTRL_CONFIG_EN_FLAG         0x0004U
#define EXTENDED_CTRL_TEST_MODE_SHIFT        6U

#define CONFIG1_MASTERSLAVE_FLAG             0x8000U
#define CONFIG1_FWDPHYLOC_FLAG               0x4000U
#define CONFIG1_REMWUPHY_FLAG                0x0800U
#define CONFIG1_LOCWUPHY_FLAG                0x0400U
#define CONFIG1_MII_MODE_MASK                0x0300U
#define CONFIG1_MII_DRIVER_FLAG              0x0080U
#define CONFIG1_SLEEPCONFIRM_FLAG            0x0040U
#define CONFIG1_SLEEPACK_FLAG                0x0010U
#define CONFIG1_FWDPHYREM_FLAG               0x0004U
#define CONFIG1_MII_MODE_SHIFT               8U

#define CONFIG2_SLEEPREQUESTTO_MASK          0x0003U
#define CONFIG2_SLEEPREQUESTTO_16_8          0x0003U

#define IRQ_PWON_FLAG                        0x8000U
#define IRQ_WAKEUP_FLAG                      0x4000U
#define IRQ_LPS_RECEIVED_FLAG                0x1000U
#define IRQ_LINKUP_FLAG                      0x0200U
#define IRQ_LINKDOWN_FLAG                    0x0400U

#define COMM_STATUS_LINKUP_FLAG              0x8000U

#define GENERAL_STATUS_INT_STATUS_FLAG       0x8000U
#define GENERAL_STATUS_PLL_LOCKED_FLAG       0x4000U
#define GENERAL_STATUS_ENSTATUS_FLAG         0x0400U
#define GENERAL_STATUS_RSTSTATUS_FLAG        0x0200U

#define COMMON_CONFIG_AUTO_OP_MASK           0x8000U
#define COMMON_CONFIG_CLK_MODE_MASK          0x3000U
#define COMMON_CONFIG_CLK_MODE_LSB           12U
#define COMMON_CONFIG_CLK_DRIVER_FLAG        0x0400U
#define COMMON_CONFIG_CLK_HOLD_FLAG          0x0200U
#define COMMON_CONFIG_LOC_WU_TIM_MASK        0x0180U
#define COMMON_CONFIG_LOC_WU_TIM_SHORTEST    0x0180U
#define COMMON_CONFIG_LOC_WU_TIM_SHORT       0x0100U
#define COMMON_CONFIG_LOC_WU_TIM_LONG        0x0080U

#define COMMON_CONFIG_CONFIG_WAKE_FLAG       0x0040U


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* PHY functions */
static status_t init(uint8_t phy);
static status_t getState(uint8_t phy, phy_state_t* state);
static status_t sleepAbortReject(uint8_t phy);
static status_t sleep(uint8_t phy);
static status_t wakeup(uint8_t phy);
static status_t setRole(uint8_t phy, phy_role_t role);
static status_t setLoopback(uint8_t phy, phy_loopback_t loopbackMode);
static status_t selectTestMode(uint8_t phy, uint8_t testModeIndex);
static status_t enableInterrupts(uint8_t phy);
static status_t handleInterrupt(uint8_t phy);
static status_t resume(uint8_t phy);

/* Internal/Helper functions */
static status_t writeExtendedConfig(uint8_t phy, bool config1, bool commonConfig);
static status_t handleInterruptPwon(uint8_t phy);
static status_t handleInterruptLinkUp(uint8_t phy);
static void     handleInterruptLinkDown(uint8_t phy);
static status_t handleInterruptWakeUp(uint8_t phy);
static void     handleInterruptSleep(uint8_t phy);
static status_t goFromStandbyToNormalMode(uint8_t phy);
static status_t setManagedOperation(uint8_t phy);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*!
 * @brief TJA110x PHY driver structure.
 */
phy_driver_t PHY_driver_tja110x =
{
    .init             = init,
    .reset            = NULL,
    .mainFunction     = handleInterrupt,
    .getId            = NULL,
    .getState         = getState,
    .getLinkStatus    = NULL,
    .getLinkSpeed     = NULL,
    .getDelay         = NULL,
    .suspend          = NULL,
    .resume           = resume,
    .sleepAbortReject = sleepAbortReject,
    .sleep            = sleep,
    .wakeup           = wakeup,
    .setRole          = setRole,
    .setLoopback      = setLoopback,
    .selectTestMode   = selectTestMode,
    .enableInterrupts = enableInterrupts,
    .handleInterrupt  = handleInterrupt,
    .enableAutoneg    = NULL,
    .disableAutoneg   = NULL,
    .restartAutoneg   = NULL,
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
 * Function Name : enableLink
 * Description   : Enables the link for the specified PHY device.
 *
 *END**************************************************************************/
static inline status_t enableLink(uint8_t phy)
{
    return PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_LINK_CTRL_FLAG, EXTENDED_CTRL_LINK_CTRL_FLAG);
}

/*FUNCTION**********************************************************************
*
* Function Name : disableLink
* Description   : Disables the link for the specified PHY device.
*
*END**************************************************************************/
static inline status_t disableLink(uint8_t phy)
{
   return PHY_RMW(phy, EXTENDED_CTRL_ADDR, 0U, EXTENDED_CTRL_LINK_CTRL_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : enableConfig
 * Description   : Enables the extended configuration registers for the specified PHY device.
 *
 *END**************************************************************************/
static inline status_t enableConfig(uint8_t phy)
{
    return PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_CONFIG_EN_FLAG, EXTENDED_CTRL_CONFIG_EN_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : disableConfig
 * Description   : Disables the extended configuration registers for the specified PHY device.
 *
 *END**************************************************************************/
static inline status_t disableConfig(uint8_t phy)
{
    return PHY_RMW(phy, EXTENDED_CTRL_ADDR, 0, EXTENDED_CTRL_CONFIG_EN_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : init
 * Description   : Initializes the specified PHY device.
 *
 *END**************************************************************************/
static status_t init(uint8_t phy)
{
    status_t status;
    static bool initialized;
    phy_id_t id;
    uint16_t config1;
    uint8_t miiMode;

    if (initialized == false)
    {
        /* Make mapping to generic functions on first invocation of init function */
        PHY_driver_tja110x.reset          = PHY_driver_generic.reset;
        PHY_driver_tja110x.getId          = PHY_driver_generic.getId;
        PHY_driver_tja110x.getLinkStatus  = PHY_driver_generic.getLinkStatus;
        PHY_driver_tja110x.getLinkSpeed   = PHY_driver_generic.getLinkSpeed;
        PHY_driver_tja110x.getDelay       = PHY_driver_generic.getDelay;
        PHY_driver_tja110x.suspend        = PHY_driver_generic.suspend;
        PHY_driver_tja110x.enableAutoneg  = PHY_SHARED_ReturnNotSupported;
        PHY_driver_tja110x.disableAutoneg = PHY_SHARED_ReturnNotSupported;
        PHY_driver_tja110x.restartAutoneg = PHY_SHARED_ReturnNotSupported;

        initialized = true;
    }

    status = PHY_driver_tja110x.getId(phy, &id);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = ((id.oui != OUI) ? (STATUS_PHY_INCOMPATIBLE_DEVICE) : (STATUS_SUCCESS));
    status = (((id.oui == 0U) && (id.typeNo == 0U) && (id.revisionNo == 0U)) ? (STATUS_SUCCESS) : (status));  /* If everything is 0, this is TJA1102_P1 */
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (id.typeNo == TJA1100_TYPE_NO)
    {
        g_phyConfig[phy].privateData |= PRIVATE_TJA1100;
    }


    status = handleInterrupt(phy);  /* Handle any interrupts pending before initialization */
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (g_phyConfig[phy].manualDelay == false)
    {
        /* Read MII mode */
        status = PHY_Read(phy, CONFIG1_ADDR, &config1);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        miiMode = (uint8_t) ((config1 & CONFIG1_MII_MODE_MASK) >> CONFIG1_MII_MODE_SHIFT);

        if (miiMode == 0U)
        {
            /* MII mode */
            g_phyConfig[phy].delay.miiToMdiMax = 300;
            g_phyConfig[phy].delay.miiToMdiMin = 140;
            g_phyConfig[phy].delay.mdiToMiiMax = 920;
            g_phyConfig[phy].delay.mdiToMiiMin = 760;
        }
        else
        {
            /* RMII mode */
            g_phyConfig[phy].delay.miiToMdiMax = 540;
            g_phyConfig[phy].delay.miiToMdiMin = 190;
            g_phyConfig[phy].delay.mdiToMiiMax = 1070;
            g_phyConfig[phy].delay.mdiToMiiMin = 700;
        }
    }

    return STATUS_SUCCESS;
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
    uint16_t extCtrl;

    status = PHY_Read(phy, EXTENDED_CTRL_ADDR, &extCtrl);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    extCtrl &= EXTENDED_CTRL_POWER_MODE_MASK;

    switch (extCtrl)
    {
        case EXTENDED_CTRL_POWER_MODE_SLEEP:
            *state = PHY_STATE_SLEEP;
            break;
        case EXTENDED_CTRL_POWER_MODE_STANDBY:
            *state = PHY_STATE_OFF;
            break;
        case EXTENDED_CTRL_POWER_MODE_SILENT:
            *state = PHY_STATE_SILENT;
            break;
        case EXTENDED_CTRL_POWER_MODE_NORMAL_REQ:
            *state = PHY_STATE_ACTIVE;
            break;
        case EXTENDED_CTRL_POWER_MODE_SLEEP_REQ:
            *state = PHY_STATE_SLEEPREQ;
            break;
        default:
            *state = PHY_STATE_UNKNOWN;
            break;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sleepAbortReject
 * Description   : Aborts an initiated sleep request or rejects an received sleep request.
 *
 *END**************************************************************************/
static status_t sleepAbortReject(uint8_t phy)
{
    return PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_POWER_MODE_NORMAL_REQ, EXTENDED_CTRL_POWER_MODE_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sleep
 * Description   : Sends a sleep request to the specified PHY device.
 *
 *END**************************************************************************/
static status_t sleep(uint8_t phy)
{
    return PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_POWER_MODE_SLEEP_REQ, EXTENDED_CTRL_POWER_MODE_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : wakeup
 * Description   : Wakes up the specified PHY device.
 *
 *END**************************************************************************/
static status_t wakeup(uint8_t phy)
{
    status_t status;
    uint16_t extCtrl;

    if ((g_phyConfig[phy].privateData & PRIVATE_TJA1100) != 0U)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        status = PHY_Read(phy, EXTENDED_CTRL_ADDR, &extCtrl);

        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if ((extCtrl & EXTENDED_CTRL_POWER_MODE_MASK) == EXTENDED_CTRL_POWER_MODE_SLEEP)
        {
            status = PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_POWER_MODE_STANDBY, EXTENDED_CTRL_POWER_MODE_MASK);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }
        else
        {
            status = PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_WAKE_REQ_FLAG, EXTENDED_CTRL_WAKE_REQ_FLAG);
        }
    }

    return status;
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

    if (role == PHY_ROLE_AUTO)
    {
        return STATUS_UNSUPPORTED;
    }
    else
    {
        status = disableLink(phy);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        status = enableConfig(phy);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        reg = ((role == PHY_ROLE_MASTER) ? (uint16_t)(CONFIG1_MASTERSLAVE_FLAG) : (uint16_t)0U);

        status = PHY_RMW(phy, CONFIG1_ADDR, reg, CONFIG1_MASTERSLAVE_FLAG);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        g_phyConfig[phy].role = role;  /* Update role in local shadow */

        status = disableConfig(phy);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        status = enableLink(phy);
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
    status_t status = STATUS_SUCCESS;
    uint16_t extCtrl = 0;
    uint16_t basicCtrl = BASIC_CTRL_LOOPBACK_FLAG;

    switch (loopbackMode)
    {
        case PHY_LOOPBACK_INTERNAL:
            extCtrl = EXTENDED_CTRL_LOOPBACK_MODE_INTERNAL;
            break;
        case PHY_LOOPBACK_EXTERNAL:
            extCtrl = EXTENDED_CTRL_LOOPBACK_MODE_EXTERNAL;
            break;
        case PHY_LOOPBACK_REMOTE:
            extCtrl = EXTENDED_CTRL_LOOPBACK_MODE_REMOTE;
            break;
        case PHY_LOOPBACK_NONE:
            extCtrl = 0;
            basicCtrl = 0;
            break;
        default:
            status = STATUS_UNSUPPORTED;
            break;
    }

    if (status == STATUS_SUCCESS)
    {
        if ((g_phyConfig[phy].privateData & PRIVATE_TJA1100) == 0U)
        {
            status = disableLink(phy);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }
        status = PHY_RMW(phy, EXTENDED_CTRL_ADDR, extCtrl, EXTENDED_CTRL_LOOPBACK_MODE_MASK);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        status = PHY_RMW(phy, BASIC_CTRL_ADDR, basicCtrl, BASIC_CTRL_LOOPBACK_FLAG);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if ((g_phyConfig[phy].privateData & PRIVATE_TJA1100) == 0U)
        {
            status = enableLink(phy);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SelectTestMode
 * Description   : Sets the test mode for the specified PHY device.
 *
 *END**************************************************************************/
static status_t selectTestMode(uint8_t phy, uint8_t testModeIndex)
{
    status_t status;
    uint16_t testModeField = (uint16_t) ((uint16_t) testModeIndex << EXTENDED_CTRL_TEST_MODE_SHIFT);

    if (testModeIndex > 6U)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        if (testModeIndex != 0U)
        {
            /* disable link to go to test mode */
            status = disableLink(phy);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }

        status = PHY_RMW(phy, EXTENDED_CTRL_ADDR, testModeField, EXTENDED_CTRL_TEST_MODE_MASK);

        if ((testModeIndex == 0U) && (status == STATUS_SUCCESS))
        {
            /* going back to normal, enable link */
            status = enableLink(phy);
        }
    }

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : enableInterrupts
 * Description   : Enables the interrupts for the specified PHY device.
 *
 *END**************************************************************************/
static status_t enableInterrupts(uint8_t phy)
{
    uint16_t irqs;

    irqs = (uint16_t)(IRQ_LINKUP_FLAG | IRQ_LINKDOWN_FLAG | IRQ_LPS_RECEIVED_FLAG | IRQ_WAKEUP_FLAG | IRQ_PWON_FLAG);

    return PHY_RMW(phy, IRQ_ENABLE_ADDR, irqs, irqs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterrupt
 * Description   : Handles the interrupts for the specified PHY device.
 *
 *END**************************************************************************/
static status_t handleInterrupt(uint8_t phy)
{
    status_t status;
    uint16_t generalStatus;
    uint16_t irqs;
    bool pwonHandled = false;

    status = PHY_Read(phy, GENERAL_STATUS_ADDR, &generalStatus);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((generalStatus & (GENERAL_STATUS_RSTSTATUS_FLAG | GENERAL_STATUS_ENSTATUS_FLAG)) != 0U)
    {
        /* A resetting situation has occurred. Restore configuration */
        pwonHandled = true;
        status = handleInterruptPwon(phy);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        pwonHandled = true; /* pwon sequence already done. no need to do again */
    }

    if ((generalStatus & GENERAL_STATUS_INT_STATUS_FLAG) != 0U)
    {
        status = PHY_Read(phy, IRQ_STATUS_ADDR, &irqs);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        /* Interrupt handling */
        if (((irqs & IRQ_PWON_FLAG) != 0U) && (pwonHandled == false))
        {
            status = handleInterruptPwon(phy);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }

        if ((irqs & IRQ_WAKEUP_FLAG) != 0U)
        {
            status = handleInterruptWakeUp(phy);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }

        if ((irqs & IRQ_LPS_RECEIVED_FLAG) != 0U)
        {
            handleInterruptSleep(phy);
        }

        if ((irqs & IRQ_LINKDOWN_FLAG) != 0U)
        {
            handleInterruptLinkDown(phy);
        }

        if ((irqs & IRQ_LINKUP_FLAG) != 0U)
        {
            status = handleInterruptLinkUp(phy);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : writeExtendedConfig
 * Description   : Writes the extended configuration to the device
 *
 *END**************************************************************************/
static status_t writeExtendedConfig(uint8_t phy, bool config1, bool commonConfig)
{
    status_t status = STATUS_SUCCESS;
    uint16_t reg = 0;
    uint16_t mask = 0;

    const phy_tja110x_config_t *extConfig = (phy_tja110x_config_t*) g_phyConfig[phy].extendedConfig;

    status = enableConfig(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (config1 == true)
    {
        /* configuration 1 */
        reg |= (extConfig->fwdPhyLocal == true) ? (CONFIG1_FWDPHYLOC_FLAG) : (0U);
        reg |= (uint16_t) ((extConfig->fwdPhyRemote == true) ? ((uint16_t) CONFIG1_FWDPHYREM_FLAG) : (0U));
        reg |= (uint16_t) ((extConfig->localWakeupPhy == true) ? ((uint16_t) CONFIG1_LOCWUPHY_FLAG) : (0U));
        reg |= (uint16_t) ((extConfig->remoteWakeupPhy == true) ? ((uint16_t) CONFIG1_REMWUPHY_FLAG) : (0U));
        reg |= (uint16_t) ((extConfig->reducedMiiDriveStrength == true) ? ((uint16_t) CONFIG1_MII_DRIVER_FLAG) : (0U));
        mask |= (uint16_t) (CONFIG1_FWDPHYLOC_FLAG | CONFIG1_FWDPHYREM_FLAG | CONFIG1_LOCWUPHY_FLAG | CONFIG1_REMWUPHY_FLAG | CONFIG1_MII_DRIVER_FLAG);
    }

    /* Always write to config1 register to make TC-10 compliant settings */
    reg |= (uint16_t) (CONFIG1_SLEEPACK_FLAG | CONFIG1_SLEEPCONFIRM_FLAG);

    mask |= (uint16_t) (CONFIG1_SLEEPACK_FLAG | CONFIG1_SLEEPCONFIRM_FLAG);

    status = PHY_RMW(phy, CONFIG1_ADDR, reg, mask);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    status = PHY_RMW(phy, CONFIG2_ADDR, CONFIG2_SLEEPREQUESTTO_16_8, CONFIG2_SLEEPREQUESTTO_MASK);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }


    if (commonConfig == true)
    {
        /* common configuration */
        reg  = (extConfig->reducedClkDriveStrength == true) ? ((uint16_t) COMMON_CONFIG_CLK_DRIVER_FLAG) : (0U);
        reg |= (uint16_t) ((extConfig->clkHold == true) ? ((uint16_t) COMMON_CONFIG_CLK_HOLD_FLAG) : (0U));
        reg |= (uint16_t) ((extConfig->wakeInRatiometricThreshold == true) ? ((uint16_t) COMMON_CONFIG_CONFIG_WAKE_FLAG) : (0U));
        reg |= ((uint16_t) ((uint16_t) extConfig->clkMode << COMMON_CONFIG_CLK_MODE_LSB)) & (uint16_t) COMMON_CONFIG_CLK_MODE_MASK;
        switch (extConfig->localWuTimer)
        {
            case PHY_TJA110X_LOCALWUTIME_SHORTEST:
                reg |= COMMON_CONFIG_LOC_WU_TIM_SHORTEST;
                break;
            case PHY_TJA110X_LOCALWUTIME_SHORT:
                reg |= COMMON_CONFIG_LOC_WU_TIM_SHORT;
                break;
            case PHY_TJA110X_LOCALWUTIME_LONG:
                reg |= COMMON_CONFIG_LOC_WU_TIM_LONG;
                break;
            default:
                /* longest */
                break;
        }
        mask = (uint16_t) (COMMON_CONFIG_CLK_MODE_MASK | COMMON_CONFIG_CLK_DRIVER_FLAG | COMMON_CONFIG_CLK_HOLD_FLAG | COMMON_CONFIG_CONFIG_WAKE_FLAG | COMMON_CONFIG_LOC_WU_TIM_MASK);
        status = PHY_RMW(phy, COMMON_CONFIG_ADDR, reg, mask);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    status = disableConfig(phy);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterruptPwon
 * Description   : Handles the power-on interrupt for the specified PHY device.
 *
 *END**************************************************************************/
static status_t handleInterruptPwon(uint8_t phy)
{
    status_t status;
    phy_id_t id;
    bool extendedConfigured;

    /* After power-on, make the necessary settings to the device */
    status = setManagedOperation(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = enableInterrupts(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (g_phyConfig[phy].role != PHY_ROLE_AUTO)
    {
        status = setRole(phy, g_phyConfig[phy].role);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    status = PHY_driver_tja110x.getId(phy, &id);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = ((id.oui != OUI) ? STATUS_PHY_INCOMPATIBLE_DEVICE : STATUS_SUCCESS);
    if (id.typeNo == TJA1102P0_TYPE_NO)
    {
        /* this is TJA1102_P0. Also init TJA1102_P1 */
        status = enableInterrupts(phy + 1U); /* enable interrupt on the second PHY instance within the dual PHY */
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        if (g_phyConfig[phy + 1U].role != PHY_ROLE_AUTO)
        {
            status = setRole(phy + 1U, g_phyConfig[phy + 1U].role);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
        }
    }
    /* Extended configuration handling */
    if (id.typeNo != TJA1100_TYPE_NO)
    {
        extendedConfigured = (g_phyConfig[phy].extendedConfig != NULL) ? (true) : (false);
        status = writeExtendedConfig(phy, extendedConfigured, extendedConfigured); /*TJA1101 or TJA1102_P0 */
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }
    if (id.typeNo == TJA1102P0_TYPE_NO)
    {
        /* also write specific config to TJA1102_P1 */
        extendedConfigured = (g_phyConfig[phy + 1U].extendedConfig != NULL) ? (true) : (false);
        status = writeExtendedConfig((uint8_t) (phy + 1U), extendedConfigured, false);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    status = goFromStandbyToNormalMode(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = enableLink(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }


    if(id.typeNo == TJA1102P0_TYPE_NO)
    {
        status = goFromStandbyToNormalMode(phy + 1U);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        status = enableLink(phy + 1U);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterruptLinkDown
 * Description   : Handles the link down interrupt for the specified PHY device.
 *
 *END**************************************************************************/
static void handleInterruptLinkDown(uint8_t phy)
{
    if (g_phyConfig[phy].linkDownEventCB != NULL)
    {
        g_phyConfig[phy].linkDownEventCB(phy);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterruptLinkUp
 * Description   : Handles the link up interrupt for the specified PHY device.
 *
 *END**************************************************************************/
static status_t handleInterruptLinkUp(uint8_t phy)
{
    status_t status;
    uint16_t reg;
    bool linkUp;

    /* Check if the link is still up. Do not report link up if it has gone down meanwhile */
    status = PHY_Read(phy, COMM_STATUS_ADDR, &reg);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    linkUp = ((reg & COMM_STATUS_LINKUP_FLAG) != 0U) ? (true) : (false);

    if (linkUp == true)
    {
        if (g_phyConfig[phy].linkUpEventCB != NULL)
        {
            g_phyConfig[phy].linkUpEventCB(phy);
        }
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterruptWakeUp
 * Description   : Handles the wakeup interrupt for the specified PHY device.
 *
 *END**************************************************************************/
static status_t handleInterruptWakeUp(uint8_t phy)
{
    status_t status;

    status = goFromStandbyToNormalMode(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    status = enableLink(phy);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : handleInterruptSleep
 * Description   : Handles the sleep interrupt for the specified PHY device.
 *
 *END**************************************************************************/
static void handleInterruptSleep(uint8_t phy)
{
    if (g_phyConfig[phy].sleepRequestCB != NULL)
    {
        g_phyConfig[phy].sleepRequestCB(phy);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : resume
 * Description   : Resumes the specified PHY device.
 *
 *END**************************************************************************/
static status_t resume(uint8_t phy)
{
    status_t status;

    status = PHY_driver_generic.resume(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = goFromStandbyToNormalMode(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    return enableLink(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : goFromStandbyToNormalMode
 * Description   : Requests the specified PHY device to enter normal mode.
 *
 *END**************************************************************************/
static status_t goFromStandbyToNormalMode(uint8_t phy)
{
    status_t status;
    uint16_t generalStatus;
    uint16_t phyMode;

    status = PHY_Read(phy, EXTENDED_CTRL_ADDR, &phyMode);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((phyMode & EXTENDED_CTRL_POWER_MODE_MASK) == EXTENDED_CTRL_POWER_MODE_NORMAL_REQ)
    {
        return STATUS_SUCCESS;
    }

    status = PHY_RMW(phy, EXTENDED_CTRL_ADDR, EXTENDED_CTRL_POWER_MODE_NORMAL_REQ, EXTENDED_CTRL_POWER_MODE_MASK);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    do
    {
        status = PHY_Read(phy, GENERAL_STATUS_ADDR, &generalStatus);
        /* Wait for PLL to be locked */
    } while (((generalStatus & GENERAL_STATUS_PLL_LOCKED_FLAG) == 0U) && (status == STATUS_SUCCESS));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : setManagedOperation
 * Description   : Sets the specified PHY device in managed operation.
 *
 *END**************************************************************************/
static status_t setManagedOperation(uint8_t phy)
{
    status_t status;

    status = enableConfig(phy);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = PHY_RMW(phy, COMMON_CONFIG_ADDR, 0, COMMON_CONFIG_AUTO_OP_MASK);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = disableConfig(phy);

    return status;
}
/*! @}*/

/*******************************************************************************
 * EOF
 ******************************************************************************/
