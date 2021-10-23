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
* @file       AMCLIB_SpeedLoop.h
*
* @version    1.0.17.0
*
* @date       17-April-2019
*
* @brief      Header file for AMCLIB_SpeedLoop function
*
******************************************************************************/
#ifndef AMCLIB_SPEEDLOOP_H
#define AMCLIB_SPEEDLOOP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section AMCLIB_SpeedLoop_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section AMCLIB_SpeedLoop_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section AMCLIB_SpeedLoop_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section AMCLIB_SpeedLoop_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section AMCLIB_SpeedLoop_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
*
* @section AMCLIB_SpeedLoop_h_REF_6
* Violates MISRA 2004 Required Rule 1.4, Identifier clash.
* The compiler can distinguish names with more than 31 characters.
*
*/
#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"
#include "GFLIB_Ramp.h"
#include "GDFLIB_FilterMA.h"
#include "GFLIB_ControllerPIpAW.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoopInit(...)     macro_dispatcher(AMCLIB_SpeedLoopInit, __VA_ARGS__)(__VA_ARGS__)     /* This function clears the AMCLIB_SpeedLoop state variables. */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoopSetState(...)     macro_dispatcher(AMCLIB_SpeedLoopSetState, __VA_ARGS__)(__VA_ARGS__)     /* This function initializes the AMCLIB_SpeedLoop state variables to achieve the required output values. */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoop(...)     macro_dispatcher(AMCLIB_SpeedLoop, __VA_ARGS__)(__VA_ARGS__)     /* This function adjusts the torque of the motor to achieve the required speed. The function employs the Field Weakening technique to extend the available speed range. */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoopDebug(...)     macro_dispatcher(AMCLIB_SpeedLoopDebug, __VA_ARGS__)(__VA_ARGS__)     /* This function adjusts the torque of the motor to achieve the required speed. The function employs the Field Weakening technique to extend the available speed range. Debugging information is provided. */
  
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_SPEED_LOOP_T                                        AMCLIB_SPEED_LOOP_T_F32
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00118 */
    #define AMCLIB_SpeedLoopInit_Dsptchr_1(pCtrl)     AMCLIB_SpeedLoopInit_Dsptchr_2(pCtrl,F32)     /* Function dispatcher for AMCLIB_SpeedLoopInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00125 */
    #define AMCLIB_SpeedLoopSetState_Dsptchr_4(f32FilterMAWOut,f32ControllerPIpAWQOut,f32RampOut,pCtrl)     AMCLIB_SpeedLoopSetState_Dsptchr_5(f32FilterMAWOut,f32ControllerPIpAWQOut,f32RampOut,pCtrl,F32)     /* Function dispatcher for AMCLIB_SpeedLoopSetState_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00111 */
    #define AMCLIB_SpeedLoop_Dsptchr_4(f32VelocityReq,f32VelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoop_Dsptchr_5(f32VelocityReq,f32VelocityFbck,pIDQReq,pCtrl,F32)     /* Function dispatcher for AMCLIB_SpeedLoop_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00149 */
    #define AMCLIB_SpeedLoopDebug_Dsptchr_4(f32VelocityReq,f32VelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoopDebug_Dsptchr_5(f32VelocityReq,f32VelocityFbck,pIDQReq,pCtrl,F32)     /* Function dispatcher for AMCLIB_SpeedLoopDebug_Dsptchr_4, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_SPEED_LOOP_T                                        AMCLIB_SPEED_LOOP_T_F16
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00118 */
    #define AMCLIB_SpeedLoopInit_Dsptchr_1(pCtrl)     AMCLIB_SpeedLoopInit_Dsptchr_2(pCtrl,F16)     /* Function dispatcher for AMCLIB_SpeedLoopInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00125 */
    #define AMCLIB_SpeedLoopSetState_Dsptchr_4(f16FilterMAWOut,f16ControllerPIpAWQOut,f32RampOut,pCtrl)     AMCLIB_SpeedLoopSetState_Dsptchr_5(f16FilterMAWOut,f16ControllerPIpAWQOut,f32RampOut,pCtrl,F16)     /* Function dispatcher for AMCLIB_SpeedLoopSetState_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00111 */
    #define AMCLIB_SpeedLoop_Dsptchr_4(f16VelocityReq,f16VelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoop_Dsptchr_5(f16VelocityReq,f16VelocityFbck,pIDQReq,pCtrl,F16)     /* Function dispatcher for AMCLIB_SpeedLoop_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00149 */
    #define AMCLIB_SpeedLoopDebug_Dsptchr_4(f16VelocityReq,f16VelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoopDebug_Dsptchr_5(f16VelocityReq,f16VelocityFbck,pIDQReq,pCtrl,F16)     /* Function dispatcher for AMCLIB_SpeedLoopDebug_Dsptchr_4, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_SPEED_LOOP_T                                        AMCLIB_SPEED_LOOP_T_FLT
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00118 */
    #define AMCLIB_SpeedLoopInit_Dsptchr_1(pCtrl)     AMCLIB_SpeedLoopInit_Dsptchr_2(pCtrl,FLT)     /* Function dispatcher for AMCLIB_SpeedLoopInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMCLIB00125 */
    #define AMCLIB_SpeedLoopSetState_Dsptchr_4(fltFilterMAWOut,fltControllerPIpAWQOut,fltRampOut,pCtrl)     AMCLIB_SpeedLoopSetState_Dsptchr_5(fltFilterMAWOut,fltControllerPIpAWQOut,fltRampOut,pCtrl,FLT)     /* Function dispatcher for AMCLIB_SpeedLoopSetState_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00111 */
    #define AMCLIB_SpeedLoop_Dsptchr_4(fltVelocityReq,fltVelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoop_Dsptchr_5(fltVelocityReq,fltVelocityFbck,pIDQReq,pCtrl,FLT)     /* Function dispatcher for AMCLIB_SpeedLoop_Dsptchr_4, do not modify!!! */
    /*
    * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMCLIB00149 */
    #define AMCLIB_SpeedLoopDebug_Dsptchr_4(fltVelocityReq,fltVelocityFbck,pIDQReq,pCtrl)     AMCLIB_SpeedLoopDebug_Dsptchr_5(fltVelocityReq,fltVelocityFbck,pIDQReq,pCtrl,FLT)     /* Function dispatcher for AMCLIB_SpeedLoopDebug_Dsptchr_4, do not modify!!! */
  #endif 
  
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref AMCLIB_SpeedLoop_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define AMCLIB_SpeedLoopInit_Dsptchr_2(pCtrl,Impl)    AMCLIB_SpeedLoopInit_Dsptchr_(pCtrl,Impl)     /* Function dispatcher for AMCLIB_SpeedLoopInit_Dsptchr_2, do not modify!!! */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DAMCLIB00120 */
  #define AMCLIB_SpeedLoopInit_Dsptchr_(pCtrl,Impl)     AMCLIB_SpeedLoopInit_##Impl(pCtrl)            /* Function dispatcher for AMCLIB_SpeedLoopInit_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref AMCLIB_SpeedLoop_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define AMCLIB_SpeedLoopSetState_Dsptchr_5(FilterMAWOut,ControllerPIpAWQOut,RampOut,pCtrl,Impl)    AMCLIB_SpeedLoopSetState_Dsptchr_(FilterMAWOut,ControllerPIpAWQOut,RampOut,pCtrl,Impl)     /* Function dispatcher for AMCLIB_SpeedLoopSetState_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  */
  /* @remarks Implements DAMCLIB00127 */
  #define AMCLIB_SpeedLoopSetState_Dsptchr_(FilterMAWOut,ControllerPIpAWQOut,RampOut,pCtrl,Impl)     AMCLIB_SpeedLoopSetState_##Impl(FilterMAWOut,ControllerPIpAWQOut,RampOut,pCtrl)            /* Function dispatcher for AMCLIB_SpeedLoopSetState_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoop_Dsptchr_5(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)    AMCLIB_SpeedLoop_Dsptchr_(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)     /* Function dispatcher for AMCLIB_SpeedLoop_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DAMCLIB00113 */
  #define AMCLIB_SpeedLoop_Dsptchr_(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)     AMCLIB_SpeedLoop_##Impl(VelocityReq,VelocityFbck,pIDQReq,pCtrl)            /* Function dispatcher for AMCLIB_SpeedLoop_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_SpeedLoopDebug_Dsptchr_5(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)    AMCLIB_SpeedLoopDebug_Dsptchr_(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)     /* Function dispatcher for AMCLIB_SpeedLoopDebug_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_SpeedLoop_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DAMCLIB00151 */
  #define AMCLIB_SpeedLoopDebug_Dsptchr_(VelocityReq,VelocityFbck,pIDQReq,pCtrl,Impl)     AMCLIB_SpeedLoopDebug_##Impl(VelocityReq,VelocityFbck,pIDQReq,pCtrl)            /* Function dispatcher for AMCLIB_SpeedLoopDebug_Dsptchr_, do not modify!!! */
