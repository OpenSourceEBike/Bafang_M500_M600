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
  UUID: a6d43dcb-893a-49cd-9d9d-0bb26ff0f05b
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "peripherals_ewm_config_1.h"

/*******************************************************************************
 * ewm_config_1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'ewm_config_1'
- type: 'ewm_config'
- mode: 'general'
- custom_name_enabled: 'true'
- type_id: 'ewm'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'EWM'
- config_sets:
  - ewm:
    - ewmConfig:
      - 0:
        - name: 'ewm_Config0'
        - readOnly: 'true'
        - interrupt: 'true'
        - prescaler: '251'
        - compareLow: '0'
        - compareHigh: '254'
        - inputPin: 'EWM_IN_ASSERT_ON_LOGIC_ONE'
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

    /* EWM user configuration 0 */
const ewm_init_config_t ewm_Config0 =
{
    .interruptEnable = true,
    .prescaler = 251U,
    .compareLow = 0U,
    .compareHigh = 254U,
    .assertLogic = EWM_IN_ASSERT_ON_LOGIC_ONE
};


