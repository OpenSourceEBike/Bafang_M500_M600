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

#include "sdk_project_config.h"

volatile int exit_code = 0;

void delay(volatile int cycles)
{
  /* Delay function - do nothing for a number of cycles */
  while(cycles--);
}

int main(void)
{
  uint32_t duty = 0;
  bool increaseDuty = true;
  uint8_t channel = pwm_pal_1_configs.pwmChannels[0].channel;
  status_t status = STATUS_SUCCESS;
  /* Initialize and configure clocks
   *  -   see clock manager component for details
   */
  status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(status == STATUS_SUCCESS);
  /* Initialize pins
   *  -   See PinSettings component for more info
   */
  status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(status == STATUS_SUCCESS);

  /* Initialize PWM */
  status = PWM_Init(&pwm_pal_1_instance, &pwm_pal_1_configs);
  DEV_ASSERT(status == STATUS_SUCCESS);

  while(1)
  {
    status = PWM_UpdateDuty(&pwm_pal_1_instance, channel, duty);
    DEV_ASSERT(status == STATUS_SUCCESS);
      if (increaseDuty == true)
         {
            duty++;
            if (duty > 4999)
                increaseDuty = false;
         }
      else
         {
            duty--;
            if (duty < 1U)
                increaseDuty = true;
         }
    delay(100U);
  }
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
}
