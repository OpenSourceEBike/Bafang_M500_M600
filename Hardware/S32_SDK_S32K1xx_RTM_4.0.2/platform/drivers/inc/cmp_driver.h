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
/*!
 * @file cmp_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef CMP_DRIVER_H
#define CMP_DRIVER_H

#include "status.h"
#include "device_registers.h"

/*!
 * @addtogroup cmp_driver Comparator Driver
 * @ingroup cmp
 * @brief Comparator Peripheral Driver
 * @{
 *******************************************************************************
 * Definitions
 ******************************************************************************/
#define CMP_INPUT_FLAGS_MASK 0xFF0000
#define CMP_INPUT_FLAGS_SHIFT 16U
#define CMP_ROUND_ROBIN_CHANNELS_MASK 0xFF0000
#define CMP_ROUND_ROBIN_CHANNELS_SHIFT 16U

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/
 /*! @brief Power Modes selection
  * Implements : cmp_power_mode_t_Class
  */
typedef enum
{
    CMP_LOW_SPEED   = 0U,   /*!< Module in low speed mode. */
    CMP_HIGH_SPEED  = 1U    /*!< Module in high speed mode. */
} cmp_power_mode_t;

 /*! @brief Voltage Reference selection
  * Implements : cmp_voltage_reference_t_Class
  */
typedef enum
{
    CMP_VIN1 = 0U,  /*!< Use Vin1 as supply reference source for DAC. */
    CMP_VIN2 = 1U   /*!< Use Vin2 as supply reference source for DAC. */
} cmp_voltage_reference_t;

 /*! @brief Port Mux Source selection
  * Implements : cmp_port_mux_t_Class
  */
typedef enum
{
    CMP_DAC = CMP_DAC_SOURCE,   /*!< Select DAC as source for the comparator port. */
    CMP_MUX = CMP_MUX_SOURCE    /*!< Select MUX8 as source for the comparator port. */
} cmp_port_mux_t;

 /*! @brief Comparator output invert selection
  * Implements : cmp_inverter_t_Class
  */
typedef enum
{
    CMP_NORMAL = 0U,    /*!< Output signal isn't inverted. */
    CMP_INVERT = 1U     /*!< Output signal is inverted. */
} cmp_inverter_t;

 /*! @brief Comparator output select selection
  * Implements : cmp_output_select_t_Class
  */
typedef enum
{
    CMP_COUT  = 0U,     /*!< Select COUT as comparator output signal. */
    CMP_COUTA = 1U      /*!< Select COUTA as comparator output signal. */
} cmp_output_select_t;

 /*! @brief Comparator output pin enable selection
  * Implements : cmp_output_enable_t_Class
  */
typedef enum
{
    CMP_UNAVAILABLE  = 0U,  /*!< Comparator output isn't available to a specific pin*/
    CMP_AVAILABLE    = 1U   /*!< Comparator output is available to a specific pin*/
} cmp_output_enable_t;

#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
 /*! @brief Comparator hard block offset control
  * Implements : cmp_offset_t_Class
  */
typedef enum
{
    CMP_LEVEL_OFFSET_0 = 0U,
    CMP_LEVEL_OFFSET_1 = 1U
} cmp_offset_t;
#endif

 /*! @brief Comparator hysteresis control
  * Implements : cmp_hysteresis_t_Class
  */
typedef enum
{
    CMP_LEVEL_HYS_0 = 0U,
    CMP_LEVEL_HYS_1 = 1U,
    CMP_LEVEL_HYS_2 = 2U,
    CMP_LEVEL_HYS_3 = 3U
} cmp_hysteresis_t;

 /*! @brief Comparator Round-Robin fixed port
  * Implements : cmp_fixed_port_t_Class
  */
typedef enum
{
    CMP_PLUS_FIXED      = 0U,   /*!< The Plus port is fixed. Only the inputs to the Minus port are swept in each round. */
    CMP_MINUS_FIXED     = 1U    /*!< The Minus port is fixed. Only the inputs to the Plus port are swept in each round. */
} cmp_fixed_port_t;

 /*! @brief Comparator output interrupt configuration
  * Implements : cmp_output_trigger_t_Class
  */
