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
 * @sbc_uja113x.c
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
 * Violates MISRA 2012 Advisory Rule 2.5, Macro not referenced
 * Macros which are not currently referenced in code but can be used in the
 * future or by user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 * 
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast 
 * from 'essentially Boolean' to 'essentially unsigned'.
 * This is required by the conversion of a bool into a unsigned char.
 */

#define SPI_FRAME_LEN_16    2U
#define SPI_FRAME_LEN_24    3U
#define SPI_FRAME_LEN_32    4U
#define SPI_FRAME_LEN_64    8U
#define SPI_FRAME_LEN_128   16U

#define UJA113X_DRIVER_COUNT    2U

#include "sbc_uja113x_map.h"
#include "sbc_uja113x.h"
#include "device_registers.h"
#include "lpspi_master_driver.h"

/**
 * @brief Internal SBC driver structure
 * @ingroup internals
 * 
 * Implements : uja113x_driver_t_Class
 */
typedef struct
{
    /**
     * @brief Chip ID
     */
    uja113x_chip_t chipType;
    /**
     * @brief LPSPI used to communicate with SBC
     */
    uint32_t lpspiInstance;
    /**
     * @brief SPI timeout
     */
    uint32_t timeout;
} uja113x_driver_t;

#ifndef UJA113X_DRIVER_COUNT
#warning "UJA113X_DRIVER_COUNT not defined! Default value set to 1."
#define UJA113X_DRIVER_COUNT    1U
#endif

#define DRIVER_COUNT__    (UJA113X_DRIVER_COUNT + 0U)

#if (DRIVER_COUNT__ < 1U)
#error "Invalid driver count! UJA113X_DRIVER_COUNT has to be > 0"
#endif

/**
 * @brief Internal array of SBC units connected to MCU
 * @details The count of SBC units can be influenced by UJA113X_DRIVER_COUNT
 * which can be defined by the user in sbc_uja113x_usr.h file. If not defined,
 * the default value is 1!
 * 
 * @ingroup internals
 * 
 * Implements : multiple_sbc_support_Class
 */
static uja113x_driver_t g_drivers[DRIVER_COUNT__];

/*FUNCTION********************************************************************
 * Implements : UJA113X_SbcTransfer_Activity
 *END************************************************************************/
status_t UJA113X_SbcTransfer(uint32_t instance,
                             bool     read,
                             uint8_t * txData,
                             uint8_t * rxData,
                             uint8_t  size)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);

    if (true == read)
    {
        /* Set LSB = read */
        txData[0U] |= 0x01U;
    }
    else
    {
        /* Clear LSB = write */
        txData[0U] &= 0xFEU;
    }

    retval = LPSPI_DRV_MasterTransferBlocking(g_drivers[instance].lpspiInstance,
                                              txData, rxData, size,
                                              g_drivers[instance].timeout);
    /* 
     * Check the communication status and first byte returned by SBC, the
     * byte must match the register address
     */
    if (STATUS_SUCCESS == retval)
    {
        if (txData[0U] != rxData[0U])
        {
           retval = SBC_CMD_ERROR;
        }
    }

    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_Init_Activity
 *END************************************************************************/
