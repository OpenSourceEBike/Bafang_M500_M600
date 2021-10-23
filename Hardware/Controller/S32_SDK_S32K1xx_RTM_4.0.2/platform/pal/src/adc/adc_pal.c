/*
 * Copyright 2017-2020 NXP
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
 * @file adc_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is part of PAL API, so it defined to be used by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to
 * object type and a pointer to a different object type.
 * The type of the configuration structure extension is platform dependent
 * and needs to be casted from a generic void pointer.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void
 * to pointer to other type
 * The type of the configuration structure extension is platform dependent
 * and needs to be casted from a generic void pointer.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * Assignment is required to calculate CTU FIFO IRQnumber.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially unsigned' to 'essentially enum<i>'
 * The cast is required to calculate CTU FIFO IRQnumber.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function 'ADC_SAR_CTU_Init(uint32_t, const adc_config_t *)'
 * Early return is used to reduce code nesting depth.
 *
 */

#include <stddef.h>
#include "adc_pal.h"
#include "adc_irq.h"
#include "device_registers.h"
#include "interrupt_manager.h"
#include "osif.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*! @cond DRIVER_INTERNAL_USE_ONLY */


typedef struct
{
    uint16_t bufferLength;            /*!< Length of the buffer associated with the current active conversion group */
    uint16_t currentBufferOffset;     /*!< Offset (in elements) of the next position to be written in the result buffer */
    bool notificationEn;              /*!< Flag for enabling/disabling notification */
    bool active;                      /*!< Flag for state of the group enabled/disabled */
#if defined (ADC_PAL_SAR_CTU)
    uint32_t groupIdx;                /*!< Index of the group for which the state is stored */
#endif
} adc_group_state_t;

/*!
 * @brief Runtime state of the ADC PAL
 *
 * This structure is used by the ADC PAL for storing internal information, needed at runtime.
 */
typedef struct
{
    const adc_group_config_t * groupArray;                            /*!< Pointer to the array of group configurations */
    uint16_t numGroups;                                               /*!< Number of group configurations available in the input array */
    adc_group_state_t hwTrigGroupState[ADC_PAL_MAX_NUM_HW_GROUPS_EN]; /*!< State of Hw triggered groups */
    adc_group_state_t swTrigGroupState;                               /*!< State of the most recent activated SW trigger group */
    uint32_t latestGroupIdx;                                          /*!< Index of the most recently enabled group (HW or SW triggered) group (might not be active anymore) */
#if (defined (ADC_PAL_S32K1xx))
    pdb_clk_prescaler_div_t pdbPrescaler;                            /*!< PDB clock prescaler */
#endif
#if (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))
    uint32_t stateIdxMapping[ADC_PAL_TOTAL_NUM_GROUPS];               /*!< Maps groupIdx to index in hwTrigGroupState. For groupIdx corresponding to swTrigGroups, value is invalid. */
#endif
} adc_pal_state_t;


#if defined (ADC_PAL_S32K1xx)

#if (ADC_PAL_MAX_NUM_HW_GROUPS_EN != 1u)
#error "The current device supports maximum 1 HW triggered group enabled"
#endif /* (ADC_PAL_MAX_NUM_HW_GROUPS_EN != 1) */

/* For S32K1xx, ADC PAL instances are mapped 1:1 with ADC instances (and PDB instances). */
#define NUMBER_OF_ADC_PAL_INSTANCES ADC_INSTANCE_COUNT

#define ADC_PAL_PDB_CHAN            (0u)                /*!< PDB channel used for triggering ADC */
#define ADC_PAL_TRGMUX_IDX          (0u)                /*!< TRGMUX instance used by ADC PAL */
#define ADC_PAL_MAX_CONVS_IN_GROUP  (PDB_DLY_COUNT)     /*!< Maximum number of ADC conversions in a group of conversions. */

/* For S32K1xx, the ADC PAL assumes that ADC and PDB instances are mapped 1:1, thus PDB_INSTANCE_COUNT == ADC_INSTANCE_COUNT */
static const trgmux_target_module_t adcPalTrgmuxTarget[ADC_INSTANCE_COUNT] = {
#if (ADC_INSTANCE_COUNT >= 1u)
    TRGMUX_TARGET_MODULE_PDB0_TRG_IN,
#endif
#if (ADC_INSTANCE_COUNT >= 2u)
    TRGMUX_TARGET_MODULE_PDB1_TRG_IN
#endif
};

static status_t ADC_Init_S32K1xx(const uint32_t instance,
                                 const adc_config_t * const config);

static inline void ADC_ConfigPdbAndPretriggers(const uint32_t instIdx,
                                               const pdb_trigger_src_t trgSrc,
                                               const adc_group_config_t * currentGroupCfg);

static void ADC_ConfigGroup(const uint32_t instance,
                            const uint32_t groupIdx,
                            const bool hwTriggerFlag);

static status_t ADC_StopGroupBlocking(const uint32_t instance,
                                      const uint32_t timeout);

#elif defined (ADC_PAL_MPC574xC_G_R)

/* For MPC574xC_G_R, ADC PAL instances are mapped 1:1 with BCTU instances.
 * Each BCTU instance may have multiple ADCs connected. */
#define NUMBER_OF_ADC_PAL_INSTANCES        BCTU_INSTANCE_COUNT

#define ADC_PAL_MAX_CONVS_IN_GROUP_HW_TRIG (BCTU_LISTCHR__COUNT * 2u)  /*!< Maximum number of ADC conversions in a hw triggered group of conversions - each LISTCHR reg contains 2 list elements */

static status_t ADC_Init_MPC574xC_G_R(const uint32_t instIdx,
                                      const adc_config_t * const config);

static void ADC_ConfigHwTriggeredGroup(const uint32_t instIdx,
                                       const adc_group_config_t * const groupConfig,
                                       const uint8_t bctuStartListIdx);

static uint32_t ADC_GetGroupIdx(const uint32_t instIdx,
                                const adc_trigger_source_t hwTrigSource);

#elif defined (ADC_PAL_SAR_CTU)

/* For ADC_PAL_SAR_CTU, ADC PAL instances are mapped 1:1 with CTU instances.
 * Each CTU instance may have multiple ADCs connected. */
#define NUMBER_OF_ADC_PAL_INSTANCES CTU_INSTANCE_COUNT

/* Table of base addresses for CTU instances. */
static CTU_Type * const ctuBase[CTU_INSTANCE_COUNT] = CTU_BASE_PTRS;

static status_t ADC_SAR_CTU_Init(const uint32_t instIdx,
                                 const adc_config_t * const config);

#endif /* defined(ADC_PAL_SAR_CTU) */


#if (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

static const IRQn_Type adcEocIrqNums[ADC_INSTANCE_COUNT] = {
    ADC0_EOC_IRQn,
#if ADC_INSTANCE_COUNT > 1u
    ADC1_EOC_IRQn,
#endif
#if ADC_INSTANCE_COUNT > 2u
    ADC2_EOC_IRQn,
#endif
#if ADC_INSTANCE_COUNT > 3u
    ADC3_EOC_IRQn
#endif
};

/* Table of base addresses for ADC instances. */
static ADC_Type * const adcBase[ADC_INSTANCE_COUNT] = ADC_BASE_PTRS;

static inline uint8_t ADC_GetChanIdx(const adc_input_chan_t adcPalInputChan);

static inline uint8_t ADC_GetAdcAbsoluteIdx(const adc_input_chan_t adcPalInputChan);

static uint16_t ADC_GetUsedAdcInstances(const uint32_t instance);

static status_t ADC_ConfigAdcMPC574x(const uint32_t instIdx,
                                     const adc_config_t * const config);

static void ADC_ResetAdcMPC574x(const uint32_t instIdx);

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void ADC_CheckChanDuplicate(const adc_config_t * const config);

#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */


#if (defined (ADC_PAL_S32K1xx) || defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

static inline adc_group_state_t * ADC_GetHwGroupStatePtr(const uint32_t instIdx,
                                                         const uint32_t groupIdx);

static bool ADC_AnyHwTrigGroupActive(const uint32_t instIdx);

static void ADC_PalStateUpdateStart(const uint32_t instIdx,
                                    const uint32_t groupIdx);

static void ADC_CallNotificationCb(const adc_pal_state_t * const palState,
                                   const uint32_t groupIdx,
                                   const adc_group_state_t * const groupState);

#endif /* (defined(ADC_PAL_S32K1xx) || defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */


/*! @endcond */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Static variable to store the PAL state information */
static adc_pal_state_t adcPalState[NUMBER_OF_ADC_PAL_INSTANCES];

/*******************************************************************************
 * Public Functions
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_Init
 * Description   : This function initializes the ADC PAL instance, including the
 * other platform specific HW units used together with ADC.
 *
 * Implements : ADC_Init_Activity
 * END**************************************************************************/
