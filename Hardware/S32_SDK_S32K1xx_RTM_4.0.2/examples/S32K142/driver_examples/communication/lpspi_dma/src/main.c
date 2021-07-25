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

/* User includes */
#include <stdint.h>
#include <stdbool.h>

#define TIMEOUT             1000U
#define NUMBER_OF_FRAMES    100U
#define RED_LED             (1<<15U)    /* RED_LED  PTD[15] */
#define GPIO_LED            PTD

/* Struct that defines RX and TX buffer arrays */
typedef struct
{
  uint8_t tx[NUMBER_OF_FRAMES];
  uint8_t rx[NUMBER_OF_FRAMES];
} spi_buffer_t;

/*!
* @brief Initialize the SPI buffer with different values for TX/RX
*
* @param spiBuffer Pointer to the buffer that will be initialized
* @param master    True if the buffer is used with the master device,
*                  False if not
*/
void InitSPIBuffer(spi_buffer_t * spiBuffer, bool master)
{
    uint8_t cnt;
    /* Fill the buffers */
    for(cnt = 0U; cnt < NUMBER_OF_FRAMES; cnt++)
    {
        /* If the master flag is set, then the txBuffer will take the value of the counter,
         * else the value will be (TRANSFER_SIZE - Counter).
         * This approach is taken to make the data transfer more visible.
         */
        spiBuffer->tx[cnt] = ((master == true) ? (cnt) : (NUMBER_OF_FRAMES - cnt));
        spiBuffer->rx[cnt] = 0U;
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
    spi_buffer_t master_buffer, slave_buffer;
    InitSPIBuffer(&master_buffer, true);
    InitSPIBuffer(&slave_buffer, false);
    /* Variable used for the loop that checks the data buffers */
    uint8_t i;
    uint8_t frame_sent = 1;
  
    /* Initialize and configure clocks
     *     -   see clock manager component for details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize DMA */
    EDMA_DRV_Init(&dmaController_State, &dmaController_InitConfig, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
    /* SPI master configuration: clock speed: 500 kHz, 8 bits/frame, LSB first */
    LPSPI_DRV_MasterInit(INST_LPSPI_1, &lpspi_1State, &lpspi_0_MasterConfig0);
    /* SPI slave configuration: 8 bits/frame, LSB first */
    LPSPI_DRV_SlaveInit(INST_LPSPI_2, &lpspi_2State, &lpspi_1_SlaveConfig0);
    /* Configure delay between transfer, delay between SCK and PCS and delay between PCS and SCK */
    LPSPI_DRV_MasterSetDelay(INST_LPSPI_1, 1, 1, 1);

    while(1)
    {
        LPSPI_DRV_SlaveTransfer(INST_LPSPI_2, slave_buffer.tx,slave_buffer.rx, NUMBER_OF_FRAMES);
        /* Start the blocking transfer */
        LPSPI_DRV_MasterTransferBlocking(INST_LPSPI_1, master_buffer.tx,master_buffer.rx, NUMBER_OF_FRAMES, TIMEOUT);

        for(i=0;i<NUMBER_OF_FRAMES;i++)
        {
            if((master_buffer.rx[i]  != slave_buffer.tx[i])||(slave_buffer.rx[i]  != master_buffer.tx[i]))
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
