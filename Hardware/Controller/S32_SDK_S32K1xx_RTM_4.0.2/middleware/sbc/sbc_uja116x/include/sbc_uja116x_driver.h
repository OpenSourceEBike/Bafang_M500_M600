/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2021 NXP
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

#ifndef SOURCES_SBC_SBC_UJA_116X_H_
#define SOURCES_SBC_SBC_UJA_116X_H_

#include "sbc_uja116x_map.h"
#include "lpspi_master_driver.h"
#include "status.h"

/*!
 * @addtogroup sbc_uja116x_driver UJA116xA SBC Driver
 * @ingroup sbc_uja116x
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SBC_UJA_TIMEOUT                1000U /*!< Timeout for the transfer
in milliseconds. If the transfer takes longer than this time, the transfer
is aborted and LPSPI_STATUS_SBC_UJA_TIMEOUT error is reported. */

#define SBC_UJA_COUNT_ID_REG           4U
#define SBC_UJA_COUNT_MASK             4U
#define SBC_UJA_COUNT_DMASK            8U
/*******************************************************************************
 * Enumerations
 ******************************************************************************/

/*!
 * @brief Register map.
 *
 * Implements    : sbc_register_t_Class
 */
typedef enum{
    /* Primary control registers. */
    SBC_UJA_WTDOG_CTR                  = 0x00U, /*  Watchdog control. */
    SBC_UJA_MODE                       = 0x01U, /*  Mode control. */
    SBC_UJA_FAIL_SAFE                  = 0x02U, /*  Fail-safe control. */
    SBC_UJA_MAIN                       = 0x03U, /*  Main status. */
    SBC_UJA_SYSTEM_EVNT                = 0x04U, /*  System event enable. */
    SBC_UJA_WTDOG_STAT                 = 0x05U, /*  Watchdog status. */
    SBC_UJA_MEMORY_0                   = 0x06U, /*  Memory 0. */
    SBC_UJA_MEMORY_1                   = 0x07U, /*  Memory 1. */
    SBC_UJA_MEMORY_2                   = 0x08U, /*  Memory 2. */
    SBC_UJA_MEMORY_3                   = 0x09U, /*  Memory 3. */
    SBC_UJA_LOCK                       = 0x0AU, /*  Lock control. */
    /* Regulator control registers. */
    SBC_UJA_REGULATOR                  = 0x10U, /*  Regulator control. */
    SBC_UJA_SUPPLY_STAT                = 0x1BU, /*  Supply status. */
    SBC_UJA_SUPPLY_EVNT                = 0x1CU, /*  Supply event enable. */
    /* Transceiver control and partial networking registers. */
    SBC_UJA_CAN                        = 0x20U, /*  CAN control. */
    SBC_UJA_TRANS_STAT                 = 0x22U, /*  Transceiver status. */
    SBC_UJA_TRANS_EVNT                 = 0x23U, /*  Transceiver event enable. */
    SBC_UJA_DAT_RATE                   = 0x26U, /*  Data rate. */
    SBC_UJA_IDENTIF_0                  = 0x27U, /*  Identifier 0. */
    SBC_UJA_IDENTIF_1                  = 0x28U, /*  Identifier 1. */
    SBC_UJA_IDENTIF_2                  = 0x29U, /*  Identifier 2. */
    SBC_UJA_IDENTIF_3                  = 0x2AU, /*  Identifier 3. */
    SBC_UJA_MASK_0                     = 0x2BU, /*  Mask 0. */
    SBC_UJA_MASK_1                     = 0x2CU, /*  Mask 1. */
    SBC_UJA_MASK_2                     = 0x2DU, /*  Mask 2. */
    SBC_UJA_MASK_3                     = 0x2EU, /*  Mask 3. */
    SBC_UJA_FRAME_CTR                  = 0x2FU, /*  Frame control. */
    SBC_UJA_DAT_MASK_0                 = 0x68U, /*  Data mask 0. */
    SBC_UJA_DAT_MASK_1                 = 0x69U, /*  Data mask 1. */
    SBC_UJA_DAT_MASK_2                 = 0x6AU, /*  Data mask 2. */
    SBC_UJA_DAT_MASK_3                 = 0x6BU, /*  Data mask 3. */
    SBC_UJA_DAT_MASK_4                 = 0x6CU, /*  Data mask 4. */
    SBC_UJA_DAT_MASK_5                 = 0x6DU, /*  Data mask 5. */
    SBC_UJA_DAT_MASK_6                 = 0x6EU, /*  Data mask 6. */
    SBC_UJA_DAT_MASK_7                 = 0x6FU, /*  Data mask 7. */
    /* WAKE pin control and status registers. */
    SBC_UJA_WAKE_STAT                  = 0x4BU, /*  WAKE pin status. */
    SBC_UJA_WAKE_EN                    = 0x4CU, /*  WAKE pin enable. */
    /* Event capture registers. */
    SBC_UJA_GL_EVNT_STAT               = 0x60U, /*  Global event status. */
    SBC_UJA_SYS_EVNT_STAT              = 0x61U, /*  System event status. */
    SBC_UJA_SUP_EVNT_STAT              = 0x62U, /*  Supply event status. */
    SBC_UJA_TRANS_EVNT_STAT            = 0x63U, /*  Transceiver event status. */
    SBC_UJA_WAKE_EVNT_STAT             = 0x64U, /*  WAKE pin event status. */
    /* MTPNV status register. */
    SBC_UJA_MTPNV_STAT                 = 0x70U, /*  MTPNV status. */
    /* Start-up control register. */
    SBC_UJA_START_UP                   = 0x73U, /*  Start-up control. */
    /* SBC configuration control. */
    SBC_UJA_SBC                        = 0x74U, /* SBC configuration control. */
    /* CRC control register. */
    SBC_UJA_MTPNV_CRC                  = 0x75U, /*  MTPNV CRC control. */
    /* Identification register. */
    SBC_UJA_IDENTIF                    = 0x7EU, /* Identification. */
}sbc_register_t;

/*!
 * @brief  Watchdog control register,  watchdog mode control (0x00).
 * The UJA116xA contains a watchdog that supports three operating modes:
 * Window, Timeout and Autonomous. In Window mode (available only
 * in SBC Normal mode), a watchdog trigger event within a defined
 * watchdog window triggers and resets the watchdog timer. In Timeout
 * mode, the watchdog runs continuously and can be triggered and reset
 * at any time within the watchdog period by a watchdog trigger.
 * Watchdog time-out mode can also be used for cyclic wake-up of the
 * microcontroller. In Autonomous mode, the watchdog can be off or
 * autonomously in Timeout mode, depending on the selected SBC mode.
 * The watchdog mode is selected via bits WMC in the Watchdog control
 * register.
 * The SBC must be in Standby mode when the watchdog mode is changed.
 *
 * Implements    : sbc_wtdog_ctr_wmc_t_Class
 */
typedef enum{
    SBC_UJA_WTDOG_CTR_WMC_AUTO         = SBC_UJA_WTDOG_CTR_WMC_F(1U), /*!< Autonomous mode. */
    SBC_UJA_WTDOG_CTR_WMC_TIME         = SBC_UJA_WTDOG_CTR_WMC_F(2U), /*!< Timeout mode. */
    SBC_UJA_WTDOG_CTR_WMC_WIND         = SBC_UJA_WTDOG_CTR_WMC_F(4U), /*!< Window mode
    (available only in SBC Normal mode). */
}sbc_wtdog_ctr_wmc_t;

/*!
 * @brief  Watchdog control register, nominal watchdog period (0x00).
 * Eight watchdog periods are supported, from 8 ms to 4096 ms.
 * The watchdog period is programmed via bits NWP. The selected period
 * is valid for both Window and Timeout modes. The default watchdog
 * period is 128 ms.
 * A watchdog trigger event resets the watchdog timer. A watchdog
 * trigger event is any valid write access to the Watchdog control
 * register. If the watchdog mode or the watchdog period have changed
 * as a result of the write access, the new values are immediately
 * valid.
 *
 * Implements    : sbc_wtdog_ctr_nwp_t_Class
 */
typedef enum{
    SBC_UJA_WTDOG_CTR_NWP_8            = 0x08U, /*!< 8 ms. */
    SBC_UJA_WTDOG_CTR_NWP_16           = 0x01U, /*!< 16 ms. */
    SBC_UJA_WTDOG_CTR_NWP_32           = 0x02U, /*!< 32 ms. */
    SBC_UJA_WTDOG_CTR_NWP_64           = 0x0BU, /*!< 64 ms. */
    SBC_UJA_WTDOG_CTR_NWP_128          = 0x04U, /*!< 128 ms. */
    SBC_UJA_WTDOG_CTR_NWP_256          = 0x0DU, /*!< 256 ms. */
    SBC_UJA_WTDOG_CTR_NWP_1024         = 0x0EU, /*!< 1024 ms. */
    SBC_UJA_WTDOG_CTR_NWP_4096         = 0x07U, /*!< 4096 ms. */
}sbc_wtdog_ctr_nwp_t;

/*!
 * @brief  Mode control register, mode control (0x01)
 *
 * Implements    : sbc_mode_mc_t_Class
 */
typedef enum{
    SBC_UJA_MODE_MC_SLEEP              = 0x01U, /*!< Sleep mode. */
    SBC_UJA_MODE_MC_STANDBY            = 0x04U, /*!< Standby mode. */
    SBC_UJA_MODE_MC_NORMAL             = 0x07U  /*!< Normal mode. */
}sbc_mode_mc_t;

/*!
 * @brief  Fail-safe control register, LIMP home control (0x02).
 * The dedicated LIMP pin can be used to enable so called limp home hardware
 * in the event of a serious ECU failure. Detectable failure conditions
 * include SBC overtemperature events, loss of watchdog service, short-circuits
 * on pins RSTN or V1 and user-initiated or external reset events.
 * The LIMP pin is a battery-robust, active-LOW, open-drain
 * output. The LIMP pin can also be forced LOW by setting bit LHC in the
 * Fail-safe control register.
 *
 * Implements    : sbc_fail_safe_lhc_t_Class
 */
typedef enum{
    SBC_UJA_FAIL_SAFE_LHC_FLOAT        = SBC_UJA_FAIL_SAFE_LHC_F(0U), /*!< LIMP pin is floating. */
    SBC_UJA_FAIL_SAFE_LHC_LOW          = SBC_UJA_FAIL_SAFE_LHC_F(1U), /*!< LIMP pin is driven LOW. */
}sbc_fail_safe_lhc_t;

/*!
 * @brief  Fail-safe control register, reset counter control (0x02).
 * incremented every time the SBC enters Reset mode while FNMC = 0;
 * RCC overflows from 11 to 00; default at power-on is 00
 *
 * Implements    : sbc_fail_safe_rcc_t_Class
 */
typedef uint8_t sbc_fail_safe_rcc_t;

/*!
 * @brief  Main status register, Overtemperature warning status (0x03).
 *
 * Implements    : sbc_main_otws_t_Class
 */
typedef enum{
    SBC_UJA_MAIN_OTWS_BELOW            = SBC_UJA_MAIN_OTWS_F(0U), /*!< IC temperature
    below overtemperature warning threshold. */
    SBC_UJA_MAIN_OTWS_ABOVE            = SBC_UJA_MAIN_OTWS_F(1U), /*!< IC temperature
    above overtemperature warning threshold. */
}sbc_main_otws_t;

/*!
 * @brief  Main status register, normal mode status  (0x03).
 *
 * Implements    : sbc_main_nms_t_Class
 */
