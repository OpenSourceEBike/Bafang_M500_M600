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
* @file       MLIB_ShLSat.h
*
* @version    1.0.57.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_ShLSat function
*
******************************************************************************/
#ifndef MLIB_SHLSAT_H
#define MLIB_SHLSAT_H
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
* @section MLIB_ShLSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_ShLSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_ShLSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_ShLSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_ShLSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_ShLSat_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
* 
* @section MLIB_ShLSat_h_REF_7
* Violates MISRA 2004 Required Rule 10.5, Shift left of signed quantity (int). 
* Shifting of signed values is intentionally handled when necessary. 
*
* @section MLIB_ShLSat_h_REF_8
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/

#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_ShL.h"
  #include "MLIB_ShR.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_ShL.h"
  #include "CCOV_MLIB_ShR.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_ShLSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ShLSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShLSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_ShLSat(...)     macro_dispatcher(MLIB_ShLSat, __VA_ARGS__)(__VA_ARGS__)     /* This function shifts the first parameter to left by number defined by second parameter and saturate if necessary. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_ShLSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_ShLSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_ShLSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00174 */
    #define MLIB_ShLSat_Dsptchr_2(In1,In2)     MLIB_ShLSat_Dsptchr_3(In1,In2,F32)     /* Function dispatcher for MLIB_ShLSat_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_ShLSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_ShLSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_ShLSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00174 */
    #define MLIB_ShLSat_Dsptchr_2(In1,In2)     MLIB_ShLSat_Dsptchr_3(In1,In2,F16)     /* Function dispatcher for MLIB_ShLSat_Dsptchr_2, do not modify!!! */
  #endif

  /*
  * @violates @ref MLIB_ShLSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ShLSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShLSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_ShLSat_Dsptchr_3(In1,In2,Impl)    MLIB_ShLSat_Dsptchr_(In1,In2,Impl)   /* Function dispatcher for MLIB_ShLSat_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_ShLSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ShLSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShLSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref MLIB_ShLSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00176 */
  #define MLIB_ShLSat_Dsptchr_(In1,In2,Impl)     MLIB_ShLSat_##Impl(In1,In2)          /* Function dispatcher for MLIB_ShLSat_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/

/****************************************************************************
* Exported function prototypes
****************************************************************************/

