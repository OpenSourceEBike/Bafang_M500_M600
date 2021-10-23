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

#ifndef ADC_PAL_H
#define ADC_PAL_H

#include "adc_pal_cfg.h"
#include "adc_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*! @file adc_pal.h */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros are defined for mapping multiple ADC instances to ADC PAL instance.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * Function-like macros are used for mapping ADC instances to PAL instances.
 */

/*!
 * @ingroup adc_pal
 * @addtogroup adc_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined (ADC_PAL_S32K1xx)

/*!
 * @brief Defines the enumeration with ADC PAL input channels
 *
 * Implements : adc_input_chan_t_Class
 */
typedef adc_inputchannel_t adc_input_chan_t;

/*!
 * @brief Defines the enumeration with ADC PAL hardware trigger sources
 *
 * Implements : adc_trigger_source_t_Class
 */
typedef trgmux_trigger_source_t adc_trigger_source_t;

#elif defined (ADC_PAL_MPC574xC_G_R) || defined (ADC_PAL_SAR_CTU)

/*!
 * @brief Defines the type used for ADC PAL input channels.
 * The ADC channel numbers (for each instance) are mapped on this type according to the ADC_CHAN_ macros.
 * The mapping is required because an instance of ADC PAL uses multiple ADC instances linked to the same triggering unit.
 * Therefore, please use the ADC_CHAN_ macros together with adc_input_chan_t type variables.
 *
 * Implements : adc_input_chan_t_Class
 */
typedef uint16_t adc_input_chan_t;

#if defined (ADC_PAL_MPC574xC_G_R)
/*!
 * @brief Typedef for the ADC PAL hardware trigger sources
 * Should be used with BCTU trigger number, which selects a trigger source from other peripherals available on the platform.
 *
 * Implements : adc_trigger_source_t_Class
 */
typedef uint8_t adc_trigger_source_t;

#elif defined (ADC_PAL_SAR_CTU)

/*!
 * @brief Defines the enumeration with ADC PAL hardware trigger sources
 * This is a mask which selects rising, falling or both edges for each available input trigger.
 * Should be initialized using CTU_INPUT_TRIG_BITMASK_RISING/FALLING/BOTH macros, with ctu_input_trig_t parameter.
 *
 * Implements : adc_trigger_source_t_Class
 */
typedef uint32_t adc_trigger_source_t;

#endif /* defined(ADC_PAL_MPC574xC_G_R) */


/*!
 * @brief Defines mapping of ADC channel numbers (for each ADC instance) to the ADC PAL adc_input_chan_t type.
 * The mapping is done because an instance of ADC PAL uses multiple ADC instances linked to the same instance of BCTU/CTU.
 *
 */
#define ADC_CHAN_ADC_IDX_MASK   (0xF000u)              /*!< Bitmask corresponding to ADC index mapped in ADC PAL input channel (adc_input_chan_t) */
#define ADC_CHAN_CHAN_IDX_MASK  (0xFFFu)               /*!< Bitmask corresponding to ADC channel mapped in ADC PAL input channel (adc_input_chan_t) */
#define ADC_CHAN_ADC_IDX_OFFSET (12u)                  /*!< Offset of the ADC instance index mapped in ADC PAL input channel (adc_input_chan_t) */
/*!< Macro which maps a channel from an ADC instance to the corresponding ADC PAL input channel value (adc_input_chan_t) */
#define ADC_CHAN_MAP(ADC_IDX, CHAN_IDX) ((adc_input_chan_t)(((CHAN_IDX)&ADC_CHAN_CHAN_IDX_MASK) + ((((uint16_t)ADC_IDX) << ADC_CHAN_ADC_IDX_OFFSET) & ADC_CHAN_ADC_IDX_MASK)))

#endif /* defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU) */


/*!
 * @brief Defines an enumeration which contains the types of delay configurations for ADC conversions within a group.
 *
 * Implements : adc_delay_type_t_Class
 */
typedef enum
{
    ADC_DELAY_TYPE_NO_DELAY         = 0u,   /*!< First conversion can start right after the trigger occurrence, and the rest of conversions execute one after another */
    ADC_DELAY_TYPE_GROUP_DELAY      = 1u,   /*!< Delay only first conversion, and the rest execute one after another */
    ADC_DELAY_TYPE_INDIVIDUAL_DELAY = 2u    /*!< Individual delay for each conversion in the group (each measured from the occurrence of the trigger) */
} adc_delay_type_t;

/*!
 * @brief Defines the configuration structure for an ADC PAL conversion group
 *
 * Implements : adc_group_config_t_Class
 */
