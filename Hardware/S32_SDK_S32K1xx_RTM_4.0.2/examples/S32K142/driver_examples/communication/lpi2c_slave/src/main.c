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


/* Including necessary configuration files. */
#include "sdk_project_config.h"

/* Definition of the data transfer size */
#define BUFF_SIZE 64

/* Definition LED */
#define LED_PORT  (PTD)
#define LED_RED   (15u)
#define LED_GREEN (16u)

/* Initialization of slave buffers */
/* compare buffers definition */
uint8_t compareBuffer[BUFF_SIZE];
/* Slave TX and RX buffers definition */
uint8_t slaveTxBuffer[BUFF_SIZE];
uint8_t slaveRxBuffer[BUFF_SIZE];
bool CheckResult = true;
/* Variable used for the loop that initializes the data buffers */
uint8_t cnt = 0;
/*!
 *  @brief LPI2C Slave Callback
 *
 *  @param [in] instance   LPI2C instance number
 *  @param [in] slaveEvent Event received on the I2C bus
 *  @param [in] userData   User defined data that is passed to the callback
 *  @return None
 *
 *  @details This function will be called by LPI2C interrupt handler and it
 *  will assign the buffer for TX or RX events.
 *  If an error event occurs, it will abort the current transfer.
 */
void lpi2c0_SlaveCallback0(i2c_slave_event_t slaveEvent,void *userData)
{
    /* Get the instance number from userData */
    uint32_t instance;
    instance = (uint32_t)userData;
    /* Depending on the event received, set the buffers or abort the transfer */
    switch(slaveEvent)
    {
        case I2C_SLAVE_EVENT_RX_REQ:
            /*
             * If the bus master requests data, then set the destination RX buffer
             * and accepted transfer size
             */
            LPI2C_DRV_SlaveSetRxBuffer(instance, slaveRxBuffer, BUFF_SIZE);
            break;
        case I2C_SLAVE_EVENT_TX_REQ:
            /*
             * If the bus master sends data, then set the source TX buffer
             * and accepted transfer size
             */
            LPI2C_DRV_SlaveSetTxBuffer(instance, slaveTxBuffer, BUFF_SIZE);
            break;
        case I2C_SLAVE_EVENT_TX_EMPTY:
                    /*
             * If the TX buffer is empty. Because the example does not handle 
             * this case there is no action taken.
             */
        case I2C_SLAVE_EVENT_RX_FULL:
            /*
             * If the RX buffer is full, check the slave receive buffer is correct
             */
            for (cnt = 0; cnt < BUFF_SIZE; cnt++)
            {
                if (slaveRxBuffer[cnt] != compareBuffer[cnt])
                {
                    CheckResult = false;
                    break;
                }
            }
            /* Turn on Red or Green LED depending on the check result */
            PINS_DRV_WritePin(LED_PORT, LED_RED, (1u - CheckResult));
            PINS_DRV_WritePin(LED_PORT, LED_GREEN, CheckResult);
            break;
        case I2C_SLAVE_EVENT_STOP:
            /*
             * This case is used when a stop condition is on the bus. Because
             * the example does not handle this case there is no action taken.
             */
            break;
    }
}


volatile int exit_code = 0;
/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Allocate memory for the LPI2C driver state structure */
    lpi2c_slave_state_t lpi2c0SlaveState;

    /* Initialize the data buffer */
    for(cnt = 0; cnt < BUFF_SIZE; cnt++)
    {
        compareBuffer[cnt] = (uint8_t)cnt;
        slaveTxBuffer[cnt]  = (uint8_t)(BUFF_SIZE - cnt - 1u);
        slaveRxBuffer[cnt]  = 0U;
    }
    /* Use as callback parameter the LPI2C instance number */
    lpi2c0_SlaveConfig0.callbackParam = (uint32_t *)INST_LPI2C0;

    /* Initialize and configure clocks */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize LPI2C Slave configuration
     *  - Slave listening enabled
     *  - Slave address 0x32
     *  - Fast operating mode
     *  -   See LPI2C components for configuration details
     */
    LPI2C_DRV_SlaveInit(INST_LPI2C0, &lpi2c0_SlaveConfig0, &lpi2c0SlaveState);
    /* End of the driver example */

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
