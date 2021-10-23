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
* @file       AMCLIB_Windmilling.h
*
* @version    1.0.17.0
*
* @date       17-April-2019
*
* @brief      Header file for AMCLIB_Windmilling function
*
******************************************************************************/
#ifndef AMCLIB_WINDMILLING_H
#define AMCLIB_WINDMILLING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section AMCLIB_Windmilling_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section AMCLIB_Windmilling_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section AMCLIB_Windmilling_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section AMCLIB_Windmilling_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section AMCLIB_Windmilling_h_REF_5
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
* 
* @section AMCLIB_Windmilling_h_REF_6
* Violates MISRA 2004 Required Rule 1.4, Identifier clash.
* The compiler can distinguish names with more than 31 characters.
*
*/
#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"
#include "AMCLIB_TrackObsrv.h"
#include "GDFLIB_FilterMA.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_WindmillingInit(...)     macro_dispatcher(AMCLIB_WindmillingInit, __VA_ARGS__)(__VA_ARGS__)     /* This function initializes the state of the AMCLIB_Windmilling. */
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_Windmilling(...)     macro_dispatcher(AMCLIB_Windmilling, __VA_ARGS__)(__VA_ARGS__)     /* This function calculates the AMCLIB_Windmilling. */
  
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_WINDMILLING_T                                        AMCLIB_WINDMILLING_T_F32
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_WindmillingInit_Dsptchr_2(ADCMaxError,pCtrl)     AMCLIB_WindmillingInit_Dsptchr_3(ADCMaxError,pCtrl,F32)     /* Function dispatcher for AMCLIB_WindmillingInit_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_Windmilling_Dsptchr_4(pUabcIn,pPosEst,pVelocityEst,pCtrl)     AMCLIB_Windmilling_Dsptchr_5(pUabcIn,pPosEst,pVelocityEst,pCtrl,F32)     /* Function dispatcher for AMCLIB_Windmilling_Dsptchr_4, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_WINDMILLING_T                                        AMCLIB_WINDMILLING_T_F16
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_WindmillingInit_Dsptchr_2(ADCMaxError,pCtrl)     AMCLIB_WindmillingInit_Dsptchr_3(ADCMaxError,pCtrl,F16)     /* Function dispatcher for AMCLIB_WindmillingInit_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_Windmilling_Dsptchr_4(pUabcIn,pPosEst,pVelocityEst,pCtrl)     AMCLIB_Windmilling_Dsptchr_5(pUabcIn,pPosEst,pVelocityEst,pCtrl,F16)     /* Function dispatcher for AMCLIB_Windmilling_Dsptchr_4, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define AMCLIB_WINDMILLING_T                                        AMCLIB_WINDMILLING_T_FLT
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
    * characters.
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_WindmillingInit_Dsptchr_2(ADCMaxError,pCtrl)     AMCLIB_WindmillingInit_Dsptchr_3(ADCMaxError,pCtrl,FLT)     /* Function dispatcher for AMCLIB_WindmillingInit_Dsptchr_2, do not modify!!! */
    /*
    * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
    * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DAMMCLIB00018 */
    #define AMCLIB_Windmilling_Dsptchr_4(pUabcIn,pPosEst,pVelocityEst,pCtrl)     AMCLIB_Windmilling_Dsptchr_5(pUabcIn,pPosEst,pVelocityEst,pCtrl,FLT)     /* Function dispatcher for AMCLIB_Windmilling_Dsptchr_4, do not modify!!! */
  #endif 
  
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_5 MISRA 2004 Required Rule 5.1, Identifier exceeds 31
  * characters.
  * @violates @ref AMCLIB_Windmilling_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  #define AMCLIB_WindmillingInit_Dsptchr_3(ADCMaxError,pCtrl,Impl)    AMCLIB_WindmillingInit_Dsptchr_(ADCMaxError,pCtrl,Impl)     /* Function dispatcher for AMCLIB_WindmillingInit_Dsptchr_3, do not modify!!! */
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_6 MISRA 2004 Required Rule 1.4, Identifier clash.
  */
  /* @remarks Implements DAMMCLIB00020 */
  #define AMCLIB_WindmillingInit_Dsptchr_(ADCMaxError,pCtrl,Impl)     AMCLIB_WindmillingInit_##Impl(ADCMaxError,pCtrl)            /* Function dispatcher for AMCLIB_WindmillingInit_Dsptchr_, do not modify!!! */
  
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define AMCLIB_Windmilling_Dsptchr_5(pUabcIn,pPosEst,pVelocityEst,pCtrl,Impl)    AMCLIB_Windmilling_Dsptchr_(pUabcIn,pPosEst,pVelocityEst,pCtrl,Impl)     /* Function dispatcher for AMCLIB_Windmilling_Dsptchr_5, do not modify!!! */
  /*
  * @violates @ref AMCLIB_Windmilling_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref AMCLIB_Windmilling_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref AMCLIB_Windmilling_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
  */
  /* @remarks Implements DAMMCLIB00020 */
  #define AMCLIB_Windmilling_Dsptchr_(pUabcIn,pPosEst,pVelocityEst,pCtrl,Impl)     AMCLIB_Windmilling_##Impl(pUabcIn,pPosEst,pVelocityEst,pCtrl)            /* Function dispatcher for AMCLIB_Windmilling_Dsptchr_, do not modify!!! */
