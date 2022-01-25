/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
 * @file lpi2c_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros were defined for consistency reasons, all the registers have a corresponding ID.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower
 * or different essential type [MISRA 2012 Rule 10.3, required]
 * This is required by the conversion of a unsigned value of a bitfield/bit into a enum value.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially
 * unsigned' to 'essentially Boolean'. This is required by the conversion of a bit into a bool.
 * Impermissible cast; cannot cast from 'essentially Boolean' to 'essentially unsigned'.
 * This is required by the conversion of a bool into a bit.
 * Impermissible cast; cannot cast from 'essentially unsigned' to 'essentially enum<i>'.
 * This is required by the conversion of a bitfield of a register into a enum.
 */

#if !defined(LPI2C_HW_ACCESS_H)
#define LPI2C_HW_ACCESS_H

#include <stdbool.h>
#include "lpi2c_driver.h"
#include "device_registers.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*!
 * LPI2C master interrupts
 */

#define   LPI2C_MASTER_DATA_MATCH_INT           0x4000U   /*!< Data Match Interrupt       */
#define   LPI2C_MASTER_PIN_LOW_TIMEOUT_INT      0x2000U   /*!< Pin Low Timeout Interrupt  */
#define   LPI2C_MASTER_FIFO_ERROR_INT           0x1000U   /*!< FIFO Error Interrupt       */
#define   LPI2C_MASTER_ARBITRATION_LOST_INT      0x800U   /*!< Arbitration Lost Interrupt */
#define   LPI2C_MASTER_NACK_DETECT_INT           0x400U   /*!< NACK Detect Interrupt      */
#define   LPI2C_MASTER_STOP_DETECT_INT           0x200U   /*!< STOP Detect Interrupt      */
#define   LPI2C_MASTER_END_PACKET_INT            0x100U   /*!< End Packet Interrupt       */
#define   LPI2C_MASTER_RECEIVE_DATA_INT            0x2U   /*!< Receive Data Interrupt     */
#define   LPI2C_MASTER_TRANSMIT_DATA_INT           0x1U   /*!< Transmit Data Interrupt    */

/*!
 * LPI2C slave interrupts
 */
#define   LPI2C_SLAVE_SMBUS_ALERT_RESPONSE_INT  0x8000U   /*!< SMBus Alert Response Interrupt */
#define   LPI2C_SLAVE_GENERAL_CALL_INT          0x4000U   /*!< General Call Interrupt         */
#define   LPI2C_SLAVE_ADDRESS_MATCH_1_INT       0x2000U   /*!< Address Match 1 Interrupt      */
#define   LPI2C_SLAVE_ADDRESS_MATCH_0_INT       0x1000U   /*!< Address Match 0 Interrupt      */
#define   LPI2C_SLAVE_FIFO_ERROR_INT             0x800U   /*!< FIFO Error Interrupt           */
#define   LPI2C_SLAVE_BIT_ERROR_INT              0x400U   /*!< Bit Error Interrupt            */
#define   LPI2C_SLAVE_STOP_DETECT_INT            0x200U   /*!< STOP Detect Interrupt          */
#define   LPI2C_SLAVE_REPEATED_START_INT         0x100U   /*!< Repeated Start Interrupt       */
#define   LPI2C_SLAVE_TRANSMIT_ACK_INT             0x8U   /*!< Transmit ACK Interrupt         */
#define   LPI2C_SLAVE_ADDRESS_VALID_INT            0x4U   /*!< Address Valid Interrupt        */
#define   LPI2C_SLAVE_RECEIVE_DATA_INT             0x2U   /*!< Receive Data Interrupt         */
#define   LPI2C_SLAVE_TRANSMIT_DATA_INT            0x1U   /*!< Transmit Data Interrupt        */

/*! @brief Pin configuration selection
 */
typedef enum
{
    LPI2C_CFG_2PIN_OPEN_DRAIN             = 0U,  /*!< 2-pin open drain mode */
    LPI2C_CFG_2PIN_OUTPUT_ONLY            = 1U,  /*!< 2-pin output only mode (ultra-fast mode) */
    LPI2C_CFG_2PIN_PUSH_PULL              = 2U,  /*!< 2-pin push-pull mode */
    LPI2C_CFG_4PIN_PUSH_PULL              = 3U,  /*!< 4-pin push-pull mode */
    LPI2C_CFG_2PIN_OPEN_DRAIN_SLAVE       = 4U,  /*!< 2-pin open drain mode with separate LPI2C slave */
    LPI2C_CFG_2PIN_OUTPUT_ONLY_SLAVE      = 5U,  /*!< 2-pin output only mode (ultra-fast mode) with separate LPI2C slave */
    LPI2C_CFG_2PIN_PUSH_PULL_SLAVE        = 6U,  /*!< 2-pin push-pull mode with separate LPI2C slave */
    LPI2C_CFG_4PIN_PUSH_PULL_INVERTED     = 7U,  /*!< 4-pin push-pull mode (inverted outputs) */
} lpi2c_pin_config_t;

/*! @brief Master NACK reaction configuration
 */
typedef enum
{
    LPI2C_NACK_RECEIVE    = 0U,  /*!< Receive ACK and NACK normally */
    LPI2C_NACK_IGNORE     = 1U,  /*!< Treat a received NACK as if it was an ACK */
} lpi2c_nack_config_t;

/*! @brief LPI2C master prescaler options
 */
