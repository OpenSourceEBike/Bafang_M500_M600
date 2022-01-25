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

/*!
 * @sbc_uja113x_map.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined
 * This is required to allow the use of a macro for shifting and masking
 * uint8_t type values only.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Macro not referenced
 * Macros which are not currently referenced in code but can be used in the
 * future or by user.
 */

#ifndef SOURCES_SBC_UJA113X_MAP_H_
#define SOURCES_SBC_UJA113X_MAP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! TODO
 * @brief  Register address macros.
 */
#define UJA113X_REG_ADDR_MASK                (0xFEU)
#define UJA113X_REG_ADDR_SHIFT               (1U)
#define UJA113X_REG_ADDR_F(x)                ((uint8_t)((uint8_t)(x)<<UJA113X_REG_ADDR_SHIFT)&UJA113X_REG_ADDR_MASK)


/*******************************************************************************
 * Primary control registers (0x00 - 0x0F)
 ******************************************************************************/

/*******************************************************************************
 * System interrupt enable register (0x04)
 ******************************************************************************/
/*!
 * @brief System interrupt enable register, overtemperature warning interrupt
 * enable macros.
 */
#define UJA113X_SYSIE_OTWIE_MASK        (0x04U)
#define UJA113X_SYSIE_OTWIE_SHIFT       (2U)
#define UJA113X_SYSIE_OTWIE_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_SYSIE_OTWIE_SHIFT) & \
										UJA113X_SYSIE_OTWIE_MASK)

/*!
 * @brief System interrupt enable register, SPI failure enable.
 */
#define UJA113X_SYSIE_SPIFIE_MASK       (0x02U)
#define UJA113X_SYSIE_SPIFIE_SHIFT      (1U)
#define UJA113X_SYSIE_SPIFIE_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_SYSIE_SPIFIE_SHIFT) & \
										UJA113X_SYSIE_SPIFE_MASK)

/*!
 * @brief System interrupt enable register.
 */
#define UJA113X_SYSIE_MASK              (UJA113X_SYSIE_OTWIE_MASK | UJA113X_SYSIE_SPIFIE_MASK)
#define UJA113X_SYSIE_SHIFT             (1U)
#define UJA113X_SYSIE_F(x)              ((uint8_t)((uint8_t)(x) << UJA113X_SYSIE_SHIFT) & \
										UJA113X_SYSIE_MASK)

/*!
 * @brief Mode control register, mode control macros.
 */
#define UJA113X_MC_MC_MASK            	(0x07U)
#define UJA113X_MC_MC_SHIFT           	(0U)
#define UJA113X_MC_MC_F(x)            	((uint8_t)((uint8_t)(x) << UJA113X_MC_MC_SHIFT) & \
										UJA113X_MC_MC_MASK)
/*!
 * @brief Main status register, overtemperature warning status macros.
 */
#define UJA113X_MSS_OTWS_MASK          	(0x40U)
#define UJA113X_MSS_OTWS_SHIFT         	(6U)
#define UJA113X_MSS_OTWS_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_MSS_OTWS_SHIFT) & \
										UJA113X_MSS_OTWS_MASK)

/*!
 * @brief Main status register, Normal mode status macros.
 */
#define UJA113X_MSS_NMS_MASK           	(0x20U)
#define UJA113X_MSS_NMS_SHIFT          	(5U)
#define UJA113X_MSS_NMS_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_MSS_NMS_SHIFT) & \
										UJA113X_MSS_NMS_MASK)

/*!
 * @brief Main status register, Reset source status macros.
 */
#define UJA113X_MSS_RSS_MASK           	(0x1FU)
#define UJA113X_MSS_RSS_SHIFT          	(0U)
#define UJA113X_MSS_RSS_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_MSS_RSS_SHIFT) & \
										UJA113X_MSS_RSS_MASK)

/*!
 * @brief Main status register macros.
 */
#define UJA113X_MSS_MASK                (UJA113X_MAIN_OTWS_MASK | UJA113X_MAIN_NMS_MASK | \
										UJA113X_MAIN_RSS_MASK)
#define UJA113X_MSS_SHIFT               (0U)
#define UJA113X_MSS_F(x)                ((uint8_t)((uint8_t)(x) << UJA113X_MAIN_SHIFT) & \
										UJA113X_MAIN_MASK)

/*!
 * @brief Watchdog control register, watchdog mode control macros.
 */
#define UJA113X_WDC_WMC_MASK           	(0xE0U)
#define UJA113X_WDC_WMC_SHIFT          	(5U)
#define UJA113X_WTDOG_CTR_WMC_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_WDC_WMC_SHIFT) & \
										UJA113X_WDC_WMC_MASK)

/*!
 * @brief Watchdog control register, nominal watchdog period macros.
 */
#define UJA113X_WDC_NWP_MASK           	(0x0FU)
#define UJA113X_WDC_NWP_SHIFT          	(0U)
#define UJA113X_WDC_NWP_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_WDC_NWP_SHIFT) & \
										UJA113X_WDC_NWP_MASK)

/*!
 * @brief Watchdog control register macros.
 */
#define UJA113X_WDC_MASK               	(UJA113X_WDC_WMC_MASK | UJA113X_WDC_NWP_MASK)
#define UJA113X_WDC_SHIFT              	(0U)
#define UJA113X_WDC_F(x)               	((uint8_t)((uint8_t)(x) << UJA113X_WDC_SHIFT) & \
										UJA113X_WTDOG_CTR_MASK)

/*!
 * @brief Watchdog status register, forced Normal mode status macros.
 */
#define UJA113X_WDS_FNMS_MASK         	(0x08U)
#define UJA113X_WDS_FNMS_SHIFT        	(3U)
#define UJA113X_WDS_FNMS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_WDS_FNMS_SHIFT) & \
										UJA113X_WDS_FNMS_MASK)

/*!
 * @brief Watchdog status register, Software Development mode status macros.
 */
#define UJA113X_WDS_SDMS_MASK         	(0x04U)
#define UJA113X_WDS_SDMS_SHIFT        	(2U)
#define UJA113X_WDS_SDMS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_WDS_SDMS_SHIFT) & \
										UJA113X_WTDOG_STAT_SDMS_MASK)

/*!
 * @brief Watchdog status register, watchdog status macros.
 */
#define UJA113X_WDS_WDS_MASK          	(0x03U)
#define UJA113X_WDS_WDS_SHIFT         	(0U)
#define UJA113X_WDS_WDS_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_WDS_WDS_SHIFT) & \
										UJA113X_WTDOG_STAT_WDS_MASK)

/*!
 * @brief Watchdog status register macros.
 */
#define UJA113X_WDS_MASK              	(UJA113X_WDS_FNMS_MASK | UJA113X_WDS_SDMS_MASK | \
                                        UJA113X_WDS_WDS_MASK)
#define UJA113X_WDS_SHIFT             	(0U)
#define UJA113X_WDS_F(x)              	((uint8_t)((uint8_t)(x) << UJA113X_WDS_SHIFT) & \
										UJA113X_WDS_MASK)

/*!
 * @brief Fail-safe control register, EN shut-down control macros.
 */
#define UJA113X_FSC_ENSC_MASK      		(0xC0U)
#define UJA113X_FSC_ENSC_SHIFT     		(6U)
#define UJA113X_FSC_ENSC_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_FSC_ENSC_SHIFT) & \
										UJA113X_FSC_ENSC_MASK)

/*!
 * @brief Fail-safe control register, EN high-side driver activation macros.
 */
#define UJA113X_FSC_ENDC_MASK      		(0x20U)
#define UJA113X_FSC_ENDC_SHIFT     		(5U)
#define UJA113X_FSC_ENDC_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_FSC_ENDC_SHIFT) & \
										UJA113X_FSC_ENDC_MASK)

/*!
 * @brief Fail-safe control register, EN output configuration macros.
 */
#define UJA113X_FSC_ENC_MASK      		(0x18U)
#define UJA113X_FSC_ENC_SHIFT     		(3U)
#define UJA113X_FSC_ENC_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_FSC_ENC_SHIFT) & \
										UJA113X_FSC_ENC_MASK)

/*!
 * @brief Fail-safe control register, LIMP home control macros.
 */
#define UJA113X_FSC_LHC_MASK      		(0x04U)
#define UJA113X_FSC_LHC_SHIFT     		(2U)
#define UJA113X_FSC_LHC_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_FSC_LHC_SHIFT) & \
										UJA113X_FSC_LHC_MASK)

/*!
 * @brief  Fail-safe control register, reset counter control macros.
 */
#define UJA113X_FSC_RCC_MASK      		(0x03U)
#define UJA113X_FSC_RCC_SHIFT     		(0U)
#define UJA113X_FSC_RCC_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_FAIL_SAFE_RCC_SHIFT) & \
										UJA113X_FAIL_SAFE_RCC_MASK)

/*!
 * @brief  Fail-safe control register macros.
 */
#define UJA113X_FSC_MASK               	(UJA113X_FSC_LHC_MASK | UJA113X_FSC_RCC_MASK | \
										UJA113X_FSC_ENSC_MASK | UJA113X_FSC_ENDC_MASK | \
										UJA113X_FSC_ENC_MASK)
#define UJA113X_FSC_SHIFT              	(0U)
#define UJA113X_FSC_F(x)               	((uint8_t)((uint8_t)(x)<<UJA113X_FAIL_SAFE_SHIFT)&UJA113X_FAIL_SAFE_MASK)

/*******************************************************************************
 * Lock control register (0x0A)
 ******************************************************************************/
/*!
 * @brief  Lock control 6: address area 0x68 to 0x6F macros.
 */
#define UJA113X_LC_LK6C_MASK            (0x40U)
#define UJA113X_LC_LK6C_SHIFT           (6U)
#define UJA113X_LC_LK6C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK6C_SHIFT) & \
										UJA113X_LC_LK6C_MASK)

/*!
 * @brief  Lock control control 5: address area 0x50 to 0x5F - Timer control macros.
 */
#define UJA113X_LC_LK5C_MASK            (0x20U)
#define UJA113X_LC_LK5C_SHIFT           (5U)
#define UJA113X_LC_LK5C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK5C_SHIFT) & \
										UJA113X_LC_LK5C_MASK)

/*!
 * @brief  Lock control 4: address area 0x40 to 0x4F - HVIO5 to HVIO8 control macros.
 */
#define UJA113X_LC_LK4C_MASK            (0x10U)
#define UJA113X_LC_LK4C_SHIFT           (4U)
#define UJA113X_LC_LK4C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK4C_SHIFT) & \
										UJA113X_LC_LK4C_MASK)

/*!
 * @brief  Lock control 3: address area 0x30 to 0x3F - address area 0x30 to
 * 0x3F - HVIO1 to HVIO4 control macros.
 */
#define UJA113X_LC_LK3C_MASK            (0x08U)
#define UJA113X_LC_LK3C_SHIFT           (3U)
#define UJA113X_LC_LK3C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK3C_SHIFT) & \
										UJA113X_LC_LK3C_MASK)

/*!
 * @brief  Lock control 2: address area 0x20 to 0x2F - transceiver control macros.
 */
#define UJA113X_LC_LK2C_MASK            (0x04U)
#define UJA113X_LC_LK2C_SHIFT           (2U)
#define UJA113X_LC_LK2C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK2C_SHIFT) & \
										UJA113X_LC_LK2C_MASK)

/*!
 * @brief  Lock control 1: address area 0x10 to 0x1F - supply control macros.
 */
#define UJA113X_LC_LK1C_MASK            (0x02U)
#define UJA113X_LC_LK1C_SHIFT           (1U)
#define UJA113X_LC_LK1C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK1C_SHIFT) & \
										UJA113X_LC_LK1C_MASK)

/*!
 * @brief  Lock control 0: address area 0x06 to 0x09 - general-purpose memory macros.
 */
#define UJA113X_LC_LK0C_MASK            (0x01U)
#define UJA113X_LC_LK0C_SHIFT           (0U)
#define UJA113X_LC_LK0C_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_LC_LK0C_SHIFT) & \
										UJA113X_LC_LK0C_MASK)

/*!
 * @brief  Lock control register macros.
 */
#define UJA113X_LC_MASK               	(UJA113X_LC_LK0C_MASK | UJA113X_LC_LK1C_MASK | \
                                         UJA113X_LC_LK2C_MASK | UJA113X_LC_LK3C_MASK | \
										                     UJA113X_LC_LK4C_MASK | UJA113X_LC_LK5C_MASK | \
										                     UJA113X_LC_LK6C_MASK)
#define UJA113X_LC_SHIFT              	(0U)
#define UJA113X_LC_F(x)               	((uint8_t)((uint8_t)(x) << UJA113X_LC_SHIFT) & \
										UJA113X_LC_MASK)

/*******************************************************************************
 * Supply control registers (0x10 - 0x1F)
 ******************************************************************************/

/*******************************************************************************
 * Regulator control register (0x10)
 ******************************************************************************/
/*!
 * @brief Regulator control register, V2 shutdown response to a battery
 * over- or undervoltage macros.
 */
#define UJA113X_RC_V2SC_MASK           	(0x30U)
#define UJA113X_RC_V2SC_SHIFT          	(4U)
#define UJA113X_RC_V2SC_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_RC_V2SC_SHIFT) & \
										UJA113X_RC_V2SC_MASK)

/*!
 * @brief Regulator control register, V2 control macros.
 */