#endif

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/
/************************************************************************//**
@enum   AMCLIB_WINDMILLING_RET_T

@brief  Enum type of the AMCLIB_Windmilling return value.
*//*************************************************************************/
typedef enum
{
  UNDECIDED = 0,   /**< AMCLIB_Windmilling has not yet decided whether the motor is spinning or not. The AMCLIB_Windmilling must be called again in the next sampling period. */
  SPINNING  = 1,   /**< The motor is spinning. */
  STOPPED  = 2,    /**< The motor is stopped. */
} AMCLIB_WINDMILLING_RET_T;




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct AMCLIB_WINDMILLING_T_F32 "\AMCLIB_Windmilling.h" 

  @brief  Observer configuration structure.
  *//**********************************************************************/
  typedef struct
  {
    tFrac32 f32ADCComp;  /**< A/D converter error compensation constant. This parameter is initialized by #AMCLIB_WindmillingInit_F32 based on user's input. */
    AMCLIB_TRACK_OBSRV_T_F32 pParamATO;  /**< PI controller and integrator parameters and state. These parameters must be configured by the user before the first call of #AMCLIB_Windmilling_F32. */
    GDFLIB_FILTER_MA_T_F32 pDetMA;  /**< PLL lock detector internal state. Do not change. This sub-structure and all its parameters are automatically initialized by #AMCLIB_WindmillingInit_F32. */
    tU8 u8DetNoiseCnt;   /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F32. */
    tU8 u8DetSpin;       /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F32. */
    tU16 u16DetHystCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F32. */
    tU16 u16DetStopCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F32. */
  } AMCLIB_WINDMILLING_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_WindmillingInit_F32(tFrac32 f32ADCMaxError, AMCLIB_WINDMILLING_T_F32 *const pCtrl);
  extern AMCLIB_WINDMILLING_RET_T AMCLIB_Windmilling_F32(const SWLIBS_3Syst_F32 *pUabcIn,
    tFrac32 *pPosEst, tFrac32 *pVelocityEst, AMCLIB_WINDMILLING_T_F32 *const pCtrl);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct AMCLIB_WINDMILLING_T_F16 "\AMCLIB_Windmilling.h" 

  @brief  Observer configuration structure.
  *//**********************************************************************/
  typedef struct
  {
    tFrac16 f16ADCComp;  /**< A/D converter error compensation constant. This parameter is initialized by #AMCLIB_WindmillingInit_F16 based on user's input. */
    AMCLIB_TRACK_OBSRV_T_F16 pParamATO;  /**< PI controller and integrator parameters and state. These parameters must be configured by the user before the first call of #AMCLIB_Windmilling_F16. */
    GDFLIB_FILTER_MA_T_F16 pDetMA; /**< PLL lock detector internal state. Do not change. This sub-structure and all its parameters are automatically initialized by #AMCLIB_WindmillingInit_F16. */
    tU8 u8DetNoiseCnt;   /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F16. */
    tU8 u8DetSpin;       /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F16. */
    tU16 u16DetHystCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F16. */
    tU16 u16DetStopCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_F16. */
  } AMCLIB_WINDMILLING_T_F16;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_WindmillingInit_F16(tFrac16 f16ADCMaxError, AMCLIB_WINDMILLING_T_F16 *const pCtrl);
  extern AMCLIB_WINDMILLING_RET_T AMCLIB_Windmilling_F16(const SWLIBS_3Syst_F16 *pUabcIn,
    tFrac16 *pPosEst, tFrac16 *pVelocityEst, AMCLIB_WINDMILLING_T_F16 *const pCtrl);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /************************************************************************/
  /**
  @struct AMCLIB_WINDMILLING_T_FLT "\AMCLIB_Windmilling.h"

  @brief  Observer configuration structure.
  *//**********************************************************************/
  typedef struct
  {
    tFloat fltADCComp;   /**< A/D converter error compensation constant. This parameter is initialized by #AMCLIB_WindmillingInit_FLT based on user's input. */
    AMCLIB_TRACK_OBSRV_T_FLT pParamATO;  /**< PI controller and integrator parameters and state. These parameters must be configured by the user before the first call of #AMCLIB_Windmilling_FLT. */
    GDFLIB_FILTER_MA_T_FLT pDetMA;  /**< PLL lock detector internal state. Do not change. This sub-structure and all its parameters are automatically initialized by #AMCLIB_WindmillingInit_FLT. */
    tU8 u8DetNoiseCnt;   /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_FLT. */
    tU8 u8DetSpin;       /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_FLT. */
    tU16 u16DetHystCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_FLT. */
    tU16 u16DetStopCnt;  /**< PLL lock detector internal state. Do not change. This parameter is automatically initialized by #AMCLIB_WindmillingInit_FLT. */
  } AMCLIB_WINDMILLING_T_FLT;
  
/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern void AMCLIB_WindmillingInit_FLT(tFloat fltADCMaxError, AMCLIB_WINDMILLING_T_FLT *const pCtrl);
  extern AMCLIB_WINDMILLING_RET_T AMCLIB_Windmilling_FLT(const SWLIBS_3Syst_FLT *pUabcIn,
    tFloat *pPosEst, tFloat *pVelocityEst, AMCLIB_WINDMILLING_T_FLT *const pCtrl);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */
/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* AMCLIB_WINDMILLING_H */
