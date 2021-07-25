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
**     Project     : can_pal_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-05, 11:48, # CodeGen: 2
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/



#include "sdk_project_config.h"
#include <interrupt_manager.h>
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

/******************************************************************************
 * Function prototypes
 ******************************************************************************/
void buttonISR(void);
void BoardInit(void);
void GPIOInit(void);

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
    bool sendFrame = false;

    if(buttonsPressed != 0)
    {

        /* Set FlexCAN TX value according to the button pressed */
        switch (buttonsPressed)
        {
            case (1 << BTN1_PIN):
                ledRequested = LED0_CHANGE_REQUESTED;
                sendFrame = true;
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

        if (sendFrame)
        {
            /* Set information about the data to be sent
             *  - Standard message ID
             *  - Bit rate switch enabled to use a different bitrate for the data segment
             *  - Flexible data rate enabled
             *  - Use zeros for FD padding
             */
            can_buff_config_t buffCfg =  {
                .enableFD = true,
                .enableBRS = true,
                .fdPadding = 0U,
                .idType = CAN_MSG_ID_STD,
                .isRemote = false
            };

            /* Configure TX buffer with index TX_MAILBOX*/
            CAN_ConfigTxBuff(&can_pal1_instance, TX_MAILBOX, &buffCfg);

            /* Prepare message to be sent */
            can_message_t message = {
                .cs = 0U,
                .id = TX_MSG_ID,
                .data[0] = ledRequested,
                .length = 1U
            };

            /* Send the information via CAN */
            CAN_Send(&can_pal1_instance, TX_MAILBOX, &message);
        }
    }
}

/*
 * @brief : Initialize clocks, pins and power modes
 */
void BoardInit(void)
{

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure FlexCAN clock, GPIO
     *  -   see clock manager component for more details
     */
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
    PINS_DRV_SetPinsDirection(GPIO_PORT, (1 << LED1) | (1 << LED0));

    /* Set Output value LEDs */
    PINS_DRV_SetPins(GPIO_PORT, (1 << LED1) | (1 << LED0));

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

volatile int exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{

    /* Do the initializations required for this application */
    BoardInit();
    GPIOInit();

    CAN_Init(&can_pal1_instance, &can_pal1_Config0);

    /* Set information about the data to be sent
     *  - Standard message ID
     *  - Bit rate switch enabled to use a different bitrate for the data segment
     *  - Flexible data rate enabled
     *  - Use zeros for FD padding
     */
    can_buff_config_t buffCfg =  {
        .enableFD = true,
        .enableBRS = true,
        .fdPadding = 0U,
        .idType = CAN_MSG_ID_STD,
        .isRemote = false
    };

    /* Configure RX buffer with index RX_MAILBOX */
    CAN_ConfigRxBuff(&can_pal1_instance, RX_MAILBOX, &buffCfg, RX_MSG_ID);

    while(1)
    {
        /* Define receive buffer */
        can_message_t recvMsg;

        /* Start receiving data in RX_MAILBOX. */
        CAN_Receive(&can_pal1_instance, RX_MAILBOX, &recvMsg);

        /* Wait until the previous FlexCAN receive is completed */
        while(CAN_GetTransferStatus(&can_pal1_instance, RX_MAILBOX) == STATUS_BUSY);

        /* Check the received message ID and payload */
        if((recvMsg.data[0] == LED0_CHANGE_REQUESTED) &&
                recvMsg.id == RX_MSG_ID)
        {
            /* Toggle output value LED0 */
            PINS_DRV_TogglePins(GPIO_PORT, (1 << LED0));
        }
        else if((recvMsg.data[0] == LED1_CHANGE_REQUESTED) &&
                recvMsg.id == RX_MSG_ID)
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

