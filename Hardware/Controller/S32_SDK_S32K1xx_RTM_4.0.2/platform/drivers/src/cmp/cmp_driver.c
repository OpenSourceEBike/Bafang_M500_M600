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
* Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
* The function is defined for use by application code.
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
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
* The cast is required to initialize a pointer with an unsigned long define, representing an address. 
*
* @section [global]
* Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
* The cast is required to initialize a pointer with an unsigned long define, representing an address. 
*/
#include "cmp_driver.h"
#include "cmp_hw_access.h"
#include <stddef.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Table of base addresses for CMP instances. */
static CMP_Type * const g_cmpBase[] = CMP_BASE_PTRS;
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_Reset
 * Description   : This function set all CMP registers to reset values.
 *
 * Implements : CMP_DRV_Reset_Activity
 *END**************************************************************************/
status_t CMP_DRV_Reset(const uint32_t instance)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* baseAddr = g_cmpBase[instance];    
    baseAddr->C0 = C0_RESET_VALUE;
    baseAddr->C1 = C1_RESET_VALUE;
    baseAddr->C2 = C1_RESET_VALUE;
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInitConfigAll
 * Description   : Return a configuration structure with reset values for all components
 * from comparator module.
 * Implements : CMP_DRV_GetInitConfigAll_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInitConfigAll(cmp_module_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
	(config->comparator).dmaTriggerState = false;
    (config->comparator).outputInterruptTrigger = CMP_NO_EVENT;
    (config->comparator).mode = CMP_DISABLED; 
    (config->comparator).filterSampleCount = 0U;
    (config->comparator).filterSamplePeriod = 0U;
    (config->comparator).powerMode = CMP_LOW_SPEED;
    (config->comparator).inverterState = CMP_NORMAL;
    (config->comparator).outputSelect = CMP_COUT;
    (config->comparator).pinState = CMP_UNAVAILABLE;
    (config->comparator).hysteresisLevel = CMP_LEVEL_HYS_0;
    (config->dac).state = false;
    (config->dac).voltageReferenceSource = CMP_VIN1;
    (config->dac).voltage = 0U;
    (config->mux).negativeInputMux = 0U;
    (config->mux).positiveInputMux = 0U;
    (config->mux).negativePortMux = CMP_DAC;
    (config->mux).positivePortMux = CMP_DAC;
    (config->triggerMode).roundRobinState = false;
    (config->triggerMode).roundRobinInterruptState = false; 
    (config->triggerMode).fixedPort = CMP_PLUS_FIXED;
    (config->triggerMode).fixedChannel = 0U;
    (config->triggerMode).samples = 0U;
    (config->triggerMode).roundRobinChannelsState = 0U;
    (config->triggerMode).programedState = 0U;

#if FEATURE_CMP_HAS_INIT_DELAY
    (config->triggerMode).initializationDelay = 0U;
#endif

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    (config->comparator).offsetLevel = CMP_LEVEL_OFFSET_0;
#endif

#if FEATURE_CMP_DAC_FIX_SELECTION
    (config->dac).fixRefInputMux = false;
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetDefaultConfig
 * Description   : Return a default configuration structure for all components
 * from comparator module.
 * Implements : CMP_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetDefaultConfig(cmp_module_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
	(config->comparator).dmaTriggerState = false;
    (config->comparator).outputInterruptTrigger = CMP_BOTH_EDGES;
    (config->comparator).mode = CMP_CONTINUOUS;
    (config->comparator).filterSampleCount = 0U;
    (config->comparator).filterSamplePeriod = 0U;
    (config->comparator).powerMode = CMP_LOW_SPEED;
    (config->comparator).inverterState = CMP_NORMAL;
    (config->comparator).outputSelect = CMP_COUT;
    (config->comparator).pinState = CMP_UNAVAILABLE;
    (config->comparator).hysteresisLevel = CMP_LEVEL_HYS_0;
    (config->dac).state = true;
    (config->dac).voltageReferenceSource = CMP_VIN1;
    (config->dac).voltage = 127U;
    (config->mux).negativeInputMux = 1U;
    (config->mux).positiveInputMux = 1U;
    (config->mux).negativePortMux = CMP_DAC;
    (config->mux).positivePortMux = CMP_MUX;
    (config->triggerMode).roundRobinState = false;
    (config->triggerMode).roundRobinInterruptState = false;
    (config->triggerMode).fixedPort = CMP_PLUS_FIXED;
    (config->triggerMode).fixedChannel = 0U;
    (config->triggerMode).samples = 0U;
    (config->triggerMode).roundRobinChannelsState = 0U;
    (config->triggerMode).programedState = 0U;

#if FEATURE_CMP_HAS_INIT_DELAY
    (config->triggerMode).initializationDelay = 0U;
#endif

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    (config->comparator).offsetLevel = CMP_LEVEL_OFFSET_0;
#endif

#if FEATURE_CMP_DAC_FIX_SELECTION
    (config->dac).fixRefInputMux = false;
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_Init
 * Description   : Configure all components from comparator module.
 *
 * Implements : CMP_DRV_Init_Activity 
 *END**************************************************************************/
status_t CMP_DRV_Init(const uint32_t instance, const cmp_module_t* const config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* base = g_cmpBase[instance];
    CMP_SetDMATriggerState(base, (config->comparator).dmaTriggerState );
    CMP_SetOutputInterruptTrigger(base, (config->comparator).outputInterruptTrigger);
    CMP_SetFunctionalMode(base, (config->comparator).mode, (config->comparator).filterSampleCount, (config->comparator).filterSamplePeriod);
    CMP_SetPowerMode(base, (config->comparator).powerMode);
    CMP_SetInverterState(base, (config->comparator).inverterState);
    CMP_SetComparatorOutputSource(base, (config->comparator).outputSelect);
    CMP_SetOutputPinState(base, (config->comparator).pinState);
    CMP_SetHysteresis(base, (config->comparator).hysteresisLevel);
    CMP_SetDACState(base, (config->dac).state);
    CMP_SetVoltageReference(base, (config->dac).voltageReferenceSource);
    CMP_SetVoltage(base, (config->dac).voltage);
    CMP_SetMinusMUXControl(base, (config->mux).negativeInputMux);
    CMP_SetPlusMuxControl(base, (config->mux).positiveInputMux);
    CMP_SetNegativePortInput(base, (config->mux).negativePortMux);
    CMP_SetPositivePortInput(base, (config->mux).positivePortMux);
    CMP_SetFixedPort(base, (config->triggerMode).fixedPort);
    CMP_SetFixedChannel(base, (config->triggerMode).fixedChannel);
    CMP_SetRoundRobinSamplesNumber(base, (config->triggerMode).samples);
    CMP_SetRoundRobinChannels(base, (config->triggerMode).roundRobinChannelsState);
    CMP_SetPresetState(base, (config->triggerMode).programedState);
    CMP_SetRoundRobinInterruptState(base, (config->triggerMode).roundRobinInterruptState);
    CMP_SetRoundRobinState(base, (config->triggerMode).roundRobinState);

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    CMP_SetOffset(base, (config->comparator).offsetLevel);
#endif

#if FEATURE_CMP_HAS_INIT_DELAY
    CMP_SetInitDelay(base, (config->triggerMode).initializationDelay);
#endif

#if FEATURE_CMP_DAC_FIX_SELECTION
    CMP_SetFixedDACState(base, (config->dac).fixRefInputMux);
#endif

    /* Clear all flags*/
    (void)CMP_DRV_ClearOutputFlags(instance);
    (void)CMP_DRV_ClearInputFlags(instance);
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetConfigAll
 * Description   : This function returns the configuration for all components 
 * from comparator module.
 * Implements : CMP_DRV_GetConfigAll_Activity  
 *
 *END**************************************************************************/
status_t CMP_DRV_GetConfigAll(const uint32_t instance, cmp_module_t* const config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* base = g_cmpBase[instance];
    (config->comparator).dmaTriggerState = CMP_GetDMATriggerState(base);
    (config->comparator).outputInterruptTrigger = CMP_GetOutputInterruptTrigger(base);
    (config->comparator).mode = CMP_GetFunctionalMode(base);
    (config->comparator).filterSampleCount = CMP_GetFilterSampleCount(base);
    (config->comparator).filterSamplePeriod = CMP_GetFilterSamplePeriod(base);
    (config->comparator).powerMode = CMP_GetPowerMode(base);
    (config->comparator).inverterState = CMP_GetInverterState(base);
    (config->comparator).outputSelect = CMP_GetComparatorOutputSource(base);
    (config->comparator).pinState = CMP_GetOutputPinState(base);
    (config->comparator).hysteresisLevel = CMP_GetHysteresis(base);
    (config->dac).state = CMP_GetDACState(base);
    (config->dac).voltageReferenceSource = CMP_GetVoltageReference(base);
    (config->dac).voltage = CMP_GetVoltage(base);
    (config->mux).negativePortMux = CMP_GetNegativePortInput(base);
    (config->mux).positivePortMux = CMP_GetPositivePortInput(base);
    (config->mux).negativeInputMux = CMP_GetMinusMUXControl(base);
    (config->mux).positiveInputMux = CMP_GetPlusMUXControl(base);
    (config->triggerMode).roundRobinState = CMP_GetRoundRobinState(base);
    (config->triggerMode).roundRobinInterruptState = CMP_GetRoundRobinInterruptState(base);
    (config->triggerMode).fixedPort = CMP_GetFixedPort(base);
    (config->triggerMode).fixedChannel = CMP_GetFixedChannel(base);
    (config->triggerMode).samples = CMP_GetRoundRobinSamplesNumber(base);
    (config->triggerMode).roundRobinChannelsState = CMP_GetRoundRobinChannels(base);
    (config->triggerMode).programedState = CMP_GetLastComparisonResult(base);

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    (config->comparator).offsetLevel = CMP_GetOffset(base);
#endif

#if FEATURE_CMP_HAS_INIT_DELAY
    (config->triggerMode).initializationDelay = CMP_GetInitDelay(base);
#endif

#if FEATURE_CMP_DAC_FIX_SELECTION
    (config->dac).fixRefInputMux = CMP_GetFixedDACState(base);
#endif

    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInitConfigDAC
 * Description   : Return configuration structure with reset values for DAC 
 * component from comparator module.
 * Implements : CMP_DRV_GetInitConfigDAC_Activity 
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInitConfigDAC(cmp_dac_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    config->state = false;
    config->voltageReferenceSource = CMP_VIN1;
    config->voltage = 0U;
	
#if FEATURE_CMP_DAC_FIX_SELECTION
	config->fixRefInputMux = false;
#endif
	
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ConfigDAC
 * Description   : Configure only DAC component from comparator module.
 * Implements : CMP_DRV_ConfigDAC_Activity  
 *
 *END**************************************************************************/
status_t CMP_DRV_ConfigDAC(const uint32_t instance, const cmp_dac_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
#if CMP_DAC_RESOLUTION != 255U
    DEV_ASSERT(config->voltage <= CMP_DAC_RESOLUTION);
#endif
    CMP_Type* base = g_cmpBase[instance];
    CMP_SetDACState(base, config->state);
    CMP_SetVoltageReference(base, config->voltageReferenceSource);
    CMP_SetVoltage(base, config->voltage);
#if FEATURE_CMP_DAC_FIX_SELECTION
    CMP_SetFixedDACState(base, config->fixRefInputMux);
#endif
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetDAC
 * Description   : Return configuration for DAC component from comparator module.
 * Implements : CMP_DRV_GetDACConfig_Activity 
 *
 *END**************************************************************************/
status_t CMP_DRV_GetDACConfig(const uint32_t instance, cmp_dac_t* const config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* base = g_cmpBase[instance];
    config->state = CMP_GetDACState(base);
    config->voltageReferenceSource = CMP_GetVoltageReference(base);
    config->voltage = CMP_GetVoltage(base);	
#if FEATURE_CMP_DAC_FIX_SELECTION
    config->fixRefInputMux = CMP_GetFixedDACState(base);
#endif
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInitConfigMUX
 * Description   : Return configuration structure with reset values for 
 * the MUX component which select source signals for comparator ports.
 * Implements : CMP_DRV_GetInitConfigMUX_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInitConfigMUX(cmp_anmux_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    config->negativePortMux = CMP_DAC;
    config->positivePortMux = CMP_DAC;
    config->negativeInputMux = 0U;
    config->positiveInputMux = 0U;

    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ConfigMUX
 * Description   : Configure only MUX component from comparator module to select
 * source signals for comparator ports.
 * Implements : CMP_DRV_ConfigMUX_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_ConfigMUX(const uint32_t instance, const cmp_anmux_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* base = g_cmpBase[instance];

    CMP_SetNegativePortInput(base, config->negativePortMux);
    CMP_SetPositivePortInput(base, config->positivePortMux);
    CMP_SetMinusMUXControl(base, config->negativeInputMux);
    CMP_SetPlusMuxControl(base, config->positiveInputMux);
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetMUX
 * Description   : Return configuration for the MUX component which select 
 * source signals for comparator ports.
 * Implements : CMP_DRV_GetMUXConfig_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetMUXConfig(const uint32_t instance, cmp_anmux_t* const config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* base = g_cmpBase[instance];
    config->negativePortMux = CMP_GetNegativePortInput(base);
    config->positivePortMux = CMP_GetPositivePortInput(base);
    config->negativeInputMux = CMP_GetMinusMUXControl(base);
    config->positiveInputMux = CMP_GetPlusMUXControl(base);
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInitTriggerMode
 * Description   : Return configuration structure with reset values for Trigger Mode
 * from comparator module.
 * Implements : CMP_DRV_GetInitTriggerMode_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInitTriggerMode(cmp_trigger_mode_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    config->roundRobinState = false;
    config->roundRobinInterruptState = false; 
    config->fixedPort = CMP_PLUS_FIXED;
    config->fixedChannel = 0U;
    config->samples = 0U;
    config->roundRobinChannelsState = 0U;
    config->programedState = 0U;
#if FEATURE_CMP_HAS_INIT_DELAY
    config->initializationDelay = 0U;
#endif
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ConfigTriggerMode
 * Description   : Configure comparator in trigger mode.
 * Implements : CMP_DRV_ConfigTriggerMode_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_ConfigTriggerMode(const uint32_t instance, const cmp_trigger_mode_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* base = g_cmpBase[instance];
    CMP_SetRoundRobinState(base, config->roundRobinState);
    CMP_SetRoundRobinInterruptState(base, config->roundRobinInterruptState);
    CMP_SetFixedPort(base, config->fixedPort);
    CMP_SetFixedChannel(base, config->fixedChannel);
    CMP_SetRoundRobinSamplesNumber(base, config->samples);

#if FEATURE_CMP_HAS_INIT_DELAY
    CMP_SetInitDelay(base, config->initializationDelay);
#endif

    CMP_SetRoundRobinChannels(base, config->roundRobinChannelsState);
    CMP_SetPresetState(base, config->programedState);
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetTriggerMode
 * Description   : Return configuration for the trigger mode.
 * Implements : CMP_DRV_GetTriggerModeConfig_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetTriggerModeConfig(const uint32_t instance, cmp_trigger_mode_t* const config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* base = g_cmpBase[instance];
    config->roundRobinState = CMP_GetRoundRobinState(base);
    config->roundRobinInterruptState = CMP_GetRoundRobinInterruptState(base);
    config->fixedPort = CMP_GetFixedPort(base);
    config->fixedChannel = CMP_GetFixedChannel(base);
    config->samples = CMP_GetRoundRobinSamplesNumber(base);
    config->roundRobinChannelsState = CMP_GetRoundRobinChannels(base);
    config->programedState = CMP_GetLastComparisonResult(base);

#if FEATURE_CMP_HAS_INIT_DELAY
    config->initializationDelay =  CMP_GetInitDelay(base);
#endif

    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetOutputFlags
 * Description   : Return in <flags> comparator output flags(rising and falling edge on output).
 * Implements : CMP_DRV_GetOutputFlags_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetOutputFlags(const uint32_t instance, cmp_output_trigger_t *flags)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(flags != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* baseAddr = g_cmpBase[instance];
    bool rising_enabled = (bool)(((baseAddr->C0) >> CMP_C0_CFR_SHIFT) & (uint32_t)1U);
    bool falling_enabled = (bool)(((baseAddr->C0) >> CMP_C0_CFF_SHIFT) & (uint32_t)1U);
    *flags = (cmp_output_trigger_t) (((uint32_t)rising_enabled << (uint32_t)1U) | ((uint32_t)falling_enabled));
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ClearOutputFlags
 * Description   : Clear comparator output flags(rising and falling edge on output).
 * Implements : CMP_DRV_ClearOutputFlags_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_ClearOutputFlags(const uint32_t instance)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* baseAddr = g_cmpBase[instance];
    
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_CFR_MASK);
    tmp |= CMP_C0_CFR(1);
    tmp &= ~(CMP_C0_CFF_MASK);
    tmp |= CMP_C0_CFF(1);
    baseAddr->C0 = tmp;
    
    #ifdef ERRATA_E9005
        (void)baseAddr->C0;
    #endif
    return status;
 }
 
 /*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInputFlags
 * Description   : Return all input change flags in <flags>.
 * <flags> format : Flag_Ch7 Flag_Ch6 ... Flag_Ch0
 * Implements : CMP_DRV_GetInputFlags_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInputFlags(const uint32_t instance,  cmp_ch_list_t *flags)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(flags != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* baseAddr = g_cmpBase[instance];
    
    uint32_t tmp = baseAddr->C2;
    *flags = (cmp_ch_list_t)((tmp & (uint32_t)CMP_INPUT_FLAGS_MASK) >> CMP_INPUT_FLAGS_SHIFT);
    return status;
 }
 /*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ClearInputFlags
 * Description   : Clear all input change flags .
 * Implements : CMP_DRV_ClearInputFlags_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_ClearInputFlags(const uint32_t instance)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* baseAddr = g_cmpBase[instance];
    
    uint32_t tmp = baseAddr->C2;
    tmp |= (uint32_t)CMP_INPUT_FLAGS_MASK;
    baseAddr->C2 = tmp;
    #ifdef ERRATA_E9005
        (void)baseAddr->C2;
    #endif    
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetInitConfigComparator
 * Description   : Return configuration structure with reset values for comparator features (functional mode, power mode,
 * inverter, hysteresis, offset, filter sampling period and samples count).
 * Implements : CMP_DRV_GetInitConfigComparator_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetInitConfigComparator(cmp_comparator_t* config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    config->outputInterruptTrigger = CMP_NO_EVENT;
    config->dmaTriggerState = false;
    config->mode = CMP_DISABLED; 
    config->filterSampleCount = 0U;
    config->filterSamplePeriod = 0U;
    config->powerMode = CMP_LOW_SPEED;
    config->inverterState = CMP_NORMAL;
    config->outputSelect = CMP_COUT;
    config->pinState = CMP_UNAVAILABLE; 
#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    config->offsetLevel = CMP_LEVEL_OFFSET_0;
#endif
    config->hysteresisLevel = CMP_LEVEL_HYS_0;
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_ConfigComparator
 * Description   : Configure only comparator features (functional mode, power mode,
 * inverter, hysteresis, offset, filter sampling period and samples count).
 * Implements : CMP_DRV_ConfigComparator_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_ConfigComparator(const uint32_t instance, const cmp_comparator_t *config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    CMP_Type* base = g_cmpBase[instance];
    CMP_SetDMATriggerState(base, config->dmaTriggerState );
    CMP_SetOutputInterruptTrigger(base, config->outputInterruptTrigger);
    CMP_SetFunctionalMode(base, config->mode, config->filterSampleCount, config->filterSamplePeriod);
    CMP_SetFilterSamplePeriod(base, config->filterSamplePeriod);
    CMP_SetFilterSampleCount(base, config->filterSampleCount);
    CMP_SetPowerMode(base, config->powerMode);
    CMP_SetInverterState(base, config->inverterState);
    CMP_SetComparatorOutputSource(base, config->outputSelect);
    CMP_SetOutputPinState(base, config->pinState);

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    CMP_SetOffset(base, config->offsetLevel);
#endif

    CMP_SetHysteresis(base, config->hysteresisLevel);
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CMP_DRV_GetComparator
 * Description   : Return configuration for comparator components from CMP module.
 * Implements : CMP_DRV_GetComparatorConfig_Activity
 *
 *END**************************************************************************/
status_t CMP_DRV_GetComparatorConfig(const uint32_t instance, cmp_comparator_t *config)
{
    status_t  status = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(instance < CMP_INSTANCE_COUNT);
    const CMP_Type* base = g_cmpBase[instance];
    config->dmaTriggerState = CMP_GetDMATriggerState(base);
    config->outputInterruptTrigger = CMP_GetOutputInterruptTrigger(base);
    config->mode = CMP_GetFunctionalMode(base);
    config->filterSampleCount = CMP_GetFilterSampleCount(base);
    config->filterSamplePeriod = CMP_GetFilterSamplePeriod(base);
    config->powerMode = CMP_GetPowerMode(base);
    config->inverterState = CMP_GetInverterState(base);
    config->outputSelect = CMP_GetComparatorOutputSource(base);
    config->pinState = CMP_GetOutputPinState(base);

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    config->offsetLevel = CMP_GetOffset(base);
#endif

    config->hysteresisLevel = CMP_GetHysteresis(base);
    return status;
}
/******************************************************************************
 * EOF
 *****************************************************************************/

