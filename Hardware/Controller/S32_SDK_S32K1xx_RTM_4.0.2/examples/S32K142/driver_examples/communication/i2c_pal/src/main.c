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
#include "sdk_project_config.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Definition of the data transfer size */
#define TRANSFER_SIZE 8

/* Definition LED */
#define LED_PORT  (PTD)
#define LED_RED   (15u)
#define LED_GREEN (16u)

/* Initialization of slave buffers */
uint8_t slaveTxBuffer[TRANSFER_SIZE] = { 0x0, 0x1, 0x02, 0x3, 0x4, 0x5, 0x6, 0x7};
uint8_t slaveRxBuffer[TRANSFER_SIZE] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/*!
 *  @brief I2C Slave Callback
 *
 *  @param [in] instance   I2C instance number
 *  @param [in] slaveEvent Event received on the I2C bus
 *  @param [in] userData   User defined data that is passed to the callback
 *  @return None
 *
 *  @details This function will be called by I2C interrupt handler and it
 *  will assign the buffer for TX or RX events.
 *  If an error event occurs, it will abort the current transfer.
 */
void i2c1_SlaveCallback0(i2c_slave_event_t slaveEvent, void *userData)
{
	/* Get instance number from userData */
	i2c_instance_t * instance;
	instance = (i2c_instance_t *) userData;

	/* Check the event type:
	 *  - set RX or TX buffers depending on the master request type
	 */
	if (slaveEvent == I2C_SLAVE_EVENT_RX_REQ)
		I2C_SlaveSetRxBuffer(instance, slaveRxBuffer, TRANSFER_SIZE);
	if (slaveEvent == I2C_SLAVE_EVENT_TX_REQ)
		I2C_SlaveSetTxBuffer(instance, slaveTxBuffer, TRANSFER_SIZE);
}

volatile int exit_code = 0;


int main(void)
{
	/* Declaration of the I2C transfer buffer */
	uint8_t masterTxBuffer[TRANSFER_SIZE];
	/* Variable that is used to initialize the buffers */
	uint8_t cnt;
	volatile bool isTransferOk = true;

	/* Variable used for the loop that initializes the data buffer */
	uint16_t i;

	/* Use as callback parameter for slave module the I2C instance number */
	i2c_pal_lpi2c0_SlaveConfig0.callbackParam = (uint32_t *) &lpi2c0_instance;

	/* Initialize and configure clocks
	 *  - Configure system clocks and dividers
	 *  - Configure LPI2C clock gating
	 *  -   see clock manager component for details
	 */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

	/* Initialize pins
	 *  - Configure I2C pins
	 *  -   See PinSettings component for more info
	 */
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

	/* Initializes I2C master for FlexIO
	 *  See I2C PAL component for configuration details
	 */
	I2C_MasterInit(&flexio_instance, &i2c_pal_flexio_MasterConfig0);

	 /* Initialize I2C slave instance for LPI2C driver
	  * See I2C PAL component for configuration details
	  */
	I2C_SlaveInit(&lpi2c0_instance, &i2c_pal_lpi2c0_SlaveConfig0);

    /* Initialize the data buffer */
    for (i = 0u; i < TRANSFER_SIZE; i++)
    {
        masterTxBuffer[i] = i;
    }

    /* FlexIO master sends masterTxBuffer to LPI2C0 configured as slave */
    I2C_MasterSendDataBlocking(&flexio_instance, masterTxBuffer, TRANSFER_SIZE, true, 0xFFFF);

    /* Check if transfer is completed with no errors */
    for (cnt = 0U; cnt < TRANSFER_SIZE; cnt++)
    {
        /* If the values are not equal, break the loop and set isTransferOk to false */
        if((masterTxBuffer[cnt] != slaveRxBuffer[cnt]))
        {
            isTransferOk = false;
            break;
        }
    }

    /* Turn on Red or Green LED depending on the check result */
    PINS_DRV_WritePin(LED_PORT, LED_RED, (1u - isTransferOk));
    PINS_DRV_WritePin(LED_PORT, LED_GREEN, isTransferOk);

    for(;;) {
      if(exit_code != 0) {
        break;
      }
    }
    return exit_code;
}
