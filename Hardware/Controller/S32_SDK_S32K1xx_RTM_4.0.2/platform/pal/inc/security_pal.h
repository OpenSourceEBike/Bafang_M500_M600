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

#ifndef SECURITY_PAL_H_
#define SECURITY_PAL_H_

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, global typedef is used for
 * debug purpose, even if it's not used directly in the PAL code.
 *
 */

#include "security_pal_cfg.h"
#include "security_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*! @file security_pal.h */

/*!
 * @addtogroup security_pal_driver Security PAL
 * @ingroup security_pal
 * @brief Security Peripheral Abstraction Layer
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Define instances for SECURITY PAL
 * Implements : security_instance_t_Class
 */
typedef enum
{
    SECURITY_INSTANCE0 = 0U
} security_instance_t;

/*!
 * @brief Defines the security keys
 * Implements : security_key_id_t_Class
 */
typedef enum
{
    SECURITY_SECRET_KEY = 0x0U,
    SECURITY_MASTER_ECU = 0x1U,
    SECURITY_BOOT_MAC_KEY = 0x2U,
    SECURITY_BOOT_MAC = 0x3U,
    SECURITY_KEY_1,
    SECURITY_KEY_2,
    SECURITY_KEY_3,
    SECURITY_KEY_4,
    SECURITY_KEY_5,
    SECURITY_KEY_6,
    SECURITY_KEY_7,
    SECURITY_KEY_8,
    SECURITY_KEY_9,
    SECURITY_KEY_10,
#if (defined(SECURITY_OVER_HSM))
	SECURITY_RAM_KEY = 0xEU,
#elif (defined(SECURITY_OVER_CSEC))
    SECURITY_RAM_KEY = 0xFU,
#endif
    SECURITY_KEY_11 = 0x14U,
    SECURITY_KEY_12,
    SECURITY_KEY_13,
    SECURITY_KEY_14,
    SECURITY_KEY_15,
    SECURITY_KEY_16,
	SECURITY_KEY_17,
#if (defined(SECURITY_OVER_HSM))	
	SECURITY_KEY_18,
    SECURITY_KEY_19,
    SECURITY_KEY_20
#endif
} security_key_id_t;

/*!
 * @brief Defines the security boot flavor
 * Implements : security_boot_flavor_t_Class
 */
typedef enum
{
    SECURITY_BOOT_STRICT = 0U,
    SECURITY_BOOT_SERIAL = 1U,
    SECURITY_BOOT_PARALLEL = 2U,
    SECURITY_BOOT_NOT_DEFINED = 3U
} security_boot_flavor_t;

/*!
 * @brief Defines the security command
 * Implements : security_cmd_t_Class
 */
typedef enum {
#if (defined(SECURITY_OVER_HSM))
    SECURITY_CMD_NONE = 0U,
#endif
    SECURITY_CMD_ENC_ECB = 1U,
    SECURITY_CMD_ENC_CBC,
    SECURITY_CMD_DEC_ECB,
    SECURITY_CMD_DEC_CBC,
    SECURITY_CMD_GENERATE_MAC,
    SECURITY_CMD_VERIFY_MAC,
    SECURITY_CMD_LOAD_KEY,
    SECURITY_CMD_LOAD_PLAIN_KEY,
    SECURITY_CMD_EXPORT_RAM_KEY,
    SECURITY_CMD_INIT_RNG,
    SECURITY_CMD_EXTEND_SEED,
    SECURITY_CMD_RND,
#if (defined(SECURITY_OVER_CSEC))
    SECURITY_CMD_RESERVED_1,
#elif (defined(SECURITY_OVER_HSM))	
    SECURITY_CMD_SECURE_BOOT,
#endif
    SECURITY_CMD_BOOT_FAILURE,
    SECURITY_CMD_BOOT_OK,
    SECURITY_CMD_GET_ID,
#if (defined(SECURITY_OVER_CSEC))	
	SECURITY_CMD_BOOT_DEFINE,
#elif (defined(SECURITY_OVER_HSM))	
    SECURITY_CMD_BOOT_CANCEL,
#endif
    SECURITY_CMD_DBG_CHAL,
    SECURITY_CMD_DBG_AUTH,
#if (defined(SECURITY_OVER_CSEC))
    SECURITY_CMD_RESERVED_2,
    SECURITY_CMD_RESERVED_3,	
    SECURITY_CMD_MP_COMPRESS
#elif (defined(SECURITY_OVER_HSM))
	SECURITY_CMD_TRNG_RND,
    SECURITY_CMD_GET_VER,
    SECURITY_CMD_CHANGE_TRNG_CLK_SOURCE
#endif
} security_cmd_t;

