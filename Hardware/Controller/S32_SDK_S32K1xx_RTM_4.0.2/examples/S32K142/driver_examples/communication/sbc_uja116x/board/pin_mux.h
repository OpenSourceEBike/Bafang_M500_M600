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
#define PTD0_PORT    PTD
#define PTD0_PIN     0U
#define PTE4_PORT    PTE
#define PTE4_PIN     4U
#define PTD16_PORT    PTD
#define PTD16_PIN     16U
#define PTD15_PORT    PTD
#define PTD15_PIN     15U
#define PTC1_PORT    PTC
#define PTC1_PIN     1U
#define PTC0_PORT    PTC
#define PTC0_PIN     0U
#define PTC13_PORT    PTC
#define PTC13_PIN     13U
#define PTC12_PORT    PTC
#define PTC12_PIN     12U
#define PTB17_PORT    PTB
#define PTB17_PIN     17U
#define PTB16_PORT    PTB
#define PTB16_PIN     16U
#define PTB15_PORT    PTB
#define PTB15_PIN     15U
#define PTB14_PORT    PTB
#define PTB14_PIN     14U
/*! @brief User number of configured pins */
#define NUM_OF_CONFIGURED_PINS0 12
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

