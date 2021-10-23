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

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Defines containing the ADC max value, ADC Vbg value, ADC input channel and LPIT channel */
#define ADC_MAX_VALUE   ((1 << 12) - 1)
#define ADC_VBG_VALUE   (1000)
/* ADC channel and LPIT channel defines
 * They must be equal and in the range [0:3]
 */
#define ADC_CHANNEL     (0UL)
#define LPIT_CHANNEL    (0UL)

/* Extra ADC channels, used for external Vref measurement */
#define ADC_EXT_BGAP    (1UL)

/* template used to print the ADC bar on screen */
#define ADC_BAR_TEMPLATE    "ADC%1u-CH%02u [%s] Vin=%4umV Raw=%4u\n\r"
/* how long should the graph bar be */
#define ADC_BAR_LENGTH      32
/* an ADC conversion must be differ from the previous one with more than this number of counts to be considered valid */
#define ADC_HYSTERESIS      2

/* maximum time to wait for all data to be sent across LPUART, in ms */
#define LPUART_MAX_TIMEOUT  2000


#define WELCOME_TXT     "\n\rWelcome to ADC example project !\n\r" \
                        "\n\r" \
                        " This demo application shows how to use alternative hardware\n\r" \
                        "triggering for ADC via TRGMUX, LPUART with DMA and Power Manager.\n\r" \
                        " The ADC will not generate an interrupt if Vin is lower than Vref/2.\n\r" \
                        " At power on, the software determines the value in mV of Vref.\n\r\n\r" \
                        " Once a valid conversion is done, a bargraph and some information\n\r" \
                        "will be printed on screen.\n\r" \
                        " Nothing is printed on screen in the following conditions:\n\r" \
                        "- the ADC does not generate interrupts;\n\r" \
                        "- two following ADC conversions differ by less than 3 counts;\n\r" \
                        "- Vin is less than Vref/2;\n\r" \
                        "- Vin is equal or higher than Vref.\n\r" \
                        "\n\r" \
                        ""
#define ACK_WELCOME     "\n\rPress \"A\" or \"a\" to continue ... \n\r"

#define ACK_OK_WELCOME  "\n\rStarting example ... \n\r\n\r"

#define EXAMPLE_USAGE   "\n\rMove potentiometer to get a bargraph and some information displayed.\n\r\n\r"

#define VREF_PRINT_TEMPLATE "Vref=%4umV\n\r"

/* Global flag which stores if an ADC conversion is complete */
volatile bool g_adcEvent;
/* Global variable that will be used to save the conversion result */
volatile uint16_t g_conversionResult;

/* absolute */
#define mod_abs(a,b) ((a)<=(b) ? (b)-(a) : (a)-(b) )

/*
 * @brief Function that will create an array of chars that contain
 * a visual representation of the ADC result in the form of a bargraph.
 *
 * @param value[in]   : value to be represented
 * @param result[out] : pointer to an array of uint8_t where the graph will be saved
 * @param graphLen    : the length of the bar graph
 *
 * @return None
 */
void CreateGraph(uint16_t value, uint8_t * result, uint8_t graphLen)
{
    /* Local counter variable */
    uint8_t i;
    /* Number of the dots used in the graph by the conversion result */
    uint16_t numberOfDots = (uint16_t)((value *  graphLen)/ ADC_MAX_VALUE);

    /* Loop to manipulate the output array */
    for (i = 0; i < graphLen; i++)
    {
        if (i < numberOfDots)
        {
            result[i] = '#';
        }
        else
        {
            result[i] = '-';
        }
    }
}

/*
 * @brief ADC conversion complete interrupt service routine
 *  This function will set a flag to indicate an ADC event and
 *  will get the conversion result.
 *
 * @return None
 */
void adcISR(void)

{
    static uint16_t old_conversionResult=0;
    uint16_t loc_conversionResult=0;
    /* Get the conversion result */
    ADC_DRV_GetChanResult(INST_ADCONV1, ADC_CHANNEL, (uint16_t *)&loc_conversionResult);
    /* same result as before ? */
    if (mod_abs(loc_conversionResult, old_conversionResult) > ADC_HYSTERESIS)
    {
        /* Copy current conversion result */
        old_conversionResult = loc_conversionResult;
        g_conversionResult = loc_conversionResult;

        /* Set the ADC conversion complete flag */
        g_adcEvent = true;
    }
}

/*
 * @brief Function to measure the value of Vref using the
 * internal Vbg (bandgap reference) as known voltage.
 *
 * Warning !
 * This function resets the ADC configuration.
 *
 * @return Measured value
 */
