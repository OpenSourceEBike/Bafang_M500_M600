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
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define PBRIDGE_RTC_SLOT    29
#define EVB

/* Define PIN Ports */
#ifdef EVB
    #define ALARM           15U
    #define SECOND          16U
    #define LED_GPIO        PTD
    #define BTN_GPIO        PTC
    #define BTN_PIN         13U
    #define BTN_PORT        PORTC
    #define BTN_PORT_IRQn   PORTC_IRQn
#else
    #define ALARM           0U
    #define SECOND          1U
    #define LED_GPIO        PTC
    #define BTN_GPIO        PTC
    #define BTN_PIN         13U
    #define BTN_PORT        PORTC
    #define BTN_PORT_IRQn   PORTC_IRQn
#endif


/* Switch to supervisor mode */
void swSupervisorMode(void)
{
    __asm(" MOVS R0, #0x0 ");
    __asm(" MSR CONTROL, R0 ");
    __asm("DSB");
    __asm("ISB");
}

/* Switch to user mode */
void swUserMode(void)
{
    __asm(" MOVS R0, #0x1 ");
    __asm(" MSR CONTROL, R0 ");
    __asm("DSB");
    __asm("ISB");
}

/* SVC Exception interrupt */
void SVC_Handler(void)
{
    /* Switch to supervisor mode need to be done through an exception handler*/
    swSupervisorMode();
}

/* Time Seconds interrupt handler */
void secondsISR(void * callbackParam)
{
    /* Toggle the seconds led */
    PINS_DRV_TogglePins(LED_GPIO, (1 << SECOND));
}

/* Alarm interrupt handler */
void alarmISR(void * callbackParam)
{
    /* Toggle the alarm led */
    PINS_DRV_TogglePins(LED_GPIO, (1 << ALARM));
}

/* Button interrupt handler */
void buttonISR(void)
{
    /* Check if the button was pressed */
    if((PINS_DRV_GetPortIntFlag(BTN_PORT) & (1 << BTN_PIN)) != 0)
    {

        rtc_timedate_t tempTime;
        /* Get current time */
        RTC_DRV_GetCurrentTimeDate(RTCTIMER, &tempTime);
        /* Add to current time 5 seconds */
        tempTime.seconds += 5;
        /* Set new alarm time and date */
        rtcTimerAlarmCfg0.alarmTime = tempTime;
        /* Configure the alarm */
        RTC_DRV_ConfigureAlarm(RTCTIMER, &rtcTimerAlarmCfg0);
    }
    /* Clear interrupt flag */
    PINS_DRV_ClearPinIntFlagCmd(BTN_PORT, BTN_PIN);
}


/*!
  \brief The main function for the project.
*/
int main(void)
{
    uint8_t regIdx = PBRIDGE_RTC_SLOT/8;

    /* Initialize and configure clocks */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* Setup alarm pin and seconds pin as output */
    PINS_DRV_SetPinsDirection(LED_GPIO, (1 << ALARM) | (1 << SECOND));
    PINS_DRV_ClearPins(LED_GPIO, (1 << ALARM) | (1 << SECOND));

    /* Setup alarm button pin */
    PINS_DRV_SetPinsDirection(BTN_GPIO, ~(1 << BTN_PIN));

    /* Setup alarm pin interrupt */
#ifdef EVB
    /* Falling edge for EVB */
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN_PIN, PORT_INT_FALLING_EDGE);
#else
    /* Rising edge for Validation board */
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN_PIN, PORT_INT_RISING_EDGE);
#endif

    /* Initialize RTC instance
     *  - See RTC configuration component for options
     */
    RTC_DRV_Init(RTCTIMER, &rtcTimerInitCfg0);
    /* Configure RTC Time Seconds Interrupt */
    RTC_DRV_ConfigureSecondsInt(RTCTIMER, &rtcTimerSecIntCfg0);

    /* Set APIS to allow usermode access to RTC Memory Space */
    /* Clear bit position to grant usermode access level */
    AIPS->OPACR[regIdx] &= ~AIPS_OPACR_SP5_MASK;
    /* Switch to user mode */
    swUserMode();

    /* Set the time and date */
    RTC_DRV_SetTimeDate(RTCTIMER, &rtcTimerInitCfg0_StartTime);

    /* Start the RTC counter */
    RTC_DRV_StartCounter(RTCTIMER);

    /* Switch to supervisor mode by calling exception SVC_Handler */
    __asm("svc #0x32");

    /* Install button ISR */
    INT_SYS_InstallHandler(BTN_PORT_IRQn, &buttonISR, (isr_t*) 0);
    /* Enable button interrupt */
    INT_SYS_EnableIRQ(BTN_PORT_IRQn);
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
