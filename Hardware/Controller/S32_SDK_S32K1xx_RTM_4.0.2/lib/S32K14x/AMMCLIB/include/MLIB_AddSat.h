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
* @file       MLIB_AddSat.h
*
* @version    1.0.82.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_AddSat function
*
******************************************************************************/
#ifndef MLIB_ADDSAT_H
#define MLIB_ADDSAT_H
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
* @section MLIB_AddSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_AddSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_AddSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_AddSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_AddSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_AddSat_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
*
* @section MLIB_AddSat_h_REF_7
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_Add.h"
#else
  /* Following include serves for NXP internal testing purposes only. 
  *  This header is not part of the release. */
  #include "CCOV_MLIB_Add.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_AddSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_AddSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_AddSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_AddSat(...)     macro_dispatcher(MLIB_AddSat, __VA_ARGS__)(__VA_ARGS__)     /* This function returns sum of two input parameters and saturate if necessary. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_AddSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_AddSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_AddSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00057 */
    #define MLIB_AddSat_Dsptchr_2(In1,In2)     MLIB_AddSat_Dsptchr_3(In1,In2,F32)     /* Function dispatcher for MLIB_AddSat_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_AddSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_AddSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_AddSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00057 */
    #define MLIB_AddSat_Dsptchr_2(In1,In2)     MLIB_AddSat_Dsptchr_3(In1,In2,F16)     /* Function dispatcher for MLIB_AddSat_Dsptchr_2, do not modify!!! */
  #endif

  /*
  * @violates @ref MLIB_AddSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_AddSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_AddSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_AddSat_Dsptchr_3(In1,In2,Impl)    MLIB_AddSat_Dsptchr_(In1,In2,Impl)     /* Function dispatcher for MLIB_AddSat_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_AddSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_AddSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_AddSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref MLIB_AddSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00059*/  
  #define MLIB_AddSat_Dsptchr_(In1,In2,Impl)     MLIB_AddSat_##Impl(In1,In2)            /* Function dispatcher for MLIB_AddSat_Dsptchr_, do not modify!!! */
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
/* MLIB_AddSat_F32 implementation variant - C */
INLINE tFrac32 AddSat_F32_C(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tS32 s32z;
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tS32 s32SatMin, s32SatMax;

  s32z = F32TOINT32(MLIB_Add_F32(f32In1, f32In2));

  /*
  * @violates @ref MLIB_AddSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type (|). 
  * @violates @ref MLIB_AddSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type (&). 
  * @violates @ref MLIB_AddSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type (~). 
  */
  s32SatMax = ~(F32TOINT32(f32In1) | F32TOINT32(f32In2)) & s32z;
  /*
  * @violates @ref MLIB_AddSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type (~). 
  * @violates @ref MLIB_AddSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type (&). 
  */
  s32SatMin = (F32TOINT32(f32In1) & F32TOINT32(f32In2)) & ~s32z;
  s32z = (s32SatMin < 0) ? INT32_MIN : s32z;
  s32z = (s32SatMax < 0) ? INT32_MAX : s32z;

  return(INT32TOF32(s32z));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac32 AddSat_F32_GCC_A32T32(register tFrac32 f32In1, register tFrac32 f32In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  __asm volatile(" qadd %0,%1,%2" : "=r"(f32Return): "r"(f32In1), "r"(f32In2));
  return(f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac32 AddSat_F32_IAR_A32T32(register tFrac32 f32In1, register tFrac32 f32In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  __asm(" qadd %0,%1,%2" : "=r"(f32Return): "r"(f32In1), "r"(f32In2));
  return(f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac32 AddSat_F32_GHS_A32T32(register tFrac32 f32In1, register tFrac32 f32In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;

  asm(" qadd %0,%1,%2" : "=r"(f32Return): "r"(f32In1) "r"(f32In2));
  return(f32Return);
}
#endif /* __GNUC__ && __ARM_EABI__ */
/* MLIB_AddSat_F16 implementation variant - C */
INLINE tFrac16 AddSat_F16_C(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tS32 s32z;

  /* Cast the input values to 32-bit and sum in 32-bit */
  s32z = F32TOINT32(MLIB_Add_F32((tFrac32)(f16In1), (tFrac32)(f16In2)));

  s32z = (s32z > (INT16TOINT32(INT16_MAX))) ? (INT16TOINT32(INT16_MAX)) : s32z;
  s32z = (s32z < (INT16TOINT32(INT16_MIN))) ? (INT16TOINT32(INT16_MIN)) : s32z;

  return(INT32TOF16(s32z));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac16 AddSat_F16_GCC_A32T32(register tFrac16 f16In1, register tFrac16 f16In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm volatile(" qadd16 %0,%1,%2" : "=r"(f16Return): "r"(f16In1), "r"(f16In2));
  __asm volatile(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac16 AddSat_F16_IAR_A32T32(register tFrac16 f16In1, register tFrac16 f16In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm(" qadd16 %0,%1,%2" : "=r"(f16Return): "r"(f16In1), "r"(f16In2));
  __asm(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac16 AddSat_F16_GHS_A32T32(register tFrac16 f16In1, register tFrac16 f16In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  asm(" qadd16 %0,%1,%2" : "=r"(f16Return): "r"(f16In1) "r"(f16In2));
  asm(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#endif /* __GNUC__ && __ARM_EABI__ */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_AddSat

@param[in]         f32In1     First value to be add.

@param[in]         f32In2     Second value to be add.

@return       Sum of two input values, saturated if necessary.

@details      The input values as well as output value is considered as 32-bit fractional data 
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_AddSat_F32
              \image rtf addsatF32Eq1.math "MLIB_AddSat_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In1, f32In2;
tFrac32 f32Out;

void main(void)
{
  // input value 1 = 0.25
  f32In1 = FRAC32(0.25);
  // input value 2 = 0.25
  f32In2 = FRAC32(0.25);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_AddSat_F32(f32In1, f32In2);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_AddSat(f32In1, f32In2, F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_AddSat(f32In1, f32In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00058, DMLIB00052, DMLIB00054, DMLIB00056, DMLIB00061 */
INLINE tFrac32 MLIB_AddSat_F32(register tFrac32 f32In1, register tFrac32 f32In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file.
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F32_GCC_A32T32(f32In1,f32In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F32_IAR_A32T32(f32In1,f32In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F32_GHS_A32T32(f32In1,f32In2));
  #else
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F32_C(f32In1,f32In2));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_AddSat

@param[in]         f16In1     First value to be add.

@param[in]         f16In2     Second value to be add.

@return       Sum of two input values, saturated if necessary.

@details      The input values as well as output value is considered as 16-bit fractional data 
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_AddSat_F16
              \image rtf addsatF16Eq1.math "MLIB_AddSat_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline assembly, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In1, f16In2;
tFrac16 f16Out;

void main(void)
{
  // input value 1 = 0.25
  f16In1 = FRAC16(0.25);
  // input value 2 = 0.25
  f16In2 = FRAC16(0.25);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_AddSat_F16(f16In1, f16In2);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_AddSat(f16In1, f16In2, F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_AddSat(f16In1, f16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00058, DMLIB00052, DMLIB00053, DMLIB00056, DMLIB00062 */
INLINE tFrac16 MLIB_AddSat_F16(register tFrac16 f16In1, register tFrac16 f16In2)
/*
* @violates @ref MLIB_AddSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F16_GCC_A32T32(f16In1,f16In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F16_IAR_A32T32(f16In1,f16In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F16_GHS_A32T32(f16In1,f16In2));
  #else
    /* @remarks Implements DMLIB00055 */
    return(AddSat_F16_C(f16In1,f16In2));
  #endif
}

#ifdef __cplusplus
}
#endif

#endif /* MLIB_ADDSAT_H */
