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
**     Project     : flexcan_encrypted_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-11, 14:41, # CodeGen: 1
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/

#include "sdk_project_config.h"
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define EVB

#ifdef EVB
    #define LED_PORT        PORTD
    #define GPIO_PORT       PTD
    #define PCC_INDEX       PCC_PORTD_INDEX
    #define LED0            15U
    #define LED1            16U
    #define LED2            0U

    #define BTN_GPIO        PTC
    #define BTN1_PIN        13U
    #define BTN2_PIN        12U
    #define BTN_PORT        PORTC
    #define BTN_PORT_IRQn   PORTC_IRQn
#else
    #define LED_PORT        PORTC
    #define GPIO_PORT       PTC
    #define PCC_INDEX       PCC_PORTC_INDEX
    #define LED0            0U
    #define LED1            1U
    #define LED2            2U

    #define BTN_GPIO        PTC
    #define BTN1_PIN        13U
    #define BTN2_PIN        12U
    #define BTN_PORT        PORTC
    #define BTN_PORT_IRQn   PORTC_IRQn
#endif

/* Use this define to specify if the application runs as master or slave */
#define MASTER
/* #define SLAVE */

/* Definition of the TX and RX message buffers depending on the bus role */
#if defined(MASTER)
    #define TX_MAILBOX  (1UL)
    #define TX_MSG_ID   (1UL)
    #define RX_MAILBOX  (0UL)
    #define RX_MSG_ID   (2UL)
#elif defined(SLAVE)
    #define TX_MAILBOX  (0UL)
    #define TX_MSG_ID   (2UL)
    #define RX_MAILBOX  (1UL)
    #define RX_MSG_ID   (1UL)
#endif

typedef enum
{
    LED0_CHANGE_REQUESTED = 0x00U,
    LED1_CHANGE_REQUESTED = 0x01U
} can_commands_list;

uint8_t ledRequested = (uint8_t)LED0_CHANGE_REQUESTED;

bool useEncryption = false;

/******************************************************************************
 * Function prototypes
 ******************************************************************************/
void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len);
void buttonISR(void);
void BoardInit(void);
void GPIOInit(void);
void FlexCANInit(void);

/******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * Button interrupt handler
 */
void buttonISR(void)
{
    /* Check if one of the buttons was pressed */
    uint32_t buttonsPressed = PINS_DRV_GetPortIntFlag(BTN_PORT) &
                                           ((1 << BTN1_PIN) | (1 << BTN2_PIN));
    uint32_t buttons = PINS_DRV_ReadPins(BTN_GPIO);
    bool sendFrame = false;

    if(buttonsPressed != 0)
    {

        /* Set FlexCAN TX value according to the button pressed */
        switch (buttonsPressed)
        {
            case (1 << BTN1_PIN):
                if (buttons & ((1 << BTN2_PIN)))
                {
                    useEncryption = !useEncryption;
                    PINS_DRV_TogglePins(GPIO_PORT, (1 << LED2));
                }
                else
                {
                    ledRequested = LED0_CHANGE_REQUESTED;
                    sendFrame = true;
                }
                /* Clear interrupt flag */
                PINS_DRV_ClearPinIntFlagCmd(BTN_PORT, BTN1_PIN);
                break;
            case (1 << BTN2_PIN):
                ledRequested = LED1_CHANGE_REQUESTED;
                sendFrame = true;
                /* Clear interrupt flag */
                PINS_DRV_ClearPinIntFlagCmd(BTN_PORT, BTN2_PIN);
                break;
            default:
                PINS_DRV_ClearPortIntFlagCmd(BTN_PORT);
                break;
        }

        if (useEncryption && sendFrame)
        {
            uint8_t plaintext[16] = {0,};
            uint8_t ciphertext[16];
            status_t stat;

            plaintext[0] = ledRequested;

            /* Encrypt data using AES-128 ECB and the first non-volatile user key */
            stat = CSEC_DRV_EncryptECB(CSEC_KEY_1, plaintext, 16UL, ciphertext, 1UL);

            if (stat == STATUS_SUCCESS)
            {
                /* Send the information via CAN */
                SendCANData(TX_MAILBOX, TX_MSG_ID, ciphertext, 16UL);
            }
        }
        else if (sendFrame)
        {
            /* Send the information via CAN */
            SendCANData(TX_MAILBOX, TX_MSG_ID, &ledRequested, 1UL);
        }
    }
}

