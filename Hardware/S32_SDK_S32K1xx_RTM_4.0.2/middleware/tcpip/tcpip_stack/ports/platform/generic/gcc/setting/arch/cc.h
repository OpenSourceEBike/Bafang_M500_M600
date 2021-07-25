/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 *
 * This file is derived from lwIP contribution example with the following copyright:
 *
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, A function should be used in preference
 * to a function-like macro where they are interchangeable.
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 */

#ifndef LWIP_ARCH_CC_H
#define LWIP_ARCH_CC_H

#include "device_registers.h"
#include <stdlib.h>
#include "lwipcfg.h"

#define LWIP_PROVIDE_ERRNO              1

typedef int sys_prot_t;

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
  handler;} } while(0)

#ifndef LWIP_PLATFORM_DIAG
#ifdef CPU_SJA1110
#include <stdio.h>
#define U8_F "c"
#define S8_F "c"
#define X8_F "x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define LWIP_PLATFORM_DIAG(x)  do {printf x;} while(0)
#else
#define LWIP_PLATFORM_DIAG(x) do {} while(0)
#endif /* CPU_SJA1110 */
#endif /* LWIP_PLATFORM_DIAG */

#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) do { BKPT_ASM; } while(1)
#endif /* LWIP_PLATFORM_ASSERT */

/* C runtime functions redefined */
#define LWIP_RAND() ((u32_t)rand())

#define PPP_INCLUDE_SETTINGS_HEADER

#endif /* LWIP_ARCH_CC_H */
