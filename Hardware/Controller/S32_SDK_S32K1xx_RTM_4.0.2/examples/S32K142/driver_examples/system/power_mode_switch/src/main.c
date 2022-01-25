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
**     Project     : power_mode_switch_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-18, 11:21, # CodeGen: 0
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

#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MENU_MESSAGE "Press:\r\n\
\t1) for HSRUN\r\n\
\t2) for RUN\r\n\
\t3) for VLPR\r\n\
\t4) for STOP1\r\n\
\t5) for STOP2\r\n\
\t6) for VLPS\r\n\
-->Press SW3 to wake up the CPU from STOP1,STOP2 or VLPS mode\r\n\
Enter your input:\r\n"

#define LED_PORT  (PTD)
#define LED_0     (15u)
#define LED_1     (16u)
#define ON 1
#define OFF 0

#define SEPARATOR "----------------------------------------------------------------------------\r\n"

/* Definition of power modes indexes, as configured in Power Manager Component
 *  Refer to the Reference Manual for details about the power modes.
 */
#define HSRUN (0u) /* High speed run      */
#define RUN   (1u) /* Run                 */
#define VLPR  (2u) /* Very low power run  */
#define STOP1 (3u) /* Stop option 1       */
#define STOP2 (4u) /* Stop option 2       */
#define VLPS  (5u) /* Very low power stop */


/* brief: Send a string to user via LPUART
 * parameter sourceStr: pointer to the array of characters
 *                  that you wish to send.
 * return:          None
 */
void print(const char *sourceStr)
{
    uint32_t bytesRemaining;

    /* Send data via LPUART */
    LPUART_DRV_SendData(INST_LPUART1, (uint8_t *) sourceStr, strlen(sourceStr));
    /* Wait for transmission to be successful */
    while (LPUART_DRV_GetTransmitStatus(INST_LPUART1, &bytesRemaining) != STATUS_SUCCESS){};
}

/* Function that reads the option chosen by the user */
void readOptionFromConsole(uint8_t * option)
{
    uint32_t bytesRemaining;
    bool strReceived = false;
    uint8_t i = 0;
    uint8_t buffer[255];

    while(strReceived == false)
    {
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LPUART_DRV_ReceiveData(INST_LPUART1, &buffer[i], 1UL);
        /* Wait for transfer to be completed */
        while(LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining) != STATUS_SUCCESS){};
            strReceived = true;
    }

    /* Check for invalid options */
    if (buffer[0] < '1' || buffer[0] > '6')
    {
        print((const char *)"Invalid option!\r\n");
        *option = 0;
    }
    else
    {
        *option = buffer[0];
    }
}

