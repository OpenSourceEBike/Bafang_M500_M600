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
* @file       GFLIB_ControllerPIr.h
*
* @version    1.0.10.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_ControllerPIr function
*
******************************************************************************/
#ifndef GFLIB_CONTROLLERPIR_H
#define GFLIB_CONTROLLERPIR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_ControllerPIr_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_ControllerPIr_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_ControllerPIr_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_ControllerPIr_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GFLIB_ControllerPIr_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section GFLIB_ControllerPIr_h_REF_6
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
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIrInit(...)     macro_dispatcher(GFLIB_ControllerPIrInit, __VA_ARGS__)(__VA_ARGS__)     /* This function clears the state variables of the Proportional-Integral controller. */
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIrSetState(...)     macro_dispatcher(GFLIB_ControllerPIrSetState, __VA_ARGS__)(__VA_ARGS__)     /* This function sets the state variables of the Proportional-Integral controller to achieve the required output value. */
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIr(...)     macro_dispatcher(GFLIB_ControllerPIr, __VA_ARGS__)(__VA_ARGS__)     /* This function calculates a standard recurrent form of the Proportional- Integral controller, without integral anti-windup. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_T                         GFLIB_CONTROLLER_PI_R_T_F32                         /**< Definition of GFLIB_CONTROLLER_PI_R_T as alias for GFLIB_CONTROLLER_PI_R_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_DEFAULT                   GFLIB_CONTROLLER_PI_R_DEFAULT_F32                   /**< Definition of GFLIB_CONTROLLER_PI_R_DEFAULT as alias for GFLIB_CONTROLLER_PI_R_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00403 */
    #define GFLIB_ControllerPIrInit_Dsptchr_1(pParam)     GFLIB_ControllerPIrInit_Dsptchr_2(pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIrInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00410 */
    #define GFLIB_ControllerPIrSetState_Dsptchr_2(ControllerPIrOut,pParam)     GFLIB_ControllerPIrSetState_Dsptchr_3(ControllerPIrOut,pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIrSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00274 */
    #define GFLIB_ControllerPIr_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIr_Dsptchr_3(InErr,pParam,F32)     /* Function dispatcher for GFLIB_ControllerPIr_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_T                         GFLIB_CONTROLLER_PI_R_T_F16                         /**< Definition of GFLIB_CONTROLLER_PI_R_T as alias for GFLIB_CONTROLLER_PI_R_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_DEFAULT                   GFLIB_CONTROLLER_PI_R_DEFAULT_F16                   /**< Definition of GFLIB_CONTROLLER_PI_R_DEFAULT as alias for GFLIB_CONTROLLER_PI_R_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00403 */
    #define GFLIB_ControllerPIrInit_Dsptchr_1(pParam)     GFLIB_ControllerPIrInit_Dsptchr_2(pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIrInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00410 */
    #define GFLIB_ControllerPIrSetState_Dsptchr_2(ControllerPIrOut,pParam)     GFLIB_ControllerPIrSetState_Dsptchr_3(ControllerPIrOut,pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIrSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00274 */
    #define GFLIB_ControllerPIr_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIr_Dsptchr_3(InErr,pParam,F16)     /* Function dispatcher for GFLIB_ControllerPIr_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_T                         GFLIB_CONTROLLER_PI_R_T_FLT                         /**< Definition of GFLIB_CONTROLLER_PI_R_T as alias for GFLIB_CONTROLLER_PI_R_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_CONTROLLER_PI_R_DEFAULT                   GFLIB_CONTROLLER_PI_R_DEFAULT_FLT                   /**< Definition of GFLIB_CONTROLLER_PI_R_DEFAULT as alias for GFLIB_CONTROLLER_PI_R_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00403 */
    #define GFLIB_ControllerPIrInit_Dsptchr_1(pParam)     GFLIB_ControllerPIrInit_Dsptchr_2(pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIrInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGFLIB00410 */
    #define GFLIB_ControllerPIrSetState_Dsptchr_2(ControllerPIrOut,pParam)     GFLIB_ControllerPIrSetState_Dsptchr_3(ControllerPIrOut,pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIrSetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00274 */
    #define GFLIB_ControllerPIr_Dsptchr_2(InErr,pParam)     GFLIB_ControllerPIr_Dsptchr_3(InErr,pParam,FLT)     /* Function dispatcher for GFLIB_ControllerPIr_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_ControllerPIrInit_Dsptchr_2(pParam,Impl)    GFLIB_ControllerPIrInit_Dsptchr_(pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIrInit_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  /* @remarks Implements DGFLIB00405 */
  #define GFLIB_ControllerPIrInit_Dsptchr_(pParam,Impl)     GFLIB_ControllerPIrInit_##Impl(pParam)            /* Function dispatcher for GFLIB_ControllerPIrInit_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_ControllerPIrSetState_Dsptchr_3(ControllerPIrOut,pParam,Impl)    GFLIB_ControllerPIrSetState_Dsptchr_(ControllerPIrOut,pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIrSetState_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  /* @remarks Implements DGFLIB00412 */
  #define GFLIB_ControllerPIrSetState_Dsptchr_(ControllerPIrOut,pParam,Impl)     GFLIB_ControllerPIrSetState_##Impl(ControllerPIrOut,pParam)            /* Function dispatcher for GFLIB_ControllerPIrSetState_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_ControllerPIr_Dsptchr_3(InErr,pParam,Impl)    GFLIB_ControllerPIr_Dsptchr_(InErr,pParam,Impl)     /* Function dispatcher for GFLIB_ControllerPIr_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  */
  /* @remarks Implements DGFLIB00277 */
  #define GFLIB_ControllerPIr_Dsptchr_(InErr,pParam,Impl)     GFLIB_ControllerPIr_##Impl(InErr,pParam)            /* Function dispatcher for GFLIB_ControllerPIr_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define GFLIB_CONTROLLER_PI_R_DEFAULT_F32 {(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tU16)0}     /**< Default value for GFLIB_CONTROLLER_PI_R_T_F32. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PI_R_T_F32 "\GFLIB_ControllerPIr.h" 

  @brief  Structure containing parameters and states of the recurrent form PI controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00276 */
  typedef struct{
    tFrac32 f32CC1sc;      /**< CC1 coefficient, fractional format normalized to fit into (-\f$2^{31}\f$, \f$2^{31}\f$-1). */
    tFrac32 f32CC2sc;      /**< CC2 coefficient, fractional format normalized to fit into (-\f$2^{31}\f$, \f$2^{31}\f$-1). */
    tFrac32 f32Acc;        /**< State variable - internal controller accumulator. */
    tFrac32 f32InErrK1;    /**< State variable - controller input from the previous calculation step. */
    tU16    u16NShift;     /**< Scaling bitwise shift applied to the controller coefficients, integer format [0, 31]. */
  }GFLIB_CONTROLLER_PI_R_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIrInit_F32(GFLIB_CONTROLLER_PI_R_T_F32 *const pParam);
  extern void GFLIB_ControllerPIrSetState_F32(tFrac32 f32ControllerPIrOut,GFLIB_CONTROLLER_PI_R_T_F32 *const pParam);
  extern tFrac32 GFLIB_ControllerPIr_F32(tFrac32 f32InErr,GFLIB_CONTROLLER_PI_R_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_CONTROLLER_PI_R_DEFAULT_F16 {(tFrac16)0,(tFrac16)0,(tFrac32)0,(tFrac16)0,(tU16)0}     /**< Default value for GFLIB_CONTROLLER_PI_R_T_F16. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PI_R_T_F16 "\GFLIB_ControllerPIr.h" 

  @brief  Structure containing parameters and states of the recurrent form PI controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00276 */
  typedef struct{
    tFrac16 f16CC1sc;      /**< CC1 coefficient, fractional format normalized to fit into (-\f$2^{15}\f$, \f$2^{15}\f$-1). */
    tFrac16 f16CC2sc;      /**< CC2 coefficient, fractional format normalized to fit into (-\f$2^{15}\f$, \f$2^{15}\f$-1). */
    tFrac32 f32Acc;        /**< State variable - internal controller accumulator. */
    tFrac16 f16InErrK1;    /**< State variable - controller input from the previous calculation step. */
    tU16    u16NShift;     /**< Scaling bitwise shift applied to the controller coefficients, integer format [0, 15]. */
  }GFLIB_CONTROLLER_PI_R_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIrInit_F16(GFLIB_CONTROLLER_PI_R_T_F16 *const pParam);
  extern void GFLIB_ControllerPIrSetState_F16(tFrac16 f16ControllerPIrOut,GFLIB_CONTROLLER_PI_R_T_F16 *const pParam);
  extern tFrac16 GFLIB_ControllerPIr_F16(tFrac16 f16InErr,GFLIB_CONTROLLER_PI_R_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_ControllerPIr_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GFLIB_ControllerPIr_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GFLIB_CONTROLLER_PI_R_DEFAULT_FLT {(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0}     /**< Default value for GFLIB_CONTROLLER_PI_R_T_FLT. */

  /************************************************************************/
  /**
  @struct GFLIB_CONTROLLER_PI_R_T_FLT "\GFLIB_ControllerPIr.h" 

  @brief  Structure containing parameters and states of the recurrent form PI controller.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00276 */
  typedef struct{
    tFloat fltCC1sc;       /**< CC1 coefficient, single precision floating point format. */
    tFloat fltCC2sc;       /**< CC2 coefficient, single precision floating point format. */
    tFloat fltAcc;         /**< State variable - internal controller accumulator, single precision floating point format. */
    tFloat fltInErrK1;     /**< State variable - controller input from the previous calculation step, single precision floating point format. */
  }GFLIB_CONTROLLER_PI_R_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_ControllerPIrInit_FLT(GFLIB_CONTROLLER_PI_R_T_FLT *const pParam);
  extern void GFLIB_ControllerPIrSetState_FLT(tFloat fltControllerPIrOut,GFLIB_CONTROLLER_PI_R_T_FLT *const pParam);
  extern tFloat GFLIB_ControllerPIr_FLT(tFloat fltInErr,GFLIB_CONTROLLER_PI_R_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_CONTROLLERPIR_H */
