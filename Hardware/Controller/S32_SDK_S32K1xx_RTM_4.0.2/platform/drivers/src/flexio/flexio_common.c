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

#include "device_registers.h"
#include "flexio_common.h"
#include "interrupt_manager.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for initializing pointers to the module's memory map, which is located at a
 * fixed address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * This is required for initializing pointers to the module's memory map, which is located at a
 * fixed address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code
 * structure and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable 'g_flexioDMASrc' at block scope
 * The variable is used in all flexio drivers so it must remain global.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for FLEXIO instances. */
FLEXIO_Type * const g_flexioBase[FLEXIO_INSTANCE_COUNT] = FLEXIO_BASE_PTRS;

/* Pointer to device state structures. This structure contains data common to all drivers on one device */
flexio_device_state_t *g_flexioDeviceStatePtr[FLEXIO_INSTANCE_COUNT] = {NULL};

/* Table for FLEXIO IRQ numbers */
const IRQn_Type g_flexioIrqId[FLEXIO_INSTANCE_COUNT] = FLEXIO_IRQS;

/* PCC clock sources, for getting the input clock frequency */
const clock_names_t g_flexioClock[FLEXIO_INSTANCE_COUNT] = {FLEXIO0_CLK};

/* FlexIO DMA request sources */
const uint8_t g_flexioDMASrc[FLEXIO_INSTANCE_COUNT][FEATURE_FLEXIO_MAX_SHIFTER_COUNT] =
    {{(uint8_t)FEATURE_FLEXIO_DMA_REQ_0, (uint8_t)FEATURE_FLEXIO_DMA_REQ_1,
      (uint8_t)FEATURE_FLEXIO_DMA_REQ_2, (uint8_t)FEATURE_FLEXIO_DMA_REQ_3}};


/*******************************************************************************
 * Code
 ******************************************************************************/


/* Declaration of FLEXIO_IRQHandler. Needed just to avoid MISRA violation. */
void FLEXIO_IRQHandler(void);


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_InitDevice
 * Description   : Initializes the FlexIO device
 * Implements : FLEXIO_DRV_InitDevice_Activity
 *
 *END**************************************************************************/
