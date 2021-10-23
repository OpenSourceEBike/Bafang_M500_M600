/*
 * Copyright 2017 NXP
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

/**
 * @sbc_uja113x.h
 * 
 * @page misra_violations MISRA-C:2012 violations
 * 
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Macro not referenced
 * Macros which are not currently referenced in code but can be used in the
 * future or by user.
 */
#ifndef SBC_UJA113X_GUARD_
#define SBC_UJA113X_GUARD_

#include <stdbool.h>
#include <stdint.h>
#include "status.h"

#define BANK_HVIO_CNT       4U /**< HVIO channels per bank */
#define HVIO_MAX_CHANNELS   8U /**< Maximum number of HVIO channels available on UJA113x devices */
#define HVIO_MAX_BANKS      2U /**< Maximum number of HVIO banks available on UJA113x devices */

/**
 * @brief Driver types
 * 
 * @ingroup internals
 *
 * Implements : uja113x_chip_t_Class
 */
typedef enum
{
    UJA1131HW5V0    = 0x1100U,
    UJA1131HW3V3    = 0x1000U,
    UJA1132HW5V0    = 0x0100U,
    UJA1132HW3V3    = 0x0000U,
    UJA1135HW5V0    = 0x1101U,
    UJA1135HW3V3    = 0x1001U,
    UJA1136HW5V0    = 0x0101U,
    UJA1136HW3V3    = 0x0001U,
    UJA1131HWFD5V4  = 0x5100U,
    UJA1131HWFD3V4  = 0x5000U,
    UJA1131HWFD5V0  = 0x7100U,
    UJA1131HWFD3V0  = 0x7000U,
    UJA1132HWFD5V4  = 0x4100U,
    UJA1132HWFD3V4  = 0x4000U,
    UJA1132HWFD5V0  = 0x6100U,
    UJA1132HWFD3V0  = 0x6000U,
} uja113x_chip_t;

/**
 * @ingroup internals
 * 
 * @brief
 * @details The SPI protocol requires the first byte transmitted on MOSI line 
 * to be set to register address the user wants to access (write/read). The 
 * byte consists of ADDRESS[7:1] and READ_BIT[0]. This section lists all the 
 * register addresses available on UJA113x devices which can be used throughout 
 * the source code instead of fixed values. The defined addresses are already 
 * shifted.\n
 * The address is useful in UJA113X_SbcTransfer() function, which allows
 * generic read/write access.
 * 
 * @defgroup regmap Register address map
 * @{
 */
/**
 * @name Primary control registers (0x00 - 0x0F)
 * @{
 */
#define WTDC_ADDRESS    ((uint8_t) 0x00U << 1U) /**< @brief Watchdog control register address */
#define MC_ADDRESS      ((uint8_t) 0x01U << 1U) /**< @brief Mode control register address */
#define FSC_ADDRESS     ((uint8_t) 0x02U << 1U) /**< @brief Fail-safe control register address */
#define MSBCS_ADDRESS   ((uint8_t) 0x03U << 1U) /**< @brief Main SBC status register address */
#define SYSIE_ADDRESS   ((uint8_t) 0x04U << 1U) /**< @brief System interrupt enable register address */
#define WTDS_ADDRESS    ((uint8_t) 0x05U << 1U) /**< @brief Watchdog status register address */
#define MEM0_ADDRESS    ((uint8_t) 0x06U << 1U) /**< @brief General purpose memory 0 address */
#define MEM1_ADDRESS    ((uint8_t) 0x07U << 1U) /**< @brief General purpose memory 1 address */
#define MEM2_ADDRESS    ((uint8_t) 0x08U << 1U) /**< @brief General purpose memory 2 address */
#define MEM3_ADDRESS    ((uint8_t) 0x09U << 1U) /**< @brief General purpose memory 3 address */
#define LCKC_ADDRESS    ((uint8_t) 0x0AU << 1U) /**< @brief Lock control register address */
/** @}*/ 

/**
 * @name Supply control registers (0x10 - 0x1F)
 * @{
 */
#define RC_ADDRESS      ((uint8_t) 0x10U << 1U) /**< @brief Regulator control register address */
#define BMETSC_ADDRESS  ((uint8_t) 0x11U << 1U) /**< @brief Battery monitor event trigger source control register address */
#define BMUTC_ADDRESS   ((uint8_t) 0x12U << 1U) /**< @brief Battery monitor undervoltage threshold control register address */
#define BMOTC_ADDRESS   ((uint8_t) 0x13U << 1U) /**< @brief Battery monitor overvoltage threshold control register address */
#define BMHC_ADDRESS    ((uint8_t) 0x14U << 1U) /**< @brief Battery monitor hysteresis control register address */
#define VBADC1_ADDRESS  ((uint8_t) 0x15U << 1U) /**< @brief ADC conversion results for Vbat register 1 address */
#define VBADC2_ADDRESS  ((uint8_t) 0x16U << 1U) /**< @brief ADC conversion results for Vbat register 2 address */
#define BSADC1_ADDRESS  ((uint8_t) 0x17U << 1U) /**< @brief ADC conversion results for Vbatsense register 1 address */
#define BSADC2_ADDRESS  ((uint8_t) 0x18U << 1U) /**< @brief ADC conversion results for Vbatsense register 2 address */
#define SMPSC_ADDRESS   ((uint8_t) 0x19U << 1U) /**< @brief SMPS control register address */
#define SMPSOVC_ADDRESS ((uint8_t) 0x1AU << 1U) /**< @brief SMPS out voltage control register address */
#define SUPVS_ADDRESS    ((uint8_t) 0x1BU << 1U) /**< @brief Supply voltage status register address */
#define SUPIE_ADDRESS   ((uint8_t) 0x1CU << 1U) /**< @brief Supply interrupt enable register address */
/** @}*/

/**
 * @name Transceiver control registers (0x20 - 0x2F)
 * @{
 */
#define CANC_ADDRESS    ((uint8_t) 0x20U << 1U) /**< @brief CAN control register address */
#define LINC_ADDRESS    ((uint8_t) 0x21U << 1U) /**< @brief LIN control register address */
#define TS_ADDRESS      ((uint8_t) 0x22U << 1U) /**< @brief Transceiver status register address */
#define TIE_ADDRESS     ((uint8_t) 0x23U << 1U) /**< @brief Transceiver interrupt enable register address */
#define DR_ADDRESS      ((uint8_t) 0x26U << 1U) /**< @brief Data rate register address */
#define ID0_ADDRESS     ((uint8_t) 0x27U << 1U) /**< @brief ID register 0 address */
#define ID1_ADDRESS     ((uint8_t) 0x28U << 1U) /**< @brief ID register 1 address */
#define ID2_ADDRESS     ((uint8_t) 0x29U << 1U) /**< @brief ID register 2 address */
#define ID3_ADDRESS     ((uint8_t) 0x2AU << 1U) /**< @brief ID register 3 address */
#define IDM0_ADDRESS    ((uint8_t) 0x2BU << 1U) /**< @brief ID mask register 0 address */
#define IDM1_ADDRESS    ((uint8_t) 0x2CU << 1U) /**< @brief ID mask register 1 address */
#define IDM2_ADDRESS    ((uint8_t) 0x2DU << 1U) /**< @brief ID mask register 2 address */
#define IDM3_ADDRESS    ((uint8_t) 0x2EU << 1U) /**< @brief ID mask register 3 address */
#define FC_ADDRESS      ((uint8_t) 0x2FU << 1U) /**< @brief Frame control register address */
/** @}*/

/**
 * @name HVIO control registers (0x30 - 0x4F)
 * @{
 */
#define HVIO1C_ADDRESS  ((uint8_t) 0x30U << 1U) /**< @brief HVIO 1 control register address */
#define HVIO2C_ADDRESS  ((uint8_t) 0x31U << 1U) /**< @brief HVIO 2 control register address */
#define HVIO3C_ADDRESS  ((uint8_t) 0x32U << 1U) /**< @brief HVIO 3 control register address */
#define HVIO4C_ADDRESS  ((uint8_t) 0x33U << 1U) /**< @brief HVIO 4 control register address */
#define B0WUPTHRC_ADDRESS ((uint8_t) 0x34U << 1U) /**< @brief Bank 0 (HVIO 1 - 4) wake-up threshold control register address */
#define B0WUPS_ADDRESS  ((uint8_t) 0x35U << 1U) /**< @brief Bank 0 wake-up status register address */
#define B0DRVS_ADDRESS  ((uint8_t) 0x36U << 1U) /**< @brief Bank 0 driver status register address */
#define B0WUPIE_ADDRESS ((uint8_t) 0x37U << 1U) /**< @brief Bank 0 wake-up interrupt enable register address */
#define B0FIE_ADDRESS   ((uint8_t) 0x38U << 1U) /**< @brief Bank 0 fail interrupt enable register address */
#define B0SCDTC_ADDRESS ((uint8_t) 0x39U << 1U) /**< @brief Bank 0 short-circuit detection threshold control register address */
#define B0OLDTC_ADDRESS ((uint8_t) 0x3AU << 1U) /**< @brief Bank 0 open-laod detection threshold control register address */
#define HVIO5C_ADDRESS  ((uint8_t) 0x40U << 1U) /**< @brief HVIO 5 control register address */
#define HVIO6C_ADDRESS  ((uint8_t) 0x41U << 1U) /**< @brief HVIO 6 control register address */
#define HVIO7C_ADDRESS  ((uint8_t) 0x42U << 1U) /**< @brief HVIO 7 control register address */
#define HVIO8C_ADDRESS  ((uint8_t) 0x43U << 1U) /**< @brief HVIO 8 control register address */
#define B1WUPTHRC_ADDRESS ((uint8_t) 0x44U << 1U) /**< @brief Bank 1 (HVIO 5 - 8) wake-up threshold control register address */
#define B1WUPS_ADDRESS  ((uint8_t) 0x45U << 1U) /**< @brief Bank 1 wake-up status register address */
#define B1DRVS_ADDRESS  ((uint8_t) 0x46U << 1U) /**< @brief Bank 1 driver status register address */
#define B1WUPIE_ADDRESS ((uint8_t) 0x47U << 1U) /**< @brief Bank 1 wake-up interrupt enable register address */
#define B1FIE_ADDRESS   ((uint8_t) 0x48U << 1U) /**< @brief Bank 1 fail interrupt enable register address */
#define B1SCDTC_ADDRESS ((uint8_t) 0x49U << 1U) /**< @brief Bank 1 short-circuit detection threshold control register address */
#define B1OLDTC_ADDRESS ((uint8_t) 0x4AU << 1U) /**< @brief Bank 1 open-laod detection threshold control register address */
/** @}*/

/**
 * @name Timer control registers (0x50 - 0x5F)
 * @{
 */
#define TIM1C_ADDRESS   ((uint8_t) 0x50U << 1U) /**< @brief Timer 1 control register address */
#define TIM1DCC_ADDRESS ((uint8_t) 0x51U << 1U) /**< @brief Timer 1 duty cycle control register address */
#define TIM2C_ADDRESS   ((uint8_t) 0x52U << 1U) /**< @brief Timer 2 control register address */
#define TIM2DCC_ADDRESS ((uint8_t) 0x53U << 1U) /**< @brief Timer 2 duty cycle control register address */
#define TIM3C_ADDRESS   ((uint8_t) 0x54U << 1U) /**< @brief Timer 3 control register address */
#define TIM3DCC_ADDRESS ((uint8_t) 0x55U << 1U) /**< @brief Timer 3 duty cycle control register address */
#define TIM4C_ADDRESS   ((uint8_t) 0x56U << 1U) /**< @brief Timer 4 control register address */
#define TIM4DCC_ADDRESS ((uint8_t) 0x57U << 1U) /**< @brief Timer 4 duty cycle control register address */
/** @}*/

/**
 * @name Interrupt status registers (0x60 - 0x6F)
 * @{
 */
#define GIS_ADDRESS     ((uint8_t) 0x60U << 1U) /**< @brief Global interrupt status register address */
#define SYSIS_ADDRESS   ((uint8_t) 0x61U << 1U) /**< @brief System interrupt status register address */
#define SUPIS_ADDRESS   ((uint8_t) 0x62U << 1U) /**< @brief Supply interrupt status register address */
#define TIS_ADDRESS     ((uint8_t) 0x63U << 1U) /**< @brief Transceiver interrupt status register address */
#define B0WUPIS_ADDRESS ((uint8_t) 0x64U << 1U) /**< @brief Bank 0 wake-up interrupt status register address */
#define B0FIS_ADDRESS   ((uint8_t) 0x65U << 1U) /**< @brief Bank 0 fail interrupt status register address */
#define B1WUPIS_ADDRESS ((uint8_t) 0x66U << 1U) /**< @brief Bank 1 wake-up interrupt status register address */
#define B1FIS_ADDRESS   ((uint8_t) 0x67U << 1U) /**< @brief Bank 1 fail interrupt status register address */
#define DM0_ADDRESS     ((uint8_t) 0x68U << 1U) /**< @brief Data mask register 0 address */
#define DM1_ADDRESS     ((uint8_t) 0x69U << 1U) /**< @brief Data mask register 1 address */
#define DM2_ADDRESS     ((uint8_t) 0x6AU << 1U) /**< @brief Data mask register 2 address */
#define DM3_ADDRESS     ((uint8_t) 0x6BU << 1U) /**< @brief Data mask register 3 address */
#define DM4_ADDRESS     ((uint8_t) 0x6CU << 1U) /**< @brief Data mask register 4 address */
#define DM5_ADDRESS     ((uint8_t) 0x6DU << 1U) /**< @brief Data mask register 5 address */
#define DM6_ADDRESS     ((uint8_t) 0x6EU << 1U) /**< @brief Data mask register 6 address */
#define DM7_ADDRESS     ((uint8_t) 0x6FU << 1U) /**< @brief Data mask register 7 address */
/** @}*/

