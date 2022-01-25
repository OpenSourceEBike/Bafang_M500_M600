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
 * @sbc_fs45_driver.c
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
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined
 * This is required to allow the use of a macro for shifting and masking
 * uint8_t type values only.
 * 
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially Boolean' type to 'essentially unsigned'.This is required by the
 * conversion of a bit-field of a bool type into a bit-field of a register type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "sbc_fs45_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FS45_SPI_FRAME_SIZE_BYTES      0x02U /* Length of the communication frame (bytes). */
#define FS45_REG_ADDR_MASK             0x3FU /* Register address mask (not shifted) */
#define FS45_REG_ADDR_SHIFT            0x01U /* SPI register address shift */
#define FS45_SPI_BIT_15_8              1U    /* High byte. */
#define FS45_SPI_BIT_7_0               0U    /* Low byte. */
#define FS45_SPI_RW_BIT_MASK           0x80U /* R/W bit mask. */
#define FS45_SPI_W_BIT_VALUE           0x80U /* R/W bit - write. */
#define FS45_SPI_R_BIT_VALUE           0x00U /* R/W bit - read. */
#define FS45_SPI_DEVICE_STATUS_EX_MASK 0xF0U /* High 4 bits. */
#define FS45_SECURE_DATA_READ_MASK     0x0FU /* Low 4 bits. */
#define FS45_SECURE_REGS_COUNT         14U   /* Number of secured registers. */
#define FS45_SECURE_WRITE_SHIFT        4U    /* Shift of the register write value for secured bits. */

/** @brief This macro updates value of bits specified by the mask. It is assumed that
 *  value is already shifted. Write value is masked also. */
#define FS45_BO_SETVAL(data, val, mask)   ((uint8_t)(((uint8_t)data) & ~((uint8_t)mask)) | (uint8_t)(((uint8_t)val) & ((uint8_t)mask)))

/** @brief This macro updates value of bits specified by the mask. Additionally range
 *  check on the value is performed. It is assumed that value is not shifted. */
#define FS45_BO_SETVAL_EXT(data, value, mask, shift) \
    (((data) & ~(mask << shift)) | (((value) & (mask)) << (shift)))

/** @brief This macro returns value from register. */
#define FS45_BO_GETVAL(value, mask, shift) (((value) & (mask)) >> (shift))

/**
 * @brief Returns true if register is fail-safe
 *
 * @param address   Register address.
 * @return true if register is fail-safe.
 */
#define FS45_IS_REG_FAILSAFE(address) (((uint16_t)address) & (uint16_t)0x20)

/** @brief Command type. */
typedef enum
{
    fs45RegRead,   /**< @brief Register Read */
    fs45RegWrite   /**< @brief Register Write */
} fs45_command_t;

/** @brief Parity type. */
typedef enum
{
    fs45ParityOdd, /**< @brief Number of 1s is odd. */
    fs45ParityEven /**< @brief Number of 1s is even. */
} fs45_parity_t;

/** @brief Structure representing transmit data frame. */
typedef struct
{
    /** @brief true if the SPI command is secured. */
    bool isSecured;
    /** @brief Register address. */
    uint8_t registerAddress;
    /** @brief Command type (R/W). */
    fs45_command_t commandType;
    /** @brief Data to be written to the register.
     *         If commandType is "read", this value will be ignored.
     */
    uint8_t writeData;
} fs45_tx_data_t;

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Local Functions Prototypes
 ******************************************************************************/
/**
 * @brief Returns true if register SPI transfer is secured by security bits.
 *
 * @param address Register address.
 * @return true if register SPI transfer is secured.
 */
static inline bool FS45_IsRegisterSecured(uint8_t address);

/**
 * @brief This function calculates parity value of passed data array.
 *        Returns 1 if the parity is even, 0 otherwise.
 *
 * @param data Computes parity of input data.
 * @return Parity.
 */
static fs45_parity_t FS45_ComputeParity(uint16_t data);

/**
 * @brief Performs SPI transfer of the txData. Received frame is saved into
 *        rxData structure or ignored if rxData is NULL.
 *
 * @param txData Data to be transmitter (MOSI).
 * @param rxData Data received (MISO).
 * @return @ref status_t Status return code.
 */
static status_t FS45_TransferData(const fs45_tx_data_t* txData, fs45_rx_data_t* rxData);

/**
 * @brief Creates a raw frame for SPI transfer.
 *
 * @param txData tx data.
 * @param txFrame Raw frame prepared for SPI transmission (MOSI).
 */
static void FS45_CreateSendFrame(const fs45_tx_data_t* txData, uint8_t* txFrame);

/**
 * @brief Writes set of configuration registers values.
 *
 * If registers is NULL or numOfItems is 0, function just returns OK status code.
 *
 * @param [in] registers Pointer to array of register configuration values.
 * @param [in] numOfItems Number of items.
 *
 * @return @ref status_t Status return code.
 */
static status_t FS45_WriteRegisters(const fs45_reg_config_value_t* registers, uint8_t numOfItems);

/**
 * @brief Computes security bits from 4 MSbits and write it to 4 LSbits.
 *
 * @param [in, out] data
 */
static void FS45_ComputeSecurityBits(uint8_t* data);

/**
 * @brief Reads challenge token (next generated LFSR state) from the SBC.
 *
 * @param [out] wdSeed Challenge token.
 * @return Status result of the function.
 */
static status_t FS45_WD_ReadChallengeToken(uint8_t* wdSeed);

/**
 * @brief Compute Challenger Watchdog answer.
 *
 * @param token Actual LFSR value.
 * @return Computed watchdog answer.
 */
static uint8_t FS45_WD_ComputeAnswer(uint8_t token);

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

static uint32_t g_spiInstance;
static uint32_t g_spiTimeout = 5U;
static bool g_fs1bEnabled = false;

/*******************************************************************************
 * Local Functions - Implementation
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_IsRegisterSecured
 * Description : Returns true if register SPI transfer is secured by security bits.
 *
 *END**************************************************************************/
