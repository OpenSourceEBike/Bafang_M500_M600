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
 * @file enet_hw_access.c
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

#include "enet_hw_access.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Wrap-around value for timeout */
#define ENET_TIMEOUT_WRAP  0xFFFFFFFFU

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
 
 /*FUNCTION**********************************************************************
 *
 * Function Name : ENET_Timeout
 * Description   : Checks for timeout condition
 *
 *END**************************************************************************/
static bool ENET_Timeout(uint32_t startTime, uint32_t timeout)
{
    uint32_t currentTime;
    bool retVal;

    currentTime = OSIF_GetMilliseconds();
    if (currentTime >= startTime)
    {
        retVal = ((currentTime - startTime) > timeout) ? true : false;
    }
    else
    {
        /* wrap around */
        retVal = ((ENET_TIMEOUT_WRAP - startTime + currentTime) > timeout) ? true : false;
    }
    return retVal;
}

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigBufferDescriptors
 * Description   : Configures the receive and transmit buffer descriptors.
 *
 *END**************************************************************************/
void ENET_ConfigBufferDescriptors(ENET_Type * base,
                                  uint8_t ring,
                                  const enet_buffer_config_t *bufferConfig,
                                  uint16_t buffSize)
{
    /* Initializes transmit buffer descriptor rings start address, two start address should be aligned. */
    if (ring == 0U)
    {
        base->TDSR = (uint32_t)bufferConfig->txRingAligned;
        base->RDSR = (uint32_t)bufferConfig->rxRingAligned;
        base->MRBR = buffSize;
    }
#if (FEATURE_ENET_RING_COUNT > 1U)
    if (ring == 1U)
    {
        base->TDSR1 = (uint32_t)bufferConfig->txRingAligned;
        base->RDSR1 = (uint32_t)bufferConfig->rxRingAligned;
        base->MRBR1 = buffSize;
        base->DMACFG[0] = ENET_DMACFG_DMA_CLASS_EN_MASK;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
#if (FEATURE_ENET_RING_COUNT > 2U)
    if (ring == 2U)
    {
        base->TDSR2 = (uint32_t)bufferConfig->txRingAligned;
        base->RDSR2 = (uint32_t)bufferConfig->rxRingAligned;
        base->MRBR2 = buffSize;
        base->DMACFG[1] = ENET_DMACFG_DMA_CLASS_EN_MASK;
    }
#endif /* (FEATURE_ENET_RING_COUNT > 2U) */
}

#if FEATURE_ENET_HAS_RX_CONFIG

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigReceiveControl
 * Description   : Configures the receive block.
 *
 *END**************************************************************************/
void ENET_ConfigReceiveControl(ENET_Type *base,
                               const enet_config_t *config)
{
    uint32_t rcr = 0;

    /* Configures MAC receive controller with user configure structure. */
    rcr = ENET_RCR_NLC(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_PAYLOAD_LEN_CHECK) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_CFEN(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_FLOW_CONTROL) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_CRCFWD(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_STRIP_CRC_FIELD) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_FCE(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_FLOW_CONTROL) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PAUFWD(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PADEN(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_REMOVE_PADDING) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_BC_REJ(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_REJECT_BROADCAST_FRAMES) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PROM(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_PROMISCUOUS_MODE) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_MII_MODE(1U) |
#if FEATURE_ENET_HAS_RGMII
          ENET_RCR_RGMII_EN((config->miiMode == ENET_RGMII_MODE) ? 1UL : 0UL) |
#endif /* FEATURE_ENET_HAS_RGMII */
          ENET_RCR_RMII_MODE((config->miiMode == ENET_RMII_MODE) ? 1UL : 0UL) |
          ENET_RCR_RMII_10T((config->miiSpeed == ENET_MII_SPEED_10M) ? 1UL : 0UL) |
          ENET_RCR_MAX_FL(config->maxFrameLen);

    /* Receive setting for half duplex. */
    if (config->miiDuplex == ENET_MII_HALF_DUPLEX)
    {
        rcr |= ENET_RCR_DRT(1);
    }
    /* Sets internal loopback. */
    if ((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_MII_LOOPBACK) != 0U)
    {
        rcr |= ENET_RCR_LOOP(1);
        rcr &= ~ENET_RCR_DRT_MASK;
    }
    base->RCR = rcr;

#if FEATURE_ENET_HAS_SPEED_1000M
    if (config->miiSpeed == ENET_MII_SPEED_1000M)
    {
        base->ECR |= ENET_ECR_SPEED(1U);
    }
#endif /* FEATURE_ENET_HAS_SPEED_1000M */
}

#endif /* FEATURE_ENET_HAS_RX_CONFIG */

#if FEATURE_ENET_HAS_TX_CONFIG

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigTransmitControl
 * Description   : Configures the transmit block.
 *
 *END**************************************************************************/
void ENET_ConfigTransmitControl(ENET_Type *base,
                                const enet_config_t *config)
{
    uint32_t tcr = base->TCR;

    /* Configures MAC transmit controller: duplex mode, mac address insertion. */
    tcr &= ~(ENET_TCR_FDEN_MASK | ENET_TCR_CRCFWD_MASK | ENET_TCR_ADDINS_MASK);
    tcr |= ENET_TCR_FDEN(config->miiDuplex) |
           ENET_TCR_CRCFWD(((config->txConfig & (uint32_t)ENET_TX_CONFIG_DISABLE_CRC_APPEND) != 0UL) ? 1UL : 0UL) |
           ENET_TCR_ADDINS(((config->txConfig & (uint32_t)ENET_TX_CONFIG_ENABLE_MAC_ADDR_INSERTION) != 0UL) ? 1UL : 0UL);

    base->TCR = tcr;

#if (FEATURE_ENET_RING_COUNT > 1U)
    /* By default, use round robin arbitration scheme */
       base->QOS = ENET_QOS_TX_SCHEME(1U);
#endif /* (FEATURE_ENET_RING_COUNT > 1U) */
}

#endif /* FEATURE_ENET_HAS_TX_CONFIG */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_WriteManagementConfig
 * Description   : Configures the MII management interface.
 *
 *END**************************************************************************/
void ENET_WriteManagementConfig(ENET_Type * base,
                                uint32_t miiSpeed,
                                uint32_t holdTime,
                                bool miiPreambleDisabled)
{
	uint32_t config;
	
	config = ENET_MSCR_MII_SPEED(miiSpeed) | ENET_MSCR_DIS_PRE(miiPreambleDisabled ? 1UL : 0UL) | ENET_MSCR_HOLDTIME(holdTime);
	base->MSCR = config;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_WriteManagementFrame
 * Description   : Writes a MII management frame.
 *
 *END**************************************************************************/
status_t ENET_WriteManagementFrame(ENET_Type * base,
                                   uint8_t phyAddr,
                                   uint8_t phyReg,
                                   enet_mmfr_op_type_t opType,
                                   uint16_t data,
                                   bool clause45,
                                   uint32_t startTime,
                                   uint32_t timeoutMs)
{
    status_t status = STATUS_TIMEOUT;

    base->MMFR = (clause45 ? ENET_MMFR_ST(ENET_MMFR_START_INDICATION_45) : ENET_MMFR_ST(ENET_MMFR_START_INDICATION)) |
                              ENET_MMFR_OP(opType) |
                              ENET_MMFR_PA(phyAddr) |
                              ENET_MMFR_RA(phyReg) |
                              ENET_MMFR_TA(ENET_MMFR_TURN_AROUND) |
                              ENET_MMFR_DATA(data);

    /* Wait for completion */
    do
    {
        if ((ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK) != 0U)
        {
            status = STATUS_SUCCESS;
            /* Clear the flag */
            ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);
            break;
        }
    }
    while (!ENET_Timeout(startTime, timeoutMs));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigCounters
 * Description   : Enables/Disables the MIB counters
 *
 *END**************************************************************************/
void ENET_ConfigCounters(uint8_t instance,
                         bool enable)
 {
    ENET_Type *base = s_enetBases[instance];

    if (enable)
    {
        /* Clear counters - MIB_CLEAR bit needs to be set, then cleared */
        base->MIBC |= ENET_MIBC_MIB_CLEAR_MASK;
        (void) base->MIBC;
        base->MIBC &= ~ENET_MIBC_MIB_CLEAR_MASK;

        base->MIBC &= ~ENET_MIBC_MIB_DIS_MASK;
    }
    else
    {
        base->MIBC |= ENET_MIBC_MIB_DIS_MASK;
    }
 }
 
/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_SetSpeed
 * Description   : Sets the speed of the MII interface
 *
 *END**************************************************************************/
void ENET_SetSpeed(ENET_Type * base,
                   enet_mii_speed_t speed)
{
    if (speed == ENET_MII_SPEED_10M)
    {
    	base->RCR |= ENET_RCR_RMII_10T_MASK;
    }
	else
	{
		base->RCR &= ~ENET_RCR_RMII_10T_MASK;
	}
#if FEATURE_ENET_HAS_SPEED_1000M
    if (speed == ENET_MII_SPEED_1000M)
    {
    	base->ECR |= ENET_ECR_SPEED_MASK;
    }
	else
    {
    	base->ECR &= ~ENET_ECR_SPEED_MASK;
    }
#endif /* FEATURE_ENET_HAS_SPEED_1000M */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_TransmitIRQHandler
 * Description   : Handler for ENET transmission interrupts.
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_TransmitIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the transmit interrupt happen. */
    if ((((uint32_t)ENET_TX_BUFFER_INTERRUPT | (uint32_t)ENET_TX_FRAME_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_TX_EVENT, 0U);
        }
        /* Clear the transmit interrupt event. */
        base->EIR = (uint32_t)ENET_TX_FRAME_INTERRUPT | (uint32_t)ENET_TX_BUFFER_INTERRUPT;

#ifdef ERRATA_E6358
        if (base->TDAR == 0U)
        {
            const enet_buffer_descriptor_t *bd;
            bool inUse = false;
            bool finished = false;

            bd = g_enetState[instance]->txBdBase[0U];

            /* Go through all buffer descriptors */
            while (!finished)
            {
                /* Check if the buffer descriptor is still in use. */
                if ((bd->control & ENET_BUFFDESCR_TX_READY_MASK) != 0U)
                {
                    inUse = true;
                    break;
                }

                /* This was the last buffer in the ring. */
                if ((bd->control & ENET_BUFFDESCR_TX_WRAP_MASK) != 0U)
                {
                    finished = true;
                }

                bd++;
            }

            if (inUse)
            {
                /* Set TDAR bit, as packets are still waiting for transmission */
                base->TDAR = ENET_TDAR_TDAR_MASK;
            }
        }
#endif /* ERRATA_E6358 */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ReceiveIRQHandler
 * Description   : Handler for ENET reception interrupts.
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 *END**************************************************************************/
void ENET_ReceiveIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the receive interrupt happen. */
    if ((((uint32_t)ENET_RX_BUFFER_INTERRUPT | (uint32_t)ENET_RX_FRAME_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_RX_EVENT, 0U);
        }
        /* Clear the receive interrupt event. */
        base->EIR = (uint32_t)ENET_RX_FRAME_INTERRUPT | (uint32_t)ENET_RX_BUFFER_INTERRUPT;
    }
}

#if (FEATURE_ENET_RING_COUNT > 1U)

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_Transmit1IRQHandler
 * Description   : Handler for ENET transmission interrupts for ring 1.
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_Transmit1IRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the transmit interrupt happen. */
    if ((((uint32_t)ENET_TX_BUFFER_1_INTERRUPT | (uint32_t)ENET_TX_FRAME_1_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_TX_EVENT, 1U);
        }
        /* Clear the transmit interrupt event. */
        base->EIR = (uint32_t)ENET_TX_FRAME_1_INTERRUPT | (uint32_t)ENET_TX_BUFFER_1_INTERRUPT;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_Receive1IRQHandler
 * Description   : Handler for ENET reception interrupts for ring 1.
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 *END**************************************************************************/
void ENET_Receive1IRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the receive interrupt happen. */
    if ((((uint32_t)ENET_RX_BUFFER_1_INTERRUPT | (uint32_t)ENET_RX_FRAME_1_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_RX_EVENT, 1U);
        }

        /* Clear the receive interrupt event. */
        base->EIR = (uint32_t)ENET_RX_FRAME_1_INTERRUPT | (uint32_t)ENET_RX_BUFFER_1_INTERRUPT;
    }
}

#endif /* (FEATURE_ENET_RING_COUNT > 1U) */

#if (FEATURE_ENET_RING_COUNT > 2U)

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_Transmit2IRQHandler
 * Description   : Handler for ENET transmission interrupts for ring 2.
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_Transmit2IRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the transmit interrupt happen. */
    if ((((uint32_t)ENET_TX_BUFFER_2_INTERRUPT | (uint32_t)ENET_TX_FRAME_2_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_TX_EVENT, 2U);
        }

        /* Clear the transmit interrupt event. */
        base->EIR = (uint32_t)ENET_TX_FRAME_2_INTERRUPT | (uint32_t)ENET_TX_BUFFER_2_INTERRUPT;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_Receive2IRQHandler
 * Description   : Handler for ENET reception interrupts for ring 2.
 * This handler extracts the received frame and invokes the installed callback,
 * if available. After the callback completes, the buffers will be marked as
 * empty and the interrupt flags will be cleared.
 *
 *END**************************************************************************/
void ENET_Receive2IRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the receive interrupt happen. */
    if ((((uint32_t)ENET_RX_BUFFER_2_INTERRUPT | (uint32_t)ENET_RX_FRAME_2_INTERRUPT) & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_RX_EVENT, 2U);
        }
        /* Clear the receive interrupt event. */
        base->EIR = (uint32_t)ENET_RX_FRAME_2_INTERRUPT | (uint32_t)ENET_RX_BUFFER_2_INTERRUPT;
    }
}

#endif /* (FEATURE_ENET_RING_COUNT > 2U) */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ErrorIRQHandler
 * Description   : Handler for ENET error interrupts.
 * This handler invokes the installed callback, if available, and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_ErrorIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    uint32_t errMask = (uint32_t)ENET_BABR_INTERRUPT | (uint32_t)ENET_BABT_INTERRUPT | (uint32_t)ENET_EBERR_INTERRUPT |
                       (uint32_t)ENET_PAYLOAD_RX_INTERRUPT | (uint32_t)ENET_LATE_COLLISION_INTERRUPT |
                       (uint32_t)ENET_RETRY_LIMIT_INTERRUPT | (uint32_t)ENET_UNDERRUN_INTERRUPT;

    if ((errMask & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_ERR_EVENT, 0);
        }
        /* Clear the error interrupt event. */
        errMask &= base->EIR;
        base->EIR = errMask;
    }
}

#if FEATURE_ENET_HAS_SLEEP_WAKEUP

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_WakeIRQHandler
 * Description   : Handler for ENET wakeup interrupts.
 * This handler invokes the installed callback, if available, and clears
 * the interrupt flags. Also, the ENET module is set to normal operation mode.
 *
 *END**************************************************************************/
void ENET_WakeIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    if (((uint32_t)ENET_WAKEUP_INTERRUPT & base->EIR) != 0U)
    {
        ENET_DRV_SetSleepMode(instance, false);
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_WAKE_UP_EVENT, 0);
        }
        /* Clear the wakeup interrupt event. */
        base->EIR = (uint32_t)ENET_WAKEUP_INTERRUPT;
    }
}

