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
* @file       GFLIB_Sqrt.h
*
* @version    1.0.15.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_Sqrt function
*
******************************************************************************/
#ifndef GFLIB_SQRT_H
#define GFLIB_SQRT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_Sqrt_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_Sqrt_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_Sqrt_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_Sqrt_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GFLIB_Sqrt_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Inlined functions require definition in the header file.
*/

#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Sqrt_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Sqrt(...)     macro_dispatcher(GFLIB_Sqrt, __VA_ARGS__)(__VA_ARGS__)     /* This function returns the square root of input value. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Sqrt_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00112 */
    #define GFLIB_Sqrt_Dsptchr_1(In)     GFLIB_Sqrt_Dsptchr_2(In,F32)     /* Function dispatcher for GFLIB_Sqrt_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Sqrt_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00112 */
    #define GFLIB_Sqrt_Dsptchr_1(In)     GFLIB_Sqrt_Dsptchr_2(In,F16)     /* Function dispatcher for GFLIB_Sqrt_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Sqrt_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00112 */
    #define GFLIB_Sqrt_Dsptchr_1(In)     GFLIB_Sqrt_Dsptchr_2(In,FLT)     /* Function dispatcher for GFLIB_Sqrt_Dsptchr_1, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Sqrt_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Sqrt_Dsptchr_2(In,Impl)    GFLIB_Sqrt_Dsptchr_(In,Impl)   /* Function dispatcher for GFLIB_Sqrt_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_Sqrt_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Sqrt_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Sqrt_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGFLIB00114 */
  #define GFLIB_Sqrt_Dsptchr_(In,Impl)     GFLIB_Sqrt_##Impl(In)          /* Function dispatcher for GFLIB_Sqrt_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if ((SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)||(SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON))

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac32 GFLIB_Sqrt_F32(tFrac32 f32In);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac16 GFLIB_Sqrt_F16(tFrac16 f16In);
#endif /* ((SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)||(SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)) */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)

/****************************************************************************
* Inline functions
****************************************************************************/
#if (defined(__GNUC__) && defined(__ARM_EABI__))
  #define GFLIB_SQRT_FLT_OPTIMIZED
  INLINE tFloat GFLIB_Sqrt_FLT(tFloat fltIn)
  /*
  * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  {
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFloat fltReturn;
    
    __asm volatile(" vsqrt.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
    return(fltReturn);
  }
#elif defined(__IAR_SYSTEMS_ICC__)
  #define GFLIB_SQRT_FLT_OPTIMIZED
  INLINE tFloat GFLIB_Sqrt_FLT(tFloat fltIn)
  /*
  * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  {
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFloat fltReturn;
    
    __asm(" vsqrt.f32 %0,%1" : "=t"(fltReturn): "t"(fltIn));
    return(fltReturn);
  }
#elif defined(__ghs__)
  #define GFLIB_SQRT_FLT_OPTIMIZED
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
  INLINE tFloat GFLIB_Sqrt_FLT(tFloat fltIn)
  /*
  * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  {
    /*
    * @violates @ref GFLIB_Sqrt_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
    * header file.
    */
    register tFloat fltReturn;
    
    #pragma ghs optasm
    asm(" vsqrt.f32 %0,%1" : "=f"(fltReturn): "f"(fltIn));
    return(fltReturn);
  }
#else

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern tFloat GFLIB_Sqrt_FLT(tFloat fltIn);
#endif
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_SQRT_H */