typedef enum
{
    LPI2C_MASTER_PRESC_DIV_1    = 0U,  /*!< Divide by 1   */
    LPI2C_MASTER_PRESC_DIV_2    = 1U,  /*!< Divide by 2   */
    LPI2C_MASTER_PRESC_DIV_4    = 2U,  /*!< Divide by 4   */
    LPI2C_MASTER_PRESC_DIV_8    = 3U,  /*!< Divide by 8   */
    LPI2C_MASTER_PRESC_DIV_16   = 4U,  /*!< Divide by 16  */
    LPI2C_MASTER_PRESC_DIV_32   = 5U,  /*!< Divide by 32  */
    LPI2C_MASTER_PRESC_DIV_64   = 6U,  /*!< Divide by 64  */
    LPI2C_MASTER_PRESC_DIV_128  = 7U,  /*!< Divide by 128 */
} lpi2c_master_prescaler_t;

/*! @brief Slave address configuration
 */
typedef enum
{
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT              = 0U,  /*!< Address match 0 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT             = 1U,  /*!< Address match 0 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_7BIT    = 2U,  /*!< Address match 0 (7-bit) or Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_10BIT  = 3U,  /*!< Address match 0 (10-bit) or Address match 1 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_7BIT_OR_1_10BIT   = 4U,  /*!< Address match 0 (7-bit) or Address match 1 (10-bit) */
    LPI2C_SLAVE_ADDR_MATCH_0_10BIT_OR_1_7BIT   = 5U,  /*!< Address match 0 (10-bit) or Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_RANGE_7BIT          = 6U,  /*!< From Address match 0 (7-bit) to Address match 1 (7-bit) */
    LPI2C_SLAVE_ADDR_MATCH_RANGE_10BIT         = 7U,  /*!< From Address match 0 (10-bit) to Address match 1 (10-bit) */
} lpi2c_slave_addr_config_t;

/*! @brief Slave NACK reaction configuration
 */
typedef enum
{
    LPI2C_SLAVE_NACK_END_TRANSFER       = 0U,  /*!< Slave will end transfer when NACK detected */
    LPI2C_SLAVE_NACK_CONTINUE_TRANSFER  = 1U,  /*!< Slave will not end transfer when NACK detected */
} lpi2c_slave_nack_config_t;

/*! @brief Slave ACK transmission options
 */
typedef enum
{
    LPI2C_SLAVE_TRANSMIT_ACK   = 0U,  /*!< Transmit ACK for received word  */
    LPI2C_SLAVE_TRANSMIT_NACK  = 1U,  /*!< Transmit NACK for received word */
} lpi2c_slave_nack_transmit_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Get the size of the Master Receive FIFO
 * 
 * This function returns the size of the Master Receive FIFO, always a power of 2.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  Master Receive FIFO Size
 */
static inline uint16_t  LPI2C_Get_MasterRxFIFOSize(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->PARAM;
    tmp = (tmp & LPI2C_PARAM_MRXFIFO_MASK) >> LPI2C_PARAM_MRXFIFO_SHIFT;
    tmp = 1UL << tmp;     /* RX FIFO size = 2^MRXFIFO */
    return (uint16_t)tmp;
}


/*!
 * @brief Get the size of the Master Transmit FIFO
 * 
 * This function returns the size of the Master Transmit FIFO, always a power of 2.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  Master Transmit FIFO Size
 */
static inline uint16_t LPI2C_Get_MasterTxFIFOSize(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->PARAM;
    tmp = (tmp & LPI2C_PARAM_MTXFIFO_MASK) >> LPI2C_PARAM_MTXFIFO_SHIFT;
    tmp = 1UL << tmp;      /* TX FIFO size = 2^MTXFIFO */
    return (uint16_t)tmp;
}


/*!
 * @brief Reset the master receive FIFO
 * 
 * This function empties the receive FIFO of the LPI2C master.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Reset_MasterRxFIFOCmd(LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_RRF_MASK));
    regValue |= LPI2C_MCR_RRF(1u);
    baseAddr->MCR = (uint32_t)regValue;
}


/*!
 * @brief Reset the master transmit FIFO
 * 
 * This function empties the transmit FIFO of the LPI2C master. 
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Reset_MasterTxFIFOCmd(LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_RTF_MASK));
    regValue |= LPI2C_MCR_RTF(1u);
    baseAddr->MCR = (uint32_t)regValue;
}

/*!
 * @brief Set/clear the master reset command
 *
 * Calling this function with enable parameter set to true resets all internal 
 * master logic and registers, except the Master Control Register. The reset state 
 * persists until this function is called with enable parameter set to false.
 *
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies the reset state of the LPI2C master logic
 */
static inline void LPI2C_Set_MasterSoftwareReset(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_RST_MASK));
    regValue |= LPI2C_MCR_RST(enable);
    baseAddr->MCR = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable the LPI2C master
 * 
 * This function enables or disables the LPI2C module in master mode. If the module 
 * is enabled, the transmit FIFO  is not empty and the I2C bus is idle, then 
 * the LPI2C master will immediately initiate a transfer on the I2C bus.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable the LPI2C master
 */
static inline void LPI2C_Set_MasterEnable(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(LPI2C_MCR_MEN_MASK));
    regValue |= LPI2C_MCR_MEN(enable);
    baseAddr->MCR = (uint32_t)regValue;
}

/*!
 * @brief Indicate the availability of receive data
 * 
 * This function returns true when the number of words in the receive FIFO is greater 
 * than the receive FIFO watermark. See function LPI2C_MasterSetRxFIFOWatermark()
 * for configuring the receive FIFO watermark.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  receive data ready/not ready
 */
static inline bool LPI2C_Get_MasterReceiveDataReadyEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue = (regValue & LPI2C_MSR_RDF_MASK) >> LPI2C_MSR_RDF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Indicate if the LPI2C master requests more data
 * 
 * This function returns true when the number of words in the transmit FIFO is equal 
 * or less than the transmit FIFO watermark. See function LPI2C_Set_MasterTxFIFOWatermark()
 * for configuring the transmit FIFO watermark.
 *
 * @param baseAddr  base address of the LPI2C module
 * @return  transmit data requested/not requested
 */