#endif /* FEATURE_ENET_HAS_SLEEP_WAKEUP */

#if FEATURE_ENET_HAS_ADJUSTABLE_TIMER

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_TimerIRQHandler
 * Description   : Handler for ENET timer interrupts.
 * This handler invokes the installed callback (if available) and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_TimerIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    if ((base->EIR & ENET_EIR_TS_TIMER_MASK) != 0U)
    {
        if (g_enetState[instance]->timerCallback != NULL)
        {
            g_enetState[instance]->timerCallback(instance, (uint8_t)base->TGSR);
        }

        /* Clear the timer interrupt event. */
        base->EIR = (uint32_t)ENET_EIR_TS_TIMER_MASK;
    }
}

#endif /* FEATURE_ENET_HAS_ADJUSTABLE_TIMER */

#if FEATURE_ENET_HAS_RECEIVE_PARSER

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ParserIRQHandler
 * Description   : Handler for ENET receive parser.
 * This handler invokes the installed callback (if available) and clears the
 * interrupt flags.
 *
 *END**************************************************************************/
void ENET_ParserIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    uint32_t parserMask = (uint32_t)ENET_PARSERERR_INTERRUPT | (uint32_t)ENET_PARSRF_INTERRUPT;

    if ((parserMask & base->EIR) != 0U)
    {
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_PARSER_EVENT, 0);
        }
        /* Clear the parser interrupt event. */
        parserMask &= base->EIR;
        base->EIR = parserMask;
    }
}

#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */

/*******************************************************************************
 * EOF
 ******************************************************************************/
