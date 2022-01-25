/*
 * Copyright 2017-2018, 2021 NXP
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
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.7, Symbol 'inst' not referenced
 * Symbol using depends on ifdef
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced
 * These macro are used by user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 4.9, Function-like macro defined
 * Macro must be redefined for portability between different mcu header files.
 */
#ifndef SAI_HW_ACCESS_H_
#define SAI_HW_ACCESS_H_

#include "device_registers.h"

#define SAI_FIFO_SIZE 4

/* Redefine register access for use with Calypso */
#ifdef I2S_0_BASE
    #define SAI_BASE_PTRS          I2S_BASE_PTRS
    #define SAI_INSTANCE_COUNT     I2S_INSTANCE_COUNT
    #define SAI_Type               I2S_Type
    #define SAI_TX_IRQS            I2S_TX_IRQS
    #define SAI_RX_IRQS            I2S_RX_IRQS
    /* TCSR  Bit Fields */
    #define SAI_TCSR_FRDE_MASK     I2S_TCSR_FRDE_MASK
    #define SAI_TCSR_FRDE_SHIFT    I2S_TCSR_FRDE_SHIFT
    #define SAI_TCSR_FRDE_WIDTH    I2S_TCSR_FRDE_WIDTH
    #define SAI_TCSR_FRDE(x)       I2S_TCSR_FRDE(x)
    #define SAI_TCSR_FWDE_MASK     I2S_TCSR_FWDE_MASK
    #define SAI_TCSR_FWDE_SHIFT    I2S_TCSR_FWDE_SHIFT
    #define SAI_TCSR_FWDE_WIDTH    I2S_TCSR_FWDE_WIDTH
    #define SAI_TCSR_FWDE(x)       I2S_TCSR_FWDE(x)
    #define SAI_TCSR_FRIE_MASK     I2S_TCSR_FRIE_MASK
    #define SAI_TCSR_FRIE_SHIFT    I2S_TCSR_FRIE_SHIFT
    #define SAI_TCSR_FRIE_WIDTH    I2S_TCSR_FRIE_WIDTH
    #define SAI_TCSR_FRIE(x)       I2S_TCSR_FRIE(x)
    #define SAI_TCSR_FWIE_MASK     I2S_TCSR_FWIE_MASK
    #define SAI_TCSR_FWIE_SHIFT    I2S_TCSR_FWIE_SHIFT
    #define SAI_TCSR_FWIE_WIDTH    I2S_TCSR_FWIE_WIDTH
    #define SAI_TCSR_FWIE(x)       I2S_TCSR_FWIE(x)
    #define SAI_TCSR_FEIE_MASK     I2S_TCSR_FEIE_MASK
    #define SAI_TCSR_FEIE_SHIFT    I2S_TCSR_FEIE_SHIFT
    #define SAI_TCSR_FEIE_WIDTH    I2S_TCSR_FEIE_WIDTH
    #define SAI_TCSR_FEIE(x)       I2S_TCSR_FEIE(x)
    #define SAI_TCSR_SEIE_MASK     I2S_TCSR_SEIE_MASK
    #define SAI_TCSR_SEIE_SHIFT    I2S_TCSR_SEIE_SHIFT
    #define SAI_TCSR_SEIE_WIDTH    I2S_TCSR_SEIE_WIDTH
    #define SAI_TCSR_SEIE(x)       I2S_TCSR_SEIE(x)
    #define SAI_TCSR_WSIE_MASK     I2S_TCSR_WSIE_MASK
    #define SAI_TCSR_WSIE_SHIFT    I2S_TCSR_WSIE_SHIFT
    #define SAI_TCSR_WSIE_WIDTH    I2S_TCSR_WSIE_WIDTH
    #define SAI_TCSR_WSIE(x)       I2S_TCSR_WSIE(x)
    #define SAI_TCSR_FRF_MASK      I2S_TCSR_FRF_MASK
    #define SAI_TCSR_FRF_SHIFT     I2S_TCSR_FRF_SHIFT
    #define SAI_TCSR_FRF_WIDTH     I2S_TCSR_FRF_WIDTH
    #define SAI_TCSR_FRF(x)        I2S_TCSR_FRF(x)
    #define SAI_TCSR_FWF_MASK      I2S_TCSR_FWF_MASK
    #define SAI_TCSR_FWF_SHIFT     I2S_TCSR_FWF_SHIFT
    #define SAI_TCSR_FWF_WIDTH     I2S_TCSR_FWF_WIDTH
    #define SAI_TCSR_FWF(x)        I2S_TCSR_FWF(x)
    #define SAI_TCSR_FEF_MASK      I2S_TCSR_FEF_MASK
    #define SAI_TCSR_FEF_SHIFT     I2S_TCSR_FEF_SHIFT
    #define SAI_TCSR_FEF_WIDTH     I2S_TCSR_FEF_WIDTH
    #define SAI_TCSR_FEF(x)        I2S_TCSR_FEF(x)
    #define SAI_TCSR_SEF_MASK      I2S_TCSR_SEF_MASK
    #define SAI_TCSR_SEF_SHIFT     I2S_TCSR_SEF_SHIFT
    #define SAI_TCSR_SEF_WIDTH     I2S_TCSR_SEF_WIDTH
    #define SAI_TCSR_SEF(x)        I2S_TCSR_SEF(x)
    #define SAI_TCSR_WSF_MASK      I2S_TCSR_WSF_MASK
    #define SAI_TCSR_WSF_SHIFT     I2S_TCSR_WSF_SHIFT
    #define SAI_TCSR_WSF_WIDTH     I2S_TCSR_WSF_WIDTH
    #define SAI_TCSR_WSF(x)        I2S_TCSR_WSF(x)
    #define SAI_TCSR_SR_MASK       I2S_TCSR_SR_MASK
    #define SAI_TCSR_SR_SHIFT      I2S_TCSR_SR_SHIFT
    #define SAI_TCSR_SR_WIDTH      I2S_TCSR_SR_WIDTH
    #define SAI_TCSR_SR(x)         I2S_TCSR_SR(x)
    #define SAI_TCSR_FR_MASK       I2S_TCSR_FR_MASK
    #define SAI_TCSR_FR_SHIFT      I2S_TCSR_FR_SHIFT
    #define SAI_TCSR_FR_WIDTH      I2S_TCSR_FR_WIDTH
    #define SAI_TCSR_FR(x)         I2S_TCSR_FR(x)
    #define SAI_TCSR_BCE_MASK      I2S_TCSR_BCE_MASK
    #define SAI_TCSR_BCE_SHIFT     I2S_TCSR_BCE_SHIFT
    #define SAI_TCSR_BCE_WIDTH     I2S_TCSR_BCE_WIDTH
    #define SAI_TCSR_BCE(x)        I2S_TCSR_BCE(x)
    #define SAI_TCSR_DBGE_MASK     I2S_TCSR_DBGE_MASK
    #define SAI_TCSR_DBGE_SHIFT    I2S_TCSR_DBGE_SHIFT
    #define SAI_TCSR_DBGE_WIDTH    I2S_TCSR_DBGE_WIDTH
    #define SAI_TCSR_DBGE(x)       I2S_TCSR_DBGE(x)
    #define SAI_TCSR_TE_MASK       I2S_TCSR_TE_MASK
    #define SAI_TCSR_TE_SHIFT      I2S_TCSR_TE_SHIFT
    #define SAI_TCSR_TE_WIDTH      I2S_TCSR_TE_WIDTH
    #define SAI_TCSR_TE(x)         I2S_TCSR_TE(x)
    /* TCR1 Bit Fields */
    #define SAI_TCR1_TFW_MASK      I2S_TCR1_TFW_MASK
    #define SAI_TCR1_TFW_SHIFT     I2S_TCR1_TFW_SHIFT
    #define SAI_TCR1_TFW_WIDTH     I2S_TCR1_TFW_WIDTH
    #define SAI_TCR1_TFW(x)        I2S_TCR1_TFW(x)
    /* TCR2 Bit Fields */
    #define SAI_TCR2_DIV_MASK      I2S_TCR2_DIV_MASK
    #define SAI_TCR2_DIV_SHIFT     I2S_TCR2_DIV_SHIFT
    #define SAI_TCR2_DIV_WIDTH     I2S_TCR2_DIV_WIDTH
    #define SAI_TCR2_DIV(x)        I2S_TCR2_DIV(x)
    #define SAI_TCR2_BCD_MASK      I2S_TCR2_BCD_MASK
    #define SAI_TCR2_BCD_SHIFT     I2S_TCR2_BCD_SHIFT
    #define SAI_TCR2_BCD_WIDTH     I2S_TCR2_BCD_WIDTH
    #define SAI_TCR2_BCD(x)        I2S_TCR2_BCD(x)
    #define SAI_TCR2_BCP_MASK      I2S_TCR2_BCP_MASK
    #define SAI_TCR2_BCP_SHIFT     I2S_TCR2_BCP_SHIFT
    #define SAI_TCR2_BCP_WIDTH     I2S_TCR2_BCP_WIDTH
    #define SAI_TCR2_BCP(x)        I2S_TCR2_BCP(x)
    #define SAI_TCR2_MSEL_MASK     I2S_TCR2_MSEL_MASK
    #define SAI_TCR2_MSEL_SHIFT    I2S_TCR2_MSEL_SHIFT
    #define SAI_TCR2_MSEL_WIDTH    I2S_TCR2_MSEL_WIDTH
    #define SAI_TCR2_MSEL(x)       I2S_TCR2_MSEL(x)
    #define SAI_TCR2_BCI_MASK      I2S_TCR2_BCI_MASK
    #define SAI_TCR2_BCI_SHIFT     I2S_TCR2_BCI_SHIFT
    #define SAI_TCR2_BCI_WIDTH     I2S_TCR2_BCI_WIDTH
    #define SAI_TCR2_BCI(x)        I2S_TCR2_BCI(x)
    #define SAI_TCR2_SYNC_MASK     I2S_TCR2_SYNC_MASK
    #define SAI_TCR2_SYNC_SHIFT    I2S_TCR2_SYNC_SHIFT
    #define SAI_TCR2_SYNC_WIDTH    I2S_TCR2_SYNC_WIDTH
    #define SAI_TCR2_SYNC(x)       I2S_TCR2_SYNC(x)
    /* TCR3 Bit Fields */
    #define SAI_TCR3_WDFL_MASK     I2S_TCR3_WDFL_MASK
    #define SAI_TCR3_WDFL_SHIFT    I2S_TCR3_WDFL_SHIFT
    #define SAI_TCR3_WDFL_WIDTH    I2S_TCR3_WDFL_WIDTH
    #define SAI_TCR3_WDFL(x)       I2S_TCR3_WDFL(x)
    #define SAI_TCR3_TCE_MASK      I2S_TCR3_TCE_MASK
    #define SAI_TCR3_TCE_SHIFT     I2S_TCR3_TCE_SHIFT
    #define SAI_TCR3_TCE_WIDTH     I2S_TCR3_TCE_WIDTH
    #define SAI_TCR3_TCE(x)        I2S_TCR3_TCE(x)
    #define SAI_TCR3_CFR_MASK      I2S_TCR3_CFR_MASK
    #define SAI_TCR3_CFR_SHIFT     I2S_TCR3_CFR_SHIFT
    #define SAI_TCR3_CFR_WIDTH     I2S_TCR3_CFR_WIDTH
    #define SAI_TCR3_CFR(x)        I2S_TCR3_CFR(x)
    /* TCR4 Bit Fields */
    #define SAI_TCR4_FSD_MASK      I2S_TCR4_FSD_MASK
    #define SAI_TCR4_FSD_SHIFT     I2S_TCR4_FSD_SHIFT
    #define SAI_TCR4_FSD_WIDTH     I2S_TCR4_FSD_WIDTH
    #define SAI_TCR4_FSD(x)        I2S_TCR4_FSD(x)
    #define SAI_TCR4_FSP_MASK      I2S_TCR4_FSP_MASK
    #define SAI_TCR4_FSP_SHIFT     I2S_TCR4_FSP_SHIFT
    #define SAI_TCR4_FSP_WIDTH     I2S_TCR4_FSP_WIDTH
    #define SAI_TCR4_FSP(x)        I2S_TCR4_FSP(x)
    #define SAI_TCR4_ONDEM_MASK    I2S_TCR4_ONDEM_MASK
    #define SAI_TCR4_ONDEM_SHIFT   I2S_TCR4_ONDEM_SHIFT
    #define SAI_TCR4_ONDEM_WIDTH   I2S_TCR4_ONDEM_WIDTH
    #define SAI_TCR4_ONDEM(x)      I2S_TCR4_ONDEM(x)
    #define SAI_TCR4_FSE_MASK      I2S_TCR4_FSE_MASK
    #define SAI_TCR4_FSE_SHIFT     I2S_TCR4_FSE_SHIFT
    #define SAI_TCR4_FSE_WIDTH     I2S_TCR4_FSE_WIDTH
    #define SAI_TCR4_FSE(x)        I2S_TCR4_FSE(x)
    #define SAI_TCR4_MF_MASK       I2S_TCR4_MF_MASK
    #define SAI_TCR4_MF_SHIFT      I2S_TCR4_MF_SHIFT
    #define SAI_TCR4_MF_WIDTH      I2S_TCR4_MF_WIDTH
    #define SAI_TCR4_MF(x)         I2S_TCR4_MF(x)
    #define SAI_TCR4_SYWD_MASK     I2S_TCR4_SYWD_MASK
    #define SAI_TCR4_SYWD_SHIFT    I2S_TCR4_SYWD_SHIFT
    #define SAI_TCR4_SYWD_WIDTH    I2S_TCR4_SYWD_WIDTH
    #define SAI_TCR4_SYWD(x)       I2S_TCR4_SYWD(x)
    #define SAI_TCR4_FRSZ_MASK     I2S_TCR4_FRSZ_MASK
    #define SAI_TCR4_FRSZ_SHIFT    I2S_TCR4_FRSZ_SHIFT
    #define SAI_TCR4_FRSZ_WIDTH    I2S_TCR4_FRSZ_WIDTH
    #define SAI_TCR4_FRSZ(x)       I2S_TCR4_FRSZ(x)
    #define SAI_TCR4_FPACK_MASK    I2S_TCR4_FPACK_MASK
    #define SAI_TCR4_FPACK_SHIFT   I2S_TCR4_FPACK_SHIFT
    #define SAI_TCR4_FPACK_WIDTH   I2S_TCR4_FPACK_WIDTH
    #define SAI_TCR4_FPACK(x)      I2S_TCR4_FPACK(x)
    #define SAI_TCR4_FCOMB_MASK    I2S_TCR4_FCOMB_MASK
    #define SAI_TCR4_FCOMB_SHIFT   I2S_TCR4_FCOMB_SHIFT
    #define SAI_TCR4_FCOMB_WIDTH   I2S_TCR4_FCOMB_WIDTH
    #define SAI_TCR4_FCOMB(x)      I2S_TCR4_FCOMB(x)
    #define SAI_TCR4_FCONT_MASK    I2S_TCR4_FCONT_MASK
    #define SAI_TCR4_FCONT_SHIFT   I2S_TCR4_FCONT_SHIFT
    #define SAI_TCR4_FCONT_WIDTH   I2S_TCR4_FCONT_WIDTH
    #define SAI_TCR4_FCONT(x)      I2S_TCR4_FCONT(x)
    /* TCR5 Bit Fields */
    #define SAI_TCR5_FBT_MASK      I2S_TCR5_FBT_MASK
    #define SAI_TCR5_FBT_SHIFT     I2S_TCR5_FBT_SHIFT
    #define SAI_TCR5_FBT_WIDTH     I2S_TCR5_FBT_WIDTH
    #define SAI_TCR5_FBT(x)        I2S_TCR5_FBT(x)
    #define SAI_TCR5_W0W_MASK      I2S_TCR5_W0W_MASK
    #define SAI_TCR5_W0W_SHIFT     I2S_TCR5_W0W_SHIFT
    #define SAI_TCR5_W0W_WIDTH     I2S_TCR5_W0W_WIDTH
    #define SAI_TCR5_W0W(x)        I2S_TCR5_W0W(x)
    #define SAI_TCR5_WNW_MASK      I2S_TCR5_WNW_MASK
    #define SAI_TCR5_WNW_SHIFT     I2S_TCR5_WNW_SHIFT
    #define SAI_TCR5_WNW_WIDTH     I2S_TCR5_WNW_WIDTH
    #define SAI_TCR5_WNW(x)        I2S_TCR5_WNW(x)
    /* TDR Bit Fields */
    #define SAI_TDR_TDR_MASK       I2S_TDR_TDR_MASK
    #define SAI_TDR_TDR_SHIFT      I2S_TDR_TDR_SHIFT
    #define SAI_TDR_TDR_WIDTH      I2S_TDR_TDR_WIDTH
    #define SAI_TDR_TDR(x)         I2S_TDR_TDR(x)
    /* TFR Bit Fields */
    #define SAI_TFR_RFP_MASK       I2S_TFR_RFP_MASK
    #define SAI_TFR_RFP_SHIFT      I2S_TFR_RFP_SHIFT
    #define SAI_TFR_RFP_WIDTH      I2S_TFR_RFP_WIDTH
    #define SAI_TFR_RFP(x)         I2S_TFR_RFP(x)
    #define SAI_TFR_WFP_MASK       I2S_TFR_WFP_MASK
    #define SAI_TFR_WFP_SHIFT      I2S_TFR_WFP_SHIFT
    #define SAI_TFR_WFP_WIDTH      I2S_TFR_WFP_WIDTH
    #define SAI_TFR_WFP(x)         I2S_TFR_WFP(x)
    #define SAI_TFR_WCP_MASK       I2S_TFR_WCP_MASK
    #define SAI_TFR_WCP_SHIFT      I2S_TFR_WCP_SHIFT
    #define SAI_TFR_WCP_WIDTH      I2S_TFR_WCP_WIDTH
    #define SAI_TFR_WCP(x)         I2S_TFR_WCP(x)
    /* TMR Bit Fields */
    #define SAI_TMR_TWM_MASK       I2S_TMR_TWM_MASK
    #define SAI_TMR_TWM_SHIFT      I2S_TMR_TWM_SHIFT
    #define SAI_TMR_TWM_WIDTH      I2S_TMR_TWM_WIDTH
    #define SAI_TMR_TWM(x)         I2S_TMR_TWM(x)
    /* RCSR Bit Fields */
    #define SAI_RCSR_FRDE_MASK     I2S_RCSR_FRDE_MASK
    #define SAI_RCSR_FRDE_SHIFT    I2S_RCSR_FRDE_SHIFT
    #define SAI_RCSR_FRDE_WIDTH    I2S_RCSR_FRDE_WIDTH
    #define SAI_RCSR_FRDE(x)       I2S_RCSR_FRDE(x)
    #define SAI_RCSR_FWDE_MASK     I2S_RCSR_FWDE_MASK
    #define SAI_RCSR_FWDE_SHIFT    I2S_RCSR_FWDE_SHIFT
    #define SAI_RCSR_FWDE_WIDTH    I2S_RCSR_FWDE_WIDTH
    #define SAI_RCSR_FWDE(x)       I2S_RCSR_FWDE(x)
    #define SAI_RCSR_FRIE_MASK     I2S_RCSR_FRIE_MASK
    #define SAI_RCSR_FRIE_SHIFT    I2S_RCSR_FRIE_SHIFT
    #define SAI_RCSR_FRIE_WIDTH    I2S_RCSR_FRIE_WIDTH
    #define SAI_RCSR_FRIE(x)       I2S_RCSR_FRIE(x)
    #define SAI_RCSR_FWIE_MASK     I2S_RCSR_FWIE_MASK
    #define SAI_RCSR_FWIE_SHIFT    I2S_RCSR_FWIE_SHIFT
    #define SAI_RCSR_FWIE_WIDTH    I2S_RCSR_FWIE_WIDTH
    #define SAI_RCSR_FWIE(x)       I2S_RCSR_FWIE(x)
    #define SAI_RCSR_FEIE_MASK     I2S_RCSR_FEIE_MASK
    #define SAI_RCSR_FEIE_SHIFT    I2S_RCSR_FEIE_SHIFT
    #define SAI_RCSR_FEIE_WIDTH    I2S_RCSR_FEIE_WIDTH
    #define SAI_RCSR_FEIE(x)       I2S_RCSR_FEIE(x)
    #define SAI_RCSR_SEIE_MASK     I2S_RCSR_SEIE_MASK
    #define SAI_RCSR_SEIE_SHIFT    I2S_RCSR_SEIE_SHIFT
    #define SAI_RCSR_SEIE_WIDTH    I2S_RCSR_SEIE_WIDTH
    #define SAI_RCSR_SEIE(x)       I2S_RCSR_SEIE(x)
    #define SAI_RCSR_WSIE_MASK     I2S_RCSR_WSIE_MASK
    #define SAI_RCSR_WSIE_SHIFT    I2S_RCSR_WSIE_SHIFT
    #define SAI_RCSR_WSIE_WIDTH    I2S_RCSR_WSIE_WIDTH
    #define SAI_RCSR_WSIE(x)       I2S_RCSR_WSIE(x)
    #define SAI_RCSR_FRF_MASK      I2S_RCSR_FRF_MASK
    #define SAI_RCSR_FRF_SHIFT     I2S_RCSR_FRF_SHIFT
    #define SAI_RCSR_FRF_WIDTH     I2S_RCSR_FRF_WIDTH
    #define SAI_RCSR_FRF(x)        I2S_RCSR_FRF(x)
    #define SAI_RCSR_FWF_MASK      I2S_RCSR_FWF_MASK
    #define SAI_RCSR_FWF_SHIFT     I2S_RCSR_FWF_SHIFT
    #define SAI_RCSR_FWF_WIDTH     I2S_RCSR_FWF_WIDTH
    #define SAI_RCSR_FWF(x)        I2S_RCSR_FWF(x)
    #define SAI_RCSR_FEF_MASK      I2S_RCSR_FEF_MASK
    #define SAI_RCSR_FEF_SHIFT     I2S_RCSR_FEF_SHIFT
    #define SAI_RCSR_FEF_WIDTH     I2S_RCSR_FEF_WIDTH
    #define SAI_RCSR_FEF(x)        I2S_RCSR_FEF(x)
    #define SAI_RCSR_SEF_MASK      I2S_RCSR_SEF_MASK
    #define SAI_RCSR_SEF_SHIFT     I2S_RCSR_SEF_SHIFT
    #define SAI_RCSR_SEF_WIDTH     I2S_RCSR_SEF_WIDTH
    #define SAI_RCSR_SEF(x)        I2S_RCSR_SEF(x)
    #define SAI_RCSR_WSF_MASK      I2S_RCSR_WSF_MASK
    #define SAI_RCSR_WSF_SHIFT     I2S_RCSR_WSF_SHIFT
    #define SAI_RCSR_WSF_WIDTH     I2S_RCSR_WSF_WIDTH
    #define SAI_RCSR_WSF(x)        I2S_RCSR_WSF(x)
    #define SAI_RCSR_SR_MASK       I2S_RCSR_SR_MASK
    #define SAI_RCSR_SR_SHIFT      I2S_RCSR_SR_SHIFT
    #define SAI_RCSR_SR_WIDTH      I2S_RCSR_SR_WIDTH
    #define SAI_RCSR_SR(x)         I2S_RCSR_SR(x)
    #define SAI_RCSR_FR_MASK       I2S_RCSR_FR_MASK
    #define SAI_RCSR_FR_SHIFT      I2S_RCSR_FR_SHIFT
    #define SAI_RCSR_FR_WIDTH      I2S_RCSR_FR_WIDTH
    #define SAI_RCSR_FR(x)         I2S_RCSR_FR(x)
    #define SAI_RCSR_BCE_MASK      I2S_RCSR_BCE_MASK
    #define SAI_RCSR_BCE_SHIFT     I2S_RCSR_BCE_SHIFT
    #define SAI_RCSR_BCE_WIDTH     I2S_RCSR_BCE_WIDTH
    #define SAI_RCSR_BCE(x)        I2S_RCSR_BCE(x)
    #define SAI_RCSR_DBGE_MASK     I2S_RCSR_DBGE_MASK
    #define SAI_RCSR_DBGE_SHIFT    I2S_RCSR_DBGE_SHIFT
    #define SAI_RCSR_DBGE_WIDTH    I2S_RCSR_DBGE_WIDTH
    #define SAI_RCSR_DBGE(x)       I2S_RCSR_DBGE(x)
    #define SAI_RCSR_RE_MASK       I2S_RCSR_RE_MASK
    #define SAI_RCSR_RE_SHIFT      I2S_RCSR_RE_SHIFT
    #define SAI_RCSR_RE_WIDTH      I2S_RCSR_RE_WIDTH
    #define SAI_RCSR_RE(x)         I2S_RCSR_RE(x)
    /* RCR1 Bit Fields */
    #define SAI_RCR1_RFW_MASK      I2S_RCR1_RFW_MASK
    #define SAI_RCR1_RFW_SHIFT     I2S_RCR1_RFW_SHIFT
    #define SAI_RCR1_RFW_WIDTH     I2S_RCR1_RFW_WIDTH
    #define SAI_RCR1_RFW(x)        I2S_RCR1_RFW(x)
    /* RCR2 Bit Fields */
    #define SAI_RCR2_DIV_MASK      I2S_RCR2_DIV_MASK
    #define SAI_RCR2_DIV_SHIFT     I2S_RCR2_DIV_SHIFT
    #define SAI_RCR2_DIV_WIDTH     I2S_RCR2_DIV_WIDTH
    #define SAI_RCR2_DIV(x)        I2S_RCR2_DIV(x)
    #define SAI_RCR2_BCD_MASK      I2S_RCR2_BCD_MASK
    #define SAI_RCR2_BCD_SHIFT     I2S_RCR2_BCD_SHIFT
    #define SAI_RCR2_BCD_WIDTH     I2S_RCR2_BCD_WIDTH
    #define SAI_RCR2_BCD(x)        I2S_RCR2_BCD(x)
    #define SAI_RCR2_BCP_MASK      I2S_RCR2_BCP_MASK
    #define SAI_RCR2_BCP_SHIFT     I2S_RCR2_BCP_SHIFT
    #define SAI_RCR2_BCP_WIDTH     I2S_RCR2_BCP_WIDTH
    #define SAI_RCR2_BCP(x)        I2S_RCR2_BCP(x)
    #define SAI_RCR2_MSEL_MASK     I2S_RCR2_MSEL_MASK
    #define SAI_RCR2_MSEL_SHIFT    I2S_RCR2_MSEL_SHIFT
    #define SAI_RCR2_MSEL_WIDTH    I2S_RCR2_MSEL_WIDTH
    #define SAI_RCR2_MSEL(x)       I2S_RCR2_MSEL(x)
    #define SAI_RCR2_BCI_MASK      I2S_RCR2_BCI_MASK
    #define SAI_RCR2_BCI_SHIFT     I2S_RCR2_BCI_SHIFT
    #define SAI_RCR2_BCI_WIDTH     I2S_RCR2_BCI_WIDTH
    #define SAI_RCR2_BCI(x)        I2S_RCR2_BCI(x)
    #define SAI_RCR2_SYNC_MASK     I2S_RCR2_SYNC_MASK
    #define SAI_RCR2_SYNC_SHIFT    I2S_RCR2_SYNC_SHIFT
    #define SAI_RCR2_SYNC_WIDTH    I2S_RCR2_SYNC_WIDTH
    #define SAI_RCR2_SYNC(x)       I2S_RCR2_SYNC(x)
    /* RCR3 Bit Fields */
    #define SAI_RCR3_WDFL_MASK     I2S_RCR3_WDFL_MASK
    #define SAI_RCR3_WDFL_SHIFT    I2S_RCR3_WDFL_SHIFT
    #define SAI_RCR3_WDFL_WIDTH    I2S_RCR3_WDFL_WIDTH
    #define SAI_RCR3_WDFL(x)       I2S_RCR3_WDFL(x)
    #define SAI_RCR3_RCE_MASK      I2S_RCR3_RCE_MASK
    #define SAI_RCR3_RCE_SHIFT     I2S_RCR3_RCE_SHIFT
    #define SAI_RCR3_RCE_WIDTH     I2S_RCR3_RCE_WIDTH
    #define SAI_RCR3_RCE(x)        I2S_RCR3_RCE(x)
    #define SAI_RCR3_CFR_MASK      I2S_RCR3_CFR_MASK
    #define SAI_RCR3_CFR_SHIFT     I2S_RCR3_CFR_SHIFT
    #define SAI_RCR3_CFR_WIDTH     I2S_RCR3_CFR_WIDTH
    #define SAI_RCR3_CFR(x)        I2S_RCR3_CFR(x)
    /* RCR4 Bit Fields */
    #define SAI_RCR4_FSD_MASK      I2S_RCR4_FSD_MASK
    #define SAI_RCR4_FSD_SHIFT     I2S_RCR4_FSD_SHIFT
    #define SAI_RCR4_FSD_WIDTH     I2S_RCR4_FSD_WIDTH
    #define SAI_RCR4_FSD(x)        I2S_RCR4_FSD(x)
    #define SAI_RCR4_FSP_MASK      I2S_RCR4_FSP_MASK
    #define SAI_RCR4_FSP_SHIFT     I2S_RCR4_FSP_SHIFT
    #define SAI_RCR4_FSP_WIDTH     I2S_RCR4_FSP_WIDTH
    #define SAI_RCR4_FSP(x)        I2S_RCR4_FSP(x)
    #define SAI_RCR4_ONDEM_MASK    I2S_RCR4_ONDEM_MASK
    #define SAI_RCR4_ONDEM_SHIFT   I2S_RCR4_ONDEM_SHIFT
    #define SAI_RCR4_ONDEM_WIDTH   I2S_RCR4_ONDEM_WIDTH
    #define SAI_RCR4_ONDEM(x)      I2S_RCR4_ONDEM(x)
    #define SAI_RCR4_FSE_MASK      I2S_RCR4_FSE_MASK
    #define SAI_RCR4_FSE_SHIFT     I2S_RCR4_FSE_SHIFT
    #define SAI_RCR4_FSE_WIDTH     I2S_RCR4_FSE_WIDTH
    #define SAI_RCR4_FSE(x)        I2S_RCR4_FSE(x)
    #define SAI_RCR4_MF_MASK       I2S_RCR4_MF_MASK
    #define SAI_RCR4_MF_SHIFT      I2S_RCR4_MF_SHIFT
    #define SAI_RCR4_MF_WIDTH      I2S_RCR4_MF_WIDTH
    #define SAI_RCR4_MF(x)         I2S_RCR4_MF(x)
    #define SAI_RCR4_SYWD_MASK     I2S_RCR4_SYWD_MASK
    #define SAI_RCR4_SYWD_SHIFT    I2S_RCR4_SYWD_SHIFT
    #define SAI_RCR4_SYWD_WIDTH    I2S_RCR4_SYWD_WIDTH
    #define SAI_RCR4_SYWD(x)       I2S_RCR4_SYWD(x)
    #define SAI_RCR4_FRSZ_MASK     I2S_RCR4_FRSZ_MASK
    #define SAI_RCR4_FRSZ_SHIFT    I2S_RCR4_FRSZ_SHIFT
    #define SAI_RCR4_FRSZ_WIDTH    I2S_RCR4_FRSZ_WIDTH
    #define SAI_RCR4_FRSZ(x)       I2S_RCR4_FRSZ(x)
    #define SAI_RCR4_FPACK_MASK    I2S_RCR4_FPACK_MASK
    #define SAI_RCR4_FPACK_SHIFT   I2S_RCR4_FPACK_SHIFT
    #define SAI_RCR4_FPACK_WIDTH   I2S_RCR4_FPACK_WIDTH
    #define SAI_RCR4_FPACK(x)      I2S_RCR4_FPACK(x)
    #define SAI_RCR4_FCOMB_MASK    I2S_RCR4_FCOMB_MASK
    #define SAI_RCR4_FCOMB_SHIFT   I2S_RCR4_FCOMB_SHIFT
    #define SAI_RCR4_FCOMB_WIDTH   I2S_RCR4_FCOMB_WIDTH
    #define SAI_RCR4_FCOMB(x)      I2S_RCR4_FCOMB(x)
    #define SAI_RCR4_FCONT_MASK    I2S_RCR4_FCONT_MASK
    #define SAI_RCR4_FCONT_SHIFT   I2S_RCR4_FCONT_SHIFT
    #define SAI_RCR4_FCONT_WIDTH   I2S_RCR4_FCONT_WIDTH
    #define SAI_RCR4_FCONT(x)      I2S_RCR4_FCONT(x)
    /* RCR5 Bit Fields */
    #define SAI_RCR5_FBT_MASK      I2S_RCR5_FBT_MASK
    #define SAI_RCR5_FBT_SHIFT     I2S_RCR5_FBT_SHIFT
    #define SAI_RCR5_FBT_WIDTH     I2S_RCR5_FBT_WIDTH
    #define SAI_RCR5_FBT(x)        I2S_RCR5_FBT(x)
    #define SAI_RCR5_W0W_MASK      I2S_RCR5_W0W_MASK
    #define SAI_RCR5_W0W_SHIFT     I2S_RCR5_W0W_SHIFT
    #define SAI_RCR5_W0W_WIDTH     I2S_RCR5_W0W_WIDTH
    #define SAI_RCR5_W0W(x)        I2S_RCR5_W0W(x)
    #define SAI_RCR5_WNW_MASK      I2S_RCR5_WNW_MASK
    #define SAI_RCR5_WNW_SHIFT     I2S_RCR5_WNW_SHIFT
    #define SAI_RCR5_WNW_WIDTH     I2S_RCR5_WNW_WIDTH
    #define SAI_RCR5_WNW(x)        I2S_RCR5_WNW(x)
    /* RDR Bit Fields */
    #define SAI_RDR_RDR_MASK       I2S_RDR_RDR_MASK
    #define SAI_RDR_RDR_SHIFT      I2S_RDR_RDR_SHIFT
    #define SAI_RDR_RDR_WIDTH      I2S_RDR_RDR_WIDTH
    #define SAI_RDR_RDR(x)         I2S_RDR_RDR(x)
    /* RFR Bit Fields */
    #define SAI_RFR_RFP_MASK       I2S_RFR_RFP_MASK
    #define SAI_RFR_RFP_SHIFT      I2S_RFR_RFP_SHIFT
    #define SAI_RFR_RFP_WIDTH      I2S_RFR_RFP_WIDTH
    #define SAI_RFR_RFP(x)         I2S_RFR_RFP(x)
    #define SAI_RFR_RCP_MASK       I2S_RFR_RCP_MASK
    #define SAI_RFR_RCP_SHIFT      I2S_RFR_RCP_SHIFT
    #define SAI_RFR_RCP_WIDTH      I2S_RFR_RCP_WIDTH
    #define SAI_RFR_RCP(x)         I2S_RFR_RCP(x)
    #define SAI_RFR_WFP_MASK       I2S_RFR_WFP_MASK
    #define SAI_RFR_WFP_SHIFT      I2S_RFR_WFP_SHIFT
    #define SAI_RFR_WFP_WIDTH      I2S_RFR_WFP_WIDTH
    #define SAI_RFR_WFP(x)         I2S_RFR_WFP(x)
    /* RMR Bit Fields */
    #define SAI_RMR_RWM_MASK       I2S_RMR_RWM_MASK
    #define SAI_RMR_RWM_SHIFT      I2S_RMR_RWM_SHIFT
    #define SAI_RMR_RWM_WIDTH      I2S_RMR_RWM_WIDTH
    #define SAI_RMR_RWM(x)         I2S_RMR_RWM(x)
    /* MCR Bit Fields */
    #define SAI_MCR_MICS_MASK      I2S_MCR_MICS_MASK
    #define SAI_MCR_MICS_SHIFT     I2S_MCR_MICS_SHIFT
    #define SAI_MCR_MICS_WIDTH     I2S_MCR_MICS_WIDTH
    #define SAI_MCR_MICS(x)        I2S_MCR_MICS(x)
    #define SAI_MCR_MOE_MASK       I2S_MCR_MOE_MASK
    #define SAI_MCR_MOE_SHIFT      I2S_MCR_MOE_SHIFT
    #define SAI_MCR_MOE_WIDTH      I2S_MCR_MOE_WIDTH
    #define SAI_MCR_MOE(x)         I2S_MCR_MOE(x)
    #define SAI_MCR_DUF_MASK       I2S_MCR_DUF_MASK
    #define SAI_MCR_DUF_SHIFT      I2S_MCR_DUF_SHIFT
    #define SAI_MCR_DUF_WIDTH      I2S_MCR_DUF_WIDTH
    #define SAI_MCR_DUF(x)         I2S_MCR_DUF(x)
    /* MDR Bit Fields */
    #define SAI_MDR_DIVIDE_MASK    I2S_MDR_DIVIDE_MASK
    #define SAI_MDR_DIVIDE_SHIFT   I2S_MDR_DIVIDE_SHIFT
    #define SAI_MDR_DIVIDE_WIDTH   I2S_MDR_DIVIDE_WIDTH
    #define SAI_MDR_DIVIDE(x)      I2S_MDR_DIVIDE(x)
    #define SAI_MDR_FRACT_MASK     I2S_MDR_FRACT_MASK
    #define SAI_MDR_FRACT_SHIFT    I2S_MDR_FRACT_SHIFT
    #define SAI_MDR_FRACT_WIDTH    I2S_MDR_FRACT_WIDTH
    #define SAI_MDR_FRACT(x)       I2S_MDR_FRACT(x)
