/*
 * Copyright 2016-2020 NXP
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
 * @file flash_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * flash_swap_callback_t can be used in user code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to enable the use of a macro needed by
 * the user code (even if the macro is not used inside the flash driver code)
 * or driver code in the future.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * This macro is needed in creating a common name for any IP.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include "status.h"
#include "device_registers.h"

/*!
 * @defgroup flash_driver Flash Memory (Flash)
 * @ingroup flash
 * @details This section describes the programming interface of the Flash Peripheral Driver.
 * @{
 */

/*******************************************************************************
 * FTFx - Register instance definitions
 *******************************************************************************/
/*******************************************************************************
 * FTFE
 *******************************************************************************/
#ifdef FTFE
#define FTFx_BASE                               FTFE_BASE
#define FTFx_FSTAT                              FTFE->FSTAT
#define FTFx_FCNFG                              FTFE->FCNFG
#define FTFx_FSEC                               FTFE->FSEC
#define FTFx_FOPT                               FTFE->FOPT
#define FTFx_FCCOB3                             FTFE->FCCOB[0]
#define FTFx_FCCOB2                             FTFE->FCCOB[1]
#define FTFx_FCCOB1                             FTFE->FCCOB[2]
#define FTFx_FCCOB0                             FTFE->FCCOB[3]
#define FTFx_FCCOB7                             FTFE->FCCOB[4]
#define FTFx_FCCOB6                             FTFE->FCCOB[5]
#define FTFx_FCCOB5                             FTFE->FCCOB[6]
#define FTFx_FCCOB4                             FTFE->FCCOB[7]
#define FTFx_FCCOBB                             FTFE->FCCOB[8]
#define FTFx_FCCOBA                             FTFE->FCCOB[9]
#define FTFx_FCCOB9                             FTFE->FCCOB[10]
#define FTFx_FCCOB8                             FTFE->FCCOB[11]
#define FTFx_FPROT3                             FTFE->FPROT[0]
#define FTFx_FPROT2                             FTFE->FPROT[1]
#define FTFx_FPROT1                             FTFE->FPROT[2]
#define FTFx_FPROT0                             FTFE->FPROT[3]
#define FTFx_FEPROT                             FTFE->FEPROT
#define FTFx_FDPROT                             FTFE->FDPROT
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
#define FTFx_FERSTAT                            FTFE->FERSTAT
#define FTFx_FERCNFG                            FTFE->FERCNFG
#endif
/* FSTAT Bit Fields */
#define FTFx_FSTAT_MGSTAT0_MASK                 FTFE_FSTAT_MGSTAT0_MASK
#define FTFx_FSTAT_MGSTAT0_SHIFT                FTFE_FSTAT_MGSTAT0_SHIFT
#define FTFx_FSTAT_MGSTAT0_WIDTH                FTFE_FSTAT_MGSTAT0_WIDTH
#define FTFx_FSTAT_MGSTAT0(x)                   FTFE_FSTAT_MGSTAT0(x)
#define FTFx_FSTAT_FPVIOL_MASK                  FTFE_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_FPVIOL_SHIFT                 FTFE_FSTAT_FPVIOL_SHIFT
#define FTFx_FSTAT_FPVIOL_WIDTH                 FTFE_FSTAT_FPVIOL_WIDTH
#define FTFx_FSTAT_FPVIOL(x)                    FTFE_FSTAT_FPVIOL(x)
#define FTFx_FSTAT_ACCERR_MASK                  FTFE_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_ACCERR_SHIFT                 FTFE_FSTAT_ACCERR_SHIFT
#define FTFx_FSTAT_ACCERR_WIDTH                 FTFE_FSTAT_ACCERR_WIDTH
#define FTFx_FSTAT_ACCERR(x)                    FTFE_FSTAT_ACCERR(x)
#define FTFx_FSTAT_RDCOLERR_MASK                FTFE_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_RDCOLERR_SHIFT               FTFE_FSTAT_RDCOLERR_SHIFT
#define FTFx_FSTAT_RDCOLERR_WIDTH               FTFE_FSTAT_RDCOLERR_WIDTH
#define FTFx_FSTAT_RDCOLERR(x)                  FTFE_FSTAT_RDCOLERR(x)
#define FTFx_FSTAT_CCIF_MASK                    FTFE_FSTAT_CCIF_MASK
#define FTFx_FSTAT_CCIF_SHIFT                   FTFE_FSTAT_CCIF_SHIFT
#define FTFx_FSTAT_CCIF_WIDTH                   FTFE_FSTAT_CCIF_WIDTH
#define FTFx_FSTAT_CCIF(x)                      FTFE_FSTAT_CCIF(x)
/* FCNFG Bit Fields */
#define FTFx_FCNFG_EEERDY_MASK                  FTFE_FCNFG_EEERDY_MASK
#define FTFx_FCNFG_EEERDY_SHIFT                 FTFE_FCNFG_EEERDY_SHIFT
#define FTFx_FCNFG_EEERDY_WIDTH                 FTFE_FCNFG_EEERDY_WIDTH
#define FTFx_FCNFG_EEERDY(x)                    FTFE_FCNFG_EEERDY(x)
#define FTFx_FCNFG_RAMRDY_MASK                  FTFE_FCNFG_RAMRDY_MASK
#define FTFx_FCNFG_RAMRDY_SHIFT                 FTFE_FCNFG_RAMRDY_SHIFT
#define FTFx_FCNFG_RAMRDY_WIDTH                 FTFE_FCNFG_RAMRDY_WIDTH
#define FTFx_FCNFG_RAMRDY(x)                    FTFE_FCNFG_RAMRDY(x)
#define FTFx_FCNFG_PFLSH_MASK                   FTFE_FCNFG_PFLSH_MASK
#define FTFx_FCNFG_PFLSH_SHIFT                  FTFE_FCNFG_PFLSH_SHIFT
#define FTFx_FCNFG_PFLSH_WIDTH                  FTFE_FCNFG_PFLSH_WIDTH
#define FTFx_FCNFG_PFLSH(x)                     FTFE_FCNFG_PFLSH(x)
#define FTFx_FCNFG_ERSSUSP_MASK                 FTFE_FCNFG_ERSSUSP_MASK
#define FTFx_FCNFG_ERSSUSP_SHIFT                FTFE_FCNFG_ERSSUSP_SHIFT
#define FTFx_FCNFG_ERSSUSP_WIDTH                FTFE_FCNFG_ERSSUSP_WIDTH
#define FTFx_FCNFG_ERSSUSP(x)                   FTFE_FCNFG_ERSSUSP(x)
#define FTFx_FCNFG_ERSAREQ_MASK                 FTFE_FCNFG_ERSAREQ_MASK
#define FTFx_FCNFG_ERSAREQ_SHIFT                FTFE_FCNFG_ERSAREQ_SHIFT
#define FTFx_FCNFG_ERSAREQ_WIDTH                FTFE_FCNFG_ERSAREQ_WIDTH
#define FTFx_FCNFG_ERSAREQ(x)                   FTFE_FCNFG_ERSAREQ(x)
#define FTFx_FCNFG_RDCOLLIE_MASK                FTFE_FCNFG_RDCOLLIE_MASK
#define FTFx_FCNFG_RDCOLLIE_SHIFT               FTFE_FCNFG_RDCOLLIE_SHIFT
#define FTFx_FCNFG_RDCOLLIE_WIDTH               FTFE_FCNFG_RDCOLLIE_WIDTH
#define FTFx_FCNFG_RDCOLLIE(x)                  FTFE_FCNFG_RDCOLLIE(x)
#define FTFx_FCNFG_CCIE_MASK                    FTFE_FCNFG_CCIE_MASK
#define FTFx_FCNFG_CCIE_SHIFT                   FTFE_FCNFG_CCIE_SHIFT
#define FTFx_FCNFG_CCIE_WIDTH                   FTFE_FCNFG_CCIE_WIDTH
#define FTFx_FCNFG_CCIE(x)                      FTFE_FCNFG_CCIE(x)
/* FSEC Bit Fields */
#define FTFx_FSEC_SEC_MASK                      FTFE_FSEC_SEC_MASK
#define FTFx_FSEC_SEC_SHIFT                     FTFE_FSEC_SEC_SHIFT
#define FTFx_FSEC_SEC_WIDTH                     FTFE_FSEC_SEC_WIDTH
#define FTFx_FSEC_SEC(x)                        FTFE_FSEC_SEC(x)
#define FTFx_FSEC_FSLACC_MASK                   FTFE_FSEC_FSLACC_MASK
#define FTFx_FSEC_FSLACC_SHIFT                  FTFE_FSEC_FSLACC_SHIFT
#define FTFx_FSEC_FSLACC_WIDTH                  FTFE_FSEC_FSLACC_WIDTH
#define FTFx_FSEC_FSLACC(x)                     FTFE_FSEC_FSLACC(x)
#define FTFx_FSEC_MEEN_MASK                     FTFE_FSEC_MEEN_MASK
#define FTFx_FSEC_MEEN_SHIFT                    FTFE_FSEC_MEEN_SHIFT
#define FTFx_FSEC_MEEN_WIDTH                    FTFE_FSEC_MEEN_WIDTH
#define FTFx_FSEC_MEEN(x)                       FTFE_FSEC_MEEN(x)
#define FTFx_FSEC_KEYEN_MASK                    FTFE_FSEC_KEYEN_MASK
#define FTFx_FSEC_KEYEN_SHIFT                   FTFE_FSEC_KEYEN_SHIFT
#define FTFx_FSEC_KEYEN_WIDTH                   FTFE_FSEC_KEYEN_WIDTH
#define FTFx_FSEC_KEYEN(x)                      FTFE_FSEC_KEYEN(x)
/* FOPT Bit Fields */
#define FTFx_FOPT_OPT_MASK                      FTFE_FOPT_OPT_MASK
#define FTFx_FOPT_OPT_SHIFT                     FTFE_FOPT_OPT_SHIFT
#define FTFx_FOPT_OPT_WIDTH                     FTFE_FOPT_OPT_WIDTH
#define FTFx_FOPT_OPT(x)                        FTFE_FOPT_OPT(x)
/* FCCOB Bit Fields */
#define FTFx_FCCOB_CCOBn_MASK                   FTFE_FCCOB_CCOBn_MASK
#define FTFx_FCCOB_CCOBn_SHIFT                  FTFE_FCCOB_CCOBn_SHIFT
#define FTFx_FCCOB_CCOBn_WIDTH                  FTFE_FCCOB_CCOBn_WIDTH
#define FTFx_FCCOB_CCOBn(x)                     FTFE_FCCOB_CCOBn(x)
/* FPROT Bit Fields */
#define FTFx_FPROT_PROT_MASK                    FTFE_FPROT_PROT_MASK
#define FTFx_FPROT_PROT_SHIFT                   FTFE_FPROT_PROT_SHIFT
#define FTFx_FPROT_PROT_WIDTH                   FTFE_FPROT_PROT_WIDTH
#define FTFx_FPROT_PROT(x)                      FTFE_FPROT_PROT(x)
/* FEPROT Bit Fields */
#define FTFx_FEPROT_EPROT_MASK                  FTFE_FEPROT_EPROT_MASK
#define FTFx_FEPROT_EPROT_SHIFT                 FTFE_FEPROT_EPROT_SHIFT
#define FTFx_FEPROT_EPROT_WIDTH                 FTFE_FEPROT_EPROT_WIDTH
#define FTFx_FEPROT_EPROT(x)                    FTFE_FEPROT_EPROT(x)
/* FDPROT Bit Fields */
#define FTFx_FDPROT_DPROT_MASK                  FTFE_FDPROT_DPROT_MASK
#define FTFx_FDPROT_DPROT_SHIFT                 FTFE_FDPROT_DPROT_SHIFT
#define FTFx_FDPROT_DPROT_WIDTH                 FTFE_FDPROT_DPROT_WIDTH
#define FTFx_FDPROT_DPROT(x)                    FTFE_FDPROT_DPROT(x)
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
/* FERSTAT Bit Fields */
#define FTFx_FERSTAT_DFDIF_MASK                 FTFE_FERSTAT_DFDIF_MASK
#define FTFx_FERSTAT_DFDIF_SHIFT                FTFE_FERSTAT_DFDIF_SHIFT
#define FTFx_FERSTAT_DFDIF_WIDTH                FTFE_FERSTAT_DFDIF_WIDTH
#define FTFx_FERSTAT_DFDIF(x)                   FTFE_FERSTAT_DFDIF(x)
/* FERCNFG Bit Fields */
#define FTFx_FERCNFG_DFDIE_MASK                 FTFE_FERCNFG_DFDIE_MASK
#define FTFx_FERCNFG_DFDIE_SHIFT                FTFE_FERCNFG_DFDIE_SHIFT
#define FTFx_FERCNFG_DFDIE_WIDTH                FTFE_FERCNFG_DFDIE_WIDTH
#define FTFx_FERCNFG_DFDIE(x)                   FTFE_FERCNFG_DFDIE(x)
#define FTFx_FERCNFG_FDFD_MASK                  FTFE_FERCNFG_FDFD_MASK
#define FTFx_FERCNFG_FDFD_SHIFT                 FTFE_FERCNFG_FDFD_SHIFT
#define FTFx_FERCNFG_FDFD_WIDTH                 FTFE_FERCNFG_FDFD_WIDTH
#define FTFx_FERCNFG_FDFD(x)                    FTFE_FERCNFG_FDFD(x)
#endif
/*******************************************************************************
 * FTFC
 *******************************************************************************/
