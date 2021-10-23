/*
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
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

#include "csec_driver.h"
#include "csec_hw_access.h"
#include "osif.h"

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
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to
 * object type and a pointer to a different object type.
 * The cast is required in order to extract the address of a buffer as an integer
 * and write it to CSE_PRAM (for the commands using pointers to Flash).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Represents the size of a CSE_PRAM page in bytes */
#define CSEC_PAGE_SIZE_IN_BYTES        (16U)

/* Represents the number of CSE_PRAM pages available for data (excluding the
first page, used for command header) */
#define CSEC_DATA_PAGES_AVAILABLE      (7U)
/* Represents the number of bytes in CSE_PRAM available for data (excluding the
first page, used for command header) */
#define CSEC_DATA_BYTES_AVAILABLE      (112U)

/* Represents the shift used for converting CSE_PRAM pages number to/from bytes number */
#define CSEC_BYTES_TO_FROM_PAGES_SHIFT (4U)
/* Represents the shift used for converting CSE_PRAM pages number to/from bits number */
#define CSEC_BYTES_TO_FROM_BITS_SHIFT  (3U)

/* Represents the size in bytes of the M1 entry (used for key management) */
#define CSEC_M1_SIZE_IN_BYTES          (16U)
/* Represents the size in bytes of the M2 entry (used for key management) */
#define CSEC_M2_SIZE_IN_BYTES          (32U)
/* Represents the size in bytes of the M3 entry (used for key management) */
#define CSEC_M3_SIZE_IN_BYTES          (16U)
/* Represents the size in bytes of the M4 entry (used for key management) */
#define CSEC_M4_SIZE_IN_BYTES          (32U)
/* Represents the size in bytes of the M5 entry (used for key management) */
#define CSEC_M5_SIZE_IN_BYTES          (16U)

/* Pointer to runtime state structure.*/
static csec_state_t * g_csecStatePtr = NULL;

/* The FTFC interrupt handler, used when a CSEc command is completed. */
void FTFC_IRQHandler(void);

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

static void CSEC_DRV_InitState(csec_key_id_t keyId,
                               csec_cmd_t cmd,
                               const uint8_t * inBuff,
                               uint8_t * outBuff,
                               uint32_t length);

/* Rounds up the value to the first number multiple of roundTo */
static inline uint32_t CSEC_DRV_RoundTo(uint32_t value,
                                        uint32_t roundTo)
{
    return (value + (roundTo - 1U)) & ~(roundTo - 1U);
}

