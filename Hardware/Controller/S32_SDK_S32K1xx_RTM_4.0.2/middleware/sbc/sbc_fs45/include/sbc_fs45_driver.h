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

#ifndef SBC_FS45_H_
#define SBC_FS45_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "sbc_fs45_map.h"
#include "lpspi_master_driver.h"

/*!
 * @addtogroup sbc_fs45_driver SBC FS45 Driver
 * @ingroup sbc_fs45
 * @brief SBC FS45 driver
 * @{
 */
 /*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief FSxb pin release options.
 *
 * Implements : fs45_fsxb_release_t_Enumeration
 */
typedef enum
{
    fs45ReleaseFs0b,    /**< @brief Release FS0b pin only. */
    fs45ReleaseFs1b,    /**< @brief Release FS1b pin only. */
    fs45ReleaseFs0bFs1b /**< @brief Release both FS0b and FS1b pins. */
} fs45_fsxb_release_t;

/**
 * @brief Previous SBC state.
 *
 * Implements : fs45_prev_mode_t_Enumeration
 */
typedef enum
{
    fs45PrevModePOR,  /**< @brief Previous SBC mode was POR (power on reset). */
    fs45PrevModeDFS,  /**< @brief Resume from deep fail-safe mode. */
    fs45PrevModeLPOFF /**< @brief Resume from LPOFF mode. */
} fs45_prev_mode_t;

/**
 * @brief Actual SBC state.
 *
 * Implements : fs45_current_mode_t_Enumeration
 */
typedef enum
{
    fs45ModeUnknown, /**< @brief Current SBC mode is unknown. */
    fs45ModeInit,    /**< @brief Current SBC mode is INIT. */
    fs45ModeNormal   /**< @brief Current SBC mode is NORMAL. */
} fs45_current_mode_t;

/**
 * @brief Voltage outputs. Can be used with function @ref FS45_SetRegulatorState().
 *
 * Implements : fs45_reg_mode_t_Enumeration
 */
typedef enum
{
    /** @brief VCAN control. */
    fs45VCan,
    /** @brief VAUX control (switch off not recommended if VAUX is safety critical). */
    fs45Aux,
    /** @brief VCCA control (switch off not recommended if VCCA is safety critical). */
    fs45Vcca,
    /** @brief VCORE control (switch off not recommended if VCORE is safety critical). */
    fs45Vcore,
    /** @brief VKAM control. */
    fs45Vkam
} fs45_reg_mode_t;

/**
 * @brief AMUX channel selection. Can be used with function @ref FS45_SwitchAMUXchannel().
 *
 * Implements : fs45_amux_selection_t_Enumeration
 */
typedef enum
{
    fs45AmuxVref = FS45_RW_M_AMUX_VREF,                /**< @brief V_REF. */
    fs45AmuxVsnsWide = FS45_RW_M_AMUX_VSNS_W,          /**< @brief V_SNS wide range.  */
    fs45AmuxIO_0Wide = FS45_RW_M_AMUX_IO_0_W,          /**< @brief IO_0 wide range. */
    fs45AmuxIO_5Wide = FS45_RW_M_AMUX_IO_5_W,          /**< @brief IO_5 wide range. */
    fs45AmuxVsnsTight = FS45_RW_M_AMUX_VSNS_T,         /**< @brief V_SNS tight range. */
    fs45AmuxIO_0Tight = FS45_RW_M_AMUX_IO_0_T,         /**< @brief IO_0 tight range. */
    fs45AmuxIO_5TightDivVkam = FS45_RW_M_AMUX_IO_5_T,  /**< @brief IO_5 tight range/VKAM. */
    fs45AmuxDieTempSensor = FS45_RW_M_AMUX_TEMP_SENSOR /**< @brief Die Temperature Sensor. */
} fs45_amux_selection_t;

/**
 * @brief CAN mode. Can be used with function @ref FS45_CAN_SetMode().
 *
 * Implements : fs45_can_mode_t_Enumeration
 */