#elif defined(FTFC)
#define FTFx_BASE                               FTFC_BASE
#define FTFx_FSTAT                              FTFC->FSTAT
#define FTFx_FCNFG                              FTFC->FCNFG
#define FTFx_FSEC                               FTFC->FSEC
#define FTFx_FOPT                               FTFC->FOPT
#define FTFx_FCCOB3                             FTFC->FCCOB[0]
#define FTFx_FCCOB2                             FTFC->FCCOB[1]
#define FTFx_FCCOB1                             FTFC->FCCOB[2]
#define FTFx_FCCOB0                             FTFC->FCCOB[3]
#define FTFx_FCCOB7                             FTFC->FCCOB[4]
#define FTFx_FCCOB6                             FTFC->FCCOB[5]
#define FTFx_FCCOB5                             FTFC->FCCOB[6]
#define FTFx_FCCOB4                             FTFC->FCCOB[7]
#define FTFx_FCCOBB                             FTFC->FCCOB[8]
#define FTFx_FCCOBA                             FTFC->FCCOB[9]
#define FTFx_FCCOB9                             FTFC->FCCOB[10]
#define FTFx_FCCOB8                             FTFC->FCCOB[11]
#define FTFx_FPROT3                             FTFC->FPROT[0]
#define FTFx_FPROT2                             FTFC->FPROT[1]
#define FTFx_FPROT1                             FTFC->FPROT[2]
#define FTFx_FPROT0                             FTFC->FPROT[3]
#define FTFx_FEPROT                             FTFC->FEPROT
#define FTFx_FDPROT                             FTFC->FDPROT
#define FTFx_FCSESTAT                           FTFC->FCSESTAT
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
#define FTFx_FERSTAT                            FTFC->FERSTAT
#define FTFx_FERCNFG                            FTFC->FERCNFG
#endif
/* FSTAT Bit Fields */
#define FTFx_FSTAT_MGSTAT0_MASK                 FTFC_FSTAT_MGSTAT0_MASK
#define FTFx_FSTAT_MGSTAT0_SHIFT                FTFC_FSTAT_MGSTAT0_SHIFT
#define FTFx_FSTAT_MGSTAT0_WIDTH                FTFC_FSTAT_MGSTAT0_WIDTH
#define FTFx_FSTAT_MGSTAT0(x)                   FTFC_FSTAT_MGSTAT0(x)
#define FTFx_FSTAT_FPVIOL_MASK                  FTFC_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_FPVIOL_SHIFT                 FTFC_FSTAT_FPVIOL_SHIFT
#define FTFx_FSTAT_FPVIOL_WIDTH                 FTFC_FSTAT_FPVIOL_WIDTH
#define FTFx_FSTAT_FPVIOL(x)                    FTFC_FSTAT_FPVIOL(x)
#define FTFx_FSTAT_ACCERR_MASK                  FTFC_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_ACCERR_SHIFT                 FTFC_FSTAT_ACCERR_SHIFT
#define FTFx_FSTAT_ACCERR_WIDTH                 FTFC_FSTAT_ACCERR_WIDTH
#define FTFx_FSTAT_ACCERR(x)                    FTFC_FSTAT_ACCERR(x)
#define FTFx_FSTAT_RDCOLERR_MASK                FTFC_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_RDCOLERR_SHIFT               FTFC_FSTAT_RDCOLERR_SHIFT
#define FTFx_FSTAT_RDCOLERR_WIDTH               FTFC_FSTAT_RDCOLERR_WIDTH
#define FTFx_FSTAT_RDCOLERR(x)                  FTFC_FSTAT_RDCOLERR(x)
#define FTFx_FSTAT_CCIF_MASK                    FTFC_FSTAT_CCIF_MASK
#define FTFx_FSTAT_CCIF_SHIFT                   FTFC_FSTAT_CCIF_SHIFT
#define FTFx_FSTAT_CCIF_WIDTH                   FTFC_FSTAT_CCIF_WIDTH
#define FTFx_FSTAT_CCIF(x)                      FTFC_FSTAT_CCIF(x)
/* FCNFG Bit Fields */
#define FTFx_FCNFG_EEERDY_MASK                  FTFC_FCNFG_EEERDY_MASK
#define FTFx_FCNFG_EEERDY_SHIFT                 FTFC_FCNFG_EEERDY_SHIFT
#define FTFx_FCNFG_EEERDY_WIDTH                 FTFC_FCNFG_EEERDY_WIDTH
#define FTFx_FCNFG_EEERDY(x)                    FTFC_FCNFG_EEERDY(x)
#define FTFx_FCNFG_RAMRDY_MASK                  FTFC_FCNFG_RAMRDY_MASK
#define FTFx_FCNFG_RAMRDY_SHIFT                 FTFC_FCNFG_RAMRDY_SHIFT
#define FTFx_FCNFG_RAMRDY_WIDTH                 FTFC_FCNFG_RAMRDY_WIDTH
#define FTFx_FCNFG_RAMRDY(x)                    FTFC_FCNFG_RAMRDY(x)
#define FTFx_FCNFG_ERSSUSP_MASK                 FTFC_FCNFG_ERSSUSP_MASK
#define FTFx_FCNFG_ERSSUSP_SHIFT                FTFC_FCNFG_ERSSUSP_SHIFT
#define FTFx_FCNFG_ERSSUSP_WIDTH                FTFC_FCNFG_ERSSUSP_WIDTH
#define FTFx_FCNFG_ERSSUSP(x)                   FTFC_FCNFG_ERSSUSP(x)
#define FTFx_FCNFG_ERSAREQ_MASK                 FTFC_FCNFG_ERSAREQ_MASK
#define FTFx_FCNFG_ERSAREQ_SHIFT                FTFC_FCNFG_ERSAREQ_SHIFT
#define FTFx_FCNFG_ERSAREQ_WIDTH                FTFC_FCNFG_ERSAREQ_WIDTH
#define FTFx_FCNFG_ERSAREQ(x)                   FTFC_FCNFG_ERSAREQ(x)
#define FTFx_FCNFG_RDCOLLIE_MASK                FTFC_FCNFG_RDCOLLIE_MASK
#define FTFx_FCNFG_RDCOLLIE_SHIFT               FTFC_FCNFG_RDCOLLIE_SHIFT
#define FTFx_FCNFG_RDCOLLIE_WIDTH               FTFC_FCNFG_RDCOLLIE_WIDTH
#define FTFx_FCNFG_RDCOLLIE(x)                  FTFC_FCNFG_RDCOLLIE(x)
#define FTFx_FCNFG_CCIE_MASK                    FTFC_FCNFG_CCIE_MASK
#define FTFx_FCNFG_CCIE_SHIFT                   FTFC_FCNFG_CCIE_SHIFT
#define FTFx_FCNFG_CCIE_WIDTH                   FTFC_FCNFG_CCIE_WIDTH
#define FTFx_FCNFG_CCIE(x)                      FTFC_FCNFG_CCIE(x)
/* FSEC Bit Fields */
#define FTFx_FSEC_SEC_MASK                      FTFC_FSEC_SEC_MASK
#define FTFx_FSEC_SEC_SHIFT                     FTFC_FSEC_SEC_SHIFT
#define FTFx_FSEC_SEC_WIDTH                     FTFC_FSEC_SEC_WIDTH
#define FTFx_FSEC_SEC(x)                        FTFC_FSEC_SEC(x)
#define FTFx_FSEC_FSLACC_MASK                   FTFC_FSEC_FSLACC_MASK
#define FTFx_FSEC_FSLACC_SHIFT                  FTFC_FSEC_FSLACC_SHIFT
#define FTFx_FSEC_FSLACC_WIDTH                  FTFC_FSEC_FSLACC_WIDTH
#define FTFx_FSEC_FSLACC(x)                     FTFC_FSEC_FSLACC(x)
#define FTFx_FSEC_MEEN_MASK                     FTFC_FSEC_MEEN_MASK
#define FTFx_FSEC_MEEN_SHIFT                    FTFC_FSEC_MEEN_SHIFT
#define FTFx_FSEC_MEEN_WIDTH                    FTFC_FSEC_MEEN_WIDTH
#define FTFx_FSEC_MEEN(x)                       FTFC_FSEC_MEEN(x)
#define FTFx_FSEC_KEYEN_MASK                    FTFC_FSEC_KEYEN_MASK
#define FTFx_FSEC_KEYEN_SHIFT                   FTFC_FSEC_KEYEN_SHIFT
#define FTFx_FSEC_KEYEN_WIDTH                   FTFC_FSEC_KEYEN_WIDTH
#define FTFx_FSEC_KEYEN(x)                      FTFC_FSEC_KEYEN(x)
/* FOPT Bit Fields */
#define FTFx_FOPT_OPT_MASK                      FTFC_FOPT_OPT_MASK
#define FTFx_FOPT_OPT_SHIFT                     FTFC_FOPT_OPT_SHIFT
#define FTFx_FOPT_OPT_WIDTH                     FTFC_FOPT_OPT_WIDTH
#define FTFx_FOPT_OPT(x)                        FTFC_FOPT_OPT(x)
/* FCCOB Bit Fields */
#define FTFx_FCCOB_CCOBn_MASK                   FTFC_FCCOB_CCOBn_MASK
#define FTFx_FCCOB_CCOBn_SHIFT                  FTFC_FCCOB_CCOBn_SHIFT
#define FTFx_FCCOB_CCOBn_WIDTH                  FTFC_FCCOB_CCOBn_WIDTH
#define FTFx_FCCOB_CCOBn(x)                     FTFC_FCCOB_CCOBn(x)
/* FPROT Bit Fields */
#define FTFx_FPROT_PROT_MASK                    FTFC_FPROT_PROT_MASK
#define FTFx_FPROT_PROT_SHIFT                   FTFC_FPROT_PROT_SHIFT
#define FTFx_FPROT_PROT_WIDTH                   FTFC_FPROT_PROT_WIDTH
#define FTFx_FPROT_PROT(x)                      FTFC_FPROT_PROT(x)
/* FEPROT Bit Fields */
#define FTFx_FEPROT_EPROT_MASK                  FTFC_FEPROT_EPROT_MASK
#define FTFx_FEPROT_EPROT_SHIFT                 FTFC_FEPROT_EPROT_SHIFT
#define FTFx_FEPROT_EPROT_WIDTH                 FTFC_FEPROT_EPROT_WIDTH
#define FTFx_FEPROT_EPROT(x)                    FTFC_FEPROT_EPROT(x)
/* FDPROT Bit Fields */
#define FTFx_FDPROT_DPROT_MASK                  FTFC_FDPROT_DPROT_MASK
#define FTFx_FDPROT_DPROT_SHIFT                 FTFC_FDPROT_DPROT_SHIFT
#define FTFx_FDPROT_DPROT_WIDTH                 FTFC_FDPROT_DPROT_WIDTH
#define FTFx_FDPROT_DPROT(x)                    FTFC_FDPROT_DPROT(x)
/* FCSESTAT Bit Fields */
#define FTFx_FCSESTAT_IDB_MASK                  FTFC_FCSESTAT_IDB_MASK
#define FTFx_FCSESTAT_IDB_SHIFT                 FTFC_FCSESTAT_IDB_SHIFT
#define FTFx_FCSESTAT_IDB_WIDTH                 FTFC_FCSESTAT_IDB_WIDTH
#define FTFx_FCSESTAT_IDB(x)                    FTFC_FCSESTAT_IDB(x)
#define FTFx_FCSESTAT_EDB_MASK                  FTFC_FCSESTAT_EDB_MASK
#define FTFx_FCSESTAT_EDB_SHIFT                 FTFC_FCSESTAT_EDB_SHIFT
#define FTFx_FCSESTAT_EDB_WIDTH                 FTFC_FCSESTAT_EDB_WIDTH
#define FTFx_FCSESTAT_EDB(x)                    FTFC_FCSESTAT_EDB(x)
#define FTFx_FCSESTAT_RIN_MASK                  FTFC_FCSESTAT_RIN_MASK
#define FTFx_FCSESTAT_RIN_SHIFT                 FTFC_FCSESTAT_RIN_SHIFT
#define FTFx_FCSESTAT_RIN_WIDTH                 FTFC_FCSESTAT_RIN_WIDTH
#define FTFx_FCSESTAT_RIN(x)                    FTFC_FCSESTAT_RIN(x)
#define FTFx_FCSESTAT_BOK_MASK                  FTFC_FCSESTAT_BOK_MASK
#define FTFx_FCSESTAT_BOK_SHIFT                 FTFC_FCSESTAT_BOK_SHIFT
#define FTFx_FCSESTAT_BOK_WIDTH                 FTFC_FCSESTAT_BOK_WIDTH
#define FTFx_FCSESTAT_BOK(x)                    FTFC_FCSESTAT_BOK(x)
#define FTFx_FCSESTAT_BFN_MASK                  FTFC_FCSESTAT_BFN_MASK
#define FTFx_FCSESTAT_BFN_SHIFT                 FTFC_FCSESTAT_BFN_SHIFT
#define FTFx_FCSESTAT_BFN_WIDTH                 FTFC_FCSESTAT_BFN_WIDTH
#define FTFx_FCSESTAT_BFN(x)                    FTFC_FCSESTAT_BFN(x)
#define FTFx_FCSESTAT_BIN_MASK                  FTFC_FCSESTAT_BIN_MASK
#define FTFx_FCSESTAT_BIN_SHIFT                 FTFC_FCSESTAT_BIN_SHIFT
#define FTFx_FCSESTAT_BIN_WIDTH                 FTFC_FCSESTAT_BIN_WIDTH
#define FTFx_FCSESTAT_BIN(x)                    FTFC_FCSESTAT_BIN(x)
#define FTFx_FCSESTAT_SB_MASK                   FTFC_FCSESTAT_SB_MASK
#define FTFx_FCSESTAT_SB_SHIFT                  FTFC_FCSESTAT_SB_SHIFT
#define FTFx_FCSESTAT_SB_WIDTH                  FTFC_FCSESTAT_SB_WIDTH
#define FTFx_FCSESTAT_SB(x)                     FTFC_FCSESTAT_SB(x)
#define FTFx_FCSESTAT_BSY_MASK                  FTFC_FCSESTAT_BSY_MASK
#define FTFx_FCSESTAT_BSY_SHIFT                 FTFC_FCSESTAT_BSY_SHIFT
#define FTFx_FCSESTAT_BSY_WIDTH                 FTFC_FCSESTAT_BSY_WIDTH
#define FTFx_FCSESTAT_BSY(x)                    FTFC_FCSESTAT_BSY(x)
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
/* FERSTAT Bit Fields */
#define FTFx_FERSTAT_DFDIF_MASK                 FTFC_FERSTAT_DFDIF_MASK
#define FTFx_FERSTAT_DFDIF_SHIFT                FTFC_FERSTAT_DFDIF_SHIFT
#define FTFxFERSTAT_DFDIF_WIDTH                 FTFC_FERSTAT_DFDIF_WIDTH
#define FTFx_FERSTAT_DFDIF(x)                   FTFC_FERSTAT_DFDIF(x)
/* FERCNFG Bit Fields */
#define FTFx_FERCNFG_DFDIE_MASK                 FTFC_FERCNFG_DFDIE_MASK
#define FTFx_FERCNFG_DFDIE_SHIFT                FTFC_FERCNFG_DFDIE_SHIFT
#define FTFx_FERCNFG_DFDIE_WIDTH                FTFC_FERCNFG_DFDIE_WIDTH
#define FTFx_FERCNFG_DFDIE(x)                   FTFC_FERCNFG_DFDIE(x)
#define FTFx_FERCNFG_FDFD_MASK                  FTFC_FERCNFG_FDFD_MASK
#define FTFx_FERCNFG_FDFD_SHIFT                 FTFC_FERCNFG_FDFD_SHIFT
#define FTFx_FERCNFG_FDFD_WIDTH                 FTFC_FERCNFG_FDFD_WIDTH
#define FTFx_FERCNFG_FDFD(x)                    FTFC_FERCNFG_FDFD(x)
#endif
/*******************************************************************************
 * FTFM
 *******************************************************************************/
