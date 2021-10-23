/* 
 * Copyright 2018-2020 NXP
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

#ifndef LPI2C_IRQ_H__
#define LPI2C_IRQ_H__

#include "device_registers.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined (S32K11x_SERIES)

/* LPI2C0 master and slave handler named in startup code. */
void LPI2C0_Master_Slave_IRQHandler(void);

#else

#if (LPI2C_INSTANCE_COUNT > 0u)
/* LPI2C0 master handler named in startup code. */
void LPI2C0_Master_IRQHandler(void);

/* LPI2C0 slave handler named in startup code. */
void LPI2C0_Slave_IRQHandler(void);

#if(LPI2C_INSTANCE_COUNT == 2u)

/* LPI2C1 master handler named in startup code. */
void LPI2C1_Master_IRQHandler(void);

/* LPI2C1 slave handler named in startup code. */
void LPI2C1_Slave_IRQHandler(void);

#endif /* LPI2C_INSTANCE_COUNT == 2u */

#endif /* LPI2C_INSTANCE_COUNT > 0u */

#endif /* endif cpu defined */

#endif /* LPI2C_IRQ_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
