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
 * @file phy.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long value, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long value,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer variable could be declared as
 * pointing to const
 * This is a pointer to a driver structure containing function pointers that might
 * be updated by the device-specific driver.
 *
 */

#include <stdint.h>

#include "device_registers.h"
#include "status.h"

#include "phy.h"
#include "phy_hw_access.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PHY_MDIO_TIMEOUT    (2U)

#define MMD_ACCESS_CONTROL_ADDR   13U
#define MMD_ACCESS_REG_DATA_ADDR  14U

#define MMD_ACCESS_CONTROL_DATA     0x4000U  /* Data, no post increment */

#define ALIGN32_SHIFT  4U           /* Shift needed to obtain a byte address */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Clause 22 PHY write access */
static status_t writeCl22(uint8_t phy, uint8_t registerAddr, uint16_t value);
/*! @brief Clause 22 PHY read access */
static status_t readCl22(uint8_t phy, uint8_t registerAddr, uint16_t *value);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointer to an array of pointers towards the driver structures */
static phy_driver_t** s_phyDrivers;
/*! @brief Pointer to an array of PHY configuration structures */
phy_config_t* g_phyConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_FrameworkInit
 * Description   : Initializes the PHY driver.
 * Implements    : PHY_FrameworkInit_Activity
 *
 *END**************************************************************************/
