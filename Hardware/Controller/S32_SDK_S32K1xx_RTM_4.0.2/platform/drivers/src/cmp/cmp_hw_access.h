/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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
/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially
* unsigned' to 'essentially Boolean'.
* This is required by the conversion of a bit into a bool.
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially
* unsigned' to 'essentially enum<i>'.
* This is required by the conversion of a bitfiel of a register into a enum.
*
* @section [global]
* Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
*(different essential type categories).
* This is required by the conversion of a bit/bitfield of a register into boolean or a enum type.
*
* @section [global]
* Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower
* or different essential type [MISRA 2012 Rule 10.3, required]
* This is required by the conversion of a unsigned value of a bitfield/bit into a enum value.
*/

#ifndef CMP_H
#define CMP_H

#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"
#include "cmp_driver.h"

/*!
 * @addtogroup cmp_hal Comparator HAL
 * @ingroup cmp
 * @brief Comparator Hardware Abstraction Layer
 * @{
 */


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name CMP_HAL.
 * @{
 */

 #if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Gets the comparator functional mode. If you want to get filter count and filter period please use CMP_GetFilterSamplePeriod
 * and CMP_GetSamplingState.
 * @param baseAddr - cmp base pointer
 * @return - functional mode
 *   CMP_DISABLED
 *   CMP_CONTINUOUS
 *   CMP_SAMPLED_NONFILTRED_INT_CLK
 *   CMP_SAMPLED_NONFILTRED_EXT_CLK
 *   CMP_SAMPLED_FILTRED_INT_CLK
 *   CMP_SAMPLED_FILTRED_EXT_CLK
 *   CMP_WINDOWED
 *   CMP_WINDOWED_RESAMPLED
 *   CMP_WINDOWED_FILTRED
 */
cmp_mode_t CMP_GetFunctionalMode(const CMP_Type* baseAddr);

/*!
 * @brief Sets the comparator functional mode (mode, filter count, filter period)
 * @param baseAddr - cmp base pointer
 * @param mode - functional mode
 *  CMP_DISABLED
 *  CMP_CONTINUOUS
 *  CMP_SAMPLED_NONFILTRED_INT_CLK
 *  CMP_SAMPLED_NONFILTRED_EXT_CLK
 *  CMP_SAMPLED_FILTRED_INT_CLK
 *  CMP_SAMPLED_FILTRED_EXT_CLK
 *  CMP_WINDOWED
 *  CMP_WINDOWED_RESAMPLED
 *  CMP_WINDOWED_FILTRED
 * @param filter_sample_count - number of consecutive samples that must agree prior to the comparator ouput filter
 * accepting a new output state
 * @param filter_sample_period - sampling period
 * @return -void
 */
void CMP_SetFunctionalMode(CMP_Type* baseAddr, cmp_mode_t mode, uint8_t filter_sample_count, uint8_t filter_sample_period);

/*!
 * @brief Verify if the DMA transfer trigger is enabled
 * @param baseAddr - cmp base pointer
 * @return - DMA transfer trigger state
 *  true - DMA trigger is enabled
 *  false - DAM trigger is disabled
 */
static inline bool CMP_GetDMATriggerState(const CMP_Type* baseAddr)
{
    return (bool)(((baseAddr->C0) >> (CMP_C0_DMAEN_SHIFT)) & 1U);
}

/*!
 * @brief Configure the DMA transfer trigger
 * @param baseAddr - cmp base pointer
 * @param to_set - DMA transfer trigger state
 *  true - DMA trigger is enabled
 *  false - DAM trigger is disabled
 * @return - void
 */
static inline void CMP_SetDMATriggerState(CMP_Type* baseAddr, bool to_set)
{
    baseAddr->C0 = (baseAddr->C0 & (~(CMP_C0_DMAEN_MASK))) | ((uint32_t)to_set << CMP_C0_DMAEN_SHIFT);
}

/*!
 * @brief Return the comparator output interrupts source configuration(none, rising edge, falling edge or both edges)
 * @param baseAddr - cmp base pointer
 * @return - comparator output interrupts configuration
 *  CMP_NO_EVENT
 *  CMP_FALLING_EDGE
 *  CMP_RISING_EDGE
 *  CMP_BOTH_EDGES
 */
static inline cmp_output_trigger_t CMP_GetOutputInterruptTrigger(const CMP_Type* baseAddr)
{
    bool rising_enabled = (bool)(((baseAddr->C0) >> CMP_C0_IER_SHIFT) & 1U);
    bool falling_enabled = (bool)(((baseAddr->C0) >> CMP_C0_IEF_SHIFT) & 1U);
    return (cmp_output_trigger_t) (((uint32_t)rising_enabled << (uint32_t)1U) | ((uint32_t)falling_enabled));
}

/*!
 * @brief Set the comparator output interrupts source configuration(none, rising edge, falling edge or both edges)
 * @param baseAddr - cmp base pointer
 * @param to_set - comparator output interrupts configuration
 *  CMP_NO_EVENT
 *  CMP_FALLING_EDGE
 *  CMP_RISING_EDGE
 *  CMP_BOTH_EDGES
 * @return - void
 */
static inline void CMP_SetOutputInterruptTrigger(CMP_Type* baseAddr, cmp_output_trigger_t to_set)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_IER_MASK) & ~(CMP_C0_IEF_MASK);
    tmp |= CMP_C0_IER((uint32_t)to_set>>1) | CMP_C0_IEF((uint32_t)to_set & (uint32_t)0x01);
    baseAddr->C0 = tmp;
}

