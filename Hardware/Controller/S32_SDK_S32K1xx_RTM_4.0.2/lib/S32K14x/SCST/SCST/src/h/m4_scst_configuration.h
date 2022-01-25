/******************************************************************************
*
* Copyright 2015-2016 Freescale
* Copyright 2016-2017, 2019 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only 
* be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing, activating
* and/or otherwise using the software, you are agreeing that you have read, 
* and that you agree to comply with and are bound by, such license terms. 
* If you do not agree to be bound by the applicable license terms, 
* then you may not retain, install, activate or otherwise use the software.
*
******************************************************************************/


#ifndef M4_SCST_CONFIGURATION_H
#define M4_SCST_CONFIGURATION_H


/**********************************************************************************
* Definitions related to single-core / multi-core execution mode
**********************************************************************************/

#define M4_SCST_TESTS_NUM   44U

/* Nested Vector Interrupt (NVIC) registers */
#define M4_ICTR_REG     0xE000E004

/* System Control Block (SCB) registers  */
#define M4_ICSR_REG     0xE000ED04
#define M4_VTOR_REG     0xE000ED08
#define M4_CCR_REG      0xE000ED14
#define M4_SHPR1_REG    0xE000ED18
#define M4_SHPR2_REG    0xE000ED1C
#define M4_SHPR3_REG    0xE000ED20
#define M4_SHCSR_REG    0xE000ED24
#define M4_CFSR_REG     0xE000ED28
#define M4_MMFSR_REG    0xE000ED28
#define M4_BFSR_REG     0xE000ED29
#define M4_UFSR_REG     0xE000ED2A
#define M4_HFSR_REG     0xE000ED2C
#define M4_BFAR_REG     0xE000ED38


#endif /* M4_SCST_CONFIGURATION_H */