static void CSEC_DRV_StartEncDecECBCmd(void);
static void CSEC_DRV_ContinueEncDecECBCmd(void);
static void CSEC_DRV_StartEncDecCBCCmd(void);
static void CSEC_DRV_ContinueEncDecCBCCmd(void);
static void CSEC_DRV_StartGenMACCmd(void);
static void CSEC_DRV_ContinueGenMACCmd(void);
static void CSEC_DRV_StartVerifMACCmd(void);
static void CSEC_DRV_ContinueVerifMACCmd(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_Init
 * Description   : This function initializes the internal state of the driver
 * and enables the FTFC interrupt.
 *
 * Implements    : CSEC_DRV_Init_Activity
 * END**************************************************************************/
void CSEC_DRV_Init(csec_state_t * state)
{
    DEV_ASSERT(state != NULL);

    g_csecStatePtr = state;
    g_csecStatePtr->cmdInProgress = false;

    INT_SYS_EnableIRQ(FTFC_IRQn);

    OSIF_TimeDelay(0U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_Deinit
 * Description   : This function clears the internal state of the driver and
 * disables the FTFC interrupt.
 *
 * Implements    : CSEC_DRV_Deinit_Activity
 * END**************************************************************************/
void CSEC_DRV_Deinit()
{
    g_csecStatePtr = NULL;

    INT_SYS_DisableIRQ(FTFC_IRQn);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_EncryptECB
 * Description   : This function performs the AES-128 encryption in ECB mode of
 * the input plain text buffer.
 *
 * Implements    : CSEC_DRV_EncryptECB_Activity
 * END**************************************************************************/
status_t CSEC_DRV_EncryptECB(csec_key_id_t keyId,
                                      const uint8_t * plainText,
                                      uint32_t length,
                                      uint8_t * cipherText,
                                      uint32_t timeout)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_ENC_ECB, plainText, cipherText, length);

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartEncDecECBCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueEncDecECBCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DecryptECB
 * Description   : This function performs the AES-128 decryption in ECB mode of
 * the input cipher text buffer.
 *
 * Implements    : CSEC_DRV_DecryptECB_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DecryptECB(csec_key_id_t keyId,
                                      const uint8_t * cipherText,
                                      uint32_t length,
                                      uint8_t * plainText,
                                      uint32_t timeout)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_DEC_ECB, cipherText, plainText, length);

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartEncDecECBCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueEncDecECBCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_EncryptCBC
 * Description   : This function performs the AES-128 encryption in CBC mode of
 * the input cipher text buffer.
 *
 * Implements    : CSEC_DRV_EncryptCBC_Activity
 * END**************************************************************************/
status_t CSEC_DRV_EncryptCBC(csec_key_id_t keyId,
                                      const uint8_t * plainText,
                                      uint32_t length,
                                      const uint8_t * iv,
                                      uint8_t * cipherText,
                                      uint32_t timeout)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_ENC_CBC, plainText, cipherText, length);
    g_csecStatePtr->iv = iv;

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartEncDecCBCCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueEncDecCBCCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DecryptCBC
 * Description   : This function performs the AES-128 decryption in CBC mode of
 * the input cipher text buffer.
 *
 * Implements    : CSEC_DRV_DecryptCBC_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DecryptCBC(csec_key_id_t keyId,
                                      const uint8_t * cipherText,
                                      uint32_t length,
                                      const uint8_t * iv,
                                      uint8_t * plainText,
                                      uint32_t timeout)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_DEC_CBC, cipherText, plainText, length);
    g_csecStatePtr->iv = iv;

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartEncDecCBCCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueEncDecCBCCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GenerateMAC
 * Description   : This function calculates the MAC of a given message using
 * CMAC with AES-128.
 *
 * Implements    : CSEC_DRV_GenerateMAC_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GenerateMAC(csec_key_id_t keyId,
                                       const uint8_t * msg,
                                       uint32_t msgLen,
                                       uint8_t * cmac,
                                       uint32_t timeout)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(cmac != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_GENERATE_MAC, msg, cmac, CSEC_DRV_RoundTo(msgLen, 0x8) >> CSEC_BYTES_TO_FROM_BITS_SHIFT);
    g_csecStatePtr->msgLen = msgLen;

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartGenMACCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueGenMACCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GenerateMACAddrMode
 * Description   : This function calculates the MAC of a given message using
 * CMAC with AES-128. It is different from the CSEC_DRV_GenerateMAC function in
 * the sense that it does not involve an extra copy of the data on which the
 * CMAC is computed and the message pointer should be a pointer to Flash memory.
 *
 * Implements    : CSEC_DRV_GenerateMACAddrMode_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GenerateMACAddrMode(csec_key_id_t keyId,
                                               const uint8_t * msg,
                                               uint32_t msgLen,
                                               uint8_t * cmac)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(cmac != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the address of the message */
    CSEC_WriteCommandWords(FEATURE_CSEC_FLASH_START_ADDRESS_OFFSET, (uint32_t *)&msg, 1U);
    /* Write the size of the message (in bits) */
    CSEC_WriteCommandWords(FEATURE_CSEC_MESSAGE_LENGTH_OFFSET, &msgLen, 1U);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCmdAndWait(CSEC_CMD_GENERATE_MAC, CSEC_FUNC_FORMAT_ADDR, CSEC_CALL_SEQ_FIRST, keyId);

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();
    /* Read the resulted MAC */
    if (stat == STATUS_SUCCESS)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, cmac, CSEC_PAGE_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_VerifyMAC
 * Description   : This function verifies the MAC of a given message using CMAC
 * with AES-128.
 *
 * Implements    : CSEC_DRV_VerifyMAC_Activity
 * END**************************************************************************/
status_t CSEC_DRV_VerifyMAC(csec_key_id_t keyId,
                                     const uint8_t * msg,
                                     uint32_t msgLen,
                                     const uint8_t * mac,
                                     uint16_t macLen,
                                     bool * verifStatus,
                                     uint32_t timeout)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mac != NULL);
    DEV_ASSERT(verifStatus != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Initialize the internal state of the driver */
    CSEC_DRV_InitState(keyId, CSEC_CMD_VERIFY_MAC, msg, NULL, CSEC_DRV_RoundTo(msgLen, 0x8) >> CSEC_BYTES_TO_FROM_BITS_SHIFT);
    g_csecStatePtr->msgLen = msgLen;
    g_csecStatePtr->verifStatus = verifStatus;
    g_csecStatePtr->macWritten = false;
    g_csecStatePtr->mac = mac;
    g_csecStatePtr->macLen = macLen;

    startTime = OSIF_GetMilliseconds();

    CSEC_DRV_StartVerifMACCmd();

    while (g_csecStatePtr->cmdInProgress)
    {
        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            g_csecStatePtr->errCode = STATUS_TIMEOUT;
            break;
        }

        CSEC_DRV_ContinueVerifMACCmd();
    }

    return g_csecStatePtr->errCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_VerifyMACAddrMode
 * Description   : This function verifies the MAC of a given message using CMAC
 * with AES-128. It is different from the CSEC_DRV_VerifyMAC function in the
 * sense that it does not involve an extra copy of the data on which the CMAC is
 * computed and the message pointer should be a pointer to Flash memory.
 *
 * Implements    : CSEC_DRV_VerifyMACAddrMode_Activity
 * END**************************************************************************/
