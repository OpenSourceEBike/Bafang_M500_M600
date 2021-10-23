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
* @file       MLIB_Neg.h
*
* @version    1.0.60.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_Neg function
*
******************************************************************************/
#ifndef MLIB_NEG_H
#define MLIB_NEG_H
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
* @section MLIB_Neg_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_Neg_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_Neg_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_Neg_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_Neg_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*
* @section MLIB_Neg_h_REF_6
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/

#include "SWLIBS_Defines.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Neg_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Neg(...)     macro_dispatcher(MLIB_Neg, __VA_ARGS__)(__VA_ARGS__)    /* This function returns negative value of input parameter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Neg_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00027 */
    #define MLIB_Neg_Dsptchr_1(In)     MLIB_Neg_Dsptchr_2(In,F32)                   /* Function dispatcher for MLIB_Neg_Dsptchr_1, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Neg_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00027 */
    #define MLIB_Neg_Dsptchr_1(In)     MLIB_Neg_Dsptchr_2(In,F16)                   /* Function dispatcher for MLIB_Neg_Dsptchr_1, do not modify!!! */
  #endif
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Neg_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00027 */
    #define MLIB_Neg_Dsptchr_1(In)     MLIB_Neg_Dsptchr_2(In,FLT)                   /* Function dispatcher for MLIB_Neg_Dsptchr_1, do not modify!!! */
  #endif
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */

  /*
  * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Neg_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Neg_Dsptchr_2(In,Impl)    MLIB_Neg_Dsptchr_(In,Impl)     /* Function dispatcher for MLIB_Neg_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref MLIB_Neg_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Neg_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Neg_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00029 */
  #define MLIB_Neg_Dsptchr_(In,Impl)     MLIB_Neg_##Impl(In)            /* Function dispatcher for MLIB_Neg_Dsptchr_, do not modify!!! */
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

@functionality MLIB_Neg

@param[in]    f32In      Input value which negative value should be returned.

@return       Negative value of input parameter.

@details      The input values as well as output value is considered as 32-bit fractional values.
              The output saturation is not implemented in this function, thus in case the negation
              of input values is outside the [-1, 1) interval, the output value will overflow.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Neg_F32
              \image rtf negF32Eq1.math "MLIB_Neg_F32_Eq1"

@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In;
tFrac32 f32Out;

void main(void)
{
  // input value = 0.25
  f32In = FRAC32(0.25);

  // output should be FRAC32(-0.25) = 0xA0000000
  f32Out = MLIB_Neg_F32(f32In);

  // output should be FRAC32(-0.25) = 0xA0000000
  f32Out = MLIB_Neg(f32In, F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC32(-0.25) = 0xA0000000
  f32Out = MLIB_Neg(f32In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00028, DMLIB00021, DMLIB00024, DMLIB00026 */
INLINE tFrac32 MLIB_Neg_F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Neg_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
    /* @remarks Implements DMLIB00025 */
    return((tFrac32)(-f32In));
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Neg

@param[in]    f16In       Input value which negative value should be returned.

@return       Negative value of input parameter.

@details      The input values as well as output value is considered as 16-bit fractional values.
              The output saturation is not implemented in this function, thus in case the negation
              of input values is outside the [-1, 1) interval, the output value will overflow.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Neg_F16
              \image rtf negF16Eq1.math "MLIB_Neg_F16_Eq1"

@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In;
tFrac16 f16Out;

void main(void)
{
  // input value = 0.25
  f16In = FRAC16(0.25);

  // output should be FRAC16(-0.25) = 0xA000
  f16Out = MLIB_Neg_F16(f16In);

  // output should be FRAC16(-0.25) = 0xA000
  f16Out = MLIB_Neg(f16In, F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC16(-0.25) = 0xA000
  f16Out = MLIB_Neg(f16In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00028, DMLIB00021, DMLIB00023, DMLIB00026 */
INLINE tFrac16 MLIB_Neg_F16(register tFrac16 f16In)
/*
* @violates @ref MLIB_Neg_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
    /* @remarks Implements DMLIB00025 */
    return((tFrac16)(-f16In));
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Neg

@param[in]    fltIn      Input value which negative value should be returned.

@return       Negative value of input parameter.

@details      The input values as well as output value is considered as single precision floating
              point data type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Neg_FLT
              \image rtf negFLTEq1.math "MLIB_Neg_FLT_Eq1"

@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.

@par Code Example
\code
#include "mlib.h"

tFloat fltIn;
tFloat fltOut;

void main(void)
{
  // input value = 0.25
  fltIn = (tFloat)0.25;

  // output should be (-0.25)
  fltOut = MLIB_Neg_FLT(fltIn);

  // output should be (-0.25)
  fltOut = MLIB_Neg(fltIn, FLT);

  // ##############################################################
  // Available only if single precision floating point
  // implementation selected as default
  // ##############################################################

  // output should be (-0.25)
  fltOut = MLIB_Neg(fltIn);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00028, DMLIB00021, DMLIB00022, DMLIB00026 */
INLINE tFloat MLIB_Neg_FLT(register tFloat fltIn)
/*
* @violates @ref MLIB_Neg_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00025 */
  return((tFloat)(-fltIn));
}


#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* MLIB_NEG_H */