/*!
 * @brief Define user configuration
 * Implements : security_user_config_t_Class
 */
typedef struct
{
    security_callback_t callback;    /*!< The callback invoked when an asynchronous command is completed */
    void *callbackParam;             /*!< User parameter for the command completion callback */
} security_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the configuration structure
 *
 * This function initializes the configuration struct members to default values.
 *
 * @param[out] config configuration struct pointer
 */
void SECURITY_GetDefaultConfig(security_user_config_t *config);

/*!
 * @brief Initializes the SECURITY module
 *
 * This function initializes and enables the requested SECURITY instance.
 *
 * @param[in]  instance: security module instance
 * @param[in]  config: pointer to security module configuration structure
 * @return     Error or success status returned by API
 */
status_t SECURITY_Init(security_instance_t instance, const security_user_config_t *config);

/*!
 * @brief De-initializes the SECURITY module
 *
 * This function de-initializes the requested SECURITY instance.
 *
 * @param[in] instance: security module instance
 * @return    Error or success status returned by API
 */
status_t SECURITY_Deinit(security_instance_t instance);

/*!
 * @brief ECB Encryption
 *
 * Perform AES-128 encryption in ECB mode of the input plain text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation
 * @param[in]  plainText: Pointer to the plain text buffer
 * @param[in]  msgLen: Number of bytes of plain text message to be encrypted.
 *             It is multiple of 16 bytes.
 * @param[out] cipherText: Pointer to the cipher text buffer. The buffer
 *             shall have the same size as the plain text buffer.
 * @param[in]  timeout: Specifies the maximum time allowed for command 
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_EncryptEcbBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *plainText,
                                     uint32_t msgLen,
                                     uint8_t *cipherText,
                                     uint32_t timeout);

/*!
 * @brief ECB Decryption
 *
 * Perform AES-128 decryption in ECB mode of the input cipher text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation
 * @param[in]  cipherText: Pointer to the cipher text buffer.
 * @param[in]  msgLen: Number of bytes of plain text message to be encrypted.
 *             It is multiple of 16 bytes.
 * @param[out] plainText: Pointer to the plain text buffer. The buffer shall
 *             have the same size as the cipher text buffer.
 * @param[in]  timeout: Specifies the maximum time allowed for command 
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_DecryptEcbBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *cipherText,
                                     uint32_t msgLen,
                                     uint8_t *plainText,
                                     uint32_t timeout);

/*!
 * @brief CBC Decryption
 *
 * Perform AES-128 decryption in CBC mode of the input cipher text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation
 * @param[in]  plainText: Pointer to the plain text buffer. The buffer shall
 *             have the same size as the cipher text buffer.
 * @param[in]  msgLen: Number of bytes of plain text message to be encrypted.
 *             It is multiple of 16 bytes.
 * @param[in]  iv: Pointer to the initialization vector buffer.
 * @param[out] cipherText: Pointer to the cipher text buffer.
 * @param[in]  timeout: Specifies the maximum time allowed for command 
 *             completion, else STATUS_TIMEOUT is returned. 
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_EncryptCbcBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *plainText,
                                     uint32_t msgLen,
                                     const uint8_t *iv,
                                     uint8_t *cipherText,
                                     uint32_t timeout);

/*!
 * @brief CBC Decryption
 *
 * Perform AES-128 decryption in CBC mode of the input cipher text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation
 * @param[in]  cipherText: Pointer to the cipher text buffer.
 * @param[in]  msgLen: Number of bytes of plain text message to be encrypted.
 *             It is multiple of 16 bytes.
 * @param[in]  iv: Pointer to the initialization vector buffer.
 * @param[out] plainText: Pointer to the plain text buffer. The buffer shall
 *             have the same size as the cipher text buffer.
 * @param[in]  timeout: Specifies the maximum time allowed for command 
 *             completion, else STATUS_TIMEOUT is returned. 
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_DecryptCbcBlocking(security_instance_t instance,
                                     security_key_id_t keyId,
                                     const uint8_t *cipherText,
                                     uint32_t msgLen,
                                     const uint8_t* iv,
                                     uint8_t *plainText,
                                     uint32_t timeout);

/*!
 * @brief MAC Generation
 *
 * Calculates MAC of a given message using CMAC with AES-128.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation.
 * @param[in]  msg: Pointer to the message buffer.
 * @param[in]  msgLen: Number of bits of message on which CMAC will be computed.
 * @param[out] cmac: Pointer to the buffer containing the result of the
 *             CMAC computation.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @param[in]  timeout: Specifies the maximum time allowed for command 
 *             completion, else STATUS_TIMEOUT is returned. 
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_GenerateMacBlocking(security_instance_t instance,
                                      security_key_id_t keyId,
                                      const uint8_t *msg,
                                      uint64_t msgLen,
                                      uint8_t *cmac,
                                      uint32_t timeout);

/*!
 * @brief MAC Verification
 *
 * Verifies the MAC of a given message using CMAC with AES-128.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic operation.
 * @param[in]  msg: Pointer to the message buffer.
 * @param[in]  msgLen: Number of bits of message on which CMAC will be computed.
 * @param[in]  mac: Pointer to the buffer containing the CMAC to be verified.
 * @param[in]  macLen: Number of bits of the CMAC to be compared. A macLength
 *             value of zero indicates that all 128-bits are compared.
 * @param[out] verifStatus: Status of MAC verification command
 *             (true: verification operation passed,
 *             false: verification operation failed).
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_VerifyMacBlocking(security_instance_t instance,
                                    security_key_id_t keyId,
                                    const uint8_t *msg,
                                    uint64_t msgLen,
                                    const uint8_t *mac,
                                    uint16_t macLen,
                                    bool *verifStatus,
                                    uint32_t timeout);

/*!
 * @brief Load Key
 *
 * Updates an internal key per the SHE specification.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID of the key to be updated.
 * @param[in]  m1: Pointer to the 128-bit M1 message containing the UID,
 *             Key ID and Authentication Key ID.
 * @param[in]  m2: Pointer to the 256-bit M2 message contains the new
 *             security flags, counter and the key value all
 *             encrypted using a derived key generated from the
 *             Authentication Key.
 * @param[in]  m3: Pointer to the 128-bit M3 message is a MAC generated
 *             over messages M1 and M2.
 * @param[out] m4: Pointer to a 256 bits buffer where the computed M4
 *             parameter is stored.
 * @param[out] m5: Pointer to a 128 bits buffer where the computed M5
 *             parameter is stored.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters
 *             are valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_LoadKey(security_instance_t instance,
                          security_key_id_t keyId,
                          const uint8_t *m1,
                          const uint8_t *m2,
                          const uint8_t *m3,
                          uint8_t *m4,
                          uint8_t *m5,
                          uint32_t timeout);

/*!
 * @brief Load Plain Key
 *
 * Updates the RAM key memory slot with a 128-bit plaintext.
 *
 * @param[in]  instance: security module instance
 * @param[in]  plainKey: Pointer to the 128-bit buffer containing the
 *             key that needs to be copied in RAM_KEY slot.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */
