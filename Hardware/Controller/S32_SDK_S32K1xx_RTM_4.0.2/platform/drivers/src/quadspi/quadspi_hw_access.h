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
 * @file quadspi_hw_access.h
 *
 */

#ifndef QUADSPI_HW_ACCESS_H
#define QUADSPI_HW_ACCESS_H

#include <stdbool.h>
#include "quadspi_driver.h"


/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * These are macros used for accessing the bit-fields from registers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially 
 * unsigned' to 'essentially Boolean'.
 * This is required by the conversion of a bit into a bool.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression 
 *(different essential type categories).
 * This is required by the conversion of a bit/bit-field of a register into boolean or an enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for working with register bit-fields which represent addresses.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long.
 * This is required for working with register bit-fields which represent addresses.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, local macro not referenced
 * Those are register manipulation macros for platform-specific bit-fields, declared in the same
 * style as in the SoC header file. They could be needed for future enhancements.
 *
 */

/*!
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

 /* Bit-fields of chip-specific MCR[SCLKCFG] field */
#define QSPI_MCR_SCLKCFG_INPUT_EN      0x80U    /* Enable input buffer of QSPI pads */
#define QSPI_MCR_SCLKCFG_CLK_MOD       0x40U    /* Quadspi Clocking mode selection  */
#define QSPI_MCR_SCLKCFG_EXT_DQS       0x20U    /* Use external DQS (HyperRAM mode) */
#define QSPI_MCR_SCLKCFG_CLK_SRC       0x10U    /* QuadSPI source clock selection   */
#define QSPI_MCR_SCLKCFG_DQS_INV_B     0x08U    /* B-side DQS invert                */
#define QSPI_MCR_SCLKCFG_DQS_SEL_B     0x04U    /* B-side DQS select                */
#define QSPI_MCR_SCLKCFG_DQS_INV_A     0x02U    /* A-side DQS invert                */
#define QSPI_MCR_SCLKCFG_DQS_SEL_A     0x01U    /* A-side DQS select                */
 /* Bit-fields of chip-specific SOCCR[SOCCFG] field */
     /* Programmable Divider Selection */
#define QuadSPI_SOCCR_PD_MASK                 0xE0000000u
#define QuadSPI_SOCCR_PD_SHIFT                29u
#define QuadSPI_SOCCR_PD(x)                   (((uint32_t)(((uint32_t)(x))<<QuadSPI_SOCCR_PD_SHIFT))&QuadSPI_SOCCR_PD_MASK)
     /* Programmable Divider Disable */
#define QuadSPI_SOCCR_PDD_MASK                0x10000000u
#define QuadSPI_SOCCR_PDD_SHIFT               28u
#define QuadSPI_SOCCR_PDD(x)                  (((uint32_t)(((uint32_t)(x))<<QuadSPI_SOCCR_PDD_SHIFT))&QuadSPI_SOCCR_PDD_MASK)

#define QuadSPI_SOCCR_DSQ_DEL_B               8u
#define QuadSPI_SOCCR_DSQ_DEL_A               0u


#define QSPI_RX_READOUT_IP   1U        /* RX Buffer content is read using the AHB Bus registers QSPI_ARDBn */
#define QSPI_RX_READOUT_AHB  0U        /* RX Buffer content is read using the IP Bus registers QSPI_RBDRn  */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/*
 * Triggers an IP transaction
 */
static inline void QSPI_DRV_IpTrigger(QuadSPI_Type *baseAddr, 
                                      uint8_t seqID,
                                      uint16_t dataSize)
{
    baseAddr->IPCR =  QuadSPI_IPCR_SEQID(seqID)
                    | QuadSPI_IPCR_IDATSZ(dataSize);
}


/*
 * Clear Rx buffer
 */
static inline void QSPI_DRV_ClearRxBuf(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_CLR_RXF_MASK;
}


/*
 * Clear Tx buffer
 */
static inline void QSPI_DRV_ClearTxBuf(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_CLR_TXF_MASK;
}


/*
 * Enable QuadSPI device
 */
static inline void QSPI_DRV_Enable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR &= ~QuadSPI_MCR_MDIS_MASK;
}


/*
 * Disable QuadSPI device
 */
static inline void QSPI_DRV_Disable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_MDIS_MASK;
}


/*
 * Enable DDR mode
 */
static inline void QSPI_DDR_Enable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_DDR_EN_MASK;
}


/*
 * Disable DDR mode
 */
static inline void QSPI_DDR_Disable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR &= ~QuadSPI_MCR_DDR_EN_MASK;
}


/*
 * Enable DQS
 */
static inline void QSPI_DQS_Enable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_DQS_EN_MASK;
}


/*
 * Disable DQS
 */
static inline void QSPI_DQS_Disable(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR &= ~QuadSPI_MCR_DQS_EN_MASK;
}


