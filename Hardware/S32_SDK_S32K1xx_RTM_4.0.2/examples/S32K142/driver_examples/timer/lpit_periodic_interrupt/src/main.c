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
**     Project     : lpit_periodic_interrupt_s32k142
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

/* This define configures led pin and port for use with EVB
 */
#define LED0            0U
#define LED_GPIO_PORT   PTD

/* LPIT channel used */
#define LPIT_CHANNEL        0UL
#define LPIT_Channel_IRQn   LPIT0_Ch0_IRQn

/*!
 * @brief: LPIT interrupt handler.
 *         When an interrupt occurs clear channel flag and toggle LED0
 */
void LPIT_ISR(void)
{
	if (LPIT_DRV_GetInterruptFlagTimerChannels(INST_LPIT_CONFIG_1,(1 << LPIT_CHANNEL)))
	{
		/* Clear LPIT channel flag */
		LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT_CONFIG_1, (1 << LPIT_CHANNEL));
		/* Toggle LED0 */
		PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED0));
	}
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{
    status_t status;
    /* Initialize clock */
    status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Initialize pins
     *  -   Setup pin used for LED(used as GPIO)
     */
    status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    DEV_ASSERT(status == STATUS_SUCCESS);
    /* Set LED pin direction as output */
    PINS_DRV_SetPinsDirection(LED_GPIO_PORT, (1 << LED0));
    /* Turn off LED0 */
    PINS_DRV_SetPins(LED_GPIO_PORT, (1 << LED0));
    /* Turn on LED0 */
    PINS_DRV_ClearPins(LED_GPIO_PORT, (1 << LED0));

    /* Initialize LPIT instance 0
     *  -   Reset and enable peripheral
     */
    LPIT_DRV_Init(INST_LPIT_CONFIG_1, &lpit1_InitConfig);
    /* Initialize LPIT channel 0 and configure it as a periodic counter
     * which is used to generate an interrupt every second.
     */
    status = LPIT_DRV_InitChannel(INST_LPIT_CONFIG_1, LPIT_CHANNEL, &lpit1_ChnConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Install LPIT_ISR as LPIT interrupt handler */
    INT_SYS_InstallHandler(LPIT_Channel_IRQn, &LPIT_ISR, NULL);

    /* Start LPIT0 channel 0 counter */
    LPIT_DRV_StartTimerChannels(INST_LPIT_CONFIG_1, (1 << LPIT_CHANNEL));

    /* Peripherals Initialization is complete, now the program will wait for
     * LPIT interrupt.
     */

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
