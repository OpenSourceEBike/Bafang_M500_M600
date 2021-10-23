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
 * csec_utils.h
 *
 *  Created on: Nov 10, 2016
 *      Author: B50609
 */

#ifndef SOURCES_CSEC_UTILS_H_
#define SOURCES_CSEC_UTILS_H_

#include "csec_driver.h"

/* This function sets the MASTER_ECU key with a key (g_authIdKey) defined in the
 * csec_utils.c file. This key will be used as an authorization secret for updating
 * user keys.
 * Setting the MASTER_ECU key will work only for the first time, in order to use
 * another value for the key, there are two options:
 * - erase the keys and then update g_authIdKey;
 * - use loadKey with counter > 1 and then update g_authIdKey;
 * */
bool setAuthKey(void);

/* This function erases all the key. After using it, the Flash needs to be partitioned
 * again.
 * */
bool eraseKeys(void);

/* This function loads/updates a non-volatile key.
 * When updating the key, the counter needs to be greater then the previous one.
 * */
bool loadKey(csec_key_id_t keyId, uint8_t *keyNew, uint8_t counter);

#endif /* SOURCES_CSEC_UTILS_H_ */