/*!
 * @brief Return the sample period for filter(clock cycles)
 * @param baseAddr - cmp base pointer
 * @return - sampling period(in bus cycles)
 */
static inline uint8_t CMP_GetFilterSamplePeriod(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp = (tmp & CMP_C0_FPR_MASK) >> CMP_C0_FPR_SHIFT;
    return ( uint8_t ) (tmp);
}

/*!
 * @brief Set the filter sample period(clock cycles)
 * @param baseAddr -cmp base pointer
 * @param to_set - number of bus cycles
 * @return - void
 */
static inline void CMP_SetFilterSamplePeriod(CMP_Type* baseAddr, uint8_t to_set)
{
    uint32_t tmp = baseAddr->C0;
    baseAddr->C0 &= ~(CMP_C0_FPR_MASK);
    tmp &= ~(CMP_C0_FPR_MASK);
    tmp |= CMP_C0_FPR(to_set);
    baseAddr->C0 = tmp;
}

/*!
 * @brief Return the current power mode
 * @param baseAddr - cmp base pointer
 * @return - current power mode
 *  CMP_LOW_SPEED
 *  CMP_HIGH_SPEED
 */
static inline cmp_power_mode_t CMP_GetPowerMode(const CMP_Type* baseAddr)
{
    return (cmp_power_mode_t)(((baseAddr->C0) >> CMP_C0_PMODE_SHIFT) & 1U);
}

/*!
 * @brief Set the power mode
 * @param baseAddr - cmp base pointer
 * @param to_set - power mode
 *  CMP_LOW_SPEED
 *  CMP_HIGH_SPEED
 */
static inline void CMP_SetPowerMode(CMP_Type* baseAddr, cmp_power_mode_t to_set)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_PMODE_MASK))) | ((uint32_t)to_set << CMP_C0_PMODE_SHIFT);
}

/*!
 * @brief Return the current comparator output inverter
 * @param baseAddr cmp base pointer
 * @return - inverter state
 *  CMP_NORMAL
 *  CMP_INVERT
 */
static inline cmp_inverter_t CMP_GetInverterState(const CMP_Type* baseAddr)
{
    return (cmp_inverter_t)(((baseAddr->C0) >> CMP_C0_INVT_SHIFT) & 1U);
}

