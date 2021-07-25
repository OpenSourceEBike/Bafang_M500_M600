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

#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include <stddef.h>
#include "status.h"
#include "interrupt_manager.h"

/*!
 * @addtogroup rtc_driver RTC Driver
 * @ingroup rtc
 * @brief Real Time Clock Peripheral Driver
 * @details This section describes the programming interface of the RTC driver.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SECONDS_IN_A_DAY     (86400UL)
#define SECONDS_IN_A_HOUR    (3600U)
#define SECONDS_IN_A_MIN     (60U)
#define MINS_IN_A_HOUR       (60U)
#define HOURS_IN_A_DAY       (24U)
#define DAYS_IN_A_YEAR       (365U)
#define DAYS_IN_A_LEAP_YEAR  (366U)
#define YEAR_RANGE_START     (1970U)
#define YEAR_RANGE_END       (2099U)

/*!
 * @brief RTC Seconds interrupt configuration
 * Implements : rtc_second_int_cfg_t_Class
 */
typedef enum
{
    RTC_INT_1HZ   = 0x00U,     /*!< RTC seconds interrupt occurs at 1 Hz      */
    RTC_INT_2HZ   = 0x01U,     /*!< RTC seconds interrupt occurs at 2 Hz      */
    RTC_INT_4HZ   = 0x02U,     /*!< RTC seconds interrupt occurs at 4 Hz      */
    RTC_INT_8HZ   = 0x03U,     /*!< RTC seconds interrupt occurs at 8 Hz      */
    RTC_INT_16HZ  = 0x04U,     /*!< RTC seconds interrupt occurs at 16 Hz     */
    RTC_INT_32HZ  = 0x05U,     /*!< RTC seconds interrupt occurs at 32 Hz     */
    RTC_INT_64HZ  = 0x06U,     /*!< RTC seconds interrupt occurs at 64 Hz     */
    RTC_INT_128HZ = 0x07U      /*!< RTC seconds interrupt occurs at 128 Hz    */
} rtc_second_int_cfg_t;

/*!
 * @brief RTC CLKOUT pin configuration
 * Implements : rtc_clk_out_config_t_Class
 */
typedef enum
{
    RTC_CLKOUT_DISABLED  = 0x00U,  /*!< Clock out pin is disabled                                    */
    RTC_CLKOUT_SRC_TSIC  = 0x01U,  /*!< Output on RTC_CLKOUT as configured on Time seconds interrupt */
    RTC_CLKOUT_SRC_32KHZ = 0x02U   /*!< Output on RTC_CLKOUT of the 32KHz clock                      */
} rtc_clk_out_config_t;

/*!
 * @brief RTC clock select
 * Implements : rtc_clk_select_t_Class
 */
typedef enum
{
    RTC_CLK_SRC_OSC_32KHZ = 0x00U, /*!< RTC Prescaler increments using 32 KHz crystal  */
    RTC_CLK_SRC_LPO_1KHZ  = 0x01U  /*!< RTC Prescaler increments using 1KHz LPO        */
} rtc_clk_select_t;

/*!
 * @brief RTC register lock
 * Implements : rtc_lock_register_select_t_Class
 */
typedef enum
{
    RTC_LOCK_REG_LOCK   = 0x00U,   /*!< RTC Lock Register lock         */
    RTC_STATUS_REG_LOCK = 0x01U,   /*!< RTC Status Register lock       */
    RTC_CTRL_REG_LOCK   = 0x02U,   /*!< RTC Control Register lock      */
    RTC_TCL_REG_LOCK    = 0x03U    /*!< RTC Time Compensation Reg lock */
} rtc_lock_register_select_t;

/*!
 * @brief RTC Time Date structure
 * Implements : rtc_timedate_t_Class
 */
