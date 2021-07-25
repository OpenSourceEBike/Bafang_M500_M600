/******************************************************************************
*
*   Copyright 2019 NXP
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
* @file       GMCLIB_BetaProjection3Ph.h
*
* @version    1.0.0.0
*
* @date       23-August-2019
*
* @brief      Header file for GSWLIBS_BetaProjection3Ph function
*
******************************************************************************/
#ifndef GMCLIB_BETAPROJECTION3PH_H
#define GMCLIB_BETAPROJECTION3PH_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section GMCLIB_BetaProjection3Ph_h_REF_6
* Violates MISRA 2004 Required Rule 1.4, Identifier clash.
* The compiler can distinguish names with more than 31 characters.
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
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GMCLIB_BetaProjection3Ph(...)     macro_dispatcher(GMCLIB_BetaProjection3Ph, __VA_ARGS__)(__VA_ARGS__)     /* This function implements the Beta Projection transformation. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
    */
    /* @remarks Implements DAMMCLIB00011 */
    #define GMCLIB_BetaProjection3Ph_Dsptchr_2(pOut,pIn)     GMCLIB_BetaProjection3Ph_Dsptchr_3(pOut,pIn,F32)     /* Function dispatcher for GMCLIB_BetaProjection3Ph_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
    */
    /* @remarks Implements DAMMCLIB00011 */
    #define GMCLIB_BetaProjection3Ph_Dsptchr_2(pOut,pIn)     GMCLIB_BetaProjection3Ph_Dsptchr_3(pOut,pIn,F16)     /* Function dispatcher for GMCLIB_BetaProjection3Ph_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
    */
    /* @remarks Implements DAMMCLIB00011 */
    #define GMCLIB_BetaProjection3Ph_Dsptchr_2(pOut,pIn)     GMCLIB_BetaProjection3Ph_Dsptchr_3(pOut,pIn,FLT)     /* Function dispatcher for GMCLIB_BetaProjection3Ph_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define GMCLIB_BetaProjection3Ph_Dsptchr_3(pOut,pIn,Impl)    GMCLIB_BetaProjection3Ph_Dsptchr_(pOut,pIn,Impl)   /* Function dispatcher for GMCLIB_BetaProjection3Ph_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref GMCLIB_BetaProjection3Ph_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DAMMCLIB00013 */
  #define GMCLIB_BetaProjection3Ph_Dsptchr_(pOut,pIn,Impl)     GMCLIB_BetaProjection3Ph_##Impl(pOut,pIn)          /* Function dispatcher for GMCLIB_BetaProjection3Ph_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_BetaProjection3Ph_F32(SWLIBS_3Syst_F32 *const pOut, const SWLIBS_3Syst_F32 *const pIn);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_BetaProjection3Ph_F16(SWLIBS_3Syst_F16 *const pOut, const SWLIBS_3Syst_F16 *const pIn);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_BetaProjection3Ph_FLT(SWLIBS_3Syst_FLT *const pOut, const SWLIBS_3Syst_FLT *const pIn);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GMCLIB_BETAPROJECTION3PH_H */
