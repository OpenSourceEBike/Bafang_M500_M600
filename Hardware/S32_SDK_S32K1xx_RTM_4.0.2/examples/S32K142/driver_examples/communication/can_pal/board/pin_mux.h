#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "pins_driver.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/*! @brief Definitions/Declarations for BOARD_InitPins Functional Group */
/*! @brief User definition pins */
#define CAN_TX_PORT    PTE
#define CAN_TX_PIN     5U
#define CAN_RX_PORT    PTE
#define CAN_RX_PIN     4U
#define led0_evb_PORT    PTD
#define led0_evb_PIN     15U
#define led1_evb_PORT    PTD
#define led1_evb_PIN     16U
#define led0_mb_PORT    PTC
#define led0_mb_PIN     0U
#define led1_mb_PORT    PTC
#define led1_mb_PIN     1U
#define btn2_PORT    PTC
#define btn2_PIN     12U
#define btn1_PORT    PTC
#define btn1_PIN     13U
/*! @brief User number of configured pins */
#define NUM_OF_CONFIGURED_PINS0 8
/*! @brief User configuration structure */
extern pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0];


#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