typedef struct
{
    uint16_t year;      /*!< Year       */
    uint16_t month;     /*!< Month      */
    uint16_t day;       /*!< Day        */
    uint16_t hour;      /*!< Hour       */
    uint16_t minutes;   /*!< Minutes    */
    uint8_t seconds;    /*!< Seconds    */
} rtc_timedate_t;

/*!
 * @brief RTC Initialization structure
 * Implements : rtc_init_config_t_Class
 */
typedef struct
{
    uint8_t                 compensationInterval;       /*!< Compensation Interval                                                             */
    int8_t                  compensation;               /*!< Compensation Value                                                                */
    rtc_clk_select_t        clockSelect;                /*!< RTC Clock Select                                                                  */
    rtc_clk_out_config_t    clockOutConfig;             /*!< RTC Clock Out Source                                                              */
    bool                    updateEnable;               /*!< Enable changing the Time Counter Enable bit even if the Status register is locked */
    bool                    nonSupervisorAccessEnable;  /*!< Enable writes to the registers in non Supervisor Mode                             */
} rtc_init_config_t;

/*!
 * @brief RTC alarm configuration
 * Implements : rtc_alarm_config_t_Class
 */
typedef struct
{
    rtc_timedate_t  alarmTime;                                /*!< Alarm time                                       */
    uint32_t        repetitionInterval;                       /*!< Interval of repetition in sec                    */
    uint32_t        numberOfRepeats;                          /*!< Number of alarm repeats                          */
    bool            repeatForever;                            /*!< Repeat forever if set, discard number of repeats */
    bool            alarmIntEnable;                           /*!< Enable alarm interrupt                           */
    void            (* alarmCallback)(void * callbackParam);  /*!< Pointer to the user callback method.             */
    void            * callbackParams;                         /*!< Pointer to the callback parameters.              */
} rtc_alarm_config_t;

/*!
 * @brief RTC interrupt configuration. It is used to configure interrupt other
 *        than Time Alarm and Time Seconds interrupt
 * Implements : rtc_interrupt_config_t_Class
 */
typedef struct
{
    bool                 overflowIntEnable;                       /*!< Enable Time Overflow Interrupt       */
    bool                 timeInvalidIntEnable;                    /*!< Enable Time Invalid Interrupt        */
    void                (* rtcCallback)(void * callbackParam);    /*!< Pointer to the user callback method. */
    void                * callbackParams;                         /*!< Pointer to the callback parameters.  */
} rtc_interrupt_config_t;

/*!
 * @brief RTC Seconds Interrupt Configuration
 * Implements : rtc_seconds_int_config_t_Class
 */
typedef struct
{
    rtc_second_int_cfg_t secondIntConfig;                             /*!< Seconds Interrupt frequency          */
    bool                 secondIntEnable;                             /*!< Seconds Interrupt enable             */
    void                (* rtcSecondsCallback)(void * callbackParam); /*!< Pointer to the user callback method. */
    void                * secondsCallbackParams;                      /*!< Pointer to the callback parameters.  */
} rtc_seconds_int_config_t;

/*!
 * @brief RTC Register Lock Configuration
 * Implements : rtc_register_lock_config_t_Class
 */
typedef struct
{
    bool lockRegisterLock;              /*!< Lock state of the Lock Register              */
    bool statusRegisterLock;            /*!< Lock state of the Status Register            */
    bool controlRegisterLock;           /*!< Lock state of the Control Register           */
    bool timeCompensationRegisterLock;  /*!< Lock state of the Time Compensation Register */
} rtc_register_lock_config_t;

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief This function initializes the RTC instance with the settings
 *        provided by the user via the rtcUserCfg parameter. The user must ensure
 *        that clock is enabled for the RTC instance used. If the Control register
 *        is locked then this method returns STATUS_ERROR.
 *        In order to clear the CR Lock the user must perform a power-on reset.
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] rtcUserCfg Pointer to the user's configuration structure
 * @return    STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *            if Control is locked.
 */
status_t RTC_DRV_Init(uint32_t instance, const rtc_init_config_t * const rtcUserCfg);

