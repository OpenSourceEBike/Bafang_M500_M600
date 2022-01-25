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

/*!
 * @file mpu_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a different essential type.
 * The cast is required expression assigned to a narrower or different essential type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast;
 * cannot cast from 'essentially unsigned' type to 'essentially enum<i>';
 * cannot cast from 'essentially enum<i>' to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field of a register into a enum type
 * or between two different enum types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, The cast performed between a pointer to
 * object type and a pointer to a different object type.
 * This is required for working with the common initialize function in initialize function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to pointer to other type
 * This is required for working with the common initialize function in initialize function.
 */

#include "mpu_pal.h"
#include <stddef.h>

/*******************************************************************************
 * Privates
 ******************************************************************************/
#if defined(MPU_OVER_MPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertMpuErrorConfig
 * Description   : Converts MPU error structure.
 *
 *END**************************************************************************/
static void MPU_ConvertMpuErrorConfig(const mpu_access_err_info_t * mpuErrPtr,
                                      mpu_error_info_t * errResult)
{
    /* Converts error information */
    errResult->master = mpuErrPtr->master;
    errResult->overrun = false;
    errResult->attributes = (mpu_error_attributes_t)mpuErrPtr->attributes;
    errResult->accessType = (mpu_error_access_type_t)mpuErrPtr->accessType;
    errResult->accessCtr = mpuErrPtr->accessCtr;
    errResult->addr = mpuErrPtr->addr;
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    errResult->processId = mpuErrPtr->processorIdentification;
#endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertMpuMasterStruct
 * Description   : Converts MPU region structure.
 *
 *END**************************************************************************/
static void MPU_ConvertMpuRegionConfig(const mpu_region_config_t * regionConfig,
                                       mpu_master_access_right_t * masterAccessRight,
                                       mpu_user_config_t * configResult)
{
    uint8_t masterIdx = 0U;
    mpu_master_access_permission_t masterPrivilegeRight;

    /* Allocates region address */
    configResult->startAddr = regionConfig->startAddr;
    configResult->endAddr = regionConfig->endAddr;

    /* Allocates access rights for all masters */
    for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
    {
        masterPrivilegeRight = regionConfig->masterAccRight[masterIdx];

        /* Allocates master number */
        masterAccessRight[masterIdx].masterNum = masterPrivilegeRight.masterNum;
        /* Allocates access rights */
        masterAccessRight[masterIdx].accessRight = (mpu_access_rights_t)masterPrivilegeRight.accessRight;
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        /* Allocates process identifier */
        masterAccessRight[masterIdx].processIdentifierEnable = ((regionConfig->processIdEnable
                                                                 & (1UL << masterPrivilegeRight.masterNum))
                                                                != 0U);
    #endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
    }

    /* Allocates master configuration in region */
    configResult->masterAccRight = masterAccessRight;

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    /* Allocates process identifier */
    configResult->processIdentifier = regionConfig->processIdentifier;
    configResult->processIdMask = regionConfig->processIdMask;
#endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
}
#elif defined(MPU_OVER_SMPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertSmpuErrorConfig
 * Description   : Converts SMPU error structure.
 *
 *END**************************************************************************/
static void MPU_ConvertSmpuErrorConfig(const smpu_access_err_info_t * smpuErrPtr,
                                       mpu_error_info_t * errResult)
{
    /* Converts error information */
    errResult->master = smpuErrPtr->master;
    errResult->overrun = smpuErrPtr->Overrun;
    errResult->attributes = (mpu_error_attributes_t)smpuErrPtr->attributes;
    errResult->accessType = (mpu_error_access_type_t)smpuErrPtr->accessType;
    errResult->accessCtr = smpuErrPtr->accessCtr;
    errResult->addr = smpuErrPtr->addr;
#if FEATURE_SMPU_HAS_PROCESS_IDENTIFIER
    errResult->processId = smpuErrPtr->processorIdentification;
#endif /* FEATURE_SMPU_HAS_PROCESS_IDENTIFIER */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_ConvertSmpuRegionConfig
 * Description   : Converts SMPU region structure.
 *
 *END**************************************************************************/
static void MPU_ConvertSmpuRegionConfig(const mpu_region_config_t * regionConfig,
                                        smpu_master_access_right_t * masterAccessRight,
                                        smpu_user_config_t * configResult)
{
    uint8_t masterIdx = 0U;
    mpu_master_access_permission_t masterPrivilegeRight;

    /* Allocates region address */
    configResult->startAddr = regionConfig->startAddr;
    configResult->endAddr = regionConfig->endAddr;

    /* Allocates access rights for masters */
    for (masterIdx = 0U; masterIdx < FEATURE_SMPU_MASTER_COUNT; masterIdx++)
    {
        masterPrivilegeRight = regionConfig->masterAccRight[masterIdx];

        /* Allocates master number */
        masterAccessRight[masterIdx].masterNum = masterPrivilegeRight.masterNum;
        /* Allocates access rights */
        masterAccessRight[masterIdx].accessRight = (smpu_access_rights_t)masterPrivilegeRight.accessRight;
    }

    /* Allocates master configuration in region */
    configResult->masterAccRight = masterAccessRight;

#if FEATURE_SMPU_HAS_PROCESS_IDENTIFIER
    /* Allocates process identifier */
    configResult->processIdEnable = (regionConfig->processIdEnable != 0U);
    configResult->processIdentifier = regionConfig->processIdentifier;
    configResult->processIdMask = regionConfig->processIdMask;
#endif /* FEATURE_SMPU_HAS_PROCESS_IDENTIFIER */

    if (regionConfig->extension != NULL)
    {
    #if (FEATURE_SMPU_HAS_SPECIFIC_ACCESS_RIGHT_COUNT != 0U)
        configResult->specAccessEnable = ((mpu_extension_smpu_region_t *)regionConfig->extension)->specAccessEnable;
        configResult->specAccessSet = (smpu_specific_access_rights_t *)((mpu_extension_smpu_region_t *)regionConfig->extension)->specAccessSet;
    #endif
        configResult->cacheInhibitEnable = ((mpu_extension_smpu_region_t *)regionConfig->extension)->cacheInhibitEnable;
        configResult->lockConfig = (smpu_lock_t)((mpu_extension_smpu_region_t *)regionConfig->extension)->lockConfig;
    }
    else
    {
    #if (FEATURE_SMPU_HAS_SPECIFIC_ACCESS_RIGHT_COUNT != 0U)
        configResult->specAccessEnable = false;
        configResult->specAccessSet = NULL;
    #endif
        configResult->cacheInhibitEnable = true;
        configResult->lockConfig = SMPU_UNLOCK;
    }
}
#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_Init
 * Description   : Initializes memory protection unit by allocating regions
 *                 and granting access rights for masters.
 *
 * Implements    : MPU_Init_Activity
 *END**************************************************************************/
status_t MPU_Init(const mpu_instance_t * const instance,
                  uint8_t regionCnt,
                  const mpu_region_config_t * configPtr)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(configPtr != NULL);
    DEV_ASSERT(regionCnt > 0U);

    status_t status = STATUS_UNSUPPORTED;
    uint8_t regionIdx = 0U;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        DEV_ASSERT(regionCnt <= MPU_RGD_COUNT);

        mpu_user_config_t mpuConfig[MPU_RGD_COUNT];
        mpu_master_access_right_t mpuMasterConfig[MPU_RGD_COUNT * FEATURE_MPU_MASTER_COUNT];

        /* Allocates all region configuration */
        for (regionIdx = 0U; regionIdx < regionCnt; regionIdx++)
        {
            MPU_ConvertMpuRegionConfig(&configPtr[regionIdx],
                                       &mpuMasterConfig[regionIdx * FEATURE_MPU_MASTER_COUNT],
                                       &mpuConfig[regionIdx]);
        }
        /* Initializes MPU module */
        status = MPU_DRV_Init(instance->instIdx, regionCnt, mpuConfig);
    }
#endif /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        DEV_ASSERT(regionCnt <= SMPU_RGD_COUNT);

        smpu_user_config_t smpuConfig[SMPU_RGD_COUNT];
        smpu_master_access_right_t smpuMasterConfig[SMPU_RGD_COUNT * FEATURE_SMPU_MASTER_COUNT];

        /* Allocates all region configuration */
        for (regionIdx = 0U; regionIdx < regionCnt; regionIdx++)
        {
            MPU_ConvertSmpuRegionConfig(&configPtr[regionIdx],
                                        &smpuMasterConfig[regionIdx * FEATURE_SMPU_MASTER_COUNT],
                                        &smpuConfig[regionIdx]);
        }
        /* Initializes SMPU module */
        status = SMPU_DRV_Init(instance->instIdx, regionCnt, smpuConfig);
    }