typedef enum
{
    CMP_NO_EVENT            = 0U,   /*!< Comparator output interrupts are disabled OR no event occurred. */
    CMP_FALLING_EDGE        = 1U,   /*!< Comparator output interrupts will be generated only on falling edge OR only falling edge event occurred. */
    CMP_RISING_EDGE         = 2U,   /*!< Comparator output interrupts  will be generated only on rising edge OR only rising edge event occurred. */
    CMP_BOTH_EDGES          = 3U    /*!< Comparator output interrupts  will be generated on both edges OR both edges event occurred. */
} cmp_output_trigger_t;

 /*! @brief Comparator functional modes
  * Implements : cmp_mode_t_Class
  */
typedef enum
{
    CMP_DISABLED                        = 0U,
    CMP_CONTINUOUS                      = 1U,
    CMP_SAMPLED_NONFILTRED_INT_CLK      = 2U,
    CMP_SAMPLED_NONFILTRED_EXT_CLK      = 3U,
    CMP_SAMPLED_FILTRED_INT_CLK         = 4U,
    CMP_SAMPLED_FILTRED_EXT_CLK         = 5U,
    CMP_WINDOWED                        = 6U,
    CMP_WINDOWED_RESAMPLED              = 7U,
    CMP_WINDOWED_FILTRED                = 8U
} cmp_mode_t;

 /*! @brief Comparator channels list (1bit/channel)
   |---------|---------|-----|---------|---------|
   |CH7_state|CH6_state|.....|CH1_state|CH0_state|
   |---------|---------|-----|---------|---------|
  * Implements : cmp_ch_list_t_Class
  */
typedef  uint8_t cmp_ch_list_t;

 /*! @brief Number of channel
  * Implements : cmp_ch_number_t_Class  
  */
typedef  uint8_t cmp_ch_number_t;

/*!
 * @brief Defines the block configuration
 *
 * This structure is used to configure only comparator block module(filtering, sampling, power_mode etc.)
 * Implements : cmp_comparator_t_Class
 */
typedef struct
{
    bool                    dmaTriggerState;        /*!< True if DMA transfer trigger from comparator is enable.*/
    cmp_output_trigger_t    outputInterruptTrigger; /*!< CMP_NO_INTERRUPT comparator output would not trigger any interrupt.*/
                                                    /*!< CMP_FALLING_EDGE comparator output would trigger an interrupt on falling edge.*/
                                                    /*!< CMP_RISING_EDGE comparator output would trigger an interrupt on rising edge.*/
                                                    /*!< CMP_BOTH_EDGES comparator output would trigger an interrupt on rising and falling edges.*/
    cmp_mode_t              mode;                   /*!< Configuration structure which define: the comparator functional mode, sample period and sample count.*/ 
    uint8_t                 filterSamplePeriod;     /*!< Filter sample period. */
    uint8_t                 filterSampleCount;      /*!< Number of sample count for filtering. */
    cmp_power_mode_t        powerMode;              /*!< CMP_LOW_SPEED if low speed mode is selected. */
                                                    /*!< CMP_HIGH_SPEED if high speed mode is selected*/
    cmp_inverter_t          inverterState;          /*!< CMP_NORMAL if does not invert the comparator output.*/
                                                    /*!< CMP_INVERT if inverts the comparator output.*/
    cmp_output_enable_t     pinState;               /*!< CMP_UNAVAILABLE if comparator output is not available to package pin.*/
                                                    /*!< CMP_AVAILABLE if comparator output is available to package pin.*/
    cmp_output_select_t     outputSelect;           /*!< CMP_COUT if output signal is equal to COUT(filtered).*/     
                                                    /*!< CMP_COUTA if output signal is equal to COUTA(unfiltered).*/
#if FEATURE_CMP_HAS_HARD_BLOCK_OFFSET
    cmp_offset_t            offsetLevel;            /*!< CMP_LEVEL_OFFSET_0 if hard block output has level 0 offset.*/
                                                    /*!< CMP_LEVEL_OFFSET_1 if hard block output has level 1 offset.*/
#endif
    cmp_hysteresis_t        hysteresisLevel;        /*!< CMP_LEVEL_HYS_0 if hard block output has level 0 hysteresis.*/
                                                    /*!< CMP_LEVEL_HYS_1 if hard block output has level 1 hysteresis.*/
                                                    /*!< CMP_LEVEL_HYS_2 if hard block output has level 2 hysteresis.*/     
                                                    /*!< CMP_LEVEL_HYS_3 if hard block output has level 3 hysteresis.*/
}cmp_comparator_t;

