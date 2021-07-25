/******************************************************************************
*
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
* @file       GFLIB_SinCos.h
*
* @version    1.0.5.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_SinCos function
*
******************************************************************************/
#ifndef GFLIB_SINCOS_H
#define GFLIB_SINCOS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_SinCos_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_SinCos_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_SinCos_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_SinCos_h_REF_4
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
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GFLIB_SinCos(...)     macro_dispatcher(GFLIB_SinCos, __VA_ARGS__)(__VA_ARGS__)

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_T                        GFLIB_SINCOS_T_F32                           /**< Definition of GFLIB_SINCOS_T as alias for GFLIB_SINCOS_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_DEFAULT                  GFLIB_SINCOS_DEFAULT_F32                     /**< Definition of GFLIB_SINCOS_DEFAULT  as alias for GFLIB_SINCOS_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_SinCos_Dsptchr_3(In,CosOut,pParam)     GFLIB_SinCos_Dsptchr_4(In,CosOut,pParam,F32)        /* Function dispatcher for GFLIB_SinCos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00335 */
    #define GFLIB_SinCos_Dsptchr_2(In,CosOut)            GFLIB_SinCos_Dsptchr_3(In,CosOut,GFLIB_SINCOS_DEFAULT) /* Function dispatcher for GFLIB_SinCos_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_T                        GFLIB_SINCOS_T_F16                           /**< Definition of GFLIB_SINCOS_T as alias for GFLIB_SINCOS_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_DEFAULT                  GFLIB_SINCOS_DEFAULT_F16                     /**< Definition of GFLIB_SINCOS_DEFAULT  as alias for GFLIB_SINCOS_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_SinCos_Dsptchr_3(In,CosOut,pParam)     GFLIB_SinCos_Dsptchr_4(In,CosOut,pParam,F16)        /* Function dispatcher for GFLIB_SinCos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00335 */
    #define GFLIB_SinCos_Dsptchr_2(In,CosOut)            GFLIB_SinCos_Dsptchr_3(In,CosOut,GFLIB_SINCOS_DEFAULT) /* Function dispatcher for GFLIB_SinCos_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_T                        GFLIB_SINCOS_T_FLT                           /**< Definition of GFLIB_SINCOS_T as alias for GFLIB_SINCOS_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_SINCOS_DEFAULT                  GFLIB_SINCOS_DEFAULT_FLT                     /**< Definition of GFLIB_SINCOS_DEFAULT  as alias for GFLIB_SINCOS_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_SinCos_Dsptchr_3(In,CosOut,pParam)     GFLIB_SinCos_Dsptchr_4(In,CosOut,pParam,FLT)        /* Function dispatcher for GFLIB_SinCos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00335 */
    #define GFLIB_SinCos_Dsptchr_2(In,CosOut)            GFLIB_SinCos_Dsptchr_3(In,CosOut,GFLIB_SINCOS_DEFAULT) /* Function dispatcher for GFLIB_SinCos_Dsptchr_1, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  */
  #define GFLIB_SinCos_Dsptchr_4(In,CosOut,pParam,Impl)    GFLIB_SinCos_Dsptchr_(In,CosOut,pParam,Impl)     /* Function dispatcher for GFLIB_SinCos_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  * @violates @ref GFLIB_SinCos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_SinCos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter 
  * in definition of macro. 
  * @violates @ref GFLIB_SinCos_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGFLIB00337 */
  #define GFLIB_SinCos_Dsptchr_(In,CosOut,pParam,Impl)     GFLIB_SinCos_##Impl(In,CosOut,pParam)            /* Function dispatcher for GFLIB_SinCos_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_SINCOS_DEFAULT_F32  &f32gflibSinCosCoef     /**< Default approximation coefficients for GFLIB_SinCos_F32 function. */
  
  /************************************************************************/
  /**
  @struct GFLIB_SINCOS_T_F32 "\GFLIB_SinCos.h"

  @brief  Array of five 32-bit elements for storing coefficients of the Taylor polynomial.
  *//**********************************************************************/
  typedef struct{
    tFrac32  f32A[5];
  }GFLIB_SINCOS_T_F32;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for Sine and Cosine approximation.
  *//**********************************************************************/

  extern const GFLIB_SINCOS_T_F32 f32gflibSinCosCoef;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_SinCos_F32(tFrac32 f32In, SWLIBS_2Syst_F32 *pOut, const GFLIB_SINCOS_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_SINCOS_DEFAULT_F16  &f16gflibSinCosCoef     /**< Default approximation coefficients for GFLIB_SinCos_F16 function. */
  
  /************************************************************************/
  /**
  @struct GFLIB_SINCOS_T_F16 "\GFLIB_SinCos.h"

  @brief  Array of four 16-bit elements for storing coefficients of the Taylor polynomial.
  *//**********************************************************************/
  typedef struct{
    tFrac16  f16A[4];
  }GFLIB_SINCOS_T_F16;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for Sine and Cosine approximation.
  *//**********************************************************************/

  extern const GFLIB_SINCOS_T_F16 f16gflibSinCosCoef;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_SinCos_F16(tFrac16 f16In, SWLIBS_2Syst_F16 *pOut, const GFLIB_SINCOS_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_SinCos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
  */
  #define GFLIB_SINCOS_DEFAULT_FLT  &fltgflibSinCosCoef     /**< Default approximation coefficients for GFLIB_SinCos_FLT function. */

  /************************************************************************/
  /**
  @struct GFLIB_SINCOS_T_FLT "\GFLIB_SinCos.h"

  @brief  Array of three single precision floating point elements for storing coefficients of the floating point optimized
          minimax approximation polynomial.
  *//**********************************************************************/
  typedef struct{
    tFloat  fltA[3];
  }GFLIB_SINCOS_T_FLT;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for Sine and Cosine approximation.
  *//**********************************************************************/

  extern const GFLIB_SINCOS_T_FLT fltgflibSinCosCoef;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GFLIB_SinCos_FLT(tFloat fltIn, SWLIBS_2Syst_FLT *pOut, const GFLIB_SINCOS_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_SINCOS_H */
