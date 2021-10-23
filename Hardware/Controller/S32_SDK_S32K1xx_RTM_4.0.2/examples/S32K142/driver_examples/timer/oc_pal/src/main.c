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
**     Project     : oc_pal_s32k142
**     Processor   : S32K142_100
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-05-18, 14:05, # CodeGen: 0
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



/* Including needed modules to compile this module/procedure */
#include "sdk_project_config.h"
#include <stdio.h>

volatile int exit_code = 0;

void oc_pal1_channel_callBack0(oc_event_t event, void * userData)
{
    PINS_DRV_TogglePins(LED_GREEN_PORT, (1 << LED_GREEN_PIN));    /*Toggle RGB_GREEN led on the board*/
    (void)event;
    (void)userData;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Initialize and configure clocks */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);
    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Initialize oc_pal */
    OC_Init(&oc_pal_instance_0, &oc_pal_instance_0_InitConfig);
    for(;;) {
        if(exit_code != 0) {
            break;
        }
    }

    return exit_code;
}

/* END main */