/*
 * @brief: Send data via CAN to the specified mailbox with the specified message id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len)
{
    /* Set information about the data to be sent
     *  - 1 byte in length
     *  - Standard message ID
     *  - Bit rate switch enabled to use a different bitrate for the data segment
     *  - Flexible data rate enabled
     *  - Use zeros for FD padding
     */
    flexcan_data_info_t dataInfo =
    {
            .data_length = len,
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .enable_brs  = true,
            .fd_enable   = true,
            .fd_padding  = 0U
    };

    /* Configure TX message buffer with index TX_MSG_ID and TX_MAILBOX*/
    FLEXCAN_DRV_ConfigTxMb(INST_FLEXCAN_CONFIG_1, mailbox, &dataInfo, messageId);

    /* Execute send non-blocking */
    FLEXCAN_DRV_Send(INST_FLEXCAN_CONFIG_1, mailbox, &dataInfo, messageId, data);
}

/*
 * @brief : Initialize clocks, pins and power modes
 */
void BoardInit(void)
{

	/* Configure clocks for PORT */
	CLOCK_DRV_Init(&clockMan1_InitConfig0);


    /* Initialize pins
     *  -   Init FlexCAN and GPIO pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
}

/*
 * @brief Function which configures the LEDs and Buttons
 */
void GPIOInit(void)
{
    /* Output direction for LEDs */
    PINS_DRV_SetPinsDirection(GPIO_PORT, (1 << LED2) | (1 << LED1) | (1 << LED0));

    /* Set Output value LEDs */
    PINS_DRV_ClearPins(GPIO_PORT, 1 << LED1);
    PINS_DRV_SetPins(GPIO_PORT, 1 << LED2);

    /* Setup button pin */
    PINS_DRV_SetPinsDirection(BTN_GPIO, ~((1 << BTN1_PIN)|(1 << BTN2_PIN)));

    /* Setup button pins interrupt */
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN1_PIN, PORT_INT_RISING_EDGE);
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN2_PIN, PORT_INT_RISING_EDGE);

    /* Install buttons ISR */
    INT_SYS_InstallHandler(BTN_PORT_IRQn, &buttonISR, NULL);

    /* Enable buttons interrupt */
    INT_SYS_EnableIRQ(BTN_PORT_IRQn);
}

/*
 * @brief Initialize FlexCAN driver and configure the bit rate
 */
void FlexCANInit(void)
{
    /*
     * Initialize FlexCAN driver
     *  - 8 byte payload size
     *  - FD enabled
     *  - Bus clock as peripheral engine clock
     */
    FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);
}

volatile int exit_code = 0;

int main(void)
{


    /* Do the initializations required for this application */
    BoardInit();
    GPIOInit();

    FlexCANInit();

    CSEC_DRV_Init(&csecState);

    /* Set information about the data to be received
     *  - 1 byte in length
     *  - Standard message ID
     *  - Bit rate switch enabled to use a different bitrate for the data segment
     *  - Flexible data rate enabled
     *  - Use zeros for FD padding
     */
    flexcan_data_info_t dataInfo =
    {
            .data_length = 1U,
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .enable_brs  = true,
            .fd_enable   = true,
            .fd_padding  = 0U
    };

    /* Configure RX message buffer with index RX_MSG_ID and RX_MAILBOX */
    FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &dataInfo, RX_MSG_ID);

    while(1)
    {
        /* Define receive buffer */
        flexcan_msgbuff_t recvBuff;

        /* Start receiving data in RX_MAILBOX. */
        FLEXCAN_DRV_Receive(INST_FLEXCAN_CONFIG_1, RX_MAILBOX, &recvBuff);

        /* Wait until the previous FlexCAN receive is completed */
        while(FLEXCAN_DRV_GetTransferStatus(INST_FLEXCAN_CONFIG_1, RX_MAILBOX) == STATUS_BUSY);
        if (useEncryption)
		{
			/* Check the length of the received message */
			if (recvBuff.dataLen == 16)
			{
				status_t stat;

				/* Decrypt data using AES-128 ECB and the first non-volatile user key */
				stat = CSEC_DRV_DecryptECB(CSEC_KEY_1, recvBuff.data, 16UL, recvBuff.data, 1UL);

				if (stat != STATUS_SUCCESS)
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}

        /* Check the received message ID and payload */
        if((recvBuff.data[0] == LED0_CHANGE_REQUESTED) &&
                recvBuff.msgId == RX_MSG_ID)
        {
            /* Toggle output value LED0 */
            PINS_DRV_TogglePins(GPIO_PORT, (1 << LED0));
        }
        else if((recvBuff.data[0] == LED1_CHANGE_REQUESTED) &&
                recvBuff.msgId == RX_MSG_ID)
        {
            /* Toggle output value LED1 */
            PINS_DRV_TogglePins(GPIO_PORT, (1 << LED1));
        }
    }
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}



