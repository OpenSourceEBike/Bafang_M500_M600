/*
 * Copyright 2017,2020 NXP
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

#ifndef ENET_H
#define ENET_H

#include "device_registers.h"
#include "status.h"
#include "osif.h"
#include <stdlib.h>

/* */
/* */
/* */

/*!
 * @file enet_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro might be used by the user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * The specified typedefs might be used in the user code for composing masks
 * which could be passed to some of the ENET driver functions.
 *
 */

/*!
 * @defgroup enet_driver ENET Driver
 * @ingroup enet
 * @addtogroup enet_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the maximum Ethernet frame size. */
#define ENET_FRAME_MAX_FRAMELEN         1518U
/*! @brief ENET minimum buffer size. */
#define ENET_MIN_BUFFERSIZE             64U

/*! @brief Definitions used for aligning the data buffers */
#define ENET_BUFF_ALIGN(x)       (((uint32_t)(x) + (FEATURE_ENET_BUFF_ALIGNMENT - 1UL)) & ~(FEATURE_ENET_BUFF_ALIGNMENT - 1UL))
#define ENET_BUFF_IS_ALIGNED(x)  (((uint32_t)(x) & ~(FEATURE_ENET_BUFF_ALIGNMENT - 1UL)) != 0UL)

/*! @brief Definitions used for aligning the buffer descriptors */
#define ENET_BUFFDESCR_ALIGN(x)       (((uint32_t)(x) + (FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL)) & ~(FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL))
#define ENET_BUFFDESCR_IS_ALIGNED(x)  (((uint32_t)(x) & ~(FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL)) != 0UL)

#define ENET_TX_WATERMARK_SHIFT       (6U)
#define ENET_TX_WATERMARK_MASK        (0x3FU)

/*!
 * @brief Media Independent Interface mode selection
 * Implements : enet_mii_mode_t_Class
 */
typedef enum
{
    ENET_MII_MODE = 0U, /*!< MII mode for data interface. */
    ENET_RMII_MODE,     /*!< RMII mode for data interface. */
#if FEATURE_ENET_HAS_RGMII
	ENET_RGMII_MODE
#endif /* FEATURE_ENET_HAS_RGMII */
} enet_mii_mode_t;

/*!
 * @brief Media Independent Interface speed selection
 * Implements : enet_mii_speed_t_Class
 */
typedef enum
{
    ENET_MII_SPEED_10M = 0U,  /*!< Speed 10 Mbps. */
    ENET_MII_SPEED_100M,      /*!< Speed 100 Mbps. */
#if FEATURE_ENET_HAS_SPEED_1000M
	ENET_MII_SPEED_1000M      /*!< Speed 1000 Mbps. */
#endif /* FEATURE_ENET_HAS_SPEED_1000M */
} enet_mii_speed_t;

/*!
 * @brief Media Independent Interface full-/half-duplex selection
 * Implements : enet_mii_duplex_t_Class
 */
typedef enum
{
    ENET_MII_HALF_DUPLEX = 0U, /*!< Half-duplex mode. */
    ENET_MII_FULL_DUPLEX       /*!< Full-duplex mode. */
} enet_mii_duplex_t;

#if FEATURE_ENET_HAS_RX_CONFIG

/*!
 * @brief Special receive control configurations
 * Implements : enet_rx_special_config_t_Class
 */
typedef enum
{
#if FEATURE_ENET_RX_CONFIG_PAYLOAD_LEN_CHECK
    ENET_RX_CONFIG_ENABLE_PAYLOAD_LEN_CHECK  = 0x0001U,
#endif /* FEATURE_ENET_RX_CONFIG_PAYLOAD_LEN_CHECK */
#if FEATURE_ENET_RX_CONFIG_STRIP_CRC_FIELD
    ENET_RX_CONFIG_STRIP_CRC_FIELD           = 0x0002U,
#endif /* FEATURE_ENET_RX_CONFIG_STRIP_CRC_FIELD */
#if FEATURE_ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES
    ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES      = 0x0004U,
#endif /* FEATURE_ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES */
#if FEATURE_ENET_RX_CONFIG_REMOVE_PADDING
    ENET_RX_CONFIG_REMOVE_PADDING            = 0x0008U,
#endif /* FEATURE_ENET_RX_CONFIG_REMOVE_PADDING */
    ENET_RX_CONFIG_ENABLE_FLOW_CONTROL       = 0x0010U,
    ENET_RX_CONFIG_REJECT_BROADCAST_FRAMES   = 0x0020U,
    ENET_RX_CONFIG_ENABLE_PROMISCUOUS_MODE   = 0x0040U,
    ENET_RX_CONFIG_ENABLE_MII_LOOPBACK       = 0x0080U
} enet_rx_special_config_t;

#endif /* FEATURE_ENET_HAS_RX_CONFIG */

#if FEATURE_ENET_HAS_TX_CONFIG

/*!
 * @brief Special transmit control configurations
 * Implements : enet_tx_special_config_t_Class
 */
typedef enum
{
    ENET_TX_CONFIG_DISABLE_CRC_APPEND           = 0x0001U,
    ENET_TX_CONFIG_ENABLE_MAC_ADDR_INSERTION    = 0x0002U,
} enet_tx_special_config_t;

#endif /* FEATURE_ENET_HAS_TX_CONFIG */

/*!
 * @brief Interrupt sources
 * Implements : enet_interrupt_t_Class
 */
