/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
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
  * @lpspi_irq.c
  *
  * @page misra_violations MISRA-C:2012 violations
  *
  * @section [global]
  * Violates MISRA 2012 Required Rule 5.1, Object/function previously declared.
  * This requirement is fulfilled since the function is declared as external in and only in 
  * one configuration C file.
  *
  * @section [global]
  * Violates MISRA 2012 Required Rule 5.2, identifier clash
  * The supported compilers use more than 31 significant characters for identifiers.
  *
  * @section [global]
  * Violates MISRA 2012 Required Rule 5.4, identifier clash
  * The supported compilers use more than 31 significant characters for identifiers.
  *
  * @section [global]
  * Violates MISRA 2012 Required Rule 5.5, identifier clash
  * The supported compilers use more than 31 significant characters for identifiers. 
  *
  * @section [global]
  * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
  * visible when an object or function with external linkage is defined.
  *
  * @section [global]
  * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
  * Function is defined for usage by IRQs handlers and can't be defined as static
  *  
  */
 
#include <assert.h>
#include <stdbool.h>
#include "device_registers.h"
#include "lpspi_shared_function.h"


/*!
 * @addtogroup lpspi_driver Low Power Serial Peripheral Interface (LPSPI)
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (LPSPI_INSTANCE_COUNT == 1U)
/*!
 * @brief This function is the implementation of LPSPI0 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI0_IRQHandler(void);
 
void LPSPI0_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(0U);
}

#elif (LPSPI_INSTANCE_COUNT == 2U)
/*!
 * @brief This function is the implementation of LPSPI0 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI0_IRQHandler(void);
 
void LPSPI0_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(0U);
}

/*!
 * @brief This function is the implementation of LPSPI1 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI1_IRQHandler(void); 
 
void LPSPI1_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(1U);
}

#else

/*!
 * @brief This function is the implementation of LPSPI0 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI0_IRQHandler(void);
 
void LPSPI0_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(0U);
}

/*!
 * @brief This function is the implementation of LPSPI1 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI1_IRQHandler(void);
 
void LPSPI1_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(1U);
}

/*!
 * @brief This function is the implementation of LPSPI2 handler named in startup code.
 *
 * It passes the instance to the shared LPSPI IRQ handler.
 */
void LPSPI2_IRQHandler(void);
 
void LPSPI2_IRQHandler(void)
{
    LPSPI_DRV_IRQHandler(2U);
}

#endif

/*! @} */

/*******************************************************************************
 * EOF
 ******************************************************************************/

