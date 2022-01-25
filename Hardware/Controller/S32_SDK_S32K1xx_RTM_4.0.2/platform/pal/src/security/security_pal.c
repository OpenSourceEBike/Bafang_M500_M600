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
 
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or 
 * different essential type. 
 * The expression converts the pal type to specific driver type. The types have 
 * the same size.
 * 
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 
 * 'essentially enum<i>' to 'essentially enum<i>'. 
 * The expression converts the pal type to specific driver type. The types have 
 * the same size.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 * 
 */

#include "security_pal.h"
#include "device_registers.h"

/* Include driver files */
#if (defined (SECURITY_OVER_CSEC))
    #include "csec_driver.h"
#elif (defined (SECURITY_OVER_HSM))
    #include "hsm_driver.h"
#endif
	
#if (defined (SECURITY_OVER_CSEC))
    #define NO_OF_INSTS_FOR_SECURITY_PAL    NO_OF_CSEC_INSTS_FOR_SECURITY
    static bool g_bCsecStateIsAllocated[NO_OF_INSTS_FOR_SECURITY_PAL];
    static security_instance_t g_tCsecInstance[NO_OF_INSTS_FOR_SECURITY_PAL];
#elif (defined (SECURITY_OVER_HSM))
    #define NO_OF_INSTS_FOR_SECURITY_PAL    NO_OF_HSM_INSTS_FOR_SECURITY
    static bool g_bHsmStateIsAllocated[NO_OF_INSTS_FOR_SECURITY_PAL];
    static security_instance_t g_tHsmInstance[NO_OF_INSTS_FOR_SECURITY_PAL];
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : SecurityAllocateInstance
 * Description   : Allocate state.
 *
 *END**************************************************************************/
