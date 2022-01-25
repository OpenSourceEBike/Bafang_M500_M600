/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2021 NXP
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

#ifndef LIN_COMMONTL_PROTO_H
#define LIN_COMMONTL_PROTO_H

#include "lin.h"
#if (SUPPORT_TRANSPORT_LAYER == 1U)
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* PCI type */
#define PCI_SF                          0x00U       /*!< Single Frame */
#define PCI_FF                          0x01U       /*!< First Frame */
#define PCI_CF                          0x02U       /*!< Consecutive Frame */

/* COMMON FUNCTIONS */
/*
 * @brief Pack the information specified by data and length into one or multiple pdus.
 *
 * @param[in] iii Lin interface handle
 * @param[in] length Length of data to package
 * @param[in] message_size Size of message to package
 * @param[in] NAD Node address of slave node
 * @param[in] data Data to be package
 * @return void
 */
void lin_tl_package_pdu(l_ifc_handle iii,
                        l_u16 length,
                        l_u8 message_size,
                        l_u8 NAD,
                        const l_u8 * const data);
/*
 * @brief Copy the specified raw data to the specified queue.
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data
 * @param[in] *queue Queue for containing data
 * @param[in] direction Direction
 * @return void
 */
void tl_put_raw(l_ifc_handle iii,
                const l_u8 * const data,
                lin_transport_layer_queue_t * queue,
                l_u8 direction);

/*
 * @brief Get the raw data from the specified queue to
 * the specified raw data
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data
 * @param[in] *queue Queue for containing data
 * @param[in] direction Direction
 * @return void
 */
void tl_get_raw(l_ifc_handle iii,
                l_u8 * const data,
                lin_transport_layer_queue_t * queue,
                l_u8 direction);

#endif /*end (SUPPORT_TRANSPORT_LAYER == 1U) */
#endif /* End of LIN_COMMONTL_PROTO_H */

/*! @} */

/******************************************************************************/
/* EOF */
/******************************************************************************/