typedef enum
{
    fs45CanModeSleepNoWakeup = FS45_RW_M_CAN_MODE_SLN_WU,   /**< @brief Sleep/no wake-up capability. */
    fs45CanModeListenOnly = FS45_RW_M_CAN_MODE_LISTEN_ONLY, /**< @brief Listen only. */
    fs45CanModeSleepWakeup = FS45_RW_M_CAN_MODE_SL_WU,      /**< @brief Sleep/wake-up capability. */
    fs45CanModeNormal = FS45_RW_M_CAN_MODE_NORMAL           /**< @brief Normal operation mode. */
} fs45_can_mode_t;

#ifdef FS45_FEATURE_LIN
/**
 * @brief LIN mode. Can be used with function @ref FS45_LIN_SetMode().
 *
 * Implements : fs45_lin_mode_t_Enumeration
 */
typedef enum
{
    fs45LinModeSleepNoWakeup = FS45_RW_M_LIN_MODE_SLN_WU,   /**< @brief Sleep/no wake-up capability. */
    fs45LinModeListenOnly = FS45_RW_M_LIN_MODE_LISTEN_ONLY, /**< @brief Listen only. */
    fs45LinModeSleepWakeup = FS45_RW_M_LIN_MODE_SL_WU,      /**< @brief Sleep/wake-up capability. */
    fs45LinModeNormal = FS45_RW_M_LIN_MODE_NORMAL           /**< @brief Normal operation mode. */
} fs45_lin_mode_t;
#endif

/**
 * @brief LDT operating function. Can be used with function @ref FS45_LDT_SetTimerOperation().
 *
 * Implements : fs45_ldt_function_t_Enumeration
 */
typedef enum
{
    /** @brief In normal mode count and generate flag or INT when counter reaches the after run value. */
    fs45LdtFunc1 = FS45_RW_M_F2_F0_FUNCTION1,
    /** @brief In normal mode count until after run value is reached, then enters in LPOFF. */
    fs45LdtFunc2 = FS45_RW_M_F2_F0_FUNCTION2,
    /** @brief In normal mode count until after run value is reached, then enters in LPOFF. Once in LPOFF, count until
     *         wake-up value is reached and wake-up. */
    fs45LdtFunc3 = FS45_RW_M_F2_F0_FUNCTION3,
    /** @brief In LPOFF, count until wake-up value is reached and wake-up. */
    fs45LdtFunc4 = FS45_RW_M_F2_F0_FUNCTION4,
    /** @brief In LPOFF, count and do not wake-up. Counter value is stored in wake-up register. */
    fs45LdtFunc5 = FS45_RW_M_F2_F0_FUNCTION5
} fs45_ldt_function_t;

/**
 * @brief LDT mode. Can be used with function @ref FS45_LDT_SetTimerMode().
 *
 * Implements : fs45_ldt_mode_t_Enumeration
 */
typedef enum
{
    fs45LdtModeCalibration = FS45_RW_M_MODE_CALIBRATION, /**< @brief Calibration mode (488 us resolution). */
    fs45LdtModeNormal = FS45_RW_M_MODE_NORMAL            /**< @brief Normal mode (1 s resolution). */
} fs45_ldt_mode_t;

/**
 * @brief Wake-up register source. Can be used with function @ref FS45_LDT_SetWakeUpRegSrc().
 *
 * Implements : fs45_ldt_wu_scr_t_Enumeration
 */
typedef enum
{
    /** @brief Read programmed wake-up register. */
    fs45LdtWakeupProg = FS45_RW_M_REG_SE_PROGRAMMED_REG,
    /** @brief Read real time counter into wake-up register (after counter is stopped with LDT_ENABLE bit). */
    fs45LdtWakeupRTC = FS45_RW_M_REG_SE_RTC_REG
} fs45_ldt_wu_scr_t;