/*!
 * @brief Defines the analog mux
 *
 * This structure is used to configure the analog multiplexor to select compared signals 
 * Implements : cmp_anmux_t_Class
 */
typedef struct
{
    cmp_port_mux_t      positivePortMux;    /*!< Select positive port signal.*/
                                            /*!< CMP_DAC if source is digital to analog converter.*/
                                            /*!< CMP_MUX if source is 8 ch MUX*/
    cmp_port_mux_t      negativePortMux;    /*!< Select negative port signal.*/
                                            /*!< CMP_DAC if source is digital to analog converter.*/
                                            /*!< CMP_MUX if source is 8 ch MUX */

    cmp_ch_number_t     positiveInputMux;   /*!< Select which channel is selected for the plus mux.*/
    cmp_ch_number_t     negativeInputMux;   /*!< Select which channel is selected for the minus mux.*/
}cmp_anmux_t;

/*!
 * @brief Defines the DAC block
 *
 * This structure is used to configure the DAC block integrated in comparator module
 * Implements : cmp_dac_t_Class
 */
typedef struct
{
    cmp_voltage_reference_t     voltageReferenceSource;     /*!< CMP_VIN1 if selected voltage reference is VIN1.*/  
                                                            /*!< CMP_VIN2 if selected voltage reference is VIN2.*/
    uint8_t                     voltage;                    /*!< The digital value which is converted to analog signal.*/
    bool                        state;                      /*!< True if DAC is enabled.*/

#if FEATURE_CMP_DAC_FIX_SELECTION
    bool						fixRefInputMux;				/*!< FALSE - The DAC output is not used as the reference input to the fixed mux port.
																 TRUE  - The DAC output is used as the reference input to the fixed mux port,
																 regardless of the FXMXCH configuration.*/
#endif
}cmp_dac_t;

/*!
 * @brief Defines the trigger mode
 *
 * This structure is used to configure the trigger mode operation when MCU enters STOP modes
 * Implements : cmp_trigger_mode_t_Class
 */
typedef struct
{
    bool                roundRobinState;            /*!< True if Round-Robin is enabled.*/
    bool                roundRobinInterruptState;   /*!< True if Round-Robin interrupt is enabled.*/
    cmp_fixed_port_t    fixedPort;                  /*!< CMP_PLUS_FIXED if plus port is fixed.*/
                                                    /*!< CMP_MINUS_FIXED if minus port is fixed.*/
    cmp_ch_number_t     fixedChannel;               /*!< Select which channel would be assigned to the fixed port.*/
    uint8_t             samples;                    /*!< Select number of round-robin clock cycles for a given channel.*/
#if FEATURE_CMP_HAS_INIT_DELAY
    uint8_t             initializationDelay;        /*!< Select dac and comparator initialization delay(clock cycles).*/
#endif
    cmp_ch_list_t       roundRobinChannelsState;    /*!< One bite for each channel state.*/
                                                    /*!< |---------|---------|-----|---------|---------|*/
                                                    /*!< |CH7_state|CH6_state|.....|CH1_state|CH0_state|*/
                                                    /*!< |---------|---------|-----|---------|---------|*/
    cmp_ch_list_t       programedState;             /*!< Pre-programmed state for comparison result.*/
}cmp_trigger_mode_t;

/*!
 * @brief Defines the comparator module configuration
 *
 * This structure is used to configure all components of comparator module
 * Implements : cmp_module_t_Class
 */
typedef struct
{
    cmp_comparator_t        comparator;
    cmp_anmux_t             mux;
    cmp_dac_t               dac;
    cmp_trigger_mode_t      triggerMode;
}cmp_module_t;


