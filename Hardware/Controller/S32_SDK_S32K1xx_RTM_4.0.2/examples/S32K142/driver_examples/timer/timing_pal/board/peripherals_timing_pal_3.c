/***********************************************************************************************************************
 * This file was generated by the S32 Configuration Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Configuration Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v9.0
processor: S32K142
package_id: S32K142_LQFP100
mcu_data: s32sdk_s32k1xx_rtm_402
processor_version: 0.0.0
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: f680c506-f49b-4dfd-bc04-f34aa7a60054
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "peripherals_timing_pal_3.h"

/*******************************************************************************
 * timing_pal_3 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'timing_pal_3'
- type: 'timing_pal_config'
- mode: 'general'
- custom_name_enabled: 'true'
- type_id: 'timing_pal'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'FTM_0'
- config_sets:
  - timing_pal:
    - timingPalInst:
      - name: 'timingFtmInst0'
      - readonly: 'true'
    - timingPalConfig:
      - name: 'timingFtm_InitConfig0'
      - readonly: 'true'
      - ftmExt:
        - ftmClockSource: 'FTM_CLOCK_SOURCE_EXTERNALCLK'
        - ftmPrescaler: 'FTM_CLOCK_DIVID_BY_128'
        - finalValue: '65535'
      - chanConfigList:
        - 0:
          - channel: '0U'
          - chanType: 'TIMER_CHAN_TYPE_CONTINUOUS'
          - timerCallback: 'timingFtmInst0_callback'
          - callbackParameter: 'NULL'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */

/* PAL instance information */
const timing_instance_t timingFtmInst0 = {
   .instType = TIMING_INST_TYPE_FTM,
   .instIdx = 0
};
/* Channel configuration array */
const timer_chan_config_t timingFtmInst0_chanConfig[1] = {
    /* Channel configuration 0 */
    {
       .channel = 0U,
       .chanType = TIMER_CHAN_TYPE_CONTINUOUS,
       .callback = timingFtmInst0_callback,
       .callbackParam = NULL
    }
};

/* FTM specific extension configuration */
extension_ftm_for_timer_t timingFtmInst0_extension = {
   .clockSelect = FTM_CLOCK_SOURCE_EXTERNALCLK,
   .prescaler = FTM_CLOCK_DIVID_BY_128,
   .finalValue = 65535
};

/* Timing configuration */
const timer_config_t timingFtm_InitConfig0 = {
   .chanConfigArray = timingFtmInst0_chanConfig,
   .numChan = 1U,
   .extension = &timingFtmInst0_extension
};

