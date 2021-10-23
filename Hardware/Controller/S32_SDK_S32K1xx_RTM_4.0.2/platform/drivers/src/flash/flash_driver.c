/*
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
 * @file flash_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
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
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.1, Conversion between a pointer
 * to function and another type.
 * The cast is required to define a callback function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 18.4, Pointer arithmetic other than array
 * indexing used.
 * This is required to increment the source address.
 */

#include "flash_driver.h"
#include "interrupt_manager.h"

START_FUNCTION_DECLARATION_RAMSECTION
static status_t FLASH_DRV_CommandSequence(const flash_ssd_config_t * pSSDConfig)
END_FUNCTION_DECLARATION_RAMSECTION
static status_t FLASH_DRV_ProgramCheckExecute(const flash_ssd_config_t * pSSDConfig,
                                              uint32_t dest,
                                              uint32_t size,
                                              const uint8_t * pExpectedData,
                                              uint32_t * pFailAddr,
                                              uint8_t marginLevel);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetDEPartitionCode
 * Description   : Gets DFlash size from FlexNVM Partition Code.
 *
 *END**************************************************************************/
static void FLASH_DRV_GetDEPartitionCode(flash_ssd_config_t * const pSSDConfig,
                                         uint8_t DEPartitionCode)
{
    /* Select D-Flash size */
    switch (DEPartitionCode)
    {
        case 0x00U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0000;
            break;
        case 0x01U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0001;
            break;
        case 0x02U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0010;
            break;
        case 0x03U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0011;
            break;
        case 0x04U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0100;
            break;
        case 0x05U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0101;
            break;
        case 0x06U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0110;
            break;
        case 0x07U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_0111;
            break;
        case 0x08U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1000;
            break;
        case 0x09U:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1001;
            break;
        case 0x0AU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1010;
            break;
        case 0x0BU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1011;
            break;
        case 0x0CU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1100;
            break;
        case 0x0DU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1101;
            break;
        case 0x0EU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1110;
            break;
        case 0x0FU:
            pSSDConfig->DFlashSize = (uint32_t)FEATURE_FLS_DF_SIZE_1111;
            break;
        default:
            /* Undefined value */
            break;
    }
}

#if (FEATURE_FLS_IS_FTFC == 0U)
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetEEEDataSetSize
 * Description   : Gets EEPROM size from EEPROM Data Set Size.
 *
 *END**************************************************************************/
static void FLASH_DRV_GetEEEDataSetSize(flash_ssd_config_t * const pSSDConfig,
                                        uint8_t EEEDataSetSize)
{
    /* Select EEPROM size */
    switch (EEEDataSetSize)
    {
        case 0x00U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0000;
            break;
        case 0x01U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0001;
            break;
        case 0x02U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0010;
            break;
        case 0x03U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0011;
            break;
        case 0x04U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0100;
            break;
        case 0x05U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0101;
            break;
        case 0x06U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0110;
            break;
        case 0x07U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_0111;
            break;
        case 0x08U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1000;
            break;
        case 0x09U:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1001;
            break;
        case 0x0AU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1010;
            break;
        case 0x0BU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1011;
            break;
        case 0x0CU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1100;
            break;
        case 0x0DU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1101;
            break;
        case 0x0EU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1110;
            break;
        case 0x0FU:
            pSSDConfig->EEESize = (uint32_t)FEATURE_FLS_EE_SIZE_1111;
            break;
        default:
            /* Undefined value */
            break;
    }
}
#endif /* if (FEATURE_FLS_IS_FTFC == 0U) */

/*FUNCTION*******************************************************************************
 *
 * Function Name : FLASH_DRV_Init
 * Description   : Initializes Flash module by reporting the memory configuration via
 * SSD configuration structure.
 *
 * Implements    : FLASH_DRV_Init_Activity
 *END***********************************************************************************/
