/******************************************************************************
*
*   Copyright 2017-2019 NXP
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
* @file       AMCLIB_FW.h
*
* @version    1.0.17.0
*
* @date       17-April-2019
*
* @brief      Header file for AMCLIB_FW function
*
******************************************************************************/
#ifndef AMCLIB_FW_H
#define AMCLIB_FW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section AMCLIB_FW_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section AMCLIB_FW_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section AMCLIB_FW_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section AMCLIB_FW_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section AMCLIB_FW_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
*
* @section AMCLIB_FW_h_REF_6
* Violates MISRA 2004 Required Rule 1.4, Identifier clash.
* The compiler can distinguish names with more than 31 characters.
*
*/
#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"
#include "GDFLIB_FilterMA.h"
#include "GFLIB_ControllerPIpAW.h"
#include "GFLIB_Sign.h"
#include "GFLIB_SinCos.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FWInit(...)     macro_dispatcher(AMCLIB_FWInit, __VA_ARGS__)(__VA_ARGS__)     /* This function clears the AMCLIB_FW state variables. */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FWSetState(...)     macro_dispatcher(AMCLIB_FWSetState, __VA_ARGS__)(__VA_ARGS__)     /* This function initializes the AMCLIB_FW state variables to achieve the required output values. */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FW(...)     macro_dispatcher(AMCLIB_FW, __VA_ARGS__)(__VA_ARGS__)     /* This function adjusts the torque of the motor to achieve the required speed. The function employs the Field Weakening technique to extend the available speed range. */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FWDebug(...)     macro_dispatcher(AMCLIB_FWDebug, __VA_ARGS__)(__VA_ARGS__)     /* This function adjusts the torque of the motor to achieve the required speed. The function employs the Field Weakening technique to extend the available speed range. Debugging information is provided. */
  
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_FW_T                                        AMCLIB_FW_T_F32
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00074 */
    #define AMCLIB_FWInit_Dsptchr_1(pCtrl)     AMCLIB_FWInit_Dsptchr_2(pCtrl,F32)     /* Function dispatcher for AMCLIB_FWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00081 */
    #define AMCLIB_FWSetState_Dsptchr_3(f32FilterMAFWOut,f32ControllerPIpAWFWOut,pCtrl)     AMCLIB_FWSetState_Dsptchr_4(f32FilterMAFWOut,f32ControllerPIpAWFWOut,pCtrl,F32)     /* Function dispatcher for AMCLIB_FWSetState_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00067 */
    #define AMCLIB_FW_Dsptchr_4(f32IDQReqAmp,f32VelocityFbck,pIDQReq,pCtrl)     AMCLIB_FW_Dsptchr_5(f32IDQReqAmp,f32VelocityFbck,pIDQReq,pCtrl,F32)     /* Function dispatcher for AMCLIB_FW_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00133 */
    #define AMCLIB_FWDebug_Dsptchr_4(f32IDQReqAmp,f32VelocityFbck,pIDQReq,pCtrl)     AMCLIB_FWDebug_Dsptchr_5(f32IDQReqAmp,f32VelocityFbck,pIDQReq,pCtrl,F32)     /* Function dispatcher for AMCLIB_FWDebug_Dsptchr_4, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_FW_T                                        AMCLIB_FW_T_F16
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00074 */
    #define AMCLIB_FWInit_Dsptchr_1(pCtrl)     AMCLIB_FWInit_Dsptchr_2(pCtrl,F16)     /* Function dispatcher for AMCLIB_FWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00081 */
    #define AMCLIB_FWSetState_Dsptchr_3(f16FilterMAFWOut,f16ControllerPIpAWFWOut,pCtrl)     AMCLIB_FWSetState_Dsptchr_4(f16FilterMAFWOut,f16ControllerPIpAWFWOut,pCtrl,F16)     /* Function dispatcher for AMCLIB_FWSetState_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00067 */
    #define AMCLIB_FW_Dsptchr_4(f16IDQReqAmp,f16VelocityFbck,pIDQReq,pCtrl)     AMCLIB_FW_Dsptchr_5(f16IDQReqAmp,f16VelocityFbck,pIDQReq,pCtrl,F16)     /* Function dispatcher for AMCLIB_FW_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00133 */
    #define AMCLIB_FWDebug_Dsptchr_4(f16IDQReqAmp,f16VelocityFbck,pIDQReq,pCtrl)     AMCLIB_FWDebug_Dsptchr_5(f16IDQReqAmp,f16VelocityFbck,pIDQReq,pCtrl,F16)     /* Function dispatcher for AMCLIB_FWDebug_Dsptchr_4, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_FW_T                                        AMCLIB_FW_T_FLT
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00074 */
    #define AMCLIB_FWInit_Dsptchr_1(pCtrl)     AMCLIB_FWInit_Dsptchr_2(pCtrl,FLT)     /* Function dispatcher for AMCLIB_FWInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00081 */
    #define AMCLIB_FWSetState_Dsptchr_3(fltFilterMAFWOut,fltControllerPIpAWFWOut,pCtrl)     AMCLIB_FWSetState_Dsptchr_4(fltFilterMAFWOut,fltControllerPIpAWFWOut,pCtrl,FLT)     /* Function dispatcher for AMCLIB_FWSetState_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00067 */
    #define AMCLIB_FW_Dsptchr_4(fltIDQReqAmp,fltVelocityFbck,pIDQReq,pCtrl)     AMCLIB_FW_Dsptchr_5(fltIDQReqAmp,fltVelocityFbck,pIDQReq,pCtrl,FLT)     /* Function dispatcher for AMCLIB_FW_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00133 */
    #define AMCLIB_FWDebug_Dsptchr_4(fltIDQReqAmp,fltVelocityFbck,pIDQReq,pCtrl)     AMCLIB_FWDebug_Dsptchr_5(fltIDQReqAmp,fltVelocityFbck,pIDQReq,pCtrl,FLT)     /* Function dispatcher for AMCLIB_FW_Dsptchr_4, do not modify!!! */
  #endif 
  
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref AMCLIB_FW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define AMCLIB_FWInit_Dsptchr_2(pCtrl,Impl)    AMCLIB_FWInit_Dsptchr_(pCtrl,Impl)     /* Function dispatcher for AMCLIB_FWInit_Dsptchr_2, do not modify!!! */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  * @violates @ref AMCLIB_FW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DAMCLIB00076 */
  #define AMCLIB_FWInit_Dsptchr_(pCtrl,Impl)     AMCLIB_FWInit_##Impl(pCtrl)            /* Function dispatcher for AMCLIB_FWInit_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref AMCLIB_FW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define AMCLIB_FWSetState_Dsptchr_4(FilterMAFWOut,ControllerPIpAWFWOut,pCtrl,Impl)    AMCLIB_FWSetState_Dsptchr_(FilterMAFWOut,ControllerPIpAWFWOut,pCtrl,Impl)     /* Function dispatcher for AMCLIB_FWSetState_Dsptchr_4, do not modify!!! */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  * @violates @ref AMCLIB_FW_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DAMCLIB00083 */
  #define AMCLIB_FWSetState_Dsptchr_(FilterMAFWOut,ControllerPIpAWFWOut,pCtrl,Impl)     AMCLIB_FWSetState_##Impl(FilterMAFWOut,ControllerPIpAWFWOut,pCtrl)            /* Function dispatcher for AMCLIB_FWSetState_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FW_Dsptchr_5(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)    AMCLIB_FW_Dsptchr_(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)     /* Function dispatcher for AMCLIB_FW_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DAMCLIB00069 */
  #define AMCLIB_FW_Dsptchr_(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)     AMCLIB_FW_##Impl(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl)            /* Function dispatcher for AMCLIB_FW_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_FWDebug_Dsptchr_5(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)    AMCLIB_FWDebug_Dsptchr_(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)     /* Function dispatcher for AMCLIB_FWDebug_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_FW_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_FW_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_FW_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_FW_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DAMCLIB00135 */
  #define AMCLIB_FWDebug_Dsptchr_(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl,Impl)     AMCLIB_FWDebug_##Impl(IDQReqAmp,VelocityFbck,pIDQReq,pCtrl)            /* Function dispatcher for AMCLIB_FWDebug_Dsptchr_, do not modify!!! */