uint16_t getVrefValue(void)
{
    uint16_t Vbg_measured_with_VrefAlt;
    uint16_t VrefAltH_measured_with_Vbg;
    uint16_t VrefAltL_measured_with_Vbg;
    uint16_t Vref;
    uint16_t VrefH;
    uint16_t VrefL;

    /* clean driver */
    ADC_DRV_Reset(INST_ADCONV1);

    /* Configure ADC converter to use software trigger and to have
     * a resolution of 12 bits
     */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig1);

    /* Configure the ADC channel and disable the conversion complete
     * interrupt.
     */
    ADC_DRV_ConfigChan(INST_ADCONV1, ADC_CHANNEL, &adConv1_ChnConfig1);

    /* Wait for the ADC to complete the conversion */
    ADC_DRV_WaitConvDone(INST_ADCONV1);

    /* Get the conversion result */
    ADC_DRV_GetChanResult(INST_ADCONV1, ADC_CHANNEL, (uint16_t *)&Vbg_measured_with_VrefAlt);

    /* clean driver */
    ADC_DRV_Reset(INST_ADCONV1);

    /* Configure ADC converter to use software trigger and to have
     * a resolution of 12 bits
     */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig2);

    /* Configure the ADC channel and disable the conversion complete
     * interrupt.
     */
    ADC_DRV_ConfigChan(INST_ADCONV1, ADC_CHANNEL, &adConv1_ChnConfig2);

    /* Wait for the ADC to complete the conversion */
    ADC_DRV_WaitConvDone(INST_ADCONV1);

    /* Get the conversion result */
    ADC_DRV_GetChanResult(INST_ADCONV1, ADC_CHANNEL, (uint16_t *)&VrefAltH_measured_with_Vbg);

    /* clean driver */
    ADC_DRV_Reset(INST_ADCONV1);

    /* Configure ADC converter to use software trigger and to have
     * a resolution of 12 bits
     */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig3);

    /* Configure the ADC channel and disable the conversion complete
     * interrupt.
     */
    ADC_DRV_ConfigChan(INST_ADCONV1, ADC_CHANNEL, &adConv1_ChnConfig3);

    /* Wait for the ADC to complete the conversion */
    ADC_DRV_WaitConvDone(INST_ADCONV1);

    /* Get the conversion result */
    ADC_DRV_GetChanResult(INST_ADCONV1, ADC_CHANNEL, (uint16_t *)&VrefAltL_measured_with_Vbg);

    /* clean driver */
    ADC_DRV_Reset(INST_ADCONV1);

    /* if Vbg_measured_with_VrefAlt is lower than (2^12-1) or 4095, then Vref is higher than Vbg */
    if (Vbg_measured_with_VrefAlt < ADC_MAX_VALUE)
    {
        /* Vref > Vbg , must calculate Vref */
        Vref = (uint16_t)((uint32_t)(ADC_MAX_VALUE * ADC_VBG_VALUE)/Vbg_measured_with_VrefAlt);
        /* We have the actual Vref value, return it to user */
        return (Vref);
    }
    else
    {
        /* Vref <= Vbg */
        VrefH = (uint16_t)((uint32_t)(ADC_MAX_VALUE * ADC_VBG_VALUE)/VrefAltH_measured_with_Vbg);
        VrefL = (uint16_t)((uint32_t)(ADC_MAX_VALUE * ADC_VBG_VALUE)/VrefAltL_measured_with_Vbg);
        Vref = (VrefH - VrefL);
        /* We have the actual Vref value, return it to user */
        return (Vref);
    }
    /* something went wrong */
    return (0);
}

/*
 * @brief Function to initialize the LPIT-TRGMUX-ADC trigger chain.
 *
 * @return None
 */
void InitPeripherals(void)
{
    /* Configure TRGMUX to route the trigger from LPIT to ADC */
    TRGMUX_DRV_Init(INST_TRGMUX, &trgmux1_InitConfig0);

    /* Initialize LPIT
     *  - Configured to run in Doze and Debug mode in 32 bit counter mode
     *  -   see LPIT component for more details
     */
    LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);

    /* Configure LPIT channel to have a period of 100 ms */
    LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL, &lpit1_ChnConfig0);

    /* Configure ADC converter to use hardware trigger and to have
     * a resolution of 12 bits
     */
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig0);

    /* Execute an Auto-Calibration sequence */
    ADC_DRV_AutoCalibration(INST_ADCONV1);

    /* Configure hardware compare to generate an validate the conversion
     * result only if the value is greater than half of the range
     * (In this case 1/2 of VDD)
     */
    ADC_DRV_ConfigHwCompare(INST_ADCONV1, &adConv1_HwCompConfig0);

    /* Configure the ADC channel and enable the conversion complete
     * interrupt.
     */
    ADC_DRV_ConfigChan(INST_ADCONV1, ADC_CHANNEL, &adConv1_ChnConfig0);

    /* Enable ADC interrupt and install an ISR */
    IRQn_Type adc_irq_id = ADC_DRV_GetInterruptNumber(INST_ADCONV1);
    INT_SYS_InstallHandler(adc_irq_id, &adcISR, (isr_t*) 0);
    INT_SYS_EnableIRQ(adc_irq_id);

    /* Initialize the LPUART instance used to send the ADC conversion data to the host computer */
    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);
}