status_t FLASH_DRV_Init(const flash_user_config_t * const pUserConf,
                        flash_ssd_config_t * const pSSDConfig)
{
    DEV_ASSERT(pUserConf != NULL);
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;
#if FEATURE_FLS_HAS_FLEX_NVM
    uint8_t DEPartitionCode;    /* store D/E-Flash Partition Code */
#endif

    pSSDConfig->PFlashBase = pUserConf->PFlashBase;
    pSSDConfig->PFlashSize = pUserConf->PFlashSize;
    pSSDConfig->DFlashBase = pUserConf->DFlashBase;
    pSSDConfig->EERAMBase = pUserConf->EERAMBase;
    pSSDConfig->CallBack = pUserConf->CallBack;

#if FEATURE_FLS_HAS_FLEX_NVM
    /* Temporary solution for FTFC and S32K144 CSEc part */
    /* Get DEPART from Flash Configuration Register 1 */
    DEPartitionCode = (uint8_t)((SIM->FCFG1 & SIM_FCFG1_DEPART_MASK) >> SIM_FCFG1_DEPART_SHIFT);
    /* Get data flash size */
    FLASH_DRV_GetDEPartitionCode(pSSDConfig, DEPartitionCode);
    if (pSSDConfig->DFlashSize < FEATURE_FLS_DF_BLOCK_SIZE)
    {
        pSSDConfig->EEESize = FEATURE_FLS_FLEX_RAM_SIZE;
    }
    else
    {
        pSSDConfig->EEESize = 0U;
    }
#else /* FEATURE_FLS_HAS_FLEX_NVM == 0 */
    /* If size of D/E-Flash = 0 */
    pSSDConfig->DFlashSize = 0U;
    pSSDConfig->EEESize = 0U;
#endif /* End of FEATURE_FLS_HAS_FLEX_NVM */

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CommandSequence
 * Description   : Perform command write sequence on Flash.
 * It is internal function, called by driver APIs only.
 *
 *END**************************************************************************/
START_FUNCTION_DEFINITION_RAMSECTION
static status_t FLASH_DRV_CommandSequence(const flash_ssd_config_t * pSSDConfig)
{
    status_t ret = STATUS_SUCCESS;    /* Return code variable */

    /* Clear CCIF to launch command */
    FTFx_FSTAT |= FTFx_FSTAT_CCIF_MASK;

    while (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        /* Wait till CCIF bit is set
         * Serve callback function as often as possible
         */
        if (NULL_CALLBACK != pSSDConfig->CallBack)
        {
            /* Temporarily disable compiler's check for ROM access call from within a ram function.
             * The use of a function pointer type makes this check irrelevant.
             * Nevertheless, it is imperative that the user-provided callback be defined in RAMSECTION */
            DISABLE_CHECK_RAMSECTION_FUNCTION_CALL
            (pSSDConfig->CallBack)();
            ENABLE_CHECK_RAMSECTION_FUNCTION_CALL
        }
    }

    if ( FTFx_FSTAT_ERROR_BITS != 0U)
    {
        ret = STATUS_ERROR;
    }

    return ret;
}
END_FUNCTION_DEFINITION_RAMSECTION

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetPFlashProtection
 * Description   : Retrieves the current P-Flash protection status.
 * Considering the time consumption for getting protection is very low and
 * even can be ignored. It is not necessary to utilize the Callback function to
 * support the time-critical events.
 *
 * Implements    : FLASH_DRV_GetPFlashProtection_Activity
 *END**************************************************************************/
void FLASH_DRV_GetPFlashProtection(uint32_t * protectStatus)
{
    DEV_ASSERT(protectStatus != NULL);
    uint32_t reg0, reg1, reg2, reg3;

    reg0 = FTFx_FPROT0;
    reg1 = FTFx_FPROT1;
    reg2 = FTFx_FPROT2;
    reg3 = FTFx_FPROT3;

    *protectStatus = (uint32_t)((reg0 << 24U) | (reg1 << 16U) | (reg2 << 8U) | reg3);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SetPFlashProtection
 * Description   : Sets the P-Flash protection to the intended protection status.
 * Setting P-Flash protection status is subject to a protection transition
 * restriction. If there is a setting violation, it returns an error code
 * and the current protection status will not be changed.
 *
 * Implements    : FLASH_DRV_SetPFlashProtection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SetPFlashProtection(uint32_t protectStatus)
{
    status_t ret = STATUS_SUCCESS;
    uint8_t reg0, reg1, reg2, reg3;
    bool flag0, flag1, flag2, flag3;

    /* Get register */
    reg0 = GET_BIT_24_31(protectStatus);
    reg1 = GET_BIT_16_23(protectStatus);
    reg2 = GET_BIT_8_15(protectStatus);
    reg3 = GET_BIT_0_7(protectStatus);

    /* Write to register */
    FTFx_FPROT0 = reg0;
    FTFx_FPROT1 = reg1;
    FTFx_FPROT2 = reg2;
    FTFx_FPROT3 = reg3;

    /* Compare changes */
    flag0 = (FTFx_FPROT0 != reg0);
    flag1 = (FTFx_FPROT1 != reg1);
    flag2 = (FTFx_FPROT2 != reg2);
    flag3 = (FTFx_FPROT3 != reg3);

    /* Read the value of FPPROT registers */
    if (flag0 || flag1 || flag2 || flag3)
    {
        ret = STATUS_ERROR;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetSecurityState
 * Description   : Retrieves the current Flash security status, including
 * the security enabling state and the back door key enabling state.
 *
 * Implements    : FLASH_DRV_GetSecurityState_Activity
 *END**************************************************************************/
void FLASH_DRV_GetSecurityState(uint8_t * securityState)
{
    DEV_ASSERT(securityState != NULL);
    /* Store data read from flash register */
    uint8_t regValue;

    /* Get flash security register value */
    regValue = FTFx_FSEC;

    /* Check the status of the flash security bits in the security register */
    if (FLASH_SECURITY_STATE_UNSECURED == (regValue & FTFx_FSEC_SEC_MASK))
    {
        /* Flash in unsecured state */
        *securityState = FLASH_NOT_SECURE;
    }
    else
    {
        /* Flash in secured state
         * Check for backdoor key security enable bit
         */
        if (0x80U == (regValue & FTFx_FSEC_KEYEN_MASK))
        {
            /* Backdoor key security enabled */
            *securityState = FLASH_SECURE_BACKDOOR_ENABLED;
        }
        else
        {
            /* Backdoor key security disabled */
            *securityState = FLASH_SECURE_BACKDOOR_DISABLED;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SecurityBypass
 * Description   : Un-secures the device by comparing the user's provided back
 * door key with the ones in the Flash Configuration Field. If they are
 * matched, the security is released. Otherwise, an error code is returned.
 *
 * Implements    : FLASH_DRV_SecurityBypass_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SecurityBypass(const flash_ssd_config_t * pSSDConfig,
                                  const uint8_t * keyBuffer)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(keyBuffer != NULL);
    status_t ret;    /* Return code variable */
    uint32_t temp;   /* Temporary variable */
    uint8_t i;

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_SECURITY_BY_PASS;
        for (i = 0U; i < 8U; i++)
        {
            temp = FTFx_BASE + i + 0x08U;
            *(uint8_t *)temp = keyBuffer[i];
        }

        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseAllBlock
 * Description   : Erases all Flash memory, initializes the FlexRAM, verifies
 * all memory contents, and then releases the MCU security.
 *
 * Implements    : FLASH_DRV_EraseAllBlock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EraseAllBlock(const flash_ssd_config_t * pSSDConfig)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_ERASE_ALL_BLOCK;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_VerifyAllBlock
 * Description   : Checks to see if the P-Flash and/or D-Flash, EEPROM
 * backup area, and D-Flash IFR have been erased to the specified read
 * margin level, if applicable, and releases security if the readout passes.
 *
 * Implements    : FLASH_DRV_VerifyAllBlock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_VerifyAllBlock(const flash_ssd_config_t * pSSDConfig,
                                  uint8_t marginLevel)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_VERIFY_ALL_BLOCK;
        FTFx_FCCOB1 = marginLevel;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseSector
 * Description   : Erases one or more sectors in P-Flash or D-Flash memory.
 * This API always returns STATUS_SUCCESS if size provided by the user is
 * zero regardless of the input validation.
 *
 * Implements    : FLASH_DRV_EraseSector_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EraseSector(const flash_ssd_config_t * pSSDConfig,
                               uint32_t dest,
                               uint32_t size)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;       /* Return code variable */
    uint32_t sectorSize;                 /* Size of one sector   */
    uint32_t temp;                       /* Temporary variable   */
    uint32_t tempSize = size;            /* Temporary of size variation */

#if FEATURE_FLS_HAS_FLEX_NVM
    temp = pSSDConfig->DFlashBase;
    if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
    {
        DEV_ASSERT((dest % FEATURE_FLS_DF_SECTOR_CMD_ADDRESS_ALIGMENT) == 0U);
        dest += 0x800000U - temp;
        sectorSize = (uint32_t)FEATURE_FLS_DF_BLOCK_SECTOR_SIZE;
    }
    else
#endif
    {
        temp = pSSDConfig->PFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
        {
            DEV_ASSERT((dest % FEATURE_FLS_PF_SECTOR_CMD_ADDRESS_ALIGMENT) == 0U);
            dest -= temp;
            sectorSize = (uint32_t)FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;
        }
        else
        {
            ret = STATUS_ERROR;
            tempSize = 0U;
            sectorSize = 0U;
        }
    }

    /* Check if the size is sector alignment or not */
    if ((tempSize & (sectorSize - 1U)) != 0U)
    {
        /* Return an error code */
        ret = STATUS_ERROR;
    }

    while ((tempSize > 0U) && (STATUS_SUCCESS == ret))
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_ERASE_SECTOR;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);

            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);

            /* Update size and destination address */
            tempSize -= sectorSize;
            dest += sectorSize;
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_VerifySection
 * Description   : Checks if a section of the P-Flash or the D-Flash memory
 * is erased to the specified read margin level.
 *
 * Implements    : FLASH_DRV_VerifySection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_VerifySection(const flash_ssd_config_t * pSSDConfig,
                                 uint32_t dest,
                                 uint16_t number,
                                 uint8_t marginLevel)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;

    /* Check if the destination is aligned or not */
#if FEATURE_FLS_HAS_FLEX_NVM
    temp = pSSDConfig->DFlashBase;
    if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
    {
        DEV_ASSERT((dest % FEATURE_FLS_DF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
        dest += 0x800000U - temp;
    }
    else
#endif
    {
        temp = pSSDConfig->PFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
        {
            DEV_ASSERT((dest % FEATURE_FLS_PF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
            dest -= temp;
        }
        else
        {
            ret = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == ret)
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_VERIFY_SECTION;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);
            FTFx_FCCOB4 = GET_BIT_8_15(number);
            FTFx_FCCOB5 = GET_BIT_0_7(number);
            FTFx_FCCOB6 = marginLevel;

            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseSuspend
 * Description   : Suspend a current operation of Flash erase sector command.
 * This function must be located in RAM memory or different Flash blocks which are
 * targeted for writing to avoid the RWW error.
 *
 * Implements    : FLASH_DRV_EraseSuspend_Activity
 *END**************************************************************************/
void FLASH_DRV_EraseSuspend(void)
{
    uint32_t count = SUSPEND_WAIT_CNT;    /* Counter variable */

    if ((FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0U)
    {
        FTFx_FCNFG |= FTFx_FCNFG_ERSSUSP_MASK;

        /* Wait till CCIF bit is set */
        while (((FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0U) && (count > 0U))
        {
            count--;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseResume
 * Description   : Resume a previous suspended operation of Flash erase sector command
 * This function must be located in RAM memory or different Flash blocks which are targeted
 * for writing to avoid RWW error.
 *
 * Implements    : FLASH_DRV_EraseResume_Activity
 *END**************************************************************************/
void FLASH_DRV_EraseResume(void)
{
    uint16_t i = 0U;    /* Counter variable */

    /* Check ERSSUSP bit of the flash configuration register */
    if ((FTFx_FCNFG & FTFx_FCNFG_ERSSUSP_MASK) == FTFx_FCNFG_ERSSUSP_MASK)
    {
        /* Clear CCIF to launch command */
        FTFx_FSTAT |= FTFx_FSTAT_CCIF_MASK;
        /* Wait for completion of this command */
        while ((0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK)) && (i < RESUME_WAIT_CNT))
        {
            i++;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ReadOnce
 * Description   : Read out a reserved 64 byte field located in the P-Flash IFR via given number
 * of record. See the corresponding reference manual to get the correct value of this number.
 *
 * Implements    : FLASH_DRV_ReadOnce_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ReadOnce(const flash_ssd_config_t * pSSDConfig,
                            uint8_t recordIndex,
                            uint8_t * pDataArray)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pDataArray != NULL);
    status_t ret;              /* Return code variable */
    uint32_t temp;             /* Temporary variable */
    uint8_t i;

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_READ_ONCE;
        FTFx_FCCOB1 = recordIndex;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);

        /* Checking for the success of command execution */
        if (STATUS_SUCCESS == ret)
        {
            /* Read the data from the FCCOB registers into the pDataArray */
            for (i = 0U; i < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; i++)
            {
                temp = FTFx_BASE + i + 0x08U;
                pDataArray[i] = *(uint8_t *)temp;
            }
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ProgramOnce
 * Description   : Program to a reserved 64 byte field located in the
 * P-Flash IFR via given number of record. See the corresponding reference manual
 * to get correct value of this number.
 *
 * Implements    : FLASH_DRV_ProgramOnce_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ProgramOnce(const flash_ssd_config_t * pSSDConfig,
                               uint8_t recordIndex,
                               const uint8_t * pDataArray)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pDataArray != NULL);
    status_t ret;    /* Return code variable */
    uint32_t temp;   /* Temporary variable */
    uint8_t i;

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_PROGRAM_ONCE;
        FTFx_FCCOB1 = recordIndex;

        for (i = 0U; i < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; i++)
        {
            temp = FTFx_BASE + i + 0x08U;
            *(uint8_t *)temp = pDataArray[i];
        }

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}

#if FEATURE_FLS_HAS_READ_RESOURCE_CMD
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ReadResource
 * Description   : Read data from special purpose memory in Flash memory module
 * including P-Flash IFR, swap IFR, D-Flash IFR space and version ID.
 *
 *END**************************************************************************/
status_t FLASH_DRV_ReadResource(const flash_ssd_config_t * pSSDConfig,
                                uint32_t dest,
                                uint8_t * pDataArray,
                                uint8_t resourceSelectCode)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pDataArray != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;
    uint8_t i;

    /* Check if the destination is aligned or not */
#if FEATURE_FLS_HAS_FLEX_NVM
    temp = pSSDConfig->DFlashBase;
    if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
    {
        DEV_ASSERT(dest % FEATURE_FLS_DF_RESOURCE_CMD_ADDRESS_ALIGMENT == 0);
        dest += 0x800000U - temp;
    }
    else
#endif
    {
        DEV_ASSERT(dest % FEATURE_FLS_PF_RESOURCE_CMD_ADDRESS_ALIGMENT == 0);
        temp = pSSDConfig->PFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
        {
            dest -= temp;
        }
        else
        {
            ret = STATUS_ERROR;
        }
    }

    if (ret == STATUS_SUCCESS)
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;
            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_READ_RESOURCE;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);
            FTFx_RSRC_CODE_REG = resourceSelectCode;
            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);

            if (STATUS_SUCCESS == ret)
            {
                /* Read the data from the FCCOB registers into the pDataArray */
                for (i = 0U; i < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; i++)
                {
                    temp = FTFx_BASE + i + 0x08U;
                    pDataArray[i] = *(uint8_t *)temp;
                }
            }
        }
    }

    return ret;
}
#endif /* if FEATURE_FLS_HAS_READ_RESOURCE_CMD */

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_Program
 * Description   : Program 4 consecutive bytes (for program long word command)
 * and 8 consecutive bytes (for program phrase command) on P-Flash or D-Flash block.
 * This API always returns STATUS_SUCCESS if size provided by user is
 * zero regardless of the input validation.
 *
 * Implements    : FLASH_DRV_Program_Activity
 *END**************************************************************************/
status_t FLASH_DRV_Program(const flash_ssd_config_t * pSSDConfig,
                           uint32_t dest,
                           uint32_t size,
                           const uint8_t * pData)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pData != NULL);
    DEV_ASSERT((dest % FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE) == 0U);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;
    uint8_t i;

    if ((size & (FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE - 1U)) != 0U)
    {
        ret = STATUS_ERROR;
    }
    else
    {
    #if FEATURE_FLS_HAS_FLEX_NVM
        temp = pSSDConfig->DFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
        {
            dest += 0x800000U - temp;
        }
        else
    #endif
        {
            temp = pSSDConfig->PFlashBase;
            if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
            {
                dest -= temp;
            }
            else
            {
                ret = STATUS_ERROR;
            }
        }

        while ((size > 0U) && (STATUS_SUCCESS == ret))
        {
            /* Check CCIF to verify the previous command is completed */
            if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
            {
                ret = STATUS_BUSY;
            }
            else
            {
                /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
                CLEAR_FTFx_FSTAT_ERROR_BITS;

                /* Passing parameter to the command */
                #if (FEATURE_FLS_DF_BLOCK_WRITE_UNIT_SIZE == FTFx_PHRASE_SIZE)
                FTFx_FCCOB0 = FTFx_PROGRAM_PHRASE;
                #else
                FTFx_FCCOB0 = FTFx_PROGRAM_LONGWORD;
                #endif
                FTFx_FCCOB1 = GET_BIT_16_23(dest);
                FTFx_FCCOB2 = GET_BIT_8_15(dest);
                FTFx_FCCOB3 = GET_BIT_0_7(dest);

                for (i = 0U; i < FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE; i++)
                {
                    temp = FTFx_BASE + i + 0x08U;
                    *(uint8_t *)(temp) = pData[i];
                }

                /* Calling flash command sequence function to execute the command */
                ret = FLASH_DRV_CommandSequence(pSSDConfig);

                /* Update destination address for next iteration */
                dest += FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
                /* Update size for next iteration */
                size -= FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
                /* Increment the source address by 1 */
                pData += FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE;
            }
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ProgramCheck
 * Description   : Tests a previously programmed P-Flash or D-Flash long word
 * to see if it reads correctly at the specified margin level. This
 * API always returns STATUS_SUCCESS if size provided by user is zero
 * regardless of the input validation
 *
 * Implements    : FLASH_DRV_ProgramCheck_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ProgramCheck(const flash_ssd_config_t * pSSDConfig,
                                uint32_t dest,
                                uint32_t size,
                                const uint8_t * pExpectedData,
                                uint32_t * pFailAddr,
                                uint8_t marginLevel)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pExpectedData != NULL);
    DEV_ASSERT(pFailAddr != NULL);
    DEV_ASSERT((dest % FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT) == 0U);
    status_t ret = STATUS_SUCCESS;  /* Return code variable */
    uint32_t failAddr = 0U;

    if ((size & (FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT - 1U)) != 0U)
    {
        ret = STATUS_ERROR;
    }
    else
    {
        ret = FLASH_DRV_ProgramCheckExecute(pSSDConfig,dest,size,pExpectedData,&failAddr,marginLevel);
        *pFailAddr = failAddr;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_CheckSum
 * Description   : Performs 32 bit sum of each byte data over a specified Flash
 * memory range without carry which provides rapid method for checking data integrity.
 * The callback time period of this API is determined via FLASH_CALLBACK_CS macro in the
 * flash_driver.h which is used as a counter value for the CallBack() function calling in
 * this API. This value can be changed as per the user requirement. User can change this value
 * to obtain the maximum permissible callback time period.
 * This API always returns STATUS_SUCCESS if size provided by user is zero regardless of the input
 * validation.
 *
 * Implements    : FLASH_DRV_CheckSum_Activity
 *END**************************************************************************/
status_t FLASH_DRV_CheckSum(const flash_ssd_config_t * pSSDConfig,
                            uint32_t dest,
                            uint32_t size,
                            uint32_t * pSum)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pSum != NULL);
    status_t ret = STATUS_SUCCESS;      /* Return code variable           */
    uint32_t counter = 0U;              /* Counter for callback operation */
    uint32_t data;                      /* Data read from Flash address   */
    uint32_t endAddress;                /* PFlash end address             */
    uint32_t tempSize = size;           /* Temporary of size variation    */

    /* Calculating Flash end address */
    endAddress = dest + tempSize;

    /* Check for valid range of the target addresses */
    if ((dest < pSSDConfig->PFlashBase) || (endAddress > (pSSDConfig->PFlashBase + pSSDConfig->PFlashSize)))
    {
        #if FEATURE_FLS_HAS_FLEX_NVM
        if ((dest < pSSDConfig->DFlashBase) || (endAddress > (pSSDConfig->DFlashBase + pSSDConfig->DFlashSize)))
        {
        #endif
            ret = STATUS_ERROR;
            tempSize = 0U;
        #if FEATURE_FLS_HAS_FLEX_NVM
        }
        #endif /* End of if FEATURE_FLS_HAS_FLEX_NVM */
    }

    *pSum = 0U;
    /* Doing sum operation */
    while (tempSize > 0U)
    {
        data = *(uint8_t *)(dest);
        *pSum += data;
        dest += 1U;
        tempSize -= 1U;
        ++counter;

        /* Check if need to serve callback function */
        if (counter >= FLASH_CALLBACK_CS)
        {
            /* Serve callback function if counter reaches limitation */
            if (NULL_CALLBACK != pSSDConfig->CallBack)
            {
                pSSDConfig->CallBack();
            }

            /* Reset counter */
            counter = 0U;
        }
    }

    return ret;
}

#if FEATURE_FLS_HAS_PROGRAM_SECTION_CMD
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ProgramSection
 * Description   : Program the data found in the Section Program Buffer
 * to previously erased locations in the Flash memory. Data is preloaded into
 * the Section Program Buffer by writing to the acceleration Ram and FlexRam
 * while it is set to function as a RAM. The Section Program Buffer is limited
 * to the value of FlexRam divides by a ratio. Refer to the associate reference
 * manual to get correct value of this ratio.
 * For derivatives including swap feature, the swap indicator address is encountered
 * during FlashProgramSection, it is bypassed without setting FPVIOL but the content
 * are not be programmed. In addition, the content of source data used to program to
 * swap indicator will be re-initialized to 0xFF after completion of this command.
 *
 * Implements    : FLASH_DRV_ProgramSection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_ProgramSection(const flash_ssd_config_t * pSSDConfig,
                                  uint32_t dest,
                                  uint16_t number)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;

    /* Check RAMRDY bit of the flash configuration register */
    if (0U == (FTFx_FCNFG & FTFx_FCNFG_RAMRDY_MASK))
    {
        /* Return an error code */
        ret = STATUS_UNSUPPORTED;
    }
    else
    {
    #if FEATURE_FLS_HAS_FLEX_NVM
        temp = pSSDConfig->DFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
        {
            DEV_ASSERT((dest % FEATURE_FLS_DF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
            dest += 0x800000U - temp;
        }
        else
    #endif
        {
            temp = pSSDConfig->PFlashBase;
            if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
            {
                DEV_ASSERT((dest % FEATURE_FLS_PF_SECTION_CMD_ADDRESS_ALIGMENT) == 0U);
                dest -= temp;
            }
            else
            {
                ret = STATUS_ERROR;
            }
        }

        if (ret == STATUS_SUCCESS)
        {
            /* Check CCIF to verify the previous command is completed */
            if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
            {
                ret = STATUS_BUSY;
            }
            else
            {
                /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register.
                 * Write 1 to clear
                 */
                CLEAR_FTFx_FSTAT_ERROR_BITS;
                /* Passing parameter to command */
                FTFx_FCCOB0 = FTFx_PROGRAM_SECTION;
                FTFx_FCCOB1 = GET_BIT_16_23(dest);
                FTFx_FCCOB2 = GET_BIT_8_15(dest);
                FTFx_FCCOB3 = GET_BIT_0_7(dest);
                FTFx_FCCOB4 = GET_BIT_8_15(number);
                FTFx_FCCOB5 = GET_BIT_0_7(number);

                /* Calling flash command sequence function to execute the command */
                ret = FLASH_DRV_CommandSequence(pSSDConfig);
            }
        }
    }

    return ret;
}
#endif /* if FEATURE_FLS_HAS_PROGRAM_SECTION_CMD */

#if FEATURE_FLS_HAS_ERASE_BLOCK_CMD
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseBlock
 * Description   : Erases all addresses in an individual P-Flash or D-Flash block.
 * For the derivatives including multiply logical P-Flash or D-Flash blocks,
 * this API erases a single block in a single call.
 *
 * Implements    : FLASH_DRV_EraseBlock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EraseBlock(const flash_ssd_config_t * pSSDConfig,
                              uint32_t dest)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;       /* Return code variable */
    uint32_t temp;                       /* Temporary variable   */

    /* Check if the destination is aligned or not */
#if FEATURE_FLS_HAS_FLEX_NVM
    temp = pSSDConfig->DFlashBase;
    if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
    {
        DEV_ASSERT((dest % FEATURE_FLS_DF_BLOCK_CMD_ADDRESS_ALIGMENT) == 0U);
        dest += 0x800000U - temp;
    }
    else
#endif
    {
        temp = pSSDConfig->PFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
        {
            DEV_ASSERT((dest % FEATURE_FLS_PF_BLOCK_CMD_ADDRESS_ALIGMENT) == 0U);
            dest -= temp;
        }
        else
        {
            ret = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == ret)
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_ERASE_BLOCK;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);

            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);
        }
    }

    return ret;
}
#endif /* if FEATURE_FLS_HAS_ERASE_BLOCK_CMD */

#if FEATURE_FLS_HAS_READ_1S_BLOCK_CMD
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_VerifyBlock
 * Description   : Checks to see if an entire P-Flash or D-Flash block has been
 * erased to the specified margin level. For the derivatives including multiply
 * logical P-Flash or D-Flash blocks, this API erases a single block in a single call.
 *
 * Implements    : FLASH_DRV_VerifyBlock_Activity
 *END**************************************************************************/
status_t FLASH_DRV_VerifyBlock(const flash_ssd_config_t * pSSDConfig,
                               uint32_t dest,
                               uint8_t marginLevel)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;

    /* Check if the destination is aligned or not */
#if FEATURE_FLS_HAS_FLEX_NVM
    temp = pSSDConfig->DFlashBase;
    if ((dest >= temp) && (dest < (temp + pSSDConfig->DFlashSize)))
    {
        DEV_ASSERT((dest % FEATURE_FLS_DF_BLOCK_CMD_ADDRESS_ALIGMENT) == 0U);
        dest += 0x800000U - temp;
    }
    else
#endif
    {
        temp = pSSDConfig->PFlashBase;
        if ((dest >= temp) && (dest < (temp + pSSDConfig->PFlashSize)))
        {
            DEV_ASSERT((dest % FEATURE_FLS_PF_BLOCK_CMD_ADDRESS_ALIGMENT) == 0U);
            dest -= temp;
        }
        else
        {
            ret = STATUS_ERROR;
        }
    }

    if (STATUS_SUCCESS == ret)
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_VERIFY_BLOCK;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);
            FTFx_FCCOB4 = marginLevel;

            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);
        }
    }

    return ret;
}
#endif /* if FEATURE_FLS_HAS_READ_1S_BLOCK_CMD */

