/* 
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.                             
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

#ifndef SOURCES_HELPER_FUNCTIONS_H_
#define SOURCES_HELPER_FUNCTIONS_H_

#include "device_registers.h"
#include <stdint.h>

/***********************************
 * @brief: Convert a float to null terminated char array
 * @param srcValue:  pointer to the source float value
 * @param destStr:   pointer to the destination string
 * @param maxLen:    maximum lenght of the string
 ***********************************/
void floatToStr(const float *srcValue, char *destStr, uint8_t maxLen);

/***********************************
 * @brief: Wait for a number of cycles
 * @param nbOfCycles is number of cycles to be waited for
 ***********************************/
void delayCycles(uint32_t nbOfCycles);

/* Method that enables on-device FPU
 * param: 	None
 * return:	None
 */

void enableFPU(void);

#endif /* SOURCES_HELPER_FUNCTIONS_H_ */
