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


#ifndef LWIP_FREERTOS_ARCH_SYS_ARCH_H
#define LWIP_FREERTOS_ARCH_SYS_ARCH_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 4.9, Function-like macro defined
 * Macro must be redefined for portability between different mcu header files.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 20.9, Undefined preprocessor variable, assumed 0
 * Preprocessor variable will be defined in further implementations or not defined intentionally.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.1, Defined macro 'errno' is reserved to the compiler.
 * This is exactly its intended purpose: to provide a thread-safe errno implementation
 *
 */

/*
 * The stack be used either in a bare metal environment (NO_SYS=1) or in an OS environment (NO_SYS=0).
 * This implementation supports FreeRTOS as operating system (USING_OS_FREERTOS)
*/

/* Sanity checks */
#if NO_SYS
#if defined(USING_OS_FREERTOS)
#error NO_SYS cannot be used with USING_OS_FREERTOS
#endif /* defined(USING_OS_FREERTOS) */
#else /* NO_SYS */
#if defined(USING_OS_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#else /* defined(USING_OS_FREERTOS) */
#error USING_OS_FREERTOS needs to be enabled when NO_SYS=0
#endif /* defined(USING_OS_FREERTOS) */
#endif /* NO_SYS */


#if defined(USING_OS_FREERTOS)

typedef QueueHandle_t               sys_mbox_t;
typedef SemaphoreHandle_t           sys_sem_t;
typedef SemaphoreHandle_t           sys_mutex_t;
typedef TaskHandle_t                sys_thread_t;

#define SYS_MBOX_NULL               (QueueHandle_t)0
#define SYS_SEM_NULL                (SemaphoreHandle_t)0
#define SYS_MUTEX_NULL              SYS_SEM_NULL

#define sys_mbox_valid(mbox)        (((uintptr_t)(mbox) != (uintptr_t)NULL) && (*(mbox) != SYS_SEM_NULL))
#define sys_mbox_set_invalid(mbox)  do { if ((mbox) != NULL) { *(mbox) = SYS_SEM_NULL; }} while(0)

#define sys_sem_valid(sem)          (((sem) != NULL) && (*(sem) != SYS_SEM_NULL))
#define sys_sem_set_invalid(sem)    do {if ((sem) != NULL) { *(sem) = SYS_SEM_NULL; }} while(0)

#define sys_mutex_valid(sem)        sys_sem_valid(sem)
#define sys_mutex_set_invalid(sem)  sys_sem_set_invalid(sem)

/* These functions are not part of the lwIP sys_arch API
   But since the functionality is support by underlying FreeRTOS, they may be useful for implementing netifs */
void sys_mbox_post_to_front(sys_mbox_t *mbox, void *data);
void sys_thread_delete(sys_thread_t thread);

#if LWIP_SOCKET_SET_ERRNO
#ifndef set_errno
#define set_errno(err) do { if (err) { vTaskSetThreadLocalStoragePointer(NULL, 0, (void *)(err)); } } while(0)
#define errno ((int)pvTaskGetThreadLocalStoragePointer(NULL, 0))
#endif /* set_errno */
#endif /* LWIP_SOCKET_SET_ERRNO */

#if LWIP_NETCONN_SEM_PER_THREAD
#define LWIP_NETCONN_THREAD_SEM_GET() ((sys_sem_t*)pvTaskGetThreadLocalStoragePointer(NULL, 1))
#define LWIP_NETCONN_THREAD_SEM_ALLOC() do {                                                                    \
                                                sys_sem_t* pSem = (sys_sem_t*)pvPortMalloc(sizeof(sys_sem_t));  \
                                                if (ERR_OK == sys_sem_new(pSem, 0)) {                           \
                                                    vTaskSetThreadLocalStoragePointer(NULL, 1, (void*)pSem);    \
                                                }                                                               \
                                           } while(0)
#define LWIP_NETCONN_THREAD_SEM_FREE()  do {                                                                    \
                                                sys_sem_t* pSem = LWIP_NETCONN_THREAD_SEM_GET();                \
                                                sys_sem_free(pSem);                                             \
                                                vPortFree(pSem);                                                \
                                                vTaskSetThreadLocalStoragePointer(NULL, 1, NULL);               \
                                           } while(0)
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#endif /* defined(USING_OS_FREERTOS) */

#endif /* LWIP_FREERTOS_ARCH_SYS_ARCH_H */

