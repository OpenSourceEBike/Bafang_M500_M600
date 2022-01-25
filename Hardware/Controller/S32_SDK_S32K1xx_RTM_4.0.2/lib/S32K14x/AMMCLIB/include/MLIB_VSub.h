/******************************************************************************
*
*   Copyright 2020 NXP
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
* @file       MLIB_VSub.h
*
* @version    1.0.0.0
*
* @date       28-May-2020
*
* @brief      Header file for MLIB_VSub function
*
******************************************************************************/
#ifndef MLIB_VSUB_H
#define MLIB_VSUB_H
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
* @section MLIB_VSub_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_VSub_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_VSub_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_VSub_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_VSub_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_SubSat.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_Sub.h"
  #include "CCOV_MLIB_SubSat.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_VSub(...)     macro_dispatcher(MLIB_VSub, __VA_ARGS__)(__VA_ARGS__)     /* This function returns sum of two input parameters. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00302 */
    #define MLIB_VSub_Dsptchr_3(Out,In1,In2)     MLIB_VSub_Dsptchr_4(Out,In1,In2,F32)       /* Function dispatcher for MLIB_VSub_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00302 */
    #define MLIB_VSub_Dsptchr_3(Out,In1,In2)     MLIB_VSub_Dsptchr_4(Out,In1,In2,F16)       /* Function dispatcher for MLIB_VSub_Dsptchr_2, do not modify!!! */
  #endif
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00302 */
    #define MLIB_VSub_Dsptchr_3(Out,In1,In2)     MLIB_VSub_Dsptchr_4(Out,In1,In2,FLT)       /* Function dispatcher for MLIB_VSub_Dsptchr_2, do not modify!!! */
  #endif
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
  /*
  * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_VSub_Dsptchr_4(Out,In1,In2,Impl)    MLIB_VSub_Dsptchr_(Out,In1,In2,Impl)     /* Function dispatcher for MLIB_VSub_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_VSub_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_VSub_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_VSub_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref MLIB_VSub_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00304*/
  #define MLIB_VSub_Dsptchr_(Out,In1,In2,Impl)     MLIB_VSub_##Impl(Out,In1,In2)            /* Function dispatcher for MLIB_VSub_Dsptchr_, do not modify!!! */
#endif

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/

/****************************************************************************
* Exported function prototypes
****************************************************************************/

/****************************************************************************
* Inline functions
****************************************************************************/


/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_VSub

@param[out]        f32Out     Subtraction of two input vectors.

@param[in]         f32In1     Vector representing first operand.

@param[in]         f32In2     Vector representing second operand.

@details      The input values as well as output value is considered as vectors defined by two 
              32-bit fractional coordinates stored in SWLIBS_2Syst_F32 structure.

@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.

@par Code Example:
\code
#include "mlib.h"

SWLIBS_2Syst_F32 f32In1, f32In2;
SWLIBS_2Syst_F32 f32Out;

