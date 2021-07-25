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
* @file       MLIB_ShR.h
*
* @version    1.0.29.0
*
* @date       17-April-2019
*
* @brief      Header file for MLIB_ShR function
*
******************************************************************************/
#ifndef MLIB_SHR_H
#define MLIB_SHR_H
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
* @section MLIB_ShR_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section MLIB_ShR_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section MLIB_ShR_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section MLIB_ShR_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section MLIB_ShR_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
* 
* @section MLIB_ShR_h_REF_6
* Violates MISRA 2004 Required Rule 12.7, Bitwise operator applied to signed underlying type. 
* The fractional arithmetic requires the bit-wise operations on signed values. 
* 
* @section MLIB_ShR_h_REF_7
* Violates MISRA 2004 Required Rule 10.5, Shift right of signed quantity (int). 
* Shifting of signed values is intentionally handled when necessary. 
*/

#include "SWLIBS_Defines.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_ShR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_ShR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_ShR(...)     macro_dispatcher(MLIB_ShR, __VA_ARGS__)(__VA_ARGS__)     /* This function shifts the first parameter to right by number defined by second parameter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_ShR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_ShR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_ShR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00185 */
    #define MLIB_ShR_Dsptchr_2(In1,In2)     MLIB_ShR_Dsptchr_3(In1,In2,F32)     /* Function dispatcher for MLIB_ShR_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_ShR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref MLIB_ShR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref MLIB_ShR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DMLIB00185 */
    #define MLIB_ShR_Dsptchr_2(In1,In2)     MLIB_ShR_Dsptchr_3(In1,In2,F16)     /* Function dispatcher for MLIB_ShR_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref MLIB_ShR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_ShR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define MLIB_ShR_Dsptchr_3(In1,In2,Impl)    MLIB_ShR_Dsptchr_(In1,In2,Impl)   /* Function dispatcher for MLIB_ShR_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref MLIB_ShR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref MLIB_ShR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref MLIB_ShR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref MLIB_ShR_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DMLIB00187 */
  #define MLIB_ShR_Dsptchr_(In1,In2,Impl)     MLIB_ShR_##Impl(In1,In2)          /* Function dispatcher for MLIB_ShR_Dsptchr_, do not modify!!! */
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
/* MLIB_ShR_F16 implementation variant - C */
INLINE tFrac16 ShR_F16_C(register tFrac16 f16In,register tU16 u16In)
/*
* @violates @ref MLIB_ShR_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header
* file.
*/
{
  /*
  * @violates @ref MLIB_ShR_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  * @violates @ref MLIB_ShR_h_REF_7 MISRA 2004 Required Rule 10.5, Shift right of signed quantity 
  * (int). 
  */
  return((tFrac16)(f16In >> u16In));
}





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ShR

@param[in]         f32In1     First value to be right shift.

@param[in]         u16In2     The shift amount value.

@return       32-bit fractional value shifted right by the shift amount.
              The bits beyond the 32-bit boundary of the result are discarded.

@note         The shift amount cannot exceed in magnitude the bit-width of
              the shifted value, that means it must be within the range 0...31.
              Otherwise the result of the function is undefined.
              Due to effectivity reason this function is implemented as inline,
              and thus is not ANSI-C compliant.

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

    // output should be 0x10000000 ~ FRAC32(0.125)
    f32Out = MLIB_ShR_F32(f32In1, u16In2);

    // output should be 0x10000000 ~ FRAC32(0.125)
    f32Out = MLIB_ShR(f32In1, u16In2, F32);

    // ##############################################################
    // Available only if 32-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 0x10000000 ~ FRAC32(0.125)
    f32Out = MLIB_ShR(f32In1, u16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00186, DMLIB00180, DMLIB00184 */
INLINE tFrac32 MLIB_ShR_F32(register tFrac32 f32In1,register tU16 u16In2)
/*
* @violates @ref MLIB_ShR_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /*
  * @violates @ref MLIB_ShR_h_REF_6 MISRA 2004 Required Rule 12.7, Bitwise operator applied to 
  * signed underlying type. 
  */
  /* @remarks Implements DMLIB00182, DMLIB00183 */
  return((tFrac32)(f32In1 >> u16In2));
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    MLIB_GROUP

@functionality MLIB_ShR

@param[in]         f16In1     First value to be right shift.

@param[in]         u16In2     The shift amount value.

@return       16-bit fractional value shifted right by the shift amount.
              The bits beyond the 16-bit boundary of the result are discarded.

*/
/**
@note         The shift amount cannot exceed in magnitude the bit-width of
              the shifted value, that means it must be within the range 0...15.
              Otherwise the result of the function is undefined.
              Due to effectivity reason this function is implemented as inline,
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

    // output should be 0x1000 ~ FRAC16(0.125)
    f16Out = MLIB_ShR_F16(f16In1, u16In2);

    // output should be 0x1000 ~ FRAC16(0.125)
    f16Out = MLIB_ShR(f16In1, u16In2, F16);

    // ##############################################################
    // Available only if 16-bit fractional implementation selected
    // as default
    // ##############################################################

    // output should be 0x1000 ~ FRAC16(0.125)
    f16Out = MLIB_ShR(f16In1, u16In2);
}
\endcode
****************************************************************************/
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif /* if defined __CWCC__ || defined __MWERKS__ */
/* @remarks Implements DMLIB00186, DMLIB00180, DMLIB00184 */
INLINE tFrac16 MLIB_ShR_F16(register tFrac16 f16In1,register tU16 u16In2)
/*
* @violates @ref MLIB_ShR_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
  /* @remarks Implements DMLIB00181, DMLIB00183 */
  return(ShR_F16_C(f16In1,u16In2));
}

#ifdef __cplusplus
}
#endif

#endif /* MLIB_SHR_H */