static inline bool LPI2C_Get_MasterTransmitDataRequestEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue = (regValue & LPI2C_MSR_TDF_MASK) >> LPI2C_MSR_TDF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the occurrence of a FIFO error event
 * 
 * This function returns true if the LPI2C master detects an attempt to send or 
 * receive data without first generating a (repeated) START condition. This can 
 * occur if the transmit FIFO underflows when the AUTOSTOP bit is set. When this 
 * flag is set, the LPI2C master will send a STOP condition (if busy) and will 
 * not initiate a new START condition until this flag has been cleared.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a FIFO error event
 */
static inline bool LPI2C_Get_MasterFIFOErrorEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue = (regValue & LPI2C_MSR_FEF_MASK) >> LPI2C_MSR_FEF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the occurrence of an arbitration lost event
 * 
 * This function returns true if the LPI2C master detects an arbitration lost
 * condition, as defined by the I2C standard. When this flag sets, the LPI2C 
 * master will release the bus (go idle) and will not initiate a new START 
 * condition until this flag has been cleared.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of an arbitration lost event
 */
static inline bool LPI2C_Get_MasterArbitrationLostEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue = (regValue & LPI2C_MSR_ALF_MASK) >> LPI2C_MSR_ALF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the occurrence of an unexpected NACK event
 * 
 * This function returns true if the LPI2C master detects a NACK when 
 * transmitting an address or data. If a NACK is expected for a given address 
 * (as configured by the command word) then the flag will set if a NACK is not
 * generated. When set, the master will transmit a STOP condition and will not 
 * initiate a new START condition until this flag has been cleared.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of an unexpected NACK event
 */
static inline bool LPI2C_Get_MasterNACKDetectEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->MSR;
    regValue = (regValue & LPI2C_MSR_NDF_MASK) >> LPI2C_MSR_NDF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Clear the FIFO error event flag
 * 
 * This function clears the FIFO error event. This event must be cleared before 
 * the LPI2C master can initiate a START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_MasterFIFOErrorEvent(LPI2C_Type *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << LPI2C_MSR_FEF_SHIFT);
}

/*!
 * @brief Clear the arbitration lost event flag
 * 
 * This function clears the arbitration lost event. This event must be cleared 
 * before the LPI2C master can initiate a START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_MasterArbitrationLostEvent(LPI2C_Type *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << LPI2C_MSR_ALF_SHIFT);
}

/*!
 * @brief Clear the unexpected NACK event flag
 * 
 * This function clears the unexpected NACK event. This event must be cleared 
 * before the LPI2C master can initiate a START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_MasterNACKDetectEvent(LPI2C_Type *baseAddr)
{
    baseAddr->MSR = ((uint32_t)1U << LPI2C_MSR_NDF_SHIFT);
}

/*!
 * @brief Enable/disable receive data DMA requests
 * 
 * This function enables or disables generation of Rx DMA requests when data
 * can be read from the receive FIFO, as configured by the receive FIFO watermark.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable DMA requests
 */
static inline void LPI2C_Set_MasterRxDMA(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MDER;
    regValue &= (uint32_t)(~(LPI2C_MDER_RDDE_MASK));
    regValue |= LPI2C_MDER_RDDE(enable);
    baseAddr->MDER = (uint32_t)regValue;
}

/*!
 * @brief Enable/disable transmit data DMA requests
 * 
 * This function enables or disables generation of Tx DMA requests when data
 * can be written to the transmit FIFO, as configured by the transmit FIFO watermark.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable DMA requests
 */
static inline void LPI2C_Set_MasterTxDMA(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MDER;
    regValue &= (uint32_t)(~(LPI2C_MDER_TDDE_MASK));
    regValue |= LPI2C_MDER_TDDE(enable);
    baseAddr->MDER = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable specified LPI2C master interrupts
 * 
 * This function can enable or disable one or more master interrupt sources 
 * specified by the interrupts parameter.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param interrupts  interrupts to be enabled or disabled; 
 *  must be a bitwise or between one or more of the following constants: 
 *  - LPI2C_MASTER_DATA_MATCH_INT          - Data Match Interrupt
 *  - LPI2C_MASTER_PIN_LOW_TIMEOUT_INT     - Pin Low Timeout Interrupt
 *  - LPI2C_MASTER_FIFO_ERROR_INT          - FIFO Error Interrupt
 *  - LPI2C_MASTER_ARBITRATION_LOST_INT    - Arbitration Lost Interrupt
 *  - LPI2C_MASTER_NACK_DETECT_INT         - NACK Detect Interrupt
 *  - LPI2C_MASTER_STOP_DETECT_INT         - STOP Detect Interrupt
 *  - LPI2C_MASTER_END_PACKET_INT          - End Packet Interrupt
 *  - LPI2C_MASTER_RECEIVE_DATA_INT        - Receive Data Interrupt
 *  - LPI2C_MASTER_TRANSMIT_DATA_INT       - Transmit Data Interrupt
 * @param enable  specifies whether to enable or disable specified interrupts
 */
static inline void LPI2C_Set_MasterInt(LPI2C_Type *baseAddr, uint32_t interrupts, bool enable)
{
    uint32_t tmp = baseAddr->MIER;

    if (enable == true)
    {
        tmp |= interrupts;
    }
    else
    {
        tmp &= ~interrupts;
    }
    baseAddr->MIER = tmp;
}

/*!
 * @brief Set the pin mode of the module
 * 
 * This function sets the pin mode of the module. See type lpi2c_pin_config_t for 
 * a description of available modes.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param configuration  pin mode of the module
 */
static inline void LPI2C_Set_MasterPinConfig(LPI2C_Type *baseAddr, lpi2c_pin_config_t configuration)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp &= ~(LPI2C_MCFGR1_PINCFG_MASK);
    tmp |= LPI2C_MCFGR1_PINCFG(configuration);
    baseAddr->MCFGR1 = tmp;
}

