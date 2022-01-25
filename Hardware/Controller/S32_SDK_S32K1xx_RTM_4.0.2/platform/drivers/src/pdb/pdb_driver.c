/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

#include "pdb_driver.h"
#include "pdb_hw_access.h"
#include "interrupt_manager.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type
 * The base addresses are provided as integers so they need to be cast to
 * pointers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer
 * The base addresses are provided as integers so they need to be cast to
 * pointers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for PDB instances. */
static PDB_Type * const s_pdbBase[PDB_INSTANCE_COUNT] = PDB_BASE_PTRS;

/* Table to save PDB IRQ enum numbers. */
static const IRQn_Type s_pdbIrqId[PDB_INSTANCE_COUNT] = PDB_IRQS;

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Init
 * Description   : This function initializes the PDB counter, input triggers
 * and general pre-trigger settings.
 * It resets PDB registers and enables the clock for PDB. So it should be
 * called before any operation to PDB module. After initialized, the PDB can
 * act as a triggered timer, which lays the foundation for other features in
 * PDB module.
 *
 * Implements : PDB_DRV_Init_Activity
 *END*************************************************************************/
void PDB_DRV_Init(const uint32_t instance,
                  const pdb_timer_config_t * const userConfigPtr)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(userConfigPtr != NULL);
    /* Table of PDB clock names as defined in clock manager. */
#if (PDB_INSTANCE_COUNT == 1U)
    static const clock_names_t s_pdbClkNames[PDB_INSTANCE_COUNT] = {PDB0_CLK};
#elif (PDB_INSTANCE_COUNT == 2U)
    static const clock_names_t s_pdbClkNames[PDB_INSTANCE_COUNT] = {PDB0_CLK, PDB1_CLK};
#endif
    status_t clkStatus;

    PDB_Type * base = s_pdbBase[instance];

    clock_names_t instanceClkName = s_pdbClkNames[instance];

    /* Get the PDB clock as configured in the clock manager */
    clkStatus = CLOCK_SYS_GetFreq(instanceClkName, NULL);

    /* Exit if clock is not enabled for current instance. */
    DEV_ASSERT(clkStatus == STATUS_SUCCESS);
    (void)clkStatus;

    /* Reset the registers for PDB module to reset state. */
    PDB_Init(base);
    PDB_ConfigTimer(base, userConfigPtr);

#if FEATURE_PDB_HAS_INSTANCE_BACKTOBACK
    /* Note: this configuration is common between PDB instances 0 and 1 (configures the same register even if configured for either PDB instance)
       Enabling when configuring PDB0, then disabling when configuring PDB1, will result in being disabled. */
    if (userConfigPtr->instanceBackToBackEnable)
    {
        PDB_SIM_EnableInstanceBackToBack();
    }
    else
    {
        PDB_SIM_DisableInstanceBackToBack();
    }
#endif /* FEATURE_PDB_HAS_INSTANCE_BACKTOBACK */

#if FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK
    if (userConfigPtr->interchannelBackToBackEnable)
    {
        PDB_SIM_EnableInterchannelBackToBack(instance);
    }
    else
    {
        PDB_SIM_DisableInterchannelBackToBack(instance);
    }
#endif /* FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK */

    /* Configure NVIC. */
    if (userConfigPtr->intEnable || userConfigPtr->seqErrIntEnable)
    {
        INT_SYS_EnableIRQ(s_pdbIrqId[instance]); /* Enable PDB interrupt in NVIC level.*/
    }
    else
    {
        INT_SYS_DisableIRQ(s_pdbIrqId[instance]); /* Disable PDB interrupt in NVIC level.*/
    }
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Deinit
 * Description   : De-initialize the PDB module.
 * When the PDB module is not used. Calling this function would shut down the
 * PDB module and reduce the power consumption.
 *
 * Note: instance back to back configuration is common between PDB instances 0 and 1 (configures the same register even if configured for either PDB instance)
 * This function disables it, so affects all other instances.
 *
 * Implements : PDB_DRV_Deinit_Activity
 *END*************************************************************************/
