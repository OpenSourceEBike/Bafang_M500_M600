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
* @file       MLIB_ConvertPU.h
*
* @version    1.0.54.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_ConvertPU function
*
******************************************************************************/
#ifndef MLIB_CONVERTPU_H
#define MLIB_CONVERTPU_H
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
* @section MLIB_ConvertPU_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_ConvertPU_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_ConvertPU_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_ConvertPU_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_ConvertPU_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_ConvertPU_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
* 
* @section MLIB_ConvertPU_h_REF_7
* Violates MISRA 2004 Required Rule 10.4, Cast of complex expression from floating point to integer 
* type. 
* The casting of the expression required for data type conversion. 
*/

#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_ShL.h"
  #include "MLIB_Mul.h"
  #include "MLIB_ShR.h"
  #include "MLIB_Div.h"
#else
  /* Following includes serve for NXP internal testing purposes only. 
  *  These headers are not part of the release. */
  #include "CCOV_MLIB_ShL.h"
  #include "CCOV_MLIB_Mul.h"
  #include "CCOV_MLIB_ShR.h"
  #include "CCOV_MLIB_Div.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ConvertPU_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref MLIB_ConvertPU_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_ConvertPU(...)     macro_dispatcher(MLIB_ConvertPU, __VA_ARGS__)(__VA_ARGS__)     /* This function converts the input value to different representation with scale. */

  /*
  * @violates @ref MLIB_ConvertPU_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ConvertPU_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref MLIB_ConvertPU_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define MLIB_ConvertPU_Dsptchr_2(In,Impl)    MLIB_ConvertPU_Dsptchr_(In,Impl)     /* Function dispatcher for MLIB_ConvertPU_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref MLIB_ConvertPU_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref MLIB_ConvertPU_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DMLIB00252*/
  #define MLIB_ConvertPU_Dsptchr_(In,Impl)     MLIB_ConvertPU_##Impl(In)            /* Function dispatcher for MLIB_ConvertPU_Dsptchr_, do not modify!!! */
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
INLINE tFrac32 ConvertPU_F32F16_C(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac32)((tFrac32)f16In << ((tU16)16)));
}
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
INLINE tFrac32 ConvertPU_F32FLT_C(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_7 MISRA 2004 Required Rule 10.4, Cast of complex expression 
  * from floating point to integer type. 
  */
  return((fltIn>=(tFloat)1) ? (tFrac32)INT32_MAX : ((fltIn<(tFloat)-1) ? (tFrac32)INT32_MIN : ((tFrac32)(fltIn * (tFloat)2147483648.0))));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac32 ConvertPU_F32FLT_GCC_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm volatile(" vmov.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
  __asm volatile(" vcvt.s32.f32 %0,%0,#31" : "+t"(fltReturn):);
  __asm volatile(" vmov %0,%1" : "=r"(f32Return): "t"(fltReturn));
  return((tFrac32)f32Return);
}
#elif defined(__IAR_SYSTEMS_ICC__)
/* [MCL-382] workaround for IAR bug */
#pragma optimize=low    
INLINE tFrac32 ConvertPU_F32FLT_IAR_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm(" vmov.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
  __asm(" vcvt.s32.f32 %0,%0,#31" : "+t"(fltReturn):);
  __asm(" vmov %0,%1" : "=r"(f32Return): "t"(fltReturn));
  return((tFrac32)f32Return);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac32 ConvertPU_F32FLT_GHS_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  asm(" vmov.f32 %0,%1" : "=f"(fltReturn): "f"(fltIn));
  asm(" vcvt.s32.f32 %0,%0,#31" : "+f"(fltReturn):);
  asm(" vmov %0,%1" : "=r"(f32Return): "f"(fltReturn));
  return((tFrac32)f32Return);
}
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
INLINE tFrac16 ConvertPU_F16F32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac16)(f32In >> ((tU16)16)));
}
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
INLINE tFrac16 ConvertPU_F16FLT_C(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_7 MISRA 2004 Required Rule 10.4, Cast of complex expression 
  * from floating point to integer type. 
  */
  return((fltIn>=(tFloat)1) ? (tFrac16)INT16_MAX : ((fltIn<(tFloat)-1) ? (tFrac16)INT16_MIN : ((tFrac16)(fltIn * (((tFloat)INT16_MAX)+(tFloat)1)))));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFrac16 ConvertPU_F16FLT_GCC_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm volatile(" vmov.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
  __asm volatile(" vcvt.s32.f32 %0,%0,#31" : "+t"(fltReturn):);
  __asm volatile(" vmov %0,%1" : "=r"(f32Return): "t"(fltReturn));
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac16)(f32Return>>16));
}
#elif defined(__IAR_SYSTEMS_ICC__)
/* [MCL-382] workaround for IAR bug */
#pragma optimize=low    
INLINE tFrac16 ConvertPU_F16FLT_IAR_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm(" vmov.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
  __asm(" vcvt.s32.f32 %0,%0,#31" : "+t"(fltReturn):);
  __asm(" vmov %0,%1" : "=r"(f32Return): "t"(fltReturn));
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac16)(f32Return>>16));
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFrac16 ConvertPU_F16FLT_GHS_A32T32(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register tFrac32 f32Return;
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  asm(" vmov.f32 %0,%1" : "=f"(fltReturn): "f"(fltIn));
  asm(" vcvt.s32.f32 %0,%0,#31" : "+f"(fltReturn):);
  asm(" vmov %0,%1" : "=r"(f32Return): "f"(fltReturn));
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  return((tFrac16)(f32Return>>16));
}
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
INLINE tFloat ConvertPU_FLTF16_C(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  return(MLIB_Mul_FLT((tFloat)f16In,((tFloat)3.0517578e-5)));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFloat ConvertPU_FLTF16_GCC_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm volatile(" vmov %0,%1" : "=t"(fltReturn): "r"(f16In));
  __asm volatile(" vcvt.f32.s16 %0,%0,#15" : "+t"(fltReturn):);
  return(fltReturn);
}
#elif defined(__IAR_SYSTEMS_ICC__)
/* [MCL-382] workaround for IAR bug */
#pragma optimize=low    
INLINE tFloat ConvertPU_FLTF16_IAR_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm(" vmov %0,%1" : "=t"(fltReturn): "r"(f16In));
  __asm(" vcvt.f32.s16 %0,%0,#15" : "+t"(fltReturn):);
  return(fltReturn);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFloat ConvertPU_FLTF16_GHS_A32T32(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  asm(" vmov %0,%1" : "=f"(fltReturn): "r"(f16In));
  asm(" vcvt.f32.s16 %0,%0,#15" : "+f"(fltReturn):);
  return(fltReturn);
}
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
INLINE tFloat ConvertPU_FLTF32_C(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  return(MLIB_Mul_FLT((tFloat)f32In,(tFloat)4.6566129e-10));
}
#if (defined(__GNUC__) && defined(__ARM_EABI__))
INLINE tFloat ConvertPU_FLTF32_GCC_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm volatile(" vmov %0,%1" : "=t"(fltReturn): "r"(f32In));
  __asm volatile(" vcvt.f32.s32 %0,%0,#31" : "+t"(fltReturn):);
  return(fltReturn);
}
#elif defined(__IAR_SYSTEMS_ICC__)
/* [MCL-382] workaround for IAR bug */
#pragma optimize=low    
INLINE tFloat ConvertPU_FLTF32_IAR_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  __asm(" vmov %0,%1" : "=t"(fltReturn): "r"(f32In));
  __asm(" vcvt.f32.s32 %0,%0,#31" : "+t"(fltReturn):);
  return(fltReturn);
}
#elif defined(__ghs__)
#pragma ghs static_call 0
#pragma ghs extra_stack 0
INLINE tFloat ConvertPU_FLTF32_GHS_A32T32(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
  * header file. 
  */
  register tFloat fltReturn;

  asm(" vmov %0,%1" : "=f"(fltReturn): "r"(f32In));
  asm(" vcvt.f32.s32 %0,%0,#31" : "+f"(fltReturn):);
  return(fltReturn);
}
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    f16In     Input value in 16-bit fractional format to be converted.

