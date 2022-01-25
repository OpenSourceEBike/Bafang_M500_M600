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
* @file       GFLIB_VMin.h
*
* @version    1.0.0.0
*
* @date       17-April-2019
*
* @brief      Header file for GFLIB_VMin function
*
******************************************************************************/
#ifndef GFLIB_VMIN_H
#define GFLIB_VMIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
* 
* @section GFLIB_VMin_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of disallowed macro 
* definition. 
* 
* @section GFLIB_VMin_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires the function-like macro 
* definition. 
* 
* @section GFLIB_VMin_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of unparenthesized 
* macro parameters. 
* 
* @section GFLIB_VMin_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro. 
* To allow the user utilize the benefits of using all three supported implementation of each 
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators. 
* 
* @section GFLIB_VMin_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file. 
* Inlined functions require definition in the header file.
* 
* @section GFLIB_VMin_h_REF_6
* Violates MISRA 2004 Required Rule 17.4, Pointer arithmetic other than array indexing used.
* This is an array indexing operation.
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
  * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_VMin(...)     macro_dispatcher(GFLIB_VMin, __VA_ARGS__)(__VA_ARGS__)     /* This function finds the vector minimum. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00368 */
    #define GFLIB_VMin_Dsptchr_2(pIn,N)     GFLIB_VMin_Dsptchr_3(pIn,N,F32)     /* Function dispatcher for GFLIB_VMin_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00368 */
    #define GFLIB_VMin_Dsptchr_2(pIn,N)     GFLIB_VMin_Dsptchr_3(pIn,N,F16)     /* Function dispatcher for GFLIB_VMin_Dsptchr_2, do not modify!!! */
  #endif 
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_FLT)
    /*
    * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
    * macro. 
    * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro 
    * defined. 
    * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
    * parameter in definition of macro. 
    */
    /* @remarks Implements DGFLIB00368 */
    #define GFLIB_VMin_Dsptchr_2(pIn,N)     GFLIB_VMin_Dsptchr_3(pIn,N,FLT)     /* Function dispatcher for GFLIB_VMin_Dsptchr_2, do not modify!!! */
  #endif 

  /*
  * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  */
  #define GFLIB_VMin_Dsptchr_3(pIn,N,Impl)    GFLIB_VMin_Dsptchr_(pIn,N,Impl)   /* Function dispatcher for GFLIB_VMin_Dsptchr_3, do not modify!!! */
  
  /*
  * @violates @ref GFLIB_VMin_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for 
  * macro. 
  * @violates @ref GFLIB_VMin_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined. 
  * @violates @ref GFLIB_VMin_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro 
  * parameter in definition of macro. 
  * @violates @ref GFLIB_VMin_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in 
  * macro. 
  */
  /* @remarks Implements DGFLIB00370 */
  #define GFLIB_VMin_Dsptchr_(pIn,N,Impl)     GFLIB_VMin_##Impl(pIn,N)          /* Function dispatcher for GFLIB_VMin_Dsptchr_, do not modify!!! */
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
  extern tU32 GFLIB_VMin_F32(const tFrac32 *pIn, tU32 u32N);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */


/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/
  extern tU16 GFLIB_VMin_F16(const tFrac16 *pIn, tU16 u16N);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */


/****************************************************************************
* Implementation variant: Single precision floating point
****************************************************************************/
#if (SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON)
  /****************************************************************************
  * Exported function prototypes
  ****************************************************************************/ 
  extern tU32 GFLIB_VMin_FLT(const tFloat *pIn, tU32 u32N);
#endif /* SWLIBS_SUPPORT_FLT == SWLIBS_STD_ON */




/****************************************************************************
* Inline functions
****************************************************************************/

