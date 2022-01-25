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
* @file       MLIB_Norm.h
*
* @version    1.0.78.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_Norm function
*
******************************************************************************/
#ifndef MLIB_NORM_H
#define MLIB_NORM_H
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
* @section MLIB_Norm_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_Norm_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_Norm_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_Norm_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_Norm_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_Norm_h_REF_6
* Violates MISRA 2004 Required Rule 14.7, Return statement before end of function. 
* There is no required code which needs to be executed after the calculation in the branch, thus the 
* performance of the function will be negatively influenced in case the single return point is used. 
* 
* @section MLIB_Norm_h_REF_7
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
*
* @section MLIB_Norm_h_REF_8
* Violates MISRA 2004 Required Rule 10.1, Prohibited Implicit Conversion.
* This violation originates within a header provided by the compiler.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_ShR.h"
  #include "MLIB_ShL.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_ShR.h"
  #include "CCOV_MLIB_ShL.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_Norm_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Norm_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Norm_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Norm(...)     macro_dispatcher(MLIB_Norm, __VA_ARGS__)(__VA_ARGS__)     /* This function returns the number of left shifts needed to normalize the input parameter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_Norm_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Norm_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Norm_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00195 */
    #define MLIB_Norm_Dsptchr_1(In)     MLIB_Norm_Dsptchr_2(In,F32)     /* Function dispatcher for MLIB_Norm_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_Norm_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_Norm_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_Norm_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00195 */
    #define MLIB_Norm_Dsptchr_1(In)     MLIB_Norm_Dsptchr_2(In,F16)     /* Function dispatcher for MLIB_Norm_Dsptchr_1, do not modify!!! */
  #endif 

  /*
  * @violates @ref MLIB_Norm_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Norm_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Norm_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_Norm_Dsptchr_2(In,Impl)    MLIB_Norm_Dsptchr_(In,Impl)   /* Function dispatcher for MLIB_Norm_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref MLIB_Norm_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_Norm_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_Norm_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00197 */
  #define MLIB_Norm_Dsptchr_(In,Impl)     MLIB_Norm_##Impl(In)          /* Function dispatcher for MLIB_Norm_Dsptchr_, do not modify!!! */
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
/* MLIB_Norm_F32 implementation variant - C */
INLINE tU16 Norm_F32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tU16 u16ShiftCount = (tU16)0;

  if(f32In == (tFrac32)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f32In > (tFrac32)0){
    if (f32In < (tFrac32)0x00FF0000u){
      do{
        f32In = MLIB_ShR_F32(f32In,(tU16)1);
        u16ShiftCount += (tU16)1;
      } while(f32In > (tFrac32)0);
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)31-u16ShiftCount);
    }else{
      /*
      * @violates @ref MLIB_Norm_h_REF_7 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
      * signed underlying type. 
      */
      while(((f32In & 0x40000000)==0)){
        f32In = MLIB_ShL_F32(f32In,(tU16)1);
        u16ShiftCount += (tU16)1;
      }
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)u16ShiftCount);
    }  
  }else{
    if (f32In > (tFrac32)0xFF000000u){
      if(f32In == (tFrac32)-1){
        /*
        * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
        * function.
        */
        return((tU16)31);    
      }
      do{
        f32In = MLIB_ShR_F32(f32In,(tU16)1);
        u16ShiftCount += (tU16)1;
      } while(f32In != (tFrac32)-1);
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)31-u16ShiftCount);
    }else{
      /*
      * @violates @ref MLIB_Norm_h_REF_7 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
      * signed underlying type. 
      */
      while((f32In | (tFrac32)0xBFFFFFFFu) != (tFrac32)0xBFFFFFFFu){
        f32In = MLIB_ShL_F32(f32In,(tU16)1);
        u16ShiftCount += (tU16)1;
      }
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)u16ShiftCount);
    }  
  }      
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tU16 Norm_F32_GCC_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;

  if(f32In == (tFrac32)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f32In == (tFrac32)-1){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)31);
  }
  __asm volatile(" movs %0,%1" : "=r"(f32Return): "r"(f32In));
  __asm volatile(" itt mi\n"
        " rsbmi %0,%0,#0\n"
        " submi %0,%0,#1"  : "+r"(f32Return): );
  __asm volatile(" lsl %0,%0,#1"  : "+r"(f32Return): );
  __asm volatile(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tU16 Norm_F32_IAR_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;

  if(f32In == (tFrac32)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f32In == (tFrac32)-1){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)31);
  }
  __asm(" movs %0,%1" : "=r"(f32Return): "r"(f32In));
  __asm(" itt mi\n"
        " rsbmi %0,%0,#0\n"
        " submi %0,%0,#1"  : "+r"(f32Return): );
  __asm(" lsl %0,%0,#1"  : "+r"(f32Return): );
  __asm(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tU16 Norm_F32_GHS_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;

  if(f32In == (tFrac32)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f32In == (tFrac32)-1){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)31);
  }
  asm(" movs %0,%1" : "=r"(f32Return): "r"(f32In));
  asm(" itt mi\n"
      " rsbmi %0,%0,#0\n"
      " submi %0,%0,#1"  : "+r"(f32Return): );
  asm(" lsl %0,%0,#1"  : "+r"(f32Return): );
  asm(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */
/* MLIB_Norm_F16 implementation variant - C */
INLINE tU16 Norm_F16_C(register tFrac16 f16In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tU16 u16Return = (tU16)0;

  if(f16In == (tFrac16)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f16In > (tFrac16)0){
    if (f16In < (tFrac16)0x00FF){
      do{
        f16In = MLIB_ShR_F16(f16In,(tU16)1);
        u16Return += (tU16)1;
      } while(f16In > (tFrac16)0);
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)15-u16Return);
    }else{
      /*
      * @violates @ref MLIB_Norm_h_REF_7 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
      * signed underlying type. 
      */
      while(((f16In & 0x4000)==0)){
        f16In = MLIB_ShL_F16(f16In,(tU16)1);
        u16Return += (tU16)1;
      }
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)u16Return);
    }  
  }else{
    if (f16In > (tFrac16)0xFF00){
      if(f16In == (tFrac16)0xFFFF){
        /*
        * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
        * function.
        */
        return((tU16)15);    
      }  
      do{
        f16In = MLIB_ShR_F16(f16In,(tU16)1);
        u16Return += (tU16)1;
      } while(f16In != (tFrac16)0xFFFF);
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)15-u16Return);
    }else{
      /*
      * @violates @ref MLIB_Norm_h_REF_7 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
      * signed underlying type. 
      */
      while((f16In | (tFrac16)0xBFFF) != (tFrac16)0xBFFF){
        f16In = MLIB_ShL_F16(f16In,(tU16)1);
        u16Return += (tU16)1;
      }
      /*
      * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
      * function.
      */
      return((tU16)u16Return);
    }  
  }
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tU16 Norm_F16_GCC_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;

  if(f16In == (tFrac16)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f16In == (tFrac16)0xFFFF){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)15);
  }  
  __asm volatile(" movs %0,%1,lsl #16" : "=r"(f32Return): "r"(f16In));
  __asm volatile(" itt mi\n"
        " rsbmi %0,%0,#0\n"
        " submi %0,%0,#1" : "+r"(f32Return): );
  __asm volatile(" lsl %0,%0,#1" : "+r"(f32Return): );
  __asm volatile(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
INLINE tU16 Norm_F16_IAR_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */    
  register tFrac32 f32Return;

  if(f16In == (tFrac16)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f16In == (tFrac16)0xFFFF){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)15);
  }  
  __asm(" movs %0,%1,lsl #16" : "=r"(f32Return): "r"(f16In));
  __asm(" itt mi\n"
        " rsbmi %0,%0,#0\n"
        " submi %0,%0,#1" : "+r"(f32Return): );
  __asm(" lsl %0,%0,#1" : "+r"(f32Return): );
  __asm(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tU16 Norm_F16_GHS_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;

  if(f16In == (tFrac16)0){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)0);
  }
  if(f16In == (tFrac16)0xFFFF){
    /*
    * @violates @ref MLIB_Norm_h_REF_6 MISRA 2004 Required Rule 14.7, Return statement before end of
    * function.
    */
    return((tU16)15);
  }  
  asm(" movs %0,%1,lsl #16" : "=r"(f32Return): "r"(f16In));
  asm(" itt mi\n"
      " rsbmi %0,%0,#0\n"
      " submi %0,%0,#1" : "+r"(f32Return):);
  asm(" lsl %0,%0,#1" : "+r"(f32Return): );
  asm(" clz %0,%0" : "+r"(f32Return): );
  return((tU16)f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Norm

@param[in]         f32In     The first value to be normalized.

@return       The number of left shift needed to normalize the argument.

*/
/**
@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In;
tU16 u16Out;

void main(void)
{
    // first input = 0.00005
    f32In = FRAC32(0.00005);

    // output should be 14
    u16Out = MLIB_Norm_F32(f32In);

    // output should be 14
    u16Out = MLIB_Norm(f32In,F32);

    // ##############################################################
    // Available only if 32-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 14
    u16Out = MLIB_Norm(f32In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00196, DMLIB00189, DMLIB00192, DMLIB00194 */
INLINE tU16 MLIB_Norm_F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00193 */
    return(Norm_F32_GCC_A32T32(f32In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00193 */
    return(Norm_F32_IAR_A32T32(f32In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00193 */
    return(Norm_F32_GHS_A32T32(f32In));
  #else
    /* @remarks Implements DMLIB00193 */
    return(Norm_F32_C(f32In));
  #endif
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_Norm

@param[in]         f16In     The first value to be normalized.

@return       The number of left shift needed to normalize the argument.

*/
/**
@note         Due to effectivity reason this function is implemented as inline and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In;
tU16 u16Out;

void main(void)
{
    // first input = 0.00005
    f16In = FRAC16(0.00005);

    // output should be 14
    u16Out = MLIB_Norm_F16(f16In);

    // output should be 14
    u16Out = MLIB_Norm(f16In,F16);

    // ##############################################################
    // Available only if 16-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 14
    u16Out = MLIB_Norm(f16In);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
/* @remarks Implements DMLIB00196, DMLIB00189, DMLIB00191, DMLIB00194 */
INLINE tU16 MLIB_Norm_F16(register tFrac16 f16In)
/*
* @violates @ref MLIB_Norm_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00193 */
    return(Norm_F16_GCC_A32T32(f16In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00193 */
    return(Norm_F16_IAR_A32T32(f16In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00193 */
    return(Norm_F16_GHS_A32T32(f16In));
  #else
    /* @remarks Implements DMLIB00193 */
    return(Norm_F16_C(f16In));
  #endif
}

#ifdef __cplusplus
}
#endif

#endif /* MLIB_NORM_H */
