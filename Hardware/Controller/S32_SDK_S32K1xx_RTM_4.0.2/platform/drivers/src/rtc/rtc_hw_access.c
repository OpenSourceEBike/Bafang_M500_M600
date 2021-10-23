/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

/*!
 * @file rtc_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type.
 */

#include "rtc_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_Enable
 * Description   : This function enables the RTC counter
 * Return          STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if the counter is enabled.
 *END**************************************************************************/
status_t RTC_Enable(RTC_Type * const base)
{
    status_t statusCode = STATUS_SUCCESS;
    bool isCounterEnabled;

    /* Get the flags signaling if the counter is enabled */
    isCounterEnabled = RTC_GetTimeCounterEnable(base);

    /* Check if the RTC counter is enabled */
    if (isCounterEnabled)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Enable oscillator and seconds counter */
        RTC_SetTimeCounterEnable(base, true);
    }
    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_Disable
 * Description   : This function disables the RTC counter
 * Return          STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if the counter was not disabled.
 *END**************************************************************************/
status_t RTC_Disable(RTC_Type * const base)
{
    /* Check if counter is enabled and disable it */
	if (RTC_GetTimeCounterEnable(base) == true)
    {
        RTC_SetTimeCounterEnable(base, false);
    }

    /* Read TCE bit to check if the counter is really disabled and return the
     * corresponding result.
     *  -   Error if the timer is still enabled (The register can be locked)
     *  -   Success if the timer is disabled
     */
    return (status_t)(RTC_GetTimeCounterEnable(base) ? STATUS_ERROR : STATUS_SUCCESS);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_SetTimeSecondsRegister
 * Description   : This function will help you set the time at a specified value.
 *                 The write will fail if the Time Counter is enabled and will return
 *                 STATUS_ERROR, otherwise the return will be STATUS_SUCCESS
 * Return        : STATUS_SUCCESS if the write is succeeded or STATUS_ERROR if
 *                 the counter is enabled.
 *END**************************************************************************/
status_t RTC_SetTimeSecondsRegister(RTC_Type * const base, uint32_t seconds)
{
    status_t statusCode = STATUS_SUCCESS;

    if (RTC_GetTimeCounterEnable(base) == true)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        base->TSR  = seconds;
        statusCode = STATUS_SUCCESS;
    }
    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_ConfigureRegisterLock
 * Description   : This method will allow you to lock the registers. It will return
 *                 STATUS_SUCCESS if the lock was successful or if the register
 *                 was already locked, STATUS_ERROR if the Lock Register is
 *                 already locked or if the registerToConfig parameter is not a
 *                 valid register.
 * Return        : Status of the operation
 *END**************************************************************************/
status_t RTC_ConfigureRegisterLock(RTC_Type * const base, rtc_lock_register_select_t registerToConfig)
{
    status_t statusCode = STATUS_SUCCESS;

    /* Check if the Lock Register is already locked,
     * if true, any other register lock status cannot
     * be modified.
     */
    if (RTC_GetLockRegisterLock(base) == true)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* If the Lock Register is not locked we can
         * configure the register lock.
         */
        switch (registerToConfig)
        {
            case RTC_LOCK_REG_LOCK:
                RTC_LockRegisterLock(base);
                break;
            case RTC_STATUS_REG_LOCK:
                RTC_StatusRegisterLock(base);
                break;
            case RTC_CTRL_REG_LOCK:
                RTC_ControlRegisterLock(base);
                break;
            case RTC_TCL_REG_LOCK:
                RTC_TimeCompensationLock(base);
                break;
            default:
                /* If the register is not recognized, return error */
                statusCode = STATUS_ERROR;
                break;
        }
    }
    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_IsRegisterLocked
 * Description   : This method will get the register lock status
 * Return        : True if the register is locked, false if not
 *END**************************************************************************/
bool RTC_IsRegisterLocked(const RTC_Type * const base, rtc_lock_register_select_t reg)
{
    bool state = false;

    switch (reg)
    {
        case RTC_LOCK_REG_LOCK:
            state = RTC_GetLockRegisterLock(base);
            break;
        case RTC_CTRL_REG_LOCK:
            state = RTC_GetControlRegisterLock(base);
            break;
        case RTC_STATUS_REG_LOCK:
            state = RTC_GetStatusRegisterLock(base);
            break;
        case RTC_TCL_REG_LOCK:
            state = RTC_GetTimeCompensationLock(base);
            break;
        default:
            /* This statement should not be reached */
            break;
    }
    /* Return the exit code */
    return state;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_ConfigureClockOut
 * Description   : This method will allow you to configure the RTC Clock out pin.
 *                 It will return STATUS_SUCCESS if the configuration was successful
 *                 STATUS_ERROR if the Control Register is locked.
 * Return        : Status of the operation
 *END**************************************************************************/
status_t RTC_ConfigureClockOut(RTC_Type * const base, rtc_clk_out_config_t config)
{
    status_t statusCode = STATUS_SUCCESS;

    /*
     * Check if the Control Register is already locked,
     * if true, clock out configuration cannot be modified.
     */
    if (RTC_GetControlRegisterLock(base) == true)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        switch (config)
        {
            case RTC_CLKOUT_DISABLED:
                /* Disable the clock out pin */
                base->CR &= ~RTC_CR_CPE_MASK;
                break;
            case RTC_CLKOUT_SRC_TSIC:
                /* Select clock out source as Time Seconds Interrupt and enable the pin */
                base->CR &= ~(RTC_CR_CPE_MASK | RTC_CR_CPS_MASK);
                base->CR |= (RTC_CR_CPE(1U) | RTC_CR_CPS(0U));
                break;
            case RTC_CLKOUT_SRC_32KHZ:
                /* Select clock out source as the 32 KHz clock and enable the pin */
                base->CR &= ~(RTC_CR_CPE_MASK | RTC_CR_CPS_MASK);
                base->CR |= (RTC_CR_CPE(1U) | RTC_CR_CPS(1U));
                break;
            default:
                /* This statement should not be reached */
                break;
        }
    }
    /* Return the exit code */
    return statusCode;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
