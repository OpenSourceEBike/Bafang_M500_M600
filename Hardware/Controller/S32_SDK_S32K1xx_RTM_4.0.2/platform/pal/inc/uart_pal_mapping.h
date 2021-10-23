/*
 * Copyright 2017-2020 NXP
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

/*!
 * @file uart_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef UART_PAL_MAPPING_H
#define UART_PAL_MAPPING_H
#include "device_registers.h"

#if (defined(S32K14x_SERIES) || defined(S32K11x_SERIES))
#define FLEXIO_UART_INSTANCE_COUNT    2U
#endif

/*! @brief Default baudrate value for UART */
#define UART_DEFAULT_BAUDRATE_VALUE   9600U

/*!
 * @brief Enumeration with the types of peripherals supported by UART PAL
 *
 * This enumeration contains the types of peripherals supported by UART PAL.
 * Implements : uart_inst_type_t_Class
 */
typedef enum
{
#if (defined(LPUART_INSTANCE_COUNT))
    UART_INST_TYPE_LPUART       = 0u,
#endif /* defined(LPUART_INSTANCE_COUNT) */

#if (defined(FLEXIO_UART_INSTANCE_COUNT))
    UART_INST_TYPE_FLEXIO_UART  = 1u,
#endif /* defined(FLEXIO_UART_INSTANCE_COUNT) */

#if (defined(LINFlexD_INSTANCE_COUNT))
    UART_INST_TYPE_LINFLEXD     = 2u
#endif /* defined(LINFlexD_INSTANCE_COUNT) */
} uart_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : uart_instance_t_Class
 */
typedef struct
{
    uart_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;             /*!< Instance index of the peripheral over which the PAL is used */
} uart_instance_t;

#endif /* UART_PAL_MAPPING_H */