typedef struct
{
    const adc_input_chan_t * inputChannelArray;   /*!< Pointer to the array of ADC input channels. Each entry in this array corresponds to an individual conversion in the group.
                                                   *   Only on some of the platforms the same input channel may appear multiple times - see device family specific details in the ADC PAL documentation. */
    uint16_t * resultBuffer;                      /*!< Pointer to the array for conversion results */
    uint8_t numChannels;                          /*!< Number of input channels in the array */
    uint8_t numSetsResultBuffer;                  /*!< Number of sets of results which can be stored in result buffer:
                                                   *   length of the result buffer = numChannels x numSetsResultBuffer */
    bool hwTriggerSupport;                        /*!< Conversion group is HW triggered (true) or SW triggered (false).  */
    adc_trigger_source_t triggerSource;           /*!< HW trigger source associated with the conversion group. Will be ignored if (hwTriggerSupport == false).
                                                   *   Note for ADC_SAR_CTU: this enables the HW trigger source for all other groups; the actual order of execution of groups depends on the order of occurrence of triggers. */
    adc_delay_type_t delayType;                   /*!< Type of delay configuration. Supported values are platform dependent. */
    uint16_t * delayArray;                        /*!< Pointer to array of delay values introduced from the occurrence of a HW trigger event until each ADC conversion in the group can start execution.
                                                   *   Expressed in clock ticks. Note: the delay might be bigger if there is an overlap with another conversion already executing. */
    bool continuousConvEn;                        /*!< Flag for enabling continuous conversions of a group - used only for SW triggered groups i.e. hwTriggerSupport==false. */

    adc_callback_t callback;                      /*!< Callback function associated with group conversion complete */
    void * callbackUserData;                      /*!< Pointer to additional user data to be passed by the callback */
} adc_group_config_t;


/*!
 * @brief Defines the configuration structure for ADC PAL
 *
 * Implements : adc_config_t_Class
 */
typedef struct
{
    const adc_group_config_t * groupConfigArray;  /*!< Array of group configurations */
    uint16_t numGroups;                           /*!< Number of elements in groupConfigArray */

    uint8_t sampleTicks;                          /*!< Duration of sample time expressed in ADC clock ticks */
    void * extension;                             /*!< This field is used to add extra IP specific settings to the basic configuration. */
} adc_config_t;


#if defined (ADC_PAL_S32K1xx)
/*!
 * @brief Defines the extension structure for ADC S32K1xx
 *
 * Implements : extension_adc_s32k1xx_t_Class
 */
typedef struct
{
    adc_clk_divide_t clockDivide;         /*!< Divider of the input clock for the ADC */
    adc_resolution_t resolution;          /*!< ADC resolution (8,10,12 bit) */
    adc_input_clock_t inputClock;         /*!< Input clock source */
    adc_voltage_reference_t voltageRef;   /*!< Voltage reference used */
    bool supplyMonitoringEnable;          /*!< Enable internal supply monitoring */
    pdb_clk_prescaler_div_t pdbPrescaler; /*!< PDB clock prescaler. Delays are measured based on PDB clock divided by prescaler.
                                           *   Only relevant if delays are used. */
} extension_adc_s32k1xx_t;
#endif /* defined(ADC_PAL_S32K1xx) */


#if defined (ADC_PAL_MPC574xC_G_R)
/*!
 * @brief Defines the extension structure for ADC MPC574xC, MPC574xG
 *
 * Implements : extension_adc_mpc574xc_g_r_t_Class
 */
typedef struct
{
    adc_clk_sel_t clkSelect;            /*!< Clock input */
    adc_ref_sel_t refSelect;            /*!< Reference selection  */
    bool autoClockOff;                  /*!< Enable Auto Clock Off */
} extension_adc_mpc574xc_g_r_t;
#endif /* defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_SAR_CTU) */


#if defined (ADC_PAL_SAR_CTU)
/*!
 * @brief Defines the extension structure for ADC PAL of type SAR_CTU
 *
 * Implements : extension_adc_sar_ctu_t_Class
 */
typedef struct
{
    adc_clk_sel_t clkSelect;                    /*!< ADC conversion clock prescale */
    adc_ref_sel_t refSelect;                    /*!< Voltage reference selection */
    bool autoClockOff;                          /*!< Enable ADC Auto Clock Off */
    ctu_prescaler_t ctuPrescaler;               /*!< CTU prescale value - affects input clock for group delays */
} extension_adc_sar_ctu_t;

#endif /* defined(ADC_PAL_SAR_CTU) */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ADC PAL instance
 *
 * This function initializes the ADC PAL instance, including the other platform specific HW units used together with ADC.
 * Notifications are default enabled after init.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] config The ADC PAL configuration structure
 * @return status:
 * \n - STATUS_ERROR: platform specific error encountered while initializing one of the HW modules used by ADC PAL.
 * On MPC574x returned  if ADC calibration did not succeed for all the selected ADCs.
 * On S32K1xx returned if it cannot reconfigure successfully the TRGMUX trigger source of the used PDB instance.
 * \n - STATUS_BUSY: on MPC574x platforms, if the BCTU module could not be reset
 * \n - STATUS_SUCCESS: ADC PAL initialized successfully
 */
