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
* @file       MLIB_MulSat.h
*
* @version    1.0.78.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_MulSat function
*
******************************************************************************/
#ifndef MLIB_MULSAT_H
#define MLIB_MULSAT_H
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
* @section MLIB_MulSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_MulSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_MulSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_MulSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_MulSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*
* @section MLIB_MulSat_h_REF_6
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_Mul.h"
#else
  /* Following include serves for NXP internal testing purposes only. 
  *  This header is not part of the release. */
  #include "CCOV_MLIB_Mul.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_MulSat(...)     macro_dispatcher(MLIB_MulSat, __VA_ARGS__)(__VA_ARGS__)     /* This function multiply two input parameters and saturate if necessary. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00101 */
    #define MLIB_MulSat_Dsptchr_2(In1,In2)     MLIB_MulSat_Dsptchr_3(In1,In2,F32)     /* Function dispatcher for MLIB_MulSat_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00101 */
    #define MLIB_MulSat_Dsptchr_2(In1,In2)     MLIB_MulSat_Dsptchr_3(In1,In2,F16)     /* Function dispatcher for MLIB_MulSat_Dsptchr_2, do not modify!!! */
  #endif

  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_MulSat_Dsptchr_3(In1,In2,Impl)    MLIB_MulSat_Dsptchr_(In1,In2,Impl)     /* Function dispatcher for MLIB_MulSat_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref MLIB_MulSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00103 */
  #define MLIB_MulSat_Dsptchr_(In1,In2,Impl)     MLIB_MulSat_##Impl(In1,In2)            /* Function dispatcher for MLIB_MulSat_Dsptchr_, do not modify!!! */
#endif
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/

/****************************************************************************
* Exported function prototypes
****************************************************************************/

