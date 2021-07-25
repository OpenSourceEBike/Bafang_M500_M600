/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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

#ifndef ERM_HW_ACCESS_H
#define ERM_HW_ACCESS_H

/*! @file erm_hw_access.h */

#include "erm_driver.h"

/*!
 * erm_hw_access ERM Hardware Access
 * @details This section describes the programming interface of the ERM Hardware Access.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The distance between channels */
#define ERM_CHANNELS_OFFSET_SIZE (4U)
/*! @brief Start mask of non correctable error */
#define ERM_NCE_START_MASK       ERM_CR0_ENCIE0_MASK
/*! @brief Start mask of single bit correction */
#define ERM_SBC_START_MASK       ERM_CR0_ESCIE0_MASK

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name ERM HAL API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ERM module
 *
 * This function initializes the module to default configuration,
 * the configuration register is initialized with interrupt notification disabled
 * for all channels and the status register events are cleared
 *
 * @param[in] base The ERM peripheral base address
 */
void ERM_Init(ERM_Type * const base);

/*!
 * @brief Enables Memory n interrupt event
 *
 * This function enables Memory n interrupt event
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The configured memory channel
 * @param[in] eccEvent The configured event
 * @param[in] enable Enable interrupt events
 *            - true: Interrupt event is enabled
 *            - false: Interrupt event is disabled
 */
static inline void ERM_EnableEventInterrupt(ERM_Type * const base,
                                            uint8_t channel,
                                            erm_ecc_event_t eccEvent,
                                            bool enable)
{
    uint32_t tempCtrl = base->CR0;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        if (enable)
        {
            tempCtrl |= ERM_SBC_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE);
        }
        else
        {
            tempCtrl &= ~(ERM_SBC_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
        }
    }
    /* Non-correctable */
    else if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
    {
        if (enable)
        {
            tempCtrl |= ERM_NCE_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE);
        }
        else
        {
            tempCtrl &= ~(ERM_NCE_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
        }
    }
    else
    {
        /* Do nothing */
    }

    /* Write to register */
    base->CR0 = tempCtrl;
}

/*!
 * @brief Checks if the Memory n interrupt event is enabled
 *
 * This function checks if the Memory n interrupt event is enabled
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @param[in] eccEvent The examined event
 * @return Interrupt event
 *         - true: Interrupt event is enabled
 *         - false: Interrupt event is disabled
 */
static inline bool ERM_IsEventInterruptEnabled(const ERM_Type * const base,
                                               uint8_t channel,
                                               erm_ecc_event_t eccEvent)
{
    uint32_t retVal = 0U;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        retVal = base->CR0 & (ERM_SBC_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    /* Non-correctable */
    else if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
    {
        retVal = base->CR0 & (ERM_NCE_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    else
    {
        /* Do nothing */
    }

    return retVal != 0U;
}

/*!
 * @brief Checks if the Memory n error event is detected
 *
 * This function checks if the Memory n error event is detected
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @param[in] eccEvent The examined event
 * @return The status of Memory n error event
 *         - true: Error event on Memory n detected
 *         - false: No error event on Memory n detected
 */
static inline bool ERM_IsEventDetected(const ERM_Type * const base,
                                       uint8_t channel,
                                       erm_ecc_event_t eccEvent)
{
    uint32_t retVal = 0U;

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        retVal = base->SR0 & (ERM_SBC_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    /* Non-correctable */
    else if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
    {
        retVal = base->SR0 & (ERM_NCE_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE));
    }
    else
    {
        /* Do nothing */
    }

    return retVal != 0U;
}

/*!
 * @brief Clears error event single and the corresponding interrupt notification
 *
 * This function clears error event single and the corresponding interrupt notification
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The configured memory channel
 */
static inline void ERM_ClearEventSingle(ERM_Type * const base,
                                        uint8_t channel)
{
    base->SR0 = ERM_SBC_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE);

#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    (void)base->SR0;
#endif
}

/*!
 * @brief Clears error event double and the corresponding interrupt notification
 *
 * This function clears error event double and the corresponding interrupt notification
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The configured memory channel
 */
static inline void ERM_ClearEventDouble(ERM_Type * const base,
                                        uint8_t channel)
{
    base->SR0 = ERM_NCE_START_MASK >> (channel * ERM_CHANNELS_OFFSET_SIZE);

#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    (void)base->SR0;
#endif
}

/*!
 * @brief Gets the address of the last ECC event in Memory n
 *
 * This function gets the address of the last ECC event in Memory n
 *
 * @param[in] base The ERM peripheral base address
 * @param[in] channel The examined memory channel
 * @return Address of the last ECC event
 */
    static inline uint32_t ERM_GetLastErrorAddress(const ERM_Type * const base,
                                                   uint8_t channel)
    {
        #if(ERM_EARn_COUNT >= 2u)
            return base->EARn[channel].EAR;
        #else
            return base->EARn[channel];
        #endif
    }


/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* ERM_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
