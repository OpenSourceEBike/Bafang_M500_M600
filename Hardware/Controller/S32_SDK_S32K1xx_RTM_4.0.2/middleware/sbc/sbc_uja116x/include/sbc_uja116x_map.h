/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
 * @sbc_uja116x_map.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined
 * This is required to allow the use of a macro for shifting and masking
 * uint8_t type values only.
 * the user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Macro not referenced
 * There are macro which is not currently referenced to other code but it can
 * contains memory maps which can be used by user.
 * the user.
 */


#ifndef SOURCES_SBC_UJA_116X_H_
#define SOURCES_SBC_UJA_116X_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief  Register address macros.
 */
#define SBC_UJA_REG_ADDR_MASK                (0xFEU)
#define SBC_UJA_REG_ADDR_SHIFT               (1U)
#define SBC_UJA_REG_ADDR_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_REG_ADDR_SHIFT)&SBC_UJA_REG_ADDR_MASK)

/*!
 * @brief  Watchdog mode control,  watchdog mode control macros.
 */
#define SBC_UJA_WTDOG_CTR_WMC_MASK           (0xE0U)
#define SBC_UJA_WTDOG_CTR_WMC_SHIFT          (5U)
#define SBC_UJA_WTDOG_CTR_WMC_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_CTR_WMC_SHIFT)&SBC_UJA_WTDOG_CTR_WMC_MASK)

/*!
 * @brief  Watchdog mode control,  nominal watchdog period macros.
 */
#define SBC_UJA_WTDOG_CTR_NWP_MASK           (0x0FU)
#define SBC_UJA_WTDOG_CTR_NWP_SHIFT          (0U)
#define SBC_UJA_WTDOG_CTR_NWP_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_CTR_NWP_SHIFT)&SBC_UJA_WTDOG_CTR_NWP_MASK)

/*!
 * @brief  Watchdog control macros.
 */
#define SBC_UJA_WTDOG_CTR_MASK               (SBC_UJA_WTDOG_CTR_WMC_MASK | SBC_UJA_WTDOG_CTR_NWP_MASK)
#define SBC_UJA_WTDOG_CTR_SHIFT              (0U)
#define SBC_UJA_WTDOG_CTR_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_CTR_SHIFT)&SBC_UJA_WTDOG_CTR_MASK)

/*!
 * @brief  Mode control,  mode control macros.
 */
#define SBC_UJA_MODE_MC_MASK                 (0x07U)
#define SBC_UJA_MODE_MC_SHIFT                (0U)
#define SBC_UJA_MODE_MC_F(x)                 ((uint8_t)((uint8_t)(x)<<SBC_UJA_MODE_MC_SHIFT)&SBC_UJA_MODE_MC_MASK)

/*!
 * @brief  Mode control macros.
 */
#define SBC_UJA_MODE_MASK                    (SBC_UJA_MODE_MC_MASK)
#define SBC_UJA_MODE_SHIFT                   (SBC_UJA_MODE_MC_SHIFT)
#define SBC_UJA_MODE_F(x)                    ((uint8_t)((uint8_t)(x)<<SBC_UJA_MODE_SHIFT)&SBC_UJA_MODE_MASK)

/*!
 * @brief  Fail-safe control register, LIMP home control macros.
 */
#define SBC_UJA_FAIL_SAFE_LHC_MASK           (0x04U)
#define SBC_UJA_FAIL_SAFE_LHC_SHIFT          (2U)
#define SBC_UJA_FAIL_SAFE_LHC_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_FAIL_SAFE_LHC_SHIFT)&SBC_UJA_FAIL_SAFE_LHC_MASK)

/*!
 * @brief  Fail-safe control register, reset counter control macros.
 */
#define SBC_UJA_FAIL_SAFE_RCC_MASK           (0x03U)
#define SBC_UJA_FAIL_SAFE_RCC_SHIFT          (0U)
#define SBC_UJA_FAIL_SAFE_RCC_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_FAIL_SAFE_RCC_SHIFT)&SBC_UJA_FAIL_SAFE_RCC_MASK)

/*!
 * @brief  Fail-safe control register macros.
 */
#define SBC_UJA_FAIL_SAFE_MASK               (SBC_UJA_FAIL_SAFE_LHC_MASK | SBC_UJA_FAIL_SAFE_RCC_MASK)
#define SBC_UJA_FAIL_SAFE_SHIFT              (0U)
#define SBC_UJA_FAIL_SAFE_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_FAIL_SAFE_SHIFT)&SBC_UJA_FAIL_SAFE_MASK)

/*!
 * @brief  Main status register, Overtemperature warning status macros.
 */
#define SBC_UJA_MAIN_OTWS_MASK               (0x40U)
#define SBC_UJA_MAIN_OTWS_SHIFT              (6U)
#define SBC_UJA_MAIN_OTWS_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MAIN_OTWS_SHIFT)&SBC_UJA_MAIN_OTWS_MASK)

/*!
 * @brief  Main status register, Normal mode status macros.
 */
#define SBC_UJA_MAIN_NMS_MASK                (0x20U)
#define SBC_UJA_MAIN_NMS_SHIFT               (5U)
#define SBC_UJA_MAIN_NMS_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_MAIN_NMS_SHIFT)&SBC_UJA_MAIN_NMS_MASK)

/*!
 * @brief  Main status register, Reset source status macros.
 */
#define SBC_UJA_MAIN_RSS_MASK                (0x1FU)
#define SBC_UJA_MAIN_RSS_SHIFT               (0U)
#define SBC_UJA_MAIN_RSS_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_MAIN_RSS_SHIFT)&SBC_UJA_MAIN_RSS_MASK)

/*!
 * @brief  Main status macros.
 */
#define SBC_UJA_MAIN_MASK                    (SBC_UJA_MAIN_OTWS_MASK | SBC_UJA_MAIN_NMS_MASK | SBC_UJA_MAIN_RSS_MASK)
#define SBC_UJA_MAIN_SHIFT                   (0U)
#define SBC_UJA_MAIN_F(x)                    ((uint8_t)((uint8_t)(x)<<SBC_UJA_MAIN_SHIFT)&SBC_UJA_MAIN_MASK)

/*!
 * @brief   System event capture enable, overtemperature warning enable macros.
 */
#define SBC_UJA_SYS_EVNT_OTWE_MASK           (0x04U)
#define SBC_UJA_SYS_EVNT_OTWE_SHIFT          (2U)
#define SBC_UJA_SYS_EVNT_OTWE_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_OTWE_SHIFT)&SBC_UJA_SYS_EVNT_OTWE_MASK)

/*!
 * @brief   System event capture enable, SPI failure enable.
 */
#define SBC_UJA_SYS_EVNT_SPIFE_MASK          (0x02U)
#define SBC_UJA_SYS_EVNT_SPIFE_SHIFT         (1U)
#define SBC_UJA_SYS_EVNT_SPIFE_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_SPIFE_SHIFT)&SBC_UJA_SYS_EVNT_SPIFE_MASK)

