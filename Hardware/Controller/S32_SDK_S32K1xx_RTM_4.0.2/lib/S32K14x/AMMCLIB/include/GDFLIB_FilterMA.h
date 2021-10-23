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
* @file     GDFLIB_FilterMA.h
*
* @version  1.0.11.0
*
* @date     17-April-2019
*
* @brief    Header file for GDFLIB_FilterMA function
*
******************************************************************************/
#ifndef GDFLIB_FILTERMA_H
#define GDFLIB_FILTERMA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GDFLIB_FilterMA_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GDFLIB_FilterMA_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GDFLIB_FilterMA_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GDFLIB_FilterMA_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GDFLIB_FilterMA_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section GDFLIB_FilterMA_h_REF_6
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
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterMAInit(...)     macro_dispatcher(GDFLIB_FilterMAInit, __VA_ARGS__)(__VA_ARGS__)     /* This function clears the internal filter accumulator. */
  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterMASetState(...)     macro_dispatcher(GDFLIB_FilterMASetState, __VA_ARGS__)(__VA_ARGS__)     /* This function sets the internal filter accumulator. */
  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterMA(...)         macro_dispatcher(GDFLIB_FilterMA, __VA_ARGS__)(__VA_ARGS__)         /* This function implements a moving average recursive filter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_T                        GDFLIB_FILTER_MA_T_F32                        /**< Definition of GDFLIB_FILTER_MA_T as alias for GDFLIB_FILTER_MA_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_DEFAULT                  GDFLIB_FILTER_MA_DEFAULT_F32                  /**< Definition of GDFLIB_FILTER_MA_DEFAULT as alias for GDFLIB_FILTER_MA_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00122 */
    #define GDFLIB_FilterMAInit_Dsptchr_1(pParam)     GDFLIB_FilterMAInit_Dsptchr_2(pParam,F32)     /* Function dispatcher for GDFLIB_FilterMAInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGDFLIB00134 */
    #define GDFLIB_FilterMASetState_Dsptchr_2(FilterMAOut,pParam)     GDFLIB_FilterMASetState_Dsptchr_3(FilterMAOut,pParam,F32)     /* Function dispatcher for GDFLIB_FilterMASetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00108 */
    #define GDFLIB_FilterMA_Dsptchr_2(In,pParam)      GDFLIB_FilterMA_Dsptchr_3(In,pParam,F32)      /* Function dispatcher for GDFLIB_FilterMA_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_T                        GDFLIB_FILTER_MA_T_F16                        /**< Definition of GDFLIB_FILTER_MA_T as alias for GDFLIB_FILTER_MA_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_DEFAULT                  GDFLIB_FILTER_MA_DEFAULT_F16                  /**< Definition of GDFLIB_FILTER_MA_DEFAULT as alias for GDFLIB_FILTER_MA_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00122 */
    #define GDFLIB_FilterMAInit_Dsptchr_1(pParam)     GDFLIB_FilterMAInit_Dsptchr_2(pParam,F16)     /* Function dispatcher for GDFLIB_FilterMAInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGDFLIB00134 */
    #define GDFLIB_FilterMASetState_Dsptchr_2(FilterMAOut,pParam)     GDFLIB_FilterMASetState_Dsptchr_3(FilterMAOut,pParam,F16)     /* Function dispatcher for GDFLIB_FilterMASetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00108 */
    #define GDFLIB_FilterMA_Dsptchr_2(In,pParam)      GDFLIB_FilterMA_Dsptchr_3(In,pParam,F16)      /* Function dispatcher for GDFLIB_FilterMA_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_T                        GDFLIB_FILTER_MA_T_FLT                        /**< Definition of GDFLIB_FILTER_MA_T as alias for GDFLIB_FILTER_MA_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTER_MA_DEFAULT                  GDFLIB_FILTER_MA_DEFAULT_FLT                  /**< Definition of GDFLIB_FILTER_MA_DEFAULT as alias for GDFLIB_FILTER_MA_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00122 */
    #define GDFLIB_FilterMAInit_Dsptchr_1(pParam)     GDFLIB_FilterMAInit_Dsptchr_2(pParam,FLT)     /* Function dispatcher for GDFLIB_FilterMAInit_Dsptchr_1, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
    * characters. 
    */
    /* @remarks Implements DGDFLIB00134 */
    #define GDFLIB_FilterMASetState_Dsptchr_2(FilterMAOut,pParam)     GDFLIB_FilterMASetState_Dsptchr_3(FilterMAOut,pParam,FLT)     /* Function dispatcher for GDFLIB_FilterMASetState_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00108 */
    #define GDFLIB_FilterMA_Dsptchr_2(In,pParam)      GDFLIB_FilterMA_Dsptchr_3(In,pParam,FLT)      /* Function dispatcher for GDFLIB_FilterMA_Dsptchr_2, do not modify!!! */
  #endif

  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterMAInit_Dsptchr_2(pParam,Impl)    GDFLIB_FilterMAInit_Dsptchr_(pParam,Impl)     /* Function dispatcher for GDFLIB_FilterMAInit_Dsptchr_2, do not modify!!! */
  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGDFLIB00129 */
  #define GDFLIB_FilterMAInit_Dsptchr_(pParam,Impl)     GDFLIB_FilterMAInit_##Impl(pParam)            /* Function dispatcher for GDFLIB_FilterMAInit_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GDFLIB_FilterMA_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GDFLIB_FilterMASetState_Dsptchr_3(FilterMAOut,pParam,Impl)    GDFLIB_FilterMASetState_Dsptchr_(FilterMAOut,pParam,Impl)     /* Function dispatcher for GDFLIB_FilterMASetState_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GDFLIB_FilterMA_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  /* @remarks Implements DGDFLIB00136 */
  #define GDFLIB_FilterMASetState_Dsptchr_(FilterMAOut,pParam,Impl)     GDFLIB_FilterMASetState_##Impl(FilterMAOut,pParam)            /* Function dispatcher for GDFLIB_FilterMASetState_Dsptchr_, do not modify!!! */

  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterMA_Dsptchr_3(In,pParam,Impl)     GDFLIB_FilterMA_Dsptchr_(In,pParam,Impl)      /* Function dispatcher for GDFLIB_FilterMA_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref GDFLIB_FilterMA_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterMA_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GDFLIB_FilterMA_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGDFLIB00116 */
  #define GDFLIB_FilterMA_Dsptchr_(In,pParam,Impl)      GDFLIB_FilterMA_##Impl(In,pParam)             /* Function dispatcher for GDFLIB_FilterMA_Dsptchr_, do not modify!!! */