/**
 * @brief Watchdog window duration. Duty cycle is set to 50 %.
 *        Can be used with function @ref FS45_WD_ChangeWindow().
 *
 * Implements : fs45_wd_window_t_Enumeration
 */
typedef enum
{
    fs45WdWindowDisable = FS45_W_FS_WD_WINDOW_DISABLE, /**< @brief Disable (in INIT phase only). */
    fs45WdWindow1ms = FS45_W_FS_WD_WINDOW_1MS,         /**< @brief 1.0 ms. */
    fs45WdWindow2ms = FS45_W_FS_WD_WINDOW_2MS,         /**< @brief 2.0 ms. */
    fs45WdWindow3ms = FS45_W_FS_WD_WINDOW_3MS,         /**< @brief 3.0 ms. */
    fs45WdWindow4ms = FS45_W_FS_WD_WINDOW_4MS,         /**< @brief 4.0 ms. */
    fs45WdWindow6ms = FS45_W_FS_WD_WINDOW_6MS,         /**< @brief 6.0 ms. */
    fs45WdWindow8ms = FS45_W_FS_WD_WINDOW_8MS,         /**< @brief 8.0 ms. */
    fs45WdWindow12ms = FS45_W_FS_WD_WINDOW_12MS,       /**< @brief 12 ms. */
    fs45WdWindow16ms = FS45_W_FS_WD_WINDOW_16MS,       /**< @brief 16 ms. */
    fs45WdWindow24ms = FS45_W_FS_WD_WINDOW_24MS,       /**< @brief 24 ms. */
    fs45WdWindow32ms = FS45_W_FS_WD_WINDOW_32MS,       /**< @brief 32 ms. */
    fs45WdWindow64ms = FS45_W_FS_WD_WINDOW_64MS,       /**< @brief 64 ms. */
    fs45WdWindow128ms = FS45_W_FS_WD_WINDOW_128MS,     /**< @brief 128 ms. */
    fs45WdWindow256ms = FS45_W_FS_WD_WINDOW_256MS,     /**< @brief 256 ms. */
    fs45WdWindow512ms = FS45_W_FS_WD_WINDOW_512MS,     /**< @brief 512 ms. */
    fs45WdWindow1024ms = FS45_W_FS_WD_WINDOW_1024MS    /**< @brief 1024 ms. */
} fs45_wd_window_t;

/**
 * @brief Fail-safe output selection for its low level request.
 *        Can be used with function @ref FS45_RequestFSxLow().
 *
 * Implements : fs45_fsx_req_type_t_Enumeration
 */
typedef enum
{
    /** @brief Request FS0B assertion. */
    fs45ReqFS0B = FS45_W_FS_FS0B_REQ_FS0B_REQ,
    /** @brief Request FS1B assertion with tDELAY controlled by the backup delay (open S1). */
    fs45ReqFS1BDelay = FS45_W_FS_FS1B_DLY_REQ_FS1B_REQ,
    /** @brief Request FS1B assertion with immediate assertion, no delay. */
    fs45ReqFS1B = FS45_W_FS_FS1B_REQ_FS1B_REQ
} fs45_fsx_req_type_t;

/**
 * @brief Structure representing received data frame.
 *
 * Implements : fs45_rx_data_t_Class
 */
typedef struct
{
    /** @brief A device status is returned into this byte after a successful transfer. */
    uint8_t deviceStatus;
    /** @brief Extended diagnostics. Sent by Main or Fail-safe if secured register is accessed.
     *
     * Diagnostics content is saved in 4 highest bits. */
    uint8_t deviceStatusEx;
    /** @brief Content of a read register. */
    uint8_t readData;
} fs45_rx_data_t;

/** @brief Structure representing configuration value of one register. */
typedef struct
{
    /**< @brief Register address */
    uint8_t address;
    /**< @brief Value of the register. */
    uint8_t value;
    /**< @brief Mask used for register read value check. */
    uint8_t readMask;
} fs45_reg_config_value_t;

