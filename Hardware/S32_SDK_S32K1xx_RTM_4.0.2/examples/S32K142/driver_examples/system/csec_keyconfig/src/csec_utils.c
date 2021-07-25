/* 
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.                             
 * Copyright 2020 NXP                                                                    
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
/*
 * csec_utils.c
 *
 *  Created on: Nov 10, 2016
 *      Author: B50609
 */

#include <stdint.h>
#include <stdbool.h>

#include "csec_utils.h"

/* AuthId is the MASTER_ECU key */
uint8_t g_emptyKey[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t g_authIdKey[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
csec_key_id_t g_authId = CSEC_MASTER_ECU;

/* Constants defined by the SHE spec */
uint8_t key_update_enc_c[16] = {0x01, 0x01, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};
uint8_t key_update_mac_c[16] = {0x01, 0x02, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};
uint8_t key_debug_key_c[16] = {0x01, 0x03, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};

/* Derives a key with a given constant */
status_t deriveKey(const uint8_t *key, uint8_t *constant, uint8_t *derivedKey)
{
    uint8_t concat[32];
    int i;

    for (i = 0; i < 16; i++)
    {
        concat[i] = key[i];
        concat[i+16] = constant[i];
    }

    return CSEC_DRV_MPCompress(concat, 2U, derivedKey, 1U);
}

/* Computes the M1-M3 values */
status_t computeM1M2M3(uint8_t *authKey, csec_key_id_t authId, csec_key_id_t keyId, const uint8_t *key, uint32_t counter,
                                uint8_t *uid, uint8_t *m1, uint8_t *m2, uint8_t *m3)
{
    status_t stat;
    int i;
    uint8_t iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t k1[16];
    uint8_t k2[16];

    uint8_t m2Plain[32];

    uint8_t m1m2[48];

    /* Derive K1 and K2 from AuthID */
    deriveKey(authKey, key_update_enc_c, k1);
    deriveKey(authKey, key_update_mac_c, k2);

    /* Compute M1 = UID | ID | AuthID */
    for (i = 0; i < 15; i++)
    {
        m1[i] = uid[i];
    }
    m1[15] = ((keyId & 0xF) << 4) | (authId & 0xF);

    /* Compute M2 (C = counter, F = 0) */
    for(i = 0; i < 16; i++)
    {
        m2Plain[i] = 0;
        m2Plain[16 + i] = key[i];
    }
    m2Plain[0] = (counter & 0xFF00000) >> 20;
    m2Plain[1] = (counter & 0xFF000) >> 12;
    m2Plain[2] = (counter & 0xFF0) >> 4;
    m2Plain[3] = (counter & 0xF) << 4;

    /* Encrypt M2 */
    stat = CSEC_DRV_LoadPlainKey(k1);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = CSEC_DRV_EncryptCBC(CSEC_RAM_KEY, m2Plain, 32U, iv, m2, 1U);
    if (stat != STATUS_SUCCESS)
        return stat;

    /* Compute M3 as CMAC(key=k2, m1|m2)*/
    for (i = 0; i < 16; i++)
    {
        m1m2[i] = m1[i];
    }
    for(i = 0; i < 32; i++)
    {
        m1m2[16 + i] = m2[i];
    }

    stat = CSEC_DRV_LoadPlainKey(k2);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = CSEC_DRV_GenerateMAC(CSEC_RAM_KEY, m1m2, 384U, m3, 1U);
    if (stat != STATUS_SUCCESS)
        return stat;

    return STATUS_SUCCESS;
}

/* Computes the M4 and M5 values */
status_t computeM4M5(csec_key_id_t authId, csec_key_id_t keyId, const uint8_t *key, uint32_t counter,
                                uint8_t *uid, uint8_t *m4, uint8_t *m5)
{
    status_t stat;
    int i;

    uint8_t k3[16];
    uint8_t k4[16];

    uint8_t m4StarPlain[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m4StarCipher[16];

    /* Derive K4 and K5 from key ID */
    deriveKey(key, key_update_enc_c, k3);
    deriveKey(key, key_update_mac_c, k4);

    m4StarPlain[0] = (counter & 0xFF00000) >> 20;
    m4StarPlain[1] = (counter & 0xFF000) >> 12;
    m4StarPlain[2] = (counter & 0xFF0) >> 4;
    m4StarPlain[3] = ((counter & 0xF) << 4) | 0x8;

    /* Encrypt M4* */
    stat = CSEC_DRV_LoadPlainKey(k3);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = CSEC_DRV_EncryptECB(CSEC_RAM_KEY, m4StarPlain, 16U, m4StarCipher, 1U);
    if (stat != STATUS_SUCCESS)
        return stat;

    /* Compute M4 = UID | ID | AuthID | M4* */
    for (i = 0; i < 15; i++)
    {
        m4[i] = uid[i];
    }
    m4[15] = ((keyId & 0xF) << 4) | (authId & 0xF);
    for (i = 0; i < 16; i++)
    {
        m4[16 + i] = m4StarCipher[i];
    }

    stat = CSEC_DRV_LoadPlainKey(k4);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = CSEC_DRV_GenerateMAC(CSEC_RAM_KEY, m4, 256U, m5, 1U);
    if (stat != STATUS_SUCCESS)
        return stat;

    return STATUS_SUCCESS;
}

/* Sets the AuthID key (MASTER_ECU_KEY) for the first time */
bool setAuthKey(void)
{
    uint8_t uid[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m1[16];
    uint8_t m2[32];
    uint8_t m3[16];
    uint8_t m4[32];
    uint8_t m5[16];

    status_t stat;

    stat = computeM1M2M3(g_emptyKey, g_authId, CSEC_MASTER_ECU, g_authIdKey, 1, uid, m1, m2, m3);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = CSEC_DRV_LoadKey(CSEC_MASTER_ECU, m1, m2, m3, m4, m5);
    if (stat != STATUS_SUCCESS)
        return false;

    return true;
}

/* Extracts the UID. */
bool getUID(uint8_t *uid)
{
    status_t stat;
    uint8_t challenge[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t sreg;
    uint8_t mac[16];
    uint8_t verif[32];
    bool verifStatus;
    uint8_t i;

    stat = CSEC_DRV_GetID(challenge, uid, &sreg, mac);
    if (stat != STATUS_SUCCESS)
        return false;

    for (i = 0; i < 16; i++) {
        verif[i] = challenge[i];
    }
    for (i = 0; i < 15; i++) {
        verif[16 + i] = uid[i];
    }
    verif[31] = CSEC_DRV_GetStatus() | CSEC_STATUS_BUSY;

    stat = CSEC_DRV_LoadPlainKey(g_authIdKey);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = CSEC_DRV_VerifyMAC(CSEC_RAM_KEY, verif, 256U, mac, 128U, &verifStatus, 1U);
    if (stat != STATUS_SUCCESS)
        return false;

    return verifStatus;
}

/* Erases all the keys. */
bool eraseKeys(void)
{
    status_t stat;
    uint8_t challenge[16];
    uint8_t auth[16];
    uint8_t authPlain[31];
    uint8_t k[16];
    uint8_t uid[15];

    uint8_t i;

    CSEC_DRV_InitRNG();
    getUID(uid);

    deriveKey(g_authIdKey, key_debug_key_c, k);

    stat = CSEC_DRV_LoadPlainKey(k);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = CSEC_DRV_DbgChal(challenge);
    if (stat != STATUS_SUCCESS)
        return false;

    for (i = 0; i < 16; i++)
    {
        authPlain[i] = challenge[i];
    }
    for (i = 0; i < 15; i++)
    {
        authPlain[i + 16] = uid[i];
    }

    stat = CSEC_DRV_GenerateMAC(CSEC_RAM_KEY, authPlain, 248U, auth, 1U);
    if (stat != STATUS_SUCCESS)
        return false;

    CSEC_DRV_DbgAuth(auth);

    return true;
}

/* Loads/updates a non-volatile key. */
bool loadKey(csec_key_id_t keyId, uint8_t *keyNew, uint8_t counter)
{
    uint8_t uid[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m1[16];
    uint8_t m2[32];
    uint8_t m3[16];
    uint8_t m4[32];
    uint8_t m5[16];

    status_t stat;

    stat = computeM1M2M3(g_authIdKey, g_authId, keyId, keyNew, counter, uid, m1, m2, m3);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = CSEC_DRV_LoadKey(keyId, m1, m2, m3, m4, m5);
    if (stat != STATUS_SUCCESS)
        return false;

    return true;
}