status_t ADC_Init(const adc_instance_t * const instance,
                  const adc_config_t * const config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(config->extension != NULL);
    DEV_ASSERT(config->groupConfigArray != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    status_t status = STATUS_SUCCESS;
    uint8_t idx = 0u;

    /* Initialize PAL state structure members common for all platforms */
    palState->groupArray     = config->groupConfigArray;
    palState->numGroups      = config->numGroups;
    for (idx = 0; idx < ADC_PAL_MAX_NUM_HW_GROUPS_EN; idx++)
    {
        palState->hwTrigGroupState[idx].active              = false;
        palState->hwTrigGroupState[idx].bufferLength        = 0u;
        palState->hwTrigGroupState[idx].currentBufferOffset = 0u;
        palState->hwTrigGroupState[idx].notificationEn      = false;
    }

    palState->swTrigGroupState.active               = false;
    palState->swTrigGroupState.bufferLength         = 0u;
    palState->swTrigGroupState.currentBufferOffset  = 0u;
    palState->swTrigGroupState.notificationEn       = false;
    palState->latestGroupIdx    = 0u;

#if defined (ADC_PAL_S32K1xx)

    /* Initialize state members specific for this platform */
    const extension_adc_s32k1xx_t * const extension = (extension_adc_s32k1xx_t *)(config->extension);
    palState->pdbPrescaler = extension->pdbPrescaler;

    status = ADC_Init_S32K1xx(instIdx, config);

#elif (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

    /* Initialize state members specific for this platform */
    uint8_t hwTrigGroupIdx = 0;
    for (idx = 0; idx < palState->numGroups; idx++)
    {
        if (palState->groupArray[idx].hwTriggerSupport == true)
        {
            /* For hwTrigGroups, stateIdxMapping holds the index in hwTrigGroupState array */
            palState->stateIdxMapping[idx] = hwTrigGroupIdx;
            hwTrigGroupIdx++;
        }
        else
        {
            /* A single SW triggered group active is supported,
             * so for swTrigGroups, stateIdxMapping holds a value which is not used. */
            palState->stateIdxMapping[idx] = 0u;
        }
    }

#if defined (ADC_PAL_MPC574xC_G_R)

    status = ADC_Init_MPC574xC_G_R(instIdx, config);

#else

    status = ADC_SAR_CTU_Init(instIdx, config);

#endif /* defined(ADC_PAL_MPC574xC_G_R) */

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_Deinit
 * Description   : This function resets the ADC PAL instance, including
 * the other platform specific HW units used together with ADC.
 *
 * Implements : ADC_Deinit_Activity
 * END**************************************************************************/
status_t ADC_Deinit(const adc_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    bool hwTrigGroupActive           = false;
    status_t status;
    uint16_t idx;

    hwTrigGroupActive = ADC_AnyHwTrigGroupActive(instIdx);

    if ((palState->swTrigGroupState.active == true) || (hwTrigGroupActive == true))
    {
        status = STATUS_BUSY;
    }
    else
    {
#if defined (ADC_PAL_S32K1xx)

        /* No state members specific for this platform to reset */

        PDB_DRV_Deinit(instIdx);

        status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instIdx]);
        DEV_ASSERT(status == STATUS_SUCCESS);

        ADC_DRV_Reset(instIdx);

        /* Disable interrupt from INT manager */
        IRQn_Type adcIrqId;
        adcIrqId = ADC_DRV_GetInterruptNumber(instIdx);
        INT_SYS_DisableIRQ(adcIrqId);

        status = STATUS_SUCCESS;

#elif defined (ADC_PAL_MPC574xC_G_R)

        /* Reset state members specific for this platform */
        for (idx = 0; idx < palState->numGroups; idx++)
        {
            palState->stateIdxMapping[idx] = 0u;
        }

        /* Deinit BCTU and ADCs */
        status_t bctuStatus;
        bctuStatus = BCTU_DRV_Reset(instIdx, /* default timeout value 2ms*/ 2u);
        if (bctuStatus == STATUS_TIMEOUT)
        {
            status = STATUS_BUSY;
        }
        else
        {
            ADC_ResetAdcMPC574x(instIdx);

            status = STATUS_SUCCESS;
        }

#elif defined (ADC_PAL_SAR_CTU)

        /* Reset state members specific for this platform */
        for (idx = 0; idx < palState->numGroups; idx++)
        {
            palState->stateIdxMapping[idx] = 0u;
        }

        CTU_DRV_Reset(instIdx);

        ADC_ResetAdcMPC574x(instIdx);

        status = STATUS_SUCCESS;

#endif /* defined(ADC_PAL_SAR_CTU) */

        /* Reset PAL state structure members common for all platforms */
        palState->groupArray = NULL;
        palState->numGroups  = 0u;
        for (idx = 0; idx < ADC_PAL_MAX_NUM_HW_GROUPS_EN; idx++)
        {
            palState->hwTrigGroupState[idx].active = false;
        }

        palState->swTrigGroupState.active = false;
        palState->latestGroupIdx          = 0u;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_EnableHardwareTrigger
 * Description   : Enables the selected HW trigger for a conversion group,
 * if the conversion group has support for HW trigger.
 *
 * Implements : ADC_EnableHardwareTrigger_Activity
 * END**************************************************************************/
status_t ADC_EnableHardwareTrigger(const adc_instance_t * const instance,
                                   const uint32_t groupIdx)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instIdx].numGroups);
    DEV_ASSERT(adcPalState[instIdx].groupArray != NULL);

    const adc_pal_state_t * const palState            = &(adcPalState[instIdx]);
    const adc_group_config_t * const currentGroupCfg  = &(palState->groupArray[groupIdx]);
    const adc_group_state_t * const currentGroupState = ADC_GetHwGroupStatePtr(instIdx, groupIdx);
    status_t status       = STATUS_SUCCESS;
    status_t statusTrgmux = STATUS_SUCCESS;
    (void)statusTrgmux;

    DEV_ASSERT(currentGroupCfg->hwTriggerSupport == true);
    (void)currentGroupCfg;

    if ((palState->swTrigGroupState.active == true) || (currentGroupState->active == true))
    {
        /* A conversion group is already active */
        status = STATUS_BUSY;
    }
    else
    {
        /* Update ADC PAL and group state structures */
        ADC_PalStateUpdateStart(instIdx, groupIdx);

#if defined (ADC_PAL_S32K1xx)

        /* Enable the ADC interrupt from Interrupt Manager */
        IRQn_Type adcIrqId;
        adcIrqId = ADC_DRV_GetInterruptNumber(instIdx);
        INT_SYS_EnableIRQ(adcIrqId);

        /* The group shall be configured each time it is enabled. */
        ADC_ConfigGroup(instIdx, groupIdx, true);

        /* Enable in TRGMUX the selected HW trigger source for PDB.
         * Must be called after all PDB pre-triggers have been configured, to make sure no triggers occur during configuration. */
        statusTrgmux = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, currentGroupCfg->triggerSource, adcPalTrgmuxTarget[instIdx]);
        DEV_ASSERT(statusTrgmux == STATUS_SUCCESS);

#elif defined (ADC_PAL_MPC574xC_G_R)
        const uint8_t numChans = (palState->groupArray[groupIdx]).numChannels;

        if (numChans == 1u)
        {
            INT_SYS_EnableIRQ(BCTU_ConvUpdate_IRQn);
        }
        else
        {
            INT_SYS_EnableIRQ(BCTU_ListLast_IRQn);
        }

        /* De-activate low-power mode. Any pending triggers will become active, immediately triggering a conversion. */
        BCTU_DRV_SetLowPowerMode(instIdx, false);

        /* All groups get configured at ADC PAL initialization,
         * so here they only need to be enabled. */
        BCTU_DRV_EnableHwTrigger(instIdx, currentGroupCfg->triggerSource);

#elif defined (ADC_PAL_SAR_CTU)
        /* internalTrigIdx is mapped 1:1 with stateIdx */
        const uint32_t internalTrigIdx = palState->stateIdxMapping[groupIdx];
        const uint32_t shift           = internalTrigIdx * CTU_THCR1_T1_ADCE_SHIFT;
        CTU_Type * const ctuInstBase   = ctuBase[instIdx];

        /* Enable internal trigger corresponding to current groupIdx */
        ctuInstBase->THCR1 |= (uint32_t)CTU_OUTPUT_TRIG_EN << shift; /* Assumes: internalTrigIdx < 4 */

        /* THCR is double-buffered, so mark that MRS can reload the value */
        CTU_DRV_EnableGeneralReload(instIdx);

#endif /* defined(ADC_PAL_SAR_CTU) */

        status = STATUS_SUCCESS;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_DisableHardwareTrigger
 * Description   : Disables the selected HW trigger for a conversion group,
 * if the conversion group has support for HW trigger.
 *
 * Implements : ADC_DisableHardwareTrigger_Activity
 * END**************************************************************************/
status_t ADC_DisableHardwareTrigger(const adc_instance_t * const instance,
                                    const uint32_t groupIdx,
                                    const uint32_t timeout)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    adc_group_state_t * const currentGroupState = ADC_GetHwGroupStatePtr(instIdx, groupIdx);

    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instIdx].numGroups);
    DEV_ASSERT(adcPalState[instIdx].groupArray != NULL);
    DEV_ASSERT(adcPalState[instIdx].groupArray[groupIdx].hwTriggerSupport == true);

    status_t status = STATUS_SUCCESS;

    /* Make sure no more notifications are received from current groupIdx */
    status = ADC_DisableNotification(instance, groupIdx);

    if ((currentGroupState->active == true) && (status == STATUS_SUCCESS))
    {
#if defined (ADC_PAL_S32K1xx)
        /* Disable from TRGMUX the trigger source for PDB */
        status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instIdx]);
        DEV_ASSERT(status == STATUS_SUCCESS);

        status = ADC_StopGroupBlocking(instIdx, timeout);

#elif defined (ADC_PAL_MPC574xC_G_R)
        const adc_pal_state_t * const palState   = &(adcPalState[instIdx]);
        const adc_trigger_source_t triggerSource = palState->groupArray[groupIdx].triggerSource;
        const uint8_t numChans                   = palState->groupArray[groupIdx].numChannels;
        uint32_t startTime, deltaTime;

        if (numChans == 1u)
        {
            INT_SYS_DisableIRQ(BCTU_ConvUpdate_IRQn);
        }
        else
        {
            INT_SYS_DisableIRQ(BCTU_ListLast_IRQn);
        }

        OSIF_TimeDelay(0u); /* Make sure OSIF timer is initialized. */

        startTime = OSIF_GetMilliseconds();
        deltaTime = 0u;

        /* Prevent new conversions from starting */
        BCTU_DRV_DisableHwTrigger(instIdx, triggerSource);

        /* Check if conversion is still running until execution finished or timeout occurred */
        while ((BCTU_DRV_IsConvRunning(instIdx, triggerSource) == true) && (deltaTime < timeout))
        {
            deltaTime = OSIF_GetMilliseconds() - startTime;
        }

        if (deltaTime >= timeout)
        {
            status = STATUS_TIMEOUT;
        }
        else
        {
            status = STATUS_SUCCESS;

            /* Activate low-power mode */
            BCTU_DRV_SetLowPowerMode(instIdx, true);
        }