/** @brief Initialization structure with INIT_VREG content. */
typedef struct
{
    /** @brief Current limitation threshold for VCCA. */
    uint8_t iccaLim;
    /** @brief Current limitation duration before VCCA is switched off. */
    uint8_t tccaLimOff;
    /** @brief Disable the input power feed forward function of VPRE. */
    uint8_t ipffDis;
    /** @brief CAN_5V overvoltage monitoring. */
    uint8_t vcanOvMon;
    /** @brief Current limitation duration before VAUX is switched off. */
    uint8_t tauxLimOff;
    /** @brief Enable VAUX regulator as a tracker of VCCA. */
    uint8_t vauxTrkEn;
} fs45_vreg_reg_t;

/** @brief Initialization structure with INIT_WU1 content. */
typedef struct
{
    /** @brief IO_0 wake-up configuration. */
    uint8_t wuIo0;
    /** @brief IO_2 wake-up configuration. */
    uint8_t wuIo2;
    /** @brief IO_3 wake-up configuration. */
    uint8_t wuIo3;
    /** @brief IO_4 wake-up configuration. */
    uint8_t wuIo4;
} fs45_wu1_reg_t;

/** @brief Initialization structure with INIT_WU2 content. */
typedef struct
{
    /** @brief IO_5 wake-up configuration. */
    uint8_t wuIo5;
    /** @brief CAN behavior when FS1B is asserted low. */
    uint8_t canDisCfg;
    /** @brief CAN wake-up timeout. */
    uint8_t canWuTo;
    /** @brief Comply with J2602 standard. Recessive mode when VSUP < 7.0 V. */
    uint8_t linJ2602Dis;
    /** @brief LIN slew rate. */
    uint8_t linSr;
} fs45_wu2_reg_t;

/** @brief Initialization structure with INIT_INT content. */
typedef struct
{
    /** @brief Duration of the interrupt pulse. */
    uint8_t duration;
    /** @brief Inhibit the interrupt for LIN error bits. */
    uint8_t inhLin;
    /** @brief Inhibit ALL the interrupt. */
    uint8_t inhAll;
    /** @brief Inhibit the interrupt for VSNS_UV. */
    uint8_t inhVsns;
    /** @brief Inhibit the interrupt for VPRE status event. */
    uint8_t inhVpre;
    /** @brief Inhibit the interrupt for VCORE status event. */
    uint8_t inhVcore;
    /** @brief Inhibit the interrupt for VCCA/VAUX and VCAN status event. */
    uint8_t inhVother;
    /** @brief Inhibit the interrupt for CAN error bits. */
    uint8_t inhCan;
} fs45_int_reg_t;

/** @brief Initialization structure with INIT_INH_INT content. */
typedef struct
{
    /** @brief Inhibit the interrupt pulse for IO_5 (masked in IO_G). */
    uint8_t inh5;
    /** @brief Inhibit the interrupt pulse for IO_4 (masked in IO_G). */
    uint8_t inh4;
    /** @brief Inhibit the interrupt pulse for IO_3 (masked in IO_G). */
    uint8_t inh3;
    /** @brief Inhibit the interrupt pulse for IO_2 (masked in IO_G). */
    uint8_t inh2;
    /** @brief Inhibit the interrupt pulse for IO_0 (masked in IO_G). */
    uint8_t inh0;
} fs45_inh_int_reg_t;

/** @brief Initialization structure with INIT_FS1B_TIMING content. */
typedef struct
{
    /** @brief Delay between FS0B and FS1B activation. */
    uint8_t fs1bTime;
} fs45_fs1b_timing_reg_t;

/** @brief Initialization structure with INIT_SUPERVISOR content. */
typedef struct
{
    /** @brief VCORE undervoltage in degraded mode. Only valid for 5.0 V. */
    uint8_t vcore5d;
    /** @brief VCCA undervoltage in degraded mode. Only valid for 5.0 V. */
    uint8_t vcca5d;
    /** @brief VAUX undervoltage in degraded mode. Only valid for 5.0 V. */
    uint8_t vaux5d;
    /** @brief FS1B timing range factor x1 or x8. */
    uint8_t fs1bTimeRange;
} fs45_supervisor_reg_t;