/*!
 * @brief Configure the reaction of the module on NACK reception
 * 
 * This function configures how the LPI2C master reacts when receiving a NACK. NACK responses can 
 * be treated normally or ignored. In Ultra-Fast mode it is necessary to configure the module to 
 * ignore NACK responses.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param configuration  set reaction of the module on NACK reception
 */
static inline void LPI2C_Set_MasterNACKConfig(LPI2C_Type *baseAddr, lpi2c_nack_config_t configuration)
{
    uint32_t regValue = (uint32_t)baseAddr->MCFGR1;
    regValue &= (uint32_t)(~(LPI2C_MCFGR1_IGNACK_MASK));
    regValue |= LPI2C_MCFGR1_IGNACK(configuration);
    baseAddr->MCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Configure the LPI2C master prescaler
 * 
 * This function configures the clock prescaler used for all LPI2C master logic, 
 * except the digital glitch filters.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param prescaler  LPI2C master prescaler
 */
static inline void LPI2C_Set_MasterPrescaler(LPI2C_Type *baseAddr, lpi2c_master_prescaler_t prescaler)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp &= ~(LPI2C_MCFGR1_PRESCALE_MASK);
    tmp |= LPI2C_MCFGR1_PRESCALE(prescaler);
    baseAddr->MCFGR1 = tmp;
}

/*!
 * @brief Return the LPI2C master prescaler
 * 
 * This function returns the currently configured clock prescaler.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  LPI2C master prescaler
 */
static inline lpi2c_master_prescaler_t LPI2C_Get_MasterPrescaler(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MCFGR1;
    tmp = (tmp & LPI2C_MCFGR1_PRESCALE_MASK) >> LPI2C_MCFGR1_PRESCALE_SHIFT;
    return (lpi2c_master_prescaler_t)tmp;
}

/*!
 * @brief Return the configured minimum clock high period
 * 
 * This function returns the currently configured value for clock high period.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  minimum clock high period
 */
static inline uint8_t LPI2C_Get_MasterClockHighPeriod(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp = (tmp & LPI2C_MCCR0_CLKHI_MASK) >> LPI2C_MCCR0_CLKHI_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Set the minimum clock high period
 * 
 * This function configures the minimum number of cycles (minus one) that the 
 * SCL clock is driven high by the master. The SCL high time is extended by the 
 * time it takes to detect a rising edge on the external SCL pin. Ignoring any 
 * additional board delay due to external loading, this is equal to 
 * (2 + FILTSCL) / 2^PRESCALE cycles.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  minimum clock high period
 */
static inline void LPI2C_Set_MasterClockHighPeriod(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(LPI2C_MCCR0_CLKHI_MASK);
    tmp |= LPI2C_MCCR0_CLKHI(value);
    baseAddr->MCCR0 = tmp;
}

/*!
 * @brief Set the data hold time for SDA
 * 
 * This function sets the minimum number of cycles (minus one) that is used as the 
 * data hold time for SDA. Must be configured less than the minimum SCL low period.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  value of the data hold time for SDA
 */
static inline void LPI2C_Set_MasterDataValidDelay(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(LPI2C_MCCR0_DATAVD_MASK);
    tmp |= LPI2C_MCCR0_DATAVD(value);
    baseAddr->MCCR0 = tmp;
}

/*!
 * @brief Set the setup and hold delay for a START / STOP condition
 * 
 * This function configures the Minimum number of cycles (minus one) that is used 
 * by the master as the setup and hold time for a (repeated) START condition and setup 
 * time for a STOP condition. The setup time is extended by the time it takes to detect 
 * a rising edge on the external SCL pin. Ignoring any additional board delay due to 
 * external loading, this is equal to (2 + FILTSCL) / 2^PRESCALE cycles.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  setup and hold time for a START / STOP condition
 */
static inline void LPI2C_Set_MasterSetupHoldDelay(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(LPI2C_MCCR0_SETHOLD_MASK);
    tmp |= LPI2C_MCCR0_SETHOLD(value);
    baseAddr->MCCR0 = tmp;
}

/*!
 * @brief Set the minimum clock low period
 * 
 * This function configures the minimum number of cycles (minus one) that the 
 * SCL clock is driven low by the master. This value is also used for the 
 * minimum bus free time between a STOP and a START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  minimum clock low period
 */
static inline void LPI2C_Set_MasterClockLowPeriod(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp &= ~(LPI2C_MCCR0_CLKLO_MASK);
    tmp |= LPI2C_MCCR0_CLKLO(value);
    baseAddr->MCCR0 = tmp;
}

/*!
 * @brief Return the configured minimum clock low period
 * 
 * This function returns the currently configured value for clock low period.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  minimum clock low period
 */
static inline uint8_t LPI2C_Get_MasterClockLowPeriod(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR0;
    tmp = (tmp & LPI2C_MCCR0_CLKLO_MASK) >> LPI2C_MCCR0_CLKLO_SHIFT;
    return (uint8_t)tmp;
}

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the data hold time for SDA in high-speed mode
 * 
 * This function sets the minimum number of cycles (minus one) that is used as the 
 * data hold time for SDA in High-Speed mode. Must be configured less than the 
 * minimum SCL low period.
 * This setting only has effect during High-Speed mode transfers.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  value of the data hold time for SDA
 */
static inline void LPI2C_Set_MasterDataValidDelayHS(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp &= ~(LPI2C_MCCR1_DATAVD_MASK);
    tmp |= LPI2C_MCCR1_DATAVD(value);
    baseAddr->MCCR1 = tmp;
}
#endif

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the setup and hold time for a START / STOP condition in high-speed mode
 * 
 * This function configures the Minimum number of cycles (minus one) that is used 
 * by the master as the setup and hold time for a (repeated) START condition and setup 
 * time for a STOP condition. The setup time is extended by the time it takes to detect 
 * a rising edge on the external SCL pin. Ignoring any additional board delay due to 
 * external loading, this is equal to (2 + FILTSCL) / 2^PRESCALE cycles.
 * This setting only has effect during High-Speed mode transfers.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  setup and hold time for a START / STOP condition
 */
static inline void LPI2C_Set_MasterSetupHoldDelayHS(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp &= ~(LPI2C_MCCR1_SETHOLD_MASK);
    tmp |= LPI2C_MCCR1_SETHOLD(value);
    baseAddr->MCCR1 = tmp;
}
#endif

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the minimum clock high period in high-speed mode
 * 
 * This function configures the minimum number of cycles (minus one) that the 
 * SCL clock is driven high by the master. The SCL high time is extended by the 
 * time it takes to detect a rising edge on the external SCL pin. Ignoring any 
 * additional board delay due to external loading, this is equal to 
 * (2 + FILTSCL) / 2^PRESCALE cycles.
 * This setting only has effect during High-Speed mode transfers.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  minimum clock high period
 */
static inline void LPI2C_Set_MasterClockHighPeriodHS(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp &= ~(LPI2C_MCCR1_CLKHI_MASK);
    tmp |= LPI2C_MCCR1_CLKHI(value);
    baseAddr->MCCR1 = tmp;
}
#endif

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Return the configured minimum clock high period in high-speed mode
 * 
 * This function returns the currently configured value for clock high period
 * in high-speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  minimum clock high period
 */
static inline uint8_t LPI2C_Get_MasterClockHighPeriodHS(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp = (tmp & LPI2C_MCCR1_CLKHI_MASK) >> LPI2C_MCCR1_CLKHI_SHIFT;
    return (uint8_t)tmp;
}
#endif

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Set the minimum clock low period in high-speed mode
 * 
 * This function configures the minimum number of cycles (minus one) that the 
 * SCL clock is driven low by the master. This value is also used for the 
 * minimum bus free time between a STOP and a START condition.
 * This setting only has effect during High-Speed mode transfers.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  minimum clock low period
 */
static inline void LPI2C_Set_MasterClockLowPeriodHS(LPI2C_Type *baseAddr, uint8_t value)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp &= ~(LPI2C_MCCR1_CLKLO_MASK);
    tmp |= LPI2C_MCCR1_CLKLO(value);
    baseAddr->MCCR1 = tmp;
}
#endif