typedef enum
{
    ENET_BABR_INTERRUPT = ENET_EIR_BABR_MASK,
    ENET_BABT_INTERRUPT = ENET_EIR_BABT_MASK,
    ENET_GRACE_STOP_INTERRUPT = ENET_EIR_GRA_MASK,
    ENET_TX_FRAME_INTERRUPT = ENET_EIR_TXF_MASK,
    ENET_TX_BUFFER_INTERRUPT = ENET_EIR_TXB_MASK,
    ENET_RX_FRAME_INTERRUPT = ENET_EIR_RXF_MASK,
    ENET_RX_BUFFER_INTERRUPT = ENET_EIR_RXB_MASK,
    ENET_MII_INTERRUPT = ENET_EIR_MII_MASK,
    ENET_EBERR_INTERRUPT = ENET_EIR_EBERR_MASK,
    ENET_LATE_COLLISION_INTERRUPT = ENET_EIR_LC_MASK,
    ENET_RETRY_LIMIT_INTERRUPT = ENET_EIR_RL_MASK,
    ENET_UNDERRUN_INTERRUPT = ENET_EIR_UN_MASK,
#ifdef ENET_EIR_PLR_MASK
    ENET_PAYLOAD_RX_INTERRUPT = ENET_EIR_PLR_MASK,
#endif /* ENET_EIR_PLR_MASK */
#ifdef ENET_EIR_WAKEUP_MASK
    ENET_WAKEUP_INTERRUPT = ENET_EIR_WAKEUP_MASK,
#endif /* ENET_EIR_WAKEUP_MASK */
#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER
    ENET_TS_AVAIL_INTERRUPT = ENET_EIR_TS_AVAIL_MASK,
    ENET_TS_TIMER_INTERRUPT = ENET_EIR_TS_TIMER_MASK,
#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */
#if (FEATURE_ENET_RING_COUNT > 1U)
    ENET_RX_FLUSH_2_INTERRUPT = ENET_EIR_RXFLUSH_2_MASK,
    ENET_RX_FLUSH_1_INTERRUPT = ENET_EIR_RXFLUSH_1_MASK,
    ENET_RX_FLUSH_0_INTERRUPT = ENET_EIR_RXFLUSH_0_MASK,
    ENET_TX_FRAME_1_INTERRUPT = ENET_EIR_TXF1_MASK,
    ENET_TX_BUFFER_1_INTERRUPT = ENET_EIR_TXB1_MASK,
    ENET_RX_FRAME_1_INTERRUPT = ENET_EIR_RXF1_MASK,
    ENET_RX_BUFFER_1_INTERRUPT = ENET_EIR_RXB1_MASK,
    ENET_TX_FRAME_2_INTERRUPT = ENET_EIR_TXF2_MASK,
    ENET_TX_BUFFER_2_INTERRUPT = ENET_EIR_TXB2_MASK,
    ENET_RX_FRAME_2_INTERRUPT = ENET_EIR_RXF2_MASK,
    ENET_RX_BUFFER_2_INTERRUPT = ENET_EIR_RXB2_MASK,
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
#if FEATURE_ENET_HAS_RECEIVE_PARSER
    ENET_PARSERERR_INTERRUPT = ENET_EIR_PARSERR_MASK,
    ENET_PARSRF_INTERRUPT = ENET_EIR_PARSRF_MASK,
#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */
} enet_interrupt_t;

#if FEATURE_ENET_HAS_ACCELERATOR

/*!
 * @brief Transmit accelerator configurations
 * Implements : enet_tx_accelerator_t_Class
 */
typedef enum
{
    ENET_TX_ACCEL_ENABLE_SHIFT16        = ENET_TACC_SHIFT16_MASK,
    ENET_TX_ACCEL_INSERT_IP_CHECKSUM    = ENET_TACC_IPCHK_MASK,
    ENET_TX_ACCEL_INSERT_PROTO_CHECKSUM = ENET_TACC_PROCHK_MASK
} enet_tx_accelerator_t;

/*!
 * @brief Receive accelerator configurations
 * Implements : enet_rx_accelerator_t_Class
 */
typedef enum
{
    ENET_RX_ACCEL_REMOVE_PAD         = ENET_RACC_PADREM_MASK,
    ENET_RX_ACCEL_ENABLE_IP_CHECK    = ENET_RACC_IPDIS_MASK,
    ENET_RX_ACCEL_ENABLE_PROTO_CHECK = ENET_RACC_PRODIS_MASK,
    ENET_RX_ACCEL_ENABLE_MAC_CHECK   = ENET_RACC_LINEDIS_MASK,
    ENET_RX_ACCEL_ENABLE_SHIFT16     = ENET_RACC_SHIFT16_MASK
} enet_rx_accelerator_t;

#endif /* FEATURE_ENET_HAS_ACCELERATOR */

/*!
 * @brief Send/Receive buffer information for the user
 * Implements : enet_buffer_t_Class
 */
typedef struct
{
    uint8_t *data;
    uint16_t length;
} enet_buffer_t;

#if (defined(CORE_BIG_ENDIAN))
/*!
 * @brief Send/Receive internal buffer descriptor
 * Implements : enet_buffer_descriptor_t_Class
 */
typedef struct
{
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#if FEATURE_ENET_HAS_ENHANCED_BD
    uint32_t enh1;    /*!< First word of the extended part of the buffer descriptor. */
    uint32_t enh2;    /*!< Second word of the extended part of the buffer descriptor. */
    uint32_t enh3;    /*!< Third word of the extended part of the buffer descriptor. */
    uint32_t timestamp;  /*!< Timestamp of the frame. */
    uint32_t reserved1;  /*!< Reserved. */
    uint32_t reserved2;  /*!< Reserved. */
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
} enet_buffer_descriptor_t;
#elif (defined(CORE_LITTLE_ENDIAN))
/*!
 * @brief Send/Receive internal buffer descriptor
 * Implements : enet_buffer_descriptor_t_Class
 */
typedef struct
{
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#if FEATURE_ENET_HAS_ENHANCED_BD
    uint32_t enh1;    /*!< First word of the extended part of the buffer descriptor. */
    uint32_t enh2;    /*!< Second word of the extended part of the buffer descriptor. */
    uint32_t enh3;    /*!< Third word of the extended part of the buffer descriptor. */
    uint32_t timestamp;  /*!< Timestamp of the frame. */
    uint32_t reserved1;  /*!< Reserved. */
    uint32_t reserved2;  /*!< Reserved. */
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
} enet_buffer_descriptor_t;
#else
    #error "Endianness not defined!"
#endif

/*!
 * @brief Event specifier for the callback function
 * Implements : enet_event_t_Class
 */
typedef enum
{
    ENET_RX_EVENT,
    ENET_TX_EVENT,
    ENET_ERR_EVENT,
    ENET_WAKE_UP_EVENT,
    ENET_PARSER_EVENT
} enet_event_t;

/*!
 * @brief Callback function invoked when one of the events in "enet_event_t" is encountered
 * Implements : enet_callback_t_Class
 */
typedef void (*enet_callback_t)(uint8_t instance, enet_event_t event, uint8_t ring);

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*!
 * @brief Callback function invoked when a timer event is encountered. The channels parameter
 * represents a mask of the channels on which an event occurred.
 *
 * Implements : enet_timer_callback_t_Class
 */
typedef void (*enet_timer_callback_t)(uint8_t instance, uint8_t channels);

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

/*!
 * @brief Defines the ENET buffer descriptors ring configuration structure
 * Implements : enet_buffer_config_t_Class
 */
typedef struct
{
    uint16_t rxRingSize;                              /*!< Receive buffer descriptors number. */
    uint16_t txRingSize;                              /*!< Transmit buffer descriptors number. */
    enet_buffer_descriptor_t *rxRingAligned;          /*!< Aligned receive buffer descriptor ring start address. */
    enet_buffer_descriptor_t *txRingAligned;          /*!< Aligned transmit buffer descriptor ring start address. */
    uint8_t *rxBufferAligned;                         /*!< Receive data buffers start address. */
} enet_buffer_config_t;

/*!
 * @brief Defines the ENET module configuration structure
 * Implements : enet_config_t_Class
 */