typedef enum{
    SBC_UJA_MAIN_NMS_NORMAL            = SBC_UJA_MAIN_NMS_F(0U), /*!< UJA116xA has entered
    Normal mode (after power-up)*/
    SBC_UJA_MAIN_NMS_PWR_UP            = SBC_UJA_MAIN_NMS_F(1U)  /*!< UJA116xA has
    powered up but has not yet switched to Normal mode. */
}sbc_main_nms_t;

/*!
 * @brief  Main status register, Reset source status (0x03).
 *
 * Implements    : sbc_main_rss_t_Class
 */
typedef enum{
    SBC_UJA_MAIN_RSS_OFF_MODE          = 0x00U, /*!< Left Off mode (power-on). */
    SBC_UJA_MAIN_RSS_CAN_WAKEUP        = 0x01U, /*!< CAN wake-up in Sleep mode. */
    SBC_UJA_MAIN_RSS_SLP_WAKEUP        = 0x04U, /*!< Wake-up via WAKE pin in Sleep mode. */
    SBC_UJA_MAIN_RSS_OVF_SLP           = 0x0CU, /*!< Watchdog overflow
    in Sleep mode (Timeout mode). */
    SBC_UJA_MAIN_RSS_DIAG_WAKEUP       = 0x0DU, /*!< Diagnostic wake-up in Sleep mode */
    SBC_UJA_MAIN_RSS_WATCH_TRIG        = 0x0EU, /*!< Watchdog triggered too early
    (Window mode). */
    SBC_UJA_MAIN_RSS_WATCH_OVF         = 0x0FU, /*!< Watchdog overflow
    (Window mode or Timeout mode with WDF = 1) */
    SBC_UJA_MAIN_RSS_ILLEG_WATCH       = 0x10U, /*!< Illegal watchdog mode control access. */
    SBC_UJA_MAIN_RSS_RSTN_PULDW        = 0x11U, /*!< RSTN pulled down externally. */
    SBC_UJA_MAIN_RSS_LFT_OVERTM        = 0x12U, /*!< Left Overtemp mode. */
    SBC_UJA_MAIN_RSS_V1_UNDERV         = 0x13U, /*!< V1 undervoltage. */
    SBC_UJA_MAIN_RSS_ILLEG_SLP         = 0x14U, /*!< Illegal Sleep mode command received. */
    SBC_UJA_MAIN_RSS_WAKE_SLP          = 0x16U, /*!< Wake-up from Sleep mode due to a
    frame detect error */
}sbc_main_rss_t;

/*!
 * @brief    System event capture enable, overtemperature warning enable (0x04).
 *
 * Implements    : sbc_sys_evnt_otwe_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_OTWE_DIS          = SBC_UJA_SYS_EVNT_OTWE_F(0U), /*!< Overtemperature
    warning disabled. */
    SBC_UJA_SYS_EVNT_OTWE_EN           = SBC_UJA_SYS_EVNT_OTWE_F(1U)  /*!< Overtemperature
    warning enabled. */
}sbc_sys_evnt_otwe_t;

/*!
 * @brief    System event capture enable, SPI failure enable (0x04).
 *
 * Implements    : sbc_sys_evnt_spife_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_SPIFE_DIS         = SBC_UJA_SYS_EVNT_SPIFE_F(0U), /*!< SPI failure
    detection disabled. */
    SBC_UJA_SYS_EVNT_SPIFE_EN          = SBC_UJA_SYS_EVNT_SPIFE_F(1U)  /*!< SPI failure
    detection enabled. */
}sbc_sys_evnt_spife_t;

/*!
 * @brief   Watchdog status register, forced Normal mode status (0x05).
 *
 * Implements    : sbc_wtdog_stat_fnms_t_Class
 */
typedef enum{
    SBC_UJA_WTDOG_STAT_FNMS_N_NORMAL   = SBC_UJA_WTDOG_STAT_FNMS_F(0U), /*!< SBC is not
    in Forced Normal mode. */
    SBC_UJA_WTDOG_STAT_FNMS_NORMAL     = SBC_UJA_WTDOG_STAT_FNMS_F(1U)  /*!< SBC is
    in Forced Normal mode. */
}sbc_wtdog_stat_fnms_t;

/*!
 * @brief   Watchdog status register, Software Development mode status (0x05).
 *
 * Implements    : sbc_wtdog_stat_sdms_t_Class
 */
typedef enum{
    SBC_UJA_WTDOG_STAT_SDMS_N_NORMAL   = SBC_UJA_WTDOG_STAT_SDMS_F(0U), /*!< SBC is not
    in Software Development mode. */
    SBC_UJA_WTDOG_STAT_SDMS_NORMAL     = SBC_UJA_WTDOG_STAT_SDMS_F(1U)  /*!< SBC is
    in Software Development mode. */
}sbc_wtdog_stat_sdms_t;

/*!
 * @brief   Watchdog status register, watchdog status (0x05).
 *
 * Implements    : sbc_wtdog_stat_wds_t_Class
 */
typedef enum{
    SBC_UJA_WTDOG_STAT_WDS_OFF         = SBC_UJA_WTDOG_STAT_WDS_F(0U), /*!< Watchdog
    is off. */
    SBC_UJA_WTDOG_STAT_WDS_FIH         = SBC_UJA_WTDOG_STAT_WDS_F(1U), /*!< Watchdog
    is in first half of the nominal period. */
    SBC_UJA_WTDOG_STAT_WDS_SEH         = SBC_UJA_WTDOG_STAT_WDS_F(2U), /*!< Watchdog
    is in second half of the nominal period. */
}sbc_wtdog_stat_wds_t;

/*!
 * @brief  Lock control(0x0A).
 * Sections of the register address area can be write-protected to protect
 * against unintended modifications. This facility only protects locked bits
 * from being modified via the SPI and will not prevent the UJA116xA updating
 * status registers etc.
 *
 * Implements    : sbc_lock_t_Class
 */
typedef enum{
    /*!< Lock control 0: address area 0x06 to 0x09 - general-purpose memory
         * macros. */
    LK0C                           = SBC_UJA_LOCK_LK0C_MASK,
    /*!< Lock control 1: address area 0x10 to 0x1F - regulator control macros. */
    LK1C                           = SBC_UJA_LOCK_LK1C_MASK,
    /*!< Lock control 2: address area 0x20 to 0x2F - transceiver control macros. */
    LK2C                           = SBC_UJA_LOCK_LK2C_MASK,
    /*!< Lock control 3: address area 0x30 to 0x3F - unused register range macros. */
    LK3C                           = SBC_UJA_LOCK_LK3C_MASK,
    /*!< Lock control 4: address area 0x40 to 0x4F - WAKE pin control macros. */
    LK4C                           = SBC_UJA_LOCK_LK4C_MASK,
    /*!< Lock control 5: address area 0x50 to 0x5F. */
    LK5C                           = SBC_UJA_LOCK_LK5C_MASK,
    /*!< Lock control 6: address area 0x68 to 0x6F macros. */
    LK6C                           = SBC_UJA_LOCK_LK6C_MASK,
    /*!< Lock control All: address area 0x10 to 0x6F macros. */
    LKAC                           = SBC_UJA_LOCK_LKNC_MASK
}sbc_lock_t;

/*!
 * @brief  Regulator control register, power distribution control (0x10).
 * PDC is not available on UJA1168 device variants, use any of these two values,
 * the value written to the device will be ignored.
 *
 * Implements    : sbc_regulator_pdc_t_Class
 */
typedef enum{
    SBC_UJA_REGULATOR_PDC_HV           = SBC_UJA_REGULATOR_PDC_F(0U), /*!<V1 threshold
    current for activating the external PNP transistor, load current
    rising; Ith(act)PNP (higher value)
    V1 threshold current for deactivating the external PNP transistor, load
    current falling; Ith(deact)PNP (higher value). */
    SBC_UJA_REGULATOR_PDC_LV           = SBC_UJA_REGULATOR_PDC_F(1U) /*!< V1 threshold
    current for activating the external PNP transistor; load current
    rising; Ith(act)PNP (lower value)
    V1 threshold current for deactivating the external PNP transistor; load
    current falling; Ith(deact)PNP (lower value). */
}sbc_regulator_pdc_t;

/*!
 * @brief  Regulator control register, V2/VEXT configuration (0x10).
 *
 * Implements    : sbc_regulator_v2c_t_Class
 */
typedef enum{
    SBC_UJA_REGULATOR_V2C_OFF          = SBC_UJA_REGULATOR_V2C_F(0U), /*!< V2/VEXT off in
    all modes. */
    SBC_UJA_REGULATOR_V2C_N            = SBC_UJA_REGULATOR_V2C_F(1U), /*!< V2/VEXT on in
    Normal mode. */
    SBC_UJA_REGULATOR_V2C_N_S_R        = SBC_UJA_REGULATOR_V2C_F(2U), /*!< V2/VEXT on in
    Normal, Standby and Reset modes. */
    SBC_UJA_REGULATOR_V2C_N_S_S_R      = SBC_UJA_REGULATOR_V2C_F(3U), /*!< V2/VEXT on in
    Normal, Standby, Sleep and Reset modes. */
}sbc_regulator_v2c_t;

/*!
 * @brief  Regulator control register, set V1 reset threshold (0x10).
 *
 * Implements    : sbc_regulator_v1rtc_t_Class
 */
typedef enum{
    SBC_UJA_REGULATOR_V1RTC_90         = SBC_UJA_REGULATOR_V1RTC_F(0U), /*!< Reset threshold
    set to 90 % of V1 nominal output voltage. */
    SBC_UJA_REGULATOR_V1RTC_80         = SBC_UJA_REGULATOR_V1RTC_F(1U), /*!< Reset threshold
    set to 80 % of V1 nominal output voltage. */
    SBC_UJA_REGULATOR_V1RTC_70         = SBC_UJA_REGULATOR_V1RTC_F(2U), /*!< Reset threshold
    set to 70 % of V1 nominal output voltage. */
    SBC_UJA_REGULATOR_V1RTC_60         = SBC_UJA_REGULATOR_V1RTC_F(3U), /*!< Reset threshold
    set to 60 % of V1 nominal output voltage. */
}sbc_regulator_v1rtc_t;

/*!
 * @brief  Supply voltage status register, V2/VEXT status (0x1B).
 *
 * Implements    : sbc_supply_stat_v2s_t_Class
 */
typedef enum{
    SBC_UJA_SUPPLY_STAT_V2S_VOK        = SBC_UJA_SUPPLY_STAT_V2S_F(0U), /*!< V2/VEXT
    voltage ok. */
    SBC_UJA_SUPPLY_STAT_V2S_VBE        = SBC_UJA_SUPPLY_STAT_V2S_F(1U), /*!< V2/VEXT
    output voltage below undervoltage threshold */
    SBC_UJA_SUPPLY_STAT_V2S_VAB        = SBC_UJA_SUPPLY_STAT_V2S_F(2U), /*!< V2/VEXT
    output voltage above overvoltage threshold */
    SBC_UJA_SUPPLY_STAT_V2S_DIS        = SBC_UJA_SUPPLY_STAT_V2S_F(3U), /*!< V2/VEXT
    disabled */
}sbc_supply_stat_v2s_t;

/*!
 * @brief  Supply voltage status register, V1 status (0x1B).
 *
 * Implements    : sbc_supply_stat_v1s_t_Class
 */