#if(LPI2C_HAS_HIGH_SPEED_MODE)
/*!
 * @brief Return the configured minimum clock low period in high-speed mode
 * 
 * This function returns the currently configured value for clock low period
 * in high-speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  minimum clock low period
 */
static inline uint8_t LPI2C_Get_MasterClockLowPeriodHS(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MCCR1;
    tmp = (tmp & LPI2C_MCCR1_CLKLO_MASK) >> LPI2C_MCCR1_CLKLO_SHIFT;
    return (uint8_t)tmp;
}
#endif

/*!
 * @brief Set the receive FIFO watermark
 * 
 * This function configures the receive FIFO watermark. Whenever the number of words in the receive 
 * FIFO is greater than the receive FIFO watermark, a receive data ready event is generated.
 * Writing a value equal or greater than the FIFO size will be truncated.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  number of words in the receive FIFO that will cause the receive data flag to be set
 */
static inline void LPI2C_Set_MasterRxFIFOWatermark(LPI2C_Type *baseAddr, uint16_t value)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp &= ~(LPI2C_MFCR_RXWATER_MASK);
    tmp |= LPI2C_MFCR_RXWATER(value);
    baseAddr->MFCR = tmp;
}

/*!
 * @brief Return the configured receive FIFO watermark
 * 
 * This function returns the currently configured value for receive FIFO watermark
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  number of words in the receive FIFO that will cause the receive data flag to be set
 */