/*!
 * @brief   System event capture enable.
 */
#define SBC_UJA_SYS_EVNT_MASK                (SBC_UJA_SYS_EVNT_OTWE_MASK | SBC_UJA_SYS_EVNT_SPIFE_MASK)
#define SBC_UJA_SYS_EVNT_SHIFT               (1U)
#define SBC_UJA_SYS_EVNT_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_SHIFT)&SBC_UJA_SYS_EVNT_MASK)

/*!
 * @brief  Watchdog status register, forced Normal mode status macros.
 */
#define SBC_UJA_WTDOG_STAT_FNMS_MASK         (0x08U)
#define SBC_UJA_WTDOG_STAT_FNMS_SHIFT        (3U)
#define SBC_UJA_WTDOG_STAT_FNMS_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_STAT_FNMS_SHIFT)&SBC_UJA_WTDOG_STAT_FNMS_MASK)

/*!
 * @brief  Watchdog status register, Software Development mode status macros.
 */
#define SBC_UJA_WTDOG_STAT_SDMS_MASK         (0x04U)
#define SBC_UJA_WTDOG_STAT_SDMS_SHIFT        (2U)
#define SBC_UJA_WTDOG_STAT_SDMS_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_STAT_SDMS_SHIFT)&SBC_UJA_WTDOG_STAT_SDMS_MASK)

/*!
 * @brief  Watchdog status register, watchdog status macros.
 */
#define SBC_UJA_WTDOG_STAT_WDS_MASK          (0x03U)
#define SBC_UJA_WTDOG_STAT_WDS_SHIFT         (0U)
#define SBC_UJA_WTDOG_STAT_WDS_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_STAT_WDS_SHIFT)&SBC_UJA_WTDOG_STAT_WDS_MASK)

/*!
 * @brief  Watchdog status macros.
 */
#define SBC_UJA_WTDOG_STAT_MASK              (SBC_UJA_WTDOG_STAT_FNMS_MASK | SBC_UJA_WTDOG_STAT_SDMS_MASK \
                                         | SBC_UJA_WTDOG_STAT_WDS_MASK)
#define SBC_UJA_WTDOG_STAT_SHIFT             (0U)
#define SBC_UJA_WTDOG_STAT_F(x)              ((uint8_t)((uint8_t)(x)<<SBC_UJA_WTDOG_STAT_SHIFT)&SBC_UJA_WTDOG_STAT_MASK)

/*!
 * @brief  Memory X macros.
 */
#define SBC_UJA_MEMORY_X_MASK                (0xFFU)
#define SBC_UJA_MEMORY_X_SHIFT               (0U)
#define SBC_UJA_MEMORY_X_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_MEMORY_X_SHIFT)&SBC_UJA_MEMORY_X_MASK)

/*!
 * @brief  Lock control 6: address area 0x68 to 0x6F macros.
 */
#define SBC_UJA_LOCK_LK6C_MASK               (0x40U)
#define SBC_UJA_LOCK_LK6C_SHIFT              (6U)
#define SBC_UJA_LOCK_LK6C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK6C_SHIFT)&SBC_UJA_LOCK_LK6C_MASK)

/*!
 * @brief  Lock control control 5: address area 0x50 to 0x5F - unused register range macros.
 */
#define SBC_UJA_LOCK_LK5C_MASK               (0x20U)
#define SBC_UJA_LOCK_LK5C_SHIFT              (5U)
#define SBC_UJA_LOCK_LK5C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK5C_SHIFT)&SBC_UJA_LOCK_LK5C_MASK)

/*!
 * @brief  Lock control 4: address area 0x40 to 0x4F - WAKE pin control macros.
 */
#define SBC_UJA_LOCK_LK4C_MASK               (0x10U)
#define SBC_UJA_LOCK_LK4C_SHIFT              (4U)
#define SBC_UJA_LOCK_LK4C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK4C_SHIFT)&SBC_UJA_LOCK_LK4C_MASK)

/*!
 * @brief  Lock control 3: address area 0x30 to 0x3F - unused register range macros.
 */
#define SBC_UJA_LOCK_LK3C_MASK               (0x08U)
#define SBC_UJA_LOCK_LK3C_SHIFT              (3U)
#define SBC_UJA_LOCK_LK3C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK3C_SHIFT)&SBC_UJA_LOCK_LK3C_MASK)

/*!
 * @brief  Lock control 2: address area 0x20 to 0x2F - transceiver control macros.
 */
#define SBC_UJA_LOCK_LK2C_MASK               (0x04U)
#define SBC_UJA_LOCK_LK2C_SHIFT              (2U)
#define SBC_UJA_LOCK_LK2C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK2C_SHIFT)&SBC_UJA_LOCK_LK2C_MASK)

/*!
 * @brief  Lock control 1: address area 0x10 to 0x1F - regulator control macros.
 */
#define SBC_UJA_LOCK_LK1C_MASK               (0x02U)
#define SBC_UJA_LOCK_LK1C_SHIFT              (1U)
#define SBC_UJA_LOCK_LK1C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK1C_SHIFT)&SBC_UJA_LOCK_LK1C_MASK)

/*!
 * @brief  Lock control 0: address area 0x06 to 0x09 - general-purpose memory macros.
 */
#define SBC_UJA_LOCK_LK0C_MASK               (0x01U)
#define SBC_UJA_LOCK_LK0C_SHIFT              (0U)
#define SBC_UJA_LOCK_LK0C_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LK0C_SHIFT)&SBC_UJA_LOCK_LK0C_MASK)

/*!
 * @brief  Lock control N macros.
 */
#define SBC_UJA_LOCK_LKNC_MASK               (SBC_UJA_LOCK_LK0C_MASK | SBC_UJA_LOCK_LK1C_MASK \
                                         | SBC_UJA_LOCK_LK2C_MASK | SBC_UJA_LOCK_LK3C_MASK \
										                               | SBC_UJA_LOCK_LK4C_MASK | SBC_UJA_LOCK_LK5C_MASK \
																	                        | SBC_UJA_LOCK_LK6C_MASK)
#define SBC_UJA_LOCK_LKNC_SHIFT              (0U)
#define SBC_UJA_LOCK_LKNC_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_LOCK_LKNC_SHIFT)&SBC_UJA_LOCK_LKNC_MASK)

/*!
 * @brief  Regulator control register, power distribution control macros.
 */
#define SBC_UJA_REGULATOR_PDC_MASK           (0x40U)
#define SBC_UJA_REGULATOR_PDC_SHIFT          (6U)
#define SBC_UJA_REGULATOR_PDC_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_REGULATOR_PDC_SHIFT)&SBC_UJA_REGULATOR_PDC_MASK)

/*!
 * @brief  Regulator control register, V2/VEXT configuration macros.
 */
