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

/* Including needed modules to compile this module/procedure */
#include "freemaster.h"
#include "sdk_project_config.h"

#define ADC_INSTANCE        0UL
#define ADC_CHN             12U
#define ADC_NR_SAMPLES      72

volatile int exit_code = 0;

/* Variable used to count the number of ADC conversions */
volatile uint32_t   adcConvCnt;

/* Global variables used by FreeMASTER */
/* Variable to store value from ADC conversion */
volatile uint16_t   adcRawValue;
/* Variable to store number of ADC conversions per 10 ms */
volatile uint32_t   adcConvs;
/* Variable to store mean value of ADC_NR_SAMPLES conversions */
uint32_t            adcMeanValue;

/* @brief: ADC Interrupt Service Routine.
 *        Read the conversion result, store it
 *        into a variable and increment a counter.
 */
void ADC_IRQHandler(void)
{
    /* Get channel result from ADC channel */
    ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, (uint16_t *)&adcRawValue);
    /* Increment the counter */
    adcConvCnt++;
}

/* @brief: LPTMR Interrupt Service Routine.
 *        Clear the compare flag, store the
 *        number of ADC conversions into a variable,
 *        reset the ADC conversions counter
 *        and set a specified flag.
 */
void LPTMR_IRQHandler(void)
{
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);

    /* Store the number of ADC conversions */
    adcConvs = adcConvCnt;
    /* Reset the ADC conversions counter */
    adcConvCnt = 0;
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
    /* Variables used for computing ADC mean value */
    status_t error;
    uint16_t cnt;
    uint32_t adcSum;

    /* Variables used for processing FreeMASATER application commands */
    static FMSTR_APPCMD_CODE cmd;
    static FMSTR_APPCMD_PDATA cmdDataP;
    static FMSTR_SIZE cmdSize;

  /* Write your code here */
    /* Initialize counters and flags */
    adcConvCnt = 0;
    adcConvs = 0;

    /* Initialize and configure clocks
     *  -   see clock manager component for details
     */
    error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
    DEV_ASSERT(error == STATUS_SUCCESS);

    /* Initialize pins
     *  -   See PinSettings component for more info
     */
    error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    DEV_ASSERT(error == STATUS_SUCCESS);

    /* Initialize LPUART instance */
    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

    INT_SYS_InstallHandler(LPUART1_RxTx_IRQn, FMSTR_Isr, NULL);

    /* Configure the ADC converter
     *  -   See ADC component for the configuration details
     */
    adConv1_ChnConfig0.channel = ADC_CHN;
    adConv1_HwAvgConfig0.hwAvgEnable = 0;

    ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
    ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

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
    /* Install IRQ handler for ADC interrupt */
    INT_SYS_InstallHandler(adcIRQ, &ADC_IRQHandler, (isr_t*) 0);

    /* Init LPTMR as timer
     *  -   interrupt after 1 second
     *  -   FIRC as clock source
     */
    LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_GeneralConfig0, false);
    /* Install IRQ handler for LPTMR interrupt */
    INT_SYS_InstallHandler(LPTMR0_IRQn, &LPTMR_IRQHandler, (isr_t *)0);
    /* Enable IRQ for LPTMR */
    INT_SYS_EnableIRQ(LPTMR0_IRQn);
    /* Start LPTMR counter */
    LPTMR_DRV_StartCounter(INST_LPTMR1);

    /* Enable ADC interrupt */
    INT_SYS_EnableIRQ(adcIRQ);

    /* Initialize FreeMASTER driver */
    FMSTR_Init();

    while(1)
    {
        /* Compute mean value of ADC conversions */
        cnt = 0;
        adcSum = 0;

        while(cnt < ADC_NR_SAMPLES) {
            adcSum += adcRawValue;
            cnt++;
        }
        adcMeanValue = adcSum / ADC_NR_SAMPLES;

        /* Process FreeMASTER application commands */
        cmd = FMSTR_GetAppCmd();
        if (cmd != FMSTR_APPCMDRESULT_NOCMD) {
            cmdDataP = FMSTR_GetAppCmdData(&cmdSize);
            switch(cmd) {
            /* set_ADC_resolution command */
            case 0:
                adConv1_ConvConfig0.resolution = *cmdDataP;

                ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
                ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

                /* Acknowledge the command */
                FMSTR_AppCmdAck(0);
                break;
            /* set_ADC_sampleTime command */
            case 1:
                adConv1_ConvConfig0.sampleTime = *cmdDataP;

                ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
                ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

                /* Acknowledge the command */
                FMSTR_AppCmdAck(0);
                break;
            /* set_ADC_hwAvgEnable command */
            case 2:
                adConv1_HwAvgConfig0.hwAvgEnable = *cmdDataP;

                ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
                ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

                /* Acknowledge the command */
                FMSTR_AppCmdAck(0);
                break;
            /* set_ADC_hwAverage command */
            case 3:
                adConv1_HwAvgConfig0.hwAverage = *cmdDataP;

                ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
                ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

                /* Acknowledge the command */
                FMSTR_AppCmdAck(0);
                break;
            default:
                /* Acknowledge the command with failure */
                FMSTR_AppCmdAck(1);
                break;
            }
        }

        /* Handle the protocol decoding and execution */
        FMSTR_Poll();
    }

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}