#elif defined(FTFM)
#define FTFx_BASE                               FTFM_BASE
#define FTFx_FSTAT                              FTFM->FSTAT
#define FTFx_FCNFG                              FTFM->FCNFG
#define FTFx_FSEC                               FTFM->FSEC
#define FTFx_FOPT                               FTFM->FOPT
#define FTFx_FCCOB3                             FTFM->FCCOB[0]
#define FTFx_FCCOB2                             FTFM->FCCOB[1]
#define FTFx_FCCOB1                             FTFM->FCCOB[2]
#define FTFx_FCCOB0                             FTFM->FCCOB[3]
#define FTFx_FCCOB7                             FTFM->FCCOB[4]
#define FTFx_FCCOB6                             FTFM->FCCOB[5]
#define FTFx_FCCOB5                             FTFM->FCCOB[6]
#define FTFx_FCCOB4                             FTFM->FCCOB[7]
#define FTFx_FCCOBB                             FTFM->FCCOB[8]
#define FTFx_FCCOBA                             FTFM->FCCOB[9]
#define FTFx_FCCOB9                             FTFM->FCCOB[10]
#define FTFx_FCCOB8                             FTFM->FCCOB[11]
#define FTFx_FPROT3                             FTFM->FPROT[0]
#define FTFx_FPROT2                             FTFM->FPROT[1]
#define FTFx_FPROT1                             FTFM->FPROT[2]
#define FTFx_FPROT0                             FTFM->FPROT[3]
#define FTFx_FEPROT                             FTFM->FEPROT
#define FTFx_FDPROT                             FTFM->FDPROT
#define FTFx_FCSESTAT0                          FTFM->FCSESTAT0
#define FTFx_FCSESTAT1                          FTFM->FCSESTAT1
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
#define FTFx_FERSTAT                            FTFM->FERSTAT
#define FTFx_FERCNFG                            FTFM->FERCNFG
#endif
/* FSTAT Bit Fields */
#define FTFx_FSTAT_MGSTAT0_MASK                 FTFM_FSTAT_MGSTAT0_MASK
#define FTFx_FSTAT_MGSTAT0_SHIFT                FTFM_FSTAT_MGSTAT0_SHIFT
#define FTFx_FSTAT_MGSTAT0_WIDTH                FTFM_FSTAT_MGSTAT0_WIDTH
#define FTFx_FSTAT_MGSTAT0(x)                   FTFM_FSTAT_MGSTAT0(x)
#define FTFx_FSTAT_MGSTAT1_MASK                 FTFM_FSTAT_MGSTAT1_MASK
#define FTFx_FSTAT_MGSTAT1_SHIFT                FTFM_FSTAT_MGSTAT1_SHIFT
#define FTFx_FSTAT_MGSTAT1_WIDTH                FTFM_FSTAT_MGSTAT1_WIDTH
#define FTFx_FSTAT_MGSTAT1(x)                   FTFM_FSTAT_MGSTAT1(x)
#define FTFx_FSTAT_MGSTAT2_MASK                 FTFM_FSTAT_MGSTAT2_MASK
#define FTFx_FSTAT_MGSTAT2_SHIFT                FTFM_FSTAT_MGSTAT2_SHIFT
#define FTFx_FSTAT_MGSTAT2_WIDTH                FTFM_FSTAT_MGSTAT2_WIDTH
#define FTFx_FSTAT_MGSTAT2(x)                   FTFM_FSTAT_MGSTAT2(x)
#define FTFx_FSTAT_MGSTAT3_MASK                 FTFM_FSTAT_MGSTAT3_MASK
#define FTFx_FSTAT_MGSTAT3_SHIFT                FTFM_FSTAT_MGSTAT3_SHIFT
#define FTFx_FSTAT_MGSTAT3_WIDTH                FTFM_FSTAT_MGSTAT3_WIDTH
#define FTFx_FSTAT_MGSTAT3(x)                   FTFM_FSTAT_MGSTAT3(x)
#define FTFx_FSTAT_FPVIOL_MASK                  FTFM_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_FPVIOL_SHIFT                 FTFM_FSTAT_FPVIOL_SHIFT
#define FTFx_FSTAT_FPVIOL_WIDTH                 FTFM_FSTAT_FPVIOL_WIDTH
#define FTFx_FSTAT_FPVIOL(x)                    FTFM_FSTAT_FPVIOL(x)
#define FTFx_FSTAT_ACCERR_MASK                  FTFM_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_ACCERR_SHIFT                 FTFM_FSTAT_ACCERR_SHIFT
#define FTFx_FSTAT_ACCERR_WIDTH                 FTFM_FSTAT_ACCERR_WIDTH
#define FTFx_FSTAT_ACCERR(x)                    FTFM_FSTAT_ACCERR(x)
#define FTFx_FSTAT_RDCOLERR_MASK                FTFM_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_RDCOLERR_SHIFT               FTFM_FSTAT_RDCOLERR_SHIFT
#define FTFx_FSTAT_RDCOLERR_WIDTH               FTFM_FSTAT_RDCOLERR_WIDTH
#define FTFx_FSTAT_RDCOLERR(x)                  FTFM_FSTAT_RDCOLERR(x)
#define FTFx_FSTAT_CCIF_MASK                    FTFM_FSTAT_CCIF_MASK
#define FTFx_FSTAT_CCIF_SHIFT                   FTFM_FSTAT_CCIF_SHIFT
#define FTFx_FSTAT_CCIF_WIDTH                   FTFM_FSTAT_CCIF_WIDTH
#define FTFx_FSTAT_CCIF(x)                      FTFM_FSTAT_CCIF(x)
/* FCNFG Bit Fields */
#define FTFx_FCNFG_EEERDY_MASK                  FTFM_FCNFG_EEERDY_MASK
#define FTFx_FCNFG_EEERDY_SHIFT                 FTFM_FCNFG_EEERDY_SHIFT
#define FTFx_FCNFG_EEERDY_WIDTH                 FTFM_FCNFG_EEERDY_WIDTH
#define FTFx_FCNFG_EEERDY(x)                    FTFM_FCNFG_EEERDY(x)
#define FTFx_FCNFG_RAMRDY_MASK                  FTFM_FCNFG_RAMRDY_MASK
#define FTFx_FCNFG_RAMRDY_SHIFT                 FTFM_FCNFG_RAMRDY_SHIFT
#define FTFx_FCNFG_RAMRDY_WIDTH                 FTFM_FCNFG_RAMRDY_WIDTH
#define FTFx_FCNFG_RAMRDY(x)                    FTFM_FCNFG_RAMRDY(x)
#define FTFx_FCNFG_ERSSUSP_MASK                 FTFM_FCNFG_ERSSUSP_MASK
#define FTFx_FCNFG_ERSSUSP_SHIFT                FTFM_FCNFG_ERSSUSP_SHIFT
#define FTFx_FCNFG_ERSSUSP_WIDTH                FTFM_FCNFG_ERSSUSP_WIDTH
#define FTFx_FCNFG_ERSSUSP(x)                   FTFM_FCNFG_ERSSUSP(x)
#define FTFx_FCNFG_ERSAREQ_MASK                 FTFM_FCNFG_ERSAREQ_MASK
#define FTFx_FCNFG_ERSAREQ_SHIFT                FTFM_FCNFG_ERSAREQ_SHIFT
#define FTFx_FCNFG_ERSAREQ_WIDTH                FTFM_FCNFG_ERSAREQ_WIDTH
#define FTFx_FCNFG_ERSAREQ(x)                   FTFM_FCNFG_ERSAREQ(x)
#define FTFx_FCNFG_RDCOLLIE_MASK                FTFM_FCNFG_RDCOLLIE_MASK
#define FTFx_FCNFG_RDCOLLIE_SHIFT               FTFM_FCNFG_RDCOLLIE_SHIFT
#define FTFx_FCNFG_RDCOLLIE_WIDTH               FTFM_FCNFG_RDCOLLIE_WIDTH
#define FTFx_FCNFG_RDCOLLIE(x)                  FTFM_FCNFG_RDCOLLIE(x)
#define FTFx_FCNFG_CCIE_MASK                    FTFM_FCNFG_CCIE_MASK
#define FTFx_FCNFG_CCIE_SHIFT                   FTFM_FCNFG_CCIE_SHIFT
#define FTFx_FCNFG_CCIE_WIDTH                   FTFM_FCNFG_CCIE_WIDTH
#define FTFx_FCNFG_CCIE(x)                      FTFM_FCNFG_CCIE(x)
/* FSEC Bit Fields */
#define FTFx_FSEC_SEC_MASK                      FTFM_FSEC_SEC_MASK
#define FTFx_FSEC_SEC_SHIFT                     FTFM_FSEC_SEC_SHIFT
#define FTFx_FSEC_SEC_WIDTH                     FTFM_FSEC_SEC_WIDTH
#define FTFx_FSEC_SEC(x)                        FTFM_FSEC_SEC(x)
#define FTFx_FSEC_FSLACC_MASK                   FTFM_FSEC_FSLACC_MASK
#define FTFx_FSEC_FSLACC_SHIFT                  FTFM_FSEC_FSLACC_SHIFT
#define FTFx_FSEC_FSLACC_WIDTH                  FTFM_FSEC_FSLACC_WIDTH
#define FTFx_FSEC_FSLACC(x)                     FTFM_FSEC_FSLACC(x)
#define FTFx_FSEC_MEEN_MASK                     FTFM_FSEC_MEEN_MASK
#define FTFx_FSEC_MEEN_SHIFT                    FTFM_FSEC_MEEN_SHIFT
#define FTFx_FSEC_MEEN_WIDTH                    FTFM_FSEC_MEEN_WIDTH
#define FTFx_FSEC_MEEN(x)                       FTFM_FSEC_MEEN(x)
#define FTFx_FSEC_KEYEN_MASK                    FTFM_FSEC_KEYEN_MASK
#define FTFx_FSEC_KEYEN_SHIFT                   FTFM_FSEC_KEYEN_SHIFT
#define FTFx_FSEC_KEYEN_WIDTH                   FTFM_FSEC_KEYEN_WIDTH
#define FTFx_FSEC_KEYEN(x)                      FTFM_FSEC_KEYEN(x)
/* FOPT Bit Fields */
#define FTFx_FOPT_OPT_MASK                      FTFM_FOPT_OPT_MASK
#define FTFx_FOPT_OPT_SHIFT                     FTFM_FOPT_OPT_SHIFT
#define FTFx_FOPT_OPT_WIDTH                     FTFM_FOPT_OPT_WIDTH
#define FTFx_FOPT_OPT(x)                        FTFM_FOPT_OPT(x)
/* FCCOB Bit Fields */
#define FTFx_FCCOB_CCOBn_MASK                   FTFM_FCCOB_CCOBn_MASK
#define FTFx_FCCOB_CCOBn_SHIFT                  FTFM_FCCOB_CCOBn_SHIFT
#define FTFx_FCCOB_CCOBn_WIDTH                  FTFM_FCCOB_CCOBn_WIDTH
#define FTFx_FCCOB_CCOBn(x)                     FTFM_FCCOB_CCOBn(x)
/* FPROT Bit Fields */
#define FTFx_FPROT_PROT_MASK                    FTFM_FPROT_PROT_MASK
#define FTFx_FPROT_PROT_SHIFT                   FTFM_FPROT_PROT_SHIFT
#define FTFx_FPROT_PROT_WIDTH                   FTFM_FPROT_PROT_WIDTH
#define FTFx_FPROT_PROT(x)                      FTFM_FPROT_PROT(x)
/* FEPROT Bit Fields */
#define FTFx_FEPROT_EPROT_MASK                  FTFM_FEPROT_EPROT_MASK
#define FTFx_FEPROT_EPROT_SHIFT                 FTFM_FEPROT_EPROT_SHIFT
#define FTFx_FEPROT_EPROT_WIDTH                 FTFM_FEPROT_EPROT_WIDTH
#define FTFx_FEPROT_EPROT(x)                    FTFM_FEPROT_EPROT(x)
/* FDPROT Bit Fields */
#define FTFx_FDPROT_DPROT_MASK                  FTFM_FDPROT_DPROT_MASK
#define FTFx_FDPROT_DPROT_SHIFT                 FTFM_FDPROT_DPROT_SHIFT
#define FTFx_FDPROT_DPROT_WIDTH                 FTFM_FDPROT_DPROT_WIDTH
#define FTFx_FDPROT_DPROT(x)                    FTFM_FDPROT_DPROT(x)
/* FCSESTAT1 Bit Fields */
#define FTFx_FCSESTAT1_IDB_MASK                  FTFM_FCSESTAT1_IDB_MASK
#define FTFx_FCSESTAT1_IDB_SHIFT                 FTFM_FCSESTAT1_IDB_SHIFT
#define FTFx_FCSESTAT1_IDB_WIDTH                 FTFM_FCSESTAT1_IDB_WIDTH
#define FTFx_FCSESTAT1_IDB(x)                    FTFM_FCSESTAT1_IDB(x)
#define FTFx_FCSESTAT1_EDB_MASK                  FTFM_FCSESTAT1_EDB_MASK
#define FTFx_FCSESTAT1_EDB_SHIFT                 FTFM_FCSESTAT1_EDB_SHIFT
#define FTFx_FCSESTAT1_EDB_WIDTH                 FTFM_FCSESTAT1_EDB_WIDTH
#define FTFx_FCSESTAT1_EDB(x)                    FTFM_FCSESTAT1_EDB(x)
#define FTFx_FCSESTAT1_RIN_MASK                  FTFM_FCSESTAT1_RIN_MASK
#define FTFx_FCSESTAT1_RIN_SHIFT                 FTFM_FCSESTAT1_RIN_SHIFT
#define FTFx_FCSESTAT1_RIN_WIDTH                 FTFM_FCSESTAT1_RIN_WIDTH
#define FTFx_FCSESTAT1_RIN(x)                    FTFM_FCSESTAT1_RIN(x)
#define FTFx_FCSESTAT1_BOK_MASK                  FTFM_FCSESTAT1_BOK_MASK
#define FTFx_FCSESTAT1_BOK_SHIFT                 FTFM_FCSESTAT1_BOK_SHIFT
#define FTFx_FCSESTAT1_BOK_WIDTH                 FTFM_FCSESTAT1_BOK_WIDTH
#define FTFx_FCSESTAT1_BOK(x)                    FTFM_FCSESTAT1_BOK(x)
#define FTFx_FCSESTAT1_BFN_MASK                  FTFM_FCSESTAT1_BFN_MASK
#define FTFx_FCSESTAT1_BFN_SHIFT                 FTFM_FCSESTAT1_BFN_SHIFT
#define FTFx_FCSESTAT1_BFN_WIDTH                 FTFM_FCSESTAT1_BFN_WIDTH
#define FTFx_FCSESTAT1_BFN(x)                    FTFM_FCSESTAT1_BFN(x)
#define FTFx_FCSESTAT1_BIN_MASK                  FTFM_FCSESTAT1_BIN_MASK
#define FTFx_FCSESTAT1_BIN_SHIFT                 FTFM_FCSESTAT1_BIN_SHIFT
#define FTFx_FCSESTAT1_BIN_WIDTH                 FTFM_FCSESTAT1_BIN_WIDTH
#define FTFx_FCSESTAT1_BIN(x)                    FTFM_FCSESTAT1_BIN(x)
#define FTFx_FCSESTAT1_SB_MASK                   FTFM_FCSESTAT1_SB_MASK
#define FTFx_FCSESTAT1_SB_SHIFT                  FTFM_FCSESTAT1_SB_SHIFT
#define FTFx_FCSESTAT1_SB_WIDTH                  FTFM_FCSESTAT1_SB_WIDTH
#define FTFx_FCSESTAT1_SB(x)                     FTFM_FCSESTAT1_SB(x)
#define FTFx_FCSESTAT1_BSY_MASK                  FTFM_FCSESTAT1_BSY_MASK
#define FTFx_FCSESTAT1_BSY_SHIFT                 FTFM_FCSESTAT1_BSY_SHIFT
#define FTFx_FCSESTAT1_BSY_WIDTH                 FTFM_FCSESTAT1_BSY_WIDTH
#define FTFx_FCSESTAT1_BSY(x)                    FTFM_FCSESTAT1_BSY(x)
/* FCSESTAT0 Bit Fields */
#define FTFx_FCSESTAT0_CMDTYPE_MASK              FTFM_FCSESTAT0_CMDTYPE_MASK
#define FTFx_FCSESTAT0_CMDTYPE_SHIFT             FTFM_FCSESTAT0_CMDTYPE_SHIFT
#define FTFx_FCSESTAT0_CMDTYPE_WIDTH             FTFM_FCSESTAT0_CMDTYPE_WIDTH
#define FTFx_FCSESTAT0_CMDTYPE(x)                FTFM_FCSESTAT0_CMDTYPE(x)
#define FTFx_FCSESTAT0_MEMERR_MASK               FTFM_FCSESTAT0_MEMERR_MASK
#define FTFx_FCSESTAT0_MEMERR_SHIFT              FTFM_FCSESTAT0_MEMERR_SHIFT
#define FTFx_FCSESTAT0_MEMERR_WIDTH              FTFM_FCSESTAT0_MEMERR_WIDTH
#define FTFx_FCSESTAT0_MEMERR(x)                 FTFM_FCSESTAT0_MEMERR(x)
#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
/* FERSTAT Bit Fields */
#define FTFx_FERSTAT_PDFDIF_MASK                 FTFM_FERSTAT_PDFDIF_MASK
#define FTFx_FERSTAT_PDFDIF_SHIFT                FTFM_FERSTAT_PDFDIF_SHIFT
#define FTFx_FERSTAT_PDFDIF_WIDTH                FTFM_FERSTAT_PDFDIF_WIDTH
#define FTFx_FERSTAT_PDFDIF(x)                   FTFM_FERSTAT_PDFDIF(x)
#define FTFx_FERSTAT_DFDIF_MASK                  FTFM_FERSTAT_DFDIF_MASK
#define FTFx_FERSTAT_DFDIF_SHIFT                 FTFM_FERSTAT_DFDIF_SHIFT
#define FTFx_FERSTAT_DFDIF_WIDTH                 FTFM_FERSTAT_DFDIF_WIDTH
#define FTFx_FERSTAT_DFDIF(x)                    FTFM_FERSTAT_DFDIF(x)
#define FTFx_FERSTAT_EDFDIF_MASK                 FTFM_FERSTAT_EDFDIF_MASK
#define FTFx_FERSTAT_EDFDIF_SHIFT                FTFM_FERSTAT_EDFDIF_SHIFT
#define FTFx_FERSTAT_EDFDIF_WIDTH                FTFM_FERSTAT_EDFDIF_WIDTH
#define FTFx_FERSTAT_EDFDIF(x)                   FTFM_FERSTAT_EDFDIF(x)
#define FTFx_FERSTAT_CDFDIF_MASK                 FTFM_FERSTAT_CDFDIF_MASK
#define FTFx_FERSTAT_CDFDIF_SHIFT                FTFM_FERSTAT_CDFDIF_SHIFT
#define FTFx_FERSTAT_CDFDIF_WIDTH                FTFM_FERSTAT_CDFDIF_WIDTH
#define FTFx_FERSTAT_CDFDIF(x)                   FTFM_FERSTAT_CDFDIF(x)
/* FERCNFG Bit Fields */
#define FTFx_FERCNFG_PDFDIE_MASK                 FTFM_FERCNFG_PDFDIE_MASK
#define FTFx_FERCNFG_PDFDIE_SHIFT                FTFM_FERCNFG_PDFDIE_SHIFT
#define FTFx_FERCNFG_PDFDIE_WIDTH                FTFM_FERCNFG_PDFDIE_WIDTH
#define FTFx_FERCNFG_PDFDIE(x)                   FTFM_FERCNFG_PDFDIE(x)
#define FTFx_FERCNFG_DFDIE_MASK                  FTFM_FERCNFG_DFDIE_MASK
#define FTFx_FERCNFG_DFDIE_SHIFT                 FTFM_FERCNFG_DFDIE_SHIFT
#define FTFx_FERCNFG_DFDIE_WIDTH                 FTFM_FERCNFG_DFDIE_WIDTH
#define FTFx_FERCNFG_DFDIE(x)                    FTFM_FERCNFG_DFDIE(x)
#define FTFx_FERCNFG_EDFDIE_MASK                 FTFM_FERCNFG_EDFDIE_MASK
#define FTFx_FERCNFG_EDFDIE_SHIFT                FTFM_FERCNFG_EDFDIE_SHIFT
#define FTFx_FERCNFG_EDFDIE_WIDTH                FTFM_FERCNFG_EDFDIE_WIDTH
#define FTFx_FERCNFG_EDFDIE(x)                   FTFM_FERCNFG_EDFDIE(x)
#define FTFx_FERCNFG_CDFDIE_MASK                 FTFM_FERCNFG_CDFDIE_MASK
#define FTFx_FERCNFG_CDFDIE_SHIFT                FTFM_FERCNFG_CDFDIE_SHIFT
#define FTFx_FERCNFG_CDFDIE_WIDTH                FTFM_FERCNFG_CDFDIE_WIDTH
#define FTFx_FERCNFG_CDFDIE(x)                   FTFM_FERCNFG_CDFDIE(x)
#define FTFx_FERCNFG_PFDFD_MASK                  FTFM_FERCNFG_PFDFD_MASK
#define FTFx_FERCNFG_PFDFD_SHIFT                 FTFM_FERCNFG_PFDFD_SHIFT
#define FTFx_FERCNFG_PFDFD_WIDTH                 FTFM_FERCNFG_PFDFD_WIDTH
#define FTFx_FERCNFG_PFDFD(x)                    FTFM_FERCNFG_PFDFD(x)
#define FTFx_FERCNFG_FDFD_MASK                   FTFM_FERCNFG_FDFD_MASK
#define FTFx_FERCNFG_FDFD_SHIFT                  FTFM_FERCNFG_FDFD_SHIFT
#define FTFx_FERCNFG_FDFD_WIDTH                  FTFM_FERCNFG_FDFD_WIDTH
#define FTFx_FERCNFG_FDFD(x)                     FTFM_FERCNFG_FDFD(x)
#define FTFx_FERCNFG_EFDFD_MASK                  FTFM_FERCNFG_EFDFD_MASK
#define FTFx_FERCNFG_EFDFD_SHIFT                 FTFM_FERCNFG_EFDFD_SHIFT
#define FTFx_FERCNFG_EFDFD_WIDTH                 FTFM_FERCNFG_EFDFD_WIDTH
#define FTFx_FERCNFG_EFDFD(x)                    FTFM_FERCNFG_EFDFD(x)
#define FTFx_FERCNFG_CFDFD_MASK                  FTFM_FERCNFG_CFDFD_MASK
#define FTFx_FERCNFG_CFDFD_SHIFT                 FTFM_FERCNFG_CFDFD_SHIFT
#define FTFx_FERCNFG_CFDFD_WIDTH                 FTFM_FERCNFG_CFDFD_WIDTH
#define FTFx_FERCNFG_CFDFD(x)                    FTFM_FERCNFG_CFDFD(x)
#endif

