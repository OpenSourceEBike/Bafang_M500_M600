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

/*!
 * @file sbc_uja116x_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type.
 * This is required by the conversion of a bit-field (uint8_t) of a register
 * into a enum type. Enum type is strictly defined as unsigned integer.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially unsigned' type to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field (uint8_t) of a register
 * into a enum type. Enum type is strictly defined as unsigned integer.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite
 * expression (different essential type categories).
 * This is required by the conversion of a bit-field (uint8_t) of a register
 * into a enum type. Enum type is strictly defined as unsigned integer.
 */


#include "sbc_uja116x_driver.h"
#include "clock_manager.h"
#include "osif.h"

/*******************************************************************************
 * Variables - for internal use only.
 ******************************************************************************/
typedef struct{
    sbc_wtdog_ctr_t watchdogCtr;
    uint32_t lpspiIntace;
    bool isInit;
}drv_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Structure for storing SBC internal runtime data. */
static drv_config_t g_drvConfig;

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

static status_t sbc_write_can_others(const sbc_can_conf_t* const can);
static status_t sbc_read_can_others(sbc_can_conf_t* const can);
static void sbc_clean_events_status(sbc_evn_capt_t *event);
static status_t sbc_change_factories_direct(const sbc_factories_conf_t* const factory);
static uint8_t sbc_get_factories_crc(uint8_t* data);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_InitDriver
 * Description   : This function internally stores LPSPI instance used by the driver.
 *
 * Implements    : SBC_InitDriver_Activity
 *END**************************************************************************/