/**
 * @name MTPNV and ID registers (0x70 - 0x7F)
 * @{
 */
#define MTPNVS_ADDRESS     ((uint8_t) 0x70U << 1U) /**< @brief MTPNV status register address */
#define HVIOHSDRVC_ADDRESS ((uint8_t) 0x71U << 1U) /**< @brief HVIO high-side driver control register address */
#define HVIOLSDRVC_ADDRESS ((uint8_t) 0x72U << 1U) /**< @brief HVIO low-side driver control register address */
#define SUPC_ADDRESS       ((uint8_t) 0x73U << 1U) /**< @brief Start-up control register address */
#define SBCCC_ADDRESS      ((uint8_t) 0x74U << 1U) /**< @brief SBC configuration control register address */
#define MTPNVCRCC_ADDRESS  ((uint8_t) 0x75U << 1U) /**< @brief MTPVN CRC control register address */
#define DID1_ADDRESS       ((uint8_t) 0x7EU << 1U) /**< @brief Device identification register 1 address */
#define DID2_ADDRESS       ((uint8_t) 0x7FU << 1U) /**< @brief Device identification register 2 address */
/** @}*/
/** @}*/

/*--------------------------------------------------------------------------*/
/* SBC Primary Control registers definitions (0x00 - 0x0F)                  */
/*--------------------------------------------------------------------------*/
/**
 * @brief Watchdog mode configuration options (0x00:WMC[7:5])
 * 
 * @ingroup wtdcconfig
 *
 * Implements : uja113x_wtdc_wmc_t_Class
 */
typedef enum
{
    WMC_AUTONOMOUS = 0x20U, /**< Watchdog autonomous mode */
    WMC_TIMEOUT    = 0x40U, /**< Watchdog timeout mode. */
    WMC_WINDOW     = 0x80U, /**< Watchdog window mode. */
} uja113x_wtdc_wmc_t;

/**
 * @brief Watchdog nominal period configuration options (0x00:NWP[3:0])
 * 
 * @ingroup wtdcconfig
 *
 * Implements : uja113x_wtdc_nwp_t_Class
 */
typedef enum
{
    NWP_PERIOD_8    = 0x08U, /**< 8 ms nominal period. */
    NWP_PERIOD_16   = 0x01U, /**< 16 ms nominal watchdog period. */
    NWP_PERIOD_32   = 0x02U, /**< 32 ms nominal watchdog period. */
    NWP_PERIOD_64   = 0x0BU, /**< 64 ms nominal watchdog period. */
    NWP_PERIOD_128  = 0x04U, /**< 128 ms nominal watchdog period. */
    NWP_PERIOD_256  = 0x0DU, /**< 256 ms nominal watchdog period. */
    NWP_PERIOD_1024 = 0x0EU, /**< 1024 ms nominal watchdog period. */
    NWP_PERIOD_4096 = 0x07U, /**< 4096 ms nominal watchdog period. */
} uja113x_wtdc_nwp_t;

/**
 * @brief SBC mode configuration options (0x01:MC[2:0])
 * 
 * @ingroup modeconfig
 *
 * Implements : uja113x_mc_mc_t_Class
 */
typedef enum
{
    MC_SLEEP   = 0x01U, /**< Set SBC mode to Sleep. */
    MC_STANDBY = 0x04U, /**< Set SBC mode to Standby. */
    MC_NORMAL  = 0x07U, /**< Set SBC mode to Normal. */
} uja113x_mc_mc_t;

/**
 * @brief EN pin shut-down control configuration options (0x02:ENSC[7:6])
 * 
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_ensc_t_Class
 */
typedef enum
{
    ENSC_NONE         = 0x00U, /**< EN pin not influenced by battery over- or undervoltage. */
    ENSC_UNDRVOLTAGE  = 0x40U, /**< EN pin driven LOW when battery undervoltage detected. */
    ENSC_OVERVOLTAGE  = 0x80U, /**< EN pin driven LOW when battery overvoltage detected. */
    ENSC_FULL         = 0xC0U, /**< EN pin driven LOW when battery over- or undervoltage detected. */
} uja113x_fsc_ensc_t;

/**
 * @brief EN high-side driver activation options (0x02:ENDC[5])
 * @details false - EN high-side driver enabled; push-pull output\n
 * true - EN high-side driver disabled; open-drain low-side driver
 * 
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_endc_t_Class
 */
typedef bool uja113x_fsc_endc_t;

/**
 * @brief EN pin output control configuration options (0x02:ENC[4:3])
 * 
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_enc_t_Class
 */
typedef enum
{
    ENC_LOW      = 0x00U, /**< EN pin is driven permanently low. */
    ENC_HIGH_NRS = 0x08U, /**< EN pin is high (or floating if ENDC = 1) when 
                               SBC is in Normal, Reset or Standby mode. */
    ENC_HIGH_N   = 0x10U, /**< EN pin is high (or floating if ENDC = 1) when 
                               SBC is in Normal mode. */
    ENC_TIMER    = 0x18U, /**< EN pin is controller by Timer 2. */
} uja113x_fsc_enc_t;

/**
 * @brief LIMP output configuration options (0x02:LHC[2])
 * @details false - LIMP pin is floating\n
 * true - LIMP pin is driven LOW
 *
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_lhc_t_Class
 */
typedef bool uja113x_fsc_lhc_t;

/**
 * @brief SBC reset counter configuration option (0x02:RCC[1:0])
 * 
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_rcc_t_Class
 */
typedef uint8_t uja113x_fsc_rcc_t; /**< 0 - 3 */

/**
 * @defgroup sysie_group System interrupt enable configuration options (0x04)
 * @details This section lists all the possible options of 0x04 register related
 * to:
 * <ul>
 * <li>Overtemperature interrupt</li>
 * <li>SPI failure interrupt</li>
 * </ul>
 * @see UJA113X_SetSystemInterrupts()
 * @ingroup isr
 * @{
 */
/**
 * @name Overtemperature warning interrupt configuration options (0x04:OTWIE[2])
 * @{
 */
#define OTWIE_DISABLE 0x00U /**< @brief Disable overtemperature warning interrupt */
#define OTWIE_ENABLE  0x04U /**< @brief Enable overtemperature warning interrupt */
/** @}*/
/**
 * @name SPI failure interrupt configuration options (0x04:SPIFIE[1])
 * @{
 */
#define SPIFIE_DISABLE 0x00U /**< @brief Disable SPI failure interrupt */
#define SPIFIE_ENABLE  0x02U /**< @brief Enable SPI failure interrupt */
/** @}*/
/** @}*/ /* sysie_group end */

/**
 * @brief Lock Control configuration options (0x0A)
 * @details false - do not lock memory; SPI write access enabled\n
 * true - lock memory; SPI write access disabled
 * 
 * @ingroup lckcconfig
 * 
 * Implements : uja113x_lckc_lkxc_t_Class
 */
typedef bool uja113x_lckc_lkxc_t;

/**
 * @brief V2 shut-down response to over- or undervoltage configuration options (0x10:V2SC[5:4])
 * 
 * @ingroup rcconfig
 * 
 * Implements : uja113x_rc_v2sc_t_Class
 */
typedef enum
{
    V2SC_NONE         = 0x00U, /**< No shut-down in response to over- or undervoltage. */
    V2SC_UNDRVOLTAGE  = 0x10U, /**< Shut-down in response to undervoltage. */
    V2SC_OVERVOLTAGE  = 0x20U, /**< Shut-down in response to overvoltage. */
    V2SC_FULL         = 0x30U, /**< Shut-down in response to over- or undervoltage. */
} uja113x_rc_v2sc_t;

/**
 * @brief V2 control configuration options (0x10:V2C[3:2])
 * 
 * @ingroup rcconfig
 * 
 * Implements : uja113x_rc_v2c_t_Class
 */
typedef enum
{
    V2C_OFF       = 0x00U, /**< V2 off in all modes. */
    V2C_ON_N      = 0x04U, /**< V2 on in Normal mode. */
    V2C_ON_NSR    = 0x08U, /**< V2 on in Normal, Standby and Reset modes. */
    V2C_ON_NSSRF  = 0x0CU, /**< V2 on in Normal, Standby, Sleep, Reset and FSP mode. */
} uja113x_rc_v2c_t;

/**
 * @brief V1 undervoltage reset threshold configuration options (0x10:V1RTC[1:0])
 * 
 * @ingroup rcconfig
 * 
 * Implements : uja113x_rc_v1rtc_t_Class
 */
typedef enum
{
    V1RTC_90_NOMINAL_OUT = 0x00U, /**< Reset threshold set to 90% of V1 nominal output voltage. */
    V1RTC_80_NOMINAL_OUT = 0x01U, /**< Reset threshold set to 80% of V1 nominal output voltage. */ 
    V1RTC_70_NOMINAL_OUT = 0x02U, /**< Reset threshold set to 70% of V1 nominal output voltage. */
    V1RTC_60_NOMINAL_OUT = 0x03U, /**< Reset threshold set to 60% of V1 nominal output voltage. */
} uja113x_rc_v1rtc_t;

/**
 * @brief Battery monitor event trigger source control (0x11:BMSC[0])
 * @details false - BAT pin is source of event triggers\n
 * true - BATSENSE pin is source of event triggers
 * 
 * @ingroup bmconfig
 * 
 * Implements : uja113x_bmetsc_bmsc_t_Class
 */
typedef bool uja113x_bmetsc_bmsc_t;

/**
 * @brief Battery monitor undervoltage threshold control parameter (0x12:BMUTC[7:0])
 * 
 * @ingroup bmconfig
 * 
 * Implements : uja113x_bmutc_bmutc_t_Class
 */
typedef uint8_t uja113x_bmutc_bmutc_t; /**< Allowed range: 0 - 255 */

/**
 * @brief Battery monitor overvoltage threshold control parameter (13:BMOTC[7:0])
 * 
 * @ingroup bmconfig
 * 
 * Implements : uja113x_bmotc_bmotc_t_Class
 */
typedef uint8_t uja113x_bmotc_bmotc_t; /**< Allowed range: 0 - 255 */

/**
 * @brief Battery monitor hysteresis for overvoltage control parameter (0x14:BMHOC[7:4])
 * 
 * @ingroup bmconfig
 * 
 * Implements uja113x_bmhc_bmhoc_t_Class
 */
typedef uint8_t uja113x_bmhc_bmhoc_t;

/**
 * @brief Battery monitor hysteresis for undervoltage control parameter (0x14:BMHUC[3:0])
 *
 * @ingroup bmconfig
 * 
 * Implements uja113x_bmhc_bmhuc_t_Class
 */
typedef uint8_t uja113x_bmhc_bmhuc_t;

/**
 * @brief SMPS overtemperature configuration options (0x19:SMPSOTC[3])
 * @details false - Vvsmps output not modified during overtemperature warning (OTWI interrupt)\n
 * true - Vvsmps output reduced to 5V when chip temperature is above the 
 * overtemperature warning threshold, Tth(warn)otp
 * 
 * @ingroup smpsconfig
 * 
 * Implements : uja113x_smpsc_smpsotc_t_Class
 */
typedef bool uja113x_smpsc_smpsotc_t;

/**
 * @brief SMPS on/off configuration options (0x19:SMPSC[1:0])
 * 
 * @ingroup smpsconfig
 * 
 * Implements : uja113x_smpsc_smpsc_t_Class
 */
typedef enum
{
    SMPSC_ON_NSR            = 0x00U, /**< SMPS is on in Normal, Standby and 
                                          Reset modes and shut-down in all other modes */
    SMPSC_ON_NSRS           = 0x01U, /**< SMPS is on in Normal, Standby, Reset
                                          and Sleep modes nad shut-down in all other modes */ 
    SMPSC_PASS_THROUGH_NSS  = 0x03U, /**< Pass-through mode is requested in Normal,
                                          Standby and Sleep modes */
} uja113x_smpsc_smpsc_t;

/**
 * @brief SMPS output voltage configuration options (0x1A:SMPSOC[3:0])
 * 
 * @ingroup smpsconfig
 * 
 * Implements : uja113x_smpsovc_smpsoc_t_Class
 */
typedef enum
{
    SMPSOC_50V = 0x00U, /**< SMPS output voltage 5.0 V */
    SMPSOC_52V = 0x01U, /**< SMPS output voltage 5.2 V */
    SMPSOC_54V = 0x02U, /**< SMPS output voltage 5.4 V */
    SMPSOC_56V = 0x03U, /**< SMPS output voltage 5.6 V */
    SMPSOC_58V = 0x04U, /**< SMPS output voltage 5.8 V */
    SMPSOC_60V = 0x05U, /**< SMPS output voltage 6.0 V */
    SMPSOC_62V = 0x06U, /**< SMPS output voltage 6.2 V */
    SMPSOC_64V = 0x07U, /**< SMPS output voltage 6.4 V */
    SMPSOC_66V = 0x08U, /**< SMPS output voltage 6.6 V */
    SMPSOC_68V = 0x09U, /**< SMPS output voltage 6.8 V */
    SMPSOC_70V = 0x0AU, /**< SMPS output voltage 7.0 V */
    SMPSOC_72V = 0x0BU, /**< SMPS output voltage 7.2 V */
    SMPSOC_74V = 0x0CU, /**< SMPS output voltage 7.4 V */
    SMPSOC_76V = 0x0DU, /**< SMPS output voltage 7.6 V */
    SMPSOC_78V = 0x0EU, /**< SMPS output voltage 7.8 V */
    SMPSOC_80V = 0x0FU, /**< SMPS output voltage 8.0 V */
} uja113x_smpsovc_smpsoc_t;

