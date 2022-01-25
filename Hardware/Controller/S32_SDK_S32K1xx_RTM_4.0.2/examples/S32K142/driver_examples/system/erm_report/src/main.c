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
**     Project     : erm_report_s32k142
**     Processor   : S32K142_100
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

#include <stdint.h>
#include <stdbool.h>

#define LED0            15U /* pin PTD15 - LED RED of LED RGB on DEV-KIT    */
#define LED1            16U /* pin PTD16 - LED GREEN of LED RGB on DEV-KIT  */
#define LED_GPIO        PTD /* LED GPIO type */
#define SW              12U /* pin PTC12 - SW2_BTN0 on DEV-KIT              */
#define SW_GPIO         PTC /* SW GPIO type                                 */

/*!
  \brief The ERM_ISR funcion invert state Led
  *- Clear event when have notify interrupt
*/
void ERM_ISR(void)
{
    /* Turn off GREEN LED */
    PINS_DRV_WritePin(LED_GPIO, LED1, 0u);
    /* Turn on RED LED */
    PINS_DRV_WritePin(LED_GPIO, LED0, 1u);
    /* The interrupt notification will be cleared.*/
    ERM_DRV_ClearEvent(INST_ERM_CONFIG_1, 0U, ERM_EVENT_SINGLE_BIT);
}

/* Declare global variables */
uint32_t test;
uint32_t addr;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Initialize address used to test */
    *(uint32_t *)0x1FFFFFF0U = 0U;

    /* Initialize and configure clocks
     *  -   Setup system clocks
     *  -   Enable clock feed for Ports and ERM and EIM
     *  -   See Clock Manager component for more info
     */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins
     *  -   Setup pins for EWM
     *  -   Setup output pins for LEDs
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Turn off RED LED */
    PINS_DRV_WritePin(LED_GPIO, LED0, 0u);
    /* Turn on GREEN LED */
    PINS_DRV_WritePin(LED_GPIO, LED1, 1u);

    for(;;)
    {

        if (PINS_DRV_ReadPins(SW_GPIO) & (1U << SW))
        {
            /* Install IRQ Handlers for ERM and SysTick */
            INT_SYS_InstallHandler(ERM_single_fault_IRQn, ERM_ISR, (isr_t *)0);
            /* Enable ERM IRQ */
            INT_SYS_EnableIRQ(ERM_single_fault_IRQn);

            /* Hardware initialization */
            /* Initial for ERM module */
            ERM_DRV_Init(INST_ERM_CONFIG_1, ERM_CHANNEL_COUNT0, erm_InitConfig0);
            /* Initial for EIM module */
            EIM_DRV_Init(INST_EIM_CONFIG_1, EIM_CHANNEL_COUNT0, eim_InitConfig0);

            /* Read any address on RAM  */
            /* Enable read region Ram (0x1FFF8000 - 0x20006FFF) when debug equal Flash */
            test = *(uint32_t *)0x1FFFFFF0U;
            (void)test;

            /* Deinit EIM module */
            EIM_DRV_Deinit(INST_EIM_CONFIG_1);

            /* Get address which EIM injection error */
            ERM_DRV_GetErrorDetail(INST_ERM_CONFIG_1, 0U, &addr);
            break;
        }

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
