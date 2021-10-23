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
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

#include "sdk_project_config.h"


volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>

/* Definition of the data transfer size */
#define BUFF_SIZE (64u)

/* Definition LED */
#define LED_PORT  (PTD)
#define LED_RED   (15u)
#define LED_GREEN (16u)

/* Master TX and RX buffers definition */
uint8_t masterTxBuffer[BUFF_SIZE];
uint8_t masterRxBuffer[BUFF_SIZE];
/* compare buffers definition */
uint8_t compareBuffer[BUFF_SIZE];
bool CheckResult;
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
    /* Allocate memory for the LPI2C driver state structure */
    lpi2c_master_state_t lpi2c1MasterState;
    /* Variable used for the loop that initializes the data buffer */
    uint8_t cnt = 0;

    /* Initialize the data buffer */
    for(cnt = 0; cnt < BUFF_SIZE; cnt++)
    {
        masterTxBuffer[cnt] = (uint8_t)cnt;
        masterRxBuffer[cnt] = 0U;
        compareBuffer[cnt]  = (uint8_t)(BUFF_SIZE - cnt - 1u);
    }
    /* Initialize and configure clocks
     *  - Configure system clocks and dividers
     *  - Configure LPI2C clock gating
     *  -   see clock manager component for details
     */

     CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins
     *  - Configure LPI2C pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize LPI2C Master configuration
     *  - Slave address 0x32
     *  - Fast operating mode, 400 KHz SCL frequency
     *  -   See LPI2C components for configuration details
     */
    LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0, &lpi2c1MasterState);

    /* Send a packet of data to the bus slave */
    LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, masterTxBuffer, BUFF_SIZE, true, OSIF_WAIT_FOREVER);

    /* Request data from the bus slave */
    LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C0, masterRxBuffer, BUFF_SIZE, true, OSIF_WAIT_FOREVER);

    CheckResult = true;
    for (cnt = 0; cnt < BUFF_SIZE; cnt++)
    {
        if (masterRxBuffer[cnt] != compareBuffer[cnt])
        {
            CheckResult = false;
            break;
        }
    }

    /* Turn on Red or Green LED depending on the check result */
    PINS_DRV_WritePin(LED_PORT, LED_RED, (1u - CheckResult));
    PINS_DRV_WritePin(LED_PORT, LED_GREEN, CheckResult);
    /* End of the driver example */

    for(;;) {
      if(exit_code != 0) {
        break;
      }
    }
    return exit_code;
}