#define SBC_UJA_REGULATOR_V2C_MASK           (0x0CU)
#define SBC_UJA_REGULATOR_V2C_SHIFT          (2U)
#define SBC_UJA_REGULATOR_V2C_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_REGULATOR_V2C_SHIFT)&SBC_UJA_REGULATOR_V2C_MASK)

/*!
 * @brief  Regulator control register, set V1 reset threshold macros.
 */
#define SBC_UJA_REGULATOR_V1RTC_MASK         (0x03U)
#define SBC_UJA_REGULATOR_V1RTC_SHIFT        (0U)
#define SBC_UJA_REGULATOR_V1RTC_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_REGULATOR_V1RTC_SHIFT)&SBC_UJA_REGULATOR_V1RTC_MASK)

/*!
 * @brief  Regulator control register macros.
 */
#define SBC_UJA_REGULATOR_MASK               (SBC_UJA_REGULATOR_PDC_MASK | SBC_UJA_REGULATOR_V2C_MASK | SBC_UJA_REGULATOR_V1RTC_MASK)
#define SBC_UJA_REGULATOR_SHIFT              (0U)
#define SBC_UJA_REGULATOR_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_REGULATOR_SHIFT)&SBC_UJA_REGULATOR_MASK)

/*!
 * @brief  Supply voltage status register, V2/VEXT status macros
 */
#define SBC_UJA_SUPPLY_STAT_V2S_MASK         (0x06U)
#define SBC_UJA_SUPPLY_STAT_V2S_SHIFT        (1U)
#define SBC_UJA_SUPPLY_STAT_V2S_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_STAT_V2S_SHIFT)&SBC_UJA_SUPPLY_STAT_V2S_MASK)

/*!
 * @brief  Supply voltage status register, V1 status macros
 */
#define SBC_UJA_SUPPLY_STAT_V1S_MASK         (0x01U)
#define SBC_UJA_SUPPLY_STAT_V1S_SHIFT        (0U)
#define SBC_UJA_SUPPLY_STAT_V1S_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_STAT_V1S_SHIFT)&SBC_UJA_SUPPLY_STAT_V1S_MASK)

/*!
 * @brief  Supply voltage status register macros.
 */
#define SBC_UJA_SUPPLY_STAT_MASK             (SBC_UJA_SUPPLY_STAT_V2S_MASK | SBC_UJA_SUPPLY_STAT_V1S_MASK)
#define SBC_UJA_SUPPLY_STAT_SHIFT            (0U)
#define SBC_UJA_SUPPLY_STAT_F(x)             ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_STAT_SHIFT)&SBC_UJA_SUPPLY_STAT_MASK)

/*!
 * @brief  Supply event capture enable register,
 *         V2/VEXT overvoltage enable macros.
 */
#define SBC_UJA_SUPPLY_EVNT_V2OE_MASK        (0x04U)
#define SBC_UJA_SUPPLY_EVNT_V2OE_SHIFT       (2U)
#define SBC_UJA_SUPPLY_EVNT_V2OE_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_EVNT_V2OE_SHIFT)&SBC_UJA_SUPPLY_EVNT_V2OE_MASK)

/*!
 * @brief  Supply event capture enable register,
 *         V2/VEXT undervoltage enable macros.
 */
#define SBC_UJA_SUPPLY_EVNT_V2UE_MASK        (0x02U)
#define SBC_UJA_SUPPLY_EVNT_V2UE_SHIFT       (1U)
#define SBC_UJA_SUPPLY_EVNT_V2UE_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_EVNT_V2UE_SHIFT)&SBC_UJA_SUPPLY_EVNT_V2UE_MASK)

/*!
 * @brief  Supply event capture enable register, V1 undervoltage enable macros.
 */
#define SBC_UJA_SUPPLY_EVNT_V1UE_MASK        (0x01U)
#define SBC_UJA_SUPPLY_EVNT_V1UE_SHIFT       (0U)
#define SBC_UJA_SUPPLY_EVNT_V1UE_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_EVNT_V1UE_SHIFT)&SBC_UJA_SUPPLY_EVNT_V1UE_MASK)

/*!
 * @brief  Supply event capture enable register macros.
 */
#define SBC_UJA_SUPPLY_EVNT_MASK             (SBC_UJA_SUPPLY_EVNT_V2OE_MASK| SBC_UJA_SUPPLY_EVNT_V2UE_MASK \
		                                       | SBC_UJA_SUPPLY_EVNT_V1UE_MASK)
#define SBC_UJA_SUPPLY_EVNT_SHIFT            (0U)
#define SBC_UJA_SUPPLY_EVNT_F(x)             ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUPPLY_EVNT_SHIFT)&SBC_UJA_SUPPLY_EVNT_MASK)

/*!
 * @brief  CAN control register, CAN FD control macros.
 */
#define SBC_UJA_CAN_CFDC_MASK                (0x40U)
#define SBC_UJA_CAN_CFDC_SHIFT               (6U)
#define SBC_UJA_CAN_CFDC_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_CAN_CFDC_SHIFT)&SBC_UJA_CAN_CFDC_MASK)

/*!
 * @brief  CAN control register, CAN partial networking configuration OK macros.
 */
#define SBC_UJA_CAN_PNCOK_MASK               (0x20U)
#define SBC_UJA_CAN_PNCOK_SHIFT              (5U)
#define SBC_UJA_CAN_PNCOK_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_CAN_PNCOK_SHIFT)&SBC_UJA_CAN_PNCOK_MASK)

/*!
 * @brief  CAN control register, CAN partial networking control macros.
 */
#define SBC_UJA_CAN_CPNC_MASK                (0x10U)
#define SBC_UJA_CAN_CPNC_SHIFT               (4U)
#define SBC_UJA_CAN_CPNC_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_CAN_CPNC_SHIFT)&SBC_UJA_CAN_CPNC_MASK)

/*!
 * @brief  CAN control register, CAN mode control macros.
 */
#define SBC_UJA_CAN_CMC_MASK                 (0x03U)
#define SBC_UJA_CAN_CMC_SHIFT                (0U)
#define SBC_UJA_CAN_CMC_F(x)                 ((uint8_t)((uint8_t)(x)<<SBC_UJA_CAN_CMC_SHIFT)&SBC_UJA_CAN_CMC_MASK)

/*!
 * @brief  CAN control register macros.
 */
#define SBC_UJA_CAN_MASK                     (SBC_UJA_CAN_CFDC_MASK | SBC_UJA_CAN_PNCOK_MASK \
		                                       | SBC_UJA_CAN_CPNC_MASK | SBC_UJA_CAN_CMC_MASK)
#define SBC_UJA_CAN_SHIFT                    (0U)
#define SBC_UJA_CAN_F(x)                     ((uint8_t)((uint8_t)(x)<<SBC_UJA_CAN_SHIFT)&SBC_UJA_CAN_MASK)

/*!
 * @brief  Transceiver status register, CAN transceiver status macros.
 */
