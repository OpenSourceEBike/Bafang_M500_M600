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
 * @file mpu_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type.
 * This is required by the conversion of a bit-field of a register into a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially unsigned' type to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field of a register into a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite
 * expression (different essential type categories).
 * This is required by the conversion of a bit-field of a register into enum type.
 */

#include "mpu_hw_access.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/
/* Reset value of all master access right in region 0 */
#define REGION_0_ACCESS_RIGHT_RESET_VALUE (0x0061F7DFU)
/* Reset value of end address */
#define END_ADDRESS_RESET_VALUE           (0x1FU)
/* Default value of privilege right */
#define DEFAULT_PRIVILEGE_RIGHT           (MPU_SUPERVISOR_RWX_USER_RWX)
/* Default value of access right */
#define DEFAULT_ACCESS_RIGHT              (MPU_RW)
/* Mask and Shift of access right */
#define MPU_USER_MASK               (0x07U)
#define MPU_USER_SHIFT              (0U)
#define MPU_SUPERVISOR_MASK         (0x18U)
#define MPU_SUPERVISOR_SHIFT        (3U)
#define MPU_W_MASK                  (0x20U)
#define MPU_W_SHIFT                 (5U)
#define MPU_R_MASK                  (0x40U)
#define MPU_R_SHIFT                 (6U)

/*******************************************************************************
 * Code
 *******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_SetRegionAddr
 * Description   : Sets region start and end address.
 * Please note that using this function will clear the valid bit of the region,
 * and a further validation might be needed.
 *
 *END**************************************************************************/