/*
 * Assert QuadSPI sw reset bits
 */
static inline void QSPI_DRV_SwResetOn(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR |= QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK;
}


/*
 * Deassert QuadSPI sw reset bits
 */
static inline void QSPI_DRV_SwResetOff(QuadSPI_Type *baseAddr)
{
    baseAddr->MCR &= ~(QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK);
}


/*
 * Configure idle values for data lines 2:3
 */
static inline void QSPI_DRV_SetIdleLineValues(QuadSPI_Type *baseAddr, 
                                              uint8_t iofa2IdleValue, 
                                              uint8_t iofa3IdleValue, 
                                              uint8_t iofb2IdleValue, 
                                              uint8_t iofb3IdleValue)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~(QuadSPI_MCR_ISD2FA_MASK | QuadSPI_MCR_ISD3FA_MASK | QuadSPI_MCR_ISD2FB_MASK | QuadSPI_MCR_ISD3FB_MASK));
    regValue |= (QuadSPI_MCR_ISD2FA(iofa2IdleValue) | 
                 QuadSPI_MCR_ISD3FA(iofa3IdleValue) | 
                 QuadSPI_MCR_ISD2FB(iofb2IdleValue) | 
                 QuadSPI_MCR_ISD3FB(iofb3IdleValue));
    baseAddr->MCR = (uint32_t)regValue;
}


/*
 * Configure serial flash endianess
 */
static inline void QSPI_DRV_SetEndianess(QuadSPI_Type *baseAddr, qspi_endianess_t endianess)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~QuadSPI_MCR_END_CFG_MASK);
    regValue |= QuadSPI_MCR_END_CFG(endianess);
    baseAddr->MCR = (uint32_t)regValue;
}


/*
 * Configure doze mode
 */
static inline void QSPI_DRV_SetDozeMode(QuadSPI_Type *baseAddr, bool enable)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~QuadSPI_MCR_DOZE_MASK);
    regValue |= QuadSPI_MCR_DOZE(enable);
    baseAddr->MCR = (uint32_t)regValue;
}


/*
 * Configure chip-specific clock options
 */
static inline void QSPI_DRV_SetClockOptions(QuadSPI_Type *baseAddr, uint8_t option)
{
    uint32_t regValue = (uint32_t)baseAddr->MCR;
    regValue &= (uint32_t)(~QuadSPI_MCR_SCLKCFG_MASK);
    regValue |= QuadSPI_MCR_SCLKCFG(option);
    baseAddr->MCR = (uint32_t)regValue;
}


/*
 * Configure chip-specific options
 */
static inline void QSPI_DRV_SetChipOptions(QuadSPI_Type *baseAddr, uint32_t option)
{
    baseAddr->SOCCR = option;
}


/*
 * Configure external flash memory map
 */
static inline void QSPI_DRV_SetMemMap(QuadSPI_Type *baseAddr, qspi_flash_side_t side, uint32_t size)
{
    if (side == QSPI_FLASH_SIDE_A)
    {
        baseAddr->SFA1AD = FEATURE_QSPI_AMBA_BASE + size + 1U;
        baseAddr->SFA2AD = FEATURE_QSPI_AMBA_BASE + size + 1U;
    }
    else /* side == QSPI_FLASH_SIDE_B */
    {
        baseAddr->SFA1AD = FEATURE_QSPI_AMBA_BASE;
        baseAddr->SFA2AD = FEATURE_QSPI_AMBA_BASE;
    }
    baseAddr->SFB1AD = FEATURE_QSPI_AMBA_BASE + size + 1U;
    baseAddr->SFB2AD = FEATURE_QSPI_AMBA_BASE + size + 1U;
}


/*
 * Set CS hold time in serial clock cycles
 */
static inline void QSPI_DRV_SetCsHoldTime(QuadSPI_Type *baseAddr, uint8_t cycles)
{
    uint32_t regValue = (uint32_t)baseAddr->FLSHCR;
    regValue &= (uint32_t)(~(QuadSPI_FLSHCR_TCSH_MASK));
    regValue |= QuadSPI_FLSHCR_TCSH(cycles);
    baseAddr->FLSHCR = (uint32_t)regValue;
}


/*
 * Set CS setup time
 */
static inline void QSPI_DRV_SetCsSetupTime(QuadSPI_Type *baseAddr, uint8_t cycles)
{
    uint32_t regValue = (uint32_t)baseAddr->FLSHCR;
    regValue &= (uint32_t)(~(QuadSPI_FLSHCR_TCSS_MASK));
    regValue |= QuadSPI_FLSHCR_TCSS(cycles);
    baseAddr->FLSHCR = (uint32_t)regValue;
}


/*
 * Sets AHB buffer 0 configuration
 */
