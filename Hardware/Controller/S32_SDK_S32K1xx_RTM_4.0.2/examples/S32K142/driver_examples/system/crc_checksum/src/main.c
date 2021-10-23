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
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>

extern const uint32_t CRC_data[];

/* Define CRC data size */
#define CRC_DATA_SIZE           (548U)
/* Define result CRC calculator with CCITT 16 bits standard */
#define RESULT_CRC_16_CCITT     (0xb532U)

/* To use with other board than EVB please comment the following line */
#define EVB

#ifdef EVB
    #define LED0            15U /* pin PTD15 - LED RED of LED RGB on DEV-KIT    */
    #define LED1            16U /* pin PTD16 - LED GREEN of LED RGB on DEV-KIT  */
    #define LED_GPIO        PTD /* LED GPIO type                                */
    #define SW              12U /* pin PTC12 - SW2_BTN0 on DEV-KIT              */
    #define SW_GPIO         PTC /* SW GPIO type                                 */
#else
    #define LED0            0U /* pin PTC0 - LED0 on Motherboard                */
    #define LED1            1U /* pin PTC1 - LED1 on Motherboard                */
    #define LED_GPIO        PTC /* LED GPIO type                                */
    #define SW              12U /* pin PTC12 - SW2_BTN0 on DEV-KIT              */
    #define SW_GPIO         PTC /* SW GPIO type                                 */
#endif

/*!
  \brief The main function for the project.
*/
int main(void)
{
    /* Write your local variable definition here */
    volatile uint32_t result;

    /* Initializes clocks for CRC instances used */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Turn off both LEDs */
    PINS_DRV_WritePin(LED_GPIO, LED0, 0u);
    PINS_DRV_WritePin(LED_GPIO, LED1, 0u);

    /* CCITT 16 bits standard */
    /*
        .crcWidth = CRC_BITS_16,
        .seed = 0xFFFFU,
        .polynomial = 0x1021U,
        .writeTranspose = CRC_TRANSPOSE_NONE,
        .readTranspose = CRC_TRANSPOSE_NONE,
        .complementChecksum = false
    */
    /* result = 0xb532 */
    /* Initialize CRC module */
    CRC_DRV_Init(INST_CRC, &crc_Cfg0);

    /* Infinite loop */
    for(;;) {

        if (PINS_DRV_ReadPins(SW_GPIO) & (1U << SW))
        {
            /* Calculate CRC value for CRC_data with configuration of 16 bit wide result */
            CRC_DRV_WriteData(INST_CRC, (const uint8_t*)CRC_data, CRC_DATA_SIZE);
            result = CRC_DRV_GetCrcResult(INST_CRC);
            /* If result of CRC calculator is correct */
            if (result == RESULT_CRC_16_CCITT)
            {
                /* Turn off RED LED */
                PINS_DRV_WritePin(LED_GPIO, LED0, 0u);
                /* Turn on GREEN LED */
                PINS_DRV_WritePin(LED_GPIO, LED1, 1u);
            }
            else
            {
                /* Turn on RED LED */
                PINS_DRV_WritePin(LED_GPIO, LED0, 1u);
                /* Turn off GREEN LED */
                PINS_DRV_WritePin(LED_GPIO, LED1, 0u);
            }
            break;
        }
    }

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