/**
 * @defgroup supie_group Supply interrupt enable configuration options (0x1C)
 * @details This section lists all the possible options of 0x1C register related
 * to:
 * <ul>
 * <li>SMPS status interrupt</li>
 * <li>Battery monitor overvoltage interrupt</li>
 * <li>Battery monitor undervoltage interrupt</li>
 * <li>VEXT overvoltage interrupt</li>
 * <li>VEXT undervoltage interrupt</li>
 * <li>V1 undervoltage interrupt</li>
 * </ul>
 * @see UJA113X_SetSupplyInterrupts()
 * @ingroup isr
 * @{
 */
/**
 * @name SMPS status interrupt configuration options (0x1C:SMPSSIE[5])
 * @{
 */
#define SMPSSIE_DISABLE 0x00U /**< @brief Disable SMPS status interrupt */
#define SMPSSIE_ENABLE  0x20U /**< @brief Enable SMPS status interrupt */
/** @}*/
/**
 * @name Battery monitor overvoltage interrupt configuration options (0x1C:BMOIE[4])
 * @{
 */
#define BMOIE_DISABLE 0x00U /**< @brief Disable battery monitor overvoltage interrupt */
#define BMOIE_ENABLE  0x10U /**< @brief Enable battery monitor overvoltage interrupt */
/** @}*/
/**
 * @name Battery monitor undervoltage interrupt configuration options (0x1C:BMUIE[3])
 * @{
 */
#define BMUIE_DISABLE 0x00U /**< @brief Disable battery monitor undervoltage interrupt */
#define BMUIE_ENABLE  0x08U /**< @brief Enable battery monitor undervoltage interrupt */
/** @}*/
/**
 * @name VEXT overvoltage interrupt configuration options (0x1C:VEXTOIE[2])
 * @{
 */
#define VEXTOIE_DISABLE 0x00U /**< @brief Disable VEXT overvoltage interrupt */
#define VEXTOIE_ENABLE  0x04U /**< @brief Enable VEXT overvoltage interrupt */
/** @}*/
/**
 * @name VEXT undervoltage interrupt configuration options (0x1C:VEXTUIE[1])
 * @{
 */
#define VEXTUIE_DISABLE 0x00U /**< @brief Disable VEXT undervoltage interrupt */
#define VEXTUIE_ENABLE  0x02U /**< @brief Enable VEXT undervoltage interrupt */
/** @}*/
/**
 * @name V1UIE undervoltage interrupt configuration options (0x1C:V1UIE[0])
 * @{
 */
#define V1UIE_DISABLE 0x00U /**< @brief Disable V1 undervoltage interrupt */
#define V1UIE_ENABLE  0x01U /**< @brief Enable V1 undervoltage interrupt */
/** @}*/
/** @}*/ /* end of supply interrupt config group */


/**
 * @brief CAN FD control (0x20:CFDC[6])
 * @details false - CAN FD tolerance disabled, the CAN FD frames are detected
 * as erroneous while in sleep with partial networking enabled\n
 * true - CAN FD tolerance enabled, the can FD frames are ignored while in
 * sleep with partial networking enabled
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_canc_cfdc_t_Class
 */
typedef bool uja113x_canc_cfdc_t;

/**
 * @brief CAN partial networking control parameter (0x20:CPNC[4])
 * @details false - Disable CAN selective wake-up\n
 * true - Enable CAN selective wake-up
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_canc_cpnc_t_Class
 */
typedef bool uja113x_canc_cpnc_t;

/**
 * @brief CAN shut-down control configuration options (0x20:CSC[3:2])
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_canc_csc_t_Class
 */
typedef enum
{
    CSC_NONE        = 0x00U, /**< CAN transceiver is not shut down when a 
                                  battery monitor under- or overvoltage 
                                  interrupt is generated. */
    CSC_UNDRVOLTAGE = 0x04U, /**< CAN transceiver shuts down in response to
                                  a battery monitor undervoltage (BMUI)
                                  interrupt (SBC in normal mode). */
    CSC_OVERVOLTAGE = 0x08U, /**< CAN transceiver shuts down in reponse to
                                  a battery monitor overvoltage (BMOI)
                                  unterrupt (SBC in normal mode). */
    CSC_UNDRV_OVERV = 0x0CU, /**< CAN transceiver shuts down in response to
                                  BMUI or BMOI interrupt (SBC in normal mode). */
} uja113x_canc_csc_t;

/**
 * @brief CAN transceiver operating mode configuration options (0x20:CMC[1:0])
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_canc_cmc_t_Class
 */
typedef enum
{
    CMC_TRANSCEIVER_OFF     = 0x00U, /**< Offline/Offline bias mode. */
    CMC_TRANSCEIVER_ACTIVE  = 0x01U, /**< Active mode (when the SBC is in
                                          normal mode). */
    CMC_TRANSCEIVER_ACTIVE2 = 0x02U, /**< Active mode (when the SBC is in
                                          normal mode); VCAN undervoltage
                                          disabled. */
    CMC_TRANSCEIVER_LISTEN  = 0x03U, /**< Listen-only mode. */
} uja113x_canc_cmc_t;

/**
 * @brief LIN2 slope control configuration options (0x21:LSC2[7:6])
 * 
 * @ingroup lincconfig
 *
 * Implements : uja113x_linc_lsc2_t_Class
 */
typedef enum
{
    LSC2_SLOPE_ACTIVE              = 0x00U, /**< Slope control active. */
    LSC2_SLOPE_ACTIVE2             = 0x40U, /**< Slope control active. */
    LSC2_SLOPE_ACTIVE_TXDL_TIMEOUT = 0x80U, /**< Slope control active and TXDL
                                              dominant time-out deactivated.*/
} uja113x_linc_lsc2_t;

/**
 * @brief LIN2 transceiver operating mode configuration options (0x21:LMC2[5:4])
 * 
 * @ingroup lincconfig
 *
 * Implements : uja113x_linc_lmc2_t_Class
 */
typedef enum
{
    LMC2_TRANSCEIVER_OFF    = 0x00U, /**< Offline. */
    LMC2_TRANSCEIVER_ON     = 0x10U, /**< Active mode (when SBC in normal mode).*/
    LMC2_TRANSCEIVER_ON2    = 0x20U, /**< Active mode (when SBC in normal mode).*/
    LMC2_TRANSCEIVER_LISTEN = 0x30U, /**< Listen-only mode.*/
} uja113x_linc_lmc2_t;

/**
 * @brief LIN/LIN1 slope control configuration options (0x21:LSC1[3:2])
 * 
 * @ingroup lincconfig
 *
 * Implements : uja113x_linc_lsc1_t_Class
 */
typedef enum
{
    LSC1_SLOPE_ACTIVE              = 0x00U, /**< Slope control active. */
    LSC1_SLOPE_ACTIVE2             = 0x04U, /**< Slope control active. */
    LSC1_SLOPE_ACTIVE_TXDL_TIMEOUT = 0x08U, /**< Slope control active and TXDL
                                              dominant time-out deactivated.*/
} uja113x_linc_lsc1_t;

/**
 * @brief LIN/LIN1 transceiver operating mode configuration options (0x21:LMC1[1:0])
 * 
 * @ingroup lincconfig
 *
 * Implements : uja113x_linc_lmc1_t_Class
 */
typedef enum
{
    LMC1_TRANSCEIVER_OFF    = 0x00U, /**< Offline. */
    LMC1_TRANSCEIVER_ON     = 0x01U, /**< Active mode (when SBC in normal mode).*/
    LMC1_TRANSCEIVER_ON2    = 0x02U, /**< Active mode (when SBC in normal mode).*/
    LMC1_TRANSCEIVER_LISTEN = 0x03U, /**< Listen-only mode.*/
} uja113x_linc_lmc1_t;

/**
 * @defgroup tie_group Transceiver interrupt enable configuration options (0x23)
 * @details This section lists all the possible options of 0x23 register related
 * to:
 * <ul>
 * <li>CAN bus silence interrupt</li>
 * <li>LIN 2 wake-up interrupt</li>
 * <li>LIN 1 wake-up interrupt</li>
 * <li>CAN failure interrupt</li>
 * <li>CAN wake-up interrupt</li>
 * </ul>
 * @see UJA113X_SetTransceiverInterrupts()
 * @ingroup isr
 * @{
 */
/**
 * @name CAN bus silence interrupt configuration options (0x23:CBSIE[4])
 * @{
 */
#define CBSIE_DISABLE 0x00U /**< @brief Disable CAN bus silence interrupt */
#define CBSIE_ENABLE  0x10U /**< @brief Enable CAN bus silence interrupt */
/** @}*/
/**
 * @name LIN 2 wake-up interrupt configuration options (0x23:LWI2E[3])
 * @{
 */
#define L2WIE_DISABLE 0x00U /**< @brief Disable LIN 2 wake-up interrupt */
#define L2WIE_ENABLE  0x08U /**< @brief Enable LIN 2 wake-up interrupt */
/** @}*/
/**
 * @name LIN 1 wake-up interrupt configuration options (0x23:LWI2E[2])
 * @{
 */
#define L1WIE_DISABLE 0x00U /**< @brief Disable LIN 1 wake-up interrupt */
#define L1WIE_ENABLE  0x04U /**< @brief Enable LIN 1 wake-up interrupt */
/** @}*/
/**
 * @name CAN failure interrupt configuration options (0x23:CFIE[1])
 * @{
 */
#define CFIE_DISABLE 0x00U /**< @brief Disable CAN failure interrupt */
#define CFIE_ENABLE  0x02U /**< @brief Enable CAN failure interrupt */
/** @}*/
/**
 * @name CAN wake-up interrupt configuration options (0x23:CWIE[0])
 * @{
 */
#define CWIE_DISABLE 0x00U /**< @brief Disable CAN wake-up interrupt */
#define CWIE_ENABLE  0x01U /**< @brief Enable CAN wake-up interrupt */
/** @}*/
/** @}*/ /* tie_group end */

/**
 * @brief CAN data rate configuration options (0x26:CDR[2:0])
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_dr_cdr_t_Class
 */
typedef enum
{
    CDR_DATA_RATE_50    = 0x00U, /**< @brief 50 kbit/s */
    CDR_DATA_RATE_100   = 0x01U, /**< @brief 100 kbit/s */
    CDR_DATA_RATE_125   = 0x02U, /**< @brief 125 kbit/s */ 
    CDR_DATA_RATE_250   = 0x03U, /**< @brief 250 kbit/s */
    CDR_DATA_RATE_500a  = 0x04U, /**< @brief reserved (intended for future 
                                      use; currently selects 500 kbit/s) */
    CDR_DATA_RATE_500b  = 0x05U, /**< @brief 500 kbit/s */
    CDR_DATA_RATE_500c  = 0x06U, /**< @brief reserved (intended for future 
                                      use; currently selects 500 kbit/s) */
    CDR_DATA_RATE_1000  = 0x07U, /**< @brief 1000 kbit/s */
} uja113x_dr_cdr_t;

/**
 * @brief CAN partial networking ID (0x27 - 0x2A)
 * 
 * @ingroup cancconfig
 * 
 * Implements : uja113x_id_id_t_Class
 */
typedef uint32_t uja113x_id_id_t;

/**
 * @brief CAN partial networking ID mask (0x2B - 0x2E)
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_idm_idm_t_Class
 */
typedef uint32_t uja113x_idm_idm_t;

/**
 * @brief CAN partial networking ID format options (0x2F:IDE[7])
 * @details false - standard frame format (11-bit)\n
 * true - extended frame format (11-bit)
 * 
 * @ingroup cancconfig
 * 
 * Implements : uja113x_fc_ide_t_Class
 */
typedef bool uja113x_fc_ide_t;

/**
 * @brief CAN partial networking data mask options (0x2F:PNDM[6])
 * @details false - data length code and data field are ignored for wake-up\n
 * true - data length code and data field are evaluated at wake-up
 * 
 * @ingroup cancconfig
 * 
 * Implements : uja113x_fc_pndm_t_Class
 */
typedef bool uja113x_fc_pndm_t;

/**
 * @brief Number of data bytes in a CAN wake-up frame options (0x2F:DLC[3:0])
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_fc_dlc_t_Class
 */
typedef enum
{
    DLC_0= 0x00U, /**< @brief 0 bytes expected */
    DLC_1= 0x01U, /**< @brief 1 bytes expected */
    DLC_2= 0x02U, /**< @brief 2 bytes expected */
    DLC_3= 0x03U, /**< @brief 3 bytes expected */
    DLC_4= 0x04U, /**< @brief 4 bytes expected */
    DLC_5= 0x05U, /**< @brief 5 bytes expected */
    DLC_6= 0x06U, /**< @brief 6 bytes expected */
    DLC_7= 0x07U, /**< @brief 7 bytes expected */
    DLC_8= 0x08U, /**< @brief 8 bytes expected */
} uja113x_fc_dlc_t;

/**
 * @brief HVIOn shut-down configuration options (0x3N:IOnSC[7:6], N = 0, 1, 2, 3)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_hvion_ionsc_t_Class
 */
typedef enum
{
    IONSC_NONE        = 0x00U,/**< @brief HVIOn doesn't respond to over- or undervoltage */
    IONSC_UNDRVOLTAGE = 0x40U, /**< @brief HVIOn shuts down when battery undervoltage is detected in normal mode */
    IONSC_OVERVOLTAGE = 0x80U, /**< @brief HVIOn shuts down when battery overvoltage is detected in normal mode */
    IONSC_FULL        = 0xC0U, /**< @brief HVIOn shuts down when battery over- or undervoltage is detected in normal mode */
} uja113x_hvion_ionsc_t;

