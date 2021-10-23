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
**     Project     : lin_master_baremetal_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-02-17, 10:58, # CodeGen: 0
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


/* Including necessary configuration files. */
#include "sdk_project_config.h"

#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
volatile int exit_code = 0;
/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/
#define EVB
#define RevD /*S32K142 EVB -Q100 Rev D SCH-29701*/

#ifdef EVB
    #ifdef RevD
        #define INST_LIN_NUM          (0U)
    #else
        #define INST_LIN_NUM          (1U)
    #endif
    #define PORT_LED0_INDEX           (0u)
    #define PORT_LED1_INDEX           (15u)
    #define PORT_LED2_INDEX           (16u)
    #define PORT_LED0_MASK            (0x1u << PORT_LED0_INDEX)
    #define PORT_LED1_MASK            (0x1u << PORT_LED1_INDEX)
    #define PORT_LED2_MASK            (0x1u << PORT_LED2_INDEX)
    #define LED_GPIO_PORT             (PTD)
    #define PORT_BTN0_INDEX           (12u)
    #define PORT_BTN1_INDEX           (13u)
    #define PORT_BTN0_MASK            (0x1u << PORT_BTN0_INDEX)
    #define PORT_BTN1_MASK            (0x1u << PORT_BTN1_INDEX)
    #define BTN_GPIO_PORT             (PTC)
    #define BTN_PORT_NAME             (PORTC)
    #define BTN_PORT_IRQn             (PORTC_IRQn)
    #define USE_LIN_XCVR              (1U)
    #define LIN_XCVR_ENABLE_PIN       (9UL)
    #define LIN_XCVR_ENABLE_MASK      (0x1u << LIN_XCVR_ENABLE_PIN)
    #define LIN_XCVR_ENABLE_GPIO_PORT (PTE)
#else
    #define INST_LIN_NUM              (1U)
    #define PORT_LED0_INDEX           (0u)
    #define PORT_LED1_INDEX           (1u)
    #define PORT_LED2_INDEX           (2u)
    #define PORT_LED0_MASK            (0x1u << PORT_LED0_INDEX)
    #define PORT_LED1_MASK            (0x1u << PORT_LED1_INDEX)
    #define PORT_LED2_MASK            (0x1u << PORT_LED2_INDEX)
    #define LED_GPIO_PORT             (PTC)
    #define PORT_BTN0_INDEX           (12u)
    #define PORT_BTN1_INDEX           (13u)
    #define PORT_BTN0_MASK            (0x1u << PORT_BTN0_INDEX)
    #define PORT_BTN1_MASK            (0x1u << PORT_BTN1_INDEX)
    #define BTN_GPIO_PORT             (PTC)
    #define BTN_PORT_NAME             (PORTC)
    #define BTN_PORT_IRQn             (PORTC_IRQn)
#endif

/* (CLK (MHz)* timer period (us) / Prescaler) */
#define TIMER_COMPARE_VAL             (uint16_t)(2000U)
#define TIMER_TICKS_1US               (uint16_t)(4U)
#define DATA_SIZE                     (8U)
#define FRAME_SLAVE_RECEIVE_DATA      (1U)
#define FRAME_MASTER_RECEIVE_DATA     (2U)
#define FRAME_GO_TO_SLEEP             (3U)
#define TIMEOUT                       (500U)

uint16_t timerOverflowInterruptCount = 0U;
volatile bool wakeupSignalFlag = false;

/* Define for DATA buffer transmit */
uint8_t txBuff1[DATA_SIZE] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
uint8_t txBuff2[DATA_SIZE] = {0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11};
/* Define for DATA buffer receiver */
uint8_t rxBuff[DATA_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*!
 * @brief LPTMR Interrupt Service Routine
 * The ISR will call LIN timeout service every 500us
 */
void LPTMR_ISR(void)
{
    /* Timer Interrupt Handler */
    LIN_DRV_TimeoutService(INST_LIN_NUM);
    /* Increment overflow count */
    timerOverflowInterruptCount++;
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR_1);
}

/*!
 * @brief Callback function to get time interval in nano seconds
 * @param[out] ns - number of nanoseconds passed since the last call of the function
 * @return dummy value
 */