#else  /* ifndef FTFE */
#endif /* ifdef FTFE */

#define CLEAR_FTFx_FSTAT_ERROR_BITS FTFx_FSTAT = (uint8_t)(FTFx_FSTAT_FPVIOL_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_RDCOLERR_MASK)

/* Word size 2 bytes */
#define FTFx_WORD_SIZE     0x0002U
/* Long word size 4 bytes */
#define FTFx_LONGWORD_SIZE 0x0004U
/* Phrase size 8 bytes */
#define FTFx_PHRASE_SIZE   0x0008U
/* Double-phrase size 16 bytes */
#define FTFx_DPHRASE_SIZE  0x0010U

/* FCCOB offset address to store resource code */
#if (FEATURE_FLS_PF_BLOCK_WRITE_UNIT_SIZE == FTFx_PHRASE_SIZE)
#define FTFx_RSRC_CODE_REG FTFx_FCCOB4
#else
#define FTFx_RSRC_CODE_REG FTFx_FCCOB8
#endif

/*******************************************************************************
* Flash hardware algorithm operation commands
*******************************************************************************/
#define FTFx_VERIFY_BLOCK             0x00U
#define FTFx_VERIFY_SECTION           0x01U
#define FTFx_PROGRAM_CHECK            0x02U
#define FTFx_READ_RESOURCE            0x03U
#define FTFx_PROGRAM_LONGWORD         0x06U
#define FTFx_PROGRAM_PHRASE           0x07U
#define FTFx_ERASE_BLOCK              0x08U
#define FTFx_ERASE_SECTOR             0x09U
#define FTFx_PROGRAM_SECTION          0x0BU
#define FTFx_VERIFY_ALL_BLOCK         0x40U
#define FTFx_READ_ONCE                0x41U
#define FTFx_PROGRAM_ONCE             0x43U
#define FTFx_ERASE_ALL_BLOCK          0x44U
#define FTFx_SECURITY_BY_PASS         0x45U
#define FTFx_PFLASH_SWAP              0x46U
#define FTFx_ERASE_ALL_BLOCK_UNSECURE 0x49U
#define FTFx_PROGRAM_PARTITION        0x80U
#define FTFx_SET_EERAM                0x81U