status_t PHY_FrameworkInit(phy_config_t* phyConfig, phy_driver_t** phyDrivers)
{
    /* Store pointers to configuration and driver structures */
    s_phyDrivers = phyDrivers;
    g_phyConfig = phyConfig;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Init
 * Description   : Initializes the specified PHY device.
 * Implements    : PHY_Init_Activity
 *
 *END**************************************************************************/
status_t PHY_Init(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->init(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Reset
 * Description   : Resets the specified PHY device.
 * Implements    : PHY_Reset_Activity
 *
 *END**************************************************************************/
status_t PHY_Reset(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->reset(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_MainFunction
 * Description   : Provides polling support, by handling specific events.
 * Implements    : PHY_MainFunction_Activity
 *
 *END**************************************************************************/
status_t PHY_MainFunction(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->mainFunction(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetId
 * Description   : Gets the ID of the specified PHY device.
 * Implements    : PHY_GetId_Activity
 *
 *END**************************************************************************/
status_t PHY_GetId(uint8_t phy, phy_id_t* id)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getId(phy, id);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetState
 * Description   : Gets the current state of the specified PHY device
 * Implements    : PHY_GetState_Activity
 *
 *END**************************************************************************/
status_t PHY_GetState(uint8_t phy, phy_state_t* state)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getState(phy, state);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetLinkStatus
 * Description   : Gets the link status for the specified PHY device.
 * Implements    : PHY_GetLinkStatus_Activity
 *
 *END**************************************************************************/
status_t PHY_GetLinkStatus(uint8_t phy, bool* linkUp)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getLinkStatus(phy, linkUp);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetLinkSpeed
 * Description   : Gets the link speed for the specified PHY device.
 * Implements    : PHY_GetLinkSpeed_Activity
 *
 *END**************************************************************************/
status_t PHY_GetLinkSpeed(uint8_t phy, phy_speed_t* speed)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getLinkSpeed(phy, speed);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_GetDelay
 * Description   : Gets the propagation delays for the specified PHY device.
 * Implements    : PHY_GetDelay_Activity
 *
 *END**************************************************************************/
status_t PHY_GetDelay(uint8_t phy, phy_delay_t *delay)
{
    return s_phyDrivers[g_phyConfig[phy].type]->getDelay(phy, delay);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Suspend
 * Description   : Suspends the specified PHY device.
 * Implements    : PHY_Suspend_Activity
 *
 *END**************************************************************************/
status_t PHY_Suspend(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->suspend(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Resume
 * Description   : Resumes the specified PHY device.
 * Implements    : PHY_Resume_Activity
 *
 *END**************************************************************************/
status_t PHY_Resume(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->resume(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SleepAbortReject
 * Description   : Aborts an initiated sleep request or rejects an received sleep request.
 * Implements    : PHY_SleepAbortReject_Activity
 *
 *END**************************************************************************/
status_t PHY_SleepAbortReject(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->sleepAbortReject(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Sleep
 * Description   : Sends a sleep request to the specified PHY device.
 * Implements    : PHY_Sleep_Activity
 *
 *END**************************************************************************/
status_t PHY_Sleep(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->sleep(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Wakeup
 * Description   : Wakes up the specified PHY device.
 * Implements    : PHY_Wakeup_Activity
 *
 *END**************************************************************************/
status_t PHY_Wakeup(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->wakeup(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SetRole
 * Description   : Sets the role of the specified PHY device.
 * Implements    : PHY_SetRole_Activity
 *
 *END**************************************************************************/
status_t PHY_SetRole(uint8_t phy, phy_role_t role)
{
    return s_phyDrivers[g_phyConfig[phy].type]->setRole(phy, role);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SetLoopback
 * Description   : Sets the loopback mode for the specified PHY device.
 * Implements    : PHY_SetLoopback_Activity
 *
 *END**************************************************************************/
status_t PHY_SetLoopback(uint8_t phy, phy_loopback_t loopbackMode)
{
    return s_phyDrivers[g_phyConfig[phy].type]->setLoopback(phy, loopbackMode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_SelectTestMode
 * Description   : Sets the test mode for the specified PHY device.
 * Implements    : PHY_SelectTestMode_Activity
 *
 *END**************************************************************************/
status_t PHY_SelectTestMode(uint8_t phy, uint8_t testModeIndex)
{
    return s_phyDrivers[g_phyConfig[phy].type]->selectTestMode(phy, testModeIndex);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_EnableInterrupts
 * Description   : Enables interrupts for the specified PHY device.
 * Implements    : PHY_EnableInterrupts_Activity
 *
 *END**************************************************************************/
status_t PHY_EnableInterrupts(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->enableInterrupts(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_HandleInterrupt
 * Description   : Handles interrupts for the specified PHY device.
 * Implements    : PHY_HandleInterrupt_Activity
 *
 *END**************************************************************************/
status_t PHY_HandleInterrupt(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->handleInterrupt(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_EnableAutoneg
 * Description   : Enables auto-negotiation for the specified PHY device.
 * Implements    : PHY_EnableAutoneg_Activity
 *
 *END**************************************************************************/
status_t PHY_EnableAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->enableAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_DisableAutoneg
 * Description   : Disables auto-negotiation for the specified PHY device.
 * Implements    : PHY_DisableAutoneg_Activity
 *
 *END**************************************************************************/
status_t PHY_DisableAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->disableAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_RestartAutoneg
 * Description   : Restarts auto-negotiation for the specified PHY device.
 * Implements    : PHY_RestartAutoneg_Activity
 *
 *END**************************************************************************/
status_t PHY_RestartAutoneg(uint8_t phy)
{
    return s_phyDrivers[g_phyConfig[phy].type]->restartAutoneg(phy);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Write
 * Description   : Writes a register of the specified PHY device.
 * Implements    : PHY_Write_Activity
 *
 *END**************************************************************************/
status_t PHY_Write(uint8_t phy, uint8_t registerAddr, uint16_t value)
{
    status_t status = STATUS_SUCCESS;

    if (s_phyDrivers[g_phyConfig[phy].type]->write != NULL)
    {  /* driver has a custom write function */
        status = s_phyDrivers[g_phyConfig[phy].type]->write(phy, registerAddr, value);
    }
    else
    {
        status = writeCl22(phy, registerAddr, value);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_Read
 * Description   : Reads a register of the specified PHY device.
 * Implements    : PHY_Read_Activity
 *
 *END**************************************************************************/
status_t PHY_Read(uint8_t phy, uint8_t registerAddr, uint16_t* value)
{
    status_t status = STATUS_SUCCESS;

    if (s_phyDrivers[g_phyConfig[phy].type]->read != NULL)
    {  /* driver has a custom read function */
        status = s_phyDrivers[g_phyConfig[phy].type]->read(phy, registerAddr, value);
    }
    else
    {
        status = readCl22(phy, registerAddr, value);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_RMW
 * Description   : Reads and modifies a register of the specified PHY device.
 * Implements    : PHY_RMW_Activity
 *
 *END**************************************************************************/
status_t PHY_RMW(uint8_t phy, uint8_t registerAddr, uint16_t value, uint16_t mask)
{
    status_t status;
    uint16_t reg;

    if (s_phyDrivers[g_phyConfig[phy].type]->rmw != NULL)
    {  /* driver has a custom read modify write function */
        status = s_phyDrivers[g_phyConfig[phy].type]->rmw(phy, registerAddr, value, mask);
    }
    else
    {
        /* Read */
        status = PHY_Read(phy, registerAddr, &reg);
        if (status == STATUS_SUCCESS)
        {
            /* Modify */
            reg &= (uint16_t)~mask;           /* Clear masked bits */
            reg |= (uint16_t)(mask & value);  /* Set masked bits to value */

            /* Write */
            status = PHY_Write(phy, registerAddr, reg);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_WriteMMD
 * Description   : Writes a register of the specified MMD in a PHY device.
 * Implements    : PHY_WriteMMD_Activity
 *
 *END**************************************************************************/
status_t PHY_WriteMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value)
{
    status_t status;

    if ((s_phyDrivers[g_phyConfig[phy].type]->writeMMD != NULL) && (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL))
    {  /* driver has a custom MMD write function for internal access */
        status = s_phyDrivers[g_phyConfig[phy].type]->writeMMD(phy, mmd, registerAddr, value);
    }
    else
    {
        if ((g_phyConfig[phy].location == PHY_LOCATION_EXTERNAL) && (g_phyConfig[phy].clause45 == true))
        {
            status = PHY_HW_WriteCl45(g_phyConfig[phy].enetInstance, (uint8_t) g_phyConfig[phy].addr, mmd, registerAddr, value, PHY_MDIO_TIMEOUT);
        }
        else
        {
            status = writeCl22(phy, MMD_ACCESS_CONTROL_ADDR, mmd);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = writeCl22(phy, MMD_ACCESS_REG_DATA_ADDR, registerAddr);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = writeCl22(phy, MMD_ACCESS_CONTROL_ADDR, MMD_ACCESS_CONTROL_DATA | mmd);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = writeCl22(phy, MMD_ACCESS_REG_DATA_ADDR, value);
        }

    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_ReadMMD
 * Description   : Reads a register of the specified MMD in a PHY device.
 * Implements    : PHY_ReadMMD_Activity
 *
 *END**************************************************************************/
status_t PHY_ReadMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t *value)
{
    status_t status;

    if ((s_phyDrivers[g_phyConfig[phy].type]->readMMD != NULL) && (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL))
    {  /* driver has a custom MMD read function for internal access */
        status = s_phyDrivers[g_phyConfig[phy].type]->readMMD(phy, mmd, registerAddr, value);
    }
    else
    {
        if ((g_phyConfig[phy].location == PHY_LOCATION_EXTERNAL) && (g_phyConfig[phy].clause45 == true))
        {
            status = PHY_HW_ReadCl45(g_phyConfig[phy].enetInstance, (uint8_t) g_phyConfig[phy].addr, mmd, registerAddr, value, PHY_MDIO_TIMEOUT);
        }
        else
        {
            status = writeCl22(phy, MMD_ACCESS_CONTROL_ADDR, mmd);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = writeCl22(phy, MMD_ACCESS_REG_DATA_ADDR, registerAddr);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = writeCl22(phy, MMD_ACCESS_CONTROL_ADDR, MMD_ACCESS_CONTROL_DATA | mmd);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }
            status = readCl22(phy, MMD_ACCESS_REG_DATA_ADDR, value);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PHY_RMWMMD
 * Description   : Reads and modifies a register of the specified MMD in a PHY device.
 * Implements    : PHY_RMWMMD_Activity
 *
 *END**************************************************************************/
status_t PHY_RMWMMD(uint8_t phy, uint8_t mmd, uint16_t registerAddr, uint16_t value, uint16_t mask)
{
    status_t status;
    uint16_t reg;

    if ((s_phyDrivers[g_phyConfig[phy].type]->rmwMMD != NULL) && (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL))
    {  /* driver has a custom MMD read-modify-write function  for internal access */
        status = s_phyDrivers[g_phyConfig[phy].type]->rmwMMD(phy, mmd, registerAddr, value, mask);
    }
    else
    {
        status = PHY_ReadMMD(phy, mmd, registerAddr, &reg);
        if (status == STATUS_SUCCESS)
        {
            /* Modify */
            reg &= (uint16_t)~mask;           /* Clear masked bits */
            reg |= (uint16_t)(mask & value);  /* Set masked bits to value */

            /* Write */
            status = PHY_WriteMMD(phy, mmd, registerAddr, reg);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : writeCl22
 * Description   : Writes a register of the specified PHY device.
 *
 *END**************************************************************************/
static status_t writeCl22(uint8_t phy, uint8_t registerAddr, uint16_t value)
{
    status_t status = STATUS_SUCCESS;

    if (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL)
    {
        ((volatile uint16_t *)g_phyConfig[phy].addr)[(uint32_t) ((uint32_t) registerAddr << ALIGN32_SHIFT)] = value;
    }
    else
    {
        if (PHY_HW_WriteCl22(g_phyConfig[phy].enetInstance, (uint8_t)g_phyConfig[phy].addr, registerAddr, value, PHY_MDIO_TIMEOUT) != STATUS_SUCCESS)
        {
            status = STATUS_PHY_ACCESS_FAILED;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : readCl22
 * Description   : Reads a register of the specified PHY device.
 *
 *END**************************************************************************/
static status_t readCl22(uint8_t phy, uint8_t registerAddr, uint16_t* value)
{
    status_t status = STATUS_SUCCESS;
    if (g_phyConfig[phy].location == PHY_LOCATION_INTERNAL)
    {
        *value = ((volatile uint16_t *)g_phyConfig[phy].addr)[(uint32_t) ((uint32_t) registerAddr << ALIGN32_SHIFT)];
    }
    else
    {
        if (PHY_HW_ReadCl22(g_phyConfig[phy].enetInstance, (uint8_t)g_phyConfig[phy].addr, registerAddr, value, PHY_MDIO_TIMEOUT) != STATUS_SUCCESS)
        {
            status = STATUS_PHY_ACCESS_FAILED;
        }
    }
    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