/**
 * @brief HVIOn activation configuration options (0x3N:IOnAC[5:3]; N = 0, 1, 2, 3)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_hvion_ionac_t_Class
 */
typedef enum
{
    IONAC_DEACTIVATED = 0x00U, /**< @brief HVIOn is deactivated */
    IONAC_ENABLED     = 0x08U, /**< @brief HVIOn is enabled */
    IONAC_TMR_1       = 0x10U, /**< @brief HVIOn is controlled by timer 1 */
    IONAC_TMR_2       = 0x18U, /**< @brief HVIOn is controlled by timer 2 */
    IONAC_TMR_3       = 0x20U, /**< @brief HVIOn is controlled by timer 3 */
    IONAC_TMR_4       = 0x28U, /**< @brief HVIOn is controlled by timer 4 */
    IONAC_HVIO_INV    = 0x30U, /**< @brief HVIOn is controlled by HVIOn + 4 (inverted control;only available for bank 0) */
    IONAC_HVIO        = 0x38U, /**< @brief HVIOn is controlled by HVIOn + 4 (non-inverted control; only available for bank 0) */
} uja113x_hvion_ionac_t;

/**
 * @brief HVIOn output configuration options (0x3N:IOnCC[2:0]; N = 0, 1, 2, 3)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_hvion_ioncc_t_Class
 */
typedef enum
{
    IONCC_OFF          = 0x00U, /**< @brief HVIOn is off */
    IONCC_HS_SLOPE     = 0x01U, /**< @brief Configure HVIOn as high-side driver with slope control */
    IONCC_LS_SLOPE     = 0x02U, /**< @brief Configure HVIOn as low-side driver with slope control */
    IONCC_WUP          = 0x03U, /**< @brief Configure HVIOn as wake-up input */
    IONCC_HS_WUP_SLOPE = 0x04U, /**< @brief Configure HVIOn as high-side driver and wake-up input with slope control */
    IONCC_HS           = 0x05U, /**< @brief Configure HVIOn as high-side driver without slope control */
    IONCC_LS           = 0x06U, /**< @brief Configure HVIOn as low-side driver without slope control */
    IONCC_HS_WUP       = 0x07U, /**< @brief Configure HVIOn as high-side driver and wake-up input without slope control */
} uja113x_hvion_ioncc_t;

/**
 * @brief Bank N wake-up threshold configuration options (0x34 / 0x44)
 * @details false - threshold is ratiometric to Vbaths1\n
 * true - threshold is absolute
 * 
 * @ingroup hviocr
 * 
 * Implements : uja113x_bwtc_bnwtc_t_Class
 */
typedef bool uja113x_bwtc_bnwtc_t;

/**
 * @defgroup bnwupie_group Bank N wake-up interrupt enable configuration options (0x37/0x47)
 * @details This section lists all the possible options of 0x37/47 registers 
 * related to:
 * <ul>
 * <li>HVIO-N falling edge interrupt</li>
 * <li>HVIO-N rising edge interrupt</li>
 * </ul>
 * @note As HVIO bank 0 & 1 has the same memory layout, these macros can be 
 * used for Bank 0 & Bank 1 wake-up interrupt configuration
 * 
 * @see UJA113X_SetBankWupInterrupts()
 * @ingroup isr
 * @{
 */
/**
 * @name HVIO4/8 falling edge interrupt configuration options (0x37:IO4FEIE[7]/0x47:IO8FEIE[7])
 * @{
 */
#define IO48FEIE_DISABLE 0x00U /**< @brief Disable HVIO 4/8 falling edge interrupt */
#define IO48FEIE_ENABLE  0x80U /**< @brief Enable HVIO 4/8 falling edge interrupt */
/** @}*/
/**
 * @name HVIO4/8 rising edge interrupt configuration options (0x37:IO4REIE[6]/0x47:IO8REIE[6])
 * @{
 */
#define IO48REIE_DISABLE 0x00U /**< @brief Disable HVIO 4/8 rising edge interrupt */
#define IO48REIE_ENABLE  0x40U /**< @brief Enable HVIO 4/8 rising edge interrupt */
/** @}*/
/**
 * @name HVIO3/7 falling edge interrupt configuration options (0x37:IO3FEIE[5]/0x47:IO7FEIE[5])
 * @{
 */
#define IO37FEIE_DISABLE 0x00U /**< @brief Disable HVIO 3/7 falling edge interrupt */
#define IO37FEIE_ENABLE  0x20U /**< @brief Enable HVIO 3/7 falling edge interrupt */
/** @}*/
/**
 * @name HVIO3/7 rising edge interrupt configuration options (0x37:IO3REIE[4]/0x47:IO7REIE[4])
 * @{
 */
#define IO37REIE_DISABLE 0x00U /**< @brief Disable HVIO 3/7 rising edge interrupt */
#define IO37REIE_ENABLE  0x10U /**< @brief Enable HVIO 3/7 rising edge interrupt */
/** @}*/
/**
 * @name HVIO2/6 falling edge interrupt configuration options (0x37:IO2FEIE[3]/0x47:IO6FEIE[3])
 * @{
 */
#define IO26FEIE_DISABLE 0x00U /**< @brief Disable HVIO 2/6 falling edge interrupt */
#define IO26FEIE_ENABLE  0x08U /**< @brief Enable HVIO 2/6 falling edge interrupt */
/** @}*/
/**
 * @name HVIO2/6 rising edge interrupt configuration options (0x37:IO2REIE[2]/0x47:IO6[REIE[2])
 * @{
 */
#define IO26REIE_DISABLE 0x00U /**< @brief Disable HVIO 2/6 rising edge interrupt */
#define IO26REIE_ENABLE  0x04U /**< @brief Enable HVIO 2/6 rising edge interrupt */
/** @}*/
/**
 * @name HVIO1/5 falling edge interrupt configuration options (0x37:IO1FEIE[1]/0x47:IO5FEIE[1])
 * @{
 */
#define IO15FEIE_DISABLE 0x00U /**< @brief Disable HVIO 1/5 falling edge interrupt */
#define IO15FEIE_ENABLE  0x02U /**< @brief Enable HVIO 1/5 falling edge interrupt */
/** @}*/
/**
 * @name HVIO1/5 rising edge interrupt configuration options (0x37:IO1REIE[0]/0x47:IO1REIE[0])
 * @{
 */
#define IO15REIE_DISABLE 0x00U /**< @brief Disable HVIO 1/5 rising edge interrupt */
#define IO15REIE_ENABLE  0x01U /**< @brief Enable HVIO 1/5 rising edge interrupt */
/** @}*/
/** @}*/ /* bnwupie_group end */

/**
 * @defgroup bnfie_group Bank N fail interrupt enable configuration options (0x38/0x48)
 * @details This section lists all the possible options of 0x38/48 registers 
 * related to:
 * <ul>
 * <li>HVIO-N short-circuit interrupt</li>
 * <li>HVIO-N open load interrupt</li>
 * </ul>
 * @note As HVIO bank 0 & 1 has the same memory layout, these macros can be 
 * used for Bank 0 & Bank 1 fail interrupt configuration
 * 
 * @see UJA113X_SetBankFailInterrupts()
 * @ingroup isr
 * @{
 */
/**
 * @name HVIO4/8 short circuit interrupt configuration options (0x38:IO4SCIE[7] / 0x48:IO8SCIE[7])
 * @{
 */
#define IO48SCIE_DISABLE 0x00U /**< @brief Disable HVIO 4/8 short circuit interrupt */
#define IO48SCIE_ENABLE  0x80U /**< @brief Enable HVIO 4/8 short circuit interrupt */
/** @}*/
/**
 * @name HVIO4/8 open load interrupt configuration options (0x38:IO4OLIE[6] / 0x48:IO8OLIE[6])
 * @{
 */
#define IO48OLIE_DISABLE 0x00U /**< @brief Disable HVIO 4/8 open load interrupt */
#define IO48OLIE_ENABLE  0x40U /**< @brief Enable HVIO 4/8 open load interrupt */
/** @}*/
/**
 * @name HVIO3/7 short circuit interrupt configuration options (0x38:IO3SCIE[5] / 0x48:IO7SCIE[5])
 * @{
 */
#define IO37SCIE_DISABLE 0x00U /**< @brief Disable HVIO 3/7 short circuit interrupt */
#define IO37SCIE_ENABLE  0x20U /**< @brief Enable HVIO 3/7 short circuit interrupt */
/** @}*/
/**
 * @name HVIO3/7 open load interrupt configuration options (0x38:IO3OLIE[4] / 0x48:IO7OLIE[4])
 * @{
 */
#define IO37OLIE_DISABLE 0x00U /**< @brief Disable HVIO 3/7 open load interrupt */
#define IO37OLIE_ENABLE  0x10U /**< @brief Enable HVIO 3/7 open load interrupt */
/** @}*/
/**
 * @name HVIO2/6 short circuit interrupt configuration options (0x38:IO2SCIE[3] / 0x48:IO6SCIE[3])
 * @{
 */
#define IO26SCIE_DISABLE 0x00U /**< @brief Disable HVIO 2/6 short circuit interrupt */
#define IO26SCIE_ENABLE  0x08U /**< @brief Enable HVIO 2/6 short circuit interrupt */
/** @}*/
/**
 * @name HVIO2/6 open load interrupt configuration options (0x38:IO2OLIE[2] / 0x48:IO6OLIE[2])
 * @{
 */
#define IO26OLIE_DISABLE 0x00U /**< @brief Disable HVIO 2/6 open load interrupt */
#define IO26OLIE_ENABLE  0x04U /**< @brief Enable HVIO 2/6 open load interrupt */
/** @}*/
/**
 * @name HVIO1/5 short circuit interrupt configuration options (0x38:IO1SCIE[1] / 0x48:IO5SCIE[1])
 * @{
 */
#define IO15SCIE_DISABLE 0x00U /**< @brief Disable HVIO 1/5 short circuit interrupt */
#define IO15SCIE_ENABLE  0x02U /**< @brief Enable HVIO 1/5 short circuit interrupt */
/** @}*/
/**
 * @name HVIO1/5 open load interrupt configuration options (0x38:IO1OLIE[0] / 0x48:IO5OLIE[1])
 * @{
 */
#define IO15OLIE_DISABLE 0x00U /**< @brief Disable HVIO 1/5 open load interrupt */
#define IO15OLIE_ENABLE  0x01U /**< @brief Enable HVIO 1/5 open load interrupt */
/** @}*/
/** @}*/ /* bnwupie_group end */

/**
 * @brief Bank N short-circuit detection threshold configuration options (0x39 / 0x49)
 * 
 * @note For details consult the reference manual - "Static characterisctis: Ith(det)sc"
 * @ingroup bmconfig
 *
 * Implements : uja113x_bscdt_ionsctc_t_Class
 */
typedef enum
{
    IONSCTC_30_MA = 0U, /**< @brief HVIO N short-circuit detection threshold set to 30 mA */
    IONSCTC_45_MA = 1U, /**< @brief HVIO N short-circuit detection threshold set to 45 mA */
    IONSCTC_60_MA = 2U, /**< @brief HVIO N short-circuit detection threshold set to 60 mA */
    IONSCTC_90_MA = 3U, /**< @brief HVIO N short-circuit detection threshold set to 90 mA */
} uja113x_bscdt_ionsctc_t;

/**
 * @brief Bank N open-load detection threshold configuration options (0x3A / 0x4A)
 * @note For details, consult the reference manual - "Static characteristics: Ith(det)open"
 * @ingroup bmconfig
 * 
 * Implements : uja113x_boldt_ionoltc_t_Class
 */
typedef enum
{
    IONOLTC_2_MA = 0U,  /**< @brief HVIO N open-load detection threshold set to 2 mA */
    IONOLTC_5_MA = 1U,  /**< @brief HVIO N open-load detection threshold set to 5 mA */
    IONOLTC_10_MA = 2U, /**< @brief HVIO N open-load detection threshold set to 10 mA */
    IONOLTC_20_MA = 3U, /**< @brief HVIO N open-load detection threshold set to 20 mA */
} uja113x_boldt_ionoltc_t;

/**
 * @brief Timer X period configuration options (0x5N:TXPC[5:2]); (N = 0, 2, 4, 6)
 * 
 * @ingroup timcr
 *
 * Implements : uja113x_timxc_txpc_t_Class
 */
typedef enum
{
    TXPC_4MS_PERIOD     = 0x00U, /**< Timer period 4 ms */
    TXPC_8MS_PERIOD     = 0x04U, /**< Timer period 8 ms */
    TXPC_20MS_PERIOD    = 0x08U, /**< Timer period 20 ms */
    TXPC_30MS_PERIOD    = 0x0CU, /**< Timer period 30 ms */
    TXPC_50MS_PERIOD    = 0x10U, /**< Timer period 50 ms */
    TXPC_100MS_PERIOD   = 0x14U, /**< Timer period 100 ms */
    TXPC_200MS_PERIOD   = 0x18U, /**< Timer period 200 ms */
    TXPC_400MS_PERIOD   = 0x1CU, /**< Timer period 400 ms */
    TXPC_800MS_PERIOD   = 0x20U, /**< Timer period 800 ms */
    TXPC_1000MS_PERIOD  = 0x24U, /**< Timer period 1 s */
    TXPC_2000MS_PERIOD  = 0x28U, /**< Timer period 2 s */
    TXPC_4000MS_PERIOD  = 0x2CU, /**< Timer period 4 s */
} uja113x_timxc_txpc_t;

/**
 * @brief Timer X mode control configuration options (0x5N:TXMC[0]); (N = 0, 2, 4, 6)
 * 
 * @warning Last two options (TXMC_TRIG_START and TXMC_TRIG_END) are not valid
 * for Timer 1!
 * @ingroup timcr
 *
 * Implements : uja113x_timxc_txmc_t_Class
 */
