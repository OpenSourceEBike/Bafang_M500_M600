/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * @file mpu_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned int.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 */

#include <stddef.h>
#include "mpu_hw_access.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Table of base addresses for MPU instances. */
static MPU_Type * const s_mpuBase[MPU_INSTANCE_COUNT] = MPU_BASE_PTRS;

/*******************************************************************************
 * Code
 *******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Init
 * Description   : Initializes the memory protection unit by setting the access
 * configurations of all available masters, process identifier and the memory
 * location for the given regions; and activate module finally.
 * Please note that access rights for region 0 will always be configured and
 * regionCnt takes values between 1 and the maximum region count supported by
 * the hardware. E.g. In S32K144 the number of supported regions is 8.
 * The user must make sure that the clock is enabled.
 *
 * Implements    : MPU_DRV_Init_Activity
 *END**************************************************************************/
status_t MPU_DRV_Init(uint32_t instance,
                      uint8_t regionCnt,
                      const mpu_user_config_t * userConfigArr)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT((regionCnt > 0U) && (regionCnt <= MPU_RGD_COUNT));
    DEV_ASSERT(userConfigArr != NULL);

    MPU_Type * base = s_mpuBase[instance];
    status_t retStatus = STATUS_SUCCESS;
    uint8_t regionNum = 0U;

    /* De-initializes all region descriptor */
    MPU_DRV_Deinit(instance);

    for (regionNum = 0U; regionNum < regionCnt; regionNum++)
    {
        /* Sets region configuration */
        retStatus = MPU_DRV_SetRegionConfig(instance, regionNum, &userConfigArr[regionNum]);

        if (retStatus != STATUS_SUCCESS)
        {
            break;
        }
    }

    if (retStatus == STATUS_SUCCESS)
    {
        /* Enables the MPU module operation */
        MPU_Enable(base, true);
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Deinit
 * Description   : De-initializes the memory protection unit by reseting
 * all regions to default and disable module.
 *
 * Implements    : MPU_DRV_Deinit_Activity
 *END**************************************************************************/
void MPU_DRV_Deinit(uint32_t instance)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);

    MPU_Type * base = s_mpuBase[instance];
    uint8_t regionNum = 0U;
    uint8_t slaveNum = 0U;

    /* Disables the entire MPU module */
    MPU_Enable(base, false);

    for (slaveNum = 0U; slaveNum < FEATURE_MPU_SLAVE_COUNT; slaveNum++)
    {
        /* Clears slave error flag */
        MPU_ClearErrorFlag(base, slaveNum);
    }

    /* De-initializes all region descriptors */
    for (regionNum = 0U; regionNum < MPU_RGD_COUNT; regionNum++)
    {
        /* Resets the region configuration to default */
        MPU_InitRegion(base, regionNum);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionAddr
 * Description   : Sets the region start address and end address.
 *
 * Implements    : MPU_DRV_SetRegionAddr_Activity
 *END**************************************************************************/
void MPU_DRV_SetRegionAddr(uint32_t instance,
                           uint8_t regionNum,
                           uint32_t startAddr,
                           uint32_t endAddr)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(startAddr <= endAddr);

    MPU_Type * base = s_mpuBase[instance];

    /* Sets a region's start and end addresses */
    MPU_SetRegionAddr(base, regionNum, startAddr, endAddr);

    /* Re-enables the region descriptor valid bit */
    MPU_SetRegionValidCmd(base, regionNum, true);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionConfig
 * Description   : Sets the region configuration.
 * Updates the access configuration of all available masters,
 * process identifier and memory location in a given region.
 *
 * Implements    : MPU_DRV_SetRegionConfig_Activity
 *END**************************************************************************/
status_t MPU_DRV_SetRegionConfig(uint32_t instance,
                                 uint8_t regionNum,
                                 const mpu_user_config_t * userConfigPtr)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(userConfigPtr != NULL);
    DEV_ASSERT(userConfigPtr->startAddr <= userConfigPtr->endAddr);

    MPU_Type * base = s_mpuBase[instance];
    uint8_t masterIdx = 0U;
    status_t returnCode = STATUS_SUCCESS;

    /* Sets access right for masters */
    for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
    {
        if (userConfigPtr->masterAccRight[masterIdx].masterNum <= FEATURE_MPU_MAX_HIGH_MASTER_NUMBER)
        {
            MPU_SetMasterAccessRight(base,
                                     regionNum,
                                     &userConfigPtr->masterAccRight[masterIdx]);
        }
        else
        {
            returnCode = STATUS_ERROR;
            break;
        }
    }

    /* Configures address, process identifier except region 0 */
    if (regionNum > 0U)
    {
        if (returnCode == STATUS_SUCCESS)
        {
            /* Sets a region's start and end addresses */
            MPU_SetRegionAddr(base, regionNum, userConfigPtr->startAddr, userConfigPtr->endAddr);

        #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
            /* Sets process identifier */
            MPU_SetProcessIdentifier(base, regionNum, userConfigPtr->processIdentifier);

            /* Sets process identifier mask */
            MPU_SetProcessIdentifierMask(base, regionNum, userConfigPtr->processIdMask);
        #endif

            /* Enables the region descriptor valid bit */
            MPU_SetRegionValidCmd(base, regionNum, true);
        }
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetMasterAccessRights
 * Description   : Configures access permission for bus master in region.
 *
 * Implements    : MPU_DRV_SetMasterAccessRights_Activity
 *END**************************************************************************/
status_t MPU_DRV_SetMasterAccessRights(uint32_t instance,
                                       uint8_t regionNum,
                                       const mpu_master_access_right_t * accessRightsPtr)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(accessRightsPtr != NULL);

    MPU_Type * base = s_mpuBase[instance];
    status_t returnCode = STATUS_SUCCESS;

    /* Sets access rights for master */
    if (accessRightsPtr->masterNum <= FEATURE_MPU_MAX_HIGH_MASTER_NUMBER)
    {
        MPU_SetMasterAccessRight(base, regionNum, accessRightsPtr);
    }
    else
    {
        returnCode = STATUS_ERROR;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_GetDetailErrorAccessInfo
 * Description   : Checks and gets the MPU access error detail information
 * for a slave port. Clears bus error flag if an error occurs.
 *
 * Implements    : MPU_DRV_GetDetailErrorAccessInfo_Activity
 *END**************************************************************************/
bool MPU_DRV_GetDetailErrorAccessInfo(uint32_t instance,
                                      uint8_t slavePortNum,
                                      mpu_access_err_info_t * errInfoPtr)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(slavePortNum < FEATURE_MPU_SLAVE_COUNT);
    DEV_ASSERT(errInfoPtr != NULL);

    MPU_Type * base = s_mpuBase[instance];
    bool errorStatus = false;
    uint8_t slaveNum = slavePortNum;

#ifdef ERRATA_E11109
    if (slavePortNum == FEATURE_MPU_SLAVE_QUADSPI)
    {
        /* The access violation will be captured on slave port 3 */
        slaveNum = 3U;
    }
#endif

    /* Gets slaver port error status */
    errorStatus = MPU_GetErrorStatus(base, slaveNum);

    /* Check if there is access violation in the slave port */
    if (errorStatus)
    {
        /* Get the slave port detail error */
        MPU_GetErrorInfo(base, slaveNum, errInfoPtr);

        /* Clears slave port error flag */
        MPU_ClearErrorFlag(base, slaveNum);

    #ifdef ERRATA_E11109
        if (slavePortNum == FEATURE_MPU_SLAVE_QUADSPI)
        {
            /* Clears slave port error flag */
            MPU_ClearErrorFlag(base, slavePortNum);
        }
    #endif
    }

    return errorStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_GetDefaultRegionConfig
 * Description   : Gets default region configuration. Grants all access rights
 * for masters and disables PID.
 *
 * Implements    : MPU_DRV_GetDefaultRegionConfig_Activity
 *END**************************************************************************/
mpu_user_config_t MPU_DRV_GetDefaultRegionConfig(mpu_master_access_right_t * masterAccRight)
{
    DEV_ASSERT(masterAccRight != NULL);

    uint8_t masterIdx = 0U;
    uint8_t masterNum[FEATURE_MPU_MASTER_COUNT] = FEATURE_MPU_MASTER;
    mpu_user_config_t regionConfig;

    /* Allocates entire memory */
    regionConfig.startAddr = 0x0U;
    regionConfig.endAddr = 0xFFFFFFFFU;

    /* All access rights of masters are allowed */
    for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
    {
        MPU_GetDefaultMasterAccRight(masterNum[masterIdx], &masterAccRight[masterIdx]);
    }
    regionConfig.masterAccRight = masterAccRight;

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    /* Disables process identifier */
    regionConfig.processIdentifier = 0U;
    regionConfig.processIdMask = 0U;
#endif

    return regionConfig;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_EnableRegion
 * Description   : Enables/Disables region descriptor.
 * Please note that region 0 should not be disabled.
 *
 * Implements    : MPU_DRV_EnableRegion_Activity
 *END**************************************************************************/
void MPU_DRV_EnableRegion(uint32_t instance,
                          uint8_t regionNum,
                          bool enable)
{
    DEV_ASSERT(instance < MPU_INSTANCE_COUNT);
    DEV_ASSERT(regionNum < MPU_RGD_COUNT);
    DEV_ASSERT(regionNum > 0U);

    MPU_Type * base = s_mpuBase[instance];

    /* Enables/Disables region descriptor */
    MPU_SetRegionValidCmd(base, regionNum, enable);
}

/*******************************************************************************
 * EOF
 *******************************************************************************/