uint32_t lin1TimerGetTimeIntervalCallback0(uint32_t *ns)
{
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = LPTMR_DRV_GetCounterValueByCount(INST_LPTMR_1);
    *ns = ((uint32_t)(counterValue + timerOverflowInterruptCount * TIMER_COMPARE_VAL - previousCountValue)) * 1000U / TIMER_TICKS_1US;
    timerOverflowInterruptCount = 0UL;
    previousCountValue = counterValue;

    return 0UL;
}

/*!
 * @brief Interrupt Service Routine for buttons
 * Depending on which buttons were pressed LIN scheme will be
 * set to sleep mode or normal mode.
 */
void BTNPORT_IRQHandler(void)
{
    static uint8_t counter = 0;

    /* If SW2/Button 0 is pressed */
    if (PINS_DRV_GetPortIntFlag(BTN_PORT_NAME) & (1 << PORT_BTN0_INDEX))
    {
        /* Send header to bus */
        if((counter % 2U) == 1U)
        {
            LIN_DRV_MasterSendHeader(INST_LIN_NUM, FRAME_SLAVE_RECEIVE_DATA);
        }
        else
        {
            LIN_DRV_MasterSendHeader(INST_LIN_NUM, FRAME_MASTER_RECEIVE_DATA);
        }

        /* Clear flag */
        PINS_DRV_ClearPinIntFlagCmd(BTN_PORT_NAME, PORT_BTN0_INDEX);
    }

    /* Increment counter */
    counter++;

    /* If SW3/Button 1 is pressed */
    if (PINS_DRV_GetPortIntFlag(BTN_PORT_NAME) & (1U << PORT_BTN1_INDEX))
    {

        if(LIN_NODE_STATE_SLEEP_MODE == LIN_DRV_GetCurrentNodeState(INST_LIN_NUM))
        {
            /* Send wakeup signal */
            LIN_DRV_SendWakeupSignal(INST_LIN_NUM);
            /* Set wakeup signal flag */
            wakeupSignalFlag = true;
        }
        else
        {
            /* Send header to set the node to sleep mode */
            LIN_DRV_MasterSendHeader(INST_LIN_NUM, FRAME_GO_TO_SLEEP);

        }
        /* Clear flag */
        PINS_DRV_ClearPinIntFlagCmd(BTN_PORT_NAME, PORT_BTN1_INDEX);
    }

}

/**
* Func:     CallbackHandler()
* Desc:     Declare Callback handler function
*/
lin_callback_t CallbackHandler(uint32_t instance, lin_state_t * lin1_State)
{
    lin_callback_t callbackCurrent;
    callbackCurrent = lin1_State->Callback;
    (void)instance;

    switch (lin1_State->currentEventId)
    {
        case LIN_PID_OK:

            /* Set timeout */
            LIN_DRV_SetTimeoutCounter(INST_LIN_NUM, TIMEOUT);

            /* If PID is FRAME_SLAVE_RECEIVE_DATA, salve node will receive data from master node */
            if(FRAME_SLAVE_RECEIVE_DATA == lin1_State->currentId)
            {
                /* Call to Send Frame DATA Function */
                LIN_DRV_SendFrameData(INST_LIN_NUM, txBuff1, sizeof(txBuff1));
            }

            /* If PID is FRAME_MASTER_RECEIVE_DATA, master node will receive data */
            if(FRAME_MASTER_RECEIVE_DATA == lin1_State->currentId)
            {
                /* Call to Receive Frame DATA Function */
                LIN_DRV_ReceiveFrameData(INST_LIN_NUM, rxBuff, sizeof(rxBuff));
            }

            /* If PID is FRAME_GO_TO_SLEEP, salve node will go to sleep mode */
            if(FRAME_GO_TO_SLEEP == lin1_State->currentId)
            {
                /* Go to sleep mode */
                LIN_DRV_GoToSleepMode(INST_LIN_NUM);
            }
            break;
        case LIN_PID_ERROR:
            /* Go to idle mode */
            LIN_DRV_GoToSleepMode(INST_LIN_NUM);
            break;
        case LIN_TX_COMPLETED:
        case LIN_RX_COMPLETED:
            /* Go to idle mode */
            LIN_DRV_GotoIdleState(INST_LIN_NUM);
            break;
        case LIN_CHECKSUM_ERROR:
        case LIN_READBACK_ERROR:
        case LIN_FRAME_ERROR:
        case LIN_RECV_BREAK_FIELD_OK:
            /* Set timeout */
            LIN_DRV_SetTimeoutCounter(INST_LIN_NUM, TIMEOUT);
            break;
        case LIN_WAKEUP_SIGNAL:
            /* Set wakeup signal flag */
            wakeupSignalFlag = true;
            break;
        case LIN_SYNC_ERROR:
        case LIN_BAUDRATE_ADJUSTED:
        case LIN_NO_EVENT:
        case LIN_SYNC_OK:
        default:
        /* do nothing */
            break;
    }
    return callbackCurrent;
}