void MPU_SetRegionAddr(MPU_Type * const base,
                       uint8_t regionNum,
                       uint32_t startAddr,
                       uint32_t endAddr)
{
    /* Write start address to RGD_WORD0 */
    base->RGD[regionNum].WORD0 = startAddr;

    /* Write end address to RGD_WORD1 */
    base->RGD[regionNum].WORD1 = endAddr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_SetMasterAccessRight
 * Description   : Sets access permission for master in region descriptor.
 *
 *END**************************************************************************/
void MPU_SetMasterAccessRight(MPU_Type * const base,
                              uint8_t regionNum,
                              const mpu_master_access_right_t * masterAccRight)
{
    uint32_t accRight;
    uint32_t accMask;
    uint32_t accShift;
    uint32_t temp;
    uint32_t masterNum = masterAccRight->masterNum;

    if (masterNum <= FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
    {
        /* Prepare Supervisor Mode Access Control and User Mode Access Control value */
        accRight = MPU_RGDAAC_M0UM(((uint32_t)masterAccRight->accessRight & MPU_USER_MASK) >> MPU_USER_SHIFT)
                 | MPU_RGDAAC_M0SM(((uint32_t)masterAccRight->accessRight & MPU_SUPERVISOR_MASK) >> MPU_SUPERVISOR_SHIFT);
        accMask  = MPU_RGDAAC_M0UM_MASK | MPU_RGDAAC_M0SM_MASK;
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        /* Enables/Disables process identifier */
        if (masterNum < FEATURE_MPU_NUM_MASTER_HAS_PROCESS_IDENTIFIER)
        {
            accRight |= MPU_RGDAAC_M0PE(masterAccRight->processIdentifierEnable ? 1UL : 0UL);
        }
        accMask |= MPU_RGDAAC_M0PE_MASK;
    #endif
        /* Shift FEATURE_MPU_LOW_MASTER_CONTROL_WIDTH-bit field defining
         * separate privilege rights depend on bus master number
         */
        accShift = (masterNum * FEATURE_MPU_LOW_MASTER_CONTROL_WIDTH);
    }
    else
    {
        /* Prepare Read Enable and Write Enable value */
        accRight = MPU_RGDAAC_M4WE(((uint32_t)masterAccRight->accessRight & MPU_W_MASK) >> MPU_W_SHIFT)
                 | MPU_RGDAAC_M4RE(((uint32_t)masterAccRight->accessRight & MPU_R_MASK) >> MPU_R_SHIFT);
        accMask  = MPU_RGDAAC_M4WE_MASK | MPU_RGDAAC_M4RE_MASK;
        /* Low master number is numbered from 0 so master number count will be FEATURE_MPU_MAX_LOW_MASTER_NUMBER added to 1 */
        accShift = FEATURE_MPU_HIGH_MASTER_CONTROL_WIDTH * (masterNum - (FEATURE_MPU_MAX_LOW_MASTER_NUMBER + 1U));
    }

    accRight = accRight << accShift;
    accMask  = accMask << accShift;

    /* Set access rights */
    temp = base->RGDAAC[regionNum];
    temp = (temp & ~accMask) | accRight;
    base->RGDAAC[regionNum] = temp;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_InitRegion
 * Description   : Resets the region descriptor to default.
 *
 *END**************************************************************************/
void MPU_InitRegion(MPU_Type * const base,
                    uint8_t regionNum)
{

    if (regionNum == 0U)
    {
        /* Sets default access right for region 0 */
        base->RGDAAC[0U] = REGION_0_ACCESS_RIGHT_RESET_VALUE;
    }
    else
    {
        base->RGD[regionNum].WORD3 = 0U;
        base->RGD[regionNum].WORD2 = 0U;
        base->RGD[regionNum].WORD1 = END_ADDRESS_RESET_VALUE;
        base->RGD[regionNum].WORD0 = 0U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetErrorInfo
 * Description   : Gets MPU detail error access info.
 *
 *END**************************************************************************/
void MPU_GetErrorInfo(const MPU_Type * const base,
                      uint8_t slaveNum,
                      mpu_access_err_info_t * errInfoPtr)
{
    uint32_t temp;

    /* Read the Error Detail Register for the slave port */
    temp = base->EAR_EDR[slaveNum].EDR;

    /* Read Error Access Control Detail of slave port (EACD) in EDRn register:
     * If EDRn contains a captured error and EACD is cleared, an access did not hit
     * in any region descriptor.
     * If only a single EACD bit is set, the protection error was caused by a single
     * non-overlapping region descriptor.
     * If two or more EACD bits are set, the protection error was caused by an overlapping
     * set of region descriptors.
     */
    errInfoPtr->accessCtr = (uint16_t)((temp & MPU_EDR_EACD_MASK) >> MPU_EDR_EACD_SHIFT);

    /* Report Error Master Number to user */
    errInfoPtr->master = (uint8_t)((temp & MPU_EDR_EMN_MASK) >> MPU_EDR_EMN_SHIFT);

    /* Report Error Attributes to user */
    errInfoPtr->attributes = (mpu_err_attributes_t)((temp & MPU_EDR_EATTR_MASK) >> MPU_EDR_EATTR_SHIFT);

    /* Report Error Read/Write to user */
    errInfoPtr->accessType = (mpu_err_access_type_t)((temp & MPU_EDR_ERW_MASK) >> MPU_EDR_ERW_SHIFT);

    /* Report Error Address to user */
    errInfoPtr->addr = base->EAR_EDR[slaveNum].EAR;

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    /* Report Error Process Identification to user */
    errInfoPtr->processorIdentification = (uint8_t)((temp & MPU_EDR_EPID_MASK) >> MPU_EDR_EPID_SHIFT);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_GetDefaultMasterAccRight
 * Description   : Reports the default master access rights.
 *
 *END**************************************************************************/
void MPU_GetDefaultMasterAccRight(uint8_t masterNum,
                                  mpu_master_access_right_t * masterAccRight)
{
    masterAccRight->masterNum = masterNum;

    if (masterNum <= FEATURE_MPU_MAX_LOW_MASTER_NUMBER)
    {
        masterAccRight->accessRight = DEFAULT_PRIVILEGE_RIGHT;
    }
    else
    {
        masterAccRight->accessRight = DEFAULT_ACCESS_RIGHT;
    }

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    masterAccRight->processIdentifierEnable = false;
#endif
}

/*******************************************************************************
 * EOF
 *******************************************************************************/
