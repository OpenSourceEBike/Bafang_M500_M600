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
* @file       MLIB_DivSat.h
*
* @version    1.0.25.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_DivSat function
*
******************************************************************************/
#ifndef MLIB_DIVSAT_H
#define MLIB_DIVSAT_H
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
* @section MLIB_DivSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_DivSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_DivSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_DivSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_DivSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_DivSat_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
* 
* @section MLIB_DivSat_h_REF_7
* Violates MISRA 2004 Required Rule 1.2, Both sides have side effects.
* This violation originates within headers provided by the compiler.
*
* @section MLIB_DivSat_h_REF_8
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*
* @section MLIB_DivSat_h_REF_9
* Violates MISRA 2004 Required Rule 10.2, Significant prototype coercion.
* This violation originates within a header provided by the compiler.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_Div.h"
  #include "MLIB_AbsSat.h"
  #include "MLIB_ShR.h"
  #include "MLIB_ShL.h"
  #include "MLIB_ShLSat.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_Div.h"
  #include "CCOV_MLIB_AbsSat.h"
  #include "CCOV_MLIB_ShR.h"
  #include "CCOV_MLIB_ShL.h"
  #include "CCOV_MLIB_ShLSat.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_DivSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_DivSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_DivSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_DivSat(...)     macro_dispatcher(MLIB_DivSat, __VA_ARGS__)(__VA_ARGS__)     /* This function divides the first parameter by the second one as saturate. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_DivSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_DivSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_DivSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00134 */
    #define MLIB_DivSat_Dsptchr_2(In1,In2)     MLIB_DivSat_Dsptchr_3(In1,In2,F32)     /* Function dispatcher for MLIB_DivSat_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_DivSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_DivSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_DivSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00134 */
    #define MLIB_DivSat_Dsptchr_2(In1,In2)     MLIB_DivSat_Dsptchr_3(In1,In2,F16)     /* Function dispatcher for MLIB_DivSat_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref MLIB_DivSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_DivSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_DivSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_DivSat_Dsptchr_3(In1,In2,Impl)    MLIB_DivSat_Dsptchr_(In1,In2,Impl)     /* Function dispatcher for MLIB_DivSat_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_DivSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_DivSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_DivSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref MLIB_DivSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00136 */
  #define MLIB_DivSat_Dsptchr_(In1,In2,Impl)     MLIB_DivSat_##Impl(In1,In2)            /* Function dispatcher for MLIB_DivSat_Dsptchr_, do not modify!!! */
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

/* MLIB_DivSat_F32 implementation variant - C */
INLINE tFrac32 DivSat_F32_C(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tU32 u32X, u32D, u32R, u32Temp, u32i;
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return, f32Neg;
  
  /*
  * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
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
  INLINE tFrac32 DivSat_F32_S32DSARM_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if ((u32D == 0u) || (u32X >= u32D))
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
      #ifdef _lint    /* workaround for incorrect parsing of assembly statement by PClint */
      if (0u == u32D)
      {
        u32D = 1u;
      }
      #endif
      u32R = (u32X / u32D);
      u32R = (u32R > 0xFFFFu) ? 0x7FFFFFFFu : (u32R << 15u);
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#elif defined(__ghs__)
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
  INLINE tFrac32 DivSat_F32_GHS_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if ((u32D == 0u) || (u32X >= u32D))
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
      #ifdef _lint    /* workaround for incorrect parsing of assembly statement by PClint */
      if (0u == u32D)
      {
        u32D = 1u;
      }
      #endif
      u32R = (u32X / u32D);
      u32R = (u32R > 0xFFFFu) ? 0x7FFFFFFFu : (u32R << 15u);
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#elif defined(__IAR_SYSTEMS_ICC__)
  INLINE tFrac32 DivSat_F32_IAR_A32T32(register tFrac32 f32In1,register tFrac32 f32In2)
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
  * file.
  */
  {
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tU32 u32X, u32D, u32R, u32i, u32Temp;
    /*
    * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFrac32 f32Return, f32Neg;
    
    /*
    * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to
    * signed underlying type (>>,^).
    */
    f32Neg = (tFrac32)(f32In1 ^ f32In2) >> 31;
    u32Temp = 0u - (tU32)f32In1;
    u32X = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In1;
    u32Temp = 0u - (tU32)f32In2;
    u32D = ((tFrac32)u32Temp > (tFrac32)0) ? u32Temp : (tU32)f32In2;
    if ((u32D == 0u) || (u32X >= u32D))
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
      #ifdef _lint    /* workaround for incorrect parsing of assembly statement by PClint */
      if (0u == u32D)
      {
        u32D = 1u;
      }
      #endif
      u32R = (u32X / u32D);
      u32R = (u32R > 0xFFFFu) ? 0x7FFFFFFFu : (u32R << 15u);
    }
    u32R = u32R ^ (tU32)f32Neg;
    f32Return = (tFrac32)u32R;
    
    return(f32Return);
  }