#define UJA113X_RC_V2C_MASK           	(0x0CU)
#define UJA113X_RC_V2C_SHIFT          	(2U)
#define UJA113X_RC_V2C_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_RC_V2C_SHIFT) & \
										UJA113X_RC_V2C_MASK)

/*!
 * @brief Regulator control register, V1 undervoltage reset threshold macros.
 */
#define UJA113X_RC_V1RTC_MASK         	(0x03U)
#define UJA113X_RC_V1RTC_SHIFT       	(0U)
#define UJA113X_RC_V1RTC_F(x)         	((uint8_t)((uint8_t)(x)<<UJA113X_RC_V1RTC_SHIFT) & \
										UJA113X_REGULATOR_V1RTC_MASK)

/*!
 * @brief Regulator control register macros.
 */
#define UJA113X_RC_MASK               	(UJA113X_RC_V2SC_MASK | UJA113X_RC_V2C_MASK | \
										UJA113X_RC_V1RTC_MASK)
#define UJA113X_RC_SHIFT              	(0U)
#define UJA113X_RC_F(x)               	((uint8_t)((uint8_t)(x) << UJA113X_RC_SHIFT) & \
										UJA113X_RC_MASK)

/*******************************************************************************
 * Battery monitor event trigger source control register (0x11)
 ******************************************************************************/
/*!
 * @brief Battery monitor event trigger source control register, trigger source
 * for generating battery monitoring/overvoltage/undervoltage/shutdown events.
 */
#define UJA113X_BMETSC_BMSC_MASK        (0x01U)
#define UJA113X_BMETSC_BMSC_SHIFT       (0U)
#define UJA113X_BMETSC_BMSC_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_BMETSC_BMSC_SHIFT) & \
										UJA113X_BMETSC_BMSC_MASK)

/*!
 * @brief Battery monitor event trigger source control register macros.
 */
#define UJA113X_BMETSC_MASK             (UJA113X_BMETSC_BMSC_MASK)
#define UJA113X_BMETSC_SHIFT            (0U)
#define UJA113X_BMETSC_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_BMETSC_SHIFT) & \
										UJA113X_BMETSC_MASK)

/*******************************************************************************
 * Battery monitor undervoltage threshold control register (0x12)
 ******************************************************************************/
/*!
 * @brief Battery monitor undervoltage threshold control register, threshold
 * for triggering a battery undervoltage event and BMUI interrupt.
 */
#define UJA113X_BMUTC_BMUTC_MASK        (0xFFU)
#define UJA113X_BMUTC_BMUTC_SHIFT       (0U)
#define UJA113X_BMUTC_BMUTC_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_BMUTC_BMUTC_SHIFT) & \
										UJA113X_BMUTC_BMUTC_MASK)

/*!
 * @brief Battery monitor event trigger source control register macros.
 */
#define UJA113X_BMUTC_MASK             	(UJA113X_BMUTC_BMUTC_MASK)
#define UJA113X_BMUTC_SHIFT            	(0U)
#define UJA113X_BMUTC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BMUTC_BMUTC_SHIFT) & \
										UJA113X_BMUTC_BMUTC_MASK)

/*******************************************************************************
 * Battery monitor overvoltage threshold control register (0x13)
 ******************************************************************************/
/*!
 * @brief Battery monitor overvoltage threshold control register, threshold
 * for triggering a battery overvoltage event and BMOI interrupt.
 */
#define UJA113X_BMOTC_BMOTC_MASK        (0xFFU)
#define UJA113X_BMOTC_BMOTC_SHIFT       (0U)
#define UJA113X_BMOTC_BMOTC_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_BMOTC_BMOTC_SHIFT) & \
										UJA113X_BMOTC_BMOTC_MASK)

/*!
 * @brief Battery monitor event trigger source control register macros.
 */
#define UJA113X_BMOTC_MASK             	(UJA113X_BMOTC_BMOTC_MASK)
#define UJA113X_BMOTC_SHIFT            	(0U)
#define UJA113X_BMOTC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BMOTC_BMOTC_SHIFT) & \
										UJA113X_BMOTC_BMOTC_MASK)

/*******************************************************************************
 * Battery monitor hysteresis control register (0x14)
 ******************************************************************************/
/*!
 * @brief Battery monitor hysteresis control register, battery monitor
 * overvoltage threshold release level.
 */
#define UJA113X_BMHC_BMHOC_MASK        	(0xF0U)
#define UJA113X_BMHC_BMHOC_SHIFT       	(4U)
#define UJA113X_BMHC_BMHOC_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_BMHC_BMHOC_SHIFT) & \
										UJA113X_BMHC_BMHOC_MASK)

/*!
 * @brief Battery monitor hysteresis control register, battery monitor
 * undervoltage threshold release level.
 */
#define UJA113X_BMHC_BMHUC_MASK        	(0x0FU)
#define UJA113X_BMHC_BMHUC_SHIFT       	(0U)
#define UJA113X_BMHC_BMHUC_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_BMHC_BMHUC_SHIFT) & \
										UJA113X_BMHC_BMHUC_MASK)

/*!
 * @brief Battery monitor event trigger source control register macros.
 */
#define UJA113X_BMHC_MASK             	(UJA113X_BMHC_BMHOC_MASK | UJA113X_BMHC_BMHUC_MASK)
#define UJA113X_BMHC_SHIFT            	(0U)
#define UJA113X_BMHC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BMHC_SHIFT) & \
										UJA113X_BMHC_MASK)

/*******************************************************************************
 * ADC conversion results for VBAT register 1 (0x15)
 ******************************************************************************/
/*!
 * @brief ADC conversion results for VBAT register 1, ADC conversion results
 * for voltage measured on pin BAT; 8 most significant bits.
 */
#define UJA113X_ACRVBAT1_BMBCD_MASK     (0xFFU)
#define UJA113X_ACRVBAT1_BMBCD_SHIFT    (0U)
#define UJA113X_ACRVBAT1_BMBCD_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBAT1_BMBCD_SHIFT) & \
										UJA113X_ACRVBAT1_BMBCD_MASK)

/*!
 * @brief ADC conversion results for VBAT register 1 macros.
 */
#define UJA113X_ACRVBAT1_MASK           (UJA113X_ACRVBAT1_BMBCD_MASK)
#define UJA113X_ACRVBAT1_SHIFT          (0U)
#define UJA113X_ACRVBAT1_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBAT1_SHIFT) & \
										UJA113X_ACRVBAT1_MASK)

/*******************************************************************************
 * ADC conversion results for VBAT register 2 (0x16)
 ******************************************************************************/
/*!
 * @brief ADC conversion results for VBAT register 2, ADC conversion results for
 * VBAT read out via SPI.
 */
#define UJA113X_ACRVBAT2_BMBCS_MASK     (0x04U)
#define UJA113X_ACRVBAT2_BMBCS_SHIFT    (2U)
#define UJA113X_ACRVBAT2_BMBCS_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBAT2_BMBCS_SHIFT) & \
										UJA113X_ACRVBAT2_BMBCS_MASK)

/*!
 * @brief ADC conversion results for VBAT register 2, ADC conversion results
 * for voltage measured on pin BAT; 2 least significant bits.
 */
#define UJA113X_ACRVBAT2_BMBCD_MASK     (0x03U)
#define UJA113X_ACRVBAT2_BMBCD_SHIFT    (0U)
#define UJA113X_ACRVBAT2_BMBCD_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBAT2_BMBCD_SHIFT) & \
										UJA113X_ACRVBAT2_BMBCD_MASK)

/*!
 * @brief ADC conversion results for VBATSENSE register 2 macros.
 */
#define UJA113X_ACRVBAT2_MASK           (UJA113X_ACRVBAT2_BMBCS_MASK | UJA113X_ACRVBAT2_BMBCD_MASK)
#define UJA113X_ACRVBAT2_SHIFT          (0U)
#define UJA113X_ACRVBAT2_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBAT_SHIFT) & \
										UJA113X_ACRVBAT_MASK)

/*******************************************************************************
 * ADC conversion results for VBATSENSE register 1 (0x17)
 ******************************************************************************/
/*!
 * @brief ADC conversion results for VBATSENSE register 1, ADC conversion results
 * for voltage measured on pin BAT; 8 most significant bits.
 */
#define UJA113X_ACRVBATSEN1_BMSCD_MASK  (0xFFU)
#define UJA113X_ACRVBATSEN1_BMSCD_SHIFT (0U)
#define UJA113X_ACRVBATSEN1_BMSCD_F(x)  ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBATSEN1_BMSCD_SHIFT) & \
										UJA113X_ACRVBATSEN1_BMSCD_MASK)

/*!
 * @brief ADC conversion results for VBATSENSE register 1 macros.
 */
#define UJA113X_ACRVBATSEN1_MASK        (UJA113X_ACRVBATSEN1_BMSCD_MASK)
#define UJA113X_ACRVBATSEN1_SHIFT       (0U)
#define UJA113X_ACRVBATSEN1_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBATSEN1_SHIFT) & \
										UJA113X_ACRVBATSEN1_MASK)

/*******************************************************************************
 * ADC conversion results for VBATSENSE register 2 (0x16)
 ******************************************************************************/
/*!
 * @brief ADC conversion results for VBATSENSE register 2, ADC conversion results for
 * VBATSENSE read out via SPI.
 */
#define UJA113X_ACRVBATSEN2_BMSCS_MASK  (0x04U)
#define UJA113X_ACRVBATSEN2_BMSCS_SHIFT (2U)
#define UJA113X_ACRVBATSEN2_BMSCS_F(x)  ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBATSEN2_BMSCS_SHIFT) & \
										UJA113X_ACRVBATSEN2_BMSCS_MASK)

/*!
 * @brief ADC conversion results for VBATSENSE register 2, ADC conversion results
 * for voltage measured on pin BATSENSE; 2 least significant bits.
 */
#define UJA113X_ACRVBATSEN2_BMSCD_MASK  (0x03U)
#define UJA113X_ACRVBATSEN2_BMSCD_SHIFT (0U)
#define UJA113X_ACRVBATSEN2_BMSCD_F(x)  ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBATSEN2_BMSCD_SHIFT) & \
										UJA113X_ACRVBATSEN2_BMSCD_MASK)

/*!
 * @brief ADC conversion results for VBATSENSE register 2 macros.
 */
#define UJA113X_ACRVBATSEN2_MASK        (UJA113X_ACRVBATSEN2_BMSCS_MASK | UJA113X_ACRVBATSEN2_BMSCD_MASK)
#define UJA113X_ACRVBATSEN2_SHIFT       (0U)
#define UJA113X_ACRVBATSEN2_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_ACRVBATSEN2_SHIFT) & \
										UJA113X_ACRVBATSEN2_MASK)

/*******************************************************************************
 * SMPS control register (0x19)
 ******************************************************************************/
/*!
 * @brief SMPS control register, SMPS overtemperature control macros.
 */
#define UJA113X_SMPSC_SMPSOTC_MASK      (0x08U)
#define UJA113X_SMPSC_SMPSOTC_SHIFT     (3U)
#define UJA113X_SMPSC_SMPSOTC_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_SMPSC_SMPSOTC_SHIFT) & \
										UJA113X_SMPSC_SMPSOTC_MASK)

/*!
 * @brief SMPS control register, SMPS on/off control macros.
 */
#define UJA113X_SMPSC_SMPSC_MASK      	(0x03U)
#define UJA113X_SMPSC_SMPSC_SHIFT     	(0U)
#define UJA113X_SMPSC_SMPSC_F(x)      	((uint8_t)((uint8_t)(x) << UJA113X_SMPSC_SMPSC_SHIFT) & \
										UJA113X_SMPSC_SMPSC_MASK)

/*!
 * @brief Supply voltage status register macros.
 */
#define UJA113X_SMPSC_MASK             	(UJA113X_SMPSC_SMPSOTC_MASK | UJA113X_SMPSC_SMPSC_MASK)
#define UJA113X_SMPSC_SHIFT            	(0U)
#define UJA113X_SMPSC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_SMPSC_SHIFT) & \
										UJA113X_SMPSC_MASK)

/*******************************************************************************
 * SMPS output voltage control register (0x1A)
 ******************************************************************************/
/*!
 * @brief SMPS output voltage control register, SMPS output voltage macros.
 */
#define UJA113X_SMPSOVC_SMPOC_MASK      (0x0FU)
#define UJA113X_SMPSOVC_SMPOC_SHIFT     (0U)
#define UJA113X_SMPSOVC_SMPOC_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_SMPSOVC_SMPOC_SHIFT) & \
										UJA113X_SMPSOVC_SMPOC_MASK)

/*!
 * @brief SMPS output voltage control register macros.
 */
#define UJA113X_SMPSOVC_MASK            (UJA113X_SMPSOVC_SMPOC_MASK)
#define UJA113X_SMPSOVC_SHIFT           (0U)
#define UJA113X_SMPSOVC_F(x)            ((uint8_t)((uint8_t)(x) << UJA113X_SMPSOVC_SHIFT) & \
										UJA113X_SMPSOVC_MASK)

/*******************************************************************************
 * Supply voltage status register (0x1B)
 ******************************************************************************/
/*!
 * @brief Supply voltage status register, overvoltage status of voltage on
 * selected (via BMSC) event trigger source (BAT or BATSENSE) macros.
 */
#define UJA113X_SVS_BMOVS_MASK         	(0x20U)
#define UJA113X_SVS_BMOVS_SHIFT        	(5U)
#define UJA113X_SVS_BMOVS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_SVS_BMOVS_SHIFT) & \
										UJA113X_SVS_BMOVS_MASK)

