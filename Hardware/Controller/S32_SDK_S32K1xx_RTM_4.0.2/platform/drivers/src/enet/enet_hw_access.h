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

/*!
 * @file enet_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 */

#ifndef ENET_HW_ACCESS_H
#define ENET_HW_ACCESS_H

#include "enet_driver.h"
#include "device_registers.h"
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ENET_MMFR_START_INDICATION     (1U)
#define ENET_MMFR_START_INDICATION_45  (0U)
#define ENET_MMFR_TURN_AROUND       (2U)

/*! @brief Control and status region bit masks of the receive buffer descriptor. */
#define ENET_BUFFDESCR_RX_EMPTY_MASK 0x8000U
#define ENET_BUFFDESCR_RX_INUSE_MASK 0x4000U
#define ENET_BUFFDESCR_RX_WRAP_MASK  0x2000U

/*! @brief Control and status bit masks of the transmit buffer descriptor. */
#define ENET_BUFFDESCR_TX_READY_MASK        0x8000U
#define ENET_BUFFDESCR_TX_WRAP_MASK         0x2000U
#define ENET_BUFFDESCR_TX_LAST_MASK         0x0800U
#define ENET_BUFFDESCR_TX_TRANSMITCRC_MASK  0x0400U

/*! @brief Masks for the bits in the CRC-32 value */
#define ENET_CRC32_BIT_1_MASK       0x80000000UL
#define ENET_CRC32_BITS_2_6_MASK    0x7C000000UL
#define ENET_CRC32_BITS_2_6_SHIFT   26UL

#if FEATURE_ENET_HAS_ENHANCED_BD

#define ENET_RX_ENH_ERR_MASK    (0x86000030U)
#define ENET_TX_ENH_ERR_MASK    (0x00003F00U)

/*! @brief Control and status bit masks of the enhanced receive buffer descriptor. */
#define ENET_RX_ENH1_FRAG_MASK        0x00000001UL
#define ENET_RX_ENH1_IPV6_MASK        0x00000002UL
#define ENET_RX_ENH1_VPCP_MASK        0x0000E000UL
#define ENET_RX_ENH1_VPCP_SHIFT       13UL
#define ENET_RX_ENH1_INT_MASK         0x00800000UL
#define ENET_RX_ENH1_UNICAST_MASK     0x01000000UL
#define ENET_RX_ENH2_CSUM_MASK        0x0000FFFFUL
#define ENET_RX_ENH2_PROTO_MASK       0x00FF0000UL
#define ENET_RX_ENH2_PROTO_SHIFT      16UL
#define ENET_RX_ENH2_HLEN_MASK        0xF8000000UL
#define ENET_RX_ENH2_HLEN_SHIFT       27UL

/*! @brief Control and status bit masks of the enhanced transmit buffer descriptor. */
#define ENET_TX_ENH1_IINS_MASK        0x08000000UL
#define ENET_TX_ENH1_PINS_MASK        0x10000000UL
#define ENET_TX_ENH1_INT_MASK         0x40000000UL
#if FEATURE_ENET_HAS_TBS
#define ENET_TX_ENH1_UTLT_MASK        0x01000000UL
#endif /* FEATURE_ENET_HAS_CBS */
#if (FEATURE_ENET_RING_COUNT > 1U)
#define ENET_TX_ENH1_FTYPE_SHIFT      20U
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

#define ENET_RX_INTERRUPTS_MASK  (ENET_EIR_RXF_MASK | ENET_EIR_RXB_MASK)
#define ENET_TX_INTERRUPTS_MASK  (ENET_EIR_TXF_MASK | ENET_EIR_TXB_MASK)

#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

/*!
 * @brief Management Frame operation type
 */
typedef enum
{
    ENET_MMFR_OP_ADDR = 0U,
    ENET_MMFR_OP_WRITE,
    ENET_MMFR_OP_READ,
    ENET_MMFR_OP_READ_45
} enet_mmfr_op_type_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to enet handles for each instance. */
extern enet_state_t *g_enetState[ENET_INSTANCE_COUNT];

/*! @brief Pointers to ENET bases for each instance. */
static ENET_Type *const s_enetBases[] = ENET_BASE_PTRS;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures the receive and transmit buffer descriptors.
 *
 * @param[in] base The base address of the module
 * @param[in] bufferConfig The configuration used for the receive and transmit descriptors
 * @param[in] buffSize The maximum buffer size
 */
void ENET_ConfigBufferDescriptors(ENET_Type * base,
                                  uint8_t ring,
                                  const enet_buffer_config_t *bufferConfig,
                                  uint16_t buffSize);

#if FEATURE_ENET_HAS_RX_CONFIG

/*!
 * @brief Configures the receive block.
 *
 * @param[in] base The base address of the module
 * @param[in] config The user configuration structure
 */