/*!
 * @name PFlash swap control codes
 * @{
 */
/*! @brief Initialize Swap System control code */
#define FTFx_SWAP_SET_INDICATOR_ADDR 0x01U
/*! @brief Set Swap in Update State */
#define FTFx_SWAP_SET_IN_PREPARE     0x02U
/*! @brief Set Swap in Complete State */
#define FTFx_SWAP_SET_IN_COMPLETE    0x04U
/*! @brief  Report Swap Status */
#define FTFx_SWAP_REPORT_STATUS      0x08U
/*@}*/

/*!
 * @name PFlash swap states
 * @{
 */
/*! @brief  Uninitialized swap mode */
#define FTFx_SWAP_UNINIT        0x00U
/*! @brief  Ready swap mode */
#define FTFx_SWAP_READY         0x01U
/*! @brief  Update swap mode */
#define FTFx_SWAP_UPDATE        0x02U
/*! @brief  Update-Erased swap mode */
#define FTFx_SWAP_UPDATE_ERASED 0x03U
/*! @brief  Complete swap mode */
#define FTFx_SWAP_COMPLETE      0x04U
/*@}*/

/*! @brief Resume wait count used in FLASH_DRV_EraseResume function */
#define RESUME_WAIT_CNT         0x20U
/*! @brief Suspend wait count used in FLASH_DRV_EraseSuspend function */
#define SUSPEND_WAIT_CNT        0x40U

#if (FEATURE_FLS_IS_FTFE == 1U)
#define DFLASH_IFR_READRESOURCE_ADDRESS 0x8003F8U
#else /* FEATURE_FLASH_IS_FTFL == 1 or FEATURE_FLASH_IS_FTFA == 1 */
#define DFLASH_IFR_READRESOURCE_ADDRESS 0x8000FCU
#endif

#define GET_BIT_0_7(value)   ((uint8_t)(((uint32_t)(value)) & 0xFFU))
#define GET_BIT_8_15(value)  ((uint8_t)((((uint32_t)(value)) >> 8) & 0xFFU))
#define GET_BIT_16_23(value) ((uint8_t)((((uint32_t)(value)) >> 16) & 0xFFU))
#define GET_BIT_24_31(value) ((uint8_t)(((uint32_t)(value)) >> 24))

/* Flash security status */
#define FLASH_SECURITY_STATE_KEYEN     0x80U
#define FLASH_SECURITY_STATE_UNSECURED 0x02U

#if (FEATURE_FLS_IS_FTFM == 1U)
/* CSEc user key size */
#define CSE_KEY_SIZE_CODE_MAX 0x07U
/* protection violation error */
#define FTFx_FSTAT_ERROR_BITS (FTFx_FSTAT & (FTFx_FSTAT_MGSTAT0_MASK | FTFx_FSTAT_MGSTAT1_MASK | FTFx_FSTAT_MGSTAT2_MASK | FTFx_FSTAT_MGSTAT3_MASK | FTFx_FSTAT_FPVIOL_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_RDCOLERR_MASK))
#else
#define CSE_KEY_SIZE_CODE_MAX 0x03U
#define FTFx_FSTAT_ERROR_BITS (FTFx_FSTAT & (FTFx_FSTAT_MGSTAT0_MASK | FTFx_FSTAT_FPVIOL_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_RDCOLERR_MASK))
#endif

/*!
 * @addtogroup flash_driver
 * @{
 */

/*!
 * @brief FlexRAM Function control Code
 *
 * Implements : flash_flexRAM_function_control_code_t_Class
 */
typedef enum
{
    EEE_ENABLE                              = 0x00U,    /*!< Make FlexRAM available for emulated EEPROM */
    EEE_QUICK_WRITE                         = 0x55U,    /*!< Make FlexRAM available for EEPROM quick writes */
    EEE_STATUS_QUERY                        = 0x77U,    /*!< EEPROM quick write status query */
    EEE_COMPLETE_INTERRUPT_QUICK_WRITE      = 0xAAU,    /*!< Complete interrupted EEPROM quick write process */
    EEE_DISABLE                             = 0xFFU,    /*!< Make FlexRAM available as RAM */
} flash_flexRam_function_control_code_t;

/*!
 * @name Flash security status
 * @{
 */
/*! @brief  Flash currently not in secure state */
#define FLASH_NOT_SECURE               0x01U
/*! @brief  Flash is secured and backdoor key access enabled */
#define FLASH_SECURE_BACKDOOR_ENABLED  0x02U
/*! @brief  Flash is secured and backdoor key access disabled */
#define FLASH_SECURE_BACKDOOR_DISABLED 0x04U
/*@}*/

/*******************************************************************************
* CallBack function period
*******************************************************************************/
#ifndef FLASH_CALLBACK_CS
/*! @brief  Callback period count for FlashCheckSum
 *
 * This value is only relevant for FlashCheckSum operation, where a high rate of calling back can impair performance.
 * The rest of the flash operations invoke the callback as often as possible while waiting for the flash controller
 * to finish the requested operation.
 */
#define FLASH_CALLBACK_CS 0x0AU
#endif

/*******************************************************************************
* Null Callback function definition
*******************************************************************************/
/*!
 * @name Null Callback function definition
 * @{
 */
/*! @brief  Null callback */
#define NULL_CALLBACK      ((flash_callback_t)0xFFFFFFFFU)

#if FEATURE_FLS_HAS_PF_BLOCK_SWAP
/*! @brief  Null swap callback */
#define NULL_SWAP_CALLBACK ((flash_swap_callback_t)0xFFFFFFFFU)
#endif
/*@}*/

/*******************************************************************************
* Callback function prototype
*******************************************************************************/
/*! @brief Call back function pointer data type
 *
 *   If using callback in the application, any code reachable from this function
 *   must not be placed in a Flash block targeted for a program/erase operation.
 *   Functions can be placed in RAM section by using the START/END_FUNCTION_DEFINITION/DECLARATION_RAMSECTION macros.
 */
typedef void (* flash_callback_t)(void);
#if FEATURE_FLS_HAS_PF_BLOCK_SWAP
/*! @brief Swap call back function pointer data type */
typedef bool (* flash_swap_callback_t)(uint8_t function);
#endif

/*******************************************************************************
* Flash SSD Configuration Structure
*******************************************************************************/
/*!
 * @brief Flash User Configuration Structure
 *
 * Implements : flash_user_config_t_Class
 */
typedef struct
{
    uint32_t PFlashBase;            /*!< The base address of P-Flash memory */
    uint32_t PFlashSize;            /*!< The size in byte of P-Flash memory */
    uint32_t DFlashBase;            /*!< For FlexNVM device, this is the base address of D-Flash memory
                                     *    (FlexNVM memory); For non-FlexNVM device, this field is unused */
    uint32_t EERAMBase;             /*!< The base address of FlexRAM (for FlexNVM device)
                                     *    or acceleration RAM memory (for non-FlexNVM device) */
    flash_callback_t CallBack;      /*!< Call back function to service the time critical events. Any code reachable from this function
                                     *   must not be placed in a Flash block targeted for a program/erase operation */
} flash_user_config_t;

/*!
 * @brief Flash SSD Configuration Structure
 *
 * The structure includes the static parameters for C90TFS/FTFx which are
 * device-dependent. The fields including
 * PFlashBlockBase, PFlashBlockSize, DFlashBlockBase, EERAMBlockBase,
 * and CallBack are passed via flash_user_config_t.
 * The rest of parameters such as DFlashBlockSize, and EEEBlockSize will be
 * initialized in FLASH_DRV_Init() automatically.
 *
 * Implements : flash_ssd_config_t_Class
 */
typedef struct
{
    uint32_t PFlashBase;          /*!< The base address of P-Flash memory */
    uint32_t PFlashSize;          /*!< The size in byte of P-Flash memory */
    uint32_t DFlashBase;          /*!< For FlexNVM device, this is the base address of D-Flash memory (FlexNVM memory);
                                   *    For non-FlexNVM device, this field is unused */
    uint32_t DFlashSize;          /*!< For FlexNVM device, this is the size in byte of area
                                   *    which is used as D-Flash from FlexNVM memory;
                                   *    For non-FlexNVM device, this field is unused */
    uint32_t EERAMBase;           /*!< The base address of FlexRAM (for FlexNVM device)
                                   *    or acceleration RAM memory (for non-FlexNVM device) */
    uint32_t EEESize;             /*!< For FlexNVM device, this is the size in byte of EEPROM area which was partitioned
                                   *    from FlexRAM; For non-FlexNVM device, this field is unused */
    flash_callback_t CallBack;    /*!< Call back function to service the time critical events. Any code reachable from this function
                                   *   must not be placed in a Flash block targeted for a program/erase operation */
} flash_ssd_config_t;

/*!
 * @brief EEPROM status structure
 *
 * Implements : flash_eeprom_status_t_Class
 */
typedef struct
{
    uint8_t brownOutCode;               /*!< Brown-out detection code */
    uint16_t numOfRecordReqMaintain;    /*!< Number of EEPROM quick write records requiring maintenance */
    uint16_t sectorEraseCount;          /*!< EEPROM sector erase count */
} flash_eeprom_status_t;

/*******************************************************************************
* Function Prototypes for Flash SSD
*******************************************************************************/
/*!
 * @name Flash driver APIs
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initializes Flash.
 *
 * This API initializes Flash module by reporting the memory configuration
 * via SSD configuration structure.
 *
 * @param[in] pUserConf The user configuration structure pointer.
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 */
status_t FLASH_DRV_Init(const flash_user_config_t * const pUserConf,
                        flash_ssd_config_t * const pSSDConfig);

/*!
 * @brief P-Flash get protection.
 *
 * This API retrieves the current P-Flash protection status. Considering
 * the time consumption for getting protection is very low and even can
 * be ignored. It is not necessary to utilize the Callback function to
 * support the time-critical events.
 *
 * @param[out] protectStatus To return the current value of the P-Flash Protection.
 *                          Each bit is corresponding to protection of 1/32 of the
 *                          total P-Flash. The least significant bit is corresponding
 *                          to the lowest address area of P-Flash. The most significant bit
 *                          is corresponding to the highest address area of P-Flash
 *                          and so on. There are two possible cases as below:
 *                          - 0: this area is protected.
 *                          - 1: this area is unprotected.
 */
void FLASH_DRV_GetPFlashProtection(uint32_t * protectStatus);

