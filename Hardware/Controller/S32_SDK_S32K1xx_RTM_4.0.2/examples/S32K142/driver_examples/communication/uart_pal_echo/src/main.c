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
**     Project     : uart_pal_echo_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-02-20, 18:47, # CodeGen: 2
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

volatile int exit_code = 0;

#include <string.h>

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/
#define EVB

#ifdef EVB
    #define LED_PORT    PTD
    #define LED_PIN     16U
    #define LED_OFF     PINS_DRV_ClearPins(LED_PORT, 1 << LED_PIN)
    #define LED_ON      PINS_DRV_SetPins(LED_PORT, 1 << LED_PIN)
#else
    #define LED_PORT    PTC
    #define LED_PIN     1U
    #define LED_OFF     PINS_DRV_SetPins(LED_PORT, 1 << LED_PIN)
    #define LED_ON      PINS_DRV_ClearPins(LED_PORT, 1 << LED_PIN)
#endif

/* Use this define to specify if the application uses FLEXIO or LPUART */
//#define USE_FLEXIO_UART

#ifdef USE_FLEXIO_UART
    #define uart_instance     uart_instance_fi0
    #define UART_PAL_CONFIG   &FLEXIO_uart_pal_config0
#else
    #define uart_instance     uart_instance_lu1
    #define UART_PAL_CONFIG   &LPUART_1_uart_pal_config0
#endif

#define welcomeMsg "This example is an simple echo using UART_PAL\r\n\
it will send back any character you send to it.\r\n\
The board will greet you if you send 'Hello!'\r\
\nNow you can begin typing:\r\n"

/* Error message displayed at the console, in case data is received erroneously */
#define errorMsg "An error occurred! The application will stop!\r\n"

/* Timeout in ms for blocking operations */
#define TIMEOUT         200UL
#define BUFFER_SIZE     256UL

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
            UART_SetRxBuffer(&uart_instance, &buffer[bufferIdx], 1U);
        }
    }
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
  /* Write your code here */
  status_t status;
  uint32_t bytesRemaining = 0;

  /* Initialize and configure clocks
   *    -   see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins
   *    -   See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

#ifdef USE_FLEXIO_UART
  /* Turn on the LED to indicate the Flexio is being used */
  LED_ON;

  /* Disable LPUART1 pins */
  PINS_DRV_SetMuxModeSel(PORTC, 6U, PORT_PIN_DISABLED);
  PINS_DRV_SetMuxModeSel(PORTC, 7U, PORT_PIN_DISABLED);

#else
  /* Turn off the LED to indicate the Lpuart is being used */
  LED_OFF;
#endif

  /* Initialize for UART_PAL */
  UART_Init(&uart_instance, UART_PAL_CONFIG);

  /* Send a welcome message */
  UART_SendDataBlocking(&uart_instance, (uint8_t *)welcomeMsg, strlen(welcomeMsg), TIMEOUT);

  /* Infinite loop:
   *     - Receive data from user
   *     - Echo the received data back
   */
  while(1)
  {
      /* Receive and store data byte by byte until new line character is received,
       * or the buffer becomes full (256 characters received)
       */
      UART_ReceiveData(&uart_instance, buffer, 1U);
      /* Wait for transfer to be completed */
      while(UART_GetReceiveStatus(&uart_instance, &bytesRemaining) == STATUS_BUSY);

      /* Check the status */
      status = UART_GetReceiveStatus(&uart_instance, &bytesRemaining);

      if (status != STATUS_SUCCESS)
      {
          /* If an error occurred, send the error message and exit the loop */
          UART_SendDataBlocking(&uart_instance, (uint8_t *)errorMsg, strlen(errorMsg), TIMEOUT);
          break;
      }

      /* Process end of line in Doc/Window(CRLF) or Unix(LF) */
      if (buffer[bufferIdx - 1] == '\r')
      {
          bufferIdx = bufferIdx - 1;
          /* Replace end of line character with null */
          buffer[bufferIdx] = 0U;
      }
      else
      {
          /* Replace end of line character with null */
          buffer[bufferIdx] = 0U;
      }

      /* If the received string is "Hello!", send back "Hello World!" */
      if(strcmp((char *)buffer, "Hello!") == 0)
      {
          strcpy((char *)buffer, "Hello World!\n");
      }
      else
      {
          buffer[bufferIdx] = '\n';
          bufferIdx++;
          /* Append string terminator to the received data */
          buffer[bufferIdx] = 0U;
      }
      /* Send the received data back */
      UART_SendDataBlocking(&uart_instance, buffer, strlen((char *) buffer), TIMEOUT);
      /* Reset the buffer index to start a new reception */
      bufferIdx = 0U;
  }

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}

/* END main */
/*!
** @}
*/