status_t UJA113X_Init(uint32_t instance,
                      const uja113x_init_cfg_t * const deviceConfig,
                      const uja113x_drv_cfg_t * drvConfig)
{
    uint32_t retval = (uint32_t) STATUS_SUCCESS;
    status_t retStatus = STATUS_SUCCESS;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != deviceConfig);
    DEV_ASSERT(NULL != drvConfig);

    uint8_t txData[SPI_FRAME_LEN_16] = {0U};
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    retStatus = UJA113X_RegisterDevice(instance, drvConfig);
    retval = (uint32_t) retStatus;

    if (NULL != deviceConfig->timscTimer1Cfg)
    {
        retStatus = UJA113X_SetTimer(instance, deviceConfig->timscTimer1Cfg);
        retval |= (uint32_t) retStatus;
    }

    retStatus = UJA113X_SetSystemInterrupts(instance, deviceConfig->sysieMask);
    retval |= (uint32_t) retStatus;

    txData[0U] = MEM0_ADDRESS;
    txData[1U] = deviceConfig->memory0;
    retStatus = UJA113X_SbcTransfer(instance, false, txData, rxData, SPI_FRAME_LEN_16);
    retval |= (uint32_t) retStatus;

    txData[0U] = MEM1_ADDRESS;
    txData[1U] = deviceConfig->memory1;

    retStatus = UJA113X_SbcTransfer(instance, false, txData, rxData, SPI_FRAME_LEN_16);
    retval |= (uint32_t) retStatus;

    txData[0U] = MEM2_ADDRESS;
    txData[1U] = deviceConfig->memory2;
    retStatus = UJA113X_SbcTransfer(instance, false, txData, rxData, SPI_FRAME_LEN_16);
    retval |= (uint32_t) retStatus;

    txData[0U] = MEM3_ADDRESS;
    txData[1U] = deviceConfig->memory3;
    retStatus = UJA113X_SbcTransfer(instance, false, txData, rxData, SPI_FRAME_LEN_16);
    retval |= (uint32_t) retStatus;

    if (NULL != deviceConfig->timscTimer2Cfg)
    {
        retStatus = UJA113X_SetTimer(instance, deviceConfig->timscTimer2Cfg);
        retval |= (uint32_t) retStatus;
    }

    if (NULL != deviceConfig->timscTimer3Cfg)
    {
        retStatus = UJA113X_SetTimer(instance, deviceConfig->timscTimer3Cfg);
        retval |= (uint32_t) retStatus;
    }

    if (NULL != deviceConfig->timscTimer4Cfg)
    {
        retStatus = UJA113X_SetTimer(instance, deviceConfig->timscTimer4Cfg);
        retval |= (uint32_t) retStatus;
    }

    retStatus = UJA113X_SetFailSafe(instance, &deviceConfig->fscCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetRegulator(instance, &deviceConfig->rcRegulatorCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBatteryMonitor(instance, &deviceConfig->bmcBatMonitorCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetSmps(instance, &deviceConfig->smpscSmpsCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetSupplyInterrupts(instance, deviceConfig->supieMask);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetCan(instance, &deviceConfig->cancCanCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetLin(instance, &deviceConfig->linxLinCtrl);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetCanPartNet(instance, &deviceConfig->cpncCanPartNetCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetDataMasks(instance, &deviceConfig->dmCanDataMaskCfg);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetTransceiverInterrupts(instance, deviceConfig->tieMask);
    retval |= (uint32_t) retStatus;

    if (NULL != deviceConfig->hvioBank0Cfg)
    {
        retStatus = UJA113X_SetHvio(instance,
                                             &deviceConfig->hvioBank0Cfg->hvioConfig[0U],
                                             0U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetHvio(instance,
                                             &deviceConfig->hvioBank0Cfg->hvioConfig[1U],
                                             1U);
        retval |= (uint32_t) retStatus;
    }

    retStatus = UJA113X_SetHvio(instance,
                                &deviceConfig->hvioBank0Cfg->hvioConfig[2U],
                                2U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetHvio(instance,
                                &deviceConfig->hvioBank0Cfg->hvioConfig[3U],
                                3U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBankWupThr(instance,
                                deviceConfig->hvioBank0Cfg->bankWupThr,
                                0U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBankOpenLoadThr(instance,
                                &deviceConfig->hvioBank0Cfg->boldtBankOpenLoad,
                                0U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBankShortCircThr(instance,
                                &deviceConfig->hvioBank0Cfg->bscdtBankShortCirc,
                                0U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBankWupInterrupts(instance,
                                deviceConfig->hvioBank0Cfg->bankWupIntMask,
                                0U);
    retval |= (uint32_t) retStatus;

    retStatus = UJA113X_SetBankFailInterrupts(instance,
                                deviceConfig->hvioBank0Cfg->bankFailIntMask,
                                0U);
    retval |= (uint32_t) retStatus;

    if (NULL != deviceConfig->hvioBank1Cfg)
    {
        retStatus = UJA113X_SetHvio(instance,
                                    &deviceConfig->hvioBank1Cfg->hvioConfig[0U],
                                    4U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetHvio(instance,
                                    &deviceConfig->hvioBank1Cfg->hvioConfig[1U],
                                    5U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetHvio(instance,
                                    &deviceConfig->hvioBank1Cfg->hvioConfig[2U],
                                    6U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetHvio(instance,
                                    &deviceConfig->hvioBank1Cfg->hvioConfig[3U],
                                    7U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetBankWupThr(instance,
                                        deviceConfig->hvioBank1Cfg->bankWupThr,
                                        1U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetBankOpenLoadThr(instance,
                                                &deviceConfig->hvioBank1Cfg->boldtBankOpenLoad,
                                                1U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetBankShortCircThr(instance,
                                                &deviceConfig->hvioBank1Cfg->bscdtBankShortCirc,
                                                1U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetBankWupInterrupts(instance,
                                                deviceConfig->hvioBank1Cfg->bankWupIntMask,
                                                1U);
        retval |= (uint32_t) retStatus;

        retStatus = UJA113X_SetBankFailInterrupts(instance,
                                                deviceConfig->hvioBank1Cfg->bankFailIntMask,
                                                1U);
        retval |= (uint32_t) retStatus;
    }

    retStatus = UJA113X_SetWatchdog(instance, &deviceConfig->wdcWatchdogCfg);
    retval |= (uint32_t) retStatus;


    if (NULL != deviceConfig->lckcLockRegCfg)
    {
        retStatus = UJA113X_SetLock(instance, deviceConfig->lckcLockRegCfg);
        retval |= (uint32_t) retStatus;
    }

    retStatus = UJA113X_SetMode(instance, deviceConfig->mcSbcMode);
    retval |= (uint32_t) retStatus;

	if ((retval & (uint32_t)SBC_CMD_ERROR) == (uint32_t)SBC_CMD_ERROR)
	{
		retval = SBC_CMD_ERROR;
	}
	if ((retval & (uint32_t)SBC_ERR_NA) == (uint32_t)SBC_ERR_NA)
	{
		retval = SBC_ERR_NA;
	}
    return (status_t) retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_RegisterDevice_Activity
 *END************************************************************************/
status_t UJA113X_RegisterDevice(uint32_t instance,
                                const uja113x_drv_cfg_t * drvConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != drvConfig);

    g_drivers[instance].chipType      = drvConfig->chipType;
    g_drivers[instance].lpspiInstance = drvConfig->lpspiInstance;
    g_drivers[instance].timeout       = drvConfig->timeout;
    retval = STATUS_SUCCESS;

    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetWatchdog_Activity
 *END************************************************************************/
status_t UJA113X_SetWatchdog(uint32_t instance,
                             const uja113x_wtdc_cfg_t * wtdcConfig)
{
    status_t retval = SBC_ERR_NA; /* set SBC error to SBC Not Available */

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != wtdcConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = WTDC_ADDRESS;
    txData[1U] = (uint8_t)((uint8_t) wtdcConfig->wtdModeCtrl | 
                           (uint8_t) wtdcConfig->nomWtdPeriod);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetMode_Activity
 *END************************************************************************/
status_t UJA113X_SetMode(uint32_t instance,
                         uja113x_mc_mc_t modeConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = MC_ADDRESS;
    txData[1U] = (uint8_t) modeConfig;

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetFailSafe_Activity
 *END************************************************************************/
status_t UJA113X_SetFailSafe(uint32_t instance,
                             const uja113x_fsc_cfg_t * fscConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != fscConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = FSC_ADDRESS;
    txData[1U] = (uint8_t)((uint8_t) fscConfig->enShutdownCtrl       | 
                           ((uint8_t)fscConfig->enDriverCtrl << 5U)  |
                           (uint8_t) fscConfig->enCtrl               |
                           ((uint8_t)fscConfig->limpHomeCtrl << 2U)  |
                           (uint8_t) fscConfig->resetCntCtrl);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetSystemInterrupts_Activity
 *END************************************************************************/
status_t UJA113X_SetSystemInterrupts(uint32_t instance,
                                     uint8_t sysieMask)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = SYSIE_ADDRESS;
    txData[1U] = (uint8_t)(sysieMask & 0x06U);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetLock_Activity
 *END************************************************************************/
status_t UJA113X_SetLock(uint32_t instance,
                         const uja113x_lckc_cfg_t * lckcConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != lckcConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = LCKC_ADDRESS;
    txData[1U] = (uint8_t)((uint8_t) lckcConfig->lock0 |
                          ((uint8_t) lckcConfig->lock1 << 1U) |
                          ((uint8_t) lckcConfig->lock2 << 2U) |
                          ((uint8_t) lckcConfig->lock3 << 3U) |
                          ((uint8_t) lckcConfig->lock4 << 4U) |
                          ((uint8_t) lckcConfig->lock5 << 5U) |
                          ((uint8_t) lckcConfig->lock6 << 6U));

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetRegulator_Activity
 *END************************************************************************/
status_t UJA113X_SetRegulator(uint32_t instance,
                              const uja113x_rc_cfg_t * rcConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != rcConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = RC_ADDRESS;
    txData[1U] = (uint8_t)((uint8_t) rcConfig->v2ShutdownCtrl |
                           (uint8_t) rcConfig->v2Ctrl         |
                           (uint8_t) rcConfig->v1RstThrCtrl);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBatteryMonitor_Activity
 *END************************************************************************/
status_t UJA113X_SetBatteryMonitor(uint32_t instance,
                                   const uja113x_bmc_cfg_t * bmcConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != bmcConfig);

    /* Prepare the 1. frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_32];
    uint8_t rxData[SPI_FRAME_LEN_32] = {0U};

    txData[0U] = BMETSC_ADDRESS;
    txData[1U] = (uint8_t) bmcConfig->bmSrcCtrl;
    txData[2U] = (uint8_t) bmcConfig->bmUndrVoltThr;
    txData[3U] = (uint8_t) bmcConfig->bmOverVoltThr;

    /* 1. transfer out of 2 */
    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_32);

    if (STATUS_SUCCESS == retval)
    {
        /* Prepare the 2. frame to transfer */
        txData[0U] = BMHC_ADDRESS;
        txData[1U] = (uint8_t)((uint8_t) (bmcConfig->bmOverVoltThrRel << 4U) |
                               (uint8_t) (bmcConfig->bmUndrVoltThrRel & 0x0FU));

        /* 2. transfer out of 2 */
        retval = UJA113X_SbcTransfer(instance,
                                     false,
                                     txData, rxData,
                                     SPI_FRAME_LEN_16);
    }

    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetSmps_Activity
 *END************************************************************************/
status_t UJA113X_SetSmps(uint32_t instance,
                         const uja113x_smpsc_cfg_t * smpscConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != smpscConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_24];
    uint8_t rxData[SPI_FRAME_LEN_24] = {0U};

    txData[0U] = SMPSC_ADDRESS;
    txData[1U] = (uint8_t)(((uint8_t) smpscConfig->smpsOvertempCtrl << 3U) |
                           (uint8_t) smpscConfig->smpsCtrl);
    txData[2U] = (uint8_t) smpscConfig->smpsOutVolt;


    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_24);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetSupplyInterrupts_Activity
 *END************************************************************************/
status_t UJA113X_SetSupplyInterrupts(uint32_t instance,
                                     uint8_t supieMask)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = SUPIE_ADDRESS;
    txData[1U] = (uint8_t)(supieMask & 0x3FU);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetCan_Activity
 *END************************************************************************/
status_t UJA113X_SetCan(uint32_t instance,
                        const uja113x_canc_cfg_t * cancConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != cancConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = CANC_ADDRESS;
    txData[1U] = (uint8_t)(((uint8_t) cancConfig->canFDCtrl << 6U)      |
                           ((uint8_t) cancConfig->canPartNetCtrl << 4U) |
                            (uint8_t) cancConfig->canShutdownCtrl       |
                            (uint8_t) cancConfig->canModeCtrl);

    /* Set PNCOK bit if CAN partial networking is requested */
    if (true == cancConfig->canPartNetCtrl)
    {
        txData[1U] |= (uint8_t)(1U << 5U);
    }

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetLin_Activity
 *END************************************************************************/
status_t UJA113X_SetLin(uint32_t instance,
                        const uja113x_linc_cfg_t * lincConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != lincConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = LINC_ADDRESS;
    txData[1U] = (uint8_t)((uint8_t) lincConfig->lin2SlopeCtrl |
                           (uint8_t) lincConfig->lin2ModeCtrl  |
                           (uint8_t) lincConfig->lin1SlopeCtrl |
                           (uint8_t) lincConfig->lin1ModeCtrl);

    /*
     * For devices incorporating only LIN 1 clear LIN2 parameters. This is 
     * mentioned in the SBC reference manual (See table 27. LIN control 
     * register (21h); remark [1]), that these bits are reserved 
     * and should be left cleared in comparison to other registers, which 
     * are only reserved!
     */
    if ((UJA1131HW5V0 == g_drivers[instance].chipType) ||
        (UJA1131HW3V3 == g_drivers[instance].chipType) ||
        (UJA1135HW5V0 == g_drivers[instance].chipType) ||
        (UJA1135HW3V3 == g_drivers[instance].chipType) ||
        (UJA1131HWFD5V4 == g_drivers[instance].chipType) ||
        (UJA1131HWFD3V4 == g_drivers[instance].chipType) ||
        (UJA1131HWFD5V0 == g_drivers[instance].chipType) ||
        (UJA1131HWFD3V0 == g_drivers[instance].chipType))
    {
        txData[1U] &= 0x0FU;
    }

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetTransceiverInterrupts_Activity
 *END************************************************************************/
status_t UJA113X_SetTransceiverInterrupts(uint32_t instance,
                                          uint8_t tieMask)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    txData[0U] = TIE_ADDRESS;
    txData[1U] = (uint8_t)(tieMask & 0x1FU);

    retval = UJA113X_SbcTransfer(instance,
                                 false,
                                 txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetCanPartNet_Activity
 *END************************************************************************/
status_t UJA113X_SetCanPartNet(uint32_t instance,
                               const uja113x_cpnc_cfg_t * cpncConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != cpncConfig);

    /* Prepare the 1. & 2. frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_128];
    uint8_t rxData[SPI_FRAME_LEN_32] = {0U};

    txData[0U] = DR_ADDRESS;
    txData[4U] = ID2_ADDRESS;
    txData[8U] = IDM1_ADDRESS;

    txData[1U] = (uint8_t) cpncConfig->canDataRate;

    if (false == cpncConfig->ide)
    {
        /* Standard frame format */
        txData[2U] = 0x00U;
        txData[3U] = 0x00U;
        txData[5U] = (uint8_t)((cpncConfig->id & 0x0000003FU) << 2U);
        txData[6U] = (uint8_t)((cpncConfig->id & 0x000007C0U) >> 6U);

        txData[7U]  = 0x00U;
        txData[9U]  = 0x00U;
        txData[10U] = (uint8_t)((cpncConfig->idMask & 0x0000003FU) << 2U);
        txData[11U] = (uint8_t)((cpncConfig->idMask & 0x000007C0U) >> 6U);
    }
    else
    {
        /* Extended frame format */
        txData[2U] = (uint8_t) cpncConfig->id;
        txData[3U] = (uint8_t)(cpncConfig->id >> 8U);
        txData[5U] = (uint8_t)(cpncConfig->id >> 16U);
        txData[6U] = (uint8_t)((cpncConfig->id & 0x1F000000U) >> 24U);

        txData[7U]  = (uint8_t) cpncConfig->idMask;
        txData[9U]  = (uint8_t)(cpncConfig->idMask >> 8U);
        txData[10U] = (uint8_t)(cpncConfig->idMask >> 16U);
        txData[11U] = (uint8_t)((cpncConfig->idMask & 0x1F000000U) >> 24U);
    }

    /* Transfer 1. frame */
    retval = UJA113X_SbcTransfer(instance, false, &txData[0U], rxData,
                                 SPI_FRAME_LEN_32);

    /* Transfer 2. frame */
    if (STATUS_SUCCESS == retval)
    {
        retval = UJA113X_SbcTransfer(instance, false, &txData[4U], rxData,
                                     SPI_FRAME_LEN_32);
    }

    /* Transfer 3. frame */
    if (STATUS_SUCCESS == retval)
    {
        retval = UJA113X_SbcTransfer(instance, false, &txData[8U], rxData,
                                     SPI_FRAME_LEN_32);
    }

    if (STATUS_SUCCESS == retval)
    {
        /* Prepare 4. frame */
        txData[0U] = FC_ADDRESS;
        txData[1U] = (uint8_t)(((uint8_t) cpncConfig->ide << 7U)             |
                               ((uint8_t) cpncConfig->partNetDataMask << 6U) |
                                (uint8_t) cpncConfig->dataLenCode);

        retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                     SPI_FRAME_LEN_16);
    }

    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetDataMasks_Activity
 *END************************************************************************/
status_t UJA113X_SetDataMasks(uint32_t instance,
                              const uja113x_dm_cfg_t * dmConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != dmConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_32];
    uint8_t rxData[SPI_FRAME_LEN_32] = {0U};

    txData[0U] = DM0_ADDRESS;
    txData[1U] = dmConfig->dm0;
    txData[2U] = dmConfig->dm1;
    txData[3U] = dmConfig->dm2;

    /* Transfer 1. frame */
    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_32);
    if (STATUS_SUCCESS == retval)
    {

        txData[0U] = DM3_ADDRESS;
        txData[1U] = dmConfig->dm3;
        txData[2U] = dmConfig->dm4;
        txData[3U] = dmConfig->dm5;

        /* Transfer 2. frame */
        retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                     SPI_FRAME_LEN_32);
    }

    if (STATUS_SUCCESS == retval)
    {
        txData[0U] = DM6_ADDRESS;
        txData[1U] = dmConfig->dm6;
        txData[2U] = dmConfig->dm7;

        /* Transfer 3. frame */
        retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                     SPI_FRAME_LEN_24);
    }

    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetHvio_Activity
 *END************************************************************************/
status_t UJA113X_SetHvio(uint32_t instance,
                         const uja113x_hvio_cfg_t * hvioConfig,
                         uint8_t hvioId)
{
    status_t retval = SBC_ERR_NA; /* set SBC error to SBC Not Available */
    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != hvioConfig);
    DEV_ASSERT(hvioId   < HVIO_MAX_CHANNELS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};
    uint8_t bankIdx;

    /* Adjust the address based on hvioId index:
     * HVIO1 - 4 = 0x30 - 0x33
     * HVIO5 - 8 = 0x40 - 0x43
     */
    bankIdx = (uint8_t)(hvioId >> 2U); /* Divide by 4 */
    if (0U != bankIdx)
    {
        hvioId -= 4U;
    }

    txData[0U] = (uint8_t)(HVIO1C_ADDRESS + (uint8_t)(((0x10U * bankIdx) + hvioId) << 1U));
    txData[1U] = (uint8_t)((uint8_t) hvioConfig->ionShutdownCtrl |
                           (uint8_t) hvioConfig->ionActivateCtrl |
                           (uint8_t) hvioConfig->ionConfCtrl);

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBankWupThr_Activity
 *END************************************************************************/
status_t UJA113X_SetBankWupThr(uint32_t instance,
                               uja113x_bwtc_bnwtc_t bankWupThrConfig,
                               uint8_t bankId)
{
    status_t retval = SBC_ERR_NA; /* set SBC error to SBC Not Available */

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(bankId < HVIO_MAX_BANKS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    /* Adjust the address based on bank index:
     * Bank0: HVIO1 - 4 (0x34)
     * Bank1: HVIO5 - 8 (0x44)
     */
    txData[0U] = (uint8_t)(B0WUPTHRC_ADDRESS + (uint8_t)((0x10U * bankId) << 1U));
    txData[1U] = (uint8_t) bankWupThrConfig;

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBankWupInterrupts_Activity
 *END************************************************************************/
status_t UJA113X_SetBankWupInterrupts(uint32_t instance,
                                      uint8_t bankWupIntMask,
                                      uint8_t bankId)
{
    status_t retval = SBC_ERR_NA;
    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(bankId < HVIO_MAX_BANKS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    /* Adjust the address based on bank index:
     * Bank0: HVIO1 - 4 (0x37)
     * Bank1: HVIO5 - 8 (0x47)
     */
    txData[0U] = (uint8_t)(B0WUPIE_ADDRESS + (uint8_t)((0x10U * bankId) << 1U));
    txData[1U] = bankWupIntMask;

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBankFailInterrupts_Activity
 *END************************************************************************/
status_t UJA113X_SetBankFailInterrupts(uint32_t instance,
                                       uint8_t bankFailIntMask,
                                       uint8_t bankId)
{
    status_t retval = SBC_ERR_NA;
    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(bankId   < HVIO_MAX_BANKS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    /* Adjust the address based on bank index:
     * Bank0: HVIO1 - 4 (0x38)
     * Bank1: HVIO5 - 8 (0x48)
     */
    txData[0U] = (uint8_t)(B0FIE_ADDRESS + (uint8_t)((0x10U * bankId) << 1U));
    txData[1U] = bankFailIntMask;

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBankShortCircThr_Activity
 *END************************************************************************/
status_t UJA113X_SetBankShortCircThr(uint32_t instance,
                                     const uja113x_bscdt_cfg_t * bscdtConfig,
                                     uint8_t bankId)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != bscdtConfig);
    DEV_ASSERT(bankId   < HVIO_MAX_BANKS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    /* Adjust the address based on bank index:
     * Bank0: HVIO1 - 4 (0x39)
     * Bank1: HVIO5 - 8 (0x49)
     */
    txData[0U] = (uint8_t)(B0SCDTC_ADDRESS + (uint8_t)((0x10U * bankId) << 1U));
    txData[1U] = (uint8_t)((uint8_t) ((uint8_t) bscdtConfig->io48ShortCircuitThrCtrl << 6U) |
                           (uint8_t) ((uint8_t) bscdtConfig->io37ShortCircuitThrCtrl << 4U) |
                           (uint8_t) ((uint8_t) bscdtConfig->io26ShortCircuitThrCtrl << 2U) |
                           (uint8_t) bscdtConfig->io15ShortCircuitThrCtrl);

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetBankOpenLoadThr_Activity
 *END************************************************************************/
status_t UJA113X_SetBankOpenLoadThr(uint32_t instance,
                                    const uja113x_boldt_cfg_t * boldtConfig,
                                    uint8_t bankId)
{
    status_t retval = SBC_ERR_NA;
    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(NULL != boldtConfig);
    DEV_ASSERT(bankId   < HVIO_MAX_BANKS);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_16];
    uint8_t rxData[SPI_FRAME_LEN_16] = {0U};

    /* Adjust the address based on bank index:
     * Bank0: HVIO1 - 4 (0x3A)
     * Bank1: HVIO5 - 8 (0x4A)
     */
    txData[0U] = (uint8_t)(B0OLDTC_ADDRESS + (uint8_t)((0x10U * bankId) << 1U));
    txData[1U] = (uint8_t)((uint8_t) ((uint8_t) boldtConfig->io48OpenLoadThrCtrl << 6U) |
                           (uint8_t) ((uint8_t) boldtConfig->io37OpenLoadThrCtrl << 4U) |
                           (uint8_t) ((uint8_t) boldtConfig->io26OpenLoadThrCtrl << 2U) |
                           (uint8_t) boldtConfig->io15OpenLoadThrCtrl);

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_16);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetTimer_Activity
 *END************************************************************************/
status_t UJA113X_SetTimer(uint32_t instance,
                          const uja113x_timsc_cfg_t * timscConfig)
{
    status_t retval = SBC_ERR_NA;

    DEV_ASSERT(instance < DRIVER_COUNT__);
    DEV_ASSERT(timscConfig->nTimer < 4U);
    DEV_ASSERT(NULL != timscConfig);

    /* Prepare the frame to transfer */
    uint8_t txData[SPI_FRAME_LEN_24];
    uint8_t rxData[SPI_FRAME_LEN_24] = {0U};
    uint8_t mask = 0x03U; /* Default mask for timer 2, 3, 4 */

    if (0U == timscConfig->nTimer)
    {
        mask = 0x01U; /* Mask for timer 1 */
    }

    txData[0U] = (uint8_t)(TIM1C_ADDRESS + (uint8_t)((2U * timscConfig->nTimer) << 1U));
    txData[1U] = (uint8_t)(((uint8_t) timscConfig->timXPeriodCtrl) |
                           ((uint8_t) timscConfig->timXModeCtrl & mask));
    txData[2U] = timscConfig->timXDutyCycleCtrl;

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_24);
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_SetMtpnv_Activity
 *END************************************************************************/
status_t UJA113X_SetMtpnv(uint32_t instance,
                          const uja113x_mtpnv_cfg_t * mtpnvConfig)
{
    status_t retval = SBC_ERR_NA;
    
    DEV_ASSERT(instance < DRIVER_COUNT__);
 
    uint8_t txData[SPI_FRAME_LEN_64] = {0U};
    uint8_t rxData[SPI_FRAME_LEN_32] = {0U};
    crc_spec_t crcDesc;

    /* Prepare frame 1. and write it into SBC */
    txData[0U] = HVIOHSDRVC_ADDRESS;
    txData[1U] = mtpnvConfig->highSideCtrl;
    txData[2U] = mtpnvConfig->lowSideCtrl;
    txData[3U] = mtpnvConfig->startupCtrl;
    txData[4U] = mtpnvConfig->sbcConfigCtrl;

    /* HVIO1 - 4 devices */
    if ((UJA1131HWFD5V4 == g_drivers[instance].chipType) ||
        (UJA1131HWFD3V4 == g_drivers[instance].chipType) ||
        (UJA1132HWFD5V4 == g_drivers[instance].chipType) ||
        (UJA1132HWFD3V4 == g_drivers[instance].chipType))
    {
        txData[1U] &= 0x0FU;
        txData[2U] &= 0x0FU;
    }
    /* None HVIO functionality available */
    else if ((UJA1131HWFD5V0 == g_drivers[instance].chipType) ||
             (UJA1131HWFD3V0 == g_drivers[instance].chipType) ||
             (UJA1132HWFD5V0 == g_drivers[instance].chipType) ||
             (UJA1132HWFD3V0 == g_drivers[instance].chipType))
    {
        txData[1U] = 0x00U;
        txData[2U] = 0x00U;
    }
    else
    {
        /* No need to mask something here, these devices have HVIO's */
    }

    retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                 SPI_FRAME_LEN_32);
    if (STATUS_SUCCESS == retval)
    {
        /* Prepare frame 2 (with CRC) and write it into SBC */
        txData[0U] = SBCCC_ADDRESS;
        crcDesc.poly    = 0x2FU;
        crcDesc.init    = 0xFFU;
        crcDesc.xorOut  = 0xFFU;

        txData[2U] = UJA113X_ComputeCrc8(&crcDesc, &txData[1U], 4U);
        txData[1U] = txData[4U];

        retval = UJA113X_SbcTransfer(instance, false, txData, rxData,
                                     SPI_FRAME_LEN_24);
    }
    return retval;
}

/*FUNCTION********************************************************************
 * Implements : UJA113X_ComputeCrc8_Activity
 *END************************************************************************/
uint8_t UJA113X_ComputeCrc8(const crc_spec_t * crcDesc,
                            const uint8_t data[],
                            uint8_t size)
{
    uint8_t crc = crcDesc->init;
    uint32_t i = 0U;
    uint8_t  j = 0U;

    for (i = 0U; i < size; i++)
    {
        crc ^= data[i];
        for (j = 0U; j < 8U; j++)
        {
            if ((crc & 0x80U) != 0U)
            {
                crc = (uint8_t)(crc << 1U);
                crc = (uint8_t)(crc ^ crcDesc->poly);
            }
            else
            {
                crc = (uint8_t)(crc << 1U);
            }
        }
    }

    return (uint8_t)(crc ^ crcDesc->xorOut);
}
