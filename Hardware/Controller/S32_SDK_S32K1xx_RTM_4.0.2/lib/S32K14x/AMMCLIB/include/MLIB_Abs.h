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
* @file       MLIB_Abs.h
*
* @version    1.0.67.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_Abs function
*
******************************************************************************/
#ifndef MLIB_ABS_H
#define MLIB_ABS_H
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
* @section MLIB_Abs_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro.
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_Abs_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_Abs_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_Abs_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_Abs_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*
* @section MLIB_Abs_h_REF_6
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/

#include "SWLIBS_Defines.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Abs_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Abs(...)     macro_dispatcher(MLIB_Abs, __VA_ARGS__)(__VA_ARGS__)     /* This function returns absolute value of input parameter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Abs_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00006 */  
    #define MLIB_Abs_Dsptchr_1(In)     MLIB_Abs_Dsptchr_2(In,F32)     /* Function dispatcher for MLIB_Abs_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Abs_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00006 */
    #define MLIB_Abs_Dsptchr_1(In)     MLIB_Abs_Dsptchr_2(In,F16)     /* Function dispatcher for MLIB_Abs_Dsptchr_1, do not modify!!! */
  #endif 
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Abs_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00006 */
    #define MLIB_Abs_Dsptchr_1(In)     MLIB_Abs_Dsptchr_2(In,FLT)     /* Function dispatcher for MLIB_Abs_Dsptchr_1, do not modify!!! */
  #endif 
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */

  /*
  * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Abs_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Abs_Dsptchr_2(In,Impl)    MLIB_Abs_Dsptchr_(In,Impl)     /* Function dispatcher for MLIB_Abs_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref MLIB_Abs_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Abs_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Abs_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00008 */
  #define MLIB_Abs_Dsptchr_(In,Impl)     MLIB_Abs_##Impl(In)            /* Function dispatcher for MLIB_Abs_Dsptchr_, do not modify!!! */
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
/* MLIB_Abs_F32 implementation variant - C */
INLINE tFrac32 Abs_F32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file.
*/
{
  return((f32In < (tFrac32)0) ? (-f32In) : (f32In));
}
/* MLIB_Abs_F16 implementation variant - C */
INLINE tFrac16 Abs_F16_C(register tFrac16 f16In)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  return((f16In < (tFrac16)0) ? (-f16In) : (f16In));
}
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/* MLIB_Abs_FLT implementation variant - C */
INLINE tFloat Abs_FLT_C(register tFloat fltIn)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  return((fltIn<(tFloat)0) ? (-fltIn) : (fltIn));
}
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Abs

@param[in]    f32In      Input value.

@return       Absolute value of input parameter.

@details      The input value as well as output value is considered as 32-bit fractional data type.
              The output saturation is not implemented in this function, thus in case the absolute
              value of input parameter is outside the [-1, 1) interval, the output value will
              overflow.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Abs_F32
              \image rtf absF32Eq1.math "MLIB_Abs_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In;
tFrac32 f32Out;

void main(void)
{
    // input value = -0.25
    f32In = FRAC32(-0.25);

    // output should be FRAC32(0.25)
    f32Out = MLIB_Abs_F32(f32In);

    // output should be FRAC32(0.25)
    f32Out = MLIB_Abs(f32In, F32);

    // ##############################################################
    // Available only if 32-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be FRAC32(0.25)
    f32Out = MLIB_Abs(f32In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00007, DMLIB00000, DMLIB00003, DMLIB00005 */
INLINE tFrac32 MLIB_Abs_F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00004 */
  return(Abs_F32_C(f32In));
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Abs

@param[in]    f16In      Input value.

@return       Absolute value of input parameter.

@details      The input value as well as output value is considered as 16-bit fractional data type.
              The output saturation is not implemented in this function, thus in case the absolute
              value of input parameter is outside the [-1, 1) interval, the output value will
              overflow.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Abs_F16
              \image rtf absF16Eq1.math "MLIB_Abs_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In;
tFrac16 f16Out;

void main(void)
{
    // input value = -0.25
    f16In = FRAC16(-0.25);

    // output should be FRAC16(0.25)
    f16Out = MLIB_Abs_F16(f16In);

    // output should be FRAC16(0.25)
    f16Out = MLIB_Abs(f16In, F16);

    // ##############################################################
    // Available only if 16-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be FRAC16(0.25)
    f16Out = MLIB_Abs(f16In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00007, DMLIB00000, DMLIB00002, DMLIB00005 */
INLINE tFrac16 MLIB_Abs_F16(register tFrac16 f16In)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00004 */
  return(Abs_F16_C(f16In));
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Abs

@param[in]    fltIn      Input value.

@return       Absolute value of input parameter.

@details      The input value as well as output value is considered as single precision floating
              point data type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_Abs_FLT
              \image rtf absFLTEq1.math "MLIB_Abs_FLT_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFloat fltIn;
tFloat fltOut;

void main(void)
{
    // input value = -0.25
    fltIn = (tFloat)-0.25;

    // output should be 0.25
    fltOut = MLIB_Abs_FLT(fltIn);

    // output should be 0.25
    fltOut = MLIB_Abs(fltIn, FLT);

    // ##############################################################
    // Available only if single precision floating point
    // implementation selected as default
    // ##############################################################

    // output should be 0.25
    fltOut = MLIB_Abs(fltIn);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00007, DMLIB00000, DMLIB00001, DMLIB00005 */
INLINE tFloat MLIB_Abs_FLT(register tFloat fltIn)
/*
* @violates @ref MLIB_Abs_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00004 */
  return(Abs_FLT_C(fltIn));
}


#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* MLIB_ABS_H */
