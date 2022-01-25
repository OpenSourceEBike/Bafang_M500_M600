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
* @file       GDFLIB_FilterFIR.h
*
* @version    1.0.8.0
*
* @date       17-April-2019
*
* @brief      Header file for GDFLIB_FilterFIR function
*
******************************************************************************/
#ifndef GDFLIB_FILTERFIR_H
#define GDFLIB_FILTERFIR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GDFLIB_FilterFIR_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GDFLIB_FilterFIR_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GDFLIB_FilterFIR_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GDFLIB_FilterFIR_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GDFLIB_FilterFIR_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Inline function is necessary for efficient implementation of function wrapper for
* use by Embedded Coder.
*/
#include "SWLIBS_Defines.h"
#include "mlib.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterFIRInit(...)     macro_dispatcher(GDFLIB_FilterFIRInit, __VA_ARGS__)(__VA_ARGS__)     /* This function initializes the FIR filter buffers. */
  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterFIR(...)         macro_dispatcher(GDFLIB_FilterFIR, __VA_ARGS__)(__VA_ARGS__)         /* The function performs a single iteration of an FIR filter. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_PARAM_T                                 GDFLIB_FILTERFIR_PARAM_T_F32                                 /**< Definition of alias for GDFLIB_FILTERFIR_PARAM_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_STATE_T                                 GDFLIB_FILTERFIR_STATE_T_F32                                 /**< Definition of alias for GDFLIB_FILTERFIR_STATE_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00091 */
    #define GDFLIB_FilterFIRInit_Dsptchr_3(pParam,pState,pInBuf)     GDFLIB_FilterFIRInit_Dsptchr_4(pParam,pState,pInBuf,F32)     /* Function dispatcher for GDFLIB_FilterFIRInit_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00077 */
    #define GDFLIB_FilterFIR_Dsptchr_3(In,pParam,pState)             GDFLIB_FilterFIR_Dsptchr_4(In,pParam,pState,F32)             /* Function dispatcher for GDFLIB_FilterFIR_Dsptchr_3, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_PARAM_T                                 GDFLIB_FILTERFIR_PARAM_T_F16                                 /**< Definition of alias for GDFLIB_FILTERFIR_PARAM_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_STATE_T                                 GDFLIB_FILTERFIR_STATE_T_F16                                 /**< Definition of alias for GDFLIB_FILTERFIR_STATE_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00091 */
    #define GDFLIB_FilterFIRInit_Dsptchr_3(pParam,pState,pInBuf)     GDFLIB_FilterFIRInit_Dsptchr_4(pParam,pState,pInBuf,F16)     /* Function dispatcher for GDFLIB_FilterFIRInit_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00077 */
    #define GDFLIB_FilterFIR_Dsptchr_3(In,pParam,pState)             GDFLIB_FilterFIR_Dsptchr_4(In,pParam,pState,F16)             /* Function dispatcher for GDFLIB_FilterFIR_Dsptchr_3, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_PARAM_T                                 GDFLIB_FILTERFIR_PARAM_T_FLT                                 /**< Definition of alias for GDFLIB_FILTERFIR_PARAM_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    */
    #define GDFLIB_FILTERFIR_STATE_T                                 GDFLIB_FILTERFIR_STATE_T_FLT                                 /**< Definition of alias for GDFLIB_FILTERFIR_STATE_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00091 */
    #define GDFLIB_FilterFIRInit_Dsptchr_3(pParam,pState,pInBuf)     GDFLIB_FilterFIRInit_Dsptchr_4(pParam,pState,pInBuf,FLT)     /* Function dispatcher for GDFLIB_FilterFIRInit_Dsptchr_3, do not modify!!! */
    /*
    * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
    * for macro. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGDFLIB00077 */
    #define GDFLIB_FilterFIR_Dsptchr_3(In,pParam,pState)             GDFLIB_FilterFIR_Dsptchr_4(In,pParam,pState,FLT)             /* Function dispatcher for GDFLIB_FilterFIR_Dsptchr_3, do not modify!!! */
  #endif 

  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterFIRInit_Dsptchr_4(pParam,pState,pInBuf,Impl)    GDFLIB_FilterFIRInit_Dsptchr_(pParam,pState,pInBuf,Impl)     /* Function dispatcher for GDFLIB_FilterFIRInit_Dsptchr_4, do not modify!!! */
  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGDFLIB00098 */
  #define GDFLIB_FilterFIRInit_Dsptchr_(pParam,pState,pInBuf,Impl)     GDFLIB_FilterFIRInit_##Impl(pParam,pState,pInBuf)            /* Function dispatcher for GDFLIB_FilterFIRInit_Dsptchr_, do not modify!!! */  

  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GDFLIB_FilterFIR_Dsptchr_4(In,pParam,pState,Impl)            GDFLIB_FilterFIR_Dsptchr_(In,pParam,pState,Impl)             /* Function dispatcher for GDFLIB_FilterFIR_Dsptchr_4, do not modify!!! */
  /*
  * @violates @ref GDFLIB_FilterFIR_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GDFLIB_FilterFIR_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGDFLIB00085 */
  #define GDFLIB_FilterFIR_Dsptchr_(In,pParam,pState,Impl)             GDFLIB_FilterFIR_##Impl(In,pParam,pState)                    /* Function dispatcher for GDFLIB_FilterFIR_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_PARAM_T_F32 "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing parameters of the filter.
  *//***********************************************************************/  
  /* @remarks Implements DGDFLIB00082 */
  typedef struct{
    tU32           u32Order;         /**< FIR filter order, must be in the interval [1, 32767]. */
    const tFrac32 *pCoefBuf;         /**< FIR filter coefficients buffer. The array stores (u32Order + 1) filter coefficients. */
  }GDFLIB_FILTERFIR_PARAM_T_F32;

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_STATE_T_F32 "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing the current state of the filter.
  *//***********************************************************************/
  /* @remarks Implements DGDFLIB00096, DGDFLIB00083, DGDFLIB00079, DGDFLIB00093*/
  typedef struct{
    tU32     u32Idx;                 /**< Input buffer index. */
    tFrac32 *pInBuf;                 /**< Pointer to the input buffer. The array stores (u32Order + 1) samples. */
  }GDFLIB_FILTERFIR_STATE_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GDFLIB_FilterFIRInit_F32(const GDFLIB_FILTERFIR_PARAM_T_F32 *const pParam, GDFLIB_FILTERFIR_STATE_T_F32 *const pState, tFrac32 *pInBuf);
  extern tFrac32 GDFLIB_FilterFIR_F32(tFrac32 f32In, const GDFLIB_FILTERFIR_PARAM_T_F32 *const pParam,GDFLIB_FILTERFIR_STATE_T_F32 *const pState);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_PARAM_T_F16 "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing parameters of the filter.
  *//***********************************************************************/  
  /* @remarks Implements DGDFLIB00082 */
  typedef struct{
    tU16           u16Order;         /**< FIR filter order, must be in the interval [1, 32767]. */
    const tFrac16 *pCoefBuf;         /**< FIR filter coefficients buffer. The array stores (u16Order + 1) filter coefficients. The array must be aligned to a 32-bit boundary and there must be 2 readable bytes after the last element of the array. */
  }GDFLIB_FILTERFIR_PARAM_T_F16;

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_STATE_T_F16 "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing the current state of the filter.
  *//***********************************************************************/
  /* @remarks Implements DGDFLIB00096, DGDFLIB00083, DGDFLIB00080, DGDFLIB00094*/
  typedef struct{
    tU16     u16Idx;                 /**< Input buffer index. */
    tFrac16 *pInBuf;                 /**< Pointer to the input buffer. The array stores (u16Order + 1) samples. The array must be aligned to a 32-bit boundary and there must be 2 readable bytes after the last element of the array. */
  }GDFLIB_FILTERFIR_STATE_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GDFLIB_FilterFIRInit_F16(const GDFLIB_FILTERFIR_PARAM_T_F16 *const pParam, GDFLIB_FILTERFIR_STATE_T_F16 *const pState, tFrac16 *pInBuf);
  extern tFrac16 GDFLIB_FilterFIR_F16(tFrac16 f16In, const GDFLIB_FILTERFIR_PARAM_T_F16 *const pParam,GDFLIB_FILTERFIR_STATE_T_F16 *const pState);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_PARAM_T_FLT "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing parameters of the filter.
  *//***********************************************************************/
  /* @remarks Implements DGDFLIB00082 */
  typedef struct{
    tU32          u32Order;          /**< FIR filter order, must be in the interval [1, 32767]. */
    const tFloat *pCoefBuf;          /**< FIR filter coefficients buffer. The array stores (u32Order + 1) filter coefficients. */
  }GDFLIB_FILTERFIR_PARAM_T_FLT;

  /*************************************************************************/
  /**
  @struct GDFLIB_FILTERFIR_STATE_T_FLT "\GDFLIB_FilterFIR.h" 

  @brief  Structure containing the current state of the filter.
  *//***********************************************************************/
  /* @remarks Implements DGDFLIB00096, DGDFLIB00083, DGDFLIB00081, DGDFLIB00095*/
  typedef struct{
    tU32    u32Idx;                  /**< Input buffer index. */
    tFloat *pInBuf;                  /**< Pointer to the input buffer. The array stores (u32Order + 1) samples. */
  }GDFLIB_FILTERFIR_STATE_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GDFLIB_FilterFIRInit_FLT(const GDFLIB_FILTERFIR_PARAM_T_FLT *const pParam, GDFLIB_FILTERFIR_STATE_T_FLT *const pState, tFloat *pInBuf);
  extern tFloat GDFLIB_FilterFIR_FLT(tFloat fltIn, const GDFLIB_FILTERFIR_PARAM_T_FLT *const pParam,GDFLIB_FILTERFIR_STATE_T_FLT *const pState);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GDFLIB_FILTERFIR_H */
