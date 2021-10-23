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
 * @file enet_driver.c
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
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbol is declared in the hardware access file as external; it is needed
 * for accessing the installed callback, but is not a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter could be declared as
 * pointing to const
 * This is a pointer to a buffer which is used by the internal Rx mechanism, and
 * the application must make no assumption that the content of the buffer will not
 * be modified.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 */

#include "enet_driver.h"
#include "enet_hw_access.h"
#include "clock_manager.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ENET_ROUNDED_UP_DIV(a, b)   (((uint32_t)(a) + (uint32_t)(b) - 1UL) / (uint32_t)(b))
#define ENET_NS_IN_SECOND           (1000000000U)

#define ENET_ALL_INTERRUPTS_MASK    (0xFFFFFFFFU)

#define ENET_BYTE_0_IN_ADDR_SHIFT   (24U)
#define ENET_BYTE_1_IN_ADDR_SHIFT   (16U)
#define ENET_BYTE_2_IN_ADDR_SHIFT   (8U)
#define ENET_BYTE_3_IN_ADDR_SHIFT   (0U)
#define ENET_BYTE_4_IN_ADDR_SHIFT   (8U)
#define ENET_BYTE_5_IN_ADDR_SHIFT   (0U)

#define ENET_BYTE_MASK  (0xFFU)

#if FEATURE_ENET_HAS_INTCOAL
#define ENET_IC_CLOCK_PERIODS       (64U)
#define ENET_US_IN_SECOND           (1000000U)
#endif /* FEATURE_ENET_HAS_INTCOAL */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to ENET internal driver state for each instance. */
enet_state_t *g_enetState[ENET_INSTANCE_COUNT] = {NULL};

#ifdef FEATURE_ENET_TX_IRQS
/*! @brief ENET transmit IRQ number for each instance. */
static const IRQn_Type s_enetTxIrqId[] = FEATURE_ENET_TX_IRQS;
#endif /* FEATURE_ENET_TX_IRQS */
#ifdef FEATURE_ENET_RX_IRQS
/*! @brief ENET receive IRQ number for each instance. */
static const IRQn_Type s_enetRxIrqId[] = FEATURE_ENET_RX_IRQS;
#endif /* FEATURE_ENET_RX_IRQS */
#ifdef FEATURE_ENET_TX_1_IRQS
/*! @brief ENET transmit IRQ number for each instance. */
static const IRQn_Type s_enetTx1IrqId[] = FEATURE_ENET_TX_1_IRQS;
#endif /* FEATURE_ENET_TX_1_IRQS */
#ifdef FEATURE_ENET_RX_1_IRQS
/*! @brief ENET receive IRQ number for each instance. */
static const IRQn_Type s_enetRx1IrqId[] = FEATURE_ENET_RX_1_IRQS;
#endif /* FEATURE_ENET_RX_1_IRQS */
#ifdef FEATURE_ENET_TX_2_IRQS
/*! @brief ENET transmit IRQ number for each instance. */
static const IRQn_Type s_enetTx2IrqId[] = FEATURE_ENET_TX_2_IRQS;
#endif /* FEATURE_ENET_TX_2_IRQS */
#ifdef FEATURE_ENET_RX_2_IRQS
/*! @brief ENET receive IRQ number for each instance. */
static const IRQn_Type s_enetRx2IrqId[] = FEATURE_ENET_RX_2_IRQS;
#endif /* FEATURE_ENET_RX_2_IRQS */
#ifdef FEATURE_ENET_ERR_IRQS
/*! @brief ENET error IRQ number for each instance. */
static const IRQn_Type s_enetErrIrqId[] = FEATURE_ENET_ERR_IRQS;
#endif /* FEATURE_ENET_ERR_IRQS */
#ifdef FEATURE_ENET_TIMER_IRQS
/*! @brief ENET timer IRQ number for each instance. */
static const IRQn_Type s_enetTimerIrqId[] = FEATURE_ENET_TIMER_IRQS;
#endif /* FEATURE_ENET_TIMER_IRQS */
#ifdef FEATURE_ENET_TS_IRQS
/*! @brief ENET time stamp IRQ number for each instance. */
static const IRQn_Type s_enetTSIrqId[] = FEATURE_ENET_TS_IRQS;
#endif /* FEATURE_ENET_TS_IRQS */
#ifdef FEATURE_ENET_PARSER_IRQS
/*! @brief ENET parser IRQ number for each instance. */
static const IRQn_Type s_enetParserIrqId[] = FEATURE_ENET_PARSER_IRQS;
#endif /* FEATURE_ENET_PARSER_IRQS */
#ifdef FEATURE_ENET_WAKE_IRQS
/*! @brief ENET wakeup IRQ number for each instance. */
static const IRQn_Type s_enetWakeupIrqId[] = FEATURE_ENET_WAKE_IRQS;
#endif /* FEATURE_ENET_WAKE_IRQS */
#ifdef FEATURE_ENET_ALL_IRQS
/*! @brief ENET IRQ numbers for each instance. */
static const IRQn_Type s_enetAllIrqId[ENET_INSTANCE_COUNT][ENET_ALL_IRQS_COUNT] = FEATURE_ENET_ALL_IRQS;
#endif /* FEATURE_ENET_ALL_IRQS */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void ENET_DRV_SetTxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *txBdStartAlign,
                                        uint32_t txRingSize,
                                        uint8_t ring);

static void ENET_DRV_SetRxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *rxBdStartAlign,
                                        uint32_t rxRingSize,
                                        uint8_t *rxBuffStartAlign,
                                        uint16_t buffSize);

static uint32_t ENET_DRV_ComputeCRC32(const uint8_t *mac);

#if FEATURE_ENET_HAS_INTCOAL
static uint16_t ENET_DRV_ComputeICTimerThreshold(uint8_t instance, uint16_t us);
#endif /* FEATURE_ENET_HAS_INTCOAL */

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetTxBufferDescriptors
 * Description   : Configures the software transmit buffer descriptors.
 *
 *END**************************************************************************/
static void ENET_DRV_SetTxBufferDescriptors(const enet_config_t *config,
                                            enet_buffer_descriptor_t *txBdStartAlign,
                                            uint32_t txRingSize,
                                            uint8_t ring)
{
    uint32_t count;
    enet_buffer_descriptor_t *curBuffDescrip = txBdStartAlign;

#ifdef FEATURE_ENET_TX_MIN_RING_SIZE
    DEV_ASSERT(txRingSize >= FEATURE_ENET_TX_MIN_RING_SIZE);
#endif

    for (count = 0; count < txRingSize; count++)
    {
        curBuffDescrip->control = ENET_BUFFDESCR_TX_TRANSMITCRC_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
#if (FEATURE_ENET_RING_COUNT > 1U)
        curBuffDescrip->enh1 = (uint32_t)ring << (uint32_t)ENET_TX_ENH1_FTYPE_SHIFT;
#else
        (void)ring;
        curBuffDescrip->enh1 = 0UL;
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
        curBuffDescrip->enh2 = 0UL;
        curBuffDescrip->enh3 = 0UL;
        if ((config->interrupts & ENET_TX_INTERRUPTS_MASK) != 0UL)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_INT_MASK;
        }
        if ((config->txAccelerConfig & ENET_TACC_IPCHK_MASK) != 0U)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_IINS_MASK;
        }
        if ((config->txAccelerConfig & ENET_TACC_PROCHK_MASK) != 0U)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_PINS_MASK;
        }
        curBuffDescrip->reserved1 = 0UL;
        curBuffDescrip->reserved2 = 0UL;
#else
        (void)ring;
        (void)config;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == (txRingSize - 1U))
        {
            curBuffDescrip->control |= ENET_BUFFDESCR_TX_WRAP_MASK;
        }

        /* Increase the index. */
        curBuffDescrip++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetRxBufferDescriptors
 * Description   : Configures the software receive buffer descriptors.
 *
 *END**************************************************************************/