#define SBC_UJA_TRANS_STAT_CTS_MASK          (0x80U)
#define SBC_UJA_TRANS_STAT_CTS_SHIFT         (7U)
#define SBC_UJA_TRANS_STAT_CTS_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_CTS_SHIFT)&SBC_UJA_TRANS_STAT_CTS_MASK)

/*!
 * @brief  Transceiver status register, CAN partial networking error macros.
 */
#define SBC_UJA_TRANS_STAT_CPNERR_MASK       (0x40U)
#define SBC_UJA_TRANS_STAT_CPNERR_SHIFT      (6U)
#define SBC_UJA_TRANS_STAT_CPNERR_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_CPNERR_SHIFT)&SBC_UJA_TRANS_STAT_CPNERR_MASK)

/*!
 * @brief  Transceiver status register, CAN partial networking status macros.
 */
#define SBC_UJA_TRANS_STAT_CPNS_MASK         (0x20U)
#define SBC_UJA_TRANS_STAT_CPNS_SHIFT        (5U)
#define SBC_UJA_TRANS_STAT_CPNS_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_CPNS_SHIFT)&SBC_UJA_TRANS_STAT_CPNS_MASK)

/*!
 * @brief  Transceiver status register, CAN oscillator status macros.
 */
#define SBC_UJA_TRANS_STAT_COSCS_MASK        (0x10U)
#define SBC_UJA_TRANS_STAT_COSCS_SHIFT       (4U)
#define SBC_UJA_TRANS_STAT_COSCS_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_COSCS_SHIFT)&SBC_UJA_TRANS_STAT_COSCS_MASK)

/*!
 * @brief  Transceiver status register, CAN-bus silence status macros.
 */
#define SBC_UJA_TRANS_STAT_CBSS_MASK         (0x08U)
#define SBC_UJA_TRANS_STAT_CBSS_SHIFT        (3U)
#define SBC_UJA_TRANS_STAT_CBSS_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_CBSS_SHIFT)&SBC_UJA_TRANS_STAT_CBSS_MASK)

/*!
 * @brief  Transceiver status register, VCAN status macros.
 */
#define SBC_UJA_TRANS_STAT_VCS_MASK          (0x02U)
#define SBC_UJA_TRANS_STAT_VCS_SHIFT         (1U)
#define SBC_UJA_TRANS_STAT_VCS_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_VCS_SHIFT)&SBC_UJA_TRANS_STAT_VCS_MASK)

/*!
 * @brief  Transceiver status register, CAN failure status macros.
 */
#define SBC_UJA_TRANS_STAT_CFS_MASK          (0x01U)
#define SBC_UJA_TRANS_STAT_CFS_SHIFT         (0U)
#define SBC_UJA_TRANS_STAT_CFS_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_CFS_SHIFT)&SBC_UJA_TRANS_STAT_CFS_MASK)

/*!
 * @brief  Transceiver status register macros.
 */
#define SBC_UJA_TRANS_STAT_MASK              (SBC_UJA_TRANS_STAT_CTS_MASK | SBC_UJA_TRANS_STAT_CPNERR_MASK \
		                                       | SBC_UJA_TRANS_STAT_CPNS_MASK | SBC_UJA_TRANS_STAT_COSCS_MASK \
											                              | SBC_UJA_TRANS_STAT_CBSS_MASK | SBC_UJA_TRANS_STAT_VCS_MASK \
																		                       | SBC_UJA_TRANS_STAT_CFS_MASK)
#define SBC_UJA_TRANS_STAT_SHIFT             (0U)
#define SBC_UJA_TRANS_STAT_F(x)              ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_STAT_SHIFT)&SBC_UJA_TRANS_STAT_MASK)

/*!
 * @brief  Transceiver event capture enable register, CAN-bus silence enable macros.
 */
#define SBC_UJA_TRANS_EVNT_CBSE_MASK         (0x10U)
#define SBC_UJA_TRANS_EVNT_CBSE_SHIFT        (4U)
#define SBC_UJA_TRANS_EVNT_CBSE_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_CBSE_SHIFT)&SBC_UJA_TRANS_EVNT_CBSE_MASK)

/*!
 * @brief  Transceiver event capture enable register, CAN failure enable macros.
 */
#define SBC_UJA_TRANS_EVNT_CFE_MASK          (0x02U)
#define SBC_UJA_TRANS_EVNT_CFE_SHIFT         (1U)
#define SBC_UJA_TRANS_EVNT_CFE_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_CFE_SHIFT)&SBC_UJA_TRANS_EVNT_CFE_MASK)

/*!
 * @brief  Transceiver event capture enable register, CAN wake-up enable.
 */
#define SBC_UJA_TRANS_EVNT_CWE_MASK          (0x01U)
#define SBC_UJA_TRANS_EVNT_CWE_SHIFT         (0U)
#define SBC_UJA_TRANS_EVNT_CWE_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_CWE_SHIFT)&SBC_UJA_TRANS_EVNT_CWE_MASK)

/*!
 * @brief  Transceiver event capture enable register macros.
 */
#define SBC_UJA_TRANS_EVNT_MASK              (SBC_UJA_TRANS_EVNT_CBSE_MASK | SBC_UJA_TRANS_EVNT_CFE_MASK \
                                         | SBC_UJA_TRANS_EVNT_CWE_MASK)
#define SBC_UJA_TRANS_EVNT_SHIFT             (0U)
#define SBC_UJA_TRANS_EVNT_F(x)              ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_SHIFT)&SBC_UJA_TRANS_EVNT_MASK)

/*!
 * @brief  Data rate register, CAN data rate selection macros.
 */
#define SBC_UJA_DAT_RATE_CDR_MASK            (0x07U)
#define SBC_UJA_DAT_RATE_CDR_SHIFT           (0U)
#define SBC_UJA_DAT_RATE_CDR_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_DAT_RATE_CDR_SHIFT)&SBC_UJA_DAT_RATE_CDR_MASK)

/*!
 * @brief  Data rate register macros.
 */
#define SBC_UJA_DAT_RATE_MASK                (SBC_UJA_DAT_RATE_CDR_MASK)
#define SBC_UJA_DAT_RATE_SHIFT               (SBC_UJA_DAT_RATE_CDR_SHIFT)
#define SBC_UJA_DAT_RATE_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_DAT_RATE_SHIFT)&SBC_UJA_DAT_RATE_MASK)

/*!
 * @brief  Identifier 0x27 - ID07:ID00 bits macros.
 */
#define SBC_UJA_IDENTIF_0700_MASK            (0xFFU)
#define SBC_UJA_IDENTIF_0700_SHIFT           (0U)
#define SBC_UJA_IDENTIF_0700_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_0700_SHIFT)&SBC_UJA_IDENTIF_0700_MASK)

/*!
 * @brief  Identifier 0x28 - ID15:ID08 bits macros.
 */
