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
* @file       GFLIB_AtanYXShifted.h
*
* @version    1.0.9.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_AtanYXShifted function
*
******************************************************************************/
#ifndef GFLIB_ATANYXSHIFTED_H
#define GFLIB_ATANYXSHIFTED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_AtanYXShifted_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_AtanYXShifted_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_AtanYXShifted_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_AtanYXShifted_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
*/
#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"
#include "GFLIB_AtanYX.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_AtanYXShifted(...)     macro_dispatcher(GFLIB_AtanYXShifted, __VA_ARGS__)(__VA_ARGS__)     /* This function calculates the angle of two sine waves shifted in phase to each other. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_ATANYXSHIFTED_T                             GFLIB_ATANYXSHIFTED_T_F32                             /**< Definition of GFLIB_ATANYXSHIFTED_T as alias for GFLIB_ATANYXSHIFTED_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00099 */
    #define GFLIB_AtanYXShifted_Dsptchr_3(InY,InX,pParam)     GFLIB_AtanYXShifted_Dsptchr_4(InY,InX,pParam,F32)     /* Function dispatcher for GFLIB_AtanYXShifted_Dsptchr_3, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_ATANYXSHIFTED_T                             GFLIB_ATANYXSHIFTED_T_F16                             /**< Definition of GFLIB_ATANYXSHIFTED_T as alias for GFLIB_ATANYXSHIFTED_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00099 */
    #define GFLIB_AtanYXShifted_Dsptchr_3(InY,InX,pParam)     GFLIB_AtanYXShifted_Dsptchr_4(InY,InX,pParam,F16)     /* Function dispatcher for GFLIB_AtanYXShifted_Dsptchr_3, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GFLIB_ATANYXSHIFTED_T                             GFLIB_ATANYXSHIFTED_T_FLT                             /**< Definition of GFLIB_ATANYXSHIFTED_T as alias for GFLIB_ATANYXSHIFTED_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00099 */
    #define GFLIB_AtanYXShifted_Dsptchr_3(InY,InX,pParam)     GFLIB_AtanYXShifted_Dsptchr_4(InY,InX,pParam,FLT)     /* Function dispatcher for GFLIB_AtanYXShifted_Dsptchr_3, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_AtanYXShifted_Dsptchr_4(InY,InX,pParam,Impl)    GFLIB_AtanYXShifted_Dsptchr_(InY,InX,pParam,Impl)     /* Function dispatcher for GFLIB_AtanYXShifted_Dsptchr_4, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_AtanYXShifted_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition 
  * for macro. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_AtanYXShifted_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  */
  /* @remarks Implements DGFLIB00101 */
  #define GFLIB_AtanYXShifted_Dsptchr_(InY,InX,pParam,Impl)     GFLIB_AtanYXShifted_##Impl(InY,InX,pParam)            /* Function dispatcher for GFLIB_AtanYXShifted_Dsptchr_, do not modify!!! */
#endif
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct GFLIB_ATANYXSHIFTED_T_F32 "\GFLIB_AtanYXShifted.h"

  @brief  Structure containing the parameter for the AtanYXShifted function.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00102 */
  typedef struct{
    tFrac32     f32Ky;        /**< Multiplication coefficient for the y-signal. */
    tFrac32     f32Kx;        /**< Multiplication coefficient for the x-signal. */
    tS32        s32Ny;        /**< Scaling coefficient for the y-signal. */
    tS32        s32Nx;        /**< Scaling coefficient for the x-signal. */
    tFrac32     f32ThetaAdj;  /**< Adjusting angle. */
  }GFLIB_ATANYXSHIFTED_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac32 GFLIB_AtanYXShifted_F32(tFrac32 f32InY,tFrac32 f32InX, const GFLIB_ATANYXSHIFTED_T_F32 *pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct GFLIB_ATANYXSHIFTED_T_F16 "\GFLIB_AtanYXShifted.h"

  @brief  Structure containing the parameter for the AtanYXShifted function.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00102 */
  typedef struct{
    tFrac16     f16Ky;        /**< Multiplication coefficient for the y-signal. */
    tFrac16     f16Kx;        /**< Multiplication coefficient for the x-signal. */
    tS16        s16Ny;        /**< Scaling coefficient for the y-signal. */
    tS16        s16Nx;        /**< Scaling coefficient for the x-signal. */
    tFrac16     f16ThetaAdj;  /**< Adjusting angle. */
  }GFLIB_ATANYXSHIFTED_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFrac16 GFLIB_AtanYXShifted_F16(tFrac16 f16InY,tFrac16 f16InX, const GFLIB_ATANYXSHIFTED_T_F16 *pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct GFLIB_ATANYXSHIFTED_T_FLT "\GFLIB_AtanYXShifted.h"

  @brief  Structure containing the parameter for the AtanYXShifted function.
  *//**********************************************************************/
  /* @remarks Implements DGFLIB00102 */
  typedef struct{
    tFloat      fltKy;        /**< Multiplication coefficient for the y-signal. */
    tFloat      fltKx;        /**< Multiplication coefficient for the x-signal. */
    tFloat      fltThetaAdj;  /**< Adjusting angle. */
  }GFLIB_ATANYXSHIFTED_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern tFloat GFLIB_AtanYXShifted_FLT(tFloat fltInY,tFloat fltInX, const GFLIB_ATANYXSHIFTED_T_FLT *pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GFLIB_ATANYXSHIFTED_H */
