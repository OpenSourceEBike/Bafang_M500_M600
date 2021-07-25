/******************************************************************************
*
*   Copyright 2020 NXP
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
* @file       GMCLIB_Vpol2xy.h
*
* @version    1.0.0.0
*
* @date       10-June-2020
*
* @brief      Header file for GMCLIB_Vpol2xy function
*
******************************************************************************/
#ifndef GMCLIB_VPOL2XY_H
#define GMCLIB_VPOL2XY_H
/**
@if GMCLIB_GROUP
    @addtogroup GMCLIB_GROUP
@else
    @defgroup GMCLIB_GROUP   GMCLIB
@endif
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GMCLIB_Vpol2xy_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GMCLIB_Vpol2xy_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GMCLIB_Vpol2xy_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GMCLIB_Vpol2xy_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GMCLIB_Vpol2xy_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Allowing the inline functions significantly increase the speed of the library thus the lowest 
* layer (GMCLIB) is implemented as inline. This approach removes the overhead caused by standard 
* function calling. 
*/
#include "SWLIBS_Defines.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GMCLIB_Vpol2xy(...)     macro_dispatcher(GMCLIB_Vpol2xy, __VA_ARGS__)(__VA_ARGS__)     /* This function returns sum of two input parameters. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00126 */
    #define GMCLIB_Vpol2xy_Dsptchr_2(Out,In)     GMCLIB_Vpol2xy_Dsptchr_3(Out,In,F32)       /* Function dispatcher for GMCLIB_Vpol2xy_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00126 */
    #define GMCLIB_Vpol2xy_Dsptchr_2(Out,In)     GMCLIB_Vpol2xy_Dsptchr_3(Out,In,F16)       /* Function dispatcher for GMCLIB_Vpol2xy_Dsptchr_2, do not modify!!! */
  #endif
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00126 */
    #define GMCLIB_Vpol2xy_Dsptchr_2(Out,In)     GMCLIB_Vpol2xy_Dsptchr_3(Out,In,FLT)       /* Function dispatcher for GMCLIB_Vpol2xy_Dsptchr_2, do not modify!!! */
  #endif
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
  /*
  * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GMCLIB_Vpol2xy_Dsptchr_3(Out,In,Impl)    GMCLIB_Vpol2xy_Dsptchr_(Out,In,Impl)     /* Function dispatcher for GMCLIB_Vpol2xy_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GMCLIB_Vpol2xy_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref GMCLIB_Vpol2xy_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGMCLIB00128*/
  #define GMCLIB_Vpol2xy_Dsptchr_(Out,In,Impl)     GMCLIB_Vpol2xy_##Impl(Out,In)            /* Function dispatcher for GMCLIB_Vpol2xy_Dsptchr_, do not modify!!! */
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


/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
extern void GMCLIB_Vpol2xy_F32(SWLIBS_2Syst_F32 *const f32OutVec, const SWLIBS_2Syst_F32 *const f32InVec);

/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
extern void GMCLIB_Vpol2xy_F16(SWLIBS_2Syst_F16 *const f16OutVec, const SWLIBS_2Syst_F16 *const f16InVec);

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
extern void GMCLIB_Vpol2xy_FLT(SWLIBS_2Syst_FLT *const fltOutVec, const SWLIBS_2Syst_FLT *const fltInVec);

#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
#ifdef __cplusplus
}
#endif

#endif /* GMCLIB_VPOL2XY_H */
