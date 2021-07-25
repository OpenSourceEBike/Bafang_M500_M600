/******************************************************************************
*
* Copyright 2015-2016 Freescale
* Copyright 2019 NXP
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
#ifndef M4_SCST_TEST_SHELL_H
#define M4_SCST_TEST_SHELL_H


/******************************************************************************
* Includes
******************************************************************************/
#include "m4_scst_typedefs.h"

/******************************************************************************
* Macros
******************************************************************************/

/* Return values for Fsl_call_test_execution_icst function */

#define M4_SCST_INIT_OK                             0xFF000000U

#define M4_SCST_WRONG_RANGE                         0xFF030303U
#define M4_SCST_TEST_WAS_INTERRUPTED                0xFF121212U
#define M4_SCST_FAULT_INJECT                        0x53481521U
#define M4_SCST_DEFAULT_FAULT_INJECT_TEST_INDEX     0xFFFFFFFFU


/******************************************************************************
* Global Variables
******************************************************************************/

extern m4_scst_uint32_t m4_scst_last_executed_test_number;
extern m4_scst_vuint32_t m4_scst_test_was_interrupted;
extern m4_scst_vuint32_t m4_scst_accumulated_signature;
extern m4_scst_uint32_t m4_scst_fault_inject_test_index;


/******************************************************************************
* Global functions
******************************************************************************/

m4_scst_uint32_t m4_scst_execute_core_tests(m4_scst_uint32_t start, m4_scst_uint32_t end);


#endif /* M4_SCST_TEST_SHELL_H */