/*!
 * @brief This function deinitializes the RTC instance.
 *        If the Control register is locked then this method returns
 *        STATUS_ERROR.
 *
 * @param[in] instance The number of the RTC instance used
 * @return    STATUS_SUCCESS if the operation was successful or
 *            STATUS_ERROR if Control register is locked.
 */
status_t RTC_DRV_Deinit(uint32_t instance);

/*!
 * @brief This function will set the default configuration
 *        values into the structure passed as a parameter.
 * @param[out] config Pointer to the structure in which the
 * 				      configuration will be saved.
 * @return None
 */
void RTC_DRV_GetDefaultConfig(rtc_init_config_t * const config);

/*!
 * @brief Start RTC instance counter. Before calling this function the user
 * should use RTC_DRV_SetTimeDate to configure the start time
 *
 * @param[in] instance The number of the RTC instance used
 * @return    STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *            if the counter cannot be enabled or is already enabled.
 */
status_t RTC_DRV_StartCounter(uint32_t instance);

/*!
 * @brief Disable RTC instance counter
 *
 * @param[in] instance The number of the RTC instance used
 * @return    STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *            if the counter could not be stopped.
 */
status_t RTC_DRV_StopCounter(uint32_t instance);

/*!
 * @brief Get current time and date from RTC instance
 * @param[in]  instance The number of the RTC instance used
 * @param[out] currentTime Pointer to the variable in which to store the result
 * @return     STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *             if there was a problem.
 */
status_t RTC_DRV_GetCurrentTimeDate(uint32_t instance, rtc_timedate_t * const currentTime);

/*!
 * @brief Set time and date for RTC instance. The user must stop the counter
 * before using this function. Otherwise it will return an error
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] time Pointer to the variable in which the time is stored
 * @return    STATUS_SUCCESS if the operation was successful, STATUS_ERROR
 *            if the time provided was invalid or if the counter was not
 *            stopped.
 */
status_t RTC_DRV_SetTimeDate(uint32_t instance, const rtc_timedate_t * const time);

/*!
 * @brief This method configures register lock for the corresponding
 *        RTC instance. Remember that all the registers are unlocked
 *        only by software reset or power on reset.
 *        (Except for CR that is unlocked only by POR).
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] lockConfig Pointer to the lock configuration structure
 * @return    STATUS_SUCCESS if the operation was successful,
 *            STATUS_ERROR if the Lock Register is locked.
 */
status_t RTC_DRV_ConfigureRegisterLock(uint32_t instance, const rtc_register_lock_config_t * const lockConfig);

/*!
 * @brief Get which registers are locked for RTC instance
 *
 * @param[in]  instance The number of the RTC instance used
 * @param[out] lockConfig Pointer to the lock configuration structure in which to
 *             save the data
 * @return None
 */
void RTC_DRV_GetRegisterLock(uint32_t instance, rtc_register_lock_config_t * const lockConfig);

/*!
 * @brief This method configures time compensation. Data is passed by
 *         the compInterval and compensation parameters.
 *         For more details regarding coefficient calculation see the
 *         Reference Manual.
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] compInterval Compensation interval
 * @param[in] compensation Compensation value
 * @return    STATUS_SUCCESS if the operation was successful,
 *            STATUS_ERROR if the TC Register is locked.
 */
status_t RTC_DRV_ConfigureTimeCompensation(uint32_t instance, uint8_t  compInterval, int8_t   compensation);

/*!
 * @brief This retrieves the time compensation coefficients and saves
 *        them on the variables referenced by the parameters.
 *
 * @param[in]  instance The number of the RTC instance used
 * @param[out] compInterval Pointer to the variable in which to save the compensation
 *             interval
 * @param[out] compensation Pointer to the variable in which to save the compensation
 *             value
 * @return None
 */
void RTC_DRV_GetTimeCompensation(uint32_t instance, uint8_t * compInterval, int8_t * compensation);