/*!
 * @brief Configure the comparator output inverter mode
 * @param baseAddr - cmp base pointer
 * @param to_set - comparator output inverter mode
 *  CMP_NORMAL
 *  CMP_INVERT
 * @return - void
 */
static inline void CMP_SetInverterState(CMP_Type* baseAddr, cmp_inverter_t to_set)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_INVT_MASK))) | ((uint32_t)to_set << CMP_C0_INVT_SHIFT);
}

/*!
 * @brief Return the current comparator output selected
 * @param baseAddr - cmp base pointer
 * @return - comparator output signal source
 *  CMP_COUT
 *  CMP_COUTA
 */
static inline cmp_output_select_t CMP_GetComparatorOutputSource(const CMP_Type* baseAddr)
{
    return (cmp_output_select_t)(((baseAddr->C0) >> CMP_C0_COS_SHIFT) & 1U);
}

/*!
 * @brief Select the comparator output signal source
 * @param baseAddr - cmp base pointer
 * @param to_set - comparator output signal source
 *  CMP_COUT
 *  CMP_COUTA
 * @return vid
 */
static inline void CMP_SetComparatorOutputSource(CMP_Type* baseAddr, cmp_output_select_t to_set)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_COS_MASK))) | ((uint32_t)to_set << CMP_C0_COS_SHIFT);
}

/*!
 * @brief Verify if the comparator output state(available/not available in a packaged pin)
 * @param baseAddr - cmp base pointer
 * @return - comparator output state
 *  CMP_UNAVAILABLE
 *  CMP_AVAILABLE
 */
static inline cmp_output_enable_t CMP_GetOutputPinState(const CMP_Type* baseAddr)
{
    return (cmp_output_enable_t)(((baseAddr->C0) >> CMP_C0_OPE_SHIFT) & 1U);
}

/*!
 * @brief Set the comparator output pin state(available/not available in a packaged pin)
 * @param baseAddr - cmp base pointer
 * @param to_set - comparator output state
 *  CMP_UNAVAILABLE
 *  CMP_AVAILABLE
 * @return - void
 */
static inline void CMP_SetOutputPinState(CMP_Type* baseAddr, cmp_output_enable_t to_set)
{

    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_OPE_MASK))) | ((uint32_t)to_set << CMP_C0_OPE_SHIFT);
}

/*!
 * @brief Return the number of consecutive samples that must agree prior to the comparator output filter
accepting a new output state
 * @param baseAddr - cmp base pointer
 * @return - filter sample count
 */
static inline uint8_t CMP_GetFilterSampleCount(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp = (tmp & CMP_C0_FILTER_CNT_MASK) >> CMP_C0_FILTER_CNT_SHIFT;
    return ( uint8_t ) (tmp);
}

/*!
 * @brief Set the number of consecutive samples that must agree prior to the comparator output filter
accepting a new output state
 * @param baseAddr - cmp base pointer
 * @param to_set - filter sample count(min value 0, max value 7)
 * @return - void
 */
static inline void CMP_SetFilterSampleCount(CMP_Type* baseAddr, uint8_t to_set)
{
    uint32_t tmp = baseAddr->C0;
    baseAddr->C0 &= ~(CMP_C0_FILTER_CNT_MASK);
    tmp &= ~(CMP_C0_FILTER_CNT_MASK);
    tmp |= CMP_C0_FILTER_CNT(to_set);
    baseAddr->C0 = tmp;
}

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET

/*!
 * @brief Return the current offset level
 * @param baseAddr - cmp base pointer
 * @return - offset level
 *  CMP_LEVEL_OFFSET_0
 *  CMP_LEVEL_OFFSET_1
 */
static inline cmp_offset_t CMP_GetOffset(const CMP_Type* baseAddr)
{
    return (cmp_offset_t)(((baseAddr->C0) >> CMP_C0_OFFSET_SHIFT) & 1U);
}

/*!
 * @brief Set the offset level
 * @param baseAddr - cmp base pointer
 * @param to_set - offset level
 *  CMP_LEVEL_OFFSET_0
 *  CMP_LEVEL_OFFSET_1
 * @return - void
 */
