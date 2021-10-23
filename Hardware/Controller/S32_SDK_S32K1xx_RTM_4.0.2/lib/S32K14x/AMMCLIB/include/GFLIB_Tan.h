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
* @file       GFLIB_Tan.h
*
* @version    1.0.8.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_Tan function
*
******************************************************************************/
#ifndef GFLIB_TAN_H
#define GFLIB_TAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_Tan_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_Tan_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_Tan_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_Tan_h_REF_4
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
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GFLIB_Tan(...)     macro_dispatcher(GFLIB_Tan, __VA_ARGS__)(__VA_ARGS__)     /* This function implements polynomial approximation of tangent function. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_T                        GFLIB_TAN_T_F32                           /**< Definition of GFLIB_TAN_T as alias for GFLIB_TAN_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_DEFAULT                  GFLIB_TAN_DEFAULT_F32                     /**< Definition of GFLIB_TAN_DEFAULT as alias for GFLIB_TAN_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Tan_Dsptchr_2(In,pParam)     GFLIB_Tan_Dsptchr_3(In,pParam,F32)        /* Function dispatcher for GFLIB_Tan_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00035 */
    #define GFLIB_Tan_Dsptchr_1(In)            GFLIB_Tan_Dsptchr_2(In,GFLIB_TAN_DEFAULT) /* Function dispatcher for GFLIB_Tan_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_T                        GFLIB_TAN_T_F16                           /**< Definition of GFLIB_TAN_T as alias for GFLIB_TAN_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_DEFAULT                  GFLIB_TAN_DEFAULT_F16                     /**< Definition of GFLIB_TAN_DEFAULT as alias for GFLIB_TAN_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Tan_Dsptchr_2(In,pParam)     GFLIB_Tan_Dsptchr_3(In,pParam,F16)        /* Function dispatcher for GFLIB_Tan_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00035 */
    #define GFLIB_Tan_Dsptchr_1(In)            GFLIB_Tan_Dsptchr_2(In,GFLIB_TAN_DEFAULT) /* Function dispatcher for GFLIB_Tan_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_T                        GFLIB_TAN_T_FLT                           /**< Definition of GFLIB_TAN_T as alias for GFLIB_TAN_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_TAN_DEFAULT                  GFLIB_TAN_DEFAULT_FLT                     /**< Definition of GFLIB_TAN_DEFAULT as alias for GFLIB_TAN_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Tan_Dsptchr_2(In,pParam)     GFLIB_Tan_Dsptchr_3(In,pParam,FLT)        /* Function dispatcher for GFLIB_Tan_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00035 */
    #define GFLIB_Tan_Dsptchr_1(In)            GFLIB_Tan_Dsptchr_2(In,GFLIB_TAN_DEFAULT) /* Function dispatcher for GFLIB_Tan_Dsptchr_1, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GFLIB_Tan_Dsptchr_3(In,pParam,Impl)    GFLIB_Tan_Dsptchr_(In,pParam,Impl)     /* Function dispatcher for GFLIB_Tan_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_Tan_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Tan_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref GFLIB_Tan_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGFLIB00037 */
  #define GFLIB_Tan_Dsptchr_(In,pParam,Impl)     GFLIB_Tan_##Impl(In,pParam)            /* Function dispatcher for GFLIB_Tan_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_TAN_DEFAULT_F32   &f32gflibTanCoef     /**< Default approximation coefficients for GFLIB_Tan_F32 function. */
  
  /************************************************************************/
  /**
  @struct GFLIB_TAN_TAYLOR_COEF_T_F32 "\GFLIB_Tan.h"

  @brief  Structure containing four polynomial coefficients for one sub-interval.

  @details  Output of tan(\f$\Pi\f$ * f32In) for interval [0, \f$\Pi\f$/4) of
            the input angles is divided into eight sub-sectors. Polynomial approximation is done using a 4th order
            polynomial, for each sub-sector respectively. Four coefficients for a single sub-interval are stored in
            this (#GFLIB_TAN_TAYLOR_COEF_T_F32) structure.
  *//**********************************************************************/
  typedef struct{
    const tFrac32  f32A[4];
  }GFLIB_TAN_TAYLOR_COEF_T_F32;

  /************************************************************************/
  /**
  @struct GFLIB_TAN_T_F32 "\GFLIB_Tan.h" 

  @brief    Output of tan(PI * f32In) for interval [0, PI/4) of
            the input angles is divided into eight sub-sectors. Polynomial approximation is done using a 4th order
            polynomial, for each sub-sector respectively. Eight arrays, each including four polynomial coefficients for
            each sub-interval, are stored in this (#GFLIB_TAN_T_F32) structure.
  *//**********************************************************************/
  typedef struct{
    GFLIB_TAN_TAYLOR_COEF_T_F32  GFLIB_TAN_SECTOR[8];
  }GFLIB_TAN_T_F32;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for tangent approximation.
  *//**********************************************************************/

  extern const GFLIB_TAN_T_F32 f32gflibTanCoef;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac32 GFLIB_Tan_F32(tFrac32 f32In, const GFLIB_TAN_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_TAN_DEFAULT_F16   &f16gflibTanCoef     /**< Default approximation coefficients for GFLIB_Tan_F16 function. */

  /************************************************************************/
  /**
  @struct GFLIB_TAN_TAYLOR_COEF_T_F16 "\GFLIB_Tan.h"

  @brief  Structure containing four polynomial coefficients for one sub-interval.

  @details  Output of tan(\f$\Pi\f$ * f16In) for interval [0, \f$\Pi\f$/4) of
            the input angles is divided into eight sub-sectors. Polynomial approximation is done using a 4th order
            polynomial, for each sub-sector respectively. Four coefficients for a single sub-interval are stored in
            this (#GFLIB_TAN_TAYLOR_COEF_T_F16) structure.
  *//**********************************************************************/
  typedef struct{
    const tFrac16  f16A[4];
  }GFLIB_TAN_TAYLOR_COEF_T_F16;

  /************************************************************************/
  /**
  @struct GFLIB_TAN_T_F16 "\GFLIB_Tan.h"

  @brief    Output of tan(PI * f16In) for interval [0, PI/4) of
            the input angles is divided into eight sub-sectors. Polynomial approximation is done using a 4th order
            polynomial, for each sub-sector respectively. Eight arrays, each including four polynomial coefficients for
            each sub-interval, are stored in this (#GFLIB_TAN_T_F16) structure.
  *//**********************************************************************/
  typedef struct{
    GFLIB_TAN_TAYLOR_COEF_T_F16  GFLIB_TAN_SECTOR[8];
  }GFLIB_TAN_T_F16;
  
  /************************************************************************/
  /**
  @brief  Default approximation coefficients for tangent approximation.
  *//**********************************************************************/

  extern const GFLIB_TAN_T_F16 f16gflibTanCoef;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac16 GFLIB_Tan_F16(tFrac16 f16In, const GFLIB_TAN_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Tan_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_TAN_DEFAULT_FLT   &fltgflibTanCoef     /**< Default approximation coefficients for GFLIB_Tan_FLT function. */

  /************************************************************************/
  /**
  @struct GFLIB_TAN_T_FLT "\GFLIB_Tan.h" 

  @brief  Polynomial coefficient for fractional approximation in single precision floating point format.
  *//**********************************************************************/
  typedef struct{
    tFloat  fltA[4];
  }GFLIB_TAN_T_FLT;
  
  /************************************************************************/
  /**
  @brief  Default approximation coefficients for tangent approximation.
  *//**********************************************************************/

  extern const GFLIB_TAN_T_FLT fltgflibTanCoef;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFloat GFLIB_Tan_FLT(tFloat fltIn, const GFLIB_TAN_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_TAN_H */