/****************************************************************************
* Inline functions
****************************************************************************/
/* MLIB_ShLSat_F32 implementation variant - C */
INLINE tFrac32 ShLSat_F32_C(register tFrac32 f32In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Out;
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Temp;

  f32Out = MLIB_ShL_F32(f32In, u16In);
  f32Temp = MLIB_ShR_F32(f32Out, u16In);
  if(f32In != f32Temp){
    f32Temp = MLIB_ShR_F32(f32In, 31u);
    /*
    * @violates @ref MLIB_ShLSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
    * signed underlying type. 
    */
    f32Out = f32Temp ^ INT32_MAX;
  }
  return(f32Out);
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac32 ShLSat_F32_GCC_A32T32(register tFrac32 f32In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return, f32Temp, f32OutMax;

  __asm volatile(" ubfx %0,%1,#31,#1" : "=r"(f32Return) : "r"(f32In));
  __asm volatile(" mvn %0,#0x80000000": "=r"(f32OutMax) :);
  __asm volatile(" add %0,%1" : "+r"(f32OutMax) : "r"(f32Return));
  __asm volatile(" lsl %0,%3,%4\n"
        " asr %1,%0,%4\n"
        " cmp %1,%3\n"
        " it ne\n"
        " movne %0,%2" : "+r"(f32Return), "+r"(f32Temp): "r"(f32OutMax), "r"(f32In), "r"(u16In): "cc");
  return(f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac32 ShLSat_F32_IAR_A32T32(register tFrac32 f32In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return, f32Temp, f32OutMax;

  __asm(" ubfx %0,%1,#31,#1" : "=r"(f32Return) : "r"(f32In));
  __asm(" mvn %0,#0x80000000": "=r"(f32OutMax) :); 
  __asm(" add %0,%1" : "+r"(f32OutMax) : "r"(f32Return));
  __asm(" lsl %0,%2,%3\n"
        " asr %1,%0,%3": "+r"(f32Return), "=r"(f32Temp) : "r"(f32In), "r"(u16In): "cc");
  __asm( "cmp %1,%3\n"
        " it ne\n"
        " movne %0,%2" : "+r"(f32Return): "r"(f32Temp), "r"(f32OutMax), "r"(f32In): "cc");
  return(f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */
/* MLIB_ShLSat_F16 implementation variant - C */
INLINE tFrac16 ShLSat_F16_C(register tFrac16 f16In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac16 f16Out;
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac16 f16Temp;

  f16Out = MLIB_ShL_F16(f16In, u16In);
  f16Temp = MLIB_ShR_F16(f16Out, u16In);
  if(f16In != f16Temp){
    f16Temp = MLIB_ShR_F16(f16In, 15u);
    /*
    * @violates @ref MLIB_ShLSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
    * signed underlying type. 
    */
    f16Out = f16Temp ^ INT16_MAX;
  }
  return(f16Out);
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac16 ShLSat_F16_GCC_A32T32(register tFrac16 f16In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return, f32Temp, f32OutMax,f32In;
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm volatile(" lsl %0,%1,#16"  : "=r"(f32In) : "r"(f16In));
  __asm volatile(" ubfx %0,%1,#31,#1" : "=r"(f32Return) : "r"(f32In));
  __asm volatile(" mvn %0,#0x80000000": "=r"(f32OutMax) :);
  __asm volatile(" add %0,%1" : "+r"(f32OutMax) : "r"(f32Return));
  __asm volatile(" lsl %0,%3,%4\n"
        " asr %1,%0,%4\n"
        " cmp %1,%3\n"
        " it ne\n"
        " movne %0,%2" : "+r"(f32Return), "+r"(f32Temp): "r"(f32OutMax), "r"(f32In), "r"(u16In): "cc");
  __asm volatile(" asr %0,%1,#16"  : "=r"(f16Return) : "r"(f32Return));
  __asm volatile(" sxth %0,%0" : "+r"(f16Return): );
  return(f16Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tFrac16 ShLSat_F16_IAR_A32T32(register tFrac16 f16In,register tU16 u16In)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return, f32Temp, f32OutMax,f32In;
  /*
  * @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  __asm(" lsl %0,%1,#16"  : "=r"(f32In) : "r"(f16In));
  __asm(" ubfx %0,%1,#31,#1" : "=r"(f32Return) : "r"(f32In));
  __asm(" mvn %0,#0x80000000": "=r"(f32OutMax) :);
  __asm(" add %0,%1" : "+r"(f32OutMax) : "r"(f32Return));
  __asm(" lsl %0,%2,%3\n"
        " asr %1,%0,%3": "+r"(f32Return), "=r"(f32Temp) : "r"(f32In), "r"(u16In): "cc");
  __asm(" cmp %1,%3\n"
        " it ne\n"
        " movne %0,%2" : "+r"(f32Return): "r"(f32Temp), "r"(f32OutMax), "r"(f32In): "cc");
  __asm(" asr %0,%1,#16"  : "=r"(f16Return) : "r"(f32Return));
  __asm(" sxth %0,%0" : "+r"(f16Return): ); 
  return(f16Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ShLSat

@param[in]         f32In1     First value to be left shift.

@param[in]         u16In2     The shift amount value.

@return       32-bit fractional value shifted to left by the shift amount.
              The bits beyond the 32-bit boundary are discarded.

*/
/**
@note         The shift amount cannot exceed in magnitude the bit-width of the
              shift value, that means must be within the range 0...31. Otherwise
              the result of the function is undefined.
              Due to effectivity reason this function is implemented as inline assembly,
              and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In1;
tFrac32 f32Out;
tU16 u16In2;

void main(void)
{
    // first input = 0.25
    f32In1 = FRAC32(0.25);
    // second input = 1
    u16In2 = (tU16)1;

    // output should be 0x40000000 ~ FRAC32(0.5)
    f32Out = MLIB_ShLSat_F32(f32In1, u16In2);

    // output should be 0x40000000 ~ FRAC32(0.5)
    f32Out = MLIB_ShLSat(f32In1, u16In2, F32);

    // ##############################################################
    // Available only if 32-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 0x40000000 ~ FRAC32(0.5)
    f32Out = MLIB_ShLSat(f32In1, u16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00175, DMLIB00169, DMLIB00173, DMLIB00178 */
INLINE tFrac32 MLIB_ShLSat_F32(register tFrac32 f32In1,register tU16 u16In2)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00171, DMLIB00172 */
    return(ShLSat_F32_GCC_A32T32(f32In1,u16In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00171, DMLIB00172 */
    return(ShLSat_F32_IAR_A32T32(f32In1,u16In2));
  #else
    /* @remarks Implements DMLIB00171, DMLIB00172 */
    return(ShLSat_F32_C(f32In1,u16In2));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ShLSat

@param[in]         f16In1     First value to be left shift.

@param[in]         u16In2     The shift amount value.

@return       16-bit fractional value shifted to left by the shift amount.
              The bits beyond the 16-bit boundary are discarded.

*/
/**
@note         The shift amount cannot exceed in magnitude the bit-width of the
              shift value, that means must be within the range 0...15. Otherwise
              the result of the function is undefined.
              Due to effectivity reason this function is implemented as inline assembly,
              and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In1;
tFrac16 f16Out;
tU16 u16In2;

void main(void)
{
    // first input = 0.25
    f16In1 = FRAC16(0.25);
    // second input = 1
    u16In2 = (tU16)1;

    // output should be 0x4000 ~ FRAC16(0.5)
    f16Out = MLIB_ShLSat_F16(f16In1, u16In2);

    // output should be 0x4000 ~ FRAC16(0.5)
    f16Out = MLIB_ShLSat(f16In1, u16In2, F16);

    // ##############################################################
    // Available only if 16-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 0x4000 ~ FRAC16(0.5)
    f16Out = MLIB_ShLSat(f16In1, u16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00175, DMLIB00169, DMLIB00173, DMLIB00179 */
INLINE tFrac16 MLIB_ShLSat_F16(register tFrac16 f16In1,register tU16 u16In2)
/*
* @violates @ref MLIB_ShLSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00170, DMLIB00172 */
    return(ShLSat_F16_GCC_A32T32(f16In1,u16In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00170, DMLIB00172 */
    return(ShLSat_F16_IAR_A32T32(f16In1,u16In2));
  #else
    /* @remarks Implements DMLIB00170, DMLIB00172 */
    return(ShLSat_F16_C(f16In1,u16In2));
  #endif
}

#ifdef __cplusplus
}
#endif

#endif /* MLIB_SHLSAT_H */
