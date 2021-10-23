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
**     Project     : ewm_interrupt_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-18, 14:05, # CodeGen: 0
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


/* Including necessary configuration files. */
#include "interrupt_manager.h"
#include "sdk_project_config.h"

volatile int exit_code = 0;

/* Initialize SysTick counter */
void SysTick_Init(void)
{
	S32_SysTick->RVR = 0xFFFFFFul;
	S32_SysTick->CVR = 0ul;
	S32_SysTick->CSR = 0u;
}

/* Enable SysTick counter and interrupt */
void SysTick_Enable(void)
{
	S32_SysTick->CSR = S32_SysTick_CSR_TICKINT(1u) | S32_SysTick_CSR_ENABLE(1);
}

/* Disable SysTick */
void SysTick_Disable(void)
{
	S32_SysTick->CSR = 0ul;
}

/* SysTick IRQ handler */
void SysTick_Handler(void)
{
	/* Variable that stores the number of EWM refreshes */
	static uint8_t s_refreshNumber = 0;
	/* After 30 watchdog refreshes, the interrupt will no longer reset the wdog counter */
	if(s_refreshNumber < 30)
	{
		/* Reset Watchdog counter */
		EWM_DRV_Refresh(EWM_CONFIG_1);
		/* Toggle LED_RED */
		PINS_DRV_TogglePins(LED_RED_PORT, (1 << LED_RED_PIN));
		/* Increment refresh number */
		s_refreshNumber++;
	}
}

/* EWM IRQ handler */
void EWM_ISR(void)
{
    /* Turn both LEDs on */
    PINS_DRV_WritePin(LED_RED_PORT, LED_RED_PIN, 1u);
    PINS_DRV_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 1u);
	/* Disable the SysTick timer */
	SysTick_Disable();
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
	/* Initialize and configure clocks
	 *  -   Setup system clocks
	 *  -   Enable clock feed for Ports and EWM
	 *  -   See Clock Manager component for more info
	 */
	CLOCK_DRV_Init(&clockMan1_InitConfig0);


	/* Initialize pins
	 *  -   Setup pins for EWM
	 *  -   Setup output pins for LEDs
	 *  -   See PinSettings component for more info
	 */
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);


	/* Turn both LEDs off */
	PINS_DRV_WritePin(LED_RED_PORT, LED_RED_PIN, 0u);
	PINS_DRV_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 0u);

	/* Install IRQ Handlers for EWM and SysTick */
	INT_SYS_InstallHandler(WDOG_EWM_IRQn, EWM_ISR, (isr_t *)0);
	INT_SYS_InstallHandler(SysTick_IRQn, SysTick_Handler, (isr_t *)0);

	/* Enable EWM IRQ */
	INT_SYS_EnableIRQ(WDOG_EWM_IRQn);

	/* Initialize and enable SysTick */
	SysTick_Init();
	SysTick_Enable();

	/* Initialize EWM */
	EWM_DRV_Init(EWM_CONFIG_1, &ewm_Config0);

	while(1)
	{
		/*do nothing*/
	}

    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
