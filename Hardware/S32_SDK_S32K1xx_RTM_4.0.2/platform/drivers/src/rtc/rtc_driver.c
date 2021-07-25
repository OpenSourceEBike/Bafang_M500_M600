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

#include "rtc_hw_access.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

/*!< Table of RTC base pointers */
static RTC_Type * const g_rtcBase[RTC_INSTANCE_COUNT] = RTC_BASE_PTRS;

/*!< @brief Table used to store the RTC IRQ names */
static const IRQn_Type       g_rtcIrqNumbers[]   = RTC_IRQS;
/*!< @brief Table used to store the RTC Seconds IRQ names */
static const IRQn_Type       g_rtcSecondsIrqNb[] = RTC_SECONDS_IRQS;

/* Table of month length (in days) for the Un-leap-year*/
static const uint8_t ULY[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/* Table of month length (in days) for the Leap-year*/
static const uint8_t LY[] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/* Number of days from begin of the non Leap-year*/
static const uint16_t MONTH_DAYS[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

/*!
 * @brief static RTC runtime structure, it is designed only for internal
 * purposes such as storing interrupt configuration for each instance.
 */
static struct
{
    rtc_alarm_config_t          * alarmConfig;      /*!< Alarm configuration                  */
    bool                          isAlarmTimeNew;   /*!< Check if there is a new alarm        */
    rtc_interrupt_config_t      * intConfig;        /*!< Interrupt configuration              */
    rtc_seconds_int_config_t    * secondsIntConfig; /*!< Time seconds interrupt configuration */

} g_rtcRuntimeConfig[RTC_INSTANCE_COUNT];


/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Init
 * Description   : This function initializes the RTC instance with the settings
 *                 provided by the user via the rtcUserCfg parameter. The user must ensure
 *                 that clock is enabled for the RTC instance used. If the Control register
 *                 is locked then this method returns STATUS_ERROR.
 *                 In order to clear the CR Lock the user must perform a power-on reset.
 * Return        : STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if Control Register is locked.
 * Implements    : RTC_DRV_Init_Activity
 *END**************************************************************************/
status_t RTC_DRV_Init(uint32_t instance, const rtc_init_config_t * const rtcUserCfg)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(rtcUserCfg != NULL);

    status_t statusCode = STATUS_SUCCESS;
    RTC_Type * basePtr = g_rtcBase[instance];

    /* Initialize runtime structure */
    g_rtcRuntimeConfig[instance].alarmConfig        = NULL;
    g_rtcRuntimeConfig[instance].intConfig          = NULL;
    g_rtcRuntimeConfig[instance].secondsIntConfig   = NULL;
    g_rtcRuntimeConfig[instance].isAlarmTimeNew     = false;

    /* Check if the control register is locked. If true, the method cannot
     * continue.
     */
    if (RTC_IsRegisterLocked(basePtr, RTC_CTRL_REG_LOCK) == true)
    {
        /* Return status code */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Disable the RTC instance IRQ to perform a software reset */
        INT_SYS_DisableIRQ(g_rtcIrqNumbers[instance]);
        /* Perform a software reset */
        RTC_SoftwareReset(basePtr);
        /* Set seconds register to 1 to clear TIF flag and stop generating interrupts */
        statusCode = RTC_SetTimeSecondsRegister(basePtr, 1U);
        /* Clear the pending interrupt generated by the software reset */
        INT_SYS_ClearPending(g_rtcIrqNumbers[instance]);
        /* Setup the RTC instance as configured in the structure */
        (void)RTC_ConfigureClockOut(basePtr, rtcUserCfg->clockOutConfig);
        RTC_SetLPOSelect(basePtr, rtcUserCfg->clockSelect);
        RTC_SetUpdateMode(basePtr, rtcUserCfg->updateEnable);
        RTC_SetNonSupervisorAccess(basePtr, rtcUserCfg->nonSupervisorAccessEnable);
        /* Check if compensation needs to be updated */
        if (rtcUserCfg->compensation != 0)
        {
        	RTC_SetTimeCompensation(basePtr,
        							rtcUserCfg->compensation,
                                    rtcUserCfg->compensationInterval);
        }
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Deinit
 * Description   : This function deinitializes the RTC instance.
 *                 If the Control register is locked then this method returns
 *                 STATUS_ERROR.
 *                 In order to clear the CR Lock the user must perform a power-on reset.
 * Return        : STATUS_SUCCESS if the operation was successful or
 *                 STATUS_ERROR if Control register is locked.
 * Implements    : RTC_DRV_Deinit_Activity
 *END**************************************************************************/
status_t RTC_DRV_Deinit(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    status_t statusCode = STATUS_SUCCESS;

    /* Check if the control register is locked. If true, the method cannot
     * continue.
     */
    if (RTC_IsRegisterLocked(g_rtcBase[instance], RTC_CTRL_REG_LOCK) == true)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Disable RTC instance's interrupts */
        INT_SYS_DisableIRQ(g_rtcIrqNumbers[instance]);
        INT_SYS_DisableIRQ(g_rtcSecondsIrqNb[instance]);
        /* Perform a software reset */
        RTC_SoftwareReset(g_rtcBase[instance]);
        /* Clear the pending interrupt generated by the software reset */
        INT_SYS_ClearPending(g_rtcIrqNumbers[instance]);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetDefaultConfig
 * Description   : This function will set the default configuration
 *        		   values into the structure passed as a parameter
 * Return        : None
 * Implements    : RTC_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void RTC_DRV_GetDefaultConfig(rtc_init_config_t * const config)
{
	DEV_ASSERT(config != NULL);

	config->clockSelect               = RTC_CLK_SRC_OSC_32KHZ;
	config->clockOutConfig            = RTC_CLKOUT_DISABLED;
	config->updateEnable              = true;
	config->nonSupervisorAccessEnable = true;
	config->compensation              = 0;
	config->compensationInterval      = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_CanWriteTCE
 * Description   : This function checks the following conditions to find if the
 *                 Time Counter Enable bit is writable.
 *                      - if Update Mode bitfield if 1 and:
 *                          - Time is invalid or
 *                          - Time Seconds Register has overflowed or
 *                          - Time Counter is disabled,
 *                      then the TCE bit can be set even if Status Register is locked.
 *
 *                 This method is a private one, it is used only by the API internally.
 * Return        : True if the TCE can be set, otherwise false
 *END**************************************************************************/
static bool RTC_DRV_CanWriteTCE(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    bool result = false;
    bool tifFlagSet;
    bool tofFlagSet;
    bool tceFlagSet;

    /* Check if the status register is locked */
    if (RTC_IsRegisterLocked(g_rtcBase[instance], RTC_STATUS_REG_LOCK) == false)
    {
        result = true;
    }
    /* Get the Update Mode bit */
    else if (RTC_GetUpdateMode(g_rtcBase[instance]))
    {
        tifFlagSet = RTC_GetTimeInvalidFlag(g_rtcBase[instance]);
        tofFlagSet = RTC_GetTimeOverflowFlag(g_rtcBase[instance]);
        tceFlagSet = RTC_GetTimeCounterEnable(g_rtcBase[instance]);

        /* Check for the specified conditions */
        if ((tifFlagSet == true) || (tofFlagSet == true) || (tceFlagSet == false))
        {
            result = true;
        }
    }
    else
    {
        result = false;
    }

    /* Return the exit code */
    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_StartCounter
 * Description   : Start RTC instance counter. Before calling this function the user
 *                 should use RTC_DRV_SetTimeDate to configure the start time
 * Return        : STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if the counter cannot be enabled or is already enabled.
 * Implements    : RTC_DRV_StartCounter_Activity
 *END**************************************************************************/
status_t RTC_DRV_StartCounter(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    status_t statusCode = STATUS_SUCCESS;

    /* Check if the TCE is writable and return corresponding status
     * if it is not
     */
    if (RTC_DRV_CanWriteTCE(instance) == false)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Enable the counter */
        statusCode = RTC_Enable(g_rtcBase[instance]);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_StopCounter
 * Description   : This function disables the RTC instance counter.
 * Return        : STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if the counter could not be stopped.
 * Implements    : RTC_DRV_StopCounter_Activity
 *END**************************************************************************/
status_t RTC_DRV_StopCounter(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    status_t statusCode = STATUS_SUCCESS;

    /* Check if the TCE is writable */
    if (RTC_DRV_CanWriteTCE(instance) == false)
    {
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Disable the RTC instance */
        statusCode = RTC_Disable(g_rtcBase[instance]);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetCurrentTimeDate
 * Description   : This retrieves the current time and date from the RTC instance.
 * Data is saved into currentTime, which is a pointer of the rtc_timedate_t
 * type.
 * Return        : STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if there was a problem.
 * Implements    : RTC_DRV_GetCurrentTimeDate_Activity
 *END**************************************************************************/
status_t RTC_DRV_GetCurrentTimeDate(uint32_t instance, rtc_timedate_t * const currentTime)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(currentTime != NULL);

    /* Declare local variables */
    status_t statusCode = STATUS_SUCCESS;
    uint32_t seconds;
    uint32_t tempSeconds;

    /* Make two consecutive reads to ensure that the read was not
     * done when the counter is incrementing.
     * This is recommended in the reference manual.
     */
    tempSeconds  = RTC_GetTimeSecondsRegister(g_rtcBase[instance]);
    seconds      = RTC_GetTimeSecondsRegister(g_rtcBase[instance]);
    /* If the read was done when the counter was incrementing,
     * try and read again.
     */
    if (tempSeconds != seconds)
    {
        /* Reinitialize the temporary variable */
        tempSeconds = 0UL;
        /* Get the current time again */
        tempSeconds = RTC_GetTimeSecondsRegister(g_rtcBase[instance]);
        if (tempSeconds != seconds)
        {
            /* If the last two reads are not equal, there is an error */
            statusCode = STATUS_ERROR;
        }
        else
        {
            /* Convert the current time from seconds to time date structure */
            RTC_DRV_ConvertSecondsToTimeDate(&seconds, currentTime);
        }
    }
    else
    {
        /* Convert the current time from seconds to time date structure */
        RTC_DRV_ConvertSecondsToTimeDate(&seconds, currentTime);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_SetTimeDate
 * Description   : This modifies the time and date of the RTC instance.
 * Return        : STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *                 if the time provided was invalid or if the counter was not
 *                 stopped.
 * Implements    : RTC_DRV_SetTimeDate_Activity
 *END**************************************************************************/
status_t RTC_DRV_SetTimeDate(uint32_t instance, const rtc_timedate_t * const time)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(time != NULL);

    /* Declare local variables */
    status_t statusCode = STATUS_SUCCESS;
    uint32_t seconds = 0;

    /* Check if the time is in the correct format */
    if (RTC_DRV_IsTimeDateCorrectFormat(time) == false)
    {
        /* Set the exit code to error */
        statusCode = STATUS_ERROR;
    }
    /* Check if the TCE bit is writable */
    else if (RTC_DRV_CanWriteTCE(instance) == false)
    {
        /* Set the exit code to locked */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Convert the desired time to seconds */
        RTC_DRV_ConvertTimeDateToSeconds(time, &seconds);
        /* Set the time */
        statusCode = RTC_SetTimeSecondsRegister(g_rtcBase[instance], seconds);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureRegisterLock
 * Description   : This method configures register lock for the corresponding
 *                 RTC instance. Remember that all the registers are unlocked
 *                 only by software reset or power on reset.
 *                 (Except for CR that is unlocked only by POR).
 * Return        : STATUS_SUCCESS if the operation was successful,
 *                 STATUS_ERROR if the Lock Register is locked.
 * Implements    : RTC_DRV_ConfigureRegisterLock_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigureRegisterLock(uint32_t instance, const rtc_register_lock_config_t * const lockConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(lockConfig != NULL);

    /* Declare local variables */
    status_t statusCode = STATUS_SUCCESS;

    /* Lock the requested registers, but always the Lock register last,
     * otherwise other registers can not be locked.
     */

    /* Configure Control register lock */
    if (lockConfig->controlRegisterLock)
    {
        statusCode = RTC_ConfigureRegisterLock(g_rtcBase[instance], RTC_CTRL_REG_LOCK);
    }
    /* Configure Status register lock */
    if (lockConfig->statusRegisterLock)
    {
        statusCode = RTC_ConfigureRegisterLock(g_rtcBase[instance], RTC_STATUS_REG_LOCK);
    }
    /* Configure Time Compensation register lock */
    if (lockConfig->timeCompensationRegisterLock)
    {
        statusCode = RTC_ConfigureRegisterLock(g_rtcBase[instance], RTC_TCL_REG_LOCK);
    }
    /* Configure Lock register lock */
    if (lockConfig->lockRegisterLock)
    {
        statusCode = RTC_ConfigureRegisterLock(g_rtcBase[instance], RTC_LOCK_REG_LOCK);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetRegisterLock
 * Description   : This retrieves the register lock configuration from the RTC
 *                 instance.
 *                 Data is stored in the structure referenced by the lockConfig
 *                 pointer.
 * Return        : None
 * Implements    : RTC_DRV_GetRegisterLock_Activity
 *END**************************************************************************/
void RTC_DRV_GetRegisterLock(uint32_t instance, rtc_register_lock_config_t * const lockConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(lockConfig != NULL);

    /* Get the configuration of the register lock */

    /* For the Lock Register */
    lockConfig->lockRegisterLock = RTC_IsRegisterLocked(g_rtcBase[instance], RTC_LOCK_REG_LOCK);
    /* For the Control Register */
    lockConfig->controlRegisterLock = RTC_IsRegisterLocked(g_rtcBase[instance], RTC_CTRL_REG_LOCK);
    /* For the Status Register */
    lockConfig->statusRegisterLock = RTC_IsRegisterLocked(g_rtcBase[instance], RTC_STATUS_REG_LOCK);
    /* For the Time Compensation Register */
    lockConfig->timeCompensationRegisterLock = RTC_IsRegisterLocked(g_rtcBase[instance], RTC_TCL_REG_LOCK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureTimeCompensation
 * Description   : This method configures time compensation. Data is passed by
 *                 the compInterval and compensation parameters.
 *                 For more details regarding coefficient calculation see the
 *                 Reference Manual.
 * Return        : STATUS_SUCCESS if the operation was successful,
 *                 STATUS_ERROR if the TC Register is locked.
 * Implements : RTC_DRV_ConfigureTimeCompensation_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigureTimeCompensation(uint32_t instance, uint8_t compInterval, int8_t compensation)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Declare local variables */
    status_t statusCode = STATUS_SUCCESS;

    /* Check if the TCR is locked */
    if (RTC_IsRegisterLocked(g_rtcBase[instance], RTC_TCL_REG_LOCK) == true)
    {
        /* Set the exit code to locked */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Set the corresponding values for compensation and compensation
         * interval.
         */
        RTC_SetTimeCompensation(g_rtcBase[instance], compensation, compInterval);
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetTimeCompensation
 * Description   : This retrieves the time compensation coefficients and saves
 *                 them on the variables referenced by the parameters.
 * Return        : None
 * Implements    : RTC_DRV_GetTimeCompensation_Activity
 *END**************************************************************************/
void RTC_DRV_GetTimeCompensation(uint32_t instance, uint8_t * compInterval, int8_t * compensation)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(compInterval != NULL);
    DEV_ASSERT(compensation != NULL);

    /* Get the compensation interval */
    RTC_GetCurrentTimeCompensation(g_rtcBase[instance], compensation, compInterval);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConvertSecondsToTimeDate
 * Description   : This method converts seconds into time-date format.
 * Return        : None
 * Implements    : RTC_DRV_ConvertSecondsToTimeDate_Activity
 *END**************************************************************************/
void RTC_DRV_ConvertSecondsToTimeDate(const uint32_t * const seconds, rtc_timedate_t * const timeDate)
{
    DEV_ASSERT(seconds != NULL);
    DEV_ASSERT(timeDate != NULL);

    /* Declare the variables needed */
    uint8_t i;
    bool yearLeap = false;
    uint32_t numberOfDays = 0U;
    uint32_t tempSeconds;
    uint16_t daysInYear;

    /* Because the starting year(1970) is not leap, set the daysInYear
     * variable with the number of the days in a normal year
     */
    daysInYear = DAYS_IN_A_YEAR;

    /* Set the year to the beginning of the range */
    timeDate->year = YEAR_RANGE_START;

    /* Get the number of days */
    numberOfDays = (*seconds) / SECONDS_IN_A_DAY;
    /* Get the number of seconds remaining */
    tempSeconds = (*seconds) % SECONDS_IN_A_DAY;

    /* Get the current hour */
    timeDate->hour        = (uint16_t)(tempSeconds / SECONDS_IN_A_HOUR);
    /* Get the remaining seconds */
    tempSeconds           = tempSeconds % SECONDS_IN_A_HOUR;
    /* Get the minutes */
    timeDate->minutes     = (uint16_t)(tempSeconds / SECONDS_IN_A_MIN);
    /* Get seconds */
    timeDate->seconds = (uint8_t)(tempSeconds % SECONDS_IN_A_MIN);

    /* Get the current year */
    while (numberOfDays >= daysInYear)
    {
        /* Increment year if the number of days is greater than the ones in
         * one year
         */
        timeDate->year++;
        /* Subtract the number of the days */
        numberOfDays -= daysInYear;

        /* Check if the year is leap or unleap */
        if (!RTC_DRV_IsYearLeap(timeDate->year))
        {
            /* Set the number of non leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_YEAR;
        }
        else
        {
            /* Set the number of leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_LEAP_YEAR;
        }
    }

    /* Add the current day */
    numberOfDays += 1U;

    /* Check if the current year is leap */
    yearLeap = RTC_DRV_IsYearLeap(timeDate->year);

    /* Get the month */
    for (i = 1U; i <= 12U; i++)
    {
        uint32_t daysInCurrentMonth = ((yearLeap == true) ? (uint32_t)LY[i] : (uint32_t)ULY[i]);
        if (numberOfDays <= daysInCurrentMonth)
        {
            timeDate->month = (uint16_t)i;
            break;
        }
        else
        {
            numberOfDays -= daysInCurrentMonth;
        }

    }

    /* Set the current day */
    timeDate->day = (uint16_t)numberOfDays;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConvertTimeDateToSeconds
 * Description   : This method converts time-date into seconds.
 * Return        : None
 * Implements    : RTC_DRV_ConvertTimeDateToSeconds_Activity
 *END**************************************************************************/
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t * const timeDate, uint32_t * const seconds)
{
    DEV_ASSERT(seconds != NULL);
    DEV_ASSERT(timeDate != NULL);

    /* Declare local variables */
    uint16_t year;

    /* Convert years to seconds */
    (*seconds) = (uint32_t)(DAYS_IN_A_YEAR * (uint32_t)(SECONDS_IN_A_DAY));
    (*seconds) *= ((uint32_t)timeDate->year - YEAR_RANGE_START);

    /* Add the seconds from the leap years */
    for (year = YEAR_RANGE_START; year < timeDate->year; year++)
    {
        if (RTC_DRV_IsYearLeap(year))
        {
            (*seconds) += SECONDS_IN_A_DAY;
        }
    }

    /* If the current year is leap and 29th of February has passed, add
     * another day to seconds passed.
     */
    if ((RTC_DRV_IsYearLeap(year)) && (timeDate->month > 2U))
    {
        (*seconds) += SECONDS_IN_A_DAY;
    }

    /* Add the rest of the seconds from the current month */
    (*seconds) += MONTH_DAYS[timeDate->month] * SECONDS_IN_A_DAY;
    /* Add the rest of the seconds from the current day */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->day - 1U) * (uint32_t)SECONDS_IN_A_DAY);
    /* Add the rest of the seconds from the current time */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->hour * SECONDS_IN_A_HOUR)   + \
                             ((uint32_t)timeDate->minutes * SECONDS_IN_A_MIN) + \
                             (uint32_t)timeDate->seconds);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsTimeDateCorrectFormat
 * Description   : This method checks if date-time structure is in a correct
 *                 format
 * Return        : True if the following conditions are met:
 *                  - is a valid year, month and date
 *                  - is a valid time format
 *                 False otherwise
 * Implements    : RTC_DRV_IsTimeDateCorrectFormat_Activity
 *END**************************************************************************/
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t * const timeDate)
{
    DEV_ASSERT(timeDate != NULL);

    /* Declare local variables */
    bool returnCode = true;
    const uint8_t * pDays;

    /* Set the days-in-month table for the corresponding year */
    pDays = RTC_DRV_IsYearLeap(timeDate->year) ? (LY) : (ULY);

    /* Check if the time and date are in the correct ranges */
    if ((timeDate->year < YEAR_RANGE_START) || (timeDate->year > YEAR_RANGE_END)
            ||  (timeDate->month < 1U) || (timeDate->month > 12U)
            ||  (timeDate->day < 1U) || (timeDate->day > 31U)
            ||  (timeDate->hour >= HOURS_IN_A_DAY)
            ||  (timeDate->minutes >= MINS_IN_A_HOUR) || (timeDate->seconds >= SECONDS_IN_A_MIN))
    {
        returnCode = false;
    }
    /* Check if the day is a valid day from the corresponding month */
    else if (timeDate->day > pDays[timeDate->month])
    {
        returnCode = false;
    }
    else
    {
        returnCode = true;
    }

    /* Return the exit code */
    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsYearLeap
 * Description   : This method checks if the year passed as a parameter is a leap
 *                 one.
 * Return        : True if the year is leap, false if otherwise.
 * Implements    : RTC_DRV_IsYearLeap_Activity
 *END**************************************************************************/
bool RTC_DRV_IsYearLeap(uint16_t year)
{
    bool isYearLeap = false;

    if ((year % 4U) > 0U)
    {
        isYearLeap = false;
    }
    else if ((year % 100U) > 0U)
    {
        isYearLeap = true;
    }
    else if ((year % 400U) > 0U)
    {
        isYearLeap = false;
    }
    else
    {
        isYearLeap = true;
    }

    /* Return the exit code */
    return isYearLeap;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IRQHandler
 * Description   : This method is the API's Interrupt handler for generic and
 *                 alarm IRQ. It will handle the alarm repetition and calls the
 *                 user callbacks if they are not NULL.
 * Return        : None
 *
 * Implements    : RTC_DRV_IRQHandler_Activity
 *END**************************************************************************/
void RTC_DRV_IRQHandler(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    uint32_t tempSeconds;
    /* Get the alarm configuration */
    rtc_alarm_config_t * alarmConfig = g_rtcRuntimeConfig[instance].alarmConfig;
    /* Get the fault interrupt configuration */
    const rtc_interrupt_config_t * const intConfig = g_rtcRuntimeConfig[instance].intConfig;

    /* Check if an alarm has occurred */
    if (RTC_GetTimeAlarmFlag(g_rtcBase[instance]) == true)
    {
        /* If the alarm interrupt configuration has been defined process the
         * alarm IRQ
         */
        if ((alarmConfig != NULL))
        {
            /* If recurrence is enabled modify the alarm register to the next
             * alarm.
             */
            if ((alarmConfig->numberOfRepeats > 0UL) || (alarmConfig->repeatForever == true))
            {
                /* Get current time */
            	tempSeconds = RTC_GetTimeSecondsRegister(g_rtcBase[instance]);
            	/* Current time is incremented with the repetition value */
                tempSeconds += alarmConfig->repetitionInterval - 1UL;
                /* Set new value to trigger the alarm */
                RTC_SetTimeAlarmRegister(g_rtcBase[instance], tempSeconds);

                g_rtcRuntimeConfig[instance].isAlarmTimeNew = true;
                /* If the alarm repeats forever, set number of repeats to 0
                 * to avoid an accidental trigger of the core overflow flag
                 */
                alarmConfig->numberOfRepeats = (alarmConfig->repeatForever == false) ? (alarmConfig->numberOfRepeats - 1UL) : 0UL;
            }
            else
            {
                /* If the alarm does not repeat, write 0 to TAR to clear the
                 * alarm flag.
                 */
                RTC_SetTimeAlarmRegister(g_rtcBase[instance], 0UL);
                /* Set the internal variable which indicates that a new alarm is enabled to false */
                g_rtcRuntimeConfig[instance].isAlarmTimeNew = false;
            }
            /* If the user has defined a callback, call it */
            if (alarmConfig->alarmCallback != NULL)
            {
                alarmConfig->alarmCallback(alarmConfig->callbackParams);
            }
        }
    }
    /* If the IRQ is not caused by the alarm then call the user callback if
     * defined.
     */
    else if ( intConfig != NULL )
    {	
        if ( intConfig->rtcCallback != NULL )
       {
           /* Call the RTC interrupt callback function with callback parameter */
        	intConfig->rtcCallback(intConfig->callbackParams);
       }
    }
    else
    {
      /* Do nothing*/
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_SecondsIRQHandler
 * Description   : This method is the API's Interrupt handler for RTC Second
 *                 interrupt. This ISR will call the user callback if defined.
 * Return        : None
 * Implements    : RTC_DRV_SecondsIRQHandler_Activity
 *END**************************************************************************/
void RTC_DRV_SecondsIRQHandler(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    const rtc_seconds_int_config_t * const intCfg = g_rtcRuntimeConfig[instance].secondsIntConfig;

    /* If the interrupt is configured by the driver API and the user callback
     * is not NULL, then call it.
     */
    if ((intCfg != NULL) && (intCfg->rtcSecondsCallback != NULL))
    {
    	/* Call the RTC Seconds interrupt callback function with callback parameter */
        intCfg->rtcSecondsCallback(intCfg->secondsCallbackParams);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureFaultInt
 * Description   : This method configures fault interrupts such as:
 *                  - Time Overflow Interrupt
 *                  - Time Invalid Interrupt
 *                 with the user provided configuration struct intConfig.
 * Return        : None
 * Implements    : RTC_DRV_ConfigureFaultInt_Activity
 *END**************************************************************************/
void RTC_DRV_ConfigureFaultInt(uint32_t instance, rtc_interrupt_config_t * const intConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(intConfig != NULL);

    /* Disable the IRQ to avoid accidental interrupt requests */
    INT_SYS_DisableIRQ(g_rtcIrqNumbers[instance]);
    /* Save the configuration into the instance's runtime structure */
    g_rtcRuntimeConfig[instance].intConfig = intConfig;

    /* Enable or disable selected interrupts */
    RTC_SetTimeOverflowIntEnable(g_rtcBase[instance], intConfig->overflowIntEnable);

    RTC_SetTimeInvalidIntEnable(g_rtcBase[instance], intConfig->timeInvalidIntEnable);

    /* After the configuration is done, re-enable the interrupt in NVIC */
    INT_SYS_EnableIRQ(g_rtcIrqNumbers[instance]);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureSecondsInt
 * Description   : This method configures the Time Seconds Interrupt with the
 *                 configuration from the intConfig parameter.
 * Return        : None
 * Implements    : RTC_DRV_ConfigureSecondsInt_Activity
 *END**************************************************************************/
void RTC_DRV_ConfigureSecondsInt(uint32_t instance, rtc_seconds_int_config_t * const intConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(intConfig != NULL);

    /* Disable the IRQ to avoid accidental interrupt requests */
    INT_SYS_DisableIRQ(g_rtcSecondsIrqNb[instance]);
    /* Disable the IRQ to avoid accidental interrupt requests */
    g_rtcRuntimeConfig[instance].secondsIntConfig = intConfig;

    /* Configure the interrupt frequency */
    RTC_SetTimeSecondsIntConf(g_rtcBase[instance], intConfig->secondIntConfig);

    /* Enable or disable Time Seconds interrupt */
    RTC_SetTimeSecondsIntEnable(g_rtcBase[instance], intConfig->secondIntEnable);

    /* After the configuration is done, re-enable the interrupt in NVIC */
    INT_SYS_EnableIRQ(g_rtcSecondsIrqNb[instance]);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureAlarm
 * Description   : This method configures the alarm with the
 *                 configuration from the alarmConfig parameter.
 *
 * Return        : STATUS_SUCCESS if the configuration is successful or
 *                 STATUS_ERROR if the alarm time is invalid.
 * Implements    : RTC_DRV_ConfigureAlarm_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigureAlarm(uint32_t instance, rtc_alarm_config_t * const alarmConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(alarmConfig != NULL);

    status_t statusCode = STATUS_SUCCESS;
    uint32_t alarmTime;
    uint32_t currentTime;

    /* Check if the alarm time is in a correct format */
    if (RTC_DRV_IsTimeDateCorrectFormat(&(alarmConfig->alarmTime)) == true)
    {
        /* Convert the time date to seconds */
        RTC_DRV_ConvertTimeDateToSeconds(&(alarmConfig->alarmTime), &alarmTime);
        /* Get current time in seconds */
        currentTime = RTC_GetTimeSecondsRegister(g_rtcBase[instance]);

        /* Check if the alarm time is greater than current time */
        if(alarmTime > currentTime)
        {
            /* Disable the IRQ to avoid accidental interrupt requests */
            INT_SYS_DisableIRQ(g_rtcIrqNumbers[instance]);
            g_rtcRuntimeConfig[instance].alarmConfig = alarmConfig;

            /* Write alarm time into Time Alarm Register */
            RTC_SetTimeAlarmRegister(g_rtcBase[instance], alarmTime);
            /* Enable/disable interrupt source based on the configuration */
            RTC_SetTimeAlarmIntEnable(g_rtcBase[instance], alarmConfig->alarmIntEnable);
            /* After the configuration is done, re-enable the interrupt in
             * NVIC.
             */
            INT_SYS_EnableIRQ(g_rtcIrqNumbers[instance]);
        }
        else
        {
            statusCode = STATUS_ERROR;
        }
    }
    else
    {
        statusCode = STATUS_ERROR;
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetAlarmConfig
 * Description   : This method retrieves the alarm configuration.
 * Return        : None
 * Implements    : RTC_DRV_GetAlarmConfig_Activity
 *END**************************************************************************/
void RTC_DRV_GetAlarmConfig(uint32_t instance, rtc_alarm_config_t * alarmConfig)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(alarmConfig != NULL);

    *alarmConfig = *(g_rtcRuntimeConfig[instance].alarmConfig);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsAlarmPending
 * Description   : This method specifies if an alarm has occurred.
 * Return        : True if an alarm has occurred, false if not.
 * Implements    : RTC_DRV_IsAlarmPending_Activity
 *END**************************************************************************/
bool RTC_DRV_IsAlarmPending(uint32_t instance)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Return the exit code */
    return RTC_GetTimeAlarmFlag(g_rtcBase[instance]);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetNextAlarmTime
 * Description   : This method retrieves the next alarm time;
 * Return        : STATUS_SUCCESS if the next alarm time is valid
 *                 STATUS_ERROR if there is no new alarm
 *
 * Implements    : RTC_DRV_GetNextAlarmTime_Activity
 *END**************************************************************************/
status_t RTC_DRV_GetNextAlarmTime(uint32_t instance, rtc_timedate_t * const alarmTime)
{
    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(alarmTime != NULL);

    status_t statusCode = STATUS_SUCCESS;
    uint32_t alarmInSec;
    /* Check if is a new alarm and if true update alarm time date format from time seconds */
    if (g_rtcRuntimeConfig[instance].isAlarmTimeNew == true)
    {
        alarmInSec = RTC_GetTimeAlarmRegister(g_rtcBase[instance]);
        RTC_DRV_ConvertSecondsToTimeDate(&alarmInSec, alarmTime);
    }
    else
    {
        statusCode = STATUS_ERROR;
    }
    /* Return the exit code */

    return statusCode;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
