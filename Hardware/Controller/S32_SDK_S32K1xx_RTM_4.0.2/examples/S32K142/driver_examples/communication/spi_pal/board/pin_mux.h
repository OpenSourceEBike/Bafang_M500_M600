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
#define PCS1_PORT    PTB
#define PCS1_PIN     0U
#define SCK1_PORT    PTB
#define SCK1_PIN     2U
#define SIN1_PORT    PTB
#define SIN1_PIN     3U
#define SOUT1_PORT    PTB
#define SOUT1_PIN     1U
#define LED_PORT    PTD
#define LED_PIN     15U
/*! @brief User number of configured pins */
#define NUM_OF_CONFIGURED_PINS0 9
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