#endif

/******************************************************************************
* Typedefs and structures       (scope: module-local)
******************************************************************************/




/******************************************************************************
* Implementation variant: 32-bit fractional
******************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  #define GDFLIB_FILTER_MA_DEFAULT_F32   {0,0}  /**< Default value for GDFLIB_FILTER_MA_T_F32. */

  /************************************************************************/
  /**
  @struct GDFLIB_FILTER_MA_T_F32 "\GDFLIB_FilterMA.h"

  @brief  Structure containing filter buffer and coefficients.
  *//**********************************************************************/
  /* @remarks Implements DGDFLIB00127, DGDFLIB00114, DGDFLIB00110, DGDFLIB00113, DGDFLIB00124 */
  typedef struct{
    tFrac32   f32Acc;        /**< State variable - filter accumulator. */
    tU16      u16NSamples;   /**< Recalculated smoothing factor [0, 31].*/
  }GDFLIB_FILTER_MA_T_F32;

  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern void GDFLIB_FilterMAInit_F32(GDFLIB_FILTER_MA_T_F32 *pParam);
  extern void GDFLIB_FilterMASetState_F32(tFrac32 f32FilterMAOut, GDFLIB_FILTER_MA_T_F32 *pParam);
  extern tFrac32 GDFLIB_FilterMA_F32(tFrac32 f32In, GDFLIB_FILTER_MA_T_F32 *pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/******************************************************************************
* Implementation variant: 16-bit fractional
******************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  #define GDFLIB_FILTER_MA_DEFAULT_F16   {0,0}  /**< Default value for GDFLIB_FILTER_MA_T_F16. */

  /************************************************************************/
  /**
  @struct GDFLIB_FILTER_MA_T_F16 "\GDFLIB_FilterMA.h"

  @brief  Structure containing filter buffer and coefficients.
  *//**********************************************************************/
  /* @remarks Implements DGDFLIB00127, DGDFLIB00114, DGDFLIB00111, DGDFLIB00113, DGDFLIB00125 */
  typedef struct{
    tFrac32   f32Acc;         /**< State variable - filter accumulator. */
    tU16      u16NSamples;    /**< Recalculated smoothing factor [0, 15].*/
  }GDFLIB_FILTER_MA_T_F16;

  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern void GDFLIB_FilterMAInit_F16(GDFLIB_FILTER_MA_T_F16 *pParam);
  extern void GDFLIB_FilterMASetState_F16(tFrac16 f16FilterMAOut, GDFLIB_FILTER_MA_T_F16 *pParam);
  extern tFrac16 GDFLIB_FilterMA_F16(tFrac16 f16In, GDFLIB_FILTER_MA_T_F16 *pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/******************************************************************************
* Implementation variant: Single precision floating point
******************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  #define GDFLIB_FILTER_MA_DEFAULT_FLT   {0,0} /**< Default value for GDFLIB_FILTER_MA_T_FLT. */

  /************************************************************************/
  /**
  @struct GDFLIB_FILTER_MA_T_FLT "\GDFLIB_FilterMA.h"

  @brief  Structure containing filter buffer and coefficients.
  *//**********************************************************************/
  /* @remarks Implements DGDFLIB00127, DGDFLIB00114, DGDFLIB00112, DGDFLIB00113, DGDFLIB00126 */
  typedef struct{
    tFloat fltAcc;           /**< State variable - filter accumulator. */
    tFloat fltLambda;         /**< Smoothing factor [0, 1]. */
  }GDFLIB_FILTER_MA_T_FLT;

  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern void GDFLIB_FilterMAInit_FLT(GDFLIB_FILTER_MA_T_FLT *pParam);
  extern void GDFLIB_FilterMASetState_FLT(tFloat fltFilterMAOut, GDFLIB_FILTER_MA_T_FLT *pParam);
  extern tFloat GDFLIB_FilterMA_FLT(tFloat fltIn, GDFLIB_FILTER_MA_T_FLT *pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/******************************************************************************
* Inline functions
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GDFLIB_FILTERMA_H */