/*
 * @brief Initialize system drivers and services such as clocks, pins
 * power management and eDMA.
 *
 * @return None
 */
void InitSystem(void)
{
     /* Initialize and configure clocks
     *  - Configure system clocks
     *  - Configure peripheral's clock gating and source
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT, \
                       g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);

    /* Update the clock configuration */
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *  - Configure pin muxing for LPUART, ADC and GPIO
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize Power Manager
     *  - Configure the Stop scheme for the MCU
     *  -   See Power Manager component for more info
     */
    POWER_SYS_Init(&powerConfigsArr, 2U, &powerStaticCallbacksConfigsArr, 0U);

    /* Initialize the Enhanced DMA to be used for LPUART transfers
     *  - Setup channel allocation
     *  -   See EDMA component for more info
     */
    EDMA_DRV_Init(&dmaController_State,
                  &dmaController_InitConfig,
                  edmaChnStateArray,
                  edmaChnConfigArray,
                  EDMA_CONFIGURED_CHANNELS_COUNT);
}

/*!
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{
	/* Local variable used to store the bar graph */
	    uint8_t barbuff[ADC_BAR_LENGTH+1];
	    /* local variable to store the full bargraph plus the formatted template, thus an overhead must be allocated */
	    uint8_t buff[sizeof(ADC_BAR_TEMPLATE)+ADC_BAR_LENGTH+64];
	    /* variable used to rx chars */
	    uint8_t rxbuff[1];

	    /* Setup system drivers */
	        InitSystem();
	        /* calculate Vref value in mV */
	        uint16_t extVref_mV=getVrefValue();
	        /* Configure LPIT+TRGMUX+ADC chain */
	        InitPeripherals();

	        /* Clear the ADC Event flag */
	        g_adcEvent = false;
	        /* Start the LPIT channel corresponding to the ADC channel */
	        LPIT_DRV_StartTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL));

	        /* Welcome screen */
	        LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *) WELCOME_TXT, (uint32_t)sizeof(WELCOME_TXT), LPUART_MAX_TIMEOUT);

	        /* translate printf modifiers for Vref printing */
	        sprintf((char *)buff, VREF_PRINT_TEMPLATE, extVref_mV);
	        /* Send the data to the host PC, no stalling */
	        LPUART_DRV_SendDataBlocking((uint32_t)INST_LPUART1, (const uint8_t *)buff, strlen((char *)buff), LPUART_MAX_TIMEOUT);

	        /* Acknowledge Welcome screen */
	        LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *) ACK_WELCOME, (uint32_t)sizeof(ACK_WELCOME), LPUART_MAX_TIMEOUT);
	        /* wait ack keys */
	        while (1)
	        {
	            /* get UART data, blocking */
	            if (STATUS_SUCCESS == LPUART_DRV_ReceiveDataBlocking(INST_LPUART1, &rxbuff[0], 1, LPUART_MAX_TIMEOUT*5))
	            {
	                /* check if correct key was pressed */
	                if (rxbuff[0]=='A' || rxbuff[0]=='a')
	                {
	                    break;
	                }
	            }
	        }
	        /* Acknowledge Welcome screen */
	        LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *) ACK_OK_WELCOME, (uint32_t)sizeof(ACK_OK_WELCOME), LPUART_MAX_TIMEOUT);

	        /* Usage, yet again */
	        LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *) EXAMPLE_USAGE, (uint32_t)sizeof(EXAMPLE_USAGE), LPUART_MAX_TIMEOUT);

	        /* Infinite loop */
	        while (1)
	        {

	            /* If the conversion is complete, read the result and send it via LPUART,
	             * then set the CPU in sleep mode
	             */
	            if (g_adcEvent == true)
	            {
	                /* Set the power mode to RUN */
	                POWER_SYS_SetMode(1U, POWER_MANAGER_POLICY_AGREEMENT);
	                /* calculate voltage */
	                uint16_t g_vin=(extVref_mV*g_conversionResult)/(ADC_MAX_VALUE);
	                /* Call the function that generates the bar graph from the ADC result */
	                CreateGraph(g_conversionResult, (uint8_t *)(barbuff), ADC_BAR_LENGTH);
	                /* ensure that string is termninated */
	                barbuff[ADC_BAR_LENGTH]='\0';
	                /* translate printf modifiers */
	                sprintf((char *)buff, ADC_BAR_TEMPLATE, INST_ADCONV1, adConv1_ChnConfig0.channel, barbuff, g_vin, g_conversionResult);
	                /* Send the data to the host PC, no stalling */
	                LPUART_DRV_SendDataBlocking((uint32_t)INST_LPUART1, (const uint8_t *)buff, strlen((char *)buff), LPUART_MAX_TIMEOUT);
	                /* Clear the event flag */
	                g_adcEvent = false;
	            }
	            /* Set the power mode to SLEEP */
	            POWER_SYS_SetMode(0U, POWER_MANAGER_POLICY_AGREEMENT);
	        }
}