/*!
 * @brief Supply voltage status register, undervoltage status of voltage on
 * selected (via BMSC) event trigger source (BAT or BATSENSE) macros.
 */
#define UJA113X_SVS_BMUVS_MASK         	(0x10U)
#define UJA113X_SVS_BMUVS_SHIFT        	(4U)
#define UJA113X_SVS_BMUVS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_SVS_BMUVS_SHIFT) & \
										UJA113X_SVS_BMUVS_MASK)

/*!
 * @brief Supply voltage status register, status of voltage on pin VSMPS macros.
 */
#define UJA113X_SVS_SMPSS_MASK         	(0x08U)
#define UJA113X_SVS_SMPSS_SHIFT        	(3U)
#define UJA113X_SVS_SMPSS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_SVS_SMPSS_SHIFT) & \
										UJA113X_SVS_SMPSS_MASK)

/*!
 * @brief Supply voltage status register, VEXT status macros.
 */
#define UJA113X_SVS_VEXTS_MASK         	(0x06U)
#define UJA113X_SVS_VEXTS_SHIFT        	(1U)
#define UJA113X_SVS_VEXTS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_SVS_VEXTS_SHIFT) & \
										UJA113X_SVS_VEXTS_MASK)

/*!
 * @brief Supply voltage status register, V1 status macros.
 */
#define UJA113X_SVS_V1S_MASK         	(0x01U)
#define UJA113X_SVS_V1S_SHIFT        	(0U)
#define UJA113X_SVS_V1S_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_SVS_V1S_SHIFT) & \
										UJA113X_SVS_V1S_MASK)

/*!
 * @brief Supply voltage status register macros.
 */
#define UJA113X_SVS_MASK             	(UJA113X_SVS_V2S_MASK | UJA113X_SVS_V1S_MASK)
#define UJA113X_SVS_SHIFT            	(0U)
#define UJA113X_SVS_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_SVS_SHIFT) & \
										UJA113X_SVS_MASK)

/*******************************************************************************
 * Supply interrupt enable register (0x1B)
 ******************************************************************************/
/*!
 * @brief Supply interrupt enable register, SMPS status interrupt enable macros.
 */
#define UJA113X_SUPIE_SMPSSIE_MASK      (0x20U)
#define UJA113X_SUPIE_SMPSSIE_SHIFT     (5U)
#define UJA113X_SUPIE_SMPSSIE_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_SMPSSIE_SHIFT) & \
										UJA113X_SUPIE_SMPSSIE_MASK)

/*!
 * @brief Supply interrupt enable register, battery monitor overvoltage
 * interrupt enable macros.
 */
#define UJA113X_SUPIE_BMOIE_MASK      	(0x10U)
#define UJA113X_SUPIE_BMOIE_SHIFT     	(4U)
#define UJA113X_SUPIE_BMOIE_F(x)      	((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_BMOIE_SHIFT) & \
										UJA113X_SUPIE_BMOIE_MASK)

/*!
 * @brief Supply interrupt enable register, battery monitor undervoltage
 * interrupt enable macros.
 */
#define UJA113X_SUPIE_BMUIE_MASK      	(0x08U)
#define UJA113X_SUPIE_BMUIE_SHIFT     	(3U)
#define UJA113X_SUPIE_BMUIE_F(x)      	((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_BMUIE_SHIFT) & \
										UJA113X_SUPIE_BMUIE_MASK)

/*!
 * @brief Supply interrupt enable register, VEXT overvoltage interrupt enable macros.
 */
#define UJA113X_SUPIE_VEXTOIE_MASK      (0x04U)
#define UJA113X_SUPIE_VEXTOIE_SHIFT     (2U)
#define UJA113X_SUPIE_VEXTOIE_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_VEXTOIE_SHIFT) & \
										UJA113X_SUPIE_VEXTOIE_MASK)

/*!
 * @brief Supply interrupt enable register, VEXT undervoltage interrupt enable macros.
 */
#define UJA113X_SUPIE_VEXTUIE_MASK      (0x02U)
#define UJA113X_SUPIE_VEXTUIE_SHIFT     (1U)
#define UJA113X_SUPIE_VEXTUIE_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_VEXTUIE_SHIFT) & \
										UJA113X_SUPIE_VEXTUIE_MASK)

/*!
 * @brief Supply interrupt enable register, V1 undervoltage enable macros.
 */
#define UJA113X_SUPIE_V1UIE_MASK        (0x01U)
#define UJA113X_SUPIE_V1UIE_SHIFT       (0U)
#define UJA113X_SUPIE_V1UIE_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_V1UIE_SHIFT) & \
										UJA113X_SUPIE_V1UIE_MASK)

/*!
 * @brief Supply interrupt enable register macros.
 */
#define UJA113X_SUPIE_MASK             	(UJA113X_SUPIE_SMPSSIE_MASK| UJA113X_SUPIE_BMOIE_MASK | \
		                               	UJA113X_SUPIE_BMOIE_MASK | UJA113X_SUPIE_BMUIE_MASK | \
										UJA113X_SUPIE_VEXTOIE_MASK | UJA113X_SUPIE_VEXTUIE_MASK | \
									   	UJA113X_SUPIE_V1UIE_MASK)
#define UJA113X_SUPIE_SHIFT            	(0U)
#define UJA113X_SUPIE_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_SUPIE_SHIFT) & \
										UJA113X_SUPIE_MASK)

/*******************************************************************************
 * Transceiver control registers (0x20 - 0x2F)
 ******************************************************************************/

/*******************************************************************************
 * CAN control register (0x20)
 ******************************************************************************/
/*!
 * @brief CAN control register, CAN FD control macros.
 */
#define UJA113X_CANC_CFDC_MASK          (0x40U)
#define UJA113X_CANC_CFDC_SHIFT         (6U)
#define UJA113X_CANC_CFDC_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_CANC_CFDC_SHIFT) & \
										UJA113X_CANC_CFDC_MASK)

/*!
 * @brief CAN control register, CAN partial networking configuration OK macros.
 */
#define UJA113X_CANC_PNCOK_MASK         (0x20U)
#define UJA113X_CANC_PNCOK_SHIFT        (5U)
#define UJA113X_CANC_PNCOK_F(x)         ((uint8_t)((uint8_t)(x) << UJA113X_CANC_PNCOK_SHIFT) & \
										UJA113X_CANC_PNCOK_MASK)

/*!
 * @brief  CAN control register, CAN partial networking control macros.
 */
#define UJA113X_CANC_CPNC_MASK          (0x10U)
#define UJA113X_CANC_CPNC_SHIFT         (4U)
#define UJA113X_CANC_CPNC_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_CANC_CPNC_SHIFT) & \
										UJA113X_CANC_CPNC_MASK)

/*!
 * @brief CAN control register, CAN shut-down control macros.
 */
#define UJA113X_CANC_CSC_MASK          	(0x0CU)
#define UJA113X_CANC_CSC_SHIFT         	(2U)
#define UJA113X_CANC_CSC_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_CANC_CSC_SHIFT) & \
										UJA113X_CANC_CSC_MASK)

/*!
 * @brief  CAN control register, CAN transceiver operating mode selection macros.
 */
#define UJA113X_CANC_CMC_MASK           (0x03U)
#define UJA113X_CANC_CMC_SHIFT          (0U)
#define UJA113X_CANC_CMC_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_CANC_CMC_SHIFT) & \
										UJA113X_CANC_CMC_MASK)

/*!
 * @brief  CAN control register macros.
 */
#define UJA113X_CANC_MASK               (UJA113X_CANC_CFDC_MASK | UJA113X_CANC_PNCOK_MASK | \
		                                UJA113X_CANC_CPNC_MASK | UJA113X_CANC_CMC_MASK | \
										UJA113X_CANC_CSC_MASK)
#define UJA113X_CANC_SHIFT              (0U)
#define UJA113X_CANC_F(x)               ((uint8_t)((uint8_t)(x) << UJA113X_CANC_SHIFT) & \
										UJA113X_CANC_MASK)

/*******************************************************************************
 * LIN control register (0x21)
 ******************************************************************************/
/*!
 * @brief LIN control register, LIN2 slope control macros.
 */
#define UJA113X_LINC_LSC2_MASK          (0xC0U)
#define UJA113X_LINC_LSC2_SHIFT         (6U)
#define UJA113X_LINC_LSC2_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_LINC_LSC2_SHIFT) & \
										UJA113X_LINC_LSC2_MASK)

/*!
 * @brief LIN control register, LIN2 transceiver operating mode selection macros.
 */
#define UJA113X_LINC_LMC2_MASK          (0x30U)
#define UJA113X_LINC_LMC2_SHIFT         (4U)
#define UJA113X_LINC_LMC2_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_LINC_LMC2_SHIFT) & \
										UJA113X_LINC_LMC2_MASK)

/*!
 * @brief LIN control register, LIN1 slope control macros.
 */
#define UJA113X_LINC_LSC1_MASK          (0x0CU)
#define UJA113X_LINC_LSC1_SHIFT         (2U)
#define UJA113X_LINC_LSC1_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_LINC_LSC1_SHIFT) & \
										UJA113X_LINC_LSC1_MASK)

/*!
 * @brief LIN control register, LIN1 transceiver operating mode selection macros.
 */
#define UJA113X_LINC_LMC1_MASK          (0x03U)
#define UJA113X_LINC_LMC1_SHIFT         (0U)
#define UJA113X_LINC_LMC1_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_LINC_LMC1_SHIFT) & \
										UJA113X_LINC_LMC1_MASK)

/*!
 * @brief LIN control register macros.
 */
#define UJA113X_LINC_MASK               (UJA113X_LINC_LSC2_MASK | UJA113X_LINC_LMC2_MASK | \
										UJA113X_LINC_LSC1_MASK | UJA113X_LINC_LMC1_MASK)
#define UJA113X_LINC_SHIFT              (0U)
#define UJA113X_LINC_F(x)               ((uint8_t)((uint8_t)(x) << UJA113X_LINC_SHIFT) & \
										UJA113X_LINC_MASK)

/*******************************************************************************
 * Transceiver status register (0x22)
 ******************************************************************************/
/*!
 * @brief Transceiver status register, CAN transmitter status macros.
 */
#define UJA113X_TS_CTS_MASK          	(0x80U)
#define UJA113X_TS_CTS_SHIFT        	(7U)
#define UJA113X_TS_CTS_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_TS_CTS_SHIFT) & \
										UJA113X_TS_CTS_MASK)

/*!
 * @brief Transceiver status register, CAN partial networking error macros.
 */
#define UJA113X_TS_CPNERR_MASK       	(0x40U)
#define UJA113X_TS_CPNERR_SHIFT      	(6U)
#define UJA113X_TS_CPNERR_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_TS_CPNERR_SHIFT) & \
										UJA113X_TRANS_STAT_CPNERR_MASK)

/*!
 * @brief Transceiver status register, CAN partial networking status macros.
 */
#define UJA113X_TS_CPNS_MASK         	(0x20U)
#define UJA113X_TS_CPNS_SHIFT        	(5U)
#define UJA113X_TS_CPNS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TS_CPNS_SHIFT) & \
										UJA113X_TS_CPNS_MASK)

/*!
 * @brief  Transceiver status register, CAN oscillator status macros.
 */
#define UJA113X_TS_COSCS_MASK        	(0x10U)
#define UJA113X_TS_COSCS_SHIFT       	(4U)
#define UJA113X_TS_COSCS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_TS_COSCS_SHIFT) & \
										UJA113X_TS_COSCS_MASK)

/*!
 * @brief  Transceiver status register, CAN-bus silence status macros.
 */
#define UJA113X_TS_CBSS_MASK         	(0x08U)
#define UJA113X_TS_CBSS_SHIFT        	(3U)
#define UJA113X_TS_CBSS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TS_CBSS_SHIFT) & \
										UJA113X_TS_CBSS_MASK)

/*!
 * @brief  Transceiver status register, LIN supply status macros.
 */
#define UJA113X_TS_VLINS_MASK         	(0x04U)
#define UJA113X_TS_VLINS_SHIFT        	(2U)
#define UJA113X_TS_VLINS_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TS_VLINS_SHIFT) & \
										UJA113X_TS_VLINS_MASK)

/*!
 * @brief  Transceiver status register, CAN supply status macros.
 */
#define UJA113X_TS_VCS_MASK          	(0x02U)
#define UJA113X_TS_VCS_SHIFT         	(1U)
#define UJA113X_TS_VCS_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_TS_VCS_SHIFT) & \
										UJA113X_TS_VCS_MASK)

/*!
 * @brief  Transceiver status register, CAN failure status macros.
 */
#define UJA113X_TS_CFS_MASK          	(0x01U)
#define UJA113X_TS_CFS_SHIFT         	(0U)
#define UJA113X_TS_CFS_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_TS_CFS_SHIFT) & \
										UJA113X_TS_CFS_MASK)

/*!
 * @brief  Transceiver status register macros.
 */
#define UJA113X_TS_MASK              	(UJA113X_TS_CTS_MASK | UJA113X_TS_CPNERR_MASK | \
		                                UJA113X_TS_CPNS_MASK | UJA113X_TS_COSCS_MASK | \
										UJA113X_TS_CBSS_MASK | UJA113X_TS_VLINS_MASK | \
										UJA113X_TS_VCS_MASK | UJA113X_TS_CFS_MASK)
