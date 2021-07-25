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
 * This file is derived from the Ethernet Interface Skeleton in lwIP with the following copyright:
 *
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 */

#ifndef ENETIF_H
#define ENETIF_H

#include "lwip/err.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "enet_driver.h"

/*! @brief The ENET instance that you want to work on */
#define ENET_INSTANCE       0
#define ENET_QUEUE          0

/*! @brief Number of Rx and Tx descriptors */
#ifndef ENET_RXBD_NUM
#define ENET_RXBD_NUM       6U
#endif
#ifndef ENET_TXBD_NUM
#define ENET_TXBD_NUM       2U
#endif

/*! @brief Receive buffer descriptors */
#define ENET_RXBUFF_SIZE    ENET_BUFF_ALIGN(ENET_FRAME_MAX_FRAMELEN)

/* Code returned by the pre-input handler in the case when the frame should be forwarded to the stack */
#define FORWARD_FRAME   (0U)

typedef unsigned int (*rx_buff_process_condition_handler_t)(uint8_t eth_instance, enet_buffer_t *buff);


extern mutex_t enetif_tx_lock;

err_t enet_ethernetif_init(struct netif *netif);
void enet_ethernetif_shutdown(struct netif *netif);

#if LWIP_IPV6
err_t mld_enet_filter (struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action);
#endif /*LWIP_IPV6*/

#if LWIP_IGMP
err_t igmp_enet_filter (struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action);
#endif /*LWIP_IGMP*/

#if NO_SYS
err_t enet_poll_interface(struct netif *netif);
#endif /* NO_SYS */

#ifdef COVERAGE_ENABLED
/*!
 * @brief Send one byte to transfer buffer
 *
 * This function sends one byte to transfer buffer
 */
void send_tx_pbuffs_dummy_char(void);
/*!
 * @brief Send one byte to receive buffer
 *
 * This function sends one byte to receive buffer
 */
void send_rx_pbuffs_dummy_char(void);
#endif /*COVERAGE_ENABLED*/


void enetif_register_rx_buff_process_condition_handler(rx_buff_process_condition_handler_t handler);

/* API's exposed to the Application */
#define ETHIF_INIT enet_ethernetif_init
#define ETHIF_SHUTDOWN enet_ethernetif_shutdown
#define ETHIF_BUFFER_t enet_buffer_t
#define ETHIF_POLL_INTERFACE enet_poll_interface
#define ETHIF_REGISTER_RX_BUFF_PROCESS_CONDITION_HANDLER \
	enetif_register_rx_buff_process_condition_handler

#endif /* ENETIF_H */