#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
#ifndef RTWTYPES_H
/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 4 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 4-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[4] = {6, 3, 1, 4};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin4_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_inline on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00352 */
INLINE tU16 GFLIB_VMin4_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile(" ldm %[pIn],{r1,r2}\n"
                   " mov %[u32IdxA],#0x10000\n"
                   " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                   " ssub16 %[u32ValA],r1,r2\n"
                   " sel %[u32ValA],r2,r1\n"
                   " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                   " ror %[u32IdxB],%[u32IdxA],#16\n"
                   " sasx %[u32ValB],%[u32ValA],%[u32ValA]\n"
                   " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif (defined(__IAR_SYSTEMS_ICC__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pIn],{r1,r2}\n"
          " mov %[u32IdxA],#0x10000\n"
          " add %[u32IdxB],%[u32IdxA],#0x20002\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValB],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValB]"+r"(u32ValB), [u32ValA]"+r"(u32ValA),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    
    #pragma ghs optasm
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile(" ldm %[pIn],{r1,r2}\n"
                   " mov %[u32IdxA],#0x10000\n"
                   " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                   " ssub16 %[u32ValA],r1,r2\n"
                   " sel %[u32ValA],r2,r1\n"
                   " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                   " ror %[u32IdxB],%[u32IdxA],#16\n"
                   " sasx %[u32ValB],%[u32ValA],%[u32ValA]\n"
                   " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 5 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 5-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. There must be two consecutive readable bytes in the memory following
              the last element of the input array. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[5] = {6, 3, 1, 4, 5};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin5_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00353 */
INLINE tU16 GFLIB_VMin5_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pIn],{r1,r2,r3}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " mov %[u32IdxB],#0x40004\n"
                    " pkhbt r3,r3,r3,LSL #16\n"
                    " ssub16 r2,r1,r3\n"
                    " sel r1,r3,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx r1,r1,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pIn],{r1,r2,r3}\n"
          " ssub16 %[u32IdxA],r1,r2\n"
          " sel r1,r2,r1\n"
          " mov %[u32IdxA],#0x10000\n"
          " add %[u32IdxB],%[u32IdxA],#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " mov %[u32IdxB],#0x40004\n"
          " pkhbt r3,r3,r3,LSL #16\n"
          " ssub16 r2,r1,r3\n"
          " sel r1,r3,r1\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx r1,r1,r1\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pIn],{r1,r2,r3}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " mov %[u32IdxB],#0x40004\n"
                    " pkhbt r3,r3,r3,LSL #16\n"
                    " ssub16 r2,r1,r3\n"
                    " sel r1,r3,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx r1,r1,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 6 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 6-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[6] = {6, 3, 1, 4, 5, 6};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin6_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00354 */
INLINE tU16 GFLIB_VMin6_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pIn],{r1,r2,r3}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ssub16 r2,r1,r3\n"
                    " sel r1,r3,r1\n"
                    " add %[u32IdxB],%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx r1,r1,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pIn],{r1,r2,r3}\n"
          " ssub16 %[u32IdxA],r1,r2\n"
          " sel r1,r2,r1\n"
          " mov %[u32IdxA],#0x10000\n"
          " add %[u32IdxB],%[u32IdxA],#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ssub16 r2,r1,r3\n"
          " sel r1,r3,r1\n"
          " add %[u32IdxB],%[u32IdxB],#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx r1,r1,r1\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32IdxA, u32IdxB;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pIn],{r1,r2,r3}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ssub16 r2,r1,r3\n"
                    " sel r1,r3,r1\n"
                    " add %[u32IdxB],%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx r1,r1,r1\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB)
    : [pIn]"r"(pIn)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 7 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 7-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[7] = {6, 3, 1, 4, 5, 6, 7};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin7_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00355 */
INLINE tU16 GFLIB_VMin7_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 8 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 8-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[8] = {6, 3, 1, 4, 5, 6, 7, 8};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin8_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00356 */
INLINE tU16 GFLIB_VMin8_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;

  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[pInTmp],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxB],%[u32IdxB],r2\n"
                    " ssub16 r2,%[u32ValA],r1\n"
                    " sel %[u32ValA],r1,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32IdxA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov %[u32IdxA],#0x10000\n"
          " add %[u32IdxB],%[u32IdxA],#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ldm %[pInTmp],{r1,r2}\n"
          " ssub16 %[pInTmp],r1,r2\n"
          " sel r1,r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxB],%[u32IdxB],r2\n"
          " ssub16 r2,%[u32ValA],r1\n"
          " sel %[u32ValA],r1,%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32IdxA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov %[u32IdxA],#0x10000\n"
                    " add %[u32IdxB],%[u32IdxA],#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[pInTmp],r1,r2\n"
                    " sel r1,r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxB],%[u32IdxB],r2\n"
                    " ssub16 r2,%[u32ValA],r1\n"
                    " sel %[u32ValA],r1,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 9 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 9-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. There must be two consecutive readable bytes in the memory following
              the last element of the input array. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[9] = {6, 3, 1, 4, 5, 6, 7, 8, 9};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin9_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00357 */
INLINE tU16 GFLIB_VMin9_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
    
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxB],%[u32IdxB],r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " pkhbt %[u32ValB],r3,r3,LSL #16\n"
                    " mov %[u32IdxB],#0x80008\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
    
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp],{r1,r2,r3}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxB],%[u32IdxB],r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " pkhbt %[u32ValB],r3,r3,LSL #16\n"
          " mov %[u32IdxB],#0x80008\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
    
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxB],%[u32IdxB],r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " pkhbt %[u32ValB],r3,r3,LSL #16\n"
                    " mov %[u32IdxB],#0x80008\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 10 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 10-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[10] = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin10_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00358 */