/* Interrupt service routine for SW3 */
void ButtonISR(void)
{
    /* Clear button IRQ flag */
    PINS_DRV_ClearPortIntFlagCmd(PORTC);
    /* Board EVB - red led off */
    PINS_DRV_WritePin(LED_PORT, LED_0, OFF);
    /* Board EVB - green led on */
    PINS_DRV_WritePin(LED_PORT, LED_1, ON);
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
    /* Variable used for storing user option */
    uint8_t option;
    /* Variable used for storing core frequency */
    uint32_t frequency;
    /* Variable used for status of changing mode */
    status_t retV = STATUS_SUCCESS;

    /* Buffer for sprintf operations */
    char buffer[20];

    /* Initialize and configure clocks
     *  -   see clock manager component for details
     */
    DEV_ASSERT(CLOCK_DRV_Init(&clockMan1_InitConfig0) == STATUS_SUCCESS);

    /* Initialize pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize Power Manager
     * -   See PowerSettings component for more info
     */
    DEV_ASSERT(POWER_SYS_Init(&powerConfigsArr, POWER_MANAGER_CONFIG_CNT, &powerStaticCallbacksConfigsArr, POWER_MANAGER_CALLBACK_CNT) == STATUS_SUCCESS);

    /* Initialize LPUART for host - board configurations */
    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

    /* Install hander for Button ISR */
    INT_SYS_InstallHandler(PORTC_IRQn, ButtonISR, (isr_t *)0);
    /* Enable button port IRQ */
    INT_SYS_EnableIRQ(PORTC_IRQn);

    /* Board EVB - red led off */
    PINS_DRV_WritePin(LED_PORT, LED_0, OFF);
    /* Board EVB - green led on */
    PINS_DRV_WritePin(LED_PORT, LED_1, ON);

    while(1)
    {
        /* Print menu */
        print((const char *)MENU_MESSAGE);
        /* Read user option */
        readOptionFromConsole(&option);

        /* If the option is valid configure the CPU power mode accordingly */
        switch (option)
        {
        /* HSRUN */
        case '1':
            /* Set power mode to HSRUN */
            retV = POWER_SYS_SetMode(HSRUN, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"************************ CPU is in HSRUN mode.\r\n");
                print((const char*)"************************ Core frequency: ");
                /* Get current core clock frequency */
                (void)CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);
                /* Transform the frequency from decimal to string */
                sprintf(buffer, "%ld", frequency);
                /* Print CPU frequency */
                print(buffer);
                print((const char*)"[Hz] \r\n");
            }
            else
            {
                print((const char*)"Switch HSRUN mode unsuccessfully\r\n");
            }
            break;
        /* RUN */
        case '2':
            /* Set power mode to RUN */
            retV = POWER_SYS_SetMode(RUN, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"************************ CPU is in RUN mode.\r\n");
                print((const char*)"************************ Core frequency: ");

                /* Get current core clock frequency */
                (void)CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);

                /* Transform the frequency from decimal to string */
                sprintf(buffer, "%ld", frequency);

                /* Print CPU frequency */
                print(buffer);
                print((const char*)"[Hz] \r\n");
            }
            else
            {
                print((const char*)"Switch RUN mode unsuccessfully\r\n");
            }
            break;
        /* VLPR */
        case '3':
            /* Set power mode to VLPR */
            retV = POWER_SYS_SetMode(VLPR, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"************************ CPU is in VLPR mode.\r\n");
                print((const char*)"************************ Core frequency: ");
                /* Get current core clock frequency */
                (void)CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);
                /* Transform the frequency from decimal to string */
                sprintf(buffer, "%ld", frequency);
                /* Print CPU frequency */
                print(buffer);
                print((const char*)"[Hz] \r\n");
            }
            else
            {
                print((const char*)"Switch VLPR mode unsuccessfully\r\n");
            }
            break;
        /* STOP1 */
        case '4':
            print((const char*)"******** CPU is going in STOP1 mode...\r\n");
            /* Board EVB - red led on */
            PINS_DRV_WritePin(LED_PORT, LED_0, ON);
            /* Board EVB - green led off */
            PINS_DRV_WritePin(LED_PORT, LED_1, OFF);
            /* Set power mode to STOP1 */
            retV = POWER_SYS_SetMode(STOP1, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"CPU was entered STOP1 mode successfully and then woke up to exit STOP1 mode.\r\n");
                print((const char*)"Current mode is RUN because STOP mode can only be switched from this mode.\r\n");
            }
            else
            {
                print((const char*)"Switch STOP1 mode unsuccessfully\r\n");
            }
            break;
        /* STOP2 */
        case '5':
            print((const char*)"******** CPU is going in STOP2 mode...\r\n");
            /* Board EVB - red led on */
            PINS_DRV_WritePin(LED_PORT, LED_0, ON);
            /* Board EVB - green led off */
            PINS_DRV_WritePin(LED_PORT, LED_1, OFF);
           /* Set power mode to STOP2 */
            retV = POWER_SYS_SetMode(STOP2, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"CPU was entered STOP2 mode successfully and then woke up to exit STOP2 mode.\r\n");
                print((const char*)"Current mode is RUN because STOP mode can only be switched from this mode.\r\n");
            }
            else
            {
                print((const char*)"Switch STOP2 mode unsuccessfully\r\n");
            }
            break;
        /* VLPS */
        case '6':
            print((const char*)"******** CPU is going in VLPS mode...\r\n");
            /* Board EVB - red led on */
            PINS_DRV_WritePin(LED_PORT, LED_0, ON);
            /* Board EVB - green led off */
            PINS_DRV_WritePin(LED_PORT, LED_1, OFF);
            /* Set power mode to VLPS */
            retV = POWER_SYS_SetMode(VLPS, POWER_MANAGER_POLICY_AGREEMENT);
            if (retV == STATUS_SUCCESS)
            {
                print((const char*)"CPU was entered VLPS mode successfully and then woke up to exit VLPS mode.\r\n");
                if (POWER_SYS_GetCurrentMode() == POWER_MANAGER_RUN)
                {
                    print((const char*)"Current mode is RUN mode.\r\nClock source is remained in SIRC (8 MHz) before MCU switches from RUN to VLP mode.\r\nIn order to set to default clock, presses option RUN mode or re-initialize clock configuration.\r\n");
                    DEV_ASSERT(CLOCK_DRV_Init(&clockMan1_InitConfig0) == STATUS_SUCCESS);
                    print((const char*)"************************ Core frequency after re-initialized clock: ");
                    (void)CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);
                    /* Transform the frequency from decimal to string */
                    sprintf(buffer, "%ld", frequency);
                    /* Print CPU frequency */
                    print(buffer);
                    print((const char*)"[Hz] \r\n");
                }
                else
                {
                    print((const char*)"Current mode is VLPR mode.\r\n");
                    print((const char*)"************************ Core frequency: ");
                    (void)CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);
                    /* Transform the frequency from decimal to string */
                    sprintf(buffer, "%ld", frequency);
                    /* Print CPU frequency */
                    print(buffer);
                    print((const char*)"[Hz] \r\n");
                }
            }
            else
            {
                print((const char*)"Switch VLPS mode unsuccessfully\r\n");
            }
            break;
        default:
            /* This statement should not be reached */
            break;
        }

        /* Print separator */
        print((const char *)SEPARATOR);
    }


  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