static inline void CMP_SetOffset(CMP_Type* baseAddr, cmp_offset_t to_set)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_OFFSET_MASK);
    tmp |= CMP_C0_OFFSET(to_set);
    baseAddr->C0 = tmp;
}

#endif

/*!
 * @brief Return the current hysteresis level
 * @param baseAddr - cmp base pointer
 * @return - current hysteresis level
 *  CMP_LEVEL_HYS_0
 *  CMP_LEVEL_HYS_1
 *  CMP_LEVEL_HYS_2
 *  CMP_LEVEL_HYS_3
 */
static inline cmp_hysteresis_t CMP_GetHysteresis(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp = (tmp & CMP_C0_HYSTCTR_MASK) >> CMP_C0_HYSTCTR_SHIFT;
    return ( cmp_hysteresis_t ) (tmp);
}

/*!
 * @brief Set the hysteresis level
 * @param baseAddr - cmp base pointer
 * @param to_set - hysteresis level
 *  CMP_LEVEL_HYS_0
 *  CMP_LEVEL_HYS_1
 *  CMP_LEVEL_HYS_2
 *  CMP_LEVEL_HYS_3
 * @return - void
 */
static inline void CMP_SetHysteresis(CMP_Type* baseAddr, cmp_hysteresis_t to_set)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_HYSTCTR_MASK);
    tmp |= CMP_C0_HYSTCTR(to_set);
    baseAddr->C0 = tmp;
}

/*!
 * @brief Return the current source for positive port of the comparator
 * @param baseAddr - cmp base pointer
 * @return - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline cmp_port_mux_t CMP_GetPositivePortInput(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp = (tmp & CMP_C1_INPSEL_MASK) >> CMP_C1_INPSEL_SHIFT;
    return ( cmp_port_mux_t ) (tmp);
}

/*!
 * @brief Set the source for positive port of the comparator
 * @param baseAddr cmp base pointer
 * @param to_set - signal source
 *  CMP_DAC
 *  CMP_MUX
 * @return - void
 */
static inline void CMP_SetPositivePortInput(CMP_Type* baseAddr, cmp_port_mux_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_INPSEL_MASK);
    tmp |= CMP_C1_INPSEL(to_set);
    baseAddr->C1 = tmp;
}

/*!
 * @brief Return the current source for negative port of the comparator
 * @param baseAddr - cmp base pointer
 * @return - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline cmp_port_mux_t CMP_GetNegativePortInput(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp = (tmp & CMP_C1_INNSEL_MASK) >> CMP_C1_INNSEL_SHIFT;
    return ( cmp_port_mux_t ) (tmp);
}

/*!
 * @brief Set the source for negative port of the comparator
 * @param baseAddr - cmp base pointer
 * @param to_set - signal source
 *  CMP_DAC
 *  CMP_MUX
 * @return - void
 */
static inline void CMP_SetNegativePortInput(CMP_Type* baseAddr, cmp_port_mux_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_INNSEL_MASK);
    tmp |= CMP_C1_INNSEL(to_set);
    baseAddr->C1 = tmp;
}

/*!
 * @brief Return which channels are used for round-robin checker
 * @param baseAddr - cmp base pointer
 * @return - channels states, one bite for each channel state
 * |---------|---------|-----|---------|---------|
 * |CH7_state|CH6_state|.....|CH1_state|CH0_state|
 * |---------|---------|-----|---------|---------|
 */
static inline cmp_ch_list_t CMP_GetRoundRobinChannels(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp =  tmp & (uint32_t)CMP_ROUND_ROBIN_CHANNELS_MASK;
    return(cmp_ch_list_t) (tmp >> CMP_ROUND_ROBIN_CHANNELS_SHIFT);
}

/*!
 * @brief Set which channels are use for round-robin checker
 * @param baseAddr - cmp base pointer
 * @param to_set - channels states, one bite for each channel state
 * |---------|---------|-----|---------|---------|
 * |CH7_state|CH6_state|.....|CH1_state|CH0_state|
 * |---------|---------|-----|---------|---------|
 * @return - void
 */