#elif defined (ADC_PAL_SAR_CTU)
        const adc_pal_state_t * const palState = &(adcPalState[instIdx]);
        /* internalTrigIdx is mapped 1:1 with stateIdx */
        const uint32_t internalTrigIdx = palState->stateIdxMapping[groupIdx];
        const uint32_t shift           = internalTrigIdx * CTU_THCR1_T1_ADCE_SHIFT;
        CTU_Type * const ctuInstBase   = ctuBase[instIdx];

        (void)timeout;

        /* Disable internal trigger corresponding to current groupIdx */
        /* Register is double-buffered so the new value will not have effect in the current control cycle
         * - only after a new MRS occurs */
        ctuInstBase->THCR1 &= ~((uint32_t)CTU_OUTPUT_TRIG_EN << shift); /* Assumes: fifoIdx < 4 */

        /* THCR is double-buffered, so mark that MRS can reload the value. */
        CTU_DRV_EnableGeneralReload(instIdx);

        currentGroupState->active = false;

#endif /* defined(ADC_PAL_SAR_CTU) */

#ifndef ADC_PAL_SAR_CTU

        if (status == STATUS_SUCCESS)
        {
            currentGroupState->active = false;
        }

#endif /* ADC_PAL_SAR_CTU */
    }


    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StartGroupConversion
 * Description   : Starts the execution of a selected ADC conversion group.
 *
 * Implements : ADC_StartGroupConversion_Activity
 * END**************************************************************************/