void PDB_DRV_Deinit(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    INT_SYS_DisableIRQ(s_pdbIrqId[instance]);

#if FEATURE_PDB_HAS_INSTANCE_BACKTOBACK
    /* Note: instance back to back configuration is common between PDB instances 0 and 1 (configures the same register even if configured for either PDB instance)
       Enabling when configuring PDB0, then disabling when configuring PDB1, will result in being disabled. */
    PDB_SIM_DisableInstanceBackToBack();
#endif /* FEATURE_PDB_HAS_INSTANCE_BACKTOBACK */

#if FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK
    PDB_SIM_DisableInterchannelBackToBack(instance);
#endif /* FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK */

    PDB_Disable(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetDefaultConfig
 * Description   : Gets the default configuration structure of PDB with default settings.
 * This function is used to initialize the hardware configuration structure to default values.
 *
 * Implements : PDB_DRV_GetDefaultConfig_Activity
 *END*************************************************************************/
void PDB_DRV_GetDefaultConfig(pdb_timer_config_t * const config)
{
    DEV_ASSERT(config != NULL);
    /* Load immediately after load operation. */
    config->loadValueMode = PDB_LOAD_VAL_IMMEDIATELY;
    /* Sequence error interrupt disable */
    config->seqErrIntEnable = false;
    /* Prescaler divider select by MULT */
    config->clkPreDiv = PDB_CLK_PREDIV_BY_1;
    /* Select multiplication source mode */
    config->clkPreMultFactor = PDB_CLK_PREMULT_FACT_AS_1;
    /* Trigger input source selection */
    config->triggerInput = PDB_TRIGGER_IN0;
    /* Run in One-shot mode */
    config->continuousModeEnable = false;
    /* Disable DMA */
    config->dmaEnable = false;
    /* Disable interrupt */
    config->intEnable = false;
#if FEATURE_PDB_HAS_INSTANCE_BACKTOBACK
    /* Disable instanceBackToBackEnable */	
    config->instanceBackToBackEnable = false;
#endif /* FEATURE_PDB_HAS_INSTANCE_BACKTOBACK */
#if FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK
    /* Disable interchannelBackToBackEnable */	
    config->interchannelBackToBackEnable = false;
#endif /* FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK */
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Enable
 * Description   : Enables the PDB module, counter is on.
 *
 * Implements : PDB_DRV_Enable_Activity
 *END*************************************************************************/
void PDB_DRV_Enable(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);

    PDB_Type * base = s_pdbBase[instance];
    PDB_Enable(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_Disable
 * Description   : This function disables the PDB module, counter is off also.
 *
 * Implements : PDB_DRV_Disable_Activity
 *END*************************************************************************/
void PDB_DRV_Disable(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);

    PDB_Type * base = s_pdbBase[instance];
    PDB_Disable(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SoftTriggerCmd
 * Description   : Trigger PDB by software trigger.
 * When the PDB is set to use software trigger as input, Calling this function
 * would trigger the PDB.
 *
 * Implements : PDB_DRV_SoftTriggerCmd_Activity
 *END*************************************************************************/
void PDB_DRV_SoftTriggerCmd(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetSoftTriggerCmd(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetTimerValue
 * Description   : Get the current value of the PDB counter.
 *
 * Implements : PDB_DRV_GetTimerValue_Activity
 *END*************************************************************************/
uint32_t PDB_DRV_GetTimerValue(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type const * base = s_pdbBase[instance];

    return PDB_GetTimerValue(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetTimerIntFlag
 * Description   : Get the interrupt flag for PDB module. It will be
 * asserted if the PDB interrupt occurs.
 *
 * Implements : PDB_DRV_GetTimerIntFlag_Activity
 *END*************************************************************************/
bool PDB_DRV_GetTimerIntFlag(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type const * base = s_pdbBase[instance];

    return PDB_GetTimerIntFlag(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ClearTimerIntFlag
 * Description   : Clear the interrupt flag for PDB module.
 *
 * Implements : PDB_DRV_ClearTimerIntFlag_Activity
 *END*************************************************************************/
void PDB_DRV_ClearTimerIntFlag(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_ClearTimerIntFlag(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_LoadValuesCmd
 * Description   : Execute the command of loading values.
 *
 * Implements : PDB_DRV_LoadValuesCmd_Activity
 *END*************************************************************************/
void PDB_DRV_LoadValuesCmd(const uint32_t instance)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetLoadValuesCmd(base);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetTimerModulusValue
 * Description   : Set the value of timer modulus.
 *
 * Implements : PDB_DRV_SetTimerModulusValue_Activity
 *END*************************************************************************/
void PDB_DRV_SetTimerModulusValue(const uint32_t instance,
                                  const uint16_t value)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetTimerModulusValue(base, value);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetValueForTimerInterrupt
 * Description   : Set the value for the timer interrupt.
 *
 * Implements : PDB_DRV_SetValueForTimerInterrupt_Activity
 *END*************************************************************************/
void PDB_DRV_SetValueForTimerInterrupt(const uint32_t instance,
                                       const uint16_t value)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetValueForTimerInterrupt(base, value);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ConfigAdcPreTrigger
 * Description   : Configure the ADC pre_trigger in the PDB module.
 *
 * Implements : PDB_DRV_ConfigAdcPreTrigger_Activity
 *END*************************************************************************/
void PDB_DRV_ConfigAdcPreTrigger(const uint32_t instance,
                                 const uint32_t chn,
                                 const pdb_adc_pretrigger_config_t * configPtr)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
    DEV_ASSERT(configPtr != NULL);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetAdcPreTriggerEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)), configPtr->preTriggerEnable);
    PDB_SetAdcPreTriggerOutputEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)), configPtr->preTriggerOutputEnable);
    PDB_SetAdcPreTriggerBackToBackEnable(base, chn, ((uint32_t)1U << (configPtr->adcPreTriggerIdx)), configPtr->preTriggerBackToBackEnable);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetAdcPreTriggerFlags
 * Description   : Get the ADC pre_trigger flag in the PDB module.
 *
 * Implements : PDB_DRV_GetAdcPreTriggerFlags_Activity
 *END*************************************************************************/
uint32_t PDB_DRV_GetAdcPreTriggerFlags(const uint32_t instance,
                                       const uint32_t chn,
                                       const uint32_t preChnMask)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
    PDB_Type const * base = s_pdbBase[instance];

    return PDB_GetAdcPreTriggerFlags(base, chn, preChnMask);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ClearAdcPreTriggerFlags
 * Description   : Clear the ADC pre_trigger flag in the PDB module.
 *
 * Implements : PDB_DRV_ClearAdcPreTriggerFlags_Activity
 *END*************************************************************************/
void PDB_DRV_ClearAdcPreTriggerFlags(const uint32_t instance,
                                     const uint32_t chn,
                                     const uint32_t preChnMask)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_ClearAdcPreTriggerFlags(base, chn, preChnMask);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_GetAdcPreTriggerSeqErrFlags
 * Description   : Get the ADC pre_trigger flag in the PDB module.
 *
 * Implements : PDB_DRV_GetAdcPreTriggerSeqErrFlags_Activity
 *END*************************************************************************/
uint32_t PDB_DRV_GetAdcPreTriggerSeqErrFlags(const uint32_t instance,
                                             const uint32_t chn,
                                             const uint32_t preChnMask)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
    PDB_Type const * base = s_pdbBase[instance];

    return PDB_GetAdcPreTriggerSeqErrFlags(base, chn, preChnMask);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_ClearAdcPreTriggerSeqErrFlags
 * Description   : Clear the ADC pre_trigger flag in the PDB module.
 *
 * Implements : PDB_DRV_ClearAdcPreTriggerSeqErrFlags_Activity
 *END*************************************************************************/
void PDB_DRV_ClearAdcPreTriggerSeqErrFlags(const uint32_t instance,
                                           const uint32_t chn,
                                           const uint32_t preChnMask)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_ClearAdcPreTriggerSeqErrFlags(base, chn, preChnMask);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetAdcPreTriggerDelayValue
 * Description   : Set the ADC pre_trigger delay value in the PDB module.
 *
 * Implements : PDB_DRV_SetAdcPreTriggerDelayValue_Activity
 *END*************************************************************************/
void PDB_DRV_SetAdcPreTriggerDelayValue(const uint32_t instance,
                                        const uint32_t chn,
                                        const uint32_t preChn,
                                        const uint32_t value)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(chn < PDB_CH_COUNT);
	DEV_ASSERT(value <= PDB_DLY_DLY_MASK);
	DEV_ASSERT(value > 0u);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetAdcPreTriggerDelayValue(base, chn, preChn, value);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetCmpPulseOutEnable
 * Description   : Switch on/off the CMP pulse out in the PDB module.
 *
 * Implements : PDB_DRV_SetCmpPulseOutEnable_Activity
 *END*************************************************************************/
void PDB_DRV_SetCmpPulseOutEnable(const uint32_t instance,
                                  const uint32_t pulseChnMask,
                                  const bool enable)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetCmpPulseOutEnable(base, pulseChnMask, enable);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetCmpPulseOutDelayForHigh
 * Description   : Set the CMP pulse out delay value for high in the PDB module.
 *
 * Implements : PDB_DRV_SetCmpPulseOutDelayForHigh_Activity
 *END*************************************************************************/
void PDB_DRV_SetCmpPulseOutDelayForHigh(const uint32_t instance,
                                        const uint32_t pulseChn,
                                        const uint32_t value)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(pulseChn < PDB_POnDLY_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetCmpPulseOutDelayForHigh(base, pulseChn, value);
}

/*FUNCTION*********************************************************************
 *
 * Function Name : PDB_DRV_SetCmpPulseOutDelayForLow
 * Description   : Set the CMP pulse out delay value for low in the PDB module.
 *
 * Implements : PDB_DRV_SetCmpPulseOutDelayForLow_Activity
 *END*************************************************************************/
void PDB_DRV_SetCmpPulseOutDelayForLow(const uint32_t instance,
                                       const uint32_t pulseChn,
                                       const uint32_t value)
{
    DEV_ASSERT(instance < PDB_INSTANCE_COUNT);
    DEV_ASSERT(pulseChn < PDB_POnDLY_COUNT);
    PDB_Type * base = s_pdbBase[instance];

    PDB_SetCmpPulseOutDelayForLow(base, pulseChn, value);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