status_t CSEC_DRV_VerifyMACAddrMode(csec_key_id_t keyId,
                                             const uint8_t * msg,
                                             uint32_t msgLen,
                                             const uint8_t * mac,
                                             uint16_t macLen,
                                             bool * verifStatus)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mac != NULL);
    DEV_ASSERT(verifStatus != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the address of the message */
    CSEC_WriteCommandWords(FEATURE_CSEC_FLASH_START_ADDRESS_OFFSET, (uint32_t *)&msg, 1U);
    /* Write the MAC to be verified */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, mac, CSEC_PAGE_SIZE_IN_BYTES);
    /* Write the size of the message (in bits) */
    CSEC_WriteCommandWords(FEATURE_CSEC_MESSAGE_LENGTH_OFFSET, &msgLen, 1U);
    /* Write the number of bits of the MAC to be compared */
    CSEC_WriteCommandHalfWord(FEATURE_CSEC_MAC_LENGTH_OFFSET, macLen);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCmdAndWait(CSEC_CMD_VERIFY_MAC, CSEC_FUNC_FORMAT_ADDR, CSEC_CALL_SEQ_FIRST, keyId);

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    /* Read the result of the MAC verification */
    if (stat == STATUS_SUCCESS)
    {
    	uint32_t verifWord = CSEC_ReadCommandWord(FEATURE_CSEC_VERIFICATION_STATUS_OFFSET);

        *verifStatus = ((verifWord & CSEC_UPPER_HALF_MASK) == 0UL);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_LoadKey
 * Description   : This function updates an internal key per the SHE
 * specification.
 *
 * Implements    : CSEC_DRV_LoadKey_Activity
 * END**************************************************************************/
status_t CSEC_DRV_LoadKey(csec_key_id_t keyId,
                                   const uint8_t * m1,
                                   const uint8_t * m2,
                                   const uint8_t * m3,
                                   uint8_t * m4,
                                   uint8_t * m5)
{
    status_t stat;

    DEV_ASSERT(m1 != NULL);
    DEV_ASSERT(m2 != NULL);
    DEV_ASSERT(m3 != NULL);
    DEV_ASSERT(m4 != NULL);
    DEV_ASSERT(m5 != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the values of M1-M3 */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, m1, CSEC_M1_SIZE_IN_BYTES);
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, m2, CSEC_M2_SIZE_IN_BYTES);
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_4_OFFSET, m3, CSEC_M3_SIZE_IN_BYTES);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_LOAD_KEY, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, keyId);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    /* Read the obtained M4 and M5 */
    if (stat == STATUS_SUCCESS)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_5_OFFSET, m4, CSEC_M4_SIZE_IN_BYTES);
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_7_OFFSET, m5, CSEC_M5_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_LoadPlainKey
 * Description   : Updates the RAM key memory slot with a 128-bit plaintext.
 *
 * Implements    : CSEC_DRV_LoadPlainKey_Activity
 * END**************************************************************************/
status_t CSEC_DRV_LoadPlainKey(const uint8_t * plainKey)
{
    DEV_ASSERT(plainKey != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the bytes of the key */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, plainKey, CSEC_PAGE_SIZE_IN_BYTES);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_LOAD_PLAIN_KEY, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_RAM_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ExportRAMKey
 * Description   : This function exports the RAM_KEY into a format protected by
 * SECRET_KEY.
 *
 * Implements    : CSEC_DRV_ExportRAMKey_Activity
 * END**************************************************************************/
status_t CSEC_DRV_ExportRAMKey(uint8_t * m1,
                                        uint8_t * m2,
                                        uint8_t * m3,
                                        uint8_t * m4,
                                        uint8_t * m5)
{
    DEV_ASSERT(m1 != NULL);
    DEV_ASSERT(m2 != NULL);
    DEV_ASSERT(m3 != NULL);
    DEV_ASSERT(m4 != NULL);
    DEV_ASSERT(m5 != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_EXPORT_RAM_KEY, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_RAM_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();
    if (stat == STATUS_SUCCESS)
    {
        /* Read the M1-M5 values associated with the key */
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, m1, CSEC_M1_SIZE_IN_BYTES);
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, m2, CSEC_M2_SIZE_IN_BYTES);
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_4_OFFSET, m3, CSEC_M3_SIZE_IN_BYTES);
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_5_OFFSET, m4, CSEC_M4_SIZE_IN_BYTES);
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_7_OFFSET, m5, CSEC_M5_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_InitRNG
 * Description   : The function initializes the seed and derives a key for the
 * PRNG. The function must be called before CMD_RND after every power cycle/reset.
 *
 * Implements    : CSEC_DRV_InitRNG_Activity
 * END**************************************************************************/
status_t CSEC_DRV_InitRNG()
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_INIT_RNG, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ExtendSeed
 * Description   : Extends the seed of the PRNG by compressing the former seed
 * value and the supplied entropy into a new seed. This new seed is then to be
 * used to generate a random number by invoking the CMD_RND command. The random
 * number generator must be initialized by CMD_INIT_RNG before the seed may be
 * extended.
 *
 * Implements    : CSEC_DRV_ExtendSeed_Activity
 * END**************************************************************************/
