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
#ifndef M4_SCST_TEST_DATA_H
#define M4_SCST_TEST_DATA_H


/******************************************************************************
* Includes / Defines
******************************************************************************/
#include "m4_scst_typedefs.h"
  
/******************************************************************************
* Types
******************************************************************************/
  
/* The m4_scst_atomic_test_icst_DESC_t data type specifies a descriptor for individual atomic test. */
typedef struct {
    m4_scst_uint32_t (* atomic_test_start_address) (void);
    m4_scst_uint32_t expected_signature;
} m4_scst_atomic_test_icst_DESC_t;


#endif /* M4_SCST_TEST_DATA_H */