#define SBC_UJA_IDENTIF_1508_MASK            (0xFFU)
#define SBC_UJA_IDENTIF_1508_SHIFT           (0U)
#define SBC_UJA_IDENTIF_1508_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_1508_SHIFT)&SBC_UJA_IDENTIF_1508_MASK)

/*!
 * @brief  Identifier 0x29 - ID23:ID18 bits macros.
 */
#define SBC_UJA_IDENTIF_2318_MASK            (0xFCU)
#define SBC_UJA_IDENTIF_2318_SHIFT           (2U)
#define SBC_UJA_IDENTIF_2318_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_2318_SHIFT)&SBC_UJA_IDENTIF_2318_MASK)

/*!
 * @brief  Identifier 0x29 - ID17:ID16 bits macros.
 */
#define SBC_UJA_IDENTIF_1716_MASK            (0x03U)
#define SBC_UJA_IDENTIF_1716_SHIFT           (0U)
#define SBC_UJA_IDENTIF_1716_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_1716_SHIFT)&SBC_UJA_IDENTIF_1716_MASK)

/*!
 * @brief  Identifier 0x29 - ID23:ID16 bits macros.
 */
#define SBC_UJA_IDENTIF_2316_MASK            (0xFFU)
#define SBC_UJA_IDENTIF_2316_SHIFT           (0U)
#define SBC_UJA_IDENTIF_2316_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_2316_SHIFT)&SBC_UJA_IDENTIF_2316_MASK)

/*!
 * @brief  Identifier 0x2A - ID28:ID24 bits macros.
 */
#define SBC_UJA_IDENTIF_2824_MASK            (0x1FU)
#define SBC_UJA_IDENTIF_2824_SHIFT           (0U)
#define SBC_UJA_IDENTIF_2824_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_2824_SHIFT)&SBC_UJA_IDENTIF_2824_MASK)

/*!
 * @brief  Identifier X macros (0x27-0x2Ah).
 */
#define SBC_UJA_IDENTIF_X_MASK               (0xFFU)
#define SBC_UJA_IDENTIF_X_SHIFT              (0U)
#define SBC_UJA_IDENTIF_X_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_0_SHIFT)&SBC_UJA_IDENTIF_0_MASK)

/*!
 * @brief   Mask 0x2b - M07:M00 macros.
 */
#define SBC_UJA_MASK_0700_MASK               (0xFFU)
#define SBC_UJA_MASK_0700_SHIFT              (0U)
#define SBC_UJA_MASK_0700_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_0700_SHIFT)&SBC_UJA_MASK_0700_MASK)

/*!
 * @brief   Mask 0x2c - M15:M08 macros.
 */
#define SBC_UJA_MASK_1508_MASK               (0xFFU)
#define SBC_UJA_MASK_1508_SHIFT              (0U)
#define SBC_UJA_MASK_1508_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_1508_SHIFT)&SBC_UJA_MASK_1508_MASK)

/*!
 * @brief   Mask 0x2d - M23:M18 macros.
 */
#define SBC_UJA_MASK_2318_MASK               (0xFCU)
#define SBC_UJA_MASK_2318_SHIFT              (2U)
#define SBC_UJA_MASK_2318_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_2318_SHIFT)&SBC_UJA_MASK_2318_MASK)

/*!
 * @brief   Mask 0x2d - M17:M16 macros.
 */
#define SBC_UJA_MASK_1716_MASK               (0x03U)
#define SBC_UJA_MASK_1716_SHIFT              (2U)
#define SBC_UJA_MASK_1716_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_1716_SHIFT)&SBC_UJA_MASK_1716_MASK)

/*!
 * @brief   Mask 0x2d - M23:M16 macros.
 */
#define SBC_UJA_MASK_2316_MASK               (0xFFU)
#define SBC_UJA_MASK_2316_SHIFT              (0U)
#define SBC_UJA_MASK_2316_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_2316_SHIFT)&SBC_UJA_MASK_2316_MASK)

/*!
 * @brief   Mask 0x2e - M28:M24 macros.
 */
#define SBC_UJA_MASK_2824_MASK               (0x1FU)
#define SBC_UJA_MASK_2824_SHIFT              (0U)
#define SBC_UJA_MASK_2824_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_2824_SHIFT)&SBC_UJA_MASK_2824_MASK)

/*!
 * @brief   Mask X macros (0x2b-0x2e).
 */
#define SBC_UJA_MASK_X_MASK                  (0xFFU)
#define SBC_UJA_MASK_X_SHIFT                 (0U)
#define SBC_UJA_MASK_X_F(x)                  ((uint8_t)((uint8_t)(x)<<SBC_UJA_MASK_X_SHIFT)&SBC_UJA_MASK_X_MASK)

/*!
 * @brief  Frame control register, identifier format macros.
 */
#define SBC_UJA_FRAME_CTR_IDE_MASK           (0x80U)
#define SBC_UJA_FRAME_CTR_IDE_SHIFT          (7U)
#define SBC_UJA_FRAME_CTR_IDE_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_FRAME_CTR_IDE_SHIFT)&SBC_UJA_FRAME_CTR_IDE_MASK)

/*!
 * @brief  Frame control register, partial networking data mask macros.
 */
#define SBC_UJA_FRAME_CTR_PNDM_MASK          (0x40U)
#define SBC_UJA_FRAME_CTR_PNDM_SHIFT         (6U)
#define SBC_UJA_FRAME_CTR_PNDM_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_FRAME_CTR_PNDM_SHIFT)&SBC_UJA_FRAME_CTR_PNDM_MASK)

/*!
 * @brief  Frame control register,  number of data bytes expected in a CAN frame macros.
 */
#define SBC_UJA_FRAME_CTR_DLC_MASK           (0x0FU)
#define SBC_UJA_FRAME_CTR_DLC_SHIFT          (0U)
#define SBC_UJA_FRAME_CTR_DLC_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_FRAME_CTR_DLC_SHIFT)&SBC_UJA_FRAME_CTR_DLC_MASK)

/*!
 * @brief  Frame control register.
 */
#define SBC_UJA_FRAME_CTR_MASK               (SBC_UJA_FRAME_CTR_IDE_MASK | SBC_UJA_FRAME_CTR_PNDM_MASK \
		                                       | SBC_UJA_FRAME_CTR_DLC_MASK)
#define SBC_UJA_FRAME_CTR_SHIFT              (0U)
#define SBC_UJA_FRAME_CTR_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_FRAME_CTR_SHIFT)&SBC_UJA_FRAME_CTR_MASK)

/*!
 * @brief  Data mask registers (0x68 to 0x6F) macros
 *         Data mask 0-7 configuration.
 */
#define SBC_UJA_DATA_MASK_X_MASK             (0xFFU)
#define SBC_UJA_DATA_MASK_X_SHIFT            (0U)
#define SBC_UJA_DATA_MASK_X_F(x)             ((uint8_t)((uint8_t)(x)<<SBC_UJA_DATA_MASK_X_SHIFT)&SBC_UJA_DATA_MASK_X_MASK)

