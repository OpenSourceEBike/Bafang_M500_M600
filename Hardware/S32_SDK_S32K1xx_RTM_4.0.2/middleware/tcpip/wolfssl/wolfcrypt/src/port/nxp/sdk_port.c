/* sdk_port.c
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#if defined(NXP_SDK)

#include <wolfssl/wolfcrypt/port/nxp/sdk_port.h>

#include "status.h"

#if defined(NXP_SDK_HSM)
#include "hsm_driver.h"
static hsm_state_t g_hsm_state;
#endif /* NXP_SDK_HSM */

#if defined(NXP_SDK_CSEC)
#include "csec_driver.h"
static csec_state_t g_csec_state;
#endif /* NXP_SDK_CSEC */

#if defined(NXP_SDK_CSE)
#include <wolfssl/wolfcrypt/port/nxp/firmware_cse.h>
#include "cse_driver.h"
static cse_state_t g_cse_state;
#endif /* NXP_SDK_CSE */

#if defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K) || defined(NXP_SDK_PIT)
#include "lwip/def.h"
#if defined(NXP_SDK_RTC_C55)
#include "rtc_api_driver.h"
static rtc_state_t g_rtc_state;
#elif defined(NXP_SDK_RTC_S32K)
#include "rtc_driver.h"
#elif defined(NXP_SDK_PIT)
#include "task.h"
#endif
#define RTC_INSTANCE 0U
static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
#endif /* NXP_SDK_RTC_C55 || NXP_SDK_RTC_S32K || NXP_SDK_PIT*/



#if defined (NXP_SDK_PIT)

typedef struct
{
    uint16_t year;      /*!< Year       */
    uint16_t month;     /*!< Month      */
    uint16_t day;       /*!< Day        */
    uint16_t hour;      /*!< Hour       */
    uint16_t minutes;   /*!< Minutes    */
    uint8_t seconds;    /*!< Seconds    */
} rtc_timedate_t;

#define SECONDS_IN_A_DAY     (86400UL)
#define SECONDS_IN_A_HOUR    (3600U)
#define SECONDS_IN_A_MIN     (60U)
#define MINS_IN_A_HOUR       (60U)
#define HOURS_IN_A_DAY       (24U)
#define DAYS_IN_A_YEAR       (365U)
#define DAYS_IN_A_LEAP_YEAR  (366U)
#define YEAR_RANGE_START     (1970U)
#define YEAR_RANGE_END       (2099U)