@return       Converted input value in 32-bit fractional format.

@details      The input value is considered as 16-bit fractional data type and output value is
              considered as 32-bit fractional data type. The output saturation is not implemented
              in this function, thus in case the input value is outside the [-1, 1) interval, the
              output value will overflow.

              \par
              
              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_F32F16
              \image rtf convertpuF32F16Eq1.math "MLIB_ConvertPU_F32F16_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac16 f16In;
tFrac32 f32Out;

void main(void)
{
    // input value = 0.25 = 0x2000
    f16In = FRAC16(0.25);

    // output should be FRAC32(0.25) = 0x20000000
    f32Out = MLIB_ConvertPU_F32F16(f16In);

    // output should be FRAC32(0.25) = 0x20000000
    f32Out = MLIB_ConvertPU(f16In, F32F16);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00247, DMLIB00250 */
INLINE tFrac32 MLIB_ConvertPU_F32F16(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  /* @remarks Implements DMLIB00247 */
  return(ConvertPU_F32F16_C(f16In));
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    fltIn     Input value in single precision floating point format to be converted.

@return       Converted input value in 32-bit fractional format.

@details      The input value is considered as single precision floating point data type and output
              value is considered as 32-bit fractional data type. The output saturation is
              implemented in this function, thus in case the input value is outside the [-1, 1)
              interval, the output value is limited to the boundary value.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_F32FLT
              \image rtf convertpuF32FLTEq1.math "MLIB_ConvertPU_F32FLT_Eq1"

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,
              inexact, input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFloat fltIn;
tFrac32 f32Out;

void main(void)
{
    // input value = 0.25
    fltIn = (tFloat)0.25;

    // output should be FRAC32(0.25) = 0x20000000
    f32Out = MLIB_ConvertPU_F32FLT(fltIn);

    // output should be FRAC32(0.25) = 0x20000000
    f32Out = MLIB_ConvertPU(fltIn, F32FLT);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00248, DMLIB00250 */
INLINE tFrac32 MLIB_ConvertPU_F32FLT(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00248 */
    return(ConvertPU_F32FLT_GCC_A32T32(fltIn));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00248 */
    return(ConvertPU_F32FLT_IAR_A32T32(fltIn));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00248 */
    return(ConvertPU_F32FLT_GHS_A32T32(fltIn));
  #else
    /* @remarks Implements DMLIB00248 */
    return(ConvertPU_F32FLT_C(fltIn));
  #endif
}





#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    f32In     Input value in 32-bit fractional format to be converted.

@return       Converted input value in 16-bit fractional format.

@details      The input value is considered as 32-bit fractional data type and output value is
              considered as 16-bit fractional data type. The output saturation is not implemented in
              this function, thus in case the input value is outside the [-1, 1) interval, the
              output value will overflow without any detection.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_F16F32
              \image rtf convertpuF16F32Eq1.math "MLIB_ConvertPU_F16F32_Eq1"

*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFrac32 f32In;
tFrac16 f16Out;

void main(void)
{
    // input value = 0.25 = 0x2000 0000
    f32In = FRAC32(0.25);

    // output should be FRAC16(0.25) = 0x2000
    f16Out = MLIB_ConvertPU_F16F32(f32In);

    // output should be FRAC16(0.25) = 0x2000
    f16Out = MLIB_ConvertPU(f32In, F16F32);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00244, DMLIB00250 */
INLINE tFrac16 MLIB_ConvertPU_F16F32(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  /* @remarks Implements DMLIB00244 */
  return(ConvertPU_F16F32_C(f32In));
}





#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    fltIn     Input value in single precision floating point format to be converted.

@return       Converted input value in 16-bit fractional format.

@details      The input value is considered as single precision floating  point data type and output
              value is considered as 16-bit fractional data type. The output saturation is
              implemented in this function, thus in case the input value is outside the [-1, 1)
              interval, the output value is limited to the boundary value.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_F16FLT
              \image rtf convertpuF16FLTEq1.math "MLIB_ConvertPU_F16FLT_Eq1"

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,
              inexact, input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tFloat fltIn;
tFrac16 f16Out;

void main(void)
{
    // input value = 0.25
    fltIn = (tFloat)0.25;

    // output should be FRAC16(0.25) = 0x2000
    f16Out = MLIB_ConvertPU_F16FLT(fltIn);

    // output should be FRAC16(0.25) = 0x2000
    f16Out = MLIB_ConvertPU(fltIn, F16FLT);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00249, DMLIB00250 */
INLINE tFrac16 MLIB_ConvertPU_F16FLT(register tFloat fltIn)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00249 */
    return(ConvertPU_F16FLT_GCC_A32T32(fltIn));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00249 */
    return(ConvertPU_F16FLT_IAR_A32T32(fltIn));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00249 */
    return(ConvertPU_F16FLT_GHS_A32T32(fltIn));
  #else
    /* @remarks Implements DMLIB00249 */
    return(ConvertPU_F16FLT_C(fltIn));
  #endif
}





#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    f16In     Input value in 16-bit fractional format to be converted.

@return       Converted input value in single precision floating point format.

@details      The input value is considered as 16-bit fractional data type and output value is
              considered as single precision floating point data type. The output saturation is not
              implemented in this function.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_FLTF16
              \image rtf convertpuFLTF16Eq1.math "MLIB_ConvertPU_FLTF16_Eq1"

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,
              inexact, input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tF16 f16In;
tFloat fltOut;

void main(void)
{
    // input value = 0.25 = 0x2000
    f16In = FRAC16(0.25);

    // output should be 0.25
    fltOut = MLIB_ConvertPU_FLTF16(f16In);

    // output should be 0.25
    fltOut = MLIB_ConvertPU(f16In, FLTF16);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00246, DMLIB00250 */
INLINE tFloat MLIB_ConvertPU_FLTF16(register tFrac16 f16In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00246 */
    return(ConvertPU_FLTF16_GCC_A32T32(f16In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00246 */
    return(ConvertPU_FLTF16_IAR_A32T32(f16In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00246 */
    return(ConvertPU_FLTF16_GHS_A32T32(f16In));
  #else
    /* @remarks Implements DMLIB00246 */
    return(ConvertPU_FLTF16_C(f16In));
  #endif
}





#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ConvertPU

@param[in]    f32In     Input value in 32-bit fractional format to be converted.

@return       Converted input value in single precision floating point format.

@details      The input value is considered as 32-bit fractional data type and output value is
              considered as single precision floating point data type. The output saturation is not
              implemented in this function.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_ConvertPU_FLTF32
              \image rtf convertpuFLTF32Eq1.math "MLIB_ConvertPU_FLTF32_Eq1"

*/
/**
@note         The function may raise floating-point exceptions (invalid operation,
              inexact, input denormal). The floating-point unit must be enabled
              in CPACR register to prevent NOCP UsageFault exception.
*/
/**
@note         Due to effectivity reason this function is implemented as inline, and thus is not ANSI-C compliant.
*/
/**

@par Code Example
\code
#include "mlib.h"

tF32 f32In;
tFloat fltOut;

void main(void)
{
    // input value = 0.25 = 0x20000000
    f32In = FRAC32(0.25);

    // output should be 0.25
    fltOut = MLIB_ConvertPU_FLTF32(f32In);

    // output should be 0.25
    fltOut = MLIB_ConvertPU(f32In, FLTF32);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00251, DMLIB00245, DMLIB00250 */
INLINE tFloat MLIB_ConvertPU_FLTF32(register tFrac32 f32In)
/*
* @violates @ref MLIB_ConvertPU_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in 
* header file. 
*/
{
  #if (defined(__GNUC__) && defined(__ARM_EABI__))
    /* @remarks Implements DMLIB00245 */
    return(ConvertPU_FLTF32_GCC_A32T32(f32In));
  #elif defined(__IAR_SYSTEMS_ICC__)
    /* @remarks Implements DMLIB00245 */
    return(ConvertPU_FLTF32_IAR_A32T32(f32In));
  #elif defined(__ghs__)
    /* @remarks Implements DMLIB00245 */
    return(ConvertPU_FLTF32_GHS_A32T32(f32In));
  #else
    /* @remarks Implements DMLIB00245 */
    return(ConvertPU_FLTF32_C(f32In));
  #endif
}


#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* MLIB_CONVERTPU_H */
