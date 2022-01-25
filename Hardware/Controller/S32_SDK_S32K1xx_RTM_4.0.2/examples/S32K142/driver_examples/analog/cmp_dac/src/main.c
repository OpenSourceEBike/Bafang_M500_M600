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


/* User includes */

#include <stdint.h>
#include <stdbool.h>
#include "interrupt_manager.h"

/* Including necessary configuration files. */
#include "sdk_project_config.h"

volatile int exit_code = 0;
/* User includes */


/* To use with other board than EVB please comment the following line */
#define EVB

#ifdef EVB
    #define LED0            15U
    #define LED1            16U
    #define LED_GPIO        PTD
#else
    #define LED0             0U
    #define LED1             1U
    #define LED_GPIO        PTC
#endif


/*! brief Comparator Interrupt Service routine
 * It will read the output flags and based on the result will light the
 * required LED:
 *  -   Green if Vin is greater than DAC voltage
 *  -   Red if Vin is lower than DAC voltage
 *  -   Both LEDs if both edges are detected
 */
void ComparatorISR(void)
{
    /* Variable used to store comparator output flags */
    cmp_output_trigger_t cmpOutputFlags;
    /* Get output flags */
    CMP_DRV_GetOutputFlags(INST_CMP_1, &cmpOutputFlags);

    switch(cmpOutputFlags)
    {
    /* Vin is greater than DAC voltage */
    case CMP_FALLING_EDGE:
        /* Light the RED led */
        PINS_DRV_ClearPins(LED_GPIO, (1 << LED0));
        /* Close the GREEN led */
        PINS_DRV_SetPins(LED_GPIO, (1 << LED1));
        break;
    /* Vin is lower than DAC voltage */
    case CMP_RISING_EDGE:
        /* Light the GREEN led */
        PINS_DRV_ClearPins(LED_GPIO, (1 << LED1));
        /* Close the RED led */
        PINS_DRV_SetPins(LED_GPIO, (1 << LED0));
        break;
    /* Case is unknown or special (both edges detected) */
    default:
        /* Light both LEDs */
        PINS_DRV_ClearPins(LED_GPIO, (1 << LED0) | (1 << LED1));
        break;
    }

    /* Clear output flags */
    CMP_DRV_ClearOutputFlags(INST_CMP_1);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Initialize and configure clocks
     *  -   Setup system clocks
     *  -   Enable clock feed for Ports and Comparator
     *  -   See Clock Manager component for more info
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);


    /* Initialize pins
     *  -   Setup input pins for Comparator
     *  -   Setup output pins for LEDs
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Setup GPIO pins
     *  -   LED0 and LED1 as output
     */
    PINS_DRV_SetPinsDirection(LED_GPIO, (1 << LED0)|(1 << LED1));

    /* Turn off both LEDs */
    PINS_DRV_SetPins(LED_GPIO, (1 << LED0)|(1 << LED1));

    /* Initialize Comparator
     *  -   Positive source from AIN0
     *  -   Negative source from internal DAC with half the reference voltage
     *  5V/2 = 2.5V(For EVB). For other boards please contact the boards reference
     *  manual.
     *  -   Output interrupt enabled for both edges
     */
    CMP_DRV_Init(INST_CMP_1, &cmp_InitConfig0);

    /* Install Comparator interrupt handler */
    INT_SYS_InstallHandler(CMP0_IRQn, &ComparatorISR, (isr_t *)0);

    /* Enable Comparator interrupt */
    INT_SYS_EnableIRQ(CMP0_IRQn);

    /* Initialization ends here. The rest of the example is based on the
     * Comparator Interrupt (ComparatorISR function).
     */

    /* Write your code here */
    /* For example: for(;;) { } */

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