/*!
 * @brief P-Flash set protection.
 *
 * This API sets the P-Flash protection to the intended protection status.
 * Setting P-Flash protection status is subject to a protection, transition
 * restriction. If there is a setting violation, it returns
 * an error code and the current protection status will not be changed.
 *
 * @param[in] protectStatus The expected protect status user wants to set to
 *                          P-Flash protection register. Each bit is corresponding
 *                          to protection of 1/32 of the total P-Flash. The least
 *                          significant bit is corresponding to the lowest
 *                          address area of P-Flash. The most significant bit
 *                          is corresponding to the highest address area of P-
 *                          Flash, and so on. There are two possible cases as shown below:
 *                          - 0: this area is protected.
 *                          - 1: this area is unprotected.
 * @return operation status
 *        - STATUS_SUCCESS:  Operation was successful.
 *        - STATUS_ERROR:    Operation failure was occurred.
 */
status_t FLASH_DRV_SetPFlashProtection(uint32_t protectStatus);

/*!
 * @brief Flash get security state.
 *
 * This API retrieves the current Flash security status, including
 * the security enabling state and the back door key enabling state.
 *
 * @param[out] securityState To return the current security status code.
 *                          - FLASH_NOT_SECURE (0x01U): Flash currently not in secure state
 *                          - FLASH_SECURE_BACKDOOR_ENABLED (0x02U): Flash is secured and
 *                          back door key access enabled
 *                          - FLASH_SECURE_BACKDOOR_DISABLED (0x04U): Flash is secured and
 *                          back door key access disabled.
 */
void FLASH_DRV_GetSecurityState(uint8_t * securityState);

/*!
 * @brief Flash security bypass.
 *
 * This API un-secures the device by comparing the user's provided back
 * door key with the ones in the Flash Configuration Field. If they are
 * matched, the security is released. Otherwise, an error code is returned.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] keyBuffer Point to the user buffer containing the back door key.
 * @return operation status
 *        - STATUS_SUCCESS:  Operation was successful.
 *        - STATUS_ERROR:    Operation failure was occurred.
 *        - STATUS_BUSY:     Operation was busy.
 */
status_t FLASH_DRV_SecurityBypass(const flash_ssd_config_t * pSSDConfig,
                                  const uint8_t * keyBuffer);

/*!
 * @brief Flash erase all blocks.
 *
 * This API erases all Flash memory, initializes the FlexRAM, verifies
 * all memory contents, and then releases the MCU security.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:     Operation was busy.
 */

status_t FLASH_DRV_EraseAllBlock(const flash_ssd_config_t * pSSDConfig);

/*!
 * @brief Flash verify all blocks.
 *
 * This function checks to see if the P-Flash and/or D-Flash, EEPROM
 * backup area, and D-Flash IFR have been erased to the specified read
 * margin level, if applicable, and releases security if the readout passes.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] marginLevel Read Margin Choice as follows:
 *                         - marginLevel = 0x0U: use the Normal read level
 *                         - marginLevel = 0x1U: use the User read
 *                         - marginLevel = 0x2U: use the Factory read
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:      Operation was busy.
 */
status_t FLASH_DRV_VerifyAllBlock(const flash_ssd_config_t * pSSDConfig,
                                  uint8_t marginLevel);

/*!
 * @brief Flash erase sector.
 *
 * This API erases one or more sectors in P-Flash or D-Flash memory.
 * This API always returns FTFx_OK if size provided by the user is
 * zero regardless of the input validation.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Address in the first sector to be erased.
 *            User need to make sure the dest address in of P-FLASH or D-FLASH block.
 *            This address should be aligned to bytes following a below table
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148 | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16   |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16   |    8     |    8     |
 *
 * @param[in] size Size to be erased in bytes. It is used to determine number of sectors to be erased.
 *            This size should be aligned to bytes following a below table
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148  | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16    |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16    |    8     |    8     |
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_EraseSector(const flash_ssd_config_t * pSSDConfig,
                               uint32_t dest,
                               uint32_t size);

/*!
 * @brief Flash verify section.
 *
 * This API checks if a section of the P-Flash or the D-Flash memory
 * is erased to the specified read margin level.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended verify operation.
 *            User need to make sure the dest address in of P-FLASH or D-FLASH block.
 *            This address should be aligned to bytes following a below table
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148  | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16    |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16    |    8     |    8     |
 *
 * @param[in] number Number of alignment unit to be verified. Refer to corresponding
 *                   reference manual to get correct information of alignment constrain.
 * @param[in] marginLevel Read Margin Choice as follows:
 *                        - marginLevel = 0x0U: use Normal read level
 *                        - marginLevel = 0x1U: use the User read
 *                        - marginLevel = 0x2U: use the Factory read
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:      Operation was busy.
 */
status_t FLASH_DRV_VerifySection(const flash_ssd_config_t * pSSDConfig,
                                 uint32_t dest,
                                 uint16_t number,
                                 uint8_t marginLevel);

/*!
 * @brief Flash erase suspend.
 *
 * This API is used to suspend a current operation of Flash erase sector command.
 * This function must be located in RAM memory or different Flash blocks which are
 * targeted for writing to avoid the RWW error.
 */
void FLASH_DRV_EraseSuspend(void);

/*!
 * @brief Flash erase resume.
 *
 * This API is used to resume a previous suspended operation of Flash erase sector command
 * This function must be located in RAM memory or different Flash blocks which are targeted
 * for writing to avoid RWW error.
 */
void FLASH_DRV_EraseResume(void);

/*!
 * @brief Flash read once.
 *
 * This API is used to read out a reserved 64 byte field located in the P-Flash IFR via given number
 * of record. See the corresponding reference manual to get the correct value of this number.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] recordIndex The record index will be read. It can be from 0x0U
 *                        to 0x7U or from 0x0U to 0xF according to specific derivative.
 * @param[in] pDataArray Pointer to the array to return the data read by the read once command.
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:      Operation was busy.
 */
status_t FLASH_DRV_ReadOnce(const flash_ssd_config_t * pSSDConfig,
                            uint8_t recordIndex,
                            uint8_t * pDataArray);

/*!
 * @brief Flash program once.
 *
 * This API is used to program to a reserved 64 byte field located in the
 * P-Flash IFR via given number of record. See the corresponding reference manual
 * to get correct value of this number.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] recordIndex The record index will be read. It can be from 0x0U
 *                        to 0x7U or from 0x0U to 0xF according to specific derivative.
 * @param[in] pDataArray Pointer to the array from which data will be
 *                       taken for program once command.
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:      Operation was busy.
 */
status_t FLASH_DRV_ProgramOnce(const flash_ssd_config_t * pSSDConfig,
                               uint8_t recordIndex,
                               const uint8_t * pDataArray);

#if FEATURE_FLS_HAS_READ_RESOURCE_CMD
/*!
 * @brief Flash read resource.
 *
 * This API is used to read data from special purpose memory in Flash memory module
 * including P-Flash IFR, swap IFR, D-Flash IFR space and version ID.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended read operation.
 *                 This address should be aligned to 8 bytes.
 * @param[out] pDataArray Pointer to the data returned by the read resource command.
 * @param[in] resourceSelectCode Read resource select code:
 *                               - 0: Flash IFR
 *                               - 1: Version ID
 * @return operation status
 *        - STATUS_SUCCESS:   Operation was successful.
 *        - STATUS_ERROR:     Operation failure was occurred.
 *        - STATUS_BUSY:      Operation was busy.
 */
status_t FLASH_DRV_ReadResource(const flash_ssd_config_t * pSSDConfig,
                                uint32_t dest,
                                uint8_t * pDataArray,
                                uint8_t resourceSelectCode);
#endif

/*!
 * @brief Flash program
 *
 * This API is used to program 4 consecutive bytes (for program long
 * word command) and 8 consecutive bytes (for program phrase command) on
 * P-Flash or D-Flash block. This API always returns FTFx_OK if size
 * provided by user is zero regardless of the input validation
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program operation.
                   This address should be aligned to 8 bytes.
 * @param[in] size Size in byte to be programmed.
                   This size should be aligned to 8 bytes.
 * @param[in] pData Pointer of source address from which data has to
 *                  be taken for program operation.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_Program(const flash_ssd_config_t * pSSDConfig,
                           uint32_t dest,
                           uint32_t size,
                           const uint8_t * pData);

/*!
 * @brief Flash program check
 *
 * This API tests a previously programmed P-Flash or D-Flash long word
 * to see if it reads correctly at the specified margin level. This
 * API always returns FTFx_OK if size provided by user is zero
 * regardless of the input validation
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program check operation.
 *                 This address should be aligned to 4 bytes.
 * @param[in] size Size in byte to check accuracy of program operation.
 *                 This size should be aligned to 4 bytes.
 * @param[in] pExpectedData The pointer to the expected data.
 * @param[in] pFailAddr Returned the first aligned failing address.
 * @param[in] marginLevel Read margin choice as follows:
 *                        - marginLevel = 0x1U: read at User margin 1/0 level.
 *                        - marginLevel = 0x2U: read at Factory margin 1/0 level.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_ProgramCheck(const flash_ssd_config_t * pSSDConfig,
                                uint32_t dest,
                                uint32_t size,
                                const uint8_t * pExpectedData,
                                uint32_t * pFailAddr,
                                uint8_t marginLevel);

/*!
 * @brief Calculates check sum.
 *
 * This API performs 32 bit sum of each byte data over a specified Flash
 * memory range without carry which provides rapid method for checking data integrity.
 * The callback time period of this API is determined via FLASH_CALLBACK_CS macro in
 * flash_driver.h which is used as a counter value for the CallBack() function calling in
 * this API. This value can be changed as per the user requirement. User can change this value
 * to obtain the maximum permissible callback time period.
 * This API always returns STATUS_SUCCESS if size provided by user is zero regardless of the input
 * validation.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address of the Flash range to be summed.
 * @param[in] size Size in byte of the Flash range to be summed.
 * @param[in] pSum To return the sum value.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 */
status_t FLASH_DRV_CheckSum(const flash_ssd_config_t * pSSDConfig,
                            uint32_t dest,
                            uint32_t size,
                            uint32_t * pSum);

#if FEATURE_FLS_HAS_PROGRAM_SECTION_CMD
/*!
 * @brief Flash program section
 *
 * This API will program the data found in the Section Program Buffer
 * to previously erased locations in the Flash memory. Data is preloaded into
 * the Section Program Buffer by writing to the acceleration Ram and FlexRam
 * while it is set to function as a RAM. The Section Program Buffer is limited
 * to the value of FlexRam divides by a ratio. Refer to the associate reference
 * manual to get correct value of this ratio.
 * For derivatives including swap feature, the swap indicator address is encountered
 * during FlashProgramSection, it is bypassed without setting FPVIOL but the content
 * are not be programmed. In addition, the content of source data used to program to
 * swap indicator will be re-initialized to 0xFF after completion of this command.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended program operation.
 *            User need to make sure the dest address in of P-FLASH or D-FLASH block.
 *            This address should be aligned to bytes following a below table:
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148  | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16    |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16    |    8     |    8     |
 *
 * @param[in] number Number of alignment unit to be programmed. Refer to associate
 *                   reference manual to get correct value of this alignment constrain.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_UNSUPPORTED:     Operation was unsupported.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_ProgramSection(const flash_ssd_config_t * pSSDConfig,
                                  uint32_t dest,
                                  uint16_t number);
#endif

#if FEATURE_FLS_HAS_ERASE_BLOCK_CMD
/*!
 * @brief Flash erase block
 *
 * This API erases all addresses in an individual P-Flash or D-Flash block.
 * For the derivatives including multiply logical P-Flash or D-Flash blocks,
 * this API erases a single block in a single call.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended erase operation.
 *            User need to make sure the dest address in of P-FLASH or D-FLASH block.
 *            This address should be aligned to bytes following a below table:
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148  | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16    |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16    |    8     |    8     |
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_EraseBlock(const flash_ssd_config_t * pSSDConfig,
                              uint32_t dest);
#endif

#if FEATURE_FLS_HAS_READ_1S_BLOCK_CMD
/*!
 * @brief Flash verify block
 *
 * This API checks to see  if an entire P-Flash or D-Flash block has been
 * erased to the specified margin level
 * For the derivatives including multiply logical P-Flash or D-Flash blocks,
 * this API erases a single block in a single call.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest start address for the intended verify operation.
 *            User need to make sure the dest address in of P-FLASH or D-FLASH block.
 *            This address should be aligned to bytes following a below table:
 *           |    FLASH TYPE/MCU   | S32K116 | S32K118 | S32K142 | S32K144 | S32K146 | S32K148  | S32K142W | S32K144W |
 *           |:-------------------:|:-------:|:-------:|:-------:|:-------:|:-------:|:--------:|:--------:|:--------:|
 *           |       P-FLASH       |    8    |    8    |    8    |    16   |    16   |    16    |    8     |    16    |
 *           |       D-FLASH       |    8    |    8    |    8    |    8    |    8    |    16    |    8     |    8     |
 * @param[in] marginLevel Read Margin Choice as follows:
 *                        - marginLevel = 0x0U: use Normal read level
 *                        - marginLevel = 0x1U: use the User read
 *                        - marginLevel = 0x2U: use the Factory read
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_VerifyBlock(const flash_ssd_config_t * pSSDConfig,
                               uint32_t dest,
                               uint8_t marginLevel);
#endif

#if FEATURE_FLS_HAS_FLEX_NVM
/*!
 * @brief EERAM get protection
 *
 * This API retrieves which EEPROM sections of FlexRAM are protected
 * against program and erase operations. Considering the time consumption
 * for getting protection is very low and even can be ignored, it is not necessary
 * to utilize the Callback function to support the time-critical events
 *
 * @param[out] protectStatus To return the current value of the EEPROM
 *                          Protection Register. Each bit is corresponding to
 *                          protection status of 1/8 of the total EEPROM
 *                          use. The least significant bit is corresponding to
 *                          the lowest address area of EEPROM. The most
 *                          significant bit is corresponding to the highest
 *                          address area of EEPROM and so on. There are
 *                          two possible cases as below:
 *                          - 0: this area is protected.
 *                          - 1: this area is unprotected.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_UNSUPPORTED:     Operation was unsupported.
 */