static inline uint16_t LPI2C_Get_MasterRxFIFOWatermark(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp = (tmp & LPI2C_MFCR_RXWATER_MASK) >> LPI2C_MFCR_RXWATER_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Set the transmit FIFO watermark
 * 
 * This function configures the transmit FIFO watermark. Whenever the number of words in the transmit 
 * FIFO is greater than the transmit FIFO watermark, a transmit data request event is generated.
 * Writing a value equal or greater than the FIFO size will be truncated.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param value  number of words in the transmit FIFO that will cause the transmit data flag to be set
 */
static inline void LPI2C_Set_MasterTxFIFOWatermark(LPI2C_Type *baseAddr, uint16_t value)
{
    uint32_t tmp = baseAddr->MFCR;
    tmp &= ~(LPI2C_MFCR_TXWATER_MASK);
    tmp |= LPI2C_MFCR_TXWATER(value);
    baseAddr->MFCR = tmp;
}

/*!
 * @brief Return the number of words in the receive FIFO
 * 
 * This function returns the number of words currently available in the receive FIFO.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  the number of words in the receive FIFO
 */
static inline uint16_t LPI2C_Get_MasterRxFIFOCount(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MFSR;
    tmp = (tmp & LPI2C_MFSR_RXCOUNT_MASK) >> LPI2C_MFSR_RXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Return the number of words in the transmit FIFO
 * 
 * This function returns the number of words currently available in the transmit FIFO.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  the number of words in the transmit FIFO
 */
static inline uint16_t LPI2C_Get_MasterTxFIFOCount(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MFSR;
    tmp = (tmp & LPI2C_MFSR_TXCOUNT_MASK) >> LPI2C_MFSR_TXCOUNT_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Provide commands and data for the LPI2C master
 * 
 * This function stores commands and data in the transmit FIFO and increments the FIFO 
 * write pointer.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param cmd  command for the LPI2C master
 * @param data  data for the LPI2C master
 */
static inline void LPI2C_Cmd_MasterTransmit(LPI2C_Type *baseAddr, lpi2c_master_command_t cmd, uint8_t data)
{
    baseAddr->MTDR = ((uint32_t)cmd << 8U) + (uint32_t)data;
}

/*!
 * @brief Return the received data
 * 
 * This function returns data received by the I2C master that has not been discarded 
 * due to data match settings or active command, and increments the FIFO read pointer.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  data received by the LPI2C master
 */
static inline uint8_t LPI2C_Get_MasterRxData(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->MRDR;
    tmp = (tmp & LPI2C_MRDR_DATA_MASK) >> LPI2C_MRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Enable or disable the LPI2C slave
 * 
 * This function enables or disables the LPI2C module in slave mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable the LPI2C slave
 */
static inline void LPI2C_Set_SlaveEnable(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCR;
    regValue &= (uint32_t)(~(LPI2C_SCR_SEN_MASK));
    regValue |= LPI2C_SCR_SEN(enable);
    baseAddr->SCR = (uint32_t)regValue;
}

/*!
 * @brief Set/clear the slave reset command
 * 
 * Calling this function with enable parameter set to true will perform a software 
 * reset of the LPI2C slave.
 *
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies the reset state of the LPI2C slave logic
 */
static inline void LPI2C_Set_SlaveSoftwareReset(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCR;
    regValue &= (uint32_t)(~(LPI2C_SCR_RST_MASK));
    regValue |= LPI2C_SCR_RST(enable);
    baseAddr->SCR = (uint32_t)regValue;
}

#if(LPI2C_HAS_ULTRA_FAST_MODE)
/*!
 * @brief Check the detection of a FIFO overflow or underflow
 * 
 * This function checks for the occurrence of a slave FIFO overflow or underflow. 
 * This event can only occur if clock stretching is disabled.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a FIFO overflow or underflow
 */
static inline bool LPI2C_Get_SlaveFIFOErrorEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_FEF_MASK) >> LPI2C_SSR_FEF_SHIFT;
    return (bool)regValue;
}
#endif

/*!
 * @brief Check the detection of a bit error
 * 
 * This function checks for the occurrence of a bit error event. This event occurs
 * if the LPI2C slave transmits a logic one and detects a logic zero on the I2C bus. The
 * slave will ignore the rest of the transfer until the next (repeated) START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a bit error
 */
static inline bool LPI2C_Get_SlaveBitErrorEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_BEF_MASK) >> LPI2C_SSR_BEF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the detection of a STOP condition
 * 
 * This function checks for the detection of a STOP condition, after the LPI2C slave 
 * matched the last address byte.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a STOP condition
 */
static inline bool LPI2C_Get_SlaveSTOPDetectEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_SDF_MASK) >> LPI2C_SSR_SDF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the detection of a repeated START condition
 * 
 * This function checks for the detection of a repeated START condition, after 
 * the LPI2C slave matched the last address byte. This event does not occur
 * when the slave first detects a START condition.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a repeated START condition
 */
static inline bool LPI2C_Get_SlaveRepeatedStartEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_RSF_MASK) >> LPI2C_SSR_RSF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the validity of the Address Status Register
 * 
 * This function checks for the detection of a valid address. The event is 
 * cleared by reading the address - see function LPI2C_Get_SlaveReceivedAddr().
 * It can also be cleared by reading the data register, when data register has 
 * been configured to allow address reads.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of the validity of the Address Status Register
 */
static inline bool LPI2C_Get_SlaveAddressValidEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_AVF_MASK) >> LPI2C_SSR_AVF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check the availability of receive data
 * 
 * This function checks for the availability of data received by the I2C slave.
 * The event is cleared by reading the received data - see function 
 * LPI2C_Get_SlaveData(). The event is not cleared by calling
 * LPI2C_Get_SlaveData() if the data register is configured to allow address
 * reads and an address valid event is active.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of receive data availability
 */
static inline bool LPI2C_Get_SlaveReceiveDataEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_RDF_MASK) >> LPI2C_SSR_RDF_SHIFT;
    return (bool)regValue;
}

/*!
 * @brief Check if transmit data is requested
 * 
 * This function checks if the LPI2C slave requests data to transmit. The
 * event is cleared by providing transmit data - see function 
 * LPI2C_Transmit_SlaveData(). The event can also be automatically cleared
 * if the LPI2C module detects a NACK or a repeated START or STOP condition
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  indication of a transmit data request
 */
static inline bool LPI2C_Get_SlaveTransmitDataEvent(const LPI2C_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SSR;
    regValue = (regValue & LPI2C_SSR_TDF_MASK) >> LPI2C_SSR_TDF_SHIFT;
    return (bool)regValue;
}

#if(LPI2C_HAS_ULTRA_FAST_MODE)
/*!
 * @brief Clear the FIFO overflow or underflow flag
 * 
 * This function clears the FIFO overflow or underflow event.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_SlaveFIFOErrorEvent(LPI2C_Type *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << LPI2C_SSR_FEF_SHIFT);
}
#endif

/*!
 * @brief Clear bit error flag
 * 
 * This function clears the bit error event.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_SlaveBitErrorEvent(LPI2C_Type *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << LPI2C_SSR_BEF_SHIFT);
}

/*!
 * @brief Clear the STOP detect flag
 * 
 * This function clears the STOP detect event.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_SlaveSTOPDetectEvent(LPI2C_Type *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << LPI2C_SSR_SDF_SHIFT);
}

/*!
 * @brief Clear the repeated START detect flag
 * 
 * This function clears the repeated START detect event.
 * 
 * @param baseAddr  base address of the LPI2C module
 */
static inline void LPI2C_Clear_SlaveRepeatedStartEvent(LPI2C_Type *baseAddr)
{
    baseAddr->SSR = ((uint32_t)1U << LPI2C_SSR_RSF_SHIFT);
}

