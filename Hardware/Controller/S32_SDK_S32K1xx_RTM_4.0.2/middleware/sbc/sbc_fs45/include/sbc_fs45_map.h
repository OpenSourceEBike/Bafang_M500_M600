/*
 * Copyright 2019 NXP
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
 * @sbc_fs45_map.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Macro not referenced
 * Macros which are not currently referenced in code but can be used in the
 * future or by user.
 */

#ifndef SBC_FS45_MAP_H__
#define SBC_FS45_MAP_H__

/******************************************************************************/
/* INIT_VREG - Type: RW                                                       */
/******************************************************************************/

#define FS45_M_INIT_VREG_ADDR            0x01U
#define FS45_M_INIT_VREG_DEFAULT         0x45U

/**
 * Report a battery disconnection (POR of the main logic)
 */
#define FS45_R_M_BAT_FAIL_MASK           0x01U
/**
 * Configure VAUX regulator as a tracker of VCCA
 */
#define FS45_RW_M_VAUX_TRK_EN_MASK       0x02U
/**
 * Configure the current limitation duration before VAUX is switched off.
 */
#define FS45_RW_M_TAUX_LIM_OFF_MASK      0x04U
/**
 * CAN_5V overvoltage monitoring
 */
#define FS45_RW_M_VCAN_OV_MON_MASK       0x10U
/**
 * DISABLE the input power feed forward (IPFF) function of VPRE
 */
#define FS45_RW_M_IPFF_DIS_MASK          0x20U
/**
 * Configure the current limitation duration before VCCA is switched off. Only available for external PNP.
 */
#define FS45_RW_M_TCCA_LIM_OFF_MASK      0x40U
/**
 * Configure the current limitation threshold for VCCA. Only available for external PNP.
 */
#define FS45_RW_M_ICCA_LIM_MASK          0x80U

/**
 * Report a battery disconnection (POR of the main logic)
 */
#define FS45_R_M_BAT_FAIL_SHIFT          0x00U
/**
 * Configure VAUX regulator as a tracker of VCCA
 */
#define FS45_RW_M_VAUX_TRK_EN_SHIFT      0x01U
/**
 * Configure the current limitation duration before VAUX is switched off.
 */
#define FS45_RW_M_TAUX_LIM_OFF_SHIFT     0x02U
/**
 * CAN_5V overvoltage monitoring
 */
#define FS45_RW_M_VCAN_OV_MON_SHIFT      0x04U
/**
 * DISABLE the input power feed forward (IPFF) function of VPRE
 */
#define FS45_RW_M_IPFF_DIS_SHIFT         0x05U
/**
 * Configure the current limitation duration before VCCA is switched off. Only available for external PNP.
 */
#define FS45_RW_M_TCCA_LIM_OFF_SHIFT     0x06U
/**
 * Configure the current limitation threshold for VCCA. Only available for external PNP.
 */
#define FS45_RW_M_ICCA_LIM_SHIFT         0x07U

/**
 * NO POR
 */
#define FS45_R_M_BAT_FAIL_NO_POR         (0x00U << FS45_R_M_BAT_FAIL_SHIFT)
/**
 * POR occurred
 */
#define FS45_R_M_BAT_FAIL_POR            (0x01U << FS45_R_M_BAT_FAIL_SHIFT)

/**
 * NO tracking
 */
#define FS45_RW_M_VAUX_TRK_EN_NO_TRACKING (0x00U << FS45_RW_M_VAUX_TRK_EN_SHIFT)
/**
 * Tracking mode enabled and latched
 */
#define FS45_RW_M_VAUX_TRK_EN_TRACKING   (0x01U << FS45_RW_M_VAUX_TRK_EN_SHIFT)

/**
 * 10 ms
 */
#define FS45_RW_M_TAUX_LIM_OFF_10_MS     (0x00U << FS45_RW_M_TAUX_LIM_OFF_SHIFT)
/**
 * 50 ms
 */
#define FS45_RW_M_TAUX_LIM_OFF_50_MS     (0x01U << FS45_RW_M_TAUX_LIM_OFF_SHIFT)

/**
 * Off. VCAN OV is not monitored. Flag is ignored.
 */
#define FS45_RW_M_VCAN_OV_MON_OFF        (0x00U << FS45_RW_M_VCAN_OV_MON_SHIFT)
/**
 * On. VCAN OV is monitored. If OV the CAN_5V regulator is switched off.
 */
#define FS45_RW_M_VCAN_OV_MON_ON         (0x01U << FS45_RW_M_VCAN_OV_MON_SHIFT)

/**
 * Enabled
 */
#define FS45_RW_M_IPFF_DIS_ENABLED       (0x00U << FS45_RW_M_IPFF_DIS_SHIFT)
/**
 * Disabled
 */
#define FS45_RW_M_IPFF_DIS_DISABLED      (0x01U << FS45_RW_M_IPFF_DIS_SHIFT)

/**
 * 10 ms
 */
#define FS45_RW_M_TCCA_LIM_OFF_10_MS     (0x00U << FS45_RW_M_TCCA_LIM_OFF_SHIFT)
/**
 * 50 ms
 */
#define FS45_RW_M_TCCA_LIM_OFF_50_MS     (0x01U << FS45_RW_M_TCCA_LIM_OFF_SHIFT)

/**
 * ICCA_LIM_OUT
 */
#define FS45_RW_M_ICCA_LIM_ICCA_LIM_OUT  (0x00U << FS45_RW_M_ICCA_LIM_SHIFT)
/**
 * ICCA_LIM_INT
 */
#define FS45_RW_M_ICCA_LIM_ICCA_LIM_INT  (0x01U << FS45_RW_M_ICCA_LIM_SHIFT)

/******************************************************************************/
/* LONG_DURATION_TIMER - Type: RW                                             */
/******************************************************************************/

#define FS45_M_LONG_DURATION_TIMER_ADDR  0x06U
#define FS45_M_LONG_DURATION_TIMER_DEFAULT 0x04U

/**
 * Counter status
 */
#define FS45_R_M_LDT_INT_MASK            0x01U
/**
 * LDT counter control
 */
#define FS45_RW_M_LDT_ENABLE_MASK        0x02U
/**
 * Operating mode selection
 */
#define FS45_RW_M_MODE_MASK              0x04U
/**
 * Counter status
 */
#define FS45_R_M_LDT_RUNNING_MASK        0x08U
/**
 * Counter register selection
 */
#define FS45_RW_M_REG_SE_MASK            0x10U
/**
 * Select timer operating function
 */
#define FS45_RW_M_F2_F0_MASK             0xE0U

/**
 * Counter status
 */
#define FS45_R_M_LDT_INT_SHIFT           0x00U
/**
 * LDT counter control
 */
#define FS45_RW_M_LDT_ENABLE_SHIFT       0x01U
/**
 * Operating mode selection
 */
#define FS45_RW_M_MODE_SHIFT             0x02U
/**
 * Counter status
 */
#define FS45_R_M_LDT_RUNNING_SHIFT       0x03U
/**
 * Counter register selection
 */
#define FS45_RW_M_REG_SE_SHIFT           0x04U
/**
 * Select timer operating function
 */
#define FS45_RW_M_F2_F0_SHIFT            0x05U

/**
 * Counter not running
 */
#define FS45_R_M_LDT_INT_NOT_RUNNING     (0x00U << FS45_R_M_LDT_INT_SHIFT)
/**
 * Counter running
 */
#define FS45_R_M_LDT_INT_RUNNING         (0x01U << FS45_R_M_LDT_INT_SHIFT)

/**
 * LDT counter stop
 */
#define FS45_RW_M_LDT_ENABLE_STOP        (0x00U << FS45_RW_M_LDT_ENABLE_SHIFT)
/**
 * LDT counter start
 */
#define FS45_RW_M_LDT_ENABLE_START       (0x01U << FS45_RW_M_LDT_ENABLE_SHIFT)

/**
 * Calibration mode (488 us resolution)
 */
#define FS45_RW_M_MODE_CALIBRATION       (0x00U << FS45_RW_M_MODE_SHIFT)
/**
 * Normal mode (1 s resolution)
 */
#define FS45_RW_M_MODE_NORMAL            (0x01U << FS45_RW_M_MODE_SHIFT)

/**
 * Counter not running
 */
#define FS45_R_M_LDT_RUNNING_NOT_RUNNING (0x00U << FS45_R_M_LDT_RUNNING_SHIFT)
/**
 * Counter running
 */
#define FS45_R_M_LDT_RUNNING_RUNNING     (0x01U << FS45_R_M_LDT_RUNNING_SHIFT)

/**
 * Read programmed wake-up register
 */
#define FS45_RW_M_REG_SE_PROGRAMMED_REG  (0x00U << FS45_RW_M_REG_SE_SHIFT)
/**
 * Read real time counter into wake-up register (after counter is stopped with LDT_ENABLE bit)
 */
#define FS45_RW_M_REG_SE_RTC_REG         (0x01U << FS45_RW_M_REG_SE_SHIFT)

/**
 * In normal mode count and generate flag or INT when counter reaches the after run value
 */
#define FS45_RW_M_F2_F0_FUNCTION1        (0x00U << FS45_RW_M_F2_F0_SHIFT)
/**
 * In normal mode count until after run value is reached, then enters in LPOFF
 */
#define FS45_RW_M_F2_F0_FUNCTION2        (0x01U << FS45_RW_M_F2_F0_SHIFT)
/**
 * In normal mode count until after run value is reached, then enters in LPOFF. Once in LPOFF, count until wake-up 
 * value is reached and wake-up
 */
#define FS45_RW_M_F2_F0_FUNCTION3        (0x02U << FS45_RW_M_F2_F0_SHIFT)
/**
 * In LPOFF, count until wake-up value is reached and wake-up
 */
#define FS45_RW_M_F2_F0_FUNCTION4        (0x03U << FS45_RW_M_F2_F0_SHIFT)
/**
 * In LPOFF, count and do not wake-up. Counter value is stored in wake-up register
 */
#define FS45_RW_M_F2_F0_FUNCTION5        (0x04U << FS45_RW_M_F2_F0_SHIFT)

/******************************************************************************/
/* HW_CONFIG - Type: R                                                        */
/******************************************************************************/

#define FS45_M_HW_CONFIG_ADDR            0x08U
#define FS45_M_HW_CONFIG_DEFAULT         0x04U

/**
 * Report the configuration of the DEBUG mode
 */
#define FS45_R_M_DBG_HW_MASK             0x01U
/**
 * Report the deep fail-safe hardware configuration (main logic)
 */
#define FS45_R_M_DFS_HW1_MASK            0x02U
/**
 * Report the hardware configuration for VAUX
 */
#define FS45_R_M_VAUX_HW_MASK            0x08U
/**
 * Report the hardware configuration for VCCA
 */
#define FS45_R_M_VCCA_HW_MASK            0x10U
/**
 * Report the connection of an external PNP on VCCA
 */
#define FS45_R_M_VCCA_PNP_DET_MASK       0x20U
/**
 * Report the hardware configuration of VPRE
 */
#define FS45_R_M_LS_DETECT_MASK          0x80U

/**
 * Report the configuration of the DEBUG mode
 */
#define FS45_R_M_DBG_HW_SHIFT            0x00U
/**
 * Report the deep fail-safe hardware configuration (main logic)
 */
#define FS45_R_M_DFS_HW1_SHIFT           0x01U
/**
 * Report the hardware configuration for VAUX
 */
#define FS45_R_M_VAUX_HW_SHIFT           0x03U
/**
 * Report the hardware configuration for VCCA
 */
#define FS45_R_M_VCCA_HW_SHIFT           0x04U
/**
 * Report the connection of an external PNP on VCCA
 */
#define FS45_R_M_VCCA_PNP_DET_SHIFT      0x05U
/**
 * Report the hardware configuration of VPRE
 */
#define FS45_R_M_LS_DETECT_SHIFT         0x07U

/**
 * Normal operation
 */
#define FS45_R_M_DBG_HW_NORMAL           (0x00U << FS45_R_M_DBG_HW_SHIFT)
/**
 * Debug mode selected
 */
#define FS45_R_M_DBG_HW_DEBUG            (0x01U << FS45_R_M_DBG_HW_SHIFT)

/**
 * Deep fail-safe disable
 */
#define FS45_R_M_DFS_HW1_DISABLE         (0x00U << FS45_R_M_DFS_HW1_SHIFT)
/**
 * Deep fail-safe enable
 */
#define FS45_R_M_DFS_HW1_ENABLE          (0x01U << FS45_R_M_DFS_HW1_SHIFT)

/**
 * 5.0 V
 */
#define FS45_R_M_VAUX_HW_5_0V            (0x00U << FS45_R_M_VAUX_HW_SHIFT)
/**
 * 3.3 V
 */
#define FS45_R_M_VAUX_HW_3_3V            (0x01U << FS45_R_M_VAUX_HW_SHIFT)

/**
 * 3.3 V
 */
#define FS45_R_M_VCCA_HW_3_3V            (0x00U << FS45_R_M_VCCA_HW_SHIFT)
/**
 * 5.0 V
 */
#define FS45_R_M_VCCA_HW_5_0V            (0x01U << FS45_R_M_VCCA_HW_SHIFT)

/**
 * External PNP connected
 */
#define FS45_R_M_VCCA_PNP_DET_PNP_CONNECTED (0x00U << FS45_R_M_VCCA_PNP_DET_SHIFT)
/**
 * Internal MOSFET
 */
#define FS45_R_M_VCCA_PNP_DET_INT_MOSFET (0x01U << FS45_R_M_VCCA_PNP_DET_SHIFT)

/**
 * Buck-boost
 */
#define FS45_R_M_LS_DETECT_BUCK_BOOST    (0x00U << FS45_R_M_LS_DETECT_SHIFT)
/**
 * Buck only
 */
#define FS45_R_M_LS_DETECT_BUCK_ONLY     (0x01U << FS45_R_M_LS_DETECT_SHIFT)

/******************************************************************************/
/* WU_SOURCE - Type: R                                                        */
/******************************************************************************/

#define FS45_M_WU_SOURCE_ADDR            0x09U
#define FS45_M_WU_SOURCE_DEFAULT         0x00U

/**
 * Report a wake-up event from CAN or LIN
 */
#define FS45_R_M_PHY_WU_MASK             0x01U
/**
 * Report a wake-up event from long duration timer
 */
#define FS45_R_M_LDT_WU_MASK             0x02U
/**
 * Report an automatic wake-up event
 */
#define FS45_R_M_AUTO_WU_MASK            0x04U
/**
 * Report a wake-up event from IO_0
 */
#define FS45_R_M_IO_0_WU_MASK            0x08U
/**
 * Report a wake-up event from IO_2
 */
#define FS45_R_M_IO_2_WU_MASK            0x10U
/**
 * Report a wake-up event from IO_3
 */
#define FS45_R_M_IO_3_WU_MASK            0x20U
/**
 * Report a wake-up event from IO_4
 */
#define FS45_R_M_IO_4_WU_MASK            0x40U
/**
 * Report a wake-up event from IO_5
 */
#define FS45_R_M_IO_5_WU_MASK            0x80U

/**
 * Report a wake-up event from CAN or LIN
 */
#define FS45_R_M_PHY_WU_SHIFT            0x00U
/**
 * Report a wake-up event from long duration timer
 */
#define FS45_R_M_LDT_WU_SHIFT            0x01U
/**
 * Report an automatic wake-up event
 */
#define FS45_R_M_AUTO_WU_SHIFT           0x02U
/**
 * Report a wake-up event from IO_0
 */
#define FS45_R_M_IO_0_WU_SHIFT           0x03U
/**
 * Report a wake-up event from IO_2
 */
#define FS45_R_M_IO_2_WU_SHIFT           0x04U
/**
 * Report a wake-up event from IO_3
 */
#define FS45_R_M_IO_3_WU_SHIFT           0x05U
/**
 * Report a wake-up event from IO_4
 */
#define FS45_R_M_IO_4_WU_SHIFT           0x06U
/**
 * Report a wake-up event from IO_5
 */
#define FS45_R_M_IO_5_WU_SHIFT           0x07U

/**
 * No wake-up
 */
#define FS45_R_M_PHY_WU_NO_EVENT         (0x00U << FS45_R_M_PHY_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_PHY_WU_EVENT            (0x01U << FS45_R_M_PHY_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_LDT_WU_NO_EVENT         (0x00U << FS45_R_M_LDT_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_LDT_WU_EVENT            (0x01U << FS45_R_M_LDT_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_AUTO_WU_NO_EVENT        (0x00U << FS45_R_M_AUTO_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_AUTO_WU_EVENT           (0x01U << FS45_R_M_AUTO_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_IO_0_WU_NO_EVENT        (0x00U << FS45_R_M_IO_0_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_IO_0_WU_EVENT           (0x01U << FS45_R_M_IO_0_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_IO_2_WU_NO_EVENT        (0x00U << FS45_R_M_IO_2_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_IO_2_WU_EVENT           (0x01U << FS45_R_M_IO_2_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_IO_3_WU_NO_EVENT        (0x00U << FS45_R_M_IO_3_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_IO_3_WU_EVENT           (0x01U << FS45_R_M_IO_3_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_IO_4_WU_NO_EVENT        (0x00U << FS45_R_M_IO_4_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_IO_4_WU_EVENT           (0x01U << FS45_R_M_IO_4_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_IO_5_WU_NO_EVENT        (0x00U << FS45_R_M_IO_5_WU_SHIFT)
/**
 * Wake-up event detected
 */
#define FS45_R_M_IO_5_WU_EVENT           (0x01U << FS45_R_M_IO_5_WU_SHIFT)

/******************************************************************************/
/* DEVICE_ID - Type: R                                                        */
/******************************************************************************/

#define FS45_M_DEVICE_ID_ADDR            0x0AU
#define FS45_M_DEVICE_ID_DEFAULT         0x00U

/**
 * Device silicon revision
 */
#define FS45_R_M_DEV_REV_MASK            0x07U
/**
 * VKAM supply
 */
#define FS45_R_M_VKAM_MASK               0x08U
/**
 * CAN or LIN physical layer
 */
#define FS45_R_M_PHY_MASK                0x30U
/**
 * VCORE current capability
 */
#define FS45_R_M_VCORE_MASK              0xC0U

/**
 * Device silicon revision
 */
#define FS45_R_M_DEV_REV_SHIFT           0x00U
/**
 * VKAM supply
 */
#define FS45_R_M_VKAM_SHIFT              0x03U
/**
 * CAN or LIN physical layer
 */
#define FS45_R_M_PHY_SHIFT               0x04U
/**
 * VCORE current capability
 */
#define FS45_R_M_VCORE_SHIFT             0x06U

/**
 * Silicon Rev. 000
 */
#define FS45_R_M_DEV_REV_REV_000         (0x00U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 001
 */
#define FS45_R_M_DEV_REV_REV_001         (0x01U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 010
 */
#define FS45_R_M_DEV_REV_REV_010         (0x02U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 011
 */
#define FS45_R_M_DEV_REV_REV_011         (0x03U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 100
 */
#define FS45_R_M_DEV_REV_REV_100         (0x04U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 101
 */
#define FS45_R_M_DEV_REV_REV_101         (0x05U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 110
 */
#define FS45_R_M_DEV_REV_REV_110         (0x06U << FS45_R_M_DEV_REV_SHIFT)
/**
 * Silicon Rev. 111
 */
#define FS45_R_M_DEV_REV_REV_111         (0x07U << FS45_R_M_DEV_REV_SHIFT)

/**
 * VKAM off by default
 */
#define FS45_R_M_VKAM_OFF                (0x00U << FS45_R_M_VKAM_SHIFT)
/**
 * VKAM on by default
 */
#define FS45_R_M_VKAM_ON                 (0x01U << FS45_R_M_VKAM_SHIFT)

/**
 * No CAN/no LIN
 */
#define FS45_R_M_PHY_NOCAN_NOLIN         (0x00U << FS45_R_M_PHY_SHIFT)
/**
 * CAN only
 */
#define FS45_R_M_PHY_CAN                 (0x01U << FS45_R_M_PHY_SHIFT)
/**
 * LIN only
 */
#define FS45_R_M_PHY_LIN                 (0x02U << FS45_R_M_PHY_SHIFT)
/**
 * CAN and LIN
 */
#define FS45_R_M_PHY_CAN_LIN             (0x03U << FS45_R_M_PHY_SHIFT)

