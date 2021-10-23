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
**     Project     : lin_master_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-12, 10:58, # CodeGen: 0
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
#include "lin_cfg.h"
#include "lin_common_api.h"
#include "lin_diagnostic_service.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "peripherals_LPTMR_0.h"
#include "peripherals_LPUART_0.h"
#include "peripherals_linstack_config.h"

#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
volatile int exit_code = 0;

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
#define LPUART_PORT_NAME          (PORTB)
#define LPUART_Rx_INDEX           (0u)
#define LPUART_PORT_IRQn          (PORTB_IRQn)
#define USE_LIN_XCVR              (1)
#define LIN_XCVR_ENABLE_PIN       (9UL)
#define LIN_XCVR_ENABLE_MASK      (0x1u << LIN_XCVR_ENABLE_PIN)
#define LIN_XCVR_ENABLE_GPIO_PORT (PTE)

/* (CLK (MHz)* timer period (us) / Prescaler) */
#define TIMER_COMPARE_VAL (uint16_t)(2000U)
#define TIMER_TICKS_1US   (uint16_t)(4U)

#define MOTOR_SELECTION_INCREASE (1u)
#define MOTOR_SELECTION_DECREASE (2u)
#define MOTOR_SELECTION_STOP     (3u)

#define MOTOR1_OVER_TEMP   (200u)
#define MOTOR1_MAX_TEMP    (100u)
#define MOTOR1_MIN_TEMP    (30u)

uint16_t timerOverflowInterruptCount = 0U;
uint8_t Motor1_temp;

/*!
 * @brief LPTMR Interrupt Service Routine
 * The ISR will call LIN timeout service every 500us
 * and will provide the required tick for LIN (every 5 ms)
 */
void LPTMR_ISR(void)
{
    /* Static variable, used to count if the timeout has passed to
     * provide the LIN scheme tick.
     */
    static uint32_t interruptCount = 0UL;
    /* Timer Interrupt Handler */
    lin_lld_timeout_service(LI0);

    /* If 5 ms have passed, provide the required tick */
    if(++interruptCount == 10UL)
    {
        l_sch_tick(LI0);
        interruptCount = 0UL;
    }

    /* Increment overflow count */
    timerOverflowInterruptCount++;
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(INST_LPTMR_0);
}

/*!
 * @brief Callback function to get time interval in nano seconds
 * @param[out] ns - number of nanoseconds passed since the last call of the function
 * @return dummy value
 */
uint32_t lin0TimerGetTimeIntervalCallback0(uint32_t *ns)
{
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = LPTMR_DRV_GetCounterValueByCount(INST_LPTMR_0);
    *ns = ((uint32_t)(counterValue + timerOverflowInterruptCount * TIMER_COMPARE_VAL - previousCountValue)) * 1000 / TIMER_TICKS_1US;
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
    /* If SW2/Button 1 is pressed */
    if (PINS_DRV_GetPortIntFlag(BTN_PORT_NAME) & (1 << PORT_BTN0_INDEX))
    {
        PINS_DRV_ClearPinIntFlagCmd(BTN_PORT_NAME, PORT_BTN0_INDEX);
        l_sch_set(LI0, LI0_GOTO_SLEEP_SCHEDULE, 0u);
    }

    /* If SW3/Button 2 is pressed */
    if (PINS_DRV_GetPortIntFlag(BTN_PORT_NAME) & (1 << PORT_BTN1_INDEX))
    {
        PINS_DRV_ClearPinIntFlagCmd(BTN_PORT_NAME, PORT_BTN1_INDEX);
        /* Toggle RED Led */
        PINS_DRV_TogglePins(LED_GPIO_PORT, PORT_LED1_MASK);
        /* Switch to normal table */
        l_ifc_wake_up(LI0);
        l_sch_set(LI0, LI0_NormalTable, 0u);
    }
}

/*!
 * @brief Main LIN master task
 * This function will initialize the LIN interface and set the scheme
 * to the normal table.
 * After the LIN network is initialized, the function will monitor
 * the Motor temperature and will turn LEDs on and off depending
 * on the retrieved temperature.
 */
void lin_master_task(void)
{
    /* Initialize LIN network interface */
    l_sys_init();
    l_ifc_init(LI0);
    /* Set Schedule table to Normal */
    l_sch_set(LI0, LI0_NormalTable, 0u);
    /* Infinite loop */
    for (;;)
    {
        /* Check if information about the Motor1 Temp has been received */
        if (l_flg_tst_LI0_Motor1Temp_flag())
        {
            /* Clear this flag... */
            l_flg_clr_LI0_Motor1Temp_flag();

            /* Store temperature data */
            Motor1_temp = l_u8_rd_LI0_Motor1Temp();

            /* Request stop motor by power off */
            if (MOTOR1_OVER_TEMP < Motor1_temp)
            {
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_STOP);
                /* Turn on Red LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 1U);
                /* Turn off Blue LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 0U);
                /* Turn off Green LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 0U);
            }
            /* Request to reduce motor speed */
            else if ((MOTOR1_MAX_TEMP < Motor1_temp) && (MOTOR1_OVER_TEMP > Motor1_temp))
            {
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_DECREASE);
                /* Turn on Blue LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 1U);
                /* Turn off Red LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 0U);
                /* Turn off Green LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 0U);
            }
            /* Request to increase motor speed if users request */
            else
            {
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_INCREASE);
                /* Turn on Green LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED2_INDEX, 1U);
                /* Turn off Red LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED1_INDEX, 0U);
                /* Turn off Blue LED */
                PINS_DRV_WritePin(LED_GPIO_PORT, PORT_LED0_INDEX, 0U);
            }
        }
         /* Check node state */
        if (LIN_NODE_STATE_SLEEP_MODE == lin_lld_get_state(LI0))
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
    LPTMR_DRV_Init(INST_LPTMR_0, &LPTMR0_config0, false);
    INT_SYS_InstallHandler(LPTMR0_IRQn, LPTMR_ISR, (isr_t *)NULL);
    INT_SYS_EnableIRQ(LPTMR0_IRQn);
    LPTMR_DRV_StartCounter(INST_LPTMR_0);

    /* Start LIN master task */
    lin_master_task();

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
