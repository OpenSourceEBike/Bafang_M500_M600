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
* @file     SWLIBS_Defines.h
*
* @version  1.0.33.0
*
* @date     17-April-2019
*
* @brief    Defines and common Macros definition file.
*
*******************************************************************************
*
******************************************************************************/

#ifndef SWLIBS_DEFINES_H
#define SWLIBS_DEFINES_H

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section SWLIBS_Defines_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of disallowed macro
* definition.
*
* @section SWLIBS_Defines_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires the function-like macro
* definition.
*
* @section SWLIBS_Defines_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of unparenthesized
* macro parameters.
*/

#include "SWLIBS_Typedefs.h"

/******************************************************************************
* Defines and macros            (scope: module-local)
******************************************************************************/

/* Definition of inline directive for all supported compilers. */
#if defined(__ghs__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE inline
  #endif
#elif defined(__MWERKS__) || defined(__CWCC__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static __inline
  #endif
#elif defined(__DCC__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static __inline__
  #endif
#elif defined(__CSMC__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static @inline
  #endif
#elif defined(__IAR_SYSTEMS_ICC__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static inline
  #endif
#elif defined(__GNUC__) && defined(__PPC_EABI__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static inline
  #endif
#elif defined(__GNUC__)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static __inline__
  #endif
#elif defined(_MATLAB_BAM_CREATE)
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro.
    */
    #define INLINE static __inline
  #endif
#else
  #ifndef INLINE
    /*
    * @violates @ref SWLIBS_Defines_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    */
    #define INLINE static inline
  #endif
#endif

/** Constant representing the maximal negative value of a signed 16-bit fixed point fractional number, floating point representation. */
#define SFRACT_MIN          (-1.0)

/** Constant representing the maximal positive value of a signed 16-bit fixed point fractional
 *  number,  floating point representation. */
#define SFRACT_MAX          (0.999969482421875)

/** Constant representing the maximal negative value of signed 32-bit fixed point fractional number,  floating point representation. */
#define FRACT_MIN           (-1.0)

/** Constant representing the maximal positive value of a signed 32-bit fixed point fractional
 *  number,  floating point representation. */
#define FRACT_MAX           (0.9999999995343387126922607421875)

/** Value 0.5 in 32-bit fixed point fractional format. */
#define FRAC32_0_5           ((tFrac32) 0x40000000)

/** Value 0.5 in 16-bit fixed point fractional format. */
#define FRAC16_0_5           ((tFrac16) 0x4000)

/** Value 0.25 in 32-bit fixed point fractional format. */
#define FRAC32_0_25           ((tFrac32) 0x20000000)

/** Value 0.25 in 16-bit fixed point fractional format. */
#define FRAC16_0_25           ((tFrac16) 0x2000)

/** Constant representing the maximal positive value of a unsigned 16-bit fixed point integer
 *  number, equal to \f$2^{15}\f$ = 0x8000. */
#ifndef UINT16_MAX
#define UINT16_MAX          ((tU16) 0x8000)
#endif

/** Constant representing the maximal positive value of a signed 16-bit fixed point integer
 *  number, equal to \f$2^{15}\f$-1 = 0x7fff. */
#ifndef INT16_MAX
#define INT16_MAX           ((tS16) 0x7fff)
#endif

/** Constant representing the maximal negative value of a signed 16-bit fixed point integer
 *  number, equal to -\f$2^{15}\f$ = 0x8000. */
#ifndef INT16_MIN
#define INT16_MIN           ((tS16) 0x8000)
#endif

/** Constant representing the maximal positive value of a unsigned 32-bit fixed point integer
 *  number, equal to \f$2^{31}\f$ = 0x80000000. */
#ifndef UINT32_MAX
#define UINT32_MAX          ((tU32) 0x80000000U)
#endif

/** Constant representing the maximal positive value of a signed 32-bit fixed point integer
 *  number, equal to \f$2^{31}\f$-1 = 0x7fff ffff. */
#ifndef INT32_MAX
#define INT32_MAX           ((tS32) 0x7fffffff)
#endif

/** Constant representing the maximal negative value of a signed 32-bit fixed point integer
 *  number, equal to -\f$2^{31}\f$ = 0x8000 0000. */