INLINE tU16 GFLIB_VMin10_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp],{r1,r2,r3}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ssub16 r2,%[u32ValA],r3\n"
          " sel %[u32ValA],r3,%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else

  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 11 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 11-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. There must be two consecutive readable bytes in the memory following
              the last element of the input array. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[11] = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10, 11};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin11_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00359 */
INLINE tU16 GFLIB_VMin11_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " pkhbt r2,r2,r2,LSL #16\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " mov r1,#0xA000A\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2}\n"
          " pkhbt r2,r2,r2,LSL #16\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " mov r1,#0xA000A\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " pkhbt r2,r2,r2,LSL #16\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " mov r1,#0xA000A\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 12 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 12-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[12] = {6, 3, 1, 4, 5, 6, 7, 8, 9,
      10, 11, 12};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin12_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00360 */
INLINE tU16 GFLIB_VMin12_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[11] <= pIn[u16Idx])
  {
    u16Idx = 11u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 13 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 13-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. There must be two consecutive readable bytes in the memory following
              the last element of the input array. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[13] = {6, 3, 1, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin13_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00361 */
INLINE tU16 GFLIB_VMin13_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " pkhbt r3,r3,r3,LSL #16\n"
                    " mov %[u32IdxB],#0xC000C\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2,r3}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " pkhbt r3,r3,r3,LSL #16\n"
          " mov %[u32IdxB],#0xC000C\n"
          " ssub16 r2,%[u32ValA],r3\n"
          " sel %[u32ValA],r3,%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
  
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " pkhbt r3,r3,r3,LSL #16\n"
                    " mov %[u32IdxB],#0xC000C\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[11] <= pIn[u16Idx])
  {
    u16Idx = 11u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[12] <= pIn[u16Idx])
  {
    u16Idx = 12u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 14 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 14-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[14] = {6, 3, 1, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13, 14};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin14_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00362 */
INLINE tU16 GFLIB_VMin14_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2,r3}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ssub16 r2,%[u32ValA],r3\n"
          " sel %[u32ValA],r3,%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2,r3}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ssub16 r2,%[u32ValA],r3\n"
                    " sel %[u32ValA],r3,%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2", "r3");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[11] <= pIn[u16Idx])
  {
    u16Idx = 11u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[12] <= pIn[u16Idx])
  {
    u16Idx = 12u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[13] <= pIn[u16Idx])
  {
    u16Idx = 13u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 15 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 15-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. There must be two consecutive readable bytes in the memory following
              the last element of the input array. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[15] = {6, 3, 1, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13, 14, 15};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin15_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00363 */
INLINE tU16 GFLIB_VMin15_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " pkhbt r2,r2,r2,LSL #16\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " mov r1,#0xE000E\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2}\n"
          " pkhbt r2,r2,r2,LSL #16\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " mov r1,#0xE000E\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;
      
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    #pragma ghs optasm
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " pkhbt r2,r2,r2,LSL #16\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " mov r1,#0xE000E\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[11] <= pIn[u16Idx])
  {
    u16Idx = 11u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[12] <= pIn[u16Idx])
  {
    u16Idx = 12u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[13] <= pIn[u16Idx])
  {
    u16Idx = 13u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[14] <= pIn[u16Idx])
  {
    u16Idx = 14u;
  }
  #endif
  return(u16Idx);
}


/***************************************************************************/
/**
@ingroup    GFLIB_GROUP

@functionality GFLIB_VMin

@param[in]    pIn   Pointer to an array of 16-bit fixed-point signed input values. The length
                    of the array must be 16 elements.

@return       The function returns the index of the smallest element of the input array.

@details      The function finds the minimum in a 16-element vector of values and returns the index
              of that value. If there are several equal minimums, the index of an arbitrary one of
              them is returned. This function is faster than #GFLIB_VMin_F16.

@par Code Example
\code
#include "gflib.h"

void main(void)
{
    tFrac16 pInputArray[16] = {6, 3, 1, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13, 14, 15, 16};
    tFrac16 f16MinValue;
    
    f16MinValue = pInputArray[GFLIB_VMin16_F16(pInputArray)];
}
\endcode
****************************************************************************/  
#if defined(__CWCC__) || defined(__MWERKS__)
#pragma always_INLINE on
#endif
#ifdef __ghs__
  #pragma ghs static_call 0
  #pragma ghs extra_stack 0