/** @brief Initialization structure with INIT_FAULT content. */
typedef struct
{
    /** @brief Values of the fault error counter. */
    uint8_t fltErrFs;
    /** @brief CAN behavior when FS1B is asserted low. */
    uint8_t fs1bCanImpact;
    /** @brief RSTB and FS0B behavior when fault error counter >= intermediate value. */
    uint8_t fltErrImp;
} fs45_fault_reg_t;

/** @brief Initialization structure with INIT_FSSM content. */
typedef struct
{
    /** @brief IO_4:5 as safety inputs for external IC error monitoring. */
    uint8_t io45Fs;
    /** @brief IO_3:2 as safety inputs for FCCU monitoring. */
    uint8_t io23Fs;
    /** @brief FCCU polarity. */
    uint8_t ps;
    /** @brief RSTB low duration time. */
    uint8_t rstbDuration;
} fs45_fssm_reg_t;

/** @brief Initialization structure with INIT_SF_IMPACT content. */
typedef struct
{
    /** @brief FS1B delay or FS1B duration mode selection. */
    uint8_t tdlyTdur;
    /** @brief Disable the 8.0 s timer used to enter deep fail-safe mode. */
    uint8_t dis8s;
    /** @brief Watchdog impact on RSTB and/or FS0B assertion. */
    uint8_t wdImpact;
} fs45_sf_impact_reg_t;

/** @brief Initialization structure with INIT_WD_CNT content. */
typedef struct
{
    /** @brief Maximum value of the WD error counter. */
    uint8_t wdCntErr;
    /** @brief Maximum value of the WD refresh counter. */
    uint8_t wdCntRfr;
} fs45_wd_cnt_reg_t;

/** @brief Initialization structure with INIT_VCORE_OVUV_IMPACT content. */
typedef struct
{
    /** @brief VCORE_FB overvoltage safety impact. */
    uint8_t vcoreFsOv;
    /** @brief VCORE_FB undervoltage safety impact. */
    uint8_t vcoreFsUv;
} fs45_vcore_ovuv_impact_reg_t;

/** @brief Initialization structure with INIT_VCCA_OVUV_IMPACT content. */
typedef struct
{
    /** @brief VCCA overvoltage safety impact. */
    uint8_t vccaFsOv;
    /** @brief VCCA undervoltage safety impact. */
    uint8_t vccaFsUv;
} fs45_vcca_ovuv_impact_reg_t;

/** @brief Initialization structure with INIT_VAUX_OVUV_IMPACT content. */
typedef struct
{
    /** @brief VAUX overvoltage safety impact. */
    uint8_t vauxFsOv;
    /** @brief VAUX undervoltage safety impact. */
    uint8_t vauxFsUv;
} fs45_vaux_ovuv_impact_reg_t;

/**
 * @brief Structure for FS45 driver configuration.
 *
 * This structure is used as a parameter for @ref FS45_Init() function.
 *
 * Implements : fs45_drv_config_t_Class
 */