/**
 * 1.5 A
 */
#define FS45_R_M_VCORE_1_5A              (0x00U << FS45_R_M_VCORE_SHIFT)
/**
 * 0.8 A
 */
#define FS45_R_M_VCORE_0_8A              (0x01U << FS45_R_M_VCORE_SHIFT)
/**
 * 0.5 A
 */
#define FS45_R_M_VCORE_0_5A              (0x02U << FS45_R_M_VCORE_SHIFT)
/**
 * 2.2 A
 */
#define FS45_R_M_VCORE_2_2A              (0x03U << FS45_R_M_VCORE_SHIFT)

/******************************************************************************/
/* IO_INPUT - Type: R                                                         */
/******************************************************************************/

#define FS45_M_IO_INPUT_ADDR             0x0BU
#define FS45_M_IO_INPUT_DEFAULT          0x00U

/**
 * Report IO_0 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_0_MASK               0x01U
/**
 * Report IO_2 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_2_MASK               0x08U
/**
 * Report IO_3 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_3_MASK               0x10U
/**
 * Report IO_4 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_4_MASK               0x40U
/**
 * Report IO_5 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_5_MASK               0x80U

/**
 * Report IO_0 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_0_SHIFT              0x00U
/**
 * Report IO_2 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_2_SHIFT              0x03U
/**
 * Report IO_3 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_3_SHIFT              0x04U
/**
 * Report IO_4 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_4_SHIFT              0x06U
/**
 * Report IO_5 digital state in normal mode. No update in LPOFF mode since wake-up features available
 */
#define FS45_R_M_IO_5_SHIFT              0x07U

/**
 * Low
 */
#define FS45_R_M_IO_0_LOW                (0x00U << FS45_R_M_IO_0_SHIFT)
/**
 * High
 */
#define FS45_R_M_IO_0_HIGH               (0x01U << FS45_R_M_IO_0_SHIFT)

/**
 * Low
 */
#define FS45_R_M_IO_2_LOW                (0x00U << FS45_R_M_IO_2_SHIFT)
/**
 * High
 */
#define FS45_R_M_IO_2_HIGH               (0x01U << FS45_R_M_IO_2_SHIFT)

/**
 * Low
 */
#define FS45_R_M_IO_3_LOW                (0x00U << FS45_R_M_IO_3_SHIFT)
/**
 * High
 */
#define FS45_R_M_IO_3_HIGH               (0x01U << FS45_R_M_IO_3_SHIFT)

/**
 * Low
 */
#define FS45_R_M_IO_4_LOW                (0x00U << FS45_R_M_IO_4_SHIFT)
/**
 * High
 */
#define FS45_R_M_IO_4_HIGH               (0x01U << FS45_R_M_IO_4_SHIFT)

/**
 * Low
 */
#define FS45_R_M_IO_5_LOW                (0x00U << FS45_R_M_IO_5_SHIFT)
/**
 * High
 */
#define FS45_R_M_IO_5_HIGH               (0x01U << FS45_R_M_IO_5_SHIFT)

/******************************************************************************/
/* DIAG_VPRE - Type: R                                                        */
/******************************************************************************/

#define FS45_M_DIAG_VPRE_ADDR            0x0CU
#define FS45_M_DIAG_VPRE_DEFAULT         0x44U

/**
 * Report a current limitation condition on VPRE
 */
#define FS45_R_M_ILIM_PRE_MASK           0x02U
/**
 * VPRE undervoltage detection
 */
#define FS45_R_M_VPRE_UV_MASK            0x04U
/**
 * VPRE overvoltage detection
 */
#define FS45_R_M_VPRE_OV_MASK            0x08U
/**
 * Thermal shutdown of VPRE
 */
#define FS45_R_M_TSD_PRE_MASK            0x10U
/**
 * Report a thermal warning from VPRE
 */
#define FS45_R_M_TWARN_PRE_MASK          0x20U
/**
 * Report the activation state of VPRE SMPS
 */
#define FS45_R_M_VPRE_STATE_MASK         0x40U
/**
 * Report a running mode of VPRE
 */
#define FS45_R_M_BOB_MASK                0x80U

/**
 * Report a current limitation condition on VPRE
 */
#define FS45_R_M_ILIM_PRE_SHIFT          0x01U
/**
 * VPRE undervoltage detection
 */
#define FS45_R_M_VPRE_UV_SHIFT           0x02U
/**
 * VPRE overvoltage detection
 */
#define FS45_R_M_VPRE_OV_SHIFT           0x03U
/**
 * Thermal shutdown of VPRE
 */
#define FS45_R_M_TSD_PRE_SHIFT           0x04U
/**
 * Report a thermal warning from VPRE
 */
#define FS45_R_M_TWARN_PRE_SHIFT         0x05U
/**
 * Report the activation state of VPRE SMPS
 */
#define FS45_R_M_VPRE_STATE_SHIFT        0x06U
/**
 * Report a running mode of VPRE
 */
#define FS45_R_M_BOB_SHIFT               0x07U

/**
 * No current limitation (IPRE_PK < IPRE_LIM)
 */
#define FS45_R_M_ILIM_PRE_NO_LIMITATION  (0x00U << FS45_R_M_ILIM_PRE_SHIFT)
/**
 * Current limitation (IPRE_PK > IPRE_LIM)
 */
#define FS45_R_M_ILIM_PRE_LIMITATION     (0x01U << FS45_R_M_ILIM_PRE_SHIFT)

/**
 * No undervoltage (VPRE > VPRE_UV)
 */
#define FS45_R_M_VPRE_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_M_VPRE_UV_SHIFT)
/**
 * Undervoltage detected (VPRE < VPRE_UV)
 */
#define FS45_R_M_VPRE_UV_UNDERVOLTAGE    (0x01U << FS45_R_M_VPRE_UV_SHIFT)

/**
 * No overvoltage (VPRE < VPRE_OV)
 */
#define FS45_R_M_VPRE_OV_NO_OVERVOLTAGE  (0x00U << FS45_R_M_VPRE_OV_SHIFT)
/**
 * Overvoltage detected (VPRE > VPRE_OV)
 */
#define FS45_R_M_VPRE_OV_OVERVOLTAGE     (0x01U << FS45_R_M_VPRE_OV_SHIFT)

/**
 * No TSD (TJ < TSD_PRE)
 */
#define FS45_R_M_TSD_PRE_NO_TSD          (0x00U << FS45_R_M_TSD_PRE_SHIFT)
/**
 * TSD occurred (TJ > TSD_PRE)
 */
#define FS45_R_M_TSD_PRE_TSD_OCCURRED    (0x01U << FS45_R_M_TSD_PRE_SHIFT)

/**
 * No thermal warning (TJ < TWARN_PRE)
 */
#define FS45_R_M_TWARN_PRE_NO_WARNING    (0x00U << FS45_R_M_TWARN_PRE_SHIFT)
/**
 * Thermal warning (TJ > TWARN_PRE)
 */
#define FS45_R_M_TWARN_PRE_WARNING       (0x01U << FS45_R_M_TWARN_PRE_SHIFT)

/**
 * SMPS off
 */
#define FS45_R_M_VPRE_STATE_OFF          (0x00U << FS45_R_M_VPRE_STATE_SHIFT)
/**
 * SMPS on
 */
#define FS45_R_M_VPRE_STATE_ON           (0x01U << FS45_R_M_VPRE_STATE_SHIFT)

/**
 * Buck
 */
#define FS45_R_M_BOB_BUCK                (0x00U << FS45_R_M_BOB_SHIFT)
/**
 * Boost
 */
#define FS45_R_M_BOB_BOOST               (0x01U << FS45_R_M_BOB_SHIFT)

/******************************************************************************/
/* DIAG_VCORE - Type: R                                                       */
/******************************************************************************/

#define FS45_M_DIAG_VCORE_ADDR           0x0DU
#define FS45_M_DIAG_VCORE_DEFAULT        0x44U

/**
 * VCORE undervoltage detection
 */
#define FS45_R_M_VCORE_FB_UV_MASK        0x04U
/**
 * VCORE overvoltage detection
 */
#define FS45_R_M_VCORE_FB_OV_MASK        0x08U
/**
 * Thermal shutdown of VCORE
 */
#define FS45_R_M_TSD_CORE_MASK           0x10U
/**
 * Report a thermal warning from VCORE
 */
#define FS45_R_M_TWARN_CORE_MASK         0x20U
/**
 * Report the activation state of VCORE SMPS
 */
#define FS45_R_M_VCORE_STATE_MASK        0x40U

/**
 * VCORE undervoltage detection
 */
#define FS45_R_M_VCORE_FB_UV_SHIFT       0x02U
/**
 * VCORE overvoltage detection
 */
#define FS45_R_M_VCORE_FB_OV_SHIFT       0x03U
/**
 * Thermal shutdown of VCORE
 */
#define FS45_R_M_TSD_CORE_SHIFT          0x04U
/**
 * Report a thermal warning from VCORE
 */
#define FS45_R_M_TWARN_CORE_SHIFT        0x05U
/**
 * Report the activation state of VCORE SMPS
 */
#define FS45_R_M_VCORE_STATE_SHIFT       0x06U

/**
 * No undervoltage (VCORE_FB > VCORE_FB_UV)
 */
#define FS45_R_M_VCORE_FB_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_M_VCORE_FB_UV_SHIFT)
/**
 * Undervoltage (VCORE_FB < VCORE_FB_UV)
 */
#define FS45_R_M_VCORE_FB_UV_UNDERVOLTAGE (0x01U << FS45_R_M_VCORE_FB_UV_SHIFT)

/**
 * No overvoltage (VCORE_FB < VCORE_FB_OV)
 */
#define FS45_R_M_VCORE_FB_OV_NO_OVERVOLTAGE (0x00U << FS45_R_M_VCORE_FB_OV_SHIFT)
/**
 * Overvoltage detected (VPRE > VPRE_OV)
 */
#define FS45_R_M_VCORE_FB_OV_OVERVOLTAGE (0x01U << FS45_R_M_VCORE_FB_OV_SHIFT)

/**
 * No TSD (TJ < TSD_PRE)
 */
#define FS45_R_M_TSD_CORE_NO_TSD         (0x00U << FS45_R_M_TSD_CORE_SHIFT)
/**
 * TSD occurred (TJ > TSD_CORE)
 */
#define FS45_R_M_TSD_CORE_TSD_OCCURRED   (0x01U << FS45_R_M_TSD_CORE_SHIFT)

/**
 * No thermal warning (TJ < TWARN_CORE)
 */
#define FS45_R_M_TWARN_CORE_NO_WARNING   (0x00U << FS45_R_M_TWARN_CORE_SHIFT)
/**
 * Thermal warning (TJ > TWARN_CORE)
 */
#define FS45_R_M_TWARN_CORE_WARNING      (0x01U << FS45_R_M_TWARN_CORE_SHIFT)

/**
 * SMPS off
 */
#define FS45_R_M_VCORE_STATE_OFF         (0x00U << FS45_R_M_VCORE_STATE_SHIFT)
/**
 * SMPS on
 */
#define FS45_R_M_VCORE_STATE_ON          (0x01U << FS45_R_M_VCORE_STATE_SHIFT)

/******************************************************************************/
/* DIAG_VCCA - Type: R                                                        */
/******************************************************************************/

#define FS45_M_DIAG_VCCA_ADDR            0x0EU
#define FS45_M_DIAG_VCCA_DEFAULT         0x04U

/**
 * Maximum current limitation duration. Available only when an external PNP is connected
 */
#define FS45_R_M_ILIM_CCA_OFF_MASK       0x01U
/**
 * Report a current limitation condition on VCCA
 */
#define FS45_R_M_ILIM_CCA_MASK           0x02U
/**
 * VCCA undervoltage detection
 */
#define FS45_R_M_VCCA_UV_MASK            0x04U
/**
 * VCCA overvoltage detection
 */
#define FS45_R_M_VCCA_OV_MASK            0x08U
/**
 * Thermal shutdown of VCCA
 */
#define FS45_R_M_TSD_CCA_MASK            0x10U
/**
 * Report a thermal warning from VCCA. Available only for internal pass MOSFET
 */
#define FS45_R_M_TWARN_CCA_MASK          0x20U

/**
 * Maximum current limitation duration. Available only when an external PNP is connected
 */
#define FS45_R_M_ILIM_CCA_OFF_SHIFT      0x00U
/**
 * Report a current limitation condition on VCCA
 */
#define FS45_R_M_ILIM_CCA_SHIFT          0x01U
/**
 * VCCA undervoltage detection
 */
#define FS45_R_M_VCCA_UV_SHIFT           0x02U
/**
 * VCCA overvoltage detection
 */
#define FS45_R_M_VCCA_OV_SHIFT           0x03U
/**
 * Thermal shutdown of VCCA
 */
#define FS45_R_M_TSD_CCA_SHIFT           0x04U
/**
 * Report a thermal warning from VCCA. Available only for internal pass MOSFET
 */
#define FS45_R_M_TWARN_CCA_SHIFT         0x05U

/**
 * T_LIMITATION < TCCA_LIM_OFF
 */
#define FS45_R_M_ILIM_CCA_OFF_NO_LIMITATION (0x00U << FS45_R_M_ILIM_CCA_OFF_SHIFT)
/**
 * T_LIMITATION > TCCA_LIM_OFF
 */
#define FS45_R_M_ILIM_CCA_OFF_LIMITATION (0x01U << FS45_R_M_ILIM_CCA_OFF_SHIFT)

/**
 * No current limitation (ICCA < ICCA_LIM)
 */
#define FS45_R_M_ILIM_CCA_NO_LIMITATION  (0x00U << FS45_R_M_ILIM_CCA_SHIFT)
/**
 * Current limitation (ICCA > ICCA_LIM)
 */
#define FS45_R_M_ILIM_CCA_LIMITATION     (0x01U << FS45_R_M_ILIM_CCA_SHIFT)

/**
 * No undervoltage (VCCA > VCCA_UV)
 */
#define FS45_R_M_VCCA_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_M_VCCA_UV_SHIFT)
/**
 * Undervoltage detected (VCCA < VCCA_UV)
 */
#define FS45_R_M_VCCA_UV_UNDERVOLTAGE    (0x01U << FS45_R_M_VCCA_UV_SHIFT)

/**
 * No overvoltage (VCCA < VCCA_OV)
 */
#define FS45_R_M_VCCA_OV_NO_OVERVOLTAGE  (0x00U << FS45_R_M_VCCA_OV_SHIFT)
/**
 * Overvoltage detected (VCCA > VCCA_OV)
 */
#define FS45_R_M_VCCA_OV_OVERVOLTAGE     (0x01U << FS45_R_M_VCCA_OV_SHIFT)

/**
 * NO TSD (TJ < TSD_CCA)
 */
#define FS45_R_M_TSD_CCA_NO_TSD          (0x00U << FS45_R_M_TSD_CCA_SHIFT)
/**
 * TSD occurred (TJ > TSD_CCA)
 */
#define FS45_R_M_TSD_CCA_TSD_OCCURRED    (0x01U << FS45_R_M_TSD_CCA_SHIFT)

/**
 * No thermal warning (TJ < TWARN_CCA)
 */
#define FS45_R_M_TWARN_CCA_NO_WARNING    (0x00U << FS45_R_M_TWARN_CCA_SHIFT)
/**
 * Thermal warning (TJ > TWARN_CCA)
 */
#define FS45_R_M_TWARN_CCA_WARNING       (0x01U << FS45_R_M_TWARN_CCA_SHIFT)

/******************************************************************************/
/* DIAG_VAUX - Type: R                                                        */
/******************************************************************************/

#define FS45_M_DIAG_VAUX_ADDR            0x0FU
#define FS45_M_DIAG_VAUX_DEFAULT         0x04U

/**
 * Maximum current limitation duration
 */
#define FS45_R_M_ILIM_AUX_OFF_MASK       0x01U
/**
 * Report a current limitation condition on VAUX
 */
#define FS45_R_M_ILIM_AUX_MASK           0x02U
/**
 * VAUX undervoltage detection
 */
#define FS45_R_M_VAUX_UV_MASK            0x04U
/**
 * VAUX overvoltage detection
 */
#define FS45_R_M_VAUX_OV_MASK            0x08U
/**
 * Thermal shutdown of VAUX
 */
#define FS45_R_M_TSD_AUX_MASK            0x10U

/**
 * Maximum current limitation duration
 */
#define FS45_R_M_ILIM_AUX_OFF_SHIFT      0x00U
/**
 * Report a current limitation condition on VAUX
 */
#define FS45_R_M_ILIM_AUX_SHIFT          0x01U
/**
 * VAUX undervoltage detection
 */
#define FS45_R_M_VAUX_UV_SHIFT           0x02U
/**
 * VAUX overvoltage detection
 */
#define FS45_R_M_VAUX_OV_SHIFT           0x03U
/**
 * Thermal shutdown of VAUX
 */
#define FS45_R_M_TSD_AUX_SHIFT           0x04U

/**
 * T_LIMITATION < TAUX_LIM_OFF
 */
#define FS45_R_M_ILIM_AUX_OFF_NO_LIMITATION (0x00U << FS45_R_M_ILIM_AUX_OFF_SHIFT)
/**
 * T_LIMITATION > TAUX_LIM_OFF
 */
#define FS45_R_M_ILIM_AUX_OFF_LIMITATION (0x01U << FS45_R_M_ILIM_AUX_OFF_SHIFT)

/**
 * No current limitation (IAUX < IAUX_LIM)
 */
#define FS45_R_M_ILIM_AUX_NO_LIMITATION  (0x00U << FS45_R_M_ILIM_AUX_SHIFT)
/**
 * Current limitation (IAUX > IAUX_LIM)
 */
#define FS45_R_M_ILIM_AUX_LIMITATION     (0x01U << FS45_R_M_ILIM_AUX_SHIFT)

/**
 * No undervoltage (VAUX > VAUX_UV)
 */
#define FS45_R_M_VAUX_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_M_VAUX_UV_SHIFT)
/**
 * Undervoltage detected (VAUX < VAUX_UV)
 */
#define FS45_R_M_VAUX_UV_UNDERVOLTAGE    (0x01U << FS45_R_M_VAUX_UV_SHIFT)

/**
 * No overvoltage (VAUX < VAUX_OV)
 */
#define FS45_R_M_VAUX_OV_NO_OVERVOLTAGE  (0x00U << FS45_R_M_VAUX_OV_SHIFT)
/**
 * Overvoltage detected (VAUX > VAUX_OV)
 */
#define FS45_R_M_VAUX_OV_OVERVOLTAGE     (0x01U << FS45_R_M_VAUX_OV_SHIFT)

/**
 * No TSD (TJ < TSD_AUX)
 */
#define FS45_R_M_TSD_AUX_NO_TSD          (0x00U << FS45_R_M_TSD_AUX_SHIFT)
/**
 * TSD occurred (TJ > TSD_AUX)
 */
#define FS45_R_M_TSD_AUX_TSD_OCCURRED    (0x01U << FS45_R_M_TSD_AUX_SHIFT)

/******************************************************************************/
/* DIAG_VSUP_VCAN - Type: R                                                   */
/******************************************************************************/

#define FS45_M_DIAG_VSUP_VCAN_ADDR       0x10U
#define FS45_M_DIAG_VSUP_VCAN_DEFAULT    0xC4U

/**
 * Report a current limitation condition on VCAN
 */
#define FS45_R_M_ILIM_CAN_MASK           0x02U
/**
 * VCAN undervoltage detection
 */
#define FS45_R_M_VCAN_UV_MASK            0x04U
/**
 * VCAN overvoltage detection
 */
#define FS45_R_M_VCAN_OV_MASK            0x08U
/**
 * Thermal shutdown of VCAN
 */
#define FS45_R_M_TSD_CAN_MASK            0x10U
/**
 * Input power feed forward (IPFF)
 */
#define FS45_R_M_IPFF_MASK               0x20U
/**
 * Detection of VSUP below VSUP_UV_7
 */
#define FS45_R_M_VSUP_UV_7_MASK          0x40U
/**
 * Detection of battery voltage below VSNS_UV
 */
#define FS45_R_M_VSNS_UV_MASK            0x80U

/**
 * Report a current limitation condition on VCAN
 */