#endif /* defined(MPU_OVER_SMPU) */

    (void)regionIdx;
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_Deinit
 * Description   : De-initializes memory protection unit by reseting all regions
 *                 and masters to default and disable module.
 *
 * Implements    : MPU_Deinit_Activity
 *END**************************************************************************/
status_t MPU_Deinit(const mpu_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        /* De-initializes MPU module */
        MPU_DRV_Deinit(instance->instIdx);
        status = STATUS_SUCCESS;
    }
#endif /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        /* De-initializes SMPU module */
        status = SMPU_DRV_Deinit(instance->instIdx);
    }
#endif /* defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetDefautRegionConfig
 * Description   : Gets default region configuration. Grants all access rights for masters;
 * disable PID and cache inhibit; unlock region descriptor.
 * Please note that the length of master array should be defined by number of masters supported by hardware.
 *
 * Implements    : MPU_GetDefautRegionConfig_Activity
 *END**************************************************************************/
status_t MPU_GetDefautRegionConfig(const mpu_instance_t * const instance,
                                   mpu_master_access_permission_t * masterAccRight,
                                   mpu_region_config_t * regionConfig)
{
    DEV_ASSERT(masterAccRight != NULL);
    DEV_ASSERT(regionConfig != NULL);

    status_t status = STATUS_UNSUPPORTED;
    uint8_t masterIdx = 0U;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        uint8_t mpuMasterNum[FEATURE_MPU_MASTER_COUNT] = FEATURE_MPU_MASTER;

        /* Gets default access rights for all masters */
        for (masterIdx = 0U; masterIdx < FEATURE_MPU_MASTER_COUNT; masterIdx++)
        {
            masterAccRight[masterIdx].masterNum = mpuMasterNum[masterIdx];
            if (mpuMasterNum[masterIdx] <= FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
            {
                masterAccRight[masterIdx].accessRight = MPU_SUPERVISOR_RWX_USER_RWX;
            }
            else
            {
                masterAccRight[masterIdx].accessRight = MPU_RW;
            }
        }

        status = STATUS_SUCCESS;
    }
#endif /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        uint8_t smpuMasterNum[FEATURE_SMPU_MASTER_COUNT] = FEATURE_SMPU_MASTER;

        /* Gets default access rights for all masters */
        for (masterIdx = 0U; masterIdx < FEATURE_SMPU_MASTER_COUNT; masterIdx++)
        {
            masterAccRight[masterIdx].masterNum = smpuMasterNum[masterIdx];
            masterAccRight[masterIdx].accessRight = MPU_RW_OR_SET_3;
        }

        status = STATUS_SUCCESS;
    }