void SBC_InitDriver(const uint32_t lpspiInstance)
{
    /* Set initial value */
    g_drvConfig.lpspiIntace = lpspiInstance;
    /* Device is being initialized. */
    g_drvConfig.isInit = true;
    /* Waiting while device leaves reset mode - no SPI access allowed. */
    OSIF_TimeDelay(1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_InitDevice
 * Description   : This function initializes all registers. Please use InitDriver first.
 *
 * Implements    : SBC_InitDevice_Activity
 *END**************************************************************************/
status_t SBC_InitDevice(const sbc_int_config_t *const config)
{
    status_t status = STATUS_SUCCESS;

    DEV_ASSERT(config != NULL);

    /* Store watchdog settings. */
    g_drvConfig.watchdogCtr.modeControl = config->watchdog.modeControl;
    g_drvConfig.watchdogCtr.nominalPeriod = config->watchdog.nominalPeriod;

    /* SBC initialization. */
    /* Take device in stand by mode during initialization. */
    status = SBC_SetMode(SBC_UJA_MODE_MC_STANDBY);

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetVreg(&config->regulatorCtr);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetWatchdog(&config->watchdog);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetFailSafe(config->lhc, NULL);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetSystemEvents(&config->sysEvnt);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetLock(config->lockMask);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetCanConfig(&config->can);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetWakePin(&config->wakePin);
    }

    if(status == STATUS_SUCCESS)
    {
        status = SBC_SetMode(config->mode);
    }
    /* Device was initialized successfully. */
    g_drvConfig.isInit = false;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetVreg
 * Description   : This function configures Regulator control registers:
 * Regulator control (0x10), Supply event enable(0x1C).
 * The following is an example of how to set up regulator registers by
 * passing regulator parameter to SBC_SetVreg function:
 *  sbc_regulator_ctr_t regulator;
 *  Regulator Register.
 *  regulator.regulator.pdc = SBC_UJA_REGULATOR_PDC_LV;
 *  regulator.regulator.v2c = SBC_UJA_REGULATOR_V2C_N_S_R;
 *  regulator.regulator.v1rtc = SBC_UJA_REGULATOR_V1RTC_80;
 *  Supply register.
 *  regulator.supplyEvnt.v1ue = SBC_UJA_SUPPLY_EVNT_V1UE_EN;
 *  regulator.supplyEvnt.v2oe = SBC_UJA_SUPPLY_EVNT_V2OE_DIS;
 *  regulator.supplyEvnt.v2ue = SBC_UJA_SUPPLY_EVNT_V2UE_EN;
 *  status_t status = SBC_SetVreg(&regulator);
 *
 * Implements    : SBC_SetVreg_Activity
 *END**************************************************************************/
status_t SBC_SetVreg(const sbc_regulator_ctr_t* const regulatorCtr)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(regulatorCtr != NULL);

    const sbc_regulator_t *regulator = &regulatorCtr->regulator;
    uint8_t regData = (uint8_t)((uint8_t)regulator->pdc | (uint8_t)regulator->v2c
            | (uint8_t)regulator->v1rtc);

    /* Send configuration to Regulator control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_REGULATOR, &regData, NULL);
    regData = 0U;

    if(spiStat == STATUS_SUCCESS )
    {
        const sbc_supply_evnt_t *sEvnt = &regulatorCtr->supplyEvnt;
        /* Send  configuration to Supply Event register. */
        regData = (uint8_t)((uint8_t)sEvnt->v2oe | (uint8_t)sEvnt->v2ue
                | (uint8_t)sEvnt->v1ue);
        spiStat = SBC_DataTransfer(SBC_UJA_SUPPLY_EVNT, &regData, NULL);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetVreg
 * Description   : This function reads Regulator control registers:
 * Regulator control (0x10), Supply event enable(0x1C).
 * The following is an example of how to read content of Regulator registers by
 * passing regulator parameter to SBC_GetVreg function:
 *  sbc_regulator_ctr_t regulator;
 *  status_t status = SBC_GetVreg(&regulator);
 *
 * Implements    : SBC_GetVreg_Activity
 *END**************************************************************************/
status_t SBC_GetVreg(sbc_regulator_ctr_t* const regulatorCtr)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(regulatorCtr != NULL);

    sbc_regulator_t *regulator = &regulatorCtr->regulator;
    uint8_t regData = 0;

    /* Read configuration from Regulator control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_REGULATOR, NULL, &regData);

    if(spiStat == STATUS_SUCCESS )
    {
        /* Set regulator structure. */
        regulator->pdc = (sbc_regulator_pdc_t)(regData & SBC_UJA_REGULATOR_PDC_MASK);
        regulator->v2c = (sbc_regulator_v2c_t)(regData & SBC_UJA_REGULATOR_V2C_MASK);
        regulator->v1rtc = (sbc_regulator_v1rtc_t)(regData & SBC_UJA_REGULATOR_V1RTC_MASK);

        sbc_supply_evnt_t *sEvnt = &regulatorCtr->supplyEvnt;
        /* Read  configuration from Supply Event register. */
        regData = 0;
        spiStat = SBC_DataTransfer(SBC_UJA_SUPPLY_EVNT, NULL, &regData);

        /* Set Supply Event structure. */
        sEvnt->v2oe = (sbc_supply_evnt_v2oe_t)(regData & SBC_UJA_SUPPLY_EVNT_V2OE_MASK);
        sEvnt->v2ue = (sbc_supply_evnt_v2ue_t)(regData & SBC_UJA_SUPPLY_EVNT_V2UE_MASK);
        sEvnt->v1ue = (sbc_supply_evnt_v1ue_t)(regData & SBC_UJA_SUPPLY_EVNT_V1UE_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetWatchdog
 * Description   : This function writes to Watchdog control register (0x00).
 * This function can select Watchdog mode control, and nominal watchdog period.
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
 * The following is an example of how to set up Watchdog register by
 * passing wtdog parameter to SBC_SetWatchdog function:
 *  wtdog.modeControl = SBC_UJA_WTDOG_CTR_WMC_AUTO;
 *  wtdog.nominalPeriod = SBC_UJA_WTDOG_CTR_NWP_4096;
 *  status_t status = SBC_SetWatchdog(&wtdog);
 *
 * Implements    : SBC_SetWatchdog_Activity
 *END**************************************************************************/
status_t SBC_SetWatchdog(const sbc_wtdog_ctr_t* const wtdog)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(wtdog != NULL);

    bool newWdMode = false;
    /* Variable for storing current Mode settings. */
    sbc_mode_mc_t currentMode = SBC_UJA_MODE_MC_SLEEP;

    /* Test if new Watchdog value is different. */
    if((g_drvConfig.watchdogCtr.modeControl != wtdog->modeControl) ||
            (g_drvConfig.watchdogCtr.nominalPeriod != wtdog->nominalPeriod)
            || g_drvConfig.isInit)
    {
        newWdMode = true;
        /* Read current mode. */
        status = SBC_GetMode(&currentMode);
    }

    /* Set device to stand by mode. */
    if(newWdMode && (status == STATUS_SUCCESS) &&
            (currentMode != SBC_UJA_MODE_MC_STANDBY))
    {
        status = SBC_SetMode(SBC_UJA_MODE_MC_STANDBY);
    }

    if(status == STATUS_SUCCESS)
    {
        uint8_t regData = (uint8_t)((uint8_t)wtdog->modeControl
                | (uint8_t)wtdog->nominalPeriod);

        /* Send configuration to Watchdog control register. */
        spiStat = SBC_DataTransfer(SBC_UJA_WTDOG_CTR, &regData, NULL);
    }

    if(newWdMode && (spiStat == STATUS_SUCCESS) &&
            (status == STATUS_SUCCESS) &&
            (currentMode != SBC_UJA_MODE_MC_STANDBY))
    {
        /* Return to previous mode. */
        status = SBC_SetMode(currentMode);

        g_drvConfig.watchdogCtr.modeControl = wtdog->modeControl;
        g_drvConfig.watchdogCtr.nominalPeriod = wtdog->nominalPeriod;
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetWatchdog
 * Description   : This function reads Watchdog control register (0x00).
 * This function reads Watchdog mode control, and nominal watchdog period.
 * The UJA116xA contains a watchdog that supports three operating modes: Window,
 * Timeout and Autonomous. In Window mode (available only in SBC Normal mode), a
 * watchdog trigger event within a defined watchdog window triggers and resets
 * the watchdog timer. In Timeout mode, the watchdog runs continuously and can
 * be triggered and reset at any time within the watchdog period by a watchdog
 * trigger. Watchdog time-out mode can also be used for cyclic wake-up
 * of the microcontroller. In Autonomous mode, the watchdog can be off or
 * autonomously in Timeout mode, depending on the selected SBC mode.
 * The following is an example of how to read from Watchdog register by
 * passing wtdog parameter to SBC_GetWatchdog function:
 * sbc_wtdog_ctr_t wtdog;
 * status_t status = SBC_GetWatchdog(&wtdog);
 *
 * Implements    : SBC_GetWatchdog_Activity
 *END**************************************************************************/
status_t SBC_GetWatchdog(sbc_wtdog_ctr_t* const wtdog)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(wtdog != NULL);

    uint8_t regData = 0;

    /* Read configuration from Watchdog control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_WTDOG_CTR, NULL, &regData);

    if(spiStat == STATUS_SUCCESS)
    {
        wtdog->modeControl = (sbc_wtdog_ctr_wmc_t)(regData & SBC_UJA_WTDOG_CTR_WMC_MASK);
        wtdog->nominalPeriod = (sbc_wtdog_ctr_nwp_t)(regData & SBC_UJA_WTDOG_CTR_NWP_MASK);

        g_drvConfig.watchdogCtr.modeControl = wtdog->modeControl;
        g_drvConfig.watchdogCtr.nominalPeriod = wtdog->nominalPeriod;
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_FeedWatchdog
 * Description   : This function refresh watchdog period by writing byte to
 * the SBC watchdog register. This function must be called periodically
 * according Watchdog mode control and Nominal watchdog period settings.
 * Note: Unxpected behaviour can happend if watchdog mode is set to timeout
 * period and watchdog is triggered exactly at 50% of period.
 * Be sure you trigger watchdog before 50% or above 50% of watchdog period.
 * The following is an example of how to use sbc_refresh_watchdog function:
 *  SBC_SetWatchdog();
 *
 * Implements    : SBC_FeedWatchdog_Activity
 *END**************************************************************************/
void SBC_FeedWatchdog(void)
{
    (void)SBC_SetWatchdog(&g_drvConfig.watchdogCtr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetMode
 * Description   : This function write to Mode control register (0x01).
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
 * The following is an example of how to set up Mode register by
 * passing mode parameter to SBC_SetMode function:
 *  sbc_mode_mc_t mode;
 *  mode = SBC_UJA_MODE_MC_STANDBY;
 *  status_t status = SBC_SetMode(&mode);
 *
 * Implements    : SBC_SetMode_Activity
 *END**************************************************************************/
status_t SBC_SetMode(const sbc_mode_mc_t mode)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    uint8_t regData = (uint8_t)mode;

    if((mode == SBC_UJA_MODE_MC_SLEEP) || (mode == SBC_UJA_MODE_MC_STANDBY)
            || (mode == SBC_UJA_MODE_MC_NORMAL))
    {
        if((mode == SBC_UJA_MODE_MC_SLEEP) || (mode == SBC_UJA_MODE_MC_STANDBY))
        {
            /* Clear all events and move to sleep mode. */
            sbc_evn_capt_t events;
            status = SBC_GetEventsStatus(&events);

            if(status == STATUS_SUCCESS)
            {
                status = SBC_CleanEvents(&events);
            }
        }
    }
    else
    {
        status = SBC_CMD_ERROR;
    }

    if(status == STATUS_SUCCESS)
    {
        /* Write mode. */
        spiStat = SBC_DataTransfer(SBC_UJA_MODE, &regData, NULL);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetMode
 * Description   : This function reads the Mode control register (0x01).
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
 * The following is an example of how to read from Mode register by
 * passing mode parameter to SBC_GetMode function:
 *  sbc_mode_mc_t mode;
 *  status_t status = SBC_GetMode(&mode);
 *
 * Implements    : SBC_GetMode_Activity
 *END**************************************************************************/
status_t SBC_GetMode(sbc_mode_mc_t* const mode)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;
    uint8_t regData = 0;

    DEV_ASSERT(mode != NULL);

    /* Read mode. */
    spiStat = SBC_DataTransfer(SBC_UJA_MODE, NULL, &regData);

    if(spiStat == STATUS_SUCCESS)
    {
        *mode = (sbc_mode_mc_t)(regData & SBC_UJA_MODE_MC_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetFailSafe
 * Description   : This function writes to Fail-safe control register (0x02).
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
 * The following is an example of how to write to limp home control by
 * passing lhc parameter to SBC_SetFailSafe function:
 *  sbc_fail_safe_lhc_t lhc;
 *  lhc = SBC_UJA_FAIL_SAFE_LHC_FLOAT;
 *  status_t status = SBC_SetFailSafe(&lhc, NULL);
 *
 * Implements    : SBC_SetFailSafe_Activity
 *END**************************************************************************/
status_t SBC_SetFailSafe(const sbc_fail_safe_lhc_t lhc,
        const sbc_fail_safe_rcc_t* const rcc)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    if((lhc == SBC_UJA_FAIL_SAFE_LHC_FLOAT) || (lhc == SBC_UJA_FAIL_SAFE_LHC_LOW ))
    {
        uint8_t regData = (uint8_t)lhc;

        /* Test if rcc should be changed. */
        if((rcc != NULL) && ( (*rcc == 0U) || (*rcc <=3U) ) )
        {
            regData |= (uint8_t)SBC_UJA_FAIL_SAFE_RCC_F(*rcc);
        }

        /* Send configuration to Fail-safe control register. */
        spiStat = SBC_DataTransfer(SBC_UJA_FAIL_SAFE, &regData, NULL);
    }
    else
    {
        status = SBC_CMD_ERROR;
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetFailSafe
 * Description   : This function reads from Fail-safe control register (0x02).
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
 * The following is an example of how to read from limp home control by
 * passing lhc parameter to SBC_GetFailSafe function:
 *  sbc_fail_safe_lhc_t lhc;
 *  sbc_fail_safe_rcc_t rcc;
 *  status_t status = SBC_GetFailSafe(&lhc, &rcc);
 *
 * Implements    : SBC_GetFailSafe_Activity
 *END**************************************************************************/
status_t SBC_GetFailSafe(sbc_fail_safe_lhc_t* const lhc,
        sbc_fail_safe_rcc_t * const rcc)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(lhc != NULL);

    uint8_t regData = 0;

    /* Read configuration from Fail-safe control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_FAIL_SAFE, NULL, &regData);

    /* Test if rcc should be read. */
    if( (rcc != NULL) && (spiStat == STATUS_SUCCESS) )
    {
        *rcc = (sbc_fail_safe_rcc_t)((regData & SBC_UJA_FAIL_SAFE_RCC_MASK) >> SBC_UJA_FAIL_SAFE_RCC_SHIFT);
    }

    if(spiStat == STATUS_SUCCESS)
    {
        *lhc = (sbc_fail_safe_lhc_t)(regData & SBC_UJA_FAIL_SAFE_LHC_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetSystemEvents
 * Description   : This function write System event capture enable
 * register (0x04).
 * This function enables or disables overtemperature warning,
 * SPI failure enable.
 * The following is an example of how to write to System event capture enable
 * register by passing sysEvnt parameter to SBC_SetSystemEvents function:
 *  sbc_sys_evnt_t sysEvnt;
 *  sysEvnt.owte = SBC_UJA_SYS_EVNT_OTWE_DIS;
 *  sysEvnt.spife = SBC_UJA_SYS_EVNT_SPIFE_DIS;
 *  status_t status = SBC_SetSystemEvents(&sysEvnt);
 *
 * Implements    : SBC_SetSystemEvents_Activity
 *END**************************************************************************/
status_t SBC_SetSystemEvents(const sbc_sys_evnt_t* const sysEvnt)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(sysEvnt != NULL);

    uint8_t regData = (uint8_t)((uint8_t)sysEvnt->owte | (uint8_t)sysEvnt->spife);

    /* Send configuration to System event register. */
    spiStat = SBC_DataTransfer(SBC_UJA_SYSTEM_EVNT, &regData, NULL);

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetSystemEvents
 * Description   : This function reads System event capture enable
 * register (0x04).
 * This function reads content of overtemperature warning and SPI failure
 * settings.
 * The following is an example of how to write to System event capture enable
 * register by passing sysEvnt parameter to SBC_GetSystemEvents function:
 *  sbc_sys_evnt_t sysEvnt;
 *  status_t status = SBC_GetSystemEvents(&sysEvnt);
 *
 * Implements    : SBC_GetSystemEvents_Activity
 *END**************************************************************************/
status_t SBC_GetSystemEvents(sbc_sys_evnt_t* const sysEvnt)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(sysEvnt != NULL);

    uint8_t regData = 0;

    /* Read configuration from System event register. */
    spiStat = SBC_DataTransfer(SBC_UJA_SYSTEM_EVNT, NULL, &regData);
    if(spiStat == STATUS_SUCCESS)
    {
        sysEvnt->owte = (sbc_sys_evnt_otwe_t)(regData & SBC_UJA_SYS_EVNT_OTWE_MASK);
        sysEvnt->spife = (sbc_sys_evnt_spife_t)(regData & SBC_UJA_SYS_EVNT_SPIFE_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetLock
 * Description   : This function writes to Lock control register (0x0A).
 * Sections of the register address area can be write-protected to protect
 * against unintended modifications. This facility only protects locked bits
 * from being modified via the SPI and will not prevent the UJA116xA updating
 * status registers etc. For SPI write disable set lock bit to 1.
 * This is mask for set lock control register.
 * The following is an example of how to write to Lock control register
 * by passing lockMask parameter to SBC_SetLock function:
 *  sbc_lock_t lockMask;
 *  lockMask = LK0C | LK1C | LK2C | LK3C | LK4C - Enable lock.
 *  lockMask &= ~(LK5C | LK6C) - Disable lock.
 *  status_t status = SBC_SetLock(lockMask);
 *
 * Implements    : SBC_SetLock_Activity
 *END**************************************************************************/
status_t SBC_SetLock(const sbc_lock_t lockMask)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    uint8_t regData = (uint8_t)lockMask;

    /* Send lock mask to Lock control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_LOCK, &regData, NULL);

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetLock
 * Description   : This function reads Lock control register (0x0A).
 * Sections of the register address area can be write-protected to protect
 * against unintended modifications. This facility only protects locked bits
 * from being modified via the SPI and will not prevent the UJA116xA updating
 * status registers etc. For SPI write disable set lock bit to 1.
 * This is mask for set lock control register.
 * The following is an example of how to read to Lock control register
 * by passing lockMask parameter to SBC_GetLock function:
 *  sbc_lock_t lockMask;
 *  status_t status = SBC_GetLock(lockMask);
 *
 * Implements    : SBC_GetLock_Activity
 *END**************************************************************************/
status_t SBC_GetLock(sbc_lock_t* const lockMask)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(lockMask != NULL);

    uint8_t regData = 0;

    /* Read configuration from Lock control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_LOCK, NULL, &regData);

    if(spiStat == STATUS_SUCCESS)
    {
        *lockMask = (sbc_lock_t)(regData & SBC_UJA_LOCK_LKNC_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetCanConfig
 * Description   : This function configures CAN peripheral behavior.
 * This function configures several registers which configure CAN.
 * It contains CAN control register, Transceiver event capture enable register,
 * CAN data rate selection, ID registers, ID mask registers,
 * Frame control register, Data mask 0 - 7 configuration.
 * The following is an example of how to write to CAN control registers
 * by passing can parameter to SBC_SetCanConfig function:
 *  sbc_can_conf_t can;
    can.canConf.cfdc = SBC_UJA_CAN_CFDC_DIS;
    can.canTransEvnt.cbse = SBC_UJA_TRANS_EVNT_CBSE_DIS;
    can.datRate = SBC_UJA_DAT_RATE_CDR_50KB;
 *  status_t status = SBC_SetCanConfig(&can);
 *
 * Implements    : SBC_SetCanConfig_Activity
 *END**************************************************************************/
status_t SBC_SetCanConfig(const sbc_can_conf_t* const can)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(can != NULL);

    const sbc_can_ctr_t *canC = &can->canConf;
    uint8_t regData = (uint8_t)((uint8_t)canC->cfdc | (uint8_t)canC->pncok
            | (uint8_t)canC->cpnc | (uint8_t)canC->cmc);

    /* Send configuration to CAN control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_CAN, &regData, NULL);
    regData = 0U;

    if(spiStat == STATUS_SUCCESS )
    {
        const sbc_trans_evnt_t *tEvnt = &can->canTransEvnt;
        /* Send configuration to Transceiver event register. */
        regData = (uint8_t)((uint8_t)tEvnt->cbse | (uint8_t)tEvnt->cfe
                | (uint8_t)tEvnt->cwe);
        spiStat = SBC_DataTransfer(SBC_UJA_TRANS_EVNT, &regData, NULL);
        regData = 0U;
    }
    if(spiStat == STATUS_SUCCESS )
    {
        /* Send configuration to CAN data rate selection. */
        regData = can->datRate;
        spiStat = SBC_DataTransfer(SBC_UJA_DAT_RATE, &regData, NULL);
    }
    if(spiStat == STATUS_SUCCESS )
    {
        /* Write other Can registers. */
        spiStat = sbc_write_can_others(can);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetCanConfig
 * Description   : This function reads CAN peripheral settings.
 * This function configures several registers which configure CAN.
 * It contains CAN control register, Transceiver event capture enable register,
 * CAN data rate selection, ID registers, ID mask registers,
 * Frame control register, Data mask 0 - 7 configuration.
 * The following is an example of how to read the CAN control registers
 * by passing can parameter to SBC_GetCanConfig function:
 *  sbc_can_conf_t can;
 *  status_t status = SBC_GetCanConfig(&can);
 *
 * Implements    : SBC_GetCanConfig_Activity
 *END**************************************************************************/
status_t SBC_GetCanConfig(sbc_can_conf_t* const can)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(can != NULL);

    sbc_can_ctr_t *canC = &can->canConf;
    uint8_t regData = 0;
    /* Read configuration from CAN control register. */
    spiStat = SBC_DataTransfer(SBC_UJA_CAN, NULL, &regData);

    if(spiStat == STATUS_SUCCESS )
    {
        /* Set CAN control register from device. */
        canC->cfdc = (sbc_can_cfdc_t)(regData & SBC_UJA_CAN_CFDC_MASK);
        canC->pncok = (sbc_can_pncok_t)(regData & SBC_UJA_CAN_PNCOK_MASK);
        canC->cpnc = (sbc_can_cpnc_t)(regData & SBC_UJA_CAN_CPNC_MASK);
        canC->cmc = (sbc_can_cmc_t)(regData & SBC_UJA_CAN_CMC_MASK);

        sbc_trans_evnt_t *tEvnt = &can->canTransEvnt;
        /* Read Transceiver event register. */
        regData = 0;
        spiStat = SBC_DataTransfer(SBC_UJA_TRANS_EVNT, NULL, &regData);
        /* Set Transceiver event register from device. */
        tEvnt->cbse = (sbc_trans_evnt_cbse_t)(regData & SBC_UJA_TRANS_EVNT_CBSE_MASK);
        tEvnt->cfe = (sbc_trans_evnt_cfe_t)(regData & SBC_UJA_TRANS_EVNT_CFE_MASK);
        tEvnt->cwe = (sbc_trans_evnt_cwe_t)(regData & SBC_UJA_TRANS_EVNT_CWE_MASK);
    }

    if(spiStat == STATUS_SUCCESS )
    {
        /* Read CAN data rate. */
        regData = 0;
        spiStat = SBC_DataTransfer(SBC_UJA_DAT_RATE, NULL, &regData);
        can->datRate = (sbc_dat_rate_t)(regData & SBC_UJA_DAT_RATE_CDR_MASK);
    }

    if(spiStat == STATUS_SUCCESS )
    {
        /* Read other can configuration register. */
        spiStat = sbc_read_can_others(can);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_SetWakePin
 * Description   : This function writes to WAKE pin event capture enable register (0x4C).
 * Local wake-up is enabled via bits WPRE and WPFE in the WAKE pin event capture
 * enable register. A wake-up event is triggered
 * by a LOW-to-HIGH (ifWPRE = 1) and/or a HIGH-to-LOW (if WPFE = 1) transition
 * on the WAKE pin. This arrangement allows for maximum flexibility when
 * designing a local wake-up circuit. In applications that do not use the local
 * wake-up facility, local wake-up should be disabled and the WAKE pin
 * connected to GND.
 * The following is an example of how to write to WAKE pin event capture enable
 * register by passing wakePin parameter to SBC_SetWakePin function:
 *  sbc_wake_t wakePin;
 *  wakePin.wpre = SBC_UJA_WAKE_EN_WPRE_DIS;
 *  wakePin.wpfe = SBC_UJA_WAKE_EN_WPFE_DIS;
 *  status_t status = SBC_SetWakePin(&wakePin);
 *
 * Implements    : SBC_SetWakePin_Activity
 *END**************************************************************************/
status_t SBC_SetWakePin(const sbc_wake_t* const wakePin)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(wakePin != NULL);

    uint8_t regData = (uint8_t)((uint8_t)wakePin->wpre | (uint8_t)wakePin->wpfe);

    /* Send configuration to WAKE pin event capture enable register. */
    spiStat = SBC_DataTransfer(SBC_UJA_WAKE_EN, &regData, NULL);

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetWakePin
 * Description   : This function reads WAKE pin event capture enable register (0x4C).
 * Local wake-up is enabled via bits WPRE and WPFE in the WAKE pin event capture
 * enable register. A wake-up event is triggered
 * by a LOW-to-HIGH (ifWPRE = 1) and/or a HIGH-to-LOW (if WPFE = 1) transition
 * on the WAKE pin. This arrangement allows for maximum flexibility when
 * designing a local wake-up circuit. In applications that do not use the local
 * wake-up facility, local wake-up should be disabled and the WAKE pin
 * connected to GND.
 * The following is an example of how to read from WAKE pin event capture enable
 * register by passing wakePin parameter to SBC_SetWakePin function:
 *  sbc_wake_t wakePin;
 *  status_t status = SBC_GetWakePin(&wakePin);
 *
 * Implements    : SBC_GetWakePin_Activity
 *END**************************************************************************/
status_t SBC_GetWakePin(sbc_wake_t* const wakePin)
{
    status_t status = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    uint8_t regData = 0;

    DEV_ASSERT(wakePin != NULL);

    /* Read configuration from WAKE pin event capture enable register. */
    spiStat = SBC_DataTransfer(SBC_UJA_WAKE_EN, NULL, &regData);

    if(spiStat == STATUS_SUCCESS)
    {
        wakePin->wpre = (sbc_wake_en_wpre_t)(regData & SBC_UJA_WAKE_EN_WPRE_MASK);
        wakePin->wpfe = (sbc_wake_en_wpfe_t)(regData & SBC_UJA_WAKE_EN_WPFE_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        status = SBC_COMM_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetMainStatus
 * Description   : This function reads the Main status register.
 * This function will clear R/W registers automatically after reading.
 * The following is an example of how to read Main status register.
 * by passing mainStatus parameter to SBC_GetMainStatus function:
 *  sbc_main_status_t mainStatus;
 *  status_t status = SBC_GetMainStatus(&mainStatus);
 *
 * Implements    : SBC_GetMainStatus_Activity
 *END**************************************************************************/
status_t SBC_GetMainStatus(sbc_main_status_t* const mainStatus)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(mainStatus != NULL);

        uint8_t readData = 0U;
        spiStat = SBC_DataTransfer(SBC_UJA_MAIN, NULL, &readData );

        if(spiStat == STATUS_SUCCESS)
        {
            mainStatus->otws = (sbc_main_otws_t)(readData & SBC_UJA_MAIN_OTWS_MASK);
            mainStatus->nms = (sbc_main_nms_t)(readData & SBC_UJA_MAIN_NMS_MASK);
            mainStatus->rss = (sbc_main_rss_t)(readData & SBC_UJA_MAIN_RSS_MASK);
        }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetWatchdogStatus
 * Description   : This function read Watchdog status register.
 * Watchdog status register.
 * This function will clear R/W registers automatically after reading.
 * The following is an example of how to read Watchdog status register
 * by passing mainStatus parameter to SBC_GetMainStatus function:
 *  sbc_wtdog_status_t watchdogStatus;
 *  status_t status = SBC_GetWatchdogStatus(&watchdogStatus);
 *
 * Implements    : SBC_GetWatchdogStatus_Activity
 *END**************************************************************************/
status_t SBC_GetWatchdogStatus(sbc_wtdog_status_t* const watchdogStatus)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(watchdogStatus != NULL);

    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_WTDOG_STAT, NULL, &readData );

    if(spiStat == STATUS_SUCCESS)
    {
        watchdogStatus->fnms = (sbc_wtdog_stat_fnms_t)(readData & SBC_UJA_WTDOG_STAT_FNMS_MASK);
        watchdogStatus->sdms = (sbc_wtdog_stat_sdms_t)(readData & SBC_UJA_WTDOG_STAT_SDMS_MASK);
        watchdogStatus->wds = (sbc_wtdog_stat_wds_t)(readData & SBC_UJA_WTDOG_STAT_WDS_MASK);
        /* Clear fnnms and sdmms register. */
        readData &=  (uint8_t)~SBC_UJA_WTDOG_STAT_FNMS_MASK;
        readData &=  (uint8_t)~SBC_UJA_WTDOG_STAT_SDMS_MASK;
        spiStat = SBC_DataTransfer(SBC_UJA_WTDOG_STAT, &readData, NULL );
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetSupplyStatus
 * Description   : This functions reads Supply voltage status register.
 * This function clear R/W status after reading writing 0 to register.
 * It contains V2/VEXT status and V1 status.
 * The following is an example of how to read Supply voltage status register
 * by passing supStatus parameter to SBC_GetSupplyStatus function:
 *  sbc_supply_status_t supStatus;
 *  status_t status = SBC_GetSupplyStatus(&supStatus);
 *
 * Implements    : SBC_GetSupplyStatus_Activity
 *END**************************************************************************/
status_t SBC_GetSupplyStatus(sbc_supply_status_t* const supStatus)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(supStatus != NULL);

    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_SUPPLY_STAT, NULL, &readData );

    if(spiStat == STATUS_SUCCESS)
    {
        supStatus->v2s = (sbc_supply_stat_v2s_t)(readData & SBC_UJA_SUPPLY_STAT_V2S_MASK);
        supStatus->v1s = (sbc_supply_stat_v1s_t)(readData & SBC_UJA_SUPPLY_STAT_V1S_MASK);
        /* Clear Supply status. */
        readData &= (uint8_t)~SBC_UJA_SUPPLY_STAT_V2S_MASK;
        readData &= (uint8_t)~SBC_UJA_SUPPLY_STAT_V1S_MASK;
        spiStat = SBC_DataTransfer(SBC_UJA_SUPPLY_STAT, &readData, NULL );
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetCanStatus
 * Description   : This functions reads Transceiver status register.
 * It contains CAN transceiver status, CAN partial networking error,
 * CAN partial networking status, CAN oscillator status, CAN-bus silence status,
 * VCAN status, CAN failure status.
 * The following is an example of how to read Transceiver status register
 * by passing transStatus parameter to SBC_GetCanStatus function:
 *  sbc_trans_stat_t transStatus;
 *  status_t status = SBC_GetCanStatus(&supStatus);
 *
 * Implements    : SBC_GetCanStatus_Activity
 *END**************************************************************************/
status_t SBC_GetCanStatus(sbc_trans_stat_t* const transStatus)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    uint8_t readData = 0U;
    DEV_ASSERT(transStatus != NULL);

    spiStat = SBC_DataTransfer(SBC_UJA_TRANS_STAT, NULL, &readData );

    if(spiStat == STATUS_SUCCESS)
    {
        transStatus->cts = (sbc_trans_stat_cts_t)(readData & SBC_UJA_TRANS_STAT_CTS_MASK);
        transStatus->cpnerr = (sbc_trans_stat_cpnerr_t)(readData & SBC_UJA_TRANS_STAT_CPNERR_MASK);
        transStatus->cpns = (sbc_trans_stat_cpns_t)(readData & SBC_UJA_TRANS_STAT_CPNS_MASK);
        transStatus->coscs = (sbc_trans_stat_coscs_t)(readData & SBC_UJA_TRANS_STAT_COSCS_MASK);
        transStatus->cbss = (sbc_trans_stat_cbss_t)(readData & SBC_UJA_TRANS_STAT_CBSS_MASK);
        transStatus->vcs = (sbc_trans_stat_vcs_t)(readData & SBC_UJA_TRANS_STAT_VCS_MASK);
        transStatus->cfs = (sbc_trans_stat_cfs_t)(readData & SBC_UJA_TRANS_STAT_CFS_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetWakeStatus
 * Description   : This functions reads WAKE pin status register.
 * This function reads switching threshold of voltage on WAKE pin.
 * The following is an example of how to read WAKE pin status register
 * by passing wakeStatus parameter to SBC_GetWakeStatus function:
 *  sbc_wake_stat_wpvs_t wakeStatus;
 *  status_t status = SBC_GetWakeStatus(&wakeStatus);
 *
 * Implements    : SBC_GetWakeStatus_Activity
 *END**************************************************************************/
status_t SBC_GetWakeStatus(sbc_wake_stat_wpvs_t* const wakeStatus)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(wakeStatus != NULL);

    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_WAKE_STAT, NULL, &readData );

    if(spiStat == STATUS_SUCCESS)
    {
        *wakeStatus = (sbc_wake_stat_wpvs_t)(readData & SBC_UJA_WAKE_STAT_WPVS_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetEventsStatus
 * Description   : This functions reads Event capture registers.
 * This functions reads global events statuses: Global event status,
 * System event status, Supply event status, Transceiver event status,
 * WAKE pin event status.
 * The following is an example of how to read WAKE pin status register
 * by passing events parameter to SBC_GetEventsStatus function:
 *  sbc_evn_capt_t events;
 *  status_t status = SBC_GetEventsStatus(&events);
 *
 * Implements    : SBC_GetEventsStatus_Activity
 *END**************************************************************************/
status_t SBC_GetEventsStatus(sbc_evn_capt_t* const events)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(events != NULL);

    /* Clean events statuses structure. */
    sbc_clean_events_status(events);
    /* Global event status. */
    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_GL_EVNT_STAT, NULL, &readData );

    sbc_gl_evnt_stat_t* glEvent = &events->glEvnt;

    if(spiStat == STATUS_SUCCESS)
    {
        glEvent->wpe = (sbc_gl_evnt_stat_wpe_t)(readData & SBC_UJA_GL_EVNT_STAT_WPE_MASK);
        glEvent->trxe = (sbc_gl_evnt_stat_trxe_t)(readData & SBC_UJA_GL_EVNT_STAT_TRXE_MASK);
        glEvent->supe = (sbc_gl_evnt_stat_supe_t)(readData & SBC_UJA_GL_EVNT_STAT_SUPE_MASK);
        glEvent->syse = (sbc_gl_evnt_stat_syse_t)(readData & SBC_UJA_GL_EVNT_STAT_SYSE_MASK);
    }

    /* System event status. */
    readData = 0;
    /* Test if System event occurred. */
    if(glEvent->syse == SBC_UJA_GL_EVNT_STAT_SYSE)
    {
        spiStat = SBC_DataTransfer(SBC_UJA_SYS_EVNT_STAT, NULL, &readData );
        if(spiStat == STATUS_SUCCESS)
        {
            sbc_sys_evnt_stat_t* sysEvent = &events->sysEvnt;
            sysEvent->po = (sbc_sys_evnt_stat_po_t)(readData & SBC_UJA_SYS_EVNT_STAT_PO_MASK);
            sysEvent->otw = (sbc_sys_evnt_stat_otw_t)(readData & SBC_UJA_SYS_EVNT_STAT_OTW_MASK);
            sysEvent->spif = (sbc_sys_evnt_stat_spif_t)(readData & SBC_UJA_SYS_EVNT_STAT_SPIF_MASK);
            sysEvent->wdf = (sbc_sys_evnt_stat_wdf_t)(readData & SBC_UJA_SYS_EVNT_STAT_WDF_MASK);
        }
    }
    /* Supply event status. */
    readData = 0;
    /* Test if Supply event occurred. */
    if(glEvent->supe == SBC_UJA_GL_EVNT_STAT_SUPE)
    {
        spiStat = SBC_DataTransfer(SBC_UJA_SUP_EVNT_STAT, NULL, &readData );
        if(spiStat == STATUS_SUCCESS)
        {
            sbc_sup_evnt_stat_t* supEvent = &events->supEvnt;
            supEvent->v2o = (sbc_sup_evnt_stat_v2o_t)(readData & SBC_UJA_SUP_EVNT_STAT_V2O_MASK);
            supEvent->v2u = (sbc_sup_evnt_stat_v2u_t)(readData & SBC_UJA_SUP_EVNT_STAT_V2U_MASK);
            supEvent->v1u = (sbc_sup_evnt_stat_v1u_t)(readData & SBC_UJA_SUP_EVNT_STAT_V1U_MASK);
        }
    }

    /* Transceiver event status. */
    readData = 0;
    /* Test if Transceiver event occurred. */
    if(glEvent->trxe == SBC_UJA_GL_EVNT_STAT_TRXE)
    {
        spiStat = SBC_DataTransfer(SBC_UJA_TRANS_EVNT_STAT, NULL, &readData );
        if(spiStat == STATUS_SUCCESS)
        {
            sbc_trans_evnt_stat_t* transEvent = &events->transEvnt;
            transEvent->pnfde = (sbc_trans_evnt_stat_pnfde_t)(readData & SBC_UJA_TRANS_EVNT_STAT_PNFDE_MASK);
            transEvent->cbs = (sbc_trans_evnt_stat_cbs_t)(readData & SBC_UJA_TRANS_EVNT_STAT_CBS_MASK);
            transEvent->cf = (sbc_trans_evnt_stat_cf_t)(readData & SBC_UJA_TRANS_EVNT_STAT_CF_MASK);
            transEvent->cw = (sbc_trans_evnt_stat_cw_t)(readData & SBC_UJA_TRANS_EVNT_STAT_CW_MASK);
        }
    }

    /* WAKE pin event status. */
    readData = 0;
    /* Test if WAKE pin event occurred. */
    if(glEvent->wpe == SBC_UJA_GL_EVNT_STAT_WPE)
    {
        spiStat = SBC_DataTransfer(SBC_UJA_WAKE_EVNT_STAT, NULL, &readData );
        if(spiStat == STATUS_SUCCESS)
        {
            sbc_wake_evnt_stat_t* wakeEvent = &events->wakePinEvnt;
            wakeEvent->wpr = (sbc_wake_evnt_stat_wpr_t)(readData & SBC_UJA_WAKE_EVNT_STAT_WPR_MASK);
            wakeEvent->wpf = (sbc_wake_evnt_stat_wpf_t)(readData & SBC_UJA_WAKE_EVNT_STAT_WPF_MASK);
        }
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_CleanEvents
 * Description   : This function clears Event capture registers.
 * It contains Global event status, System event status, Supply event status,
 * Transceiver event status, WAKE pin event status.
 * This function write 1 to bit which should be delete.
 * After an event source has been identified, the status flag should be cleared
 * (set to 0) by writing 1 to the relevant status bit
 * (writing 0 will have no effect). Write true value to appropriate enumeration.
 * The following is an example of how to read from WAKE pin event capture enable
 * register by passing events parameter to SBC_CleanEvents function
 * (Global event status does not have effect to clearing statuses):
 *  sbc_evn_capt_t events;
 *  events.sysEvnt.po = SBC_UJA_SYS_EVNT_STAT_PO;
 *  events.sysEvnt.otw = SBC_UJA_SYS_EVNT_STAT_OTW;
 *  events.sysEvnt.spif = SBC_UJA_SYS_EVNT_STAT_SPIF;
 *  events.sysEvnt.wdf = SBC_UJA_SYS_EVNT_STAT_WDF;
 *
 *  events.supEvnt.v1u = SBC_UJA_SUP_EVNT_STAT_V1U;
 *  events.supEvnt.v2u = SBC_UJA_SUP_EVNT_STAT_V2U;
 *  events.supEvnt.v2o = SBC_UJA_SUP_EVNT_STAT_V2O;
 *  status_t status = SBC_CleanEvents(&events);
 *
 * Implements    : SBC_CleanEvents_Activity
 *END**************************************************************************/
status_t SBC_CleanEvents(const sbc_evn_capt_t* const events)
{

    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;
    uint8_t clearMask = 0;

    DEV_ASSERT(events != NULL);

    const sbc_gl_evnt_stat_t* glEvents = &events->glEvnt;

    /* Clear global events. */
    clearMask = (uint8_t)((uint8_t)glEvents->supe | (uint8_t)glEvents->syse
                | (uint8_t)glEvents->trxe | (uint8_t)glEvents->wpe);
    spiStat = SBC_DataTransfer(SBC_UJA_GL_EVNT_STAT, &clearMask, NULL);
    ujaStat = (status_t)((uint32_t)ujaStat | (uint32_t)spiStat);
    const sbc_sys_evnt_stat_t* sysEvents = &events->sysEvnt;

    /* Clear system events. */
    clearMask = (uint8_t)((uint8_t)sysEvents->otw | (uint8_t)sysEvents->po
                | (uint8_t)sysEvents->spif | (uint8_t)sysEvents->wdf);
    spiStat = SBC_DataTransfer(SBC_UJA_SYS_EVNT_STAT, &clearMask, NULL);
    ujaStat = (status_t)((uint32_t)ujaStat | (uint32_t)spiStat);
    const sbc_sup_evnt_stat_t* supEvents = &events->supEvnt;

    /* Clear supply events. */
    clearMask = (uint8_t)((uint8_t)supEvents->v1u | (uint8_t)supEvents->v2o
                | (uint8_t)supEvents->v2u);
    spiStat = SBC_DataTransfer(SBC_UJA_SUP_EVNT_STAT, &clearMask, NULL);
    ujaStat = (status_t)((uint32_t)ujaStat | (uint32_t)spiStat);
    const sbc_trans_evnt_stat_t* transEvents = &events->transEvnt;

    /* Clear transfer events. */
    clearMask = (uint8_t)( (uint8_t)transEvents->cbs | (uint8_t)transEvents->cf
                | (uint8_t)transEvents->cw | (uint8_t)transEvents->pnfde);
    spiStat = SBC_DataTransfer(SBC_UJA_TRANS_EVNT_STAT, &clearMask, NULL);
    ujaStat = (status_t)((uint32_t)ujaStat | (uint32_t)spiStat);
    const sbc_wake_evnt_stat_t* wakeEvent = &events->wakePinEvnt;

    /* Clear wake up events. */
    clearMask = (uint8_t)((uint8_t)wakeEvent->wpf | (uint8_t)wakeEvent->wpr);
    spiStat = SBC_DataTransfer(SBC_UJA_WAKE_EVNT_STAT, &clearMask, NULL);
    ujaStat = (status_t)((uint32_t)ujaStat | (uint32_t)spiStat);
    
    if(ujaStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetAllStatus
 * Description   : This function reads all statuses from SBC device.
 * It reads all status registers: Main status and Watchdog status,
 * Supply voltage status,  Transceiver status, WAKE pin status,
 * Event capture registers.
 *
 * The following is an example of how to read all registers by passing status
 * parameter to SBC_GetAllStatus function:
 *  sbc_status_group_t status;
 *  status_t status = SBC_GetAllStatus(&events);
 *
 * Implements    : SBC_GetAllStatus_Activity
 *END**************************************************************************/
status_t SBC_GetAllStatus(sbc_status_group_t* const status)
{
    status_t ujaStat = STATUS_SUCCESS;

    DEV_ASSERT(status != NULL);

    ujaStat = SBC_GetMainStatus(&status->mainS);

    if(ujaStat == STATUS_SUCCESS)
    {
        ujaStat = SBC_GetWatchdogStatus(&status->wtdog);
    }

    if(ujaStat == STATUS_SUCCESS)
    {
        ujaStat = SBC_GetSupplyStatus(&status->supply);
    }

    if(ujaStat == STATUS_SUCCESS)
    {
        ujaStat = SBC_GetCanStatus(&status->trans);
    }

    if(ujaStat == STATUS_SUCCESS)
    {
        ujaStat = SBC_GetWakeStatus(&status->wakePin);
    }

    if(ujaStat == STATUS_SUCCESS)
    {
        ujaStat = SBC_GetEventsStatus(&status->events);
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetMtpnvStatus
 * Description   : This function reads MTPNV status register.
 * The MTPNV cells can be reprogrammed a maximum of 200 times (Ncy(W)MTP).
 * Bit NVMPS in the MTPNV status register indicates
 * whether the non-volatile cells can be reprogrammed. This register also
 * contains a write counter, WRCNTS, that is incremented each time the
 * MTPNV cells are reprogrammed (up to a maximum value of 111111; there is
 * no overflow; performing a factory reset also increments the counter).
 * This counter is provided for information purposes only; reprogramming will
 * not be rejected when it reaches its maximum value.
 *
 * The following is an example of how to read all registers by passing mtpnv
 * parameter to SBC_GetMtpnvStatus function:
 *  sbc_status_group_t status;
 *  status_t status = SBC_GetMtpnvStatus(&events);
 *
 * Implements    : SBC_GetMtpnvStatus_Activity
 *END**************************************************************************/
status_t SBC_GetMtpnvStatus(sbc_mtpnv_stat_t* const mtpnv)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(mtpnv != NULL);

    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_MTPNV_STAT, NULL, &readData);

    if(spiStat == STATUS_SUCCESS)
    {
        mtpnv->wrcnts = (sbc_mtpnv_stat_wrcnts_t)((readData & SBC_UJA_MTPNV_STAT_WRCNTS_MASK) >> SBC_UJA_MTPNV_STAT_WRCNTS_SHIFT);
        mtpnv->eccs = (sbc_mtpnv_stat_eccs_t)(readData & SBC_UJA_MTPNV_STAT_ECCS_MASK);
        mtpnv->nvmps = (sbc_mtpnv_stat_nvmps_t)(readData & SBC_UJA_MTPNV_STAT_NVMPS_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_GetFactoriesSettings
 * Description   : This function reads Start-up control register and
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
 * The following is an example of how to read Start-up control register and
 * SBC configuration control register by passing factoriesConf
 * parameter to SBC_GetFactoriesSettings function:
 *  sbc_factories_conf_t factoriesConf;
 *  status_t status = SBC_GetFactoriesSettings(&factoriesConf);
 *
 * Implements    : SBC_GetFactoriesSettings_Activity
 *END**************************************************************************/
status_t SBC_GetFactoriesSettings(sbc_factories_conf_t* const factoriesConf)
{
    status_t ujaStat = STATUS_SUCCESS;
    status_t spiStat = STATUS_SUCCESS;

    DEV_ASSERT(factoriesConf != NULL);

    /* Read Start-up control register. */
    uint8_t readData = 0U;
    spiStat = SBC_DataTransfer(SBC_UJA_START_UP, NULL, &readData);

    if(spiStat == STATUS_SUCCESS)
    {
        sbc_start_up_t* startUp = &factoriesConf->startUp;
        startUp->rlc = (sbc_start_up_rlc_t)(readData & SBC_UJA_START_UP_RLC_MASK);
        startUp->v2suc = (sbc_start_up_v2suc_t)(readData & SBC_UJA_START_UP_V2SUC_MASK);
        readData = 0;
        spiStat = SBC_DataTransfer(SBC_UJA_SBC, NULL, &readData);
    }

    if(spiStat == STATUS_SUCCESS)
    {
        sbc_sbc_t* sbc = &factoriesConf->control;
        sbc->v1rtsuc = (sbc_sbc_v1rtsuc_t)(readData & SBC_UJA_SBC_V1RTSUC_MASK);
        sbc->fnmc = (sbc_sbc_fnmc_t)(readData & SBC_UJA_SBC_FNMC_MASK);
        sbc->sdmc = (sbc_sbc_sdmc_t)(readData & SBC_UJA_SBC_SDMC_MASK);
        sbc->slpc = (sbc_sbc_slpc_t)(readData & SBC_UJA_SBC_SLPC_MASK);
    }

    if(spiStat != STATUS_SUCCESS)
    {
        ujaStat = SBC_COMM_ERROR;
    }

    return ujaStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_ChangeFactoriesSettings
 * Description   : This function sets Start-up control register and
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
 * The following is an example of how to write Start-up control register and
 * SBC configuration control register by passing parameter
 * to SBC_ChangeFactoriesSettings function:
 *  sbc_factories_conf_t newConf;
 *  status_t status = SBC_ChangeFactoriesSettings(&newConf);
 *
 * Implements    : SBC_ChangeFactoriesSettings_Activity
 *END**************************************************************************/
status_t SBC_ChangeFactoriesSettings(const sbc_factories_conf_t* const newConf)
{
    status_t ujaStat = STATUS_SUCCESS;
    /* Read if writing is possible. */
    sbc_mtpnv_stat_t mtvnStatus;

    DEV_ASSERT(newConf != NULL);

    ujaStat = SBC_GetMtpnvStatus(&mtvnStatus);

    if((ujaStat == STATUS_SUCCESS) && (mtvnStatus.eccs == SBC_UJA_MTPNV_STAT_ECCS_NO)
            && (mtvnStatus.nvmps == SBC_UJA_MTPNV_STAT_NVMPS)
            && (newConf->control.fnmc != SBC_UJA_SBC_FNMC_EN))
    {
        ujaStat = sbc_change_factories_direct(newConf);
        OSIF_TimeDelay(1);
    }
    else
    {
        if(ujaStat == STATUS_SUCCESS)
        {
            ujaStat = SBC_NVN_ERROR;
        }
    }

    return ujaStat;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : SBC_DataTransfer
 * Description   : This function sends data over LSPI to SBC device.
 * This function send 8 bites to SBC device register according device address
 * which is selected. This transfer uses 16bit LSPI. CS polarity - active low,
 * clock phase on second edge. Clock polarity active high.
 *
 * The following is an example of how to send data to SBC device by passing
 * sendData parameter to SBC_DataTransfer function:
 *  uint8_t sendData = wtdog->modeControl | wtdog->nominalPeriod;
 *  status_t status = SBC_DataTransfer(SBC_UJA_WTDOG_CTR, &sendData, NULL);
 *
 * The following is an example of how to read data to SBC device by passing
 * sendData parameter to SBC_DataTransfer function:
 *  uint8_t receiveData = 0;
 *  status_t status = SBC_DataTransfer(SBC_UJA_WTDOG_CTR, NULL, &receiveData);
 *
 * Implements    : SBC_DataTransfer_Activity
 *END**************************************************************************/
status_t SBC_DataTransfer(const sbc_register_t regName,
        const uint8_t* const sendData, uint8_t* const receiveData)
{

    status_t status = STATUS_SUCCESS;

    uint8_t command[2] = {0U, 0U};
    uint8_t readOnlyMask = 0x00U;
    uint8_t readData[2] = {0U, 0U};

    /* Test if there is data for sending. */
    if(sendData == NULL)
    {
       /* This transfer is read only. */
       readOnlyMask = 0x01U;
       command[0] = 0U;
    }
    else
    {
        command[0] = *sendData;
    }

    /* Address of device with read only bit. */
    command[1] = (uint8_t)((SBC_UJA_REG_ADDR_F(regName) | readOnlyMask) & 0xFFU);

    status = LPSPI_DRV_MasterTransferBlocking(g_drvConfig.lpspiIntace, command,
            readData, 2U, SBC_UJA_TIMEOUT );

    if( readData[1] != command[1])
    {
        status = SBC_COMM_ERROR;
    }

    /* Copy content of register to receive data. */
    if(receiveData != NULL)
    {
        *receiveData = readData[0];
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sbc_write_can_others
 * Description   : This function is internal function. It is not accessible outside
 * this file. This function writes to ID registers, ID mask registers, Frame
 * control register and Data mask registers.
 *
 *END**************************************************************************/
static status_t sbc_write_can_others(const sbc_can_conf_t* const can)
{
    status_t spiStat = STATUS_SUCCESS;
    uint8_t i;
    uint8_t regData = 0;
    for(i=0; i< SBC_UJA_COUNT_DMASK; i++)
    {
        if(i < SBC_UJA_COUNT_ID_REG)
        {
            /* Set ID registers. */
            spiStat = SBC_DataTransfer((sbc_register_t)( (uint8_t)SBC_UJA_IDENTIF_0 + i),
                    &can->identif[i], NULL);
            /* Set ID mask registers. */

            if(spiStat == STATUS_SUCCESS)
            {
                spiStat = SBC_DataTransfer((sbc_register_t)(
                        (uint8_t)SBC_UJA_MASK_0 + i),&can->mask[i], NULL);
            }

        }

        /* Set Data mask registers. */
        if(spiStat == STATUS_SUCCESS)
        {
            spiStat = SBC_DataTransfer((sbc_register_t)(
                    (uint8_t)SBC_UJA_DAT_MASK_0 + i),&can->dataMask[i], NULL);
        }

    }

    if(spiStat == STATUS_SUCCESS)
    {
        /* Set configuration to Frame control register. */
        const sbc_frame_t* const frame = &can->frame;
        regData = (uint8_t)( (uint8_t)frame->ide | (uint8_t)frame->pndm |
                (uint8_t)frame->dlc);
        spiStat = SBC_DataTransfer(SBC_UJA_FRAME_CTR, &regData, NULL);
    }

    return spiStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sbc_read_can_others
 * Description   : This function is internal function. It is not accessible outside
 * this file. This function reads to ID registers, ID mask registers, Frame
 * control register and Data mask registers.
 *
 *END**************************************************************************/
static status_t sbc_read_can_others(sbc_can_conf_t* const can)
{
    status_t spiStat = STATUS_SUCCESS;
    uint8_t i;
    uint8_t regData = 0;
    for(i=0; i< SBC_UJA_COUNT_DMASK; i++)
    {
        if(i < SBC_UJA_COUNT_ID_REG)
        {
            /* Read ID registers. */
            regData = 0;
            spiStat = SBC_DataTransfer((sbc_register_t)( (uint8_t)SBC_UJA_IDENTIF_0 + i), NULL, &regData);
            can->identif[i] = (sbc_identifier_t)(regData & SBC_UJA_IDENTIF_X_MASK);
            /* Read ID mask registers. */
            regData = 0;

            if(spiStat == STATUS_SUCCESS)
            {
                spiStat = SBC_DataTransfer((sbc_register_t)(
                        (uint8_t)SBC_UJA_MASK_0 + i), NULL, &regData);
                can->mask[i] = (sbc_identif_mask_t)(regData & SBC_UJA_MASK_X_MASK);
            }

        }

        /* Read Data mask registers. */
        regData = 0;

        if(spiStat == STATUS_SUCCESS)
        {
            spiStat = SBC_DataTransfer((sbc_register_t)(
                    (uint8_t)SBC_UJA_DAT_MASK_0 + i), NULL, &regData);
            can->dataMask[i] = (sbc_data_mask_t)(regData & SBC_UJA_DATA_MASK_X_MASK);
        }

    }

    if(spiStat == STATUS_SUCCESS)
    {
        /* Read Frame control register. */
        regData = 0;
        sbc_frame_t* frame = &can->frame;
        spiStat = SBC_DataTransfer(SBC_UJA_FRAME_CTR, NULL, &regData);
        frame->ide = (sbc_frame_ctr_ide_t)(regData & SBC_UJA_FRAME_CTR_IDE_MASK);
        frame->pndm = (sbc_frame_ctr_pndm_t)(regData & SBC_UJA_FRAME_CTR_PNDM_MASK);
        frame->dlc = (sbc_frame_ctr_dlc_t)(regData & SBC_UJA_FRAME_CTR_DLC_MASK);
    }

    return spiStat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sbc_clean_events_status
 * Description   : This function is internal function. It is not accessible outside
 * this file. This function clean event status structure to 0
 *
 *END**************************************************************************/
static void sbc_clean_events_status(sbc_evn_capt_t *event)
{
    /* Clean system events status. */
    event->sysEvnt.po = (sbc_sys_evnt_stat_po_t)0U;
    event->sysEvnt.otw = (sbc_sys_evnt_stat_otw_t)0U;
    event->sysEvnt.spif = (sbc_sys_evnt_stat_spif_t)0U;
    event->sysEvnt.wdf = (sbc_sys_evnt_stat_wdf_t)0U;
    /* Clean supply events status. */
    event->supEvnt.v2o = (sbc_sup_evnt_stat_v2o_t)0U;
    event->supEvnt.v2u = (sbc_sup_evnt_stat_v2u_t)0U;
    event->supEvnt.v1u = (sbc_sup_evnt_stat_v1u_t)0U;
    /* Clean trans. events status. */
    event->transEvnt.pnfde = (sbc_trans_evnt_stat_pnfde_t)0U;
    event->transEvnt.cbs = (sbc_trans_evnt_stat_cbs_t)0U;
    event->transEvnt.cf = (sbc_trans_evnt_stat_cf_t)0U;
    event->transEvnt.cw = (sbc_trans_evnt_stat_cw_t)0U;
    /* Clean wake pin events status. */
    event->wakePinEvnt.wpr = (sbc_wake_evnt_stat_wpr_t)0U;
    event->wakePinEvnt.wpf = (sbc_wake_evnt_stat_wpf_t)0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sbc_get_factories_crc
 * Description   : This function is internal function. It is not accessible outside
 * this file. It return CRC of factories settings - Start-up control register
 * and SBC configuration control register.
 *
 *END**************************************************************************/
static uint8_t sbc_get_factories_crc(uint8_t* data)
{
    uint8_t crc = 0xFF, i,j;

    for(i = 0; i<= 1U; i++ )
    {
        data[i] = (uint8_t)(data[i] ^ crc);
        for(j = 0; j<=7U; j++)
        {
            if(data[i] >= 128U)
            {
                data[i] = (uint8_t)( (uint8_t)data[i] << 1U);
                data[i] = (uint8_t)(data[i] ^ 0x2FU);
            }
            else
            {
                data[i] = (uint8_t)(data[i] << 1U);
            }
        }
        crc = data[i];
    }
    crc = (uint8_t)(crc ^ 0xFFU);
    return crc;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sbc_change_factories_direct
 * Description   : This function is internal function. It is not accessible outside
 * this file. This function tries to set Start-up control register and
 * SBC configuration control register without checking if it is possible.
 * Check must be done before.
 *
 *END**************************************************************************/
static status_t sbc_change_factories_direct(const sbc_factories_conf_t* const factory)
{
    status_t ujaStat = STATUS_SUCCESS;
    uint8_t sendData[2] = {0, 0};

    DEV_ASSERT(factory != NULL);

    sendData[0] =  (uint8_t)((uint8_t)factory->startUp.rlc | (uint8_t)factory->startUp.v2suc);
    ujaStat = SBC_DataTransfer(SBC_UJA_START_UP, &sendData[0], NULL);

    if(ujaStat == STATUS_SUCCESS)
    {
        const sbc_sbc_t* ctrl = &factory->control;
        sendData[1] =  (uint8_t)((uint8_t)ctrl->v1rtsuc | (uint8_t)ctrl->fnmc |
                (uint8_t)ctrl->sdmc | (uint8_t)ctrl->slpc);
        ujaStat = SBC_DataTransfer(SBC_UJA_SBC, &sendData[1], NULL);
    }

    if(ujaStat == STATUS_SUCCESS)
    {
        uint8_t crc = sbc_get_factories_crc(sendData);
        ujaStat = SBC_DataTransfer(SBC_UJA_MTPNV_CRC, &crc, NULL);
    }

    return ujaStat;
}
