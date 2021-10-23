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

#include "helper_functions.h"
#include <stdint.h>
#include <stdbool.h>

/***********************************
 * @brief: Convert a float to null terminated char array
 * @param srcValue:  pointer to the source float value
 * @param destStr:   pointer to the destination string
 * @param maxLen:    maximum lenght of the string
 ***********************************/
void
floatToStr (const float *srcValue, char *destStr, uint8_t maxLen)
{
  uint8_t i, lessThanOne = 0;
  float tempVal = (*srcValue);
  uint8_t currentVal;

  if (tempVal < 0)
    {
      tempVal *= -1;
      *destStr = '-';
      destStr++;
    }
  for (i = 0; i < maxLen; i++)
    {
      currentVal = (uint8_t) (tempVal);
      *destStr = currentVal + 48;
      destStr++;
      tempVal -= currentVal;
      if ((tempVal < 1) && (lessThanOne == 0))
	{
	  *destStr = '.';
	  destStr++;
	  lessThanOne = 1;
	}
      tempVal *= 10;
    }
  *destStr = 0;
}

/***********************************
 * @brief: Wait for a number of cycles
 * @param nbOfCycles is number of cycles to be waited for
 ***********************************/
void
delayCycles (uint32_t nbOfCycles)
{
  volatile uint32_t i = nbOfCycles;
  while (i--)
    ;
}

/* Method that enables on-device FPU
 * param: 	None
 * return:	None
 */

void enableFPU(void)
{
	/* Enable FPU set both CPACR[CP11] and CPACR[CP10] to Full Access - 0b11 */
	S32_SCB->CPACR |= (S32_SCB_CPACR_CP10_MASK | S32_SCB_CPACR_CP11_MASK);
}
