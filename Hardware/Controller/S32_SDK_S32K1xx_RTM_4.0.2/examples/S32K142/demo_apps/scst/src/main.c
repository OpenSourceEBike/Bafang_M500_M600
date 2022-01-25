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
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define EVB

#ifdef EVB
    #define LED_PORT    PORTD
    #define GPIO_PORT   PTD
    #define LED1        16U
    #define LED2        15U
#else
    #define LED_PORT    PORTC
    #define GPIO_PORT   PTC
    #define LED1        0U
    #define LED2        1U
#endif

/* Define Test ID for test where will be injected error */
#define INJECT_ERROR_TO_TEST_NUM 40U
#define ACCUMULATED_SIGNATURE_ID0_43 0x929940F1U

/* Linker defined variables used to copy initialized data from ROM to RAM */
extern uint32_t __SCST_DATA_ROM[];
extern uint32_t __SCST_DATA_SIZE[];
extern uint32_t __scst_data_start__[];

volatile int32_t exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - main()
*/
int main(void)
{
  m4_scst_uint32_t result;
  status_t error;
  /* Configure clocks for PORT */
  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(error == STATUS_SUCCESS);
  /* Set pins as GPIO */
  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(error == STATUS_SUCCESS);

  /* Set Output value LED0 & LED1 */
  PINS_DRV_SetPins(GPIO_PORT, 1 << LED1);
  PINS_DRV_ClearPins(GPIO_PORT, 1 << LED2);

  /* Copy sCST initialized data from ROM to RAM */
  memcpy(__scst_data_start__, __SCST_DATA_ROM, (uint32_t)__SCST_DATA_SIZE);
  
  /* Inject fault to test number defined by INJECT_ERROR_TO_TEST_NUM */
  /* Uncomment the following line to allow error injection */
  /*m4_scst_fault_inject_test_index = INJECT_ERROR_TO_TEST_NUM;*/

  /* Execute all tests */
  result = m4_scst_execute_core_tests(0U,43U);

  if(result == M4_SCST_WRONG_RANGE)
  {
    /* If the test range was incorrectly set, stop the execution flow */
    while(1);
  }
  else
  {
    /* All tests passed */
    if (m4_scst_accumulated_signature == ACCUMULATED_SIGNATURE_ID0_43)
    {
       /* Turn on GREEN LED to signal that the test was successful */
       PINS_DRV_ClearPins(GPIO_PORT, 1 << LED2);
       PINS_DRV_SetPins(GPIO_PORT, 1 << LED1);
    }
    /* Test was interrupted */
    else if (m4_scst_accumulated_signature == M4_SCST_TEST_WAS_INTERRUPTED)
    {
      /* Check the m4_scst_last_executed_test_number to verify which test was interrupted */
      /* Turn on RED LED to signal error */
      PINS_DRV_SetPins(GPIO_PORT, 1 << LED2);
      PINS_DRV_ClearPins(GPIO_PORT, 1 << LED1);
    }
    /* Some test failed or error injection was applied */
    else if ((m4_scst_accumulated_signature != 0x929940F1U) && (m4_scst_accumulated_signature != M4_SCST_TEST_WAS_INTERRUPTED))
    {
      if (m4_scst_last_executed_test_number == INJECT_ERROR_TO_TEST_NUM)
      {
        /* Stop here in case of error injection works correctly */
        /* Turn on GREEN LED to signal that the test was successful */
        PINS_DRV_ClearPins(GPIO_PORT, 1 << LED2);
        PINS_DRV_SetPins(GPIO_PORT, 1 << LED1);
      }
      else
      {
        /* Stop here in case of error injection does not work correctly */
        /* Turn on RED LED to signal error */
        PINS_DRV_SetPins(GPIO_PORT, 1 << LED2);
        PINS_DRV_ClearPins(GPIO_PORT, 1 << LED1);
      }
    }
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

