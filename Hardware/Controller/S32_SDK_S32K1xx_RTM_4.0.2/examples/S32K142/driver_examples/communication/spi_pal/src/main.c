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
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_FRAMES    10U
#define TIMEOUT             1000UL   /* 1 second interval */
#define BUFFER_SIZE         10
#define RED_LED             (1<<15U)    /* RED_LED  PTD[15] */
#define GPIO_LED            PTD

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your local variable definition here */
    uint8_t master_send[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t master_receive[BUFFER_SIZE];
    uint8_t slave_send[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t slave_receive[BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t frame_sent = 1;

    /* Write configuration for master using flexio */
    FLEXIO_MasterInitConfig0Extension.misoPin = 4;
    FLEXIO_MasterInitConfig0Extension.mosiPin = 1;
    FLEXIO_MasterInitConfig0Extension.sckPin = 2;
    FLEXIO_MasterInitConfig0Extension.ssPin = 3;

    /* Initialize and configure clocks
     *     -   see clock manager component for details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    PINS_DRV_SetPins(GPIO_LED, RED_LED);

    /* Initialize SPI0 */
    SPI_MasterInit(&spi_pal_1_Instance, &FLEXIO_MasterInitConfig0);
     /* Initialize SPI1 */
    SPI_SlaveInit(&spi_pal_2_Instance, &LPSPI_0_SlaveInitConfig0);

    /* Infinite loop
     * Do nothing, when the timeout is reached it generates an
     * interrupt which toggles a led.
     */
    while(1)
    {
        SPI_SlaveTransfer(&spi_pal_2_Instance, slave_send, slave_receive, NUMBER_OF_FRAMES);
        SPI_MasterTransferBlocking(&spi_pal_1_Instance, master_send, master_receive, (uint8_t) NUMBER_OF_FRAMES, (uint8_t) TIMEOUT);
        /* verify that the data received is correct */
        frame_sent = 1;
        for(i = 0U; i < BUFFER_SIZE; i++)
        {
            if((slave_send[i] != master_receive[i]) || (master_send[i] != slave_receive[i]))
            {
                frame_sent = 0;
                break;
            }
        }
        if(frame_sent == 0)
        {
            PINS_DRV_ClearPins(GPIO_LED, RED_LED);
        }
        else
        {
            PINS_DRV_SetPins(GPIO_LED, RED_LED);
        }
    }

    /* Write your code here */
    /* For example: for(;;) { } */

    for(;;) 
    {
        if(exit_code != 0) 
        {
            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
 ** @}
 */