static status_t SecurityAllocateInstance(bool * isAllocated, 
                                         security_instance_t * instList, 
                                         security_instance_t instId, 
                                         uint32_t numberOfInst, 
                                         uint32_t * retInstId)
{
    uint32_t counter = 0U;
    
    for (counter = 0U; counter < numberOfInst; counter++)
    {
        if (false == isAllocated[counter])
        {
            isAllocated[counter] = true;
            instList[counter] = instId;
			*retInstId = counter;
            return STATUS_SUCCESS;
        }    
    }
    
    return STATUS_ERROR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SecurityFreeInstance
 * Description   : Free state.
 *
 *END**************************************************************************/
static void SecurityFreeInstance(bool * isAllocated, 
                                     const security_instance_t * instList, 
                                     security_instance_t instId, 
                                     uint32_t numberOfInst)
{
    uint32_t counter = 0U;
    
    for (counter = 0U; counter < numberOfInst; counter++)
    {
        if (instId == instList[counter])
        {
            isAllocated[counter] = false;
            break;
        }    
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GetDefaultConfig
 * Description   : Initializes the configuration struct members to
 *                 default values.
 * Implements    : SECURITY_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
void SECURITY_GetDefaultConfig(security_user_config_t *config)
{
    DEV_ASSERT(config != NULL);
    config->callback = NULL;
    config->callbackParam = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_Init
 * Description   : Initializes the state structure, enables interrupt,
 *                 installs interrupt handler.
 * Implements    : SECURITY_Init_Activity
 *
 *END**************************************************************************/
status_t SECURITY_Init(security_instance_t instance, 
                       const security_user_config_t * config)
{
    DEV_ASSERT(config != NULL);
    status_t status = STATUS_SUCCESS;
    uint32_t stateInsts = 0U;
#if (defined (SECURITY_OVER_CSEC))  
    (void)instance;	
    static csec_state_t s_tCsecState[NO_OF_INSTS_FOR_SECURITY_PAL];	

    status = SecurityAllocateInstance(g_bCsecStateIsAllocated, g_tCsecInstance, SECURITY_INSTANCE0, NO_OF_INSTS_FOR_SECURITY_PAL, &stateInsts);
    if (STATUS_SUCCESS != status)
    {
        return STATUS_ERROR;
    }
    (void)CSEC_DRV_Init(&s_tCsecState[stateInsts]);
    (void)CSEC_DRV_InstallCallback(config->callback, config->callbackParam);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;	
	static hsm_state_t s_tHsmState[NO_OF_INSTS_FOR_SECURITY_PAL];

    status = SecurityAllocateInstance(g_bHsmStateIsAllocated, g_tHsmInstance, SECURITY_INSTANCE0, NO_OF_INSTS_FOR_SECURITY_PAL, &stateInsts);
    if (STATUS_SUCCESS != status)
    {
        return STATUS_ERROR;
    }
    status = HSM_DRV_Init(&s_tHsmState[stateInsts]);
    if (STATUS_SUCCESS != status)
    {
        return STATUS_ERROR;
    }
    (void)HSM_DRV_InstallCallback(config->callback, config->callbackParam);
#else
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));	
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_Deinit
 * Description   : Writes with '0' the entire state structure, resets internal
 *         state pointer to NULL, disables interrupts.
 * Implements    : SECURITY_Deinit_Activity
 *
 *END**************************************************************************/
status_t SECURITY_Deinit(security_instance_t instance)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
    
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)CSEC_DRV_Deinit();
    SecurityFreeInstance(g_bCsecStateIsAllocated, g_tCsecInstance, SECURITY_INSTANCE0, NO_OF_INSTS_FOR_SECURITY_PAL);

#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_Deinit();
    if (STATUS_SUCCESS != status)
    {
        return STATUS_ERROR;
    }
    SecurityFreeInstance(g_bHsmStateIsAllocated, g_tHsmInstance, SECURITY_INSTANCE0, NO_OF_INSTS_FOR_SECURITY_PAL);

#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_EncryptEcbBlocking
 * Description   : Performs AES-128 encryption in ECB mode of the input plain
 *         text buffer.
 * Implements    : SECURITY_EncryptEcbBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_EncryptEcbBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *plainText,
                                     uint32_t msgLen,
                                     uint8_t *cipherText,
                                     uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));

    status_t status = STATUS_SUCCESS;  
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_EncryptECB((csec_key_id_t)keyId, plainText, msgLen, cipherText, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_EncryptECB((hsm_key_id_t)keyId, plainText, msgLen, cipherText, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DecryptEcbBlocking
 * Description   : Performs AES-128 decryption in ECB mode of the input cipher
 *         text buffer.
 * Implements    : SECURITY_DecryptEcbBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DecryptEcbBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *cipherText,
                                     uint32_t msgLen,
                                     uint8_t *plainText,
                                     uint32_t timeout)
{  
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));

    status_t status = STATUS_SUCCESS;    
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_DecryptECB((csec_key_id_t)keyId, cipherText, msgLen, plainText, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DecryptECB((hsm_key_id_t)keyId, cipherText, msgLen, plainText, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_EncryptCbcBlocking
 * Description   : Performs AES-128 encryption in CBC mode of the input plain
 *         text buffer.
 * Implements    : SECURITY_EncryptCbcBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_EncryptCbcBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *plainText,
                                     uint32_t msgLen,
                                     const uint8_t *iv,
                                     uint8_t *cipherText,
                                     uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
    
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_EncryptCBC((csec_key_id_t)keyId, plainText, msgLen, iv, cipherText, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_EncryptCBC((hsm_key_id_t)keyId, plainText, msgLen, iv, cipherText, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DecryptCbcBlocking
 * Description   : Performs AES-128 decryption in CBC mode of the input cipher
 *         text buffer.
 * Implements    : SECURITY_DecryptCbcBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DecryptCbcBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *cipherText,
                                     uint32_t msgLen,
                                     const uint8_t* iv,
                                     uint8_t *plainText,
                                     uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
    
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_DecryptCBC((csec_key_id_t)keyId, cipherText, msgLen, iv, plainText, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DecryptCBC((hsm_key_id_t)keyId, cipherText, msgLen, iv, plainText, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GenerateMacBlocking
 * Description   : Calculates MAC of a given message using CMAC with AES-128.
 * Implements    : SECURITY_GenerateMacBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GenerateMacBlocking(security_instance_t instance,
                                      security_key_id_t keyId,
                                      const uint8_t *msg,
                                      uint64_t msgLen,
                                      uint8_t *cmac,
                                      uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
#if (defined (SECURITY_OVER_CSEC))	
	DEV_ASSERT((uint64_t)0xFFFFFFFFU >= msgLen);
#endif
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_GenerateMAC((csec_key_id_t)keyId, msg, (uint32_t)msgLen, cmac, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GenerateMAC((hsm_key_id_t)keyId, msg, msgLen, cmac, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_VerifyMacBlocking
 * Description   : Verifies the MAC of a given message using CMAC with AES-128.
 * Implements    : SECURITY_VerifyMacBlocking_Activity
 *
 *END**************************************************************************/
status_t SECURITY_VerifyMacBlocking(security_instance_t instance,
                                    security_key_id_t keyId,
                                    const uint8_t *msg,
                                    uint64_t msgLen,
                                    const uint8_t *mac,
                                    uint16_t macLen,
                                    bool *verifStatus,
                                    uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
#if (defined (SECURITY_OVER_CSEC))	
	DEV_ASSERT((uint64_t)0xFFFFFFFFU >= msgLen);
#endif
       
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_VerifyMAC((csec_key_id_t)keyId, msg, (uint32_t)msgLen, mac, macLen, verifStatus, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_VerifyMAC((hsm_key_id_t)keyId, msg, msgLen, mac, macLen, verifStatus, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_LoadKey
 * Description   : Updates an internal key per the SHE specification.
 * Implements    : SECURITY_LoadKey_Activity
 *
 *END**************************************************************************/
status_t SECURITY_LoadKey(security_instance_t instance,
                          security_key_id_t keyId,
                          const uint8_t *m1,
                          const uint8_t *m2,
                          const uint8_t *m3,
                          uint8_t *m4,
                          uint8_t *m5,
                          uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_LoadKey((csec_key_id_t)keyId, m1, m2, m3, m4, m5);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_LoadKey((hsm_key_id_t)keyId, m1, m2, m3, m4, m5, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_LoadPlainKey
 * Description   : Updates the RAM key memory slot with a 128-bit plaintext.
 * Implements    : SECURITY_LoadPlainKey_Activity
 *
 *END**************************************************************************/
status_t SECURITY_LoadPlainKey(security_instance_t instance,
                               const uint8_t *plainKey,
                               uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_LoadPlainKey(plainKey);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_LoadPlainKey(plainKey, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_ExportRamKey
 * Description   : Exports the RAM_KEY into a format protected by SECRET_KEY.
 * Implements    : SECURITY_ExportRamKey_Activity
 *
 *END**************************************************************************/
status_t SECURITY_ExportRamKey(security_instance_t instance,
                               uint8_t *m1,
                               uint8_t *m2,
                               uint8_t *m3,
                               uint8_t *m4,
                               uint8_t *m5,
                               uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_ExportRAMKey(m1, m2, m3, m4, m5);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_ExportRAMKey(m1, m2, m3, m4, m5, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_ExtendSeed
 * Description   : Extends the seed of the PRNG.
 * Implements    : SECURITY_ExtendSeed_Activity
 *
 *END**************************************************************************/
status_t SECURITY_ExtendSeed(security_instance_t instance,
                             const uint8_t *entropy,
                             uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_ExtendSeed(entropy);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_ExtendSeed(entropy, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_InitRng
 * Description   : Initializes the seed and derive a key for the PRNG.
 * Implements    : SECURITY_InitRng_Activity
 *
 *END**************************************************************************/
status_t SECURITY_InitRng(security_instance_t instance,
                          uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_InitRNG();
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_InitRNG(timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GenerateRnd
 * Description   : Generates a vector of 128 random bits.
 * Implements    : SECURITY_GenerateRnd_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GenerateRnd(security_instance_t instance,
                              uint8_t *rnd,
                              uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_GenerateRND(rnd);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GenerateRND(rnd, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GetId
 * Description   : Returns the identity (UID) and the value of the status
 *         register protected by a MAC over a challenge and the data.
 * Implements    : SECURITY_GetId_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GetId(security_instance_t instance,
                        const uint8_t *challenge,
                        uint8_t *uid,
                        uint8_t *sreg,
                        uint8_t *mac,
                        uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_GetID(challenge, uid, sreg, mac);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GetID(challenge, uid, sreg, mac, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_SecureBoot
 * Description   : Loads the command processor firmware and memory slot data
 *        and then executes the SHE secure boot protocol.
 * Implements    : SECURITY_SecureBoot_Activity
 *
 *END**************************************************************************/
status_t SECURITY_SecureBoot(security_instance_t instance,
                             uint32_t bootImageSize,
                             const uint8_t *bootImagePtr,
                             uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)bootImageSize;
    (void)bootImagePtr;
    (void)timeout;
    status = STATUS_UNSUPPORTED;
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_SecureBoot(bootImageSize, bootImagePtr, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_BootFailure
 * Description   : Signals a failure detected during later stages of the
 *         boot process.
 * Implements    : SECURITY_BootFailure_Activity
 *
 *END**************************************************************************/
status_t SECURITY_BootFailure(security_instance_t instance,
                              uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_BootFailure();
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_BootFailure(timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_BootOk
 * Description   : Marks a successful boot verification during later stages
 *         of the boot process.
 * Implements    : SECURITY_BootOk_Activity
 *
 *END**************************************************************************/
status_t SECURITY_BootOk(security_instance_t instance,
                         uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_BootOK();
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_BootOK(timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_BootDefine
 * Description   : Implements an extension of the SHE standard to define both
 *         the user boot size and boot method.
 * Implements    : SECURITY_BootDefine_Activity
 *
 *END**************************************************************************/
status_t SECURITY_BootDefine(security_instance_t instance,
                             uint32_t bootSize,
                             security_boot_flavor_t bootFlavor,
                             uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_BootDefine(bootSize, (csec_boot_flavor_t)bootFlavor);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    (void)bootSize;
    (void)bootFlavor;
    (void)timeout;
    status = STATUS_UNSUPPORTED;
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DbgChal
 * Description   : Obtains a random number which the user shall use along with
 *         the MASTER_ECU_KEY and UID to return an authorization
 *         request.
 * Implements    : SECURITY_DbgChal_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DbgChal(security_instance_t instance,
                          uint8_t *challenge,
                          uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_DbgChal(challenge);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DbgChal(challenge, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DbgAuth
 * Description   : Erases all keys (actual and outdated) stored in NVM Memory
 *         if the authorization is confirmed.
 * Implements    : SECURITY_DbgAuth_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DbgAuth(security_instance_t instance,
                          const uint8_t *authorization,
                          uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)timeout;
    status = CSEC_DRV_DbgAuth(authorization);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DbgAuth(authorization, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_MPCompress
 * Description   : Compresses the given messages by accessing  the
 *         Miyaguchi-Prenell compression feature with in the CSEc
 *         feature set.
 * Implements    : SECURITY_MPCompress_Activity
 *
 *END**************************************************************************/
status_t SECURITY_MPCompress(security_instance_t instance,
                             const uint8_t *msg,
                             uint32_t msgLen,
                             uint8_t *mpCompress,
                             uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
#if (defined (SECURITY_OVER_CSEC) || defined (SECURITY_OVER_HSM))	
	DEV_ASSERT((uint32_t)0xFFFFU >= msgLen);
#endif	
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_MPCompress(msg, (uint16_t)msgLen, mpCompress, timeout);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_MPCompress(msg, (uint16_t)msgLen, mpCompress, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GenerateTrnd
 * Description   : Generates a vector of 128 random bits using TRNG.
 * Implements    : SECURITY_GenerateTrnd_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GenerateTrnd(security_instance_t instance,
                               uint8_t *trnd,
                               uint32_t timeout)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    (void)trnd;
    (void)timeout;
    status = STATUS_UNSUPPORTED;
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GenerateTRND(trnd, timeout);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_CancelCommand
 * Description   : Cancels a previously initiated command.
 * Implements    : SECURITY_CancelCommand_Activity
 *
 *END**************************************************************************/
status_t SECURITY_CancelCommand(security_instance_t instance)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    CSEC_DRV_CancelCommand();
    status = STATUS_SUCCESS;
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_CancelCommand();
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GetAsyncCmdStatus
 * Description   : Checks the status of the execution of an asynchronous command.
 * Implements    : SECURITY_GetAsyncCmdStatus_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GetAsyncCmdStatus(security_instance_t instance)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_GetAsyncCmdStatus();
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GetAsyncCmdStatus();
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_EncryptEcb
 * Description   : Asynchronously performs the AES-128 encryption in ECB mode
 *         of the input plain text buffer.
 * Implements    : SECURITY_EncryptEcb_Activity
 *
 *END**************************************************************************/
status_t SECURITY_EncryptEcb(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *plainText,
                             uint32_t msgLen,
                             uint8_t *cipherText)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_EncryptECBAsync((csec_key_id_t)keyId, plainText, msgLen, cipherText);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_EncryptECBAsync((hsm_key_id_t)keyId, plainText, msgLen, cipherText);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DecryptEcb
 * Description   : Asynchronously performs the AES-128 decryption in ECB mode
 *         of the input cipher text buffer.
 * Implements    : SECURITY_DecryptEcb_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DecryptEcb(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *cipherText,
                             uint32_t msgLen,
                             uint8_t *plainText)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_DecryptECBAsync((csec_key_id_t)keyId, cipherText, msgLen, plainText);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DecryptECBAsync((hsm_key_id_t)keyId, cipherText, msgLen, plainText);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_EncryptCbc
 * Description   : Asynchronously performs the AES-128 encryption in CBC mode
 *         of the input plain text buffer.
 * Implements    : SECURITY_EncryptCbc_Activity
 *
 *END**************************************************************************/
status_t SECURITY_EncryptCbc(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *plainText,
                             uint32_t msgLen,
                             const uint8_t *iv,
                             uint8_t *cipherText)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_EncryptCBCAsync((csec_key_id_t)keyId, plainText, msgLen, iv, cipherText);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_EncryptCBCAsync((hsm_key_id_t)keyId, plainText, msgLen, iv, cipherText);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_DecryptCbc
 * Description   : Asynchronously performs the AES-128 decryption in CBC mode
 *         of the input cipher text buffer.
 * Implements    : SECURITY_DecryptCbc_Activity
 *
 *END**************************************************************************/
status_t SECURITY_DecryptCbc(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *cipherText,
                             uint32_t msgLen,
                             const uint8_t* iv,
                             uint8_t *plainText)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_DecryptCBCAsync((csec_key_id_t)keyId, cipherText, msgLen, iv, plainText);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_DecryptCBCAsync((hsm_key_id_t)keyId, cipherText, msgLen, iv, plainText);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_GenerateMac
 * Description   : Asynchronously calculates the MAC of a given message using
 *         CMAC with AES-128.
 * Implements    : SECURITY_GenerateMac_Activity
 *
 *END**************************************************************************/
status_t SECURITY_GenerateMac(security_instance_t instance,
                              security_key_id_t keyId,
                              const uint8_t *msg,
                              uint64_t msgLen,
                              uint8_t *cmac)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
#if (defined (SECURITY_OVER_CSEC))	
	DEV_ASSERT((uint64_t)0xFFFFFFFFU >= msgLen);
#endif
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_GenerateMACAsync((csec_key_id_t)keyId, msg, (uint32_t)msgLen, cmac);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_GenerateMACAsync((hsm_key_id_t)keyId, msg, msgLen, cmac);
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SECURITY_VerifyMac
 * Description   : Asynchronously verifies  the MAC of a given message using
 *         CMAC with AES-128.
 * Implements    : SECURITY_VerifyMac_Activity
 *
 *END**************************************************************************/
status_t SECURITY_VerifyMac(security_instance_t instance,
                            security_key_id_t keyId,
                            const uint8_t *msg,
                            uint64_t msgLen,
                            const uint8_t *mac,
                            uint16_t macLen,
                            bool *verifStatus)
{
    DEV_ASSERT((uint32_t)NO_OF_INSTS_FOR_SECURITY_PAL > ((uint32_t)instance));
#if (defined (SECURITY_OVER_CSEC))	
	DEV_ASSERT((uint64_t)0xFFFFFFFFU >= msgLen);
#endif
        
    status_t status = STATUS_SUCCESS;
    
#if (defined (SECURITY_OVER_CSEC))
    (void)instance;
    status = CSEC_DRV_VerifyMACAsync((csec_key_id_t)keyId, msg, (uint32_t)msgLen, mac, macLen, verifStatus);
#elif (defined (SECURITY_OVER_HSM))
    (void)instance;
    status = HSM_DRV_VerifyMACAsync((hsm_key_id_t)keyId, msg, msgLen, mac, macLen, verifStatus);
#endif

    return status;
}

/******************************************************************************
 * EOF
 *****************************************************************************/