status_t ADC_StartGroupConversion(const adc_instance_t * const instance,
                                  const uint32_t groupIdx)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);

    const adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    DEV_ASSERT(groupIdx < palState->numGroups);
    DEV_ASSERT(palState->groupArray != NULL);
    DEV_ASSERT(palState->groupArray[groupIdx].hwTriggerSupport == false);

    status_t status;

    if ((ADC_AnyHwTrigGroupActive(instIdx) == true) || (palState->swTrigGroupState.active == true))
    {
        /* Any other conversion group is already active */
        status = STATUS_BUSY;
    }
    else
    {
        /* Update ADC PAL and group state structures */
        ADC_PalStateUpdateStart(instIdx, groupIdx);

#if defined (ADC_PAL_S32K1xx)
        DEV_ASSERT(palState->groupArray[groupIdx].numChannels <= ADC_PAL_MAX_CONVS_IN_GROUP);

        bool hwTriggerEnabled = false;

        /* Enable the ADC interrupt from Interrupt Manager */
        IRQn_Type adcIrqId;
        adcIrqId = ADC_DRV_GetInterruptNumber(instIdx);
        INT_SYS_EnableIRQ(adcIrqId);

        ADC_ConfigGroup(instIdx, groupIdx, hwTriggerEnabled);

        /* Sw trigger PDB */
        PDB_DRV_SoftTriggerCmd(instIdx);

#elif (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

        const adc_group_config_t * currentGroupCfg  = &(palState->groupArray[groupIdx]);
        uint8_t idx, adcIdx;
        uint32_t currentChan;

        /* Assumes that all channels in the group target the same ADC instance */
        adcIdx = ADC_GetAdcAbsoluteIdx(currentGroupCfg->inputChannelArray[0u]);

#if defined (ADC_PAL_SAR_CTU)
        adcBase[adcIdx]->MCR &= ~(ADC_MCR_CTUEN(1u)); /* set ADC to CTU MODE DISABLED, to allow SW triggering */
#endif /* defined(ADC_PAL_SAR_CTU) */

        INT_SYS_EnableIRQ(adcEocIrqNums[adcIdx]);

        for (idx = 0u; idx < currentGroupCfg->numChannels; idx++)
        {
            DEV_ASSERT(adcIdx == ADC_GetAdcAbsoluteIdx(currentGroupCfg->inputChannelArray[idx])); /* channels in the group must target the same ADC instance */
            currentChan = ADC_GetChanIdx(currentGroupCfg->inputChannelArray[idx]);

            ADC_DRV_EnableChannel(adcIdx, ADC_CONV_CHAIN_NORMAL, currentChan);
        }

        ADC_DRV_StartConversion(adcIdx, ADC_CONV_CHAIN_NORMAL);

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */

        status = STATUS_SUCCESS;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StopGroupConversion
 * Description   : Stops the execution of the ADC group currently executing.
 *
 * Implements : ADC_StopGroupConversion_Activity
 * END**************************************************************************/
status_t ADC_StopGroupConversion(const adc_instance_t * const instance,
                                 const uint32_t groupIdx,
                                 const uint32_t timeout)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instIdx].numGroups);
    DEV_ASSERT(adcPalState[instIdx].groupArray != NULL);
    DEV_ASSERT(adcPalState[instIdx].groupArray[groupIdx].hwTriggerSupport == false);

    status_t status = STATUS_SUCCESS;

    if (adcPalState[instIdx].swTrigGroupState.active == true)
    {
#if defined (ADC_PAL_S32K1xx)
        (void)groupIdx;

        /* Disable the ADC interrupt from Interrupt Manager */
        IRQn_Type adcIrqId;
        adcIrqId = ADC_DRV_GetInterruptNumber(instIdx);
        INT_SYS_DisableIRQ(adcIrqId);

        status = ADC_StopGroupBlocking(instIdx, timeout);

#elif (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

        const adc_group_config_t * currentGroupCfg  = &(adcPalState[instIdx].groupArray[groupIdx]);
        uint8_t idx, adcIdx;
        uint32_t currentChan;
        uint32_t startTime, deltaTime;

        OSIF_TimeDelay(0u); /* Make sure OSIF timer is initialized. */

        startTime = OSIF_GetMilliseconds();
        deltaTime = 0u;

        /* Assumes that all channels in the group target the same ADC */
        adcIdx = ADC_GetAdcAbsoluteIdx(currentGroupCfg->inputChannelArray[0u]);

        INT_SYS_DisableIRQ(adcEocIrqNums[adcIdx]);

        /* Disable channels from current conversion group */
        for (idx = 0u; idx < currentGroupCfg->numChannels; idx++)
        {
            currentChan = ADC_GetChanIdx(currentGroupCfg->inputChannelArray[idx]);

            ADC_DRV_DisableChannel(adcIdx, ADC_CONV_CHAIN_NORMAL, currentChan);
        }

        /* Check if conversion is still running, until execution finished or timeout occurred */
        while (((ADC_DRV_GetStatusFlags(adcIdx) & ADC_FLAG_NORMAL_STARTED) != 0u) && (deltaTime < timeout))
        {
            deltaTime = OSIF_GetMilliseconds() - startTime;
        }

        /* Clear status & pending flags corresponding to a conversion group
         * which may have finished after interrupts have been disabled */
        ADC_DRV_ClearStatusFlags(adcIdx, ADC_FLAG_NORMAL_ENDCHAIN);
        for (idx = 0u; idx < ADC_CEOCFR_COUNT; idx++)
        {
            adcBase[adcIdx]->CEOCFR[idx] |= 0xFFFFFFFFu;
        }

        if (deltaTime >= timeout)
        {
            status = STATUS_TIMEOUT;
        }
        else
        {
#if defined (ADC_PAL_SAR_CTU)
            adcBase[adcIdx]->MCR |= ADC_MCR_CTUEN(1u); /* set ADC to CTU CONTROL MODE, to allow HW triggering */
#endif /* defined(ADC_PAL_SAR_CTU) */

            status = STATUS_SUCCESS;
        }

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */

        if (status == STATUS_SUCCESS)
        {
            adcPalState[instIdx].swTrigGroupState.active = false;
        }
    }
    else
    {
        status = STATUS_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_EnableNotification
 * Description   : Enables the notification callback for a configured group.
 *
 * Implements : ADC_EnableNotification_Activity
 * END**************************************************************************/
status_t ADC_EnableNotification(const adc_instance_t * const instance,
                                const uint32_t groupIdx)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * const palState            = &(adcPalState[instIdx]);
    const adc_group_config_t * currentGroupCfg  = &(adcPalState[instIdx].groupArray[groupIdx]);
    adc_group_state_t * groupState;

    DEV_ASSERT(groupIdx < palState->numGroups);

    status_t status = STATUS_SUCCESS;

    if (currentGroupCfg->hwTriggerSupport == true)
    {
        groupState = ADC_GetHwGroupStatePtr(instIdx, groupIdx);
    }
    else
    {
        groupState = &(palState->swTrigGroupState);
    }

    if (groupState->active == false)
    {
        status = STATUS_ERROR;
    }
    else
    {
#if (defined (ADC_PAL_S32K1xx) || defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

        DEV_ASSERT(palState->groupArray[groupIdx].callback != NULL);

        groupState->notificationEn = true;

#endif /* (defined(ADC_PAL_S32K1xx) || defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_DisableNotification
 * Description   : Disables the notification callback for a configured group.
 *
 * Implements : ADC_DisableNotification_Activity
 * END**************************************************************************/
status_t ADC_DisableNotification(const adc_instance_t * const instance,
                                 const uint32_t groupIdx)
{
    DEV_ASSERT(instance != NULL);

    uint32_t instIdx = instance->instIdx;
    DEV_ASSERT(instIdx < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * const palState            = &(adcPalState[instIdx]);
    const adc_group_config_t * currentGroupCfg  = &(adcPalState[instIdx].groupArray[groupIdx]);
    adc_group_state_t * groupState;

    DEV_ASSERT(groupIdx < palState->numGroups);

    status_t status = STATUS_SUCCESS;

    if (currentGroupCfg->hwTriggerSupport == true)
    {
        groupState = ADC_GetHwGroupStatePtr(instIdx, groupIdx);
    }
    else
    {
        groupState = &(palState->swTrigGroupState);
    }

    if (groupState->active == false)
    {
        status = STATUS_ERROR;
    }
    else
    {
#if (defined (ADC_PAL_S32K1xx) || defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

        groupState->notificationEn = false;

#endif /* (defined(ADC_PAL_S32K1xx) || defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */
    }

    return status;
}


/*******************************************************************************
 * Private Functions
 ******************************************************************************/

#if defined (ADC_PAL_S32K1xx)

static status_t ADC_Init_S32K1xx(const uint32_t instance,
                                 const adc_config_t * const config)
{
    const extension_adc_s32k1xx_t * const extension = (extension_adc_s32k1xx_t *)(config->extension);
    adc_converter_config_t adcCfg;
    status_t status;

    DEV_ASSERT(extension->inputClock <= NUMBER_OF_ALT_CLOCKS);

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    {
        uint16_t idx;
        uint16_t idx0, idx1;
        const adc_group_config_t * currentGroupCfg;

        /* Iterate over all conversion groups */
        for (idx = 0u; idx < adcPalState[instance].numGroups; idx++)
        {
            currentGroupCfg = &(adcPalState[instance].groupArray[idx]);
            /* Error if number of conversions is larger than max supported */
            DEV_ASSERT(currentGroupCfg->numChannels <= ADC_PAL_MAX_CONVS_IN_GROUP);

            if (currentGroupCfg->delayType == ADC_DELAY_TYPE_INDIVIDUAL_DELAY)
            {
                /* Delay values are measured relative to the trigger event.
                 * The pretriggers must not occur while another conversion in the group is running, otherwise the ADC freezes.
                 * This code only checks that the values are not identical, but it is the users' responsibility
                 * to make sure they do not overlap, i.e. delayN_plus_1 > (delayN + conversion_duration) */
                for (idx0 = 0u; idx0 < currentGroupCfg->numChannels; idx0++)
                {
                    for (idx1 = idx0 + 1u; idx1 < currentGroupCfg->numChannels; idx1++)
                    {
                        DEV_ASSERT(currentGroupCfg->delayArray[idx0] != currentGroupCfg->delayArray[idx1]);
                    }
                }
            }
        }
    }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    /* ADC configuration */
    ADC_DRV_Reset(instance);

    ADC_DRV_InitConverterStruct(&adcCfg);
    adcCfg.trigger                = ADC_TRIGGER_HARDWARE; /* ADC in hw trigger mode - sw triggering shall be done via PDB */
    adcCfg.sampleTime             = config->sampleTicks;
    adcCfg.clockDivide            = extension->clockDivide;
    adcCfg.resolution             = extension->resolution;
    adcCfg.inputClock             = extension->inputClock;
    adcCfg.voltageRef             = extension->voltageRef;
    adcCfg.supplyMonitoringEnable = extension->supplyMonitoringEnable;
    adcCfg.pretriggerSel          = ADC_PRETRIGGER_SEL_PDB; /* configure pretriggers 0->3 to be routed from PDB */

    ADC_DRV_ConfigConverter(instance, &adcCfg);

    ADC_DRV_AutoCalibration(instance);

    /* PDB init shall only be called from StartConversion() & EnableHardwareTrigger()
     * because PDB input source and continuous conversion enable need to be configured for each call. */

    /* Only reset the trgmux target register corresponding to PDB instance.
     * Calling TRGMUX init would reset all TRGMUX target registers - affecting other modules. */
    status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instance]);
    if (status != STATUS_SUCCESS)
    {
        status = STATUS_ERROR;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_S32K1xx_IrqHandler
 * Description   : Interrupt handler functionality to be called from
 * interrupt handlers for each ADC.
 * Implements : ADC_Interrupt_Activity
 * END**************************************************************************/
void ADC_S32K1xx_IrqHandler(const uint32_t instIdx)
{
    adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    const uint32_t currentGroupIdx   = palState->latestGroupIdx;
    uint8_t controlChanIdx           = 0u;
    const adc_group_config_t * activeGroupCfg;
    adc_group_state_t * groupState;

    activeGroupCfg = &(palState->groupArray[currentGroupIdx]);

    if (activeGroupCfg->hwTriggerSupport == false)
    {
        groupState = &(palState->swTrigGroupState);
    }
    else
    {
        groupState = &(palState->hwTrigGroupState[0u]); /* A single HW trigger enabled is supported by this platform */
    }

    uint16_t * result = &(activeGroupCfg->resultBuffer[groupState->currentBufferOffset]);

    /* Read all conversion results */
    for (controlChanIdx = 0u; controlChanIdx < activeGroupCfg->numChannels; controlChanIdx++)
    {
        ADC_DRV_GetChanResult(instIdx, controlChanIdx, result); /* interrupt flag is cleared when reading the result */
        result++;
    }

    /* Increment offset in result buffer */
    groupState->currentBufferOffset = (uint16_t)((groupState->currentBufferOffset + activeGroupCfg->numChannels) % groupState->bufferLength);


    if (activeGroupCfg->hwTriggerSupport == false) /* Continuous mode currently supported only for SW triggered groups */
    {
        if (activeGroupCfg->continuousConvEn == true)
        {
            /* Sw trigger PDB */
            PDB_DRV_SoftTriggerCmd(instIdx);
        }
        else
        {
            groupState->active = false;
        }
    }

    /* Call notification callback, if it is enabled */
    ADC_CallNotificationCb(palState, currentGroupIdx, groupState);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ConfigPdbAndPretriggers
 * Description   : Configures PDB global parameters, PDB pretriggers and delays (if used)
 *
 * END**************************************************************************/
static inline void ADC_ConfigPdbAndPretriggers(const uint32_t instIdx,
                                               const pdb_trigger_src_t trgSrc,
                                               const adc_group_config_t * currentGroupCfg)
{
    pdb_timer_config_t pdbCfg;
    pdb_adc_pretrigger_config_t pdbPretrigCfg;
    uint8_t idx;

    /* PDB driver instance configuration */
    pdbCfg.loadValueMode        = PDB_LOAD_VAL_IMMEDIATELY;
    pdbCfg.seqErrIntEnable      = false;
    pdbCfg.clkPreDiv            = adcPalState[instIdx].pdbPrescaler;
    pdbCfg.clkPreMultFactor     = PDB_CLK_PREMULT_FACT_AS_1;
    pdbCfg.dmaEnable            = false;
    pdbCfg.intEnable            = false;
    pdbCfg.continuousModeEnable = false; /* Continuous mode refers to Counter being reset at zero - not used in ADC PAL */
    pdbCfg.triggerInput         = trgSrc;
#if FEATURE_PDB_HAS_INSTANCE_BACKTOBACK
    pdbCfg.instanceBackToBackEnable     = false;
#endif /* FEATURE_PDB_HAS_INSTANCE_BACKTOBACK */
#if FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK
    pdbCfg.interchannelBackToBackEnable = false;
#endif /* FEATURE_PDB_HAS_INTERCHANNEL_BACKTOBACK */
	
    PDB_DRV_Init(instIdx, &pdbCfg);

    if (currentGroupCfg->delayType == ADC_DELAY_TYPE_NO_DELAY)
    {
        /* PDB pre-triggers configuration */
        pdbPretrigCfg.preTriggerEnable           = true;
        pdbPretrigCfg.preTriggerOutputEnable     = false; /* pretrigger asserts one clock cycle after input trigger is asserted (hw or sw)  */
        pdbPretrigCfg.preTriggerBackToBackEnable = false; /* the first pretrigger in the group must have BB disabled */
        pdbPretrigCfg.adcPreTriggerIdx           = 0u;
        PDB_DRV_ConfigAdcPreTrigger(instIdx, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);

        pdbPretrigCfg.preTriggerBackToBackEnable = true; /* the rest of pretriggers in the group must have BB enabled */
        for (idx = 1u; idx < currentGroupCfg->numChannels; idx++)
        {
            pdbPretrigCfg.adcPreTriggerIdx = idx;
            PDB_DRV_ConfigAdcPreTrigger(instIdx, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);
        }
    }
    else if (currentGroupCfg->delayType == ADC_DELAY_TYPE_GROUP_DELAY)
    {
        DEV_ASSERT(currentGroupCfg->delayArray != NULL);

        /* PDB pre-triggers configuration */
        pdbPretrigCfg.preTriggerEnable           = true;
        pdbPretrigCfg.preTriggerOutputEnable     = true;  /* the first pretrigger asserts when counter reaches delay value plus one */
        pdbPretrigCfg.preTriggerBackToBackEnable = false; /* the first pretrigger in the group must have BB disabled */
        pdbPretrigCfg.adcPreTriggerIdx           = 0u;
        PDB_DRV_ConfigAdcPreTrigger(instIdx, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);

        PDB_DRV_SetAdcPreTriggerDelayValue(instIdx, ADC_PAL_PDB_CHAN, pdbPretrigCfg.adcPreTriggerIdx, *(currentGroupCfg->delayArray));

        pdbPretrigCfg.preTriggerOutputEnable     = false; /* the rest of pretriggers in the group must ignore the delay value */
        pdbPretrigCfg.preTriggerBackToBackEnable = true;  /* the rest of pretriggers in the group must have BB enabled */
        for (idx = 1u; idx < currentGroupCfg->numChannels; idx++)
        {
            pdbPretrigCfg.adcPreTriggerIdx = idx;
            PDB_DRV_ConfigAdcPreTrigger(instIdx, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);
        }
    }
    else /* corresponds to currentGroupCfg->delayType == ADC_DELAY_TYPE_INDIVIDUAL_DELAY */
    {
        DEV_ASSERT(currentGroupCfg->delayArray != NULL);

        /* PDB pre-triggers configuration */
        pdbPretrigCfg.preTriggerEnable           = true;
        pdbPretrigCfg.preTriggerOutputEnable     = true;  /* each pretrigger asserts when counter reaches corresponding delay value plus one */
        pdbPretrigCfg.preTriggerBackToBackEnable = false; /* all pretriggers in the group must have BB disabled */
        for (idx = 0u; idx < currentGroupCfg->numChannels; idx++)
        {
            pdbPretrigCfg.adcPreTriggerIdx = idx;
            PDB_DRV_ConfigAdcPreTrigger(instIdx, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);

            PDB_DRV_SetAdcPreTriggerDelayValue(instIdx, ADC_PAL_PDB_CHAN, pdbPretrigCfg.adcPreTriggerIdx, currentGroupCfg->delayArray[idx]);
        }
    }

    PDB_DRV_Enable(instIdx);
    PDB_DRV_LoadValuesCmd(instIdx);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ConfigGroup
 * Description   : Configures ADC input channels and PDB for a conversion group
 *
 * END**************************************************************************/
static void ADC_ConfigGroup(const uint32_t instance,
                            const uint32_t groupIdx,
                            const bool hwTriggerFlag)
{
    const adc_group_config_t * const currentGroupCfg  = &(adcPalState[instance].groupArray[groupIdx]);

    pdb_trigger_src_t pdbTrigSrc;
    if (hwTriggerFlag == true)
    {
        pdbTrigSrc = PDB_TRIGGER_IN0;
    }
    else
    {
        pdbTrigSrc = PDB_SOFTWARE_TRIGGER;
        /* SW triggered groups do not support delays */
        DEV_ASSERT(currentGroupCfg->delayType == ADC_DELAY_TYPE_NO_DELAY);
    }

    /* Configure PDB instance and pre-triggers */
    ADC_ConfigPdbAndPretriggers(instance, pdbTrigSrc, currentGroupCfg);

    /* Configure ADC channels */
    adc_chan_config_t adcChanCfg;
    uint8_t idx;

    adcChanCfg.interruptEnable = false; /* interrupt is disabled for all conversion, except the last one in the group */
    for (idx = 0u; idx < (currentGroupCfg->numChannels - 1u); idx++)
    {
        adcChanCfg.channel = currentGroupCfg->inputChannelArray[idx]; /* set the ADC input channel */

        ADC_DRV_ConfigChan(instance, idx, &adcChanCfg); /* conversion complete flag is cleared implicitly when writing a new configuration */
    }

    adcChanCfg.interruptEnable = true; /* enable interrupt for last conversion in the group */
    adcChanCfg.channel         = currentGroupCfg->inputChannelArray[idx]; /* set the ADC input channel */
    ADC_DRV_ConfigChan(instance, idx, &adcChanCfg); /* configure the last conversion in the group */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StopGroupBlocking
 * Description   : Stops a group of conversions within a given timeout interval
 *
 * END**************************************************************************/
static status_t ADC_StopGroupBlocking(const uint32_t instance,
                                      const uint32_t timeout)
{
    const adc_pal_state_t * const palState = &(adcPalState[instance]);
    uint32_t startTime, deltaTime;
    status_t status;
    pdb_adc_pretrigger_config_t pdbPretrigCfg;
    uint8_t idx;

    /* Make sure OSIF timer is initialized. */
    OSIF_TimeDelay(0u);

    startTime = OSIF_GetMilliseconds();
    deltaTime = 0u;

    /* Reset PDB pre-trigger configurations to stop PDB from triggering other conversions in the group */
    pdbPretrigCfg.preTriggerEnable           = false;
    pdbPretrigCfg.preTriggerOutputEnable     = false;
    pdbPretrigCfg.preTriggerBackToBackEnable = false;
    for (idx = 0u; idx < palState->groupArray[palState->latestGroupIdx].numChannels; idx++)
    {
        pdbPretrigCfg.adcPreTriggerIdx = idx;
        PDB_DRV_ConfigAdcPreTrigger(instance, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);
    }

    /* Completely stop PDB */
    PDB_DRV_Deinit((uint32_t)instance);

    /* Wait for current ADC active conversion to finish execution */
    static ADC_Type * const adcBase[ADC_INSTANCE_COUNT] = ADC_BASE_PTRS;
    const ADC_Type * const base                  = adcBase[instance];
    while ((ADC_GetConvActiveFlag(base) == true) && (deltaTime < timeout))
    {
        deltaTime = OSIF_GetMilliseconds() - startTime;
    }

    if (deltaTime >= timeout)
    {
        status = STATUS_TIMEOUT;
    }
    else
    {
        status = STATUS_SUCCESS;
    }

    return status;
}

#elif defined (ADC_PAL_MPC574xC_G_R)


static status_t ADC_Init_MPC574xC_G_R(const uint32_t instIdx,
                                      const adc_config_t * const config)
{
    const adc_group_config_t * currentGroup;
    bctu_config_t bctuCfg;
    status_t funcStatus = STATUS_SUCCESS;
    status_t periphStatus;
    uint8_t idx;

    periphStatus = ADC_ConfigAdcMPC574x(instIdx, config);
    if (periphStatus != STATUS_SUCCESS)
    {
        funcStatus = STATUS_ERROR;
    }

    if (funcStatus == STATUS_SUCCESS)
    {
        /* Configure BCTU */
        periphStatus = BCTU_DRV_Reset(instIdx, /* default timeout value 2ms*/ 2u);
        if (periphStatus == STATUS_TIMEOUT)
        {
            funcStatus = STATUS_BUSY;
        }
    }

    if (funcStatus == STATUS_SUCCESS)
    {
        BCTU_DRV_GetDefaultConfig(&bctuCfg);
        bctuCfg.lowPowerModeEn   = true; /* Low power mode is enabled by default until a HW triggered group is enabled (only HW triggered groups use BCTU) */
        /* Used for HW triggered groups with a single conversion. By default enable new data interrupt for both ADCs */
        bctuCfg.newDataIntEnMask = (((uint8_t)1u << FEATURE_BCTU_NUM_ADC) - 1u);
        bctuCfg.listIntEn        = true;  /* Used for HW triggered groups with multiple conversions */

        BCTU_DRV_Config(instIdx, &bctuCfg);

        uint8_t bctuNumChansList = 0u; /* total number of channels programmed in BCTU list */

        /* Configure all HW triggered groups in BCTU.
         * SW triggered groups will be configured directly in ADC, each time ADC_StartGroupConversion() is called. */
        for (idx = 0u; idx < config->numGroups; idx++)
        {
            currentGroup = &(config->groupConfigArray[idx]);
            DEV_ASSERT(currentGroup->inputChannelArray != NULL);

            if (currentGroup->hwTriggerSupport == true)
            {
                ADC_ConfigHwTriggeredGroup(instIdx, currentGroup, bctuNumChansList);

                if (currentGroup->numChannels > 1u)
                {
                    bctuNumChansList += currentGroup->numChannels; /* Increment total number of channels programmed into BCTU LIST */
                }
            }
        }

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
        {
            ADC_CheckChanDuplicate(config);
        }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

        /* Move BCTU out of configuration mode */
        BCTU_DRV_WriteGlobalTriggerEn(instIdx, true);
    }

    return funcStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_MPC574xC_G_R_HwTrigIrqHandler
 * Description   : Interrupt handler functionality for HW triggered
 * conversion groups - to be called from BCTU interrupt handler.
 * Implements : ADC_Interrupt_Activity
 * END**************************************************************************/
void ADC_MPC574xC_G_R_HwTrigIrqHandler(const uint32_t instIdx,
                                       const uint8_t adcIdx)
{
    const adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    const adc_group_config_t * activeGroupCfg;
    adc_group_state_t * groupState;
    uint32_t currentGroupIdx;
    bctu_conv_result_t bctuResult;
    uint16_t * result;

    /* Get trigger idx for which the conv has finished */
    BCTU_DRV_GetConvResult(instIdx, adcIdx, &bctuResult);
    /* Get group idx assigned to the trigger idx */
    currentGroupIdx = ADC_GetGroupIdx(instIdx, bctuResult.triggerIdx);

    groupState = ADC_GetHwGroupStatePtr(instIdx, currentGroupIdx);

    activeGroupCfg = &(palState->groupArray[currentGroupIdx]);

    result = &(activeGroupCfg->resultBuffer[groupState->currentBufferOffset]);

    if (activeGroupCfg->numChannels == 1u)
    {
        *result = bctuResult.adcData;

        /* Increment offset in result buffer */
        groupState->currentBufferOffset = (groupState->currentBufferOffset + 1u) % groupState->bufferLength;

        BCTU_DRV_ClearStatusFlag(instIdx, adcIdx, BCTU_FLAG_NEW_DATA_AVAILABLE);
        DEV_ASSERT(BCTU_DRV_GetStatusFlag(instIdx, adcIdx, BCTU_FLAG_DATA_OVERRUN) == false);
    }
    else
    {
        /* Read all conversion results for the current conversion group */
        uint8_t idx;
        for (idx = 0u; idx < activeGroupCfg->numChannels; idx++)
        {
            *result = ADC_DRV_GetConvResult(adcIdx, ADC_GetChanIdx(activeGroupCfg->inputChannelArray[idx]));
            result++;
        }

        /* Increment offset in result buffer */
        groupState->currentBufferOffset = (groupState->currentBufferOffset + activeGroupCfg->numChannels) % groupState->bufferLength;

        BCTU_DRV_ClearStatusFlag(instIdx, adcIdx, BCTU_FLAG_LIST_LAST_CONV);
        /* Clear because conversion results are not read from BCTU result register */
        BCTU_DRV_ClearStatusFlag(instIdx, adcIdx, BCTU_FLAG_NEW_DATA_AVAILABLE);
        /* Clear because BCTU result register is overwritten */
        BCTU_DRV_ClearStatusFlag(instIdx, adcIdx, BCTU_FLAG_DATA_OVERRUN);
    }

    /* Call notification callback, if it is enabled */
    ADC_CallNotificationCb(palState, currentGroupIdx, groupState);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ConfigHwTriggeredGroup
 * Description   : Configure BCTU for a ADC PAL HW triggered group
 *
 * END**************************************************************************/
static void ADC_ConfigHwTriggeredGroup(const uint32_t instIdx,
                                       const adc_group_config_t * const groupConfig,
                                       const uint8_t bctuStartListIdx)
{
    const adc_input_chan_t * const inputChans = groupConfig->inputChannelArray;
    bctu_trig_config_t bctuTrigConfig;
    status_t bctuStatus;
    const uint8_t numChans   = groupConfig->numChannels;

    DEV_ASSERT(numChans > 0u);
    DEV_ASSERT(groupConfig->delayArray == NULL); /* Delays are not supported */
    DEV_ASSERT(groupConfig->delayType == ADC_DELAY_TYPE_NO_DELAY); /* Delays are not supported */

    bctuTrigConfig.adcTargetMask = (uint8_t)1u << (ADC_GetAdcAbsoluteIdx(inputChans[0]) % FEATURE_BCTU_NUM_ADC);
    bctuTrigConfig.hwTriggersEn  = groupConfig->hwTriggerSupport;
    bctuTrigConfig.loopEn        = groupConfig->continuousConvEn;

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    {
        /* Check that a trigger source is only used by a single HW triggered group. */
        static const BCTU_Type * const bctuBase[BCTU_INSTANCE_COUNT] = BCTU_BASE_PTRS;
        const BCTU_Type * bctu;
        bctu = bctuBase[instIdx];
        if (bctu->TRGCFG_[groupConfig->triggerSource] != 0u) /* Check if the BCTU trigger register has been already written */
        {
            /* A trigger source cannot be assigned to multiple HW triggered groups. */
            DEV_ASSERT(false);
        }
    }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    if (numChans == 1u) /* Configure groups containing a single conversion. */
    {
        bctuStatus = BCTU_DRV_ConfigTriggerSingle(instIdx, groupConfig->triggerSource, &bctuTrigConfig, ADC_GetChanIdx(inputChans[0]));
        DEV_ASSERT(bctuStatus == STATUS_SUCCESS);
        (void)bctuStatus;
    }
    else /* Configure groups containing more than a single conversion. */
    {
        /* Prepare array with adc channel indexes expressed in normal format, accepted by the BCTU driver */
        uint8_t adcChanIdx[ADC_PAL_MAX_CONVS_IN_GROUP_HW_TRIG];
        uint8_t idx;
        for (idx = 0u; idx < numChans; idx++)
        {
            adcChanIdx[idx] = ADC_GetChanIdx(inputChans[idx]);

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
            const uint8_t firstChanIdx = ADC_GetAdcAbsoluteIdx(inputChans[0u]);
            /* Channels in the group must target the same ADC instance */
            DEV_ASSERT(ADC_GetAdcAbsoluteIdx(inputChans[idx]) == firstChanIdx);
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */
        }

        /* For groups which include a minimum of 2 conversions, the total number of conversions within all these groups shall be less than
         * or equal with the number of BCTU LIST HW registers. Otherwise the first groups will be overwritten. */
        if ((bctuStartListIdx + numChans) >= (uint8_t)ADC_PAL_MAX_CONVS_IN_GROUP_HW_TRIG)
        {
            DEV_ASSERT(false);
        }
        else
        {
            bctuStatus = BCTU_DRV_ConfigTriggerList(instIdx, groupConfig->triggerSource, &bctuTrigConfig, bctuStartListIdx);
            DEV_ASSERT(bctuStatus == STATUS_SUCCESS);

            BCTU_DRV_SetConvListArray(instIdx, bctuStartListIdx, adcChanIdx, numChans);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_GetGroupIdx
 * Description   : Returns the ADC conversion group index, for which the HW trigger
 * source has been configured.
 *
 * END**************************************************************************/
static uint32_t ADC_GetGroupIdx(const uint32_t instIdx,
                                const adc_trigger_source_t hwTrigSource)
{
    uint32_t groupIdx = 0u;
    bool found = false;
    const adc_pal_state_t * const palState = &(adcPalState[instIdx]);

    while ((found == false) && (groupIdx < palState->numGroups))
    {
        if ((palState->groupArray[groupIdx].triggerSource == hwTrigSource) && \
            (palState->groupArray[groupIdx].hwTriggerSupport == true))
        {
            found = true;
        }

        groupIdx++;
    }

    groupIdx--; /* compensate for additional groupIdx increment */
    DEV_ASSERT(found == true);

    return groupIdx;
}

#elif defined (ADC_PAL_SAR_CTU)


static status_t ADC_SAR_CTU_Init(const uint32_t instIdx,
                                 const adc_config_t * const config)
{
    const extension_adc_sar_ctu_t * const extension = (extension_adc_sar_ctu_t *)(config->extension);
    const adc_group_config_t * currentGroup;
    ctu_config_t ctuCfg;
    status_t status = STATUS_SUCCESS;
    uint8_t idx;

    static const IRQn_Type ctuFirstFifoIrqNums[CTU_INSTANCE_COUNT] = {
        CTU_0_FIFO0_IRQn,
#if CTU_INSTANCE_COUNT > 1u
        CTU_1_FIFO0_IRQn
#endif
    };

    status = ADC_ConfigAdcMPC574x(instIdx, config);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    /* Configure CTU */
    CTU_DRV_Reset(instIdx);

    uint8_t ctuNumChansList = 0u; /* total number of channels programmed in CTU list */
    uint8_t internalTrigIdx = 0u; /* each HW triggered group has associated a single internal trigger and a single result FIFO */
    uint8_t fifoIdx         = 0u;
    uint16_t delay          = 0u;
    ctu_trig_config_t trigConfig;
    ctu_res_fifo_config_t resFifoConfig;
    uint32_t inputTriggerSelectMask = 0u;

    /* Configure all HW triggered groups in CTU.
     * SW triggered groups will be configured directly in ADC, each time ADC_StartGroupConversion() is called. */
    for (idx = 0u; idx < config->numGroups; idx++)
    {
        currentGroup = &(config->groupConfigArray[idx]);
        DEV_ASSERT(currentGroup->inputChannelArray != NULL);

        if (currentGroup->hwTriggerSupport == true)
        {
            DEV_ASSERT(internalTrigIdx < CTU_FR_COUNT); /* Maximum supported number of HW triggered groups is number of result FIFOs */

            /* The total number of conversions in all HW triggered groups must be <= CTU_CHANNEL_COUNT */
            DEV_ASSERT((ctuNumChansList + currentGroup->numChannels) <= CTU_CHANNEL_COUNT);

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
            {
                /* Length of result FIFOs are mentioned in the ReferenceManual. */
                static const uint8_t resultFifoLengths[CTU_FR_COUNT] = { 16u, 16u, 4u, 4u };

                /* The number of conversions in the current HW triggered group must be <= than
                 * the length of the assigned result FIFO. */
                DEV_ASSERT(currentGroup->numChannels <= resultFifoLengths[internalTrigIdx]);
            }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

            switch (currentGroup->delayType)
            {
                case ADC_DELAY_TYPE_NO_DELAY:
                    delay = 0u;
                    break;
                case ADC_DELAY_TYPE_GROUP_DELAY:
                    DEV_ASSERT(currentGroup->delayArray != NULL);
                    delay = currentGroup->delayArray[0u]; /* single delay value per group */
                    break;
                default:
                    DEV_ASSERT(false); /* Other type not supported */
                    break;
            }

            trigConfig.cmdListStartAdr  = ctuNumChansList;
            trigConfig.compareVal       = delay;
            trigConfig.intEn            = false;
            trigConfig.outputTrigEnMask = CTU_OUTPUT_TRIG_ADC_CMD_EN;
            CTU_DRV_ConfigInternalTrigger(instIdx, internalTrigIdx, &trigConfig);

            /* Input trigger select register is common for all internal triggers (i.e. HW triggered groups) */
            inputTriggerSelectMask      |= currentGroup->triggerSource;

            fifoIdx = internalTrigIdx; /* Each HW triggered group has associated a single internal trigger and a single result FIFO.*/
            resFifoConfig.dmaEn         = false;
            resFifoConfig.fifoIntEnMask = CTU_FIFO_OVERFLOW;
            resFifoConfig.fifoThreshold = (currentGroup->numChannels - 1u); /* -1 because OVERFLOW interrupt gets triggered when the number of available results is above this value */
            CTU_DRV_ConfigResultFifo(instIdx, fifoIdx, &resFifoConfig);

            IRQn_Type fifoIrqNum;
            const uint16_t fifoIrqNumU = ((uint16_t)ctuFirstFifoIrqNums[instIdx] + fifoIdx); /* MISRA workaround */
            fifoIrqNum = (IRQn_Type)fifoIrqNumU;
            INT_SYS_EnableIRQ(fifoIrqNum);

            /* Config ADC command for each conversion in the group */
            uint8_t idx1 = 0u;
            uint8_t pos, adcPort, adcAbsoluteIdx;
            ctu_adc_cmd_t adcCmd;
            adcCmd.convMode   = CTU_CONV_MODE_SINGLE;
            adcCmd.fifoIdx    = fifoIdx;
            adcCmd.intEn      = false;
            adcCmd.selfTestEn = false;

            adcCmd.adcChanA = ADC_GetChanIdx(currentGroup->inputChannelArray[idx1]);
            adcAbsoluteIdx = ADC_GetAdcAbsoluteIdx(currentGroup->inputChannelArray[idx1]);
            adcPort = adcAbsoluteIdx % FEATURE_CTU_NUM_ADC;
            switch (adcPort)
            {
                case 0u:
                    adcCmd.adcPort = CTU_ADC_PORT_A;
                    break;
                case 1u:
                    adcCmd.adcPort = CTU_ADC_PORT_B;
                    break;
                default:
                    DEV_ASSERT(false);
                    break;
            }

            pos = trigConfig.cmdListStartAdr;
            CTU_DRV_ConfigAdcCmdList(instIdx, pos, &adcCmd, 1u); /* Write one command at a time */
            for (idx1 = 1u; idx1 < currentGroup->numChannels; idx1++)
            {
                CTU_Type * const ctuInstBase = ctuBase[instIdx];

                adcCmd.adcChanA = ADC_GetChanIdx(currentGroup->inputChannelArray[idx1]);

                /* For uniformity with SW triggered groups, even if CTU supports the feature,
                 * do not allow multiple ADC instances in the same conversion group. */
                DEV_ASSERT(ADC_GetAdcAbsoluteIdx(currentGroup->inputChannelArray[idx1]) == adcAbsoluteIdx);

                pos = (trigConfig.cmdListStartAdr + idx1) % CTU_CHANNEL_COUNT;
                CTU_DRV_ConfigAdcCmdList(instIdx, pos, &adcCmd, 1u);

                /* Workaround: clear LC bit starting from second iteration,
                 * because each CTU_DRV_ConfigAdcCmdList call marks the end of the list. */
                ctuInstBase->CHANNEL[pos].CLR_A &= (~CTU_CLR_A_LC_MASK);
            }

            internalTrigIdx++; /* Move to next trigger index */
            ctuNumChansList += currentGroup->numChannels; /* Increment total number of channels programmed into CTU LIST */
        }
    }

    CTU_DRV_GetDefaultConfig(&ctuCfg);
    ctuCfg.prescaler            = extension->ctuPrescaler;
    ctuCfg.tgsMode              = CTU_TGS_MODE_TRIGGERED;
    ctuCfg.inputTrigSelectMask  = inputTriggerSelectMask;
    ctuCfg.tgsCounterCompareVal = CTU_TGSCCR_TGSCCV_MASK; /* set to max */
    ctuCfg.tgsCounterReloadVal  = 0u; /* set to minimum */

    CTU_DRV_Config(instIdx, &ctuCfg);

    /* Mark that writes to double-buffered configuration registers have ended. */
    CTU_DRV_EnableGeneralReload(instIdx);

    /* Sw trigger MRS to load all double-buffered registers and start a new CTU control cycle */
    CTU_DRV_SwMasterReloadSignal(instIdx);

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    {
        ADC_CheckChanDuplicate(config);
    }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_SAR_CTU_HwTrigIrqHandler
 * Description   : Interrupt handler functionality for HW triggered
 * conversion groups - to be called from CTU FIFO interrupt handler.
 *
 * END**************************************************************************/
void ADC_SAR_CTU_HwTrigIrqHandler(const uint32_t instIdx,
                                  const uint8_t fifoIdx)
{
    adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    const adc_group_config_t * activeGroupCfg;
    adc_group_state_t * groupState;
    uint32_t currentGroupIdx;
    ctu_conv_result_t ctuResult;
    uint16_t * result;

    DEV_ASSERT(CTU_DRV_GetFifoStatusFlags(instIdx, fifoIdx) != CTU_FIFO_OVERRUN);

    CTU_DRV_GetConvResult(instIdx, fifoIdx, CTU_RESULT_ALIGN_RIGHT_UNSIGNED, &ctuResult);

    groupState = &(palState->hwTrigGroupState[fifoIdx]);

    currentGroupIdx = groupState->groupIdx;

    activeGroupCfg = &(palState->groupArray[currentGroupIdx]);

    result = &(activeGroupCfg->resultBuffer[groupState->currentBufferOffset]);

    *result = ctuResult.convData;

    uint8_t idx;
    for (idx = 0u; idx < (activeGroupCfg->numChannels - 1u); idx++)
    {
        result++;
        *result = CTU_DRV_GetConvData(instIdx, fifoIdx, CTU_RESULT_ALIGN_RIGHT_UNSIGNED);
    }

    /* Increment offset in result buffer */
    groupState->currentBufferOffset = (groupState->currentBufferOffset + activeGroupCfg->numChannels) % groupState->bufferLength;

    /* Call notification callback, if it is enabled */
    ADC_CallNotificationCb(palState, currentGroupIdx, groupState);
}

#endif /* defined(ADC_PAL_SAR_CTU) */


#if (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_GetChanIdx
 * Description   : Returns ADC channel index, demapped from an ADC PAL input channel
 *
 * END**************************************************************************/
static inline uint8_t ADC_GetChanIdx(const adc_input_chan_t adcPalInputChan)
{
    return (uint8_t)(adcPalInputChan & ADC_CHAN_CHAN_IDX_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_GetAdcIdx
 * Description   : Returns ADC absolute index, demapped from an ADC PAL input channel.
 * E.g. each BCTU/CTU instance is linked to 2 ADCs => for ADC_3, idx = 3.
 *
 * END**************************************************************************/
static inline uint8_t ADC_GetAdcAbsoluteIdx(const adc_input_chan_t adcPalInputChan)
{
    return (uint8_t)((adcPalInputChan & ADC_CHAN_ADC_IDX_MASK) >> ADC_CHAN_ADC_IDX_OFFSET);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_GetUsedAdcMask
 * Description   : Returns a bitmask with each bit (starting from LSB) corresponding to
 * an ADC instance. If the corresponding bit is set to "1" the ADC instance is targeted
 * by at least one conversion within a group.
 *
 * END**************************************************************************/
static uint16_t ADC_GetUsedAdcInstances(const uint32_t instance)
{
    const adc_pal_state_t * const palState = &(adcPalState[instance]);
    const adc_group_config_t * currentGroup;
    uint16_t usedAdcMask = 0u;
    uint8_t idx;

    for (idx = 0u; idx < palState->numGroups; idx++)
    {
        currentGroup = &(palState->groupArray[idx]);
        /* Assumes that a group contains channels targeting the same ADC index.
         * This assumption is checked with DEV_ASSERT in other part of the code. */
        usedAdcMask |= ((uint16_t)1u << ADC_GetAdcAbsoluteIdx(currentGroup->inputChannelArray[0u]));
    }

    return usedAdcMask;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_SAR_CTU_SwTrigIrqHandler
 * Description   : Interrupt handler functionality for SW triggered
 * conversion groups - to be called from ADC End of Chain (ECH) interrupt handler.
 * Implements : ADC_Interrupt_Activity
 * END**************************************************************************/
void ADC_SAR_CTU_SwTrigIrqHandler(const uint32_t instIdx,
                                  const uint8_t adcIdx)
{
    adc_pal_state_t * const palState                = &(adcPalState[instIdx]);
    adc_group_state_t * const groupState            = &(palState->swTrigGroupState);
    const uint32_t currentGroupIdx                  = palState->latestGroupIdx;
    const adc_group_config_t * const activeGroupCfg = &(palState->groupArray[currentGroupIdx]);

    uint16_t * result = &(activeGroupCfg->resultBuffer[groupState->currentBufferOffset]);

    DEV_ASSERT(activeGroupCfg->hwTriggerSupport == false); /* This is only used for SW triggered conversion groups */

    /* Read all conversion results for the current conversion group */
    uint8_t idx;
    for (idx = 0u; idx < activeGroupCfg->numChannels; idx++)
    {
        *result = ADC_DRV_GetConvResult(adcIdx, ADC_GetChanIdx(activeGroupCfg->inputChannelArray[idx]));
        result++;
    }

    /* Increment offset in result buffer */
    groupState->currentBufferOffset = (uint16_t)((groupState->currentBufferOffset + activeGroupCfg->numChannels) % groupState->bufferLength);

    ADC_DRV_ClearStatusFlags(adcIdx, ADC_FLAG_NORMAL_ENDCHAIN);

    /* Call notification callback, if it is enabled */
    ADC_CallNotificationCb(palState, currentGroupIdx, groupState);

    if (activeGroupCfg->continuousConvEn == true) /* Continuous mode currently supported only for SW triggered groups */
    {
        ADC_DRV_StartConversion(adcIdx, ADC_CONV_CHAIN_NORMAL);
    }
    else
    {
        /* Reset all SW triggered conversion channels for the current adcIdx.
         * Assumption: a single SW triggered group is active at a time. */
        for (idx = 0u; idx < ADC_NCMR_COUNT; idx++)
        {
            adcBase[adcIdx]->NCMR[idx] = 0u;
        }

#if defined (ADC_PAL_SAR_CTU)
        adcBase[adcIdx]->MCR |= ADC_MCR_CTUEN(1u); /* set ADC to CTU CONTROL MODE, to allow HW triggering */
#endif /* defined(ADC_PAL_SAR_CTU) */

        groupState->active = false;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_ConfigAdcMPC574x
* Description   : Configure ADC instances used by the ADC_PAL for MPC574xC_G_R and ADC_PAL_SAR_CTU
* Return STATUS_ERROR if not all the configured ADCs have been calibrated successfully.
* Otherwise return STATUS_SUCCESS.
*
* END**************************************************************************/
static status_t ADC_ConfigAdcMPC574x(const uint32_t instIdx,
                                     const adc_config_t * const config)
{
#if defined (ADC_PAL_MPC574xC_G_R)
    const extension_adc_mpc574xc_g_r_t * const extension = (extension_adc_mpc574xc_g_r_t *)(config->extension);
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    const uint8_t numAdcPerTrigUnit = FEATURE_BCTU_NUM_ADC;
#endif
#elif defined (ADC_PAL_SAR_CTU)
    const extension_adc_sar_ctu_t * const extension = (extension_adc_sar_ctu_t *)(config->extension);
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    const uint8_t numAdcPerTrigUnit = FEATURE_CTU_NUM_ADC;
#endif
#endif /* defined(ADC_PAL_SAR_CTU) */
    adc_conv_config_t adcCfg;
    uint16_t idx;
    status_t status = STATUS_ERROR;

    ADC_DRV_GetDefaultConfigConverter(&adcCfg);
    adcCfg.sampleTime0  = config->sampleTicks;
    adcCfg.sampleTime1  = config->sampleTicks;
    adcCfg.sampleTime2  = config->sampleTicks;
    adcCfg.clkSelect    = extension->clkSelect;
    adcCfg.refSelect    = extension->refSelect;
    adcCfg.autoClockOff = extension->autoClockOff;
    adcCfg.convMode     = ADC_CONV_MODE_ONESHOT;
#if defined (ADC_PAL_MPC574xC_G_R)
    /* ADC may be triggered from BCTU and also from SW. */
    adcCfg.ctuMode      = ADC_CTU_MODE_TRIGGER;
#elif defined (ADC_PAL_SAR_CTU)
    /* ADC may be triggered only from CTU. To allow SW triggered groups, will toggle to CTU control disabled/enabled when starting/stopping a SW triggered conversion. */
    adcCfg.ctuMode      = ADC_CTU_MODE_CONTROL;
#endif
    adcCfg.overwriteEnable = true;

    /* Configure only the ADC instances which are used,
     * i.e. which have at least one channel assigned to at least one group. */
    uint16_t activeAdcMask = ADC_GetUsedAdcInstances(instIdx);
    for (idx = 0u; idx < ADC_INSTANCE_COUNT; idx++)
    {
        if ((activeAdcMask & ((uint16_t)1u << idx)) != 0u)
        {
            /* Check that the selected ADC is connected to the selected triggering unit.
             * Triggering unit index is mapped 1:1 with adc_pal instance index */
            DEV_ASSERT(idx >= (instIdx * numAdcPerTrigUnit));
            DEV_ASSERT(idx < ((instIdx + 1u) * numAdcPerTrigUnit));

            ADC_DRV_Reset(idx);
            status = ADC_DRV_DoCalibration(idx);
            if (status == STATUS_SUCCESS)
            {
                ADC_DRV_ConfigConverter(idx, &adcCfg);

                /* Used for SW triggered groups */
                ADC_DRV_EnableInterrupts(idx, ADC_FLAG_NORMAL_ENDCHAIN);
            }
            else
            {
                break; /* Do not calibrate the rest of the ADCs, if at least one calibration failed */
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ResetAdcInstances
 * Description   : Reset active ADC instances used by the ADC_PAL for MPC574xC_G_R and ADC_PAL_SAR_CTU
 *
 * END**************************************************************************/
static void ADC_ResetAdcMPC574x(const uint32_t instIdx)
{
#if defined (ADC_PAL_MPC574xC_G_R)
    const uint8_t numAdcPerTrigUnit = FEATURE_BCTU_NUM_ADC;
#elif defined (ADC_PAL_SAR_CTU)
    const uint8_t numAdcPerTrigUnit = FEATURE_CTU_NUM_ADC;
#endif

    uint16_t idx;

    /* Deinit only the ADC instances which correspond to the current ADC PAL instance,
     * and are active, i.e. have at least one channel assigned to at least one group. */
    uint16_t activeAdcMask  = ADC_GetUsedAdcInstances(instIdx);
    const uint16_t startIdx = instIdx * numAdcPerTrigUnit;
    const uint16_t endIdx   = (instIdx + 1u) * numAdcPerTrigUnit;
    for (idx = startIdx; idx < endIdx; idx++)
    {
        if ((activeAdcMask & ((uint16_t)1u << idx)) != 0u)
        {
            ADC_DRV_Reset(idx);
        }
    }
}

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_CheckChanDuplicate
 * Description   : Check that a channel doesn't appear multiple times within a group
 * for MPC574xC_G_R: check for SW and HW triggered groups
 * for ADC_PAL_SAR_CTU: check only for SW triggered groups. HW triggered groups may contain
 * the same channel multiple times, because results are stored in CTU result FIFOs.
 *
 * END**************************************************************************/
static void ADC_CheckChanDuplicate(const adc_config_t * const config)
{
    uint8_t numOccurrences[ADC_CDR_COUNT] = {0}; /* total number of ADC channels is given by ADC_CDR_COUNT */
    uint8_t idx1, idx;
    uint8_t chanIdx;
    const adc_group_config_t * currentGroup;

    /* Workaround for MISRA false positive reported for Rule 9.1 - array conceivably not initialized.
     * The local array is already initialized at definition. */
    for (idx = 0u; idx < ADC_CDR_COUNT; idx++)
    {
        numOccurrences[idx] = 0u;
    }

    for (idx = 0u; idx < config->numGroups; idx++)
    {
        currentGroup = &(config->groupConfigArray[idx]);

#if defined (ADC_PAL_SAR_CTU)
        if (currentGroup->hwTriggerSupport == true)
        {
            continue;
        }

#endif /* defined(ADC_PAL_SAR_CTU) */

        /* First reset the number of occurrences of each channel */
        for (idx1 = 0u; idx1 < currentGroup->numChannels; idx1++)
        {
            chanIdx = ADC_GetChanIdx(currentGroup->inputChannelArray[idx1]);
            numOccurrences[chanIdx] = 0u;
        }

        /* Count the number of occurrences for each channel */
        for (idx1 = 0u; idx1 < currentGroup->numChannels; idx1++)
        {
            chanIdx = ADC_GetChanIdx(currentGroup->inputChannelArray[idx1]);
            numOccurrences[chanIdx]++;
        }

        for (idx1 = 0u; idx1 < currentGroup->numChannels; idx1++)
        {
            chanIdx = ADC_GetChanIdx(currentGroup->inputChannelArray[idx1]);
            if (numOccurrences[chanIdx] > 1u)
            {
                DEV_ASSERT(false); /* A channel must not appear multiple times inside a SW triggered group */
            }
        }
    }
}

#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */


#if (defined (ADC_PAL_S32K1xx) || defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_GetHwGroupStatePtr
 * Description   : Returns pointer to the HW triggered state structure.
 * Mapping of groupIndex to index of the HW triggered state is platform specific.
 *
 * END**************************************************************************/
static inline adc_group_state_t * ADC_GetHwGroupStatePtr(const uint32_t instIdx,
                                                         const uint32_t groupIdx)
{
#if defined (ADC_PAL_S32K1xx)

    (void)groupIdx;

    return &(adcPalState[instIdx].hwTrigGroupState[0u]); /* S32K1xx supports only a single active group at a time, so any active groupIdx will be mapped to state idx 0 */

#elif (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

    adc_pal_state_t * const palState = &(adcPalState[instIdx]);
    const uint32_t stateIdx = palState->stateIdxMapping[groupIdx];

    return &(palState->hwTrigGroupState[stateIdx]);

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_AnyHwTrigGroupActive
 * Description   : Returns true if at least one hwTriggered group is enabled.
 * Otherwise returns false.
 *
 * END**************************************************************************/
static bool ADC_AnyHwTrigGroupActive(const uint32_t instIdx)
{
#if defined (ADC_PAL_S32K1xx)

    return adcPalState[instIdx].hwTrigGroupState[0u].active; /* S32K1xx supports only a single active group at a time, so any active groupIdx will be mapped to state idx 0 */

#elif (defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU))

    bool active = false;
    uint8_t idx = 0u;

    while ((active == false) && (idx < ADC_PAL_MAX_NUM_HW_GROUPS_EN))
    {
        if (adcPalState[instIdx].hwTrigGroupState[idx].active == true)
        {
            active = true;
        }

        idx++;
    }

    return active;

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_PalStateUpdateStart
 * Description   : Update PAL state structure with changes for StartGroupConversion and
 * EnableHardwareTigger.
 *
 * END**************************************************************************/
static void ADC_PalStateUpdateStart(const uint32_t instIdx,
                                    const uint32_t groupIdx)
{
    adc_pal_state_t * palState                       = &(adcPalState[instIdx]);
    const adc_group_config_t * const currentGroupCfg = &(palState->groupArray[groupIdx]);
    adc_group_state_t * groupState;

    if (currentGroupCfg->hwTriggerSupport == true)
    {
        groupState = ADC_GetHwGroupStatePtr(instIdx, groupIdx);
    }
    else
    {
        groupState = &(palState->swTrigGroupState);
    }

    palState->latestGroupIdx        = groupIdx;
    groupState->active              = true;
    groupState->currentBufferOffset = 0u;
#if defined (ADC_PAL_SAR_CTU)
    groupState->groupIdx            = groupIdx;
#endif
    groupState->bufferLength        = (uint16_t)currentGroupCfg->numChannels * currentGroupCfg->numSetsResultBuffer;
    if (currentGroupCfg->callback != NULL)
    {
        groupState->notificationEn = true; /* enable notification by default if callback is available */
    }
    else
    {
        groupState->notificationEn = false; /* disable notification if callback is not available */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_CallNotificationCb
 * Description   : Calls the notification callback for the current active group,
 * if it is enabled.
 *
 * END**************************************************************************/
static void ADC_CallNotificationCb(const adc_pal_state_t * const palState,
                                   const uint32_t groupIdx,
                                   const adc_group_state_t * const groupState)
{
    const adc_group_config_t * const activeGroupCfg = &(palState->groupArray[groupIdx]);

    /* Call notification callback, if it is enabled */
    if (groupState->notificationEn)
    {
        adc_callback_info_t cbInfo;
        cbInfo.groupIndex = groupIdx;

        if (groupState->currentBufferOffset == 0u)
        {
            cbInfo.resultBufferTail = (uint16_t)(groupState->bufferLength - 1u); /* set tail to the last position in buffer */
        }
        else
        {
            cbInfo.resultBufferTail = (uint16_t)(groupState->currentBufferOffset - 1u); /* set tail to last written position  */
        }

        (*(activeGroupCfg->callback))(&cbInfo, activeGroupCfg->callbackUserData);
    }
}

#endif /* (defined(ADC_PAL_S32K1xx) || defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU)) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