#ifndef INT32_MIN
#define INT32_MIN           ((tS32) 0x80000000U)
#endif

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Constant representing the maximal negative value of the 32-bit float type. */
#ifndef FLOAT_MIN
#define FLOAT_MIN           ((tFloat)(-3.4028234e+38F))
#endif
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Constant representing the maximal positive value of the 32-bit float type. */
#ifndef FLOAT_MAX
#define FLOAT_MAX           ((tFloat)(3.4028234e+38F))
#endif
#endif 

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 16-bit integer value cast to a signed 32-bit integer. */
#define INT16TOINT32(x)        ((tS32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 32-bit integer value cast to a signed 16-bit integer. */
#define INT32TOINT16(x)        ((tS16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 32-bit integer value cast to a signed 64-bit integer. */
#define INT32TOINT64(x)        ((tS64) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 64-bit integer value cast to a signed 32-bit integer. */
#define INT64TOINT32(x)        ((tS32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 16-bit fractional value cast to a signed 16-bit integer. */
#define F16TOINT16(x)       ((tS16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 16 bits of a signed 32-bit fractional value cast to a signed 16-bit integer. */
#define F32TOINT16(x)       ((tS16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 16 bits of a signed 64-bit fractional value cast to a signed 16-bit integer. */
#define F64TOINT16(x)       ((tS16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - a signed 16-bit fractional value cast to a signed 32-bit integer, the value placed at the lower
*   16-bits of the 32-bit result. */
#define F16TOINT32(x)       ((tS32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 32-bit fractional value cast to a signed 32-bit integer. */
#define F32TOINT32(x)       ((tS32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 32 bits of a signed 64-bit fractional value cast to a signed 32-bit integer. */
#define F64TOINT32(x)       ((tS32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 16-bit fractional value cast to a signed 64-bit integer, the value placed at the lower
*   16-bits of the 64-bit result. */
#define F16TOINT64(x)       ((tS64) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 32-bit fractional value cast to a signed 64-bit integer, the value placed at the lower
*   32-bits of the 64-bit result. */
#define F32TOINT64(x)       ((tS64) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 64-bit fractional value cast to a signed 64-bit integer. */
#define F64TOINT64(x)       ((tS64) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 16-bit integer value cast to a signed 16-bit fractional. */
#define INT16TOF16(x)       ((tFrac16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 16-bit integer value cast to a signed 32-bit fractional, the value placed at the lower
*   16 bits of the 32-bit result. */
#define INT16TOF32(x)       ((tFrac32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 16-bits of a signed 32-bit integer value cast to a signed 16-bit fractional. */
#define INT32TOF16(x)       ((tFrac16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - signed 32-bit integer value cast to a signed 32-bit fractional. */
#define INT32TOF32(x)       ((tFrac32) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 16-bits of a signed 64-bit integer value cast to a signed 16-bit fractional. */
#define INT64TOF16(x)       ((tFrac16) (x))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
*/
/** Type casting - lower 32-bits of a signed 64-bit integer value cast to a signed 32-bit fractional. */
#define INT64TOF32(x)       ((tFrac32) (x))

/** One over sqrt(3) with a 16-bit result, the result is rounded for a better accuracy. */
#define F16_1_DIVBY_SQRT3   ((tFrac16) 0x49E7)

/** One over sqrt(3) with a 32-bit result, the result is rounded for a better accuracy. */
#define F32_1_DIVBY_SQRT3   ((tFrac32) 0x49E69D16)

/** Sqrt(3) divided by two with a 16-bit result, the result is rounded for a better accuracy. */
#define F16_SQRT3_DIVBY_2   ((tFrac16) 0x6EDA)

/** Sqrt(3) divided by four with a 16-bit result. */
#define F16_SQRT3_DIVBY_4   ((tFrac16) 0x376D)

/** Sqrt(3) divided by two with a 32-bit result, the result is rounded for a better accuracy. */
#define F32_SQRT3_DIVBY_2   ((tFrac32) 0x6ED9EBA1)

/** Sqrt(3) divided by four with a 32-bit result. */
#define F32_SQRT3_DIVBY_4   ((tFrac32) 0x376CF5D1)

/** Sqrt(2) divided by two with a 16-bit result, the result is rounded for a better accuracy. */
#define F16_SQRT2_DIVBY_2   ((tFrac16) 0x5A82)

/** Sqrt(2) divided by two with a 32-bit result, the result is rounded for a better accuracy. */
#define F32_SQRT2_DIVBY_2   ((tFrac32) 0x5A82799A)

/** One third in 16-bit resolution, the result is rounded for a better accuracy. */
#define F16_1_DIVBY_3   ((tFrac16) 0x2AAB)

/** One third in 32-bit resolution, the result is rounded for a better accuracy. */
#define F32_1_DIVBY_3   ((tFrac32) 0x2AAAAAAB)