void ENET_ConfigReceiveControl(ENET_Type *base,
                               const enet_config_t *config);

#endif /* FEATURE_ENET_HAS_RX_CONFIG */

#if FEATURE_ENET_HAS_TX_CONFIG

/*!
 * @brief Configures the transmit block.
 *
 * @param[in] base The base address of the module
 * @param[in] config The user configuration structure
 */
void ENET_ConfigTransmitControl(ENET_Type *base,
                                const enet_config_t *config);

#endif /* FEATURE_ENET_HAS_TX_CONFIG */

/*!
 * @brief Resets the ENET module.
 *
 * @param[in] base The base address of the module
 */
static inline void ENET_Reset(ENET_Type *base)
{
    base->ECR |= ENET_ECR_RESET_MASK;
#if (defined(CORE_LITTLE_ENDIAN))
    base->ECR |= ENET_ECR_DBSWP_MASK;
#endif
#if FEATURE_ENET_HAS_ENHANCED_BD
    base->ECR |= ENET_ECR_EN1588_MASK;
#endif
}

/*!
 * @brief Indicates that the driver produced empty receive buffers with the empty bit set.
 *
 * @param[in] base The base address of the module
 * @param[in] ring The ring number
 */
static inline void ENET_ActivateReceive(ENET_Type *base, uint8_t ring)
{
    if (ring == 0U)
    {
        base->RDAR = ENET_RDAR_RDAR_MASK;
    }
#if (FEATURE_ENET_RING_COUNT > 1U)
    if (ring == 1U)
    {
        base->RDAR1 = ENET_RDAR_RDAR_MASK;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
#if (FEATURE_ENET_RING_COUNT > 2U)
    if (ring == 2U)
    {
        base->RDAR2 = ENET_RDAR_RDAR_MASK;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 2U) */
}

/*!
 * @brief Indicates that the driver produced empty transmit buffers with the ready bit set.
 *
 * @param[in] base The base address of the module
 * @param[in] ring The ring number
 */
static inline void ENET_ActivateTransmit(ENET_Type *base, uint8_t ring)
{
    volatile uint32_t *tdar = NULL;

    if (ring == 0U)
    {
        tdar = &base->TDAR;
    }
#if (FEATURE_ENET_RING_COUNT > 1U)
    if (ring == 1U)
    {
        tdar = &base->TDAR1;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
#if (FEATURE_ENET_RING_COUNT > 2U)
    if (ring == 2U)
    {
        tdar = &base->TDAR2;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 2U) */

#ifdef ERRATA_E7885
    uint8_t i;
    bool tdarTrigger = false;

    for (i = 0; i < 4U; i++)
    {
        if ((*tdar & ENET_TDAR_TDAR_MASK) == 0U)
        {
            tdarTrigger = true;
            break;
        }
    }
    if (tdarTrigger)
    {
#endif
    *tdar = ENET_TDAR_TDAR_MASK;
#ifdef ERRATA_E7885
    }
#endif
}

/*!
 * @brief Enables the specified interrupts.
 *
 * @param[in] base The base address of the module
 * @param[in] mask Mask representing the interrupts to be enabled
 */
static inline void ENET_EnableInterrupts(ENET_Type *base,
                                         uint32_t mask)
{
    base->EIMR |= mask;
}

/*!
 * @brief Disables the specified interrupts.
 *
 * @param[in] base The base address of the module
 * @param[in] mask Mask representing the interrupts to be disabled
 */
static inline void ENET_DisableInterrupts(ENET_Type *base,
                                          uint32_t mask)
{
    base->EIMR &= ~mask;
}

/*!
 * @brief Gets a mask of the interrupts flags which are set.
 *
 * @param[in] base The base address of the module
 * @return Mask representing the interrupt flags set
 */
static inline uint32_t ENET_GetInterruptStatus(const ENET_Type *base)
{
    return base->EIR;
}

/*!
 * @brief Clears the interrupts flags according to the received mask.
 *
 * @param[in] base The base address of the module
 * @param[in] mask Mask representing the interrupt flags to be cleared
 */
static inline void ENET_ClearInterruptStatus(ENET_Type *base,
                                             uint32_t mask)
{
    base->EIR = mask;
}

/*!
 * @brief Enables the ENET module.
 *
 * @param[in] base The base address of the module
 */
static inline void ENET_Enable(ENET_Type *base)
{
    base->ECR |= ENET_ECR_ETHEREN_MASK;
}

/*!
 * @brief Disables the ENET module.
 *
 * @param[in] base The base address of the module
 */
static inline void ENET_Disable(ENET_Type *base)
{
    base->ECR &= ~ENET_ECR_ETHEREN_MASK;
}

#if FEATURE_ENET_HAS_ACCELERATOR

/*!
 * @brief Configures transmit accelerator functions.
 *
 * @param[in] base The base address of the module
 * @param[in] txAccelerConfig Transmit accelerator functions to be applied
 */
static inline void ENET_ConfigTransmitAccelerator(ENET_Type * base,
                                                  uint8_t txAccelerConfig)
{
    base->TACC = txAccelerConfig;

    /* Enable store and forward when accelerator is enabled */
    if (((uint32_t)txAccelerConfig & ((uint32_t)ENET_TX_ACCEL_INSERT_IP_CHECKSUM | (uint32_t)ENET_TX_ACCEL_INSERT_PROTO_CHECKSUM)) != 0U)
    {
        base->TFWR = ENET_TFWR_STRFWD_MASK;
    }
}

/*!
 * @brief Configures receive accelerator functions.
 *
 * @param[in] base The base address of the module
 * @param[in] rxAccelerConfig Receive accelerator functions to be applied
 */
static inline void ENET_ConfigReceiveAccelerator(ENET_Type * base,
                                                 uint8_t rxAccelerConfig)
{
    base->RACC = rxAccelerConfig;

    /* Enable store and forward when accelerator is enabled */
    if (((uint32_t)rxAccelerConfig & ((uint32_t)ENET_RX_ACCEL_ENABLE_IP_CHECK | (uint32_t)ENET_RX_ACCEL_ENABLE_PROTO_CHECK)) != 0U)
    {
        base->RSFL = 0;
    }
}

#endif /* FEATURE_ENET_HAS_ACCELERATOR */

/*!
 * @brief Sets the lower 32 bits of the MAC physical address.
 *
 * @param[in] base The base address of the module
 * @param[in] addrLower The lower 32 bits of the MAC physical address
 */
static inline void ENET_SetPhyAddrLower(ENET_Type * base,
                                        uint32_t addrLower)
{
    base->PALR = addrLower;
}

/*!
 * @brief Sets the upper 16 bits of the MAC physical address.
 *
 * @param[in] base The base address of the module
 * @param[in] addrUpper The upper 16 bits of the MAC physical address
 */
static inline void ENET_SetPhyAddrUpper(ENET_Type * base,
                                        uint32_t addrUpper)
{
    base->PAUR = addrUpper << ENET_PAUR_PADDR2_SHIFT;
}

/*!
 * @brief Gets the lower 32 bits of the MAC physical address.
 *
 * @param[in] base The base address of the module
 * @return The lower 32 bits of the MAC physical address
 */
static inline uint32_t ENET_GetPhyAddrLower(const ENET_Type * base)
{
    return base->PALR;
}

/*!
 * @brief Gets the upper 16 bits of the MAC physical address.
 *
 * @param[in] base The base address of the module
 * @return The upper 16 bits of the MAC physical address
 */
static inline uint32_t ENET_GetPhyAddrUpper(const ENET_Type * base)
{
    return (base->PAUR & ENET_PAUR_PADDR2_MASK) >> ENET_PAUR_PADDR2_SHIFT;
}

/*!
 * @brief Writes a MII management frame.
 *
 * @param[in] base The base address of the module
 * @param[in] phyAddr The address of the PHY
 * @param[in] phyReg The address of the register to be accessed
 * @param[in] opType The operation type (read/write)
 * @param[in] data Data to be written, ignored if the operation is a read one
 * @param[in] clause45 True if the management frame is a clause 45 one, false for clause 22
 * @param[in] startTime Time when the operation was started
 * @param[in] timeoutMs Timeout for the management operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed succesfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t ENET_WriteManagementFrame(ENET_Type * base,
                                             uint8_t phyAddr,
                                             uint8_t phyReg,
                                             enet_mmfr_op_type_t opType,
                                             uint16_t data,
                                             bool clause45,
                                             uint32_t startTime,
                                             uint32_t timeoutMs);

/*!
 * @brief Reads the data field of a MII management frame.
 *
 * @param[in] base The base address of the module
 * @return The read data
 */
static inline uint16_t ENET_ReadManagementFrameData(const ENET_Type * base)
{
    return (uint16_t)(base->MMFR & ENET_MMFR_DATA_MASK);
}

/*!
 * @brief Configures the MII management interface.
 *
 * @param[in] base The base address of the module
 * @param[in] miiSpeed MII Speed
 * @param[in] holdTime Hold time
 * @param[in] miiPreambleDisabled Enable/Disable MII preamble
 */
void ENET_WriteManagementConfig(ENET_Type * base,
                                uint32_t miiSpeed,
                                uint32_t holdTime,
                                bool miiPreambleDisabled);

/*!
 * @brief Adds an address to the hash table used in the address recognition
 * process for receive frames with multicast destination address.
 *
 * @param[in] base The base address of the module
 * @param[in] crc The CRC-32 of the MAC address
 */
static inline void ENET_AddToGroupHashTable(ENET_Type * base,
                                            uint32_t crc)
{
    if ((crc & ENET_CRC32_BIT_1_MASK) != 0U)
    {
        base->GAUR |= (1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
    else
    {
        base->GALR |= (1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
}

/*!
 * @brief Removes an address from the hash table used in the address recognition
 * process for receive frames with multicast destination address.
 *
 * @param[in] base The base address of the module
 * @param[in] crc The CRC-32 of the MAC address
 */
static inline void ENET_RemoveFromGroupHashTable(ENET_Type * base,
                                                 uint32_t crc)
{
    if ((crc & ENET_CRC32_BIT_1_MASK) != 0U)
    {
        base->GAUR &= ~(1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
    else
    {
        base->GALR &= ~(1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
}

/*!
 * @brief Adds an address to the hash table used in the address recognition
 * process for receive frames with unicast destination address.
 *
 * @param[in] base The base address of the module
 * @param[in] crc The CRC-32 of the MAC address
 */
static inline void ENET_AddToIndividualHashTable(ENET_Type * base,
                                                 uint32_t crc)
{
    if ((crc & ENET_CRC32_BIT_1_MASK) != 0U)
    {
        base->IAUR |= (1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
    else
    {
        base->IALR |= (1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
}

/*!
 * @brief Removes an address from the hash table used in the address recognition
 * process for receive frames with unicast destination address.
 *
 * @param[in] base The base address of the module
 * @param[in] crc The CRC-32 of the MAC address
 */
static inline void ENET_RemoveFromIndividualHashTable(ENET_Type * base,
                                                      uint32_t crc)
{
    if ((crc & ENET_CRC32_BIT_1_MASK) != 0U)
    {
        base->IAUR &= ~(1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
    else
    {
        base->IALR &= ~(1UL << ((crc & ENET_CRC32_BITS_2_6_MASK) >> ENET_CRC32_BITS_2_6_SHIFT));
    }
}

/*!
 * @brief Enables/Disables the MIB counters.
 *
 * Note: When enabling the counters, their values are reset.
 *
 * @param[in] instance Instance number
 * @param[in] enable Enable/Disable MIB counters
 */
void ENET_ConfigCounters(uint8_t instance,
                         bool enable);
							 
/*!
 * @brief Sets the speed of the MII interface
 *
 * @param[in] instance Instance number
 * @param[in] speed MII speed
 */
void ENET_SetSpeed(ENET_Type * base,
                   enet_mii_speed_t speed);

/*!
 * @brief Handler for ENET transmission interrupts.
 *
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_TransmitIRQHandler(uint8_t instance);

/*!
 * @brief Handler for ENET reception interrupts.
 *
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 * @param[in] instance Instance number
 */
void ENET_ReceiveIRQHandler(uint8_t instance);

#if (FEATURE_ENET_RING_COUNT > 1U)
/*!
 * @brief Handler for ENET transmission interrupts for ring 1.
 *
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_Transmit1IRQHandler(uint8_t instance);

/*!
 * @brief Handler for ENET reception interrupts for ring 1.
 *
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 * @param[in] instance Instance number
 */
void ENET_Receive1IRQHandler(uint8_t instance);

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

#if (FEATURE_ENET_RING_COUNT > 2U)

/*!
 * @brief Handler for ENET transmission interrupts for ring 2.
 *
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_Transmit2IRQHandler(uint8_t instance);

/*!
 * @brief Handler for ENET reception interrupts for ring 2.
 *
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 * @param[in] instance Instance number
 */
void ENET_Receive2IRQHandler(uint8_t instance);

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

/*!
 * @brief Handler for ENET error interrupts.
 *
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_ErrorIRQHandler(uint8_t instance);

#if FEATURE_ENET_HAS_SLEEP_WAKEUP

/*!
 * @brief Handler for ENET wakeup interrupts.
 *
 * This handler invokes the installed callback, if available, and clears
 * the interrupt flags. Also, the ENET module is set to normal operation mode.
 *
 * @param[in] instance Instance number
 */
void ENET_WakeIRQHandler(uint8_t instance);

#endif /* FEATURE_ENET_HAS_SLEEP_WAKEUP */

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*!
 * @brief Handler for ENET timer interrupts.
 *
 * This handler invokes the installed callback (if available) and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_TimerIRQHandler(uint8_t instance);

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

#if FEATURE_ENET_HAS_RECEIVE_PARSER

/*!
 * @brief Handler for ENET receive parser.
 *
 * This handler invokes the installed callback (if available) and clears the
 * interrupt flags.
 *
 * @param[in] instance Instance number
 */
void ENET_ParserIRQHandler(uint8_t instance);

#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */

#if defined(__cplusplus)
}
#endif

#endif  /* ENET_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
