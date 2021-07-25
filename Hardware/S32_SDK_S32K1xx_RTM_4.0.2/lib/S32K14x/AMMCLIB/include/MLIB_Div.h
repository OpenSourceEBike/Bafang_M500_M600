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
* @file       MLIB_Div.h
*
* @version    1.0.84.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_Div function
*
******************************************************************************/
#ifndef MLIB_DIV_H
#define MLIB_DIV_H
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
* @section MLIB_Div_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application,the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_Div_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application,the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_Div_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application,the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_Div_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application,the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_Div_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_Div_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
*
* @section MLIB_Div_h_REF_7
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*
* @section MLIB_Div_h_REF_8
* Violates MISRA 2004 Required Rule 10.2, Significant prototype coercion.
* This violation originates within a header provided by the compiler.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_ShL.h"
  #include "MLIB_ShLSat.h"
  #include "MLIB_ShR.h"
  #include "MLIB_AbsSat.h"
  #include "MLIB_Norm.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_ShL.h"
  #include "CCOV_MLIB_ShLSat.h"
  #include "CCOV_MLIB_ShR.h"
  #include "CCOV_MLIB_AbsSat.h"
  #include "CCOV_MLIB_Norm.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Div(...)     macro_dispatcher(MLIB_Div,__VA_ARGS__)(__VA_ARGS__)     /* This function divides the first parameter by the second one. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00119 */
    #define MLIB_Div_Dsptchr_2(In1,In2)     MLIB_Div_Dsptchr_3(In1,In2,F32)       /* Function dispatcher for MLIB_Div_Dsptchr_2,do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00119 */
    #define MLIB_Div_Dsptchr_2(In1,In2)     MLIB_Div_Dsptchr_3(In1,In2,F16)       /* Function dispatcher for MLIB_Div_Dsptchr_2,do not modify!!! */
  #endif
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00119 */
    #define MLIB_Div_Dsptchr_2(In1,In2)     MLIB_Div_Dsptchr_3(In1,In2,FLT)       /* Function dispatcher for MLIB_Div_Dsptchr_2,do not modify!!! */
  #endif
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */

  /*
  * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Div_Dsptchr_3(In1,In2,Impl)    MLIB_Div_Dsptchr_(In1,In2,Impl)     /* Function dispatcher for MLIB_Div_Dsptchr_3,do not modify!!! */
  
  /*
  * @violates @ref MLIB_Div_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Div_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Div_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref MLIB_Div_h_REF_4 MISRA 2004 Advisory Rule 19.13,#/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00121 */
  #define MLIB_Div_Dsptchr_(In1,In2,Impl)     MLIB_Div_##Impl(In1,In2)            /* Function dispatcher for MLIB_Div_Dsptchr_,do not modify!!! */
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
INLINE tFrac32 Div_F32_C(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tU32 u32X, u32D, u32R, u32Temp, u32i;
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return, f32Neg;
  
  /*
  * @violates @ref MLIB_Div_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
  * signed underlying type (>>,^).
  */
  f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
  u32Temp = 0u - (tU32)f32In1;
  u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
  u32Temp = 0u - (tU32)f32In2;
  u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
  if(u32X >= u32D)
  {
    u32D = 0u;
  }
  if(0u == u32D)
  {
    u32R = 0x7FFFFFFFu;
  }
  else
  {
    u32Temp = u32D;
    if (u32Temp < 0x00FF0000u)
    {
      u32i = 32u;
      do
      {
        u32Temp = u32Temp >> 1u;
        u32i--;
      } while(u32Temp > 0u);
    }
    else
    {
      u32i = 0u;
      while(((u32Temp & 0x80000000u)==0u))
      {
        u32Temp = u32Temp << 1u;
        u32i++;
      }
    }
    u32X =  u32X << u32i;
    if (u32i>16u)
    {
      u32D = u32D << (u32i - 16u);
    }
    else
    {
      u32D = u32D >> (16u - u32i);
    }    
    u32R = (u32X / u32D);
    u32R = (u32R > 0xFFFFu) ? 0x7FFFFFFFu : (u32R << 15u);
  }
  u32R = u32R ^ (tU32)f32Neg;
  f32Return = (tFrac32)u32R;
  
  return(f32Return);
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
  INLINE tFrac32 Div_F32_S32DSARM_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_Div_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if (u32D == 0u)
    {
      u32R = 0x7FFFFFFFu;
    }
    else
    {
      __asm volatile(" clz %0,%1" : "=r"(u32i): "r"(u32D));
      #ifdef _lint  /* Assignment of some value for PClint */
        u32i = (tU32)f32In1;
      #endif
      u32X =  u32X << u32i;
      if (u32i>16u)
      {
        u32D = u32D << (u32i - 16u);
      }
      else
      {
        u32D = u32D >> (16u - u32i);
      }    
      u32R = (u32X / u32D) << 15u;
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#elif defined(__ghs__)
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
  INLINE tFrac32 Div_F32_GHS_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_Div_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if (u32D == 0u)
    {
      u32R = 0x7FFFFFFFu;
    }
    else
    {
      #pragma ghs optasm
      asm(" clz %0,%1" : "=r"(u32i): "r"(u32D));
      #ifdef _lint  /* Assignment of some value for PClint */
        u32i = (tU32)f32In1;
      #endif
      u32X =  u32X << u32i;
      if (u32i>16u)
      {
        u32D = u32D << (u32i - 16u);
      }
      else
      {
        u32D = u32D >> (16u - u32i);
      }    
      u32R = (u32X / u32D) << 15u;
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#elif defined(__IAR_SYSTEMS_ICC__)
  INLINE tFrac32 Div_F32_IAR_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_Div_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if (u32D == 0u)
    {
      u32R = 0x7FFFFFFFu;
    }
    else
    {
      __asm(" clz %0,%1" : "=r"(u32i): "r"(u32D));
      #ifdef _lint  /* Assignment of some value for PClint */
        u32i = (tU32)f32In1;
      #endif
      u32X =  u32X << u32i;
      if (u32i>16u)
      {
        u32D = u32D << (u32i - 16u);
      }
      else
      {
        u32D = u32D >> (16u - u32i);
      }    
      u32R = (u32X / u32D) << 15u;
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#endif
INLINE tFrac16 Div_F16_C(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac16 f16Return;

  if (f16In2 == (tFrac16)0)
  {
    f16Return = (f16In1 > (tFrac16)0) ? INT16TOF16(INT16_MAX) : INT16TOF16(INT16_MIN);
  }
  else
  {
    /*
    * @violates @ref MLIB_Div_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (<<).
    */
    f16Return = INT16TOF16(INT32TOINT16((F16TOINT32(f16In1)<<(tU16)15)/F16TOINT32(f16In2)));
  }
  return(f16Return);
}





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Div

@param[in]         f32In1     Numerator of division.

@param[in]         f32In2     Denominator of division.

@return       Division of two input values.

@details      The input values as well as output value is considered as 32-bit fractional data type.
              The output saturation is not implemented in this function, thus in case the
              numerator is greater or equal to denominator, the output value is undefined. The
              function will never cause division by zero exception.
              \par              
              The output of the function is defined by the following simple equation:
              \anchor eq1_Div_F32
              \image rtf divF32Eq1.math "MLIB_Div_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@warning      Due to effectivity reason the division is calculated in 16-bit precision.

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In1,f32In2;
tFrac32 f32Out;

void main(void)
{
  // input value 1 = 0.25
  f32In1 = FRAC32(0.25);
  // input value 2 = 0.5
  f32In2 = FRAC32(0.5);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_Div_F32(f32In1,f32In2);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_Div(f32In1,f32In2,F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_Div(f32In1,f32In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00120, DMLIB00107, DMLIB00116, DMLIB00118, DMLIB00108, DMLIB00111 */
INLINE tFrac32 MLIB_Div_F32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  #if defined(__GNUC__) && defined(__ARM_EABI__)
    /* @remarks Implements DMLIB00117 */
    return(Div_F32_S32DSARM_A32T32(f32In1,f32In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00117 */
    return(Div_F32_GHS_A32T32(f32In1,f32In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00117 */
    return(Div_F32_IAR_A32T32(f32In1,f32In2));
  #else
    /* @remarks Implements DMLIB00117 */
    return(Div_F32_C(f32In1,f32In2));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Div

@param[in]         f16In1     Numerator of division.

@param[in]         f16In2     Denominator of division.

@return       Division of two input values.

@details      The input values as well as output value is considered as 16-bit fractional data type.
              The output saturation is not implemented in this function, thus in case the numerator
              is greater or equal to denominator, the output value is undefined. The function will
              never cause division by zero exception.
              \par              
              The output of the function is defined by the following simple equation:
              \anchor eq1_Div_F16
              \image rtf divF16Eq1.math "MLIB_Div_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In1,f16In2;
tFrac16 f16Out;

void main(void)
{
  // input value 1 = 0.25
  f16In1 = FRAC16(0.25);
  // input value 2 = 0.5
  f16In2 = FRAC16(0.5);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_Div_F16(f16In1,f16In2);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_Div(f16In1,f16In2,F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_Div(f16In1,f16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00120, DMLIB00107, DMLIB00115, DMLIB00118, DMLIB00109, DMLIB00112 */
INLINE tFrac16 MLIB_Div_F16(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00117 */
  return(Div_F16_C(f16In1,f16In2));
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Div

@param[in]         fltIn1     Numerator of division.

@param[in]         fltIn2     Denominator of division.

@return       Division of two input values.

@details      The input values as well as output value is considered as single  precision floating point data type. Testing of input values for division by zero is not implemented in this function.
              \par
              The output of the function is defined by the following simple equation:
              \anchor eq1_Div_FLT
              \image rtf divFLTEq1.math "MLIB_Div_FLT_Eq1"

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,division by zero,
              overflow,underflow,inexact,input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.

@par Code Example
\code
#include "mlib.h"

tFloat fltIn1,fltIn2;
tFloat fltOut;

void main(void)
{
  // input value 1 = 0.25
  fltIn1 = (tFloat)0.25;
  // input value 2 = 0.5
  fltIn2 = (tFloat)0.5;

  // output should be 0.5
  fltOut = MLIB_Div_FLT(fltIn1,fltIn2);

  // output should be 0.5
  fltOut = MLIB_Div(fltIn1,fltIn2,FLT);

  // ##############################################################
  // Available only if single precision floating point
  // implementation selected as default
  // ##############################################################

  // output should be 0.5
  fltOut = MLIB_Div(fltIn1,fltIn2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00120, DMLIB00107, DMLIB00114, DMLIB00118, DMLIB00110, DMLIB00113 */
INLINE tFloat MLIB_Div_FLT(register tFloat fltIn1,register tFloat fltIn2)
/*
* @violates @ref MLIB_Div_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00117 */
  return((tFloat)(fltIn1 / fltIn2));
}


#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* MLIB_DIV_H */
