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
**     Project     : edma_transfer_s32k142
**     Processor   : S32K142
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
#include <stdio.h>
#include <stdbool.h>
#include "sdk_project_config.h"

#include <string.h>

/* Definitions */
#define EDMA_TIME_DELAY                     (1000U)
#define EDMA_LOOP_TRANSFER_NOF_BYTES        (64U)    /* Sum of bytes to be transferred;  */
#define EDMA_TRANSFER_OFFSET                (4)      /* Offset used to modify the source and/or destination address; */
#define EDMA_MINOR_LOOP_BYTE_COUNT          (4U)     /* Number of bytes to be transferred in a minor loop; */
#define EDMA_MAJOR_LOOP_NOF_MINOR_LOOPS     (16U)    /* Number of minor loops that creates a major loop; */

volatile int exit_code = 0;
/* Flag updated in eDMA ISR to signal transfer completion */
static uint32_t g_transferCompleteIteration;

/*! @brief eDma channel callbacks */
void ISR_VCH0_Callback(void *parameter, edma_chn_status_t status)
{
    (void)status;
    (void)parameter;
    g_transferCompleteIteration++; /* Increment each time the IRQ is executed */
}

/* Compare strings */
bool StringCompare(const uint8_t * str0, uint8_t * str1, uint32_t len)
{
    uint32_t cnt = 0U;
    for(cnt = 0U; cnt < len; cnt++)
    {
        if(str0[cnt] != str1[cnt])
        {
            return false;
        }
    }
    return true;
}

/* Time delay */
void Delay(uint32_t delay)
{
    while(delay > 0U)
    {
        delay--;
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
    static const uint8_t srcData[EDMA_LOOP_TRANSFER_NOF_BYTES] = "eDMA Loop Transfer configuration and transmission";
    static uint8_t dstData[EDMA_LOOP_TRANSFER_NOF_BYTES];
    static edma_loop_transfer_config_t loopTransferConfig;
    static edma_transfer_config_t transferConfig;
    uint32_t idx = 0U;
    bool result = false;

    /* Initialize clock module */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* Initialize eDMA module & channels */
    EDMA_DRV_Init(&dmaController_State,
                  &dmaController_InitConfig,
                  edmaChnStateArray,
                  edmaChnConfigArray,
                  EDMA_CONFIGURED_CHANNELS_COUNT);

    /* Initiate eDMA Loop Transfer */
    g_transferCompleteIteration = 0U;
    loopTransferConfig.majorLoopIterationCount = EDMA_MAJOR_LOOP_NOF_MINOR_LOOPS;
    transferConfig.srcAddr = (uint32_t)&srcData[0U];
    transferConfig.destAddr = (uint32_t)&dstData[0U];
    transferConfig.destOffset = EDMA_TRANSFER_OFFSET;                    /* Add offset to destination address after each transfer */
    transferConfig.srcOffset = EDMA_TRANSFER_OFFSET;                     /* Add offset to source address after each transfer */
    transferConfig.srcTransferSize = EDMA_TRANSFER_SIZE_4B;
    transferConfig.destTransferSize = EDMA_TRANSFER_SIZE_4B;
    transferConfig.srcModulo = EDMA_MODULO_OFF;
    transferConfig.destModulo = EDMA_MODULO_OFF;
    transferConfig.minorByteTransferCount = EDMA_MINOR_LOOP_BYTE_COUNT;  /* Number of bytes that form a minor loop */
    transferConfig.loopTransferConfig = &loopTransferConfig;
    transferConfig.interruptEnable = true;                               /* Enable interrupt after major loop completes */
    EDMA_DRV_ConfigLoopTransfer(dmaControllerChn0_Config.virtChnConfig, &transferConfig);

    /* Trigger software request for each minor loop */
    for(idx = 0U; idx < EDMA_MAJOR_LOOP_NOF_MINOR_LOOPS; idx++)
    {
        EDMA_DRV_TriggerSwRequest(dmaControllerChn0_Config.virtChnConfig);
        Delay(EDMA_TIME_DELAY);
    }

    /* Check the transferred string */
    result = StringCompare(srcData, dstData, EDMA_LOOP_TRANSFER_NOF_BYTES);
    if(result != true)
    {
    	DEV_ASSERT(false);
    }

    /* Check that major loop interrupt triggered only once */
    if(g_transferCompleteIteration != 1U)
    {
    	DEV_ASSERT(false);
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