typedef enum{
    SBC_UJA_SUPPLY_STAT_V1S_VAB        = SBC_UJA_SUPPLY_STAT_V1S_F(0U), /*!< V1 output
    voltage above 90 % undervoltage threshold. */
    SBC_UJA_SUPPLY_STAT_V1S_VBE        = SBC_UJA_SUPPLY_STAT_V1S_F(1U), /*!< V1 output
    voltage below 90 % undervoltage threshold. */
}sbc_supply_stat_v1s_t;

/*!
 * @brief  Supply event capture enable register,
 * V2/VEXT overvoltage enable (0x1C).
 *
 * Implements    : sbc_supply_evnt_v2oe_t_Class
 */
typedef enum{
    SBC_UJA_SUPPLY_EVNT_V2OE_DIS       = SBC_UJA_SUPPLY_EVNT_V2OE_F(0U), /*!< V2/VEXT
    overvoltage detection disabled. */
    SBC_UJA_SUPPLY_EVNT_V2OE_EN        = SBC_UJA_SUPPLY_EVNT_V2OE_F(1U), /*!< V2/VEXT
    overvoltage detection enabled. */
}sbc_supply_evnt_v2oe_t;

/*!
 * @brief  Supply event capture enable register,
 * V2/VEXT undervoltage enable (0x1C).
 *
 * Implements    : sbc_supply_evnt_v2ue_t_Class
 */
typedef enum{
    SBC_UJA_SUPPLY_EVNT_V2UE_DIS       = SBC_UJA_SUPPLY_EVNT_V2UE_F(0U), /*!< V2/VEXT
    undervoltage detection disabled. */
    SBC_UJA_SUPPLY_EVNT_V2UE_EN        = SBC_UJA_SUPPLY_EVNT_V2UE_F(1U), /*!< V2/VEXT
    undervoltage detection enabled. */
}sbc_supply_evnt_v2ue_t;

/*!
 * @brief  Supply event capture enable register, V1 undervoltage enable (0x1C).
 *
 * Implements    : sbc_supply_evnt_v1ue_t_Class
 */
typedef enum{
    SBC_UJA_SUPPLY_EVNT_V1UE_DIS       = SBC_UJA_SUPPLY_EVNT_V1UE_F(0U), /*!<  V1
    undervoltage detection disabled. */
    SBC_UJA_SUPPLY_EVNT_V1UE_EN        = SBC_UJA_SUPPLY_EVNT_V1UE_F(1U), /*!< V1
    undervoltage detection enabled. */
}sbc_supply_evnt_v1ue_t;

/*!
 * @brief  CAN control register, CAN FD control (0x20).
 *
 * Implements    : sbc_can_cfdc_t_Class
 */
typedef enum{
    SBC_UJA_CAN_CFDC_DIS               = SBC_UJA_CAN_CFDC_F(0U), /*!< CAN FD
    tolerance disabled. */
    SBC_UJA_CAN_CFDC_EN                = SBC_UJA_CAN_CFDC_F(1U), /*!< CAN FD
    tolerance enabled. */
}sbc_can_cfdc_t;

/*!
 * @brief  CAN control register, CAN partial networking configuration OK (0x20).
 *
 * Implements    : sbc_can_pncok_t_Class
 */
typedef enum{
    SBC_UJA_CAN_PNCOK_DIS              = SBC_UJA_CAN_PNCOK_F(0U), /*!< Partial networking
    register configuration invalid (wake-up via standard wake-up pattern only). */
    SBC_UJA_CAN_PNCOK_EN               = SBC_UJA_CAN_PNCOK_F(1U), /*!< Partial networking
    registers configured successfully. */
}sbc_can_pncok_t;

/*!
 * @brief  CAN control register, CAN partial networking control (0x20).
 *
 * Implements    : sbc_can_cpnc_t_Class
 */
typedef enum{
    SBC_UJA_CAN_CPNC_DIS               = SBC_UJA_CAN_CPNC_F(0U), /*!< Disable CAN
    selective wake-up. */
    SBC_UJA_CAN_CPNC_EN                = SBC_UJA_CAN_CPNC_F(1U), /*!< Enable CAN
    selective wake-up. */
}sbc_can_cpnc_t;

/*!
 * @brief  CAN control register, CAN mode control (0x20).
 *
 * Implements    : sbc_can_cmc_t_Class
 */
typedef enum{
    SBC_UJA_CAN_CMC_OFMODE             = SBC_UJA_CAN_CMC_F(0U), /*!< Offline mode. */
    SBC_UJA_CAN_CMC_ACMODE_DA          = SBC_UJA_CAN_CMC_F(1U), /*!< Active mode
    (when the SBC is in Normal mode); CAN supply undervoltage
    detection active. */
    SBC_UJA_CAN_CMC_ACMODE_DD          = SBC_UJA_CAN_CMC_F(2U), /*!<  Active mode
    (when the SBC is in Normal mode); CAN supply undervoltage
    detection disabled. */
    SBC_UJA_CAN_CMC_LISTEN             = SBC_UJA_CAN_CMC_F(3U), /*!< Listen-only mode. */
}sbc_can_cmc_t;

/*!
 * @brief  Transceiver status register, CAN transceiver status (0x22).
 *
 * Implements    : sbc_trans_stat_cts_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_CTS_INACT       = SBC_UJA_TRANS_STAT_CTS_F(0U), /*!< CAN
    transceiver not in Active mode. */
    SBC_UJA_TRANS_STAT_CTS_ACT         = SBC_UJA_TRANS_STAT_CTS_F(1U), /*!< CAN
    transceiver in Active mode. */
}sbc_trans_stat_cts_t;

/*!
 * @brief  Transceiver status register, CAN partial networking error (0x22).
 *
 * Implements    : sbc_trans_stat_cpnerr_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_CPNERR_NO_DET   = SBC_UJA_TRANS_STAT_CPNERR_F(0U), /*!< no CAN
    partial networking error detected (PNFDE = 0 AND PNCOK = 1). */
    SBC_UJA_TRANS_STAT_CPNERR_DET      = SBC_UJA_TRANS_STAT_CPNERR_F(1U), /*!< CAN
    partial networking error detected (PNFDE = 1OR PNCOK = 0;
    wake-up via standard wake-up pattern only). */
}sbc_trans_stat_cpnerr_t;

/*!
 * @brief  Transceiver status register, CAN partial networking status (0x22).
 *
 * Implements    : sbc_trans_stat_cpns_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_CPNS_ERR        = SBC_UJA_TRANS_STAT_CPNS_F(0U), /*!< CAN
    partial networking configuration error detected(PNCOK = 0). */
    SBC_UJA_TRANS_STAT_CPNS_OK         = SBC_UJA_TRANS_STAT_CPNS_F(1U), /*!< CAN
    partial networking configuration ok (PNCOK = 1). */
}sbc_trans_stat_cpns_t;

/*!
 * @brief  Transceiver status register, CAN oscillator status (0x22).
 *
 * Implements    : sbc_trans_stat_coscs_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_COSCS_NRUN      = SBC_UJA_TRANS_STAT_COSCS_F(0U), /*!< CAN
    partial networking oscillator not running at target frequency. */
    SBC_UJA_TRANS_STAT_COSCS_RUN       = SBC_UJA_TRANS_STAT_COSCS_F(1U), /*!< CAN
    partial networking oscillator running at target. */
}sbc_trans_stat_coscs_t;

/*!
 * @brief  Transceiver status register, CAN-bus silence status (0x22).
 *
 * Implements    : sbc_trans_stat_cbss_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_CBSS_ACT        = SBC_UJA_TRANS_STAT_CBSS_F(0U), /*!< CAN-bus
    active (communication detected on bus) */
    SBC_UJA_TRANS_STAT_CBSS_INACT      = SBC_UJA_TRANS_STAT_CBSS_F(1U), /*!< CAN-bus
    inactive (for longer than t_to(silence)). */
}sbc_trans_stat_cbss_t;

/*!
 * @brief  Transceiver status register, VCAN status (0x22).
 *
 * Implements    : sbc_trans_stat_vcs_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_VCS_AB          = SBC_UJA_TRANS_STAT_VCS_F(0U), /*!< CAN supply voltage
    is above the 90 % threshold. */
    SBC_UJA_TRANS_STAT_VCS_BE          = SBC_UJA_TRANS_STAT_VCS_F(1U), /*!< CAN supply voltage
    is below the 90 % threshold */
}sbc_trans_stat_vcs_t;

/*!
 * @brief  Transceiver status register, CAN failure status (0x22).
 *
 * Implements    : sbc_trans_stat_cfs_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_STAT_CFS_NO_TXD      = SBC_UJA_TRANS_STAT_CFS_F(0U), /*!< No TXD
    dominant time-out event detected. */
    SBC_UJA_TRANS_STAT_CFS_TXD         = SBC_UJA_TRANS_STAT_CFS_F(1U), /*!< CAN
    transmitter disabled due to a TXD dominant time-out event. */
}sbc_trans_stat_cfs_t;

/*!
 * @brief  Transceiver event capture enable register, CAN-bus silence enable (0x23).
 *
 * Implements    : sbc_trans_evnt_cbse_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_CBSE_DIS        = SBC_UJA_TRANS_EVNT_CBSE_F(0U), /*!< CAN-bus
    silence detection disabled. */
    SBC_UJA_TRANS_EVNT_CBSE_EN         = SBC_UJA_TRANS_EVNT_CBSE_F(1U), /*!< CAN-bus
    silence detection enabled. */
}sbc_trans_evnt_cbse_t;

/*!
 * @brief  Transceiver event capture enable register, CAN failure enable (0x23).
 *
 * Implements    : sbc_trans_evnt_cfe_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_CFE_DIS        = SBC_UJA_TRANS_EVNT_CFE_F(0U), /*!< CAN failure
    detection disabled. */
    SBC_UJA_TRANS_EVNT_CFE_EN         = SBC_UJA_TRANS_EVNT_CFE_F(1U), /*!< CAN failure
    detection enabled. */
}sbc_trans_evnt_cfe_t;

/*!
 * @brief  Transceiver event capture enable register, CAN wake-up enable (0x23).
 *
 * Implements    : sbc_trans_evnt_cwe_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_CWE_DIS        = SBC_UJA_TRANS_EVNT_CWE_F(0U), /*!< CAN wake-up
    detection disabled. */
    SBC_UJA_TRANS_EVNT_CWE_EN         = SBC_UJA_TRANS_EVNT_CWE_F(1U), /*!< CAN wake-up
    detection enabled. */
}sbc_trans_evnt_cwe_t;

/*!
 * @brief  Data rate register, CAN data rate selection (0x26).
 * CAN partial networking configuration registers.
 * Dedicated registers are provided for configuring CAN partial networking.
 *
 * Implements    : sbc_dat_rate_t_Class
 */
typedef enum{
    SBC_UJA_DAT_RATE_CDR_50KB          = SBC_UJA_DAT_RATE_CDR_F(0U), /*!< 50 kbit/s. */
    SBC_UJA_DAT_RATE_CDR_100KB         = SBC_UJA_DAT_RATE_CDR_F(1U), /*!< 100 kbit/s. */
    SBC_UJA_DAT_RATE_CDR_125KB         = SBC_UJA_DAT_RATE_CDR_F(2U), /*!< 125 kbit/s. */
    SBC_UJA_DAT_RATE_CDR_250KB         = SBC_UJA_DAT_RATE_CDR_F(3U), /*!< 250 kbit/s. */
    SBC_UJA_DAT_RATE_CDR_500KB         = SBC_UJA_DAT_RATE_CDR_F(5U), /*!< 500 kbit/s. */
    SBC_UJA_DAT_RATE_CDR_1000KB        = SBC_UJA_DAT_RATE_CDR_F(7U), /*!< 1000 kbit/s. */
}sbc_dat_rate_t;

