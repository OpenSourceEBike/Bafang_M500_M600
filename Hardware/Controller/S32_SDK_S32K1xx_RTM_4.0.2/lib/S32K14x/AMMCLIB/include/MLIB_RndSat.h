/******************************************************************************
*
*   Copyright 2013-2015 Freescale Semiconductor
*   Copyright 2016-2019 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in
*   accordance with the applicable license terms.  By expressly accepting such terms or by
*   downloading, installing, activating and/or otherwise using the software, you are agreeing that
*   you have read, and that you agree to comply with and are bound by, such license terms.  If you
*   do not agree to be bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
*
******************************************************************************/
/**
*
* @file       MLIB_RndSat.h
*
* @version    1.0.10.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_RndSat function
*
******************************************************************************/
#ifndef MLIB_RNDSAT_H
#define MLIB_RNDSAT_H
/**
@if MLIB_GROUP
    @addtogroup MLIB_GROUP
@else
    @defgroup MLIB_GROUP   MLIB
@endif
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section MLIB_RndSat_h_REF_1
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*
* @section MLIB_RndSat_h_REF_2
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
*/

#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_AddSat.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_AddSat.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/

/****************************************************************************
* Exported function prototypes
****************************************************************************/

/****************************************************************************
* Inline functions
****************************************************************************/

/* MLIB_RndSat_F16F32 implementation variant - C */
INLINE tFrac16 RndSat_F16F32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_RndSat_h_REF_1 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_RndSat_h_REF_2 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac16)(MLIB_AddSat_F32(f32In,(tFrac32)0x00008000) >> 16u));
}






/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_RndSat

@brief        This function rounds the input value to the nearest saturated value in the output
              format.

@param[in]    f32In      Input value.

@return       Rounded saturated value.

@details      This inline function rounds the input value to the nearest saturated value in the output
              format. The input value is considered as 32-bit fractional data type and output value
              is considered as 16-bit fractional data type.

@par Re-entrancy
              The function is re-entrant.

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In;
tFrac16 f16Out;

void main(void)
{
    // input value = 0.25 = 0x2000 0000
    f32In = FRAC32(0.25);

    // output should be FRAC16(0.25) = 0x2000
    f16Out = MLIB_RndSat_F16F32(f32In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00286, DMLIB00287, DMLIB00288, DMLIB00289, DMLIB00290 */
INLINE tFrac16 MLIB_RndSat_F16F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_RndSat_h_REF_1 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
    /* @remarks Implements DMLIB00288 */
    return(RndSat_F16F32_C(f32In));
}




#ifdef __cplusplus
}
#endif

#endif /* MLIB_RNDSAT_H */