#define UJA113X_TS_SHIFT             	(0U)
#define UJA113X_TS_F(x)              	((uint8_t)((uint8_t)(x) << UJA113X_TS_SHIFT) & \
										UJA113X_TS_MASK)

/*******************************************************************************
 * Transceiver interrupt enable register (0x23)
 ******************************************************************************/
/*!
 * @brief Transceiver interrupt enable register, CAN-bus silence interrupt
 * enable macros.
 */
#define UJA113X_TIE_CBSIE_MASK         	(0x10U)
#define UJA113X_TIE_CBSIE_SHIFT        	(4U)
#define UJA113X_TIE_CBSIE_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TIE_CBSIE_SHIFT) & \
										UJA113X_TIE_CBSE_MASK)

/*!
 * @brief Transceiver interrupt enable register, LIN2 wake-up interrupt
 * enable macros.
 */
#define UJA113X_TIE_LWI2E_MASK         	(0x08U)
#define UJA113X_TIE_LWI2E_SHIFT        	(3U)
#define UJA113X_TIE_LWI2E_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TIE_LWI2E_SHIFT) & \
										UJA113X_TIE_LWI2E_MASK)

/*!
 * @brief Transceiver interrupt enable register, LIN1 wake-up interrupt
 * enable macros.
 */
#define UJA113X_TIE_LWI1E_MASK         	(0x04U)
#define UJA113X_TIE_LWI1E_SHIFT        	(2U)
#define UJA113X_TIE_LWI1E_F(x)         	((uint8_t)((uint8_t)(x) << UJA113X_TIE_LWI1E_SHIFT) & \
										UJA113X_TIE_LWI1E_MASK)

/*!
 * @brief Transceiver interrupt enable register, CAN failure enable interrupt
 * macros.
 */
#define UJA113X_TIE_CFIE_MASK          	(0x02U)
#define UJA113X_TIE_CFIE_SHIFT         	(1U)
#define UJA113X_TIE_CFIE_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_TIE_CFIE_SHIFT) & \
										UJA113X_TIE_CFIE_MASK)

/*!
 * @brief Transceiver interrupt enable register, CAN wake-up interrupt
 * enable macros.
 */
#define UJA113X_TIE_CWIE_MASK          	(0x01U)
#define UJA113X_TIE_CWIE_SHIFT         	(0U)
#define UJA113X_TIE_CWIE_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_TIE_CWIE_SHIFT) & \
										UJA113X_TIE_CWIE_MASK)

/*!
 * @brief Transceiver interrupt enable register macros.
 */
#define UJA113X_TIE_MASK              (UJA113X_TIE_CBSE_MASK | UJA113X_TIE_CFE_MASK \
                                         | UJA113X_TIE_CWE_MASK)
#define UJA113X_TIE_SHIFT             (0U)
#define UJA113X_TIE_F(x)              ((uint8_t)((uint8_t)(x)<<UJA113X_TIE_SHIFT)&UJA113X_TIE_MASK)

/*******************************************************************************
 * Data rate register (0x26)
 ******************************************************************************/
/*!
 * @brief Data rate register, CAN data rate selection macros.
 */
#define UJA113X_DR_CDR_MASK            	(0x07U)
#define UJA113X_DR_CDR_SHIFT           	(0U)
#define UJA113X_DR_CDR_F(x)            	((uint8_t)((uint8_t)(x) << UJA113X_DR_CDR_SHIFT) & \
										UJA113X_DR_CDR_MASK)

/*!
 * @brief Data rate register macros.
 */
#define UJA113X_DR_MASK                	(UJA113X_DR_CDR_MASK)
#define UJA113X_DR_SHIFT               	(0U)
#define UJA113X_DR_F(x)                	((uint8_t)((uint8_t)(x) << UJA113X_DR_SHIFT) & \
										UJA113X_DR_SHIFT)

/*******************************************************************************
 * ID7:ID0 register (0x27)
 ******************************************************************************/
/*!
 * @brief bits ID7 to ID00 of the extended frame format macros.
 */
#define UJA113X_IDENTIF_0700_MASK       (0xFFU)
#define UJA113X_IDENTIF_0700_SHIFT      (0U)
#define UJA113X_IDENTIF_0700_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_IDENTIF_0700_SHIFT) & \
										UJA113X_IDENTIF_0700_MASK)

/*******************************************************************************
 * ID15:ID8 register (0x28)
 ******************************************************************************/
/*!
 * @brief bits ID15 to ID8 of the extended frame format macros.
 */
#define UJA113X_IDENTIF_1508_MASK       (0xFFU)
#define UJA113X_IDENTIF_1508_SHIFT      (0U)
#define UJA113X_IDENTIF_1508_F(x)       ((uint8_t)((uint8_t)(x)<<UJA113X_IDENTIF_1508_SHIFT)&UJA113X_IDENTIF_1508_MASK)

/*******************************************************************************
 * ID23:ID16 register (0x29)
 ******************************************************************************/
/*!
 * @brief Bits ID23 to ID18 of the extended frame format macros.
 */
#define UJA113X_IDENTIF_2318_MASK       (0xFCU)
#define UJA113X_IDENTIF_2318_SHIFT      (2U)
#define UJA113X_IDENTIF_2318_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_IDENTIF_2318_SHIFT) & \
										UJA113X_IDENTIF_2318_MASK)

/*!
 * @brief Bits ID17 to ID16 of the extended frame format macros.
 */
#define UJA113X_IDENTIF_1716_MASK       (0x03U)
#define UJA113X_IDENTIF_1716_SHIFT      (0U)
#define UJA113X_IDENTIF_1716_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_IDENTIF_1716_SHIFT) & \
										UJA113X_IDENTIF_1716_MASK)

/*******************************************************************************
 * ID28:ID24 register (0x2A)
 ******************************************************************************/
/*!
 * @brief Bits ID28 to ID24 of the extended frame format macros.
 */
#define UJA113X_IDENTIF_2824_MASK       (0x1FU)
#define UJA113X_IDENTIF_2824_SHIFT      (0U)
#define UJA113X_IDENTIF_2824_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_IDENTIF_2824_SHIFT) & \
										UJA113X_IDENTIF_2824_MASK)

/*******************************************************************************
 * IDM7:IDM0 register (0x2B)
 ******************************************************************************/
/*!
 * @brief ID mask bits 7 to 0 of extended frame format.
 */
#define UJA113X_MASK_0700_MASK          (0xFFU)
#define UJA113X_MASK_0700_SHIFT         (0U)
#define UJA113X_MASK_0700_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_MASK_0700_SHIFT) & \
										UJA113X_MASK_0700_MASK)

/*******************************************************************************
 * IDM15:IDM8 register (0x2C)
 ******************************************************************************/
/*!
 * @brief ID mask bits 15 to 8 of extended frame format.
 */
#define UJA113X_MASK_1508_MASK          (0xFFU)
#define UJA113X_MASK_1508_SHIFT         (0U)
#define UJA113X_MASK_1508_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_MASK_1508_SHIFT) & \
										UJA113X_MASK_1508_MASK)

/*******************************************************************************
 * IDM23:IDM16 register (0x2D)
 ******************************************************************************/
/*!
 * @brief ID mask bits 23 to 18 of extended frame format.
 */
#define UJA113X_MASK_2318_MASK          (0xFCU)
#define UJA113X_MASK_2318_SHIFT         (2U)
#define UJA113X_MASK_2318_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_MASK_2318_SHIFT) & \
										UJA113X_MASK_2318_MASK)

/*!
 * @brief ID mask bits 17 to 16 of extended frame format.
 */
#define UJA113X_MASK_1716_MASK          (0x03U)
#define UJA113X_MASK_1716_SHIFT         (2U)
#define UJA113X_MASK_1716_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_MASK_1716_SHIFT) & \
										UJA113X_MASK_1716_MASK)

/*******************************************************************************
 * IDM28:IDM24 register (0x2E)
 ******************************************************************************/
/*!
 * @brief ID mask bits 28 to 24 of extended frame format.
 */
#define UJA113X_MASK_2824_MASK          (0x1FU)
#define UJA113X_MASK_2824_SHIFT         (0U)
#define UJA113X_MASK_2824_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_MASK_2824_SHIFT) & \
										UJA113X_MASK_2824_MASK)

/*******************************************************************************
 * Frame control register (0x2F)
 ******************************************************************************/
/*!
 * @brief Frame control register, identifier format macros.
 */
#define UJA113X_FC_IDE_MASK           	(0x80U)
#define UJA113X_FC_IDE_SHIFT          	(7U)
#define UJA113X_FC_IDE_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_FC_IDE_SHIFT) & \
										UJA113X_FC_IDE_MASK)

/*!
 * @brief Frame control register, partial networking data mask macros.
 */
#define UJA113X_FC_PNDM_MASK          	(0x40U)
#define UJA113X_FC_PNDM_SHIFT         	(6U)
#define UJA113X_FC_PNDM_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_FC_PNDM_SHIFT) & \
										UJA113X_FC_PNDM_MASK)

/*!
 * @brief Frame control register, number of data bytes expected in a CAN frame macros.
 */
#define UJA113X_FC_DLC_MASK           	(0x0FU)
#define UJA113X_FC_DLC_SHIFT          	(0U)
#define UJA113X_FC_DLC_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_FC_DLC_SHIFT) & \
										UJA113X_FC_DLC_MASK)

/*!
 * @brief  Frame control register macros.
 */
#define UJA113X_FC_MASK               	(UJA113X_FC_IDE_MASK | UJA113X_FC_PNDM_MASK | \
		                                UJA113X_FC_DLC_MASK)
#define UJA113X_FC_SHIFT              	(0U)
#define UJA113X_FC_F(x)               	((uint8_t)((uint8_t)(x) << UJA113X_FC_SHIFT) & \
										UJA113X_FC_MASK)

/*******************************************************************************
 * HVIO bank 0 and 1 control registers (0x30 - 0x4F)
 ******************************************************************************/

/*******************************************************************************
 * HVIOn control registers (0x30 - 0x33, 0x40 - 0x43)
 ******************************************************************************/
/*!
 * @brief HVIOn control register, HVIOn shutdown control macros.
 */
#define UJA113X_HVIONC_IONSC_MASK       (0xC0U)
#define UJA113X_HVIONC_IONSC_SHIFT      (6U)
#define UJA113X_HVIONC_IONSC_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_HVIONC_IONSC_SHIFT) & \
										UJA113X_HVIONC_IONSC_MASK)

/*!
 * @brief HVIOn control register, HVIOn activation control macros.
 */
#define UJA113X_HVIONC_IONAC_MASK       (0x38U)
#define UJA113X_HVIONC_IONAC_SHIFT      (3U)
#define UJA113X_HVIONC_IONAC_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_HVIONC_IONAC_SHIFT) & \
										UJA113X_HVIONC_IONAC_MASK)

/*!
 * @brief HVIOn control register, HVIOn configuration control macros.
 */
#define UJA113X_HVIONC_IONCC_MASK       (0x07U)
#define UJA113X_HVIONC_IONCC_SHIFT      (0U)
#define UJA113X_HVIONC_IONCC_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_HVIONC_IONCC_SHIFT) & \
										UJA113X_HVIONC_IONCC_MASK)

/*!
 * @brief HVIOn control register.
 */
#define UJA113X_HVIONC_MASK             (UJA113X_HVIONC_IONSC_MASK | UJA113X_HVIONC_IONAC_MASK | \
		                                UJA113X_HVIONC_IONCC_MASK)
#define UJA113X_HVIONC_SHIFT            (0U)
#define UJA113X_HVIONC_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_HVIONC_SHIFT) & \
										UJA113X_HVIONC_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) wake-up threshold control
 * register (0x34, 0x44)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 wake-up threshold control register, wake-up threshold
 * configuration macros.
 */
#define UJA113X_BNWTC_BNWTC_MASK       	(0x01U)
#define UJA113X_BNWTC_BNWTC_SHIFT      	(0U)
#define UJA113X_BNWTC_BNWTC_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_BNWTC_BNWTC_MASK) & \
										UJA113X_BNWTC_BNWTC_SHIFT)

/*!
 * @brief Bank 0 and 1 wake-up threshold control register macros.
 */
#define UJA113X_BNWTC_MASK             	(UJA113X_BNWTC_BNWTC_MASK)
#define UJA113X_BNWTC_SHIFT            	(0U)
#define UJA113X_BNWTC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNWTC_SHIFT) & \
										UJA113X_BNWTC_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) wake-up status register
 * (0x35, 0x45)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 wake-up status register, status of input voltage on
 * HVIO4/HVIO8 macros.
 */
#define UJA113X_BNWS_IO48WLS_MASK       (0x08U)
#define UJA113X_BNWS_IO48WLS_SHIFT      (3U)
#define UJA113X_BNWS_IO48WLS_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_BNWS_IO48WLS_MASK) & \
										UJA113X_BNWS_IO48WLS_SHIFT)

/*!
 * @brief Bank 0 and 1 wake-up status register, status of input voltage on
 * HVIO3/HVIO7 macros.
 */
#define UJA113X_BNWS_IO37WLS_MASK       (0x04U)
#define UJA113X_BNWS_IO37WLS_SHIFT      (2U)
#define UJA113X_BNWS_IO37WLS_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_BNWS_IO37WLS_MASK) & \
										UJA113X_BNWS_IO37WLS_SHIFT)

/*!
 * @brief Bank 0 and 1 wake-up status register, status of input voltage on
 * HVIO2/HVIO6 macros.
 */