/*!
 * @brief  ID registers, identifier format (0x27 to 0x2A).
 * A valid WUF identifier is defined and stored in the ID registers.
 * An ID mask can be defined to allow a group of identifiers to be recognized
 * as valid by an individual node.
 *
 * Implements    : sbc_identifier_t_Class
 */
typedef uint8_t sbc_identifier_t;

/*!
 * @brief  ID mask registers (0x2B to 0x2E).
 * The identifier mask is defined in the ID mask registers,
 * where a 1 means dont care.
 *
 * Implements    : sbc_identif_mask_t_Class
 */
typedef uint8_t sbc_identif_mask_t;

/*!
 * @brief  Frame control register, identifier format (0x2F).
 * The wake-up frame format, standard (11-bit) or extended (29-bit) identifier,
 * is selected via bit IDE in the Frame control register.
 *
 * Implements    : sbc_frame_ctr_ide_t_Class
 */
typedef enum{
    SBC_UJA_FRAME_CTR_IDE_11B          = SBC_UJA_FRAME_CTR_IDE_F(0U), /*!< Standard frame
    format (11-bit). */
    SBC_UJA_FRAME_CTR_IDE_29B          = SBC_UJA_FRAME_CTR_IDE_F(1U), /*!< Extended frame
    format (29-bit). */
}sbc_frame_ctr_ide_t;

/*!
 * @brief  Frame control register, partial networking data mask (0x2F).
 *
 * Implements    : sbc_frame_ctr_pndm_t_Class
 */
typedef enum{
    SBC_UJA_FRAME_CTR_PNDM_DCARE       = SBC_UJA_FRAME_CTR_PNDM_F(0U), /*!< Data length code
    and data field are do not care for wake-up. */
    SBC_UJA_FRAME_CTR_PNDM_EVAL        = SBC_UJA_FRAME_CTR_PNDM_F(1U), /*!< Data length code
    and data field are evaluated at wake-up. */
}sbc_frame_ctr_pndm_t;

/*!
 * @brief  Frame control register, number of data bytes expected
 * in a CAN frame (0x2F).
 *
 * Implements    : sbc_frame_ctr_dlc_t_Class
 */
typedef uint8_t sbc_frame_ctr_dlc_t;

/*!
 * @brief Data mask registers.
 * The data field indicates the nodes to be woken up. Within the data field,
 * groups of nodes can be predefined and associated with bits in a data mask.
 * By comparing the incoming data field with the data mask, multiple
 * groups of nodes can be woken up simultaneously with a single wake-up message.
 *
 * Implements    : sbc_data_mask_t_Class
 */
typedef uint8_t sbc_data_mask_t;

/*!
 * @brief  WAKE pin status register, WAKE pin status (0x4B).
 *
 * Implements    : sbc_wake_stat_wpvs_t_Class
 */
typedef enum{
    SBC_UJA_WAKE_STAT_WPVS_BE          = SBC_UJA_WAKE_STAT_WPVS_F(0U), /*!< Voltage on
    WAKE pin below switching threshold (Vth(sw)). */
    SBC_UJA_WAKE_STAT_WPVS_AB          = SBC_UJA_WAKE_STAT_WPVS_F(1U), /*!<voltage on
    WAKE pin above switching threshold (Vth(sw)). */
}sbc_wake_stat_wpvs_t;

/*!
 * @brief  WAKE pin event capture enable register, WAKE pin rising-edge enable (0x4C).
 *
 * Implements    : sbc_wake_en_wpre_t_Class
 */
typedef enum{
    SBC_UJA_WAKE_EN_WPRE_DIS           = SBC_UJA_WAKE_EN_WPRE_F(0U), /*!< Rising-edge
    detection on WAKE pin disabled. */
    SBC_UJA_WAKE_EN_WPRE_EN            = SBC_UJA_WAKE_EN_WPRE_F(1U), /*!< Rising-edge
    detection on WAKE pin enabled. */
}sbc_wake_en_wpre_t;

/*!
 * @brief  WAKE pin event capture enable register, WAKE pin falling-edge enable (0x4C).
 *
 * Implements    : sbc_wake_en_wpfe_t_Class
 */
typedef enum{
    SBC_UJA_WAKE_EN_WPFE_DIS           = SBC_UJA_WAKE_EN_WPFE_F(0U), /*!< Falling-edge
    detection on WAKE pin disabled. */
    SBC_UJA_WAKE_EN_WPFE_EN            = SBC_UJA_WAKE_EN_WPFE_F(1U), /*!< Falling-edge
    detection on WAKE pin enabled. */
}sbc_wake_en_wpfe_t;

/*!
 * @brief  Global event status register, WAKE pin event (0x60).
 *
 * Implements    : sbc_gl_evnt_stat_wpe_t_Class
 */
typedef enum{
    SBC_UJA_GL_EVNT_STAT_WPE_NO        = SBC_UJA_GL_EVNT_STAT_WPE_F(0U), /*!< No pending
    WAKE pin event. */
    SBC_UJA_GL_EVNT_STAT_WPE           = SBC_UJA_GL_EVNT_STAT_WPE_F(1U), /*!<  WAKE pin
    event pending at address 0x64. */
}sbc_gl_evnt_stat_wpe_t;

/*!
 * @brief  Global event status register, transceiver event (0x60).
 *
 * Implements    : sbc_gl_evnt_stat_trxe_t_Class
 */
typedef enum{
    SBC_UJA_GL_EVNT_STAT_TRXE_NO       = SBC_UJA_GL_EVNT_STAT_TRXE_F(0U), /*!< No pending
    transceiver event. */
    SBC_UJA_GL_EVNT_STAT_TRXE          = SBC_UJA_GL_EVNT_STAT_TRXE_F(1U), /*!< Transceiver
     event pending at address 0x63. */
}sbc_gl_evnt_stat_trxe_t;

/*!
 * @brief  Global event status register, supply event (0x60).
 *
 * Implements    : sbc_gl_evnt_stat_supe_t_Class
 */
typedef enum{
    SBC_UJA_GL_EVNT_STAT_SUPE_NO       = SBC_UJA_GL_EVNT_STAT_SUPE_F(0U), /*!< No pending
    supply event. */
    SBC_UJA_GL_EVNT_STAT_SUPE          = SBC_UJA_GL_EVNT_STAT_SUPE_F(1U), /*!< Supply event
    pending at address 0x62 .*/
}sbc_gl_evnt_stat_supe_t;

/*!
 * @brief  Global event status register, system event (0x60).
 *
 * Implements    : sbc_gl_evnt_stat_syse_t_Class
 */
typedef enum{
    SBC_UJA_GL_EVNT_STAT_SYSE_NO       = SBC_UJA_GL_EVNT_STAT_SYSE_F(0U), /*!< No pending
    system event. */
    SBC_UJA_GL_EVNT_STAT_SYSE          = SBC_UJA_GL_EVNT_STAT_SYSE_F(1U), /*!< System event
    pending at address 0x61. */
}sbc_gl_evnt_stat_syse_t;

/*!
 * @brief  System event status register, power-on (0x61).
 *
 * Implements    : sbc_sys_evnt_stat_po_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_STAT_PO_NO        = SBC_UJA_SYS_EVNT_STAT_PO_F(0U), /*!< No recent
    battery power-on. */
    SBC_UJA_SYS_EVNT_STAT_PO           = SBC_UJA_SYS_EVNT_STAT_PO_F(1U), /*!< The UJA116xA
    has left Off mode after battery power-on. */
}sbc_sys_evnt_stat_po_t;

/*!
 * @brief  System event status register, overtemperature warning (0x61).
 *
 * Implements    : sbc_sys_evnt_stat_otw_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_STAT_OTW_NO       = SBC_UJA_SYS_EVNT_STAT_OTW_F(0U), /*!< Overtemperature
     not detected. */
    SBC_UJA_SYS_EVNT_STAT_OTW          = SBC_UJA_SYS_EVNT_STAT_OTW_F(1U), /*!< The global
    chip temperature has exceeded the overtemperature warning threshold,
    Tth(warn)otp (not in Sleep mode). */
}sbc_sys_evnt_stat_otw_t;

/*!
 * @brief  System event status register, SPI failure (0x61).
 *
 * Implements    : sbc_sys_evnt_stat_spif_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_STAT_SPIF_NO      = SBC_UJA_SYS_EVNT_STAT_SPIF_F(0U), /*!< No SPI
    failure detected */
    SBC_UJA_SYS_EVNT_STAT_SPIF         = SBC_UJA_SYS_EVNT_STAT_SPIF_F(1U), /*!< SPI clock
    count error (only 16-, 24- and 32-bit commands are valid),
    illegal WMC, NWP or MC code or attempted write access to locked register
    (not in Sleep mode) */
}sbc_sys_evnt_stat_spif_t;

/*!
 * @brief  System event status register, watchdog failure (0x61).
 *
 * Implements    : sbc_sys_evnt_stat_wdf_t_Class
 */
typedef enum{
    SBC_UJA_SYS_EVNT_STAT_WDF_NO       = SBC_UJA_SYS_EVNT_STAT_WDF_F(0U), /*!< No watchdog
    failure event captured */
    SBC_UJA_SYS_EVNT_STAT_WDF          = SBC_UJA_SYS_EVNT_STAT_WDF_F(1U), /*!< Watchdog
    overflow in Window or Timeout mode or watchdog triggered too early in
    Window mode; a system reset is triggered immediately in response to
    a watchdog failure in Window mode; when the watchdog overflows
    in Timeout mode, a system reset is only performed if a WDF is already
    pending (WDF = 1). */
}sbc_sys_evnt_stat_wdf_t;

/*!
 * @brief  Supply event status register, V2/VEXT overvoltage (0x62).
 *
 * Implements    : sbc_sup_evnt_stat_v2o_t_Class
 */
typedef enum{
    SBC_UJA_SUP_EVNT_STAT_V2O_NO       = SBC_UJA_SUP_EVNT_STAT_V2O_F(0U), /*!< No V2/VEXT
    overvoltage event captured. */
    SBC_UJA_SUP_EVNT_STAT_V2O          = SBC_UJA_SUP_EVNT_STAT_V2O_F(1U), /*!< V2/VEXT
    overvoltage event captured. */
}sbc_sup_evnt_stat_v2o_t;

/*!
 * @brief  Supply event status register, V2/VEXT undervoltage (0x62).
 *
 * Implements    : sbc_sup_evnt_stat_v2u_t_Class
 */
typedef enum{
    SBC_UJA_SUP_EVNT_STAT_V2U_NO       = SBC_UJA_SUP_EVNT_STAT_V2U_F(0U), /*!< No V2/VEXT
    undervoltage event captured. */
    SBC_UJA_SUP_EVNT_STAT_V2U          = SBC_UJA_SUP_EVNT_STAT_V2U_F(1U), /*!< V2/VEXT
    undervoltage event captured. */
}sbc_sup_evnt_stat_v2u_t;

/*!
 * @brief  Supply event status register, V1 undervoltage (0x62).
 *
 * Implements    : sbc_sup_evnt_stat_v1u_t_Class
 */