typedef enum
{
    TXMC_PWM        = 0x00U, /**< @brief Timer N is in PWM mode; <b>on-time</b> = TXDCC x TXPC / 255 */
    TXMC_TIM        = 0x01U, /**< @brief Timer N is in Timer mode; <b>on-time</b> = TXDCC x tw(base)tmr us */
    TXMC_TRIG_START = 0x02U, /**< @brief Timer N pulse is triggered at the start of Timer 1 pulse (master-slave mode); <b>on-time</b> = TXDCC x tw(base)tmr us */
    TXMC_TRIG_END   = 0x03U, /**< @brief Timer N pulse is triggered at the end of Timer 1 pulse (master-slave mode); <b>on-time</b> = TXDCC x tw(base)tmr us */
} uja113x_timxc_txmc_t;

/**
 * @brief Timer X duty cycle configuration options (0x5N:TDCC[7:0]); (N = 1, 3, 5, 7)
 * 
 * @ingroup timcr
 *
 * Implements : uja113x_timxdcc_txdcc_t_Class
 */
typedef uint8_t uja113x_timxdcc_txdcc_t; /**< 0 - 255;\n<b>duty cycle</b> = TXDDC \ 255 */

/**
 * @defgroup mtpnvhsc_group HVIO high-side configuration options (0x71)
 * @details This section lists all the possible options of 0x71 register related
 * to:
 * <ul>
 * <li>HVIO N high-side driver configurations</li>
 * </ul>
 * @see UJA113X_SetMtpnv()
 * @ingroup mtpnvr
 * @{
 */
/**
 * @name HVIO8 high-side driver configuration options (0x71:IO8HOC[7])
 * @{
 */
#define HHSDC_IO8HOC_ENABLED  0x00U /**< @brief HVIO8 high-side driver is controlled by the setting in the HVIO8 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO8HOC_DISABLED 0x80U /**< @brief HVIO8 high-side driver is off, regardless of the setting in the HVIO8 control register */
/** @}*/
/**
 * @name HVIO7 high-side driver configuration options (0x71:IO7HOC[6])
 * @{
 */
#define HHSDC_IO7HOC_ENABLED  0x00U /**< @brief HVIO7 high-side driver is controlled by the setting in the HVIO7 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO7HOC_DISABLED 0x40U /**< @brief HVIO7 high-side driver is off, regardless of the setting in the HVIO7 control register */
/** @}*/
/**
 * @name HVIO6 high-side driver configuration options (0x71:IO6HOC[5])
 * @{
 */
#define HHSDC_IO6HOC_ENABLED  0x00U /**< @brief HVIO6 high-side driver is controlled by the setting in the HVIO6 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO6HOC_DISABLED 0x20U /**< @brief HVIO6 high-side driver is off, regardless of the setting in the HVIO6 control register */
/** @}*/
/**
 * @name HVIO5 high-side driver configuration options (0x71:IO5HOC[4])
 * @{
 */
#define HHSDC_IO5HOC_ENABLED  0x00U /**< @brief HVIO5 high-side driver is controlled by the setting in the HVIO5 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO5HOC_DISABLED 0x10U /**< @brief HVIO5 high-side driver is off, regardless of the setting in the HVIO5 control register */
/** @}*/
/**
 * @name HVIO4 high-side driver configuration options (0x71:IO4HOC[3])
 * @{
 */
#define HHSDC_IO4HOC_ENABLED  0x00U /**< @brief HVIO4 high-side driver is controlled by the setting in the HVIO4 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO4HOC_DISABLED 0x08U /**< @brief HVIO4 high-side driver is off, regardless of the setting in the HVIO4 control register */
/** @}*/
/**
 * @name HVIO3 high-side driver configuration options (0x71:IO3HOC[2])
 * @{
 */
#define HHSDC_IO3HOC_ENABLED  0x00U /**< @brief HVIO3 high-side driver is controlled by the setting in the HVIO3 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO3HOC_DISABLED 0x04U /**< @brief HVIO3 high-side driver is off, regardless of the setting in the HVIO3 control register */
/** @}*/
/**
 * @name HVIO2 high-side driver configuration options (0x71:IO2HOC[1])
 * @{
 */
#define HHSDC_IO2HOC_ENABLED  0x00U /**< @brief HVIO2 high-side driver is controlled by the setting in the HVIO2 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO2HOC_DISABLED 0x02U /**< @brief HVIO2 high-side driver is off, regardless of the setting in the HVIO2 control register */
/** @}*/
/**
 * @name HVIO1 high-side driver configuration options (0x71:IO7HOC[0])
 * @{
 */
#define HHSDC_IO1HOC_ENABLED  0x00U /**< @brief HVIO1 high-side driver is controlled by the setting in the HVIO1 control register (See uja113x_hvio_cfg_t) */
#define HHSDC_IO1HOC_DISABLED 0x01U /**< @brief HVIO1 high-side driver is off, regardless of the setting in the HVIO1 control register */
/** @}*/
/** @}*/

/**
 * @defgroup mtpnvlsc_group HVIO low-side configuration options (0x72)
 * @details This section lists all the possible options of 0x72 register related
 * to:
 * <ul>
 * <li>HVIO N low-side driver configurations</li>
 * </ul>
 * @see UJA113X_SetMtpnv()
 * @ingroup mtpnvr
 * @{
 */
/**
 * @name HVIO8 low-side driver configuration options (0x72:IO8LOC[7])
 * @{
 */
#define HLSDC_IO8LOC_ENABLED  0x00U /**< @brief HVIO8 low-side driver is contolled by the setting in the HVIO8 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO8LOC_DISABLED 0x80U /**< @brief HVIO8 low-side driver is off, regardless of the setting in the HVIO8 control register */
/** @}*/
/**
 * @name HVIO7 low-side driver configuration options (0x72:IO7LOC[6])
 * @{
 */
#define HLSDC_IO7LOC_ENABLED  0x00U /**< @brief HVIO7 low-side driver is contolled by the setting in the HVIO8 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO7LOC_DISABLED 0x40U /**< @brief HVIO4 low-side driver is off, regardless of the setting in the HVIO8 control register */
/** @}*/
/**
 * @name HVIO6 low-side driver configuration options (0x72:IO6LOC[5])
 * @{
 */
#define HLSDC_IO6LOC_ENABLED  0x00U /**< @brief HVIO6 low-side driver is contolled by the setting in the HVIO6 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO6LOC_DISABLED 0x20U /**< @brief HVIO6 low-side driver is off, regardless of the setting in the HVIO6 control register */
/** @}*/
/**
 * @name HVIO5 low-side driver configuration options (0x72:IO5LOC[4])
 * @{
 */
#define HLSDC_IO5LOC_ENABLED  0x00U /**< @brief HVIO5 low-side driver is contolled by the setting in the HVIO5 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO5LOC_DISABLED 0x10U /**< @brief HVIO5 low-side driver is off, regardless of the setting in the HVIO5 control register */
/** @}*/
/**
 * @name HVIO4 low-side driver configuration options (0x72:IO4LOC[3])
 * @{
 */
#define HLSDC_IO4LOC_ENABLED  0x00U /**< @brief HVIO4 low-side driver is contolled by the setting in the HVIO4 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO4LOC_DISABLED 0x08U /**< @brief HVIO4 low-side driver is off, regardless of the setting in the HVIO4 control register */
/** @}*/
/**
 * @name HVIO3 low-side driver configuration options (0x72:IO3LOC[2])
 * @{
 */
#define HLSDC_IO3LOC_ENABLED  0x00U /**< @brief HVIO3 low-side driver is contolled by the setting in the HVIO3 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO3LOC_DISABLED 0x04U /**< @brief HVIO3 low-side driver is off, regardless of the setting in the HVIO3 control register */
/** @}*/
/**
 * @name HVIO2 low-side driver configuration options (0x72:IO2LOC[1])
 * @{
 */
#define HLSDC_IO2LOC_ENABLED  0x00U /**< @brief HVIO2 low-side driver is contolled by the setting in the HVIO2 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO2LOC_DISABLED 0x02U /**< @brief HVIO2 low-side driver is off, regardless of the setting in the HVIO2 control register */
/** @}*/
/**
 * @name HVIO1 low-side driver configuration options (0x72:IO1LOC[0])
 * @{
 */
#define HLSDC_IO1LOC_ENABLED  0x00U /**< @brief HVIO1 low-side driver is contolled by the setting in the HVIO1 control register (See uja113x_hvio_cfg_t) */
#define HLSDC_IO1LOC_DISABLED 0x01U /**< @brief HVIO1 low-side driver is off, regardless of the setting in the HVIO1 control register */
/** @}*/
/** @}*/

/**
 * @defgroup mtpnvsuc_group Start-up control configuration options (0x73)
 * @details This section lists all the possible options of 0x73 register related
 * to:
 * <ul>
 * <li>Reset pulse width on RSTN pin</li>
 * <li>V2 start-up configuration</li>
 * <li>HVIO 4 - 2 pin mode selection</li>
 * </ul>
 * @see UJA113X_SetMtpnv()
 * @ingroup mtpnvr
 * @{
 */
/**
 * @name RSTN output reset pulse width configuration options (0x73:RLC[5:4])
 * @{
 */
#define RLC_PULSE_WIDTH_20_25_MS 0x00U /**< @brief tw(rst) = 20 ms to 25 ms */
#define RLC_PULSE_WIDTH_10_12_MS 0x10U /**< @brief tw(rst) = 10 ms to 12.5 ms */
#define RLC_PULSE_WIDTH_03_05_MS 0x20U /**< @brief tw(rst) = 3.6 ms to 5 ms */
#define RLC_PULSE_WIDTH_01_MS    0x30U /**< @brief tw(rst) = 1 ms to 1.5 ms */
/** @}*/
/**
 * @name V2 start-up control configuration options (0x73:V2SUC[3])
 * @see uja113x_rc_v2c_t
 * @{
 */
#define V2SUC_ALWAYS_OFF 0x00U /**< @brief Bits V2C is set to @ref V2C_OFF (00) at power-up */
#define V2SUC_ALWAYS_ON  0x08U /**< @brief Bits V2C is set to @ref V2C_ON_NSSRF (11) at power-up */
/** @}*/
/**
 * @name HVIO4 configuration control options (0x73:IO4SFC[2])
 * @{
 */
#define IO4SFC_STD_IO    0x00U /**< @brief Configure pin HVIO4 as a standard I/O pin */
#define IO4SFC_LIMP_HOME 0x04U /**< @brief Enable HVIO4 limp home function */
/** @}*/
/**
 * @name HVIO3 configuration control options (0x73:IO3SFC[1])
 * @{
 */
#define IO3SFC_STD_IO    0x00U /**< @brief Configure pin HVIO3 as a standard I/O pin */
#define IO3SFC_LIMP_HOME 0x02U /**< @brief Enable HVIO3 limp home function */
/** @}*/
/**
 * @name HVIO2 configuration control options (0x73:IO2SFC[0])
 * @{
 */
#define IO2SFC_STD_IO    0x00U /**< @brief Configure pin HVIO2 as a standard I/O pin */
#define IO2SFC_LIMP_HOME 0x01U /**< @brief Enable HVIO2 limp home function */
/** @}*/
/** @}*/

/**
 * @defgroup mtpnvsbccc_group SBC configuration control options (0x74)
 * @details This section lists all the possible options of 0x74 register related
 * to:
 * <ol>
 * <li>V1 reset threshold at start-up configuration</li>
 * <li>FNMC mode configuration</li>
 * <li>SDMC mode configuration</li>
 * <li>VEXT pin mode selection</li>
 * <li>Sleep mode support configuraion</li>
 * </ol>
 * @see UJA113X_SetMtpnv()
 * @ingroup mtpnvr
 * @{
 */
/**
 * @name V1 reset threshold (defined by V1RTC) at start-up options (0x74:V1RTSUC[5:4])
 * @{
 */
#define V1RTSUC_90_NOM 0x00U /**< @brief V1 undervoltage detection at 90% of nominal value at start-up (V1RTC = 00) */
#define V1RTSUC_80_NOM 0x10U /**< @brief V1 undervoltage detection at 80% of nominal value at start-up (V1RTC = 01) */
#define V1RTSUC_70_NOM 0x20U /**< @brief V1 undervoltage detection at 70% of nominal value at start-up (V1RTC = 10) */
#define V1RTSUC_60_NOM 0x30U /**< @brief V1 undervoltage detection at 60% of nominal value at start-up (V1RTC = 11) */
/** @}*/
/**
 * @name Forced normal mode options (0x74:FNMC[3])
 * @{
 */
#define FNMC_DISABLE 0x00U /**< @brief Disable Forced Normal mode */
#define FNMC_ENABLE  0x08U /**< @brief Enabled Forced Normal mode */
/** @}*/
/**
 * @name Software Development mode options (0x74:SDMC[2])
 * @{
 */
#define SDMC_DISABLE 0x00U /**< @brief Disable Software Development mode */
#define SDMC_ENABLE  0x04U /**< @brief Enable Software Development mode */
/** @}*/
/**
 * @name V2 regulator configuration options (0x74:VEXTAC[1])
 * @{
 */
#define VEXTAC_SENSOR_SUPPLY 0x00U /**< @brief Regulator V2 can be used as a sensor supply via pin VEXT, provided pin V2 is left floating */
#define VEXTAC_NO_PROTECTION 0x02U /**< @brief Regulator V2 is not protected against shorts to higher voltages; pin V2 must be shorted to pin VEXT */
/** @}*/
/**
 * @name Sleep mode configuration options (0x74:SLPC[0])
 * @{
 */