#define FS45_R_M_ILIM_CAN_SHIFT          0x01U
/**
 * VCAN undervoltage detection
 */
#define FS45_R_M_VCAN_UV_SHIFT           0x02U
/**
 * VCAN overvoltage detection
 */
#define FS45_R_M_VCAN_OV_SHIFT           0x03U
/**
 * Thermal shutdown of VCAN
 */
#define FS45_R_M_TSD_CAN_SHIFT           0x04U
/**
 * Input power feed forward (IPFF)
 */
#define FS45_R_M_IPFF_SHIFT              0x05U
/**
 * Detection of VSUP below VSUP_UV_7
 */
#define FS45_R_M_VSUP_UV_7_SHIFT         0x06U
/**
 * Detection of battery voltage below VSNS_UV
 */
#define FS45_R_M_VSNS_UV_SHIFT           0x07U

/**
 * No current limitation (ICAN < ICAN_LIM)
 */
#define FS45_R_M_ILIM_CAN_NO_LIMITATION  (0x00U << FS45_R_M_ILIM_CAN_SHIFT)
/**
 * Current limitation (ICAN > ICAN _LIM)
 */
#define FS45_R_M_ILIM_CAN_LIMITATION     (0x01U << FS45_R_M_ILIM_CAN_SHIFT)

/**
 * No undervoltage (VCAN > VCAN_UV)
 */
#define FS45_R_M_VCAN_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_M_VCAN_UV_SHIFT)
/**
 * Undervoltage detected (VCAN < VCAN_UV)
 */
#define FS45_R_M_VCAN_UV_UNDERVOLTAGE    (0x01U << FS45_R_M_VCAN_UV_SHIFT)

/**
 * No overvoltage (VCAN < VCAN_OV)
 */
#define FS45_R_M_VCAN_OV_NO_OVERVOLTAGE  (0x00U << FS45_R_M_VCAN_OV_SHIFT)
/**
 * Overvoltage detected (VCAN > VCAN_OV)
 */
#define FS45_R_M_VCAN_OV_OVERVOLTAGE     (0x01U << FS45_R_M_VCAN_OV_SHIFT)

/**
 * NO TSD (TJ < TSD_CAN)
 */
#define FS45_R_M_TSD_CAN_NO_TSD          (0x00U << FS45_R_M_TSD_CAN_SHIFT)
/**
 * TSD occurred (TJ > TSD_CAN)
 */
#define FS45_R_M_TSD_CAN_TSD_OCCURRED    (0x01U << FS45_R_M_TSD_CAN_SHIFT)

/**
 * Normal operation
 */
#define FS45_R_M_IPFF_NORMAL             (0x00U << FS45_R_M_IPFF_SHIFT)
/**
 * IPFF mode activated
 */
#define FS45_R_M_IPFF_IPFF               (0x01U << FS45_R_M_IPFF_SHIFT)

/**
 * VSUP > VSUP_UV_7
 */
#define FS45_R_M_VSUP_UV_7_VSUP_G        (0x00U << FS45_R_M_VSUP_UV_7_SHIFT)
/**
 * VSUP < VSUP_UV_7
 */
#define FS45_R_M_VSUP_UV_7_VSUP_L        (0x01U << FS45_R_M_VSUP_UV_7_SHIFT)

/**
 * VBAT > VSNS_UV
 */
#define FS45_R_M_VSNS_UV_VBAT_G          (0x00U << FS45_R_M_VSNS_UV_SHIFT)
/**
 * VBAT < VSNS_UV
 */
#define FS45_R_M_VSNS_UV_VBAT_L          (0x01U << FS45_R_M_VSNS_UV_SHIFT)

/******************************************************************************/
/* DIAG_CAN_FD - Type: R                                                      */
/******************************************************************************/

#define FS45_M_DIAG_CAN_FD_ADDR          0x11U
#define FS45_M_DIAG_CAN_FD_DEFAULT       0x00U

/**
 * TXD dominant clamping detection (short-circuit to GND)
 */
#define FS45_R_M_TXD_DOM_MASK            0x01U
/**
 * RXD recessive clamping detection (short-circuit to 5.0 V)
 */
#define FS45_R_M_RXD_REC_MASK            0x02U
/**
 * CAN Bus dominant clamping detection
 */
#define FS45_R_M_CAN_DOM_MASK            0x08U
/**
 * CANL short-circuit to GND detection
 */
#define FS45_R_M_CANL_GND_MASK           0x10U
/**
 * CANL short-circuit to battery detection
 */
#define FS45_R_M_CANL_BATT_MASK          0x20U
/**
 * CANH short-circuit to GND detection
 */
#define FS45_R_M_CANH_GND_MASK           0x40U
/**
 * CANH short-circuit to battery detection
 */
#define FS45_R_M_CANH_BATT_MASK          0x80U

/**
 * TXD dominant clamping detection (short-circuit to GND)
 */
#define FS45_R_M_TXD_DOM_SHIFT           0x00U
/**
 * RXD recessive clamping detection (short-circuit to 5.0 V)
 */
#define FS45_R_M_RXD_REC_SHIFT           0x01U
/**
 * CAN Bus dominant clamping detection
 */
#define FS45_R_M_CAN_DOM_SHIFT           0x03U
/**
 * CANL short-circuit to GND detection
 */
#define FS45_R_M_CANL_GND_SHIFT          0x04U
/**
 * CANL short-circuit to battery detection
 */
#define FS45_R_M_CANL_BATT_SHIFT         0x05U
/**
 * CANH short-circuit to GND detection
 */
#define FS45_R_M_CANH_GND_SHIFT          0x06U
/**
 * CANH short-circuit to battery detection
 */
#define FS45_R_M_CANH_BATT_SHIFT         0x07U

/**
 * No failure
 */
#define FS45_R_M_TXD_DOM_NO_FAILURE      (0x00U << FS45_R_M_TXD_DOM_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_TXD_DOM_FAILURE         (0x01U << FS45_R_M_TXD_DOM_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_RXD_REC_NO_FAILURE      (0x00U << FS45_R_M_RXD_REC_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_RXD_REC_FAILURE         (0x01U << FS45_R_M_RXD_REC_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CAN_DOM_NO_FAILURE      (0x00U << FS45_R_M_CAN_DOM_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CAN_DOM_FAILURE         (0x01U << FS45_R_M_CAN_DOM_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CANL_GND_NO_FAILURE     (0x00U << FS45_R_M_CANL_GND_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CANL_GND_FAILURE        (0x01U << FS45_R_M_CANL_GND_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CANL_BATT_NO_FAILURE    (0x00U << FS45_R_M_CANL_BATT_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CANL_BATT_FAILURE       (0x01U << FS45_R_M_CANL_BATT_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CANH_GND_NO_FAILURE     (0x00U << FS45_R_M_CANH_GND_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CANH_GND_FAILURE        (0x01U << FS45_R_M_CANH_GND_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CANH_BATT_NO_FAILURE    (0x00U << FS45_R_M_CANH_BATT_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CANH_BATT_FAILURE       (0x01U << FS45_R_M_CANH_BATT_SHIFT)

/******************************************************************************/
/* DIAG_CAN_LIN - Type: R                                                     */
/******************************************************************************/

#define FS45_M_DIAG_CAN_LIN_ADDR         0x12U
#define FS45_M_DIAG_CAN_LIN_DEFAULT      0x00U

/**
 * CAN overcurrent detection
 */
#define FS45_R_M_CAN_OC_MASK             0x01U
/**
 * CAN overtemperature detection
 */
#define FS45_R_M_CAN_OT_MASK             0x02U
/**
 * LIN overtemperature detection
 */
#define FS45_R_M_LIN_OT_MASK             0x08U
/**
 * LIN RXD recessive clamping detection (short-circuit to 5.0 V)
 */
#define FS45_R_M_RXDL_REC_MASK           0x10U
/**
 * LIN TXD dominant clamping detection (short-circuit to GND)
 */
#define FS45_R_M_TDXL_DOM_MASK           0x40U
/**
 * LIN bus dominant clamping detection
 */
#define FS45_R_M_LIN_DOM_MASK            0x80U

/**
 * CAN overcurrent detection
 */
#define FS45_R_M_CAN_OC_SHIFT            0x00U
/**
 * CAN overtemperature detection
 */
#define FS45_R_M_CAN_OT_SHIFT            0x01U
/**
 * LIN overtemperature detection
 */
#define FS45_R_M_LIN_OT_SHIFT            0x03U
/**
 * LIN RXD recessive clamping detection (short-circuit to 5.0 V)
 */
#define FS45_R_M_RXDL_REC_SHIFT          0x04U
/**
 * LIN TXD dominant clamping detection (short-circuit to GND)
 */
#define FS45_R_M_TDXL_DOM_SHIFT          0x06U
/**
 * LIN bus dominant clamping detection
 */
#define FS45_R_M_LIN_DOM_SHIFT           0x07U

/**
 * No failure
 */
#define FS45_R_M_CAN_OC_NO_FAILURE       (0x00U << FS45_R_M_CAN_OC_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CAN_OC_FAILURE          (0x01U << FS45_R_M_CAN_OC_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_CAN_OT_NO_FAILURE       (0x00U << FS45_R_M_CAN_OT_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_CAN_OT_FAILURE          (0x01U << FS45_R_M_CAN_OT_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_LIN_OT_NO_FAILURE       (0x00U << FS45_R_M_LIN_OT_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_LIN_OT_FAILURE          (0x01U << FS45_R_M_LIN_OT_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_RXDL_REC_NO_FAILURE     (0x00U << FS45_R_M_RXDL_REC_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_RXDL_REC_FAILURE        (0x01U << FS45_R_M_RXDL_REC_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_TDXL_DOM_NO_FAILURE     (0x00U << FS45_R_M_TDXL_DOM_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_TDXL_DOM_FAILURE        (0x01U << FS45_R_M_TDXL_DOM_SHIFT)

/**
 * No failure
 */
#define FS45_R_M_LIN_DOM_NO_FAILURE      (0x00U << FS45_R_M_LIN_DOM_SHIFT)
/**
 * Failure detected
 */
#define FS45_R_M_LIN_DOM_FAILURE         (0x01U << FS45_R_M_LIN_DOM_SHIFT)

/******************************************************************************/
/* DIAG_SPI - Type: R                                                         */
/******************************************************************************/

#define FS45_M_DIAG_SPI_ADDR             0x13U
#define FS45_M_DIAG_SPI_DEFAULT          0x00U

/**
 * SPI parity bit error detection
 */
#define FS45_R_M_SPI_PARITY_MASK         0x02U
/**
 * Invalid SPI access (wrong write or read, write to INIT registers in normal mode, wrong address)
 */
#define FS45_R_M_SPI_REQ_MASK            0x08U
/**
 * SCLK error detection
 */
#define FS45_R_M_SPI_CLK_MASK            0x20U
/**
 * Secured SPI communication check
 */
#define FS45_R_M_SPI_ERR_MASK            0x80U

/**
 * SPI parity bit error detection
 */
#define FS45_R_M_SPI_PARITY_SHIFT        0x01U
/**
 * Invalid SPI access (wrong write or read, write to INIT registers in normal mode, wrong address)
 */
#define FS45_R_M_SPI_REQ_SHIFT           0x03U
/**
 * SCLK error detection
 */
#define FS45_R_M_SPI_CLK_SHIFT           0x05U
/**
 * Secured SPI communication check
 */
#define FS45_R_M_SPI_ERR_SHIFT           0x07U

/**
 * Parity bit ok
 */
#define FS45_R_M_SPI_PARITY_OK           (0x00U << FS45_R_M_SPI_PARITY_SHIFT)
/**
 * Parity bit error
 */
#define FS45_R_M_SPI_PARITY_ERROR        (0x01U << FS45_R_M_SPI_PARITY_SHIFT)

/**
 * No error
 */
#define FS45_R_M_SPI_REQ_NO_ERROR        (0x00U << FS45_R_M_SPI_REQ_SHIFT)
/**
 * SPI violation
 */
#define FS45_R_M_SPI_REQ_SPI_VIOLATION   (0x01U << FS45_R_M_SPI_REQ_SHIFT)

/**
 * 16 clock cycles during NCS low
 */
#define FS45_R_M_SPI_CLK_16_CLK_CYCLES   (0x00U << FS45_R_M_SPI_CLK_SHIFT)
/**
 * Wrong number of clock cycles (<16 or > 16)
 */
#define FS45_R_M_SPI_CLK_WRONG_NUMBER    (0x01U << FS45_R_M_SPI_CLK_SHIFT)

/**
 * No error
 */
#define FS45_R_M_SPI_ERR_NO_ERROR        (0x00U << FS45_R_M_SPI_ERR_SHIFT)
/**
 * Error detected in the secured bits
 */
#define FS45_R_M_SPI_ERR_ERROR           (0x01U << FS45_R_M_SPI_ERR_SHIFT)

/******************************************************************************/
/* MODE - Type: RW                                                            */
/******************************************************************************/

#define FS45_M_MODE_ADDR                 0x15U
#define FS45_M_MODE_DEFAULT              0x08U

/**
 * Report if the device resume from LPOFF-sleep or LPOFF_AUTO_WU mode
 */
#define FS45_R_M_LPOFF_MASK              0x01U
/**
 * Report if the device resume from deep fail-safe mode
 */
#define FS45_R_M_DFS_MASK                0x02U
/**
 * Report if normal mode of the main logic state machine is entered
 */
#define FS45_R_M_NORMAL_MASK             0x04U
/**
 * Report if INIT mode of the main logic state machine is entered
 */
#define FS45_R_M_INIT_MASK               0x08U
/**
 * Request for an INT pulse
 */
#define FS45_W_M_INT_REQ_MASK            0x10U
/**
 * Configure the device in LPOFF-SLEEP
 */
#define FS45_W_M_GO_LPOFF_MASK           0x20U
/**
 * Configure the device in LPOFF_AUTO_WU
 */
#define FS45_W_M_LPOFF_AUTO_WU_MASK      0x40U
/**
 * VKAM control (default state depends on part number)
 */
#define FS45_RW_M_VKAM_EN_MASK           0x80U

/**
 * Report if the device resume from LPOFF-sleep or LPOFF_AUTO_WU mode
 */
#define FS45_R_M_LPOFF_SHIFT             0x00U
/**
 * Report if the device resume from deep fail-safe mode
 */
#define FS45_R_M_DFS_SHIFT               0x01U
/**
 * Report if normal mode of the main logic state machine is entered
 */
#define FS45_R_M_NORMAL_SHIFT            0x02U
/**
 * Report if INIT mode of the main logic state machine is entered
 */
#define FS45_R_M_INIT_SHIFT              0x03U
/**
 * Request for an INT pulse
 */
#define FS45_W_M_INT_REQ_SHIFT           0x04U
/**
 * Configure the device in LPOFF-SLEEP
 */
#define FS45_W_M_GO_LPOFF_SHIFT          0x05U
/**
 * Configure the device in LPOFF_AUTO_WU
 */
#define FS45_W_M_LPOFF_AUTO_WU_SHIFT     0x06U
/**
 * VKAM control (default state depends on part number)
 */
#define FS45_RW_M_VKAM_EN_SHIFT          0x07U

/**
 * Not in LPOFF
 */
#define FS45_R_M_LPOFF_NOT_LPOFF         (0x00U << FS45_R_M_LPOFF_SHIFT)
/**
 * Resume from LPOFF
 */
#define FS45_R_M_LPOFF_RESUME_LPOFF      (0x01U << FS45_R_M_LPOFF_SHIFT)

/**
 * Not in deep fail-safe
 */
#define FS45_R_M_DFS_NOT_DFS             (0x00U << FS45_R_M_DFS_SHIFT)
/**
 * Resume from deep fail-safe
 */
#define FS45_R_M_DFS_RESUME_DFS          (0x01U << FS45_R_M_DFS_SHIFT)

/**
 * Not in normal mode
 */
#define FS45_R_M_NORMAL_NOT_NORMAL       (0x00U << FS45_R_M_NORMAL_SHIFT)
/**
 * Normal mode
 */
#define FS45_R_M_NORMAL_NORMAL           (0x01U << FS45_R_M_NORMAL_SHIFT)

/**
 * Not in INIT mode
 */
#define FS45_R_M_INIT_NOT_INIT           (0x00U << FS45_R_M_INIT_SHIFT)
/**
 * INIT mode
 */
#define FS45_R_M_INIT_INIT               (0x01U << FS45_R_M_INIT_SHIFT)

/**
 * No Request
 */
#define FS45_W_M_INT_REQ_NO              (0x00U << FS45_W_M_INT_REQ_SHIFT)
/**
 * Request for an INT pulse
 */
#define FS45_W_M_INT_REQ_INT_REQ         (0x01U << FS45_W_M_INT_REQ_SHIFT)

/**
 * No action
 */
#define FS45_W_M_GO_LPOFF_NO_ACTION      (0x00U << FS45_W_M_GO_LPOFF_SHIFT)
/**
 * Go to LPOFF mode and wait for wake-up event
 */
#define FS45_W_M_GO_LPOFF_LPOFF          (0x01U << FS45_W_M_GO_LPOFF_SHIFT)

/**
 * No action
 */
#define FS45_W_M_LPOFF_AUTO_WU_NO_ACTION (0x00U << FS45_W_M_LPOFF_AUTO_WU_SHIFT)
/**
 * Go to LPOFF mode and wake-up automatically after 1.0 ms
 */
#define FS45_W_M_LPOFF_AUTO_WU_LPOFF     (0x01U << FS45_W_M_LPOFF_AUTO_WU_SHIFT)

/**
 * DISABLED
 */
#define FS45_RW_M_VKAM_EN_DISABLED       (0x00U << FS45_RW_M_VKAM_EN_SHIFT)
/**
 * ENABLED
 */
#define FS45_RW_M_VKAM_EN_ENABLED        (0x01U << FS45_RW_M_VKAM_EN_SHIFT)

/******************************************************************************/
/* REG_MODE - Type: RW                                                        */
/******************************************************************************/

#define FS45_M_REG_MODE_ADDR             0x16U
#define FS45_M_REG_MODE_DEFAULT          0x0FU

/**
 * VCAN control
 */
#define FS45_R_M_VCAN_EN_MASK            0x01U
/**
 * VAUX control (switch off not recommended if VAUX is safety critical)
 */
#define FS45_R_M_VAUX_EN_MASK            0x02U
/**
 * VCCA control (switch off not recommended if VCCA is safety critical)
 */
#define FS45_R_M_VCCA_EN_MASK            0x04U
/**
 * VCORE control (switch off not recommended if VCORE is safety critical)
 */
#define FS45_R_M_VCORE_EN_MASK           0x08U
/**
 * VCAN control
 */
#define FS45_W_M_VCAN_EN_MASK            0x10U
/**
 * VAUX control (switch off not recommended if VAUX is safety critical)
 */
#define FS45_W_M_VAUX_EN_MASK            0x20U
/**
 * VCCA control (switch off not recommended if VCCA is safety critical)
 */
#define FS45_W_M_VCCA_EN_MASK            0x40U
/**
 * VCORE control (switch off not recommended if VCORE is safety critical)
 */
#define FS45_W_M_VCORE_EN_MASK           0x80U

/**
 * VCAN control
 */
#define FS45_R_M_VCAN_EN_SHIFT           0x00U
/**
 * VAUX control (switch off not recommended if VAUX is safety critical)
 */
#define FS45_R_M_VAUX_EN_SHIFT           0x01U
/**
 * VCCA control (switch off not recommended if VCCA is safety critical)
 */
#define FS45_R_M_VCCA_EN_SHIFT           0x02U
/**
 * VCORE control (switch off not recommended if VCORE is safety critical)
 */
#define FS45_R_M_VCORE_EN_SHIFT          0x03U
/**
 * VCAN control
 */
#define FS45_W_M_VCAN_EN_SHIFT           0x04U
/**
 * VAUX control (switch off not recommended if VAUX is safety critical)
 */
#define FS45_W_M_VAUX_EN_SHIFT           0x05U
/**
 * VCCA control (switch off not recommended if VCCA is safety critical)
 */
#define FS45_W_M_VCCA_EN_SHIFT           0x06U
/**
 * VCORE control (switch off not recommended if VCORE is safety critical)
 */
#define FS45_W_M_VCORE_EN_SHIFT          0x07U

/**
 * Disabled
 */