/** Two thirds in 16-bit resolution, the result is rounded for a better accuracy. */
#define F16_2_DIVBY_3   ((tFrac16) 0x5555)

/** Two thirds in 32-bit resolution, the result is rounded for a better accuracy. */
#define F32_2_DIVBY_3   ((tFrac32) 0x55555555)

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* @violates @ref SWLIBS_Defines_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
* parameter in definition of macro. 
*/
/** Macro converting a signed fractional [-1,1) number into a 16-bit fixed point number in format Q1.15.*/
#define FRAC16(x)           ((tFrac16) (((x) < SFRACT_MAX) ? (((x) >= SFRACT_MIN) ? ((x)*32768.0) : INT16_MIN) : INT16_MAX))

/*
* @violates @ref SWLIBS_Defines_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* @violates @ref SWLIBS_Defines_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
* parameter in definition of macro. 
*/
/** Macro converting a signed fractional [-1,1) number into a 32-bit fixed point number in format Q1.31.*/
#define FRAC32(x)           ((tFrac32) (((x) < FRACT_MAX) ? (((x) >= FRACT_MIN) ? ((x)*2147483648.0) : INT32_MIN) : INT32_MAX))

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** One over sqrt(3) in single precision floating point format. */
#define FLOAT_DIVBY_SQRT3   ((tFloat) 0.5773502691896258)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Sqrt(3) in single precision floating point format. */
#define FLOAT_SQRT3 ((tFloat) 1.732050776481628)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Sqrt(3) divided by two in single precision floating point format. */
#define FLOAT_SQRT3_DIVBY_2 ((tFloat) 0.866025403784439)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Sqrt(3) divided by four in single precision floating point format. */
#define FLOAT_SQRT3_DIVBY_4 ((tFloat) 0.4330127018922190)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Sqrt(3) divided by four correction constant. */
#define FLOAT_SQRT3_DIVBY_4_CORRECTION ((tFloat)0)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** 2*\f$\Pi\f$ in single precision floating point format. */
#define FLOAT_2_PI          ((tFloat) 6.28318530717958)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** \f$\Pi\f$ in single precision floating point format. */
#define FLOAT_PI            ((tFloat) 3.14159265358979)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** \f$\Pi\f$/2 in single precision floating point format. */
#define FLOAT_PI_DIVBY_2    ((tFloat) 1.57079632679490)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Tan(\f$\Pi\f$/6) in single precision floating point format. */
#define FLOAT_TAN_PI_DIVBY_6   ((tFloat)0.577350269189626000)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Tan(\f$\Pi\f$/12) in single precision floating point format. */
#define FLOAT_TAN_PI_DIVBY_12   ((tFloat)0.267949192431123000)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** \f$\Pi\f$/6 in single precision floating point format. */
#define FLOAT_PI_DIVBY_6   ((tFloat)0.523598775598299000)  
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Double to single precision correction constant for \f$\Pi\f$, equal to (\f$\Pi\f$(Double) - \f$\Pi\f$(Single)). */
#define FLOAT_PI_SINGLE_CORRECTION ((tFloat)4.371139006309477e-08)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Double to single precision correction constant for \f$\Pi\f$, equal to  (2*(\f$\Pi\f$(Double) - \f$\Pi\f$(Single)). */
#define FLOAT_PI_CORRECTION ((tFloat)8.742278012618954e-08)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** \f$\Pi\f$/4 in single precision floating point format. */
#define FLOAT_PI_DIVBY_4    ((tFloat) 0.7853981633974480)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Number four divided by \f$\Pi\f$ in single precision floating point format. */
#define FLOAT_4_DIVBY_PI    ((tFloat) 1.2732395447351600) 
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Value 0.5 in single precision floating point format. */
#define FLOAT_0_5           ((tFloat) 0.5)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Value -0.5 in single precision floating point format. */
#define FLOAT_MINUS_0_5     ((tFloat) -0.5)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Value 1 in single precision floating point format. */
#define FLOAT_PLUS_1        ((tFloat) 1)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Value -1 in single precision floating point format. */
#define FLOAT_MINUS_1       ((tFloat) -1)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Constant representing the smallest positive normalized 32-bit floating-point value. */
#define FLOAT_MIN_NORM       ((tFloat) 1.175494350822288e-38)
#endif 

#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
/** Machine epsilon for single precision floating-point data type. */
#define AMMCLIB_FLT_EPS   ((tFloat)1.175494350822288e-38)
#endif 


#endif /* SWLIBS_DEFINES_H */