/*******************************************************************************
 * API
 ******************************************************************************/
 /*!
 * @name cMP DRV.
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Reset all registers
 *
 * This function set all CMP registers to reset values.
 *
 * @param instance - instance number
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_Reset(const uint32_t instance);

/*!
 * @brief Get reset configuration for all registers
 *
 * This function returns a configuration structure with reset values for all registers
 * from comparator module.
 *
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInitConfigAll(cmp_module_t* config);

/*!
 * @brief Gets a default comparator configuration
 *
 * This function returns a default configuration for the comparator as a configuration structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetDefaultConfig(cmp_module_t* const config);

/*!
 * @brief Configure all comparator features with the given configuration structure
 *
 * This function configures the comparator module with the options
 * provided in the config structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_Init(const uint32_t instance, const cmp_module_t* const config);

/*!
 * @brief Gets the current comparator configuration
 *
 * This function returns the current configuration for comparator as a configuration structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetConfigAll(const uint32_t instance, cmp_module_t* const config);

/*!
 * @brief Get reset configuration for registers related with DAC
 *
 * This function returns a configuration structure with reset values for features associated with DAC.
 *
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInitConfigDAC(cmp_dac_t* config);

/*!
 * @brief Configure only the DAC component
 *
 * This function configures the DAC with the options provided in the config structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ConfigDAC(const uint32_t instance, const cmp_dac_t* config);

/*!
 * @brief Return current configuration for DAC
 *
 * This function returns current configuration only for DAC.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetDACConfig(const uint32_t instance, cmp_dac_t* const config);

/*!
 * @brief Get reset configuration for registers related with MUX
 *
 * This function returns a configuration structure with reset values for features associated with MUX.
 *
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInitConfigMUX(cmp_anmux_t* config);

/*!
 * @brief Configure only the MUX component
 *
 * This function configures the MUX with the options provided in the config structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ConfigMUX(const uint32_t instance, const cmp_anmux_t* config);

/*!
 * @brief Return configuration only for the MUX component
 *
 * This function returns current configuration to determine which signals go to comparator ports.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetMUXConfig(const uint32_t instance, cmp_anmux_t* const config);

/*!
 * @brief Get reset configuration for registers related with Trigger Mode
 *
 * This function returns a configuration structure with reset values for features associated with Trigger Mode.
 *
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInitTriggerMode(cmp_trigger_mode_t* config);

/*!
 * @brief Configure trigger mode
 *
 * This function configures the trigger mode with the options provided in the config structure.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ConfigTriggerMode(const uint32_t instance, const cmp_trigger_mode_t* config);

/*!
 * @brief Get current trigger mode configuration
 *
 * This function returns the current trigger mode configuration for trigger mode.
 *
 * @param instance - instance number
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetTriggerModeConfig(const uint32_t instance, cmp_trigger_mode_t* const config);

/*!
 * @brief Get comparator output flags
 *
 * This function returns in <flags> comparator output flags(rising and falling edge).
 *
 * @param instance - instance number
 * @param - flags - pointer to output flags
 *      NO_EVENT    
 *      RISING_EDGE     
 *      FALLING_EDGE    
 *      BOTH_EDGE       
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetOutputFlags(const uint32_t instance, cmp_output_trigger_t *flags);

/*!
 * @brief Clear comparator output flags
 *
 * This function clear comparator output flags(rising and falling edge).
 *
 * @param instance - instance number
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ClearOutputFlags(const uint32_t instance);

/*!
 * @brief Gets input channels change flags
 *
 * This function return in <flags> all input channels flags as uint8_t(1 bite
 *for each channel flag).
 *
 * @param instance - instance number
 * @param flags - pointer to input flags
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInputFlags(const uint32_t instance, cmp_ch_list_t *flags);

/*!
 * @brief Clear comparator input channels flags
 *
 * This function clear comparator input channels flags.
 *
 * @param instance - instance number 
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ClearInputFlags(const uint32_t instance);

/*!
 * @brief Get reset configuration for registers related with comparator features
 *
 * This function return a configuration structure with reset values for features associated with
 * comparator (DMA request, power mode, output select, interrupts enable, invert,
 * offset, hysteresis).
 *
 * @param config - the configuration structure
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetInitConfigComparator(cmp_comparator_t* config);

/*!
 * @brief Configure only comparator features
 *
 * This function configure only features related with comparator:
 * DMA request, power mode, output select, interrupts enable, invert,
 * offset, hysteresis.
 *
 * @param instance - instance number
 * @param config - the configuration structure 
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_ConfigComparator(const uint32_t instance, const cmp_comparator_t *config);

/*!
 * @brief Return configuration for comparator from CMP module
 *
 * This function return configuration for features related with comparator:
 * DMA request, power mode, output select, interrupts enable, invert,
 * offset, hysteresis.
 *
 * @param instance - instance number
 * @param config - the configuration structure returned
 * @return 
 *        - STATUS_SUCCESS : Completed successfully.
 *        - STATUS_ERROR : Error occurred.
 */
status_t CMP_DRV_GetComparatorConfig(const uint32_t instance, cmp_comparator_t *config);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __CMP_DRIVER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