typedef struct
{
    /**
     * @brief Main logic registers.
     *
     * Configuration values for main logic registers written during INIT phase.
     * Following registers can be configured:
     *  - INIT_VREG
     *  - INIT_WU1
     *  - INIT_WU2
     *  - INIT_INH_INT
     *
     * @note INIT_INT register is separate at the end of this structure.
     */
    fs45_vreg_reg_t vregReg;
    fs45_wu1_reg_t wu1Reg;
    fs45_wu2_reg_t wu2Reg;
    fs45_inh_int_reg_t inhIntReg;

    /**
     * @brief Fail-safe logic registers.
     *
     * Configuration values for fail-safe logic registers written during INIT phase.
     * Following registers can be configured:
     *  - INIT_FS1B_TIMING
     *  - INIT_SUPERVISOR
     *  - INIT_FAULT
     *  - INIT_FSSM
     *  - INIT_SF_IMPACT
     *  - INIT_WD_CNT
     *  - INIT_VCORE_OVUV_IMPACT
     *  - INIT_VCCA_OVUV_IMPACT
     *  - INIT_VAUX_OVUV_IMPACT
     *
     * These registers use secured SPI command, please use unshifted values (the same
     * as when the register is read). The shift and addition of secure bits is done
     * automatically by the Init function.
     */
    fs45_fs1b_timing_reg_t fs1bTimingReg;
    fs45_supervisor_reg_t supervisorReg;
    fs45_fault_reg_t faultReg;
    fs45_fssm_reg_t fssmReg;
    fs45_sf_impact_reg_t sfImpactReg;
    fs45_wd_cnt_reg_t wdCntReg;
    fs45_vcore_ovuv_impact_reg_t vcoreOvuvImpactReg;
    fs45_vcca_ovuv_impact_reg_t vccaOvuvImpactReg;
    fs45_vaux_ovuv_impact_reg_t vauxOvuvImpactReg;

    /**
     * @brief INIT_INT register.
     *
     * Writing into this register causes transition from INIT MAIN to NORMAL MODE.
     */
    fs45_int_reg_t intReg;

    /**
     * @brief Non-init registers (both main and fails-safe logic).
     *
     * Array of register configurations of the rest of the registers
     * (or just selection of them - based on address and nonInitRegsCount).
     * If nonInitRegs is NULL, or nonInitRegsCount is 0, it will be skipped.
     *
     * Some of these registers use secured SPI command, please use unshifted values
     * (the same as when the register is read). The shift and addition of secure bits
     * is done automatically by the Init function.
     */
    fs45_reg_config_value_t* nonInitRegs;
    uint8_t nonInitRegsCount;

} fs45_drv_config_t;

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief This function runs full initialization of the SBC device.
 *
 * This function writes configuration values to selected SBC registers
 * (i.e. content of the drvConfig structure),
 * checks results of built-in self-test diagnostics and optionally
 * runs additional procedures to ensure correct functionality of
 * safety critical features.
 * The first watchdog refresh is part of the initialization procedure,
 * the SBC should be in NORMAL mode upon the function exit.
 *
 * @param drvConfig         Configuration structure of the SW driver.
 * @param lpspiInstance     SPI peripheral instance.
 * @param lpspiTimeoutMs    SPI timeout in ms.
 *
 * @return @ref status_t Status return code.
 *
 * @remark Note that main register set is initialized only after power on reset.
 *         It retains its values after transition to low-power mode and
 *         back to normal.
 *         It is recommended to read-out wake-up sources and diagnostic statuses
 *         after initialization to clear all flags.
 */
status_t FS45_Init(const fs45_drv_config_t* drvConfig, uint32_t lpspiInstance, uint32_t lpspiTimeoutMs);

/**
 * @brief This function fills the driver configuration structure with default
 * values of INIT phase registers.
 *
 * The init registers configurations are filled with default datasheet values.
 *
 * @remark Default value of FS1B_CAN_IMPACT bit (INIT_FAULT register) is
 *         different than the datasheet value for devices with FS1B.
 *
 * @param drvConfig         Configuration structure of the SW driver.
 */
void FS45_GetDefaultInitRegsConfig(fs45_drv_config_t* drvConfig);

/**
 * @brief This function fills the driver configuration structure with default
 * values of non-init registers.
 *
 * The configurations of all writable non-init registers are filled
 * with default datasheet values.
 *
 * @param drvConfig         Configuration structure of the SW driver.
 * @param nonInitRegs       Pointer to store default values for all registers
 *                          which can be configured after init. Application should
 *                          guarantee space for 5 registers.
 */