#define FS45_R_M_VCAN_EN_DISABLED        (0x00U << FS45_R_M_VCAN_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_R_M_VCAN_EN_ENABLED         (0x01U << FS45_R_M_VCAN_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_R_M_VAUX_EN_DISABLED        (0x00U << FS45_R_M_VAUX_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_R_M_VAUX_EN_ENABLED         (0x01U << FS45_R_M_VAUX_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_R_M_VCCA_EN_DISABLED        (0x00U << FS45_R_M_VCCA_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_R_M_VCCA_EN_ENABLED         (0x01U << FS45_R_M_VCCA_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_R_M_VCORE_EN_DISABLED       (0x00U << FS45_R_M_VCORE_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_R_M_VCORE_EN_ENABLED        (0x01U << FS45_R_M_VCORE_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_W_M_VCAN_EN_DISABLED        (0x00U << FS45_W_M_VCAN_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_W_M_VCAN_EN_ENABLED         (0x01U << FS45_W_M_VCAN_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_W_M_VAUX_EN_DISABLED        (0x00U << FS45_W_M_VAUX_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_W_M_VAUX_EN_ENABLED         (0x01U << FS45_W_M_VAUX_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_W_M_VCCA_EN_DISABLED        (0x00U << FS45_W_M_VCCA_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_W_M_VCCA_EN_ENABLED         (0x01U << FS45_W_M_VCCA_EN_SHIFT)

/**
 * Disabled
 */
#define FS45_W_M_VCORE_EN_DISABLED       (0x00U << FS45_W_M_VCORE_EN_SHIFT)
/**
 * Enabled
 */
#define FS45_W_M_VCORE_EN_ENABLED        (0x01U << FS45_W_M_VCORE_EN_SHIFT)

/******************************************************************************/
/* IO_OUT_AMUX - Type: RW                                                     */
/******************************************************************************/

#define FS45_M_IO_OUT_AMUX_ADDR          0x17U
#define FS45_M_IO_OUT_AMUX_DEFAULT       0x00U

/**
 * Select AMUX output
 */
#define FS45_RW_M_AMUX_MASK              0x07U
/**
 * Configure IO_4 output gate driver state
 */
#define FS45_RW_M_IO_OUT_4_MASK          0x40U
/**
 * Enable the output gate driver capability for IO_4
 */
#define FS45_RW_M_IO_OUT_4_EN_MASK       0x80U

/**
 * Select AMUX output
 */
#define FS45_RW_M_AMUX_SHIFT             0x00U
/**
 * Configure IO_4 output gate driver state
 */
#define FS45_RW_M_IO_OUT_4_SHIFT         0x06U
/**
 * Enable the output gate driver capability for IO_4
 */
#define FS45_RW_M_IO_OUT_4_EN_SHIFT      0x07U

/**
 * VREF
 */
#define FS45_RW_M_AMUX_VREF              (0x00U << FS45_RW_M_AMUX_SHIFT)
/**
 * VSNS wide range
 */
#define FS45_RW_M_AMUX_VSNS_W            (0x01U << FS45_RW_M_AMUX_SHIFT)
/**
 * IO_0 wide range
 */
#define FS45_RW_M_AMUX_IO_0_W            (0x02U << FS45_RW_M_AMUX_SHIFT)
/**
 * IO_5 wide range
 */
#define FS45_RW_M_AMUX_IO_5_W            (0x03U << FS45_RW_M_AMUX_SHIFT)
/**
 * VSNS tight range
 */
#define FS45_RW_M_AMUX_VSNS_T            (0x04U << FS45_RW_M_AMUX_SHIFT)
/**
 * IO_0 tight range
 */
#define FS45_RW_M_AMUX_IO_0_T            (0x05U << FS45_RW_M_AMUX_SHIFT)
/**
 * IO_5 tight range/VKAM
 */
#define FS45_RW_M_AMUX_IO_5_T            (0x06U << FS45_RW_M_AMUX_SHIFT)
/**
 * Die Temperature Sensor
 */
#define FS45_RW_M_AMUX_TEMP_SENSOR       (0x07U << FS45_RW_M_AMUX_SHIFT)

/**
 * Low
 */
#define FS45_RW_M_IO_OUT_4_LOW           (0x00U << FS45_RW_M_IO_OUT_4_SHIFT)
/**
 * High
 */
#define FS45_RW_M_IO_OUT_4_HIGH          (0x01U << FS45_RW_M_IO_OUT_4_SHIFT)

/**
 * High-impedance (IO_4 configured as input)
 */
#define FS45_RW_M_IO_OUT_4_EN_Z          (0x00U << FS45_RW_M_IO_OUT_4_EN_SHIFT)
/**
 * Enabled (IO_4 configured as output gate driver)
 */
#define FS45_RW_M_IO_OUT_4_EN_ENABLED    (0x01U << FS45_RW_M_IO_OUT_4_EN_SHIFT)

/******************************************************************************/
/* CAN_LIN_MODE - Type: RW                                                    */
/******************************************************************************/

#define FS45_M_CAN_LIN_MODE_ADDR         0x18U
#define FS45_M_CAN_LIN_MODE_DEFAULT      0xB4U

/**
 * Report a wake-up event from the LIN
 */
#define FS45_R_M_LIN_WU_MASK             0x01U
/**
 * Report a wake-up event from the CAN
 */
#define FS45_R_M_CAN_WU_MASK             0x02U
/**
 * Automatic LIN mode disable
 */
#define FS45_RW_M_LIN_AUTO_DIS_MASK      0x04U
/**
 * Configure the LIN mode
 */
#define FS45_RW_M_LIN_MODE_MASK          0x18U
/**
 * Automatic CAN Tx disable
 */
#define FS45_RW_M_CAN_AUTO_DIS_MASK      0x20U
/**
 * Configure the CAN mode
 */
#define FS45_RW_M_CAN_MODE_MASK          0xC0U

/**
 * Report a wake-up event from the LIN
 */
#define FS45_R_M_LIN_WU_SHIFT            0x00U
/**
 * Report a wake-up event from the CAN
 */
#define FS45_R_M_CAN_WU_SHIFT            0x01U
/**
 * Automatic LIN mode disable
 */
#define FS45_RW_M_LIN_AUTO_DIS_SHIFT     0x02U
/**
 * Configure the LIN mode
 */
#define FS45_RW_M_LIN_MODE_SHIFT         0x03U
/**
 * Automatic CAN Tx disable
 */
#define FS45_RW_M_CAN_AUTO_DIS_SHIFT     0x05U
/**
 * Configure the CAN mode
 */
#define FS45_RW_M_CAN_MODE_SHIFT         0x06U

/**
 * No wake-up
 */
#define FS45_R_M_LIN_WU_NO_WU            (0x00U << FS45_R_M_LIN_WU_SHIFT)
/**
 * Wake-up detected
 */
#define FS45_R_M_LIN_WU_WU               (0x01U << FS45_R_M_LIN_WU_SHIFT)

/**
 * No wake-up
 */
#define FS45_R_M_CAN_WU_NO_WU            (0x00U << FS45_R_M_CAN_WU_SHIFT)
/**
 * Wake-up detected
 */
#define FS45_R_M_CAN_WU_WU               (0x01U << FS45_R_M_CAN_WU_SHIFT)

/**
 * No auto disable
 */
#define FS45_RW_M_LIN_AUTO_DIS_NO        (0x00U << FS45_RW_M_LIN_AUTO_DIS_SHIFT)
/**
 * Reset LIN_mode from 11 to 01 on LIN_OT or TXDL_DOM or RXDL_REC event
 */
#define FS45_RW_M_LIN_AUTO_DIS_RESET     (0x01U << FS45_RW_M_LIN_AUTO_DIS_SHIFT)

/**
 * Sleep/no wake-up capability
 */
#define FS45_RW_M_LIN_MODE_SLN_WU        (0x00U << FS45_RW_M_LIN_MODE_SHIFT)
/**
 * Listen only
 */
#define FS45_RW_M_LIN_MODE_LISTEN_ONLY   (0x01U << FS45_RW_M_LIN_MODE_SHIFT)
/**
 * Sleep/wake-up capability
 */
#define FS45_RW_M_LIN_MODE_SL_WU         (0x02U << FS45_RW_M_LIN_MODE_SHIFT)
/**
 * Normal operation mode
 */
#define FS45_RW_M_LIN_MODE_NORMAL        (0x03U << FS45_RW_M_LIN_MODE_SHIFT)

/**
 * NO auto disable
 */
#define FS45_RW_M_CAN_AUTO_DIS_NO        (0x00U << FS45_RW_M_CAN_AUTO_DIS_SHIFT)
/**
 * Reset CAN_mode from 11 to 01 on CAN_OT or TXD_DOM or RXD_REC event
 */
#define FS45_RW_M_CAN_AUTO_DIS_RESET     (0x01U << FS45_RW_M_CAN_AUTO_DIS_SHIFT)

/**
 * Sleep/no wake-up capability
 */
#define FS45_RW_M_CAN_MODE_SLN_WU        (0x00U << FS45_RW_M_CAN_MODE_SHIFT)
/**
 * Listen only
 */
#define FS45_RW_M_CAN_MODE_LISTEN_ONLY   (0x01U << FS45_RW_M_CAN_MODE_SHIFT)
/**
 * Sleep/wake-up capability
 */
#define FS45_RW_M_CAN_MODE_SL_WU         (0x02U << FS45_RW_M_CAN_MODE_SHIFT)
/**
 * Normal operation mode
 */
#define FS45_RW_M_CAN_MODE_NORMAL        (0x03U << FS45_RW_M_CAN_MODE_SHIFT)

/******************************************************************************/
/* LDT_AFTER_RUN_1 - Type: RW                                                 */
/******************************************************************************/

#define FS45_M_LDT_AFTER_RUN_1_ADDR      0x1AU
#define FS45_M_LDT_AFTER_RUN_1_DEFAULT   0x00U



/******************************************************************************/
/* LDT_AFTER_RUN_2 - Type: RW                                                 */
/******************************************************************************/

#define FS45_M_LDT_AFTER_RUN_2_ADDR      0x1BU
#define FS45_M_LDT_AFTER_RUN_2_DEFAULT   0x00U



/******************************************************************************/
/* LDT_WAKE_UP_1 - Type: RW                                                   */
/******************************************************************************/

#define FS45_M_LDT_WAKE_UP_1_ADDR        0x1CU
#define FS45_M_LDT_WAKE_UP_1_DEFAULT     0x00U



/******************************************************************************/
/* LDT_WAKE_UP_2 - Type: RW                                                   */
/******************************************************************************/

#define FS45_M_LDT_WAKE_UP_2_ADDR        0x1DU
#define FS45_M_LDT_WAKE_UP_2_DEFAULT     0x00U



/******************************************************************************/
/* LDT_WAKE_UP_3 - Type: RW                                                   */
/******************************************************************************/

#define FS45_M_LDT_WAKE_UP_3_ADDR        0x1EU
#define FS45_M_LDT_WAKE_UP_3_DEFAULT     0x00U



/******************************************************************************/
/* WD_ANSWER - Type: RW                                                       */
/******************************************************************************/

#define FS45_FS_WD_ANSWER_ADDR           0x29U
#define FS45_FS_WD_ANSWER_DEFAULT        0x40U

/**
 * Report an error from the EDC of the fail-safe state machine (error detection correction)
 */
#define FS45_R_FS_ERR_INT_SW_MASK        0x01U
/**
 * Report an error from an internal redundant structure of the fail-safe state machine
 */
#define FS45_R_FS_ERR_INT_HW_MASK        0x02U
/**
 * Report a watchdog timing refresh error
 */
#define FS45_R_FS_WD_BAD_TIMING_MASK     0x04U
/**
 * Report an IO monitoring error
 */
#define FS45_R_FS_IO_FS_G_MASK           0x08U
/**
 * Report a fail-safe output failure
 */
#define FS45_R_FS_FSO_G_MASK             0x10U
/**
 * Report a watchdog data refresh error
 */
#define FS45_R_FS_WD_BAD_DATA_MASK       0x20U
/**
 * Report a fail safe event
 */
#define FS45_R_FS_FSXB_MASK              0x40U
/**
 * Report a reset event
 */
#define FS45_R_FS_RSTB_MASK              0x80U
/**
 * WD answer from the MCU, Answer = (NOT(((LFSR x 4)+6)-4))/4
 */
#define FS45_W_FS_WD_ANSWER_MASK         0xFFU

/**
 * Report an error from the EDC of the fail-safe state machine (error detection correction)
 */
#define FS45_R_FS_ERR_INT_SW_SHIFT       0x00U
/**
 * WD answer from the MCU, Answer = (NOT(((LFSR x 4)+6)-4))/4
 */
#define FS45_W_FS_WD_ANSWER_SHIFT        0x00U
/**
 * Report an error from an internal redundant structure of the fail-safe state machine
 */
#define FS45_R_FS_ERR_INT_HW_SHIFT       0x01U
/**
 * Report a watchdog timing refresh error
 */
#define FS45_R_FS_WD_BAD_TIMING_SHIFT    0x02U
/**
 * Report an IO monitoring error
 */
#define FS45_R_FS_IO_FS_G_SHIFT          0x03U
/**
 * Report a fail-safe output failure
 */
#define FS45_R_FS_FSO_G_SHIFT            0x04U
/**
 * Report a watchdog data refresh error
 */
#define FS45_R_FS_WD_BAD_DATA_SHIFT      0x05U
/**
 * Report a fail safe event
 */
#define FS45_R_FS_FSXB_SHIFT             0x06U
/**
 * Report a reset event
 */
#define FS45_R_FS_RSTB_SHIFT             0x07U

/**
 * No error
 */
#define FS45_R_FS_ERR_INT_SW_NO_ERROR    (0x00U << FS45_R_FS_ERR_INT_SW_SHIFT)
/**
 * Error detected
 */
#define FS45_R_FS_ERR_INT_SW_ERROR       (0x01U << FS45_R_FS_ERR_INT_SW_SHIFT)

/**
 * No error
 */
#define FS45_R_FS_ERR_INT_HW_NO_ERROR    (0x00U << FS45_R_FS_ERR_INT_HW_SHIFT)
/**
 * Error detected
 */
#define FS45_R_FS_ERR_INT_HW_ERROR       (0x01U << FS45_R_FS_ERR_INT_HW_SHIFT)

/**
 * WD timing refresh OK
 */
#define FS45_R_FS_WD_BAD_TIMING_TIMING_OK (0x00U << FS45_R_FS_WD_BAD_TIMING_SHIFT)
/**
 * Wrong WD timing refresh
 */
#define FS45_R_FS_WD_BAD_TIMING_WRONG_TIMING (0x01U << FS45_R_FS_WD_BAD_TIMING_SHIFT)

/**
 * No error
 */
#define FS45_R_FS_IO_FS_G_NO_ERROR       (0x00U << FS45_R_FS_IO_FS_G_SHIFT)
/**
 * Error detected
 */
#define FS45_R_FS_IO_FS_G_ERROR          (0x01U << FS45_R_FS_IO_FS_G_SHIFT)

/**
 * No failure
 */
#define FS45_R_FS_FSO_G_NO_FAILURE       (0x00U << FS45_R_FS_FSO_G_SHIFT)
/**
 * Failure
 */
#define FS45_R_FS_FSO_G_FAILURE          (0x01U << FS45_R_FS_FSO_G_SHIFT)

/**
 * WD data refresh ok
 */
#define FS45_R_FS_WD_BAD_DATA_DATA_OK    (0x00U << FS45_R_FS_WD_BAD_DATA_SHIFT)
/**
 * Wrong WD data refresh
 */
#define FS45_R_FS_WD_BAD_DATA_WRONG_DATA (0x01U << FS45_R_FS_WD_BAD_DATA_SHIFT)

/**
 * No fail-safe
 */
#define FS45_R_FS_FSXB_NO_FS             (0x00U << FS45_R_FS_FSXB_SHIFT)
/**
 * Fail-safe event occurred (default state at power-up and after LPOFF as FS0B/FS1B are asserted low)
 */
#define FS45_R_FS_FSXB_FSE_OCCURRED      (0x01U << FS45_R_FS_FSXB_SHIFT)

/**
 * No reset
 */
#define FS45_R_FS_RSTB_NO_RESET          (0x00U << FS45_R_FS_RSTB_SHIFT)
/**
 * Reset occurred
 */
#define FS45_R_FS_RSTB_RESET_OCCURRED    (0x01U << FS45_R_FS_RSTB_SHIFT)

/******************************************************************************/
/* DIAG_SF_IOS - Type: R                                                      */
/******************************************************************************/

#define FS45_FS_DIAG_SF_IOS_ADDR         0x2DU
#define FS45_FS_DIAG_SF_IOS_DEFAULT      0x00U

/**
 * Report an error in the IO_45 protocol
 */
#define FS45_R_FS_IO_45_FAIL_MASK        0x01U
/**
 * Report an error in the FCCU protocol
 */
#define FS45_R_FS_IO_23_FAIL_MASK        0x02U
/**
 * Report a failure on FS1B
 */
#define FS45_R_FS_FS1B_DIAG_MASK         0x0CU
/**
 * Report a failure on FS0B
 */
#define FS45_R_FS_FS0B_DIAG_MASK         0x30U
/**
 * Report a RSTB short-circuit to high
 */
#define FS45_R_FS_RSTB_DIAG_MASK         0x40U
/**
 * Report an external RSTB
 */
#define FS45_R_FS_RSTB_EXT_MASK          0x80U

/**
 * Report an error in the IO_45 protocol
 */
#define FS45_R_FS_IO_45_FAIL_SHIFT       0x00U
/**
 * Report an error in the FCCU protocol
 */
#define FS45_R_FS_IO_23_FAIL_SHIFT       0x01U
/**
 * Report a failure on FS1B
 */
#define FS45_R_FS_FS1B_DIAG_SHIFT        0x02U
/**
 * Report a failure on FS0B
 */
#define FS45_R_FS_FS0B_DIAG_SHIFT        0x04U
/**
 * Report a RSTB short-circuit to high
 */
#define FS45_R_FS_RSTB_DIAG_SHIFT        0x06U
/**
 * Report an external RSTB
 */
#define FS45_R_FS_RSTB_EXT_SHIFT         0x07U

/**
 * No error
 */
#define FS45_R_FS_IO_45_FAIL_NO_ERROR    (0x00U << FS45_R_FS_IO_45_FAIL_SHIFT)
/**
 * Error detected
 */
#define FS45_R_FS_IO_45_FAIL_ERROR       (0x01U << FS45_R_FS_IO_45_FAIL_SHIFT)

/**
 * No error
 */
#define FS45_R_FS_IO_23_FAIL_NO_ERROR    (0x00U << FS45_R_FS_IO_23_FAIL_SHIFT)
/**
 * Error detected
 */
#define FS45_R_FS_IO_23_FAIL_ERROR       (0x01U << FS45_R_FS_IO_23_FAIL_SHIFT)

/**
 * No Failure
 */
#define FS45_R_FS_FS1B_DIAG_NO_FAILURE   (0x01U << FS45_R_FS_FS1B_DIAG_SHIFT)
/**
 * Short-circuit low/open load
 */
#define FS45_R_FS_FS1B_DIAG_SC_LOW       (0x02U << FS45_R_FS_FS1B_DIAG_SHIFT)
/**
 * Short-circuit high
 */
#define FS45_R_FS_FS1B_DIAG_SC_HIGH      (0x03U << FS45_R_FS_FS1B_DIAG_SHIFT)

/**
 * No Failure
 */
#define FS45_R_FS_FS0B_DIAG_NO_FAILURE   (0x01U << FS45_R_FS_FS0B_DIAG_SHIFT)
/**
 * Short-circuit low/open load
 */
#define FS45_R_FS_FS0B_DIAG_SC_LOW       (0x02U << FS45_R_FS_FS0B_DIAG_SHIFT)
/**
 * Short-circuit high
 */
#define FS45_R_FS_FS0B_DIAG_SC_HIGH      (0x03U << FS45_R_FS_FS0B_DIAG_SHIFT)

/**
 * No Failure
 */
#define FS45_R_FS_RSTB_DIAG_NO_FAILURE   (0x00U << FS45_R_FS_RSTB_DIAG_SHIFT)
/**
 * Short-circuit high
 */
#define FS45_R_FS_RSTB_DIAG_SC_HIGH      (0x01U << FS45_R_FS_RSTB_DIAG_SHIFT)