#endif

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_FW_DEFAULT_F32 {0,0, \
                                 (tFrac32)0,(tFrac32)0,(tS32)0,(tS32)0,INT32_MIN,INT32_MAX,(tFrac32)0,(tFrac32)0,(tU16)0, \
                                 (tFrac32 *)0, \
                                 (tFrac32 *)0, \
                                 (tFrac32 *)0}               /**< Default value for AMCLIB_FW_T_F32. */

  /************************************************************************/
  /**
  @struct AMCLIB_FW_T_F32 "\AMCLIB_FW.h" 

  @brief  FW configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00066 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F32 pFilterFW;  /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F32 pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFrac32 *pIQFbck;  /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFrac32 *pUQReq;  /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFrac32 *pUQLim;  /**< Pointer to the q-axis voltage controller upper limit. Must be a positive value. */
  }AMCLIB_FW_T_F32;

  /************************************************************************/
  /**
  @struct AMCLIB_FW_DEBUG_T_F32 "\AMCLIB_FW.h" 

  @brief  FW configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00132 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F32 pFilterFW;        /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F32 pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFrac32 *pIQFbck;                        /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFrac32 *pUQReq;                         /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFrac32 *pUQLim;                         /**< Pointer to the q-axis voltage controller upper limit. Must be a positive value. */
    tFrac32 f32IQErrSign;                    /**< FW - Current deviation after sign elimination. */
    tFrac32 f32IQErr;                        /**< FW - Current deviation. */
    tFrac32 f32FWErr;                        /**< FW - Field weakening feedback control variable. */
    tFrac32 f32UQErr;                        /**< FW - Voltage deviation. */
    tFrac32 f32FWErrFilt;                    /**< FW - Filtered field weakening feedback control variable. */
    tFrac32 f32FWAngle;                      /**< FW - Field weakening angle. */
    tFrac32 f32FWSin;                        /**< FW - Q-axis unity current phasor component. */
    tFrac32 f32FWCos;                        /**< FW - D-axis unity current phasor component. */
  }AMCLIB_FW_DEBUG_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_FWInit_F32(AMCLIB_FW_T_F32 *const pCtrl);
  extern void AMCLIB_FWSetState_F32(tFrac32 f32FilterMAFWOut, tFrac32 f32ControllerPIpAWFWOut,
    AMCLIB_FW_T_F32 *pCtrl);
  extern void AMCLIB_FW_F32(tFrac32 f32IDQReqAmp, tFrac32 f32VelocityFbck,
    SWLIBS_2Syst_F32 *const pIDQReq, AMCLIB_FW_T_F32 *pCtrl);
  extern void AMCLIB_FWDebug_F32(tFrac32 f32IDQReqAmp, tFrac32 f32VelocityFbck,
    SWLIBS_2Syst_F32 *const pIDQReq, AMCLIB_FW_DEBUG_T_F32 *pCtrl);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)

  /*
  * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_FW_DEFAULT_F16 {0,0, \
                                 (tFrac16)0,(tFrac16)0,(tS16)0,(tS16)0,INT16_MIN,INT16_MAX,(tFrac32)0,(tFrac16)0,(tU16)0, \
                                 (tFrac16 *)0, \
                                 (tFrac16 *)0, \
                                 (tFrac16 *)0}               /**< Default value for AMCLIB_FW_T_F16. */
  
  /************************************************************************/
  /**
  @struct AMCLIB_FW_T_F16 "\AMCLIB_FW.h" 

  @brief  FW configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00066 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F16 pFilterFW;  /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F16 pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFrac16 *pIQFbck;  /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFrac16 *pUQReq;  /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFrac16 *pUQLim;  /**< Pointer to the q-axis voltage controller upper limit. Must be a positive value. */
  }AMCLIB_FW_T_F16;
  
  /************************************************************************/
  /**
  @struct AMCLIB_FW_DEBUG_T_F16 "\AMCLIB_FW.h" 

  @brief  FW configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00132 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F16 pFilterFW;        /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F16 pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFrac16 *pIQFbck;                        /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFrac16 *pUQReq;                         /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFrac16 *pUQLim;                         /**< Pointer to the q-axis voltage controller upper limit (should point to the AMCLIB_CURRENT_LOOP_T_F16 structure element pPIrAWQ.f16UpperLimit). The limit must be a positive value. */
    tFrac16 f16IQErrSign;                    /**< FW - Current deviation after sign elimination. */
    tFrac16 f16IQErr;                        /**< FW - Current deviation. */
    tFrac16 f16FWErr;                        /**< FW - Field weakening feedback control variable. */
    tFrac16 f16UQErr;                        /**< FW - Voltage deviation. */
    tFrac16 f16FWErrFilt;                    /**< FW - Filtered field weakening feedback control variable. */
    tFrac16 f16FWAngle;                      /**< FW - Field weakening angle. */
    tFrac16 f16FWSin;                        /**< FW - Q-axis unity current phasor component. */
    tFrac16 f16FWCos;                        /**< FW - D-axis unity current phasor component. */
  }AMCLIB_FW_DEBUG_T_F16;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_FWInit_F16(AMCLIB_FW_T_F16 *const pCtrl);
  extern void AMCLIB_FWSetState_F16(tFrac16 f16FilterMAFWOut, tFrac16 f16ControllerPIpAWFWOut,
    AMCLIB_FW_T_F16 *pCtrl);
  extern void AMCLIB_FW_F16(tFrac16 f16IDQReqAmp, tFrac16 f16VelocityFbck,
    SWLIBS_2Syst_F16 *const pIDQReq, AMCLIB_FW_T_F16 *pCtrl);
  extern void AMCLIB_FWDebug_F16(tFrac16 f16IDQReqAmp, tFrac16 f16VelocityFbck,
    SWLIBS_2Syst_F16 *const pIDQReq, AMCLIB_FW_DEBUG_T_F16 *pCtrl);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)

  /*
  * @violates @ref AMCLIB_FW_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_FW_DEFAULT_FLT {0,0 \
                                 (tFloat)0,(tFloat)0,FLOAT_MIN,FLOAT_MAX,(tFloat)0,(tFloat)0,(tU16)0, \
                                 (tFloat *)0, \
                                 (tFloat *)0, \
                                 (tFloat *)0}               /**< Default value for AMCLIB_FW_T_FLT. */

  /************************************************************************/
  /**
  @struct AMCLIB_FW_T_FLT "\AMCLIB_FW.h"

  @brief  FW configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00066 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_FLT pFilterFW;  /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_FLT pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFloat *pIQFbck;  /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFloat *pUQReq;  /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFloat *pUQLim;  /**< Pointer to the q-axis voltage controller upper limit (should point to the AMCLIB_CURRENT_LOOP_T_FLT structure element pPIrAWQ.fltUpperLimit). The limit must be a positive value. */
    tFloat fltUmaxDivImax;  /**< Maximal coil voltage divided by the maximal coil current. */
  }AMCLIB_FW_T_FLT;
  
  /************************************************************************/
  /**
  @struct AMCLIB_FW_DEBUG_T_FLT "\AMCLIB_FW.h"

  @brief  FW configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00132 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_FLT pFilterFW;        /**< Field weakening angle FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_FLT pPIpAWFW;  /**< Field weakening angle ControllerPIpAW paremeters structure. */
    tFloat *pIQFbck;                         /**< Pointer to the q-axis component of the feedback current in the two-phase rotational orthogonal system (d-q). */
    tFloat *pUQReq;                          /**< Pointer to the q-axis component of the required voltage in the two-phase rotational orthogonal system (d-q). */
    tFloat *pUQLim;                          /**< Pointer to the q-axis voltage controller upper limit (should point to the AMCLIB_CURRENT_LOOP_T_FLT structure element pPIrAWQ.fltUpperLimit). The limit must be a positive value. */
    tFloat fltUmaxDivImax;                   /**< Maximal coil voltage divided by the maximal coil current. */
    tFloat fltIQErrSign;                     /**< FW - Current deviation after sign elimination. */
    tFloat fltIQErr;                         /**< FW - Current deviation. */
    tFloat fltFWErr;                         /**< FW - Field weakening feedback control variable. */
    tFloat fltUQErr;                         /**< FW - Voltage deviation. */
    tFloat fltFWErrFilt;                     /**< FW - Filtered field weakening feedback control variable. */
    tFloat fltFWAngle;                       /**< FW - Field weakening angle. */
    tFloat fltFWSin;                         /**< FW - Q-axis unity current phasor component. */
    tFloat fltFWCos;                         /**< FW - D-axis unity current phasor component. */
  }AMCLIB_FW_DEBUG_T_FLT;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_FWInit_FLT(AMCLIB_FW_T_FLT *const pCtrl);
  extern void AMCLIB_FWSetState_FLT(tFloat fltFilterMAFWOut, tFloat fltControllerPIpAWFWOut,
    AMCLIB_FW_T_FLT *pCtrl);
  extern void AMCLIB_FW_FLT(tFloat fltIDQReqAmp, tFloat fltVelocityFbck,
    SWLIBS_2Syst_FLT *const pIDQReq, AMCLIB_FW_T_FLT *pCtrl);
  extern void AMCLIB_FWDebug_FLT(tFloat fltIDQReqAmp, tFloat fltVelocityFbck,
    SWLIBS_2Syst_FLT *const pIDQReq, AMCLIB_FW_DEBUG_T_FLT *pCtrl);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* AMCLIB_FW_H */
