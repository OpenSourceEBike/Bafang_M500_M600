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
* @file     SWLIBS_MacroDisp.h
*
* @version  1.0.11.0
*
* @date     17-April-2019
*
* @brief    AMMCLIB dispatcher header file.
*
******************************************************************************/

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section SWLIBS_MacroDisp_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section SWLIBS_MacroDisp_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section SWLIBS_MacroDisp_h_REF_3
* Violates MISRA 2004 Required Rule 20.2, Re-use of C90 identifier. 
* Symbol required to be defined with underscore prefix to highlight this is an internal library macro and warn before using in the application.
* 
* @section SWLIBS_MacroDisp_h_REF_4
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section SWLIBS_MacroDisp_h_REF_5
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section SWLIBS_MacroDisp_h_REF_6
* Violates MISRA 2004 Required Rule 19.12, Multiple use of '#/##' operators in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
*/

#ifndef SWLIBS_MACRODISP_H_
#define SWLIBS_MACRODISP_H_

#ifndef _MATLAB_BAM_CREATE
  /*
  * @violates @ref SWLIBS_MacroDisp_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_3 MISRA 2004 Required Rule 20.2, Re-use of C90 identifier. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_4 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define _ARG2(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,...) _12
  /*
  * @violates @ref SWLIBS_MacroDisp_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_4 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define VA_NUM_ARGS(...) _ARG2(__VA_ARGS__,12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
  /*
  * @violates @ref SWLIBS_MacroDisp_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_4 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define macro_dispatcher(func, ...) macro_dispatcher_(func, VA_NUM_ARGS(__VA_ARGS__))
  /*
  * @violates @ref SWLIBS_MacroDisp_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
  * defined. 
  * @violates @ref SWLIBS_MacroDisp_h_REF_4 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define macro_dispatcher_(func, nargs) macro_dispatcher__(func, nargs)
#endif /* _MATLAB_BAM_CREATE */
/*
* @violates @ref SWLIBS_MacroDisp_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
* @violates @ref SWLIBS_MacroDisp_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
* defined. 
* @violates @ref SWLIBS_MacroDisp_h_REF_5 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
* macro. 
* @violates @ref SWLIBS_MacroDisp_h_REF_6 MISRA 2004 Required Rule 19.12, Multiple use of '#/##' 
* operators in definition of macro. 
*/
#define macro_dispatcher__(func, nargs) func##_Dsptchr_## nargs


#endif /* SWLIBS_MACRODISP_H_ */