#endif
/* MLIB_DivSat_F16 implementation variant - C */
INLINE tFrac16 DivSat_F16_C(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac16 f16Return;

  if (f16In2 == (tFrac16)0){
    f16Return = (f16In1 > (tFrac16)0) ? INT16TOF16(INT16_MAX) : INT16TOF16(INT16_MIN);
  }else{
    /*
    * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
    * signed underlying type (^). 
    * @violates @ref MLIB_DivSat_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
    * signed underlying type (~). 
    * @violates @ref MLIB_DivSat_h_REF_7 MISRA 2004 Required Rule 1.2, Both sides have side effects.
    */
    f16Return = (MLIB_AbsSat_F16(f16In1) >= MLIB_AbsSat_F16(f16In2)) ? ((((INT16_MIN & f16In1)^(INT16_MIN & f16In2)) == INT16_MIN) ? INT16TOF16(INT16_MIN) : INT16TOF16(INT16_MAX)) : MLIB_Div_F16(f16In1, f16In2);
  }
  return(f16Return);
}



/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_DivSat

@param[in]         f32In1     Numerator of division.

@param[in]         f32In2     Denominator of division.

@return       Division of two input values, saturated if necessary.

@details      The input values as well as output value is considered as 32-bit fractional data 
              type.

              \par
              
              The output of the function is defined by the following simple equation:
              \anchor eq1_DivSat_F32
              \image rtf divsatF32Eq1.math "MLIB_DivSat_F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
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
  // input value 2 = 0.5
  f32In2 = FRAC32(0.5);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_DivSat_F32(f32In1, f32In2);

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_DivSat(f32In1, f32In2, F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC32(0.5) = 0x40000000
  f32Out = MLIB_DivSat(f32In1, f32In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00135, DMLIB00123, DMLIB00131, DMLIB00133, DMLIB00138 */
INLINE tFrac32 MLIB_DivSat_F32(register tFrac32 f32In1,register tFrac32 f32In2)
/*
* @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if defined(__GNUC__) && defined(__ARM_EABI__)
    /* @remarks Implements DMLIB00124, DMLIB00127, DMLIB00132 */
    return(DivSat_F32_S32DSARM_A32T32(f32In1,f32In2));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00124, DMLIB00127, DMLIB00132 */
    return(DivSat_F32_GHS_A32T32(f32In1,f32In2));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00124, DMLIB00127, DMLIB00132 */
    return(DivSat_F32_IAR_A32T32(f32In1,f32In2));
  #else
    /* @remarks Implements DMLIB00124, DMLIB00127, DMLIB00132 */
    return(DivSat_F32_C(f32In1,f32In2));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_DivSat

@param[in]         f16In1     Numerator of division.

@param[in]         f16In2     Denominator of division.

@return       Division of two input values, saturated if necessary.

@details      The input values as well as output value is considered as 16-bit fractional data 
              type.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_DivSat_F16
              \image rtf divsatF16Eq1.math "MLIB_DivSat_F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
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
  // input value 2 = 0.5
  f16In2 = FRAC16(0.5);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_DivSat_F16(f16In1, f16In2);

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_DivSat(f16In1, f16In2, F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be FRAC16(0.5) = 0x4000
  f16Out = MLIB_DivSat(f16In1, f16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00135, DMLIB00123, DMLIB00130, DMLIB00133, DMLIB00139 */
INLINE tFrac16 MLIB_DivSat_F16(register tFrac16 f16In1,register tFrac16 f16In2)
/*
* @violates @ref MLIB_DivSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  /* @remarks Implements DMLIB00125, DMLIB00128, DMLIB00132 */
  return(DivSat_F16_C(f16In1,f16In2));
}

#ifdef __cplusplus
}
#endif

#endif /* MLIB_DIVSAT_H */
