/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

#include "rtc_driver.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
 * visible when an object or function with external linkage is defined.
 * Folder structure has only C source file for interrupt routine.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

void RTC_IRQHandler(void);
void RTC_Seconds_IRQHandler(void);
 
void RTC_IRQHandler(void)
{
    RTC_DRV_IRQHandler(0U);
}

void RTC_Seconds_IRQHandler(void)
{
    RTC_DRV_SecondsIRQHandler(0U);
}
