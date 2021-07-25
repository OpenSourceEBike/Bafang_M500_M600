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
* @file       GMCLIB_DecouplingPMSM.h
*
* @version    1.0.12.0
*
* @date       17-April-2019
*
* @brief      Header file for GMCLIB_DecouplingPMSM function
*
******************************************************************************/
#ifndef GMCLIB_DECOUPLINGPMSM_H
#define GMCLIB_DECOUPLINGPMSM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GMCLIB_DecouplingPMSM_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GMCLIB_DecouplingPMSM_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GMCLIB_DecouplingPMSM_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GMCLIB_DecouplingPMSM_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GMCLIB_DecouplingPMSM_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section GMCLIB_DecouplingPMSM_h_REF_6
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
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  */
  #define GMCLIB_DecouplingPMSM(...)     macro_dispatcher(GMCLIB_DecouplingPMSM, __VA_ARGS__)(__VA_ARGS__)     /* This function calculates the cross-coupling voltages to eliminate the dq axis coupling causing non-linearity of the field oriented control. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_T                                                  GMCLIB_DECOUPLINGPMSM_T_F32                                           /**< Definition of GMCLIB_DECOUPLINGPMSM_T as alias for GMCLIB_DECOUPLINGPMSM_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_DEFAULT                                            GMCLIB_DECOUPLINGPMSM_DEFAULT_F32                                     /**< Definition of GMCLIB_DECOUPLINGPMSM_DEFAULT as alias for GMCLIB_DECOUPLINGPMSM_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00059 */
    #define GMCLIB_DecouplingPMSM_Dsptchr_5(pUdqDec,pUdq,pIdq,AngularVel,pParam)     GMCLIB_DecouplingPMSM_Dsptchr_6(pUdqDec,pUdq,pIdq,AngularVel,pParam,F32)     /* Function dispatcher for GMCLIB_DecouplingPMSM_Dsptchr_5, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_T                                                  GMCLIB_DECOUPLINGPMSM_T_F16                                           /**< Definition of GMCLIB_DECOUPLINGPMSM_T as alias for GMCLIB_DECOUPLINGPMSM_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_DEFAULT                                            GMCLIB_DECOUPLINGPMSM_DEFAULT_F16                                     /**< Definition of GMCLIB_DECOUPLINGPMSM_DEFAULT as alias for GMCLIB_DECOUPLINGPMSM_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00059 */
    #define GMCLIB_DecouplingPMSM_Dsptchr_5(pUdqDec,pUdq,pIdq,AngularVel,pParam)     GMCLIB_DecouplingPMSM_Dsptchr_6(pUdqDec,pUdq,pIdq,AngularVel,pParam,F16)     /* Function dispatcher for GMCLIB_DecouplingPMSM_Dsptchr_5, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_T                                                  GMCLIB_DECOUPLINGPMSM_T_FLT                                           /**< Definition of GMCLIB_DECOUPLINGPMSM_T as alias for GMCLIB_DECOUPLINGPMSM_T_FLT datatype in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    */
    #define GMCLIB_DECOUPLINGPMSM_DEFAULT                                            GMCLIB_DECOUPLINGPMSM_DEFAULT_FLT                                     /**< Definition of GMCLIB_DECOUPLINGPMSM_DEFAULT as alias for GMCLIB_DECOUPLINGPMSM_DEFAULT_FLT default value in case the single precision floating point implementation is selected. */
    /*
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
    * definition for macro. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like 
    * macro defined. 
    * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
    * macro parameter in definition of macro. 
    */
    /* @remarks Implements DGMCLIB00059 */
    #define GMCLIB_DecouplingPMSM_Dsptchr_5(pUdqDec,pUdq,pIdq,AngularVel,pParam)     GMCLIB_DecouplingPMSM_Dsptchr_6(pUdqDec,pUdq,pIdq,AngularVel,pParam,FLT)     /* Function dispatcher for GMCLIB_DecouplingPMSM_Dsptchr_5, do not modify!!! */
  #endif 

  /*
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  */
  #define GMCLIB_DecouplingPMSM_Dsptchr_6(pUdqDec,pUdq,pIdq,AngularVel,pParam,Impl)  GMCLIB_DecouplingPMSM_Dsptchr_(pUdqDec,pUdq,pIdq,AngularVel,pParam,Impl)     /* Function dispatcher for GMCLIB_DecouplingPMSM_Dsptchr_6, do not modify!!! */
  
  /*
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed 
  * definition for macro. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized 
  * macro parameter in definition of macro. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used 
  * in macro. 
  */
  /* @remarks Implements DGMCLIB00065 */
  #define GMCLIB_DecouplingPMSM_Dsptchr_(pUdqDec,pUdq,pIdq,AngularVel,pParam,Impl)   GMCLIB_DecouplingPMSM_##Impl(pUdqDec,pUdq,pIdq,AngularVel,pParam)            /* Function dispatcher for GMCLIB_DecouplingPMSM_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */
 
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /*
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  */
  #define GMCLIB_DECOUPLINGPMSM_DEFAULT_F32 {(tFrac32)0,(tS16)0,(tFrac32)0,(tS16)0}     /**< Default value for GMCLIB_DECOUPLINGPMSM_T_F32. */

  /*************************************************************************/
  /**
  @struct GMCLIB_DECOUPLINGPMSM_T_F32 "\GMCLIB_DecouplingPMSM.h" 

  @brief  Structure containing coefficients for calculation of the decoupling.
  *//***********************************************************************/
  /* @remarks Implements DGMCLIB00067, DGMCLIB00063, DGMCLIB00062 */
  typedef struct{
    tFrac32 f32Kd; /**< Coefficient \f$k_{df}\f$. */
    tS16 s16KdShift; /**< Scaling coefficient \f$k_{d_shift}\f$. */
    tFrac32 f32Kq; /**< Coefficient \f$k_{qf}\f$. */
    tS16 s16KqShift; /**< Scaling coefficient \f$k_{q_shift}\f$. */
  }GMCLIB_DECOUPLINGPMSM_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_DecouplingPMSM_F32(SWLIBS_2Syst_F32 *const pUdqDec, \
                                        const SWLIBS_2Syst_F32 *const pUdq, \
                                        const SWLIBS_2Syst_F32 *const pIdq, \
                                        tFrac32 f32AngularVel, \
                                        const GMCLIB_DECOUPLINGPMSM_T_F32 *const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /*
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GMCLIB_DECOUPLINGPMSM_DEFAULT_F16 {(tFrac16)0,(tS16)0,(tFrac16)0,(tS16)0}     /**< Default value for GMCLIB_DECOUPLINGPMSM_T_F16. */

  /*************************************************************************/
  /**
  @struct GMCLIB_DECOUPLINGPMSM_T_F16 "\GMCLIB_DecouplingPMSM.h" 

  @brief  Structure containing coefficients for calculation of the decoupling.
  *//***********************************************************************/
  /* @remarks Implements DGMCLIB00067, DGMCLIB00063, DGMCLIB00061 */
  typedef struct{
    tFrac16 f16Kd; /**< Coefficient \f$k_{df}\f$. */
    tS16 s16KdShift; /**< Scaling coefficient \f$k_{d_shift}\f$. */
    tFrac16 f16Kq; /**< Coefficient \f$k_{qf}\f$. */
    tS16 s16KqShift; /**< Scaling coefficient \f$k_{q_shift}\f$. */
  }GMCLIB_DECOUPLINGPMSM_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_DecouplingPMSM_F16(SWLIBS_2Syst_F16 *const pUdqDec, \
                                        const SWLIBS_2Syst_F16 *const pUdq, \
                                        const SWLIBS_2Syst_F16 *const pIdq, \
                                        tFrac16 f16AngularVel, \
                                        const GMCLIB_DECOUPLINGPMSM_T_F16 *const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /*
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
  * characters. 
  * @violates @ref GMCLIB_DecouplingPMSM_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash. 
  */
  #define GMCLIB_DECOUPLINGPMSM_DEFAULT_FLT {(tFloat)0,(tFloat)0}     /**< Default value for GMCLIB_DECOUPLINGPMSM_T_FLT. */

  /*************************************************************************/
  /**
  @struct GMCLIB_DECOUPLINGPMSM_T_FLT "\GMCLIB_DecouplingPMSM.h" 

  @brief  Structure containing coefficients for calculation of the decoupling.
  *//***********************************************************************/
  /* @remarks Implements DGMCLIB00067, DGMCLIB00063, DGMCLIB00060 */
  typedef struct{
    tFloat fltLD; /**< \f$L_{D}\f$ inductance [H]. */
    tFloat fltLQ; /**< \f$L_{Q}\f$ inductance [H]. */
  }GMCLIB_DECOUPLINGPMSM_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void GMCLIB_DecouplingPMSM_FLT(SWLIBS_2Syst_FLT *const pUdqDec, \
                                        const SWLIBS_2Syst_FLT *const pUdq, \
                                        const SWLIBS_2Syst_FLT *const pIdq, \
                                        tFloat fltAngularVel, \
                                        const GMCLIB_DECOUPLINGPMSM_T_FLT *const pParam);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GMCLIB_DECOUPLINGPMSM_H */