static inline void QSPI_DRV_SetAhbBuf0(QuadSPI_Type *baseAddr, 
                                       uint16_t size,
                                       uint8_t master,
                                       bool highPriority)
{
    baseAddr->BUF0CR =  QuadSPI_BUF0CR_ADATSZ(size >> 3U)
                      | QuadSPI_BUF0CR_MSTRID(master)
                      | QuadSPI_BUF0CR_HP_EN(highPriority);
}


/*
 * Sets AHB buffer 1 configuration
 */
static inline void QSPI_DRV_SetAhbBuf1(QuadSPI_Type *baseAddr, 
                                       uint16_t size,
                                       uint8_t master)
{
    baseAddr->BUF1CR =  QuadSPI_BUF1CR_ADATSZ(size >> 3U)
                      | QuadSPI_BUF1CR_MSTRID(master);
}


/*
 * Sets AHB buffer 2 configuration
 */
static inline void QSPI_DRV_SetAhbBuf2(QuadSPI_Type *baseAddr, 
                                       uint16_t size,
                                       uint8_t master)
{
    baseAddr->BUF2CR =  QuadSPI_BUF2CR_ADATSZ(size >> 3U)
                      | QuadSPI_BUF2CR_MSTRID(master);
}


/*
 * Sets AHB buffer 3 configuration
 */
static inline void QSPI_DRV_SetAhbBuf3(QuadSPI_Type *baseAddr, 
                                       uint16_t size,
                                       uint8_t master,
                                       bool allMasters)
{
    baseAddr->BUF3CR =  QuadSPI_BUF3CR_ADATSZ(size >> 3U)
                      | QuadSPI_BUF3CR_MSTRID(master)
                      | QuadSPI_BUF3CR_ALLMST(allMasters);
}


/*
 * Sets AHB buffer 0 index. Parameter represents desired end index of the buffer.
 */
static inline void QSPI_DRV_SetAhbBuf0Ind(QuadSPI_Type *baseAddr, 
                                          uint32_t index)
{
    baseAddr->BUF0IND =  index;
}


/*
 * Sets AHB buffer 1 index. Parameter represents desired end index of the buffer.
 */
static inline void QSPI_DRV_SetAhbBuf1Ind(QuadSPI_Type *baseAddr, 
                                          uint32_t index)
{
    baseAddr->BUF1IND =  index;
}


/*
 * Sets AHB buffer 2 index. Parameter represents desired end index of the buffer.
 */
static inline void QSPI_DRV_SetAhbBuf2Ind(QuadSPI_Type *baseAddr, 
                                          uint32_t index)
{
    baseAddr->BUF2IND =  index;
}


/*
 * Sets address for IP transactions
 */
static inline void QSPI_DRV_SetIpAddr(QuadSPI_Type *baseAddr, 
                                      uint32_t addr)
{
    baseAddr->SFAR = FEATURE_QSPI_AMBA_BASE + addr;
}


/*
 * Sets flash address options
 */
static inline void QSPI_DRV_SetAddrOptions(QuadSPI_Type *baseAddr, 
                                           uint32_t columnAddr,
                                           bool wordAdressable)
{
    baseAddr->SFACR = QuadSPI_SFACR_CAS(columnAddr)
                    | QuadSPI_SFACR_WA(wordAdressable);
}


/*
 * Configures parameters related to sampling Rx data
 */
static inline void QSPI_DRV_SetRxCfg(QuadSPI_Type *baseAddr, 
                                     qspi_sample_delay_t delay,
                                     qspi_sample_phase_t clockPhase)
{
    baseAddr->SMPR = QuadSPI_SMPR_FSPHS(clockPhase)
                   | QuadSPI_SMPR_FSDLY(delay);
}


/*
 * Checks if module is busy with a transaction
 */
static inline bool QSPI_DRV_GetBusyStatus(const QuadSPI_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SR;
    regValue = (regValue & QuadSPI_SR_BUSY_MASK) >> QuadSPI_SR_BUSY_SHIFT;
    return (bool)regValue;
}


/*
 * Returns the current fill level of the Rx buffer
 */
static inline uint32_t QSPI_DRV_GetRxBufFill(const QuadSPI_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->RBSR;
    regValue = (regValue & QuadSPI_RBSR_RDBFL_MASK) >> QuadSPI_RBSR_RDBFL_SHIFT;
    return regValue;
}


/*
 * Checks if enough Rx data is available, according to the watermark setting
 */
static inline bool QSPI_DRV_GetRxDataEvent(const QuadSPI_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SR;
    regValue = (regValue & QuadSPI_SR_RXWE_MASK) >> QuadSPI_SR_RXWE_SHIFT;
    return (bool)regValue;
}


/*
 * Returns Tx buffer fill level expressed in 4-byte entries
 */
