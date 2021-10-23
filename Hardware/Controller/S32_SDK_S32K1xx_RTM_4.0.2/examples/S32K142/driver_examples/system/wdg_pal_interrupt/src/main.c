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
**     Project     : wdg_pal_interrupt_s32k142
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
#include "interrupt_manager.h"
#include "sdk_project_config.h"

volatile int exit_code = 0;
/* User includes */

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define EVB

#ifdef EVB
    #define LED0            15U
    #define LED1            16U
    #define LED_GPIO        PTD

#else
    #define LED0            0U
    #define LED1            1U
    #define LED_GPIO        PTC
#endif

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
    /* Variable that stores the number of WDG PAL Refresh */
    static uint8_t s_refreshNumber = 0;
    /* After 30 watchdog pal refreshes, the interrupt will no longer reset the wdg pal counter */
    if(s_refreshNumber < 30)
    {
        /* Reset WDG PAL counter */
        WDG_Refresh(&wdg_pal_1_Instance);
        /* Toggle LED0 */
        PINS_DRV_TogglePins(LED_GPIO, (1 << LED0));
        /* Increment refresh number */
        s_refreshNumber++;
    }
}

/* WDG PAL IRQ handler */
void wdg_pal_Handler(void)
{
    /* Clear interrupt flag */
    WDG_ClearIntFlag(&wdg_pal_1_Instance);
    /* Turn off both LEDs */
    PINS_DRV_ClearPins(LED_GPIO, (1 << LED0) | (1 << LED1));
    /* Disable the SysTick timer */
    SysTick_Disable();
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

    /* Initialize and configure clocks
    *  -   Setup system clocks
    *  -   Enable clock feed for Ports
    *  -   See Clock Manager component for more info
    */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *  -   Setup output pins for LEDs
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Stop if the previous reset was caused by the Watchdog */
    if(POWER_SYS_GetResetSrcStatusCmd(RCM, RCM_WATCH_DOG) == true)
    {
        /* Turn on both LEDs */
        PINS_DRV_WritePin(LED_GPIO, LED0, 1u);
        PINS_DRV_WritePin(LED_GPIO, LED1, 1u);
    }
    else
    {
        /* Turn off both LEDs */
        PINS_DRV_WritePin(LED_GPIO, LED0, 0u);
        PINS_DRV_WritePin(LED_GPIO, LED1, 0u);

        /* Install IRQ handlers for WDOG and SysTick interrupts */
        INT_SYS_InstallHandler(WDOG_EWM_IRQn, wdg_pal_Handler, (isr_t *)0);
        INT_SYS_InstallHandler(SysTick_IRQn, SysTick_Handler, (isr_t *)0);

        /* Enable Watchdog IRQ */
        INT_SYS_EnableIRQ(WDOG_EWM_IRQn);

        /* Configure and enable SysTick */
        SysTick_Init();
        SysTick_Enable();

        /* Initialize WDG PAL */
        WDG_Init(&wdg_pal_1_Instance, &wdg_pal_1_Config0);
    }

    for(;;){
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