#define UJA113X_BNWS_IO26WLS_MASK       (0x02U)
#define UJA113X_BNWS_IO26WLS_SHIFT      (1U)
#define UJA113X_BNWS_IO26WLS_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_BNWS_IO26WLS_MASK) & \
										UJA113X_BNWS_IO26WLS_SHIFT)

/*!
 * @brief Bank 0 and 1 wake-up status register, status of input voltage on
 * HVIO1/HVIO5 macros.
 */
#define UJA113X_BNWS_IO15WLS_MASK       (0x01U)
#define UJA113X_BNWS_IO15WLS_SHIFT      (0U)
#define UJA113X_BNWS_IO15WLS_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_BNWS_IO15WLS_MASK) & \
										UJA113X_BNWS_IO15WLS_SHIFT)

/*!
 * @brief Bank 0 and 1 wake-up status register macros.
 */
#define UJA113X_BNWS_MASK             	(UJA113X_BNWS_IO48WLS_MASK | UJA113X_BNWS_IO37WLS_MASK | \
										UJA113X_BNWS_IO26WLS_MASK | UJA113X_BNWS_IO15WLS_MASK)
#define UJA113X_BNWS_SHIFT            	(0U)
#define UJA113X_BNWS_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNWS_SHIFT) & \
										UJA113X_BNWS_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) driver status register
 * (0x36, 0x46)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 driver status register, HVIO4/HVIO8 driver status macros.
 */
#define UJA113X_BNDS_IO48DS_MASK       	(0xC0U)
#define UJA113X_BNDS_IO48DS_SHIFT      	(6U)
#define UJA113X_BNDS_IO48DS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_BNDS_IO48DS_MASK) & \
										UJA113X_BNDS_IO48DS_SHIFT)

/*!
 * @brief Bank 0 and 1 driver status register, HVIO3/HVIO7 driver status macros.
 */
#define UJA113X_BNDS_IO37DS_MASK       	(0x30U)
#define UJA113X_BNDS_IO37DS_SHIFT      	(4U)
#define UJA113X_BNDS_IO37DS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_BNDS_IO37DS_MASK) & \
										UJA113X_BNDS_IO37DS_SHIFT)

/*!
 * @brief Bank 0 and 1 driver status register, HVIO2/HVIO6 driver status macros.
 */
#define UJA113X_BNDS_IO26DS_MASK       	(0x0CU)
#define UJA113X_BNDS_IO26DS_SHIFT      	(2U)
#define UJA113X_BNDS_IO26DS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_BNDS_IO26DS_MASK) & \
										UJA113X_BNDS_IO26DS_SHIFT)

/*!
 * @brief Bank 0 and 1 driver status register, HVIO1/HVIO5 driver status macros.
 */
#define UJA113X_BNDS_IO15DS_MASK       	(0x03U)
#define UJA113X_BNDS_IO15DS_SHIFT      	(0U)
#define UJA113X_BNDS_IO15DS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_BNDS_IO15DS_MASK) & \
										UJA113X_BNDS_IO15DS_SHIFT)

/*!
 * @brief Bank 0 and 1 driver status register macros.
 */
#define UJA113X_BNDS_MASK             	(UJA113X_BNDS_IO48DS_MASK | UJA113X_BNDS_IO37DS_MASK | \
										UJA113X_BNDS_IO26DS_MASK | UJA113X_BNDS_IO15DS_MASK)
#define UJA113X_BNDS_SHIFT            	(0U)
#define UJA113X_BNDS_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNDS_SHIFT) & \
										UJA113X_BNDS_MASK)


/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) wake-up interrupt enable
 * register (0x37, 0x47)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO4/HVIO8 falling edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO48FEIE_MASK    	(0x80U)
#define UJA113X_BNWIE_IO48FEIE_SHIFT   	(7U)
#define UJA113X_BNWIE_IO48FEIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO48FEIE_SHIFT) & \
										UJA113X_BNWIE_IO48FEIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO4/HVIO8 rising edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO48REIE_MASK    	(0x40U)
#define UJA113X_BNWIE_IO48REIE_SHIFT   	(6U)
#define UJA113X_BNWIE_IO48REIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO48REIE_SHIFT) & \
										UJA113X_BNWIE_IO48REIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO3/HVIO7 falling edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO37FEIE_MASK    	(0x20U)
#define UJA113X_BNWIE_IO37FEIE_SHIFT   	(5U)
#define UJA113X_BNWIE_IO37FEIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO37FEIE_SHIFT) & \
										UJA113X_BNWIE_IO37FEIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO3/HVIO7 rising edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO37REIE_MASK    	(0x10U)
#define UJA113X_BNWIE_IO37REIE_SHIFT   	(4U)
#define UJA113X_BNWIE_IO37REIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO37REIE_SHIFT) & \
										UJA113X_BNWIE_IO37REIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO2/HVIO6 falling edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO26FEIE_MASK    	(0x08U)
#define UJA113X_BNWIE_IO26FEIE_SHIFT   	(3U)
#define UJA113X_BNWIE_IO26FEIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO26FEIE_SHIFT) & \
										UJA113X_BNWIE_IO26FEIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO2/HVIO6 rising edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO26REIE_MASK    	(0x04U)
#define UJA113X_BNWIE_IO26REIE_SHIFT   	(2U)
#define UJA113X_BNWIE_IO26REIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO26REIE_SHIFT) & \
										UJA113X_BNWIE_IO26REIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO1/HVIO5 falling edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO15FEIE_MASK    	(0x02U)
#define UJA113X_BNWIE_IO15FEIE_SHIFT   	(1U)
#define UJA113X_BNWIE_IO15FEIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO15FEIE_SHIFT) & \
										UJA113X_BNWIE_IO15FEIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register,
 * HVIO1/HVIO5 rising edge interrupt enable macros.
 */
#define UJA113X_BNWIE_IO15REIE_MASK    	(0x01U)
#define UJA113X_BNWIE_IO15REIE_SHIFT   	(0U)
#define UJA113X_BNWIE_IO15REIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_IO15REIE_SHIFT) & \
										UJA113X_BNWIE_IO15REIE_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt enable register macros.
 */
#define UJA113X_BNWIE_MASK             	(UJA113X_BNWIE_IO48FEIE_MASK | UJA113X_BNWIE_IO48REIE_MASK | \
										UJA113X_BNWIE_IO37FEIE_MASK | UJA113X_BNWIE_IO37REIE_MASK | \
										UJA113X_BNWIE_IO26FEIE_MASK | UJA113X_BNWIE_IO26REIE_MASK | \
										UJA113X_BNWIE_IO15FEIE_MASK | UJA113X_BNWIE_IO15REIE_MASK)
#define UJA113X_BNWIE_SHIFT            	(0U)
#define UJA113X_BNWIE_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNWIE_SHIFT) & \
										UJA113X_BNWIE_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) fail interrupt enable
 * register (0x38, 0x48)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO4/HVIO8 short circuit interrupt enable macros.
 */
#define UJA113X_BNFIE_IO48SCIE_MASK    	(0x80U)
#define UJA113X_BNFIE_IO48SCIE_SHIFT   	(7U)
#define UJA113X_BNFIE_IO48SCIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO48SCIE_SHIFT) & \
										UJA113X_BNFIE_IO48SCIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO4/HVIO8 open load interrupt enable macros.
 */
#define UJA113X_BNFIE_IO48OLIE_MASK    	(0x40U)
#define UJA113X_BNFIE_IO48OLIE_SHIFT   	(6U)
#define UJA113X_BNFIE_IO48OLIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO48OLIE_SHIFT) & \
										UJA113X_BNFIE_IO48OLIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO3/HVIO7 short circuit interrupt enable macros.
 */
#define UJA113X_BNFIE_IO37SCIE_MASK    	(0x20U)
#define UJA113X_BNFIE_IO37SCIE_SHIFT   	(5U)
#define UJA113X_BNFIE_IO37SCIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO37SCIE_SHIFT) & \
										UJA113X_BNFIE_IO37SCIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO3/HVIO7 open load interrupt enable macros.
 */
#define UJA113X_BNFIE_IO37OLIE_MASK    	(0x10U)
#define UJA113X_BNFIE_IO37OLIE_SHIFT   	(4U)
#define UJA113X_BNFIE_IO37OLIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO37OLIE_SHIFT) & \
										UJA113X_BNFIE_IO37OLIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO2/HVIO6 short circuit interrupt enable macros.
 */
#define UJA113X_BNFIE_IO26SCIE_MASK    	(0x08U)
#define UJA113X_BNFIE_IO26SCIE_SHIFT   	(3U)
#define UJA113X_BNFIE_IO26SCIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO26SCIE_SHIFT) & \
										UJA113X_BNFIE_IO26SCIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO2/HVIO6 open load interrupt enable macros.
 */
#define UJA113X_BNFIE_IO26OLIE_MASK    	(0x04U)
#define UJA113X_BNFIE_IO26OLIE_SHIFT   	(2U)
#define UJA113X_BNFIE_IO26OLIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO26OLIE_SHIFT) & \
										UJA113X_BNFIE_IO26OLIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO1/HVIO5 short circuit interrupt enable macros.
 */
#define UJA113X_BNFIE_IO15SCIE_MASK    	(0x02U)
#define UJA113X_BNFIE_IO15SCIE_SHIFT   	(1U)
#define UJA113X_BNFIE_IO15SCIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO15SCIE_SHIFT) & \
										UJA113X_BNFIE_IO15SCIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register,
 * HVIO1/HVIO5 open load interrupt enable macros.
 */
#define UJA113X_BNFIE_IO15OLIE_MASK    	(0x01U)
#define UJA113X_BNFIE_IO15OLIE_SHIFT   	(0U)
#define UJA113X_BNFIE_IO15OLIE_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_IO15OLIE_SHIFT) & \
										UJA113X_BNFIE_IO15OLIE_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt enable register macros.
 */
#define UJA113X_BNFIE_MASK             	(UJA113X_BNFIE_IO48SCIE_MASK | UJA113X_BNFIE_IO48OLIE_MASK | \
										UJA113X_BNFIE_IO37SCIE_MASK | UJA113X_BNFIE_IO37OLIE_MASK | \
										UJA113X_BNFIE_IO26SCIE_MASK | UJA113X_BNFIE_IO26OLIE_MASK | \
										UJA113X_BNFIE_IO15SCIE_MASK | UJA113X_BNFIE_IO15OLIE_MASK)
#define UJA113X_BNFIE_SHIFT            	(0U)
#define UJA113X_BNFIE_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNFIE_SHIFT) & \
										UJA113X_BNFIE_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) short-circuit detection
 * threshold control register (0x39, 0x49)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 driver short-circuit detection threshold control register,
 * HVIO4/HVIO8 short-circuit detection threshold macros.
 */
#define UJA113X_BNSCTC_IO48SCTC_MASK    (0xC0U)
#define UJA113X_BNSCTC_IO48SCTC_SHIFT   (6U)
#define UJA113X_BNSCTC_IO48SCTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNSCTC_IO48SCTC_SHIFT) & \
										UJA113X_BNSCTC_IO48SCTC_MASK)

/*!
 * @brief Bank 0 and 1 driver short-circuit detection threshold control register,
 * HVIO3/HVIO7 short-circuit detection threshold macros.
 */
#define UJA113X_BNSCTC_IO37SCTC_MASK    (0x30U)
#define UJA113X_BNSCTC_IO37SCTC_SHIFT   (4U)
#define UJA113X_BNSCTC_IO37SCTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNSCTC_IO37SCTC_SHIFT) & \
										UJA113X_BNSCTC_IO37SCTC_MASK)

/*!
 * @brief Bank 0 and 1 driver short-circuit detection threshold control register,
 * HVIO2/HVIO6 short-circuit detection threshold macros.
 */
#define UJA113X_BNSCTC_IO26SCTC_MASK    (0x0CU)
#define UJA113X_BNSCTC_IO26SCTC_SHIFT   (2U)
#define UJA113X_BNSCTC_IO26SCTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNSCTC_IO26SCTC_SHIFT) & \
										UJA113X_BNSCTC_IO26SCTC_MASK)

/*!
 * @brief Bank 0 and 1 driver short-circuit detection threshold control register,
 * HVIO1/HVIO5 short-circuit detection threshold macros.
 */
#define UJA113X_BNSCTC_IO15SCTC_MASK    (0x03U)
#define UJA113X_BNSCTC_IO15SCTC_SHIFT   (0U)
#define UJA113X_BNSCTC_IO15SCTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNSCTC_IO15SCTC_SHIFT) & \
										UJA113X_BNSCTC_IO15SCTC_MASK)

/*!
 * @brief Bank 0 and 1 driver short-circuit detection threshold control register
 * macros.
 */
#define UJA113X_BNSCTC_MASK             (UJA113X_BNSCTC_IO48SCTC_MASK | UJA113X_BNSCTC_IO37SCTC_MASK | \
										UJA113X_BNSCTC_IO26SCTC_MASK | UJA113X_BNSCTC_IO15SCTC_MASK)
#define UJA113X_BNSCTC_SHIFT            (0U)
#define UJA113X_BNSCTC_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_BNSCTC_SHIFT) & \
										UJA113X_BNSCTC_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) open-load detection
 * threshold control register (0x3A, 0x4A)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 driver open-load detection threshold control register,
 * HVIO4/HVIO8 open-load detection threshold macros.
 */