#endif /* defined(MPU_OVER_SMPU) */

    /* Gets default region configuration */
    regionConfig->startAddr = 0U;
    regionConfig->endAddr = 0xFFFFFFFFU;
    regionConfig->masterAccRight = masterAccRight;
    regionConfig->processIdEnable = 0U;
    regionConfig->processIdentifier = 0U;
    regionConfig->processIdMask = 0U;
    regionConfig->extension = NULL;

    (void)masterIdx;
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_UpdateRegion
 * Description   : Updates region configuration.
 *
 * Implements    : MPU_UpdateRegion_Activity
 *END**************************************************************************/
status_t MPU_UpdateRegion(const mpu_instance_t * const instance,
                          uint8_t regionNum,
                          const mpu_region_config_t * configPtr)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(configPtr != NULL);

    status_t status = STATUS_UNSUPPORTED;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        mpu_user_config_t mpuConfig;
        mpu_master_access_right_t mpuMasterConfig[FEATURE_MPU_MASTER_COUNT];

        /* Allocates region configuration */
        MPU_ConvertMpuRegionConfig(configPtr, mpuMasterConfig, &mpuConfig);
        /* Updates region configuration */
        status = MPU_DRV_SetRegionConfig(instance->instIdx, regionNum, &mpuConfig);
    }