static inline void CMP_SetRoundRobinChannels(CMP_Type* baseAddr, cmp_ch_list_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(uint32_t)(CMP_ROUND_ROBIN_CHANNELS_MASK);
    tmp |= (uint32_t)to_set << (uint32_t)CMP_ROUND_ROBIN_CHANNELS_SHIFT;
    baseAddr->C1 = tmp;
}

/*!
 * @brief Verify if the DAC is enabled
 * @param baseAddr - cmp base pointer
 * @return - dac state
 *  true - DAC is enabled
 *  false - DAC is disabled
 */
static inline bool CMP_GetDACState(const CMP_Type* baseAddr)
{
    return (bool)(((baseAddr->C1) >> CMP_C1_DACEN_SHIFT) & 1U);
}

/*!
 * @brief Set the DAC state (enabled/disabled)
 * @param baseAddr - cmp base pointer
 * @param to_set - DAC state
 *  true - DAC is enabled
 *  false - DAC is disabled
 * @return - void
 */
static inline void CMP_SetDACState(CMP_Type* baseAddr, bool to_set)
{
    (baseAddr->C1) = ((baseAddr->C1) & (~(CMP_C1_DACEN_MASK))) | ((uint32_t)to_set << CMP_C1_DACEN_SHIFT);
}

/*!
 * @brief Return the current voltage reference
 * @param baseAddr - cmp base pointer
 * @return - voltage referece
 *  CMP_VIN1
 *  CMP_VIN2
 */
static inline cmp_voltage_reference_t CMP_GetVoltageReference(const CMP_Type* baseAddr)
{
    return (cmp_voltage_reference_t)(((baseAddr->C1) >> CMP_C1_VRSEL_SHIFT) & 1U);
}

/*!
 * @brief Set the voltage reference
 * @param baseAddr - cmp base pointer
 * @param to_set - voltage reference
 *  CMP_VIN1
 *  CMP_VIN2
 * @return - void
 */
static inline void CMP_SetVoltageReference(CMP_Type* baseAddr, cmp_voltage_reference_t to_set)
{
    (baseAddr->C1) = ((baseAddr->C1) & (~(CMP_C1_VRSEL_SHIFT))) | ((uint32_t)to_set << CMP_C1_VRSEL_SHIFT);
}

/*!
 * @brief Determine which input is selected for the plus mux
 * @param baseAddr - cmp base pointer
 * @return - channel for the plus mux
 */
static inline cmp_ch_number_t CMP_GetPlusMUXControl(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp = (tmp & CMP_C1_PSEL_MASK) >> CMP_C1_PSEL_SHIFT;
    return ( cmp_ch_number_t ) (tmp);
}

/*!
 * @brief Select input for the plus mux
 * @param baseAddr cmp base pointer
 * @param to_set - channel for the plus mux
 * @return - void
 */
static inline void CMP_SetPlusMuxControl(CMP_Type* baseAddr, cmp_ch_number_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_PSEL_MASK);
    tmp |= CMP_C1_PSEL(to_set);
    baseAddr->C1 = tmp;
}

/*!
 * @brief Determine which input is selected for the minus mux
 * @param baseAddr - cmp base pointer
 * @return - channel for the minus mux
 */
static inline cmp_ch_number_t CMP_GetMinusMUXControl(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp = (tmp & CMP_C1_MSEL_MASK) >> CMP_C1_MSEL_SHIFT;
    return ( cmp_ch_number_t ) (tmp);
}

/*!
 * @brief Select input for the minus mux
 * @param baseAddr - cmp base pointer
 * @param to_set - channel for the minus mux
 * @return - void
 */
static inline void CMP_SetMinusMUXControl(CMP_Type* baseAddr, cmp_ch_number_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_MSEL_MASK);
    tmp |= CMP_C1_MSEL(to_set);
    baseAddr->C1 = tmp;
}