#define UJA113X_BNOLTC_IO48OLTC_MASK    (0xC0U)
#define UJA113X_BNOLTC_IO48OLTC_SHIFT   (6U)
#define UJA113X_BNOLTC_IO48OLTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNOLTC_IO48OLTC_SHIFT) & \
										UJA113X_BNOLTC_IO48OLTC_MASK)

/*!
 * @brief Bank 0 and 1 driver open-load detection threshold control register,
 * HVIO3/HVIO7 open-load detection threshold macros.
 */
#define UJA113X_BNOLTC_IO37OLTC_MASK    (0x30U)
#define UJA113X_BNOLTC_IO37OLTC_SHIFT   (4U)
#define UJA113X_BNOLTC_IO37OLTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNOLTC_IO37OLTC_SHIFT) & \
										UJA113X_BNOLTC_IO37OLTC_MASK)

/*!
 * @brief Bank 0 and 1 driver open-load detection threshold control register,
 * HVIO2/HVIO6 open-load detection threshold macros.
 */
#define UJA113X_BNOLTC_IO26OLTC_MASK    (0x0CU)
#define UJA113X_BNOLTC_IO26OLTC_SHIFT   (2U)
#define UJA113X_BNOLTC_IO26OLTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNOLTC_IO26OLTC_SHIFT) & \
										UJA113X_BNOLTC_IO26OLTC_MASK)

/*!
 * @brief Bank 0 and 1 driver open-load detection threshold control register,
 * HVIO1/HVIO5 open-load detection threshold macros.
 */
#define UJA113X_BNOLTC_IO15OLTC_MASK    (0x03U)
#define UJA113X_BNOLTC_IO15OLTC_SHIFT   (0U)
#define UJA113X_BNOLTC_IO15OLTC_F(x)    ((uint8_t)((uint8_t)(x) << UJA113X_BNOLTC_IO15OLTC_SHIFT) & \
										UJA113X_BNOLTC_IO15OLTC_MASK)

/*!
 * @brief Bank 0 and 1 driver open-load detection threshold control register
 * macros.
 */
#define UJA113X_BNOLTC_MASK             (UJA113X_BNOLTC_IO48OLTC_MASK | UJA113X_BNOLTC_IO37OLTC_MASK | \
										UJA113X_BNOLTC_IO26OLTC_MASK | UJA113X_BNOLTC_IO15OLTC_MASK)
#define UJA113X_BNOLTC_SHIFT            (0U)
#define UJA113X_BNOLTC_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_BNOLTC_SHIFT) & \
										UJA113X_BNOLTC_MASK)

/*******************************************************************************
 * Timer control registers (0x50 - 0x5F)
 ******************************************************************************/
/*******************************************************************************
 * Timer 1, 2, 3 and 4 control registers (0x50, 0x52, 0x54, 0x56)
 ******************************************************************************/
/*!
 * @brief Timer 1, 2, 3 and 4 control registers, timer 1, 2, 3 and 4 period macros.
 */
#define UJA113X_TNC_TNPC_MASK     		(0x3CU)
#define UJA113X_TNC_TNPC_SHIFT    		(2U)
#define UJA113X_TNC_TNPC_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_TNC_TNPC_SHIFT) & \
										UJA113X_TNC_TNPC_MASK)

/*!
 * @brief Timer 1 control registers, timer 1 mode macros.
 */
#define UJA113X_T1C_TNMC_MASK     		(0x01U)
#define UJA113X_T1C_TNMC_SHIFT    		(0U)
#define UJA113X_T1C_TNMC_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_T1C_TNMC_SHIFT) & \
										UJA113X_T1C_TNMC_MASK)

/*!
 * @brief Timer 1 control registers macros.
 */
#define UJA113X_T1C_MASK             	(UJA113X_TNC_TNPC_MASK | UJA113X_T1C_TNMC_MASK)
#define UJA113X_T1C_SHIFT            	(0U)
#define UJA113X_T1C_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_T1C_SHIFT) & \
										UJA113X_T1C_MASK)

/*!
 * @brief Timer 2, 3 and 4 control registers, timer 2, 3 and 4 mode macros.
 */
#define UJA113X_TNC_TNMC_MASK         (0x03U)
#define UJA113X_TNC_TNMC_SHIFT        (0U)
#define UJA113X_TNC_TNMC_F(x)         ((uint8_t)((uint8_t)(x) << UJA113X_TNC_TNMC_SHIFT) & \
                    UJA113X_TNC_TNMC_MASK)

/*!
 * @brief Timer 2, 3 and 4 control registers macros.
 */
#define UJA113X_TNC_MASK              (UJA113X_TNC_TNPC_MASK | UJA113X_TNC_TNMC_MASK)
#define UJA113X_TNC_SHIFT             (0U)
#define UJA113X_TNC_F(x)              ((uint8_t)((uint8_t)(x) << UJA113X_TNC_SHIFT) & \
                    UJA113X_TNC_MASK)

/*******************************************************************************
 * Timer 1, 2, 3 and 4 duty cycle control registers (0x51, 0x53, 0x55, 0x57)
 ******************************************************************************/
/*!
 * @brief Timer 1, 2, 3 and 4 duty cycle control registers, timer 1, 2, 3
 * and 4 period macros.
 */
#define UJA113X_TNDCC_TNDCC_MASK    	(0xFFU)
#define UJA113X_TNDCC_TNDCC_SHIFT    	(0U)
#define UJA113X_TNDCC_TNDCC_F(x)     	((uint8_t)((uint8_t)(x) << UJA113X_TNDCC_TNDCC_SHIFT) & \
										UJA113X_TNDCC_TNDCC_MASK)

/*!
 * @brief Timer 1, 2, 3 and 4 duty cycle control registers macros.
 */
#define UJA113X_TNDCC_MASK             	(UJA113X_TNDCC_TNDCC_MASK)
#define UJA113X_TNDCC_SHIFT            	(0U)
#define UJA113X_TNDCC_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_TNDCC_SHIFT) & \
										UJA113X_TNDCC_MASK)

/*******************************************************************************
 * Interrupt status registers (0x60 - 0x6F)
 ******************************************************************************/

/*******************************************************************************
 * Global interrupt status register (0x60)
 ******************************************************************************/
/*!
 * @brief Global interrupt status register, an interrupt in the bank 1 fail
 * interrupt status register macros.
 */
#define UJA113X_GIS_B1FIS_MASK        	(0x40U)
#define UJA113X_GIS_B1FIS_SHIFT       	(6U)
#define UJA113X_GIS_B1FIS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_GIS_B1FIS_SHIFT) & \
										UJA113X_GIS_B1FIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the bank 1 wake-up
 * interrupt status register macros.
 */
#define UJA113X_GIS_B1WIS_MASK        	(0x20U)
#define UJA113X_GIS_B1WIS_SHIFT       	(5U)
#define UJA113X_GIS_B1WIS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_GIS_B1WIS_SHIFT) & \
										UJA113X_GIS_B1WIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the bank 0 fail
 * interrupt status register macros.
 */
#define UJA113X_GIS_B0FIS_MASK        	(0x10U)
#define UJA113X_GIS_B0FIS_SHIFT       	(4U)
#define UJA113X_GIS_B0FIS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_GIS_B0FIS_SHIFT) & \
										UJA113X_GIS_B0FIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the bank 0 wake-up
 * interrupt status register macros.
 */
#define UJA113X_GIS_B0WIS_MASK        	(0x08U)
#define UJA113X_GIS_B0WIS_SHIFT       	(3U)
#define UJA113X_GIS_B0WIS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_GIS_B0WIS_SHIFT) & \
										UJA113X_GIS_B0WIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the Transceiver
 * interrupt status register macros.
 */
#define UJA113X_GIS_TRXIS_MASK       	(0x04U)
#define UJA113X_GIS_TRXIS_SHIFT      	(2U)
#define UJA113X_GIS_TRXIS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_GIS_TRXIS_SHIFT) & \
										UJA113X_GIS_TRXIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the Supply interrupt
 * status register macros.
 */
#define UJA113X_GIS_SUPIS_MASK       	(0x02U)
#define UJA113X_GIS_SUPIS_SHIFT      	(1U)
#define UJA113X_GIS_SUPIS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_GIS_SUPIS_SHIFT) & \
										UJA113X_GIS_SUPIS_MASK)

/*!
 * @brief Global interrupt status register, an interrupt in the System interrupt
 * status register macros.
 */
#define UJA113X_GIS_SYSIS_MASK       	(0x01U)
#define UJA113X_GIS_SYSIS_SHIFT      	(0U)
#define UJA113X_GIS_SYSIS_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_GIS_SYSIS_SHIFT) & \
										UJA113X_GIS_SYSIS_MASK)

/*!
 * @brief Global interrupt status register macros.
 */
#define UJA113X_GIS_MASK            	(UJA113X_GIS_B1FIS_MASK | UJA113X_GIS_B1WIS_MASK \
		                                UJA113X_GIS_B0FIS_MASK | UJA113X_GIS_B0WIS_MASK | \
										UJA113X_GIS_TRXIS_MASK | UJA113X_GIS_SUPIS_MASK | \
										UJA113X_GIS_SYSIS_MASK)
#define UJA113X_GIS_SHIFT           	(0U)
#define UJA113X_GIS_F(x)            	((uint8_t)((uint8_t)(x) << UJA113X_GIS_SHIFT) & \
										UJA113X_GIS_MASK)

/*******************************************************************************
 * System interrupt status register (0x61)
 ******************************************************************************/
/*!
 * @brief System interrupt status register, overvoltage shut-down interrupt status macros.
 */
#define UJA113X_SYSIS_OVSD_MASK        	(0x10U)
#define UJA113X_SYSIS_OVSD_SHIFT       	(4U)
#define UJA113X_SYSIS_OVSD_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_OVSD_SHIFT) & \
										UJA113X_SYSIS_OVSD_MASK)

/*!
 * @brief System interrupt status register, power-on status macros.
 */
#define UJA113X_SYSIS_POS_MASK        	(0x10U)
#define UJA113X_SYSIS_POS_SHIFT       	(4U)
#define UJA113X_SYSIS_POS_F(x)        	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_POS_SHIFT) & \
										UJA113X_SYSIS_POS_MASK)

/*!
 * @brief System interrupt status register, overtemperature warning interrupt status
 * macros.
 */
#define UJA113X_SYSIS_OTWI_MASK       	(0x04U)
#define UJA113X_SYSIS_OTWI_SHIFT      	(2U)
#define UJA113X_SYSIS_OTWI_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_OTWI_SHIFT) & \
										UJA113X_SYSIS_OTWI_MASK)

/*!
 * @brief System interrupt status register, SPI failure interrupt status macros.
 */
#define UJA113X_SYSIS_SPIFI_MASK      	(0x02U)
#define UJA113X_SYSIS_SPIFI_SHIFT     	(1U)
#define UJA113X_SYSIS_SPIFI_F(x)      	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_SPIFI_SHIFT) & \
										UJA113X_SYSIS_SPIF_MASK)

/*!
 * @brief System interrupt status register, watchdog failure interrupt status macros.
 */
#define UJA113X_SYSIS_WDI_MASK       	(0x01U)
#define UJA113X_SYSIS_WDI_SHIFT      	(0U)
#define UJA113X_SYSIS_WDI_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_WDI_SHIFT) & \
										UJA113X_SYSIS_WDI_MASK)

/*!
 * @brief System interrupt status register macros.
 */
#define UJA113X_SYSIS_MASK           	(UJA113X_SYSIS_OVSD_MASK | UJA113X_SYSIS_POS_MASK | \
		                           	   	UJA113X_SYSIS_OTWI_MASK | UJA113X_SYSIS_SPIFI_MASK | \
										UJA113X_SYSIS_WDI_MASK)
#define UJA113X_SYSIS_SHIFT          	(0U)
#define UJA113X_SYSIS_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_SYSIS_SHIFT) & \
										UJA113X_SYSIS_MASK)

/*******************************************************************************
 * Supply interrupt status register (0x62)
 ******************************************************************************/
/*!
 * @brief Supply interrupt status register, SMPS status interrupt macros.
 */
#define UJA113X_SUPIS_SMPSSI_MASK       (0x20U)
#define UJA113X_SUPIS_SMPSSI_SHIFT      (5U)
#define UJA113X_SUPIS_SMPSSI_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_SMPSSI_SHIFT) & \
										UJA113X_SUPIS_SMPSSI_MASK)

/*!
 * @brief Supply interrupt status register, battery monitor overvoltage interrupt
 * macros.
 */
#define UJA113X_SUPIS_BMOI_MASK       	(0x10U)
#define UJA113X_SUPIS_BMOI_SHIFT      	(4U)
#define UJA113X_SUPIS_BMOI_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_BMOI_SHIFT) & \
										UJA113X_SUPIS_BMOI_MASK)

/*!
 * @brief Supply interrupt status register, battery monitor undervoltage interrupt
 * macros.
 */
#define UJA113X_SUPIS_BMUI_MASK       	(0x08U)
#define UJA113X_SUPIS_BMUI_SHIFT      	(3U)
#define UJA113X_SUPIS_BMUI_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_BMUI_SHIFT) & \
										UJA113X_SUPIS_BMUI_MASK)

/*!
 * @brief Supply interrupt status register, VEXT overvoltage interrupt status
 * macros.
 */