/*!
 * @brief  WAKE pin status register, WAKE pin status macros.
 */
#define SBC_UJA_WAKE_STAT_WPVS_MASK          (0x02FU)
#define SBC_UJA_WAKE_STAT_WPVS_SHIFT         (1U)
#define SBC_UJA_WAKE_STAT_WPVS_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_STAT_WPVS_SHIFT)&SBC_UJA_WAKE_STAT_WPVS_MASK)

/*!
 * @brief  WAKE pin status register.
 */
#define SBC_UJA_WAKE_STAT_MASK               (SBC_UJA_WAKE_STAT_WPVS_MASK)
#define SBC_UJA_WAKE_STAT_SHIFT              (SBC_UJA_WAKE_STAT_WPVS_SHIFT)
#define SBC_UJA_WAKE_STAT_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_STAT_SHIFT)&SBC_UJA_WAKE_STAT_MASK)

/*!
 * @brief  WAKE pin event capture enable register,
 *         WAKE pin rising-edge enable macros.
 */
#define SBC_UJA_WAKE_EN_WPRE_MASK            (0x02U)
#define SBC_UJA_WAKE_EN_WPRE_SHIFT           (1U)
#define SBC_UJA_WAKE_EN_WPRE_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EN_WPRE_SHIFT)&SBC_UJA_WAKE_EN_WPRE_MASK)

/*!
 * @brief  WAKE pin event capture enable register,
 *         WAKE pin falling-edge enable macros.
 */
#define SBC_UJA_WAKE_EN_WPFE_MASK            (0x01U)
#define SBC_UJA_WAKE_EN_WPFE_SHIFT           (0U)
#define SBC_UJA_WAKE_EN_WPFE_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EN_WPFE_SHIFT)&SBC_UJA_WAKE_EN_WPFE_MASK)

/*!
 * @brief  WAKE pin event capture enable register macros.
 */
#define SBC_UJA_WAKE_EN_MASK                 (SBC_UJA_WAKE_EN_WPRE_MASK | SBC_UJA_WAKE_EN_WPFE_MASK)
#define SBC_UJA_WAKE_EN_SHIFT                (0U)
#define SBC_UJA_WAKE_EN_F(x)                 ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EN_SHIFT)&SBC_UJA_WAKE_EN_MASK)

/*!
 * @brief  Global event status register, WAKE pin event macros.
 */
#define SBC_UJA_GL_EVNT_STAT_WPE_MASK        (0x08U)
#define SBC_UJA_GL_EVNT_STAT_WPE_SHIFT       (3U)
#define SBC_UJA_GL_EVNT_STAT_WPE_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_GL_EVNT_STAT_WPE_SHIFT)&SBC_UJA_GL_EVNT_STAT_WPE_MASK)

/*!
 * @brief  Global event status register, transceiver event macros.
 */
#define SBC_UJA_GL_EVNT_STAT_TRXE_MASK       (0x04U)
#define SBC_UJA_GL_EVNT_STAT_TRXE_SHIFT      (2U)
#define SBC_UJA_GL_EVNT_STAT_TRXE_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_GL_EVNT_STAT_TRXE_SHIFT)&SBC_UJA_GL_EVNT_STAT_TRXE_MASK)

/*!
 * @brief  Global event status register, supply event macros.
 */
#define SBC_UJA_GL_EVNT_STAT_SUPE_MASK       (0x02U)
#define SBC_UJA_GL_EVNT_STAT_SUPE_SHIFT      (1U)
#define SBC_UJA_GL_EVNT_STAT_SUPE_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_GL_EVNT_STAT_SUPE_SHIFT)&SBC_UJA_GL_EVNT_STAT_SUPE_MASK)

/*!
 * @brief  Global event status register, system event macros.
 */
#define SBC_UJA_GL_EVNT_STAT_SYSE_MASK       (0x01U)
#define SBC_UJA_GL_EVNT_STAT_SYSE_SHIFT      (0U)
#define SBC_UJA_GL_EVNT_STAT_SYSE_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_GL_EVNT_STAT_SYSE_SHIFT)&SBC_UJA_GL_EVNT_STAT_SYSE_MASK)

/*!
 * @brief  Global event status register macros.
 */
#define SBC_UJA_GL_EVNT_STAT_MASK            (SBC_UJA_GL_EVNT_STAT_WPE_MASK | SBC_UJA_GL_EVNT_STAT_TRXE_MASK \
		                                       | SBC_UJA_GL_EVNT_STAT_SUPE_MASK | SBC_UJA_GL_EVNT_STAT_SYSE_MASK)
#define SBC_UJA_GL_EVNT_STAT_SHIFT           (0U)
#define SBC_UJA_GL_EVNT_STAT_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_GL_EVNT_STAT_SHIFT)&SBC_UJA_GL_EVNT_STAT_MASK)

/*!
 * @brief  System event status register, power-on macros.
 */
#define SBC_UJA_SYS_EVNT_STAT_PO_MASK        (0x10U)
#define SBC_UJA_SYS_EVNT_STAT_PO_SHIFT       (4U)
#define SBC_UJA_SYS_EVNT_STAT_PO_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_STAT_PO_SHIFT)&SBC_UJA_SYS_EVNT_STAT_PO_MASK)

/*!
 * @brief  System event status register, overtemperature warning macros.
 */
#define SBC_UJA_SYS_EVNT_STAT_OTW_MASK       (0x04U)
#define SBC_UJA_SYS_EVNT_STAT_OTW_SHIFT      (2U)
#define SBC_UJA_SYS_EVNT_STAT_OTW_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_STAT_OTW_SHIFT)&SBC_UJA_SYS_EVNT_STAT_OTW_MASK)

/*!
 * @brief  System event status register, SPI failure macros.
 */
#define SBC_UJA_SYS_EVNT_STAT_SPIF_MASK      (0x02U)
#define SBC_UJA_SYS_EVNT_STAT_SPIF_SHIFT     (1U)
#define SBC_UJA_SYS_EVNT_STAT_SPIF_F(x)      ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_STAT_SPIF_SHIFT)&SBC_UJA_SYS_EVNT_STAT_SPIF_MASK)

/*!
 * @brief  System event status register, watchdog failure macros.
 */
#define SBC_UJA_SYS_EVNT_STAT_WDF_MASK       (0x01U)
#define SBC_UJA_SYS_EVNT_STAT_WDF_SHIFT      (0U)
#define SBC_UJA_SYS_EVNT_STAT_WDF_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_STAT_WDF_SHIFT)&SBC_UJA_SYS_EVNT_STAT_WDF_MASK)

/*!
 * @brief  System event status register macros.
 */
