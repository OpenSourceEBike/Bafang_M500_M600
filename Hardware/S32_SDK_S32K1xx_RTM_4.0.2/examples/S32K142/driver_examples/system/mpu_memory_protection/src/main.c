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
**     Project     : mpu_memory_protection_s32k144w
**     Processor   : s32k144w
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

volatile int exit_code = 0;
/* User includes */

/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/
#define EVB

#ifdef EVB
    #define ON              1U         /* LED ON */
    #define OFF             0U         /* LED OFF */
    #define LED_GPIO        PTD        /* LED GPIO type */
    #define LED_RED         15U        /* pin PTD15 - LED RGB on DEV-KIT */
    #define LED_GREEN       16U        /* pin PTD16 - LED RGB on DEV-KIT */
    #define SW_GPIO         PTC        /* SW GPIO type */
    #define SW              12U        /* pin PTC12 - SW2_BTN0 on DEV-KIT */
#else
    #define ON              0U         /* LED ON */
    #define OFF             1U         /* LED OFF */
    #define LED_GPIO        PTC        /* LED GPIO type */
    #define LED_RED         0U         /* pin PTC0 - LED0 on Motherboard */
    #define LED_GREEN       1U         /* pin PTC1 - LED1 on Motherboard */
    #define SW_GPIO         PTC        /* SW GPIO type */
    #define SW              12U        /* pin PTC12 - SW2_BTN0 on Motherboard */
#endif

/* Protected address in Flash memory */
#define ADDRESS_PROTECT     0x0003FF04U
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

/* Expected error */
mpu_access_err_info_t expectedError =
{
    .master     = FEATURE_MPU_MASTER_CORE,            /* Error Core master */
    .attributes = MPU_DATA_ACCESS_IN_SUPERVISOR_MODE, /* Error data access in supervisor mode */
    .accessType = MPU_ERR_TYPE_READ,                  /* Error read access */
    .accessCtr  = 0xA000U,                            /* Error occurs on region 0 and region 2 */
    .addr       = ADDRESS_PROTECT,                    /* Error address */
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    .processorIdentification = 0U                     /* Error processor identifier */
#endif
};

/*!
 * @brief The error comparator
 *
 * @param[in] error1 The error access.
 * @param[in] error2 The error access.
 * @return Status
 *         - true : The errors are the same.
 *         - false: The errors are different.
 */
bool ErrorCompare(mpu_access_err_info_t error1, mpu_access_err_info_t error2)
{
    bool status = false;

    if ((error1.master == error2.master)
        && (error1.attributes == error2.attributes)
        && (error1.accessType == error2.accessType)
        && (error1.accessCtr  == error2.accessCtr)
        && (error1.addr == error2.addr))
    {
        status = true;
    #if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
        if(error1.processorIdentification != error2.processorIdentification)
        {
            status = false;
        }
    #endif
    }

    return status;
}

/* HardFault Handler */
void HardFault_Handler(void)
{
    /* Enables region 3 to grant Core read permission */
    MPU_DRV_EnableRegion(MPU_INST, 3U, true);
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
     bool status = false;
     uint32_t switchStatus = 0;
     status_t returnCode = STATUS_ERROR;
     volatile uint32_t test = 0U;
     mpu_access_err_info_t reportedError;

    /* ARM Disable Debug Exception and Monitor Control Register */
    *(uint32_t *)0xE000EDFC = 0x1000000;

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize LEDs and Button configuration */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* LEDs off */
    PINS_DRV_WritePin(LED_GPIO, LED_RED, OFF);
    PINS_DRV_WritePin(LED_GPIO, LED_GREEN, OFF);

    /* Check the initial state of the button */
    switchStatus = PINS_DRV_ReadPins(SW_GPIO) & (1U << SW);

    /* Initialize MPU module */
    returnCode = MPU_DRV_Init(MPU_INST, MPU_NUM_OF_REGION_CFG0, MPU_Cfg0);

    /* Check initialization */
    if (returnCode == STATUS_SUCCESS)
    {
        /* Turn on LED_GREEN */
        PINS_DRV_WritePin(LED_GPIO, LED_GREEN, ON);
    }

    /* Infinite loop */
    for(;;)
    {
        /* Check button */
        if (switchStatus != (PINS_DRV_ReadPins(SW_GPIO) & (1U << SW)))
        {
            /* Disable region 3 to ignore Core read permission */
            MPU_DRV_EnableRegion(MPU_INST, 3U, false);
        }
        /* Read address in flash memory where protected by MPU */
        test = *((uint32_t *)ADDRESS_PROTECT);
        /* Casting to void to avoid "Set but unused warnings" */
        (void)test;

        /* Get the detail of error access on slave port 0 */
        status = MPU_DRV_GetDetailErrorAccessInfo(MPU_INST, FEATURE_MPU_SLAVE_FLASH_BOOTROM, &reportedError);

        /* Check error status */
        if (status)
        {
            /* Compare with expected error */
            status = ErrorCompare(reportedError, expectedError);

            /* If true */
            if (status)
            {
                /* Turn on LED_RED and turn off LED_GREEN */
                PINS_DRV_WritePin(LED_GPIO, LED_RED, ON);
                PINS_DRV_WritePin(LED_GPIO, LED_GREEN, OFF);
                break;
            }
        }
    }
    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