#endif /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        smpu_user_config_t smpuConfig;
        smpu_master_access_right_t smpuMasterConfig[FEATURE_SMPU_MASTER_COUNT];

        /* Allocates region configuration */
        MPU_ConvertSmpuRegionConfig(configPtr, smpuMasterConfig, &smpuConfig);
        /* Updates region configuration */
        status = SMPU_DRV_SetRegionConfig(instance->instIdx, regionNum, &smpuConfig);
    }
#endif /* defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_EnableRegion
 * Description   : Enables or disables an exist region configuration.
 *
 * Implements    : MPU_EnableRegion_Activity
 *END**************************************************************************/
status_t MPU_EnableRegion(const mpu_instance_t * const instance,
                          uint8_t regionNum,
                          bool enable)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        /* Enables region */
        MPU_DRV_EnableRegion(instance->instIdx, regionNum, enable);
        status = STATUS_SUCCESS;
    }

#endif /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        /* Enables region */
        status = SMPU_DRV_EnableRegion(instance->instIdx, regionNum, enable);
    }
#endif /* defined(MPU_OVER_SMPU) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetError
 * Description   : Checks and gets the access error detail information
 * then clear error flag if the error caused by a master.
 *
 * Implements    : MPU_GetError_Activity
 *END**************************************************************************/
bool MPU_GetError(const mpu_instance_t * const instance,
                  uint8_t channel,
                  mpu_error_info_t * errPtr)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(errPtr != NULL);

    bool status = false;

#if defined(MPU_OVER_MPU)
    if (instance->instType == MPU_INST_TYPE_MPU)
    {
        mpu_access_err_info_t mpuErrPtr;

        /* Gets detail error information */
        status = MPU_DRV_GetDetailErrorAccessInfo(instance->instIdx, channel, &mpuErrPtr);

        if (status)
        {
            MPU_ConvertMpuErrorConfig(&mpuErrPtr, errPtr);
        }
    }
#endif  /* defined(MPU_OVER_MPU) */

#if defined(MPU_OVER_SMPU)
    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        smpu_access_err_info_t smpuErrPtr;

        /* Gets detail error information */
        status = SMPU_DRV_GetDetailErrorInfo(instance->instIdx, channel, &smpuErrPtr);

        if (status)
        {
            MPU_ConvertSmpuErrorConfig(&smpuErrPtr, errPtr);
        }
    }
#endif /* defined(MPU_OVER_SMPU) */

    return status;
}

#if defined(MPU_OVER_SMPU)
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_UpdateRegionLock
 * Description   : Updates lock configuration on a region.
 *
 * Implements    : MPU_UpdateRegionLock_Activity
 *END**************************************************************************/
status_t MPU_UpdateRegionLock(const mpu_instance_t * const instance,
                              uint8_t regionNum,
                              mpu_lock_t lockConfig)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_UNSUPPORTED;

    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        /* Sets region lock configuration */
        status = SMPU_DRV_SetRegionLockConfig(instance->instIdx, regionNum, (smpu_lock_t)lockConfig);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetRegionLockInfo
 * Description   : Reports lock status on a region.
 *
 * Implements    : MPU_GetRegionLockInfo_Activity
 *END**************************************************************************/
status_t MPU_GetRegionLockInfo(const mpu_instance_t * const instance,
                               uint8_t regionNum,
                               mpu_region_lock_t * regionLock)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(regionLock != NULL);

    status_t status = STATUS_UNSUPPORTED;

    if (instance->instType == MPU_INST_TYPE_SMPU)
    {
        smpu_region_lock_t smpuRegionLock;

        /* Gets region lock status */
        smpuRegionLock = SMPU_DRV_GetRegionLockInfo(instance->instIdx, regionNum);

        /* Converts to MPU region lock status structure */
        regionLock->regionNum = smpuRegionLock.regionNum;
    #if FEATURE_SMPU_HAS_OWNER_LOCK
        regionLock->masterOwner = smpuRegionLock.masterOwner;
    #endif /* FEATURE_SMPU_HAS_OWNER_LOCK */
        regionLock->lockConfig = (mpu_lock_t)smpuRegionLock.lockConfig;

        status = STATUS_SUCCESS;
    }

    return status;

}
#endif /* defined(MPU_OVER_SMPU) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
