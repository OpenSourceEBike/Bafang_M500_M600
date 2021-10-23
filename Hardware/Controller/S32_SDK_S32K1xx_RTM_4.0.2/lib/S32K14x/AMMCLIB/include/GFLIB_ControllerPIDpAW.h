/******************************************************************************
*
*   Copyright 2018-2019 NXP
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
* @file       GFLIB_ControllerPIDpAW.h
*
* @version    1.0.0.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_ControllerPIDpAW function
*
******************************************************************************/
#ifndef GFLIB_CONTROLLERPIDPAW_H
#define GFLIB_CONTROLLERPIDPAW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_ControllerPIDpAW_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_ControllerPIDpAW_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_ControllerPIDpAW_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_ControllerPIDpAW_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators.
* 
* @section GFLIB_ControllerPIDpAW_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section GFLIB_ControllerPIDpAW_h_REF_6
* Violates MISRA 2004 Required Rule 1.4, Identifier clash. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
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
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIDpAWInit(...)     macro_dispatcher(GFLIB_ControllerPIDpAWInit, __VA_ARGS__)(__VA_ARGS__)     /* This function clears the state variables of the Proportional-Integral controller. */
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIDpAWSetState(...)     macro_dispatcher(GFLIB_ControllerPIDpAWSetState, __VA_ARGS__)(__VA_ARGS__)     /* This function sets the state variables of the Proportional-Integral controller to achieve the required output value. */
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIDpAW(...)     macro_dispatcher(GFLIB_ControllerPIDpAW, __VA_ARGS__)(__VA_ARGS__)     /* This function calculates a parallel form of the Proportional- Integral controller, without integral anti-windup. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PID_P_AW_T                         GFLIB_CONTROLLER_PID_P_AW_T_F32                         /**< Definition of GFLIB_CONTROLLER_PID_P_AW_T as alias for GFLIB_CONTROLLER_PID_P_AW_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters.
    */
    #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT                   GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F32                   /**< Definition of GFLIB_CONTROLLER_PID_P_AW_DEFAULT as alias for GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00438 */
    #define GFLIB_ControllerPIDpAWInit_Dsptchr_1(pParam)     GFLIB_ControllerPIDpAWInit_Dsptchr_2(pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIDpAWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00445 */
    #define GFLIB_ControllerPIDpAWSetState_Dsptchr_2(ControllerPIDpAWAWOut,pParam)     GFLIB_ControllerPIDpAWSetState_Dsptchr_3(ControllerPIDpAWAWOut,pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIDpAWSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters.
    */
    /* @remarks Implements DGFLIB00431 */
    #define GFLIB_ControllerPIDpAW_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIDpAW_Dsptchr_3(InErr,pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIDpAW_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PID_P_AW_T                         GFLIB_CONTROLLER_PID_P_AW_T_F16                         /**< Definition of GFLIB_CONTROLLER_PID_P_AW_T as alias for GFLIB_CONTROLLER_PID_P_AW_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT                   GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F16                   /**< Definition of GFLIB_CONTROLLER_PID_P_AW_DEFAULT as alias for GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00438 */
    #define GFLIB_ControllerPIDpAWInit_Dsptchr_1(pParam)     GFLIB_ControllerPIDpAWInit_Dsptchr_2(pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIDpAWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00445 */
    #define GFLIB_ControllerPIDpAWSetState_Dsptchr_2(ControllerPIDpAWAWOut,pParam)     GFLIB_ControllerPIDpAWSetState_Dsptchr_3(ControllerPIDpAWAWOut,pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIDpAWSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00431 */
    #define GFLIB_ControllerPIDpAW_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIDpAW_Dsptchr_3(InErr,pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIDpAW_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    #define GFLIB_CONTROLLER_PID_P_AW_T                         GFLIB_CONTROLLER_PID_P_AW_T_FLT                         /**< Definition of GFLIB_CONTROLLER_PID_P_AW_T as alias for GFLIB_CONTROLLER_PID_P_AW_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro.
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT                   GFLIB_CONTROLLER_PID_P_AW_DEFAULT_FLT                   /**< Definition of GFLIB_CONTROLLER_PID_P_AW_DEFAULT as alias for GFLIB_CONTROLLER_PID_P_AW_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00438 */
    #define GFLIB_ControllerPIDpAWInit_Dsptchr_1(pParam)     GFLIB_ControllerPIDpAWInit_Dsptchr_2(pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIDpAWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00445 */
    #define GFLIB_ControllerPIDpAWSetState_Dsptchr_2(ControllerPIDpAWAWOut,pParam)     GFLIB_ControllerPIDpAWSetState_Dsptchr_3(ControllerPIDpAWAWOut,pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIDpAWSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00431 */
    #define GFLIB_ControllerPIDpAW_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIDpAW_Dsptchr_3(InErr,pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIDpAW_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_ControllerPIDpAWInit_Dsptchr_2(pParam,Impl)    GFLIB_ControllerPIDpAWInit_Dsptchr_(pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIDpAWInit_Dsptchr_2, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  /* @remarks Implements DGFLIB00440 */
  #define GFLIB_ControllerPIDpAWInit_Dsptchr_(pParam,Impl)     GFLIB_ControllerPIDpAWInit_##Impl(pParam)            /* Function dispatcher for GFLIB_ControllerPIDpAWInit_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_ControllerPIDpAWSetState_Dsptchr_3(ControllerPIDpAWAWOut,pParam,Impl)    GFLIB_ControllerPIDpAWSetState_Dsptchr_(ControllerPIDpAWAWOut,pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIDpAWSetState_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  /* @remarks Implements DGFLIB00447 */
  #define GFLIB_ControllerPIDpAWSetState_Dsptchr_(ControllerPIDpAWAWOut,pParam,Impl)     GFLIB_ControllerPIDpAWSetState_##Impl(ControllerPIDpAWAWOut,pParam)            /* Function dispatcher for GFLIB_ControllerPIDpAWSetState_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro.
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define GFLIB_ControllerPIDpAW_Dsptchr_3(InErr,pParam,Impl)    GFLIB_ControllerPIDpAW_Dsptchr_(InErr,pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIDpAW_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DGFLIB00433 */
  #define GFLIB_ControllerPIDpAW_Dsptchr_(InErr,pParam,Impl)     GFLIB_ControllerPIDpAW_##Impl(InErr,pParam)            /* Function dispatcher for GFLIB_ControllerPIDpAW_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters.
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F32 {(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tS16)0,(tS16)0,(tS16)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tU16)0}     /**< Default value for GFLIB_CONTROLLER_PID_P_AW_T_F32. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PID_P_AW_T_F32 "\GFLIB_ControllerPIDpAW.h" 

  @brief  Structure containing parameters and states of the PID controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00448 */
  typedef struct{
    tFrac32 f32PropGain;        /**< Proportional Gain, fractional format normalized to fit into (0, \f$2^{31}\f$-1). */
    tFrac32 f32IntegGain;       /**< Integral Gain, fractional format normalized to fit into  (0, \f$2^{31}\f$-1). */
    tFrac32 f32DerivGain;       /**< Derivative Gain, fractional format normalized to fit into  (0, \f$2^{31}\f$-1). */
    tFrac32 f32FiltCoef;        /**< Derivative term filter coefficient, fractional format in range (0, \f$2^{31}\f$-1). */
    tS16    s16PropGainShift;   /**< Proportional Gain Shift, integer format [-31, 31]. */
    tS16    s16IntegGainShift;  /**< Integral Gain Shift, integer format [-31, 31]. */
    tS16    s16DerivGainShift;  /**< Derivative Gain Shift, integer format [-31, 31]. */    
    tFrac32 f32LowerLimit;      /**< Lower Limit of the controller, fractional format normalized to fit into (-\f$2^{31}\f$, \f$2^{31}\f$-1). */
    tFrac32 f32UpperLimit;      /**< Upper Limit of the controller, fractional format normalized to fit into (-\f$2^{31}\f$, \f$2^{31}\f$-1). */
    tFrac32 f32IntegPartK_1;    /**< State variable integral part at step k-1. */
    tFrac32 f32DerivPartK_1;    /**< State variable derivative part filter at step k-1. */
    tFrac32 f32InK_1;           /**< State variable input error at step k-1. */
    tU16    u16LimitFlag;       /**< Limitation flag, if set to 1, the controller output has reached either the UpperLimit or LowerLimit. */
  }GFLIB_CONTROLLER_PID_P_AW_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIDpAWInit_F32(GFLIB_CONTROLLER_PID_P_AW_T_F32 *const pParam);
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters.
  */
  extern void GFLIB_ControllerPIDpAWSetState_F32(tFrac32 f32ControllerPIDpAWOut,GFLIB_CONTROLLER_PID_P_AW_T_F32 *const pParam);
  extern tFrac32 GFLIB_ControllerPIDpAW_F32(tFrac32 f32InErr,GFLIB_CONTROLLER_PID_P_AW_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT_F16 {(tFrac16)0,(tFrac16)0,(tFrac16)0,(tFrac16)0,(tS16)0,(tS16)0,(tS16)0,(tFrac16)0,(tFrac16)0,(tFrac32)0,(tFrac16)0,(tFrac16)0,(tU16)0}     /**< Default value for GFLIB_CONTROLLER_PID_P_AW_T_F16. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PID_P_AW_T_F16 "\GFLIB_ControllerPIDpAW.h" 

  @brief  Structure containing parameters and states of the parallel form PI controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00448 */
  typedef struct{
    tFrac16 f16PropGain;        /**< Proportional Gain, fractional format normalized to fit into (0, \f$2^{15}\f$-1). */
    tFrac16 f16IntegGain;       /**< Integral Gain, fractional format normalized to fit into  (0, \f$2^{15}\f$-1). */
    tFrac16 f16DerivGain;       /**< Derivative Gain, fractional format normalized to fit into  (0, \f$2^{15}\f$-1). */
    tFrac16 f16FiltCoef;        /**< Derivative term filter coefficient, fractional format in range (0, \f$2^{15}\f$-1). */
    tS16 s16PropGainShift;      /**< Proportional Gain Shift, integer format [-15, 15]. */
    tS16 s16IntegGainShift;     /**< Integral Gain Shift, integer format [-15, 15]. */
    tS16 s16DerivGainShift;     /**< Derivative Gain Shift, integer format [-15, 15]. */  
    tFrac16 f16LowerLimit;      /**< Lower Limit of the controller, fractional format normalized to fit into (-\f$2^{15}\f$, \f$2^{15}\f$-1). */
    tFrac16 f16UpperLimit;      /**< Upper Limit of the controller, fractional format normalized to fit into (-\f$2^{15}\f$, \f$2^{15}\f$-1). */
    tFrac32 f32IntegPartK_1;    /**< State variable integral part at step k-1. */
    tFrac16 f16DerivPartK_1;    /**< State variable integral part at step k-1. */
    tFrac16 f16InK_1;           /**< State variable input error at step k-1. */
    tU16    u16LimitFlag;       /**< Limitation flag, if set to 1, the controller output has reached either the UpperLimit or LowerLimit. */
  }GFLIB_CONTROLLER_PID_P_AW_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIDpAWInit_F16(GFLIB_CONTROLLER_PID_P_AW_T_F16 *const pParam);
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters.
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  extern void GFLIB_ControllerPIDpAWSetState_F16(tFrac16 f16ControllerPIDpAWOut,GFLIB_CONTROLLER_PID_P_AW_T_F16 *const pParam);
  extern tFrac16 GFLIB_ControllerPIDpAW_F16(tFrac16 f16InErr,GFLIB_CONTROLLER_PID_P_AW_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_CONTROLLER_PID_P_AW_DEFAULT_FLT {(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tU16)0}     /**< Default value for GFLIB_CONTROLLER_PID_P_AW_T_FLT. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PID_P_AW_T_FLT "\GFLIB_ControllerPIDpAW.h" 

  @brief  Structure containing parameters and states of the parallel form PI controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00448 */
  typedef struct{
    tFloat fltPropGain;     /**< Proportional Gain, positive number in single precision floating point format. */
    tFloat fltIntegGain;    /**< Integral Gain, positive number in single precision floating point format. */
    tFloat fltDerivGain;    /**< Derivative Gain, positive number in single precision floating point format. */
    tFloat fltFiltCoef;     /**< Derivative term filter coefficient, single precision floating point format. */
    tFloat fltLowerLimit;   /**< Lower Limit of the controller, single precision floating point format. */
    tFloat fltUpperLimit;   /**< Upper Limit of the controller, single precision floating point format. */
    tFloat fltIntegPartK_1; /**< State variable integral part at step k-1, single precision floating point format. */
    tFloat fltDerivPartK_1; /**< State variable integral part at step k-1, single precision floating point format. */
    tFloat fltInK_1;        /**< State variable input error at step k-1, single precision floating point format. */
    tU16      u16LimitFlag; /**< Limitation flag, if set to 1, the controller output has reached either the UpperLimit or LowerLimit. */
  }GFLIB_CONTROLLER_PID_P_AW_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIDpAWInit_FLT(GFLIB_CONTROLLER_PID_P_AW_T_FLT *const pParam);
  /*
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters.
  * @violates @ref GFLIB_ControllerPIDpAW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  extern void GFLIB_ControllerPIDpAWSetState_FLT(tFloat fltControllerPIDpAWOut,GFLIB_CONTROLLER_PID_P_AW_T_FLT *const pParam);
  extern tFloat GFLIB_ControllerPIDpAW_FLT(tFloat fltInErr,GFLIB_CONTROLLER_PID_P_AW_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_CONTROLLERPIDPAW_H */
