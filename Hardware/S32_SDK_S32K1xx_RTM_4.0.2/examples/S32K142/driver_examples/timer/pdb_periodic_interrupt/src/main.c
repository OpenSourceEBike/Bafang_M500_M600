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

#define LED0_PORT PTD
#define LED0_PIN  15


#include "sdk_project_config.h"
#include <stdint.h>


#define TIMEOUT_US   1000000UL /* 1 second interval expressed in microseconds */


void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

/* The IRQ handler uses the default IRQ handler weak symbol defined in startup (.S) file */
void PDB0_IRQHandler(void)
{
  /* Clear PDB instance Interrupt flag */
  PDB_DRV_ClearTimerIntFlag(INST_PDB_0);

  /* Toggle the LED */
  PINS_DRV_TogglePins(LED0_PORT, (uint32_t) (1u << LED0_PIN));
}


bool calculateIntValue(const pdb_timer_config_t * const pdbConfig, const uint32_t uSec, uint16_t * const intVal);


int main(void)
{
  status_t status;
  uint16_t pdbTimerVal;

  /* Configure clocks for PORT */
  status = CLOCK_DRV_Init(&clockMan1_InitConfig0);
  DEV_ASSERT(status == STATUS_SUCCESS);

  /* Set pins as GPIO */
  status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
  DEV_ASSERT(status == STATUS_SUCCESS);

  /* Calculate the value needed for PDB timer to generate an interrupt at a specified timeout.
   * If the value can not be reached, stop the application flow */
  DEV_ASSERT(calculateIntValue(&pdb_config_1_timerConfig0, TIMEOUT_US, &pdbTimerVal) == true);

  /* Initialize PDB instance with the configuration structure
    *  - See PEx component for more details
    */
  PDB_DRV_Init(INST_PDB_0, &pdb_config_1_timerConfig0);

  /* Set the calculated values to PDB interrupt and modulus registers */
  PDB_DRV_SetTimerModulusValue(INST_PDB_0, pdbTimerVal);
  PDB_DRV_SetValueForTimerInterrupt(INST_PDB_0, pdbTimerVal);
  PDB_DRV_Enable(INST_PDB_0);
  /* Command the PDB instance to load the fresh values */
  PDB_DRV_LoadValuesCmd(INST_PDB_0);
  /* Software trigger the counter */
  PDB_DRV_SoftTriggerCmd(INST_PDB_0);

  for (;;)
  {
  }
}


/* @brief: Calculate the values to be used by pdb to generate
 *         interrupt after a give.
 * @param pdbConfig: pointer to the PDB configuration struct
 * @param uSec:      interval for pdb interrupt in microseconds
 * @param intVal:    pointer to the storage element where to set the calculated value
 * @return:          Returns true if the interrupt period can be achieved, false if not
 */
bool calculateIntValue(const pdb_timer_config_t * const pdbConfig, const uint32_t uSec, uint16_t * const intVal)
{
    /* Local variables used to store different parameters
     * such as frequency and prescalers
     */
    uint32_t    intVal_l            = 0;
    uint8_t     pdbPrescaler        = (1 << pdbConfig->clkPreDiv);
    uint8_t     pdbPrescalerMult    = 0;
    uint32_t    pdbFrequency;

    bool resultValid = false;

    /* Get the Prescaler Multiplier from the configuration structure */
    switch (pdbConfig->clkPreMultFactor)
    {
        case PDB_CLK_PREMULT_FACT_AS_1:
            pdbPrescalerMult    =   1U;
            break;
        case PDB_CLK_PREMULT_FACT_AS_10:
            pdbPrescalerMult    =   10U;
            break;
        case PDB_CLK_PREMULT_FACT_AS_20:
            pdbPrescalerMult    =   20U;
            break;
        case PDB_CLK_PREMULT_FACT_AS_40:
            pdbPrescalerMult    =   40U;
            break;
        default:
            /* Defaulting the multiplier to 1 to avoid dividing by 0*/
            pdbPrescalerMult    =   1U;
            break;
    }

    /* Get the frequency of the PDB clock source and scale it
     * so that the result will be in microseconds
     */
    CLOCK_SYS_GetFreq(CORE_CLK, &pdbFrequency);
    pdbFrequency /= 1000000;

    /* Calculate the interrupt value for the prescaler, multiplier, frequency
     * configured and time needed.
     */
    intVal_l = (pdbFrequency * uSec) / (pdbPrescaler * pdbPrescalerMult);

    /* Check if the value belongs to the interval */
    if((intVal_l == 0) || (intVal_l >= (1 << 16)))
    {
        resultValid = false;
        (*intVal) = 0U;
    }
    else
    {
        resultValid = true;
        (*intVal) = (uint16_t)intVal_l;
    }

    return resultValid;
}