typedef struct
{
#if FEATURE_ENET_HAS_ACCELERATOR
    /* Accelerator configuration */
    uint8_t rxAccelerConfig;      /*!< Receive accelerator, A logical OR of "enet_rx_accelerator_t". */
    uint8_t txAccelerConfig;      /*!< Transmit accelerator, A logical OR of "enet_tx_accelerator_t". */
#endif /* FEATURE_ENET_HAS_ACCELERATOR */
    /* Basic configuration */
    uint16_t maxFrameLen;         /*!< Maximum frame length. */
    uint32_t interrupts;          /*!< MAC interrupt source. A logical OR of "enet_interrupt_t". */
    /* MII configuration */
    enet_mii_mode_t miiMode;      /*!< MII mode. */
    enet_mii_speed_t miiSpeed;    /*!< MII Speed. */
    enet_mii_duplex_t miiDuplex;  /*!< MII duplex. */
    /* Receive and transmit special configurations */
#if FEATURE_ENET_HAS_RX_CONFIG
    uint32_t rxConfig;    /*!< MAC receive special configuration. A logical OR of "enet_rx_special_config_t". */
#endif /* FEATURE_ENET_HAS_RX_CONFIG */
#if FEATURE_ENET_HAS_TX_CONFIG
    uint32_t txConfig;    /*!< MAC transmit special configuration. A logical OR of "enet_tx_special_config_t". */
#endif /* FEATURE_ENET_HAS_TX_CONFIG */
    /* Callback function */
    enet_callback_t callback;
    uint8_t ringCount;    /*!< The number of rings to be used by the driver. */
} enet_config_t;

/*!
 * @brief Internal driver state structure
 * Implements : enet_state_t_Class
 */
typedef struct
{
    enet_buffer_descriptor_t *rxBdBase[FEATURE_ENET_RING_COUNT];    /*!< Receive buffer descriptor base address pointer array. */
    enet_buffer_descriptor_t *rxBdCurrent[FEATURE_ENET_RING_COUNT]; /*!< The current available receive buffer descriptor pointer array. */
    enet_buffer_descriptor_t *rxBdAlloc[FEATURE_ENET_RING_COUNT];   /*!< Pointer array used for enqueuing Rx buffers provided using ENET_DRV_ProvideRxBuff. */
    enet_buffer_descriptor_t *txBdBase[FEATURE_ENET_RING_COUNT];    /*!< Transmit buffer descriptor base address pointer array. */
    enet_buffer_descriptor_t *txBdCurrent[FEATURE_ENET_RING_COUNT]; /*!< The current available transmit buffer descriptor pointer array. */
    enet_callback_t callback;              /*!< Callback function. */
#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER
    enet_timer_callback_t timerCallback;   /*!< Timer callback function. */
#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */
    uint8_t ringCount;                     /*!< The number of rings used by the driver. */
} enet_state_t;

/*!
 * @brief Statistics counters enumeration
 * Implements : enet_counter_t_Class
 */
typedef enum
{
    ENET_CTR_RMON_T_DROP = 0U,
    ENET_CTR_RMON_T_PACKETS,
    ENET_CTR_RMON_T_BC_PKT,
    ENET_CTR_RMON_T_MC_PKT,
    ENET_CTR_RMON_T_CRC_ALIGN,
    ENET_CTR_RMON_T_UNDERSIZE,
    ENET_CTR_RMON_T_OVERSIZE,
    ENET_CTR_RMON_T_FRAG,
    ENET_CTR_RMON_T_JAB,
    ENET_CTR_RMON_T_COL,
    ENET_CTR_RMON_T_P64,
    ENET_CTR_RMON_T_P65TO127,
    ENET_CTR_RMON_T_P128TO255,
    ENET_CTR_RMON_T_P256TO511,
    ENET_CTR_RMON_T_P512TO1023,
    ENET_CTR_RMON_T_P1024TO2047,
    ENET_CTR_RMON_T_P_GTE2048,
    ENET_CTR_RMON_T_OCTETS,
    ENET_CTR_IEEE_T_DROP,
    ENET_CTR_IEEE_T_FRAME_OK,
    ENET_CTR_IEEE_T_1COL,
    ENET_CTR_IEEE_T_MCOL,
    ENET_CTR_IEEE_T_DEF,
    ENET_CTR_IEEE_T_LCOL,
    ENET_CTR_IEEE_T_EXCOL,
    ENET_CTR_IEEE_T_MACERR,
    ENET_CTR_IEEE_T_CSERR,
    ENET_CTR_IEEE_T_SQE,
    ENET_CTR_IEEE_T_FDXFC,
    ENET_CTR_IEEE_T_OCTETS_OK = 29U,
    ENET_CTR_RMON_R_PACKETS = 33U,
    ENET_CTR_RMON_R_BC_PKT,
    ENET_CTR_RMON_R_MC_PKT,
    ENET_CTR_RMON_R_CRC_ALIGN,
    ENET_CTR_RMON_R_UNDERSIZE,
    ENET_CTR_RMON_R_OVERSIZE,
    ENET_CTR_RMON_R_FRAG,
    ENET_CTR_RMON_R_JAB,
    ENET_CTR_RMON_R_RESVD_0,
    ENET_CTR_RMON_R_P64,
    ENET_CTR_RMON_R_P65TO127,
    ENET_CTR_RMON_R_P128TO255,
    ENET_CTR_RMON_R_P256TO511,
    ENET_CTR_RMON_R_P512TO1023,
    ENET_CTR_RMON_R_P1024TO2047,
    ENET_CTR_RMON_R_P_GTE2048,
    ENET_CTR_RMON_R_OCTETS,
    ENET_CTR_IEEE_R_DROP,
    ENET_CTR_IEEE_R_FRAME_OK,
    ENET_CTR_IEEE_R_CRC,
    ENET_CTR_IEEE_R_ALIGN,
    ENET_CTR_IEEE_R_MACERR,
    ENET_CTR_IEEE_R_FDXFC,
    ENET_CTR_IEEE_R_OCTETS_OK
} enet_counter_t;

/*!
 * @brief Enhanced information related to a receive buffer descriptor
 * Implements : enet_rx_enh_info_t_Class
 */
typedef struct {
    uint32_t errMask;    /*!< Mask of ENET_RX_ENH_ERR_*, representing the errors that occurred. */
    uint32_t timestamp;  /*!< Timestamp of the received frame. */
    uint16_t checksum;   /*!< One's complement sum of the payload section of the IP frame. */
    uint8_t vlanPrio;    /*!< Frame priority level. Valid values are from 0 (best effort) to 7 (highest). */
    uint8_t headerLen;   /*!< Number of 32-bit words found within the IP and its following protocol headers. */
    uint8_t proto;       /*!< The 8-bit protocol field found within the IP header of the frame. */
    bool unicast;        /*!< Indicates that the frame is an unicast frame. */
    bool ipv6;           /*!< Indicates that the frame is an IPv6 frame. */
    bool ipv4Frag;       /*!< Indicates that the frame is an IPv4 fragment frame. */
} enet_rx_enh_info_t;

/*!
 * @brief Enhanced information related to a transmit buffer descriptor
 * Implements : enet_tx_enh_info_t_Class
 */
typedef struct {
    uint32_t errMask;    /*!< Mask of ENET_TX_ENH_ERR_*, representing the errors that occurred. */
    uint32_t timestamp;  /*!< Timestamp of the transmitted frame. */
} enet_tx_enh_info_t;


