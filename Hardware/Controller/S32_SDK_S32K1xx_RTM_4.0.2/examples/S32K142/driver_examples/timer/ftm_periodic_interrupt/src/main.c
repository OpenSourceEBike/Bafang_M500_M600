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
**     Project     : ftm_periodic_interrupt_s32k142w
**     Processor   : S32K142W
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
#include <stdio.h>

volatile int exit_code = 0;

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/
#define EVB

#ifdef EVB
    #define LED_PIN     15U         /* LED pin on PTD15 for the EVB Board */
    #define LED_PORT    PTD
#else
    #define LED_PIN     0U          /* LED pin PTC0 */
    #define LED_PORT    PTC
#endif


/* Flex Timer Instance interrupt service routine
 *  - it will toggle LED0 and clear the timer overflow flag of the FTM instance
 */
void FTM0_Ovf_Reload_IRQHandler(void)
{
    static uint32_t counter = 0;

    counter++;
    if (counter == 1000U)
    {
        PINS_DRV_TogglePins(LED_PORT, (1 << LED_PIN));
        counter = 0;
    }
    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC_1, (uint32_t)FTM_TIME_OVER_FLOW_FLAG);
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
    ftm_state_t state;

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize FTM */
    FTM_DRV_Init(INST_FLEXTIMER_MC_1, &flexTimer_mc_1_InitConfig_0, &state);

    /* Initialize counter */
    FTM_DRV_InitCounter(INST_FLEXTIMER_MC_1, &flexTimer_mc_1_TimerConfig_0);

    /* Start Counter */
    FTM_DRV_CounterStart(INST_FLEXTIMER_MC_1);

    /* Loop */
    for (;;)
    {
        /* Do no thing, just wait for the interrupt */
    }

    return exit_code;
}

/* END main */
