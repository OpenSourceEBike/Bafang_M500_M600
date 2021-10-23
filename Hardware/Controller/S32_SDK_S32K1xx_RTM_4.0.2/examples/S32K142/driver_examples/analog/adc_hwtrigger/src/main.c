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

#include "sdk_project_config.h"

#include "helper_functions.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Timeout for PDB in microseconds */
#define PDLY_TIMEOUT    1000000UL

volatile int exit_code = 0;

#define EVB

#ifdef EVB
    #define ADC_INSTANCE    0UL
    #define ADC_CHN         ADC_INPUTCHAN_EXT12
    #define ADC_VREFH       5.0f
    #define ADC_VREFL       0.0f
    #define PDB_INSTANCE    0UL
#else
    #define ADC_INSTANCE    1UL
    #define ADC_CHN         ADC_INPUTCHAN_EXT10
    #define ADC_VREFH       5.0f
    #define ADC_VREFL       0.0f
    #define PDB_INSTANCE    1UL
#endif


#define welcomeStr "This is an ADC example, it will show you the value converted"\
                   "\r\nfrom ADC1 Input 10 (or ADC0 Input 12 for EVB )\r\n"
#define headerStr  "ADC result: "

/* Flag used to store if an ADC IRQ was executed */
volatile bool adcConvDone;
/* Variable to store value from ADC conversion */
volatile uint16_t adcRawValue;

/* @brief: ADC Interrupt Service Routine.
 *        Read the conversion result, store it
 *        into a variable and set a specified flag.
 */
void ADC_IRQHandler(void)
{
    /* Get channel result from ADC channel */
    ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, (uint16_t *)&adcRawValue);
    /* Set ADC conversion complete flag */
    adcConvDone = true;
}

/* brief: Send a string to user via LPUART
 * param sourceStr: pointer to the array of characters
 *                  that you wish to send.
 * return:          None
 */
void print(const char *sourceStr)
{
    uint32_t bytesRemaining;

    /* Send data via LPUART */
    LPUART_DRV_SendData(INST_LPUART_1, (uint8_t *) sourceStr, strlen(sourceStr));
    /* Wait for transmission to be successful */
    while (LPUART_DRV_GetTransmitStatus(INST_LPUART_1, &bytesRemaining)
            != STATUS_SUCCESS)
    {
    }
}

/*!
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{
    uint16_t delayValue = 0;
    status_t status;

    /* Variables in which we store data from ADC */
    uint16_t adcMax;
    float adcValue;

    /* Buffer used to store processed data for serial communication */
    char msg[255] =
    { 0, };

    adcConvDone = false;

    /* Configure clocks for PORT */
    status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Set pins as GPIO */
    status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* Get ADC max value from the resolution */
    if (ADC_0_ConvConfig0.resolution == ADC_RESOLUTION_8BIT)
        adcMax = (uint16_t) (1 << 8);
    else if (ADC_0_ConvConfig0.resolution == ADC_RESOLUTION_10BIT)
        adcMax = (uint16_t) (1 << 10);
    else
        adcMax = (uint16_t) (1 << 12);

    /* Initialize LPUART instance
     *  -   See LPUART component for configuration details
     * If the initialization failed, trigger an hardware breakpoint
     */
    if (LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpUartInitConfig1)
            != STATUS_SUCCESS)
        __asm("bkpt #255");

    /* Configure and calibrate the ADC converter
     *  -   See ADC component for the configuration details
     */

    DEV_ASSERT(ADC_0_ChnConfig0.channel == ADC_CHN);

    ADC_DRV_ConfigConverter(ADC_INSTANCE, &ADC_0_ConvConfig0);
    ADC_DRV_AutoCalibration(ADC_INSTANCE);
    ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &ADC_0_ChnConfig0);

    IRQn_Type adcIRQ;

    switch(ADC_INSTANCE)
    {
    case 0UL:
        adcIRQ = ADC0_IRQn;
        break;
    case 1UL:
        adcIRQ = ADC1_IRQn;
        break;
    default:
        adcIRQ = ADC1_IRQn;
        break;
    }

    INT_SYS_InstallHandler(adcIRQ, &ADC_IRQHandler, (isr_t*) 0);

     /* Calculate the value needed for PDB instance
     * to generate an interrupt at a specified timeout.
     * If the value can not be reached, stop the application flow
     */
    if (!calculateIntValue(&pdb_1_timerConfig0, PDLY_TIMEOUT, &delayValue))
    {
        /* Stop the application flow */
        while(1);
    }
    /* Setup PDB instance
     *  -   See PDB component for details
     *  Note: Pre multiplier and Prescaler values come from
     *        calculateIntValue function.
     */
    PDB_DRV_Init(PDB_INSTANCE, &pdb_1_timerConfig0);
    PDB_DRV_Enable(PDB_INSTANCE);
    PDB_DRV_ConfigAdcPreTrigger(PDB_INSTANCE, 0UL, &pdb_1_adcTrigConfig0);
    PDB_DRV_SetTimerModulusValue(PDB_INSTANCE, (uint32_t) delayValue);
    PDB_DRV_SetAdcPreTriggerDelayValue(PDB_INSTANCE, 0UL, 0UL,
            (uint32_t) delayValue);
    PDB_DRV_LoadValuesCmd(PDB_INSTANCE);
    PDB_DRV_SoftTriggerCmd(PDB_INSTANCE);

    /* Enable ADC 1 interrupt */
    INT_SYS_EnableIRQ(adcIRQ);

    /* Send welcome message */
    print(welcomeStr);

    /* Infinite loop
     *  -   Wait for ADC conversion complete interrupt,
     *      then process and send the result to user.
     */

    while (1)
    {
        if (adcConvDone == true)
        {
            /* Process the result to get the value in volts */
            adcValue = ((float) adcRawValue / adcMax) * (ADC_VREFH - ADC_VREFL);
            /* And convert it to string */
            floatToStr(&adcValue, msg, 5);

            /* Send the result to the user via LPUART */
            print(headerStr);
            print(msg);
            print(" V\r\n");

            /* Clear conversion done interrupt flag */
            adcConvDone = false;
            /* Trigger PDB timer */
            PDB_DRV_SoftTriggerCmd(PDB_INSTANCE);
        }
    }

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;

}
