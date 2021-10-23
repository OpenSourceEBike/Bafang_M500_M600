/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * @file osif_freertos.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * The macros are used to validate input parameters to driver functions.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 7.2, Unsigned integer literal without a 'U' suffix
 * Register address defined by FreeRTOS header files.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer variable could be declared as pointing to const
 * Type definition is done in FreeRTOS header files.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * This is required for initializing pointers to the module's memory map, which is located at a
 * fixed address.
 *
 */

#include <stdbool.h>
#include "device_registers.h"
#include "osif.h"
#include "FreeRTOS.h"
#include "task.h"
#include "devassert.h"

#if !defined (USING_OS_FREERTOS)
#error "Wrong OSIF selected. Please define symbol USING_OS_FREERTOS in project settings or change the OSIF variant"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/*! @brief Converts milliseconds to ticks*/
#define MSEC_TO_TICK(msec) (pdMS_TO_TICKS(msec))

/* define a macro to access the FreeRTOS mutex/semaphore handle from an
 * osif mutex_t or semaphore_t */
#if configSUPPORT_STATIC_ALLOCATION == 1
#define SEM_HANDLE(sem) ((sem).handle)
#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
#define SEM_HANDLE(sem) (sem)
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : osif_IsIsrContext
 * Description   : This function returns true if the current execution context
 *  is from an ISR and false if execution is in normal thread mode.
 *
 *END**************************************************************************/
#if FEATURE_OSIF_FREERTOS_ISR_CONTEXT_METHOD == 1
/* Cortex M device - read ICSR[IPSR] value */
static inline bool osif_IsIsrContext(void)
{
    bool is_isr = false;
    uint32_t ipsr_code = (uint32_t)( (S32_SCB->ICSR & S32_SCB_ICSR_VECTACTIVE_MASK) >> S32_SCB_ICSR_VECTACTIVE_SHIFT );
    if (ipsr_code != 0u)
    {
        is_isr = true;
    }

    return is_isr;
}
#elif FEATURE_OSIF_FREERTOS_ISR_CONTEXT_METHOD == 2
/* PowerPC device, for FreeRTOS 9.0.0 read the SPRG0 reg that denotes the interrupt nesting level */
#define SPRG0_ADDR (272)
static inline bool osif_IsIsrContext(void)
{
    uint32_t interrupt_level = MFSPR(SPRG0_ADDR);
    bool is_isr = (bool)(interrupt_level > 0u);
    return is_isr;
}
#else
    #error "No method to check ISR Context"
#endif /* FEATURE_OSIF_FREERTOS_ISR_CONTEXT_METHOD */
/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_TimeDelay
 * Description   : This function blocks (sleep) the current thread for a number
 *  of milliseconds.
 *
 * Implements : OSIF_TimeDelay_freertos_Activity
 *END**************************************************************************/
