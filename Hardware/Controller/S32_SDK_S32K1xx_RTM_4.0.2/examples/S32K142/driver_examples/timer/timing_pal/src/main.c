/*
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
/* ###################################################################
**     Filename    : main.c
**     Project     : timing_pal_s32k142
**     Processor   : s32k142
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "sdk_project_config.h"

volatile int exit_code = 0;
/* User includes */


/* This define configures led pin and port for use with EVB, to make it run on
 * other boards you need to comment the following line.
 */
#define EVB

#ifdef EVB
    #define LED_BLUE        0U      /* pin PTD0 - RGB LED on DEV-KIT */
    #define LED_RED         15U     /* pin PTD15 - RGB LED on DEV-KIT */
    #define LED_GREEN       16U     /* pin PTE16 - RGB LED on DEV-KIT */
    #define LED_GPIO_PORT   PTD     /* GPIO PORT D */
#else
    #define LED_BLUE        0U      /* pin PTC0 - LED0 on Mother board */
    #define LED_RED         1U      /* pin PTC1 - LED3 LED on Mother board */
    #define LED_GREEN       2U      /* pin PTC2 - LED2 LED on Mother board */
    #define LED_GPIO_PORT   PTC     /* GPIO PORT C */
#endif


/* LPIT channel used */
#define LPIT_CHANNEL        0UL
/* LPTMR channel used */
#define LPTMR_CHANNEL       0UL
/* FTM0 channel used */
#define FTM_CHANNEL         0UL
/* Period in nanosecond unit */
#define PERIOD_BY_NS        1000000000UL /* The period is 1 second */

/*!
 * @brief: The callback function of timing over LPIT channel 0.
 *         The callback function toggles LED_BLUE
 */
void timingLpitInst0_callback(void * userData)
{
    (void)userData;
    /* Toggle LED_BLUE */
    PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_BLUE));
}

/*!
 * @brief: The callback function of timing over LPTMR channel 0.
 *         The callback function toggles LED_RED
 */
void timingLptmrInst0_callback(void * userData)
{
    (void)userData;
    /* Toggle LED_RED */
    PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_RED));
}

/*!
 * @brief: The callback function of timing over FTM0 channel 0.
 *         The callback function toggles LED_GREEN
 */
void timingFtmInst0_callback(void * userData)
{
    (void)userData;
    /* Toggle LED_GREEN */
    PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED_GREEN));
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your local variable definition here */
    uint64_t lpitResolution;
    uint64_t lptmrResolution;
    uint64_t ftmResolution;
    status_t status;
    /* Initialize clock module */
    status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Initialize LEDs and Button configuration */
    status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Initialize TIMING over LPIT */
    status = TIMING_Init(&timingLpitInst0, &timingLpit_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Initialize TIMING over LPTMR */
    status = TIMING_Init(&timingLptmrInst0, &timingLptmr_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Initialize TIMING over FTM0 */
    status = TIMING_Init(&timingFtmInst0, &timingFtm_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Get tick resolution in nanosecond unit for TIMING over LPIT */
    status = TIMING_GetResolution(&timingLpitInst0, TIMER_RESOLUTION_TYPE_NANOSECOND, &lpitResolution);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Get tick resolution in nanosecond unit for TIMING over LPTMR */
    status = TIMING_GetResolution(&timingLptmrInst0, TIMER_RESOLUTION_TYPE_NANOSECOND, &lptmrResolution);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Get tick resolution in nanosecond unit for TIMING over FTM */
    status = TIMING_GetResolution(&timingFtmInst0, TIMER_RESOLUTION_TYPE_NANOSECOND, &ftmResolution);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Start LPIT channel 0 counting with the period is 1 second,
       the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
    TIMING_StartChannel(&timingLpitInst0, LPIT_CHANNEL, PERIOD_BY_NS/lpitResolution);
    /* Start LPTMR channel 0 counting with the period is 1 second,
       the period in tick = the period in nanosecond / LPTMR tick resolution nanosecond*/
    TIMING_StartChannel(&timingLptmrInst0, LPTMR_CHANNEL, PERIOD_BY_NS/lptmrResolution);
    /* Start FTM channel 0 counting with the period is 1 second,
       the period in tick = the period in nanosecond / FTM tick resolution nanosecond*/
    TIMING_StartChannel(&timingFtmInst0, FTM_CHANNEL, PERIOD_BY_NS/ftmResolution);

    /* Infinite loop */
    for(;;) {
        if(exit_code != 0) {
        break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
