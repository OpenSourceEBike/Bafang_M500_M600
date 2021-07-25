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
**     Project     : flexio_uart_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-15, 14:05, # CodeGen: 0
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


/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>
#include <string.h>

#define WELCOME_MSG ("Hello World\r\n")
#define RX_BUFFER_SIZE 10

uint8_t rxBuffer[RX_BUFFER_SIZE];   /* Circular buffer for Rx */
volatile uint8_t rxWrite = 0;       /* Write location in Rx buffer */
volatile uint8_t rxRead = 0;        /* Read location in Rx buffer */
uint8_t txBuffer;                   /* 1-byte buffer for Tx */
flexio_device_state_t flexIODeviceState;
flexio_uart_state_t   uartStateTX;
flexio_uart_state_t   uartStateRX;

void flexio_uart_RX_Callback0(void *driverState, uart_event_t event, void *userData)
{
    (void)userData;
    (void)driverState;

    if (event == UART_EVENT_RX_FULL)
    {
        /* Advance write pointer to the next location in Rx buffer */
        rxWrite++;
        if (rxWrite == RX_BUFFER_SIZE)
        {
            rxWrite = 0;
        }
        /* Set new RX buffer */
        FLEXIO_UART_DRV_SetRxBuffer(&uartStateRX, &(rxBuffer[rxWrite]), 1UL);
    }
}

void flexio_uart_TX_Callback0(void *driverState, uart_event_t event, void *userData)
{
    (void)userData;
    (void)driverState;

    if (event == UART_EVENT_TX_EMPTY)
    {
        /* If a new char is already available, send it immediately
           This helps reduce the delays caused by stopping/restarting the transmission */
        if (rxRead != rxWrite)
        {
            /* Copy received byte to tx buffer */
            txBuffer = rxBuffer[rxRead];
            /* Advance read pointer to the next location in Rx buffer */
            rxRead++;
            if (rxRead == RX_BUFFER_SIZE)
            {
                rxRead = 0;
            }
            /* Send received byte */
            FLEXIO_UART_DRV_SetTxBuffer(&uartStateTX, &txBuffer, 1UL);
        }
    }
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - main()
*/
int main(void)
{
    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure FlexIO clock
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *  -   Init FlexIO UART RX and TX pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Init the FLEXIO device */
    FLEXIO_DRV_InitDevice(INST_FLEXIO_UART_TX, &flexIODeviceState);

    /* Init the FlexIO UART driver
     *  - 115200 baudrate
     *  - TX only
     *  - 8 bit per frame
     *  - Interrupt based
     */
     FLEXIO_UART_DRV_Init(INST_FLEXIO_UART_TX, &flexio_uart_TX_Config0, &uartStateTX);

    /* Init the FlexIO UART driver
     *  - 115200 baudrate
     *  - RX only
     *  - 8 bit per frame
     *  - Interrupt based
     */
     FLEXIO_UART_DRV_Init(INST_FLEXIO_UART_RX, &flexio_uart_RX_Config0, &uartStateRX);

     FLEXIO_UART_DRV_SendDataBlocking(&uartStateTX, (uint8_t * )WELCOME_MSG, strlen(WELCOME_MSG), 100U);
     FLEXIO_UART_DRV_ReceiveData(&uartStateRX, rxBuffer, 1U);

     while(1)
     {
        /* Wait for tx to be idle */
        if (FLEXIO_UART_DRV_GetStatus(&uartStateTX, NULL) == STATUS_SUCCESS)
        {
            if (rxRead != rxWrite)
            {
                /* Copy received byte to tx buffer */
                txBuffer = rxBuffer[rxRead];
                /* Advance read pointer to the next location in Rx buffer */
                rxRead++;
                if (rxRead == RX_BUFFER_SIZE)
                {
                    rxRead = 0;
                }
                /* Send received byte */
                FLEXIO_UART_DRV_SendData(&uartStateTX, &txBuffer, 1U);
            }
        }
     }

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