void FS45_GetDefaultNonInitRegsConfig(fs45_drv_config_t* drvConfig, fs45_reg_config_value_t* nonInitRegs);

/**
 * @brief Performs a read from a single FS45 register.
 *
 * Performs a single read register based on provided address.
 * The response is returned in @ref fs45_rx_data_t structure.
 *
 * @param address           Register address.
 * @param rxData            Structure holding the response from SBC.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_ReadRegister(uint8_t address, fs45_rx_data_t* rxData);

/**
 * @brief Sends write command to the FS45.
 *
 * @param address           Register address.
 * @param writeData         Register write value.
 * @param rxData            Diagnostic data returned by the SBC.
 *
 * @return @ref status_t Status return code.
 *
 * @remark The rxData pointer is not mandatory, if it's NULL, writeData are
 *         written to the register and received data is ignored.
 */
status_t FS45_WriteRegister(uint8_t address, uint8_t writeData,
        fs45_rx_data_t* rxData);

/**
 * @brief Changes seed of LFSR used for watchdog.
 *
 * The watchdog seed can be changed just during the INIT_FS phase (for challenger WD)
 * or during the OPEN watchdog window (for simple WD). Timing is up to the application!
 *
 * @param wdSeed            Watchdog LFSR seed.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_WD_ChangeSeed(uint8_t wdSeed);

/**
 * @brief Switches a desired channel to the AMUX pin.
 *
 * @param channelSelection  Selected channel to be delivered to AMUX pin.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_SwitchAMUXchannel(fs45_amux_selection_t channelSelection);

/**
 * @brief Sets state (enable/disable) of the selected voltage regulator.
 *
 * @param vreg              Voltage regulator enum (VCAN, VAUX, VCCA, VCORE).
 * @param enable            State (enable = true / disable = false).
 *
 * @return @ref status_t Status return code.
 *
 * @remark Disabling the regulators might result in loss of some functionality,
 *         based on the regulator type and your board design (for example loss
 *         of SPI communication, SPI MISO is powered by VDDIO internal regulator,
 *         which is usually supplied from VCORE or VCCA - depends on board connections).
 */
status_t FS45_SetRegulatorState(fs45_reg_mode_t vreg, bool enable);

/**
 * @brief Reads actual Fault Error Counter value.
 *
 * @param faultErrorCounterValue Fault Error counter value storage.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_GetFaultErrorCounterValue(uint8_t* faultErrorCounterValue);

/**
 * @brief This function gets current and previous mode of the SBC.
 *
 * @param currentMode       Current mode of the SBC device.
 * @param prevMode          Previous mode of the SBC device.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_GetMode(fs45_current_mode_t *currentMode, fs45_prev_mode_t *prevMode);

/**
 * @brief This function checks if VAUX is safety critical and optionally
 *        runs related diagnostics.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_CheckVAUX(void);

/**
 * @brief This function checks if FS1B has expected low level during
 *        initialization and runs related diagnostics.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_CheckFS1B(void);

/**
 * @brief This function releases selected fail-safe output as a part of
 *        error recovery procedure.
 *
 * @param fsOutput          Selection of fail-safe output.
 *
 * @return @ref status_t Status return code.
 *
 * @remark When a fault is removed and the fault error counter changes back to
 *         level '0', a right word must be filled in the RELEASE_FSxB register.
 *         The value depends on the current WD_LFSR. LSB, MSB must be swapped,
 *         and a negative operation per bit must be applied.
 *         The RELEASE_FSxB write command must be done after the WD_LFSR read
 *         command within the same WD period.
 */
status_t FS45_ReleaseFSx(fs45_fsxb_release_t fsOutput);