status_t FLEXIO_DRV_InitDevice(uint32_t instance, flexio_device_state_t *deviceState)
{
    FLEXIO_Type *baseAddr;
    uint8_t count;
    status_t osifStat = STATUS_SUCCESS;

    if (g_flexioDeviceStatePtr[instance] == NULL)
    {
        baseAddr = g_flexioBase[instance];
        /* Create resource allocation mutex */
        osifStat = OSIF_MutexCreate(&(deviceState->resourceLock));
        DEV_ASSERT(osifStat == STATUS_SUCCESS);
        /* Reset FlexIO module */
        FLEXIO_Init(baseAddr);
        /* Enable FLEXIO interrupt in the interrupt manager */
        INT_SYS_EnableIRQ(g_flexioIrqId[instance]);
        /* Enable module */
        FLEXIO_SetEnable(baseAddr, true);
        /* Initialize device structure */
        deviceState->resourceAllocation = 0U;
        /* Clear state structures array */
        for (count = 0U; count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT; count++)
        {
            deviceState->flexioStatePtr[count] = NULL;
        }
        g_flexioDeviceStatePtr[instance] = deviceState;
    }

    (void)osifStat;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_DeinitDevice
 * Description   : De-initializes the FlexIO device
 *
 * Implements : FLEXIO_DRV_DeinitDevice_Activity
 *END**************************************************************************/
status_t FLEXIO_DRV_DeinitDevice(uint32_t instance)
{
    FLEXIO_Type *baseAddr;

    baseAddr = g_flexioBase[instance];
    /* Reset FlexIO module */
    FLEXIO_Init(baseAddr);
    /* Disable FLEXIO interrupt in the interrupt manager */
    INT_SYS_DisableIRQ(g_flexioIrqId[instance]);
    /* Destroy resource allocation mutex */
    (void)OSIF_MutexDestroy(&(g_flexioDeviceStatePtr[instance]->resourceLock));
    /* Free resources */
    g_flexioDeviceStatePtr[instance] = NULL;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_Reset
 * Description   : Resets the FlexIO device
 *
 * Implements : FLEXIO_DRV_Reset_Activity
 *END**************************************************************************/
status_t FLEXIO_DRV_Reset(uint32_t instance)
{
    FLEXIO_Type *baseAddr;

    baseAddr = g_flexioBase[instance];
    /* Reset FlexIO module */
    FLEXIO_Init(baseAddr);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_Allocate
 * Description   : Allocate timers and shifters for a new driver instance
 *
 *END**************************************************************************/
static status_t FLEXIO_DRV_Allocate(uint32_t instance, flexio_common_state_t *driver)
{
    uint8_t count;                  /* used to iterate through resources (shifters/timers) */
    uint8_t step;                   /* keeps track of how many resources are needed for this driver */
    uint8_t size;                   /* total number of resources */
    uint8_t mask;                  /* bit-mask corresponding to current resources */
    uint8_t resourceAllocation;     /* current resource allocation map */
    const FLEXIO_Type *baseAddr;    /* base address of the FlexIO module */
    status_t status = STATUS_ERROR; /* assume the worst: no resources found for this driver */

    /* Find free resources for a new driver. Drivers may need one or two adjacent shifters and timers */
    baseAddr = g_flexioBase[instance];
    resourceAllocation = g_flexioDeviceStatePtr[instance]->resourceAllocation;
    step = driver->resourceCount;
    size = FLEXIO_GetShifterNum(baseAddr);
    count = 0U;
    mask = (uint8_t)((1U << step) - 1U);
    (void)OSIF_MutexLock(&(g_flexioDeviceStatePtr[instance]->resourceLock), OSIF_WAIT_FOREVER);

    /* find available shifters and timers for the driver */
    while ((status == STATUS_ERROR) && (((uint8_t)(count + step))<= size))
    {
        if ((resourceAllocation & mask) == 0U)
        {
            /* resources found */
            driver->resourceIndex = count;
            /* mark resources as busy */
            g_flexioDeviceStatePtr[instance]->resourceAllocation |= mask;
            status = STATUS_SUCCESS;
        }
        /* continue searching */
        count += 1U;
        mask <<= 1;
    }

    (void)OSIF_MutexUnlock(&(g_flexioDeviceStatePtr[instance]->resourceLock));

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_Deallocate
 * Description   : De-allocate timers and shifters for a driver instance
 *
 *END**************************************************************************/
static void FLEXIO_DRV_Deallocate(uint32_t instance, const flexio_common_state_t *driver)
{
    uint8_t mask;

    mask = (uint8_t)((1U << driver->resourceCount) - 1U);
    mask <<= driver->resourceIndex;
    (void)OSIF_MutexLock(&(g_flexioDeviceStatePtr[instance]->resourceLock), OSIF_WAIT_FOREVER);
    g_flexioDeviceStatePtr[instance]->resourceAllocation &= (uint8_t)~mask;
    (void)OSIF_MutexUnlock(&(g_flexioDeviceStatePtr[instance]->resourceLock));
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_ResourcesInit
 * Description   : Initializes the resources for the current driver
 *
 *END**************************************************************************/
static void FLEXIO_DRV_ResourcesInit(uint32_t instance, const flexio_common_state_t *driver)
{
    uint8_t resource;
    uint8_t resourceCount;
    uint8_t resourceIndex;
    FLEXIO_Type *baseAddr;

    baseAddr = g_flexioBase[instance];
    resourceCount = driver->resourceCount;
    resourceIndex = driver->resourceIndex;
    for (resource = resourceIndex; resource < (resourceIndex + resourceCount); resource++)
    {
        /* Ensure all shifters/timers are disabled */
        FLEXIO_SetShifterMode(baseAddr, resource, FLEXIO_SHIFTER_MODE_DISABLED);
        FLEXIO_SetTimerMode(baseAddr, resource, FLEXIO_TIMER_MODE_DISABLED);
        /* Ensure all interrupts and DMA requests are disabled */
        FLEXIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        FLEXIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        FLEXIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << resource), false);
        FLEXIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << resource), false);
        /* Clear any leftover flags */
        FLEXIO_ClearShifterStatus(baseAddr, resource);
        FLEXIO_ClearShifterErrorStatus(baseAddr, resource);
        FLEXIO_ClearTimerStatus(baseAddr, resource);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_InitDriver
 * Description   : Initializes an instance of FlexIO driver
 *
 *END**************************************************************************/
status_t FLEXIO_DRV_InitDriver(uint32_t instance, flexio_common_state_t *driver)
{
    uint16_t count;
    status_t retCode;
    flexio_common_state_t **flexioStatePtr;

    /* allocate times and shifters for the driver */
    retCode = FLEXIO_DRV_Allocate(instance, driver);
    if (retCode != STATUS_SUCCESS)
    {   /* no more resources available */
        return retCode;
    }

    /* get driver list for this device instance */
    flexioStatePtr = g_flexioDeviceStatePtr[instance]->flexioStatePtr;
    /* find an empty state structure slot for the driver */
    for (count = 0U; count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT; count++)
    {
        if (flexioStatePtr[count] == NULL)
        {
            /* found it, place the new driver struct here */
            flexioStatePtr[count] = driver;
            break;
        }
    }
    /* no need to check if there was room since allocation was successful */
    DEV_ASSERT(count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT);

    /* initialize the allocated resources */
    FLEXIO_DRV_ResourcesInit(instance, driver);

    /* init the rest of the common state structure */
    driver->isr = NULL;
    driver->instance = instance;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_DRV_DeinitDriver
 * Description   : De-initializes an instance of FlexIO driver
 *
 *END**************************************************************************/
status_t FLEXIO_DRV_DeinitDriver(const flexio_common_state_t *driver)
{
    uint16_t count;
    uint32_t instance;
    flexio_common_state_t **flexioStatePtr;

    instance = driver->instance;
    /* get driver list for this device instance */
    flexioStatePtr = g_flexioDeviceStatePtr[instance]->flexioStatePtr;
    /* find the driver in the list */
    for (count = 0U; count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT; count++)
    {
        if (flexioStatePtr[count] == driver)
        {
            /* found it - remove it */
            flexioStatePtr[count] = NULL;
            break;
        }
    }
    DEV_ASSERT(count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT);

    /* de-allocate timers and shifters for the driver */
    FLEXIO_DRV_Deallocate(instance, driver);

    return STATUS_SUCCESS;
}


#if (FLEXIO_INSTANCE_COUNT > 0U)
/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXIO_IRQHandler
 * Description   : Implementation of FLEXIO interrupt handler named in start-up code
 *
 *END**************************************************************************/
void FLEXIO_IRQHandler(void)
{
    uint16_t count;
    uint32_t instance;
    uint32_t resourceMask;
    uint32_t shifterEvents;
    uint32_t shifterErrorEvents;
    uint32_t timerEvents;
    uint32_t enabledInterrupts;
    const FLEXIO_Type *baseAddr;
    flexio_common_state_t * const *driverList;
    flexio_common_state_t *driverState;

    instance = 0U;
    baseAddr = g_flexioBase[instance];
    /* get masks of FlexIO events */
    /* read enabled interrupts in a separate instruction to avoid MISRA violation */
    enabledInterrupts = FLEXIO_GetAllShifterInterrupt(baseAddr);
    shifterEvents = FLEXIO_GetAllShifterStatus(baseAddr) & enabledInterrupts;
    enabledInterrupts = FLEXIO_GetAllShifterErrorInterrupt(baseAddr);
    shifterErrorEvents = FLEXIO_GetAllShifterErrorStatus(baseAddr) & enabledInterrupts;
    enabledInterrupts = FLEXIO_GetAllTimerInterrupt(baseAddr);
    timerEvents = FLEXIO_GetAllTimerStatus(baseAddr) & enabledInterrupts;
    /* get driver list for this device instance */
    driverList = g_flexioDeviceStatePtr[instance]->flexioStatePtr;
    /* check which driver instances need to be serviced */
    for (count = 0U; count < FEATURE_FLEXIO_MAX_SHIFTER_COUNT; count++)
    {
        driverState = driverList[count];
        /* check if driver is initialized and uses interrupts */
        if ((driverState != NULL) && (driverState->isr != NULL))
        {
            /* compute mask of shifters/timers used by this driver */
            resourceMask = ((1UL << driverState->resourceCount) - 1U) << driverState->resourceIndex;
            /* check if this instance has any pending events */
            if (((shifterEvents & resourceMask) != 0U) ||
                ((shifterErrorEvents & resourceMask) != 0U) ||
                ((timerEvents & resourceMask) != 0U))
            {
                /* there is an event for the current instance - call the isr */
                (driverState->isr)(driverState);
            }
        }
    }
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
