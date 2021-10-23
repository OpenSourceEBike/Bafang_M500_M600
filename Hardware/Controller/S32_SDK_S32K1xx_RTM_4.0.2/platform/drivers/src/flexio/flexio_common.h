/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

#ifndef FLEXIO_COMMON_DRIVER_H
#define FLEXIO_COMMON_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "flexio_hw_access.h"
#include "flexio.h"
#include "edma_driver.h"


 /*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for FLEXIO instances. */
extern FLEXIO_Type * const g_flexioBase[FLEXIO_INSTANCE_COUNT];

/* Pointer to device state structures. This structure contains data common to all drivers on one device */
extern flexio_device_state_t *g_flexioDeviceStatePtr[FLEXIO_INSTANCE_COUNT];

/* Table for FLEXIO IRQ numbers */
extern const IRQn_Type g_flexioIrqId[FLEXIO_INSTANCE_COUNT];

/* PCC clock sources, for getting the input clock frequency */
extern const clock_names_t g_flexioClock[FLEXIO_INSTANCE_COUNT];

/* FlexIO DMA request sources */
extern const uint8_t g_flexioDMASrc[FLEXIO_INSTANCE_COUNT][FEATURE_FLEXIO_MAX_SHIFTER_COUNT];

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*******************************************************************************
* Definitions
******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/


#if defined(__cplusplus)
extern "C" {
#endif


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_InitDriver
 * Description   : Initializes an instance of FlexIO driver
 *
 *END**************************************************************************/
status_t FLEXIO_DRV_InitDriver(uint32_t instance, flexio_common_state_t *driver);


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_DeinitDriver
 * Description   : De-initializes an instance of FlexIO driver
 *
 *END**************************************************************************/
status_t FLEXIO_DRV_DeinitDriver(const flexio_common_state_t *driver);


#if defined(__cplusplus)
}
#endif


#endif /* FLEXIO_COMMON_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
