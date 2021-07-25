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
* @file       GFLIB_UpperLimit.h
*
* @version    1.0.8.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_UpperLimit function
*
******************************************************************************/
#ifndef GFLIB_UPPERLIMIT_H
#define GFLIB_UPPERLIMIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_UpperLimit_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_UpperLimit_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_UpperLimit_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_UpperLimit_h_REF_4
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
  * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_UpperLimit(...)     macro_dispatcher(GFLIB_UpperLimit, __VA_ARGS__)(__VA_ARGS__)     /* This function tests whether the input value is below the upper limit. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_T                        GFLIB_UPPERLIMIT_T_F32                        /**< Definition of GFLIB_UPPERLIMIT_T as alias for GFLIB_UPPERLIMIT_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_DEFAULT                  GFLIB_UPPERLIMIT_DEFAULT_F32                  /**< Definition of GFLIB_UPPERLIMIT_DEFAULT as alias for GFLIB_UPPERLIMIT_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00235 */
    #define GFLIB_UpperLimit_Dsptchr_2(In,pParam)     GFLIB_UpperLimit_Dsptchr_3(In,pParam,F32)     /* Function dispatcher for GFLIB_UpperLimit_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_T                        GFLIB_UPPERLIMIT_T_F16                        /**< Definition of GFLIB_UPPERLIMIT_T as alias for GFLIB_UPPERLIMIT_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_DEFAULT                  GFLIB_UPPERLIMIT_DEFAULT_F16                  /**< Definition of GFLIB_UPPERLIMIT_DEFAULT as alias for GFLIB_UPPERLIMIT_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00235 */
    #define GFLIB_UpperLimit_Dsptchr_2(In,pParam)     GFLIB_UpperLimit_Dsptchr_3(In,pParam,F16)     /* Function dispatcher for GFLIB_UpperLimit_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_T                        GFLIB_UPPERLIMIT_T_FLT                        /**< Definition of GFLIB_UPPERLIMIT_T as alias for GFLIB_UPPERLIMIT_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GFLIB_UPPERLIMIT_DEFAULT                  GFLIB_UPPERLIMIT_DEFAULT_FLT                  /**< Definition of GFLIB_UPPERLIMIT_DEFAULT as alias for GFLIB_UPPERLIMIT_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00235 */
    #define GFLIB_UpperLimit_Dsptchr_2(In,pParam)     GFLIB_UpperLimit_Dsptchr_3(In,pParam,FLT)     /* Function dispatcher for GFLIB_UpperLimit_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_UpperLimit_Dsptchr_3(In,pParam,Impl)    GFLIB_UpperLimit_Dsptchr_(In,pParam,Impl)   /* Function dispatcher for GFLIB_UpperLimit_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_UpperLimit_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_UpperLimit_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_UpperLimit_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_UpperLimit_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGFLIB00237 */
  #define GFLIB_UpperLimit_Dsptchr_(In,pParam,Impl)     GFLIB_UpperLimit_##Impl(In,pParam)          /* Function dispatcher for GFLIB_UpperLimit_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  #define GFLIB_UPPERLIMIT_DEFAULT_F32 {INT32_MAX}     /**< Default value for GFLIB_UPPERLIMIT_T_F32. */

  /*************************************************************************/
  /**
  @struct GFLIB_UPPERLIMIT_T_F32 "\GFLIB_UpperLimit.h" 

  @brief  Structure containing the upper limit.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00238 */
  typedef struct{  
    tFrac32 f32UpperLimit; /**< Value determining the upper limit threshold. */
  }GFLIB_UPPERLIMIT_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac32 GFLIB_UpperLimit_F32(tFrac32 f32In, const GFLIB_UPPERLIMIT_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  #define GFLIB_UPPERLIMIT_DEFAULT_F16 {INT16_MAX}     /**< Default value for GFLIB_UPPERLIMIT_T_F16. */

  /*************************************************************************/
  /**
  @struct GFLIB_UPPERLIMIT_T_F16 "\GFLIB_UpperLimit.h" 

  @brief  Structure containing the upper limit.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00322 */
  typedef struct{    
    tFrac16 f16UpperLimit; /**< Value determining the upper limit threshold. */
  }GFLIB_UPPERLIMIT_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac16 GFLIB_UpperLimit_F16(tFrac16 f16In, const GFLIB_UPPERLIMIT_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #define GFLIB_UPPERLIMIT_DEFAULT_FLT {FLOAT_MAX}     /**< Default value for GFLIB_UPPERLIMIT_T_FLT. */

  /*************************************************************************/
  /**
  @struct GFLIB_UPPERLIMIT_T_FLT "\GFLIB_UpperLimit.h" 

  @brief  Structure containing the upper limit.
  *//***********************************************************************/
  /* @remarks Implements DGFLIB00321 */
  typedef struct{    
    tFloat fltUpperLimit; /**< Value determining the upper limit threshold. */
  }GFLIB_UPPERLIMIT_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFloat GFLIB_UpperLimit_FLT(tFloat fltIn, const GFLIB_UPPERLIMIT_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_UPPERLIMIT_H */