/**
 * No external RSTB
 */
#define FS45_R_FS_RSTB_EXT_NO            (0x00U << FS45_R_FS_RSTB_EXT_SHIFT)
/**
 * External RSTB
 */
#define FS45_R_FS_RSTB_EXT_EXTERNAL      (0x01U << FS45_R_FS_RSTB_EXT_SHIFT)

/******************************************************************************/
/* WD_COUNTER - Type: R                                                       */
/******************************************************************************/

#define FS45_FS_WD_COUNTER_ADDR          0x2EU
#define FS45_FS_WD_COUNTER_DEFAULT       0x00U

/**
 * Report the value of the watchdog refresh counter from 0 to 6 (7 generate a decrease of the FLT_ERR_CNT and this 
 * counter is reset to 0)
 */
#define FS45_R_FS_WD_RFR_MASK            0x0EU
/**
 * Report the value of the watchdog error counter from 0 to 5 (6 generate an increase of the FLT_ERR_CNT and this 
 * counter is reset to 0)
 */
#define FS45_R_FS_WD_ERR_MASK            0xE0U

/**
 * Report the value of the watchdog refresh counter from 0 to 6 (7 generate a decrease of the FLT_ERR_CNT and this 
 * counter is reset to 0)
 */
#define FS45_R_FS_WD_RFR_SHIFT           0x01U
/**
 * Report the value of the watchdog error counter from 0 to 5 (6 generate an increase of the FLT_ERR_CNT and this 
 * counter is reset to 0)
 */
#define FS45_R_FS_WD_ERR_SHIFT           0x05U

/******************************************************************************/
/* DIAG_SF_ERR - Type: R                                                      */
/******************************************************************************/

#define FS45_FS_DIAG_SF_ERR_ADDR         0x2FU
#define FS45_FS_DIAG_SF_ERR_DEFAULT      0x20U

/**
 * Report an undervoltage on FCRBM
 */
#define FS45_R_FS_FCRBM_UV_MASK          0x01U
/**
 * Report an overvoltage on FCRBM
 */
#define FS45_R_FS_FCRBM_OV_MASK          0x02U
/**
 * Report an overvoltage on V2P5 main digital regulator
 */
#define FS45_R_FS_V2P5_M_D_OV_MASK       0x04U
/**
 * Report an overvoltage on V2P5 main analog regulator
 */
#define FS45_R_FS_V2P5_M_A_OV_MASK       0x08U
/**
 * Report the value of the fault error counter
 */
#define FS45_R_FS_FLT_ERR_MASK           0xE0U

/**
 * Report an undervoltage on FCRBM
 */
#define FS45_R_FS_FCRBM_UV_SHIFT         0x00U
/**
 * Report an overvoltage on FCRBM
 */
#define FS45_R_FS_FCRBM_OV_SHIFT         0x01U
/**
 * Report an overvoltage on V2P5 main digital regulator
 */
#define FS45_R_FS_V2P5_M_D_OV_SHIFT      0x02U
/**
 * Report an overvoltage on V2P5 main analog regulator
 */
#define FS45_R_FS_V2P5_M_A_OV_SHIFT      0x03U
/**
 * Report the value of the fault error counter
 */
#define FS45_R_FS_FLT_ERR_SHIFT          0x05U

/**
 * No undervoltage (FB_Core - FCRBM > -150 mV)
 */
#define FS45_R_FS_FCRBM_UV_NO_UNDERVOLTAGE (0x00U << FS45_R_FS_FCRBM_UV_SHIFT)
/**
 * Undervoltage detected (FB_Core - FCRBM < -150 mV)
 */
#define FS45_R_FS_FCRBM_UV_UNDERVOLTAGE  (0x01U << FS45_R_FS_FCRBM_UV_SHIFT)

/**
 * No overvoltage (FB_Core - FCRBM < 150 mV)
 */
#define FS45_R_FS_FCRBM_OV_NO_OVERVOLTAGE (0x00U << FS45_R_FS_FCRBM_OV_SHIFT)
/**
 * Overvoltage detected (FB_Core - FCRBM > 150 mV)
 */
#define FS45_R_FS_FCRBM_OV_OVERVOLTAGE   (0x01U << FS45_R_FS_FCRBM_OV_SHIFT)

/**
 * No overvoltage (V2P5_M_D < V2P5_M_D_OV)
 */
#define FS45_R_FS_V2P5_M_D_OV_NO_OVERVOLTAGE (0x00U << FS45_R_FS_V2P5_M_D_OV_SHIFT)
/**
 * Overvoltage detected (V2P5_M_D > V2P5_M_D_OV)
 */
#define FS45_R_FS_V2P5_M_D_OV_OVERVOLTAGE (0x01U << FS45_R_FS_V2P5_M_D_OV_SHIFT)

/**
 * No overvoltage (V2P5_M_A < V2P5_M_A_OV)
 */
#define FS45_R_FS_V2P5_M_A_OV_NO_OVERVOLTAGE (0x00U << FS45_R_FS_V2P5_M_A_OV_SHIFT)
/**
 * Overvoltage detected (V2P5_M_A > V2P5_M_A_OV)
 */
#define FS45_R_FS_V2P5_M_A_OV_OVERVOLTAGE (0x01U << FS45_R_FS_V2P5_M_A_OV_SHIFT)

/******************************************************************************/
/* DEVICE_ID_FS - Type: R                                                     */
/******************************************************************************/

#define FS45_FS_DEVICE_ID_FS_ADDR        0x34U
#define FS45_FS_DEVICE_ID_FS_DEFAULT     0x00U

/**
 * Report the FS1B function availability (depends on part number)
 */
#define FS45_R_FS_FS1_MASK               0x01U
/**
 * Report the deep fail-safe hardware configuration (fail-safe logic)
 */
#define FS45_R_FS_DFS_HW2_MASK           0x02U

/**
 * Report the FS1B function availability (depends on part number)
 */
#define FS45_R_FS_FS1_SHIFT              0x00U
/**
 * Report the deep fail-safe hardware configuration (fail-safe logic)
 */
#define FS45_R_FS_DFS_HW2_SHIFT          0x01U

/**
 * Disabled
 */
#define FS45_R_FS_FS1_DISABLED           (0x00U << FS45_R_FS_FS1_SHIFT)
/**
 * Enabled
 */
#define FS45_R_FS_FS1_ENABLE             (0x01U << FS45_R_FS_FS1_SHIFT)

/**
 * Deep fail-safe disable
 */
#define FS45_R_FS_DFS_HW2_DISABLE        (0x00U << FS45_R_FS_DFS_HW2_SHIFT)
/**
 * Deep fail-safe enable
 */
#define FS45_R_FS_DFS_HW2_ENABLE         (0x01U << FS45_R_FS_DFS_HW2_SHIFT)

/******************************************************************************/
/* BIST - Type: RW                                                            */
/******************************************************************************/

#define FS45_FS_BIST_ADDR                0x22U
#define FS45_FS_BIST_DEFAULT             0x09U

/**
 * Diagnostic of analog BIST1 (automatically executed)
 */
#define FS45_R_FS_ABIST1_OK_MASK         0x01U
/**
 * Diagnostic of VAUX Analog BIST2 (executed on demand)
 */
#define FS45_R_FS_ABIST2_VAUX_OK_MASK    0x02U
/**
 * Diagnostic of FS1B Analog BIST2 (executed on demand)
 */
#define FS45_R_FS_ABIST2_FS1B_OK_MASK    0x04U
/**
 * Diagnostic of fail-safe logic BIST (automatically executed)
 */
#define FS45_R_FS_LBIST_OK_MASK          0x08U
/**
 * Request ABIST execution on VAUX
 */
#define FS45_W_FS_ABIST2_VAUX_MASK       0x20U
/**
 * Request ABIST execution on FS1B
 */
#define FS45_W_FS_ABIST2_FS1B_MASK       0x40U

/**
 * Diagnostic of analog BIST1 (automatically executed)
 */
#define FS45_R_FS_ABIST1_OK_SHIFT        0x00U
/**
 * Diagnostic of VAUX Analog BIST2 (executed on demand)
 */
#define FS45_R_FS_ABIST2_VAUX_OK_SHIFT   0x01U
/**
 * Diagnostic of FS1B Analog BIST2 (executed on demand)
 */
#define FS45_R_FS_ABIST2_FS1B_OK_SHIFT   0x02U
/**
 * Diagnostic of fail-safe logic BIST (automatically executed)
 */
#define FS45_R_FS_LBIST_OK_SHIFT         0x03U
/**
 * Request ABIST execution on VAUX
 */
#define FS45_W_FS_ABIST2_VAUX_SHIFT      0x05U
/**
 * Request ABIST execution on FS1B
 */
#define FS45_W_FS_ABIST2_FS1B_SHIFT      0x06U

/**
 * ABIST1 fail
 */
#define FS45_R_FS_ABIST1_OK_FAIL         (0x00U << FS45_R_FS_ABIST1_OK_SHIFT)
/**
 * ABIST1 pass
 */
#define FS45_R_FS_ABIST1_OK_PASS         (0x01U << FS45_R_FS_ABIST1_OK_SHIFT)

/**
 * VAUX ABIST fail or not executed
 */
#define FS45_R_FS_ABIST2_VAUX_OK_FAIL    (0x00U << FS45_R_FS_ABIST2_VAUX_OK_SHIFT)
/**
 * VAUX ABIST pass
 */
#define FS45_R_FS_ABIST2_VAUX_OK_PASS    (0x01U << FS45_R_FS_ABIST2_VAUX_OK_SHIFT)

/**
 * FS1B ABIST fail or not executed
 */
#define FS45_R_FS_ABIST2_FS1B_OK_FAIL    (0x00U << FS45_R_FS_ABIST2_FS1B_OK_SHIFT)
/**
 * FS1B ABIST pass
 */
#define FS45_R_FS_ABIST2_FS1B_OK_PASS    (0x01U << FS45_R_FS_ABIST2_FS1B_OK_SHIFT)

/**
 * LBIST fail
 */
#define FS45_R_FS_LBIST_OK_FAIL          (0x00U << FS45_R_FS_LBIST_OK_SHIFT)
/**
 * LBIST pass
 */
#define FS45_R_FS_LBIST_OK_PASS          (0x01U << FS45_R_FS_LBIST_OK_SHIFT)

/**
 * No action
 */
#define FS45_W_FS_ABIST2_VAUX_NO_ACTION  (0x00U << FS45_W_FS_ABIST2_VAUX_SHIFT)
/**
 * Launch ABIST on VAUX
 */
#define FS45_W_FS_ABIST2_VAUX_ABIST_VAUX (0x01U << FS45_W_FS_ABIST2_VAUX_SHIFT)

/**
 * No action
 */
#define FS45_W_FS_ABIST2_FS1B_NO_ACTION  (0x00U << FS45_W_FS_ABIST2_FS1B_SHIFT)
/**
 * Launch ABIST on FS1B
 */
#define FS45_W_FS_ABIST2_FS1B_ABIST_FS1B (0x01U << FS45_W_FS_ABIST2_FS1B_SHIFT)

/******************************************************************************/
/* RELEASE_FSXB - Type: RW                                                    */
/******************************************************************************/

#define FS45_FS_RELEASE_FSXB_ADDR        0x2AU
#define FS45_FS_RELEASE_FSXB_DEFAULT     0x01U

/**
 * Sense of RSTB pad
 */
#define FS45_R_FS_RSTB_SNS_MASK          0x01U
/**
 * Sense of FS0B pad
 */
#define FS45_R_FS_FS0B_SNS_MASK          0x02U
/**
 * Sense of FS1B pad
 */
#define FS45_R_FS_FS1B_SNS_MASK          0x04U
/**
 * Secured 8 bits word to release the FS0B and FS1B pins, depend on LFSR_out value and calculation
 */
#define FS45_W_FS_RELEASE_FSXB_MASK      0xFFU

/**
 * Sense of RSTB pad
 */
#define FS45_R_FS_RSTB_SNS_SHIFT         0x00U
/**
 * Secured 8 bits word to release the FS0B and FS1B pins, depend on LFSR_out value and calculation
 */
#define FS45_W_FS_RELEASE_FSXB_SHIFT     0x00U
/**
 * Sense of FS0B pad
 */
#define FS45_R_FS_FS0B_SNS_SHIFT         0x01U
/**
 * Sense of FS1B pad
 */
#define FS45_R_FS_FS1B_SNS_SHIFT         0x02U

/**
 * RSTB pad sense low
 */
#define FS45_R_FS_RSTB_SNS_LOW           (0x00U << FS45_R_FS_RSTB_SNS_SHIFT)
/**
 * RSTB pad sense high
 */
#define FS45_R_FS_RSTB_SNS_HIGH          (0x01U << FS45_R_FS_RSTB_SNS_SHIFT)

/**
 * FS0B pad sense low
 */
#define FS45_R_FS_FS0B_SNS_LOW           (0x00U << FS45_R_FS_FS0B_SNS_SHIFT)
/**
 * FS0B pad sense high
 */
#define FS45_R_FS_FS0B_SNS_HIGH          (0x01U << FS45_R_FS_FS0B_SNS_SHIFT)

/**
 * FS1B pad sense low
 */
#define FS45_R_FS_FS1B_SNS_LOW           (0x00U << FS45_R_FS_FS1B_SNS_SHIFT)
/**
 * FS1B pad sense high
 */
#define FS45_R_FS_FS1B_SNS_HIGH          (0x01U << FS45_R_FS_FS1B_SNS_SHIFT)

/******************************************************************************/
/* SF_OUTPUT_REQUEST - Type: RW                                               */
/******************************************************************************/

#define FS45_FS_SF_OUTPUT_REQUEST_ADDR   0x2BU
#define FS45_FS_SF_OUTPUT_REQUEST_DEFAULT 0x01U

/**
 * Sense of RSTB driver command from fail-safe logic
 */
#define FS45_R_FS_RSTB_DRV_MASK          0x01U
/**
 * Sense of FS0B driver command from fail-safe logic
 */
#define FS45_R_FS_FS0B_DRV_MASK          0x02U
/**
 * Sense of FS1B driver command from backup delay (analog)
 */
#define FS45_R_FS_FS1B_DLY_DRV_MASK      0x04U
/**
 * Sense of FS1B driver command from fail-safe logic (digital)
 */
#define FS45_R_FS_FS1B_DRV_MASK          0x08U
/**
 * Request a RSTB low pulse
 */
#define FS45_W_FS_RSTB_REQ_MASK          0x10U
/**
 * Request FS0B to be asserted low
 */
#define FS45_W_FS_FS0B_REQ_MASK          0x20U
/**
 * Request activation of FS1B backup delay (open/close switch S1)
 */
#define FS45_W_FS_FS1B_DLY_REQ_MASK      0x40U
/**
 * Request FS1B to be asserted low
 */
#define FS45_W_FS_FS1B_REQ_MASK          0x80U

/**
 * Sense of RSTB driver command from fail-safe logic
 */
#define FS45_R_FS_RSTB_DRV_SHIFT         0x00U
/**
 * Sense of FS0B driver command from fail-safe logic
 */
#define FS45_R_FS_FS0B_DRV_SHIFT         0x01U
/**
 * Sense of FS1B driver command from backup delay (analog)
 */
#define FS45_R_FS_FS1B_DLY_DRV_SHIFT     0x02U
/**
 * Sense of FS1B driver command from fail-safe logic (digital)
 */
#define FS45_R_FS_FS1B_DRV_SHIFT         0x03U
/**
 * Request a RSTB low pulse
 */
#define FS45_W_FS_RSTB_REQ_SHIFT         0x04U
/**
 * Request FS0B to be asserted low
 */
#define FS45_W_FS_FS0B_REQ_SHIFT         0x05U
/**
 * Request activation of FS1B backup delay (open/close switch S1)
 */
#define FS45_W_FS_FS1B_DLY_REQ_SHIFT     0x06U
/**
 * Request FS1B to be asserted low
 */
#define FS45_W_FS_FS1B_REQ_SHIFT         0x07U

/**
 * RSTB driver sense low
 */
#define FS45_R_FS_RSTB_DRV_LOW           (0x00U << FS45_R_FS_RSTB_DRV_SHIFT)
/**
 * RSTB driver sense high
 */
#define FS45_R_FS_RSTB_DRV_HIGH          (0x01U << FS45_R_FS_RSTB_DRV_SHIFT)

/**
 * FS0B driver sense low
 */
#define FS45_R_FS_FS0B_DRV_LOW           (0x00U << FS45_R_FS_FS0B_DRV_SHIFT)
/**
 * FS0B driver sense high
 */
#define FS45_R_FS_FS0B_DRV_HIGH          (0x01U << FS45_R_FS_FS0B_DRV_SHIFT)

/**
 * FS1B analog driver sense low
 */
#define FS45_R_FS_FS1B_DLY_DRV_FS1B_LOW  (0x00U << FS45_R_FS_FS1B_DLY_DRV_SHIFT)
/**
 * FS1B analog driver sense high
 */
#define FS45_R_FS_FS1B_DLY_DRV_FS1B_HIGH (0x01U << FS45_R_FS_FS1B_DLY_DRV_SHIFT)

/**
 * FS1B digital driver sense low
 */
#define FS45_R_FS_FS1B_DRV_FS1B_LOW      (0x00U << FS45_R_FS_FS1B_DRV_SHIFT)
/**
 * FS1B digital driver sense high
 */
#define FS45_R_FS_FS1B_DRV_FS1B_HIGH     (0x01U << FS45_R_FS_FS1B_DRV_SHIFT)

/**
 * No request
 */
#define FS45_W_FS_RSTB_REQ_NO_REQUEST    (0x00U << FS45_W_FS_RSTB_REQ_SHIFT)
/**
 * Request a RSTB low pulse
 */
#define FS45_W_FS_RSTB_REQ_RSTB_REQ      (0x01U << FS45_W_FS_RSTB_REQ_SHIFT)

/**
 * No request
 */
#define FS45_W_FS_FS0B_REQ_NO_REQUEST    (0x00U << FS45_W_FS_FS0B_REQ_SHIFT)
/**
 * Request FS0B assertion
 */
#define FS45_W_FS_FS0B_REQ_FS0B_REQ      (0x01U << FS45_W_FS_FS0B_REQ_SHIFT)

/**
 * No request (close S1)
 */
#define FS45_W_FS_FS1B_DLY_REQ_NO_REQUEST (0x00U << FS45_W_FS_FS1B_DLY_REQ_SHIFT)
/**
 * Request FS1B assertion with tDELAY controlled by the backup delay (open S1)
 */
#define FS45_W_FS_FS1B_DLY_REQ_FS1B_REQ  (0x01U << FS45_W_FS_FS1B_DLY_REQ_SHIFT)

/**
 * No request
 */
#define FS45_W_FS_FS1B_REQ_NO_REQUEST    (0x00U << FS45_W_FS_FS1B_REQ_SHIFT)
/**
 * Request FS1B assertion with immediate assertion, no delay
 */
#define FS45_W_FS_FS1B_REQ_FS1B_REQ      (0x01U << FS45_W_FS_FS1B_REQ_SHIFT)

/******************************************************************************/
/* INIT_WU1 - Type: RW                                                        */
/******************************************************************************/

#define FS45_M_INIT_WU1_ADDR             0x02U
#define FS45_M_INIT_WU1_DEFAULT          0x40U

/**
 * IO_4 wake-up configuration
 */
#define FS45_RW_M_WU_IO4_MASK            0x03U
/**
 * IO_3 wake-up configuration
 */
#define FS45_RW_M_WU_IO3_MASK            0x0CU
/**
 * IO_2 wake-up configuration
 */
#define FS45_RW_M_WU_IO2_MASK            0x30U
/**
 * IO_0 wake-up configuration
 */
#define FS45_RW_M_WU_IO0_MASK            0xC0U

/**
 * IO_4 wake-up configuration
 */
#define FS45_RW_M_WU_IO4_SHIFT           0x00U
/**
 * IO_3 wake-up configuration
 */
#define FS45_RW_M_WU_IO3_SHIFT           0x02U
/**
 * IO_2 wake-up configuration
 */
#define FS45_RW_M_WU_IO2_SHIFT           0x04U
/**
 * IO_0 wake-up configuration
 */
#define FS45_RW_M_WU_IO0_SHIFT           0x06U