#endif

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_SPEED_LOOP_DEFAULT_F32 {0,0, \
    (tFrac32)0,(tFrac32)0,(tS32)0,(tS32)0,INT32_MIN,INT32_MAX,(tFrac32)0,(tFrac32)0,(tU16)0, \
    (tFrac32)0,(tFrac32)0,(tFrac32)0} /**< Default value for AMCLIB_SPEED_LOOP_T_F32. */

  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_T_F32 "\AMCLIB_SpeedLoop.h" 

  @brief  SpeedLoop configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00110 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F32 pFilterW;  /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F32 pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_F32 pRamp;  /**< Speed ramp paremeters structure. */
  }AMCLIB_SPEED_LOOP_T_F32;

  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_DEBUG_T_F32 "\AMCLIB_SpeedLoop.h" 

  @brief  SpeedLoop configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00148 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F32 pFilterW;        /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F32 pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_F32 pRamp;                 /**< Speed ramp paremeters structure. */  
    tFrac32 f32WReqFilt;                    /**< SpeedLoop - Filtered value of the required angular velocity. */
    tFrac32 f32WErr;                        /**< SpeedLoop - Velocity deviation. */
    tFrac32 f32WFbckFilt;                   /**< SpeedLoop - Filtered value of the measured angular velocity. */
  }AMCLIB_SPEED_LOOP_DEBUG_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_SpeedLoopInit_F32(AMCLIB_SPEED_LOOP_T_F32 *const pCtrl);
  extern void AMCLIB_SpeedLoopSetState_F32(tFrac32 f32FilterMAWOut, tFrac32 f32ControllerPIpAWQOut,
    tFrac32 f32RampOut, AMCLIB_SPEED_LOOP_T_F32 *pCtrl);
  extern void AMCLIB_SpeedLoop_F32(tFrac32 f32VelocityReq, tFrac32 f32VelocityFbck,
    SWLIBS_2Syst_F32 *const pIDQReq, AMCLIB_SPEED_LOOP_T_F32 *pCtrl);
  extern void AMCLIB_SpeedLoopDebug_F32(tFrac32 f32VelocityReq, tFrac32 f32VelocityFbck,
    SWLIBS_2Syst_F32 *const pIDQReq, AMCLIB_SPEED_LOOP_DEBUG_T_F32 *pCtrl);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)

  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_SPEED_LOOP_DEFAULT_F16 {0,0, \
    (tFrac16)0,(tFrac16)0,(tS16)0,(tS16)0,INT16_MIN,INT16_MAX,(tFrac32)0,(tFrac16)0,(tU16)0, \
    (tFrac32)0,(tFrac32)0,(tFrac32)0}               /**< Default value for AMCLIB_SPEED_LOOP_T_F16. */
  
  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_T_F16 "\AMCLIB_SpeedLoop.h" 

  @brief  SpeedLoop configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00110 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F16 pFilterW;  /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F16 pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_F32 pRamp;  /**< Speed ramp paremeters structure. */
  }AMCLIB_SPEED_LOOP_T_F16;
  
  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_DEBUG_T_F16 "\AMCLIB_SpeedLoop.h" 

  @brief  SpeedLoop configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00148 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_F16 pFilterW;        /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_F16 pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_F32 pRamp;                 /**< Speed ramp paremeters structure. */
    tFrac16 f16WReqFilt;                    /**< SpeedLoop - Filtered value of the required angular velocity. */
    tFrac16 f16WErr;                        /**< SpeedLoop - Velocity deviation. */
    tFrac16 f16WFbckFilt;                   /**< SpeedLoop - Filtered value of the measured angular velocity. */
  }AMCLIB_SPEED_LOOP_DEBUG_T_F16;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_SpeedLoopInit_F16(AMCLIB_SPEED_LOOP_T_F16 *const pCtrl);
  extern void AMCLIB_SpeedLoopSetState_F16(tFrac16 f16FilterMAWOut, tFrac16 f16ControllerPIpAWQOut,
    tFrac32 f32RampOut, AMCLIB_SPEED_LOOP_T_F16 *pCtrl);
  extern void AMCLIB_SpeedLoop_F16(tFrac16 f16VelocityReq, tFrac16 f16VelocityFbck,
    SWLIBS_2Syst_F16 *const pIDQReq, AMCLIB_SPEED_LOOP_T_F16 *pCtrl);
  extern void AMCLIB_SpeedLoopDebug_F16(tFrac16 f16VelocityReq, tFrac16 f16VelocityFbck,
    SWLIBS_2Syst_F16 *const pIDQReq, AMCLIB_SPEED_LOOP_DEBUG_T_F16 *pCtrl);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)

  /*
  * @violates @ref AMCLIB_SpeedLoop_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define AMCLIB_SPEED_LOOP_DEFAULT_FLT {0,0 \
    (tFloat)0,(tFloat)0,FLOAT_MIN,FLOAT_MAX,(tFloat)0,(tFloat)0,(tU16)0, \
    (tFloat)0,(tFloat)0,(tFloat)0}               /**< Default value for AMCLIB_SPEED_LOOP_T_FLT. */

  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_T_FLT "\AMCLIB_SpeedLoop.h"

  @brief  SpeedLoop configuration structure.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00110 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_FLT pFilterW;  /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_FLT pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_FLT pRamp;  /**< Speed ramp paremeters structure. */
  }AMCLIB_SPEED_LOOP_T_FLT;

  /************************************************************************/
  /**
  @struct AMCLIB_SPEED_LOOP_DEBUG_T_FLT "\AMCLIB_SpeedLoop.h"

  @brief  SpeedLoop configuration structure with debugging information.
  *//**********************************************************************/
  /* @remarks Implements DAMCLIB00148 */
  typedef struct
  {
    GDFLIB_FILTER_MA_T_FLT pFilterW;        /**< Velocity FilterMA paremeters structure. */
    GFLIB_CONTROLLER_PIAW_P_T_FLT pPIpAWQ;  /**< Velocity ControllerPIpAW paremeters structure. */
    GFLIB_RAMP_T_FLT pRamp;                 /**< Speed ramp paremeters structure. */
    tFloat fltWReqFilt;                     /**< SpeedLoop - Filtered value of the required angular velocity. */
    tFloat fltWErr;                         /**< SpeedLoop - Velocity deviation. */
    tFloat fltWFbckFilt;                    /**< SpeedLoop - Filtered value of the measured angular velocity. */
  }AMCLIB_SPEED_LOOP_DEBUG_T_FLT;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_SpeedLoopInit_FLT(AMCLIB_SPEED_LOOP_T_FLT *const pCtrl);
  extern void AMCLIB_SpeedLoopSetState_FLT(tFloat fltFilterMAWOut, tFloat fltControllerPIpAWQOut,
    tFloat fltRampOut, AMCLIB_SPEED_LOOP_T_FLT *pCtrl);
  extern void AMCLIB_SpeedLoop_FLT(tFloat fltVelocityReq, tFloat fltVelocityFbck,
    SWLIBS_2Syst_FLT *const pIDQReq, AMCLIB_SPEED_LOOP_T_FLT *pCtrl);
  extern void AMCLIB_SpeedLoopDebug_FLT(tFloat fltVelocityReq, tFloat fltVelocityFbck,
    SWLIBS_2Syst_FLT *const pIDQReq, AMCLIB_SPEED_LOOP_DEBUG_T_FLT *pCtrl);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* AMCLIB_SPEEDLOOP_H */
