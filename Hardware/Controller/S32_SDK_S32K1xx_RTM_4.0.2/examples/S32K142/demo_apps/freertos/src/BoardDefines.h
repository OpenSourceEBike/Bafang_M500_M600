/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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

#ifndef _BOARD_DEFINES_H_
#define _BOARD_DEFINES_H_

#include <stdint.h>
#include <stdbool.h>

/* This file defines the specific ports and pins for Evaluation Board and
 * Validation Board. To use with Validation Board, comment the following
 * line
 */

#define EVB

#ifdef EVB
    #define LED1            15U
    #define LED2            16U
    #define LED_GPIO        PTD
    #define LED_PORT        PORTD
    #define LED_PORT_PCC    PCC_PORTD_CLOCK
    #define BTN_GPIO        PTC
    #define BTN_PIN         13U
    #define BTN_PORT        PORTC
    #define BTN_PORT_PCC    PCC_PORTC_CLOCK
    #define BTN_PORT_IRQn   PORTC_IRQn
#else
    #define LED1            0U
    #define LED2            1U
    #define LED_GPIO        PTC
    #define LED_PORT        PORTC
    #define LED_PORT_PCC    PCC_PORTC_CLOCK
    #define BTN_GPIO        PTC
    #define BTN_PIN         13U
    #define BTN_PORT        PORTC
    #define BTN_PORT_PCC    PCC_PORTC_CLOCK
    #define BTN_PORT_IRQn   PORTC_IRQn
#endif

#include "pins_driver.h"

static inline void boardSetup(void)
{
    /* Configure ports */
    PINS_DRV_SetMuxModeSel(LED_PORT, LED1,      PORT_MUX_AS_GPIO);
    PINS_DRV_SetMuxModeSel(LED_PORT, LED2,      PORT_MUX_AS_GPIO);
    PINS_DRV_SetMuxModeSel(BTN_PORT, BTN_PIN,   PORT_MUX_AS_GPIO);
#ifdef EVB
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN_PIN, PORT_INT_RISING_EDGE);
#else
    PINS_DRV_SetPinIntSel(BTN_PORT, BTN_PIN, PORT_INT_FALLING_EDGE);
#endif
}
#endif
