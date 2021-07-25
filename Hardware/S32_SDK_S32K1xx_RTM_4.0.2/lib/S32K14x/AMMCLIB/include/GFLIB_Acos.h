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
* @file       GFLIB_Acos.h
*
* @version    1.0.8.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_Acos function
*
******************************************************************************/
#ifndef GFLIB_ACOS_H
#define GFLIB_ACOS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_Acos_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_Acos_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_Acos_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_Acos_h_REF_4
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
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Acos(...)     macro_dispatcher(GFLIB_Acos, __VA_ARGS__)(__VA_ARGS__)     /* This function implements polynomial approximation of arccosine function. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_T                        GFLIB_ACOS_T_F32                            /**< Definition of GFLIB_ACOS_T as alias for GFLIB_ACOS_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_DEFAULT                  GFLIB_ACOS_DEFAULT_F32                      /**< Definition of GFLIB_ACOS_DEFAULT as alias for GFLIB_ACOS_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Acos_Dsptchr_2(In,pParam)     GFLIB_Acos_Dsptchr_3(In,pParam,F32)         /* Function dispatcher for GFLIB_Acos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00061 */
    #define GFLIB_Acos_Dsptchr_1(In)            GFLIB_Acos_Dsptchr_2(In,GFLIB_ACOS_DEFAULT) /* Function dispatcher for GFLIB_Acos_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_T                        GFLIB_ACOS_T_F16                            /**< Definition of GFLIB_ACOS_T as alias for GFLIB_ACOS_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_DEFAULT                  GFLIB_ACOS_DEFAULT_F16                      /**< Definition of GFLIB_ACOS_DEFAULT as alias for GFLIB_ACOS_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Acos_Dsptchr_2(In,pParam)     GFLIB_Acos_Dsptchr_3(In,pParam,F16)         /* Function dispatcher for GFLIB_Acos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00061 */
    #define GFLIB_Acos_Dsptchr_1(In)            GFLIB_Acos_Dsptchr_2(In,GFLIB_ACOS_DEFAULT) /* Function dispatcher for GFLIB_Acos_Dsptchr_1, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_T                        GFLIB_ACOS_T_FLT                            /**< Definition of GFLIB_ACOS_T as alias for GFLIB_ACOS_T_FLT datatype in case the Single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define GFLIB_ACOS_DEFAULT                  GFLIB_ACOS_DEFAULT_FLT                      /**< Definition of GFLIB_ACOS_DEFAULT as alias for GFLIB_ACOS_DEFAULT_FLT default value in case the Single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    #define GFLIB_Acos_Dsptchr_2(In,pParam)     GFLIB_Acos_Dsptchr_3(In,pParam,FLT)         /* Function dispatcher for GFLIB_Acos_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00061 */
    #define GFLIB_Acos_Dsptchr_1(In)            GFLIB_Acos_Dsptchr_2(In,GFLIB_ACOS_DEFAULT) /* Function dispatcher for GFLIB_Acos_Dsptchr_1, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_Acos_Dsptchr_3(In,pParam,Impl)    GFLIB_Acos_Dsptchr_(In,pParam,Impl)         /* Function dispatcher for GFLIB_Acos_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_Acos_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_Acos_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_Acos_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DGFLIB00063 */
  #define GFLIB_Acos_Dsptchr_(In,pParam,Impl)     GFLIB_Acos_##Impl(In,pParam)                /* Function dispatcher for GFLIB_Acos_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  */
  #define GFLIB_ACOS_DEFAULT_F32 &f32gflibAcosCoef     /**< Default approximation coefficients for GFLIB_Acos_F32 function. */

  /************************************************************************/
  /**
  @struct GFLIB_ACOS_TAYLOR_COEF_T_F32 "\GFLIB_Acos.h"

  @brief  Array of approximation coefficients for piece-wise polynomial.
  
  @details  Output of arccos(f32In) for interval [0, 1) of the input ratio is divided
            into three sub-sectors. Polynomial approximation is done using a 5th order polynomial, for each sub-sector
            respectively. Five coefficients for a single sub-interval are stored in this
            (#GFLIB_ACOS_TAYLOR_COEF_T_F32) structure.
  *//**********************************************************************/
  typedef struct{
    const tFrac32  f32A[5];  /**< Array of five 32-bit elements for storing coefficients of the piece-wise polynomial. */
  }GFLIB_ACOS_TAYLOR_COEF_T_F32;

  /************************************************************************/
  /**
  @struct GFLIB_ACOS_T_F32 "\GFLIB_Acos.h"

  @brief  Default approximation coefficients datatype for arccosine approximation.
  
  @details  Output of arccos(f32In) for interval [0, 1) of the input ratio is divided
            into three sub-sectors. Polynomial approximation is done using a 5th order polynomial, for each sub-sector
            respectively. Five coefficients for a single sub-interval are stored in #GFLIB_ACOS_TAYLOR_COEF_T_F32 structure.
  *//**********************************************************************/
  typedef struct{
    GFLIB_ACOS_TAYLOR_COEF_T_F32  GFLIB_ACOS_SECTOR[3]; /**< Array of two elements for storing three sub-arrays (each sub-array
                                                             contains five 32-bit coefficients) for all sub-intervals.*/
  }GFLIB_ACOS_T_F32;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for arccosine approximation.
  *//**********************************************************************/
  
  extern const GFLIB_ACOS_T_F32 f32gflibAcosCoef;
  
  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern tFrac32 GFLIB_Acos_F32(tFrac32 f32In, const GFLIB_ACOS_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  */
  #define GFLIB_ACOS_DEFAULT_F16 &f16gflibAcosCoef     /**< Default approximation coefficients for GFLIB_Acos_F16 function. */

  /************************************************************************/
  /**
  @struct GFLIB_ACOS_TAYLOR_COEF_T_F16 "\GFLIB_Acos.h"

  @brief  Array of approximation coefficients for piece-wise polynomial.

  @details  Output of arccos(f16In) for interval [0, 1) of the input ratio is divided
            into two sub-sectors. Polynomial approximation is done using a 5th order polynomial, for each sub-sector
            respectively. Five coefficients for a single sub-interval are stored in this
            (#GFLIB_ACOS_TAYLOR_COEF_T_F16) structure.
  *//**********************************************************************/
  typedef struct{
    const tFrac16  f16A[5];  /**< Array of five 16-bit elements for storing coefficients of the piece-wise polynomial. */
  }GFLIB_ACOS_TAYLOR_COEF_T_F16;

  /************************************************************************/
  /**
  @struct GFLIB_ACOS_T_F16 "\GFLIB_Acos.h"

  @brief  Default approximation coefficients datatype for arccosine approximation.

  @details  Output of arccos(f16In) for interval [0, 1) of the input ratio is divided
            into two sub-sectors. Polynomial approximation is done using a 5th order polynomial, for each sub-sector
            respectively. Five coefficients for a single sub-interval are stored in #GFLIB_ACOS_TAYLOR_COEF_T_F16 structure.
  *//**********************************************************************/
  typedef struct{
    GFLIB_ACOS_TAYLOR_COEF_T_F16  GFLIB_ACOS_SECTOR[2]; /**< Array of two elements for storing two sub-arrays (each sub-array
                                                             contains five 16-bit coefficients) for all sub-intervals.*/
  }GFLIB_ACOS_T_F16;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for arccosine approximation.
  *//**********************************************************************/

  extern const GFLIB_ACOS_T_F16 f16gflibAcosCoef;
  
  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern tFrac16 GFLIB_Acos_F16(tFrac16 f16In, const GFLIB_ACOS_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GFLIB_Acos_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  */
  #define GFLIB_ACOS_DEFAULT_FLT &fltgflibAcosCoef     /**< Default approximation coefficients for GFLIB_Acos_FLT function. */

  /************************************************************************/
  /**
  @struct GFLIB_ACOS_T_FLT "\GFLIB_Acos.h" 

  @brief  Default approximation coefficients datatype for arccosine approximation.

  @details  The polynomial approximation is done using the square root function of the input parameter. The essential
            assumption of this approach is a fast H/W based calculation of the square root operation. The approximation
            polynomial coefficients assume an input argument in the [-\f$\Pi\f$, \f$\Pi\f$] interval.
  *//**********************************************************************/
  typedef struct{
    const tFloat  fltA[5]; /**< Array of approximation coefficients. */
  }GFLIB_ACOS_T_FLT;

  /************************************************************************/
  /**
  @brief  Default approximation coefficients for arccosine approximation.
  *//**********************************************************************/
  
  extern const GFLIB_ACOS_T_FLT fltgflibAcosCoef;

  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/ 
  extern tFloat GFLIB_Acos_FLT(tFloat fltIn, const GFLIB_ACOS_T_FLT *const pParam);
  
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_ACOS_H */