/****************************************************************************
* inline functions
****************************************************************************/
INLINE tFrac32 MulSat_F32_C(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tS32 s32Temp;

  s32Temp = F32TOINT32(MLIB_Mul_F32(f32In1, f32In2));
  s32Temp = (s32Temp == INT32_MIN) ? INT32_MAX : s32Temp;
  return(INT32TOF32(s32Temp));
}
INLINE tFrac32 MulSat_F32F16F16_C(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tS32 s32Temp;

  s32Temp = F32TOINT32(MLIB_Mul_F32F16F16(f16In1, f16In2));
  s32Temp = (s32Temp == INT32_MIN) ? INT32_MAX : s32Temp;
  return(INT32TOF32(s32Temp));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac32 MulSat_F32_GCC_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Low, f32High, f32Return;

  __asm volatile(" smull %0,%1,%2,%3" : "=r"(f32Low), "=r"(f32High): "r"(f32In1), "r"(f32In2));
  __asm volatile(" lsr %0,%1,#31" : "=r"(f32Return): "r"(f32Low));
  __asm volatile(" qdadd %0,%0,%1" : "+r"(f32Return): "r"(f32High));
  return(f32Return);
}
INLINE tFrac32 MulSat_F32F16F16_GCC_A32T32(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Return;

  __asm(" smulbb %0,%1,%2" : "=r"(f32Return): "r"(f16In1), "r"(f16In2));
  __asm(" qadd %0,%0,%0" : "+r"(f32Return): );
  return(f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac32 MulSat_F32_IAR_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Low, f32High, f32Return;

  __asm(" smull %0,%1,%2,%3" : "=r"(f32Low), "=r"(f32High): "r"(f32In1), "r"(f32In2));
  __asm(" lsr %0,%1,#31" : "=r"(f32Return): "r"(f32Low));
  __asm(" qdadd %0,%0,%1" : "+r"(f32Return): "r"(f32High));
  return(f32Return);
}
INLINE tFrac32 MulSat_F32F16F16_IAR_A32T32(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Return;

  __asm(" smulbb %0,%1,%2" : "=r"(f32Return): "r"(f16In1), "r"(f16In2));
  __asm(" qadd %0,%0,%0" : "+r"(f32Return): );
  return(f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac32 MulSat_F32_GHS_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Low, f32High, f32Return;

  asm(" smull %0,%1,%2,%3" : "=r"(f32Low), "=r"(f32High): "r"(f32In1) "r"(f32In2));
  asm(" lsr %0,%1,#31" : "=r"(f32Return): "r"(f32Low));
  asm(" qdadd %0,%0,%1" : "+r"(f32Return): "r"(f32High));
  return(f32Return);
}
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac32 MulSat_F32F16F16_GHS_A32T32(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
  * file. 
  */
  register tFrac32 f32Return;

  asm(" smulbb %0,%1,%2" : "=r"(f32Return): "r"(f16In1) "r"(f16In2));
  asm(" qadd %0,%0,%0" : "+r"(f32Return): );
  return(f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */
INLINE tFrac16 MulSat_F16_C(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tS16 s16Temp;

  s16Temp = F16TOINT16(MLIB_Mul_F16(f16In1, f16In2));
  s16Temp = (s16Temp == INT16_MIN) ? INT16_MAX : s16Temp;
  return(INT16TOF16(s16Temp));
}





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_MulSat

@param[in]    f32In1     Operand is a 32-bit number normalized between [-1,1).

@param[in]    f32In2     Operand is a 32-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      The input values as well as output value are considered as 32-bit fractional data
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F32
              \image rtf mulsatF32Eq1.math "MLIB_MulSat_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In1;
tFrac32 f32In2;
tFrac32 f32Out;

void main(void)
{
  // first input = 0.8
  f32In1 = FRAC32(0.8);

  // second input = 0.75
  f32In2 = FRAC32(0.75);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat_F32(f32In1,f32In2);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2,F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00102, DMLIB00095, DMLIB00097, DMLIB00100, DMLIB00105 */
INLINE tFrac32 MLIB_MulSat_F32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32_GCC_A32T32(f32In1,f32In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32_IAR_A32T32(f32In1,f32In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32_GHS_A32T32(f32In1,f32In2));
  #else
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32_C(f32In1,f32In2));
  #endif
}





/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_MulSat

@param[in]    f16In1     Operand is a 16-bit number normalized between [-1,1).

@param[in]    f16In2     Operand is a 16-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      The input values are considered as 16-bit fractional data type and the output value is
              considered as 32-bit fractional data type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F32F16F16
              \image rtf mulsatF32F16F16Eq1.math "MLIB_MulSat_F32F16F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In1;
tFrac16 f16In2;
tFrac32 f32Out;

void main(void)
{
  // first input = 0.8
  f16In1 = FRAC16(0.8);

  // second input = 0.75
  f16In2 = FRAC16(0.75);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat_F32F16F16(f16In1,f16In2);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2,F32F16f16);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00102, DMLIB00095, DMLIB00099, DMLIB00100, DMLIB00105 */
INLINE tFrac32 MLIB_MulSat_F32F16F16(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32F16F16_GCC_A32T32(f16In1,f16In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32F16F16_IAR_A32T32(f16In1,f16In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32F16F16_GHS_A32T32(f16In1,f16In2));
  #else
    /* @remarks Implements DMLIB00098 */
    return(MulSat_F32F16F16_C(f16In1,f16In2));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_MulSat

@param[in]    f16In1     Operand is a 16-bit number normalized between [-1,1).

@param[in]    f16In2     Operand is a 16-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      The input values as well as output value are considered as 16-bit fractional data
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F16
              \image rtf mulsatF16Eq1.math "MLIB_MulSat_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In1;
tFrac16 f16In2;
tFrac16 f16Out;

void main(void)
{
  // first input = 0.8
  f16In1 = FRAC16(0.8);

  // second input = 0.75
  f16In2 = FRAC16(0.75);

  // output should be 0x4ccc = FRAC16(0.6)
  f16Out = MLIB_MulSat_F16(f16In1,f16In2);

  // output should be 0x4ccc = FRAC16(0.6)
  f16Out = MLIB_MulSat(f16In1,f16In2,F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be 0x4ccc = FRAC32(0.6)
  f16Out = MLIB_MulSat(f16In1,f16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00102, DMLIB00095, DMLIB00096, DMLIB00100, DMLIB00106 */
INLINE tFrac16 MLIB_MulSat_F16(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  /* @remarks Implements DMLIB00098 */
  return(MulSat_F16_C(f16In1,f16In2));
}


#ifdef __cplusplus
}
#endif

#endif /* MLIB_MULSAT_H */