#define UJA113X_SUPIS_VEXTOI_MASK       (0x04U)
#define UJA113X_SUPIS_VEXTOI_SHIFT      (2U)
#define UJA113X_SUPIS_VEXTOI_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_VEXTOI_SHIFT) & \
										UJA113X_SUPIS_VEXTOI_MASK)

/*!
 * @brief Supply interrupt status register, VEXT undervoltage interrupt status
 * macros.
 */
#define UJA113X_SUPIS_VEXTUI_MASK       (0x02U)
#define UJA113X_SUPIS_VEXTUI_SHIFT      (1U)
#define UJA113X_SUPIS_VEXTUI_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_VEXTUI_SHIFT) & \
										UJA113X_SUPIS_VEXTUI_MASK)

/*!
 * @brief  Supply interrupt status register, V1 undervoltage interrupt status
 * macros.
 */
#define UJA113X_SUPIS_V1UI_MASK       	(0x01U)
#define UJA113X_SUPIS_V1UI_SHIFT      	(0U)
#define UJA113X_SUPIS_V1UI_F(x)       	((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_V1UI_SHIFT) & \
										UJA113X_SUPIS_V1UI_MASK)

/*!
 * @brief Transceiver interrupt status register macros.
 */
#define UJA113X_SUPIS_MASK           	(UJA113X_SUPIS_SMPSSI_MASK | UJA113X_SUPIS_BMOI_MASK | \
		                             	UJA113X_SUPIS_BMUI_MASK | UJA113X_SUPIS_VEXTOI_MASK | \
									 	UJA113X_SUPIS_VEXTUI_MASK | UJA113X_SUPIS_V1UI_MASK)
#define UJA113X_SUPIS_SHIFT          	(0U)
#define UJA113X_SUPIS_F(x)           	((uint8_t)((uint8_t)(x) << UJA113X_SUPIS_SHIFT) & \
										UJA113X_SUPIS_MASK)

/*******************************************************************************
 * Transceiver interrupt status register (0x63)
 ******************************************************************************/
/*!
 * @brief Transceiver interrupt status register, partial networking frame
 * detection error macros.
 */
#define UJA113X_TIS_PNFDEI_MASK   		(0x20U)
#define UJA113X_TIS_PNFDEI_SHIFT  		(5U)
#define UJA113X_TIS_PNFDEI_F(x)   		((uint8_t)((uint8_t)(x) << UJA113X_TIS_PNFDEI_SHIFT) & \
										UJA113X_TIS_PNFDEI_MASK)

/*!
 * @brief Transceiver interrupt status register, CAN-bus silence status macros.
 */
#define UJA113X_TIS_CBSI_MASK     		(0x10U)
#define UJA113X_TIS_CBSI_SHIFT    		(4U)
#define UJA113X_TIS_CBSI_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_TIS_CBSI_SHIFT) & \
										UJA113X_TIS_CBSI_MASK)

/*!
 * @brief Transceiver interrupt status register, LIN2 wake-up interrupt status
 * macros.
 */
#define UJA113X_TIS_LWI2_MASK     		(0x08U)
#define UJA113X_TIS_LWI2_SHIFT    		(3U)
#define UJA113X_TIS_LWI2_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_TIS_LWI2_SHIFT) & \
										UJA113X_TIS_LWI2_MASK)

/*!
 * @brief Transceiver interrupt status register, LIN1 wake-up interrupt status
 * macros.
 */
#define UJA113X_TIS_LWI1_MASK     		(0x04U)
#define UJA113X_TIS_LWI1_SHIFT    		(2U)
#define UJA113X_TIS_LWI1_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_TIS_LWI1_SHIFT) & \
										UJA113X_TIS_LWI1_MASK)

/*!
 * @brief Transceiver interrupt status register, CAN failure interrupt status
 * macros.
 */
#define UJA113X_TIS_CFI_MASK      		(0x02U)
#define UJA113X_TIS_CFI_SHIFT     		(1U)
#define UJA113X_TIS_CFI_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_TIS_CFI_SHIFT) & \
										UJA113X_TIS_CFI_MASK)

/*!
 * @brief Transceiver interrupt status register, CAN wake-up interrupt status
 * macros.
 */
#define UJA113X_TIS_CWI_MASK      		(0x01U)
#define UJA113X_TIS_CWI_SHIFT     		(0U)
#define UJA113X_TIS_CWI_F(x)      		((uint8_t)((uint8_t)(x) << UJA113X_TIS_CWI_SHIFT) & \
										UJA113X_TIS_CWI_MASK)

/*!
 * @brief Transceiver interrupt status register macros.
 */
#define UJA113X_TIS_MASK         		(UJA113X_TIS_PNFDEI_MASK | UJA113X_TIS_CBSI_MASK | \
		                         	 	UJA113X_TIS_LWI2_MASK | UJA113X_TIS_LWI1_MASK | \
										UJA113X_TIS_CFI_MASK | UJA113X_TIS_CWI_MASK)
#define UJA113X_TIS_SHIFT        		(0U)
#define UJA113X_TIS_F(x)         		((uint8_t)((uint8_t)(x) << UJA113X_TIS_SHIFT) & \
										UJA113X_TIS_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) wake-up interrupt status
 * register (0x64, 0x66)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO4/HVIO8 falling edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO48FEI_MASK    	(0x80U)
#define UJA113X_BNWIS_IO48FEI_SHIFT   	(7U)
#define UJA113X_BNWIS_IO48FEI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO48FEI_SHIFT) & \
										UJA113X_BNWIS_IO48FEI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO4/HVIO8 rising edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO48REI_MASK    	(0x40U)
#define UJA113X_BNWIS_IO48REI_SHIFT   	(6U)
#define UJA113X_BNWIS_IO48REI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO48REI_SHIFT) & \
										UJA113X_BNWIS_IO48REI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO3/HVIO7 falling edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO37FEI_MASK    	(0x20U)
#define UJA113X_BNWIS_IO37FEI_SHIFT   	(5U)
#define UJA113X_BNWIS_IO37FEI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO37FEI_SHIFT) & \
										UJA113X_BNWIS_IO37FEI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO3/HVIO7 rising edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO37REI_MASK    	(0x10U)
#define UJA113X_BNWIS_IO37REI_SHIFT   	(4U)
#define UJA113X_BNWIS_IO37REI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO37REI_SHIFT) & \
										UJA113X_BNWIS_IO37REI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO2/HVIO6 falling edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO26FEI_MASK    	(0x08U)
#define UJA113X_BNWIS_IO26FEI_SHIFT   	(3U)
#define UJA113X_BNWIS_IO26FEI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO26FEI_SHIFT) & \
										UJA113X_BNWIS_IO26FEI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO2/HVIO6 rising edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO26REI_MASK    	(0x04U)
#define UJA113X_BNWIS_IO26REI_SHIFT   	(2U)
#define UJA113X_BNWIS_IO26REI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO26REI_SHIFT) & \
										UJA113X_BNWIS_IO26REI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO1/HVIO5 falling edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO15FEI_MASK    	(0x02U)
#define UJA113X_BNWIS_IO15FEI_SHIFT   	(1U)
#define UJA113X_BNWIS_IO15FEI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO15FEI_SHIFT) & \
										UJA113X_BNWIS_IO15FEI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register,
 * HVIO1/HVIO5 rising edge interrupt status macros.
 */
#define UJA113X_BNWIS_IO15REI_MASK    	(0x01U)
#define UJA113X_BNWIS_IO15REI_SHIFT   	(0U)
#define UJA113X_BNWIS_IO15REI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_IO15REI_SHIFT) & \
										UJA113X_BNWIS_IO15REI_MASK)

/*!
 * @brief Bank 0 and 1 wake-up interrupt status register macros.
 */
#define UJA113X_BNWIS_MASK             	(UJA113X_BNWIS_IO48FEI_MASK | UJA113X_BNWIS_IO48REI_MASK | \
										UJA113X_BNWIS_IO37FEI_MASK | UJA113X_BNWIS_IO37REI_MASK | \
										UJA113X_BNWIS_IO26FEI_MASK | UJA113X_BNWIS_IO26REI_MASK | \
										UJA113X_BNWIS_IO15FEI_MASK | UJA113X_BNWIS_IO15REI_MASK)
#define UJA113X_BNWIS_SHIFT            	(0U)
#define UJA113X_BNWIS_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNWIS_SHIFT) & \
										UJA113X_BNWIS_MASK)

/*******************************************************************************
 * Bank 0 (HVIO1 to HVIO4) and 1 (HVIO5 to HVIO8) fail interrupt status
 * register (0x65, 0x67)
 ******************************************************************************/
/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO4/HVIO8 short circuit interrupt status macros.
 */
#define UJA113X_BNFIS_IO48SCI_MASK    	(0x80U)
#define UJA113X_BNFIS_IO48SCI_SHIFT   	(7U)
#define UJA113X_BNFIS_IO48SCI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO48SCI_SHIFT) & \
										UJA113X_BNFIS_IO48SCI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO4/HVIO8 open load interrupt status macros.
 */
#define UJA113X_BNFIS_IO48OLI_MASK    	(0x40U)
#define UJA113X_BNFIS_IO48OLI_SHIFT   	(6U)
#define UJA113X_BNFIS_IO48OLI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO48OLI_SHIFT) & \
										UJA113X_BNFIS_IO48OLI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO3/HVIO7 short circuit interrupt status macros.
 */
#define UJA113X_BNFIS_IO37SCI_MASK    	(0x20U)
#define UJA113X_BNFIS_IO37SCI_SHIFT   	(5U)
#define UJA113X_BNFIS_IO37SCI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO37SCI_SHIFT) & \
										UJA113X_BNFIS_IO37SCI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO3/HVIO7 open load interrupt status macros.
 */
#define UJA113X_BNFIS_IO37OLI_MASK    	(0x10U)
#define UJA113X_BNFIS_IO37OLI_SHIFT   	(4U)
#define UJA113X_BNFIS_IO37OLI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO37OLI_SHIFT) & \
										UJA113X_BNFIS_IO37OLI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO2/HVIO6 short circuit interrupt status macros.
 */
#define UJA113X_BNFIS_IO26SCI_MASK    	(0x08U)
#define UJA113X_BNFIS_IO26SCI_SHIFT   	(3U)
#define UJA113X_BNFIS_IO26SCI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO26SCI_SHIFT) & \
										UJA113X_BNFIS_IO26SCI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO2/HVIO6 open load interrupt status macros.
 */
#define UJA113X_BNFIS_IO26OLI_MASK    	(0x04U)
#define UJA113X_BNFIS_IO26OLI_SHIFT   	(2U)
#define UJA113X_BNFIS_IO26OLI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO26OLI_SHIFT) & \
										UJA113X_BNFIS_IO26OLI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO1/HVIO5 short circuit interrupt status macros.
 */
#define UJA113X_BNFIS_IO15SCI_MASK    	(0x02U)
#define UJA113X_BNFIS_IO15SCI_SHIFT   	(1U)
#define UJA113X_BNFIS_IO15SCI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO15SCI_SHIFT) & \
										UJA113X_BNFIS_IO15SCI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register,
 * HVIO1/HVIO5 open load interrupt status macros.
 */
#define UJA113X_BNFIS_IO15OLI_MASK    	(0x01U)
#define UJA113X_BNFIS_IO15OLI_SHIFT   	(0U)
#define UJA113X_BNFIS_IO15OLI_F(x)    	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_IO15OLI_SHIFT) & \
										UJA113X_BNFIS_IO15OLI_MASK)

/*!
 * @brief Bank 0 and 1 fail interrupt status register macros.
 */
#define UJA113X_BNFIS_MASK             	(UJA113X_BNFIS_IO48SCI_MASK | UJA113X_BNFIS_IO48OLI_MASK | \
										UJA113X_BNFIS_IO37SCI_MASK | UJA113X_BNFIS_IO37OLI_MASK | \
										UJA113X_BNFIS_IO26SCI_MASK | UJA113X_BNFIS_IO26OLI_MASK | \
										UJA113X_BNFIS_IO15SCI_MASK | UJA113X_BNFIS_IO15OLI_MASK)
#define UJA113X_BNFIS_SHIFT            	(0U)
#define UJA113X_BNFIS_F(x)             	((uint8_t)((uint8_t)(x) << UJA113X_BNFIS_SHIFT) & \
										UJA113X_BNFIS_MASK)




/*!
 * @brief  WAKE pin event status register, WAKE pin rising edge macros.
 */
#define UJA113X_WAKE_EVNT_STAT_WPR_MASK      (0x02U)
#define UJA113X_WAKE_EVNT_STAT_WPR_SHIFT     (1U)
#define UJA113X_WAKE_EVNT_STAT_WPR_F(x)      ((uint8_t)((uint8_t)(x)<<UJA113X_WAKE_EVNT_STAT_WPR_SHIFT)&UJA113X_WAKE_EVNT_STAT_WPR_MASK)

/*!
 * @brief  WAKE pin event status register, WAKE pin falling edge macros.
 */
#define UJA113X_WAKE_EVNT_STAT_WPF_MASK      (0x01U)
#define UJA113X_WAKE_EVNT_STAT_WPF_SHIFT     (0U)
#define UJA113X_WAKE_EVNT_STAT_WPF_F(x)      ((uint8_t)((uint8_t)(x)<<UJA113X_WAKE_EVNT_STAT_WPF_SHIFT)&UJA113X_WAKE_EVNT_STAT_WPF_MASK)

/*!
 * @brief  WAKE pin event status register macros.
 */
