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

#define NUM_CONV_GROUP_ITERATIONS       10UL
#define DELAY_BETWEEN_SW_TRIG_GROUPS    1500UL /* [milliseconds] */


/* Uses ADC PAL config 0 - configured to measure ADC0 InputChannel 28, connected to the potentiometer. */
#define ADC_INSTANCE    0UL
#define ADC_CHN         ADC_INPUTCHAN_EXT12
#define ADC_VREFH       5.0f
#define ADC_VREFL       0.0f


#define welcomeStr "\r\nThis is an example for ADC PAL: it will print the average value of the conversion results in groups of conversions."\
                   "\r\nMeasurements are done on ADC0 Input 12 \r\n"
#define part1Str  "\r\n*** Part 1: software triggered conversion group \r\n\r\n"
#define part2Str  "\r\n*** Part 2: hardware triggered conversion group \r\n\r\n"
#define headerStr  "ADC avg result: "
#define exitStr    "\r\nADC PAL example execution finished successfully.\r\n"

/* Flag used to store if an ADC PAL conversion group has finished executing */
volatile bool groupConvDone = false;
/* Flag used to store the offset of the most recent result in the result buffer */
volatile uint32_t resultLastOffset = 0;

/* Variable to store value from ADC conversion */
volatile uint16_t adcRawValue;


void adc_pal_1_callback0(const adc_callback_info_t * const callbackInfo, void * userData)
{
    (void) userData;

    groupConvDone = true;
    resultLastOffset = callbackInfo->resultBufferTail;
}

void adc_pal_1_callback2(const adc_callback_info_t * const callbackInfo, void * userData)
{
    (void) userData;

    groupConvDone = true;
    resultLastOffset = callbackInfo->resultBufferTail;
}