static void ENET_DRV_SetRxBufferDescriptors(const enet_config_t *config,
                                            enet_buffer_descriptor_t *rxBdStartAlign,
                                            uint32_t rxRingSize,
                                            uint8_t *rxBuffStartAlign,
                                            uint16_t buffSize)
{
    enet_buffer_descriptor_t *curBuffDescrip = rxBdStartAlign;
    uint32_t count = 0;

    /* Initializes receive buffer descriptors. */
    for (count = 0; count < rxRingSize; count++)
    {
        /* Set data buffer and the length. */
        curBuffDescrip->buffer = &rxBuffStartAlign[count * buffSize];
        curBuffDescrip->length = 0;

        /* Initializes the buffer descriptors with empty bit. */
        curBuffDescrip->control = ENET_BUFFDESCR_RX_EMPTY_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
        curBuffDescrip->enh1 = 0UL;
        curBuffDescrip->enh2 = 0UL;
        curBuffDescrip->enh3 = 0UL;
        if ((config->interrupts & ENET_RX_INTERRUPTS_MASK) != 0UL)
        {
            curBuffDescrip->enh1 |= ENET_RX_ENH1_INT_MASK;
        }
        curBuffDescrip->reserved1 = 0UL;
        curBuffDescrip->reserved2 = 0UL;
#else
        (void)config;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == (rxRingSize - 1U))
        {
            curBuffDescrip->control |= ENET_BUFFDESCR_RX_WRAP_MASK;
        }

        /* Increase the index. */
        curBuffDescrip++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ComputeCRC32
 * Description   : Computes the CRC32 of a given MAC address.
 *
 *END**************************************************************************/
static uint32_t ENET_DRV_ComputeCRC32(const uint8_t *mac)
{
    uint32_t crc = 0xFFFFFFFFUL;
    uint8_t i, j;

    for (i = 0; i < 6U; i++)
    {
        crc = crc ^ mac[i];
        for (j = 0; j < 8U; j++)
        {
            if ((crc & 0x1U) != 0U)
            {
                crc = (crc >> 1U) ^ 0xEDB88320U;
            }
            else
            {
                crc = (crc >> 1U);
            }
        }
    }

    return crc;
}

#if FEATURE_ENET_HAS_INTCOAL

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ComputeICTimerThreshold
 * Description   : Computes the timer threshold for the interrupt coalescing.
 *
 *END**************************************************************************/
static uint16_t ENET_DRV_ComputeICTimerThreshold(uint8_t instance, uint16_t us)
{
    uint32_t moduleClk, timerThreshold;
    static const clock_names_t s_enetClkNames[] = FEATURE_ENET_CLOCK_NAMES;

    (void)CLOCK_SYS_GetFreq(s_enetClkNames[instance], &moduleClk);

    DEV_ASSERT(moduleClk != 0U);
    DEV_ASSERT((moduleClk / ENET_US_IN_SECOND) != 0U);

    timerThreshold = ((uint32_t)us * ENET_IC_CLOCK_PERIODS) / (moduleClk / ENET_US_IN_SECOND);

    return ((timerThreshold <= 0xFFFFU) ? (uint16_t)timerThreshold : 0xFFFFU);
}

#endif /* FEATURE_ENET_HAS_INTCOAL */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetDefaultConfig
 * Description   : Gets the default configuration structure
 *
 * Implements    : ENET_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void ENET_DRV_GetDefaultConfig(enet_config_t *config)
{
    /* Checks input parameter. */
    DEV_ASSERT(config != NULL);

    /* No interrupt is enabled by default. */
    config->interrupts = 0;
    /* Maximum receive frame length. */
    config->maxFrameLen = ENET_FRAME_MAX_FRAMELEN;
    /* No special receive/transmit control configuration. */
#if FEATURE_ENET_HAS_RX_CONFIG
    config->rxConfig = 0;
#endif /* FEATURE_ENET_HAS_RX_CONFIG */
#if FEATURE_ENET_HAS_TX_CONFIG
    config->txConfig = 0;
#endif /* FEATURE_ENET_HAS_TX_CONFIG */
#if FEATURE_ENET_HAS_ACCELERATOR
    /* No acceleration function enabled. */
    config->rxAccelerConfig = 0;
    config->txAccelerConfig = 0;
#endif /* FEATURE_ENET_HAS_ACCELERATOR */
    /* RMII/MII mode, full duplex, 100Mbps for MAC and PHY data interface. */
    config->miiMode = FEATURE_ENET_DEFAULT_PHY_IF;
    config->miiSpeed = ENET_MII_SPEED_100M;
    config->miiDuplex = ENET_MII_FULL_DUPLEX;
    /* No callback. */
    config->callback = NULL;
    /* One ring. */
    config->ringCount = 1U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_Init
 * Description   : Initializes the ENET module
 *
 * This function initializes and enables the ENET module, configuring receive
 * and transmit control settings, the receive and transmit descriptors rings,
 * and the MAC physical address.
 *
 * Implements    : ENET_DRV_Init_Activity
 *END**************************************************************************/
void ENET_DRV_Init(uint8_t instance,
                   enet_state_t *state,
                   const enet_config_t *config,
                   const enet_buffer_config_t bufferConfig[],
                   const uint8_t *macAddr)
{
    ENET_Type *base;
    uint8_t i;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(state != NULL);
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(config->maxFrameLen >= ENET_MIN_BUFFERSIZE);
    DEV_ASSERT(bufferConfig != NULL);
    DEV_ASSERT((config->ringCount > 0U) && (config->ringCount <= FEATURE_ENET_RING_COUNT));
#ifdef DEV_ERROR_DETECT
    for (i = 0U; i < config->ringCount; i++)
    {
        DEV_ASSERT(bufferConfig[i].rxRingAligned != NULL);
        DEV_ASSERT(ENET_BUFFDESCR_IS_ALIGNED(bufferConfig[i].rxRingAligned));
        DEV_ASSERT(bufferConfig[i].rxBufferAligned != NULL);
        DEV_ASSERT(ENET_BUFF_IS_ALIGNED(bufferConfig[i].rxBufferAligned));
        DEV_ASSERT(bufferConfig[i].rxRingSize > 0U);
        DEV_ASSERT(bufferConfig[i].txRingAligned != NULL);
        DEV_ASSERT(ENET_BUFFDESCR_IS_ALIGNED(bufferConfig[i].txRingAligned));
        DEV_ASSERT(bufferConfig[i].txRingSize > 0U);
    }
#endif /* DEV_ERROR_DETECT */
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];

    /* Reset ENET module. */
    ENET_Reset(base);

    /* Configure MAC address. */
    ENET_DRV_SetMacAddr(instance, macAddr);

    /* Configure receive and transmit control. */
#if FEATURE_ENET_HAS_RX_CONFIG
    ENET_ConfigReceiveControl(base, config);
#endif /* FEATURE_ENET_HAS_RX_CONFIG */
#if FEATURE_ENET_HAS_TX_CONFIG
    ENET_ConfigTransmitControl(base, config);
#endif /* FEATURE_ENET_HAS_TX_CONFIG */

#if FEATURE_ENET_HAS_ACCELERATOR
    /* Configure receive and transmit accelerators. */
    ENET_ConfigReceiveAccelerator(base, config->rxAccelerConfig);
    ENET_ConfigTransmitAccelerator(base, config->txAccelerConfig);
#endif /* FEATURE_ENET_HAS_ACCELERATOR */

    /* Enable Ethernet interrupts. */
#ifdef FEATURE_ENET_RX_IRQS
    INT_SYS_EnableIRQ(s_enetRxIrqId[instance]);
#endif /* FEATURE_ENET_RX_IRQS */
#ifdef FEATURE_ENET_TX_IRQS
    INT_SYS_EnableIRQ(s_enetTxIrqId[instance]);
#endif /* FEATURE_ENET_TX_IRQS */
#ifdef FEATURE_ENET_RX_1_IRQS
    INT_SYS_EnableIRQ(s_enetRx1IrqId[instance]);
#endif /* FEATURE_ENET_RX_1_IRQS */
#ifdef FEATURE_ENET_TX_1_IRQS
    INT_SYS_EnableIRQ(s_enetTx1IrqId[instance]);
#endif /* FEATURE_ENET_TX_1_IRQS */
#ifdef FEATURE_ENET_RX_2_IRQS
    INT_SYS_EnableIRQ(s_enetRx2IrqId[instance]);
#endif /* FEATURE_ENET_RX_2_IRQS */
#ifdef FEATURE_ENET_TX_2_IRQS
    INT_SYS_EnableIRQ(s_enetTx2IrqId[instance]);
#endif /* FEATURE_ENET_TX_2_IRQS */
#ifdef FEATURE_ENET_ERR_IRQS
    INT_SYS_EnableIRQ(s_enetErrIrqId[instance]);
#endif /* FEATURE_ENET_ERR_IRQS */
#ifdef FEATURE_ENET_TIMER_IRQS
    INT_SYS_EnableIRQ(s_enetTimerIrqId[instance]);
#endif /* FEATURE_ENET_TIMER_IRQS */
#ifdef FEATURE_ENET_TS_IRQS
    INT_SYS_EnableIRQ(s_enetTSIrqId[instance]);
#endif /* FEATURE_ENET_TS_IRQS */
#ifdef FEATURE_ENET_PARSER_IRQS
    INT_SYS_EnableIRQ(s_enetParserIrqId[instance]);
#endif /* FEATURE_ENET_PARSER_IRQS */
#ifdef FEATURE_ENET_WAKE_IRQS
    INT_SYS_EnableIRQ(s_enetWakeupIrqId[instance]);
#endif /* FEATURE_ENET_WAKE_IRQS */
#ifdef FEATURE_ENET_ALL_IRQS
    for (i = 0; i < ENET_ALL_IRQS_COUNT; i++)
    {
        INT_SYS_EnableIRQ(s_enetAllIrqId[instance][i]);
    }
#endif /* FEATURE_ENET_ALL_IRQS */

    ENET_EnableInterrupts(base, config->interrupts);
#ifdef ERRATA_E6358
    ENET_EnableInterrupts(base, (uint32_t)ENET_TX_FRAME_INTERRUPT);
#endif /* ERRATA_E6358 */

    /* Update state. */
    for (i = 0U; i < config->ringCount; i++)
    {
        state->rxBdBase[i] = bufferConfig[i].rxRingAligned;
        state->rxBdCurrent[i] = bufferConfig[i].rxRingAligned;
        state->rxBdAlloc[i] = bufferConfig[i].rxRingAligned;
        state->txBdBase[i] = bufferConfig[i].txRingAligned;
        state->txBdCurrent[i] = bufferConfig[i].txRingAligned;
    }
    state->callback = config->callback;
    state->ringCount = config->ringCount;

    g_enetState[instance] = state;

    /* Configure buffer descriptors. */
    for (i = 0U; i < config->ringCount; i++)
    {
        /* Initializes the ENET transmit buffer descriptors. */
        ENET_DRV_SetTxBufferDescriptors(config, bufferConfig[i].txRingAligned, bufferConfig[i].txRingSize, i);

        /* Initializes the ENET receive buffer descriptors. */
        ENET_DRV_SetRxBufferDescriptors(config, bufferConfig[i].rxRingAligned, bufferConfig[i].rxRingSize,
                                    bufferConfig[i].rxBufferAligned, (uint16_t)ENET_BUFF_ALIGN(config->maxFrameLen));

        ENET_ConfigBufferDescriptors(base, i, &bufferConfig[i], (uint16_t)ENET_BUFF_ALIGN(config->maxFrameLen));
    }

    ENET_DRV_ConfigCounters(instance, true);

    /* Enable the module. */
    ENET_Enable(base);

    /* Indicate that the receive descriptors have been updated. */
    for (i = 0U; i < config->ringCount; i++)
    {
        ENET_ActivateReceive(base, i);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_Deinit
 * Description   : Deinitializes the ENET module
 *
 * This function disables the interrupts and then disables the ENET module.
 *
 * Implements    : ENET_DRV_Deinit_Activity
 *END**************************************************************************/
void ENET_DRV_Deinit(uint8_t instance)
{
    ENET_Type *base;
#ifdef FEATURE_ENET_ALL_IRQS
    uint8_t i;
#endif /* FEATURE_ENET_ALL_IRQS */

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    /* Disable interrupts. */
    ENET_DisableInterrupts(base, ENET_ALL_INTERRUPTS_MASK);

#ifdef FEATURE_ENET_RX_IRQS
    INT_SYS_DisableIRQ(s_enetRxIrqId[instance]);
#endif /* FEATURE_ENET_RX_IRQS */
#ifdef FEATURE_ENET_TX_IRQS
    INT_SYS_DisableIRQ(s_enetTxIrqId[instance]);
#endif /* FEATURE_ENET_TX_IRQS */
#ifdef FEATURE_ENET_RX_1_IRQS
    INT_SYS_DisableIRQ(s_enetRx1IrqId[instance]);
#endif /* FEATURE_ENET_RX_1_IRQS */
#ifdef FEATURE_ENET_TX_1_IRQS
    INT_SYS_DisableIRQ(s_enetTx1IrqId[instance]);
#endif /* FEATURE_ENET_TX_1_IRQS */
#ifdef FEATURE_ENET_RX_2_IRQS
    INT_SYS_DisableIRQ(s_enetRx2IrqId[instance]);
#endif /* FEATURE_ENET_RX_2_IRQS */
#ifdef FEATURE_ENET_TX_2_IRQS
    INT_SYS_DisableIRQ(s_enetTx2IrqId[instance]);
#endif /* FEATURE_ENET_TX_2_IRQS */
#ifdef FEATURE_ENET_ERR_IRQS
    INT_SYS_DisableIRQ(s_enetErrIrqId[instance]);
#endif /* FEATURE_ENET_ERR_IRQS */
#ifdef FEATURE_ENET_TIMER_IRQS
    INT_SYS_DisableIRQ(s_enetTimerIrqId[instance]);
#endif /* FEATURE_ENET_TIMER_IRQS */
#ifdef FEATURE_ENET_TS_IRQS
    INT_SYS_DisableIRQ(s_enetTSIrqId[instance]);
#endif /* FEATURE_ENET_TS_IRQS */
#ifdef FEATURE_ENET_PARSER_IRQS
    INT_SYS_DisableIRQ(s_enetParserIrqId[instance]);
#endif /* FEATURE_ENET_PARSER_IRQS */
#ifdef FEATURE_ENET_WAKE_IRQS
    INT_SYS_DisableIRQ(s_enetWakeupIrqId[instance]);
#endif /* FEATURE_ENET_WAKE_IRQS */
#ifdef FEATURE_ENET_ALL_IRQS
    for (i = 0; i < ENET_ALL_IRQS_COUNT; i++)
    {
        INT_SYS_DisableIRQ(s_enetAllIrqId[instance][i]);
    }
#endif /* FEATURE_ENET_ALL_IRQS */

    /* Disable module. */
    ENET_Disable(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ReadFrame
 * Description   : Reads a received Ethernet frame
 *
 * This function reads the first received Ethernet frame in the Rx queue. The
 * buffer received as parameter will be updated by the driver and the .data field
 * will point to a memory area containing the frame data.
 *
 * Important: Once the application finished processing the buffer, it needs to be
 * released using the ENET_DRV_ProvideRxBuff function.
 *
 * Implements    : ENET_DRV_ReadFrame_Activity
 *END**************************************************************************/
status_t ENET_DRV_ReadFrame(uint8_t instance,
                            uint8_t queue,
                            enet_buffer_t * buff,
                            enet_rx_enh_info_t * info)
{
    enet_buffer_descriptor_t *bd;
    status_t status = STATUS_SUCCESS;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(queue < g_enetState[instance]->ringCount);
    DEV_ASSERT(buff != NULL);

    bd = g_enetState[instance]->rxBdCurrent[queue];

    if ((bd->control & (ENET_BUFFDESCR_RX_EMPTY_MASK | ENET_BUFFDESCR_RX_INUSE_MASK)) != 0U)
    {
        status = STATUS_ENET_RX_QUEUE_EMPTY;
    }
    else
    {
        buff->data = bd->buffer;
        buff->length = bd->length;

        bd->control |= ENET_BUFFDESCR_RX_INUSE_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
        if (info != NULL)
        {
            info->errMask = bd->enh1 & ENET_RX_ENH_ERR_MASK;
            info->ipv4Frag = ((bd->enh1 & ENET_RX_ENH1_FRAG_MASK) != 0UL);
            info->ipv6 = ((bd->enh1 & ENET_RX_ENH1_IPV6_MASK) != 0UL);
            info->unicast = ((bd->enh1 & ENET_RX_ENH1_UNICAST_MASK) != 0UL);
            info->vlanPrio = (uint8_t)((bd->enh1 & ENET_RX_ENH1_VPCP_MASK) >> ENET_RX_ENH1_VPCP_SHIFT);
            info->checksum = (uint16_t)(bd->enh2 & ENET_RX_ENH2_CSUM_MASK);
            info->headerLen = (uint8_t)((bd->enh2 & ENET_RX_ENH2_HLEN_MASK) >> ENET_RX_ENH2_HLEN_SHIFT);
            info->proto = (uint8_t)((bd->enh2 & ENET_RX_ENH2_PROTO_MASK) >> ENET_RX_ENH2_PROTO_SHIFT);
            info->timestamp = bd->timestamp;
        }
#else
        DEV_ASSERT(info == NULL);
        (void)info;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Update the current buffer descriptor. */
        if ((bd->control & ENET_BUFFDESCR_RX_WRAP_MASK) != 0U)
        {
           g_enetState[instance]->rxBdCurrent[queue] = g_enetState[instance]->rxBdBase[queue];
        }
        else
        {
            g_enetState[instance]->rxBdCurrent[queue]++;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SendFrame
 * Description   : Sends an Ethernet frame
 *
 * This function sends an Ethernet frame, represented by the buffer received as
 * parameter.
 *
 * Implements    : ENET_DRV_SendFrame_Activity
 *END**************************************************************************/
status_t ENET_DRV_SendFrame(uint8_t instance,
                            uint8_t queue,
                            const enet_buffer_t * buff,
                            enet_tx_options_t * options)
{
    ENET_Type *base;
    enet_buffer_descriptor_t *bd;
    status_t status = STATUS_SUCCESS;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(queue < g_enetState[instance]->ringCount);
    DEV_ASSERT(buff != NULL);

    base = s_enetBases[instance];

    bd = g_enetState[instance]->txBdCurrent[queue];

    if ((bd->control & ENET_BUFFDESCR_TX_READY_MASK) != 0U)
    {
        status = STATUS_ENET_TX_QUEUE_FULL;
    }
    else
    {
        /* Configure buffer descriptor. */
        bd->length = buff->length;
        bd->buffer = buff->data;
        bd->control |= (uint16_t)(ENET_BUFFDESCR_TX_READY_MASK | ENET_BUFFDESCR_TX_LAST_MASK | ENET_BUFFDESCR_TX_TRANSMITCRC_MASK);
#if FEATURE_ENET_HAS_ENHANCED_BD
        bd->enh1 |= ENET_TX_ENH1_INT_MASK;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        if (options != NULL)
        {
            if (options->noCRC)
            {
                bd->control &= ~ENET_BUFFDESCR_TX_TRANSMITCRC_MASK;
            }
#if FEATURE_ENET_HAS_ENHANCED_BD
            if (options->noInt)
            {
                bd->enh1 &= ~ENET_TX_ENH1_INT_MASK;
            }
#if FEATURE_ENET_HAS_TBS
            if (options->useTLT)
            {
                bd->enh1 |= ENET_TX_ENH1_UTLT_MASK;
                bd->enh2 = options->TLT;
            }
#endif /* FEATURE_ENET_HAS_TBS */
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
        }
        /* Activate the transmit buffer descriptor. */
        ENET_ActivateTransmit(base, queue);

        /* Update the current buffer descriptor pointer. */
        if ((bd->control & ENET_BUFFDESCR_TX_WRAP_MASK) != 0U)
        {
            g_enetState[instance]->txBdCurrent[queue] = g_enetState[instance]->txBdBase[queue];
        }
        else
        {
            g_enetState[instance]->txBdCurrent[queue]++;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetTransmitStatus
 * Description   : Checks if the transmission of a buffer is complete
 *
 * This function checks if the transmission of the given buffer is complete.
 *
 * Implements    : ENET_DRV_GetTransmitStatus_Activity
 *END**************************************************************************/
status_t ENET_DRV_GetTransmitStatus(uint8_t instance,
                                    uint8_t queue,
                                    const enet_buffer_t * buff,
                                    enet_tx_enh_info_t * info)
{
    const enet_buffer_descriptor_t *bd;
    status_t status = STATUS_ENET_BUFF_NOT_FOUND;
    bool finished = false;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(queue < g_enetState[instance]->ringCount);
    DEV_ASSERT(buff != NULL);
#if (FEATURE_ENET_HAS_ENHANCED_BD == 0)
    DEV_ASSERT(info == NULL);
#endif /* !FEATURE_ENET_HAS_ENHANCED_BD */

    bd = g_enetState[instance]->txBdBase[queue];

    /* Search for the buffer descriptor associated with the given buffer. */
    while (!finished)
    {
        if (bd->buffer == buff->data)
        {
            /* Check if the buffer descriptor is still in use. */
            if ((bd->control & ENET_BUFFDESCR_TX_READY_MASK) != 0U)
            {
                status = STATUS_BUSY;
            }
            else
            {
                status = STATUS_SUCCESS;
#if FEATURE_ENET_HAS_ENHANCED_BD
                if (info != NULL)
                {
                    info->errMask = bd->enh1 & ENET_TX_ENH_ERR_MASK;
                    info->timestamp = bd->timestamp;
                }
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
            }

            break;
        }

        /* This was the last buffer in the ring. */
        if ((bd->control & ENET_BUFFDESCR_TX_WRAP_MASK) != 0U)
        {
            finished = true;
        }

        bd++;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ProvideRxBuff
 * Description   : Provides a receive buffer to be used by the driver for reception.
 *
 * This function provides a buffer which can further be used by the reception
 * mechanism in order to store the received data.
 *
 * Implements    : ENET_DRV_ProvideRxBuff_Activity
 *END**************************************************************************/
void ENET_DRV_ProvideRxBuff(uint8_t instance,
                            uint8_t queue,
                            enet_buffer_t * buff)
{
    ENET_Type *base;
    enet_buffer_descriptor_t *bd;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(queue < g_enetState[instance]->ringCount);
    DEV_ASSERT(buff != NULL);

    base = s_enetBases[instance];

    /* This is the left-most allocated buffer descriptor. */
    bd = g_enetState[instance]->rxBdAlloc[queue];

    DEV_ASSERT((bd->control & ENET_BUFFDESCR_RX_EMPTY_MASK) == 0U);

    bd->buffer = buff->data;

    /* Cleanup the left-most allocated buffer descriptor. */
    bd->control &= ENET_BUFFDESCR_RX_WRAP_MASK;
    bd->control |= ENET_BUFFDESCR_RX_EMPTY_MASK;

    /* Update the pointer to the first allocated buffer descriptor. */
    if ((bd->control & ENET_BUFFDESCR_RX_WRAP_MASK) != 0U)
    {
        g_enetState[instance]->rxBdAlloc[queue] = g_enetState[instance]->rxBdBase[queue];
    }
    else
    {
        g_enetState[instance]->rxBdAlloc[queue]++;
    }

    /* New empty buffers have been produced */
    ENET_ActivateReceive(base, 0U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_EnableMDIO
 * Description   : Enables the MDIO interface
 *
 * Implements    : ENET_DRV_EnableMDIO_Activity
 *END**************************************************************************/
void ENET_DRV_EnableMDIO(uint8_t instance,
                         bool miiPreambleDisabled)
{
    ENET_Type *base;
    uint32_t moduleClk, holdTime, miiSpeed;
    const clock_names_t s_enetClkNames[] = FEATURE_ENET_CLOCK_NAMES;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    (void)CLOCK_SYS_GetFreq(s_enetClkNames[instance], &moduleClk);

    DEV_ASSERT(moduleClk != 0U);

    holdTime = ENET_ROUNDED_UP_DIV(FEATURE_ENET_MDIO_MIN_HOLD_TIME_NS * moduleClk, ENET_NS_IN_SECOND) - 1U;
    miiSpeed = ENET_ROUNDED_UP_DIV(moduleClk, (2U * FEATURE_ENET_MDC_MAX_FREQUENCY)) - 1U;

    ENET_WriteManagementConfig(base, miiSpeed, holdTime, miiPreambleDisabled);

    /* Ensure that the timer is initialized */
    OSIF_TimeDelay(0U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIOWrite
 * Description   : Writes the selected register of the PHY
 *
 * Implements    : ENET_DRV_MDIOWrite_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIOWrite(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t phyReg,
                            uint16_t data,
                            uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    startTime = OSIF_GetMilliseconds();
    /* Write management frame */
    status = ENET_WriteManagementFrame(base, phyAddr, phyReg, ENET_MMFR_OP_WRITE, data, false, startTime, timeoutMs);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIORead
 * Description   : Reads the selected register of the PHY
 *
 * Implements    : ENET_DRV_MDIORead_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIORead(uint8_t instance,
                           uint8_t phyAddr,
                           uint8_t phyReg,
                           uint16_t *data,
                           uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    startTime = OSIF_GetMilliseconds();
    /* Write management frame */
    status = ENET_WriteManagementFrame(base, phyAddr, phyReg, ENET_MMFR_OP_READ, 0, false, startTime, timeoutMs);

    if (status == STATUS_SUCCESS)
    {
        *data = ENET_ReadManagementFrameData(base);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIOWriteMMD
 * Description   : Writes a register of the specified MMD in a PHY device
 *
 * Implements    : ENET_DRV_MDIOWriteMMD_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIOWriteMMD(uint8_t instance,
                               uint8_t phyAddr,
                               uint8_t mmd,
                               uint16_t phyReg,
                               uint16_t data,
                               uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    startTime = OSIF_GetMilliseconds();
    /* Write management frame containing PHY register address */
    status = ENET_WriteManagementFrame(base, phyAddr, mmd, ENET_MMFR_OP_ADDR, phyReg, true, startTime, timeoutMs);

    if (status == STATUS_SUCCESS)
    {
        /* Write management frame containing data */
        status = ENET_WriteManagementFrame(base, phyAddr, mmd, ENET_MMFR_OP_WRITE, data, true, startTime, timeoutMs);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIOReadMMD
 * Description   : Reads a register of the specified MMD in a PHY device
 *
 * Implements    : ENET_DRV_MDIOReadMMD_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIOReadMMD(uint8_t instance,
                              uint8_t phyAddr,
                              uint8_t mmd,
                              uint16_t phyReg,
                              uint16_t *data,
                              uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    startTime = OSIF_GetMilliseconds();
    /* Write management frame containing PHY register address */
    status = ENET_WriteManagementFrame(base, phyAddr, mmd, ENET_MMFR_OP_ADDR, phyReg, true, startTime, timeoutMs);

    if (status == STATUS_SUCCESS)
    {
        /* Write management frame to read data */
        status = ENET_WriteManagementFrame(base, phyAddr, mmd, ENET_MMFR_OP_READ_45, 0, true, startTime, timeoutMs);

        if (status == STATUS_SUCCESS)
        {
            *data = ENET_ReadManagementFrameData(base);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMacAddr
 * Description   : Configures the physical address of the MAC
 *
 * Implements    : ENET_DRV_SetMacAddr_Activity
 *END**************************************************************************/
void ENET_DRV_SetMacAddr(uint8_t instance,
                         const uint8_t *macAddr)
{
    uint32_t address;
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];

    /* Set physical address lower register. */
    address = ((uint32_t)macAddr[0] << ENET_BYTE_0_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[1] << ENET_BYTE_1_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[2] << ENET_BYTE_2_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[3] << ENET_BYTE_3_IN_ADDR_SHIFT);
    ENET_SetPhyAddrLower(base, address);

    /* Set physical address high register. */
    address = ((uint32_t)macAddr[4] << ENET_BYTE_4_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[5] << ENET_BYTE_5_IN_ADDR_SHIFT);

    ENET_SetPhyAddrUpper(base, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetMacAddr
 * Description   : Gets the physical address of the MAC
 *
 * Implements    : ENET_DRV_GetMacAddr_Activity
 *END**************************************************************************/
void ENET_DRV_GetMacAddr(uint8_t instance,
                         uint8_t *macAddr)
{
    const ENET_Type *base;
    uint32_t address;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];

    /* Get from physical address lower register. */
    address = ENET_GetPhyAddrLower(base);
    macAddr[0] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_0_IN_ADDR_SHIFT));
    macAddr[1] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_1_IN_ADDR_SHIFT));
    macAddr[2] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_2_IN_ADDR_SHIFT));
    macAddr[3] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_3_IN_ADDR_SHIFT));

    /* Get from physical address high register. */
    address = ENET_GetPhyAddrUpper(base);
    macAddr[4] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_4_IN_ADDR_SHIFT));
    macAddr[5] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_5_IN_ADDR_SHIFT));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetUnicastForward
 * Description   : Enables/Disables forwarding of unicast traffic having a
 * specific MAC address as destination.
 *
 * Implements    : ENET_DRV_SetUnicastForward_Activity
 *END**************************************************************************/
void ENET_DRV_SetUnicastForward(uint8_t instance,
                                const uint8_t *macAddr,
                                bool enable)
{
    ENET_Type *base;
    uint32_t crc;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];
    crc = ENET_DRV_ComputeCRC32(macAddr);

    if (enable)
    {
        ENET_AddToIndividualHashTable(base, crc);
    }
    else
    {
        ENET_RemoveFromIndividualHashTable(base, crc);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMulticastForward
 * Description   : Enables/Disables forwarding of multicast traffic having a
 * specific MAC address as destination.
 *
 * Implements    : ENET_DRV_SetMulticastForward_Activity
 *END**************************************************************************/
void ENET_DRV_SetMulticastForward(uint8_t instance,
                                  const uint8_t *macAddr,
                                  bool enable)
{
    ENET_Type *base;
    uint32_t crc;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];
    crc = ENET_DRV_ComputeCRC32(macAddr);

    if (enable)
    {
        ENET_AddToGroupHashTable(base, crc);
    }
    else
    {
        ENET_RemoveFromGroupHashTable(base, crc);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMulticastForwardAll
 * Description   : Enables/Disables forwarding of the multicast traffic, irrespective
 * of the destination MAC address.
 *
 * Implements    : ENET_DRV_SetMulticastForwardAll_Activity
 *END**************************************************************************/
void ENET_DRV_SetMulticastForwardAll(uint8_t instance,
                                     bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    if (enable)
    {
        base->GAUR = 0xFFFFFFFFU;
        base->GALR = 0xFFFFFFFFU;
    }
    else
    {
        base->GAUR = 0x0U;
        base->GALR = 0x0U;
    }
}

#if FEATURE_ENET_HAS_SLEEP_WAKEUP

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetSleepMode
 * Description   : Sets the MAC in sleep mode or normal mode
 *
 * Implements    : ENET_DRV_SetSleepMode_Activity
 *END**************************************************************************/
void ENET_DRV_SetSleepMode(uint8_t instance,
                           bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base  = s_enetBases[instance];

    if (enable)
    {
        base->ECR |= ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK;
    }
    else
    {
        base->ECR &= ~(ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK);
    }
}

#endif /* FEATURE_ENET_HAS_SLEEP_WAKEUP */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigCounters
 * Description   : Enables/Disables the MIB counters
 *
 * Implements    : ENET_DRV_ConfigCounters_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigCounters(uint8_t instance,
                             bool enable)
{
    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    ENET_ConfigCounters(instance, enable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetCounter
 * Description   : Gets statistics from the specified counter
 *
 * Implements    : ENET_DRV_GetCounter_Activity
 *END**************************************************************************/
uint32_t ENET_DRV_GetCounter(uint8_t instance,
                             enet_counter_t counter)
{
    const ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    return (&(base->RMON_T_DROP))[(uint32_t)counter];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetInterruptFlags
 * Description   : Gets a mask of the interrupt events which occurred.
 *
 * Implements    : ENET_DRV_GetInterruptFlags_Activity
 *END**************************************************************************/
uint32_t ENET_DRV_GetInterruptFlags(uint8_t instance)
{
    const ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    return base->EIR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetSpeed
 * Description   : Sets the speed of the MII interface
 *
 * Implements    : ENET_DRV_SetSpeed_Activity
 *END**************************************************************************/
void ENET_DRV_SetSpeed(uint8_t instance,
                       enet_mii_speed_t speed)
{
    ENET_Type *base;

    DEV_ASSERT(instance < ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    ENET_SetSpeed(base, speed);
}

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerInit
 * Description   : Initializes the adjustable timer using the given configuration
 * and enables it.
 *
 * Implements    : ENET_DRV_TimerInit_Activity
 *END**************************************************************************/
void ENET_DRV_TimerInit(uint8_t instance,
                        enet_timer_config_t *timerConfig)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(timerConfig != NULL);
    DEV_ASSERT(g_enetState[instance] != NULL);

    base = s_enetBases[instance];

    g_enetState[instance]->timerCallback = timerConfig->callback;

    base->ATVR  = 0U;
    base->ATPER = ENET_ATPER_PERIOD(timerConfig->timerPeriod);
    base->ATCOR = ENET_ATCOR_COR(timerConfig->correctionPeriod);
    base->ATINC = ENET_ATINC_INC(timerConfig->timerInc) | ENET_ATINC_INC_CORR(timerConfig->correctionInc);
    base->ATCR |= ENET_ATCR_EN_MASK | ENET_ATCR_PEREN(timerConfig->irqEnable ? 1UL : 0UL);

    if (timerConfig->irqEnable)
    {
        base->EIMR |= ENET_EIMR_TS_TIMER_MASK;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerStart
 * Description   : Starts the timer.
 *
 * Implements    : ENET_DRV_TimerStart_Activity
 *END**************************************************************************/
void ENET_DRV_TimerStart(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->ATCR |= ENET_ATCR_EN_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerStop
 * Description   : Stops the timer.
 *
 * Implements    : ENET_DRV_TimerStop_Activity
 *END**************************************************************************/
void ENET_DRV_TimerStop(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->ATCR &= ~ENET_ATCR_EN_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerSet
 * Description   : Sets the value of the timer.
 *
 * Implements    : ENET_DRV_TimerSet_Activity
 *END**************************************************************************/
void ENET_DRV_TimerSet(uint8_t instance,
                       uint32_t value)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->ATVR = value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerGet
 * Description   : Gets the value of the timer.
 *
 * Implements    : ENET_DRV_TimerGet_Activity
 *END**************************************************************************/
void ENET_DRV_TimerGet(uint8_t instance,
                       uint32_t *value)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(value != NULL);

    base = s_enetBases[instance];

    base->ATCR |= ENET_ATCR_CAPTURE_MASK;
    while ((base->ATCR & ENET_ATCR_CAPTURE_MASK) != 0U)
    {
        /* Wait until the capture is available. */
    }

    *value = base->ATVR;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerSetCorrection
 * Description   : Sets the timer correction period and correction increment value.
 *
 * Implements    : ENET_DRV_TimerSetCorrection_Activity
 *END**************************************************************************/
void ENET_DRV_TimerSetCorrection(uint8_t instance,
                                 uint32_t value,
                                 uint32_t period)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->ATCOR = ENET_ATCOR_COR(period);
    base->ATINC &= ~ENET_ATINC_INC_CORR_MASK;
    base->ATINC |= ENET_ATINC_INC_CORR(value);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerEnableChannel
 * Description   : Configures and enables a specific timer channel.
 *
 * Implements    : ENET_DRV_TimerEnableChannel_Activity
 *END**************************************************************************/
void ENET_DRV_TimerEnableChannel(uint8_t instance,
                                 uint8_t channel,
                                 enet_timer_channel_config_t *config)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(channel < ENET_CHANNEL_COUNT);
    DEV_ASSERT(config != NULL);

    base = s_enetBases[instance];

    /* Disable channel, interrupt, DMA and clear flag */
    base->CHANNEL[channel].TCSR = ENET_TCSR_TF_MASK;

    while ((base->CHANNEL[channel].TCSR & ENET_TCSR_TMODE_MASK) != 0U)
    {
        /* Wait until the channel is disabled */
    }

    base->CHANNEL[channel].TCSR |= ENET_TCSR_TIE(config->irqEnable ? 1UL : 0UL) |
                                   ENET_TCSR_TDRE(config->dmaEnable ? 1UL : 0UL) |
                                   ENET_TCSR_TMODE(config->mode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerSetCompare
 * Description   : Sets the compare value for a channel configured as output compare.
 *
 * Implements    : ENET_DRV_TimerSetCompare_Activity
 *END**************************************************************************/
void ENET_DRV_TimerSetCompare(uint8_t instance,
                              uint8_t channel,
                              uint32_t value)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(channel < ENET_CHANNEL_COUNT);

    base = s_enetBases[instance];

    base->CHANNEL[channel].TCCR = value;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerGetCapture
 * Description   : Gets the last value captured on a specific channel configured as
 * input capture.
 *
 * Implements    : ENET_DRV_TimerGetCapture_Activity
 *END**************************************************************************/
void ENET_DRV_TimerGetCapture(uint8_t instance,
                              uint8_t channel,
                              uint32_t *capture)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(channel < ENET_CHANNEL_COUNT);
    DEV_ASSERT(capture != NULL);

    base = s_enetBases[instance];

    *capture = base->CHANNEL[channel].TCCR;

    base->CHANNEL[channel].TCSR |= ENET_TCSR_TF_MASK;
}

#if FEATURE_ENET_HAS_TIMER_PULSE_WIDTH_CONTROL

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_TimerSetPulseWidth
 * Description   : Sets the pulse width for a channel configured as output compare
 * with output in pulse mode.
 *
 * Implements    : ENET_DRV_TimerSetPulseWidth_Activity
 *END**************************************************************************/
void ENET_DRV_TimerSetPulseWidth(uint8_t instance,
                                 uint8_t channel,
                                 uint8_t pulseWidth)
{
    ENET_Type *base;
    uint32_t mode;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(channel < ENET_CHANNEL_COUNT);

    base = s_enetBases[instance];

    /* Read current mode */
    mode = base->CHANNEL[channel].TCSR & ENET_TCSR_TMODE_MASK;

    /* Disable channel */
    base->CHANNEL[channel].TCSR &= ~ENET_TCSR_TMODE_MASK;

    while ((base->CHANNEL[channel].TCSR & ENET_TCSR_TMODE_MASK) != 0U)
    {
        /* Wait until the channel is disabled */
    }

    base->CHANNEL[channel].TCSR &= ~ENET_TCSR_TPWC_MASK;
    base->CHANNEL[channel].TCSR |= ENET_TCSR_TPWC(pulseWidth) | mode;
}

#endif /* FEATURE_ENET_HAS_TIMER_PULSE_WIDTH_CONTROL */

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

#if (FEATURE_ENET_RING_COUNT > 1U)

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigTxScheme
 * Description   : Sets the transmitter queue selection/arbitration scheme.
 *
 * Implements    : ENET_DRV_ConfigTxScheme_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigTxScheme(uint8_t instance,
                             enet_tx_scheme_t scheme)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->QOS &= ~ENET_QOS_TX_SCHEME_MASK;
    base->QOS |= ENET_QOS_TX_SCHEME(scheme);
}

#if FEATURE_ENET_HAS_CBS

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigIdleSlope
 * Description   :Configures the idle slope for the additional classes
 * traffic.
 *
 * Implements    : ENET_DRV_ConfigIdleSlope_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigIdleSlope(uint8_t instance,
                              uint8_t class,
                              uint16_t idleSlope)
{
    ENET_Type *base;
    uint8_t index;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(class > 0U);

    index = class - 1U;

    DEV_ASSERT(index < ENET_DMACFG_COUNT);

    base = s_enetBases[instance];

    base->DMACFG[index] = ENET_DMACFG_DMA_CLASS_EN_MASK | ENET_DMACFG_IDLE_SLOPE(idleSlope);
}

#endif /* FEATURE_ENET_HAS_CBS */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigClassMatch
 * Description   : Configures the VLAN priority matching for the specified class.
 *
 * Implements    : ENET_DRV_ConfigClassMatch_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigClassMatch(uint8_t instance,
                               uint8_t class,
                               uint8_t vlanPrioNum,
                               uint8_t *vlanPrio)
{
    ENET_Type *base;
    uint8_t i;
    uint32_t rcmr = ENET_RCMR_MATCHEN_MASK;
    uint8_t index;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(vlanPrio != NULL);
    DEV_ASSERT(vlanPrioNum <= FEATURE_ENET_CLASS_MATCH_CMP_COUNT);
    DEV_ASSERT(class > 0U);

    index = class - 1U;

    DEV_ASSERT(index < ENET_RCMR_COUNT);

    base = s_enetBases[instance];

    for (i = 0U; i < FEATURE_ENET_CLASS_MATCH_CMP_COUNT; i++)
    {
        if (i < vlanPrioNum)
        {
            rcmr |= (uint32_t)vlanPrio[i] << ((uint32_t)4U * (uint32_t)i);
        }
        else
        {
            rcmr |= (uint32_t)vlanPrio[0] << ((uint32_t)4U * (uint32_t)i);
        }
    }

    base->RCMR[index] = rcmr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigRxFlush
 * Description   : Enables/Disables receive flushing for a specified ring.
 *
 * Implements    : ENET_DRV_ConfigRxFlush_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigRxFlush(uint8_t instance,
                            uint8_t queue,
                            bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(queue < FEATURE_ENET_RING_COUNT);

    base = s_enetBases[instance];

    if (enable)
    {
        base->QOS |= (uint32_t)1U << (ENET_QOS_RX_FLUSH0_SHIFT + (uint32_t)queue);
    }
    else
    {
        base->QOS &= ~((uint32_t)1U << (ENET_QOS_RX_FLUSH0_SHIFT + (uint32_t)queue));
    }
}

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

#if FEATURE_ENET_HAS_INTCOAL

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_EnableRxInterruptCoalescing
 * Description   : Enables receive interrupts coalescing.
 *
 * Implements    : ENET_DRV_EnableRxInterruptCoalescing_Activity
 *END**************************************************************************/
void ENET_DRV_EnableRxInterruptCoalescing(uint8_t instance,
                                          uint8_t queue,
                                          uint8_t frameCount,
                                          uint16_t timeoutUs)
{
    ENET_Type *base;
    uint16_t timerThreshold;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
#ifdef ENET_RXIC_COUNT
    DEV_ASSERT(queue < ENET_RXIC_COUNT);
#else
    DEV_ASSERT(queue == 0U);
    (void) queue;
#endif /* ENET_RXIC_COUNT */

    timerThreshold = ENET_DRV_ComputeICTimerThreshold(instance, timeoutUs);

    base = s_enetBases[instance];

#ifdef ENET_RXIC_COUNT
    base->RXIC[queue] &= ~(ENET_RXIC_ICFT_MASK | ENET_RXIC_ICTT_MASK);
    base->RXIC[queue] |= ENET_RXIC_ICFT(frameCount) | ENET_RXIC_ICTT(timerThreshold) | ENET_RXIC_ICCS_MASK | ENET_RXIC_ICEN_MASK;
#else
    base->RXIC &= ~(ENET_RXIC_ICFT_MASK | ENET_RXIC_ICTT_MASK);
    base->RXIC |= ENET_RXIC_ICFT(frameCount) | ENET_RXIC_ICTT(timerThreshold) | ENET_RXIC_ICCS_MASK | ENET_RXIC_ICEN_MASK;
#endif /* ENET_RXIC_COUNT */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_DisableRxInterruptCoalescing
 * Description   : Disables receive interrupts coalescing.
 *
 * Implements    : ENET_DRV_DisableRxInterruptCoalescing_Activity
 *END**************************************************************************/
void ENET_DRV_DisableRxInterruptCoalescing(uint8_t instance,
                                           uint8_t queue)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
#ifdef ENET_RXIC_COUNT
    DEV_ASSERT(queue < ENET_RXIC_COUNT);
#else
    DEV_ASSERT(queue == 0U);
    (void) queue;
#endif /* ENET_RXIC_COUNT */

    base = s_enetBases[instance];

#ifdef ENET_RXIC_COUNT
    base->RXIC[queue] &= ~ENET_RXIC_ICEN_MASK;
#else
    base->RXIC &= ~ENET_RXIC_ICEN_MASK;
#endif /* ENET_RXIC_COUNT */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_EnableTxInterruptCoalescing
 * Description   : Enables transmit interrupts coalescing.
 *
 * Implements    : ENET_DRV_EnableTxInterruptCoalescing_Activity
 *END**************************************************************************/
void ENET_DRV_EnableTxInterruptCoalescing(uint8_t instance,
                                          uint8_t queue,
                                          uint8_t frameCount,
                                          uint16_t timeoutUs)
{
    ENET_Type *base;
    uint16_t timerThreshold;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
#ifdef ENET_TXIC_COUNT
    DEV_ASSERT(queue < ENET_TXIC_COUNT);
#else
    DEV_ASSERT(queue == 0U);
    (void) queue;
#endif /* ENET_TXIC_COUNT */

    timerThreshold = ENET_DRV_ComputeICTimerThreshold(instance, timeoutUs);

    base = s_enetBases[instance];

#ifdef ENET_TXIC_COUNT
    base->TXIC[queue] &= ~(ENET_TXIC_ICFT_MASK | ENET_TXIC_ICTT_MASK);
    base->TXIC[queue] |= ENET_TXIC_ICFT(frameCount) | ENET_TXIC_ICTT(timerThreshold) | ENET_TXIC_ICCS_MASK | ENET_TXIC_ICEN_MASK;
#else
    base->TXIC &= ~(ENET_TXIC_ICFT_MASK | ENET_TXIC_ICTT_MASK);
    base->TXIC |= ENET_TXIC_ICFT(frameCount) | ENET_TXIC_ICTT(timerThreshold) | ENET_TXIC_ICCS_MASK | ENET_TXIC_ICEN_MASK;
#endif /* ENET_TXIC_COUNT */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_DisableTxInterruptCoalescing
 * Description   : Disables transmit interrupts coalescing.
 *
 * Implements    : ENET_DRV_DisableTxInterruptCoalescing_Activity
 *END**************************************************************************/
void ENET_DRV_DisableTxInterruptCoalescing(uint8_t instance,
                                           uint8_t queue)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
#ifdef ENET_TXIC_COUNT
    DEV_ASSERT(queue < ENET_TXIC_COUNT);
#else
    DEV_ASSERT(queue == 0U);
    (void) queue;
#endif /* ENET_TXIC_COUNT */

    base = s_enetBases[instance];

#ifdef ENET_TXIC_COUNT
    base->TXIC[queue] &= ~ENET_TXIC_ICEN_MASK;
#else
    base->TXIC &= ~ENET_TXIC_ICEN_MASK;
#endif /* ENET_TXIC_COUNT */
}

#endif /* FEATURE_ENET_HAS_INTCOAL */


#if FEATURE_ENET_HAS_CONFIG_FIFO_THRESHOLDS

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetFIFOThreshold
 * Description   : Sets FIFO threshold levels.
 *
 * Implements    : ENET_DRV_SetFIFOThreshold_Activity
 *END**************************************************************************/
void ENET_DRV_SetFIFOThreshold(uint8_t instance,
                               enet_fifo_threshold_t thresholdType,
                               uint8_t thresholdValue)
{

    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    (&(base->RSFL))[(uint32_t)thresholdType] = thresholdValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetTxFIFOWatermark
 * Description   : Sets the transmit FIFO watermark.
 *
 * Implements    : ENET_DRV_SetTxFIFOWatermark_Activity
 *END**************************************************************************/
void ENET_DRV_SetTxFIFOWatermark(uint8_t instance,
                                 uint16_t watermark)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT((watermark & ENET_TX_WATERMARK_MASK) == 0U);

    base = s_enetBases[instance];

    base->TFWR = ENET_TFWR_TFWR((uint32_t)watermark >> ENET_TX_WATERMARK_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_EnableTxStoreAndForward
 * Description   : Enables the store and forward feature on the transmit path.
 *
 * Implements    : ENET_DRV_EnableTxStoreAndForward_Activity
 *END**************************************************************************/
void ENET_DRV_EnableTxStoreAndForward(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->TFWR |= ENET_TFWR_STRFWD_MASK;
}

#endif /* FEATURE_ENET_HAS_CONFIG_FIFO_THRESHOLDS */

#if FEATURE_ENET_HAS_RECEIVE_PARSER

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserInit
 * Description   : Configures and enables the receive parser.
 *
 * Implements    : ENET_DRV_RxParserInit_Activity
 *END**************************************************************************/
void ENET_DRV_RxParserInit(uint8_t instance,
                           enet_rx_parser_config_t *config)
{
    ENET_Type *base;
    uint32_t reg;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(config != NULL);

    base = s_enetBases[instance];

    /* Set maximum frame offset */
    base->MAXFRMOFF = ENET_MAXFRMOFF_MXFRMOFF_MASK;

    /* Configure and enable parser */
    reg = base->RXPCTL;

    reg &= ~(ENET_RXPCTL_ACPTEERR_MASK | ENET_RXPCTL_ENDERRQ_MASK | ENET_RXPCTL_PRSRSCLR_MASK |
             ENET_RXPCTL_INVBYTORD_MASK | ENET_RXPCTL_MAXINDEX_MASK);
    reg |= ENET_RXPCTL_ACPTEERR(config->acceptEndError ? 1UL : 0UL);
    reg |= ENET_RXPCTL_ENDERRQ(config->endErrorQueue);
    reg |= ENET_RXPCTL_INVBYTORD(config->inverseByteOrder ? 1UL : 0UL);
    reg |= ENET_RXPCTL_PRSRSCLR(config->clearCounters ? 1UL : 0UL);
    reg |= ENET_RXPCTL_ENPARSER_MASK;

    base->RXPCTL = reg;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserDeinit
 * Description   : Erases all the rules and disables the receive parser.
 *
 * Implements    : ENET_DRV_RxParserDeinit_Activity
 *END**************************************************************************/
void ENET_DRV_RxParserDeinit(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    base->RXPCTL &= ~(ENET_RXPCTL_MAXINDEX_MASK | ENET_RXPCTL_ENPARSER_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserAddAcceptRule
 * Description   : Appends an acceptance rule to the receive parser.
 *
 * Implements    : ENET_DRV_RxParserAddAcceptRule_Activity
 *END**************************************************************************/
void ENET_DRV_RxParserAddAcceptRule(uint8_t instance,
                                    uint8_t queue,
                                    enet_rx_parser_rule_t *rule)
{
    ENET_Type *base;
    uint32_t ruleIndex;
    uint32_t reg;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(rule != NULL);

    base = s_enetBases[instance];

    /* Update the number of valid entries */
    reg = base->RXPCTL;
    ruleIndex = ((reg & ENET_RXPCTL_MAXINDEX_MASK) >> ENET_RXPCTL_MAXINDEX_SHIFT);
    reg &= ~ENET_RXPCTL_MAXINDEX_MASK;
    reg |= ENET_RXPCTL_MAXINDEX(ruleIndex + 1U);
    base->RXPCTL = reg;

    /* Write the new entry */
    base->MDATA = rule->compareValue;
    base->MMASK = rule->compareMask;
    base->MCONFIG = ENET_MCONFIG_AF_MASK | ENET_MCONFIG_OK_INDEX(queue) | ENET_MCONFIG_FRMOFF(rule->compareOffset);
    base->MENTRYRW = ENET_MENTRYRW_ENTRYADD(ruleIndex) | ENET_MENTRYRW_WR_MASK;

    while ((base->MENTRYRW & ENET_MENTRYRW_WR_MASK) != 0U)
    {
        /* Wait until the write access is completed */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserAddRejectRule
 * Description   : Appends a rejection rule to the receive parser.
 *
 * Implements    : ENET_DRV_RxParserAddRejectRule_Activity
 *END**************************************************************************/
void ENET_DRV_RxParserAddRejectRule(uint8_t instance,
                                    enet_rx_parser_rule_t *rule)
{
    ENET_Type *base;
    uint32_t ruleIndex;
    uint32_t reg;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(rule != NULL);

    base = s_enetBases[instance];

    /* Update the number of valid entries */
    reg = base->RXPCTL;
    ruleIndex = ((reg & ENET_RXPCTL_MAXINDEX_MASK) >> ENET_RXPCTL_MAXINDEX_SHIFT);
    reg &= ~ENET_RXPCTL_MAXINDEX_MASK;
    reg |= ENET_RXPCTL_MAXINDEX(ruleIndex + 1U);
    base->RXPCTL = reg;

    /* Write the new entry */
    base->MDATA = rule->compareValue;
    base->MMASK = rule->compareMask;
    base->MCONFIG = ENET_MCONFIG_RF_MASK | ENET_MCONFIG_FRMOFF(rule->compareOffset);
    base->MENTRYRW = ENET_MENTRYRW_ENTRYADD(ruleIndex) | ENET_MENTRYRW_WR_MASK;

    while ((base->MENTRYRW & ENET_MENTRYRW_WR_MASK) != 0U)
    {
        /* Wait until the write access is completed */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserAddLinkingRule
 * Description   : Appends a linking rule to the receive parser.
 *
 * Implements    : ENET_DRV_RxParserAddLinkingRule_Activity
 *END**************************************************************************/
void ENET_DRV_RxParserAddLinkingRule(uint8_t instance,
                                     uint8_t nextEntry,
                                     enet_rx_parser_rule_t *rule)
{
    ENET_Type *base;
    uint32_t ruleIndex;
    uint32_t reg;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(rule != NULL);

    base = s_enetBases[instance];

    /* Update the number of valid entries */
    reg = base->RXPCTL;
    ruleIndex = ((reg & ENET_RXPCTL_MAXINDEX_MASK) >> ENET_RXPCTL_MAXINDEX_SHIFT);
    reg &= ~ENET_RXPCTL_MAXINDEX_MASK;
    reg |= ENET_RXPCTL_MAXINDEX(ruleIndex + 1U);
    base->RXPCTL = reg;

    /* Write the new entry */
    base->MDATA = rule->compareValue;
    base->MMASK = rule->compareMask;
    base->MCONFIG = ENET_MCONFIG_OK_INDEX(nextEntry) | ENET_MCONFIG_FRMOFF(rule->compareOffset);
    base->MENTRYRW = ENET_MENTRYRW_ENTRYADD(ruleIndex) | ENET_MENTRYRW_WR_MASK;

    while ((base->MENTRYRW & ENET_MENTRYRW_WR_MASK) != 0U)
    {
        /* Wait until the write access is completed */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserGetState
 * Description   : Returns the status of the receive parser.
 *
 * Implements    : ENET_DRV_RxParserGetState_Activity
 *END**************************************************************************/
uint32_t ENET_DRV_RxParserGetState(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    return base->RXPARST;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_RxParserGetCount
 * Description   : Returns the value of the requested receive parser counter.
 *
 * Implements    : ENET_DRV_RxParserGetCount_Activity
 *END**************************************************************************/
uint32_t ENET_DRV_RxParserGetCount(uint8_t instance,
                                   enet_rx_parser_counter_t counter)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    return (&(base->PARSDSCD))[(uint32_t)counter];
}

#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */

/*! @} */

/*******************************************************************************
 * EOF
 ******************************************************************************/