#define ENET_RX_ENH_ERR_PROTO_CHECKSUM   (0x00000010U) /*!< Set when the checksum of the protocol is invalid or an unknown
                                                            protocol is found and checksumming could not be performed. */
#define ENET_RX_ENH_ERR_IPHDR_CHECKSUM   (0x00000020U) /*!< Set when either a non-IP frame is received or the IP header checksum
                                                            was invalid. */
#define ENET_RX_ENH_ERR_COLLISION        (0x02000000U) /*!< Set when the frame was received with a collision detected during
                                                            reception. */
#define ENET_RX_ENH_ERR_PHY              (0x04000000U) /*!< Set when the frame was received with an Error character on the PHY
                                                            interface.*/
#define ENET_RX_ENH_ERR_MAC              (0x80000000U) /*!< Set when the frame stored in the system memory was received with an
                                                            error (typically, a receive FIFO overflow). */

#define ENET_TX_ENH_ERR_TIMESTAMP        (0x00000100U) /*!< Indicates that the MAC reported a different frame type than a timestamp
                                                            frame. */
#define ENET_TX_ENH_ERR_OVERFLOW         (0x00000200U) /*!< Indicates that the MAC reported that there was a FIFO overflow condition
                                                            on transmit. */
#define ENET_TX_ENH_ERR_LATE_COLLISION   (0x00000400U) /*!< Indicates that the MAC reported that there was a Late Collision on
                                                            transmit. */
#define ENET_TX_ENH_ERR_FRAME            (0x00000800U) /*!< Indicates that the MAC reported that the uDMA reported an error when
                                                            providing the packet. */
#define ENET_TX_ENH_ERR_EXCESS_COLLISION (0x00001000U) /*!< Indicates that the MAC reported an excess collision error on
                                                            transmit.  */
#define ENET_TX_ENH_ERR_UNDERFLOW        (0x00002000U) /*!< Indicates that the MAC reported an underflow error on transmit. */

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*!
 * @brief Configuration of the adjustable timer.
 * Implements : enet_timer_config_t_Class
 */
typedef struct {
    uint32_t timerPeriod;            /*!< When the timer reaches this value, the period event occurs and the timer restarts. */
    uint8_t  timerInc;               /*!< The timer increments by this amount each clock cycle. */
    uint32_t correctionPeriod;       /*!< Defines after how many timer clock cycles the correction counter should be reset and
                                          triggers a correction increment on the timer.*/
    uint8_t  correctionInc;          /*!< This value is added every time the correction timer expires. */
    bool irqEnable;                  /*!< Generate an interrupt when a timer event occurs. */
    enet_timer_callback_t callback;  /*!< Callback to be invoked when a timer event occurs. */
} enet_timer_config_t;

/*!
 * @brief Timer channels configuration modes.
 * Implements : enet_timer_channel_mode_t_Class
 */
typedef enum {
    ENET_TIMER_IC_RISING_EDGE = 1U,               /*!< Timer Channel is configured for Input Capture on rising edge. */
    ENET_TIMER_IC_FALLING_EDGE = 2U,              /*!< Timer Channel is configured for Input Capture on falling edge. */
    ENET_TIMER_IC_BOTH_EDGES = 3U,                /*!< Timer Channel is configured for Input Capture on both edges. */
    ENET_TIMER_OC_SOFTWARE = 4U,                  /*!< Timer Channel is configured for Output Compare - software only. */
    ENET_TIMER_OC_TOGGLE_ON_COMP = 5U,            /*!< Timer Channel is configured for Output Compare - toggle output on compare. */
    ENET_TIMER_OC_CLEAR_ON_COMP = 6U,             /*!< Timer Channel is configured for Output Compare - clear output on compare. */
    ENET_TIMER_OC_SET_ON_COMP = 7U,               /*!< Timer Channel is configured for Output Compare - set output on compare. */
    ENET_TIMER_OC_CLEAR_ON_COMP_SET_ON_OVF = 10U, /*!< Timer Channel is configured for Output Compare - clear output on compare,
                                                       set output on overflow. */
    ENET_TIMER_OC_SET_ON_COMP_CLEAR_ON_OVF = 11U, /*!< Timer Channel is configured for Output Compare - set output on compare,
                                                       clear output on overflow. */
    ENET_TIMER_OC_PULSE_LOW_ON_COMP = 14U,        /*!< Timer Channel is configured for Output Compare - pulse output low on compare. */
    ENET_TIMER_OC_PULSE_HIGH_ON_COMP = 15U        /*!< Timer Channel is configured for Output Compare - pulse output high on compare. */
} enet_timer_channel_mode_t;

/*!
 * @brief Configuration structure of the timer channels.
 * Implements : enet_timer_channel_config_t_Class
 */
typedef struct {
    enet_timer_channel_mode_t mode; /*!< The mode in which the timer channel shall be configured. */
    bool irqEnable;                 /*!< Enable timer interrupt for this channel. Note:  When enabling a channel interrupt,
                                         the timer interrupt must also be enabled. */
    bool dmaEnable;                 /*!< Enable DMA request for this channel. */
} enet_timer_channel_config_t;

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

/*!
 * @brief Transmit options for a particular frame.
 * Implements : enet_tx_options_t_Class
 */
typedef struct {
    bool noCRC;    /*!< Do not append CRC. It will be provided by the application. */
    bool noInt;    /*!< Do not generate a transmit interrupt. */
#if FEATURE_ENET_HAS_TBS
    bool useTLT;   /*!< If true, use transmit launch time. */
    uint32_t TLT;  /*!< The value of the transmit launch time. */
#endif /* FEATURE_ENET_HAS_TBS */
} enet_tx_options_t;

#if (FEATURE_ENET_RING_COUNT > 1U)

/*!
 * @brief Transmitter queue selection/arbitration schemes.
 * Implements : enet_tx_scheme_t_Class
 */
typedef enum {
#if FEATURE_ENET_HAS_CBS
	ENET_CBS_SCHEME = 0U,
#endif /* FEATURE_ENET_HAS_CBS */
	ENET_ROUND_ROBIN_SCHEME = 1U
} enet_tx_scheme_t;

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

/*!
 * @brief Available FIFO threshold levels.
 * Implements : enet_fifo_threshold_t_Class
 */