typedef enum{
    SBC_UJA_SUP_EVNT_STAT_V1U_NO       = SBC_UJA_SUP_EVNT_STAT_V1U_F(0U), /*!< no V1
    undervoltage event captured. */
    SBC_UJA_SUP_EVNT_STAT_V1U          = SBC_UJA_SUP_EVNT_STAT_V1U_F(1U), /*!< voltage
    on V1 has dropped below the 90 % undervoltage threshold while V1 is active
    (event is not captured in Sleep mode because V1 is off); V1U event capture
    is independent of the setting of bits V1RTC. */
}sbc_sup_evnt_stat_v1u_t;

/*!
 * @brief  Transceiver event status register,partial
 * networking frame detection error (0x63).
 *
 * Implements    : sbc_trans_evnt_stat_pnfde_t_Class
 * */
typedef enum{
    SBC_UJA_TRANS_EVNT_STAT_PNFDE_NO   = SBC_UJA_TRANS_EVNT_STAT_PNFDE_F(0U), /*!< No partial
    networking frame detection error detected. */
    SBC_UJA_TRANS_EVNT_STAT_PNFDE      = SBC_UJA_TRANS_EVNT_STAT_PNFDE_F(1U), /*!< Partial
    networking frame detection error detected. */
}sbc_trans_evnt_stat_pnfde_t;

/*!
 * @brief  Transceiver event status register, CAN-bus status (0x63).
 *
 * Implements    : sbc_trans_evnt_stat_cbs_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_STAT_CBS_NO     = SBC_UJA_TRANS_EVNT_STAT_CBS_F(0U), /*!< CAN-bus
    active. */
    SBC_UJA_TRANS_EVNT_STAT_CBS        = SBC_UJA_TRANS_EVNT_STAT_CBS_F(1U), /*!< No activity
    on CAN-bus for tto(silence)
    (detected only when CBSE = 1 while bus active). */
}sbc_trans_evnt_stat_cbs_t;

/*!
 * @brief  Transceiver event status register, CAN failure (0x63).
 *
 * Implements    : sbc_trans_evnt_stat_cf_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_STAT_CF_NO      = SBC_UJA_TRANS_EVNT_STAT_CF_F(0U), /*!< No CAN
    failure detected. */
    SBC_UJA_TRANS_EVNT_STAT_CF         = SBC_UJA_TRANS_EVNT_STAT_CF_F(1U), /*!< CAN transceiver
    deactivated due to VCAN undervoltage OR dominant clamped TXD
    (not in Sleep mode) */
}sbc_trans_evnt_stat_cf_t;

/*!
 * @brief  Transceiver event status register, CAN wake-up (0x63).
 *
 * Implements    : sbc_trans_evnt_stat_cw_t_Class
 */
typedef enum{
    SBC_UJA_TRANS_EVNT_STAT_CW_NO      = SBC_UJA_TRANS_EVNT_STAT_CW_F(0U), /*!<No CAN wake-up
    event detected. */
    SBC_UJA_TRANS_EVNT_STAT_CW         = SBC_UJA_TRANS_EVNT_STAT_CW_F(1U), /*!< CAN wake-up
    event detected while the transceiver is in CAN Offline Mode. */
}sbc_trans_evnt_stat_cw_t;

/*!
 * @brief  WAKE pin event status register, WAKE pin rising edge (0x64).
 *
 * Implements    : sbc_wake_evnt_stat_wpr_t_Class
 */
typedef enum{
    SBC_UJA_WAKE_EVNT_STAT_WPR_NO      = SBC_UJA_WAKE_EVNT_STAT_WPR_F(0U), /*!< No rising
    edge detected on WAKE pin. */
    SBC_UJA_WAKE_EVNT_STAT_WPR         = SBC_UJA_WAKE_EVNT_STAT_WPR_F(1U), /*!< Rising edge
    detected on WAKE pin. */
}sbc_wake_evnt_stat_wpr_t;

/*!
 * @brief  WAKE pin event status register, WAKE pin falling edge (0x64).
 *
 * Implements    : sbc_wake_evnt_stat_wpf_t_Class
 */
typedef enum{
    SBC_UJA_WAKE_EVNT_STAT_WPF_NO      = SBC_UJA_WAKE_EVNT_STAT_WPF_F(0U), /*!< No falling
    edge detected on WAKE pin. */
    SBC_UJA_WAKE_EVNT_STAT_WPF         = SBC_UJA_WAKE_EVNT_STAT_WPF_F(1U), /*!< Falling edge
    detected on WAKE pin. */
}sbc_wake_evnt_stat_wpf_t;

/*!
 * @brief  MTPNV status register, write counter status (0x70).
 * 6-bits - contains the number of times the MTPNV
 * cells were reprogrammed.
 *
 * Implements    : sbc_mtpnv_stat_wrcnts_t_Class
 */
typedef uint8_t sbc_mtpnv_stat_wrcnts_t;

/*!
 * @brief  MTPNV status register, error correction code status (0x70).
 *
 * Implements    : sbc_mtpnv_stat_eccs_t_Class
 */
typedef enum{
    SBC_UJA_MTPNV_STAT_ECCS_NO         = SBC_UJA_MTPNV_STAT_ECCS_F(0U), /*!<  No bit failure
    detected in non-volatile memory. */
    SBC_UJA_MTPNV_STAT_ECCS            = SBC_UJA_MTPNV_STAT_ECCS_F(1U), /*!<  Bit failure detected
    and corrected in non-volatile memory. */
}sbc_mtpnv_stat_eccs_t;

/*!
 * @brief  MTPNV status register, non-volatile memory programming status (0x70).
 *
 * Implements    : sbc_mtpnv_stat_nvmps_t_Class
 */
typedef enum{
    SBC_UJA_MTPNV_STAT_NVMPS_NO        = SBC_UJA_MTPNV_STAT_NVMPS_F(0U), /*!<  MTPNV memory
    cannot be overwritten. */
    SBC_UJA_MTPNV_STAT_NVMPS           = SBC_UJA_MTPNV_STAT_NVMPS_F(1U), /*!<  MTPNV memory
    is ready to be reprogrammed. */
}sbc_mtpnv_stat_nvmps_t;

/*!
 * @brief  Start-up control register, RSTN output reset pulse width macros (0x73).
 *
 * Implements    : sbc_start_up_rlc_t_Class
 */
typedef enum{
    SBC_UJA_START_UP_RLC_20_25p0       = SBC_UJA_START_UP_RLC_F(0U), /*!<  Tw(rst) = 20 ms
    to 25 ms. */
    SBC_UJA_START_UP_RLC_10_12p5       = SBC_UJA_START_UP_RLC_F(1U), /*!<  Tw(rst) = 10 ms
    to 12.5 ms. */
    SBC_UJA_START_UP_RLC_03p6_05       = SBC_UJA_START_UP_RLC_F(2U), /*!<  Tw(rst) = 3.6 ms
    to 5 ms. */
    SBC_UJA_START_UP_RLC_01_01p5       = SBC_UJA_START_UP_RLC_F(3U), /*!<  Tw(rst) = 1 ms
    to 1.5 ms. */
}sbc_start_up_rlc_t;

/*!
 * @brief  Start-up control register, V2/VEXT start-up control (0x73).
 *
 * Implements    : sbc_start_up_v2suc_t_Class
 */
typedef enum{
    SBC_UJA_START_UP_V2SUC_00          = SBC_UJA_START_UP_V2SUC_F(0U), /*!<  bits V2C/VEXTC
    set to 00 at power-up. */
    SBC_UJA_START_UP_V2SUC_11          = SBC_UJA_START_UP_V2SUC_F(1U), /*!<  bits V2C/VEXTC
    set to 11 at power-up. */
}sbc_start_up_v2suc_t;

/*!
 * @brief  SBC configuration control register, V1 undervoltage
 * threshold (defined by bit V1RTC) at start-up (0x74).
 *
 * Implements    : sbc_sbc_v1rtsuc_t_Class
 */
typedef enum{
    SBC_UJA_SBC_V1RTSUC_90             = SBC_UJA_SBC_V1RTSUC_F(0U), /*!< V1 undervoltage
    detection at 90 % of nominal value at start-up (V1RTC = 00). */
    SBC_UJA_SBC_V1RTSUC_80             = SBC_UJA_SBC_V1RTSUC_F(1U), /*!< V1 undervoltage
    detection at 80 % of nominal value at start-up (V1RTC = 01). */
    SBC_UJA_SBC_V1RTSUC_70             = SBC_UJA_SBC_V1RTSUC_F(2U), /*!< V1 undervoltage
    detection at 70 % of nominal value at start-up V1RTC = 10). */
    SBC_UJA_SBC_V1RTSUC_60             = SBC_UJA_SBC_V1RTSUC_F(3U), /*!< V1 undervoltage
    detection at 60 % of nominal value at start-up (V1RTC = 11). */
}sbc_sbc_v1rtsuc_t;

/*!
 * @brief  SBC configuration control register, Forced Normal mode control (0x74).
 *
 * Implements    : sbc_sbc_fnmc_t_Class
 */
typedef enum{
    SBC_UJA_SBC_FNMC_DIS               = SBC_UJA_SBC_FNMC_F(0U), /*!<  Forced Normal
    mode disabled. */
    SBC_UJA_SBC_FNMC_EN                = SBC_UJA_SBC_FNMC_F(1U), /*!<  Forced Normal
    mode enabled. */
}sbc_sbc_fnmc_t;

/*!
 * @brief  SBC configuration control register,
 * Software Development mode control (0x74).
 *
 * Implements    : sbc_sbc_sdmc_t_Class
 */
typedef enum{
    SBC_UJA_SBC_SDMC_DIS               = SBC_UJA_SBC_SDMC_F(0U), /*!<  Software Development
    mode disabled. */
    SBC_UJA_SBC_SDMC_EN                = SBC_UJA_SBC_SDMC_F(1U), /*!<  Software Development
    mode enabled. */
}sbc_sbc_sdmc_t;

/*!
 * @brief  SBC configuration control register,
 * Sleep control (0x74).
 *
 * Implements    : sbc_sbc_slpc_t_Class
 */
typedef enum{
    SBC_UJA_SBC_SLPC_AC                = SBC_UJA_SBC_SLPC_F(0U), /*!<  Sleep mode
    commands accepted. Factory preset value. */
    SBC_UJA_SBC_SLPC_IG                = SBC_UJA_SBC_SLPC_F(1U), /*!<  Sleep mode
    commands ignored. */
}sbc_sbc_slpc_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*!
* @brief Watchdog control register structure.
* Watchdog configuration structure.
*
* Implements    : sbc_wtdog_ctr_t_Class
*/
typedef struct{
    sbc_wtdog_ctr_wmc_t modeControl;          /*!< Watchdog mode control. */
    sbc_wtdog_ctr_nwp_t nominalPeriod;        /*!< Nominal watchdog period. */
}sbc_wtdog_ctr_t;

/*!
* @brief SBC configuration control register structure.
* Two operating modes have a major impact on the operation of the watchdog:
* Forced Normal mode and Software Development mode (Software Development mode
* is provided for test and development purposes only and is not a dedicated
* SBC operating mode; the UJA116xA can be in any functional operating mode
* with Software Development mode enabled). These modes are
* enabled and disabled via bits FNMC and SDMC respectively in the SBC
* configuration control register. Note that this register
* is located in the non-volatile memory area. The watchdog is disabled
* in Forced Normal mode (FNM). In Software Development mode (SDM),
* the watchdog can be disabled or activated for test and software
* debugging purposes.
*
* Implements    : sbc_sbc_t_Class
*/
typedef struct{
    sbc_sbc_v1rtsuc_t v1rtsuc;                /*!< V1 undervoltage threshold
    (defined by bit V1RTC) at start-up (0x74). */
    sbc_sbc_fnmc_t fnmc;                      /*!< Forced Normal mode control. */
    sbc_sbc_sdmc_t sdmc;                      /*!< Software Development
    mode control. */
    sbc_sbc_slpc_t slpc;                      /*!< Sleep control. */
}sbc_sbc_t;