/*!
 * @brief Return the current output voltage level(0-255)
 * @param baseAddr - cmp base pointer
 * @return - voltage level
 */
static inline uint8_t CMP_GetVoltage(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp = (tmp & CMP_C1_VOSEL_MASK) >> CMP_C1_VOSEL_SHIFT;
    return ( uint8_t ) (tmp);
}

/*!
 * @brief Set the output voltage level
 * @param baseAddr - cmp base pointer
 * @param to_set - voltage level
 * @return - void
 */
static inline void CMP_SetVoltage(CMP_Type* baseAddr, uint8_t to_set)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_VOSEL_MASK);
    tmp |= CMP_C1_VOSEL(to_set);
    baseAddr->C1 = tmp;
}

/*!
 * @brief Verify if the round robin operation is enabled
 * @param baseAddr -cmp base pointer
 * @return - round-robin operation state
 *  true - round robin operation is enabled
 *  false - round robin operation is disabled
 */
static inline bool CMP_GetRoundRobinState(const CMP_Type* baseAddr)
{
    return (bool)(((baseAddr->C2) >> CMP_C2_RRE_SHIFT) & 1U);
}

/*!
 * @brief Set the round robin operation state
 * @param baseAddr cmp base pointer
 * @param to_set - round robin operation state
 *  true - round robin operation is enabled
 *  false - round robin operation is disabled
 * @return - void
 */
static inline void CMP_SetRoundRobinState(CMP_Type* baseAddr, bool to_set)
{
    (baseAddr->C2) = ((baseAddr->C2) &(~(CMP_C2_RRE_MASK))) | ((uint32_t)to_set << CMP_C2_RRE_SHIFT);
}

/*!
 * @brief Verify if the round robin interrupt is enabled
 * @param baseAddr - cmp base pointer
 * @return - round-robin interrupt state
 *  true - round robin interrupt is enabled
 *  false - round robin interrupt is disabled
 */
static inline bool CMP_GetRoundRobinInterruptState(const CMP_Type* baseAddr)
{
    return (bool)(((baseAddr->C2) >> CMP_C2_RRIE_SHIFT) & 1U);
}

/*!
 * @brief Set the round robin interrupt state
 * @param baseAddr - cmp base pointer
 * @param to_set - round robin interrupt state
 *  true - round robin interrupt is enabled
 *  false - round robin interrupt is disabled
 * @return - void
 */
static inline void CMP_SetRoundRobinInterruptState(CMP_Type* baseAddr, bool to_set)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_RRIE_SHIFT))) | ((uint32_t)to_set << CMP_C2_RRIE_SHIFT);
}

/*!
 * @brief Return the port fixed for round-robin operation
 * @param baseAddr - cmp base pointer
 * @return - fixed port
 */
static inline cmp_fixed_port_t CMP_GetFixedPort(const CMP_Type* baseAddr)
{
    return (cmp_fixed_port_t)(((baseAddr->C2) >> CMP_C2_FXMP_SHIFT) & 1U);
}

/*!
 * @brief Set the fixed port for round-robin operation
 * @param baseAddr - cmp base pointer
 * @param to_set - fixed port
 *  CMP_PLUS_FIXED
 *  CMP_MINUS_FIXED
 * @return - void
 */
static inline void CMP_SetFixedPort(CMP_Type* baseAddr, cmp_fixed_port_t to_set)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_FXMP_MASK))) | ((uint32_t)to_set << CMP_C2_FXMP_SHIFT);
}

/*!
 * @brief Return which channel is selected for fixed mux port(as fixed reference)
 * @param baseAddr - cmp base pointer
 * @return - fixed channel
 *  CMP_PLUS_FIXED
 *  CMP_MINUS_FIXED
 */
static inline cmp_ch_number_t CMP_GetFixedChannel(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp = (tmp & CMP_C2_FXMXCH_MASK) >> CMP_C2_FXMXCH_SHIFT;
    return ( cmp_ch_number_t ) (tmp);
}