status_t CSEC_DRV_ExtendSeed(const uint8_t * entropy)
{
    DEV_ASSERT(entropy != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the entropy parameter */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, entropy, CSEC_PAGE_SIZE_IN_BYTES);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_EXTEND_SEED, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GenerateRND
 * Description   : The function returns a vector of 128 random bits. The random
 * number generator has to be initialized by calling CSEC_DRV_InitRNG before
 * random numbers can be supplied.
 *
 * Implements    : CSEC_DRV_GenerateRND_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GenerateRND(uint8_t * rnd)
{
    DEV_ASSERT(rnd != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_RND, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();
    /* Read the resulted random bytes */
    if (stat == STATUS_SUCCESS)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, rnd, CSEC_PAGE_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_BootFailure
 * Description   : The function is called during later stages of the boot
 * process to detect a failure.
 *
 * Implements    : CSEC_DRV_BootFailure_Activity
 * END**************************************************************************/
status_t CSEC_DRV_BootFailure()
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_BOOT_FAILURE, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_BootOK
 * Description   : The function is called during later stages of the boot
 * process to mark successful boot verification.
 *
 * Implements    : CSEC_DRV_BootOK_Activity
 * END**************************************************************************/
status_t CSEC_DRV_BootOK()
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_BOOT_OK, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_BootDefine
 * Description   : The function implements an extension of the SHE standard to
 * define both the user boot size and boot method.
 *
 * Implements    : CSEC_DRV_BootDefine_Activity
 * END**************************************************************************/
status_t CSEC_DRV_BootDefine(uint32_t bootSize,
                                      csec_boot_flavor_t bootFlavor)
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint8_t flavor = (uint8_t)bootFlavor;
    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the boot size and the boot flavor parameters */
    CSEC_WriteCommandWords(FEATURE_CSEC_BOOT_SIZE_OFFSET, &bootSize, 1U);
    CSEC_WriteCommandByte(FEATURE_CSEC_BOOT_FLAVOR_OFFSET, flavor);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_BOOT_DEFINE, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GetID
 * Description   : This function returns the identity (UID) and the value of the
 * status register protected by a MAC over a challenge and the data.
 *
 * Implements    : CSEC_DRV_GetID_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GetID(const uint8_t * challenge,
                                 uint8_t * uid,
                                 uint8_t * sreg,
                                 uint8_t * mac)
{
    DEV_ASSERT(challenge != NULL);
    DEV_ASSERT(uid != NULL);
    DEV_ASSERT(sreg != NULL);
    DEV_ASSERT(mac != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the challenge */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, challenge, CSEC_PAGE_SIZE_IN_BYTES);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_GET_ID, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();
    if (stat == STATUS_SUCCESS)
    {
        /* Read the UID */
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, uid, (uint8_t)(CSEC_PAGE_SIZE_IN_BYTES - 1U));
        /* Read the value of the SREG register */
        *sreg = CSEC_ReadCommandByte(FEATURE_CSEC_SREG_OFFSET);
        /* Read the MAC over the UID and the SREG */
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_3_OFFSET, mac, CSEC_PAGE_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DbgChal
 * Description   : This function obtains a random number which the user shall
 * use along with the MASTER_ECU_KEY and UID to return an authorization request.
 *
 * Implements    : CSEC_DRV_DbgChal_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DbgChal(uint8_t * challenge)
{
    DEV_ASSERT(challenge != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_DBG_CHAL, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();
    /* Read the challenge generated by the CSEc module */
    if (stat == STATUS_SUCCESS)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, challenge, CSEC_PAGE_SIZE_IN_BYTES);
    }

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DbgAuth
 * Description   : This function erases all keys (actual and outdated) stored in
 * NVM Memory if the authorization is confirmed by CSEc.
 *
 * Implements    : CSEC_DRV_DbgAuth_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DbgAuth(const uint8_t * authorization)
{
    DEV_ASSERT(authorization != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    status_t stat;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
    g_csecStatePtr->cmdInProgress = true;

    /* Write the authorization computed from the challenge */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, authorization, CSEC_PAGE_SIZE_IN_BYTES);
    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(CSEC_CMD_DBG_AUTH, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, CSEC_SECRET_KEY);

    /* Wait until the execution of the command is complete */
    CSEC_WaitCommandCompletion();

    /* Read the status of the execution */
    stat = CSEC_ReadErrorBits();

    g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_MPCompress
 * Description   : This function accesses a Miyaguchi-Prenell compression
 * feature within the CSEc feature set to compress the given messages.
 *
 * Implements    : CSEC_DRV_MPCompress_Activity
 * END**************************************************************************/
status_t CSEC_DRV_MPCompress(const uint8_t * msg,
                                      uint16_t msgLen,
                                      uint8_t * mpCompress,
                                      uint32_t timeout)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mpCompress != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    uint32_t startTime = 0;
    uint32_t crtTime = 0;
    status_t stat = STATUS_SUCCESS;
    uint16_t numPagesLeft = msgLen;

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }
	
	/* Initialize the internal state of the driver */
    CSEC_DRV_InitState(CSEC_SECRET_KEY, CSEC_CMD_MP_COMPRESS, msg, mpCompress, (uint32_t)msgLen << CSEC_BYTES_TO_FROM_PAGES_SHIFT);

    startTime = OSIF_GetMilliseconds();

    /* Loop and launch commands until the end of the message */
    while (numPagesLeft > 0U)
    {
        uint8_t numPages = (uint8_t)((numPagesLeft > CSEC_DATA_PAGES_AVAILABLE) ?
                                      CSEC_DATA_PAGES_AVAILABLE : numPagesLeft);
        uint8_t numBytes = (uint8_t)(numPages << CSEC_BYTES_TO_FROM_PAGES_SHIFT);

        /* Write the message */
        CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &msg[g_csecStatePtr->index], numBytes);
        /* Write the size of the message */
        CSEC_WriteCommandHalfWord(FEATURE_CSEC_PAGE_LENGTH_OFFSET, msgLen);
        /* Write the command header. This will trigger the command execution. */
        CSEC_WriteCommandHeader(CSEC_CMD_MP_COMPRESS, CSEC_FUNC_FORMAT_COPY, g_csecStatePtr->seq, CSEC_SECRET_KEY);

        /* Wait until the execution of the command is complete */
        CSEC_WaitCommandCompletion();

        crtTime = OSIF_GetMilliseconds();
        if (crtTime > (startTime + timeout))
        {
            CSEC_DRV_CancelCommand();

            stat = STATUS_TIMEOUT;
        }

        if (stat == STATUS_SUCCESS)
        {
            /* Read the status of the execution */
            stat = CSEC_ReadErrorBits();
        }

        if (stat != STATUS_SUCCESS)
        {
            break;
        }

        numPagesLeft = (uint16_t)(numPagesLeft - numPages);
        g_csecStatePtr->index = (uint32_t)(g_csecStatePtr->index + numBytes);
        if (g_csecStatePtr->seq == CSEC_CALL_SEQ_FIRST)
        {
            g_csecStatePtr->seq = CSEC_CALL_SEQ_SUBSEQUENT;
        }
    }

    /* Read the result of the compression */
    if (stat == STATUS_SUCCESS)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, mpCompress, CSEC_PAGE_SIZE_IN_BYTES);
    }
	
	g_csecStatePtr->cmdInProgress = false;

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_EncryptECBAsync
 * Description   : This function starts the AES-128 encryption in ECB mode of
 * the input plain text buffer, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_EncryptECBAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_EncryptECBAsync(csec_key_id_t keyId,
                                           const uint8_t * plainText,
                                           uint32_t length,
                                           uint8_t * cipherText)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_ENC_ECB, plainText, cipherText, length);

    CSEC_DRV_StartEncDecECBCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DecryptECBAsync
 * Description   : This function starts the AES-128 decryption in ECB mode of
 * the input cipher text buffer, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_DecryptECBAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DecryptECBAsync(csec_key_id_t keyId,
                                           const uint8_t * cipherText,
                                           uint32_t length,
                                           uint8_t * plainText)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_DEC_ECB, cipherText, plainText, length);

    CSEC_DRV_StartEncDecECBCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_EncryptCBCAsync
 * Description   : This function starts the AES-128 encryption in CBC mode of
 * the input cipher text buffer, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_EncryptCBCAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_EncryptCBCAsync(csec_key_id_t keyId,
                                           const uint8_t * plainText,
                                           uint32_t length,
                                           const uint8_t * iv,
                                           uint8_t * cipherText)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_ENC_CBC, plainText, cipherText, length);
    g_csecStatePtr->iv = iv;

    CSEC_DRV_StartEncDecCBCCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_DecryptCBCAsync
 * Description   : This function starts the AES-128 decryption in CBC mode of
 * the input cipher text buffer, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_DecryptCBCAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_DecryptCBCAsync(csec_key_id_t keyId,
                                           const uint8_t * cipherText,
                                           uint32_t length,
                                           const uint8_t * iv,
                                           uint8_t * plainText)
{
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_DEC_CBC, cipherText, plainText, length);
    g_csecStatePtr->iv = iv;

    CSEC_DRV_StartEncDecCBCCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GenerateMACAsync
 * Description   : This function starts the computation the MAC of a given
 * message using CMAC with AES-128, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_GenerateMACAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GenerateMACAsync(csec_key_id_t keyId,
                                            const uint8_t * msg,
                                            uint32_t msgLen,
                                            uint8_t * cmac)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(cmac != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_GENERATE_MAC, msg, cmac, CSEC_DRV_RoundTo(msgLen, 0x8) >> CSEC_BYTES_TO_FROM_BITS_SHIFT);
    g_csecStatePtr->msgLen = msgLen;

    CSEC_DRV_StartGenMACCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_VerifyMACAsync
 * Description   : This function starts the verification the MAC of a given
 * message using CMAC with AES-128, in an asynchronous manner.
 *
 * Implements    : CSEC_DRV_VerifyMACAsync_Activity
 * END**************************************************************************/