static const uint16_t MONTH_DAYS[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

uint32_t PIT_sec;
bool PIT_enable = 0;

static bool RTC_DRV_IsYearLeap(uint16_t year);
static void RTC_DRV_ConvertTimeDateToSeconds(rtc_timedate_t * timeDate, uint32_t * seconds);

void nxp_pit_tick(void* arg)
{
    (void)arg;
    while(1)
    {

        if (PIT_enable) PIT_sec +=1 ;
        vTaskDelay( configTICK_RATE_HZ -1 );
    }
}

static void RTC_DRV_StartCounter(uint32_t instance)
{
    (void)instance;
    PIT_enable = 1;
}

static void RTC_DRV_Deinit(uint32_t instance)
{
    (void)instance;
    PIT_enable = 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsYearLeap
 * Description   : This method checks if the year passed as a parameter is a leap
 *                 one.
 * Return        : True if the year is leap, false if otherwise.
 * Implements    : RTC_DRV_IsYearLeap_Activity
 *END**************************************************************************/
static bool RTC_DRV_IsYearLeap(uint16_t year)
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
 * Function Name : RTC_DRV_ConvertTimeDateToSeconds
 * Description   : This method converts time-date into seconds.
 * Return        : None
 * Implements    : RTC_DRV_ConvertTimeDateToSeconds_Activity
 *END**************************************************************************/
static void RTC_DRV_ConvertTimeDateToSeconds(rtc_timedate_t * timeDate, uint32_t * seconds)
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

#endif /*NXP_SDK_PIT*/


int nxp_sdk_port_init(void)
{
    status_t ret = STATUS_SUCCESS;

#if defined(NXP_SDK_HSM)
    ret = HSM_DRV_Init(&g_hsm_state);
    DEV_ASSERT(ret == STATUS_SUCCESS);
    ret = HSM_DRV_InitRNG(HSM_TIMEOUT);
    DEV_ASSERT(ret == STATUS_SUCCESS);
#endif /* NXP_SDK_HSM */

#if defined(NXP_SDK_CSEC)
    CSEC_DRV_Init(&g_csec_state);
    ret = CSEC_DRV_InitRNG();
    DEV_ASSERT(ret == STATUS_SUCCESS);
#endif /* NXP_SDK_CSEC */

#if defined(NXP_SDK_CSE)
    kia[4] = (((uint32_t)&firmware) & 0xff000000) >> 24;
    kia[5] = (((uint32_t)&firmware) & 0xff0000) >> 16;
    kia[6] = (((uint32_t)&firmware) & 0xff00) >> 8;
    kia[7] = (((uint32_t)&firmware) & 0xff);
    CSE_DRV_SetKeyImageAddress((uint32_t)&kia, (uint32_t)&kia);
    CSE_DRV_Init(&g_cse_state);
    ret = CSE_DRV_InitRNG(10);
    DEV_ASSERT(ret == STATUS_SUCCESS);
#endif /* NXP_SDK_CSE */

#if defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K)
    rtc_init_config_t rtcConfig;
    RTC_DRV_GetDefaultConfig(&rtcConfig);
#if defined(NXP_SDK_RTC_C55)
    RTC_DRV_Init(RTC_INSTANCE, &g_rtc_state, &rtcConfig);
#elif defined(NXP_SDK_RTC_S32K)
    RTC_DRV_Init(RTC_INSTANCE, &rtcConfig);
#endif /* NXP_SDK_RTC_C55 */
#endif /* NXP_SDK_RTC_C55 || NXP_SDK_RTC_S32K */

#if (defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K) || defined (NXP_SDK_PIT))
    /* Initialize Time & Date based on host system's clock at compile time.
       This may need to be initialized more accurately.
    */
    rtc_timedate_t timedate;
    char dbuff[] = __DATE__;
    char tbuff[] = __TIME__;

    timedate.year = (uint16_t)atoi((const char*)&dbuff[7]);
    dbuff[6] = '\0';
    timedate.day = (uint16_t)atoi((const char*)&dbuff[4]);
    dbuff[3] = '\0';
    timedate.month = (uint16_t)((lwip_strnstr(month_names, dbuff, sizeof(month_names)) - month_names) / 3 + 1);

    timedate.seconds = (uint8_t)atoi((const char*)&tbuff[6]);
    tbuff[5] = '\0';
    timedate.minutes = (uint16_t)atoi((const char*)&tbuff[3]);
    tbuff[2] = '\0';
    timedate.hour = (uint16_t)atoi((const char*)&tbuff[0]);
#endif /* (defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K) || defined (NXP_SDK_PIT)) */

#if (defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K))
    ret = RTC_DRV_SetTimeDate(RTC_INSTANCE, &timedate);
    DEV_ASSERT(ret == STATUS_SUCCESS);
    ret = RTC_DRV_StartCounter(RTC_INSTANCE);
    DEV_ASSERT(ret == STATUS_SUCCESS);
#elif defined (NXP_SDK_PIT)
    RTC_DRV_ConvertTimeDateToSeconds(&timedate, &PIT_sec);
    RTC_DRV_StartCounter(RTC_INSTANCE);
#endif /* NXP_SDK_RTC_C55 || NXP_SDK_RTC_S32K */
    return ret;
}


uint32_t nxp_sdk_get_time(void)
{
    uint32_t secs;
#ifdef NXP_SDK_PIT
    secs = PIT_sec + (unsigned int)(xTaskGetTickCount()/configTICK_RATE_HZ);
#endif /* NXP_SDK_PIT */

#if defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K)
    rtc_timedate_t crt;
#if defined(NXP_SDK_RTC_C55)
    RTC_DRV_GetTimeDate(RTC_INSTANCE, &crt);
#elif defined(NXP_SDK_RTC_S32K)
    RTC_DRV_GetCurrentTimeDate(RTC_INSTANCE, &crt);
#endif
    RTC_DRV_ConvertTimeDateToSeconds(&crt, &secs);
#endif /* NXP_SDK_RTC_C55 || NXP_SDK_RTC_S32K */

    return secs;
}




void nxp_sdk_port_cleanup(void)
{
#if defined(NXP_SDK_HSM)
    HSM_DRV_Deinit();
#endif /* NXP_SDK_HSM */

#if defined(NXP_SDK_CSEC)
    CSEC_DRV_Deinit();
#endif /* NXP_SDK_CSEC */

#if defined(NXP_SDK_CSE)
    CSE_DRV_Deinit();
#endif /* NXP_SDK_CSEC */

#if defined(NXP_SDK_RTC_C55) || defined(NXP_SDK_RTC_S32K)
    RTC_DRV_Deinit(RTC_INSTANCE);
#elif defined (NXP_SDK_PIT)
    RTC_DRV_Deinit(RTC_INSTANCE);
#endif /* NXP_SDK_RTC_C55 || NXP_SDK_RTC_S32K */
}

#endif /* NXP_SDK */