/**
 * NO wake-up capability
 */
#define FS45_RW_M_WU_IO4_NO_WAKEUP       (0x00U << FS45_RW_M_WU_IO4_SHIFT)
/**
 * Wake-up on rising edge - or high level
 */
#define FS45_RW_M_WU_IO4_RISING_EDGE     (0x01U << FS45_RW_M_WU_IO4_SHIFT)
/**
 * Wake-up on falling edge - or low level
 */
#define FS45_RW_M_WU_IO4_FALLING_EDGE    (0x02U << FS45_RW_M_WU_IO4_SHIFT)
/**
 * Wake-up on any edge
 */
#define FS45_RW_M_WU_IO4_ANY_EDGE        (0x03U << FS45_RW_M_WU_IO4_SHIFT)

/**
 * NO wake-up capability
 */
#define FS45_RW_M_WU_IO3_NO_WAKEUP       (0x00U << FS45_RW_M_WU_IO3_SHIFT)
/**
 * Wake-up on rising edge - or high level
 */
#define FS45_RW_M_WU_IO3_RISING_EDGE     (0x01U << FS45_RW_M_WU_IO3_SHIFT)
/**
 * Wake-up on falling edge - or low level
 */
#define FS45_RW_M_WU_IO3_FALLING_EDGE    (0x02U << FS45_RW_M_WU_IO3_SHIFT)
/**
 * Wake-up on any edge
 */
#define FS45_RW_M_WU_IO3_ANY_EDGE        (0x03U << FS45_RW_M_WU_IO3_SHIFT)

/**
 * NO wake-up capability
 */
#define FS45_RW_M_WU_IO2_NO_WAKEUP       (0x00U << FS45_RW_M_WU_IO2_SHIFT)
/**
 * Wake-up on rising edge - or high level
 */
#define FS45_RW_M_WU_IO2_RISING_EDGE     (0x01U << FS45_RW_M_WU_IO2_SHIFT)
/**
 * Wake-up on falling edge - or low level
 */
#define FS45_RW_M_WU_IO2_FALLING_EDGE    (0x02U << FS45_RW_M_WU_IO2_SHIFT)
/**
 * Wake-up on any edge
 */
#define FS45_RW_M_WU_IO2_ANY_EDGE        (0x03U << FS45_RW_M_WU_IO2_SHIFT)

/**
 * NO wake-up capability
 */
#define FS45_RW_M_WU_IO0_NO_WAKEUP       (0x00U << FS45_RW_M_WU_IO0_SHIFT)
/**
 * Wake-up on rising edge - or high level
 */
#define FS45_RW_M_WU_IO0_RISING_EDGE     (0x01U << FS45_RW_M_WU_IO0_SHIFT)
/**
 * Wake-up on falling edge - or low level
 */
#define FS45_RW_M_WU_IO0_FALLING_EDGE    (0x02U << FS45_RW_M_WU_IO0_SHIFT)
/**
 * Wake-up on any edge
 */
#define FS45_RW_M_WU_IO0_ANY_EDGE        (0x03U << FS45_RW_M_WU_IO0_SHIFT)

/******************************************************************************/
/* INIT_WU2 - Type: RW                                                        */
/******************************************************************************/

#define FS45_M_INIT_WU2_ADDR             0x03U
#define FS45_M_INIT_WU2_DEFAULT          0x00U

/**
 * Configure the LIN slew rate
 */
#define FS45_RW_M_LIN_SR_MASK            0x03U
/**
 * To comply with J2602 standard. Recessive mode when VSUP < 7.0 V
 */
#define FS45_RW_M_LIN_J2602_DIS_MASK     0x04U
/**
 * Define the CAN wake-up timeout (when CAN_WU_CONF = 0)
 */
#define FS45_RW_M_CAN_WU_TO_MASK         0x10U
/**
 * Define CAN behavior when FS1B is asserted low
 */
#define FS45_RW_M_CAN_DIS_CFG_MASK       0x20U
/**
 * IO_5 wake-up configuration
 */
#define FS45_RW_M_WU_IO5_MASK            0xC0U

/**
 * Configure the LIN slew rate
 */
#define FS45_RW_M_LIN_SR_SHIFT           0x00U
/**
 * To comply with J2602 standard. Recessive mode when VSUP < 7.0 V
 */
#define FS45_RW_M_LIN_J2602_DIS_SHIFT    0x02U
/**
 * Define the CAN wake-up timeout (when CAN_WU_CONF = 0)
 */
#define FS45_RW_M_CAN_WU_TO_SHIFT        0x04U
/**
 * Define CAN behavior when FS1B is asserted low
 */
#define FS45_RW_M_CAN_DIS_CFG_SHIFT      0x05U
/**
 * IO_5 wake-up configuration
 */
#define FS45_RW_M_WU_IO5_SHIFT           0x06U

/**
 * 20 kbits/s
 */
#define FS45_RW_M_LIN_SR_20KBITS         (0x00U << FS45_RW_M_LIN_SR_SHIFT)
/**
 * 10 kbits/s
 */
#define FS45_RW_M_LIN_SR_10KBITS         (0x01U << FS45_RW_M_LIN_SR_SHIFT)
/**
 * Fast baud rate (Max: 100 kbits/s)
 */
#define FS45_RW_M_LIN_SR_FAST_RATE       (0x02U << FS45_RW_M_LIN_SR_SHIFT)

/**
 * Compliant with J2602 standard
 */
#define FS45_RW_M_LIN_J2602_DIS_COMPLIANT (0x00U << FS45_RW_M_LIN_J2602_DIS_SHIFT)
/**
 * Not compliant with J2602 standard
 */
#define FS45_RW_M_LIN_J2602_DIS_NOT_COMPLIANT (0x01U << FS45_RW_M_LIN_J2602_DIS_SHIFT)

/**
 * 120 us
 */
#define FS45_RW_M_CAN_WU_TO_120US        (0x00U << FS45_RW_M_CAN_WU_TO_SHIFT)
/**
 * 2.8 ms
 */
#define FS45_RW_M_CAN_WU_TO_2_8MS        (0x01U << FS45_RW_M_CAN_WU_TO_SHIFT)

/**
 * CAN in Rx only mode (when FS1B_CAN_ IMPACT = 1 in INIT_FAULT register)
 */
#define FS45_RW_M_CAN_DIS_CFG_RX_ONLY    (0x00U << FS45_RW_M_CAN_DIS_CFG_SHIFT)
/**
 * CAN in sleep mode (when FS1B_CAN_ IMPACT = 1 in INIT_FAULT register)
 */
#define FS45_RW_M_CAN_DIS_CFG_SLEEP      (0x01U << FS45_RW_M_CAN_DIS_CFG_SHIFT)

/**
 * NO wake-up capability
 */
#define FS45_RW_M_WU_IO5_NO_WAKEUP       (0x00U << FS45_RW_M_WU_IO5_SHIFT)
/**
 * Wake-up on rising edge - or high level
 */
#define FS45_RW_M_WU_IO5_RISING_EDGE     (0x01U << FS45_RW_M_WU_IO5_SHIFT)
/**
 * Wake-up on falling edge - or low level
 */
#define FS45_RW_M_WU_IO5_FALLING_EDGE    (0x02U << FS45_RW_M_WU_IO5_SHIFT)
/**
 * Wake-up on any edge
 */
#define FS45_RW_M_WU_IO5_ANY_EDGE        (0x03U << FS45_RW_M_WU_IO5_SHIFT)

/******************************************************************************/
/* INIT_INH_INT - Type: RW                                                    */
/******************************************************************************/

#define FS45_M_INIT_INH_INT_ADDR         0x05U
#define FS45_M_INIT_INH_INT_DEFAULT      0x06U

/**
 * Inhibit the interrupt pulse for IO_0 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_0_MASK         0x01U
/**
 * Inhibit the interrupt pulse for IO_2 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_2_MASK         0x02U
/**
 * Inhibit the interrupt pulse for IO_3 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_3_MASK         0x04U
/**
 * Inhibit the interrupt pulse for IO_4 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_4_MASK         0x08U
/**
 * Inhibit the interrupt pulse for IO_5 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_5_MASK         0x10U

/**
 * Inhibit the interrupt pulse for IO_0 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_0_SHIFT        0x00U
/**
 * Inhibit the interrupt pulse for IO_2 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_2_SHIFT        0x01U
/**
 * Inhibit the interrupt pulse for IO_3 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_3_SHIFT        0x02U
/**
 * Inhibit the interrupt pulse for IO_4 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_4_SHIFT        0x03U
/**
 * Inhibit the interrupt pulse for IO_5 (masked in IO_G)
 */
#define FS45_RW_M_INT_INH_5_SHIFT        0x04U

/**
 * INT not masked
 */
#define FS45_RW_M_INT_INH_0_NOT_MASKED   (0x00U << FS45_RW_M_INT_INH_0_SHIFT)
/**
 * INT masked
 */
#define FS45_RW_M_INT_INH_0_MASKED       (0x01U << FS45_RW_M_INT_INH_0_SHIFT)

/**
 * INT not masked
 */
#define FS45_RW_M_INT_INH_2_NOT_MASKED   (0x00U << FS45_RW_M_INT_INH_2_SHIFT)
/**
 * INT masked
 */
#define FS45_RW_M_INT_INH_2_MASKED       (0x01U << FS45_RW_M_INT_INH_2_SHIFT)

/**
 * INT not masked
 */
#define FS45_RW_M_INT_INH_3_NOT_MASKED   (0x00U << FS45_RW_M_INT_INH_3_SHIFT)
/**
 * INT masked
 */
#define FS45_RW_M_INT_INH_3_MASKED       (0x01U << FS45_RW_M_INT_INH_3_SHIFT)

/**
 * INT not masked
 */
#define FS45_RW_M_INT_INH_4_NOT_MASKED   (0x00U << FS45_RW_M_INT_INH_4_SHIFT)
/**
 * INT masked
 */
#define FS45_RW_M_INT_INH_4_MASKED       (0x01U << FS45_RW_M_INT_INH_4_SHIFT)

/**
 * INT not masked
 */
#define FS45_RW_M_INT_INH_5_NOT_MASKED   (0x00U << FS45_RW_M_INT_INH_5_SHIFT)
/**
 * INT masked
 */
#define FS45_RW_M_INT_INH_5_MASKED       (0x01U << FS45_RW_M_INT_INH_5_SHIFT)

/******************************************************************************/
/* INIT_FS1B_TIMING - Type: RW                                                */
/******************************************************************************/

#define FS45_FS_INIT_FS1B_TIMING_ADDR    0x21U
#define FS45_FS_INIT_FS1B_TIMING_DEFAULT 0x06U

/**
 * FS1B timing range factor x1(FS1B_TIME_RANGE bit = 0), FS1B timing range factor x8(FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_MASK         0x0FU
/**
 * FS1B timing range factor x1(FS1B_TIME_RANGE bit = 0), FS1B timing range factor x8(FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_MASK         0xF0U

/**
 * FS1B timing range factor x1(FS1B_TIME_RANGE bit = 0), FS1B timing range factor x8(FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_SHIFT        0x00U
/**
 * FS1B timing range factor x1(FS1B_TIME_RANGE bit = 0), FS1B timing range factor x8(FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_SHIFT        0x04U

/**
 * 0
 */
#define FS45_R_FS_FS1B_TIME_0_0          (0x00U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 10 ms (FS1B_TIME_RANGE bit = 0) | 80 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_10MS_80MS    (0x01U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 13 ms (FS1B_TIME_RANGE bit = 0) | 104 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_13_104MS     (0x02U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 17 ms (FS1B_TIME_RANGE bit = 0) | 135 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_17_135MS     (0x03U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 22 ms (FS1B_TIME_RANGE bit = 0) | 176 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_22_176MS     (0x04U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 29 ms (FS1B_TIME_RANGE bit = 0) | 228 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_29_228MS     (0x05U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 37 ms (FS1B_TIME_RANGE bit = 0) | 297 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_37_297MS     (0x06U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 48 ms (FS1B_TIME_RANGE bit = 0) | 386 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_48_386MS     (0x07U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 63 ms (FS1B_TIME_RANGE bit = 0) | 502 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_63_502MS     (0x08U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 82 ms (FS1B_TIME_RANGE bit = 0) | 653 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_82_653MS     (0x09U << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 106 ms (FS1B_TIME_RANGE bit = 0) | 848 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_106_848MS    (0x0AU << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 138 ms (FS1B_TIME_RANGE bit = 0) | 1103 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_138_1103MS   (0x0BU << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 179 ms (FS1B_TIME_RANGE bit = 0) | 1434 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_179_1434MS   (0x0CU << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 233 ms (FS1B_TIME_RANGE bit = 0) | 1864 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_233_1864MS   (0x0DU << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 303 ms (FS1B_TIME_RANGE bit = 0) | 2423 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_303_2423MS   (0x0EU << FS45_R_FS_FS1B_TIME_SHIFT)
/**
 * 394 ms (FS1B_TIME_RANGE bit = 0) | 3150 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_R_FS_FS1B_TIME_394_3150MS   (0x0FU << FS45_R_FS_FS1B_TIME_SHIFT)

/**
 * 0
 */
#define FS45_W_FS_FS1B_TIME_0_0          (0x00U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 10 ms (FS1B_TIME_RANGE bit = 0) | 80 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_10MS_80MS    (0x01U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 13 ms (FS1B_TIME_RANGE bit = 0) | 104 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_13_104MS     (0x02U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 17 ms (FS1B_TIME_RANGE bit = 0) | 135 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_17_135MS     (0x03U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 22 ms (FS1B_TIME_RANGE bit = 0) | 176 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_22_176MS     (0x04U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 29 ms (FS1B_TIME_RANGE bit = 0) | 228 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_29_228MS     (0x05U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 37 ms (FS1B_TIME_RANGE bit = 0) | 297 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_37_297MS     (0x06U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 48 ms (FS1B_TIME_RANGE bit = 0) | 386 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_48_386MS     (0x07U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 63 ms (FS1B_TIME_RANGE bit = 0) | 502 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_63_502MS     (0x08U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 82 ms (FS1B_TIME_RANGE bit = 0) | 653 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_82_653MS     (0x09U << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 106 ms (FS1B_TIME_RANGE bit = 0) | 848 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_106_848MS    (0x0AU << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 138 ms (FS1B_TIME_RANGE bit = 0) | 1103 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_138_1103MS   (0x0BU << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 179 ms (FS1B_TIME_RANGE bit = 0) | 1434 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_179_1434MS   (0x0CU << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 233 ms (FS1B_TIME_RANGE bit = 0) | 1864 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_233_1864MS   (0x0DU << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 303 ms (FS1B_TIME_RANGE bit = 0) | 2423 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_303_2423MS   (0x0EU << FS45_W_FS_FS1B_TIME_SHIFT)
/**
 * 394 ms (FS1B_TIME_RANGE bit = 0) | 3150 ms (FS1B_TIME_RANGE bit = 1)
 */
#define FS45_W_FS_FS1B_TIME_394_3150MS   (0x0FU << FS45_W_FS_FS1B_TIME_SHIFT)

/******************************************************************************/
/* INIT_SUPERVISOR - Type: RW                                                 */
/******************************************************************************/

#define FS45_FS_INIT_SUPERVISOR_ADDR     0x23U
#define FS45_FS_INIT_SUPERVISOR_DEFAULT  0x00U

/**
 * Configure the FS1B timing range factor x1 or x8
 */
#define FS45_R_FS_FS1B_TIME_RANGE_MASK   0x01U
/**
 * Configure the VAUX undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VAUX_5D_MASK           0x02U
/**
 * Configure the VCCA undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VCCA_5D_MASK           0x04U
/**
 * Configure the VCORE undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VCORE_5D_MASK          0x08U
/**
 * Configure the FS1B timing range factor x1 or x8
 */
#define FS45_W_FS_FS1B_TIME_RANGE_MASK   0x10U
/**
 * Configure the VAUX undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VAUX_5D_MASK           0x20U
/**
 * Configure the VCCA undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VCCA_5D_MASK           0x40U
/**
 * Configure the VCORE undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VCORE_5D_MASK          0x80U

/**
 * Configure the FS1B timing range factor x1 or x8
 */
#define FS45_R_FS_FS1B_TIME_RANGE_SHIFT  0x00U
/**
 * Configure the VAUX undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VAUX_5D_SHIFT          0x01U
/**
 * Configure the VCCA undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VCCA_5D_SHIFT          0x02U
/**
 * Configure the VCORE undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_R_FS_VCORE_5D_SHIFT         0x03U
/**
 * Configure the FS1B timing range factor x1 or x8
 */
#define FS45_W_FS_FS1B_TIME_RANGE_SHIFT  0x04U
/**
 * Configure the VAUX undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VAUX_5D_SHIFT          0x05U
/**
 * Configure the VCCA undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VCCA_5D_SHIFT          0x06U
/**
 * Configure the VCORE undervoltage in degraded mode. Only valid for 5.0 V
 */
#define FS45_W_FS_VCORE_5D_SHIFT         0x07U

/**
 * x1 timing range factor
 */
#define FS45_R_FS_FS1B_TIME_RANGE_X1     (0x00U << FS45_R_FS_FS1B_TIME_RANGE_SHIFT)
/**
 * x8 timing range factor
 */
#define FS45_R_FS_FS1B_TIME_RANGE_X8     (0x01U << FS45_R_FS_FS1B_TIME_RANGE_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VAUX_UV_5)
 */
#define FS45_R_FS_VAUX_5D_NORMAL         (0x00U << FS45_R_FS_VAUX_5D_SHIFT)
/**
 * Degraded mode; lower undervoltage detection threshold applied (VAUX_UV_5D)
 */
#define FS45_R_FS_VAUX_5D_DEGRADED       (0x01U << FS45_R_FS_VAUX_5D_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VCCA_UV_5)
 */
#define FS45_R_FS_VCCA_5D_NORMAL         (0x00U << FS45_R_FS_VCCA_5D_SHIFT)
/**
 * Degraded mode, lower undervoltage detection threshold applied (VCCA_UV_D)
 */
#define FS45_R_FS_VCCA_5D_DEGRADED       (0x01U << FS45_R_FS_VCCA_5D_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VCORE_FB_UV)
 */
#define FS45_R_FS_VCORE_5D_NORMAL        (0x00U << FS45_R_FS_VCORE_5D_SHIFT)
/**
 * Degraded mode, lower undervoltage detection threshold applied (VCORE_FB_UV_D)
 */
#define FS45_R_FS_VCORE_5D_DEGRADED      (0x01U << FS45_R_FS_VCORE_5D_SHIFT)

/**
 * x1 timing range factor
 */
#define FS45_W_FS_FS1B_TIME_RANGE_X1     (0x00U << FS45_W_FS_FS1B_TIME_RANGE_SHIFT)
/**
 * x8 timing range factor
 */
#define FS45_W_FS_FS1B_TIME_RANGE_X8     (0x01U << FS45_W_FS_FS1B_TIME_RANGE_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VAUX_UV_5)
 */
#define FS45_W_FS_VAUX_5D_NORMAL         (0x00U << FS45_W_FS_VAUX_5D_SHIFT)
/**
 * Degraded mode; lower undervoltage detection threshold applied (VAUX_UV_5D)
 */
#define FS45_W_FS_VAUX_5D_DEGRADED       (0x01U << FS45_W_FS_VAUX_5D_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VCCA_UV_5)
 */
#define FS45_W_FS_VCCA_5D_NORMAL         (0x00U << FS45_W_FS_VCCA_5D_SHIFT)
/**
 * Degraded mode, lower undervoltage detection threshold applied (VCCA_UV_D)
 */
#define FS45_W_FS_VCCA_5D_DEGRADED       (0x01U << FS45_W_FS_VCCA_5D_SHIFT)

/**
 * Normal 5.0 V undervoltage detection threshold (VCORE_FB_UV)
 */
#define FS45_W_FS_VCORE_5D_NORMAL        (0x00U << FS45_W_FS_VCORE_5D_SHIFT)
/**
 * Degraded mode, lower undervoltage detection threshold applied (VCORE_FB_UV_D)
 */
#define FS45_W_FS_VCORE_5D_DEGRADED      (0x01U << FS45_W_FS_VCORE_5D_SHIFT)

/******************************************************************************/
/* INIT_FAULT - Type: RW                                                      */
/******************************************************************************/

