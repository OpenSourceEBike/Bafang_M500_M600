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
#define PCS0_PORT    PTB
#define PCS0_PIN     0U
#define SCK0_PORT    PTB
#define SCK0_PIN     2U
#define SIN0_PORT    PTE
#define SIN0_PIN     1U
#define SOUT0_PORT    PTB
#define SOUT0_PIN     4U
#define PCS1_PORT    PTD
#define PCS1_PIN     3U
#define SCK1_PORT    PTD
#define SCK1_PIN     0U
#define SIN1_PORT    PTD
#define SIN1_PIN     1U
#define SOUT1_PORT    PTD
#define SOUT1_PIN     2U
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