#endif
/* @remarks Implements DGFLIB00364 */
INLINE tU16 GFLIB_VMin16_F16(const tFrac16 *pIn)
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
{
/*
* @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
* file. 
*/
  tU16 u16Idx;
  #if (defined(__GNUC__) && defined(__thumb__))
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__IAR_SYSTEMS_ICC__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm(" ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValA],r1,r2\n"
          " sel %[u32ValA],r2,r1\n"
          " mov r2,#0x10000\n"
          " add %[u32IdxB],r2,#0x20002\n"
          " sel %[u32IdxA],%[u32IdxB],r2\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r2,%[u32IdxB],#0x20002\n"
          " add r1,r2,#0x20002\n"
          " sel %[u32IdxB],r1,r2\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp]!,{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " add %[u32IdxB],r1,#0x20002\n"
          " ldm %[pInTmp],{r1,r2}\n"
          " ssub16 %[u32ValB],r1,r2\n"
          " sel %[u32ValB],r2,r1\n"
          " add r1,%[u32IdxB],#0x20002\n"
          " sel %[u32IdxB],r1,%[u32IdxB]\n"
          " ssub16 r2,%[u32ValA],%[u32ValB]\n"
          " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
          " ror %[u32IdxB],%[u32IdxA],#16\n"
          " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
          " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"+r"(u32ValA), [u32ValB]"+r"(u32ValB),
    [u32IdxA]"+r"(u32IdxA), [u32IdxB]"+r"(u32IdxB)
    :[pInTmp]"r"(pInTmp)
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #elif defined(__ghs__)
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    tU32 u32ValA, u32ValB, u32IdxA, u32IdxB;
    /*
    * @violates @ref GFLIB_VMin_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in header 
    * file. 
    */
    const tFrac16 *pInTmp;

    #pragma ghs optasm
    pInTmp = pIn;
    #ifdef _lint
      u32IdxA = 0u; /* initialization of value for lint */
    #endif
    __asm volatile( " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValA],r1,r2\n"
                    " sel %[u32ValA],r2,r1\n"
                    " mov r2,#0x10000\n"
                    " add %[u32IdxB],r2,#0x20002\n"
                    " sel %[u32IdxA],%[u32IdxB],r2\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r2,%[u32IdxB],#0x20002\n"
                    " add r1,r2,#0x20002\n"
                    " sel %[u32IdxB],r1,r2\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp]!,{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " add %[u32IdxB],r1,#0x20002\n"
                    " ldm %[pInTmp],{r1,r2}\n"
                    " ssub16 %[u32ValB],r1,r2\n"
                    " sel %[u32ValB],r2,r1\n"
                    " add r1,%[u32IdxB],#0x20002\n"
                    " sel %[u32IdxB],r1,%[u32IdxB]\n"
                    " ssub16 r2,%[u32ValA],%[u32ValB]\n"
                    " sel %[u32ValA],%[u32ValB],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]\n"
                    " ror %[u32IdxB],%[u32IdxA],#16\n"
                    " sasx %[u32ValA],%[u32ValA],%[u32ValA]\n"
                    " sel %[u32IdxA],%[u32IdxB],%[u32IdxA]"
    : [u32ValA]"=r"(u32ValA), [u32ValB]"=r"(u32ValB),
    [u32IdxA]"=r"(u32IdxA), [u32IdxB]"=r"(u32IdxB),
    [pInTmp]"+r"(pInTmp)
    :
    : "cc", "r1", "r2");
    u16Idx = (tU16)u32IdxA;
  #else
  
  u16Idx = 0u;
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[1] <= pIn[u16Idx])
  {
    u16Idx = 1u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[2] <= pIn[u16Idx])
  {
    u16Idx = 2u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[3] <= pIn[u16Idx])
  {
    u16Idx = 3u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[4] <= pIn[u16Idx])
  {
    u16Idx = 4u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[5] <= pIn[u16Idx])
  {
    u16Idx = 5u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[6] <= pIn[u16Idx])
  {
    u16Idx = 6u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[7] <= pIn[u16Idx])
  {
    u16Idx = 7u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[8] <= pIn[u16Idx])
  {
    u16Idx = 8u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[9] <= pIn[u16Idx])
  {
    u16Idx = 9u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[10] <= pIn[u16Idx])
  {
    u16Idx = 10u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[11] <= pIn[u16Idx])
  {
    u16Idx = 11u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[12] <= pIn[u16Idx])
  {
    u16Idx = 12u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[13] <= pIn[u16Idx])
  {
    u16Idx = 13u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[14] <= pIn[u16Idx])
  {
    u16Idx = 14u;
  }
/*
* @violates @ref GFLIB_VMin_h_REF_6 MISRA 2004 Required Rule 17.4, Pointer arithmetic other
* than array indexing used.
*/
  if(pIn[15] <= pIn[u16Idx])
  {
    u16Idx = 15u;
  }
  #endif
  return(u16Idx);
}
#endif /* ifndef RTWTYPES_H */
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */



#ifdef __cplusplus
}
#endif

#endif /* GFLIB_VMIN_H */