/*!
 * @brief Set which channel is used as the fixed reference input for the fixed mux port
 * @param baseAddr - cmp base pointer
 * @param to_set - fixed channel
 * @return - void
 */
static inline void CMP_SetFixedChannel(CMP_Type* baseAddr, cmp_ch_number_t to_set)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_FXMXCH_MASK);
    tmp |= CMP_C2_FXMXCH(to_set);
    baseAddr->C2 = tmp;
}

/*!
 * @brief Return how many round-robin clock cycles takes sampling
 * @param baseAddr - cmp base pointer
 * @return - number of sample clocks
 */
static inline uint8_t CMP_GetRoundRobinSamplesNumber(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp = (tmp & CMP_C2_NSAM_MASK) >> CMP_C2_NSAM_SHIFT;
    return ( uint8_t ) (tmp);
}

/*!
 * @brief Set how many round-robin clock cycles takes sampling
 * @param baseAddr - cmp base pointer
 * @param to_set - number of sample clocks(min value 0, max value 3)
 * @return - void
 */
static inline void CMP_SetRoundRobinSamplesNumber(CMP_Type* baseAddr, uint8_t to_set)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_NSAM_MASK);
    tmp |= CMP_C2_NSAM(to_set);
    baseAddr->C2 = tmp;
}

#if FEATURE_CMP_HAS_INIT_DELAY

/*!
 * @brief Return the comparator and DAC initialization delay
 * @param baseAddr - cmp base pointer
 * @return - delay(round-robin clock period)
 */
static inline uint8_t CMP_GetInitDelay(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp = (tmp & CMP_C2_INITMOD_MASK) >> CMP_C2_INITMOD_SHIFT;
    return ( uint8_t ) (tmp);
}

/*!
 * @brief Set the comparator and  DAC initialization delay
 * @param baseAddr - cmp base pointer
 * @param to_set - delay (min value 0, max value 63)
 * @return - void
 */
static inline void CMP_SetInitDelay(CMP_Type* baseAddr, uint8_t to_set)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_INITMOD_MASK);
    tmp |= CMP_C2_INITMOD(to_set);
    baseAddr->C2 = tmp;
}

#endif

/*!
 * @brief Return last input comparison results for all channels
 * @param baseAddr - cmp base pointer
 * @return - comparison results
 */
static inline cmp_ch_list_t CMP_GetLastComparisonResult(const CMP_Type* baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp = (tmp & CMP_C2_ACOn_MASK) >> CMP_C2_ACOn_SHIFT;
    return ( cmp_ch_list_t ) (tmp);
}

/*!
 * @brief Defines the pre-set state of input channels.
 * @param baseAddr cmp base pointer
 * @param to_set - state
 * @return void
 */
static inline void CMP_SetPresetState(CMP_Type* baseAddr, cmp_ch_list_t to_set)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_ACOn_MASK);
    tmp |= CMP_C2_ACOn(to_set);
    baseAddr->C2 = tmp;
}

#if FEATURE_CMP_DAC_FIX_SELECTION
/*!
 * @brief Verify if the DAC is Fixed
 * @param baseAddr - cmp base pointer
 * @return - dac state
 *  true - DAC is fixed
 *  false - DAC is not fixed
 */
static inline bool CMP_GetFixedDACState(const CMP_Type* baseAddr)
{
    return (bool)(((baseAddr->C2) >> CMP_C2_FXDACI_SHIFT) & 1U);
}

/*!
 * @brief Set the Fixed DAC state (enabled/disabled)
 * @param baseAddr - cmp base pointer
 * @param to_set - DAC Fixed state
 *  true - DAC is fixed
 *  false - DAC is not Fixed
 * @return - void
 */
static inline void CMP_SetFixedDACState(CMP_Type* baseAddr, bool to_set)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_FXDACI_MASK))) | ((uint32_t)to_set << CMP_C2_FXDACI_SHIFT);
}

#endif

/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __CMP_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