static inline bool FS45_IsRegisterSecured(uint8_t address)
{
    uint8_t i; /* Counter. */

    static const uint8_t m_securedRegisters[] = {
        FS45_M_MODE_ADDR,
        FS45_M_REG_MODE_ADDR,
        FS45_FS_INIT_FS1B_TIMING_ADDR,
        FS45_FS_BIST_ADDR,
        FS45_FS_INIT_SUPERVISOR_ADDR,
        FS45_FS_INIT_FAULT_ADDR,
        FS45_FS_INIT_FSSM_ADDR,
        FS45_FS_INIT_SF_IMPACT_ADDR,
        FS45_FS_WD_WINDOW_ADDR,
        FS45_FS_SF_OUTPUT_REQUEST_ADDR,
        FS45_FS_INIT_WD_CNT_ADDR,
        FS45_FS_INIT_VCORE_OVUV_IMPACT_ADDR,
        FS45_FS_INIT_VCCA_OVUV_IMPACT_ADDR,
        FS45_FS_INIT_VAUX_OVUV_IMPACT_ADDR
    };

    for (i = 0; i < FS45_SECURE_REGS_COUNT; i++)
    {
        if (address == m_securedRegisters[i])
        {
            return true;
        }
    }
    return false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_TransferData
 * Description : Performs SPI transfer of the txData. Received frame is saved into
 *               rxData structure or ignored if rxData is NULL.
 *
 *END**************************************************************************/
static status_t FS45_TransferData(const fs45_tx_data_t* txData, fs45_rx_data_t* rxData)
{
    status_t status;                                  /* Status variable. */
    uint8_t txFrame[FS45_SPI_FRAME_SIZE_BYTES] = {0}; /* SPI Tx frame. */
    uint8_t rxFrame[FS45_SPI_FRAME_SIZE_BYTES] = {0}; /* SPI Rx frame. */

    /* Create frame to send. */
    FS45_CreateSendFrame(txData, txFrame);

    status = LPSPI_DRV_MasterTransferBlocking(g_spiInstance, txFrame, rxFrame, 2U, g_spiTimeout);

    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (rxData != NULL)
    {
        /* Secured fail-safe registers and RELEASE_FSxB register contain extended status. */
        if (((txData->isSecured) && (FS45_IS_REG_FAILSAFE(txData->registerAddress) != 0U)) ||
                (txData->registerAddress == FS45_FS_RELEASE_FSXB_ADDR))
        {
            rxData->deviceStatusEx = rxFrame[FS45_SPI_BIT_7_0] & FS45_SPI_DEVICE_STATUS_EX_MASK;
            rxData->readData = rxFrame[FS45_SPI_BIT_7_0] & FS45_SECURE_DATA_READ_MASK;
        }
        else
        {
            rxData->deviceStatusEx = 0x00U;
            rxData->readData = rxFrame[FS45_SPI_BIT_7_0];
        }
        rxData->deviceStatus = rxFrame[FS45_SPI_BIT_15_8];
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_CreateSendFrame
 * Description : Creates a raw frame for SPI transfer.
 *
 *END**************************************************************************/
static void FS45_CreateSendFrame(const fs45_tx_data_t* txData, uint8_t* txFrame)
{
    fs45_parity_t parity; /* Tx frame parity result. */

    txFrame[FS45_SPI_BIT_7_0] = 0x00U;
    txFrame[FS45_SPI_BIT_15_8] = 0x00U;

    /* Sets address of the register. */
    txFrame[FS45_SPI_BIT_15_8] = FS45_BO_SETVAL_EXT(txFrame[FS45_SPI_BIT_15_8],
            txData->registerAddress, FS45_REG_ADDR_MASK, FS45_REG_ADDR_SHIFT);

    /* Check if register is r or w */
    switch (txData->commandType)
    {
      case fs45RegRead:
          /* Creates read command. */
          txFrame[FS45_SPI_BIT_15_8] = FS45_BO_SETVAL(txFrame[FS45_SPI_BIT_15_8],
                  FS45_SPI_R_BIT_VALUE, FS45_SPI_RW_BIT_MASK);
          /* No data to be sent. */
          txFrame[FS45_SPI_BIT_7_0] = 0x00U;
          break;

      case fs45RegWrite:
          /* Creates write command. */
          txFrame[FS45_SPI_BIT_15_8] |= FS45_BO_SETVAL(txFrame[FS45_SPI_BIT_15_8],
                  FS45_SPI_W_BIT_VALUE, FS45_SPI_RW_BIT_MASK);
          /* Sets data. */
          txFrame[FS45_SPI_BIT_7_0] = txData->writeData;
          if (txData->isSecured)
          {
              FS45_ComputeSecurityBits(&(txFrame[FS45_SPI_BIT_7_0]));
          }
          /* Sets parity bit. */
          parity = FS45_ComputeParity((((uint16_t)(txFrame[FS45_SPI_BIT_15_8]) << 8U)) + (uint16_t)(txFrame[FS45_SPI_BIT_7_0]));
          if (parity == fs45ParityEven)
          {
              txFrame[FS45_SPI_BIT_15_8] = FS45_BO_SETVAL(txFrame[FS45_SPI_BIT_15_8], 1U, 1U);
          }
          break;
      default:
          /* Do nothing. */
          break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WriteRegisters
 * Description : Writes set of configuration registers values.
 *
 *END**************************************************************************/
static status_t FS45_WriteRegisters(const fs45_reg_config_value_t* registers, uint8_t numOfItems)
{
    status_t firstStatus;               /* Status variable. */
    status_t secondStatus;              /* Status variable. */
    fs45_rx_data_t response;            /* Response to the command. */
    uint8_t i;                          /* Counter. */
    const fs45_reg_config_value_t* reg; /* Pointer to actually selected register configuration. */
    uint8_t writeValue;                 /* Register write value. */

    if ((registers == NULL) || (numOfItems == 0U))
    {
        return STATUS_SUCCESS;
    }

    for (i = 0; i < numOfItems; i++)
    {
        reg = &(registers[i]);
        /* Secured registers have the write value shifted and 4 lowest bits
         * are used for the Secured bits. */
        writeValue = FS45_IsRegisterSecured(reg->address) ?
                (uint8_t)(reg->value << FS45_SECURE_WRITE_SHIFT) : reg->value;
        firstStatus = FS45_WriteRegister(reg->address, writeValue, NULL);

        /* In case of write to the fail-safe register, add mandatory delay between
         * consequent transfers (3.5 us according to the specification, 1 ms added). */
        if (FS45_IS_REG_FAILSAFE(reg->address) != 0U)
        {
            OSIF_TimeDelay(1);
        }

        /* Check written register value. */
        secondStatus = FS45_ReadRegister(reg->address, &response);

        /* Read mask should be used for read value as some registers
         * contains status bits, that are not writable (e.g. INIT_VREG - BAT_FAIL bit) */
        if ((firstStatus != STATUS_SUCCESS) || (secondStatus != STATUS_SUCCESS) || ((response.readData & reg->readMask) != (reg->value & reg->readMask)))
        {
            return STATUS_ERROR;
        }
    }
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_ComputeSecurityBits
 * Description : Computes security bits from 4 MSbits and write it to 4 LSbits.
 *
 *END**************************************************************************/
static void FS45_ComputeSecurityBits(uint8_t* data)
{
    /* Set 4 LSbits to 0. */
    *data &= (uint8_t)0xF0U;

    *data |= (((*data & (1U << 6U)) != 0U) ? 1U : 0U); /* Secure 0 = Bit6 */
    *data |= (((*data & (1U << 7U)) != 0U) ? 2U : 0U); /* Secure 1 = Bit7 */
    *data |= (((*data & (1U << 4U)) != 0U) ? 0U : 4U); /* Secure 2 = NOT(Bit4) */
    *data |= (((*data & (1U << 5U)) != 0U) ? 0U : 8U); /* Secure 3 = NOT(Bit5) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_ComputeParity
 * Description : This function calculates parity value of passed data array.
 *               Returns 1 if the parity is even, 0 otherwise.
 *
 *END**************************************************************************/
static fs45_parity_t FS45_ComputeParity(uint16_t data)
{
    uint16_t x = data; /* Data from which the parity will be calculated. */

    x ^= x >> 8U;
    x ^= x >> 4U;
    x ^= x >> 2U;
    x ^= x >> 1U;

    return (((~x) & 1U) != 0U) ? fs45ParityEven : fs45ParityOdd;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WD_ReadChallengeToken
 * Description : Reads challenge token (next generated LFSR state) from the SBC.
 *
 *END**************************************************************************/
static status_t FS45_WD_ReadChallengeToken(uint8_t* wdSeed)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    fs45_rx_data_t response;          /* Response to the command. */

    status = FS45_ReadRegister(FS45_FS_WD_LFSR_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Store the watchdog seed from the SBC device. */
    *wdSeed = response.readData;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WD_ComputeAnswer
 * Description : Compute Challenger Watchdog answer.
 *
 *END**************************************************************************/
static uint8_t FS45_WD_ComputeAnswer(uint8_t token)
{
    uint16_t mr = token; /* Monitoring result. */

    /* Compute challenger watchdog answer. */
    mr *= 4U;
    mr += 6U;
    mr -= 4U;
    mr = ~mr;
    mr /= 4U;

    return (uint8_t)mr;
}

/*******************************************************************************
 * API - Implementation
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_Init
 * Description : This function runs full initialization of the SBC device.
 *
 * Implements : FS45_Init_Activity
 *
 *END**************************************************************************/
status_t FS45_Init(const fs45_drv_config_t* drvConfig, uint32_t lpspiInstance, uint32_t lpspiTimeoutMs)
{
    status_t status;             /* Status variable. */
    uint32_t statusInt = 0U;     /* Status variable. */
    fs45_rx_data_t response;     /* Response to the command. */
    fs45_current_mode_t sbcMode; /* Actual SBC mode. */
    fs45_prev_mode_t prevMode;   /* Previous SBC mode. */
    fs45_reg_config_value_t initMainRegs[] =
    {
            { FS45_M_INIT_VREG_ADDR, 0x00U, 0xF6U },
            { FS45_M_INIT_WU1_ADDR, 0x00U, 0xFFU },
            { FS45_M_INIT_WU2_ADDR, 0x00U, 0xF7U },
            { FS45_M_INIT_INH_INT_ADDR, 0x00U, 0x1FU }
    };
    fs45_reg_config_value_t initFailSafeRegs[] =
    {
            { FS45_FS_INIT_FS1B_TIMING_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_SUPERVISOR_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_FAULT_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_FSSM_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_SF_IMPACT_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_WD_CNT_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_VCORE_OVUV_IMPACT_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_VCCA_OVUV_IMPACT_ADDR, 0x00U, 0x0FU },
            { FS45_FS_INIT_VAUX_OVUV_IMPACT_ADDR, 0x00U, 0x0FU }
    };
    uint8_t initIntValue;

    DEV_ASSERT(drvConfig != NULL);

    g_spiInstance = lpspiInstance;
    g_spiTimeout = lpspiTimeoutMs;

    /* Check if FS1B is implemented. */
    status = FS45_ReadRegister(FS45_FS_DEVICE_ID_FS_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    
    /* Store to global variable. */
    g_fs1bEnabled = (((response.readData & FS45_R_FS_FS1_MASK) >> FS45_R_FS_FS1_SHIFT) != 0U) ? true : false;

    /* Check built-in self-test. */
    status = FS45_CheckLbistAbistOk();
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check if FS1B is at low (if implemented). */
    if (g_fs1bEnabled == true)
    {
        status = FS45_CheckFS1B();
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* Determine if it is necessary to re-initialize main register set. */
    status = FS45_GetMode(&sbcMode, &prevMode);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* MSM does not have to be initialized in case of transition
     * LPOFF -> NORMAL. */
    if ((sbcMode == fs45ModeInit) && (prevMode != fs45PrevModeLPOFF))
    {
        /* Initialize main registers:
         * - INIT_VREG
         * - INIT_WU1
         * - INIT_WU2
         * - INIT_INH_INT
         */
        initMainRegs[0].value = drvConfig->vregReg.iccaLim |
                drvConfig->vregReg.tccaLimOff |
                drvConfig->vregReg.ipffDis |
                drvConfig->vregReg.vcanOvMon |
                drvConfig->vregReg.tauxLimOff |
                drvConfig->vregReg.vauxTrkEn;
        initMainRegs[1].value = drvConfig->wu1Reg.wuIo0 |
                drvConfig->wu1Reg.wuIo2 |
                drvConfig->wu1Reg.wuIo3 |
                drvConfig->wu1Reg.wuIo4;
        initMainRegs[2].value = drvConfig->wu2Reg.wuIo5 |
                drvConfig->wu2Reg.canDisCfg |
                drvConfig->wu2Reg.canWuTo |
                drvConfig->wu2Reg.linJ2602Dis |
                drvConfig->wu2Reg.linSr;
        initMainRegs[3].value = drvConfig->inhIntReg.inh5 |
                drvConfig->inhIntReg.inh4 |
                drvConfig->inhIntReg.inh3 |
                drvConfig->inhIntReg.inh2 |
                drvConfig->inhIntReg.inh0;

        status = FS45_WriteRegisters(initMainRegs, 4U);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* FSSM can be initialized only in INIT_FS. */
    if (sbcMode == fs45ModeInit)
    {
        /* Initialize FSSM (fail-safe registers):
         * - INIT_FS1B_TIMING
         * - INIT_SUPERVISOR
         * - INIT_FAULT
         * - INIT_FSSM
         * - INIT_SF_IMPACT
         * - INIT_WD_CNT
         * - INIT_VCORE_OVUV_IMPACT
         * - INIT_VCCA_OVUV_IMPACT
         * - INIT_VAUX_OVUV_IMPACT
         */
        initFailSafeRegs[0].value = drvConfig->fs1bTimingReg.fs1bTime;
        initFailSafeRegs[1].value = drvConfig->supervisorReg.vcore5d |
                drvConfig->supervisorReg.vcca5d |
                drvConfig->supervisorReg.vaux5d |
                drvConfig->supervisorReg.fs1bTimeRange;
        initFailSafeRegs[2].value = drvConfig->faultReg.fltErrFs |
                drvConfig->faultReg.fs1bCanImpact |
                drvConfig->faultReg.fltErrImp;
        initFailSafeRegs[3].value = drvConfig->fssmReg.io45Fs |
                drvConfig->fssmReg.io23Fs |
                drvConfig->fssmReg.ps |
                drvConfig->fssmReg.rstbDuration;
        initFailSafeRegs[4].value = drvConfig->sfImpactReg.tdlyTdur |
                drvConfig->sfImpactReg.dis8s |
                drvConfig->sfImpactReg.wdImpact;
        initFailSafeRegs[5].value = drvConfig->wdCntReg.wdCntErr |
                drvConfig->wdCntReg.wdCntRfr;
        initFailSafeRegs[6].value = drvConfig->vcoreOvuvImpactReg.vcoreFsOv |
                drvConfig->vcoreOvuvImpactReg.vcoreFsUv;
        initFailSafeRegs[7].value = drvConfig->vccaOvuvImpactReg.vccaFsOv |
                drvConfig->vccaOvuvImpactReg.vccaFsUv;
        initFailSafeRegs[8].value = drvConfig->vauxOvuvImpactReg.vauxFsOv |
                drvConfig->vauxOvuvImpactReg.vauxFsUv;

        /* Check if device is in debug mode. */
        status = FS45_ReadRegister(FS45_M_HW_CONFIG_ADDR, &response);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if((response.readData & FS45_R_M_DBG_HW_MASK) == FS45_R_M_DBG_HW_DEBUG)
        {
            /* Change INIT_FAULT register mask - FS1B_CAN_IMP bit is 0 by default in debug mode. */
            initFailSafeRegs[2].readMask = 0x0BU;
        }

        status = FS45_WriteRegisters(initFailSafeRegs, 9U);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        /* INIT_MAIN -> NORMAL_MODE. */
        initIntValue = drvConfig->intReg.duration |
                drvConfig->intReg.inhLin |
                drvConfig->intReg.inhAll |
                drvConfig->intReg.inhVsns |
                drvConfig->intReg.inhVpre |
                drvConfig->intReg.inhVcore |
                drvConfig->intReg.inhVother |
                drvConfig->intReg.inhCan;

        status = FS45_WriteRegister(FS45_M_INIT_INT_ADDR, initIntValue, NULL);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* Read Diag. registers to clear all bits. */
    statusInt = (uint32_t)FS45_ReadRegister(FS45_M_DIAG_VPRE_ADDR, &response);
    statusInt|= (uint32_t)FS45_ReadRegister(FS45_M_DIAG_VCORE_ADDR, &response);
    statusInt|= (uint32_t)FS45_ReadRegister(FS45_M_DIAG_VCCA_ADDR, &response);
    statusInt|= (uint32_t)FS45_ReadRegister(FS45_M_DIAG_VAUX_ADDR, &response);
    statusInt|= (uint32_t)FS45_ReadRegister(FS45_M_DIAG_VSUP_VCAN_ADDR, &response);
    if (statusInt != (uint32_t)STATUS_SUCCESS)
    {
        return STATUS_ERROR;
    }

    /* Initialize the rest of registers.
     * If nonInitRegs is NULL, or nonInitRegsCount is 0, this step will be skipped. */
    status = FS45_WriteRegisters(drvConfig->nonInitRegs, drvConfig->nonInitRegsCount);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* INIT_FS -> NORMAL_WD */
    status = FS45_WD_Refresh();
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_GetDefaultInitRegsConfig
 * Description : This function fills the driver configuration structure
 *               with default values of INIT phase registers.
 *
 * Implements : FS45_GetDefaultInitRegsConfig_Activity
 *
 *END**************************************************************************/
void FS45_GetDefaultInitRegsConfig(fs45_drv_config_t* drvConfig)
{
    DEV_ASSERT(drvConfig != NULL);

    drvConfig->vregReg.iccaLim = FS45_RW_M_ICCA_LIM_ICCA_LIM_OUT;
    drvConfig->vregReg.tccaLimOff = FS45_RW_M_TCCA_LIM_OFF_50_MS;
    drvConfig->vregReg.ipffDis = FS45_RW_M_IPFF_DIS_ENABLED;
    drvConfig->vregReg.vcanOvMon = FS45_RW_M_VCAN_OV_MON_OFF;
    drvConfig->vregReg.tauxLimOff = FS45_RW_M_TAUX_LIM_OFF_50_MS;
    drvConfig->vregReg.vauxTrkEn = FS45_RW_M_VAUX_TRK_EN_NO_TRACKING;
    drvConfig->wu1Reg.wuIo0 = FS45_RW_M_WU_IO0_RISING_EDGE;
    drvConfig->wu1Reg.wuIo2 = FS45_RW_M_WU_IO2_NO_WAKEUP;
    drvConfig->wu1Reg.wuIo3 = FS45_RW_M_WU_IO3_NO_WAKEUP;
    drvConfig->wu1Reg.wuIo4 = FS45_RW_M_WU_IO4_NO_WAKEUP;
    drvConfig->wu2Reg.wuIo5 = FS45_RW_M_WU_IO5_NO_WAKEUP;
    drvConfig->wu2Reg.canDisCfg = FS45_RW_M_CAN_DIS_CFG_RX_ONLY;
    drvConfig->wu2Reg.canWuTo = FS45_RW_M_CAN_WU_TO_120US;
    drvConfig->wu2Reg.linJ2602Dis = FS45_RW_M_LIN_J2602_DIS_COMPLIANT;
    drvConfig->wu2Reg.linSr = FS45_RW_M_LIN_SR_20KBITS;
    drvConfig->inhIntReg.inh5 = FS45_RW_M_INT_INH_5_NOT_MASKED;
    drvConfig->inhIntReg.inh4 = FS45_RW_M_INT_INH_4_NOT_MASKED;
    drvConfig->inhIntReg.inh3 = FS45_RW_M_INT_INH_3_NOT_MASKED;
    drvConfig->inhIntReg.inh2 = FS45_RW_M_INT_INH_2_NOT_MASKED;
    drvConfig->inhIntReg.inh0 = FS45_RW_M_INT_INH_0_NOT_MASKED;

    drvConfig->fs1bTimingReg.fs1bTime = FS45_R_FS_FS1B_TIME_37_297MS;
    drvConfig->supervisorReg.vcore5d = FS45_R_FS_VCORE_5D_NORMAL;
    drvConfig->supervisorReg.vcca5d = FS45_R_FS_VCCA_5D_NORMAL;
    drvConfig->supervisorReg.vaux5d = FS45_R_FS_VAUX_5D_NORMAL;
    drvConfig->supervisorReg.fs1bTimeRange = FS45_R_FS_FS1B_TIME_RANGE_X1;
    drvConfig->faultReg.fltErrFs = FS45_R_FS_FLT_ERR_FS_INT3_FIN6;
    drvConfig->faultReg.fs1bCanImpact = FS45_R_FS_FS1B_CAN_IMPACT_NO_EFFECT;
    drvConfig->faultReg.fltErrImp = FS45_R_FS_FLT_ERR_IMP_FS0B;
    drvConfig->fssmReg.io45Fs = FS45_R_FS_IO_45_FS_NOT_SAFETY;
    drvConfig->fssmReg.io23Fs = FS45_R_FS_IO_23_FS_SAFETY_CRITICAL;
    drvConfig->fssmReg.ps = FS45_R_FS_PS_HIGH;
    drvConfig->fssmReg.rstbDuration = FS45_R_FS_RSTB_DURATION_10MS;
    drvConfig->sfImpactReg.tdlyTdur = FS45_R_FS_TDLY_TDUR_DELAY;
    drvConfig->sfImpactReg.dis8s = FS45_R_FS_DIS_8S_ENABLED;
    drvConfig->sfImpactReg.wdImpact = FS45_R_FS_WD_IMPACT_RSTB;
    drvConfig->wdCntReg.wdCntErr = FS45_R_FS_WD_CNT_ERR_6;
    drvConfig->wdCntReg.wdCntRfr = FS45_R_FS_WD_CNT_RFR_6;
    drvConfig->vcoreOvuvImpactReg.vcoreFsOv = FS45_R_FS_VCORE_FS_OV_RSTB_FS0B;
    drvConfig->vcoreOvuvImpactReg.vcoreFsUv = FS45_R_FS_VCORE_FS_UV_FS0B;
    drvConfig->vccaOvuvImpactReg.vccaFsOv = FS45_R_FS_VCCA_FS_OV_RSTB_FS0B;
    drvConfig->vccaOvuvImpactReg.vccaFsUv = FS45_R_FS_VCCA_FS_UV_FS0B;
    drvConfig->vauxOvuvImpactReg.vauxFsOv = FS45_R_FS_VAUX_FS_OV_RSTB_FS0B;
    drvConfig->vauxOvuvImpactReg.vauxFsUv = FS45_R_FS_VAUX_FS_UV_FS0B;

    drvConfig->intReg.duration = FS45_RW_M_NT_DURATION_100US;
    drvConfig->intReg.inhLin = FS45_RW_M_INT_INH_LIN_ALL_SOURCES;
    drvConfig->intReg.inhAll = FS45_RW_M_INT_INH_ALL_ALL_SOURCES;
    drvConfig->intReg.inhVsns = FS45_RW_M_INT_INH_VSNS_ALL_SOURCES;
    drvConfig->intReg.inhVpre = FS45_RW_M_INT_INH_VPRE_ALL_SOURCES;
    drvConfig->intReg.inhVcore = FS45_RW_M_INT_INH_VCORE_ALL_SOURCES;
    drvConfig->intReg.inhVother = FS45_RW_M_INT_INH_VOTHER_ALL_SOURCES;
    drvConfig->intReg.inhCan = FS45_RW_M_INT_INH_CAN_ALL_SOURCES;

    drvConfig->nonInitRegs = NULL;
    drvConfig->nonInitRegsCount = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_GetDefaultNonInitRegsConfig
 * Description : This function fills the driver configuration structure
 *               with default values of non-init registers.
 *
 * Implements : FS45_GetDefaultNonInitRegsConfig_Activity
 *
 *END**************************************************************************/
void FS45_GetDefaultNonInitRegsConfig(fs45_drv_config_t* drvConfig, fs45_reg_config_value_t* nonInitRegs)
{
    DEV_ASSERT(drvConfig != NULL);

    drvConfig->nonInitRegs = nonInitRegs;
    drvConfig->nonInitRegsCount = 5U;

    drvConfig->nonInitRegs[0].address = FS45_M_MODE_ADDR;
    drvConfig->nonInitRegs[0].value = FS45_RW_M_VKAM_EN_DISABLED;
    drvConfig->nonInitRegs[0].readMask = 0x80U;
    
    drvConfig->nonInitRegs[1].address = FS45_M_REG_MODE_ADDR;
    drvConfig->nonInitRegs[1].value = FS45_M_REG_MODE_DEFAULT;
    drvConfig->nonInitRegs[1].readMask = 0x0FU;
    
    drvConfig->nonInitRegs[2].address = FS45_M_IO_OUT_AMUX_ADDR;
    drvConfig->nonInitRegs[2].value = FS45_M_IO_OUT_AMUX_DEFAULT;
    drvConfig->nonInitRegs[2].readMask = 0xC7U;
    
    drvConfig->nonInitRegs[3].address = FS45_FS_WD_WINDOW_ADDR;
    drvConfig->nonInitRegs[3].value = FS45_FS_WD_WINDOW_DEFAULT;
    drvConfig->nonInitRegs[3].readMask = 0xFFU;

    drvConfig->nonInitRegs[4].address = FS45_FS_WD_LFSR_ADDR;
    drvConfig->nonInitRegs[4].value = FS45_FS_WD_LFSR_DEFAULT;
    drvConfig->nonInitRegs[4].readMask = 0xFFU;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_ReadRegister
 * Description : Performs a single read register based on provided address.
 *               The response is returned in @ref fs45_rx_frame_t structure.
 *
 * Implements : FS45_ReadRegister_Activity
 *
 *END**************************************************************************/
status_t FS45_ReadRegister(uint8_t address, fs45_rx_data_t* rxData)
{
    fs45_tx_data_t txData = {
        .registerAddress = address,
        .commandType = fs45RegRead
    };

    DEV_ASSERT(rxData != NULL);

    txData.isSecured = FS45_IsRegisterSecured(address);

    return FS45_TransferData(&txData, rxData);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WriteRegister
 * Description : Sends write command to the FS45.
 *
 * Implements : FS45_WriteRegister_Activity
 *
 *END**************************************************************************/
status_t FS45_WriteRegister(uint8_t address, uint8_t writeData, fs45_rx_data_t* rxData)
{
    fs45_tx_data_t txData = {
        .registerAddress = address,
        .writeData = writeData,
        .commandType = fs45RegWrite
    };

    txData.isSecured = FS45_IsRegisterSecured(address);

    return FS45_TransferData(&txData, rxData);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WD_Refresh
 * Description : Performs the watchdog refresh.
 *
 * Implements : FS45_WD_Refresh_Activity
 *
 *END**************************************************************************/
status_t FS45_WD_Refresh(void)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    uint8_t lfsr;                     /* Line feed shift register for challenger WD. */
    uint8_t answer;                   /* Calculated monitoring result. */

    /* Synchronize with LFSR generator on the device. */
    status = FS45_WD_ReadChallengeToken(&lfsr);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    answer = FS45_WD_ComputeAnswer(lfsr);

    status = FS45_WriteRegister(FS45_FS_WD_ANSWER_ADDR,
            (uint8_t)(answer << FS45_W_FS_WD_ANSWER_SHIFT), NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_SwitchAMUXchannel
 * Description : Switches a desired channel to the AMUX pin.
 *
 * Implements : FS45_SwitchAMUXchannel_Activity
 *
 *END**************************************************************************/
status_t FS45_SwitchAMUXchannel(fs45_amux_selection_t channelSelection)
{
    /* Write IO_OUT_AMUX register. */
    return FS45_WriteRegister(FS45_M_IO_OUT_AMUX_ADDR, (uint8_t)channelSelection, NULL);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_SetRegulatorState
 * Description : Sets state (enable/disable) of the selected voltage regulator.
 *
 * Implements : FS45_SetRegulatorState_Activity
 *
 *END**************************************************************************/
status_t FS45_SetRegulatorState(fs45_reg_mode_t vreg, bool enable)
{
    status_t status;                                    /* Status variable. */
    uint8_t writeValue = 0U;                            /* Register write value. */
    uint8_t address = 0U;                               /* Register address. */
    uint8_t readMask = 0U;                              /* Register read mask. */
    uint8_t writeMask = 0U;                             /* Register write mask. */
    uint8_t writeShift = 0U;                            /* Register write shift. */
    fs45_rx_data_t response;                            /* Received data. */
    uint8_t secureWriteShift = FS45_SECURE_WRITE_SHIFT; /* Shift for reg. write data. */
    bool readActualValue = true;                        /* If true, register actual value is read before write operation. */
    uint8_t enableValue = enable ? 1U : 0U;             /* Enable/disable regulator output. */

    switch (vreg)
    {
        case fs45VCan:
            address = FS45_M_REG_MODE_ADDR;
            readMask = FS45_R_M_VCAN_EN_MASK;
            writeMask = FS45_W_M_VCAN_EN_MASK;
            writeShift = FS45_W_M_VCAN_EN_SHIFT;
            break;

        case fs45Aux:
            address = FS45_M_REG_MODE_ADDR;
            readMask = FS45_R_M_VAUX_EN_MASK;
            writeMask = FS45_W_M_VAUX_EN_MASK;
            writeShift = FS45_W_M_VAUX_EN_SHIFT;
            break;

        case fs45Vcca:
            address = FS45_M_REG_MODE_ADDR;
            readMask = FS45_R_M_VCCA_EN_MASK;
            writeMask = FS45_W_M_VCCA_EN_MASK;
            writeShift = FS45_W_M_VCCA_EN_SHIFT;
            break;

        case fs45Vcore:
            address = FS45_M_REG_MODE_ADDR;
            readMask = FS45_R_M_VCORE_EN_MASK;
            writeMask = FS45_W_M_VCORE_EN_MASK;
            writeShift = FS45_W_M_VCORE_EN_SHIFT;
            break;

        case fs45Vkam:
            address = FS45_M_MODE_ADDR;
            readMask = (uint8_t)FS45_RW_M_VKAM_EN_MASK;
            writeMask = (uint8_t)FS45_RW_M_VKAM_EN_MASK;
            writeShift = FS45_RW_M_VKAM_EN_SHIFT;
            /* Value is not shifted for M_MODE register. */
            secureWriteShift = 0;
            /* Vkam bit is the only non-volatile value that can be set by M_MODE register.
             * It has no sense to read content of this register before write. */
            readActualValue = false;
            break;

        default:
            /* Not valid parameter. */
            break;
    }

    if (readActualValue)
    {
        /* Read actual register value to preserve value of other bits. */
        status = FS45_ReadRegister(address, &response);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        /* Compose write value. */
        writeValue = FS45_BO_SETVAL((uint8_t)(response.readData << secureWriteShift),
                (uint8_t)(enableValue << writeShift), writeMask);
    }
    else
    {
        /* Write just the enable/disable bit (M_MODE register only). */
        writeValue = (uint8_t)(enableValue << writeShift);
    }

    /* It is not necessary to check status. If someting goes wrong the next check catches the error. */
    (void)FS45_WriteRegister(address, writeValue, NULL);

    /* Check if the value was accepted. */
    status = FS45_ReadRegister(address, &response);
    if ((status != STATUS_SUCCESS) || (((response.readData & readMask) << secureWriteShift) != (writeValue & writeMask)))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_GetFaultErrorCounterValue
 * Description : Reads actual Fault Error Counter value.
 *
 * Implements : FS45_GetFaultErrorCounterValue_Activity
 *
 *END**************************************************************************/
status_t FS45_GetFaultErrorCounterValue(uint8_t* faultErrorCounterValue)
{
    status_t status;         /* Status value. */
    fs45_rx_data_t response; /* Register received data. */

    DEV_ASSERT(faultErrorCounterValue != NULL);

    status = FS45_ReadRegister(FS45_FS_DIAG_SF_ERR_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    *faultErrorCounterValue = FS45_BO_GETVAL(response.readData, FS45_R_FS_FLT_ERR_MASK,
            FS45_R_FS_FLT_ERR_SHIFT);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_GetMode
 * Description : This function gets current and previous mode of the SBC.
 *
 * Implements : FS45_GetMode_Activity
 *
 *END**************************************************************************/
status_t FS45_GetMode(fs45_current_mode_t *currentMode, fs45_prev_mode_t *prevMode)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    fs45_rx_data_t response;          /* Received data. */

    DEV_ASSERT(currentMode != NULL);
    DEV_ASSERT(prevMode != NULL);

    status = FS45_ReadRegister(FS45_M_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Parses response to get previous mode of the SBC. */
    switch (response.readData & (FS45_R_M_LPOFF_MASK | FS45_R_M_DFS_MASK))
    {
        case FS45_R_M_LPOFF_RESUME_LPOFF:
            *prevMode = fs45PrevModeLPOFF;
            break;
        case FS45_R_M_DFS_RESUME_DFS:
            *prevMode = fs45PrevModeDFS;
            break;
        default:
            *prevMode = fs45PrevModePOR;
            break;
    }

    /* Parses response to get current mode of the SBC. */
    switch (response.readData & (FS45_R_M_NORMAL_MASK | FS45_R_M_INIT_MASK))
    {
        case FS45_R_M_NORMAL_NORMAL:
            *currentMode = fs45ModeNormal;
            break;
        case FS45_R_M_INIT_INIT:
            *currentMode = fs45ModeInit;
            break;
        default:
            *currentMode = fs45ModeUnknown;
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_CheckVAUX
 * Description : This function checks if VAUX is safety critical and optionally
 *               runs related diagnostics.
 *
 * Implements : FS45_CheckVAUX_Activity
 *
 *END**************************************************************************/
status_t FS45_CheckVAUX(void)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    fs45_rx_data_t response;          /* Response to the command. */

    /* Check ABIST2_VAUX if necessary. */
    status = FS45_ReadRegister(FS45_FS_INIT_VAUX_OVUV_IMPACT_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if (((response.readData & FS45_R_FS_VAUX_FS_OV_MASK) != FS45_R_FS_VAUX_FS_OV_NO_EFFECT)
            || ((response.readData & FS45_R_FS_VAUX_FS_UV_MASK) != FS45_R_FS_VAUX_FS_OV_NO_EFFECT))
    {
        /* Run ABIST2_VAUX. */
        status = FS45_WriteRegister(FS45_FS_BIST_ADDR, FS45_W_FS_ABIST2_VAUX_ABIST_VAUX, NULL);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        /* Wait for 200 us (see AN5238 document for details), 1 ms added. */
        OSIF_TimeDelay(1);

        /* Check BIST for ABIST2_VAUX result. */
        status = FS45_ReadRegister(FS45_FS_BIST_ADDR, &response);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if ((response.readData & FS45_R_FS_ABIST2_VAUX_OK_MASK) != FS45_R_FS_ABIST2_VAUX_OK_PASS)
        {
            return STATUS_ERROR;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_CheckFS1B
 * Description : This function checks if FS1B has expected low level during
 *               initialization and runs related diagnostics.
 *
 * Implements : FS45_CheckFS1B_Activity
 *
 *END**************************************************************************/
status_t FS45_CheckFS1B(void)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    fs45_rx_data_t response;          /* Response to the command. */

    /* Check if FS1B is implemented. */
    if (g_fs1bEnabled == true)
    {
        status = FS45_ReadRegister(FS45_FS_RELEASE_FSXB_ADDR, &response);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        /* Check if FS1B is at high. */
        if ((response.readData & FS45_R_FS_FS1B_SNS_MASK) == FS45_R_FS_FS1B_SNS_HIGH)
        {
            status = FS45_ReadRegister(FS45_FS_DIAG_SF_IOS_ADDR, &response);
            if (status != STATUS_SUCCESS)
            {
                return status;
            }

            /* Check DIAG_SF_IOS for short-circuit to high. */
            if (((response.readData & FS45_R_FS_FS1B_DIAG_MASK) == FS45_R_FS_FS1B_DIAG_SC_LOW) ||
                ((response.readData & FS45_R_FS_FS1B_DIAG_MASK) == FS45_R_FS_FS1B_DIAG_SC_HIGH))
            {
                return STATUS_ERROR;
            }
        }

        /* Run ABIST2_FS1B. */
        status = FS45_WriteRegister(FS45_FS_BIST_ADDR, FS45_W_FS_ABIST2_FS1B_ABIST_FS1B, NULL);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        /* Wait 200 us (see AN5238 document for details), 1 ms added. */
        OSIF_TimeDelay(1);

        /* Check BIST for ABIST2_FS1B result. */
        status = FS45_ReadRegister(FS45_FS_BIST_ADDR, &response);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }

        if ((response.readData & FS45_R_FS_ABIST2_FS1B_OK_MASK) !=
                FS45_R_FS_ABIST2_FS1B_OK_PASS)
        {
            return STATUS_ERROR;
        }
    }
    else
    {
        status = STATUS_UNSUPPORTED;
    }
    
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_ReleaseFSx
 * Description : This function releases selected fail-safe output as a part of
 *               error recovery procedure.
 *
 * Implements : FS45_ReleaseFSx_Activity
 *
 *END**************************************************************************/
status_t FS45_ReleaseFSx(fs45_fsxb_release_t fsOutput)
{
    status_t status = STATUS_SUCCESS; /* Status variable. */
    uint8_t writeData = 0U;           /* Data to be written. */
    uint8_t wdSeed = 0U;              /* Watchdog seed value. */

    /* FS1B release not supported on devices without FS1B output. */
    if ((g_fs1bEnabled == false) && (fsOutput != fs45ReleaseFs0b))
    {
        return STATUS_UNSUPPORTED;
    }
    
    if (g_fs1bEnabled == true)
    {
        status = FS45_CheckFS1B();
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
    }
    
    status = FS45_CheckVAUX();
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Get actual LFSR state. */
    status = FS45_WD_ReadChallengeToken(&wdSeed);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Prepare release word. */
    switch (fsOutput)
    {
        case fs45ReleaseFs0b:
            writeData = 0x60U;
            writeData |= (uint8_t)(((wdSeed & (1U << 4U)) != 0U) ? 0x00U : 0x01U);
            writeData |= (uint8_t)(((wdSeed & (1U << 3U)) != 0U) ? 0x00U : 0x02U);
            writeData |= (uint8_t)(((wdSeed & (1U << 2U)) != 0U) ? 0x00U : 0x04U);
            writeData |= (uint8_t)(((wdSeed & (1U << 1U)) != 0U) ? 0x00U : 0x08U);
            writeData |= (uint8_t)(((wdSeed & (1U << 0U)) != 0U) ? 0x00U : 0x10U);
            break;
        case fs45ReleaseFs1b:
            writeData = 0xC0U;
            writeData |= (uint8_t)(((wdSeed & (1U << 7U)) != 0U) ? 0x00U : 0x01U);
            writeData |= (uint8_t)(((wdSeed & (1U << 6U)) != 0U) ? 0x00U : 0x02U);
            writeData |= (uint8_t)(((wdSeed & (1U << 5U)) != 0U) ? 0x00U : 0x04U);
            writeData |= (uint8_t)(((wdSeed & (1U << 4U)) != 0U) ? 0x00U : 0x08U);
            writeData |= (uint8_t)(((wdSeed & (1U << 3U)) != 0U) ? 0x00U : 0x10U);
            break;
        case fs45ReleaseFs0bFs1b:
            writeData = 0xA0U;
            writeData |= (uint8_t)(((wdSeed & (1U << 7U)) != 0U) ? 0x00U : 0x01U);
            writeData |= (uint8_t)(((wdSeed & (1U << 6U)) != 0U) ? 0x00U : 0x02U);
            writeData |= (uint8_t)(((wdSeed & (1U << 2U)) != 0U) ? 0x00U : 0x04U);
            writeData |= (uint8_t)(((wdSeed & (1U << 1U)) != 0U) ? 0x00U : 0x08U);
            writeData |= (uint8_t)(((wdSeed & (1U << 0U)) != 0U) ? 0x00U : 0x10U);
            break;
        default:
            /* Not valid parameter. */
            break;
    }

    return FS45_WriteRegister(FS45_FS_RELEASE_FSXB_ADDR, writeData, NULL);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_CheckLbistAbistOk
 * Description : Checks if the ABIST1 and LBIST diagnostics passed. If not,
 *               the function returns error code.
 *
 * Implements : FS45_CheckLbistAbistOk_Activity
 *
 *END**************************************************************************/
status_t FS45_CheckLbistAbistOk(void)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */

    /* Check BIST for LBIST & ABIST1 completion. */
    status = FS45_ReadRegister(FS45_FS_BIST_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((response.readData & FS45_R_FS_ABIST1_OK_MASK) != FS45_R_FS_ABIST1_OK_PASS)
    {
        return STATUS_ERROR;
    }

    if ((response.readData & FS45_R_FS_LBIST_OK_MASK) != FS45_R_FS_LBIST_OK_PASS)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_SetLowPowerMode
 * Description : This function switches mode of the SBC to the LPOFF,
 *               optionally with automatic wake-up 1ms after transition.
 *
 * Implements : FS45_SetLowPowerMode_Activity
 *
 *END**************************************************************************/
status_t FS45_SetLowPowerMode(bool autoWU)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t  writeData;      /* Data to be updated. */

    /* Read VKAM control state to preserve it. */
    status = FS45_ReadRegister(FS45_M_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Set previously read VKAM value to write data. */
    writeData = response.readData & FS45_RW_M_VKAM_EN_MASK;

    /* Set LPOFF mode. */
    writeData |= autoWU ? FS45_W_M_LPOFF_AUTO_WU_LPOFF : FS45_W_M_GO_LPOFF_LPOFF;

    return FS45_WriteRegister(FS45_M_MODE_ADDR, writeData, NULL);

    /* Write check is not performed because of entering LPOFF mode. */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_RequestInterrupt
 * Description : This function requests an interrupt (pulse on the INT pin).
 *
 * Implements : FS45_RequestInterrupt_Activity
 *
 *END**************************************************************************/
status_t FS45_RequestInterrupt(void)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Register write data. */

    /* Prepare content. */
    status = FS45_ReadRegister(FS45_M_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* Save VKAM value and set INT_REQ bit. */
    writeData = (response.readData & FS45_RW_M_VKAM_EN_MASK) | FS45_W_M_INT_REQ_INT_REQ;

    return FS45_WriteRegister(FS45_M_MODE_ADDR, writeData, NULL);

    /* Write check is not performed because written value cannot be read out. */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_CAN_SetMode
 * Description : This function changes CAN mode and the automatic transition
 *               of the CAN transceiver to the low-power mode on specific events.
 *
 * Implements : FS45_CAN_SetMode_Activity
 *
 *END**************************************************************************/
status_t FS45_CAN_SetMode(fs45_can_mode_t mode, bool autoDis)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_CAN_LIN_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (response.readData & (uint8_t)(FS45_RW_M_LIN_AUTO_DIS_MASK | FS45_RW_M_LIN_MODE_MASK)) |
            (uint8_t)mode | (uint8_t)(autoDis ? FS45_RW_M_CAN_AUTO_DIS_RESET : FS45_RW_M_CAN_AUTO_DIS_NO);

    status = FS45_WriteRegister(FS45_M_CAN_LIN_MODE_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_CAN_LIN_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((uint8_t)(response.readData & (FS45_RW_M_CAN_AUTO_DIS_MASK | FS45_RW_M_CAN_MODE_MASK)) !=
            (uint8_t)((uint8_t)mode | (autoDis ? FS45_RW_M_CAN_AUTO_DIS_RESET : FS45_RW_M_CAN_AUTO_DIS_NO)))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

#ifdef FS45_FEATURE_LIN
/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LIN_SetMode
 * Description : This function changes LIN mode and the automatic transition
 *               of the LIN transceiver to the low-power mode on specific events.
 *
 * Implements : FS45_LIN_SetMode_Activity
 *
 *END**************************************************************************/
status_t FS45_LIN_SetMode(fs45_lin_mode_t mode, bool autoDis)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_CAN_LIN_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (response.readData & (uint8_t)(FS45_RW_M_CAN_AUTO_DIS_MASK | FS45_RW_M_CAN_MODE_MASK)) |
            (uint8_t)mode | (uint8_t)(autoDis ? FS45_RW_M_LIN_AUTO_DIS_RESET : FS45_RW_M_LIN_AUTO_DIS_NO);

    status = FS45_WriteRegister(FS45_M_CAN_LIN_MODE_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_CAN_LIN_MODE_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((uint8_t)(response.readData & (FS45_RW_M_LIN_AUTO_DIS_MASK | FS45_RW_M_LIN_MODE_MASK)) !=
            (uint8_t)((uint8_t)mode | (uint8_t)(autoDis ? FS45_RW_M_LIN_AUTO_DIS_RESET : FS45_RW_M_LIN_AUTO_DIS_NO)))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}
#endif

/*******************************************************************************
 * Code - public functions - LDT (long duration timer)
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_SetTmrOp
 * Description : This function sets operating function of the LDT.
 *
 * Implements : FS45_LDT_SetTimerOperation_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_SetTimerOperation(fs45_ldt_function_t op)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (uint8_t)(response.readData & (uint8_t)((uint8_t)(FS45_RW_M_LDT_ENABLE_MASK | FS45_RW_M_MODE_MASK) |
            FS45_RW_M_REG_SE_MASK)) | (uint8_t)op;

    status = FS45_WriteRegister(FS45_M_LONG_DURATION_TIMER_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((response.readData & FS45_RW_M_F2_F0_MASK) != (uint8_t)op)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_SetTmrMode
 * Description : This function sets mode of the LDT (normal/calibration).
 *
 * Implements : FS45_LDT_SetTimerMode_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_SetTimerMode(fs45_ldt_mode_t mode)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (uint8_t)(response.readData & (FS45_RW_M_LDT_ENABLE_MASK | FS45_RW_M_REG_SE_MASK |
            FS45_RW_M_F2_F0_MASK)) | (uint8_t)mode;

    status = FS45_WriteRegister(FS45_M_LONG_DURATION_TIMER_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((response.readData & FS45_RW_M_MODE_MASK) != (uint8_t)mode)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_SetWakeUpRegSrc
 * Description : This function sets counter to read real-time counter or
 *               programmed value into wake-up register.
 *
 * Implements : FS45_LDT_SetWakeUpRegSrc_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_SetWakeUpRegSrc(fs45_ldt_wu_scr_t source)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (uint8_t)(response.readData & (FS45_RW_M_LDT_ENABLE_MASK | FS45_RW_M_MODE_MASK |
            FS45_RW_M_F2_F0_MASK)) | (uint8_t)source;

    status = FS45_WriteRegister(FS45_M_LONG_DURATION_TIMER_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((response.readData & FS45_RW_M_REG_SE_MASK) != (uint8_t)source)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_RunCounter
 * Description : This function starts or stops the LDT counter.
 *
 * Implements : FS45_LDT_RunCounter_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_RunCounter(bool run)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register value. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Update register. */
    writeData = (uint8_t)(response.readData & (uint8_t)(FS45_RW_M_MODE_MASK | FS45_RW_M_REG_SE_MASK |
            FS45_RW_M_F2_F0_MASK)) | (uint8_t)(run ? FS45_RW_M_LDT_ENABLE_START : FS45_RW_M_LDT_ENABLE_STOP);

    status = FS45_WriteRegister(FS45_M_LONG_DURATION_TIMER_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_LONG_DURATION_TIMER_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if ((response.readData & FS45_RW_M_LDT_ENABLE_MASK) !=
            (run ? FS45_RW_M_LDT_ENABLE_START : FS45_RW_M_LDT_ENABLE_STOP))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_SetAfterRunValue
 * Description : This function sets new after-run value for the LDT.
 *
 * Implements : FS45_LDT_SetAfterRunValue_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_SetAfterRunValue(uint16_t value)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Set high byte. */
    writeData = (uint8_t)((value >> 8) & 0xFFU);

    status = FS45_WriteRegister(FS45_M_LDT_AFTER_RUN_1_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = FS45_ReadRegister(FS45_M_LDT_AFTER_RUN_1_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (response.readData != writeData)
    {
        return STATUS_ERROR;
    }

    /* Set low byte. */
    writeData = (uint8_t)(value & 0xFFU);

    status = FS45_WriteRegister(FS45_M_LDT_AFTER_RUN_2_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = FS45_ReadRegister(FS45_M_LDT_AFTER_RUN_2_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (response.readData != writeData)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_LDT_SetWakeUpValue
 * Description : This function sets new wake-up value for the LDT.
 *
 * Implements : FS45_LDT_SetWakeUpValue_Activity
 *
 *END**************************************************************************/
status_t FS45_LDT_SetWakeUpValue(uint32_t value)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Set high byte. */
    writeData = (uint8_t)((value >> 16U) & 0xFFU);

    status = FS45_WriteRegister(FS45_M_LDT_WAKE_UP_1_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = FS45_ReadRegister(FS45_M_LDT_WAKE_UP_1_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (response.readData != writeData)
    {
        return STATUS_ERROR;
    }

    /* Set middle byte. */
    writeData = (uint8_t)((value >> 8U) & 0xFFU);

    status = FS45_WriteRegister(FS45_M_LDT_WAKE_UP_2_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = FS45_ReadRegister(FS45_M_LDT_WAKE_UP_2_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (response.readData != writeData)
    {
        return STATUS_ERROR;
    }

    /* Set low byte. */
    writeData = (uint8_t)(value & 0xFFU);

    status = FS45_WriteRegister(FS45_M_LDT_WAKE_UP_3_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    status = FS45_ReadRegister(FS45_M_LDT_WAKE_UP_3_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (response.readData != writeData)
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * Code - public functions - safety related
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WD_ChangeWindow
 * Description : This function changes duration of watchdog window.
 *
 * Implements : FS45_WD_ChangeWindow_Activity
 *
 *END**************************************************************************/
status_t FS45_WD_ChangeWindow(fs45_wd_window_t windowDuration)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the read command. */

    /* Write the WD Window register. */
    status = FS45_WriteRegister(FS45_FS_WD_WINDOW_ADDR, (uint8_t)windowDuration, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check the written data. */
    status = FS45_ReadRegister(FS45_FS_WD_WINDOW_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((response.readData & FS45_R_FS_WD_WINDOW_MASK) != ((uint8_t)windowDuration >> FS45_W_FS_WD_WINDOW_SHIFT))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_WD_ChangeSeed
 * Description : This function changes seed of LFSR used for watchdog.
 *
 * Implements : FS45_WD_ChangeSeed_Activity
 *
 *END**************************************************************************/
status_t FS45_WD_ChangeSeed(uint8_t wdSeed)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the read command. */

    DEV_ASSERT(wdSeed != 0xFFU); /* This value is not allowed. */

    /* Write the WD seed. */
    status =  FS45_WriteRegister(FS45_FS_WD_LFSR_ADDR, \
            (uint8_t)(wdSeed << FS45_RW_FS_WD_LFSR_SHIFT), NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check the written WD seed. */
    status = FS45_ReadRegister(FS45_FS_WD_LFSR_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((response.readData & FS45_RW_FS_WD_LFSR_MASK) !=
            (uint8_t)(wdSeed << FS45_RW_FS_WD_LFSR_SHIFT))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_RequestReset
 * Description : This function requests a low pulse on the RSTB (MCU reset).
 *
 * Implements : FS45_RequestReset_Activity
 *
 *END**************************************************************************/
status_t FS45_RequestReset(void)
{
    return FS45_WriteRegister(FS45_FS_SF_OUTPUT_REQUEST_ADDR, \
            FS45_W_FS_RSTB_REQ_RSTB_REQ, NULL);

    /* Write check is not performed because written value cannot be read out. */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_RequestFSxLow
 * Description : This function requests a low level on the selected fail-safe output.
 *
 * Implements : FS45_RequestFSxLow_Activity
 *
 *END**************************************************************************/
status_t FS45_RequestFSxLow(fs45_fsx_req_type_t fsxSelection)
{
    /* FS1B request not supported on devices without FS1B output. */
    if ((g_fs1bEnabled == false) && (fsxSelection != fs45ReqFS0B))
    {
        return STATUS_UNSUPPORTED;
    }
    
    return FS45_WriteRegister(FS45_FS_SF_OUTPUT_REQUEST_ADDR, (uint8_t)fsxSelection, NULL);

    /* Write check is not performed because written value cannot be read out. */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FS45_SetOUT4
 * Description : This function sets level of the IO_4 when configured
 *               as an output.
 *
 * Implements : FS45_SetOUT4_Activity
 *
 *END**************************************************************************/
status_t FS45_SetOUT4(bool level)
{
    status_t status;         /* Status variable. */
    fs45_rx_data_t response; /* Response to the command. */
    uint8_t writeData;       /* Data to be written. */

    /* Read current register data. */
    status = FS45_ReadRegister(FS45_M_IO_OUT_AMUX_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Prepare content. */
    writeData = (uint8_t)(response.readData & FS45_RW_M_AMUX_MASK) |
            (uint8_t)(((uint8_t) level) << FS45_RW_M_IO_OUT_4_SHIFT) | FS45_RW_M_IO_OUT_4_EN_ENABLED;

    /* Update register. */
    status = FS45_WriteRegister(FS45_M_IO_OUT_AMUX_ADDR, writeData, NULL);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Check written data. */
    status = FS45_ReadRegister(FS45_M_IO_OUT_AMUX_ADDR, &response);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    if ((response.readData & (uint8_t)(FS45_RW_M_IO_OUT_4_MASK | FS45_RW_M_IO_OUT_4_EN_MASK)) !=
            ((uint8_t)(((uint8_t) level) << FS45_RW_M_IO_OUT_4_SHIFT) | FS45_RW_M_IO_OUT_4_EN_ENABLED))
    {
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}