typedef enum {
    ENET_RX_SECTION_FULL = 0U, /*<! Value, in 64-bit words, of the receive FIFO section full threshold. When the FIFO
                                    level reaches the value in this field, data is available in the Receive FIFO. */
    ENET_RX_SECTION_EMPTY,     /*<! Value, in 64-bit words, of the receive FIFO section empty threshold. When the FIFO
                                    has reached this level, a pause frame will be issued. */
    ENET_RX_ALMOST_EMPTY,      /*<! Value, in 64-bit words, of the receive FIFO almost empty threshold. When the FIFO
                                    level reaches this level and the end-of-frame has not been received for the frame
                                    yet, the core receive read control stops FIFO read. A minimum value of 4 should be
                                    set. */
    ENET_RX_ALMOST_FULL,       /*<! Value, in 64-bit words, of the receive FIFO almost full threshold. When the FIFO level
                                    comes close to the maximum, so that there is no more space for at least RX_ALMOST_FULL
                                    number of words, the MAC stops writing data in the FIFO and truncates the received frame
                                    to avoid FIFO overflow. A minimum value of 4 should be set. */
    ENET_TX_SECTION_EMPTY,     /*!< Value, in 64-bit words, of the transmit FIFO section empty threshold. When the FIFO level
                                    reaches this value, a MAC status signal indicates that the transmit FIFO is getting full.
                                    This gives the ENET module an indication to slow or stop its write transaction to avoid a
                                    buffer overflow. */
    ENET_TX_ALMOST_EMPTY,      /*!< Value, in 64-bit words, of the transmit FIFO almost empty threshold. When the FIFO level
                                    reaches this level and no end-of-frame is available for the frame, the MAC transmit logic,
                                    to avoid FIFO underflow, stops reading the FIFO and transmits a frame with an MII error
                                    indication. */
    ENET_TX_ALMOST_FULL        /*!< Value, in 64-bit words, of the transmit FIFO almost full threshold. A minimum value of six
                                    is required. When the FIFO level comes close to the maximum, so that there is no more space
                                    for at least TX_ALMOST_FULL number of words, the FIFO write control logic, to avoid FIFO overflow,
                                    truncates the current frame and sets the error status. As a result, the frame will be transmitted
                                    with an GMII/MII error indication. */
} enet_fifo_threshold_t;

#if FEATURE_ENET_HAS_RECEIVE_PARSER

/*!
 * @brief Receive parser configuration.
 * Implements : enet_rx_parser_config_t_Class
 */
typedef struct {
    bool acceptEndError;    /*!< Accept the current frame when parsing is not completed for the current frame and parsing for
                                 the next frame has started. */
    uint8_t endErrorQueue;  /*!< Queue number for the accepted frames that encounter the Parser End Error. */
    bool clearCounters;     /*!< Clear Parser Statistics Counter */
    bool inverseByteOrder;  /*!< When true, the byte order within a frame word is changed before comparison. */
} enet_rx_parser_config_t;

/*!
 * @brief Receive parser rule.
 * Implements : enet_rx_parser_rule_t_Class
 */
typedef struct {
    uint32_t compareValue;  /*!< Data to be matched. */
    uint32_t compareMask;   /*!< Matching mask. */
    uint8_t compareOffset;  /*!< Points to the word, from the start of the frame, to be compared for matching. */
} enet_rx_parser_rule_t;

/*!
 * @brief Receive parser statistics counters.
 * Implements : enet_rx_parser_counter_t_Class
 */
typedef enum {
    ENET_RX_PARSER_CNT_DISCARD = 0U,  /*!< Number of frames rejected by the receive parser due to no matching,
                                           inverse matching, or parser programming error. */
    ENET_RX_PARSER_CNT_ACCEPT_0,      /*!< Number of frames accepted by the receive parser for queue 0. */
    ENET_RX_PARSER_CNT_REJECT_0,      /*!< Number of frames rejected by the receive parser for queue 0. */
    ENET_RX_PARSER_CNT_ACCEPT_1,      /*!< Number of frames accepted by the receive parser for queue 1. */
    ENET_RX_PARSER_CNT_REJECT_1,      /*!< Number of frames rejected by the receive parser for queue 1. */
    ENET_RX_PARSER_CNT_ACCEPT_2,      /*!< Number of frames accepted by the receive parser for queue 2. */
    ENET_RX_PARSER_CNT_REJECT_2       /*!< Number of frames rejected by the receive parser for queue 2. */
} enet_rx_parser_counter_t;

/*!
 * @brief Receive parser status.
 * Implements : enet_rx_parser_status_t_Class
 */
typedef enum {
    ENET_RX_PARSER_STAT_MXINDERR = ENET_RXPARST_INVMAXIDX_MASK,  /*!< Frame rejected due to parser error as table entry index greater
                                                                      than or equal to number of valid entries within the table. */
    ENET_RX_PARSER_STAT_TBLDPTERR = ENET_RXPARST_TBLDPTERR_MASK, /*!< Frame rejected due to parser error as table entry index larger
                                                                      than or equal to parser table physical depth. */
    ENET_RX_PARSER_STAT_NOMTCERR = ENET_RXPARST_NOMTCERR_MASK,   /*!< Frame rejected due to parser error as no match or invert match
                                                                      found after end of parsing. */
    ENET_RX_PARSER_STAT_FMOFFERR = ENET_RXPARST_FMOFFERR_MASK,   /*!< Frame rejected due to parser error as frame offset is larger than
                                                                      expected maximum frame offset value. */
    ENET_RX_PARSER_STAT_PRSENDERR = ENET_RXPARST_PRSENDERR_MASK, /*!< Frame parsing started for the next frame before end of parsing for
                                                                      the current frame. */
    ENET_RX_PARSER_STAT_INVMAXIDX = ENET_RXPARST_INVMAXIDX_MASK, /*!< Set to 1 if the parser is currently enabled, but the number of valid
                                                                      entries within the table is set to 0 or is larger than the physical
                                                                      table depth */
    ENET_RX_PARSER_STAT_RXPRSDN = ENET_RXPARST_RXPRSDN_MASK      /*!< Asserted to indicate that the parser is in idle state. Deasserted when
                                                                      parser is active and currently parsing a receive frame. */
} enet_rx_parser_status_t;

