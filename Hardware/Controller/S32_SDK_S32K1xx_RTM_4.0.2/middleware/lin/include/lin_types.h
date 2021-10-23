/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#ifndef LIN_TYPES_H
#define LIN_TYPES_H

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Unsigned 8 bit integer
 * Implements : l_u8_Class
 */
typedef unsigned char l_u8;

/*!
 * @brief Unsigned 16 bit integer
 * Implements : l_u16_Class
 */
typedef unsigned short int l_u16;

/*!
 * @brief Unsigned 32 bit integer
 * Implements : l_u32_Class
 */
typedef unsigned long l_u32;

/*!
 * @brief 0 is false, and non-zero (>0) is true
 * Implements : l_bool_Class
 */
typedef bool l_bool;

/*******************************************************************************
 * General
 ******************************************************************************/
/*!
 * @brief Product id structure
 * Implements : lin_product_id_t_Class
 */
typedef struct
{
    l_u16   supplier_id;        /*!< Supplier ID */
    l_u16   function_id;        /*!< Function ID */
    l_u8    variant;            /*!< Variant value */
} lin_product_id_t;

#endif /* LIN_TYPES_H */


/******************************************************************************/
/* EOF */
/******************************************************************************/