static inline uint32_t QSPI_DRV_GetTxBufFill(const QuadSPI_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->TBSR;
    regValue = (regValue & QuadSPI_TBSR_TRBFL_MASK) >> QuadSPI_TBSR_TRBFL_SHIFT;
    return regValue;
}


/*
 * Checks the Tx buffer watermark. 
 * Returns true if number of buffer entries specified by the watermark is available.
 */
static inline bool QSPI_DRV_GetTxWatermarkAvailable(const QuadSPI_Type *baseAddr)
{
    uint32_t regValue = (uint32_t)baseAddr->SR;
    regValue = (regValue & QuadSPI_SR_TXWA_MASK) >> QuadSPI_SR_TXWA_SHIFT;
    return (bool)regValue;
}


/*
 * Writes data in the Tx buffer
 */
static inline void QSPI_DRV_WriteTxData(QuadSPI_Type *baseAddr, uint32_t data)
{
    baseAddr->TBDR = data;
}


/*
 * Returns the address of the Tx data register
 */
static inline uint32_t QSPI_DRV_GetTxDataAddr(const QuadSPI_Type *baseAddr)
{
    return (uint32_t)&(baseAddr->TBDR);
}


/*
 * Returns the address of the first Rx data register
 */
static inline uint32_t QSPI_DRV_GetRxDataAddr(const QuadSPI_Type *baseAddr)
{
    return (uint32_t)&(baseAddr->RBDR[0U]);
}


/*
 * Enables Tx DMA request (when Tx buffer has room for more data)
 */
static inline void QSPI_DRV_EnableTxDmaReq(QuadSPI_Type *baseAddr)
{
    baseAddr->RSER |= QuadSPI_RSER_TBFDE_MASK;
}


/*
 * Enables Rx DMA request (when Rx buffer has room for more data)
 */
static inline void QSPI_DRV_EnableRxDmaReq(QuadSPI_Type *baseAddr)
{
    baseAddr->RSER |= QuadSPI_RSER_RBDDE_MASK;
}


/*
 * Disables both Rx and Tx DMA requests
 */
static inline void QSPI_DRV_DisableDmaReq(QuadSPI_Type *baseAddr)
{
    baseAddr->RSER &= ~(QuadSPI_RSER_TBFDE_MASK | QuadSPI_RSER_RBDDE_MASK);
}


/*
 * Perform a POP operation on the Rx buffer, removing Rx_watermark entries
 */
static inline void QSPI_DRV_RxPop(QuadSPI_Type *baseAddr)
{
    baseAddr->FR = QuadSPI_FR_RBDF_MASK;
}


/*
 * Configures the watermark for the Rx buffer, expressed in number of 4-byte entries
 */
static inline void QSPI_DRV_SetRxWatermark(QuadSPI_Type *baseAddr,
                                           uint8_t watermark)
{
    uint32_t regValue = (uint32_t)baseAddr->RBCT;
    regValue &= (uint32_t)(~(QuadSPI_RBCT_WMRK_MASK));
    regValue |= QuadSPI_RBCT_WMRK(watermark - 1U);
    baseAddr->RBCT = (uint32_t)regValue;
}


/*
 * Configures the rx for the Rx buffer, expressed in number of 4-byte entries
 */
static inline void QSPI_DRV_SetRxBufReadout(QuadSPI_Type *baseAddr,
                                            uint8_t readout)
{
    uint32_t regValue = (uint32_t)baseAddr->RBCT;
    regValue &= (uint32_t)(~(QuadSPI_RBCT_RXBRD_MASK));
    regValue |= QuadSPI_RBCT_RXBRD(readout);
    baseAddr->RBCT = (uint32_t)regValue;
}


/*
 * Configures the watermark for the Tx buffer, expressed in number of 4-byte entries
 */
static inline void QSPI_DRV_SetTxWatermark(QuadSPI_Type *baseAddr,
                                           uint8_t watermark)
{
    uint32_t regValue = (uint32_t)baseAddr->TBCT;
    regValue &= (uint32_t)(~(QuadSPI_TBCT_WMRK_MASK));
    regValue |= QuadSPI_TBCT_WMRK(watermark);
    baseAddr->TBCT = (uint32_t)regValue;
}


/*
 * Enables interrupts specified by the mask parameter
 */
static inline void QSPI_DRV_EnableInt(QuadSPI_Type *baseAddr,
                                      uint32_t mask)
{
    baseAddr->RSER |= mask;
}


/*
 * Disables interrupts specified by the mask parameter
 */
static inline void QSPI_DRV_DisableInt(QuadSPI_Type *baseAddr,
                                       uint32_t mask)
{
    baseAddr->RSER &= ~mask;
}


/*
 * Clears interrupt flags specified by the mask parameter
 */
static inline void QSPI_DRV_ClearIntFlag(QuadSPI_Type *baseAddr,
                                         uint32_t mask)
{
    baseAddr->FR = mask;
}


#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* QUADSPI_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