status_t ADC_Init(const adc_instance_t * const instance,
                  const adc_config_t * const config);

/*!
 * @brief Deinitializes the ADC PAL instance
 *
 * This function resets the ADC PAL instance, including the other platform specific HW units used together with ADC,
 * if there are no active conversions.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_BUSY: on MPC574x platforms, if the BCTU module could not be reset
 * \n - STATUS_SUCCESS: ADC PAL initialized successfully
 */
status_t ADC_Deinit(const adc_instance_t * const instance);


/*!
 * @brief Enables the selected HW trigger for a conversion group, if the conversion group has support for HW trigger.
 *
 * Enables the selected HW trigger for a conversion group, if the conversion group has support for HW trigger.
 * The function will return an error code if there is a conversion group already active.
 * If the function succeeds, the conversion group will be triggered for execution when the
 * selected HW trigger occurs.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_SUCCESS: HW trigger enabled successfully for the selected conversion group
 */
status_t ADC_EnableHardwareTrigger(const adc_instance_t * const instance,
                                   const uint32_t groupIdx);


/*!
 * @brief Disables the selected HW trigger for a conversion group, if the conversion group is HW triggered.
 *
 * This function disables the HW trigger for a configured conversion group and also may stop its execution (depending on platform),
 * if called when a conversion group is executing. If stopping is supported, the execution shall be stopped according to device specific procedures.
 * The function shall wait for the procedures to complete within the given timeout interval and return error code if they do not succeed.
 * \nS32K1xx: the function prevents new conversions from the group from starting, then waits until the current active conversion finishes execution
 * (if the function call occurred while an ADC conversion from the group is executing) or timeout occurs.
 * \nMPC574xC_G_R: the execution of a HW triggered group of conversions cannot be stopped, so the function shall wait until it is complete
 * or timeout occurs.
 * \nADC_SAR_CTU: the function always returns STATUS_SUCCESS (even if a conversion is still executing) and doesn't use 'timeout' parameter.
 * If it is called during a control cycle, between MRS and actual group conversion start, there will be an additional execution of the group, without callback.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @param[in] timeout Timeout interval in milliseconds
 * @return status:
 * \n - STATUS_TIMEOUT: the operation did not complete successfully within the provided timeout interval
 * \n - STATUS_SUCCESS: the operation completed successfully within the provided timeout interval
 */
status_t ADC_DisableHardwareTrigger(const adc_instance_t * const instance,
                                    const uint32_t groupIdx,
                                    const uint32_t timeout);


/*!
 * @brief Starts the execution of a selected SW triggered ADC conversion group.
 *
 * This function starts execution of a selected ADC conversion group, if there is no other conversion group active.
 * Conversion groups started by ADC_StartGroupConversion shall not be preempted by HW triggered conversion groups.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_SUCCESS: group conversion successfully triggered
 */
status_t ADC_StartGroupConversion(const adc_instance_t * const instance,
                                  const uint32_t groupIdx);


/*!
 * @brief Stops the selected SW triggered ADC conversion group execution.
 *
 * This function stops the execution of a SW triggered conversion group.
 * The execution shall be stopped according to device specific procedures.
 * The function shall wait for the procedures to complete within the given timeout interval and return error code if they do not succeed.
 * For ADC_SAR_CTU type and MPC574xC_G_R a conversion already started for execution cannot be stopped, so the function shall wait until it finishes
 * or timeout occurs.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @param[in] timeout Timeout interval in milliseconds
 * @return status:
 * \n - STATUS_TIMEOUT: the operation did not complete successfully within the provided timeout interval
 * \n - STATUS_SUCCESS: the operation completed successfully within the provided timeout interval
 */
status_t ADC_StopGroupConversion(const adc_instance_t * const instance,
                                 const uint32_t groupIdx,
                                 const uint32_t timeout);


/*!
 * @brief Enables the notification callback for a configured group
 *
 * This function enables the notification callback for a selected group of ADC conversions.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_ERROR: the selected group is not active (SW triggered running or HW triggered running or enabled)
 * \n - STATUS_SUCCESS: the notification has been enabled successfully
 */
status_t ADC_EnableNotification(const adc_instance_t * const instance,
                                const uint32_t groupIdx);


/*!
 * @brief Disables the notification callback for a configured group
 *
 * This function disables the notification callback for a selected group of ADC conversions.
 *
 * @param[in] instance Pointer to ADC PAL instance number structure
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_ERROR: the selected group is not active (SW triggered running or HW triggered running or enabled)
 * \n - STATUS_SUCCESS: the notification has been disabled successfully
 */
status_t ADC_DisableNotification(const adc_instance_t * const instance,
                                 const uint32_t groupIdx);


#if defined (__cplusplus)
}
#endif

/*! @}*/
#endif /* ADC_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