/*!
 * @brief Enable or disable specified LPI2C slave interrupts
 * 
 * This function can enable or disable one or more slave interrupt sources 
 * specified by the interrupts parameter.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param interrupts  interrupts to be enabled or disabled; 
 *  must be a bitwise or between one or more of the following constants: 
 *  - LPI2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - LPI2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - LPI2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - LPI2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - LPI2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - LPI2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - LPI2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - LPI2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @param enable  specifies whether to enable or disable specified interrupts
 */
static inline void LPI2C_Set_SlaveInt(LPI2C_Type *baseAddr, uint32_t interrupts, bool enable)
{
    uint32_t tmp = baseAddr->SIER;

    if (enable == true)
    {
        tmp |= interrupts;
    }
    else
    {
        tmp &= ~interrupts;
    }
    baseAddr->SIER = tmp;
}

/*!
 * @brief Return the state of the specified LPI2C slave interrupt
 * 
 * This function returns the enabled/disabled state of the slave interrupt 
 * source specified by the interrupt parameter.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param interrupts  interrupt for which the check is made; 
 *  must be one of the following constants:
 *  - LPI2C_SLAVE_SMBUS_ALERT_RESPONSE  - SMBus Alert Response Interrupt
 *  - LPI2C_SLAVE_GENERAL_CALL          - General Call Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_1       - Address Match 1 Interrupt
 *  - LPI2C_SLAVE_ADDRESS_MATCH_0       - Address Match 0 Interrupt
 *  - LPI2C_SLAVE_FIFO_ERROR            - FIFO Error Interrupt
 *  - LPI2C_SLAVE_BIT_ERROR             - Bit Error Interrupt
 *  - LPI2C_SLAVE_STOP_DETECT           - STOP Detect Interrupt
 *  - LPI2C_SLAVE_REPEATED_START        - Repeated Start Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_ACK          - Transmit ACK Interrupt
 *  - LPI2C_SLAVE_ADDRESS_VALID         - Address Valid Interrupt
 *  - LPI2C_SLAVE_RECEIVE_DATA          - Receive Data Interrupt
 *  - LPI2C_SLAVE_TRANSMIT_DATA         - Transmit Data Interrupt
 * @return  enable/disable state of specified interrupt
 */
static inline bool LPI2C_Get_SlaveInt(const LPI2C_Type *baseAddr, uint32_t interrupts)
{
    uint32_t tmp = baseAddr->SIER;
    bool hasInterrupts = false;

    if ((tmp & interrupts) != (uint32_t)0U)
    {
        hasInterrupts = true;
    }

    return hasInterrupts;
}

/*!
 * @brief Enable/disable slave receive data DMA requests
 * 
 * This function enables or disables generation of Rx DMA requests when received
 * data is available.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable receive data DMA requests
 */
static inline void LPI2C_Set_SlaveRxDMA(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SDER;
    regValue &= (uint32_t)(~(LPI2C_SDER_RDDE_MASK));
    regValue |= LPI2C_SDER_RDDE(enable);
    baseAddr->SDER = (uint32_t)regValue;
}

/*!
 * @brief Enable/disable slave transmit data DMA requests
 * 
 * This function enables or disables generation of Tx DMA requests when the module
 * requires more data to transmit.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  specifies whether to enable or disable transmit data DMA requests
 */
static inline void LPI2C_Set_SlaveTxDMA(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SDER;
    regValue &= (uint32_t)(~(LPI2C_SDER_TDDE_MASK));
    regValue |= LPI2C_SDER_TDDE(enable);
    baseAddr->SDER = (uint32_t)regValue;
}

/*!
 * @brief Control address match configuration
 * 
 * This function configures the condition that will cause an address match to 
 * occur. See type lpi2c_slave_addr_config_t for a description of available options.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param configuration  configures the condition that will cause an address to match
 */
static inline void LPI2C_Set_SlaveAddrConfig(LPI2C_Type *baseAddr, lpi2c_slave_addr_config_t configuration)
{
    uint32_t tmp = baseAddr->SCFGR1;
    tmp &= ~(LPI2C_SCFGR1_ADDRCFG_MASK);
    tmp |= LPI2C_SCFGR1_ADDRCFG(configuration);
    baseAddr->SCFGR1 = tmp;
}

/*!
 * @brief Control detection of the High-speed Mode master code
 * 
 * This function enables or disables the detection of the High-speed Mode 
 * master code of slave address 0000_1XX, but does not cause an address match 
 * on this code. When set and any Hs-mode master code is detected, the slave 
 * filter and ACK stalls are disabled until the next STOP condition is detected.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  enable/disable the detection of the High-speed Mode master code
 */
static inline void LPI2C_Set_SlaveHighSpeedModeDetect(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_HSMEN_MASK));
    regValue |= LPI2C_SCFGR1_HSMEN(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Control slave behaviour when NACK is detected
 * 
 * This function controls the option to ignore received NACKs. When enabled, the 
 * LPI2C slave will continue transfers after a NACK is detected. This option is needed
 * for Ultra-Fast mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param nack_config  slave behaviour when NACK is detected
 */
static inline void LPI2C_Set_SlaveIgnoreNACK(LPI2C_Type *baseAddr, lpi2c_slave_nack_config_t nack_config)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_IGNACK_MASK));
    regValue |= LPI2C_SCFGR1_IGNACK(nack_config);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable clock stretching for the sending of the ACK bit
 * 
 * This function enables or disables SCL clock stretching during slave-transmit address 
 * byte(s) and slave-receiver address and data byte(s) to allow software to write the 
 * Transmit ACK Register before the ACK or NACK is transmitted. Clock stretching occurs 
 * when transmitting the 9th bit and is therefore not compatible with high speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  enable or disable clock stretching
 */
