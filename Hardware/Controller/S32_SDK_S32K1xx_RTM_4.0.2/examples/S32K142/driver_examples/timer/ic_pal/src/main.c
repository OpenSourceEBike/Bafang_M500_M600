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
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

volatile int exit_code = 0;


/***********************************
 * @brief: Wait for a number of cycles
 * @param nbOfCycles is number of cycles to be waited for
 ***********************************/
void delayCycles(volatile uint32_t cycles)
{
    while (cycles--);
}

/* brief: Method used to print a string with LPUART
 * param sourceStr: pointer to the string beginning
 */

void print(const char * sourceStr)
{
    uint32_t bytesRemaining;
    LPUART_DRV_SendData(INST_LPUART_1, (uint8_t *)sourceStr, strlen(sourceStr));
    while (LPUART_DRV_GetTransmitStatus(INST_LPUART_1, &bytesRemaining) != STATUS_SUCCESS);
}

/***********************************
 * @brief: The callback application
 * @param event: An event which is triggered from the input capture callback
 * @param userData: The input parameter
 ***********************************/
void ic_pal1_channel_callBack0(ic_event_t event, void * userData)
{
    static uint32_t index = 0U;
    index++;
    userData = (void*) index;
    (void)userData;
    (void)event;
}


/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    ftm_state_t ftm1StateStruct;
    /* Variables used to store PWM frequency,
     * input capture measurement value
     */
    uint16_t inputCaptureMeas = 0U;
    uint32_t frequency;
    uint8_t channel = ic_pal_1_ChnConfig->hwChannelId;

    bool conversionComplete = false;
    /* Buffer for string processing */
    char txBuff[255];

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize LPUART instance */
    LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpUartInitConfig1);

    /* Initialize FTM instances, PWM and Input capture
     *  -   See ftm component for more details
     */
    FTM_DRV_Init(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_InitConfig, &ftm1StateStruct);

    /* Setup input capture */
    IC_Init(&ic_pal_1_instanceConfig, &ic_pal_1_InitConfig);

    /* Start PWM */
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_PwmConfig);

    /* Get the FTM frequency to calculate
     * the frequency of the measured signal.
     */
    frequency = FTM_DRV_GetFrequency(ic_pal_1_instanceConfig.instIdx);

    /* Send the hello message */
    print("This example will show you how to use IC's signal measurement feature.\r\n"
          "To achieve that we will generate a modifiable frequency PWM and read it with Input Capture.\r\n");

    /* Infinite loop
     *  -   Wait for user input
     *  -   Measure and calculate the signal frequency
     *  -   Send original and measured freq via LPUART
     *  -   Modify the PWM frequency
     */
    for ( ;; )
    {
        print("Press any key to initiate a new conversion...\r\n");
        /* Wait for user input */
        LPUART_DRV_ReceiveDataBlocking(INST_LPUART_1, (uint8_t *)&txBuff, 1U, 1 << 31U);
        conversionComplete = false;

        while (conversionComplete == false)
        {
            /* Wait a number of cycles for the PWM to reach stability */
            delayCycles(0x7FFFFU);

            /* Get the latest value */
            inputCaptureMeas = IC_GetMeasurement(&ic_pal_1_instanceConfig, channel);
            /* Calculate the signal frequency using recorded data*/
            inputCaptureMeas = frequency / (inputCaptureMeas);

            /* Stop PWM */
            FTM_DRV_DeinitPwm(INST_FLEXTIMER_PWM_1);

            /* Convert the integer to char array and send it*/
            sprintf(txBuff, "PWM frequency:%6lu, Measured frequency:%6u [Hz]\r\n",
                    flexTimer_pwm_1_PwmConfig.uFrequencyHZ, inputCaptureMeas);
            print(txBuff);

            /* Increase frequency */
            if (flexTimer_pwm_1_PwmConfig.uFrequencyHZ < 3000U)
            {
                flexTimer_pwm_1_PwmConfig.uFrequencyHZ += 100U;
            }
            else
            {
                flexTimer_pwm_1_PwmConfig.uFrequencyHZ = 1000U;
                conversionComplete = true;
            }

            /* Restart PWM with new settings */
            FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_PwmConfig);
        }
    }

    return exit_code;
}
/* END main */
