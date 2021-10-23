/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is derived from lwIP contribution example with the following copyright:
 *
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 */


/* lwIP includes. */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 2.1, A project shall not contain unreachable code.
 * These are safety checks to avoid dereferencing NULL pointers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.6, typedefs that indicate size and
 * signedness should be used in place of the basic numerical types.
 * These are function parameters whose size is platform dependent.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 4.7, Ignoring return value of function
 * The return value of the function can be ignored, as checks are done on parameters
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.3, All declarations or an object or function shall use the
 * same names and type qualifiers.
 * Function should take NULL as parameter. This is a fake finding.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'dummyptr' could be declared as pointing to const
 * Memory pointed by 'dummyptr' is used for receive.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.1, Unpermitted operand to operator '||'
 * Variable is of essential boolean type
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, The value of an expression shall not be assigned to an
 * object with a narrower essential type or a different essential type category.
 * These are exit codes returned by functions when an error occurs.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.2, Conversions shall not be performed between a pointer
 * to an incomplete type and any other type
 * This cast is to required write 8bit or 16bit data into corresponding address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer
 * to object type and a pointer to a different object type.
 * This cast is to required write 8bit or 16bit data into corresponding address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type .
 * The conversion is needed to wrap a thread function call.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, Conditional expression should have essentially Boolean type.
 * This is required for macro constructs in form do {...} while(0).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 17.7, Ignoring return value of function
 * The return value of the function can be ignored, as checks are done on parameters
 *
 */

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/stats.h"
#include "lwip/debug.h"
#include "lwip/sys.h"
#include "lwip/api.h"

#include "osif.h"

#include "arch/sys_arch.h"
#if defined(USING_OS_FREERTOS)
 /*
  Checks validity of a mutex/semaphore, based on pointer value
 */
static err_t check_synchronization_value(const sys_sem_t *synchronization_value)
{
	err_t xResult = ERR_OK;

	if (NULL == *synchronization_value)
	{
		xResult = ERR_MEM;
	}

	return xResult;
}


/*
    Returns the time elapsed (in milliseconds) since a given time
    param: startTime - the time since we want to calculate how much time has passed since
*/
static u32_t get_elapsed_time(u32_t startTime)
{
    u32_t elapsed;

    elapsed = sys_now() - startTime;
    if (0U == elapsed)
    {
      elapsed = 1U;
    }
    return elapsed;
}

/*-----------------------------------------------------------------------------------
  Creates an empty mailbox for maximum "size" elements.
*/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);

  *mbox = xQueueCreate(size, sizeof(void*));

  return ERR_OK;
}

/*-----------------------------------------------------------------------------------
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
*/
void sys_mbox_free(sys_mbox_t *mbox)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox not empty", !uxQueueMessagesWaiting(*mbox));

  vQueueDelete(*mbox);
}

/*-----------------------------------------------------------------------------------
  Posts the "msg" to the mailbox. This function have to block until
  the "msg" is really posted.
*/
void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  while (pdTRUE != xQueueSend(*mbox, &data, 10000)) /* time is arbitrary */
  {
    ;
  }
}

/*-----------------------------------------------------------------------------------
  Posts the "msg" to the front of the mailbox. This function have to block until
  the "msg" is really posted.
*/
void sys_mbox_post_to_front(sys_mbox_t *mbox, void *data)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  while (pdTRUE != xQueueSendToFront(*mbox, &data, 10000)) /* time is arbitrary */
  {
    ;
  }
}

/*-----------------------------------------------------------------------------------
  Try to post the "msg" to the mailbox. Returns ERR_MEM if this one
  is full, else, ERR_OK if the "msg" is posted.
*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *data)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  err_t status = ERR_OK; 
  
  if (pdTRUE != xQueueSend(*mbox, &data, 10000)) /* time is arbitrary */
  {
      status = ERR_MEM;
  }
  return status;
}

/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP.
*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
  void** msgptr = msg;
  u32_t startTime;

  LWIP_ASSERT("mbox != NULL", mbox != NULL);

  startTime = sys_now();

  if (timeout != 0U)
  {
    if (pdTRUE == xQueueReceive(*mbox, msgptr, pdMS_TO_TICKS(timeout)))
    {
      return get_elapsed_time(startTime);
    }
    else /* timed out blocking for message */
    {
      *msgptr = NULL;
      return SYS_ARCH_TIMEOUT;
    }
  }
  else /* block forever for a message. */
  {
    while (pdTRUE != xQueueReceive(*mbox, msgptr, 10000)) /* time is arbitrary */
    {
      ;
    }
    return get_elapsed_time(startTime);
  }
}

/*-----------------------------------------------------------------------------------*/
/*
  This is similar to sys_arch_mbox_fetch, however if a message is not
  present in the mailbox, it immediately returns with the code
  SYS_MBOX_EMPTY. On success 0 is returned.
*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  void** msgptr = msg;

  LWIP_ASSERT("mbox != NULL", mbox != NULL);

  if (pdTRUE == xQueueReceive(*mbox, msgptr, 0))
  {
    return 0U;
  }
  else /* no message */
  {
    *msgptr = NULL;
    return SYS_MBOX_EMPTY;
  }
}

