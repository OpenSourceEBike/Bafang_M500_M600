/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

#ifndef FLOATS_H
#define FLOATS_H

/*!
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Float types defined for modules that require them.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * Float types defined for modules that require them.
 *
 *
 */

#ifdef __cplusplus
extern "C"{
#endif

/**
@brief 32bit long floating point data type
*/
typedef float float32_t;

/**
@brief 64bit long floating point data type
*/
typedef double float64_t;

#ifdef __cplusplus
}
#endif

#endif /* FLOATS_H */
