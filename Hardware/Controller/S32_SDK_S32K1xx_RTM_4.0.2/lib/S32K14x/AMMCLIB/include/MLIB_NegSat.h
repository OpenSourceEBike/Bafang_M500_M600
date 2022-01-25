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
* @file       MLIB_NegSat.h
*
* @version    1.0.70.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_NegSat function
*
******************************************************************************/
#ifndef MLIB_NEGSAT_H
#define MLIB_NEGSAT_H
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
* @section MLIB_NegSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_NegSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_NegSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_NegSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_NegSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*
* @section MLIB_NegSat_h_REF_6
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/

#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_Neg.h"
#else
  /* Following include serves for NXP internal testing purposes only. 
  *  This header is not part of the release. */
  #include "CCOV_MLIB_Neg.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_NegSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_NegSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_NegSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_NegSat(...)     macro_dispatcher(MLIB_NegSat, __VA_ARGS__)(__VA_ARGS__)     /* This function returns negative value of input parameter and saturate if necessary. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_NegSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_NegSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_NegSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00036 */
    #define MLIB_NegSat_Dsptchr_1(In)     MLIB_NegSat_Dsptchr_2(In,F32)     /* Function dispatcher for MLIB_NegSat_Dsptchr_1, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_NegSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_NegSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_NegSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00036 */
    #define MLIB_NegSat_Dsptchr_1(In)     MLIB_NegSat_Dsptchr_2(In,F16)     /* Function dispatcher for MLIB_NegSat_Dsptchr_1, do not modify!!! */
  #endif

  /*
  * @violates @ref MLIB_NegSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_NegSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_NegSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_NegSat_Dsptchr_2(In,Impl)    MLIB_NegSat_Dsptchr_(In,Impl)     /* Function dispatcher for MLIB_NegSat_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref MLIB_NegSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_NegSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_NegSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00038 */
  #define MLIB_NegSat_Dsptchr_(In,Impl)     MLIB_NegSat_##Impl(In)            /* Function dispatcher for MLIB_NegSat_Dsptchr_, do not modify!!! */
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
INLINE tFrac32 NegSat_F32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Out;
  
  if (f32In == INT32TOF32(INT32_MIN)){
    f32Out = INT32TOF32(INT32_MAX);
  }else{
    f32Out = MLIB_Neg_F32(f32In);
  }
  return(INT32TOF32(f32Out));  
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac32 NegSat_F32_GCC_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  __asm volatile(" mov %0,#0" : "=r"(f32Return):);
  __asm volatile(" qsub %0,%0,%1" : "+r"(f32Return): "r"(f32In));
  return(f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac32 NegSat_F32_IAR_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  __asm(" mov %0,#0" : "=r"(f32Return):);
  __asm(" qsub %0,%0,%1" : "+r"(f32Return): "r"(f32In));
  return(f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac32 NegSat_F32_GHS_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  asm(" mov %0,#0" : "=r"(f32Return):);
  asm(" qsub %0,%0,%1" : "+r"(f32Return): "r"(f32In));
  return(f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */
INLINE tFrac16 NegSat_F16_C(register tFrac16 f16In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Out;
  
  if (f16In == INT16TOF16(INT16_MIN)){
    f16Out = INT16TOF16(INT16_MAX);
  }else{
    f16Out = MLIB_Neg_F16(f16In);
  }
  return(f16Out);
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac16 NegSat_F16_GCC_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm volatile(" mov %0,#0" : "=r"(f16Return):);
  __asm volatile(" qsub16 %0,%0,%1" : "+r"(f16Return): "r"(f16In));
  __asm volatile(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac16 NegSat_F16_IAR_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm(" mov %0,#0" : "=r"(f16Return):);
  __asm(" qsub16 %0,%0,%1" : "+r"(f16Return): "r"(f16In));
  __asm(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac16 NegSat_F16_GHS_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  asm(" mov %0,#0" : "=r"(f16Return):);
  asm(" qsub16 %0,%0,%1" : "+r"(f16Return): "r"(f16In));
  asm(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_NegSat

@param[in]    f32In      Input value which negative value should be returned.

@return       Negative value of input parameter.

@details      The input values as well as output value is considered as 32-bit fractional data 
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_NegSat_F32
              \image rtf negsatF32Eq1.math "MLIB_NegSat_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly, and thus is not ANSI-C compliant.
*/
/**

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
  f32Out = MLIB_NegSat_F32(f32In);

  // output should be FRAC32(-0.25) = 0xA0000000
  f32Out = MLIB_NegSat(f32In, F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC32(-0.25) = 0xA0000000
  f32Out = MLIB_NegSat(f32In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00037, DMLIB00031, DMLIB00033, DMLIB00035, DMLIB00040 */
INLINE tFrac32 MLIB_NegSat_F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F32_GCC_A32T32(f32In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F32_IAR_A32T32(f32In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F32_GHS_A32T32(f32In));
  #else
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F32_C(f32In));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_NegSat

@param[in]    f16In      Input value which negative value should be returned.

@return       Negative value of input parameter.

@details      The input values as well as output value is considered as 16-bit fractional data 
              type.

              \par

              The output of the function is defined by the following simple equation:

              \anchor eq1_NegSat_F16
              \image rtf negsatF16Eq1.math "MLIB_NegSat_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly, and thus is not ANSI-C compliant.
*/
/**

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
  f16Out = MLIB_NegSat_F16(f16In);

  // output should be FRAC16(-0.25) = 0xA000
  f16Out = MLIB_NegSat(f16In, F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC16(-0.25) = 0xA000
  f16Out = MLIB_NegSat(f16In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00037, DMLIB00031, DMLIB00032, DMLIB00035, DMLIB00041 */
INLINE tFrac16 MLIB_NegSat_F16(register tFrac16 f16In)
/*
* @violates @ref MLIB_NegSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F16_GCC_A32T32(f16In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F16_IAR_A32T32(f16In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F16_GHS_A32T32(f16In));
  #else
    /* @remarks Implements DMLIB00034 */
    return(NegSat_F16_C(f16In));
  #endif
}


#ifdef __cplusplus
}
#endif

#endif /* MLIB_NEGSAT_H */
