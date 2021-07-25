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
#include <stdio.h>
#include <stdbool.h>
#include "sdk_project_config.h"
#include <stdint.h>
#include <string.h>

#define MENU_MESSAGE "Press:\r\n\
\t1) [Slave] Send data\r\n\
\t2) [Slave] Received data\r\n\
Enter your input:\r\n"

#define SEPARATOR "==================================================================\r\n"

/* Definition of the data transfer size */
#define BUFF_SIZE 64

/* Definition LED */
#define LED_PORT  (PTD)
#define LED_RED   (15u)
#define LED_GREEN (16u)

/* Initialization of slave buffers */
/* compare buffers definition */
uint16_t compareBuffer[BUFF_SIZE];
/* Slave TX and RX buffers definition */
uint16_t slaveTxBuffer[BUFF_SIZE];
uint16_t slaveRxBuffer[BUFF_SIZE];
bool CheckResult;

volatile int exit_code = 0;

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

    uint8_t cnt = 0;
    extension_flexio_for_i2s_t ext;

    /* Initialize the data buffer */
    for(cnt = 0; cnt < BUFF_SIZE; cnt++)
    {
        compareBuffer[cnt] = (uint16_t)cnt;
        slaveTxBuffer[cnt]  = (uint16_t)(BUFF_SIZE - cnt - 1u);
    }

    /* Initialize clock module */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize eDMA module & channels */
    EDMA_DRV_Init(&dmaController_State,
                  &dmaController_InitConfig,
                  edmaChnStateArray,
                  edmaChnConfigArray,
                  EDMA_CONFIGURED_CHANNELS_COUNT);

    /* Initialize LPUART for host - board configurations */
    LPUART_DRV_Init(INST_LPUART_1, &lpUartState1, &lpuart_1_InitConfig0);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Initialize I2S pal */
    i2s_pal2_Config0.extension = &ext;
    ext.sckPin = 4;
    ext.wsPin = 5;
    ext.txPin = 6;
    ext.rxPin = 7;
    I2S_Init(INST_I2S_PAL1, &i2s_pal2_Config0);

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

            print((const char*)"[Slave] Send data\r\n");

            /* Slave sends */
            I2S_SendDataBlocking(INST_I2S_PAL1, (const uint8_t*) slaveTxBuffer, BUFF_SIZE, 10000);

            break;
        case '2':
            /* Red off */
            PINS_DRV_WritePin(LED_PORT, LED_RED, 0);
            /* Green off */
            PINS_DRV_WritePin(LED_PORT, LED_GREEN, 0);
            CheckResult = true;

            /* Clear RX buffer */
            for(cnt = 0; cnt < BUFF_SIZE; cnt++)
            {
                slaveRxBuffer[cnt]  = 0U;
            }

            print((const char*)"[Slave] Receive data\r\n");

            /* Call slave receive function first */
            I2S_ReceiveDataBlocking(INST_I2S_PAL1, (uint8_t*) slaveRxBuffer, BUFF_SIZE, 10000);

            /* Perform a comparison between the master and slave buffers, to check the
             * validity of the values transferred.
             * The Red LED will be turned if data does not match. For the case in which
             * the data was correctly transfered the Green LED will be lit.
             */
            for (cnt = 0; cnt < BUFF_SIZE; cnt++)
            {
                if (slaveRxBuffer[cnt] != compareBuffer[cnt])
                {
                    CheckResult = false;
                    break;
                }
            }

            /* Turn on Red or Green LED depend on the check result */
            PINS_DRV_WritePin(LED_PORT, LED_RED, (1u - CheckResult));
            PINS_DRV_WritePin(LED_PORT, LED_GREEN, CheckResult);

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