status_t SECURITY_LoadPlainKey(security_instance_t instance,
                               const uint8_t *plainKey,
                               uint32_t timeout);

/*!
 * @brief Export RAM key
 *
 * Exports the RAM_KEY into a format protected by SECRET_KEY.
 *
 * @param[in]  instance: security module instance
 * @param[out] m1: Pointer to a buffer where the M1 parameter will be exported.
 * @param[out] m2: Pointer to a buffer where the M2 parameter will be exported.
 * @param[out] m3: Pointer to a buffer where the M3 parameter will be exported.
 * @param[out] m4: Pointer to a buffer where the M4 parameter will be exported.
 * @param[out] m5: Pointer to a buffer where the M5 parameter will be exported.
 * @param[in]  timeout: Specifies the maximum time allowed for command completion,
 *             else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters are valid
 *             if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_ExportRamKey(security_instance_t instance,
                               uint8_t *m1,
                               uint8_t *m2,
                               uint8_t *m3,
                               uint8_t *m4,
                               uint8_t *m5,
                               uint32_t timeout);							   

/*!
 * @brief Initialize Random Number Generator
 *
 * Extends the seed of the PRNG by compressing the former seed value and the
 * supplied entropy into a new seed. This new seed is then to be used to
 * generate a random number by invoking the CMD_RND command. The random number
 * generator must be initialized by CMD_INIT_RNG before the seed may be
 * extended.
 *
 * @param[in]  instance: security module instance
 * @param[in]  entropy: pointer to a 128-bit buffer containing the entropy.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */
status_t SECURITY_ExtendSeed(security_instance_t instance,
                             const uint8_t *entropy,
                             uint32_t timeout);
							 
/*!
 * @brief Initialize Random Number Generator
 *
 * Initializes the seed and derive a key for the PRNG.
 *
 * @param[in]  instance: security module instance
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */	
status_t SECURITY_InitRng(security_instance_t instance,
                          uint32_t timeout);

/*!
 * @brief Generate RND
 *
 * Generates a vector of 128 random bits.
 *
 * @param[in]  instance: security module instance
 * @param[out] rnd: Pointer to a 128-bit buffer where the
 *             generated random number has to be stored.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters
 *             are valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_GenerateRnd(security_instance_t instance,
                              uint8_t *rnd,
                              uint32_t timeout);

/*!
 * @brief Get ID
 *
 * Returns the identity (UID) and the value of the status register
 * protected by a MAC over a challenge and the data.
 *
 * @param[in]  instance: security module instance
 * @param[in]  challenge: Pointer to the 128-bit buffer containing
 *             Challenge data.
 * @param[out] uid: Pointer to 120 bit buffer where the UID will be stored.
 * @param[out] sreg: Value of the status register.
 * @param[out] mac: Pointer to the 128 bit buffer where the MAC generated
 *             over challenge and UID and status  will be stored.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters are
 *             valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_GetId(security_instance_t instance,
                        const uint8_t *challenge,
                        uint8_t *uid,
                        uint8_t *sreg,
                        uint8_t *mac,
                        uint32_t timeout);

/*!
 * @brief Secure boot.
 *
 * The function loads the command processor firmware and memory slot data
 * and then executes the SHE secure boot protocol.
 *
 * @param[in] instance: security module instance
 * @param[in] bootImageSize Boot image size (in bytes).
 * @param[in] bootImagePtr Boot image start address.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t SECURITY_SecureBoot(security_instance_t instance,
                             uint32_t bootImageSize,
                             const uint8_t *bootImagePtr,
                             uint32_t timeout);

/*!
 * @brief Boot Failure
 *
 * Signals a failure detected during later stages of the boot process.
 *
 * @param[in]  instance: security module instance
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *                      completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */
status_t SECURITY_BootFailure(security_instance_t instance,
                              uint32_t timeout);

/*!
 * @brief Boot Ok
 *
 * Marks a successful boot verification during later stages of
 * the boot process.
 *
 * @param[in]  instance: security module instance
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */
status_t SECURITY_BootOk(security_instance_t instance,
                         uint32_t timeout);

