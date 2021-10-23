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
* @file       GFLIB_Hyst.h
*
* @version    1.0.9.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_Hyst function
*
******************************************************************************/
#ifndef GFLIB_HYST_H
#define GFLIB_HYST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_Hyst_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_Hyst_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_Hyst_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_Hyst_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
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
  * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Hyst(...)     macro_dispatcher(GFLIB_Hyst, __VA_ARGS__)(__VA_ARGS__)     /* The function implements the hysteresis functionality. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_T                        GFLIB_HYST_T_F32                        /**< Definition of GFLIB_HYST_T as alias for GFLIB_HYST_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_DEFAULT                  GFLIB_HYST_DEFAULT_F32                  /**< Definition of GFLIB_HYST_DEFAULT as alias for GFLIB_HYST_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00180 */
    #define GFLIB_Hyst_Dsptchr_2(In,pParam)     GFLIB_Hyst_Dsptchr_3(In,pParam,F32)     /* Function dispatcher for GFLIB_Hyst_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_T                        GFLIB_HYST_T_F16                        /**< Definition of GFLIB_HYST_T as alias for GFLIB_HYST_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_DEFAULT                  GFLIB_HYST_DEFAULT_F16                  /**< Definition of GFLIB_HYST_DEFAULT as alias for GFLIB_HYST_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00180 */
    #define GFLIB_Hyst_Dsptchr_2(In,pParam)     GFLIB_Hyst_Dsptchr_3(In,pParam,F16)     /* Function dispatcher for GFLIB_Hyst_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_T                        GFLIB_HYST_T_FLT                        /**< Definition of GFLIB_HYST_T as alias for GFLIB_HYST_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_HYST_DEFAULT                  GFLIB_HYST_DEFAULT_FLT                  /**< Definition of GFLIB_HYST_DEFAULT as alias for GFLIB_HYST_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00180 */
    #define GFLIB_Hyst_Dsptchr_2(In,pParam)     GFLIB_Hyst_Dsptchr_3(In,pParam,FLT)     /* Function dispatcher for GFLIB_Hyst_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Hyst_Dsptchr_3(In,pParam,Impl)    GFLIB_Hyst_Dsptchr_(In,pParam,Impl)   /* Function dispatcher for GFLIB_Hyst_Dsptchr_3, do not modify!!! */


  /*
  * @violates @ref GFLIB_Hyst_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Hyst_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Hyst_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_Hyst_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGFLIB00182 */ 
  #define GFLIB_Hyst_Dsptchr_(In,pParam,Impl)     GFLIB_Hyst_##Impl(In,pParam)          /* Function dispatcher for GFLIB_Hyst_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  #define GFLIB_HYST_DEFAULT_F32 {(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0,(tFrac32)0}     /**< Default value for GFLIB_HYST_T_F32. */

  /*************************************************************************/
  /**
  @struct GFLIB_HYST_T_F32 "\GFLIB_Hyst.h" 

  @brief  Structure containing parameters and states for the hysteresis function.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00184, DGFLIB00183, DGFLIB00173, DGFLIB00176, DGFLIB00179*/
  typedef struct{
    tFrac32 f32HystOn; /**< Value determining the upper threshold. */
    tFrac32 f32HystOff; /**< Value determining the lower threshold. */
    tFrac32 f32OutValOn; /**< Value of the output when input is higher than the upper threshold. */
    tFrac32 f32OutValOff; /**< Value of the output when input is lower than the lower threshold. */
    tFrac32 f32OutState; /**< Actual state of the output. */
  }GFLIB_HYST_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac32 GFLIB_Hyst_F32(tFrac32 f32In,GFLIB_HYST_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  #define GFLIB_HYST_DEFAULT_F16 {(tFrac16)0,(tFrac16)0,(tFrac16)0,(tFrac16)0,(tFrac16)0}     /**< Default value for GFLIB_HYST_T_F16. */

  /*************************************************************************/
  /**
  @struct GFLIB_HYST_T_F16 "\GFLIB_Hyst.h" 

  @brief  Structure containing parameters and states for the hysteresis function.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00184, DGFLIB00183, DGFLIB00173, DGFLIB00175, DGFLIB00178*/
  typedef struct{
    tFrac16 f16HystOn; /**< Value determining the upper threshold. */
    tFrac16 f16HystOff; /**< Value determining the lower threshold. */
    tFrac16 f16OutValOn; /**< Value of the output when input is higher than the upper threshold. */
    tFrac16 f16OutValOff; /**< Value of the output when input is lower than the lower threshold. */
    tFrac16 f16OutState; /**< Actual state of the output. */
  }GFLIB_HYST_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac16 GFLIB_Hyst_F16(tFrac16 f16In,GFLIB_HYST_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #define GFLIB_HYST_DEFAULT_FLT {(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0,(tFloat)0}     /**< Default value for GFLIB_HYST_T_FLT. */

  /*************************************************************************/
  /**
  @struct GFLIB_HYST_T_FLT "\GFLIB_Hyst.h" 

  @brief  Structure containing parameters and states for the hysteresis function.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00184, DGFLIB00183, DGFLIB00173, DGFLIB00174, DGFLIB00177*/
  typedef struct{                                   
    tFloat fltHystOn; /**< Value determining the upper threshold. */
    tFloat fltHystOff; /**< Value determining the lower threshold. */
    tFloat fltOutValOn; /**< Value of the output when input is higher than the upper threshold. */
    tFloat fltOutValOff; /**< Value of the output when input is lower than the lower threshold. */
    tFloat fltOutState; /**< Actual state of the output. */
  }GFLIB_HYST_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFloat GFLIB_Hyst_FLT(tFloat fltIn,GFLIB_HYST_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_HYST_H */