void OSIF_TimeDelay(uint32_t delay)
{
    /* One dependency for FreeRTOS config file */
    /* INCLUDE_vTaskDelay */
    vTaskDelay(MSEC_TO_TICK(delay));
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_GetMilliseconds
 * Description   : This function returns the number of miliseconds elapsed since
 *                  starting the internal timer (since scheduler was started).
 *
 * Implements : OSIF_GetMilliseconds_freertos_Activity
 *END**************************************************************************/
uint32_t OSIF_GetMilliseconds(void)
{
    /*
     * Return the tick count in miliseconds
     * Note: if configTICK_RATE_HZ is less than 1000, the return value will be truncated
     * to 32-bit wide for large values of the tick count.
     */
    return (uint32_t)((((uint64_t) xTaskGetTickCount()) * 1000u) / configTICK_RATE_HZ);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_MutexLock
 * Description   : This function obtains the mutex lock or returns error if timeout.
 *
 * Implements : OSIF_MutexLock_freertos_Activity
 *END**************************************************************************/
status_t OSIF_MutexLock(const mutex_t * const pMutex,
                        const uint32_t timeout)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be locked - do not use DEV_ASSERT in this case */

    uint32_t timeoutTicks;
    status_t osif_ret_code = STATUS_SUCCESS;
    TaskHandle_t mutex_holder_handle;
    TaskHandle_t current_task_handle;
    BaseType_t operation_status = pdFAIL;

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        /* Two dependencies for FreeRTOS config file */
        /* INCLUDE_xQueueGetMutexHolder */
        /* INCLUDE_xTaskGetCurrentTaskHandle */
        mutex_holder_handle = xSemaphoreGetMutexHolder(mutex_handle);
        current_task_handle = xTaskGetCurrentTaskHandle();

        /* If pMutex has been locked by current task, return error. */
        if (mutex_holder_handle == current_task_handle)
        {
            osif_ret_code = STATUS_ERROR;
        }
        else
        {
            /* Convert timeout from millisecond to tick. */
            if (timeout == OSIF_WAIT_FOREVER)
            {
                timeoutTicks = portMAX_DELAY;
            }
            else
            {
                timeoutTicks = MSEC_TO_TICK(timeout);
            }

            /* Try to take the semaphore */
            operation_status = xSemaphoreTake(mutex_handle, timeoutTicks);

            osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_TIMEOUT;
        }
    }

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_MutexUnlock
 * Description   : This function unlocks the mutex, fails if the current thread
 *  is not the mutex holder.
 *
 * Implements : OSIF_MutexUnlock_freertos_Activity
 *END**************************************************************************/
status_t OSIF_MutexUnlock(const mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be unlocked - do not use DEV_ASSERT in this case */

    status_t osif_ret_code = STATUS_SUCCESS;
    TaskHandle_t mutex_holder_handle;
    TaskHandle_t current_task_handle;
    BaseType_t operation_status = pdFAIL;

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        /* Two dependencies for FreeRTOS config file */
        /* INCLUDE_xQueueGetMutexHolder */
        /* INCLUDE_xTaskGetCurrentTaskHandle */
        mutex_holder_handle = xSemaphoreGetMutexHolder(mutex_handle);
        current_task_handle = xTaskGetCurrentTaskHandle();

        /* If pMutex is not locked by current task, return error. */
        if (mutex_holder_handle != current_task_handle)
        {
            osif_ret_code = STATUS_ERROR;
        }
        else
        {
            operation_status = xSemaphoreGive(mutex_handle);
            osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_ERROR;
        }
    }

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_MutexCreate
 * Description   : This function creates (registers) a mutex object to the OS.
 *
 * Implements : OSIF_MutexCreate_freertos_Activity
 *END**************************************************************************/
status_t OSIF_MutexCreate(mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be created - do not use DEV_ASSERT in this case */

    status_t osif_ret_code = STATUS_SUCCESS;

    if (pMutex != NULL)
    {
#if configSUPPORT_STATIC_ALLOCATION == 1
        pMutex->handle = xSemaphoreCreateMutexStatic(&(pMutex->buffer));
        if (pMutex->handle == NULL)
        {
            osif_ret_code = STATUS_ERROR; /* mutex not created successfully */
        }
#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
        *pMutex = xSemaphoreCreateMutex();
        if (*pMutex == NULL)
        {
            osif_ret_code = STATUS_ERROR; /* mutex not created successfully */
        }
#endif
    }

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_MutexDestroy
 * Description   : This function removes the mutex from the OS (and frees memory).
 *
 * Implements : OSIF_MutexDestroy_freertos_Activity
 *END**************************************************************************/
status_t OSIF_MutexDestroy(const mutex_t * const pMutex)
{
    /* The (pMutex == NULL) case is a valid option, signaling that the mutex does
     * not need to be destroyed - do not use DEV_ASSERT in this case */

    if (pMutex != NULL)
    {
        SemaphoreHandle_t mutex_handle = SEM_HANDLE(*pMutex);
        DEV_ASSERT(mutex_handle);
        vSemaphoreDelete(mutex_handle);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_SemaWait
 * Description   : This function performs the 'wait' (decrement) operation on a semaphore,
 *  returns error if operation times out.
 *
 * Implements : OSIF_SemaWait_freertos_Activity
 *END**************************************************************************/
status_t OSIF_SemaWait(semaphore_t * const pSem,
                       const uint32_t timeout)
{
    DEV_ASSERT(pSem);

    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);
    uint32_t timeoutTicks;
    BaseType_t operation_status;
    status_t osif_ret_code;

    /* Convert timeout from millisecond to ticks. */
    if (timeout == OSIF_WAIT_FOREVER)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(timeout);
    }

    /* Try to take the semaphore */
    operation_status = xSemaphoreTake(sem_handle, timeoutTicks);

    osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_TIMEOUT;

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_SemaPost
 * Description   : This function performs the 'post' (increment) operation on a semaphore.
 *
 * Implements : OSIF_SemaPost_freertos_Activity
 *END**************************************************************************/
status_t OSIF_SemaPost(semaphore_t * const pSem)
{
    DEV_ASSERT(pSem);

    BaseType_t operation_status = pdFAIL;
    status_t osif_ret_code;

    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);
    /* Check if the post operation is executed from ISR context */
    bool is_isr = osif_IsIsrContext();
    if (is_isr)
    {
        /* Execution from exception handler (ISR) */
        BaseType_t taskWoken = pdFALSE;
        operation_status = xSemaphoreGiveFromISR(sem_handle, &taskWoken);

        if (operation_status == pdPASS)
        {
            /* Perform a context switch if necessary */
            portYIELD_FROM_ISR(taskWoken);
        }
    }
    else
    {
        /* Execution from task */
        operation_status = xSemaphoreGive(sem_handle);
    }

    /* pdFAIL in case that the semaphore is full */
    osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_ERROR;

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_SemaCreate
 * Description   : This function creates (registers) a semaphore object to the OS.
 *
 * Implements : OSIF_SemaCreate_freertos_Activity
 *END**************************************************************************/
status_t OSIF_SemaCreate(semaphore_t * const pSem,
                         const uint8_t initValue)
{
    DEV_ASSERT(pSem);

    status_t osif_ret_code = STATUS_SUCCESS;

#if configSUPPORT_STATIC_ALLOCATION == 1
    pSem->handle = xSemaphoreCreateCountingStatic(0xFFu, initValue, &(pSem->buffer));
    if (pSem->handle == NULL)
    {
        osif_ret_code = STATUS_ERROR; /* semaphore not created successfully */
    }
#else /* configSUPPORT_STATIC_ALLOCATION == 0, it's dynamic allocation */
    *pSem = xSemaphoreCreateCounting(0xFFu, initValue);
    if (*pSem == NULL)
    {
        osif_ret_code = STATUS_ERROR; /* semaphore not created successfully */
    }
#endif

    return osif_ret_code;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSIF_SemaDestroy
 * Description   : This function removes a semaphore object from the OS (frees memory).
 *
 * Implements : OSIF_SemaDestroy_freertos_Activity
 *END**************************************************************************/
status_t OSIF_SemaDestroy(const semaphore_t * const pSem)
{
    DEV_ASSERT(pSem);
    SemaphoreHandle_t sem_handle = SEM_HANDLE(*pSem);

    DEV_ASSERT(sem_handle);
    vSemaphoreDelete(sem_handle);

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