/*!
* @brief Start-up control register structure.
* This structure contains settings of RSTN output reset pulse width
* and V2/VEXT start-up control
*
* Implements    : sbc_start_up_t_Class
*/
typedef struct{
    sbc_start_up_rlc_t rlc;                   /*!< RSTN output reset pulse
    width macros. */
    sbc_start_up_v2suc_t v2suc;               /*!< V2/VEXT start-up control. */
}sbc_start_up_t;

/*!
* @brief Regulator control register structure.
* This structure set power distribution control, V2/VEXT configuration,
* set V1 reset threshold.
*
* Implements    : sbc_regulator_t_Class
*/
typedef struct{
    sbc_regulator_pdc_t pdc;                  /*!< Power distribution control. */
    sbc_regulator_v2c_t v2c;                  /*!< V2/VEXT configuration. */
    sbc_regulator_v1rtc_t v1rtc;              /*!< Set V1 reset threshold. */
}sbc_regulator_t;

/*!
* @brief  Supply event capture enable register structure.
* This structure enables or disables detection of V2/VEXT overvoltage,
* undervoltage and V1 undervoltage enable.
*
* Implements    : sbc_supply_evnt_t_Class
*/
typedef struct{
    sbc_supply_evnt_v2oe_t v2oe;              /*!< V2/VEXT overvoltage enable. */
    sbc_supply_evnt_v2ue_t v2ue;              /*!< V2/VEXT undervoltage enable. */
    sbc_supply_evnt_v1ue_t v1ue;              /*!< SV1 undervoltage enable. */
}sbc_supply_evnt_t;

/*!
* @brief  System event capture enable register structure.
* This structure enables or disables overtemperature warning,
* SPI failure enable.
*
* Implements    : sbc_sys_evnt_t_Class
*/
typedef struct{
    sbc_sys_evnt_otwe_t owte;                 /*!< Overtemperature warning enable. */
    sbc_sys_evnt_spife_t spife;               /*!< SPI failure enable. */
}sbc_sys_evnt_t;

/*!
* @brief  CAN control register structure.
* This structure configure CAN peripheral behavior.
*
* Implements    : sbc_can_ctr_t_Class
*/
typedef struct{
    sbc_can_cfdc_t cfdc;                      /*!< CAN FD control. */
    sbc_can_pncok_t pncok;                    /*!< CAN partial networking.
    configuration OK. */
    sbc_can_cpnc_t cpnc;                      /*!< CAN partial.
    networking control. */
    sbc_can_cmc_t cmc;                        /*!< CAN mode control. */
}sbc_can_ctr_t;

/*!
* @brief  Transceiver event capture enable register structure.
* Can bus silence, Can failure and Can wake-up settings.
*
* Implements    : sbc_trans_evnt_t_Class
*/
typedef struct{
    sbc_trans_evnt_cbse_t cbse;               /*!< CAN-bus silence enable. */
    sbc_trans_evnt_cfe_t cfe;                 /*!< CAN failure enable. */
    sbc_trans_evnt_cwe_t cwe;                 /*!< CAN wake-up enable. */
}sbc_trans_evnt_t;

/*!
* @brief  Frame control register structure.
* The wake-up frame format, standard (11-bit) or extended (29-bit) identifier,
* is selected via bit IDE in the Frame control register.
*
* Implements    : sbc_frame_t_Class
*/
typedef struct{
    sbc_frame_ctr_ide_t ide;                  /*!< Identifier format. */
    sbc_frame_ctr_pndm_t pndm;                /*!< Partial networking data mask. */
    sbc_frame_ctr_dlc_t dlc;                  /*!< Number of data bytes expected. */
}sbc_frame_t;

/*!
* @brief  CAN configuration group structure.
* This structure configure CAN peripheral behavior.
*
* Implements    : sbc_can_conf_t_Class
*/
typedef struct{
    sbc_can_ctr_t canConf;                      /*!< CAN control register. */
    sbc_trans_evnt_t canTransEvnt;              /*!< Transceiver event
    capture enable register. */
    sbc_dat_rate_t datRate;                     /*!< CAN data rate selection. */
    sbc_identifier_t identif[SBC_UJA_COUNT_ID_REG];     /*!< ID registers. */
    sbc_identif_mask_t mask[SBC_UJA_COUNT_MASK];        /*!< ID mask registers. */
    sbc_frame_t frame;                          /*!< Frame control register. */
    sbc_data_mask_t dataMask[SBC_UJA_COUNT_DMASK];      /*!< Data mask 0 - 7 configuration. */
}sbc_can_conf_t;

/*!
* @brief  WAKE pin event capture enable register structure.
* Local wake-up is enabled via bits WPRE and WPFE in the WAKE pin event capture
* enable register. A wake-up event is triggered
* by a LOW-to-HIGH (ifWPRE = 1) and/or a HIGH-to-LOW (if WPFE = 1) transition
* on the WAKE pin. This arrangement allows for maximum flexibility when
* designing a local wake-up circuit. In applications that do not use the local
* wake-up facility, local wake-up should be disabled and the WAKE pin
* connected to GND.
*
* Implements    : sbc_wake_t_Class
*/
typedef struct{
    sbc_wake_en_wpre_t wpre;                  /*!< WAKE pin rising-edge enable. */
    sbc_wake_en_wpfe_t wpfe;                  /*!< WAKE pin falling-edge enable. */
}sbc_wake_t;

/*!
* @brief  Regulator control register group.
* This structure is group of regulator settings.
*
* Implements    : sbc_regulator_ctr_t_Class
*/
typedef struct{
    sbc_regulator_t regulator;                /*!< Regulator control register. */
    sbc_supply_evnt_t supplyEvnt;             /*!< Supply event capture
    enable register. */
}sbc_regulator_ctr_t;

/*!
* @brief Init configuration structure.
* This structure is used for initialization of sbc.
*
* Implements    : sbc_int_config_t_Class
*/
typedef struct{
    sbc_regulator_ctr_t regulatorCtr;         /*!< Regulator control register group. */
    sbc_wtdog_ctr_t watchdog;                 /*!< Watchdog control register. */
    sbc_mode_mc_t mode;                       /*!< Mode control register. */
    sbc_fail_safe_lhc_t lhc;                  /*!< LIMP home control. */
    sbc_sys_evnt_t sysEvnt;                   /*!< System event capture
    enable registers. */
    sbc_lock_t lockMask;                      /*!< Lock control register. */
    sbc_can_conf_t can;                       /*!< CAN configuration group. */
    sbc_wake_t wakePin;                       /*!< WAKE pin event
    capture enable register. */
}sbc_int_config_t;

/*!
* @brief Factory configuration structure.
* It contains Start-up control register and SBC configuration control register.
* This is non-volatile memory with limited write access.
* The MTPNV cells can be reprogrammed a maximum of 200 times (Ncy(W)MTP;
* Bit NVMPS in the MTPNV status register indicates whether the
* non-volatile cells can be reprogrammed. This register also contains a
* write counter, WRCNTS, that is incremented each time the MTPNV cells
* are reprogrammed (up to a maximum value of 111111; there is no overflow;
* performing a factory reset also increments the counter).
* This counter is provided for information purposes only; reprogramming will
* not be rejected when it reaches its maximum value.
* Factory preset values are restored if the following conditions apply
* continuously for at least td(MTPNV) during battery power-up:
* pin RSTN is held LOW, CANH is pulled up to VBAT, CANL is pulled down to GND
* After the factory preset values have been restored, the SBC performs
* a system reset and enters Forced normal Mode.
* Since the CAN-bus is clamped dominant, pin RXDC is forced LOW. Pin RXD
* is forced HIGH during the factory preset restore process (td(MTPNV)).
* A falling edge on RXD caused by bit PO being set after power-on indicates
* that the factory preset process has been completed.
* Note that the write counter, WRCNTS, in the MTPNV status register
* is incremented every time the factory presets are restored.
*
* Implements    : sbc_factories_conf_t_Class
*/
typedef struct{
    sbc_start_up_t startUp;                   /*!< Start-up control register. */
    sbc_sbc_t control;                        /*!< SBC configuration
    control register.  Note that this register is located in the non-volatile
    memory area.*/
}sbc_factories_conf_t;

/*!
* @brief  Main status register structure.
* The Main status register can be accessed to monitor the status of the
* overtemperature warning flag and to determine whether the UJA116xA
* has entered Normal mode after initial power-up. It also indicates the source
* of the most recent reset event.
*
* Implements    : sbc_main_status_t_Class
*/
typedef struct{
    sbc_main_otws_t otws;                     /*!< Overtemperature warning status. */
    sbc_main_nms_t nms;                       /*!< Normal mode status. */
    sbc_main_rss_t rss;                       /*!< Reset source status. */
}sbc_main_status_t;

/*!
* @brief  Watchdog status register structure.
* Information on the status of the watchdog is available from the
* Watchdog status register. This register also indicates
* whether Forced Normal and Software Development modes are active.
*
* Implements    : sbc_wtdog_status_t_Class
*/
typedef struct{
    sbc_wtdog_stat_fnms_t fnms;               /*!< Forced Normal mode status. */
    sbc_wtdog_stat_sdms_t sdms;               /*!< Software Development mode status. */
    sbc_wtdog_stat_wds_t wds;                 /*!< Watchdog status. */
}sbc_wtdog_status_t;

/*!
* @brief  Supply voltage status register structure.
* V2/VEXT and V1 undervoltage and overvoltage status.
*
* Implements    : sbc_supply_status_t_Class
*/
typedef struct{
    sbc_supply_stat_v2s_t v2s;                /*!< V2/VEXT status. */
    sbc_supply_stat_v1s_t v1s;                /*!< V1 status. */
}sbc_supply_status_t;

/*!
* @brief Transceiver status register structure.
* There are stored CAN transceiver statuses.
*
* Implements    : sbc_trans_stat_t_Class
*/
typedef struct{
    sbc_trans_stat_cts_t cts;                 /*!< CAN transceiver status. */
    sbc_trans_stat_cpnerr_t cpnerr;           /*!< CAN partial networking error. */
    sbc_trans_stat_cpns_t cpns;               /*!< CAN partial networking status. */
    sbc_trans_stat_coscs_t coscs;             /*!< CAN oscillator status. */
    sbc_trans_stat_cbss_t cbss;               /*!< CAN-bus silence status. */
    sbc_trans_stat_vcs_t vcs;                 /*!< VCAN status. */
    sbc_trans_stat_cfs_t cfs;                 /*!< CAN failure status. */
}sbc_trans_stat_t;

/*!
* @brief  Global event status register.
* The microcontroller can monitor events via the event status registers.
* An extra status register, the Global event status register,
* is provided to help speed up software polling routines. By polling the Global
* event status register, the microcontroller can quickly determine the
* type of event captured (system, supply, transceiver or WAKE pin) and then
* query the relevant event status register.
*
* Implements    : sbc_gl_evnt_stat_t_Class
*/
typedef struct{
    sbc_gl_evnt_stat_wpe_t wpe;               /*!< WAKE pin event. */
    sbc_gl_evnt_stat_trxe_t trxe;             /*!< Transceiver event. */
    sbc_gl_evnt_stat_supe_t supe;             /*!< Supply event. */
    sbc_gl_evnt_stat_syse_t syse;             /*!< System event. */
}sbc_gl_evnt_stat_t;

