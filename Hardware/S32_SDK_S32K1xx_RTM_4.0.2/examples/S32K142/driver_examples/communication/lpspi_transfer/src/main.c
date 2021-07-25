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


/* Including necessary configuration files. */
#include "sdk_project_config.h"

#define BUFFER_SIZE 1U
#define TIMEOUT 10U

volatile int exit_code = 0;
/* User includes */

#include <stdint.h>
#include <stdbool.h>

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your local variable definition here */

    ftm_state_t ftmStateStruct;
    uint8_t masterDataSend;
    uint8_t masterDataReceive;
    uint8_t slaveDataReceive;
    uint16_t adcRead;
    uint8_t slaveDataSend;
    /* Initialize and configure clocks
     *     -   see clock manager component for details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Example Description
     * Potentiometer voltage is read using ADC0- channel 0. The value is send to
     * LPSPI1 from LPSPI0. The read value represent duty for PWM (FTM0-channel 0).
     */
    /* Initialize LPSPI0 (Send)*/
    LPSPI_DRV_MasterInit(INST_LPSPI_1, &lpspi_1State, &lpspi_0_MasterConfig0);
    /* Initialize LPSPI1 (Receive)*/
    LPSPI_DRV_SlaveInit(INST_LPSPI_2, &lpspi_2State, &lpspi_1_SlaveConfig0);
    /* Initialize ADC */
    ADC_DRV_ConfigConverter(INST_ADC_CONFIG_1, &adc_config_1_ConvConfig0);
    /* Initialize FTM */
    FTM_DRV_Init(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_InitConfig, &ftmStateStruct);
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM_1, &flexTimer_pwm_1_PwmConfig);

    while(1)
    {
        ADC_DRV_ConfigChan(INST_ADC_CONFIG_1, 0U, &adc_config_1_ChnConfig0);
        ADC_DRV_WaitConvDone(INST_ADC_CONFIG_1);
        /* Store the channel result into a local variable slaveDataSend*/
        ADC_DRV_GetChanResult(INST_ADC_CONFIG_1, 0U, &adcRead);
        slaveDataSend = (uint8_t)adcRead;
        LPSPI_DRV_SlaveTransfer(INST_LPSPI_2, &slaveDataSend, &slaveDataReceive, BUFFER_SIZE);
        LPSPI_DRV_MasterTransferBlocking(INST_LPSPI_1, &masterDataSend, &masterDataReceive, BUFFER_SIZE, TIMEOUT);
        FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM_1, 0U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, masterDataReceive<<8, 0U, true);
    }

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