#define FS45_FS_INIT_FAULT_ADDR          0x24U
#define FS45_FS_INIT_FAULT_DEFAULT       0x05U

/**
 * Configure RSTB and FS0B behavior when fault error counter >= intermediate value
 */
#define FS45_R_FS_FLT_ERR_IMP_MASK       0x03U
/**
 * Configure CAN behavior when FS1B is asserted low
 */
#define FS45_R_FS_FS1B_CAN_IMPACT_MASK   0x04U
/**
 * Configure the values of the fault error counter
 */
#define FS45_R_FS_FLT_ERR_FS_MASK        0x08U
/**
 * Configure RSTB and FS0B behavior when fault error counter >= intermediate value
 */
#define FS45_W_FS_FLT_ERR_IMP_MASK       0x30U
/**
 * Configure CAN behavior when FS1B is asserted low
 */
#define FS45_W_FS_FS1B_CAN_IMPACT_MASK   0x40U
/**
 * Configure the values of the fault error counter
 */
#define FS45_W_FS_FLT_ERR_FS_MASK        0x80U

/**
 * Configure RSTB and FS0B behavior when fault error counter >= intermediate value
 */
#define FS45_R_FS_FLT_ERR_IMP_SHIFT      0x00U
/**
 * Configure CAN behavior when FS1B is asserted low
 */
#define FS45_R_FS_FS1B_CAN_IMPACT_SHIFT  0x02U
/**
 * Configure the values of the fault error counter
 */
#define FS45_R_FS_FLT_ERR_FS_SHIFT       0x03U
/**
 * Configure RSTB and FS0B behavior when fault error counter >= intermediate value
 */
#define FS45_W_FS_FLT_ERR_IMP_SHIFT      0x04U
/**
 * Configure CAN behavior when FS1B is asserted low
 */
#define FS45_W_FS_FS1B_CAN_IMPACT_SHIFT  0x06U
/**
 * Configure the values of the fault error counter
 */
#define FS45_W_FS_FLT_ERR_FS_SHIFT       0x07U

/**
 * No effect on RSTB and FS0B
 */
#define FS45_R_FS_FLT_ERR_IMP_NO_EFFECT  (0x00U << FS45_R_FS_FLT_ERR_IMP_SHIFT)
/**
 * FS0B is asserted low if FLT_ERR_CNT >= intermediate value
 */
#define FS45_R_FS_FLT_ERR_IMP_FS0B       (0x01U << FS45_R_FS_FLT_ERR_IMP_SHIFT)
/**
 * RSTB is asserted low if FLT_ERR_CNT >= intermediate value and WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_FLT_ERR_IMP_RSTB       (0x02U << FS45_R_FS_FLT_ERR_IMP_SHIFT)
/**
 * FS0B is asserted low if FLT_ERR_CNT >= intermediate value RSTB is asserted low if FLT_ERR_CNT >= intermediate value 
 * and WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_FLT_ERR_IMP_FS0B_RSTB  (0x03U << FS45_R_FS_FLT_ERR_IMP_SHIFT)

/**
 * No effect
 */
#define FS45_R_FS_FS1B_CAN_IMPACT_NO_EFFECT (0x00U << FS45_R_FS_FS1B_CAN_IMPACT_SHIFT)
/**
 * CAN in Rx only or sleep mode when FS1B is asserted (depends on CAN_DIS_CFG bit in INIT_WU2 register)
 */
#define FS45_R_FS_FS1B_CAN_IMPACT_RX_ONLY (0x01U << FS45_R_FS_FS1B_CAN_IMPACT_SHIFT)

/**
 * intermediate = 3; final = 6
 */
#define FS45_R_FS_FLT_ERR_FS_INT3_FIN6   (0x00U << FS45_R_FS_FLT_ERR_FS_SHIFT)
/**
 * intermediate = 1; final = 2
 */
#define FS45_R_FS_FLT_ERR_FS_INT1_FIN2   (0x01U << FS45_R_FS_FLT_ERR_FS_SHIFT)

/**
 * No effect on RSTB and FS0B
 */
#define FS45_W_FS_FLT_ERR_IMP_NO_EFFECT  (0x00U << FS45_W_FS_FLT_ERR_IMP_SHIFT)
/**
 * FS0B is asserted low if FLT_ERR_CNT >= intermediate value
 */
#define FS45_W_FS_FLT_ERR_IMP_FS0B       (0x01U << FS45_W_FS_FLT_ERR_IMP_SHIFT)
/**
 * RSTB is asserted low if FLT_ERR_CNT >= intermediate value and WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_FLT_ERR_IMP_RSTB       (0x02U << FS45_W_FS_FLT_ERR_IMP_SHIFT)
/**
 * FS0B is asserted low if FLT_ERR_CNT >= intermediate value RSTB is asserted low if FLT_ERR_CNT >= intermediate value 
 * and WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_FLT_ERR_IMP_FS0B_RSTB  (0x03U << FS45_W_FS_FLT_ERR_IMP_SHIFT)

/**
 * No effect
 */
#define FS45_W_FS_FS1B_CAN_IMPACT_NO_EFFECT (0x00U << FS45_W_FS_FS1B_CAN_IMPACT_SHIFT)
/**
 * CAN in Rx only or sleep mode when FS1B is asserted (depends on CAN_DIS_CFG bit in INIT_WU2 register)
 */
#define FS45_W_FS_FS1B_CAN_IMPACT_RX_ONLY (0x01U << FS45_W_FS_FS1B_CAN_IMPACT_SHIFT)

/**
 * intermediate = 3; final = 6
 */
#define FS45_W_FS_FLT_ERR_FS_INT3_FIN6   (0x00U << FS45_W_FS_FLT_ERR_FS_SHIFT)
/**
 * intermediate = 1; final = 2
 */
#define FS45_W_FS_FLT_ERR_FS_INT1_FIN2   (0x01U << FS45_W_FS_FLT_ERR_FS_SHIFT)

/******************************************************************************/
/* INIT_FSSM - Type: RW                                                       */
/******************************************************************************/

#define FS45_FS_INIT_FSSM_ADDR           0x25U
#define FS45_FS_INIT_FSSM_DEFAULT        0x04U

/**
 * Configure the RSTB low duration time
 */
#define FS45_R_FS_RSTB_DURATION_MASK     0x01U
/**
 * Configure the FCCU polarity
 */
#define FS45_R_FS_PS_MASK                0x02U
/**
 * Configure the couple of IO_3:2 as safety inputs for FCCU monitoring
 */
#define FS45_R_FS_IO_23_FS_MASK          0x04U
/**
 * Configure the couple of IO_4:5 as safety inputs for external IC error monitoring
 */
#define FS45_R_FS_IO_45_FS_MASK          0x08U
/**
 * Configure the RSTB low duration time
 */
#define FS45_W_FS_RSTB_DURATION_MASK     0x10U
/**
 * Configure the FCCU polarity
 */
#define FS45_W_FS_PS_MASK                0x20U
/**
 * Configure the couple of IO_3:2 as safety inputs for FCCU monitoring
 */
#define FS45_W_FS_IO_23_FS_MASK          0x40U
/**
 * Configure the couple of IO_4:5 as safety inputs for external IC error monitoring
 */
#define FS45_W_FS_IO_45_FS_MASK          0x80U

/**
 * Configure the RSTB low duration time
 */
#define FS45_R_FS_RSTB_DURATION_SHIFT    0x00U
/**
 * Configure the FCCU polarity
 */
#define FS45_R_FS_PS_SHIFT               0x01U
/**
 * Configure the couple of IO_3:2 as safety inputs for FCCU monitoring
 */
#define FS45_R_FS_IO_23_FS_SHIFT         0x02U
/**
 * Configure the couple of IO_4:5 as safety inputs for external IC error monitoring
 */
#define FS45_R_FS_IO_45_FS_SHIFT         0x03U
/**
 * Configure the RSTB low duration time
 */
#define FS45_W_FS_RSTB_DURATION_SHIFT    0x04U
/**
 * Configure the FCCU polarity
 */
#define FS45_W_FS_PS_SHIFT               0x05U
/**
 * Configure the couple of IO_3:2 as safety inputs for FCCU monitoring
 */
#define FS45_W_FS_IO_23_FS_SHIFT         0x06U
/**
 * Configure the couple of IO_4:5 as safety inputs for external IC error monitoring
 */
#define FS45_W_FS_IO_45_FS_SHIFT         0x07U

/**
 * 10 ms
 */
#define FS45_R_FS_RSTB_DURATION_10MS     (0x00U << FS45_R_FS_RSTB_DURATION_SHIFT)
/**
 * 1.0 ms
 */
#define FS45_R_FS_RSTB_DURATION_1MS      (0x01U << FS45_R_FS_RSTB_DURATION_SHIFT)

/**
 * Fccu_eaout_1:0 active high
 */
#define FS45_R_FS_PS_HIGH                (0x00U << FS45_R_FS_PS_SHIFT)
/**
 * Fccu_eaout_1:0 active low
 */
#define FS45_R_FS_PS_LOW                 (0x01U << FS45_R_FS_PS_SHIFT)

/**
 * Not_safety
 */
#define FS45_R_FS_IO_23_FS_NOT_SAFETY    (0x00U << FS45_R_FS_IO_23_FS_SHIFT)
/**
 * Safety_critical
 */
#define FS45_R_FS_IO_23_FS_SAFETY_CRITICAL (0x01U << FS45_R_FS_IO_23_FS_SHIFT)

/**
 * Not safety
 */
#define FS45_R_FS_IO_45_FS_NOT_SAFETY    (0x00U << FS45_R_FS_IO_45_FS_SHIFT)
/**
 * Safety critical
 */
#define FS45_R_FS_IO_45_FS_SAFETY_CRITICAL (0x01U << FS45_R_FS_IO_45_FS_SHIFT)

/**
 * 10 ms
 */
#define FS45_W_FS_RSTB_DURATION_10MS     (0x00U << FS45_W_FS_RSTB_DURATION_SHIFT)
/**
 * 1.0 ms
 */
#define FS45_W_FS_RSTB_DURATION_1MS      (0x01U << FS45_W_FS_RSTB_DURATION_SHIFT)

/**
 * Fccu_eaout_1:0 active high
 */
#define FS45_W_FS_PS_HIGH                (0x00U << FS45_W_FS_PS_SHIFT)
/**
 * Fccu_eaout_1:0 active low
 */
#define FS45_W_FS_PS_LOW                 (0x01U << FS45_W_FS_PS_SHIFT)

/**
 * Not_safety
 */
#define FS45_W_FS_IO_23_FS_NOT_SAFETY    (0x00U << FS45_W_FS_IO_23_FS_SHIFT)
/**
 * Safety_critical
 */
#define FS45_W_FS_IO_23_FS_SAFETY_CRITICAL (0x01U << FS45_W_FS_IO_23_FS_SHIFT)

/**
 * Not safety
 */
#define FS45_W_FS_IO_45_FS_NOT_SAFETY    (0x00U << FS45_W_FS_IO_45_FS_SHIFT)
/**
 * Safety critical
 */
#define FS45_W_FS_IO_45_FS_SAFETY_CRITICAL (0x01U << FS45_W_FS_IO_45_FS_SHIFT)

/******************************************************************************/
/* INIT_SF_IMPACT - Type: RW                                                  */
/******************************************************************************/

#define FS45_FS_INIT_SF_IMPACT_ADDR      0x26U
#define FS45_FS_INIT_SF_IMPACT_DEFAULT   0x01U

/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_R_FS_WD_IMPACT_MASK         0x03U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_R_FS_DIS_8S_MASK            0x04U
/**
 * FS1B delay or FS1B duration mode selection
 */
#define FS45_R_FS_TDLY_TDUR_MASK         0x08U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_W_FS_WD_IMPACT_MASK         0x30U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_W_FS_DIS_8S_MASK            0x40U
/**
 * FS1B delay or FS1B duration mode selection
 */
#define FS45_W_FS_TDLY_TDUR_MASK         0x80U

/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_R_FS_WD_IMPACT_SHIFT        0x00U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_R_FS_DIS_8S_SHIFT           0x02U
/**
 * FS1B delay or FS1B duration mode selection
 */
#define FS45_R_FS_TDLY_TDUR_SHIFT        0x03U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_W_FS_WD_IMPACT_SHIFT        0x04U
/**
 * Watchdog impact on RSTB and/or FS0B assertion
 */
#define FS45_W_FS_DIS_8S_SHIFT           0x06U
/**
 * FS1B delay or FS1B duration mode selection
 */
#define FS45_W_FS_TDLY_TDUR_SHIFT        0x07U

/**
 * No effect on RSTB and FS0B if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_WD_IMPACT_NO_EFFECT    (0x00U << FS45_R_FS_WD_IMPACT_SHIFT)
/**
 * RSTB only is asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_WD_IMPACT_RSTB         (0x01U << FS45_R_FS_WD_IMPACT_SHIFT)
/**
 * FS0B only is asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_WD_IMPACT_FS0B         (0x02U << FS45_R_FS_WD_IMPACT_SHIFT)
/**
 * RSTB and FS0B are asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_R_FS_WD_IMPACT_RSTB_FS0B    (0x03U << FS45_R_FS_WD_IMPACT_SHIFT)

/**
 * Enabled
 */
#define FS45_R_FS_DIS_8S_ENABLED         (0x00U << FS45_R_FS_DIS_8S_SHIFT)
/**
 * Disabled
 */
#define FS45_R_FS_DIS_8S_DISABLED        (0x01U << FS45_R_FS_DIS_8S_SHIFT)

/**
 * FS1B tDELAY mode
 */
#define FS45_R_FS_TDLY_TDUR_DELAY        (0x00U << FS45_R_FS_TDLY_TDUR_SHIFT)
/**
 * FS1B tDURATION mode
 */
#define FS45_R_FS_TDLY_TDUR_DURATION     (0x01U << FS45_R_FS_TDLY_TDUR_SHIFT)

/**
 * No effect on RSTB and FS0B if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_WD_IMPACT_NO_EFFECT    (0x00U << FS45_W_FS_WD_IMPACT_SHIFT)
/**
 * RSTB only is asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_WD_IMPACT_RSTB         (0x01U << FS45_W_FS_WD_IMPACT_SHIFT)
/**
 * FS0B only is asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_WD_IMPACT_FS0B         (0x02U << FS45_W_FS_WD_IMPACT_SHIFT)
/**
 * RSTB and FS0B are asserted low if WD error counter = WD_CNT_ERR[1:0]
 */
#define FS45_W_FS_WD_IMPACT_RSTB_FS0B    (0x03U << FS45_W_FS_WD_IMPACT_SHIFT)

/**
 * Enabled
 */
#define FS45_W_FS_DIS_8S_ENABLED         (0x00U << FS45_W_FS_DIS_8S_SHIFT)
/**
 * Disabled
 */
#define FS45_W_FS_DIS_8S_DISABLED        (0x01U << FS45_W_FS_DIS_8S_SHIFT)

/**
 * FS1B tDELAY mode
 */
#define FS45_W_FS_TDLY_TDUR_DELAY        (0x00U << FS45_W_FS_TDLY_TDUR_SHIFT)
/**
 * FS1B tDURATION mode
 */
#define FS45_W_FS_TDLY_TDUR_DURATION     (0x01U << FS45_W_FS_TDLY_TDUR_SHIFT)

/******************************************************************************/
/* WD_WINDOW - Type: RW                                                       */
/******************************************************************************/

#define FS45_FS_WD_WINDOW_ADDR           0x27U
#define FS45_FS_WD_WINDOW_DEFAULT        0x03U

/**
 * Configure the watchdog window duration. Duty cycle is set to 50 %
 */
#define FS45_R_FS_WD_WINDOW_MASK         0x0FU
/**
 * Configure the watchdog window duration. Duty cycle is set to 50 %
 */
#define FS45_W_FS_WD_WINDOW_MASK         0xF0U

/**
 * Configure the watchdog window duration. Duty cycle is set to 50 %
 */
#define FS45_R_FS_WD_WINDOW_SHIFT        0x00U
/**
 * Configure the watchdog window duration. Duty cycle is set to 50 %
 */
#define FS45_W_FS_WD_WINDOW_SHIFT        0x04U

/**
 * Disable (in INIT phase only)
 */
#define FS45_R_FS_WD_WINDOW_DISABLE      (0x00U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 1.0 ms
 */
#define FS45_R_FS_WD_WINDOW_1MS          (0x01U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 2.0 ms
 */
#define FS45_R_FS_WD_WINDOW_2MS          (0x02U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 3.0 ms
 */
#define FS45_R_FS_WD_WINDOW_3MS          (0x03U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 4.0 ms
 */
#define FS45_R_FS_WD_WINDOW_4MS          (0x04U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 6.0 ms
 */
#define FS45_R_FS_WD_WINDOW_6MS          (0x05U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 8.0 ms
 */
#define FS45_R_FS_WD_WINDOW_8MS          (0x06U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 12 ms
 */
#define FS45_R_FS_WD_WINDOW_12MS         (0x07U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 16 ms
 */
#define FS45_R_FS_WD_WINDOW_16MS         (0x08U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 24 ms
 */
#define FS45_R_FS_WD_WINDOW_24MS         (0x09U << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 32 ms
 */
#define FS45_R_FS_WD_WINDOW_32MS         (0x0AU << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 64 ms
 */
#define FS45_R_FS_WD_WINDOW_64MS         (0x0BU << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 128 ms
 */
#define FS45_R_FS_WD_WINDOW_128MS        (0x0CU << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 256 ms
 */
#define FS45_R_FS_WD_WINDOW_256MS        (0x0DU << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 512 ms
 */
#define FS45_R_FS_WD_WINDOW_512MS        (0x0EU << FS45_R_FS_WD_WINDOW_SHIFT)
/**
 * 1024 ms
 */
#define FS45_R_FS_WD_WINDOW_1024MS       (0x0FU << FS45_R_FS_WD_WINDOW_SHIFT)

/**
 * Disable (in INIT phase only)
 */
#define FS45_W_FS_WD_WINDOW_DISABLE      (0x00U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 1.0 ms
 */
#define FS45_W_FS_WD_WINDOW_1MS          (0x01U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 2.0 ms
 */
#define FS45_W_FS_WD_WINDOW_2MS          (0x02U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 3.0 ms
 */
#define FS45_W_FS_WD_WINDOW_3MS          (0x03U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 4.0 ms
 */
#define FS45_W_FS_WD_WINDOW_4MS          (0x04U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 6.0 ms
 */
#define FS45_W_FS_WD_WINDOW_6MS          (0x05U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 8.0 ms
 */
#define FS45_W_FS_WD_WINDOW_8MS          (0x06U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 12 ms
 */
#define FS45_W_FS_WD_WINDOW_12MS         (0x07U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 16 ms
 */
#define FS45_W_FS_WD_WINDOW_16MS         (0x08U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 24 ms
 */
#define FS45_W_FS_WD_WINDOW_24MS         (0x09U << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 32 ms
 */
#define FS45_W_FS_WD_WINDOW_32MS         (0x0AU << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 64 ms
 */
#define FS45_W_FS_WD_WINDOW_64MS         (0x0BU << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 128 ms
 */
#define FS45_W_FS_WD_WINDOW_128MS        (0x0CU << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 256 ms
 */
#define FS45_W_FS_WD_WINDOW_256MS        (0x0DU << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 512 ms
 */
#define FS45_W_FS_WD_WINDOW_512MS        (0x0EU << FS45_W_FS_WD_WINDOW_SHIFT)
/**
 * 1024 ms
 */
#define FS45_W_FS_WD_WINDOW_1024MS       (0x0FU << FS45_W_FS_WD_WINDOW_SHIFT)

/******************************************************************************/
/* WD_LFSR - Type: RW                                                         */
/******************************************************************************/

#define FS45_FS_WD_LFSR_ADDR             0x28U
#define FS45_FS_WD_LFSR_DEFAULT          0xB2U

/**
 * WD 8 bits LFSR value. Used to write the seed at any time. Default value at start-up or after a power on reset: 0xB2 
 * bit7:bit0: 10110010. Value Bit7:Bit0: 1111 1111 is prohibited. During a write command, MISO reports the previous 
 * register content.
 */
#define FS45_RW_FS_WD_LFSR_MASK          0xFFU