void main(void)
{
  // input vector 1 = (0.5,0.5)
  f32In1.f32Arg1 = FRAC32(0.5);
  f32In1.f32Arg2 = FRAC32(0.5);
  // input vector 2 = (0.25,0.25)
  f32In2.f32Arg1 = FRAC32(0.25);
  f32In2.f32Arg2 = FRAC32(0.25);

  // Both f32Out.f32Arg1 and f32Out.f32Arg2 should be FRAC32(0.25) = 0x20000000
  MLIB_VSub_F32(&f32Out, &f32In1, &f32In2);

  // Both f32Out.f32Arg1 and f32Out.f32Arg2 should be FRAC32(0.25) = 0x20000000
  MLIB_VSub(&f32Out, &f32In1, &f32In2, F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // Both f32Out.f32Arg1 and f32Out.f32Arg2 should be FRAC32(0.25) = 0x20000000
  MLIB_VSub(&f32Out, &f32In1, &f32In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00298, DMLIB00301, DMLIB00303 */
INLINE void MLIB_VSub_F32(SWLIBS_2Syst_F32 *const f32Out, const SWLIBS_2Syst_F32 *const f32In1, const SWLIBS_2Syst_F32 *const f32In2)
/*
* @violates @ref MLIB_VSub_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  f32Out->f32Arg1 = MLIB_SubSat_F32(f32In1->f32Arg1, f32In2->f32Arg1);
  f32Out->f32Arg2 = MLIB_SubSat_F32(f32In1->f32Arg2, f32In2->f32Arg2);
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_VSub

@param[out]        f16Out     Subtraction of two input vectors.

@param[in]         f16In1     Vector representing first operand.

@param[in]         f16In2     Vector representing second operand.

@details      The input values as well as output value is considered as vectors defined by two 
              16-bit fractional coordinates stored in SWLIBS_2Syst_F16 structure.

@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.

@par Code Example:
\code
#include "mlib.h"

SWLIBS_2Syst_F16 f16In1, f16In2;
SWLIBS_2Syst_F16 f16Out;

void main(void)
{
  // input vector 1 = (0.5,0.5)
  f16In1.f16Arg1 = FRAC16(0.5);
  f16In1.f16Arg2 = FRAC16(0.5);
  // input vector 2 = (0.25,0.25)
  f16In2.f16Arg1 = FRAC16(0.25);
  f16In2.f16Arg2 = FRAC16(0.25);

  // Both f16Out.f16Arg1 and f16Out.f16Arg2 should be FRAC16(0.25) = 0x2000
  MLIB_VSub_F16(&f16Out, &f16In1, &f16In2);

  // Both f16Out.f16Arg1 and f16Out.f16Arg2 should be FRAC16(0.25) = 0x2000
  MLIB_VSub(&f16Out, &f16In1, &f16In2, F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // Both f16Out.f16Arg1 and f16Out.f16Arg2 should be FRAC16(0.25) = 0x2000
  MLIB_VSub(&f16Out, &f16In1, &f16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00298, DMLIB00300, DMLIB00303 */
INLINE void MLIB_VSub_F16(SWLIBS_2Syst_F16 *const f16Out, const SWLIBS_2Syst_F16 *const f16In1, const SWLIBS_2Syst_F16 *const f16In2)
/*
* @violates @ref MLIB_VSub_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  f16Out->f16Arg1 = MLIB_SubSat_F16(f16In1->f16Arg1, f16In2->f16Arg1);
  f16Out->f16Arg2 = MLIB_SubSat_F16(f16In1->f16Arg2, f16In2->f16Arg2);
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_VSub

@param[out]        fltOut     Subtraction of two input vectors.

@param[in]         fltIn1     Vector representing first operand.

@param[in]         fltIn2     Vector representing second operand.

@details      The input values as well as output value is considered as vectors defined by two 
              single precision floating-point coordinates stored in SWLIBS_2Syst_FLT structure.

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,
              overflow, underflow, inexact, input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.

@par Code Example
\code
#include "mlib.h"

SWLIBS_2Syst_FLT fltIn1, fltIn2;
SWLIBS_2Syst_FLT fltOut;

void main(void)
{
  // input vector 1 = (0.5,0.5)
  fltIn1.fltArg1 = 0.5;
  fltIn1.fltArg2 = 0.5;
  // input vector 2 = (0.25,0.25)
  fltIn2.fltArg1 = 0.25;
  fltIn2.f1tArg2 = 0.25;

  // Both fltOut.fltArg1 and fltOut.fltArg2 should be 0.25
  MLIB_VSub_FLT(&fltOut, &fltIn1, &fltIn2);

  // Both fltOut.fltArg1 and fltOut.fltArg2 should be 0.25
  MLIB_VSub(&fltOut, &fltIn1, &fltIn2, FLT);

  // ##############################################################
  // Available only if single precision floating-point
  // implementation selected as default
  // ##############################################################

  // Both fltOut.fltArg1 and fltOut.fltArg2 should be 0.25
  MLIB_VSub(&fltOut, &fltIn1, &fltIn2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00298, DMLIB00299, DMLIB00303 */
INLINE void MLIB_VSub_FLT(SWLIBS_2Syst_FLT *const fltOut, const SWLIBS_2Syst_FLT *const fltIn1, const SWLIBS_2Syst_FLT *const fltIn2)
/*
* @violates @ref MLIB_VSub_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  fltOut->fltArg1 = MLIB_Sub_FLT(fltIn1->fltArg1, fltIn2->fltArg1);
  fltOut->fltArg2 = MLIB_Sub_FLT(fltIn1->fltArg2, fltIn2->fltArg2);
}

#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* MLIB_VSUB_H */