/*!
* @brief  System event status register.
* Wake-up and interrupt event diagnosis in the UJA116xA is intended to provide
* the microcontroller with information on the status of a range of features
* and functions. This information is stored in the event status registers
* and is signaled on pin RXD, if enabled.
*
* Implements    : sbc_sys_evnt_stat_t_Class
*/
typedef struct{
    sbc_sys_evnt_stat_po_t po;                /*!< Power-on. */
    sbc_sys_evnt_stat_otw_t otw;              /*!< Transceiver event,
    overtemperature warning */
    sbc_sys_evnt_stat_spif_t spif;            /*!< SPI failure. */
    sbc_sys_evnt_stat_wdf_t wdf;              /*!< Watchdog failure. */
}sbc_sys_evnt_stat_t;

/*!
* @brief  Supply event status register.
*
* Implements    : sbc_sup_evnt_stat_t_Class
*/
typedef struct{
    sbc_sup_evnt_stat_v2o_t v2o;              /*!< V2/VEXT overvoltage. */
    sbc_sup_evnt_stat_v2u_t v2u;              /*!< V2/VEXT undervoltage. */
    sbc_sup_evnt_stat_v1u_t v1u;              /*!< V1 undervoltage. */
}sbc_sup_evnt_stat_t;

/*!
* @brief  Transceiver event status register.
*
* Implements    : sbc_trans_evnt_stat_t_Class
*/
typedef struct{
    sbc_trans_evnt_stat_pnfde_t pnfde;        /*!< Partial networking
    frame detection error. */
    sbc_trans_evnt_stat_cbs_t cbs;            /*!< CAN-bus status. */
    sbc_trans_evnt_stat_cf_t cf;              /*!< CAN failure. */
    sbc_trans_evnt_stat_cw_t cw;              /*!< CAN wake-up. */
}sbc_trans_evnt_stat_t;

/*!
* @brief  WAKE pin event status register.
*
* Implements    : sbc_wake_evnt_stat_t_Class
*/
typedef struct{
    sbc_wake_evnt_stat_wpr_t wpr;             /*!< WAKE pin rising edge. */
    sbc_wake_evnt_stat_wpf_t wpf;             /*!< WAKE pin falling edge. */
}sbc_wake_evnt_stat_t;

/*!
* @brief  Event capture registers structure.
* This structure contains Global event status, System event status,
* Supply event status, Transceiver event status, WAKE pin event status.
*
* Implements    : sbc_evn_capt_t_Class
*/
typedef struct{
    sbc_gl_evnt_stat_t glEvnt;                /*!< Global event status. */
    sbc_sys_evnt_stat_t sysEvnt;              /*!< System event status. */
    sbc_sup_evnt_stat_t supEvnt;              /*!< Supply event status. */
    sbc_trans_evnt_stat_t transEvnt;          /*!< Transceiver event status. */
    sbc_wake_evnt_stat_t wakePinEvnt;         /*!< WAKE pin event status. */
}sbc_evn_capt_t;

/*!
* @brief  MTPNV status register.
* The MTPNV cells can be reprogrammed a maximum of 200 times (Ncy(W)MTP).
* Bit NVMPS in the MTPNV status register indicates
* whether the non-volatile cells can be reprogrammed. This register also
* contains a write counter, WRCNTS, that is incremented each time the
* MTPNV cells are reprogrammed (up to a maximum value of 111111; there is
* no overflow; performing a factory reset also increments the counter).
* This counter is provided for information purposes only; reprogramming will
* not be rejected when it reaches its maximum value.
*
* Implements    : sbc_mtpnv_stat_t_Class
*/
typedef struct{
    sbc_mtpnv_stat_wrcnts_t wrcnts;           /*!< Write counter status. */
    sbc_mtpnv_stat_eccs_t eccs;               /*!< Error correction code status. */
    sbc_mtpnv_stat_nvmps_t nvmps;             /*!< Non-volatile memory
    programming status. */
}sbc_mtpnv_stat_t;


/*!
* @brief Status group structure.
* All statuses of SBC are stored in this structure.
*
* Implements    : sbc_status_group_t_Class
*/
typedef struct{
    sbc_main_status_t mainS;              /*!< Main status. */
    sbc_wtdog_status_t wtdog;             /*!< Watchdog status. */
    sbc_supply_status_t supply;           /*!< Supply voltage status. */
    sbc_trans_stat_t trans;               /*!< Transceiver status. */
    sbc_wake_stat_wpvs_t wakePin;         /*!< WAKE pin status. */
    sbc_evn_capt_t events;                /*!< Event capture registers. */
}sbc_status_group_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*! @} */

/* Functions for configure registers of SBC. */

/*!
 * @brief This function internally stores LPSPI instance used by the driver.
 *
 * This function needs to be used before any other function after reset
 * (for example when reading pre-reset or wake-up events).
 *
 * @param lpspiInstance  is instance of LPSPI device.
 */
void SBC_InitDriver(const uint32_t lpspiInstance);

/*!
 * @brief This function initializes all registers.
 * 
 * The function needs to be used in sequence with SBC_InitDriver,
 * for example when reading pre-reset or wake-up events using SBC_GetEventsStatus
 * (SBC_InitDriver first, SBC_InitDevice last after reading the status).
 *
 * @param config  structure which contains configuration of all registers.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_InitDevice(const sbc_int_config_t *const config);

/*!
 * @brief This function configures Regulator control registers.
 *
 * Regulator control (0x10),  Supply status (0x1B), Supply event enable(0x1C).
 * @param regulatorCtr  regulator registers structure which will be written
 * to device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetVreg(const sbc_regulator_ctr_t* const regulatorCtr);

/*!
 * @brief This function reads Regulator control registers.
 *
 * Regulator control (0x10), Supply event enable(0x1C).
 *
 * @param regulatorCtr  regulator registers structure where device
 * data will be stored.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetVreg(sbc_regulator_ctr_t* const regulatorCtr);

/*!
 * @brief This function configures Watchdog control register (0x00).
 *
 * This function selects Watchdog mode control, and nominal watchdog period.
 *
 * The UJA116xA contains a watchdog that supports three operating modes: Window,
 * Timeout and Autonomous. In Window mode (available only in SBC Normal mode), a
 * watchdog trigger event within a defined watchdog window triggers and resets
 * the watchdog timer. In Timeout mode, the watchdog runs continuously and can
 * be triggered and reset at any time within the watchdog period by a watchdog
 * trigger. Watchdog time-out mode can also be used for cyclic wake-up
 * of the microcontroller. In Autonomous mode, the watchdog can be off or
 * autonomously in Timeout mode, depending on the selected SBC mode.
 * Note SBC mode will temporary set to StandBy while WatchDog configuration
 * is changed.
 *
 * @param wtdog  watchdog registers structure which will be written to device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetWatchdog(const sbc_wtdog_ctr_t* const wtdog);

/*!
 * @brief This function reads Watchdog control register (0x00).
 *
 * This function reads Watchdog mode control, and nominal watchdog period.
 *
 * The UJA116xA contains a watchdog that supports three operating modes: Window,
 * Timeout and Autonomous. In Window mode (available only in SBC Normal mode), a
 * watchdog trigger event within a defined watchdog window triggers and resets
 * the watchdog timer. In Timeout mode, the watchdog runs continuously and can
 * be triggered and reset at any time within the watchdog period by a watchdog
 * trigger. Watchdog time-out mode can also be used for cyclic wake-up
 * of the microcontroller. In Autonomous mode, the watchdog can be off or
 * autonomously in Timeout mode, depending on the selected SBC mode.
 *
 * @param wtdog  watchdog registers structure which will be read from device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetWatchdog(sbc_wtdog_ctr_t* const wtdog);

/*!
 * @brief This function writes to Mode control register. (0x01).
 *
 * Normal mode is the active operating mode. In this mode, all the hardware on
 * the device is available and can be activated. Voltage regulator
 * V1 is enabled to supply the microcontroller.
 * Standby mode is the first-level power-saving mode of the UJA116xA, offering
 * reduced current consumption. The transceiver is unable to transmit or
 * receive data in Standby mode. The SPI remains enabled and V1 is still active;
 * the watchdog is active (in Timeout mode) if enabled. The behavior
 * of V2/VEXT is determined by the SPI setting.
 * Sleep mode is the second-level power-saving mode of the UJA116xA.
 * The difference between Sleep and Standby modes is that V1 is off in
 * Sleep mode and temperature protection is inactive.
 * Note event status are cleared before device move to sleep mode. At least
 * one wake up event must be enabled before moving to sleep mode otherwise SBC
 * will be reseted.
 *
 * @param mode  device mode - Normal, StandBy, Sleep.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetMode(const sbc_mode_mc_t mode);

/*!
 * @brief This function reads Mode control register. (0x01).
 *
 * Normal mode is the active operating mode. In this mode, all the hardware on
 * the device is available and can be activated. Voltage regulator
 * V1 is enabled to supply the microcontroller.
 * Standby mode is the first-level power-saving mode of the UJA116xA, offering
 * reduced current consumption. The transceiver is unable to transmit or
 * receive data in Standby mode. The SPI remains enabled and V1 is still active;
 * the watchdog is active (in Timeout mode) if enabled. The behavior
 * of V2/VEXT is determined by the SPI setting.
 * Sleep mode is the second-level power-saving mode of the UJA116xA.
 * The difference between Sleep and Standby modes is that V1 is off in
 * Sleep mode and temperature protection is inactive.
 * Note event status are cleared before device move to sleep mode.
 *
 * @param mode  variable for store device mode - Normal, StandBy, Sleep.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetMode(sbc_mode_mc_t* const mode);

/*!
 * @brief This function writes to Fail-safe control register (0x02).
 * The dedicated LIMP pin can be used to enable so called limp home hardware
 * in the event of a serious ECU failure. Detectable failure conditions include
 * SBC overtemperature events, loss of watchdog service, short-circuits on pins
 * RSTN or V1 and user-initiated or external reset events.
 * The LIMP pin is a battery-robust, active-LOW, open-drain output.
 * The LIMP pin can also be forced LOW by setting bit LHC in the Fail-safe
 * control register.
 * The limp-home function and the reset counter are disabled in
 * Forced Normal mode. The LIMP pin is floating, RCC remains unchanged
 * and bit LHC = 0.
 * RCC -reset counter control.
 * incremented every time the SBC enters Reset mode while FNMC = 0;
 * RCC overflows from 11 to 00; default at power-on is 00. For ignore settings
 * of rcc use NULL pointer or otherwise send pointer to variable.
 * This function is not available on UJA1168 device variants.
 *
 * @param lhc  Variable for set limp home control.
 * @param rcc  Pointer to rcc. Use null pointer or wrong value for ignore parameter
 * or set value 0x00 to 0x03.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetFailSafe(const sbc_fail_safe_lhc_t lhc,
        const sbc_fail_safe_rcc_t* const rcc);

/*!
 * @brief This function reads from Fail-safe control register (0x02).
 * The dedicated LIMP pin can be used to enable so called limp home hardware
 * in the event of a serious ECU failure. Detectable failure conditions include
 * SBC overtemperature events, loss of watchdog service, short-circuits on pins
 * RSTN or V1 and user-initiated or external reset events.
 * The LIMP pin is a battery-robust, active-LOW, open-drain output.
 * The LIMP pin can also be forced LOW by setting bit LHC in the Fail-safe
 * control register.
 * The limp-home function and the reset counter are disabled in
 * Forced Normal mode. The LIMP pin is floating, RCC remains unchanged
 * and bit LHC = 0.
 * This function is not available on UJA1168 device variants.
 *
 * @param lhc  Variable for reading of limp home control.
 * @param rcc  Pointer to rcc. Use null pointer or wrong value for ignore parameter
 * or empty for reading.
 *
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetFailSafe(sbc_fail_safe_lhc_t* const lhc,
        sbc_fail_safe_rcc_t * const rcc);

/*!
 * @brief This function writes System event capture enable register (0x04).
 * This function enables or disables overtemperature warning,
 * SPI failure enable.
 *
 * @param sysEvnt  system event capture enable register structure which will be
 * written to device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetSystemEvents(const sbc_sys_evnt_t* const sysEvnt);

/*!
 * @brief This function reads System event capture enable register (0x04).
 * This function reads content of overtemperature warning and SPI failure
 * settings.
 *
 * @param sysEvnt  system event capture enable register structure for storing
 * result from SBC device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetSystemEvents(sbc_sys_evnt_t* const sysEvnt);

/*!
 * @brief This function writes to Lock control register (0x0A).
 * Sections of the register address area can be write-protected to protect
 * against unintended modifications. This facility only protects locked bits
 * from being modified via the SPI and will not prevent the UJA116xA updating
 * status registers etc. For SPI write disable set lock bit to 1.
 * This is mask for set lock control register.
 *
 * @param lockMask  writes Lock control mask for lock or unlock appropriate
 * SBC device bytes.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetLock(const sbc_lock_t lockMask);

/*!
 * @brief This function reads Lock control register (0x0A).
 * Sections of the register address area can be write-protected to protect
 * against unintended modifications. This facility only protects locked bits
 * from being modified via the SPI and will not prevent the UJA116xA updating
 * status registers etc. For SPI write disable set lock bit to 1.
 * This is mask for set lock control register.
 *
 * @param lockMask  reads CAN peripheral settings from SBC device.
 * SBC device bytes.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetLock(sbc_lock_t* const lockMask);

/*!
 * @brief This function configures CAN peripheral behavior.
 * This function configures CAN peripheral behavior.
 * This function configures several registers which configure CAN.
 * It contains CAN control register, Transceiver event capture enable register,
 * CAN data rate selection, ID registers, ID mask registers,
 * Frame control register, Data mask 0 - 7 configuration.
 *
 * @param can  writes CAN peripheral settings to SBC device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetCanConfig(const sbc_can_conf_t* const can);

/*!
 * @brief This function reads CAN peripheral settings.
 * This function configures CAN peripheral behavior.
 * This function configures several registers which configure CAN.
 * It contains CAN control register, Transceiver event capture enable register,
 * CAN data rate selection, ID registers, ID mask registers,
 * Frame control register, Data mask 0 - 7 configuration.
 *
 * @param can  reads CAN peripheral settings from SBC device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetCanConfig(sbc_can_conf_t* const can);

/*!
 * @brief This function writes to WAKE pin event capture enable register (0x4C).
 * Local wake-up is enabled via bits WPRE and WPFE in the WAKE pin event capture
 * enable register. A wake-up event is triggered
 * by a LOW-to-HIGH (ifWPRE = 1) and/or a HIGH-to-LOW (if WPFE = 1) transition
 * on the WAKE pin. This arrangement allows for maximum flexibility when
 * designing a local wake-up circuit. In applications that do not use the local
 * wake-up facility, local wake-up should be disabled and the WAKE pin
 * connected to GND.
 *
 * @param wakePin  writes configuration to WAKE pin event capture enable
 * register of SBC device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_SetWakePin(const sbc_wake_t* const wakePin);

/*!
 * @brief This function reads WAKE pin event capture enable register (0x4C).
 * Local wake-up is enabled via bits WPRE and WPFE in the WAKE pin event capture
 * enable register. A wake-up event is triggered
 * by a LOW-to-HIGH (ifWPRE = 1) and/or a HIGH-to-LOW (if WPFE = 1) transition
 * on the WAKE pin. This arrangement allows for maximum flexibility when
 * designing a local wake-up circuit. In applications that do not use the local
 * wake-up facility, local wake-up should be disabled and the WAKE pin
 * connected to GND.
 *
 * @param wakePin  reads configuration to WAKE pin event capture enable
 * register of SBC device.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetWakePin(sbc_wake_t* const wakePin);

/* Functions for reading statuses. */

