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
**     Project     : lptmr_periodic_interrupt_s32k142
**     Processor   : S32K142
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
#include "interrupt_manager.h"

volatile int exit_code = 0;
/* User includes */


/* This define configures led pin and port for use with EVB
 */
#define LED_GPIO_PORT   PTD
#define LED0_PIN_INDEX  0U


/*!
 * @brief: LPTMR IRQ handler.
 */
void LPTMR0_IRQHandler(void)
{
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR_1);
    /* Toggle LED0 */
    PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED0_PIN_INDEX));
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
    status_t status;
    /* Initialize clock module */
    status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize LEDs and Button configuration */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Init LPTMR as timer
     *  -   interrupt after 1 second
     *  -   SIRC as clock source
     *  -   counter disabled
     */
    LPTMR_DRV_Init(INST_LPTMR_1, &lptmr_1_config0, false);
    /* Enable IRQ for LPTMR */
    INT_SYS_EnableIRQ(LPTMR0_IRQn);

    /* Start LPTMR counter */
    LPTMR_DRV_StartCounter(INST_LPTMR_1);

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