/*-----------------------------------------------------------------------------------
  Creates and returns a new semaphore. The "count" argument specifies
  the initial state of the semaphore. TBD finish and test
*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  err_t xResult = ERR_OK;
  LWIP_UNUSED_ARG(count);
  LWIP_ASSERT("sem != NULL", sem != NULL);

  portENTER_CRITICAL();
  *sem = xSemaphoreCreateBinary();

  xResult = check_synchronization_value(sem);

  portEXIT_CRITICAL();

  return xResult;
}


/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  u32_t startTime;

  LWIP_ASSERT("sem != NULL", sem != NULL);

  startTime = sys_now();

  if (timeout != 0U)
  {
    if (pdTRUE == xSemaphoreTake(*sem, pdMS_TO_TICKS(timeout)))
    {
        return get_elapsed_time(startTime);
    }
    else
    {
      return SYS_ARCH_TIMEOUT;
    }
  }
  else /* must block without a timeout */
  {
    while (xSemaphoreTake(*sem, 10000) != pdTRUE)
    {
      ;
    }
    return get_elapsed_time(startTime);
  }
}

/*-----------------------------------------------------------------------------------*/
/* Signals a semaphore
*/
void sys_sem_signal(sys_sem_t *sem)
{
  LWIP_ASSERT("sem != NULL", sem != NULL);
  (void)xSemaphoreGive(*sem);
}

/*-----------------------------------------------------------------------------------*/
/* Deallocates a semaphore
*/
void sys_sem_free(sys_sem_t *sem)
{
  LWIP_ASSERT("sem != NULL", sem != NULL);
  vSemaphoreDelete(*sem);
}

/*-----------------------------------------------------------------------------------
  Creates a new mutex. The mutex is allocated to the memory that 'mutex'
  points to (which can be both a pointer or the actual OS structure).
  If the mutex has been created, ERR_OK should be returned. Returning any
  other error will provide a hint what went wrong, but except for assertions,
  no real error handling is implemented.
*/
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    LWIP_ASSERT("mutex != NULL", mutex != NULL);

    portENTER_CRITICAL();
    *mutex = xSemaphoreCreateMutex();
    portEXIT_CRITICAL();

	return check_synchronization_value(mutex);

}

/*-----------------------------------------------------------------------------------
  Blocks the thread until the mutex can be grabbed.
*/
void sys_mutex_lock(sys_mutex_t *mutex)
{
    LWIP_ASSERT("mutex != NULL", mutex != NULL);
    while (xSemaphoreTake(*mutex, 100) == pdFALSE) {}
}

/*-----------------------------------------------------------------------------------
  Releases the mutex previously locked through 'sys_mutex_lock()'
*/
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    LWIP_ASSERT("mutex != NULL", mutex != NULL);
    (void)xSemaphoreGive(*mutex);
}


/*-----------------------------------------------------------------------------------
  Deallocates a mutex
*/
void sys_mutex_free(sys_mutex_t *mutex)
{
    LWIP_ASSERT("mutex != NULL", mutex != NULL);
    vQueueDelete(*mutex);
}


/* Wrap a thread method and its argument for being passed to sys_thread_function */
struct threadfunc {
  lwip_thread_fn function;
  void *arg;
};

/* Utility method that wraps a thread function call and provides a way to add initialization - eg per-thread semaphores
 * Implements design_id_SOCKETS_Activity
 */
static void sys_thread_function(void* arg)
{
  struct threadfunc* t = (struct threadfunc*)arg;
#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
  netconn_thread_init();
#endif
  (void)t->function(t->arg);
  /* Regular FreeRTOS tasks don't return. But in case they do, make them exit cleanly. */
#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
  netconn_thread_cleanup();
#endif
  mem_free(t);
  sys_thread_delete(NULL);
}

/*-----------------------------------------------------------------------------------
  Starts a new thread named "name" with priority "prio" that will begin its
  execution in the function "thread()". The "arg" argument will be passed as an
  argument to the thread() function. The stack size to used for this thread is
  the "stacksize" parameter. The id of the new thread is returned. Both the id
  and the priority are system dependent.
*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
  TaskHandle_t CreatedTask = NULL;
  uint16_t stackDepth = ((uint16_t)stacksize != 0U) ? (uint16_t)stacksize : (uint16_t)configMINIMAL_STACK_SIZE;
  struct threadfunc* tf = (struct threadfunc*)mem_malloc(sizeof (struct threadfunc));

  LWIP_ASSERT("tf != NULL", tf != NULL);

  tf->function = thread;
  tf->arg = arg;

  xTaskCreate(sys_thread_function, name, stackDepth, tf, prio, &CreatedTask);

  return CreatedTask;
}

/*-----------------------------------------------------------------------------------
  Deletes a thread
*/
void sys_thread_delete(sys_thread_t thread)
{
    vTaskDelete(thread);
}

/*
  This optional function does a "fast" critical region protection and returns
  the previous protection level. This function is only called during very short
  critical regions. An embedded system which supports ISR-based drivers might
  want to implement this function by disabling interrupts. Task-based systems
  might want to implement this by using a mutex or disabling tasking. This
  function should support recursive calls from the same task or interrupt. In
  other words, sys_arch_protect() could be called while already protected. In
  that case the return value indicates that it is already protected.

  sys_arch_protect() is only required if your port is supporting an operating
  system.
*/
sys_prot_t sys_arch_protect(void)
{
  portENTER_CRITICAL();
  return 0;
}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
void sys_arch_unprotect(sys_prot_t pval)
{
  (void) pval;
  portEXIT_CRITICAL();
}

#endif /* defined(USING_OS_FREERTOS) */
/* Initialize sys arch layer
*/
void sys_init(void)
{
#if NO_SYS
    /* Initialize timer in baremetal environment before using OSIF_GetMilliseconds */
    OSIF_TimeDelay(0);
#endif
}

/*
   Ticks/jiffies since power up
*/
u32_t sys_jiffies(void)
{
    return (u32_t)OSIF_GetMilliseconds();
}

/*
  Returns the current time in milliseconds,
  may be the same as sys_jiffies or at least based on it.
*/
u32_t sys_now(void)
{
    return sys_jiffies();
}