/*!
 * @brief This function reads Main status register.
 * This function will clear R/W registers automatically after reading.
 *
 * @param mainStatus  variable for storing Status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetMainStatus(sbc_main_status_t* const mainStatus);

/*!
 * @brief This function reads Watchdog status register.
 * This function will clear R/W registers automatically after reading.
 *
 * @param watchdogStatus  variable for storing Status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetWatchdogStatus(sbc_wtdog_status_t* const watchdogStatus);

/*!
 * @brief This functions reads Supply voltage status register.
 * This function clear R/W status after reading writing 0 to register.
 * It contains V2/VEXT status and V1 status.
 *
 * @param supStatus  variable for storing Supply voltage status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetSupplyStatus(sbc_supply_status_t* const supStatus);

/*!
 * @brief This functions reads Transceiver status register.
 * It contains CAN transceiver status, CAN partial networking error,
 * CAN partial networking status, CAN oscillator status, CAN-bus silence status,
 * VCAN status, CAN failure status.
 *
 * @param transStatus  variable for storing Transceiver status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetCanStatus(sbc_trans_stat_t* const transStatus);

/*!
 * @brief This functions reads WAKE pin status register.
 * This function reads switching threshold of voltage on WAKE pin.
 *
 * @param wakeStatus  variable for storing WAKE pin status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetWakeStatus(sbc_wake_stat_wpvs_t* const wakeStatus);

/*!
 * @brief This functions reads Event capture registers.
 * This function reads switching threshold of voltage on WAKE pin.
 * This functions reads global events statuses: Global event status,
 * System event status, Supply event status, Transceiver event status,
 * WAKE pin event status.
 * @param events  variable for storing Event capture registers.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetEventsStatus(sbc_evn_capt_t* const events);

/*!
 * @brief This function clears Event capture registers.
 * It contains Global event status, System event status, Supply event status,
 * Transceiver event status, WAKE pin event status.
 * This function write 1 to bit which should be delete.
 * After an event source has been identified, the status flag should be cleared
 * (set to 0) by writing 1 to the relevant status bit
 * (writing 0 will have no effect). Write true value to appropriate enumeration.
 *
 * @param events  variable for clear Event capture registers, set status to 1
 * for clear appropriate status.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_CleanEvents(const sbc_evn_capt_t* const events);

/*!
 * @brief This function reads all statuses from SBC device.
 * It reads all status registers: Main status and Watchdog status,
 * Supply voltage status,  Transceiver status, WAKE pin status,
 * Event capture registers.
 *
 * @param status  variable for storing all status registers.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetAllStatus(sbc_status_group_t* const status);

/*!
 * @brief This function reads MTPNV status register.
 * The MTPNV cells can be reprogrammed a maximum of 200 times (Ncy(W)MTP).
 * Bit NVMPS in the MTPNV status register indicates
 * whether the non-volatile cells can be reprogrammed. This register also
 * contains a write counter, WRCNTS, that is incremented each time the
 * MTPNV cells are reprogrammed (up to a maximum value of 111111; there is
 * no overflow; performing a factory reset also increments the counter).
 * This counter is provided for information purposes only; reprogramming will
 * not be rejected when it reaches its maximum value.
 *
 * @param mtpnv  variable for storing MTPNV status registers.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetMtpnvStatus(sbc_mtpnv_stat_t* const mtpnv);

/* Factories settings. This function read and write from non-volatile memory */

/*!
 * @brief This function reads Start-up control register and
 * SBC configuration control register.
 * It is non-volatile memory with limited write access.
 * The UJA116xA contains Multiple Time Programmable Non-Volatile
 * (MTPNV) memory cells that allow some of the default device settings
 * to be reconfigured. The MTPNV memory address range is from 0x73 to 0x74.
 * NXP delivers the UJA116xA in so-called Forced Normal mode,
 * also referred to as the factory preset configuration. In order
 * to change the default settings, the device must be in Forced Normal mode
 * with FNMC = 1 and NVMPS = 1. In Forced Normal mode, the watchdog is disabled,
 * all regulators are on and the CAN transceiver is in Active mode.
 *
 * @param factoriesConf  variable for storing Start-up control register and
 * SBC configuration control register.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_GetFactoriesSettings(sbc_factories_conf_t* const factoriesConf);

/*!
 * @brief This function sets Start-up control register and
 * SBC configuration control register.
 * It is non-volatile memory with limited write access.
 * The UJA116xA contains Multiple Time Programmable Non-Volatile
 * (MTPNV) memory cells that allow some of the default device settings
 * to be reconfigured. The MTPNV memory address range is from 0x73 to 0x74.
 * NXP delivers the UJA116xA in so-called Forced Normal mode,
 * also referred to as the factory preset configuration. In order
 * to change the default settings, the device must be in Forced Normal mode
 * with FNMC = 1 and NVMPS = 1. In Forced Normal mode, the watchdog is disabled,
 * all regulators are on and the CAN transceiver is in Active mode.
 * Note for default settings see sbc_factories_conf_t comment.
 * If the device has been programmed previously, the factory presets may need
 * to be restored before reprogramming can begin. When the factory presets
 * have been restored successfully, a system reset is generated automatically
 * and UJA116xA switches back to Forced Normal mode.
 * Factory preset values are restored if the following conditions apply
 * continuously for at least td(MTPNV) during battery power-up:
 * -pin RSTN is held LOW
 * -CANH is pulled up to VBAT
 * -CANL is pulled down to GND
 *
 * @param newConf  value of this variable will be write to Start-up control
 * register and SBC configuration control register.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_ChangeFactoriesSettings(const sbc_factories_conf_t* const newConf);

/*!
 * @brief This function sends data over LSPI to SBC device.
 * This function sends 8 bites to SBC device register according device address
 * which is selected. This transfer uses 16bit LSPI. CS polarity - active low,
 * clock phase on second edge. Clock polarity active high.
 *
 * @param regName  this is register name for access.
 * @param sendData pointer of 8 bits variable which contains data for
 * writing to register of SBC device. Use NULL pointer for data reading.
 * @param receiveData pointer of 8 bits variable which contains data for
 * reading from SBC device. Use NULL pointer for data writing.
 * @return An error code or STATUS_SUCCESS
 */
status_t SBC_DataTransfer(const sbc_register_t regName,
        const uint8_t* const sendData, uint8_t* const receiveData);

/*!
 * @brief This function refreshes watchdog period by writing byte to
 * the SBC watchdog register. This function must be called periodically
 * according Watchdog mode control and Nominal watchdog period settings.
 * Note: Unxpected behaviour can happend if watchdog mode is set to timeout
 * period and watchdog is triggered exactly at 50% of period.
 * Be sure you trigger watchdog before 50% or above 50% of watchdog period.
 */
void SBC_FeedWatchdog(void);

#endif /* SOURCES_SBC_SBC_UJA_116X_H_ */
