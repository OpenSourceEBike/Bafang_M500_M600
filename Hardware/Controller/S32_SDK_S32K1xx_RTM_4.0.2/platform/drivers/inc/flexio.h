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

#ifndef FLEXIO_DRIVER_H
#define FLEXIO_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "osif.h"
#include "status.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * typedef is referenced from other FLEXIO devices.
 */

/*!
 * @addtogroup flexio_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*! @brief Driver type: interrupts/polling/DMA
 * Implements : flexio_driver_type_t_Class
 */
typedef enum
{
    FLEXIO_DRIVER_TYPE_INTERRUPTS    = 0U,  /*!< Driver uses interrupts for data transfers */
    FLEXIO_DRIVER_TYPE_POLLING       = 1U,  /*!< Driver is based on polling */
    FLEXIO_DRIVER_TYPE_DMA           = 2U,  /*!< Driver uses DMA for data transfers */
} flexio_driver_type_t;


/*******************************************************************************
* Definitions
******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/*
 * FlexIO interrupt service routine
 */
typedef void (*flexio_isr_t)(void *isrParam);

/*
 * FlexIO common context structure
 * This is a common structure used by all FlexIO drivers as a part of their context structure.
 * It is needed for common operations such as interrupt handling.
 */
typedef struct
{
    flexio_isr_t isr;         /* Interrupt handler for this driver instance */
    uint32_t instance;        /* FlexIO device instance number */
    uint8_t resourceCount;    /* Count of internal resources used (shifters and timers) */
    uint8_t resourceIndex;    /* Index of first used internal resource instance (shifter and timer) */
} flexio_common_state_t;

/*
 * FlexIO device context structure
 * This is a structure containing data common to all drivers on one device
 */
typedef struct
{
    uint8_t resourceAllocation;    /* Mask to keep track of resources allocated on current device */
    mutex_t resourceLock;          /* Mutex for guarding channel allocation. */
    /* Array of pointers to runtime state structures. Each FlexIO instance can have at most
       one driver instance per shifter. */
    flexio_common_state_t *flexioStatePtr[FEATURE_FLEXIO_MAX_SHIFTER_COUNT];
} flexio_device_state_t;

/*! @endcond */


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLEXIO_I2C Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the FlexIO device
 *
 * This function resets the FlexIO device, enables interrupts
 * in interrupt manager and enables the device.
 *
 * @param instance     FLEXIO peripheral instance number
 * @param deviceState  Pointer to the FLEXIO device context structure. The driver uses
 *                     this memory area for its internal logic. The application must make no
 *                     assumptions about the content of this structure, and must not free this
 *                     memory until the device is de-initialized using FLEXIO_DRV_DeinitDevice().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_DRV_InitDevice(uint32_t instance, flexio_device_state_t *deviceState);


/*!
 * @brief De-initializes the FlexIO device
 *
 * This function de-initializes the FlexIO device.
 *
 * @param instance  FLEXIO peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLEXIO_DRV_DeinitDevice(uint32_t instance);


/*!
 * @brief Resets the FlexIO device
 *
 * This function resets the FlexIO device.
 *
 * @param instance  FLEXIO peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLEXIO_DRV_Reset(uint32_t instance);



/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLEXIO_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