/*!
 * @brief Main LIN master task
 * This function will initialize the LIN interface and set the scheme
 * to the normal table.
 * After the LIN network is initialized, the function will monitor
 * the Motor temperature and will turn LEDs on and off depending
 * on the retrieved temperature.
 */
void lin_master_baremetal_task(void)
{
    uint8_t index;
    uint8_t byteRemain = 1U;
    bool receiveFlag = false;
    status_t status = STATUS_ERROR;

    /* Initialize LIN network interface */
    LIN_DRV_Init(INST_LIN_NUM, &lin1_InitConfig0, &lin1_State);

    /* Install callback function */
    LIN_DRV_InstallCallback(INST_LIN_NUM, (lin_callback_t)CallbackHandler);

    /* Infinite loop */
    for (;;)
    {
        status = LIN_DRV_GetReceiveStatus(INST_LIN_NUM, &byteRemain);

        if((status == STATUS_SUCCESS) && (0U == byteRemain) && (FRAME_MASTER_RECEIVE_DATA == lin1_State.currentId))
        {
            for(index = 0; index < DATA_SIZE; index++)
            {
                /* Check data receiving */
                if(rxBuff[index] == txBuff2[index])
                {
                    receiveFlag = true;
                }

                /* Clear Rx buffer */
                rxBuff[index] = 0x00;
            }

            /* Check data receiving */
            if (false == receiveFlag)
            {
                /* Turn off Green LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 0U);
                /* Turn on Red LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 1U);
                /* Turn off Blue LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 0U);
            }
            else
            {
                /* Turn on Green LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 1U);
                /* Turn off Red LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 0U);
                /* Turn off Blue LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 0U);
            }
        }

        /* Check wakeup signal flag */
        if (true == wakeupSignalFlag)
        {
            /* Clear wakeup signal flag */
            wakeupSignalFlag = false;
            /* Turn off Green LED */
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 0U);
            /* Turn off Red LED */
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 0U);
            /* Turn on Blue LED */
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 1U);
        }

        /* Check node state */
        if (LIN_NODE_STATE_SLEEP_MODE == LIN_DRV_GetCurrentNodeState(INST_LIN_NUM))
        {
            /* Turn off all LEDs */
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 0U);
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 0U);
            PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 0U);
        }
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

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure LPUART clock, GPIO clock
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *  -   Init LPUART and GPIO pins
     *  -   See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

#if USE_LIN_XCVR
    /* Set LIN transceiver sleep pin direction */
    PINS_DRV_SetPinsDirection(LIN_XCVR_ENABLE_GPIO_PORT, LIN_XCVR_ENABLE_MASK);
    /* Wake up LIN transceiver */
    PINS_DRV_SetPins(LIN_XCVR_ENABLE_GPIO_PORT, LIN_XCVR_ENABLE_MASK);
#endif

    /* Enable button interrupt */
    INT_SYS_InstallHandler(BTN_PORT_IRQn, BTNPORT_IRQHandler, (isr_t *)NULL);
    INT_SYS_EnableIRQ(BTN_PORT_IRQn);

    /* Initialize LPTMR */
    LPTMR_DRV_Init(INST_LPTMR_1, &lptmr_1_config0, false);
    INT_SYS_InstallHandler(LPTMR0_IRQn, LPTMR_ISR, (isr_t *)NULL);
    INT_SYS_EnableIRQ(LPTMR0_IRQn);
    LPTMR_DRV_StartCounter(INST_LPTMR_1);

    /* Start LIN master task */
    lin_master_baremetal_task();

  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}

/* END main */
/*!
** @}
*/