#if FEATURE_FLS_HAS_FLEX_NVM
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetEERAMProtection
 * Description   : Retrieves which EEPROM sections of FlexRAM are protected
 * against program and erase operations. Considering the time consumption
 * for getting protection is very low and even can be ignored, it is not necessary
 * to utilize the Callback function to support the time-critical events
 *
 * Implements    : FLASH_DRV_GetEERAMProtection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_GetEERAMProtection(uint8_t * protectStatus)
{
    DEV_ASSERT(protectStatus != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */

    /* Check if EERAM is set for EEPROM */
    if ((FTFx_FCNFG & FTFx_FCNFG_EEERDY_MASK) == FTFx_FCNFG_EEERDY_MASK)
    {
        *protectStatus = FTFx_FEPROT;
    }
    else
    {
        ret = STATUS_UNSUPPORTED;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SetEERAMProtection
 * Description   : Sets protection to the intended protection status for EEPROM us
 * area of FlexRam. This is subject to a protection transition restriction.
 * If there is a setting violation, it returns failed information and
 * the current protection status will not be changed.
 *
 * Implements    : FLASH_DRV_SetEERAMProtection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SetEERAMProtection(uint8_t protectStatus)
{
    status_t ret = STATUS_SUCCESS;    /* Return code variable */

    /* Check if FlexRAM is set for EEPROM */
    if (0U == (FTFx_FCNFG & FTFx_FCNFG_EEERDY_MASK))
    {
        /* FlexRAM is not set for EEPROM */
        ret = STATUS_UNSUPPORTED;
    }
    else
    {
        FTFx_FEPROT = protectStatus;
        if (protectStatus != FTFx_FEPROT)
        {
            ret = STATUS_ERROR;
        }
        else
        {
            /* Do nothing */
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SetFlexRamFunction
 * Description   : This function is used to change the function of the FlexRAM. When not partitioned for
 * emulated EEPROM, the FlexRAM is typically used as traditional RAM.
 * Otherwise, the FlexRam is typically used to store EEPROM data, the writing to EEPROM is normal write or
 * quick write.
 * In addition, this function may be used to get EEPROM status or complete interrupted EEPROM quick write process.
 * For example, after partitioning to have EEPROM backup, FlexRAM is used for EEPROM
 * use accordingly and if want to change FlexRAM to traditional RAM for FlashProgramSection() use, call this API
 * with the function control code is 0xFFU.
 *
 * Implements    : FLASH_DRV_SetFlexRamFunction_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SetFlexRamFunction(const flash_ssd_config_t * pSSDConfig,
                                      flash_flexRam_function_control_code_t flexRamFuncCode,
                                      uint16_t byteOfQuickWrite,
                                      flash_eeprom_status_t * const pEEPROMStatus)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_SET_EERAM;
        FTFx_FCCOB1 = (uint8_t)flexRamFuncCode;

        if (flexRamFuncCode == EEE_QUICK_WRITE)
        {
            FTFx_FCCOB4 = (uint8_t)(byteOfQuickWrite >> 0x8U);
            FTFx_FCCOB5 = (uint8_t)(byteOfQuickWrite & 0xFFU);
        }

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);

        if ((flexRamFuncCode == EEE_STATUS_QUERY) && (ret == STATUS_SUCCESS))
        {
            if (pEEPROMStatus == NULL)
            {
                ret = STATUS_ERROR;
            }
            else
            {
                pEEPROMStatus->brownOutCode = FTFx_FCCOB5;
                pEEPROMStatus->sectorEraseCount = (uint16_t)((uint16_t)FTFx_FCCOB8 << 8U);
                pEEPROMStatus->sectorEraseCount |= (uint16_t)FTFx_FCCOB9;
                pEEPROMStatus->numOfRecordReqMaintain = (uint16_t)((uint16_t)FTFx_FCCOB6 << 8U);
                pEEPROMStatus->numOfRecordReqMaintain |= (uint16_t)FTFx_FCCOB7;
            }
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_WaitEEWriteToFinish
 * Description   : Write to EEPROM with data was aligned and wait until operation finish.
 *
 *END**************************************************************************/
static status_t FLASH_DRV_WaitEEWriteToFinish(const flash_ssd_config_t * pSSDConfig,
                                              uint32_t dest,
                                              const uint8_t * pData,
                                              uint8_t step)
{
    DEV_ASSERT(pData != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint32_t temp;                    /* Temporary variable   */

    #if (FEATURE_FLS_IS_FTFM == 1U)
    temp = *(volatile uint32_t *)dest;

    switch (step)
    {
        case 0x4U:
            temp =  (uint32_t)(pData[3]) << 24U;
            temp |= (uint32_t)(pData[2]) << 16U;
            temp |= (uint32_t)(pData[1]) << 8U;
            temp |= (uint32_t)(pData[0]);
            break;
        case 0x1U:
            temp = (temp & 0xFFFFFF00U) | (uint32_t)(pData[0]);
            break;
        case 0x2U:
            temp = (temp & 0xFFFF0000U) | ((uint32_t)(pData[1]) << 8U);
            temp |= (uint32_t)(pData[0]);
            break;
        case 0x3U:
            temp = (temp & 0xFF000000U) | ((uint32_t)(pData[2]) << 16U);
            temp |= (uint32_t)(pData[1]) << 8U;
            temp |= (uint32_t)(pData[0]);
            break;
        default:
            /* Undefined value */
            break;
    }

    *(volatile uint32_t *)dest = temp;
    #else
    if (0x04U == step)
    {
        temp =  (uint32_t)(pData[3]) << 24U;
        temp |= (uint32_t)(pData[2]) << 16U;
        temp |= (uint32_t)(pData[1]) << 8U;
        temp |= (uint32_t)(pData[0]);
        *(volatile uint32_t *)dest = temp;
    }
    if (0x01U == step)
    {
        *(uint8_t *)dest = *pData;
    }
    if (0x02U == step)
    {
        temp = (uint32_t)(pData[1]) << 8U;
        temp |= (uint32_t)(pData[0]);
        *(volatile uint16_t *)dest = (uint16_t)temp;
    }
    #endif

    while (0U == (FTFx_FCNFG & FTFx_FCNFG_EEERDY_MASK))
    {
        /* Wait till EEERDY bit is set
         * Serve callback function as often as possible
         */
        if (NULL_CALLBACK != pSSDConfig->CallBack)
        {
            (pSSDConfig->CallBack)();
        }

        /* Check for protection violation error */
        if ( FTFx_FSTAT_ERROR_BITS != 0U)
        {
            break;
        }

    }

    /* Check for protection violation error */
    if ( FTFx_FSTAT_ERROR_BITS != 0U)
    {
        ret = STATUS_ERROR;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EEEWrite
 * Description   : Write data to FlexRAM section which is partitioned
 * as EEPROM use for EEPROM operation. After data has been written to EEPROM
 * use section of FlexRAM, the EEPROM file system will create new data record
 * in EEPROM back-up area of FlexNVM in round-robin fashion.
 * There is no alignment constraint for destination and size parameters
 * provided by user. However, according to user's input provided, this
 * API will set priority to write to FlexRAM with following rules:
 * 32-bit writing is invoked if destination is 32 bit aligned and size
 * is not less than 32 bits.
 * 16-bit writing is invoked if destination is 16 bit aligned and size
 * is not less than 16 bits.
 * 8-bit writing is invoked if destination is 8 bit aligned and size
 * is not less than 8 bits.
 * S32k14xW platform supports only FlexRAM aligned writes of 4 bytes (32-bits) at a time
 * When EEPROM was set to EEPROM quick writes mode, the dest address
 * should be aligned 4 bytes,the size should be divided by 4.
 * Implements    : FLASH_DRV_EEEWrite_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EEEWrite(const flash_ssd_config_t * pSSDConfig,
                            uint32_t dest,
                            uint32_t size,
                            const uint8_t * pData)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pData != NULL);
    DEV_ASSERT(size > 0U);
    #if (FEATURE_FLS_IS_FTFM == 1U)
    DEV_ASSERT(0U == (dest & 3U));
    #endif
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint8_t i = 0U;

    /* Check if EEE is enabled */
    if ((FTFx_FCNFG & FTFx_FCNFG_EEERDY_MASK) == FTFx_FCNFG_EEERDY_MASK)
    {
        /* Check range */
        if ((dest < pSSDConfig->EERAMBase) || ((dest + size) > (pSSDConfig->EERAMBase + pSSDConfig->EEESize)))
        {
            ret = STATUS_ERROR;
        }

        while ((size > 0U) && (ret == STATUS_SUCCESS))
        {
            #if (FEATURE_FLS_IS_FTFM == 1U)
            if (size >= 4U)
            {
                i = 4U;
            }
            else
            {
                i = size;
            }
            #else
            /* Dest is 32bit-aligned and size is not less than 4 */
            if ((0U == (dest & 3U)) && (size >= 4U))
            {
                i = 4U;
            }
            else if ((0U == (dest & 1U)) && (size >= 2U))
            {
                i = 2U;
            }
            else
            {
                i = 1U;
            }
            #endif

            ret = FLASH_DRV_WaitEEWriteToFinish(pSSDConfig,
                                                dest,
                                                pData,
                                                i);
            /* Update destination address for next iteration */
            dest += i;
            /* Update size for next iteration */
            size -= i;
            /* Update data for next iteration */
            pData += i;
        }
    }
    else
    {
        ret = STATUS_UNSUPPORTED;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DEFlashPartition
 * Description   : Prepares the FlexNVM block for use as D-Flash, EEPROM backup, or a combination
 * of both and initializes the FlexRAM.
 * The single partition choice should be used through entire life time of a given
 * application to guarantee the Flash endurance and data retention of Flash module.
 *
 * Implements    : FLASH_DRV_DEFlashPartition_Activity
 *END**************************************************************************/
status_t FLASH_DRV_DEFlashPartition(const flash_ssd_config_t * pSSDConfig,
                                    uint8_t uEEEDataSizeCode,
                                    uint8_t uDEPartitionCode,
                                    uint8_t uCSEcKeySize,
                                    bool uSFE,
                                    bool flexRamEnableLoadEEEData)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(uCSEcKeySize <= CSE_KEY_SIZE_CODE_MAX);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL & MGSTAT0 flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_PROGRAM_PARTITION;
        FTFx_FCCOB1 = uCSEcKeySize;
        FTFx_FCCOB2 = (uint8_t)(uSFE ? 1U : 0U);
        FTFx_FCCOB3 = (uint8_t)(flexRamEnableLoadEEEData ? 0U : 1U);
        FTFx_FCCOB4 = uEEEDataSizeCode;
        FTFx_FCCOB5 = uDEPartitionCode;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetDFlashProtection
 * Description   : Retrieves current P-Flash protection status. Considering the time consumption
 * for getting protection is very low and even can be ignored, it is not necessary to utilize
 * the Callback function to support the time-critical events.
 *
 * Implements    : FLASH_DRV_GetDFlashProtection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_GetDFlashProtection(const flash_ssd_config_t * pSSDConfig,
                                       uint8_t * protectStatus)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(protectStatus != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */

    /* Check if size of DFlash = 0 */
    if (pSSDConfig->DFlashSize == 0U)
    {
        ret = STATUS_UNSUPPORTED;
    }
    else
    {
        *protectStatus = FTFx_FDPROT;
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_SetDFlashProtection
 * Description   : Sets the D-Flash protection to the intended protection status.
 * Setting D-Flash protection status is subject to a protection transition restriction.
 * If there is a setting violation, it returns failed information
 * and the current protection status will not be changed.
 *
 * Implements    : FLASH_DRV_SetDFlashProtection_Activity
 *END**************************************************************************/
status_t FLASH_DRV_SetDFlashProtection(const flash_ssd_config_t * pSSDConfig,
                                       uint8_t protectStatus)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */

    /* Check if size of DFlash = 0 */
    if (pSSDConfig->DFlashSize == 0U)
    {
        ret = STATUS_UNSUPPORTED;
    }
    else
    {
        FTFx_FDPROT = protectStatus;
        if (protectStatus != FTFx_FDPROT)
        {
            ret = STATUS_ERROR;
        }
        else
        {
            /* Do nothing */
        }
    }

    return ret;
}
#endif /* If FEATURE_FLS_HAS_FLEX_NVM */

#if FEATURE_FLS_HAS_PF_BLOCK_SWAP
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_PFlashSwap
 * Description   : Provides to user with an ability to interfere in a swap progress by letting the
 * user code know about the swap state in each phase of the process. This is done via pSwapCallBack()
 * parameter. To stop at each intermediate swap state, set the return value of
 * this callback function to FALSE. To complete swap process within a single call,
 * set the return value of this function to TRUE.
 *
 * Erase the non-active swap indicator somewhere in the application code
 * or in the swap call back function when swap system is in UPDATE state.
 *
 * In addition, if user does not want to use the swap call back parameter, pass the NULL_SWAP_CALLBACK
 * as a null pointer. In this situation, the PFlashSwap() behaves in the same way as setting the return
 * value of pSwapCallBack to TRUE and the user does not need to erase the non-active swap
 * indicator when the swap system is in UPDATE state.
 * The swap indicator provided by the user must be within the lower half of P-Flash block but not in the
 * Flash configuration area. If P-Flash block has two logical blocks, the swap indicator must be
 * in P-Flash block 0. If the P-Flash block has four logical blocks, the swap indicator can be in block
 * 0 or block 1. It must not be in the Flash configuration field.
 * The user must use the same swap indicator for all swap control code except report swap status once
 * swap system has been initialized. To refresh swap system to un-initialization state,
 * use the FlashEraseAllBlock() to clean up the swap environment.
 *
 *END**************************************************************************/
status_t FLASH_DRV_PFlashSwap(const flash_ssd_config_t * pSSDConfig,
                              uint32_t addr,
                              flash_swap_callback_t pSwapCallback)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pSwapCallback != NULL);
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    uint8_t currentSwapMode, currentSwapBlockStatus, nextSwapBlockStatus;
    bool swapContinue;
    currentSwapMode = 0xFFU;
    currentSwapBlockStatus = 0xFFU;
    nextSwapBlockStatus = 0xFFU;
    swapContinue = false;

    /* Report current swap state */
    ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig, addr, FTFx_SWAP_REPORT_STATUS, &currentSwapMode, &currentSwapBlockStatus, &nextSwapBlockStatus);

    if (STATUS_SUCCESS == ret)
    {
        if ((FTFx_SWAP_UNINIT == currentSwapMode) || (FTFx_SWAP_READY == currentSwapMode) || (FTFx_SWAP_UPDATE == currentSwapMode))
        {
            /* If current swap mode is Uninitialized */
            if (FTFx_SWAP_UNINIT == currentSwapMode)
            {
                /* Initialize Swap to Initialized/READY state */
                ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig,
                                              addr,
                                              FTFx_SWAP_SET_INDICATOR_ADDR,
                                              &currentSwapMode,
                                              &currentSwapBlockStatus,
                                              &nextSwapBlockStatus);
            }
            /* If current swap mode is Initialized/Ready */
            else if (FTFx_SWAP_READY == currentSwapMode)
            {
                /* Initialize Swap to UPDATE state */
                ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig,
                                              addr,
                                              FTFx_SWAP_SET_IN_PREPARE,
                                              &currentSwapMode,
                                              &currentSwapBlockStatus,
                                              &nextSwapBlockStatus);
            }
            else
            {
                /* If (FTFx_SWAP_UPDATE == currentSwapMode) do nothing */
            }

            /* Check for the success of command execution
             * Report the current swap state to user via callback
             */
            if ((NULL_SWAP_CALLBACK != pSwapCallback) && (STATUS_SUCCESS == ret))
            {
                swapContinue = pSwapCallback(currentSwapMode);

                if (swapContinue == true)
                {
                    /* Report current swap state */
                    ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig,
                                                  addr,
                                                  FTFx_SWAP_REPORT_STATUS,
                                                  &currentSwapMode,
                                                  &currentSwapBlockStatus,
                                                  &nextSwapBlockStatus);
                }
            }
        }
        else
        {
            /* Do nothing */
        }

        if ((NULL_SWAP_CALLBACK == pSwapCallback) && (FTFx_SWAP_UPDATE == currentSwapMode))
        {
            /* Erase indicator sector in non active block to proceed swap system to update-erased state */
            ret = FLASH_DRV_EraseSector(pSSDConfig, addr + (pSSDConfig->PFlashSize >> 1U), (uint32_t)FEATURE_FLS_PF_BLOCK_SECTOR_SIZE);
            if (STATUS_SUCCESS == ret)
            {
                /* Now the swap state must be Update-Erased, so report current swap state */
                ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig,
                                              addr,
                                              FTFx_SWAP_REPORT_STATUS,
                                              &currentSwapMode,
                                              &currentSwapBlockStatus,
                                              &nextSwapBlockStatus);
            }
        }

        /* If current swap mode is Update or Update-Erased */
        if (FTFx_SWAP_UPDATE_ERASED == currentSwapMode)
        {
            if (NULL_SWAP_CALLBACK == pSwapCallback)
            {
                swapContinue = true;
            }
            else
            {
                swapContinue = pSwapCallback(currentSwapMode);
            }

            if (swapContinue == true)
            {
                /* Progress Swap to COMPLETE State */
                ret = FLASH_DRV_PFlashSwapCtl(pSSDConfig,
                                              addr,
                                              FTFx_SWAP_SET_IN_COMPLETE,
                                              &currentSwapMode,
                                              &currentSwapBlockStatus,
                                              &nextSwapBlockStatus);
            }
        }
    }

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_PFlashSwapCtl
 * Description   : Performs swap control command corresponding with the swap
 * control code provided via the swap command parameter.
 *
 *END**************************************************************************/
status_t FLASH_DRV_PFlashSwapCtl(const flash_ssd_config_t * pSSDConfig,
                                 uint32_t addr,
                                 uint8_t swapcmd,
                                 uint8_t * pCurrentSwapMode,
                                 uint8_t * pCurrentSwapBlockStatus,
                                 uint8_t * pNextSwapBlockStatus)
{
    DEV_ASSERT(pSSDConfig != NULL);
    DEV_ASSERT(pCurrentSwapMode != NULL);
    DEV_ASSERT(pCurrentSwapBlockStatus != NULL);
    DEV_ASSERT(pNextSwapBlockStatus != NULL);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_PFLASH_SWAP;
        FTFx_FCCOB1 = GET_BIT_16_23(addr);
        FTFx_FCCOB2 = GET_BIT_8_15(addr);
        FTFx_FCCOB3 = GET_BIT_0_7(addr);
        FTFx_FCCOB4 = swapcmd;
        FTFx_FCCOB5 = 0xFFU;
        FTFx_FCCOB6 = 0xFFU;
        FTFx_FCCOB7 = 0xFFU;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);

        if (STATUS_SUCCESS == ret)
        {
            *pCurrentSwapMode = FTFx_FCCOB5;
            *pCurrentSwapBlockStatus = FTFx_FCCOB6;
            *pNextSwapBlockStatus = FTFx_FCCOB7;
        }
    }

    return ret;
}
#endif /* End of FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP */

#if FEATURE_FLS_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EraseAllBlockUnsecure
 * Description   : Erases all Flash memory, initializes the FlexRAM, verifies
 * all memory contents, and then releases the MCU security.
 *
 * Implements    : FLASH_DRV_EraseAllBlockUnsecure_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EraseAllBlockUnsecure(const flash_ssd_config_t * pSSDConfig)
{
    DEV_ASSERT(pSSDConfig != NULL);
    status_t ret;    /* Return code variable */

    /* Check CCIF to verify the previous command is completed */
    if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
        ret = STATUS_BUSY;
    }
    else
    {
        /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
        CLEAR_FTFx_FSTAT_ERROR_BITS;

        /* Passing parameter to the command */
        FTFx_FCCOB0 = FTFx_ERASE_ALL_BLOCK_UNSECURE;

        /* Calling flash command sequence function to execute the command */
        ret = FLASH_DRV_CommandSequence(pSSDConfig);
    }

    return ret;
}
#endif /* End of FEATURE_FLS_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD */

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableCmdCompleteInterupt
 * Description   : Enable the command complete interrupt is generated when
 * an FTFC command completes.
 *
 * Implements    : FLASH_DRV_EnableCmdCompleteInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableCmdCompleteInterupt(void)
{
    const IRQn_Type flashIrqId = FTFC_COMMAND_COMPLETE_IRQS;

    /* Enable the command complete interrupt */
    FTFx_FCNFG |= FTFx_FCNFG_CCIE_MASK;
    INT_SYS_EnableIRQ(flashIrqId);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableCmdCompleteInterupt
 * Description   : Disable the command complete interrupt.
 *
 * Implements    : FLASH_DRV_DisableCmdCompleteInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableCmdCompleteInterupt(void)
{
    const IRQn_Type flashIrqId = FTFC_COMMAND_COMPLETE_IRQS;

    /* Disable the command complete interrupt */
    FTFx_FCNFG &= (uint8_t)(~FTFx_FCNFG_CCIE_MASK);
    INT_SYS_DisableIRQ(flashIrqId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableReadColisionInterupt
 * Description   : Enable the read collision error interrupt generation when an
 * FTFC read collision error occurs.
 *
 * Implements    : FLASH_DRV_EnableReadColisionInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableReadColisionInterupt(void)
{
    const IRQn_Type flashIrqId = FTFC_READ_COLLISION_IRQS;

    /* Enable the read collision error interrupt */
    FTFx_FCNFG |= FTFx_FCNFG_RDCOLLIE_MASK;
    INT_SYS_EnableIRQ(flashIrqId);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableReadColisionInterupt
 * Description   : Disable the read collision error interrupt
 *
 * Implements    : FLASH_DRV_DisableReadColisionInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableReadColisionInterupt(void)
{
    const IRQn_Type flashIrqId = FTFC_READ_COLLISION_IRQS;

    /* Disable the read collision error interrupt */
    FTFx_FCNFG &= (uint8_t)(~FTFx_FCNFG_RDCOLLIE_MASK);
    INT_SYS_DisableIRQ(flashIrqId);
}

#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableDoubleBitFaultInterupt
 * Description   : Enable the platform Flash double bit fault detect interrupt 
 * generation when an uncorrectable ECC fault is detected during a valid flash 
 * read access from the platform flash controller.
 *
 * Implements    : FLASH_DRV_EnableDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableDoubleBitFaultInterupt(void)
{
    /* Enable the double bit fault detect interrupt */
    FTFx_FERCNFG |= FTFx_FERCNFG_DFDIE_MASK;
#if FEATURE_FLS_HAS_INTERRUPT_DOUBLE_BIT_FAULT_IRQ
    INT_SYS_EnableIRQ(FTFC_Fault_IRQn);
#else
    INT_SYS_EnableIRQ(FTFC_IRQn);
#endif
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableDoubleBitFaultInterupt
 * Description   : Disable the platform Flash double bit fault detect interrupt
 *
 * Implements    : FLASH_DRV_DisableDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableDoubleBitFaultInterupt(void)
{
    /* Disable the double bit fault detect interrupt */
    FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_DFDIE_MASK);
#if FEATURE_FLS_HAS_INTERRUPT_DOUBLE_BIT_FAULT_IRQ
    INT_SYS_DisableIRQ(FTFC_Fault_IRQn);
#else
    INT_SYS_DisableIRQ(FTFC_IRQn);
#endif
}

#if (FEATURE_FLS_IS_FTFM == 1U)
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableFlexRAMDoubleBitFaultInterupt
 * Description   : Enable the platform FlexRAM double bit fault detect interrupt 
 * generation when an uncorrectable ECC fault is detected detected during a valid
 * FlexRAM or CSE_PRAM read access from the platform RAM controller.
 *
 * Implements    : FLASH_DRV_EnableFlexRAMDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableFlexRAMDoubleBitFaultInterupt(void)
{
    /* Enable the platform FlexRAM double bit fault detect interrupt */
    FTFx_FERCNFG |= FTFx_FERCNFG_PDFDIE_MASK;
    INT_SYS_EnableIRQ(FTFC_EEERAM_ECC_ERR_IRQn);
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableFlexRamDoubleBitFaultInterupt
 * Description   : Disable the platform FlexRAM double bit fault detect interrupt 
 *
 * Implements    : FLASH_DRV_DisableFlexRamDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableFlexRamDoubleBitFaultInterupt(void)
{
    /* Disable the platform FlexRAM double bit fault detect interrupt */
    FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_PDFDIE_MASK);
    INT_SYS_DisableIRQ(FTFC_EEERAM_ECC_ERR_IRQn);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableCtrFlashDoubleBitFaultInterupt
 * Description   : Enable the controller Flash double bit fault detect interrupt 
 * generation when an uncorrectable ECC fault is detected during a valid
 * internal RAM or flash read access from the flash memory controller.
 *
 * Implements    : FLASH_DRV_EnableCtrFlashDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableCtrFlashDoubleBitFaultInterupt(void)
{
    /* Enable the controller Flash double bit fault detect interrupt */
    FTFx_FERCNFG |= FTFx_FERCNFG_CDFDIE_MASK;
    INT_SYS_EnableIRQ(FTFC_Mgate_ECC_ERR_IRQn);
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableCtrFlashDoubleBitFaultInterupt
 * Description   : Disable the controller Flash double bit fault detect interrupt 
 *
 * Implements    : FLASH_DRV_DisableCtrFlashDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableCtrFlashDoubleBitFaultInterupt(void)
{
    /* Disable the controller Flash double bit fault detect interrupt */
    FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_CDFDIE_MASK);
    INT_SYS_DisableIRQ(FTFC_Mgate_ECC_ERR_IRQn);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_EnableCtrFlexRamDoubleBitFaultInterupt
 * Description   : Enable the controller FlexRAM double bit fault detect interrupt 
 *  generation when an uncorrectable ECC fault is detected during a valid FlexRAM 
 * or CSE_PRAM read access from the flash memory controller.
 *
 * Implements    : FLASH_DRV_EnableCtrFlexRamDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
status_t FLASH_DRV_EnableCtrFlexRamDoubleBitFaultInterupt(void)
{
    /* Enable the controller FlexRAM double bit fault detect interrupt */
    FTFx_FERCNFG |= FTFx_FERCNFG_EDFDIE_MASK;
    INT_SYS_EnableIRQ(FTFC_EEERAM_ECC_ERR_IRQn);
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_DisableCtrFlexRamDoubleBitFaultInterupt
 * Description   : Disable the controller FlexRAM double bit fault detect interrupt
 *
 * Implements    : FLASH_DRV_DisableCtrFlexRamDoubleBitFaultInterupt_Activity
 *END**************************************************************************/
void FLASH_DRV_DisableCtrFlexRamDoubleBitFaultInterupt(void)
{
    /* Disable the controller FlexRAM double bit fault detect interrupt */
    FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_EDFDIE_MASK);
    INT_SYS_DisableIRQ(FTFC_EEERAM_ECC_ERR_IRQn);
}
#endif /* End of FEATURE_FLS_IS_FTFM */
#endif /* End of FEATURE_FLS_HAS_DETECT_ECC_ERROR */

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_GetDefaultConfig
 * Description   : This function will get default flash user configuration.
 *
 * Implements    : FLASH_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void FLASH_DRV_GetDefaultConfig(flash_user_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->PFlashBase  = 0x00000000U;
    config->PFlashSize  = FEATURE_FLS_PF_BLOCK_SIZE;
    config->DFlashBase  = 0x10000000U;
    config->EERAMBase   = 0x14000000U;
    config->CallBack    = NULL_CALLBACK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_DRV_ProgramCheckExecute
 * Description   : Internal function used by FLASH_DRV_ProgramCheck function to
 * tests a previously programmed P-Flash or D-Flash long word
 * to see if it reads correctly at the specified margin level. This
 * API always returns STATUS_SUCCESS if size provided by user is zero
 * regardless of the input validation
 *
 *END**************************************************************************/
static status_t FLASH_DRV_ProgramCheckExecute(const flash_ssd_config_t * pSSDConfig,
                                              uint32_t dest,
                                              uint32_t size,
                                              const uint8_t * pExpectedData,
                                              uint32_t * pFailAddr,
                                              uint8_t marginLevel)
{
    status_t ret = STATUS_SUCCESS;  /* Return code variable                                 */
    uint32_t offsetAddr;            /* Offset address to convert to internal memory address */
    uint32_t temp;                  /* Temporary variable                                   */
    uint32_t tempSize = size;       /* Temporary of size variation                          */
    uint8_t i;


    /* Check if the destination is aligned or not */
#if FEATURE_FLS_HAS_FLEX_NVM
    offsetAddr = pSSDConfig->DFlashBase;
    if ((dest >= offsetAddr) && (dest < (offsetAddr + pSSDConfig->DFlashSize)))
    {
        dest += 0x800000U - offsetAddr;
    }
    else
#endif
    {
        offsetAddr = pSSDConfig->PFlashBase;
        if ((dest >= offsetAddr) && (dest < (offsetAddr + pSSDConfig->PFlashSize)))
        {
            dest -= offsetAddr;
        }
        else
        {
            ret = STATUS_ERROR;
            tempSize = 0U;
        }
    }

    while ((tempSize > 0U) && (STATUS_SUCCESS == ret))
    {
        /* Check CCIF to verify the previous command is completed */
        if (0U == (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK))
        {
            ret = STATUS_BUSY;
        }
        else
        {
            /* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
            CLEAR_FTFx_FSTAT_ERROR_BITS;

            /* Passing parameter to the command */
            FTFx_FCCOB0 = FTFx_PROGRAM_CHECK;
            FTFx_FCCOB1 = GET_BIT_16_23(dest);
            FTFx_FCCOB2 = GET_BIT_8_15(dest);
            FTFx_FCCOB3 = GET_BIT_0_7(dest);
            FTFx_FCCOB4 = marginLevel;

            for (i = 0U; i < FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT; i++)
            {
                temp = FTFx_BASE + i + 0x0CU;
                *(uint8_t *)(temp) = pExpectedData[i];
            }

            /* Calling flash command sequence function to execute the command */
            ret = FLASH_DRV_CommandSequence(pSSDConfig);

            /* Checking for the success of command execution */
            if (STATUS_SUCCESS != ret)
            {
                #if FEATURE_FLS_HAS_FLEX_NVM
                if (dest >= 0x800000U)
                {
                    *pFailAddr = dest + offsetAddr - 0x800000U;
                }
                else
                #endif
                {
                    *pFailAddr = dest + offsetAddr;
                }
            }
            else
            {
                /* Update size for next iteration */
                tempSize -= FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
                /* Increment the source address by 1 */
                pExpectedData += FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
                /* Update destination address for next iteration */
                dest += FEATURE_FLS_PF_CHECK_CMD_ADDRESS_ALIGMENT;
            }
        }
    }

    return ret;
}
/*******************************************************************************
* EOF
*******************************************************************************/