/*!
 * @brief Boot Define
 *
 * Implements an extension of the SHE standard to define both the
 * user boot size and boot method.
 *
 * @param[in]  instance: security module instance
 * @param[in]  bootSize: Number of blocks of 128-bit data to check on
 *             boot. Maximum size is 512kBytes.
 * @param[in]  bootFlavor: The boot method.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Unsupported code
 *             if function is not available.
 */
status_t SECURITY_BootDefine(security_instance_t instance,
                             uint32_t bootSize,
                             security_boot_flavor_t bootFlavor,
                             uint32_t timeout);

/*!
 * @brief Debug Challenge
 *
 * Obtains a random number which the user shall use along with the
 * MASTER_ECU_KEY and UID to return an authorization request.
 *
 * @param[in]  instance: security module instance
 * @param[out] challenge: Pointer to the 128-bit buffer where the
 *             challenge data will be stored.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters
 *             are valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_DbgChal(security_instance_t instance,
                          uint8_t *challenge,
                          uint32_t timeout);

/*!
 * @brief Debug Authentification
 *
 * Erases all keys (actual and outdated) stored in NVM Memory if
 * the authorization is confirmed.
 *
 * @param[in]  instance: security module instance
 * @param[in]  authorization: Pointer to the 128-bit buffer containing
 *             the authorization value.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution.
 */
status_t SECURITY_DbgAuth(security_instance_t instance,
                          const uint8_t *authorization,
                          uint32_t timeout);

/*!
 * @brief Miyaguchi-Prenell Compression
 *
 * Compresses the given messages by accessing  the Miyaguchi-Prenell
 * compression feature with in the CSEc feature set.
 *
 * @param[in]  instance: security module instance
 * @param[in]  msg: Pointer to the messages to be compressed. Messages
 *             must be pre-processed per SHE specification if they
 *             do not already meet the full 128-bit block size
 *             requirement.
 * @param[in]  msgLen: The number of 128 bit messages to be compressed.
 * @param[out] mpCompress: Pointer to the 128 bit buffer storing the
 *             compressed data.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters
 *             are valid if the error code is STATUS_SUCCESS.
 */
status_t SECURITY_MPCompress(security_instance_t instance,
                             const uint8_t *msg,
                             uint32_t msgLen,
                             uint8_t *mpCompress,
                             uint32_t timeout);

/*!
 * @brief Generate True Random Number
 *
 * Generates a vector of 128 random bits using TRNG.
 *
 * @param[in]  instance: security module instance
 * @param[out] trnd: Pointer to a 128-bit buffer where the generated
 *             random number is stored.
 * @param[in]  timeout: Specifies the maximum time allowed for command
 *             completion, else STATUS_TIMEOUT is returned.
 * @return     Error Code after command execution. Output parameters
 *             are valid if the error code is STATUS_SUCCESS. 
 *             Unsupported code if function is not available.
 */
status_t SECURITY_GenerateTrnd(security_instance_t instance,
                               uint8_t *trnd,
                               uint32_t timeout);

/*!
 * @brief Cancel Command
 *
 * Cancels a previously initiated command.
 *
 * @param[in]  instance: security module instance
 * @return     STATUS_SUCCES
 */
status_t SECURITY_CancelCommand(security_instance_t instance);

/*!
 * @brief Get asynchronous command status
 *
 * Checks the status of the execution of an asynchronous command.
 *
 * @param[in]  instance: security module instance
 * @return     Error Code after command execution;
 *             STATUS_BUSY if a command is still in progress.
 */
status_t SECURITY_GetAsyncCmdStatus(security_instance_t instance);