static inline void LPI2C_Set_SlaveACKStall(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_ACKSTALL_MASK));
    regValue |= LPI2C_SCFGR1_ACKSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable clock stretching for data transmission
 * 
 * This function enables or disables SCL clock stretching when the transmit data 
 * flag is set during a slave-transmit transfer. Clock stretching occurs following 
 * the 9th bit and is therefore compatible with high speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  enable or disable clock stretching
 */
static inline void LPI2C_Set_SlaveTXDStall(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_TXDSTALL_MASK));
    regValue |= LPI2C_SCFGR1_TXDSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable clock stretching for data reception
 * 
 * This function enables or disables SCL clock stretching when receive data flag 
 * is set during a slave-receive transfer. Clock stretching occurs following the 9th
 * bit and is therefore compatible with high speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  enable or disable clock stretching
 */
static inline void LPI2C_Set_SlaveRXStall(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_RXSTALL_MASK));
    regValue |= LPI2C_SCFGR1_RXSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Enable or disable clock stretching for valid address reception
 * 
 * This function enables or disables SCL clock stretching when the address valid 
 * flag is asserted. Clock stretching only occurs following the 9th bit and is 
 * therefore compatible with high speed mode.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param enable  enable or disable clock stretching
 */
static inline void LPI2C_Set_SlaveAddrStall(LPI2C_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->SCFGR1;
    regValue &= (uint32_t)(~(LPI2C_SCFGR1_ADRSTALL_MASK));
    regValue |= LPI2C_SCFGR1_ADRSTALL(enable);
    baseAddr->SCFGR1 = (uint32_t)regValue;
}

/*!
 * @brief Configure the ADDR0 address for slave address match
 * 
 * This function configures the ADDR0 value which is used to validate the received 
 * slave address. In 10-bit mode, the first address byte is compared to 
 * { 11110, ADDR0[10:9] } and the second address byte is compared to ADDR0[8:1]. 
 * In 7-bit mode, the address is compared to ADDR0[7:1]
 * The formula used for address validation is configured with function 
 * LPI2C_Set_SlaveAddrConfig().
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param addr  ADDR0 address for slave address match
 */
static inline void LPI2C_Set_SlaveAddr0(LPI2C_Type *baseAddr, uint16_t addr)
{
    uint32_t tmp = baseAddr->SAMR;
    tmp &= ~(LPI2C_SAMR_ADDR0_MASK);
    tmp |= LPI2C_SAMR_ADDR0(addr);
    baseAddr->SAMR = tmp;
}

/*!
 * @brief Return the received slave address
 * 
 * This function returns the received slave address. Reading the address clears 
 * the address valid event. The address can be 7-bit or 10-bit (10-bit addresses 
 * are prefixed by 11110) and includes the R/W bit in the least significant position.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  received address
 */
static inline uint16_t LPI2C_Get_SlaveReceivedAddr(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->SASR;
    tmp = (tmp & LPI2C_SASR_RADDR_MASK) >> LPI2C_SASR_RADDR_SHIFT;
    return (uint16_t)tmp;
}

/*!
 * @brief Configure the ACK/NACK transmission after a received byte
 * 
 * This function can be used to instruct the LPI2C slave whether to send an ACK or 
 * a NACK after receiving a byte. When ACK stall is enabled this function must be 
 * called after each matching address and after each received data byte. It can also 
 * be called when LPI2C Slave is disabled or idle to configure the default ACK/NACK.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param nack  specifies whether to transmit ACK or NACK
 */
static inline void LPI2C_Set_SlaveTransmitNACK(LPI2C_Type *baseAddr, lpi2c_slave_nack_transmit_t nack)
{
    uint32_t regValue = (uint32_t)baseAddr->STAR;
    regValue &= (uint32_t)(~(LPI2C_STAR_TXNACK_MASK));
    regValue |= LPI2C_STAR_TXNACK(nack);
    baseAddr->STAR = (uint32_t)regValue;
}

/*!
 * @brief Provide data for the LPI2C slave transmitter
 * 
 * This function provides one byte of data for the LPI2C slave to transmit. 
 * Calling this function clears the transmit data event.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @param data  data for the LPI2C slave transmitter
 */
static inline void LPI2C_Transmit_SlaveData(LPI2C_Type *baseAddr, uint8_t data)
{
    baseAddr->STDR = (uint32_t)data;
}

/*!
 * @brief Return the data received by the LPI2C slave receiver
 * 
 * This function returns the data received by the I2C slave.
 * Calling this function clears the receive data event.
 * 
 * @param baseAddr  base address of the LPI2C module
 * @return  data received by the LPI2C slave receiver
 */
static inline uint8_t LPI2C_Get_SlaveData(const LPI2C_Type *baseAddr)
{
    uint32_t tmp = baseAddr->SRDR;
    tmp = (tmp & LPI2C_SRDR_DATA_MASK) >> LPI2C_SRDR_DATA_SHIFT;
    return (uint8_t)tmp;
}

/*!
 * @brief Set the timeout for bus idle for Master
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START condition
 *
 * @param baseAddr  base address of the LPI2C module
 * @param timeout   bus idle timeout period. Zero means no bus idle timeout
 */
static inline void LPI2C_Set_MasterBusIdleTimeout(LPI2C_Type *baseAddr, uint16_t timeout)
{
    uint32_t tmp = baseAddr->MCFGR2;
    tmp &= ~(LPI2C_MCFGR2_BUSIDLE_MASK);
    tmp |= LPI2C_MCFGR2_BUSIDLE(timeout);
    baseAddr->MCFGR2 = tmp;
}

/*!
 * @brief Initializes the LPI2C module to a known state.
 *
 * This function initializes all the registers of the LPI2C module to 
 * their reset value.
 *
 * @param baseAddr  base address of the LPI2C module
 */
void LPI2C_Init(LPI2C_Type *baseAddr);

/*! @}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __LPI2C_HW_ACCESS_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

