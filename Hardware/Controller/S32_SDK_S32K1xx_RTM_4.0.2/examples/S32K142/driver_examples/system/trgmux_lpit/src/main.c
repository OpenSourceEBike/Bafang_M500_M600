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
**     Project     : trgmux_lpit_s32k142
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
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include "interrupt_manager.h"
volatile int exit_code = 0;
/* LPIT channel used */
#define LPIT_CHANNEL        0UL
#define LPIT_Channel_IRQn   LPIT0_Ch0_IRQn
/* Led blink */
#define LED_PIN             15U
#define LED_GPIO            PTD


/*!
 * @brief: LPIT interrupt handler.
 *         When an interrupt occurs clear channel flag and toggle LED0
 */
void LPIT_ISR(void)
{
    /* Clear LPIT channel flag */
    LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT_CONFIG_1, (1 << LPIT_CHANNEL));
    /* Toggle LED0 */
    PINS_DRV_TogglePins(LED_GPIO, (1 << LED_PIN));
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your code here */

    CLOCK_DRV_Init(&clockMan1_InitConfig0);
    /* Initialize pins for LED and Button*/
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Clear output logic on selected pin */
    PINS_DRV_ClearPins(LED_GPIO, (uint32_t) (1 << LED_PIN));
    /* Initialize TRGMUX peripheral */
    /* Source input: TRGMUX_IN10 */
    /* Target module: LPIT Channel 0 */
   TRGMUX_DRV_Init(INST_TRGMUX, &trgmux1_InitConfig0);

    /* Initialize LPIT instance 0. Reset and enable peripheral */
    LPIT_DRV_Init(INST_LPIT_CONFIG_1, &lpit1_InitConfig);
    /* Initialize LPIT channel 0 to generate trigger for PDB via TRGMUX */
    LPIT_DRV_InitChannel(INST_LPIT_CONFIG_1, LPIT_CHANNEL, &lpit1_ChnConfig0);
    /* Install LPIT_ISR as LPIT interrupt handler */
    INT_SYS_InstallHandler(LPIT_Channel_IRQn, &LPIT_ISR, (isr_t *)0);
    /* Start LPIT0 channel 0 counter */
    LPIT_DRV_StartTimerChannels(INST_LPIT_CONFIG_1, (1U << LPIT_CHANNEL));

    /* Wait for trigger signal when press button */
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