status_t CSEC_DRV_VerifyMACAsync(csec_key_id_t keyId,
                                          const uint8_t * msg,
                                          uint32_t msgLen,
                                          const uint8_t * mac,
                                          uint16_t macLen,
                                          bool * verifStatus)
{
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mac != NULL);
    DEV_ASSERT(verifStatus != NULL);
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    CSEC_DRV_InitState(keyId, CSEC_CMD_VERIFY_MAC, msg, NULL, CSEC_DRV_RoundTo(msgLen, 0x8) >> CSEC_BYTES_TO_FROM_BITS_SHIFT);
    g_csecStatePtr->msgLen = msgLen;
    g_csecStatePtr->verifStatus = verifStatus;
    g_csecStatePtr->macWritten = false;
    g_csecStatePtr->mac = mac;
    g_csecStatePtr->macLen = macLen;

    CSEC_DRV_StartVerifMACCmd();

    /* Enable interrupt */
    CSEC_SetInterrupt(true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_GetAsyncCmdStatus
 * Description   : This function checks the status of the execution of an
 * asynchronous command. If the command is still in progress, returns STATUS_BUSY.
 *
 * Implements    : CSEC_DRV_GetAsyncCmdStatus_Activity
 * END**************************************************************************/
status_t CSEC_DRV_GetAsyncCmdStatus()
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (!g_csecStatePtr->cmdInProgress)
    {
        return g_csecStatePtr->errCode;
    }

    return STATUS_BUSY;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_InitState
 * Description   : Initializes the internal state of the driver with a given
 * input buffer, output buffer, key, length, command, and marks the command as
 * in progress.
 *
 * END**************************************************************************/
static void CSEC_DRV_InitState(csec_key_id_t keyId,
                               csec_cmd_t cmd,
                               const uint8_t * inBuff,
                               uint8_t * outBuff,
                               uint32_t length)
{
    g_csecStatePtr->cmdInProgress = true;
    g_csecStatePtr->cmd = cmd;
    g_csecStatePtr->inputBuff = inBuff;
    g_csecStatePtr->outputBuff = outBuff;
    g_csecStatePtr->keyId = keyId;
    g_csecStatePtr->fullSize = length;
    g_csecStatePtr->index = 0U;
    g_csecStatePtr->errCode = STATUS_SUCCESS;
    g_csecStatePtr->seq = CSEC_CALL_SEQ_FIRST;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTFC_IRQHandler
 * Description   : Implementation of the FTFC interrupt handler. Handles completed
 * commands lauched by using the asynchronous CSEc driver API which were
 *
 * END**************************************************************************/
void FTFC_IRQHandler(void)
{
    uint8_t fstat = (uint8_t)(FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK);

    /* Previous command execution ended, continue execution */
    if ((fstat != 0U) && g_csecStatePtr->cmdInProgress)
    {
        switch (g_csecStatePtr->cmd)
        {
        case CSEC_CMD_ENC_ECB:
            /* Pass - through */
        case CSEC_CMD_DEC_ECB:
            CSEC_DRV_ContinueEncDecECBCmd();
            break;
        case CSEC_CMD_ENC_CBC:
            /* Pass - through */
        case CSEC_CMD_DEC_CBC:
            CSEC_DRV_ContinueEncDecCBCCmd();
            break;
        case CSEC_CMD_GENERATE_MAC:
            CSEC_DRV_ContinueGenMACCmd();
            break;
        case CSEC_CMD_VERIFY_MAC:
            CSEC_DRV_ContinueVerifMACCmd();
            break;
        default:
            /* Do nothing, all the asynchronous operations were checked above */
            break;
        }

        /* If finished processing, disable interrupt */
        if (!g_csecStatePtr->cmdInProgress)
        {
            CSEC_SetInterrupt(false);

            if (g_csecStatePtr->callback != NULL)
            {
                g_csecStatePtr->callback((uint32_t)g_csecStatePtr->cmd, g_csecStatePtr->callbackParam);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_StartEncDecECBCmd
 * Description   : Initializes the CSE_PRAM and the internal state for
 * encryption/decryption using ECB mode commands.
 *
 * END**************************************************************************/
static void CSEC_DRV_StartEncDecECBCmd(void)
{
    uint32_t numPagesLeft = (g_csecStatePtr->fullSize - g_csecStatePtr->index) >> CSEC_BYTES_TO_FROM_PAGES_SHIFT;
    uint16_t numPages = (uint16_t)((numPagesLeft > CSEC_DATA_PAGES_AVAILABLE) ? CSEC_DATA_PAGES_AVAILABLE : numPagesLeft);
    uint8_t numBytes = (uint8_t)(numPages << CSEC_BYTES_TO_FROM_PAGES_SHIFT);

    /* Write the plain/cipher text */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->inputBuff[g_csecStatePtr->index], numBytes);
    /* Write the size of the plain/cipher text (in pages) */
    CSEC_WriteCommandHalfWord(FEATURE_CSEC_PAGE_LENGTH_OFFSET, numPages);

    g_csecStatePtr->partSize = numBytes;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(g_csecStatePtr->cmd, CSEC_FUNC_FORMAT_COPY, g_csecStatePtr->seq, g_csecStatePtr->keyId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_StartEncDecCBCCmd
 * Description   : Initializes the CSE_PRAM and the internal state for
 * encryption/decryption using CBC mode commands.
 *
 * END**************************************************************************/
static void CSEC_DRV_StartEncDecCBCCmd()
{
    uint32_t numPagesLeft = (g_csecStatePtr->fullSize - g_csecStatePtr->index) >> CSEC_BYTES_TO_FROM_PAGES_SHIFT;
    uint16_t numPages = (uint16_t)((numPagesLeft > CSEC_DATA_PAGES_AVAILABLE) ? CSEC_DATA_PAGES_AVAILABLE : numPagesLeft);
    uint8_t numBytes = (uint8_t)(numPages << CSEC_BYTES_TO_FROM_PAGES_SHIFT);

    if (g_csecStatePtr->seq == CSEC_CALL_SEQ_FIRST)
    {
        numPages = (uint16_t)((numPagesLeft > (CSEC_DATA_PAGES_AVAILABLE - 1U)) ? (CSEC_DATA_PAGES_AVAILABLE - 1U) : numPagesLeft);
        numBytes = (uint8_t)(numPages << CSEC_BYTES_TO_FROM_PAGES_SHIFT);

        /* Write the initialization vector */
        CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, g_csecStatePtr->iv, CSEC_PAGE_SIZE_IN_BYTES);
        /* Write the plain/cipher text */
        CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, &g_csecStatePtr->inputBuff[g_csecStatePtr->index], numBytes);
    }
    else
    {
        /* Write the plain/cipher text */
        CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->inputBuff[g_csecStatePtr->index], numBytes);
    }

    /* Write the size of the plain/cipher text (in pages) */
    CSEC_WriteCommandHalfWord(FEATURE_CSEC_PAGE_LENGTH_OFFSET, (uint16_t)(g_csecStatePtr->fullSize >> CSEC_BYTES_TO_FROM_PAGES_SHIFT));

    g_csecStatePtr->partSize = numBytes;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(g_csecStatePtr->cmd, CSEC_FUNC_FORMAT_COPY, g_csecStatePtr->seq, g_csecStatePtr->keyId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_StartGenMACCmd
 * Description   : Initializes the CSE_PRAM and the internal state for the
 * CMAC generation command.
 *
 * END**************************************************************************/
static void CSEC_DRV_StartGenMACCmd()
{
    uint8_t numBytes = (uint8_t)(((g_csecStatePtr->fullSize - g_csecStatePtr->index) >
             CSEC_DATA_BYTES_AVAILABLE) ? CSEC_DATA_BYTES_AVAILABLE : (g_csecStatePtr->fullSize - g_csecStatePtr->index));

    /* Write the plain/cipher text */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->inputBuff[g_csecStatePtr->index], numBytes);
    /* Write the size of the message (in bits) */
    CSEC_WriteCommandWords(FEATURE_CSEC_MESSAGE_LENGTH_OFFSET, &g_csecStatePtr->msgLen, 1U);

    g_csecStatePtr->partSize = numBytes;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(g_csecStatePtr->cmd, CSEC_FUNC_FORMAT_COPY, g_csecStatePtr->seq, g_csecStatePtr->keyId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_StartVerifMACCmd
 * Description   : Initializes the CSE_PRAM and the internal state for the
 * CMAC verification command.
 *
 * END**************************************************************************/
static void CSEC_DRV_StartVerifMACCmd()
{
    uint8_t numBytes = (uint8_t)(((g_csecStatePtr->fullSize - g_csecStatePtr->index) >
             CSEC_DATA_BYTES_AVAILABLE) ? CSEC_DATA_BYTES_AVAILABLE : (g_csecStatePtr->fullSize - g_csecStatePtr->index));
    uint8_t macOffset = (uint8_t)CSEC_DRV_RoundTo(numBytes, 0x10);

    /* Write the plain/cipher text */
    CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->inputBuff[g_csecStatePtr->index], numBytes);
    /* Write the size of the message (in bits) */
    CSEC_WriteCommandWords(FEATURE_CSEC_MESSAGE_LENGTH_OFFSET, &g_csecStatePtr->msgLen, 1U);

    /* Write the number of bits of the MAC to be compared */
    CSEC_WriteCommandHalfWord(FEATURE_CSEC_MAC_LENGTH_OFFSET, (uint16_t)g_csecStatePtr->macLen);

    /* If there is available space in CSE_PRAM, write the MAC to be verified */
    if ((macOffset + CSEC_PAGE_SIZE_IN_BYTES) < CSEC_DATA_BYTES_AVAILABLE)
    {
        CSEC_WriteCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET + macOffset, g_csecStatePtr->mac, CSEC_PAGE_SIZE_IN_BYTES);
        g_csecStatePtr->macWritten = true;
    }

    g_csecStatePtr->partSize = numBytes;

    /* Write the command header. This will trigger the command execution. */
    CSEC_WriteCommandHeader(g_csecStatePtr->cmd, CSEC_FUNC_FORMAT_COPY, g_csecStatePtr->seq, g_csecStatePtr->keyId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ContinueEncDecECBCmd
 * Description   : Continues the execution of encryption/decryption using ECB
 * mode. Reads the partial result, updates the internal state and launches
 * another command, if necessary.
 *
 * END**************************************************************************/
static void CSEC_DRV_ContinueEncDecECBCmd(void)
{
    /* Read the status of the execution */
    g_csecStatePtr->errCode = CSEC_ReadErrorBits();
    if (g_csecStatePtr->errCode != STATUS_SUCCESS)
    {
        /* Do not continue launching commands if an error occurred */
        g_csecStatePtr->cmdInProgress = false;
        return;
    }

    /* Get partial result */
    CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->outputBuff[g_csecStatePtr->index], (uint8_t)g_csecStatePtr->partSize);

    g_csecStatePtr->index += (uint8_t)g_csecStatePtr->partSize;

    /* Decide if more commands are needed */
    if (g_csecStatePtr->index >= g_csecStatePtr->fullSize)
    {
        g_csecStatePtr->cmdInProgress = false;
    }
    else
    {
        /* Continue launching commands */
        CSEC_DRV_StartEncDecECBCmd();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ContinueEncDecCBCCmd
 * Description   : Continues the execution of encryption/decryption using CBC
 * mode. Reads the partial result, updates the internal state and launches
 * another command, if necessary.
 *
 * END**************************************************************************/
static void CSEC_DRV_ContinueEncDecCBCCmd(void)
{
    /* Read the status of the execution */
    g_csecStatePtr->errCode = CSEC_ReadErrorBits();
    if (g_csecStatePtr->errCode != STATUS_SUCCESS)
    {
        /* Do not continue launching commands if an error occurred */
        g_csecStatePtr->cmdInProgress = false;
        return;
    }

    /* Get partial result */
    if (g_csecStatePtr->seq == CSEC_CALL_SEQ_FIRST)
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, &g_csecStatePtr->outputBuff[g_csecStatePtr->index],
                                  (uint8_t)g_csecStatePtr->partSize);
        g_csecStatePtr->seq = CSEC_CALL_SEQ_SUBSEQUENT;
    }
    else
    {
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_1_OFFSET, &g_csecStatePtr->outputBuff[g_csecStatePtr->index],
                                  (uint8_t)g_csecStatePtr->partSize);
    }

    g_csecStatePtr->index += (uint8_t)g_csecStatePtr->partSize;

    if (g_csecStatePtr->index >= g_csecStatePtr->fullSize)
    {
        g_csecStatePtr->cmdInProgress = false;
    }
    else
    {
        /* Continue launching commands */
        CSEC_DRV_StartEncDecCBCCmd();
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ContinueGenMACCmd
 * Description   : Continues the execution of CMAC generation. Reads the status
 * of the execution, updates the internal state and launches another command,
 * if necessary. If there is no need to launch another command, reads the
 * resulted CMAC.
 *
 * END**************************************************************************/
static void CSEC_DRV_ContinueGenMACCmd(void)
{
    /* Read the status of the execution */
    g_csecStatePtr->errCode = CSEC_ReadErrorBits();
    if (g_csecStatePtr->errCode != STATUS_SUCCESS)
    {
        /* Do not continue launching commands if an error occurred */
        g_csecStatePtr->cmdInProgress = false;
        return;
    }

    if (g_csecStatePtr->seq == CSEC_CALL_SEQ_FIRST)
    {
        g_csecStatePtr->seq = CSEC_CALL_SEQ_SUBSEQUENT;
    }

    g_csecStatePtr->index += (uint8_t)g_csecStatePtr->partSize;

    /* Decide if more commands are needed */
    if (g_csecStatePtr->index >= g_csecStatePtr->fullSize)
    {
        g_csecStatePtr->cmdInProgress = false;
        CSEC_ReadCommandBytes(FEATURE_CSEC_PAGE_2_OFFSET, g_csecStatePtr->outputBuff, CSEC_PAGE_SIZE_IN_BYTES);
    }
    else
    {
        /* Continue launching commands */
        CSEC_DRV_StartGenMACCmd();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_ContinueVerifMACCmd
 * Description   : Continues the execution of CMAC verification. Reads the status
 * of the execution, updates the internal state and launches another command,
 * if necessary. If there is no need to launch another command, reads the
 * verification status.
 *
 * END**************************************************************************/
static void CSEC_DRV_ContinueVerifMACCmd(void)
{
    /* Read the status of the execution */
    g_csecStatePtr->errCode = CSEC_ReadErrorBits();
    if (g_csecStatePtr->errCode != STATUS_SUCCESS)
    {
        /* Do not continue launching commands if an error occurred */
        g_csecStatePtr->cmdInProgress = false;
        return;
    }

    if (g_csecStatePtr->seq == CSEC_CALL_SEQ_FIRST)
    {
        g_csecStatePtr->seq = CSEC_CALL_SEQ_SUBSEQUENT;
    }

    g_csecStatePtr->index += (uint8_t)g_csecStatePtr->partSize;

    /* Decide if more commands are needed */
    g_csecStatePtr->cmdInProgress = !g_csecStatePtr->macWritten;

    if (!g_csecStatePtr->cmdInProgress)
    {
    	uint32_t verifWord = CSEC_ReadCommandWord(FEATURE_CSEC_VERIFICATION_STATUS_OFFSET);

    	*(g_csecStatePtr->verifStatus) = ((verifWord & CSEC_UPPER_HALF_MASK) == 0UL);
    }
    else
    {
        /* Continue launching commands */
        CSEC_DRV_StartVerifMACCmd();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_InstallCallback
 * Description   : Installs a callback function which will be invoked when an
 * asynchronous command finishes its execution.
 *
 * Implements    : CSEC_DRV_InstallCallback_Activity
 * END**************************************************************************/
void CSEC_DRV_InstallCallback(security_callback_t callbackFunc, void *callbackParam)
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    g_csecStatePtr->callback = callbackFunc;
    g_csecStatePtr->callbackParam = callbackParam;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CSEC_DRV_CancelCommand
 * Description   : Cancels a previously launched asynchronous command.
 *
 * Implements    : CSEC_DRV_CancelCommand_Activity
 * END**************************************************************************/
void CSEC_DRV_CancelCommand(void)
{
    DEV_ASSERT(g_csecStatePtr != NULL);

    if (g_csecStatePtr->cmdInProgress)
    {
        CSEC_SetInterrupt(false);

        /* Wait until the execution of the previous command is complete */
        CSEC_WaitCommandCompletion();

        if ((g_csecStatePtr->cmd != CSEC_CMD_ENC_ECB) && (g_csecStatePtr->cmd != CSEC_CMD_DEC_ECB))
        {
            /* Was there any command already launched? If so, break the sequence */
            if (g_csecStatePtr->fullSize != g_csecStatePtr->index)
            {
                /* Write the command header. CallSeq is set to 0 in order to trigger a command
                 * that will generate a sequence error, breaking the chain of calls. */
                CSEC_WriteCommandHeader(g_csecStatePtr->cmd, CSEC_FUNC_FORMAT_COPY, CSEC_CALL_SEQ_FIRST, g_csecStatePtr->keyId);

                /* Wait until the execution of the command is complete */
                CSEC_WaitCommandCompletion();
            }
        }

        g_csecStatePtr->cmdInProgress = false;
    }
}

/******************************************************************************
 * EOF
 *****************************************************************************/
