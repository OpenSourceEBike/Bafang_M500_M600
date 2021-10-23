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
**     Project     : flexio_spi_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-13, 14:05, # CodeGen: 0
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
#include <stdbool.h>

/* Declare transfer size */
#define TRANSFER_SIZE 16u

/* Struct that defines RX and TX buffer arrays */
typedef struct
{
    uint8_t txBuffer[TRANSFER_SIZE];
    uint8_t rxBuffer[TRANSFER_SIZE];
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
    for(cnt = 0U; cnt < TRANSFER_SIZE; cnt++)
    {
        /* If the master flag is set, then the txBuffer will take the value of the counter,
         * else the value will be (TRANSFER_SIZE - Counter).
         * This approach is taken to make the data transfer more visible.
         */
        spiBuffer->txBuffer[cnt] = ((master == true) ? (cnt) : (TRANSFER_SIZE - cnt));
        spiBuffer->rxBuffer[cnt] = 0U;
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

    /* Allocate the memory necessary for the FlexIO state structures */
    flexio_device_state_t flexIODeviceState;
    flexio_spi_master_state_t flexIOSPIState_Master, flexIOSPIState_Slave;
    /* Declare the master and slave buffers */
    spi_buffer_t masterBuffer, slaveBuffer;
    volatile bool isTransferOk = true;
    uint8_t cnt;

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure FlexIO clock, Port clocks
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *  -   Init FlexIO pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Init FlexIO device */
    FLEXIO_DRV_InitDevice(INST_FLEXIO_SPI1, &flexIODeviceState);
    /* Call the init function for FlexIO SPI driver */
    FLEXIO_SPI_DRV_MasterInit(INST_FLEXIO_SPI1, &flexio_spi1_MasterConfig0, &flexIOSPIState_Master);
    /* Initialize Slave instance of the FlexIO SPI driver */
    FLEXIO_SPI_DRV_SlaveInit(INST_FLEXIO_SPI1, &flexio_spi1_SlaveConfig0, &flexIOSPIState_Slave);


    /* Initialize master and slave buffers */
    InitSPIBuffer(&masterBuffer, true);
    InitSPIBuffer(&slaveBuffer,  false);

    /* Signal to the Slave FlexIO SPI driver to start listening. Set the buffers and transfer size */
    FLEXIO_SPI_DRV_SlaveTransfer(&flexIOSPIState_Slave, slaveBuffer.txBuffer, slaveBuffer.rxBuffer, TRANSFER_SIZE);

    /* Start the transmission of data */
    FLEXIO_SPI_DRV_MasterTransferBlocking(&flexIOSPIState_Master,
                                          masterBuffer.txBuffer,
                                          masterBuffer.rxBuffer,
                                          TRANSFER_SIZE,
                                          1000UL);

    /* Check if transfer is completed with no errors */
    for (cnt = 0U; cnt < TRANSFER_SIZE; cnt++)
    {
        /* If the values are not equal, break the loop and set isTransferOk to false */
        if((masterBuffer.txBuffer[cnt] != slaveBuffer.rxBuffer[cnt]) || (masterBuffer.rxBuffer[cnt] != slaveBuffer.txBuffer[cnt]))
        {
            isTransferOk = false;

            /* Turn on Red LED if the data transfer is not correct */
            PINS_DRV_WritePin(LED_RED_PORT, LED_RED_PIN, 1u);
            PINS_DRV_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 0u);

            break;
        }
    }

    if(isTransferOk)
    {
        /* Turn on Green LED if the data transfer is correct */
        PINS_DRV_WritePin(LED_RED_PORT, LED_RED_PIN, 0u);
        PINS_DRV_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 1u);
    }


    /* Cast isTransferOk to avoid "set but not used" warnings */
    (void)isTransferOk;
    /* Test finished. */
    BKPT_ASM;

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