#define SLPC_SLEEP_SUPPORTED     0x00U /**< @brief Sleep mode is supported */
#define SLPC_SLEEP_NOT_SUPPORTED 0x01U /**< @brief Sleep mode is not supported */
/** @}*/
/** @}*/
/**
 * @brief Watchdog register configuration structure (reg 0x00)
 * 
 * @ingroup wtdcconfig
 *
 * Implements : uja113x_wtdc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures the Watchdog Mode (0x00:WMC[7:5])
     */
    uja113x_wtdc_wmc_t wtdModeCtrl;
    /**
     * @brief Configures the watchdog nominal period (0x00:NWP[3:0])
     */
    uja113x_wtdc_nwp_t nomWtdPeriod;
} uja113x_wtdc_cfg_t;

/**
 * @brief Fail-safe register configuration structure (reg 0x02)
 * 
 * @ingroup fscconfig
 *
 * Implements : uja113x_fsc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures the behaviour of EN pin during battery over-/undervoltage (0x02:ENSC[7:6])
     */
    uja113x_fsc_ensc_t enShutdownCtrl;
    /**
     * @brief Enables/disables the EN high-side driver (0x02:ENDC[5])
     */
    uja113x_fsc_endc_t enDriverCtrl;
    /**
     * @brief Configures the output level behaviour of EN pin (0x02:ENC[4:3])
     */
    uja113x_fsc_enc_t  enCtrl;
    /**
     * @brief Configures the Limp pin output behaviour (0x02:LHC[2])
     */
    uja113x_fsc_lhc_t  limpHomeCtrl;
    /**
     * @brief Configures the Reset counter value (0x02:RCC[1:0])
     */
    uja113x_fsc_rcc_t  resetCntCtrl;
} uja113x_fsc_cfg_t;

/**
 * @brief   Lock Control register configuration structure (reg 0x0A)
 * @details The structure defines, which address areas should be SPI write 
 * (un)protected.
 *
 * @see UJA113X_SetLock()
 * @ingroup lckcconfig
 * 
 * Implements : uja113x_lckc_cfg_t_Class
 */
typedef struct
{
    uja113x_lckc_lkxc_t lock6; /**< @brief Disables/enables SPI write access to
                                    address area 0x68 - 0x6F (FD version only) (0x0A:LK6C[6]). */
    uja113x_lckc_lkxc_t lock5; /**< @brief Disables/enables SPI write access to
                                    address area 0x50 - 0x55 (0x0A:LK5C[5]). */
    uja113x_lckc_lkxc_t lock4; /**< @brief Disables/enables SPI write access to
                                    address area 0x40 - 0x4F (0x0A:LK4C[4]). */
    uja113x_lckc_lkxc_t lock3; /**< @brief Disables/enables SPI write access to
                                    address area 0x30 - 0x3F (0x0A:LK3C[3]). */
    uja113x_lckc_lkxc_t lock2; /**< @brief Disables/enables SPI write access to
                                    address area 0x20 - 0x2F (0x0A:LK2C[2]). */
    uja113x_lckc_lkxc_t lock1; /**< @brief Disables/enables SPI write access to
                                    address area 0x10 - 0x1F (0x0A:LK1C[1]). */
    uja113x_lckc_lkxc_t lock0; /**< @brief Disables/enables SPI write access to
                                    address area 0x06 - 0x09 (0x0A:LK0C[0]). */
} uja113x_lckc_cfg_t;

/**
 * @brief Regulator control register configuration structure (0x10)
 * 
 * @ingroup rcconfig
 *
 * Implements : uja113x_rc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures V2 shut-down response to a battery over- or undervoltage configuration (0x10:V2SC[5:4])
     */
    uja113x_rc_v2sc_t  v2ShutdownCtrl;
    /**
     * @brief Configures output behavior on V2 (0x10:V2C[3:2])
     */
    uja113x_rc_v2c_t   v2Ctrl;
    /**
     * @brief Configures V1 undervoltage reset threshold (0x10:V1RTC[1:0])
     */
    uja113x_rc_v1rtc_t v1RstThrCtrl;
} uja113x_rc_cfg_t;

/**
 * @brief Battery monitor registers configuration structure (0x11 - 0x14)
 * 
 * @ingroup bmconfig
 *
 * Implements : uja113x_bmc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief   Selects the battery monitor event trigger source (0x11:BMSC[0])
     */  
    uja113x_bmetsc_bmsc_t bmSrcCtrl;
    /**
     * @brief   Configures the battery monitor undervoltage threshold (0x12:BMUTC[7:0])
     * @details Threshold for triggering a battery undervoltage event and BMUI 
     * interrupt;\n <b>threshold</b> = BMUTC[7:0]/255 x 20V.\n Allowed range: 0 - 255
     */
    uja113x_bmutc_bmutc_t bmUndrVoltThr;
    /**
     * @brief   Configures the battery monitor overvotlage threshold (0x13:BMOTC[7:0])
     * @details Threshold for triggering a battery overvoltage event and BMOI
     * interrupt\n <b>threshold</b> = BMOTC[7:0]/255 x 20V\n Allowed range: 0 - 255
     */
    uja113x_bmotc_bmotc_t bmOverVoltThr;
    /**
     * @brief   Configures the battery monitor hysteresis for overvoltage (0x14:BMHOC[7:4])
     * @details Battery monitor overvoltage threshold release level;
     * \n <b>release level</b> = BMHOC[7:4] x 4/255 x 20V below threshold defined by BMOTC
     * @warning Only 4 lsb are used! The upper part of byte is truncated!
     */
    uja113x_bmhc_bmhoc_t bmOverVoltThrRel;
    /**
     * @brief   Configures the battery monitor hysteresis for undervoltage (0x14:BMHUC[3:0])
     * @details Battery monitor undervoltage threshold release level;
     * \n <b>release level</b> = BMHUC[3:0] x 4/255 x 20V above threshold defined by BMUTC
     * @warning Only 4 lsb are used! The upper part of byte is truncated!
     */
    uja113x_bmhc_bmhuc_t bmUndrVoltThrRel;
} uja113x_bmc_cfg_t;

/**
 * @brief SMPS (buck-boost converter) registers configuration structure (0x19 - 0x1A)
 * 
 * @ingroup smpsconfig
 *
 * Implements : uja113x_smpsc_cfg_t_Class
 */
typedef struct
{
     /**
      * @brief Configures the behavior of Vvsmps voltage output during overtemperature (0x19:SMPSOTC[3])
     */
    uja113x_smpsc_smpsotc_t smpsOvertempCtrl;
    /**
     * @brief Configures the SMPS on/off state in different modes (0x19:SMPSC[1:0])
     */
    uja113x_smpsc_smpsc_t   smpsCtrl;
    /**
     * @brief Configures the SMPS output voltage level (0x1A:SMPSOC[3:0])
     */
    uja113x_smpsovc_smpsoc_t  smpsOutVolt;
} uja113x_smpsc_cfg_t;

/**
 * @brief CAN control register configuration structure (0x20)
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_canc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures whether CAN FD frames are tolerated or not (0x20:CFDC[6])
     */
    uja113x_canc_cfdc_t canFDCtrl;
    /**
     * @brief Disables/enables CAN selective wake-up (0x20:CPNC[4])
     */
    uja113x_canc_cpnc_t canPartNetCtrl;
    /**
     * @brief Configures CAN shut-down behaviour during battery over- or undervoltage (0x20:CSC[3:2])
     */
    uja113x_canc_csc_t  canShutdownCtrl;
    /**
     * @brief Selects the CAN transceiver operating mode (0x20:CMC[1:0])
     */
    uja113x_canc_cmc_t  canModeCtrl;
} uja113x_canc_cfg_t;

/**
 * @brief LIN control register configuration structure (0x21)
 * 
 * @ingroup lincconfig
 *
 * Implements : uja113x_linc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures LIN2 slope control behaviour (0x21:LSC2[7:6])
     */
    uja113x_linc_lsc2_t lin2SlopeCtrl;
    /**
     * @brief Configures LIN2 transceiver operating mode (0x21:LMC2[5:4])
     */
    uja113x_linc_lmc2_t lin2ModeCtrl;
    /**
     * @brief Configures LIN1 slope control behaviour (0x21:LSC1[3:2]) 
     */
    uja113x_linc_lsc1_t lin1SlopeCtrl;
    /**
     * @brief Configures LIN1 transceiver operating mode (0x21:LMC1[1:0])
     */
    uja113x_linc_lmc1_t lin1ModeCtrl;
} uja113x_linc_cfg_t;

/**
 * @brief CAN partial networking configuration structure (0x26 - 0x2F)
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_cpnc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures CAN data rate (0x26:CDR[2:0])
     */
    uja113x_dr_cdr_t canDataRate;
    /**
     * @brief Configures CAN wake-up ID (0x27 - 0x2A)
     */
    uja113x_id_id_t  id;
    /**
     * @brief Configures CAN wake-up ID mask (0x2B - 0x2E)
     */
    uja113x_idm_idm_t idMask;
    /**
     * @brief Configures whether standard/extended ID is used for wake-up (0x2F:IDE[7])
     */
    uja113x_fc_ide_t ide;
    /**
     * @brief Configures whether DLC and data are checked during wake-up (0x2F:PNDM[6])
     */
    uja113x_fc_pndm_t partNetDataMask;
    /**
     * @brief Configures the number of expected bytes during wake-up (0x2F:DLC[3:0])
     */
    uja113x_fc_dlc_t dataLenCode;
} uja113x_cpnc_cfg_t;

/**
 * @brief CAN partial networking data mask registers configuration structure (0x68 - 0x6F)
 * 
 * @ingroup cancconfig
 *
 * Implements : uja113x_dm_cfg_t_Class
 */
typedef struct
{
    uint8_t dm0; /**< @brief Data mask 0 configuration (0x68:DM0[7:0]) */
    uint8_t dm1; /**< @brief Data mask 1 configuration (0x69:DM1[7:0]) */
    uint8_t dm2; /**< @brief Data mask 2 configuration (0x6A:DM2[7:0]) */
    uint8_t dm3; /**< @brief Data mask 3 configuration (0x6B:DM3[7:0]) */
    uint8_t dm4; /**< @brief Data mask 4 configuration (0x6C:DM4[7:0]) */
    uint8_t dm5; /**< @brief Data mask 5 configuration (0x6D:DM5[7:0]) */
    uint8_t dm6; /**< @brief Data mask 6 configuration (0x6E:DM6[7:0]) */
    uint8_t dm7; /**< @brief Data mask 7 configuration (0x6F:DM7[7:0]) */
} uja113x_dm_cfg_t;

/**
 * @brief HVIOn control register configuration structure (0x3N; 0x4N; N = 0 - 3)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_hvio_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures HVIOn shut-down behavior (0x3N/0x4N:IOnSC[7:6])
     */
    uja113x_hvion_ionsc_t ionShutdownCtrl;
    /**
     * @brief Configures HVIOn activation behavior (0x3N/0x4N:[IOnAC[5:3])
     */
    uja113x_hvion_ionac_t ionActivateCtrl;
    /**
     * @brief Configures HVIOn output behavior (0x3N/0x4N:[IOnCC[2:0])
     */
    uja113x_hvion_ioncc_t ionConfCtrl;
} uja113x_hvio_cfg_t;

/**
 * @brief Bank 0/1 short-circuit detection threshold configuration structure (0x39/0x49)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_bscdt_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures HVIO4(8) short-circuit detection threshold (0x39/0x49:IO4(8)SCTC[7:6])
     */
    uja113x_bscdt_ionsctc_t io48ShortCircuitThrCtrl;
    /**
     * @brief Configures HVIO3(7) short-circuit detection threshold (0x39/0x49:IO3(7)SCTC[5:4])
     */
    uja113x_bscdt_ionsctc_t io37ShortCircuitThrCtrl;
    /**
     * @brief Configures HVIO2(6) short-circuit detection threshold (0x39/0x49:IO2(6)SCTC[3:2])
     */
    uja113x_bscdt_ionsctc_t io26ShortCircuitThrCtrl;
    /**
     * @brief Configures HVIO1(5) short-circuit detection threshold (0x39/0x49:IO1(5)SCTC[1:0])
     */
    uja113x_bscdt_ionsctc_t io15ShortCircuitThrCtrl;
} uja113x_bscdt_cfg_t;

/**
 * @brief Bank 0/1 open-load detection threshold configuration structure (0x3A/0x4A)
 * 
 * @ingroup hviocr
 *
 * Implements : uja113x_boldt_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures HVIO4(8) open-load detection threshold (0x3A/4A:IO4(8)OLTC[7:6])
     */
    uja113x_boldt_ionoltc_t io48OpenLoadThrCtrl;
    /**
     * @brief Configures HVIO4(8) open-load detection threshold (0x3A/4A:IO3(7)OLTC[5:4])
     */
    uja113x_boldt_ionoltc_t io37OpenLoadThrCtrl;
    /**
     * @brief Configures HVIO4(8) open-load detection threshold (0x3A/4A:IO2(6)OLTC[3:2])
     */
    uja113x_boldt_ionoltc_t io26OpenLoadThrCtrl;
    /**
     * @brief Configures HVIO4(8) open-load detection threshold (0x3A/4A:IO1(5)OLTC[1:0])
     */
    uja113x_boldt_ionoltc_t io15OpenLoadThrCtrl;

} uja113x_boldt_cfg_t;

