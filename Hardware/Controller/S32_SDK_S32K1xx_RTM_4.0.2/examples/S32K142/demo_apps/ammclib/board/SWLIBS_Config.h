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
* @file     SWLIBS_Config.h
*
* @version  1.0.34.0
*
* @date     17-April-2019
*
* @brief    AMMCLIB configuration header file.
*
******************************************************************************/

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section SWLIBS_Config_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section SWLIBS_Config_h_REF_2
* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
*
* @section SWLIBS_Config_h_REF_3
* Violates MISRA 2004 Required Rule 1.4, Identifier clash. 
* This is not a violation since all the used compilers interpret the identifiers correctly.
*/

#ifndef SWLIBS_CONFIG_H
#define SWLIBS_CONFIG_H

/******************************************************************************
* Library version definition (format <major number>.<minor number>.<build number>)
******************************************************************************/
#define SWLIBS_VERSION {(unsigned char)1U,(unsigned char)1U,(unsigned char)21U}

/******************************************************************************
* Global definitions
******************************************************************************/
#define SWLIBS_STD_ON    0x01U
#define SWLIBS_STD_OFF   0x00U

/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
#define F32 F32
/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
#define F16 F16
/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
#define FLT FLT

/*
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
* characters.
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash. 
*/
#define SWLIBS_DEFAULT_IMPLEMENTATION_F32   (1U) 
/*
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
* characters. 
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash.
*/
#define SWLIBS_DEFAULT_IMPLEMENTATION_F16   (2U)
/*
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 
* characters. 
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash.
*/
#define SWLIBS_DEFAULT_IMPLEMENTATION_FLT   (3U)

/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
/******************************************************************************
* Implementation supported
******************************************************************************/
#define SWLIBS_SUPPORT_F32    SWLIBS_STD_ON   /**< Enables/disables support of 32-bit fractional implementation. */
/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
#define SWLIBS_SUPPORT_F16    SWLIBS_STD_ON   /**< Enables/disables support of 16-bit fractional implementation. */
/*
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
* macro. 
*/
#define SWLIBS_SUPPORT_FLT    SWLIBS_STD_ON   /**< Enables/disables support of single precision floating point implementation. */

#define SWLIBS_SUPPORTED_IMPLEMENTATION {SWLIBS_SUPPORT_F32,\
                                         SWLIBS_SUPPORT_F16,\
                                         SWLIBS_SUPPORT_FLT,\
                                         0,0,0,0,0,0} /**< Array of supported implementations. */

/******************************************************************************
* Selection of default implementation
******************************************************************************/
#ifndef SWLIBS_DEFAULT_IMPLEMENTATION
  #define SWLIBS_DEFAULT_IMPLEMENTATION    SWLIBS_DEFAULT_IMPLEMENTATION_FLT   /**< Selection of default implementation. */
  #ifndef SWLIBS_DEFAULT_IMPLEMENTATION
    #error "Please select the default implementation in the file SWLIBS_Config.h."
  #else
    #if((SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_F32)&& \
        (SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_F16)&& \
        (SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_FLT))
      #error "Please select the default implementation in the file SWLIBS_Config.h."
    #endif
  #endif /* SWLIBS_DEFAULT_IMPLEMENTATION */
#endif /* SWLIBS_DEFAULT_IMPLEMENTATION */

/******************************************************************************
* Compiler version check
******************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
  #if (__VER__ < 8011001)
    #error ***ERROR***: You are using an incompatible version of the IAR compiler. Please update your compiler to the supported version. You can find the list of the supported compilers in the Release Notes.
  #endif
#endif

#endif /* SWLIBS_CONFIG_H */