/**
 * WD 8 bits LFSR value. Used to write the seed at any time. Default value at start-up or after a power on reset: 0xB2 
 * bit7:bit0: 10110010. Value Bit7:Bit0: 1111 1111 is prohibited. During a write command, MISO reports the previous 
 * register content.
 */
#define FS45_RW_FS_WD_LFSR_SHIFT         0x00U

/******************************************************************************/
/* INIT_WD_CNT - Type: RW                                                     */
/******************************************************************************/

#define FS45_FS_INIT_WD_CNT_ADDR         0x2CU
#define FS45_FS_INIT_WD_CNT_DEFAULT      0x00U

/**
 * Configure the maximum value of the WD refresh counter
 */
#define FS45_R_FS_WD_CNT_RFR_MASK        0x03U
/**
 * Configure the maximum value of the WD error counter
 */
#define FS45_R_FS_WD_CNT_ERR_MASK        0x0CU
/**
 * Configure the maximum value of the WD refresh counter
 */
#define FS45_W_FS_WD_CNT_RFR_MASK        0x30U
/**
 * Configure the maximum value of the WD error counter
 */
#define FS45_W_FS_WD_CNT_ERR_MASK        0xC0U

/**
 * Configure the maximum value of the WD refresh counter
 */
#define FS45_R_FS_WD_CNT_RFR_SHIFT       0x00U
/**
 * Configure the maximum value of the WD error counter
 */
#define FS45_R_FS_WD_CNT_ERR_SHIFT       0x02U
/**
 * Configure the maximum value of the WD refresh counter
 */
#define FS45_W_FS_WD_CNT_RFR_SHIFT       0x04U
/**
 * Configure the maximum value of the WD error counter
 */
#define FS45_W_FS_WD_CNT_ERR_SHIFT       0x06U

/**
 * 6
 */
#define FS45_R_FS_WD_CNT_RFR_6           (0x00U << FS45_R_FS_WD_CNT_RFR_SHIFT)
/**
 * 4
 */
#define FS45_R_FS_WD_CNT_RFR_4           (0x01U << FS45_R_FS_WD_CNT_RFR_SHIFT)
/**
 * 2
 */
#define FS45_R_FS_WD_CNT_RFR_2           (0x02U << FS45_R_FS_WD_CNT_RFR_SHIFT)
/**
 * 1
 */
#define FS45_R_FS_WD_CNT_RFR_1           (0x03U << FS45_R_FS_WD_CNT_RFR_SHIFT)

/**
 * 6
 */
#define FS45_R_FS_WD_CNT_ERR_6           (0x00U << FS45_R_FS_WD_CNT_ERR_SHIFT)
/**
 * 4
 */
#define FS45_R_FS_WD_CNT_ERR_4           (0x02U << FS45_R_FS_WD_CNT_ERR_SHIFT)
/**
 * 2
 */
#define FS45_R_FS_WD_CNT_ERR_2           (0x03U << FS45_R_FS_WD_CNT_ERR_SHIFT)

/**
 * 6
 */
#define FS45_W_FS_WD_CNT_RFR_6           (0x00U << FS45_W_FS_WD_CNT_RFR_SHIFT)
/**
 * 4
 */
#define FS45_W_FS_WD_CNT_RFR_4           (0x01U << FS45_W_FS_WD_CNT_RFR_SHIFT)
/**
 * 2
 */
#define FS45_W_FS_WD_CNT_RFR_2           (0x02U << FS45_W_FS_WD_CNT_RFR_SHIFT)
/**
 * 1
 */
#define FS45_W_FS_WD_CNT_RFR_1           (0x03U << FS45_W_FS_WD_CNT_RFR_SHIFT)

/**
 * 6
 */
#define FS45_W_FS_WD_CNT_ERR_6           (0x00U << FS45_W_FS_WD_CNT_ERR_SHIFT)
/**
 * 4
 */
#define FS45_W_FS_WD_CNT_ERR_4           (0x02U << FS45_W_FS_WD_CNT_ERR_SHIFT)
/**
 * 2
 */
#define FS45_W_FS_WD_CNT_ERR_2           (0x03U << FS45_W_FS_WD_CNT_ERR_SHIFT)

/******************************************************************************/
/* INIT_VCORE_OVUV_IMPACT - Type: RW                                          */
/******************************************************************************/

#define FS45_FS_INIT_VCORE_OVUV_IMPACT_ADDR 0x31U
#define FS45_VCORE_OVUV_IMPACT_DEFAULT 0x0EU

/**
 * VCORE_FB undervoltage safety impact
 */
#define FS45_R_FS_VCORE_FS_UV_MASK       0x03U
/**
 * VCORE_FB overvoltage safety impact
 */
#define FS45_R_FS_VCORE_FS_OV_MASK       0x0CU
/**
 * VCORE_FB undervoltage safety impact
 */
#define FS45_W_FS_VCORE_FS_UV_MASK       0x30U
/**
 * VCORE_FB overvoltage safety impact
 */
#define FS45_W_FS_VCORE_FS_OV_MASK       0xC0U

/**
 * VCORE_FB undervoltage safety impact
 */
#define FS45_R_FS_VCORE_FS_UV_SHIFT      0x00U
/**
 * VCORE_FB overvoltage safety impact
 */
#define FS45_R_FS_VCORE_FS_OV_SHIFT      0x02U
/**
 * VCORE_FB undervoltage safety impact
 */
#define FS45_W_FS_VCORE_FS_UV_SHIFT      0x04U
/**
 * VCORE_FB overvoltage safety impact
 */
#define FS45_W_FS_VCORE_FS_OV_SHIFT      0x06U

/**
 * No effect of VCORE_FB_UV on RSTB and FS0B
 */
#define FS45_R_FS_VCORE_FS_UV_NO_EFFECT  (0x00U << FS45_R_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on RSTB only
 */
#define FS45_R_FS_VCORE_FS_UV_RSTB       (0x01U << FS45_R_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on FS0B only
 */
#define FS45_R_FS_VCORE_FS_UV_FS0B       (0x02U << FS45_R_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VCORE_FS_UV_RSTB_FS0B  (0x03U << FS45_R_FS_VCORE_FS_UV_SHIFT)

/**
 * No effect of VCORE_FB_OV on RSTB and FS0B
 */
#define FS45_R_FS_VCORE_FS_OV_NO_EFFECT  (0x00U << FS45_R_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on RSTB only
 */
#define FS45_R_FS_VCORE_FS_OV_RSTB       (0x01U << FS45_R_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on FS0B only
 */
#define FS45_R_FS_VCORE_FS_OV_FS0B       (0x02U << FS45_R_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VCORE_FS_OV_RSTB_FS0B  (0x03U << FS45_R_FS_VCORE_FS_OV_SHIFT)

/**
 * No effect of VCORE_FB_UV on RSTB and FS0B
 */
#define FS45_W_FS_VCORE_FS_UV_NO_EFFECT  (0x00U << FS45_W_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on RSTB only
 */
#define FS45_W_FS_VCORE_FS_UV_RSTB       (0x01U << FS45_W_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on FS0B only
 */
#define FS45_W_FS_VCORE_FS_UV_FS0B       (0x02U << FS45_W_FS_VCORE_FS_UV_SHIFT)
/**
 * VCORE_FB_UV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VCORE_FS_UV_RSTB_FS0B  (0x03U << FS45_W_FS_VCORE_FS_UV_SHIFT)

/**
 * No effect of VCORE_FB_OV on RSTB and FS0B
 */
#define FS45_W_FS_VCORE_FS_OV_NO_EFFECT  (0x00U << FS45_W_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on RSTB only
 */
#define FS45_W_FS_VCORE_FS_OV_RSTB       (0x01U << FS45_W_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on FS0B only
 */
#define FS45_W_FS_VCORE_FS_OV_FS0B       (0x02U << FS45_W_FS_VCORE_FS_OV_SHIFT)
/**
 * VCORE_FB_OV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VCORE_FS_OV_RSTB_FS0B  (0x03U << FS45_W_FS_VCORE_FS_OV_SHIFT)

/******************************************************************************/
/* INIT_VCCA_OVUV_IMPACT - Type: RW                                           */
/******************************************************************************/

#define FS45_FS_INIT_VCCA_OVUV_IMPACT_ADDR 0x32U
#define FS45_FS_INIT_VCCA_OVUV_IMPACT_DEFAULT 0x0EU

/**
 * VCCA undervoltage safety impact
 */
#define FS45_R_FS_VCCA_FS_UV_MASK        0x03U
/**
 * VCCA overvoltage safety impact
 */
#define FS45_R_FS_VCCA_FS_OV_MASK        0x0CU
/**
 * VCCA undervoltage safety impact
 */
#define FS45_W_FS_VCCA_FS_UV_MASK        0x30U
/**
 * VCCA overvoltage safety impact
 */
#define FS45_W_FS_VCCA_FS_OV_MASK        0xC0U

/**
 * VCCA undervoltage safety impact
 */
#define FS45_R_FS_VCCA_FS_UV_SHIFT       0x00U
/**
 * VCCA overvoltage safety impact
 */
#define FS45_R_FS_VCCA_FS_OV_SHIFT       0x02U
/**
 * VCCA undervoltage safety impact
 */
#define FS45_W_FS_VCCA_FS_UV_SHIFT       0x04U
/**
 * VCCA overvoltage safety impact
 */
#define FS45_W_FS_VCCA_FS_OV_SHIFT       0x06U

/**
 * No effect of VCCA_UV on RSTB and FS0B
 */
#define FS45_R_FS_VCCA_FS_UV_NO_EFFECT   (0x00U << FS45_R_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on RSTB only
 */
#define FS45_R_FS_VCCA_FS_UV_RSTB        (0x01U << FS45_R_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on FS0B only
 */
#define FS45_R_FS_VCCA_FS_UV_FS0B        (0x02U << FS45_R_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VCCA_FS_UV_RSTB_FS0B   (0x03U << FS45_R_FS_VCCA_FS_UV_SHIFT)

/**
 * No effect of VCCA_OV on RSTB and FS0B
 */
#define FS45_R_FS_VCCA_FS_OV_NO_EFFECT   (0x00U << FS45_R_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on RSTB only
 */
#define FS45_R_FS_VCCA_FS_OV_RSTB        (0x01U << FS45_R_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on FS0B only
 */
#define FS45_R_FS_VCCA_FS_OV_FS0B        (0x02U << FS45_R_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VCCA_FS_OV_RSTB_FS0B   (0x03U << FS45_R_FS_VCCA_FS_OV_SHIFT)

/**
 * No effect of VCCA_UV on RSTB and FS0B
 */
#define FS45_W_FS_VCCA_FS_UV_NO_EFFECT   (0x00U << FS45_W_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on RSTB only
 */
#define FS45_W_FS_VCCA_FS_UV_RSTB        (0x01U << FS45_W_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on FS0B only
 */
#define FS45_W_FS_VCCA_FS_UV_FS0B        (0x02U << FS45_W_FS_VCCA_FS_UV_SHIFT)
/**
 * VCCA_UV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VCCA_FS_UV_RSTB_FS0B   (0x03U << FS45_W_FS_VCCA_FS_UV_SHIFT)

/**
 * No effect of VCCA_OV on RSTB and FS0B
 */
#define FS45_W_FS_VCCA_FS_OV_NO_EFFECT   (0x00U << FS45_W_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on RSTB only
 */
#define FS45_W_FS_VCCA_FS_OV_RSTB        (0x01U << FS45_W_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on FS0B only
 */
#define FS45_W_FS_VCCA_FS_OV_FS0B        (0x02U << FS45_W_FS_VCCA_FS_OV_SHIFT)
/**
 * VCCA_OV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VCCA_FS_OV_RSTB_FS0B   (0x03U << FS45_W_FS_VCCA_FS_OV_SHIFT)

/******************************************************************************/
/* INIT_VAUX_OVUV_IMPACT - Type: RW                                           */
/******************************************************************************/

#define FS45_FS_INIT_VAUX_OVUV_IMPACT_ADDR 0x33U
#define FS45_FS_INIT_VAUX_OVUV_IMPACT_DEFAULT 0x0EU

/**
 * VAUX undervoltage safety impact
 */
#define FS45_R_FS_VAUX_FS_UV_MASK        0x03U
/**
 * VAUX overvoltage safety impact
 */
#define FS45_R_FS_VAUX_FS_OV_MASK        0x0CU
/**
 * VAUX undervoltage safety impact
 */
#define FS45_W_FS_VAUX_FS_UV_MASK        0x30U
/**
 * VAUX overvoltage safety impact
 */
#define FS45_W_FS_VAUX_FS_OV_MASK        0xC0U

/**
 * VAUX undervoltage safety impact
 */
#define FS45_R_FS_VAUX_FS_UV_SHIFT       0x00U
/**
 * VAUX overvoltage safety impact
 */
#define FS45_R_FS_VAUX_FS_OV_SHIFT       0x02U
/**
 * VAUX undervoltage safety impact
 */
#define FS45_W_FS_VAUX_FS_UV_SHIFT       0x04U
/**
 * VAUX overvoltage safety impact
 */
#define FS45_W_FS_VAUX_FS_OV_SHIFT       0x06U

/**
 * No effect of VAUX_UV on RSTB and FS0B
 */
#define FS45_R_FS_VAUX_FS_UV_NO_EFFECT   (0x00U << FS45_R_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on RSTB only
 */
#define FS45_R_FS_VAUX_FS_UV_RSTB        (0x01U << FS45_R_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on FS0B only
 */
#define FS45_R_FS_VAUX_FS_UV_FS0B        (0x02U << FS45_R_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VAUX_FS_UV_RSTB_FS0B   (0x03U << FS45_R_FS_VAUX_FS_UV_SHIFT)

/**
 * No effect of VAUX_OV on RSTB and FS0B
 */
#define FS45_R_FS_VAUX_FS_OV_NO_EFFECT   (0x00U << FS45_R_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on RSTB only
 */
#define FS45_R_FS_VAUX_FS_OV_RSTB        (0x01U << FS45_R_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on FS0B only
 */
#define FS45_R_FS_VAUX_FS_OV_FS0B        (0x02U << FS45_R_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on RSTB and FS0B
 */
#define FS45_R_FS_VAUX_FS_OV_RSTB_FS0B   (0x03U << FS45_R_FS_VAUX_FS_OV_SHIFT)

/**
 * No effect of VAUX_UV on RSTB and FS0B
 */
#define FS45_W_FS_VAUX_FS_UV_NO_EFFECT   (0x00U << FS45_W_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on RSTB only
 */
#define FS45_W_FS_VAUX_FS_UV_RSTB        (0x01U << FS45_W_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on FS0B only
 */
#define FS45_W_FS_VAUX_FS_UV_FS0B        (0x02U << FS45_W_FS_VAUX_FS_UV_SHIFT)
/**
 * VAUX_UV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VAUX_FS_UV_RSTB_FS0B   (0x03U << FS45_W_FS_VAUX_FS_UV_SHIFT)

/**
 * No effect of VAUX_OV on RSTB and FS0B
 */
#define FS45_W_FS_VAUX_FS_OV_NO_EFFECT   (0x00U << FS45_W_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on RSTB only
 */
#define FS45_W_FS_VAUX_FS_OV_RSTB        (0x01U << FS45_W_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on FS0B only
 */
#define FS45_W_FS_VAUX_FS_OV_FS0B        (0x02U << FS45_W_FS_VAUX_FS_OV_SHIFT)
/**
 * VAUX_OV does have an impact on RSTB and FS0B
 */
#define FS45_W_FS_VAUX_FS_OV_RSTB_FS0B   (0x03U << FS45_W_FS_VAUX_FS_OV_SHIFT)

/******************************************************************************/
/* INIT_INT - Type: RW                                                        */
/******************************************************************************/

#define FS45_M_INIT_INT_ADDR             0x04U
#define FS45_M_INIT_INT_DEFAULT          0x00U

/**
 * Inhibit the interrupt for CAN error bits
 */
#define FS45_RW_M_INT_INH_CAN_MASK       0x01U
/**
 * Inhibit the interrupt for VCCA/VAUX and VCAN status event
 */
#define FS45_RW_M_INT_INH_VOTHER_MASK    0x02U
/**
 * Inhibit the interrupt for VCORE status event
 */
#define FS45_RW_M_INT_INH_VCORE_MASK     0x04U
/**
 * Inhibit the interrupt for VPRE status event
 */
#define FS45_RW_M_INT_INH_VPRE_MASK      0x08U
/**
 * Inhibit the interrupt for VSNS_UV
 */
#define FS45_RW_M_INT_INH_VSNS_MASK      0x10U
/**
 * Inhibit ALL the interrupt
 */
#define FS45_RW_M_INT_INH_ALL_MASK       0x20U
/**
 * Inhibit the interrupt for LIN error bits
 */
#define FS45_RW_M_INT_INH_LIN_MASK       0x40U
/**
 * Define the duration of the interrupt pulse
 */
#define FS45_RW_M_NT_DURATION_MASK       0x80U

/**
 * Inhibit the interrupt for CAN error bits
 */
#define FS45_RW_M_INT_INH_CAN_SHIFT      0x00U
/**
 * Inhibit the interrupt for VCCA/VAUX and VCAN status event
 */
#define FS45_RW_M_INT_INH_VOTHER_SHIFT   0x01U
/**
 * Inhibit the interrupt for VCORE status event
 */
#define FS45_RW_M_INT_INH_VCORE_SHIFT    0x02U
/**
 * Inhibit the interrupt for VPRE status event
 */
#define FS45_RW_M_INT_INH_VPRE_SHIFT     0x03U
/**
 * Inhibit the interrupt for VSNS_UV
 */
#define FS45_RW_M_INT_INH_VSNS_SHIFT     0x04U
/**
 * Inhibit ALL the interrupt
 */
#define FS45_RW_M_INT_INH_ALL_SHIFT      0x05U
/**
 * Inhibit the interrupt for LIN error bits
 */
#define FS45_RW_M_INT_INH_LIN_SHIFT      0x06U
/**
 * Define the duration of the interrupt pulse
 */
#define FS45_RW_M_NT_DURATION_SHIFT      0x07U

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_CAN_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_CAN_SHIFT)
/**
 * CAN error bits change inhibited
 */
#define FS45_RW_M_INT_INH_CAN_CAN_INHIBITED (0x01U << FS45_RW_M_INT_INH_CAN_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_VOTHER_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_VOTHER_SHIFT)
/**
 * VCCA/VAUX/VCAN status change inhibited
 */
#define FS45_RW_M_INT_INH_VOTHER_VOTHER_INHIBITED (0x01U << FS45_RW_M_INT_INH_VOTHER_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_VCORE_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_VCORE_SHIFT)
/**
 * VCORE status change inhibited
 */
#define FS45_RW_M_INT_INH_VCORE_VCORE_INHIBITED (0x01U << FS45_RW_M_INT_INH_VCORE_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_VPRE_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_VPRE_SHIFT)
/**
 * VPRE status change inhibited
 */
#define FS45_RW_M_INT_INH_VPRE_VPRE_INHIBITED (0x01U << FS45_RW_M_INT_INH_VPRE_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_VSNS_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_VSNS_SHIFT)
/**
 * VSNS_UV INT inhibited
 */
#define FS45_RW_M_INT_INH_VSNS_VSNS_UV_INHIBITED (0x01U << FS45_RW_M_INT_INH_VSNS_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_ALL_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_ALL_SHIFT)
/**
 * All INT inhibited
 */
#define FS45_RW_M_INT_INH_ALL_ALL_INHIBITED (0x01U << FS45_RW_M_INT_INH_ALL_SHIFT)

/**
 * All INT sources
 */
#define FS45_RW_M_INT_INH_LIN_ALL_SOURCES (0x00U << FS45_RW_M_INT_INH_LIN_SHIFT)
/**
 * LIN error bits change INHIBITED
 */
#define FS45_RW_M_INT_INH_LIN_LIN_INHIBITED (0x01U << FS45_RW_M_INT_INH_LIN_SHIFT)

/**
 * 100 us
 */
#define FS45_RW_M_NT_DURATION_100US      (0x00U << FS45_RW_M_NT_DURATION_SHIFT)
/**
 * 25 us
 */
#define FS45_RW_M_NT_DURATION_25US       (0x01U << FS45_RW_M_NT_DURATION_SHIFT)


#endif /* SBC_FS45_MAP_H__ */
