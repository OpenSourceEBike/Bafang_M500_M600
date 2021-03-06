/***********************************************************************************************************************
 * This file was generated by the S32 Config Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef uart_pal_LPUART_1_H
#define uart_pal_LPUART_1_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The global macro will be used in function call of the module.
 *
 */
/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "uart_pal.h"
#include <stddef.h>

/*******************************************************************************
 * Global variables 
 ******************************************************************************/

/* User configuration */

/*! @brief Device instance information */
extern uart_instance_t uart_instance_lu1;

/*! @brief Serial module configurations */

/*! @brief UART PAL User Configuration 0 */
extern const uart_user_config_t LPUART_1_uart_pal_config0;

/*! @brief Define external callbacks for RX and TX */
extern void rxCallback(void *driverState, uart_event_t event, void *userData);



#endif /* uart_pal_LPUART_1_H */
