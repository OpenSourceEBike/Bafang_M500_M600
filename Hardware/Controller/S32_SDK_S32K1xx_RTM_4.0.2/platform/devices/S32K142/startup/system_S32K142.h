/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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


/*! @addtogroup soc_support_S32K142*/
/*! @{*/

/*!
 * @file system_S32K142.h
 * @brief Device specific configuration file for S32K142
 */

#ifndef SYSTEM_S32K142_H_
#define SYSTEM_S32K142_H_                        /**< Symbol preventing repeated inclusion */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * CPU Settings.
 *****************************************************************************/

/* Watchdog disable */
#ifndef DISABLE_WDOG
  #define DISABLE_WDOG                 1
#endif

/* Cache enablement  */
#ifndef ICACHE_ENABLE
#define ICACHE_ENABLE                  0
#endif

/* Value of the external crystal or oscillator clock frequency in Hz */
#ifndef CPU_XTAL_CLK_HZ
  #define CPU_XTAL_CLK_HZ                8000000u
#endif

/* Value of the fast internal oscillator clock frequency in Hz  */
#ifndef CPU_INT_FAST_CLK_HZ
  #define CPU_INT_FAST_CLK_HZ            48000000u
#endif

/* Default System clock value */
#ifndef DEFAULT_SYSTEM_CLOCK
 #define DEFAULT_SYSTEM_CLOCK            48000000u
#endif

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the SoC.
 *
 * This function disables the watchdog, enables FPU.
 * if the corresponding feature macro is enabled.
 * SystemInit is called from startup_device file.
 */
void SystemInit(void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 * This function must be called when user does not want to use clock manager component.
 * If clock manager is used, the CLOCK_SYS_GetFreq function must be used with CORE_CLOCK
 * parameter.
 *
 */
void SystemCoreClockUpdate(void);

/**
 * @brief Initiates a system reset.
 *
 * This function is used to initiate a system reset
 */
void SystemSoftwareReset(void);

#ifdef __cplusplus
}
#endif

/*! @}*/
#endif  /* #if !defined(SYSTEM_S32K142_H_) */