#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and De-initialization
 * @{
 */

/*!
 * @brief Gets the default configuration structure
 *
 * This function gets the default configuration structure, with the following settings:
 * - no interrupt enabled
 * - maximum receive frame length equal to the maximum Ethernet frame length
 * - no special receive/transmit control configuration
 * - no acceleration function enabled
 * - RMII mode, full-duplex, 100Mbps for MAC and PHY data interface
 * - no callback installed
 *
 * @param[out] config The configuration structure
 */
void ENET_DRV_GetDefaultConfig(enet_config_t *config);

/*!
 * @brief Initializes the ENET module
 *
 * This function initializes and enables the ENET module, configuring receive and transmit control
 * settings, the receive and transmit descriptors rings, and the MAC physical address.
 *
 * @param[in] instance Instance number
 * @param[in] state Pointer to the state structure which will be used for holding
 * the internal state of the driver.
 * @param[in] config The module configuration structure
 * @param[in] bufferConfig The array of buffer descriptors configuration structures (one per ring)
 * @param[in] macAddr The physical address of the MAC
 */
void ENET_DRV_Init(uint8_t instance,
                   enet_state_t *state,
                   const enet_config_t *config,
                   const enet_buffer_config_t bufferConfig[],
                   const uint8_t *macAddr);

/*!
 * @brief Deinitializes the ENET module
 *
 * This function disables the interrupts and then disables the ENET module.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_Deinit(uint8_t instance);

/*! @} */

/*!
 * @name Transmission and reception operations
 * @{
 */

/*!
 * @brief Reads a received Ethernet frame
 *
 * This function reads the first received Ethernet frame in the Rx queue. The buffer received
 * as parameter will be updated by the driver and the .data field will point to a memory area
 * containing the frame data.
 *
 * Note: Once the application finished processing the buffer, it could be reused by the
 * driver for further receptions by invoking ENET_DRV_ProvideRxBuff.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to read a frame at the same time. This synchronization shall be implemented by the
 * application.
 *
 * @param[in]  instance Instance number
 * @param[in]  queue The queue number
 * @param[out] buff The buffer containing the frame
 * @param[out] info Enhanced information related to the buffer. If this information is not needed,
 * this parameter should be NULL.
 * @return STATUS_SUCCESS if a frame was successfully read, STATUS_ENET_RX_QUEUE_EMPTY if
 * there is no available frame in the queue.
 */
status_t ENET_DRV_ReadFrame(uint8_t instance,
                            uint8_t queue,
                            enet_buffer_t * buff,
                            enet_rx_enh_info_t * info);

/*!
 * @brief Provides a receive buffer to be used by the driver for reception.
 *
 * This function provides a buffer which can further be used by the reception mechanism
 * in order to store the received data.
 *
 * Note: The application can either provide a buffer previously obtained in a ENET_DRV_ReadFrame
 * call (when it is no longer needed after being fully processed), or allocate a new buffer,
 * pointing to a memory area having the required alignment (see FEATURE_ENET_BUFF_ALIGNMENT).
 * The former approach is recommended as it has a simpler usage model and re-uses the same initial
 * memory range for the entire driver lifetime operation. The later approach could provide more
 * flexibility, but since it involves constant memory free/alloc operations it is only recommended
 * with an efficient pool-based memory allocator.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to provide a buffer at the same time. This synchronization shall be implemented by the
 * application.
 *
 * Important: The application is responsible for providing one Rx buffer for every frame it
 * receives, otherwise the reception ring can fill-up, affecting further reception.
 *
 * Usage example:
 *
 * stat = ENET_DRV_ReadFrame(INST_ETHERNET1, &rxBuff);
 *
 * if (stat == STATUS_SUCCESS)
 * {
 *     process_buffer(&rxBuff);
 *     ENET_DRV_ProvideRxBuff(INST_ETHERNET1, &rxBuff);
 * }
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] buff The buffer to be added to the reception ring
 */
void ENET_DRV_ProvideRxBuff(uint8_t instance,
                            uint8_t queue,
                            enet_buffer_t * buff);

/*!
 * @brief Sends an Ethernet frame
 *
 * This function sends an Ethernet frame, contained in the buffer received as parameter.
 *
 * Note: Since the transmission of the frame is not complete when this function returns,
 * the application must not change/alter/re-use the provided buffer until after a call
 * to ENET_DRV_GetTransmitStatus for the same buffer returns STATUS_SUCCESS.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to send a frame at the same time. This synchronization shall be implemented by the
 * application.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] buff The buffer containing the frame
 * @param[in] options Transmit options for this buffer. Can be NULL, if no special option is required.
 * @return STATUS_SUCCESS if the frame was successfully enqueued for transmission,
 * STATUS_ENET_TX_QUEUE_FULL if there is no available space for the frame in the queue.
 */
status_t ENET_DRV_SendFrame(uint8_t instance,
                            uint8_t queue,
                            const enet_buffer_t * buff,
                            enet_tx_options_t * options);

/*!
 * @brief Checks if the transmission of a buffer is complete and extracts information
 * related to the transmission.
 *
 * This function checks if the transmission of the given buffer is complete and extracts
 * information related to the transmission.
 *
 * Note: If the buffer is not found in the Tx ring, the function will return STATUS_ENET_BUFF_NOT_FOUND;
 * in this case, the enhanced information will not be extracted.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] buff The transmit buffer for which the status shall be checked
 * @param[out] info Enhanced information related to the buffer. If this information is not needed,
 * this parameter should be NULL.
 * @return STATUS_BUSY if the frame is still enqueued for transmission, STATUS_ENET_BUFF_NOT_FOUND
 * if the buffer was not found in the Tx queue, STATUS_SUCCESS otherwise,.
 */
status_t ENET_DRV_GetTransmitStatus(uint8_t instance,
                                    uint8_t queue,
                                    const enet_buffer_t * buff,
                                    enet_tx_enh_info_t * info);

/*! @} */

/*!
 * @name MDIO configuration and operation
 * @{
 */

/*!
 * @brief Enables the MDIO interface
 *
 * @param[in] instance Instance number
 * @param[in] miiPreambleDisabled Enables/disables prepending a preamble to the MII management frame.
 */
void ENET_DRV_EnableMDIO(uint8_t instance,
                         bool miiPreambleDisabled);

/*!
 * @brief Reads the selected register of the PHY
 *
 * @param[in]  instance Instance number
 * @param[in]  phyAddr PHY device address
 * @param[in]  phyReg PHY register address
 * @param[out] data Data read from the PHY
 * @param[in] timeoutMs Timeout for the read operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed succesfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t ENET_DRV_MDIORead(uint8_t instance,
                           uint8_t phyAddr,
                           uint8_t phyReg,
                           uint16_t *data,
                           uint32_t timeoutMs);

/*!
 * @brief Writes the selected register of the PHY
 *
 * @param[in] instance Instance number
 * @param[in] phyAddr PHY device address
 * @param[in] phyReg PHY register address
 * @param[in] data Data to be written in the specified register of the PHY
 * @param[in] timeoutMs Timeout for the write operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed succesfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t ENET_DRV_MDIOWrite(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t phyReg,
                            uint16_t data,
                            uint32_t timeoutMs);

/*!
 * @brief Reads a register of the specified MMD in a PHY device
 *
 * @param[in]  instance Instance number
 * @param[in]  phyAddr PHY device address
 * @param[in]  mmd The MMD index of the target register
 * @param[in]  phyReg PHY register address
 * @param[out] data Data read from the PHY
 * @param[in] timeoutMs Timeout for the read operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed succesfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t ENET_DRV_MDIOReadMMD(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t mmd,
                            uint16_t phyReg,
                            uint16_t *data,
                            uint32_t timeoutMs);

/*!
 * @brief Writes a register of the specified MMD in a PHY device
 *
 * @param[in] instance Instance number
 * @param[in] phyAddr PHY device address
 * @param[in] mmd The MMD index of the target register
 * @param[in] phyReg PHY register address
 * @param[in] data Data to be written in the specified register of the PHY
 * @param[in] timeoutMs Timeout for the write operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed succesfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t ENET_DRV_MDIOWriteMMD(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t mmd,
                            uint16_t phyReg,
                            uint16_t data,
                            uint32_t timeoutMs);

/*! @} */

/*!
 * @name MAC Address configuration
 * @{
 */

/*!
 * @brief Configures the physical address of the MAC
 *
 * @param[in] instance Instance number
 * @param[in] macAddr The MAC address to be configured
 */
void ENET_DRV_SetMacAddr(uint8_t instance,
                         const uint8_t *macAddr);

/*!
 * @brief Gets the physical address of the MAC
 *
 * @param[in]  instance Instance number
 * @param[out] macAddr The physical address of the MAC
 */
void ENET_DRV_GetMacAddr(uint8_t instance,
                         uint8_t *macAddr);

/*!
 * @brief Enables/Disables forwarding of unicast traffic having a specific MAC address
 * as destination.
 *
 * @param[in]  instance Instance number
 * @param[in]  macAddr The physical address
 * @param[in]  enable If true, the application will receive all the unicast traffic having as
 * destination address the provided MAC address; if false, stop forwarding this kind of
 * traffic
 */
void ENET_DRV_SetUnicastForward(uint8_t instance,
                                const uint8_t *macAddr,
                                bool enable);

/*!
 * @brief Enables/Disables forwarding of multicast traffic having a specific MAC address
 * as destination.
 *
 * @param[in]  instance Instance number
 * @param[in]  macAddr The physical address
 * @param[in]  enable If true, the application will receive all the multicast traffic having as
 * destination address the provided MAC address; if false, stop forwarding this kind of
 * traffic
 */
void ENET_DRV_SetMulticastForward(uint8_t instance,
                                  const uint8_t *macAddr,
                                  bool enable);

/*!
 * @brief Enables/Disables forwarding of the multicast traffic, irrespective of the destination
 * MAC address.
 *
 * @param[in]  instance Instance number
 * @param[in]  enable If true, the application will receive all the multicast traffic; if false,
 * stop forwarding this kind of traffic
 */
void ENET_DRV_SetMulticastForwardAll(uint8_t instance,
                                     bool enable);

/*! @} */

/*!
 * @name Other basic operations
 * @{
 */

#if FEATURE_ENET_HAS_SLEEP_WAKEUP
 
/*!
 * @brief Sets the MAC in sleep mode or normal mode
 *
 * @param[in] instance Instance number
 * @param[in] enable If true, set MAC in sleep mode; if false, set MAC in normal mode
 */
void ENET_DRV_SetSleepMode(uint8_t instance,
                           bool enable);

#endif /* FEATURE_ENET_HAS_SLEEP_WAKEUP */

/*!
 * @brief Enables/Disables the MIB counters.
 *
 * Note: When enabling the counters, their values are reset.
 *
 * @param[in] instance Instance number
 * @param[in] enable Enable/Disable MIB counters
 */
void ENET_DRV_ConfigCounters(uint8_t instance,
                             bool enable);

/*!
 * @brief Gets statistics from the specified counter
 *
 * @param[in] instance Instance number
 * @param[in] counter The counter to be read
 * @return The value of the requested counter
 */
uint32_t ENET_DRV_GetCounter(uint8_t instance,
                             enet_counter_t counter);

/*!
 * @brief Gets a mask of the interrupt events which occurred.
 *
 * @param[in] instance Instance number
 * @return A logical OR of "enet_interrupt_t"
 */
uint32_t ENET_DRV_GetInterruptFlags(uint8_t instance);

/*!
 * @brief Sets the speed of the MII interface
 *
 * @param[in] instance Instance number
 * @param[in] speed MII speed
 */
void ENET_DRV_SetSpeed(uint8_t instance,
                       enet_mii_speed_t speed);

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*!
 * @brief Initializes the adjustable timer using the given configuration
 * and enables it.
 *
 * @param[in] instance Instance number
 * @param[in] timerConfig The configuration of the adjustable timer
 */
void ENET_DRV_TimerInit(uint8_t instance,
                        enet_timer_config_t *timerConfig);

/*!
 * @brief Starts the timer.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_TimerStart(uint8_t instance);

/*!
 * @brief Stops the timer.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_TimerStop(uint8_t instance);

/*!
 * @brief Sets the value of the timer.
 *
 * @param[in] instance Instance number
 * @param[in] value The value of the timer (in timer clock cycles)
 */
void ENET_DRV_TimerSet(uint8_t instance,
                       uint32_t value);

/*!
 * @brief Gets the value of the timer.
 *
 * @param[in] instance Instance number
 * @param[out] value The value of the timer (in timer clock cycles)
 */
void ENET_DRV_TimerGet(uint8_t instance,
                       uint32_t *value);

/*!
 * @brief Sets the timer correction period and correction increment value.
 *
 * @param[in] instance Instance number
 * @param[in] value This value is added every time the correction period
 * expires (in timer clock cycles)
 * @param[in] period This period of the correction timer
 */
void ENET_DRV_TimerSetCorrection(uint8_t instance,
                                 uint32_t value,
                                 uint32_t period);

/*!
 * @brief Configures and enables a specific timer channel.
 *
 * @param[in] instance Instance number
 * @param[in] channel Channel number
 * @param[in] config The configuration of channel
 */
void ENET_DRV_TimerEnableChannel(uint8_t instance,
                                 uint8_t channel,
                                 enet_timer_channel_config_t *config);

/*!
 * @brief Sets the compare value for a channel configured as output compare.
 *
 * Note: This function should be called prior to enabling the channel and prior
 * to each subsequent compare.
 *
 * @param[in] instance Instance number
 * @param[in] channel Channel number
 * @param[in] config The configuration of channel
 */
void ENET_DRV_TimerSetCompare(uint8_t instance,
                              uint8_t channel,
                              uint32_t value);

/*!
 * @brief Gets the last value captured on a specific channel configured as input capture.
 *
 * @param[in] instance Instance number
 * @param[in] channel Channel number
 * @param[out] capture The captured value
 */
void ENET_DRV_TimerGetCapture(uint8_t instance,
                              uint8_t channel,
                              uint32_t *capture);

#if FEATURE_ENET_HAS_TIMER_PULSE_WIDTH_CONTROL

/*!
 * @brief Sets the pulse width for a channel configured as output compare with
 * output in pulse mode.
 *
 * @param[in] instance Instance number
 * @param[in] channel Channel number
 * @param[in] pulseWidth The width of the pulse (in timer clock cycles)
 */
void ENET_DRV_TimerSetPulseWidth(uint8_t instance,
                                 uint8_t channel,
                                 uint8_t pulseWidth);

#endif /* FEATURE_ENET_HAS_TIMER_PULSE_WIDTH_CONTROL */

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

#if (FEATURE_ENET_RING_COUNT > 1U)

/*!
 * @brief Sets the transmitter queue selection/arbitration scheme.
 *
 * @param[in] instance Instance number
 * @param[in] scheme Transmitter queue selection/arbitration scheme.
 */
void ENET_DRV_ConfigTxScheme(uint8_t instance,
		                     enet_tx_scheme_t scheme);

#if FEATURE_ENET_HAS_CBS

/*!
 * @brief Configures the idle slope for the additional classes traffic.
 *
 * @param[in] instance Instance number
 * @param[in] class The class for which the idle slope will be configured
 * @param[in] idleSlope The idle slope value
 */
void ENET_DRV_ConfigIdleSlope(uint8_t instance,
		                      uint8_t class,
                              uint16_t idleSlope);

#endif /* FEATURE_ENET_HAS_CBS */

/*!
 * @brief Configures the VLAN priority matching for the specified class.
 *
 * @param[in] instance Instance number
 * @param[in] class The class for which the configuration will be applied
 * @param[in] vlanPrioNum The number of specified VLAN priorities
 * @param[in] vlanPrio The set of VLAN priorities matched against incoming VLAN frames
 */
void ENET_DRV_ConfigClassMatch(uint8_t instance,
                               uint8_t class,
                               uint8_t vlanPrioNum,
                               uint8_t *vlanPrio);

/*!
 * @brief Enables/Disables receive flushing for a specified ring.
 *
 * Rx flushing prevents frames in the Rx FIFO from being blocked. Blocking can occur
 * if the frame at the head of the Rx FIFO cannot be forwarded because the ring it
 * is associated with cannot accept it. When Rx flushing is enabled, the blocking
 * frame will be flushed (discarded).
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] enable Enable/Disable receive flushing
 */
void ENET_DRV_ConfigRxFlush(uint8_t instance,
                            uint8_t queue,
                            bool enable);

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

#if FEATURE_ENET_HAS_INTCOAL

/*!
 * @brief Enables receive interrupts coalescing.
 *
 * When coalescing is enabled and receive interrupts are also enabled, the MAC
 * generates an interrupt when the threshold number of frames is reached or when the
 * threshold timer expires.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] frameCount The number of frames needed to be received for raising an interrupt
 * @param[in] timeoutUs The maximum amount of time after receiving a frame before raising an
 * interrupt (in microseconds)
 */
void ENET_DRV_EnableRxInterruptCoalescing(uint8_t instance,
                                          uint8_t queue,
                                          uint8_t frameCount,
                                          uint16_t timeoutUs);

/*!
 * @brief Disables receive interrupts coalescing.
 *
 * When coalescing is disabled and receive interrupts are enabled, the MAC
 * generates an interrupt for each frame, without using coalescing.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 */
void ENET_DRV_DisableRxInterruptCoalescing(uint8_t instance,
                                           uint8_t queue);

/*!
 * @brief Enables transmit interrupts coalescing.
 *
 * When coalescing is enabled and transmit interrupts are also enabled, the MAC
 * generates an interrupt when the threshold number of frames is reached or when the
 * threshold timer expires.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 * @param[in] frameCount The number of frames needed to be transmitted for raising an interrupt
 * @param[in] timeoutUs The maximum amount of time after receiving a frame before raising an
 * interrupt (in microseconds)
 */
void ENET_DRV_EnableTxInterruptCoalescing(uint8_t instance,
                                          uint8_t queue,
                                          uint8_t frameCount,
                                          uint16_t timeoutUs);

/*!
 * @brief Disables transmit interrupts coalescing.
 *
 * When coalescing is disabled and transmit interrupts are enabled, the MAC
 * generates an interrupt for each frame, without using coalescing.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue number
 */
void ENET_DRV_DisableTxInterruptCoalescing(uint8_t instance,
                                           uint8_t queue);

#endif /* FEATURE_ENET_HAS_INTCOAL */

#if FEATURE_ENET_HAS_CONFIG_FIFO_THRESHOLDS

/*!
 * @brief Sets FIFO threshold levels.
 *
 * @param[in] instance Instance number
 * @param[in] thresholdType The threshold level to be set
 * @param[in] thresholdValue The threshold value (in 64-bit words)
 */
void ENET_DRV_SetFIFOThreshold(uint8_t instance,
                               enet_fifo_threshold_t thresholdType,
                               uint8_t thresholdValue);

/*!
 * @brief Sets the transmit FIFO watermark.
 *
 * Sets the amount of data required in the transmit FIFO before transmission of a
 * frame can begin.
 *
 * Note: If a frame with less than the threshold is written, it is still sent independently
 * of this threshold setting. The threshold is relevant only if the frame is larger than
 * the threshold given.
 * Note: Setting this watermark will disable the store and forward feature.
 *
 * @param[in] instance Instance number
 * @param[in] watermark The number of bytes, written to the transmit FIFO before
 * transmission of a frame begins. Must be a multiple of 64 bytes.
 */
void ENET_DRV_SetTxFIFOWatermark(uint8_t instance,
                                 uint16_t watermark);

/*!
 * @brief Enables the store and forward feature on the transmit path.
 *
 * When the store and forward feature is enabled, the MAC starts to transmit data only when
 * a complete frame is stored in the Transmit FIFO.
 *
 * Note: Any transmit FIFO watermark previously set will no longer apply after enabling this
 * feature.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_EnableTxStoreAndForward(uint8_t instance);

#endif /* FEATURE_ENET_HAS_CONFIG_FIFO_THRESHOLDS */

#if FEATURE_ENET_HAS_RECEIVE_PARSER

/*!
 * @brief Configures and enables the receive parser.
 *
 * @param[in] instance Instance number
 * @param[in] config Receive parser configuration
 */
void ENET_DRV_RxParserInit(uint8_t instance,
                           enet_rx_parser_config_t *config);

/*!
 * @brief Erases all the rules and disables the receive parser.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_RxParserDeinit(uint8_t instance);

/*!
 * @brief Appends an acceptance rule to the receive parser.
 *
 * @param[in] instance Instance number
 * @param[in] queue The queue in which the accepted frame will be placed
 * @param[in] rule The matching rule
 */
void ENET_DRV_RxParserAddAcceptRule(uint8_t instance,
                                    uint8_t queue,
                                    enet_rx_parser_rule_t *rule);

/*!
 * @brief Appends a rejection rule to the receive parser.
 *
 * @param[in] instance Instance number
 * @param[in] rule The matching rule
 */
void ENET_DRV_RxParserAddRejectRule(uint8_t instance,
                                    enet_rx_parser_rule_t *rule);

/*!
 * @brief Appends a linking rule to the receive parser.
 *
 * When a frame matches this rule, the parser will continue matching starting
 * with the given rule index, instead of using the subsequent rule.
 *
 * @param[in] instance Instance number
 * @param[in] nextEntry The next entry of the matching table to be used for comparison
 * instead of using the next entry sequentially
 * @param[in] rule The matching rule
 */
void ENET_DRV_RxParserAddLinkingRule(uint8_t instance,
                                     uint8_t nextEntry,
                                     enet_rx_parser_rule_t *rule);

/*!
 * @brief Returns the status of the receive parser.
 *
 * @param[in] instance Instance number
 * @return The status of the parser, as a mask of enet_rx_parser_status_t.
 */
uint32_t ENET_DRV_RxParserGetState(uint8_t instance);

/*!
 * @brief Returns the value of the requested receive parser counter.
 *
 * @param[in] instance Instance number
 * @param[in] counter The counter to be checked
 * @return The value of the counter.
 */
uint32_t ENET_DRV_RxParserGetCount(uint8_t instance,
                                   enet_rx_parser_counter_t counter);

#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif  /* ENET_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/