status_t FLASH_DRV_GetEERAMProtection(uint8_t * protectStatus);

/*!
 * @brief EERAM set protection
 *
 * This API sets protection to the intended protection status for EEPROM us
 * area of FlexRam. This is subject to a protection transition restriction.
 * If there is a setting violation, it returns failed information and
 * the current protection status will not be changed.
 *
 * @param[in] protectStatus The intended protection status value should be
 *                          written to the EEPROM Protection Register.
 *                          Each bit is corresponding to
 *                          protection status of 1/8 of the total EEPROM
 *                          use. The least significant bit is corresponding to
 *                          the lowest address area of EEPROM. The most
 *                          significant bit is corresponding to the highest
 *                          address area of EEPROM and so on. There are
 *                          two possible cases as below:
 *                          - 0: this area is protected.
 *                          - 1: this area is unprotected.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_UNSUPPORTED:     Operation was unsupported.
 */
status_t FLASH_DRV_SetEERAMProtection(uint8_t protectStatus);

/*!
 * @brief Set FlexRAM function
 *
 * This function is used to change the function of the FlexRAM. When not partitioned for
 * emulated EEPROM, the FlexRAM is typically used as traditional RAM.
 * Otherwise, the FlexRam is typically used to store EEPROM data, the writing to EEPROM is normal write or
 * quick write.
 * In addition, this function may be used to get EEPROM status or complete interrupted EEPROM quick write process.
 * For example, after partitioning to have EEPROM backup, FlexRAM is used for EEPROM
 * use accordingly and if want to change FlexRAM to traditional RAM for FlashProgramSection() use, call this API
 * with the function control code is 0xFFU.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] flexRamFuncCode FlexRam function control code. It can be:
 *                            - 0x00U: Make FlexRAM available for emulated EEPROM.
 *                            - 0x55U: Make FlexRAM available for EEPROM quick writes.
 *                            - 0x77U: EEPROM quick write status query.
 *                            - 0xAAU: Complete interrupted EEPROM quick write process.
 *                            - 0xFFU: Make FlexRAM available as RAM.
 * @param[in] byteOfQuickWrite Number of FlexRAM bytes allocated for EEPROM quick writes.
 *                              This parameter is only used for EEE_QUICK_WRITE command.
 *                              It should not be less than 16 bytes and more than 512 bytes and
 *                              It must be divided by 4 in EEPROM quick writes.
 *                              This one should be 0 if the function is not used for EEEPROM quick write.
 * @param[out] pEEPROMStatus Pointer to the EEPROM status. This parameter is only used for EEE_STATUS_QUERY command,
 *                            it should be NULL if the function is not used for EEPROM status query.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_SetFlexRamFunction(const flash_ssd_config_t * pSSDConfig,
                                      flash_flexRam_function_control_code_t flexRamFuncCode,
                                      uint16_t byteOfQuickWrite,
                                      flash_eeprom_status_t * const pEEPROMStatus);

/*!
 * @brief EEPROM Emulator Write
 *
 * This API is used to write data to FlexRAM section which is partitioned
 * as EEPROM use for EEPROM operation. After data has been written to EEPROM
 * use section of FlexRAM, the EEPROM file system will create new data record
 * in EEPROM back-up area of FlexNVM in round-robin fashion.
 * There is no alignment constraint for destination and size parameters
 * provided by user. However, according to user's input provided, this
 * API will set priority to write to FlexRAM with following rules:
 * 32-bit writing is invoked if destination is 32 bit aligned and size
 * is not less than 32 bits.
 * 16-bit writing is invoked if destination is 16 bit aligned and size
 * is not less than 16 bits.
 * 8-bit writing is invoked if destination is 8 bit aligned and size is not less than 8 bits.
 * S32k14xW platform supports only FlexRAM aligned writes of 4 bytes (32-bits) at a time
 * User must read data with the destination is aligned 4 for S32K14xW platform when the FlexRAM is used as EEPROM. 
 * So, the data will be not presented continuously in your application. 
 * It should be used in the size of bytes which are aligned by 4. Although it can write with any size of data.
 * When EEPROM was set to EEPROM quick writes mode, the dest address 
 * should be aligned 4 bytes,the size should be divided by 4.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @param[in] dest Start address for the intended write operation.
 * @param[in] size Size in byte to be written.
 * @param[in] pData Pointer to source address from which data
 *                  has to be taken for writing operation.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 */
status_t FLASH_DRV_EEEWrite(const flash_ssd_config_t * pSSDConfig,
                            uint32_t dest,
                            uint32_t size,
                            const uint8_t * pData);

/*!
 * @brief Flash D/E-Flash Partition.
 *
 * This API prepares the FlexNVM block for use as D-Flash, EEPROM backup, or a combination
 * of both and initializes the FlexRAM.
 * In addition, this function is used to configure number of user keys and user key 'Verify Only' attribute if
 * CSEc is enabled.
 *
 * The single partition choice should be used through entire life time of a given
 * application to guarantee the Flash endurance and data retention of Flash module.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer
 * @param[in] uEEEDataSizeCode EEPROM Data Size Code
 * @param[in] uDEPartitionCode FlexNVM Partition Code
 * @param[in] uCSEcKeySize CSEc Key Size Code, it should be 0 if the function is not used for configuring CSEc part.
 *             It can be:
 *             - 0x00U: Number of User Keys is Zero
 *                      Number of Bytes (subtracts from the total 4K EEERAM space) is 0
 *             - 0x01U: Number of User Keys is 1 to 6 keys
 *                      Number of Bytes (subtracts from the total 4K EEERAM space) is 128 bytes
 *             - 0x02U: Number of User Keys is 1 to 12 keys
 *                      Number of Bytes (subtracts from the total 4K EEERAM space) is 256 bytes
 *             - 0x03U: Number of User Keys is 1 to 24 keys
 *                      Number of Bytes (subtracts from the total 4K EEERAM space) is 512 bytes
 * @param[in] uSFE Security Flag Extension, it should be false if the function is not used for configuring CSEc part.
 * @param[in] flexRamEnableLoadEEEData FlexRAM load during reset option.
 *            - true : FlexRAM loaded with valid EEPROM data during reset sequence
 *            - false : FlexRAM not loaded during reset sequence
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_DEFlashPartition(const flash_ssd_config_t * pSSDConfig,
                                    uint8_t uEEEDataSizeCode,
                                    uint8_t uDEPartitionCode,
                                    uint8_t uCSEcKeySize,
                                    bool uSFE,
                                    bool flexRamEnableLoadEEEData);

/*!
 * @brief D-Flash get protection.
 *
 * This API retrieves current P-Flash protection status. Considering the time consumption
 * for getting protection is very low and even can be ignored, it is not necessary to utilize
 * the Callback function to support the time-critical events.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer
 * @param[out] protectStatus To return the current value of the D-Flash Protection
 *                          Register. Each bit is corresponding to protection status
 *                          of 1/8 of the total D-Flash. The least significant bit is
 *                          corresponding to the lowest address area of D-Flash. The
 *                          most significant bit is corresponding to the highest address
 *                          area of D-Flash and so on. There are two possible cases as below:
 *                          - 0 : this area is protected.
 *                          - 1 : this area is unprotected.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_UNSUPPORTED:     Operation was unsupported.
 */
status_t FLASH_DRV_GetDFlashProtection(const flash_ssd_config_t * pSSDConfig,
                                       uint8_t * protectStatus);

/*!
 * @brief D-Flash set protection.
 *
 * This API sets the D-Flash protection to the intended protection status. Setting D-Flash
 * protection status is subject to a protection transition restriction. If there is a setting
 * violation, it returns failed information and the current protection status will not be changed.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer
 * @param[in] protectStatus The expected protect status user wants to set to D-Flash Protection
 *                          Register. Each bit is corresponding to protection status
 *                          of 1/8 of the total D-Flash. The least significant bit is
 *                          corresponding to the lowest address area of D-Flash. The
 *                          most significant bit is corresponding to the highest address
 *                          area of D-Flash and so on. There are two possible cases as below:
 *                          - 0 : this area is protected.
 *                          - 1 : this area is unprotected.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_UNSUPPORTED:     Operation was unsupported.
 */
status_t FLASH_DRV_SetDFlashProtection(const flash_ssd_config_t * pSSDConfig,
                                       uint8_t protectStatus);
#endif /* End of FEATURE_FLS_HAS_FLEX_NVM */

#if FEATURE_FLS_HAS_PF_BLOCK_SWAP
/*!
 * @brief Swaps between the two halves of the total logical P-Flash memory blocks in the memory map.
 *
 * The swap API provides to user with an ability to interfere in a swap progress by letting the
 * user code know about the swap state in each phase of the process. This is done via pSwapCallBack()
 * parameter. To stop at each intermediate swap state, set the return value of
 * this callback function to FALSE. To complete swap process within a single call,
 * set the return value of this function to TRUE.
 *
 * Erase the non-active swap indicator  somewhere in the
 * application code or in the swap call back function when swap system is in UPDATE state.
 *
 * In addition, if user does not want to use the swap call back parameter, pass the NULL_SWAP_CALLBACK
 * as a null pointer. In this situation, the PFlashSwap() behaves in the same way as setting the return
 * value of pSwapCallBack to TRUE and the user does not need to erase the non-active swap
 * indicator when the swap system is in UPDATE state.
 *
 * Below is an example to show how to implement a swap callback:
 * @code
 * bool PFlashSwapCallback(uint8_t currentSwapMode)
 * {
 *     switch (currentSwapMode)
 *     {
 *     case FTFx_SWAP_UNINI:
 *         Put your application-specific code here
 *         break;
 *     case FTFx_SWAP_READY:
 *         Put your application-specific code here
 *         break;
 *     case FTFx_SWAP_UPDATE:
 *         Put your application-specific code here (example: erase non-active swap indicator here)
 *         break;
 *     case FTFx_SWAP_UPDATE_ERASED:
 *         Put your application-specific code here (example: erase non-active swap indicator here)
 *         break;
 *     case FTFx_SWAP_COMPLETE:
 *         Put your application-specific code here
 *         break;
 *     default:
 *         break;
 *    }
 *    return TRUE; Return FALSE to stop at intermediate swap state
 * }
 * @endcode
 * The swap indicator provided by the user must be within the lower half of P-Flash block but not in the
 * Flash configuration area. If P-Flash block has two logical blocks, the swap indicator must be
 * in P-Flash block 0. If the P-Flash block has four logical blocks, the swap indicator can be in block
 * 0 or block 1. It must not be in the Flash configuration field.
 * The user must use the same swap indicator for all swap control code except report swap status once
 * swap system has been initialized. To refresh swap system to un-initialization state,
 * use the FlashEraseAllBlock() to clean up the swap environment.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer
 * @param[in] addr Address of swap indicator.
 * @param[in] pSwapCallback Callback to do specific task while the swapping is being performed
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 */
status_t FLASH_DRV_PFlashSwap(const flash_ssd_config_t * pSSDConfig,
                              uint32_t addr,
                              flash_swap_callback_t pSwapCallback);

/*!
 * @brief Performs swap control command corresponding with the swap control code provided via the swap command parameter.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer
 * @param[in] addr Address of swap indicator.
 * @param[in] swapcmd Swap Control Code:
 *                    - 0x01U: Initialize Swap System
 *                    - 0x02U: Set Swap in Update State
 *                    - 0x04U: Set Swap in Complete State
 *                    - 0x08U: Report Swap Status
 * @param[in] pCurrentSwapMode Current Swap Mode:
 *                             - 0x00U: Uninitialized
 *                             - 0x01U: Ready
 *                             - 0x02U: Update
 *                             - 0x03U: Update-Erased
 *                             - 0x04U: Complete
 * @param[in] pCurrentSwapBlockStatus Current Swap Block Status indicates which program Flash block
 *                                    is currently located at relative Flash address 0x0_0000
 *                                    - 0x00U: Program Flash block 0
 *                                    - 0x01U: Program Flash block 1
 * @param[in] pNextSwapBlockStatus Next Swap Block Status indicates which program Flash block
 *                                 is located at relative Flash address 0x0_0000 after the next reset.
 *                                 - 0x00U: Program Flash block 0
 *                                 - 0x01U: Program Flash block 1
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_PFlashSwapCtl(const flash_ssd_config_t * pSSDConfig,
                                 uint32_t addr,
                                 uint8_t swapcmd,
                                 uint8_t * pCurrentSwapMode,
                                 uint8_t * pCurrentSwapBlockStatus,
                                 uint8_t * pNextSwapBlockStatus);

#endif /* End of FEATURE_FLS_HAS_PF_BLOCK_SWAP */