#endif /* I2S_0_BASE */

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxSetWatermark
 * Description   : Set fifo watermark
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxSetWatermark(SAI_Type* inst, uint8_t level)
{
    inst->RCR1 = SAI_RCR1_RFW((uint32_t) level);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_IsTxChannelEnabled
 * Description   : check if channel is enabled
 *
 *END**************************************************************************/
static inline bool SAI_DRV_IsTxChannelEnabled(const SAI_Type* inst, uint8_t channel)
{
    bool ret;

    if ((inst->TCR3 & (1UL << ((uint32_t)channel+(uint32_t)SAI_TCR3_TCE_SHIFT))) != 0UL)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_IsRxChannelEnabled
 * Description   : check if channel is enabled
 *
 *END**************************************************************************/
static inline bool SAI_DRV_IsRxChannelEnabled(const SAI_Type* inst, uint8_t channel)
{
    bool ret;

    if ((inst->RCR3 & (1UL << ((uint32_t)channel+(uint32_t)SAI_RCR3_RCE_SHIFT))) != 0UL)
    {
        ret = true;
    }
    else
    {
        ret =false;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxResetFifo
 * Description   : Reset fifo
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxResetFifo(SAI_Type* inst)
{
    uint32_t val = inst->TCSR;
    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= (1UL << SAI_TCSR_FR_SHIFT);
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxResetFifo
 * Description   : Reset fifo
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxResetFifo(SAI_Type* inst)
{
    uint32_t val = inst->RCSR;
    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= (1UL << SAI_RCSR_FR_SHIFT);
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDisableFifoReqInt
 * Description   : Disable fifo request interrupt
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxDisableFifoReqInt(SAI_Type* inst)
{
    uint32_t val = inst->TCSR;
    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* enable fifo request int */
    val &= ~(1UL << SAI_TCSR_FRIE_SHIFT);
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxClearFlag
 * Description   : Clear one of folowing flags: word start, fifo erro,
 *                 sync error
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxClearFlag (SAI_Type* inst, uint32_t shift)
{
    uint32_t val = inst->TCSR;
    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* clear fifo error flag */
    val |= 1UL << shift;
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxClearFlag
 * Description   : Clear one of folowing flags: word start, fifo erro,
 *                 sync error
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxClearFlag (SAI_Type* inst, uint32_t shift)
{
    uint32_t val = inst->RCSR;
    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* clear fifo error flag */
    val |= 1UL << shift;
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxEnableFifoReqInt
 * Description   : Enable fifo request interrupt
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxEnableFifoReqInt(SAI_Type* inst)
{
    uint32_t val = inst->RCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= 1UL << SAI_RCSR_FRIE_SHIFT;
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDisableFifoReqInt
 * Description   : Disable fifo request interrupt
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxDisableFifoReqInt(SAI_Type* inst)
{
    uint32_t val = inst->RCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* enable fifo request int */
    val &= ~(1UL << SAI_RCSR_FRIE_SHIFT);
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxEnableFifoReqDma
 * Description   : Enable fifo request dma
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxEnableFifoReqDma(SAI_Type* inst)
{
    uint32_t val = inst->RCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= 1UL << SAI_RCSR_FRDE_SHIFT;
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDisableFifoReqDma
 * Description   : Disable fifo request dma
 *
 *END**************************************************************************/
static inline void SAI_DRV_RxDisableFifoReqDma(SAI_Type* inst)
{
    uint32_t val = inst->RCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_RCSR_WSF_SHIFT) | (1UL << SAI_RCSR_SEF_SHIFT) | (1UL << SAI_RCSR_FEF_SHIFT));
    /* enable fifo request int */
    val &= ~(1UL << SAI_RCSR_FRDE_SHIFT);
    inst->RCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxEnableFifoReqInt
 * Description   : Enable fifo request interrupt
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxEnableFifoReqInt(SAI_Type* inst)
{
    uint32_t val = inst->TCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= 1UL << SAI_TCSR_FRIE_SHIFT;
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxEnableFifoReqDma
 * Description   : Enable fifo request dma
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxEnableFifoReqDma(SAI_Type* inst)
{
    uint32_t val = inst->TCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* enable fifo request int */
    val |= 1UL << SAI_TCSR_FRDE_SHIFT;
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDisableFifoReqDma
 * Description   : Disable fifo request dma
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxDisableFifoReqDma(SAI_Type* inst)
{
    uint32_t val = inst->TCSR;

    /* careful not to clear flags */
    val &= ~((1UL << SAI_TCSR_WSF_SHIFT) | (1UL << SAI_TCSR_SEF_SHIFT) | (1UL << SAI_TCSR_FEF_SHIFT));
    /* enable fifo request int */
    val &= ~(1UL << SAI_TCSR_FRDE_SHIFT);
    inst->TCSR = val;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_IsTxFifoFull
 * Description   : Check if Tx fifo is full
 *
 *END**************************************************************************/
static inline bool SAI_DRV_IsTxFifoFull(const SAI_Type* inst,
                          uint8_t channel)
{
    uint32_t wfp;
    uint32_t rfp;
    bool ret;

    wfp = (inst->TFR[channel] & SAI_TFR_WFP_MASK) >> SAI_TFR_WFP_SHIFT;
    rfp = (inst->TFR[channel] & SAI_TFR_RFP_MASK) >> SAI_TFR_RFP_SHIFT;

    /* if WFP and RFP is identical except msb then fifo is full */
    if ((wfp ^ rfp) == (1UL << (SAI_TFR_WFP_WIDTH - 1UL)))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_IsRxFifoEmpty
 * Description   : Check if Rx fifo is empty
 *
 *END**************************************************************************/
static inline bool SAI_DRV_IsRxFifoEmpty(const SAI_Type* inst,
                          uint8_t channel)
{
    uint32_t wfp;
    uint32_t rfp;
    bool ret;

    wfp = (inst->RFR[channel] & SAI_RFR_WFP_MASK) >> SAI_RFR_WFP_SHIFT;
    rfp = (inst->RFR[channel] & SAI_RFR_RFP_MASK) >> SAI_RFR_RFP_SHIFT;

    /* if WFP and RFP is identical except msb then fifo is full */
    if (wfp == rfp)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxWrite
 * Description   : Write data register once
 *
 *END**************************************************************************/
static inline void SAI_DRV_TxWrite(SAI_Type* inst,
                     uint8_t channel,
                     uint32_t data)
{
    inst->TDR[channel] = data;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxRead
 * Description   : Read data register once
 *
 *END**************************************************************************/
static inline uint32_t SAI_DRV_RxRead(const SAI_Type* inst,
                     uint8_t channel)
{
    return inst->RDR[channel];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxGetFifoErrorFlag
 * Description   : Return tx fifo error flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_TxGetFifoErrorFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->TCSR;
    return (((temp & SAI_TCSR_FEF_MASK) != 0UL) && ((temp & SAI_TCSR_FEIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxGetFifoErrorFlag
 * Description   : Return tx sync error flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_TxGetSyncErrorFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->TCSR;
    return (((temp & SAI_TCSR_SEF_MASK) != 0UL) && ((temp & SAI_TCSR_SEIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxGetWordStartFlag
 * Description   : Return tx word start flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_TxGetWordStartFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->TCSR;
    return (((temp & SAI_TCSR_WSF_MASK) != 0UL) && ((temp & SAI_TCSR_WSIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxGetFifoReqFlag
 * Description   : Return fifo request flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_TxGetFifoReqFlag(const SAI_Type* inst)
{
    return ((inst->TCSR & SAI_TCSR_FRF_MASK) != 0UL);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxGetFifoErrorFlag
 * Description   : Return rx fifo error flag
 *
 *END**************************************************************************/

static inline bool SAI_DRV_RxGetFifoErrorFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->RCSR;
    return (((temp & SAI_RCSR_FEF_MASK) != 0UL) && ((temp & SAI_RCSR_FEIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxGetFifoErrorFlag
 * Description   : Return rx sync error flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_RxGetSyncErrorFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->RCSR;
    return (((temp & SAI_RCSR_SEF_MASK) != 0UL) && ((temp & SAI_RCSR_SEIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxGetFifoReqFlag
 * Description   : Return fifo request flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_RxGetFifoReqFlag(const SAI_Type* inst)
{
    return ((inst->RCSR & SAI_RCSR_FRF_MASK) != 0UL);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxGetWordStartFlag
 * Description   : Return rx word start flag
 *
 *END**************************************************************************/
static inline bool SAI_DRV_RxGetWordStartFlag(const SAI_Type* inst)
{
    uint32_t temp = inst->RCSR;
    return (((temp & SAI_RCSR_WSF_MASK) != 0UL) && ((temp & SAI_RCSR_WSIE_MASK) != 0UL));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetParamFrameSize
 * Description   : Return frame size in param register
 *
 *END**************************************************************************/
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static inline uint8_t SAI_DRV_GetParamFrameSize(const SAI_Type* inst)
{
#ifdef FEATURE_SAI_HAS_PARAM
    return (uint8_t)((inst->PARAM & SAI_PARAM_FRAME_MASK) >> SAI_PARAM_FRAME_SHIFT);
#else
    return SAI_FIFO_SIZE;
#endif
}
#endif /* DEV_ERROR_DETECT */

#endif /* SAI_HW_ACCESS_H_ */