/* Function which sends a string to user via LPUART
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
    /* Write your local variable definition here */
    status_t status;
    uint8_t selectedGroupIndex;
    uint16_t resultStartOffset;
    uint32_t sum, avg;
    uint16_t adcMax;
    float avgVolts;

    /* Buffer used to store processed data for serial communication */
    char msg[255] =
    { 0, };

    /* Initialize and configure clocks
         *  -   see clock manager component for details
         */
        CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
        CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

        /* Initialize pins
         *  -   See PinSettings component for more info
         */
        PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

        /* Get ADC max value from the resolution configured in extension field */
        adc_resolution_t resolution = ((extension_adc_s32k1xx_t *)(adc_pal_1_config.extension))->resolution;
        if (resolution == ADC_RESOLUTION_8BIT)
            adcMax = (uint16_t) (1 << 8);
        else if (resolution == ADC_RESOLUTION_10BIT)
            adcMax = (uint16_t) (1 << 10);
        else
            adcMax = (uint16_t) (1 << 12);

        /* Initialize LPUART instance
         *  -   See LPUART component for configuration details
         * If the initialization failed, trigger an hardware breakpoint
         */
        status = LPUART_DRV_Init(INST_LPUART_1, &lpUartState0, &lpUartInitConfig0);
        DEV_ASSERT(status == STATUS_SUCCESS);

        /* Initialize the ADC PAL
         *  -   See ADC PAL component for the configuration details
         */
        for(int i = 0; i< 4; i++)
        {
            DEV_ASSERT(adc_pal_1_channelsArray0[i] == ADC_CHN) ;
        }
        for(int i = 0; i< 5; i++)
        {
            DEV_ASSERT(adc_pal_1_channelsArray2[i] == ADC_CHN) ;
        }
        DEV_ASSERT(adc_pal_1_instance.instIdx == ADC_INSTANCE);
        status = ADC_Init(&adc_pal_1_instance, &adc_pal_1_config);
        DEV_ASSERT(status == STATUS_SUCCESS);

        /* Send welcome message */
        print(welcomeStr);


        /***************************************************************
         * Part 1 of the example: software triggered conversion group
         ***************************************************************/
        print(part1Str);

        OSIF_TimeDelay(DELAY_BETWEEN_SW_TRIG_GROUPS);

        selectedGroupIndex = 0u; /* Select the index of a SW triggered group of conversions (see ADC PAL component) */

        /* Start the selected SW triggered group of conversions */
        status = ADC_StartGroupConversion(&adc_pal_1_instance, selectedGroupIndex);
        DEV_ASSERT(status == STATUS_SUCCESS);

        /* Called only for demonstration purpose - it is not necessary and doesn't influence application functionality. */
        status = ADC_StartGroupConversion(&adc_pal_1_instance, 1u); /* Starting another SW triggered group while other is running will return BUSY. */
        /* When running step by step, it is expected that this DEV_ASSERT fails - because the group started by the first ADC_StartGroupConversion()
        finishes execution before the second call, so the current status is SUCCESS instead of BUSY. */
        DEV_ASSERT(status == STATUS_BUSY);
        /* Called only for demonstration purpose - it is not necessary and doesn't influence application functionality. */
        status = ADC_EnableHardwareTrigger(&adc_pal_1_instance, 2u); /* Enabling another HW triggered group while other SW triggered is running will return BUSY. */
        /* When running step by step, it is expected that this DEV_ASSERT fails - because the group started by the first ADC_StartGroupConversion()
        finishes execution before calling ADC_EnableHardwareTrigger(), so the current status is SUCCESS instead of BUSY. */
        DEV_ASSERT(status == STATUS_BUSY);

        uint8_t iter = 0;
        uint8_t numChans = adc_pal_1_config.groupConfigArray[selectedGroupIndex].numChannels;
        resultStartOffset = 0u;
        while(iter < NUM_CONV_GROUP_ITERATIONS)
        {
            /* Wait for group to finish */
            if(groupConvDone == true)
            {
                /* Calculate average value of the results in the group of conversions */
                sum = 0;
                for(uint8_t idx = resultStartOffset; idx <= resultLastOffset; idx++)
                {
                    sum += adc_pal_1_results0[idx]; /* Results are directly available in resultBuffer associated with the group at initialization */
                }
                DEV_ASSERT((resultLastOffset - resultStartOffset + 1) == numChans);
                avg = sum / numChans;

                /* Convert avg to volts */
                avgVolts = ((float) avg / adcMax) * (ADC_VREFH - ADC_VREFL);
                /* Convert avg to string */
                floatToStr(&avgVolts, msg, 5);

                /* Send the result to the user via LPUART */
                print(headerStr);
                print(msg);
                print(" V\r\n");

                /* Reset flag for group conversion status */
                groupConvDone = false;
                iter ++;

                OSIF_TimeDelay(DELAY_BETWEEN_SW_TRIG_GROUPS);

                /* Restart the SW triggered group of conversions */
                status = ADC_StartGroupConversion(&adc_pal_1_instance, selectedGroupIndex); /* Restart can be avoided if SW triggered group is configured to run in continuous mode */
                DEV_ASSERT(status == STATUS_SUCCESS);
            }
        }
        /* Stop the extra SW triggered conversion */
        status = ADC_StopGroupConversion(&adc_pal_1_instance, selectedGroupIndex, 1 /* millisecond */);
        DEV_ASSERT(status == STATUS_SUCCESS);


        /***************************************************************
         * Part 2 of the example: hardware triggered conversion group
         ***************************************************************/
        print(part2Str);

        selectedGroupIndex = 2u; /* Select the index of a HW triggered group of conversions (see ADC PAL component) */

        status = ADC_EnableHardwareTrigger(&adc_pal_1_instance, selectedGroupIndex);
        DEV_ASSERT(status == STATUS_SUCCESS);

        /* Initialize and start LPTMR counter */
        LPTMR_DRV_Init(INST_LPTMR_1, &lptmr_1_config0, false);
        LPTMR_DRV_StartCounter(INST_LPTMR_1);

        iter = 0;
        numChans = adc_pal_1_config.groupConfigArray[selectedGroupIndex].numChannels;
        uint32_t resultBuffLength = (numChans * adc_pal_1_config.groupConfigArray[selectedGroupIndex].numSetsResultBuffer);
        while(iter < NUM_CONV_GROUP_ITERATIONS)
        {
            /* wait for group to finish */
            if(groupConvDone == true)
            {
                /* Calculate average value of the results in the group of conversions */
                sum = 0;
                for(uint8_t idx = resultStartOffset; idx <= resultLastOffset; idx++)
                {
                    sum += adc_pal_1_results2[idx]; /* Results are directly available in resultBuffer associated with the group at initialization */
                }
                DEV_ASSERT((resultLastOffset - resultStartOffset + 1) == numChans);
                avg = sum / numChans;

                /* Convert avg to volts */
                avgVolts = ((float) avg / adcMax) * (ADC_VREFH - ADC_VREFL);
                /* Convert avg to string */
                floatToStr(&avgVolts, msg, 5);

                /* Send the result to the user via LPUART */
                print(headerStr);
                print(msg);
                print(" V\r\n");

                /* Update the offset in the circular results buffer */
                resultStartOffset = resultLastOffset + 1;
                if(resultStartOffset >= resultBuffLength)
                {
                    resultStartOffset = 0u; /* Buffer wrap-around */
                }

                groupConvDone = false;
                iter ++;
            }
        }
        LPTMR_DRV_StopCounter(INST_LPTMR_1);

        /* Called only for demonstration purpose - it is not necessary and doesn't influence application functionality. */
        status = ADC_Deinit(&adc_pal_1_instance);
        DEV_ASSERT(status == STATUS_BUSY); /* Busy because hw triggered group is still enabled  */

        status = ADC_DisableHardwareTrigger(&adc_pal_1_instance, selectedGroupIndex,  1 /* millisecond */);
        DEV_ASSERT(status == STATUS_SUCCESS);

        status = ADC_Deinit(&adc_pal_1_instance);
        DEV_ASSERT(status == STATUS_SUCCESS);

        LPTMR_DRV_Deinit(INST_LPTMR_1);

        print(exitStr);

        status = LPUART_DRV_Deinit(INST_LPUART_1);
        DEV_ASSERT(status == STATUS_SUCCESS);
}