/**
 * @brief Single timer control registers configuration structure (0x5x - 0x5x + 1)
 * 
 * @ingroup timcr
 *
 * Implements : uja113x_timsc_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Timer to configure (0 - 3)
     */
    uint8_t nTimer;
    /**
     * @brief Configures Timer X period (reg 0x5<b>N</b>:TXPC[5:2]; N = 0, 2, 4, 6)
     */
    uja113x_timxc_txpc_t  timXPeriodCtrl;
    /**
     * @brief Configures Timer X mode (reg 0x5<b>N</b>:TXMC[1:0]); N = 0, 2, 4, 6)
     */
    uja113x_timxc_txmc_t  timXModeCtrl;
    /** 
     * @brief Configures Timer X duty cycle (reg 0x5<b>N</b>):TXDCC[7:0]; N = 1, 3, 5, 7)
     * @details <b>duty cycle</b> = TXDCC / 255 x 100 [%]
     */
    uja113x_timxdcc_txdcc_t timXDutyCycleCtrl;
} uja113x_timsc_cfg_t;

/**
 * @brief MTPNV registers configuration structure (0x71 - 0x74)
 * 
 * @ingroup mtpnvr
 *
 * Implements : uja113x_mtpnv_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Configures the HVIO high-side driver behavior (0x71); See @ref mtpnvhsc_group
     */ 
    uint8_t highSideCtrl;
    /**
     * @brief Configures the HVIO low-side driver behavior (0x72); See @ref mtpnvlsc_group
     */
    uint8_t lowSideCtrl;
    /**
     * @brief Configures the start-up control register (0x73); See @ref mtpnvsuc_group
     */
    uint8_t startupCtrl;
    /**
     * @brief Configures the SBC configuration control register (0x74); See @ref mtpnvsbccc_group
     */
    uint8_t sbcConfigCtrl;
} uja113x_mtpnv_cfg_t;

/**
 * @brief SBC driver configuration structure.
 * @details Every SBC is represented by an internal driver structure, which
 * needs to be initialized first. This is achieved by defining Based on this structure an UJA113X SBC has to be registered.
 * 
 * @ingroup internals
 * @see UJA113X_RegisterDevice()
 *
 * Implements : uja113x_drv_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Defines the SBC type used by this instance
     */
    uja113x_chip_t chipType;
    /**
     * @brief Defines which LPSPI is used to communicate with this device
     */
    uint32_t lpspiInstance;
    /**
     * @brief Defines the SPI timeout for a single transaction
     */
    uint32_t timeout;
} uja113x_drv_cfg_t;

/**
 * @brief HVIO bank configuration structure
 * 
 * @ingroup internals
 * 
 * Implements : uja113x_hvio_bank_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Array of HVIO control register configuration structures (0x30 - 0x33; 0x40 - 0x44)
     */
    uja113x_hvio_cfg_t hvioConfig[BANK_HVIO_CNT];
    /**
     * @brief Bank N wake-up threshold configuration options (0x34 / 0x44)
     */
    uja113x_bwtc_bnwtc_t bankWupThr;
    /**
     * @brief Bank N wake-up interrupt masks (0x37 / 0x47)
     */
    uint8_t bankWupIntMask;
    /**
     * @brief Bank N failure interrupt masks (0x38 / 0x48)
     */
    uint8_t bankFailIntMask;
    /**
     * @brief Bank N short-circuit detection threshold configuration structure (0x39 / 0x49)
     */
    uja113x_bscdt_cfg_t bscdtBankShortCirc;
    /**
     * @brief Bank N open-load detection threshold configuration structure (0x3A / 0x4A)
     */
    uja113x_boldt_cfg_t boldtBankOpenLoad;
} uja113x_hvio_bank_cfg_t;

/**
 * @brief SBC global initialization structure
 * 
 * @ingroup internals
 * @see UJA113X_Init()
 * 
 * Implements : uja113x_init_cfg_t_Class
 */
typedef struct
{
    /**
     * @brief Watchdog configuration
     */
    uja113x_wtdc_cfg_t        wdcWatchdogCfg;
    /**
     * @brief Desired mode after initialization
     */
    uja113x_mc_mc_t           mcSbcMode;
    /**
     * @brief Fail-safe configuration
     */
    uja113x_fsc_cfg_t         fscCfg;
    /**
     * @brief Defines which system interrupts to enable
     */
    uint8_t                   sysieMask;
    /**
     * @name General purpose memory settings (0x06 - 0x09)
     */
    uint8_t                   memory0;
    uint8_t                   memory1;
    uint8_t                   memory2;
    uint8_t                   memory3;
    /** @}*/
    /**
     * @brief Regulator control configuration parameters (0x10)
     */
    uja113x_rc_cfg_t          rcRegulatorCfg;
    /**
     * @brief Battery monitor configuration
     */
    uja113x_bmc_cfg_t         bmcBatMonitorCfg;
    /**
     * @brief SMPS output voltage control configuration parameter (0x1A)
     */
    uja113x_smpsc_cfg_t       smpscSmpsCfg;
    /**
     * @brief Supply interrupt enable configuration parameters (0x1C)
     */
    uint8_t                   supieMask;
    /**
     * @brief CAN configuration
     */
    uja113x_canc_cfg_t        cancCanCfg;
    /**
     * @brief LIN 1 and 2 configuration
     */
    uja113x_linc_cfg_t        linxLinCtrl;
    /**
     * @brief CAN partial networking configuration
     */
    uja113x_cpnc_cfg_t        cpncCanPartNetCfg;
    /**
     * @brief Data mask configuration
     */
    uja113x_dm_cfg_t          dmCanDataMaskCfg;
    /**
     * @brief Transceiver interrupt enable register configuration parameters (0x23)
     */
    uint8_t                   tieMask;
    /**
     * @brief Configuration of HVIO bank 0 (HVIO 1 - 4)
     */
    const uja113x_hvio_bank_cfg_t * hvioBank0Cfg;
    /**
     * @brief Configuration of HVIO bank 1 (HVIO 5 - 8)
     */
    const uja113x_hvio_bank_cfg_t * hvioBank1Cfg;
    /**
     * @brief Configuration of timer 1
     */
    const uja113x_timsc_cfg_t     * timscTimer1Cfg;
    /**
     * @brief Configuration of timer 2
     */
    const uja113x_timsc_cfg_t     * timscTimer2Cfg;
    /**
     * @brief Configuration of timer 3
     */
    const uja113x_timsc_cfg_t     * timscTimer3Cfg;
    /**
     * @brief Configuration of timer 4
     */
    const uja113x_timsc_cfg_t     * timscTimer4Cfg;
    /**
     * @brief Configuration of register lock
     */
    const uja113x_lckc_cfg_t      * lckcLockRegCfg;
} uja113x_init_cfg_t;

/**
 * @brief Initiates a write/read operation to/from SBC
 * @details Based on input arguments the function initiates a data transfer
 * with SBC. Although the function allows to transfer any amount of data, the 
 * minimum number of bytes to transfer is 2 (address + 1 byte), maximum is 4 
 * (address + 3 bytes) as defined by the SBCs SPI protocol. Otherwise the frame 
 * is considered as invalid and an SPI failure is raised by the SBC. The read 
 * parameter defines, whether a read or write from/to SBC is requested.\n
 * The first byte of @p txData must contain the address to read/write
 * from/to.
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] read Read operation is carried out on true, write otherwise
 * @param[in] txData Buffer to write into SBC
 * @param[out] rxData Buffer for received data
 * @param[in] size Number of bytes to write/read
 * 
 * @return STATUS_SUCCESS Write/read successful
 *         STATUS_BUSY SPI periphral busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * 
 * @warning The first byte of txData must contain the register address to read/write
 * from/to!
 * @warning The function doesn't check the size of data. Make sure you write
 * 2 - 4 bytes!
 * @note The LSB bit of the address in first byte is not taken into account.
 * The  read parameter determines, whether a read/write will be performed.
 * 
 * @see regmap
 * 
 * @ingroup internals
 */
status_t UJA113X_SbcTransfer(uint32_t instance,
                             bool     read,
                             uint8_t * txData,
                             uint8_t * rxData,
                             uint8_t  size);
/**
 * @brief Initializes complete chip based on input parameters except MTPNV 
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] deviceConfig SBC configuration structure
 * @param[in] drvConfig Driver internal data
 * @return    Error or success status returned by API
 * @ingroup internals
 */
status_t UJA113X_Init(uint32_t instance,
                      const uja113x_init_cfg_t * const deviceConfig,
                      const uja113x_drv_cfg_t * drvConfig);
/**
 * @brief Registers SBC internal parameters.
 * @details The library allows use of multiple UJA113X SBCs to be connected 
 * to an MCU. From this perspective every SBC has a set of internal data
 * necessary for proper functionality. This function should be used to set
 * these parameters for a single SBC based on input parameters.
 * 
 * @param[in] instance SBC index (must be lower than UJA113X_DRIVER_COUNT)
 * @param[in] drvConfig SBC configuration structure
 * 
 * @return SBC_SUCCESS Configuration successful
 *         SBC_ERR_NA SBC instance out of range
 * 
 * @ingroup internals
 */
status_t UJA113X_RegisterDevice(uint32_t instance,
                                const uja113x_drv_cfg_t * drvConfig);

/**
 * @brief Sets the watchdog register (0x00) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object 
 * and initiates SPI communication in blocking mode. On successful transmission 
 * the first byte is verified. In case of a mismatch the watchdog register has 
 * not been written and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] wtdcConfig Watchdog configuration object
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @warning The watchdog mode can be re-configured in Standby mode only! Changing 
 * the watchdog mode in Normal mode will cause the UJA113x to switch
 * to Reset mode and the reset source status bits (RSS) will be set to 10000 (
 * illegal watchdog mode control access). However writing the same values
 * doesn't lead to SBC reset, but a watchdog trigger which clears the watchdog
 * counter!
 *
 * @note To reset the watchdog counter, a successful write into the watchdog 
 * control register (0x00) has to be performed. This function can be used to
 * reset the counter, however keep in mind that updating mode in non-Standby
 * modes leads to SBC reset!\n
 * Although updating the watchdog period is allowed, SBC reset due to an overflow 
 * can happen after configuring a shorter period!
 *
 * @see UJA113X_SetMode()
 * @ingroup wtdcconfig
 */
status_t UJA113X_SetWatchdog(uint32_t instance, 
                             const uja113x_wtdc_cfg_t * wtdcConfig);

/**
 * @brief Sets the mode control register (0x01) to get the SBC into a new mode
 *
 * @details The function prepares a frame based on input parameter and initiates 
 * SPI communication in blocking mode. On successful transmission the first 
 * byte is verified. In case of a mismatch the watchdog register has 
 * not been written and the process has to be repeated.
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] modeConfig New mode 
 *
 * @return  STATUS_SUCCESS Configuration successful
 *          STATUS_BUSY SPI peripheral is busy
 *          STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *          SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The SBC can be set into three modes (Normal, Standby, Sleep) based on
 * the input parameter. 
 * @warning Entering the Normal or Standby mode requires no special treatment.
 * However entering a Sleep mode requires to clear all pending interrupts/events
 * (which would set the SBCs INTN1/INTN2 to high) and at least one wake-up 
 * source to be enabled. Otherwise the SBC can get stuck at Sleep mode without 
 * a possibility to wake it up!
 *
 * @ingroup modeconfig
 */
status_t UJA113X_SetMode(uint32_t instance, 
                         uja113x_mc_mc_t modeConfig);

/**
 * @brief Sets the fail-safe register (0x02) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the fail-safe control
 * register has not been written and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] fscConfig Fail-safe configuration object
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @warning Care has to be taken while configuring the reset counter (resetCntCtrl).
 * In case the SBC reset counter is set to 3 and a reset occurs, the SBC will
 * be switched into Forced Sleep Mode! This can be used as a safety measure,
 * which doesn't allow any SBC resets, however can lead to unexpected behavior
 * if configured improperly! Read the datasheet carefully to configure this
 * parameter the right way.
 *
 * @note After every SBC reset the reset counter is incremented. To reset the
 * counter, call this function with a requested reset counter value.\n
 * Bear in mind that "clearing" the reset counter requires you to write the
 * whole register. This can clear your preconfigured values from SBC initialization
 * step leading to unwanted behavior. So in case of reset counter reset, the
 * rest of parameters has to be configured as well!
 *
 * @ingroup fscconfig
 */
status_t UJA113X_SetFailSafe(uint32_t instance, 
                             const uja113x_fsc_cfg_t * fscConfig);

/**
 * @brief Sets the system interrupt enable register (0x04) based on input configuration paramater
 *
 * @details The function prepares a frame based on input configuration parameter
 * and initiates SPI communcation in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the system interrupt enable
 * register has not been written and the process has to be repeated.\n
 * Bits 7 - 3 & 0 are cleared (reserved bits).
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] sysieMask Defines which system interrupts to enable
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @see sysie_group
 * @ingroup isr
 */
status_t UJA113X_SetSystemInterrupts(uint32_t instance, 
                                     uint8_t sysieMask);

/**
 * @brief Sets the lock control register (0x0A) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the lock control
 * register has not been written and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] lckcConfig Lock control configuration object
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note This function sets a lock on certain memory areas to prevent unintented
 * register modifications over SPI, thus the function should be called after
 * the whole SBC has been initialized!\n
 * Write accesses to locked memory areas can lead to SPI failure interrupt, if
 * enabled.\n
 * The lock register is never locked, thus future (un)locking is possible.
 *
 * @ingroup lckcconfig
 * 
 *
 */
status_t UJA113X_SetLock(uint32_t instance, 
                         const uja113x_lckc_cfg_t * lckcConfig);

/**
 * @brief Sets the regulator control register (0x10) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the regulator control
 * register has not been written and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] rcConfig Regulator control configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @ingroup rcconfig
 */