/*!
 * @brief This method configures fault interrupts such as:
 *         - Time Overflow Interrupt
 *         - Time Invalid Interrupt
 *        with the user provided configuration struct intConfig.
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] intConfig Pointer to the structure which holds the configuration
 * @return    None
 */
void RTC_DRV_ConfigureFaultInt(uint32_t instance, rtc_interrupt_config_t * const intConfig);

/*!
 * @brief This method configures the Time Seconds Interrupt with the
 *        configuration from the intConfig parameter.
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] intConfig Pointer to the structure which holds the configuration
 * @return    None
 */
void RTC_DRV_ConfigureSecondsInt(uint32_t instance, rtc_seconds_int_config_t * const intConfig);

/*!
 * @brief This method configures the alarm with the
 *        configuration from the alarmConfig parameter.
 *
 * @param[in] instance The number of the RTC instance used
 * @param[in] alarmConfig Pointer to the structure which holds the alarm configuration
 * @return    STATUS_SUCCESS if the configuration is successful or
 *            STATUS_ERROR if the alarm time is invalid.
 */
status_t RTC_DRV_ConfigureAlarm(uint32_t instance, rtc_alarm_config_t * const alarmConfig);

/*!
 * @brief Get alarm configuration for RTC instance
 *
 * @param[in]  instance The number of the RTC instance used
 * @param[out] alarmConfig Pointer to the structure in which to store the alarm
 *             configuration
 * @return     None
 */
void RTC_DRV_GetAlarmConfig(uint32_t instance, rtc_alarm_config_t * alarmConfig);

/*!
 * @brief Check if alarm is pending
 *
 * @param[in] instance The number of the RTC instance used
 * @return    True if the alarm has occurred, false if not
 */
bool RTC_DRV_IsAlarmPending(uint32_t instance);

/*!
 * @brief Convert seconds to rtc_timedate_t structure
 *
 * @param[in]  seconds Pointer to the seconds
 * @param[out] timeDate Pointer to the structure in which to store the result
 * @return     None
 */
void RTC_DRV_ConvertSecondsToTimeDate(const uint32_t * seconds, rtc_timedate_t * const timeDate);

/*!
 * @brief Convert seconds to rtc_timedate_t structure
 *
 * @param[in]  timeDate Pointer to the source struct
 * @param[out] seconds Pointer to the variable in which to store the result
 * @return     None
 */
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t * const timeDate, uint32_t * const seconds);

/*!
 * @brief Check if the current year is leap
 *
 * @param[in] year Year to check
 * @return    True if the year is leap, false if not
 */
bool RTC_DRV_IsYearLeap(uint16_t year);

/*!
 * @brief Check if the date time struct is configured properly
 *
 * @param[in] timeDate Structure to check to check
 * @return    True if the time date is in the correct format, false if not
 */
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t * const timeDate);

/*!
 * @brief Gets the next alarm time
 *
 * @param[in]  instance The number of the RTC instance used
 * @param[out] alarmTime Pointer to the variable in which to store the data
 * @return     STATUS_SUCCESS if the next alarm time is valid,
 *             STATUS_ERROR if there is no new alarm or alarm configuration
 *             specified.
 */
status_t RTC_DRV_GetNextAlarmTime(uint32_t instance, rtc_timedate_t * const alarmTime);

/*!
 * @brief This method is the API's Interrupt handler for generic and alarm IRQ.
 * It will handle the alarm repetition and calls the user callbacks if they
 * are not NULL.
 *
 * @param[in] instance RTC instance used
 * @return    None
 */
void RTC_DRV_IRQHandler(uint32_t instance);

/*!
 * @brief This method is the API's Interrupt handler for RTC Second
 * interrupt. This ISR will call the user callback if defined.
 *
 * @param[in] instance RTC instance used
 * @return    None
 */
void RTC_DRV_SecondsIRQHandler(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* RTC_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