#define SBC_UJA_SYS_EVNT_STAT_MASK           (SBC_UJA_SYS_EVNT_STAT_PO_MASK | SBC_UJA_SYS_EVNT_STAT_OTW_MASK \
		                                       | SBC_UJA_SYS_EVNT_STAT_SPIF_MASK | SBC_UJA_SYS_EVNT_STAT_WDF_MASK)
#define SBC_UJA_SYS_EVNT_STAT_SHIFT          (0U)
#define SBC_UJA_SYS_EVNT_STAT_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_SYS_EVNT_STAT_SHIFT)&SBC_UJA_SYS_EVNT_STAT_MASK)

/*!
 * @brief  Supply event status register, V2/VEXT overvoltage macros.
 */
#define SBC_UJA_SUP_EVNT_STAT_V2O_MASK       (0x04U)
#define SBC_UJA_SUP_EVNT_STAT_V2O_SHIFT      (2U)
#define SBC_UJA_SUP_EVNT_STAT_V2O_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUP_EVNT_STAT_V2O_SHIFT)&SBC_UJA_SUP_EVNT_STAT_V2O_MASK)

/*!
 * @brief  Supply event status register, V2/VEXT undervoltage macros.
 */
#define SBC_UJA_SUP_EVNT_STAT_V2U_MASK       (0x02U)
#define SBC_UJA_SUP_EVNT_STAT_V2U_SHIFT      (1U)
#define SBC_UJA_SUP_EVNT_STAT_V2U_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUP_EVNT_STAT_V2U_SHIFT)&SBC_UJA_SUP_EVNT_STAT_V2U_MASK)

/*!
 * @brief  Supply event status register, V1 undervoltage: macros.
 */
#define SBC_UJA_SUP_EVNT_STAT_V1U_MASK       (0x01U)
#define SBC_UJA_SUP_EVNT_STAT_V1U_SHIFT      (0U)
#define SBC_UJA_SUP_EVNT_STAT_V1U_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUP_EVNT_STAT_V1U_SHIFT)&SBC_UJA_SUP_EVNT_STAT_V1U_MASK)

/*!
 * @brief  Supply event status register macros.
 */
#define SBC_UJA_SUP_EVNT_STAT_MASK           (SBC_UJA_SUP_EVNT_STAT_V2O_MASK | SBC_UJA_SUP_EVNT_STAT_V2U_MASK \
		                                       | SBC_UJA_SUP_EVNT_STAT_V1U_MASK)
#define SBC_UJA_SUP_EVNT_STAT_SHIFT          (0U)
#define SBC_UJA_SUP_EVNT_STAT_F(x)           ((uint8_t)((uint8_t)(x)<<SBC_UJA_SUP_EVNT_STAT_SHIFT)&SBC_UJA_SUP_EVNT_STAT_MASK)

/*!
 * @brief  Transceiver event status register, partial networking frame
 *         detection error macros.
 */
#define SBC_UJA_TRANS_EVNT_STAT_PNFDE_MASK   (0x20U)
#define SBC_UJA_TRANS_EVNT_STAT_PNFDE_SHIFT  (5U)
#define SBC_UJA_TRANS_EVNT_STAT_PNFDE_F(x)   ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_STAT_PNFDE_SHIFT)&SBC_UJA_TRANS_EVNT_STAT_PNFDE_MASK)

/*!
 * @brief  Transceiver event status register, CAN-bus status  macros.
 */
#define SBC_UJA_TRANS_EVNT_STAT_CBS_MASK     (0x10U)
#define SBC_UJA_TRANS_EVNT_STAT_CBS_SHIFT    (4U)
#define SBC_UJA_TRANS_EVNT_STAT_CBS_F(x)     ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_STAT_CBS_SHIFT)&SBC_UJA_TRANS_EVNT_STAT_CBS_MASK)

/*!
 * @brief  Transceiver event status register, CAN failure.
 */
#define SBC_UJA_TRANS_EVNT_STAT_CF_MASK      (0x02U)
#define SBC_UJA_TRANS_EVNT_STAT_CF_SHIFT     (1U)
#define SBC_UJA_TRANS_EVNT_STAT_CF_F(x)      ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_STAT_CF_SHIFT)&SBC_UJA_TRANS_EVNT_STAT_CF_MASK)

/*!
 * @brief  Transceiver event status register, CAN wake-up.
 */
#define SBC_UJA_TRANS_EVNT_STAT_CW_MASK      (0x01U)
#define SBC_UJA_TRANS_EVNT_STAT_CW_SHIFT     (0U)
#define SBC_UJA_TRANS_EVNT_STAT_CW_F(x)      ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_STAT_CW_SHIFT)&SBC_UJA_TRANS_EVNT_STAT_CW_MASK)

/*!
 * @brief  Transceiver event status register macros.
 */
#define SBC_UJA_TRANS_EVNT_STAT_MASK         (SBC_UJA_TRANS_EVNT_STAT_PNFDE_MASK | SBC_UJA_TRANS_EVNT_STAT_CBS_MASK \
		                                       | SBC_UJA_TRANS_EVNT_STAT_CF_MASK | SBC_UJA_TRANS_EVNT_STAT_CW_MASK)
#define SBC_UJA_TRANS_EVNT_STAT_SHIFT        (0U)
#define SBC_UJA_TRANS_EVNT_STAT_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_TRANS_EVNT_STAT_SHIFT)&SBC_UJA_TRANS_EVNT_STAT_MASK)

/*!
 * @brief  WAKE pin event status register, WAKE pin rising edge macros.
 */
#define SBC_UJA_WAKE_EVNT_STAT_WPR_MASK      (0x02U)
#define SBC_UJA_WAKE_EVNT_STAT_WPR_SHIFT     (1U)
#define SBC_UJA_WAKE_EVNT_STAT_WPR_F(x)      ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EVNT_STAT_WPR_SHIFT)&SBC_UJA_WAKE_EVNT_STAT_WPR_MASK)

/*!
 * @brief  WAKE pin event status register, WAKE pin falling edge macros.
 */
#define SBC_UJA_WAKE_EVNT_STAT_WPF_MASK      (0x01U)
#define SBC_UJA_WAKE_EVNT_STAT_WPF_SHIFT     (0U)
#define SBC_UJA_WAKE_EVNT_STAT_WPF_F(x)      ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EVNT_STAT_WPF_SHIFT)&SBC_UJA_WAKE_EVNT_STAT_WPF_MASK)

/*!
 * @brief  WAKE pin event status register macros.
 */
#define SBC_UJA_WAKE_EVNT_STAT_MASK          (SBC_UJA_WAKE_EVNT_STAT_WPR_MASK | SBC_UJA_WAKE_EVNT_STAT_WPF_MASK)
#define SBC_UJA_WAKE_EVNT_STAT_SHIFT         (0U)
#define SBC_UJA_WAKE_EVNT_STAT_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_WAKE_EVNT_STAT_SHIFT)&SBC_UJA_WAKE_EVNT_STAT_MASK)

/*!
 * @brief   MTPNV status register, write counter status macros.
 */