status_t UJA113X_SetRegulator(uint32_t instance, 
                              const uja113x_rc_cfg_t * rcConfig);

/**
 * @brief Sets the Battery monitor control registers (0x11 - 0x14) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the regulator control
 * registers were not written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] bmcConfig Battery monitor control configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The function configures all the battery monitor features:
 * <ol>
 * <li>Battery monitor event trigger source (0x11)</li>
 * <li>Battery monitor undervoltage threshold (0x12)</li>
 * <li>Battery monitor overvoltage threshold (0x13)</li>
 * <li>Battery monitor hysteresis (0x14)</li>
 * </ol>
 * As the SPI protocol allows only 16/24/32 bit writes, two subsequent SPI
 * transfers are carried out!
 *
 * @ingroup bmconfig
 */
status_t UJA113X_SetBatteryMonitor(uint32_t instance, 
                                   const uja113x_bmc_cfg_t * bmcConfig);

/**
 * @brief Sets the SMPS control registers (0x19 - 0x1A) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the SMPS control
 * registers were not written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] smpscConfig SMPS control configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The function configures all the SMPS features:
 * <ol>
 * <li>SMPS overtemperature control (0x19)</li>
 * <li>SMPS on/off control (0x19)</li>
 * <li>SMPS output voltage (0x1A)</li>
 * </ol>
 *
 * @ingroup smpsconfig
 */
status_t UJA113X_SetSmps(uint32_t instance, 
                         const uja113x_smpsc_cfg_t * smpscConfig);

/**
 * @brief Sets the supply interrupt enable register (0x1C) based on input parameter
 *
 * @details The function prepares a frame based on input configuration parameter
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the supplie interrupt 
 * register has not been written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] supieMask Defines which supply interrupts to enable
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @see supie_group
 * @ingroup isr
 */
status_t UJA113X_SetSupplyInterrupts(uint32_t instance, 
                                     uint8_t supieMask);

/**
 * @brief Sets the CAN control register (0x20) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the CAN control register 
 * has not been written properly and the process has to be repeated.\n
 * This function configures the PNCOK bit based on CPNC bit. In case the CAN
 * Partial Networking is enabled (CPNC is set), this function automatically
 * sets the PNCOK bit as well.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] cancConfig CAN control configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note Part of the CAN control configuration is also the PNCOK setting.
 * This bit confirms that CAN partial networking configuration is valid, however,
 * it is cleared after a write access to any of the CAN partial networking
 * configuration registers (0x26 - 0x2F, 0x68 - 0x6F).\n
 * To use the CAN partial networking but avoid unnecessary writes into the SBC
 * the CAN partial networking configuration prior to UJA113X_SetCan() call is 
 * recommended. Afterwards, this function can be called with CPNC set which
 * automatically sets the PNCOK.
 *
 * @warning The PNCOK bit is cleared after a write access to any of the CAN 
 * partial networking configuration registers (0x26 - 0x2F, 0x68 - 0x6F)
 * 
 * @see UJA113X_SetCanPartNet() UJA113X_SetDataMasks()
 * @ingroup cancconfig
 */
status_t UJA113X_SetCan(uint32_t instance, 
                        const uja113x_canc_cfg_t * cancConfig);

/**
 * @brief Sets the LIN control register (0x21) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the LIN control register 
 * has not been written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] lincConfig LIN control configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * 
 * @note The reference manual states, that bits 7:4 (LIN2 configuration) of 
 * register 0x21 should remain cleared for devices incorporating only LIN1. 
 * Therefore for these devices the function clears bits even when configured to non-zero value!
 *
 * @ingroup lincconfig
 */
status_t UJA113X_SetLin(uint32_t instance,
                        const uja113x_linc_cfg_t * lincConfig);

/**
 * @brief Sets the transceiver interrupt enable register (0x23) based on input configuration parameter
 *
 * @details The function prepares a frame based on input configuration parameter
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the transceiver interrupt
 * enable register has not been written properly and the process has to be repeated.\n
 * Bits 7 - 5 are cleared (reserved bits).
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] tieMask Defines which transceiver interrupts to enable
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @see tie_group
 * @ingroup isr
 */
status_t UJA113X_SetTransceiverInterrupts(uint32_t instance, 
                                          uint8_t tieMask);

/**
 * @brief Sets the CAN partial networking control register (0x26 - 0x2F) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the CAN partial networking
 * control registers have not been written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] cpncConfig CAN partial networking configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note It's possible to wake up on ID reception only and not evaluate the
 * DLC & data content. From this perspective it would make no sense to configure 
 * data masks. Thus the data masks have to be configured with a separate function.\n\n
 * 
 * As the SPI protocol allows only 16/24/32 bit writes, four subsequent SPI
 * transfers are carried out!
 *
 * @see UJA113X_SetDataMasks()
 * @ingroup cancconfig
 */
status_t UJA113X_SetCanPartNet(uint32_t instance, 
                               const uja113x_cpnc_cfg_t * cpncConfig);

/**
 * @brief Sets the CAN partial networking data mask registers (0x68 - 0x6F) based on input configuration object
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the CAN partial networking
 * data mask registers have not been written properly and the process has to 
 * be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] dmConfig CAN partial networking data masks configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note This function configures all the data masks. In case the user is
 * interested in configuring a single mask, a general function for SBC register
 * write can be used instead!
 * 
 * @note As the SPI protocol allows only 16/24/32 bit writes, three subsequent SPI
 * transfers are carried out!
 *
 * @see UJA113X_WriteRegister()
 * @ingroup cancconfig
 */
status_t UJA113X_SetDataMasks(uint32_t instance, 
                              const uja113x_dm_cfg_t * dmConfig);

/**
 * @brief Configures corresponding HVIO control register based on hvioId (0x3N/0x4N; N = 0 - 3)
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the HVIO control register 
 * identified by hvioId has not been written properly and the process has to be 
 * repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] hvioConfig HVIO configuration object
 * @param[in] hvioId HVIO identifier (0 = HVIO1 - 7 = HVIO8)
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The hvioId parameter determines, which HVIO shall be configured.
 * @ingroup hviocr
 */
status_t UJA113X_SetHvio(uint32_t instance,
                         const uja113x_hvio_cfg_t * hvioConfig,
                         uint8_t hvioId);
/**
 * @brief Configures corresponfing bank wake-up control register based on bankId (0x34/0x44)
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the bank N wake-up  control 
 * register identified by bankId has not been written properly and the process 
 * has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] bankWupThrConfig Bank N wake-up threshold configuration parameter
 * @param[in] bankId Bank identifier (0 - 1)
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * 
 * @warning This function only checks the maximum number of HVIO banks, as
 * write to "non-existing" registers are allowed. Thus for devices without
 * HVIO this function will perform an "empty" write.
 * @ingroup hviocr
 */
status_t UJA113X_SetBankWupThr(uint32_t instance,
                               uja113x_bwtc_bnwtc_t bankWupThrConfig,
                               uint8_t bankId);
/**
 * @brief Configures corresponding bank wake-up interrupt enable register based on bankId (0x37 / 0x47) 
 * @details The function prepares a frame based on input configuration parameter
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the bank N wake-up interrupt
 * enable register identified by bankId has not been written properly and the process 
 * has to be repeated.
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] bankWupIntMask Mask defining interrupts to be enabled
 * @param[in] bankId Bank identifier (0 - 1)
 * 
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * 
 * @warning This function only checks the maximum number of HVIO banks, as
 * write to "non-existing" registers are allowed. Thus for devices without
 * HVIO this function will perform an "empty" write.
 * @see bnwupie_group
 * @ingroup isr
 */
status_t UJA113X_SetBankWupInterrupts(uint32_t instance,
                                      uint8_t bankWupIntMask,
                                      uint8_t bankId);

/**
 * @brief Configures corresponding bank fail interrupt enable register based on bankId (0x38 / 0x48) 
 * @details The function prepares a frame based on input configuration parameter
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the bank N fail interrupt
 * enable register identified by bankId has not been written properly and the process 
 * has to be repeated.
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] bankFailIntMask Mask defining interrupts to be enabled
 * @param[in] bankId Bank identifier (0 - 1)
 * 
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * 
 * @see bnfie_group
 * @ingroup isr
 */
status_t UJA113X_SetBankFailInterrupts(uint32_t instance,
                                       uint8_t bankFailIntMask,
                                       uint8_t bankId);

/**
 * @brief Configures all HVIO's short-circuit detection threshold level based on bankId (0x39/0x49)
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the short-circuit detection
 * threshold control register identified by bankId has not been written properly 
 * and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] bscdtConfig Short-circuit threshold level configuration object
 * @param[in] bankId Bank identifier (0 - 1)
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The bankId parameter determines, which bank shall be configured.
 * @ingroup hviocr
 */
status_t UJA113X_SetBankShortCircThr(uint32_t instance,
                                     const uja113x_bscdt_cfg_t * bscdtConfig,
                                     uint8_t bankId);
/**
 * @brief Configures all HVIO's open-load detection threshold level based on bankId (0x3A/0x4A)
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the open-load detection 
 * threshold control register identified by bankId has not been written properly 
 * and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] boldtConfig Open-load threshold level configuration object
 * @param[in] bankId Bank identifier (0 - 1)
 *
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 *
 * @note The hvioId parameter determines, which HVIO shall be configured.
 * @ingroup hviocr
 */
status_t UJA113X_SetBankOpenLoadThr(uint32_t instance,
                                    const uja113x_boldt_cfg_t * boldtConfig,
                                    uint8_t bankId);

/**
 * @brief Sets a single timer configuration registers (0x5N - 0x5M) based on input configuration object (N = 0, 2, 4, 6; M = 1, 3, 5, 7)
 *
 * @details The function prepares a frame based on input configuration object
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the timer configuration 
 * registers have not been written properly and the process has to be repeated.
 *
 * @param[in] instance Index of SBC to configure
 * @param[in] timscConfig Single timer configuration object
 *
 * @return STATUS_SUCCESS  Configuration successful
 *         STATUS_BUSY SPI peripheral is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * configuration failed.
 * @retval SBC_ERR_NA Timer with provided index is not present on this device.
 *
 * @note This function configures a single timer. In case the user is
 * interested in configuring the period or duty cycle only, UJA113X_SbcTransfer()
 * be used instead!
 * 
 * @warning For Timer 1 the timXPeriodCtrl parameter is masked by 0x01! See
 * @ref uja113x_timxc_txmc_t for details!
 * @warning This function only checks the maximum number of timer channels, as
 * write to "non-existing" registers are allowed. Thus for devices without
 * timers this function will perform an "empty" write.
 * 
 * @see UJA113X_WriteRegister()
 * @ingroup timcr
 */
status_t UJA113X_SetTimer(uint32_t instance, 
                          const uja113x_timsc_cfg_t * timscConfig);

/**
 * @brief Writes the non-volitile memory parameters (0x71 - 0x75) of SBC
 * @details The function prepares a frame based on input configuration parameters
 * and initiates SPI communication in blocking mode. On successful transmission
 * the first byte is verified. In case of a mismatch the MTPNV configuration 
 * registers have not been written properly and the process has to be repeated.
 * In case a mismatch happens on CRC write, the outcome can be recognized by 
 * reading out the MTPNV registers, reading the MTPNV status bit and an SBC reset
 * in case of success.\n\n
 * 
 * @param[in] instance Index of SBC to configure
 * @param[in] mtpnvConfig Parameters to be loaded into SBC
 * 
 * @return STATUS_SUCCESS Configuration successful
 *         STATUS_BUSY SPI perihperal is busy
 *         STATUS_TIMEOUT SPI communication timed out, configuration aborted
 *         SBC_CMD_ERROR First received byte doesn't match the repeated address,
 * 
 * @note The CRC value is calculated by the function.
 * @warning During CRC computation for devices with partial/none HVIO's the
 * respective bits in registers 0x71 & 0x72 should be considered as 0 even when
 * set by user to 1! The function checks the chipType registered with 
 * UJA113X_RegisterDevice() and cleares these bits for the purpose of CRC computation.
 * 
 * @ingroup mtpnvr
 */
status_t UJA113X_SetMtpnv(uint32_t instance,
                          const uja113x_mtpnv_cfg_t * mtpnvConfig);
/**
 * @brief Defines the CRC8 algoritm to be used
 * @details This structure describes the CRC algorithm. Based on this structure
 * the UJA113X_ComputeCrc8(), knows, how to compute the CRC value.
 * 
 * @ingroup mtpnvr
 * 
 * Implements : crc_spec_t_Class
 */
typedef struct
{
    uint8_t poly;   /**< @brief Generating polynomial */
    uint8_t init;   /**< @brief Initial value */
    bool    refIn;  /**< @brief Reflect input data before computation */
    bool    refOut; /**< @brief Reflect output data before computation */
    uint8_t xorOut; /**< @brief Output xored by this value */
} crc_spec_t;

/**
 * @brief Generic function for CRC8 computation
 * @details The CRC algorithm is defined by @p crc_spec_t input parameter, which
 * holds all the relevant information for CRC8 computation. A brute force
 * mechanism is used instead of table driven.
 * 
 * @param[in] crcDesc Pointer to CRC8 algorithm description
 * @param[in] data Array of data over which CRC should be calculated
 * @param[in] size Size of array in bytes
 * 
 * @return Computed CRC8 over @p data described by @p crcDesc
 * 
 * @note For subsequent calls the CRC from previous computation should be used
 * as initial seed.
 * 
 * @warning The refIn and refOut values have no influence on the computation!
 * The functionality is not implemented yet!
 * @ingroup mtpnvr
 */
uint8_t UJA113X_ComputeCrc8(const crc_spec_t * crcDesc,
                            const uint8_t * data,
                            uint8_t size);
#endif /* header guard */