#if FEATURE_FLS_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD
/*!
 * @brief Flash erase all blocks unsecure.
 *
 * This API erases all Flash memory, initializes the FlexRAM, verifies
 * all memory contents, and then releases the MCU security.
 *
 * @param[in] pSSDConfig The SSD configuration structure pointer.
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 *        - STATUS_ERROR:           Operation failure was occurred.
 *        - STATUS_BUSY:            Operation was busy.
 */
status_t FLASH_DRV_EraseAllBlockUnsecure(const flash_ssd_config_t * pSSDConfig);
#endif /* End of FEATURE_FLS_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD */

/*!
 * @brief Enable the command complete interrupt.
 *
 * This function will enable the command complete interrupt is generated when
 * an FTFC command completes.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableCmdCompleteInterupt(void);

/*!
 * @brief Disable the command complete interrupt.
 *
 */
void FLASH_DRV_DisableCmdCompleteInterupt(void);

/*!
 * @brief Check the command complete flag has completed or not.
 *
 * @return the command complete flag
 *        - true: The FTFC command has completed.
 *        - false: The FTFC command is in progress.
 *
 * Implements    : FLASH_DRV_GetCmdCompleteFlag_Activity
 */
static inline bool FLASH_DRV_GetCmdCompleteFlag(void)
{
    return (((FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) >> FTFx_FSTAT_CCIF_SHIFT) != 0U);
}

/*!
 * @brief Enable the read collision error interrupt.
 *
 * This function will enable the read collision error interrupt generation when an
 * FTFC read collision error occurs.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableReadColisionInterupt(void);

/*!
 * @brief Disable the read collision error interrupt.
 *
 */
void FLASH_DRV_DisableReadColisionInterupt(void);

/*!
 * @brief Check the read collision error flag is detected or not.
 *
 * @return the read collision error flag
 *        - true: Collision error detected.
 *        - false: No collision error detected.
 *
 * Implements    : FLASH_DRV_GetReadColisionFlag_Activity
 */
static inline bool FLASH_DRV_GetReadColisionFlag(void)
{
    return (((FTFx_FSTAT & FTFx_FSTAT_RDCOLERR_MASK) >> FTFx_FSTAT_RDCOLERR_SHIFT) != 0U);
}

/*!
 * @brief Clear the read collision error flag.
 *
 * Implements    : FLASH_DRV_ClearReadColisionFlag_Activity
 */
static inline void FLASH_DRV_ClearReadColisionFlag(void)
{
    FTFx_FSTAT = FTFx_FSTAT_RDCOLERR_MASK;
}

#if FEATURE_FLS_HAS_DETECT_ECC_ERROR
/*!
 * @brief Enable the double bit fault detect interrupt.
 *
 * This function will enable the double bit fault detect interrupt generation when
 * an uncorrectable ECC fault is detected during a valid flash read access from
 * the platform flash controller.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableDoubleBitFaultInterupt(void);

/*!
 * @brief Disable the double bit fault detect interrupt.
 *
 */
void FLASH_DRV_DisableDoubleBitFaultInterupt(void);

/*!
 * @brief Check the double bit fault flag is detected during a valid
 * flash read access from the platform flash controller
 *
 * @return the platform flash error status
 *        - true: Double bit fault detected.
 *        - false: Double bit fault not detected.
 *
 * Implements    : FLASH_DRV_GetDoubleBitFaultFlag_Activity
 */
static inline bool FLASH_DRV_GetDoubleBitFaultFlag(void)
{
    return (((FTFx_FERSTAT & FTFx_FERSTAT_DFDIF_MASK) >> FTFx_FERSTAT_DFDIF_SHIFT) != 0U);
}

/*!
 * @brief Clear the platform Flash double bit fault detect flag.
 *
 * Implements    : FLASH_DRV_ClearDoubleBitFaultFlag_Activity
 */
static inline void FLASH_DRV_ClearDoubleBitFaultFlag(void)
{
    FTFx_FERSTAT = FTFx_FERSTAT_DFDIF_MASK;
}

/*!
 * @brief Force Double Bit Fault Detect.
 *
 * This API will enable the user to emulate the setting of the double bit fault
 * detect interrupt flag to check the associated interrupt routine.
 *
 * @param[in] isEnable Enable/disable the user to emulate the double bit fault detect flag.
 *
 * Implements    : FLASH_DRV_ForceDoubleBitFaultDetectCmd_Activity
 */
static inline void FLASH_DRV_ForceDoubleBitFaultDetectCmd(bool isEnable)
{
    if (true == isEnable)
    {
        FTFx_FERCNFG |= FTFx_FERCNFG_FDFD_MASK;
    }
    else
    {
        FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_FDFD_MASK);
    }
}

#if (FEATURE_FLS_IS_FTFM == 1U)
/*!
 * @brief Enable the platform FlexRAM double bit fault detect interrupt 
 *
 * This function will Enable the platform FlexRAM double bit fault detect interrupt 
 * generation when an uncorrectable ECC fault is detected detected during a valid
 * FlexRAM or CSE_PRAM read access from the platform RAM controller.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableFlexRAMDoubleBitFaultInterupt(void);

/*!
 * @brief Disable the platform FlexRAM double bit fault detect interrupt 
 *
 */
void FLASH_DRV_DisableFlexRamDoubleBitFaultInterupt(void);

/*!
 * @brief Enable the controller Flash double bit fault detect interrupt 
 *
 * This function will Enable the controller Flash double bit fault detect interrupt 
 * generation when an uncorrectable ECC fault is detected during a valid
 * internal RAM or flash read access from the flash memory controller.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableCtrFlashDoubleBitFaultInterupt(void);

/*!
 * @brief Disable the controller Flash double bit fault detect interrupt 
 *
 */
void FLASH_DRV_DisableCtrFlashDoubleBitFaultInterupt(void);

/*!
 * @brief Enable the controller FlexRAM double bit fault detect interrupt 
 *
 * This function will Enable the controller FlexRAM double bit fault detect interrupt 
 *  generation when an uncorrectable ECC fault is detected during a valid FlexRAM 
 * or CSE_PRAM read access from the flash memory controller.
 *
 * @return operation status
 *        - STATUS_SUCCESS:         Operation was successful.
 */
status_t FLASH_DRV_EnableCtrFlexRamDoubleBitFaultInterupt(void);


/*!
 * @brief Disable the controller FlexRAM double bit fault detect interrupt
 *
 */
void FLASH_DRV_DisableCtrFlexRamDoubleBitFaultInterupt(void);


/*!
 * @brief Check the double bit fault flag is detected during a valid 
 * FlexRAM or CSE_PRAM read access from the platform flash controller
 *
 * @return the platform FlexRam error status
 *        - true: Double bit fault detected.
 *        - false: Double bit fault not detected.
 *
 * Implements    : FLASH_DRV_GetFlexRamDoubleBitFaultFlag_Activity
 */
static inline bool FLASH_DRV_GetFlexRamDoubleBitFaultFlag(void)
{
    return (((FTFx_FERSTAT & FTFx_FERSTAT_PDFDIF_MASK) >> FTFx_FERSTAT_PDFDIF_SHIFT) != 0U);
}

/*!
 * @brief Clear the platform FlexRAM Double Bit Fault Detect Flag
 *
 * Implements    : FLASH_DRV_ClearFlexRamDoubleBitFaultFlag_Activity
 */
static inline void FLASH_DRV_ClearFlexRamDoubleBitFaultFlag(void)
{
    FTFx_FERSTAT = FTFx_FERSTAT_PDFDIF_MASK;
}

/*!
 * @brief Platform FlexRAM Force Double Bit Fault Detect
 *
 * This API will enable the user to emulate the setting of the double bit fault
 * detect interrupt flag to check the associated interrupt routine.
 *
 * @param[in] isEnable Enable/disable the user to emulate the double bit fault detect flag.
 *
 * Implements    : FLASH_DRV_ForceFlexRamDoubleBitFaultDetectCmd_Activity
 */
static inline void FLASH_DRV_ForceFlexRamDoubleBitFaultDetectCmd(bool isEnable)
{
    if (true == isEnable)
    {
        FTFx_FERCNFG |= FTFx_FERCNFG_PFDFD_MASK;
    }
    else
    {
        FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_PFDFD_MASK);
    }
}

/*!
 * @brief Check the double bit fault flag is detected during a valid internal 
 * RAM or flash read access from the flash memory controller
 *
 * @return the controller Flash error status
 *        - true: Double bit fault detected.
 *        - false: Double bit fault not detected.
 *
 * Implements    : FLASH_DRV_GetCtrFlashDoubleBitFaultFlag_Activity
 */
static inline bool FLASH_DRV_GetCtrFlashDoubleBitFaultFlag(void)
{
    return (((FTFx_FERSTAT & FTFx_FERSTAT_CDFDIF_MASK) >> FTFx_FERSTAT_CDFDIF_SHIFT) != 0U);
}

/*!
 * @brief Clear the controller Flash Double Bit Fault Detect Flag
 *
 * Implements    : FLASH_DRV_ClearCtrFlashDoubleBitFaultFlag_Activity
 */
static inline void FLASH_DRV_ClearCtrFlashDoubleBitFaultFlag(void)
{
    FTFx_FERSTAT = FTFx_FERSTAT_CDFDIF_MASK;
}

/*!
 * @brief Controller Flash Force Double Bit Fault Detect
 *
 * This API will enable the user to emulate the setting of the double bit fault
 * detect interrupt flag to check the associated interrupt routine.
 *
 * @param[in] isEnable Enable/disable the user to emulate the double bit fault detect flag.
 *
 * Implements    : FLASH_DRV_CtrFlashForceDoubleBitFaultDetectCmd_Activity
 */
static inline void FLASH_DRV_CtrFlashForceDoubleBitFaultDetectCmd(bool isEnable)
{
    if (true == isEnable)
    {
        FTFx_FERCNFG |= FTFx_FERCNFG_CFDFD_MASK;
    }
    else
    {
        FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_CFDFD_MASK);
    }
}

/*!
 * @brief Check the double bit fault flag is detected during a valid FlexRAM or
 * CSE_PRAM read access from the flash memory controller. 
 *
 * @return the controller FlexRam error status
 *        - true: Double bit fault detected.
 *        - false: Double bit fault not detected.
 *
 * Implements    : FLASH_DRV_GetCtrFlexRamDoubleBitFaultFlag_Activity
 */
static inline bool FLASH_DRV_GetCtrFlexRamDoubleBitFaultFlag(void)
{
    return (((FTFx_FERSTAT & FTFx_FERSTAT_EDFDIF_MASK) >> FTFx_FERSTAT_EDFDIF_SHIFT) != 0U);
}

/*!
 * @brief Clear the controller FlexRAM Double Bit Fault Detect Flag
 *
 * Implements    : FLASH_DRV_ClearCtrFlexRamDoubleBitFaultFlag_Activity
 */
static inline void FLASH_DRV_ClearCtrFlexRamDoubleBitFaultFlag(void)
{
    FTFx_FERSTAT = FTFx_FERSTAT_EDFDIF_MASK;
}

/*!
 * @brief Controller FlexRAM Force Double Bit Fault Detect
 *
 * This API will enable the user to emulate the setting of the double bit fault
 * detect interrupt flag to check the associated interrupt routine.
 *
 * @param[in] isEnable Enable/disable the user to emulate the double bit fault detect flag.
 *
 * Implements    : FLASH_DRV_CtrFlexRamForceDoubleBitFaultDetectCmd_Activity
 */
static inline void FLASH_DRV_CtrFlexRamForceDoubleBitFaultDetectCmd(bool isEnable)
{
    if (true == isEnable)
    {
        FTFx_FERCNFG |= FTFx_FERCNFG_EFDFD_MASK;
    }
    else
    {
        FTFx_FERCNFG &= (uint8_t)(~FTFx_FERCNFG_EFDFD_MASK);
    }
}
#endif /* End of FEATURE_FLS_IS_FTFM */

#endif /* End of FEATURE_FLS_HAS_DETECT_ECC_ERROR */

/*!
 * @brief Get default flash configuration.
 *
 * This API will get default flash user configuration.
 *
 * @param[out] config: Pointer flash user configuration structure.
 */
void FLASH_DRV_GetDefaultConfig(flash_user_config_t * const config);

#if defined(__cplusplus)
}
#endif

/*@}*/ /* End of Flash driver APIs*/
/*! @}*/ /* End of addtogroup flash_driver */

#endif /* FLASH_DRIVER_H */