#define SBC_UJA_MTPNV_STAT_WRCNTS_MASK       (0xFCU)
#define SBC_UJA_MTPNV_STAT_WRCNTS_SHIFT      (2U)
#define SBC_UJA_MTPNV_STAT_WRCNTS_F(x)       ((uint8_t)((uint8_t)(x)<<SBC_UJA_MTPNV_STAT_WRCNTS_SHIFT)&SBC_UJA_MTPNV_STAT_WRCNTS_MASK)

/*!
 * @brief   MTPNV status register, error correction code status.
 */
#define SBC_UJA_MTPNV_STAT_ECCS_MASK         (0x02U)
#define SBC_UJA_MTPNV_STAT_ECCS_SHIFT        (1U)
#define SBC_UJA_MTPNV_STAT_ECCS_F(x)         ((uint8_t)((uint8_t)(x)<<SBC_UJA_MTPNV_STAT_ECCS_SHIFT)&SBC_UJA_MTPNV_STAT_ECCS_MASK)

/*!
 * @brief   MTPNV status register, non-volatile memory programming status.
 */
#define SBC_UJA_MTPNV_STAT_NVMPS_MASK        (0x01U)
#define SBC_UJA_MTPNV_STAT_NVMPS_SHIFT       (0U)
#define SBC_UJA_MTPNV_STAT_NVMPS_F(x)        ((uint8_t)((uint8_t)(x)<<SBC_UJA_MTPNV_STAT_NVMPS_SHIFT)&SBC_UJA_MTPNV_STAT_NVMPS_MASK)

/*!
 * @brief   MTPNV status register macros.
 */
#define SBC_UJA_MTPNV_STAT_MASK              (SBC_UJA_MTPNV_STAT_WRCNTS_MASK | SBC_UJA_MTPNV_STAT_ECCS_MASK \
                                         | SBC_UJA_MTPNV_STAT_NVMPS_MASK)
#define SBC_UJA_MTPNV_STAT_SHIFT             (0U)
#define SBC_UJA_MTPNV_STAT_F(x)              ((uint8_t)((uint8_t)(x)<<SBC_UJA_MTPNV_STAT_SHIFT)&SBC_UJA_MTPNV_STAT_MASK)

/*!
 * @brief   Start-up control register, RSTN output reset pulse width macros.
 */
#define SBC_UJA_START_UP_RLC_MASK            (0x30U)
#define SBC_UJA_START_UP_RLC_SHIFT           (4U)
#define SBC_UJA_START_UP_RLC_F(x)            ((uint8_t)((uint8_t)(x)<<SBC_UJA_START_UP_RLC_SHIFT)&SBC_UJA_START_UP_RLC_MASK)

/*!
 * @brief   Start-up control register, V2/VEXT start-up control macros.
 */
#define SBC_UJA_START_UP_V2SUC_MASK          (0x08U)
#define SBC_UJA_START_UP_V2SUC_SHIFT         (3U)
#define SBC_UJA_START_UP_V2SUC_F(x)          ((uint8_t)((uint8_t)(x)<<SBC_UJA_START_UP_V2SUC_SHIFT)&SBC_UJA_START_UP_V2SUC_MASK)

/*!
 * @brief   Start-up control register macros.
 */
#define SBC_UJA_START_UP_MASK                (SBC_UJA_START_UP_RLC_MASK | SBC_UJA_START_UP_V2SUC_MASK)
#define SBC_UJA_START_UP_SHIFT               (3U)
#define SBC_UJA_START_UP_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_START_UP_SHIFT)&SBC_UJA_START_UP_MASK)

/*!
 * @brief   SBC configuration control register, V1 undervoltage threshold
 *          (defined by bit V1RTC) at start-up macros.
 */
#define SBC_UJA_SBC_V1RTSUC_MASK             (0x30U)
#define SBC_UJA_SBC_V1RTSUC_SHIFT            (4U)
#define SBC_UJA_SBC_V1RTSUC_F(x)             ((uint8_t)((uint8_t)(x)<<SBC_UJA_SBC_V1RTSUC_SHIFT)&SBC_UJA_SBC_V1RTSUC_MASK)

/*!
 * @brief   SBC configuration control register,
 *          Forced Normal mode control macros.
 */
#define SBC_UJA_SBC_FNMC_MASK                (0x08U)
#define SBC_UJA_SBC_FNMC_SHIFT               (3U)
#define SBC_UJA_SBC_FNMC_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_SBC_FNMC_SHIFT)&SBC_UJA_SBC_FNMC_MASK)

/*!
 * @brief   SBC configuration control register,
 *          Software Development mode control macros.
 */
#define SBC_UJA_SBC_SDMC_MASK                (0x04U)
#define SBC_UJA_SBC_SDMC_SHIFT               (2U)
#define SBC_UJA_SBC_SDMC_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_SBC_SDMC_SHIFT)&SBC_UJA_SBC_SDMC_MASK)

/*!
 * @brief   SBC configuration control register, Sleep control macros.
 */
#define SBC_UJA_SBC_SLPC_MASK                (0x01U)
#define SBC_UJA_SBC_SLPC_SHIFT               (0U)
#define SBC_UJA_SBC_SLPC_F(x)                ((uint8_t)((uint8_t)(x)<<SBC_UJA_SBC_SLPC_SHIFT)&SBC_UJA_SBC_SLPC_MASK)

/*!
 * @brief   SBC configuration control register macros.
 */
#define SBC_UJA_SBC_MASK                     (SBC_UJA_SBC_V1RTSUC_MASK | SBC_UJA_SBC_FNMC_MASK \
                                            | SBC_UJA_SBC_SDMC_MASK| SBC_UJA_SBC_SLPC_MASK)
#define SBC_UJA_SBC_SHIFT                    (0U)
#define SBC_UJA_SBC_F(x)                     ((uint8_t)((uint8_t)(x)<<SBC_UJA_SBC_SHIFT)&SBC_UJA_SBC_MASK)

/*!
 * @brief   MTPNV CRC control register  macros.
 */
#define SBC_UJA_MTPNV_CRC_MASK               (0xFFU)
#define SBC_UJA_MTPNV_CRC_SHIFT              (0U)
#define SBC_UJA_MTPNV_CRC_F(x)               ((uint8_t)((uint8_t)(x)<<SBC_UJA_MTPNV_CRC_SHIFT)&SBC_UJA_MTPNV_CRC_MASK)

/*!
 * @brief   Device identification register macros.
 */
#define SBC_UJA_IDENTIF_MASK                 (0xFFU)
#define SBC_UJA_IDENTIF_SHIFT                (0U)
#define SBC_UJA_IDENTIF_F(x)                 ((uint8_t)((uint8_t)(x)<<SBC_UJA_IDENTIF_SHIFT)&SBC_UJA_IDENTIF_MASK)

#endif /* SOURCES_SBC_UJA_116X_H_ */
