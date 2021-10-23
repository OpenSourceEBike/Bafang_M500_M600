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
**     Project     : lpuart_echo_s32k142
**     Processor   : S32K142
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-02-20, 12:27, # CodeGen: 1
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
#include <stdio.h>
#include <string.h>

volatile int exit_code = 0;

/* Welcome message displayed at the console */
#define welcomeMsg "This example is an simple echo using LPUART\r\n\
it will send back any character you send to it.\r\n\
The board will greet you if you send 'Hello Board'\r\
\nNow you can begin typing:\r\n"

/* Error message displayed at the console, in case data is received erroneously */
#define errorMsg "An error occurred! The application will stop!\r\n"

/* Timeout in ms for blocking operations */
#define TIMEOUT     500U

/* Receive buffer size */
#define BUFFER_SIZE 256U

/* Buffer used to receive data from the console */
uint8_t buffer[BUFFER_SIZE];
uint8_t bufferIdx;

/* UART rx callback for continuous reception, byte by byte */
void rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when newline is received or the buffer is full */
        if ((buffer[bufferIdx] != '\n') && (bufferIdx != (BUFFER_SIZE - 2U)))
        {
            /* Update the buffer index and the rx buffer */
            bufferIdx++;
            LPUART_DRV_SetRxBuffer(INST_LPUART_LPUART_1, &buffer[bufferIdx], 1U);
        }
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
  /* Declare a buffer used to store the received data */
  uint32_t bytesRemaining;

  /* Write your code here */
  /* For example: for(;;) { } */

  /* Initialize and configure clocks
   *     -    see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins
   *    -    See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

  /* Initialize LPUART instance */
  LPUART_DRV_Init(INST_LPUART_LPUART_1, &lpUartState1, &lpUartInitConfig1);
  /* Install the callback for rx events */
  LPUART_DRV_InstallRxCallback(INST_LPUART_LPUART_1, rxCallback, NULL);
  /* Send a welcome message */
  LPUART_DRV_SendDataBlocking(INST_LPUART_LPUART_1, (uint8_t *)welcomeMsg, strlen(welcomeMsg), TIMEOUT);

  /* Infinite loop:
   *     - Receive data from user
   *     - Echo the received data back
   */
  while (1)
  {
      /* Receive and store data byte by byte until new line character is received,
       * or the buffer becomes full (256 characters received)
       */
      LPUART_DRV_ReceiveData(INST_LPUART_LPUART_1, buffer, 1U);
      /* Wait for transfer to be completed */
      while(LPUART_DRV_GetReceiveStatus(INST_LPUART_LPUART_1, &bytesRemaining) == STATUS_BUSY);

      /* Check the status */
      status = LPUART_DRV_GetReceiveStatus(INST_LPUART_LPUART_1, &bytesRemaining);

      if (status != STATUS_SUCCESS)
      {
          /* If an error occurred, send the error message and exit the loop */
          LPUART_DRV_SendDataBlocking(INST_LPUART_LPUART_1, (uint8_t *)errorMsg, strlen(errorMsg), TIMEOUT);
          break;
      }

      /* Append string terminator to the received data */
      bufferIdx++;
      buffer[bufferIdx] = 0U;

      /* If the received string is "Hello Board", send back "Hello World" */
      if(strcmp((char *)buffer, "Hello Board\n") == 0)
      {
          strcpy((char *)buffer, "Hello World\n");
      }

      /* Send the received data back */
      LPUART_DRV_SendDataBlocking(INST_LPUART_LPUART_1, buffer, bufferIdx, TIMEOUT);
      /* Reset the buffer index to start a new reception */
      bufferIdx = 0U;
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