/**
 * @brief Checks if LBIST and ABIST1 diagnostics passed.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_CheckLbistAbistOk(void);

/**
 * @brief This function switches mode of the SBC to the LPOFF, optionally with
 *        automatic wake-up 1ms after transition.
 *
 * @param autoWU            Automatic wake-up 1 ms after transition.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_SetLowPowerMode(bool autoWU);

/**
 * @brief This function requests an interrupt (pulse on the INT pin).
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_RequestInterrupt(void);

/**
 * @brief This function changes CAN mode and the automatic transition of the CAN
 *        transceiver to the low-power mode on specific events.
 *
 * @param mode              CAN mode.
 * @param autoDis           Automatic transition to the LPOFF on
 *                          (CAN OT/TXD dominant/RXD recessive).
 *
 * @return @ref status_t Status return code.
 *
 * @remark The function returns STATUS_SUCCESS when used on device without CAN,
 *         but the register access does not affect the device behavior.
 */
status_t FS45_CAN_SetMode(fs45_can_mode_t mode, bool autoDis);

#ifdef FS45_FEATURE_LIN
/**
 * @brief This function changes LIN mode and the automatic transition of the LIN
 *        transceiver to the low-power mode on specific events.
 *
 * @param mode              LIN mode.
 * @param autoDis           Automatic transition to the LPOFF on
 *                          (LIN OT/TXDL dominant/RXDL recessive).
 *
 * @return @ref status_t Status return code.
 *
 * @remark The function returns STATUS_SUCCESS when used on device without LIN,
 *         but the register access does not affect the device behavior.
 */
status_t FS45_LIN_SetMode(fs45_lin_mode_t mode, bool autoDis);
#endif

/**
 * @brief This function sets operating function of the LDT.
 *
 * @param op                Operating function of the LDT.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_LDT_SetTimerOperation(fs45_ldt_function_t op);

/**
 * @brief This function sets mode of the LDT (normal/calibration).
 *
 * @param mode              Mode of the LDT.
 *
 * @return @ref status_t Status return code.
 *
 * @remark Resolution is 1s for normal mode and 488us for calibration mode.
 */
status_t FS45_LDT_SetTimerMode(fs45_ldt_mode_t mode);

/**
 * @brief This function sets counter to read real-time counter or programmed
 *        value into wake-up register.
 *
 * @param source            Source for the wake-up register.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_LDT_SetWakeUpRegSrc(fs45_ldt_wu_scr_t source);

/**
 * @brief This function starts or stops the LDT counter.
 *
 * @param run               Use true for start or false for stop the LDT counter.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_LDT_RunCounter(bool run);

/**
 * @brief This function sets new after-run value for the LDT.
 *
 * @param value             After-run value.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_LDT_SetAfterRunValue(uint16_t value);

/**
 * @brief This function sets new wake-up value for the LDT.
 *
 * @param value             Wake-up value (1 - 16777215).
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_LDT_SetWakeUpValue(uint32_t value);

/**
 * @brief This function changes duration of watchdog window.
 *
 * @param windowDuration    Watchdog window duration.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_WD_ChangeWindow(fs45_wd_window_t windowDuration);

/**
 * @brief This function refreshes watchdog of the SBC device.
 *
 * @return @ref status_t Status return code.
 *
 * @remark MCU internally computes monitoring result based on a received
 *         challenge token (LFSR state) and sends it back as an answer.
 */
status_t FS45_WD_Refresh(void);

/**
 * @brief This function requests a low pulse on the RSTB (MCU reset).
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_RequestReset(void);

/**
 * @brief This function requests a low level on the selected fail-safe output.
 *
 * @param fsxSelection      Selection of fail-safe output.
 *
 * @return @ref status_t Status return code.
 */
status_t FS45_RequestFSxLow(fs45_fsx_req_type_t fsxSelection);

/**
 * @brief This function sets level of the IO_4 when configured as an output.
 *
 * @param level             Level of IO_4 when configured as output. True for high
 *                          level, false for low level.
 *
 * @return @ref status_t Status return code.
 *
 * @remark Note that this function automatically enables IO_4 to be used as output.
 */
status_t FS45_SetOUT4(bool level);

/*! @}*/
#endif /* SBC_FS45_H_ */
