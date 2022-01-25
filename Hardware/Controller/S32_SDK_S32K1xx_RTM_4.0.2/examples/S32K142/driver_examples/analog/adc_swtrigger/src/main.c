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

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define EVB

#ifdef EVB
    #define ADC_INSTANCE    0UL
    #define ADC_CHN         ADC_INPUTCHAN_EXT12
    #define ADC_VREFH       5.0f
    #define ADC_VREFL       0.0f
#else
    #define ADC_INSTANCE    1UL
    #define ADC_CHN         ADC_INPUTCHAN_EXT10
    #define ADC_VREFH       5.0f
    #define ADC_VREFL       0.0f
#endif

#define welcomeStr "This is an ADC example, it will show you the value converted"\
                   "\r\nfrom ADC1 Input 10 (or ADC0 Input 12 for EVB )\r\n"
#define headerStr  "ADC result: "

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
	/* Enable the floating point unit */
	enableFPU();

	/* Write your local variable definition here */
	/* Variables in which we store data from ADC */
	uint16_t adcRawValue;
	uint16_t adcMax;
	float adcValue;

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

	/* Get ADC max value from the resolution */
	if (ADC_0_ConvConfig0.resolution == ADC_RESOLUTION_8BIT)
		adcMax = (uint16_t) (1 << 8);
	else if (ADC_0_ConvConfig0.resolution == ADC_RESOLUTION_10BIT)
		adcMax = (uint16_t) (1 << 10);
	else
		adcMax = (uint16_t) (1 << 12);

	/* Initialize LPUART instance
	 *  -   See LPUART component for configuration details
	 * If the initialization failed, break the program
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

	/* Send a welcome message via LPUART */
	print(welcomeStr);

	/* Infinite loop:
	 *  - Trigger a new conversion
	 *  - Wait and get the result
	 *  - Make the value more user friendly
	 *  - Send the data to LPUART
	 */

	while (1)
	{
		/* Configure ADC channel and software trigger a conversion */
		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &ADC_0_ChnConfig0);
		/* Wait for the conversion to be done */
		ADC_DRV_WaitConvDone(ADC_INSTANCE);
		/* Store the channel result into a local variable */
		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &adcRawValue);

		/* Process the result to get the value in volts */
		adcValue = ((float) adcRawValue / adcMax) * (ADC_VREFH - ADC_VREFL);
		/* And convert it to string */
		floatToStr(&adcValue, msg, 5);

		/* Send the result to the user via LPUART */
		print(headerStr);
		print(msg);
		print(" V\r\n");

		/* Do nothing for a number of cycles */
		delayCycles(0x7FFFFF);
	}
}
