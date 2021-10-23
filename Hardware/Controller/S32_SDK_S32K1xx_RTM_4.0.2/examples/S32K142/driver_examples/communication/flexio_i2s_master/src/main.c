/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MENU_MESSAGE "Press:\r\n\
\t1) [Master] Send data\r\n\
\t2) [Master] Received data\r\n\
Enter your input:\r\n"

#define SEPARATOR "==================================================================\r\n"

#define LED_PORT  (PTD)
#define LED_RED   (15u)
#define LED_GREEN (16u)

/* 2KB Transfer size, the value should be modulo 4, since the transfers are 32bit wide */
#define TRANSFER_SIZE (64u)

/* Master TX and RX buffers definition */
uint8_t masterTxBuffer[TRANSFER_SIZE];
uint8_t masterRxBuffer[TRANSFER_SIZE];
/* compare buffers definition */
uint8_t compareBuffer[TRANSFER_SIZE];

void print(const char *sourceStr)
{
    uint32_t bytesRemaining;

    /* Send data via LPUART */
    LPUART_DRV_SendData(INST_LPUART_1, (uint8_t *) sourceStr, strlen(sourceStr));
    /* Wait for transmission to be successful */
    while (LPUART_DRV_GetTransmitStatus(INST_LPUART_1, &bytesRemaining) != STATUS_SUCCESS){};
}

/* Function that reads the option chosen by the user */
void readOptionFromConsole(uint8_t * option)
{
    uint32_t bytesRemaining;
    bool strReceived = false;
    uint8_t i = 0;
    uint8_t buffer[255];

    while(strReceived == false)
    {
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LPUART_DRV_ReceiveData(INST_LPUART_1, &buffer[i], 1UL);
        /* Wait for transfer to be completed */
        while(LPUART_DRV_GetReceiveStatus(INST_LPUART_1, &bytesRemaining) != STATUS_SUCCESS){};
            strReceived = true;
    }

    /* Check for invalid options */
    if (buffer[0] < '1' || buffer[0] > '2')
    {
        print((const char *)"Invalid option!\r\n");
        *option = 0;
    }
    else
    {
        *option = buffer[0];
    }
}

int main(void)
{
    /* Variable used for storing user option */
    uint8_t option;

    /* FlexIO device state */
    flexio_device_state_t       flexIODeviceState;
    /* FlexIO I2S state structure for master and slave emulation */
    flexio_i2s_master_state_t   I2SMasterState;

    /* Counter used for initializing the buffers */
    uint32_t cnt;
    uint8_t isTransferOk;

    /* Initialize master and slave buffers for master send sequence */
    for(cnt = 0; cnt < TRANSFER_SIZE; cnt++)
    {
        masterTxBuffer[cnt] = (uint8_t)cnt;
        compareBuffer[cnt]  = (uint8_t)(TRANSFER_SIZE - cnt - 1u);
    }

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure FlexIO clock
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize LPUART for host - board configurations */
    LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpuart_1_InitConfig0);

    /* Initialize pins
     *  -   Init FlexIO I2S pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize eDMA module & channels */
    EDMA_DRV_Init(&dmaController_State,
                  &dmaController_InitConfig,
                  edmaChnStateArray,
                  edmaChnConfigArray,
                  EDMA_CONFIGURED_CHANNELS_COUNT);

    /* Initialize the FLEXIO device */
    FLEXIO_DRV_InitDevice(INST_FLEXIO_I2S_CONFIG_1, &flexIODeviceState);

    /*
     * Initialize FlexIO I2S Master driver
     *   - 1.4112 MHz clock speed
     *   - DMA transfer Type
     */
    FLEXIO_I2S_DRV_MasterInit(INST_FLEXIO_I2S_CONFIG_1, &flexio_i2s1_MasterConfig0, &I2SMasterState);

    while(1)
   {
        /* Print menu */
        print((const char *)MENU_MESSAGE);
        /* Read user option */
        readOptionFromConsole(&option);

        /* If the option is valid configure the CPU power mode accordingly */
        switch (option)
        {
        /* RUN */
        case '1':

            print((const char*)"[Master] Send data\r\n");

            /* Transfer Data to slave with DMA, using blocking method */
            FLEXIO_I2S_DRV_MasterSendDataBlocking(&I2SMasterState, masterTxBuffer, TRANSFER_SIZE, 100UL);

            break;

        case '2':
            isTransferOk = 1u;
            /* Initialize master and slave buffers for master send sequence */
            for(cnt = 0; cnt < TRANSFER_SIZE; cnt++)
            {
                masterRxBuffer[cnt] = 0U;
            }

            /* Red off */
            PINS_DRV_WritePin(LED_PORT, LED_RED, 0);
            /* Green off */
            PINS_DRV_WritePin(LED_PORT, LED_GREEN, 0);

            print((const char*)"[Master] Receive data\r\n");

            /* Transfer Data from slave with DMA, using blocking method */
            FLEXIO_I2S_DRV_MasterReceiveDataBlocking(&I2SMasterState, masterRxBuffer, TRANSFER_SIZE, 10000UL);

            /* Perform a comparison between the master and slave buffers, to check the
             * validity of the values transferred.
             * The Red LED will be turned if data does not match. For the case in which
             * the data was correctly transfered the Green LED will be lit.
             */
            for (cnt = 0; cnt < TRANSFER_SIZE; cnt++)
            {
                if (masterRxBuffer[cnt] != compareBuffer[cnt])
                {
                    isTransferOk = 0u;
                    break;
                }
            }

            /* Turn on Red or Green LED depend on the check result */
            PINS_DRV_WritePin(LED_PORT, LED_GREEN, isTransferOk);
            PINS_DRV_WritePin(LED_PORT, LED_RED, (1u - isTransferOk));

            break;

        default:
            /* This statement should not be reached */
            break;
        }
        /* Print separator */
        print((const char *)SEPARATOR);
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