/*!
 * @brief Encrypt ECB
 *
 * Asynchronously performs the AES-128 encryption in ECB mode of
 * the input plain text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  plainText: Pointer to the plain text buffer.
 * @param[in]  msgLen: Number of bytes of plain text message
 *             to be encrypted. It should be multiple of 16 bytes.
 * @param[out] cipherText: Pointer to the cipher text buffer. The
 *             buffer shall have the same size as the plain text
 *             buffer.
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_EncryptEcb(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *plainText,
                             uint32_t msgLen,
                             uint8_t *cipherText);

/*!
 * @brief Decrypt ECB
 *
 * Asynchronously performs the AES-128 decryption in ECB mode
 * of the input cipher text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  cipherText: Pointer to the cipher text buffer.
 * @param[in]  msgLen: Number of bytes of cipher text message
 *             to be decrypted. It should be multiple of 16 bytes.
 * @param[out] plainText: Pointer to the plain text buffer. The
 *             buffer shall have the same size as the cipher text
 *             buffer.
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_DecryptEcb(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *cipherText,
                             uint32_t msgLen,
                             uint8_t *plainText);

/*!
 * @brief Encrypt CBC
 *
 * Asynchronously performs the AES-128 encryption in CBC mode of
 * the input plain text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  plainText: Pointer to the plain text buffer.
 * @param[in]  msgLen: Number of bytes of plain text message to
 *             be encrypted. It should be multiple of 16 bytes.
 * @param[in]  iv: Pointer to the initialization vector buffer.
 * @param[out] cipherText: Pointer to the cipher text buffer.
 *             The buffer shall have the same size as the plain
 *             text buffer.
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_EncryptCbc(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *plainText,
                             uint32_t msgLen,
                             const uint8_t *iv,
                             uint8_t *cipherText);

/*!
 * @brief Decrypt CBC
 *
 * Asynchronously performs the AES-128 decryption in CBC mode of
 * the input cipher text buffer.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  cipherText: Pointer to the cipher text buffer.
 * @param[in]  msgLen: Number of bytes of cipher text message to
 *             be decrypted. It should be multiple of 16 bytes.
 * @param[in]  iv: Pointer to the initialization vector buffer.
 * @param[out] plainText: Pointer to the plain text buffer. The
 *             buffer shall have the same size as the cipher
 *             text buffer.
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_DecryptCbc(security_instance_t instance,
                             security_key_id_t keyId,
                             const uint8_t *cipherText,
                             uint32_t msgLen,
                             const uint8_t* iv,
                             uint8_t *plainText);

/*!
 * @brief Generate MAC
 *
 * Asynchronously calculates the MAC of a given message using
 * CMAC with AES-128.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  msg: Pointer to the message buffer.
 * @param[in]  msgLen: Number of bits of message on which CMAC
 *             will be computed.
 * @param[out] cmac: Pointer to the buffer containing the result
 *             of the CMAC computation.
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_GenerateMac(security_instance_t instance,
                              security_key_id_t keyId,
                              const uint8_t *msg,
                              uint64_t msgLen,
                              uint8_t *cmac);

/*!
 * @brief Verify MAC
 *
 * Asynchronously verifies  the MAC of a given message using
 * CMAC with AES-128.
 *
 * @param[in]  instance: security module instance
 * @param[in]  keyId: KeyID used to perform the cryptographic
 *             operation.
 * @param[in]  msg: Pointer to the message buffer.
 * @param[in]  msgLen: Number of bits of message on which CMAC
 *             will be computed.
 * @param[in]  mac: Pointer to the buffer containing the CMAC
 *             to be verified.
 * @param[in]  macLen: Number of bits of the CMAC to be compared.
 *             A macLength value of zero indicates that all
 *             128-bits are compared.
 * @param[out] verifStatus: Status of MAC verification command
 *             (true: verification operation passed,
 *             false: verification operation failed).
 * @return     STATUS_BUSY if another command is in execution,
 *             otherwise STATUS_SUCCESS.
 */
status_t SECURITY_VerifyMac(security_instance_t instance,
                            security_key_id_t keyId,
                            const uint8_t *msg,
                            uint64_t msgLen,
                            const uint8_t *mac,
                            uint16_t macLen,
                            bool *verifStatus);

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* SECURITY_PAL_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