#define UJA113X_WAKE_EVNT_STAT_MASK          (UJA113X_WAKE_EVNT_STAT_WPR_MASK | UJA113X_WAKE_EVNT_STAT_WPF_MASK)
#define UJA113X_WAKE_EVNT_STAT_SHIFT         (0U)
#define UJA113X_WAKE_EVNT_STAT_F(x)          ((uint8_t)((uint8_t)(x)<<UJA113X_WAKE_EVNT_STAT_SHIFT)&UJA113X_WAKE_EVNT_STAT_MASK)

/*******************************************************************************
 * Data mask registers (0x68 to 0x6F)
 ******************************************************************************/
/*!
 * @brief Data mask registers, data mask 0-7 configuration macros.
 */
#define UJA113X_DMX_DM_MASK             (0xFFU)
#define UJA113X_DMX_DM_SHIFT            (0U)
#define UJA113X_DMX_DM_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_DMX_DM_SHIFT) & \
										UJA113X_DMX_DM_MASK)

/*******************************************************************************
 * MTPNV and ID registers (0x70 - 0x7F)
 ******************************************************************************/

/*******************************************************************************
 * MTPNV status register (0x70)
 ******************************************************************************/
/*!
 * @brief MTPNV status register, write counter macros.
 */
#define UJA113X_MTPNVS_WRCNTS_MASK      (0xFCU)
#define UJA113X_MTPNVS_WRCNTS_SHIFT     (2U)
#define UJA113X_MTPNVS_WRCNTS_F(x)      ((uint8_t)((uint8_t)(x) << UJA113X_MTPNVS_WRCNTS_SHIFT) & \
										UJA113X_MTPNVS_WRCNTS_MASK)

/*!
 * @brief MTPNV status register, error detection during MTPVN cell programming
 * macros.
 */
#define UJA113X_MTPNVS_ECCS_MASK        (0x02U)
#define UJA113X_MTPNVS_ECCS_SHIFT       (1U)
#define UJA113X_MTPNVS_ECCS_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_MTPNVS_ECCS_SHIFT) & \
										UJA113X_MTPNVS_ECCS_MASK)

/*!
 * @brief MTPNV status register, non-volatile memory programming enable macros.
 */
#define UJA113X_MTPNVS_NVMPS_MASK       (0x01U)
#define UJA113X_MTPNVS_NVMPS_SHIFT      (0U)
#define UJA113X_MTPNVS_NVMPS_F(x)       ((uint8_t)((uint8_t)(x) << UJA113X_MTPNVS_NVMPS_SHIFT) & \
										UJA113X_MTPNVS_NVMPS_MASK)

/*!
 * @brief   MTPNV status register macros.
 */
#define UJA113X_MTPNVS_MASK             (UJA113X_MTPNVS_WRCNTS_MASK | UJA113X_MTPNVS_ECCS_MASK | \
                                        UJA113X_MTPNVS_NVMPS_MASK)
#define UJA113X_MTPNVS_SHIFT            (0U)
#define UJA113X_MTPNVS_F(x)             ((uint8_t)((uint8_t)(x) << UJA113X_MTPNVS_SHIFT) & \
										UJA113X_MTPNVS_MASK)

/*******************************************************************************
 * HVIO high-side and low-side driver control register (0x71, 0x72)
 ******************************************************************************/
/*!
 * @brief HVIO high-side and low-side driver control register, HVIO8 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO8HLOC_MASK     (0x80U)
#define UJA113X_IOHLDC_IO8HLOC_SHIFT    (7U)
#define UJA113X_IOHLDC_IO8HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO8HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO8HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO7 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO7HLOC_MASK     (0x40U)
#define UJA113X_IOHLDC_IO7HLOC_SHIFT    (6U)
#define UJA113X_IOHLDC_IO7HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO7HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO7HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO6 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO6HLOC_MASK     (0x20U)
#define UJA113X_IOHLDC_IO6HLOC_SHIFT    (5U)
#define UJA113X_IOHLDC_IO6HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO6HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO6HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO5 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO5HLOC_MASK     (0x10U)
#define UJA113X_IOHLDC_IO5HLOC_SHIFT    (4U)
#define UJA113X_IOHLDC_IO5HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO5HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO5HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO4 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO4HLOC_MASK     (0x08U)
#define UJA113X_IOHLDC_IO4HLOC_SHIFT    (3U)
#define UJA113X_IOHLDC_IO4HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO4HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO4HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO3 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO3HLOC_MASK     (0x04U)
#define UJA113X_IOHLDC_IO3HLOC_SHIFT    (2U)
#define UJA113X_IOHLDC_IO3HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO3HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO3HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO2 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO2HLOC_MASK     (0x02U)
#define UJA113X_IOHLDC_IO2HLOC_SHIFT    (1U)
#define UJA113X_IOHLDC_IO2HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO2HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO2HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register, HVIO1 high-side
 * and low-side driver control macros.
 */
#define UJA113X_IOHLDC_IO1HLOC_MASK     (0x01U)
#define UJA113X_IOHLDC_IO1HLOC_SHIFT    (0U)
#define UJA113X_IOHLDC_IO1HLOC_F(x)     ((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_IO1HLOC_SHIFT) & \
										UJA113X_IOHLDC_IO1HLOC_MASK)

/*!
 * @brief HVIO high-side and low-side driver control register macros.
 */
#define UJA113X_IOHLDC_MASK     		(UJA113X_IOHLDC_IO8HLOC_MASK | UJA113X_IOHLDC_IO7HLOC_MASK | \
										UJA113X_IOHLDC_IO6HLOC_MASK | UJA113X_IOHLDC_IO5HLOC_MASK | \
										UJA113X_IOHLDC_IO4HLOC_MASK | UJA113X_IOHLDC_IO3HLOC_MASK | \
										UJA113X_IOHLDC_IO2HLOC_MASK | UJA113X_IOHLDC_IO1HLOC_MASK
#define UJA113X_IOHLDC_SHIFT    		(0U)
#define UJA113X_IOHLDC_F(x)     		((uint8_t)((uint8_t)(x) << UJA113X_IOHLDC_SHIFT) & \
										UJA113X_IOHLDC_MASK)

/*******************************************************************************
 * Start-up control register (0x73)
 ******************************************************************************/
/*!
 * @brief Start-up control register, RSTN output reset pulse width macros.
 */
#define UJA113X_SC_RLC_MASK            	(0x30U)
#define UJA113X_SC_RLC_SHIFT           	(4U)
#define UJA113X_SC_RLC_F(x)            	((uint8_t)((uint8_t)(x) << UJA113X_SC_RLC_SHIFT) & \
										UJA113X_SC_RLC_MASK)

/*!
 * @brief Start-up control register, V2 start-up control macros.
 */
#define UJA113X_SC_V2SUC_MASK          	(0x08U)
#define UJA113X_SC_V2SUC_SHIFT         	(3U)
#define UJA113X_SC_V2SUC_F(x)          	((uint8_t)((uint8_t)(x) << UJA113X_SC_V2SUC_SHIFT) & \
										UJA113X_SC_V2SUC_MASK)

/*!
 * @brief Start-up control register, HVIO4 configuration control macros.
 */
#define UJA113X_SC_IO4SFC_MASK         	(0x04U)
#define UJA113X_SC_IO4SFC_SHIFT         (2U)
#define UJA113X_SC_IO4SFC_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_SC_IO4SFC_SHIFT) & \
										UJA113X_SC_IO4SFC_MASK)

/*!
 * @brief Start-up control register, HVIO3 configuration control macros.
 */
#define UJA113X_SC_IO3SFC_MASK         	(0x02U)
#define UJA113X_SC_IO3SFC_SHIFT         (1U)
#define UJA113X_SC_IO3SFC_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_SC_IO3SFC_SHIFT) & \
										UJA113X_SC_IO3SFC_MASK)

/*!
 * @brief Start-up control register, HVIO2 configuration control macros.
 */
#define UJA113X_SC_IO2SFC_MASK         	(0x01U)
#define UJA113X_SC_IO2SFC_SHIFT         (0U)
#define UJA113X_SC_IO2SFC_F(x)          ((uint8_t)((uint8_t)(x) << UJA113X_SC_IO2SFC_SHIFT) & \
										UJA113X_SC_IO2SFC_MASK)

/*!
 * @brief Start-up control register macros.
 */
#define UJA113X_SC_MASK                	(UJA113X_SC_RLC_MASK | UJA113X_SC_V2SUC_MASK | \
										UJA113X_SC_IO4SFC_MASK | UJA113X_SC_IO3SFC_MASK | \
										UJA113X_SC_IO2SFC_MASK)
#define UJA113X_SC_SHIFT               	(0U)
#define UJA113X_SC_F(x)                	((uint8_t)((uint8_t)(x) << UJA113X_SC_SHIFT) & \
										UJA113X_SC_MASK)

/*******************************************************************************
 * SBC configuration control register (0x74)
 ******************************************************************************/
/*!
 * @brief SBC configuration control register, V1 undervoltage threshold
 * (defined by bit V1RTC) at start-up - macros.
 */
#define UJA113X_SCC_V1RTSUC_MASK        (0x30U)
#define UJA113X_SCC_V1RTSUC_SHIFT       (4U)
#define UJA113X_SCC_V1RTSUC_F(x)        ((uint8_t)((uint8_t)(x) << UJA113X_SCC_V1RTSUC_SHIFT) & \
										UJA113X_SCC_V1RTSUC_MASK)

/*!
 * @brief SBC configuration control register, Forced Normal mode control macros.
 */
#define UJA113X_SCC_FNMC_MASK           (0x08U)
#define UJA113X_SCC_FNMC_SHIFT          (3U)
#define UJA113X_SCC_FNMC_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_SCC_FNMC_SHIFT) & \
										UJA113X_SBC_FNMC_MASK)

/*!
 * @brief SBC configuration control register, Software Development mode control
 * macros.
 */
#define UJA113X_SCC_SDMC_MASK           (0x04U)
#define UJA113X_SCC_SDMC_SHIFT          (2U)
#define UJA113X_SCC_SDMC_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_SCC_SDMC_SHIFT) & \
										UJA113X_SBC_SDMC_MASK)

/*!
 * @brief SBC configuration control register, regulator V2 control
 * macros.
 */
#define UJA113X_SCC_VEXTAC_MASK         (0x02U)
#define UJA113X_SCC_VEXTAC_SHIFT        (1U)
#define UJA113X_SCC_VEXTAC_F(x)         ((uint8_t)((uint8_t)(x) << UJA113X_SCC_VEXTAC_SHIFT) & \
										UJA113X_SBC_VEXTAC_MASK)

/*!
 * @brief SBC configuration control register, Sleep control macros.
 */
#define UJA113X_SCC_SLPC_MASK           (0x01U)
#define UJA113X_SCC_SLPC_SHIFT          (0U)
#define UJA113X_SCC_SLPC_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_SCC_SLPC_SHIFT) & \
										UJA113X_SCC_SLPC_MASK)

/*!
 * @brief SBC configuration control register macros.
 */
#define UJA113X_SCC_MASK                (UJA113X_SCC_V1RTSUC_MASK | UJA113X_SCC_FNMC_MASK | \
                                        UJA113X_SCC_SDMC_MASK| UJA113X_SCC_VEXTAC_MASK | \
										UJA113X_SBC_SLPC_MASK)
#define UJA113X_SCC_SHIFT               (0U)
#define UJA113X_SCC_F(x)                ((uint8_t)((uint8_t)(x) << UJA113X_SCC_SHIFT) & \
										UJA113X_SCC_MASK)

/*******************************************************************************
 * MTPVN CRC control register (0x75)
 ******************************************************************************/
/*!
 * @brief MTPNV CRC control register, CRC control data macros.
 */
#define UJA113X_MCRCC_CRCC_MASK         (0xFFU)
#define UJA113X_MCRCC_CRCC_SHIFT        (0U)
#define UJA113X_MCRCC_CRCC_F(x)         ((uint8_t)((uint8_t)(x) << UJA113X_MCRCC_CRCC_SHIFT) & \
										UJA113X_MTPNV_CRC_MASK)

/*******************************************************************************
 * Identification register 1 (0x7E)
 ******************************************************************************/
/*!
 * @brief Identification register 1, device identification code (part 1) macros.
 */
#define UJA113X_ID1_ID0S_MASK           (0xFFU)
#define UJA113X_ID1_ID0S_SHIFT          (0U)
#define UJA113X_ID1_ID0S_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_ID1_ID0S_SHIFT) & \
										UJA113X_ID1_ID0S_MASK)

/*******************************************************************************
 * Identification register 2 (0x7F)
 ******************************************************************************/
/*!
 * @brief Identification register 2, device identification code (part 2) macros.
 */
#define UJA113X_ID2_ID1S_MASK           (0xC0U)
#define UJA113X_ID2_ID1S_SHIFT          (6U)
#define UJA113X_ID2_ID1S_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_ID2_ID1S_SHIFT) & \
										UJA113X_ID2_ID1S_MASK)

/*!
 * @brief Identification register 2, silicon version macros.
 */
#define UJA113X_ID2_IDVS_MASK           (0x3FU)
#define UJA113X_ID2_IDVS_SHIFT          (0U)
#define UJA113X_ID2_IDVS_F(x)           ((uint8_t)((uint8_t)(x) << UJA113X_ID2_IDVS_SHIFT) & \
										UJA113X_ID2_IDVS_MASK)

#endif /* SBC_UJA113X_MAP_H_ */
