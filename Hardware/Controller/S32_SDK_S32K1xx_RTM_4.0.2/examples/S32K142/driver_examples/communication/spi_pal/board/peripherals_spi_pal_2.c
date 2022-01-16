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
#include "peripherals_spi_pal_2.h"

/*******************************************************************************
 * spi_pal_2 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'spi_pal_2'
- type: 'spi_pal'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'spi_pal'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPSPI_0'
- config_sets:
  - spi_pal:
    - spi_palMasterConfig: []
    - spi_palSlaveConfig:
      - 0:
        - name: 'LPSPI_0_SlaveInitConfig0'
        - frameSize: '8'
        - bitOrder: 'SPI_TRANSFER_MSB_FIRST'
        - clockPolarity: 'SPI_ACTIVE_HIGH'
        - ssPolarity: 'SPI_ACTIVE_LOW'
        - clockPhase: 'READ_ON_ODD_EDGE'
        - transferType: 'SPI_USING_INTERRUPTS'
        - rxDMAChannel: '0'
        - txDMAChannel: '0'
        - callback: 'NULL'
        - callbackParam: 'NULL'
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
/* Define instance structure */
spi_instance_t spi_pal_2_Instance =
{
    .instType = SPI_INST_TYPE_LPSPI,
    .instIdx = 0U
};

/* SPI_PAL Slave Configurations 0 */
spi_slave_t LPSPI_0_SlaveInitConfig0 =
{
    .frameSize        = 8U,
    .bitOrder         = SPI_TRANSFER_MSB_FIRST,
    .clockPolarity    = SPI_ACTIVE_HIGH,
    .ssPolarity       = SPI_ACTIVE_LOW,
    .clockPhase       = READ_ON_ODD_EDGE,
    .transferType     = SPI_USING_INTERRUPTS,
    .rxDMAChannel     = 0U,
    .txDMAChannel     = 0U,
    .callback         = NULL,
    .callbackParam    = NULL,
    .extension        = NULL
};


