/* ###################################################################
**     Copyright 2018 NXP 
**     All Rights Reserved.
**     
**     NXP Confidential. This software is owned or controlled by NXP and may only be
**     used strictly in accordance with the applicable license terms. By expressly
**     accepting such terms or by downloading, installing, activating and/or otherwise
**     using the software, you are agreeing that you have read, and that you agree to
**     comply with and are bound by, such license terms. If you do not agree to be
**     bound by the applicable license terms, then you may not retain, install,
**     activate or otherwise use the software. The production use license in
**     Section 2.3 is expressly granted for this software.
** ###################################################################*/

/* Component ID, used by S32 Configuration Tool *//* FSL_COMPONENT_ID "platform.os.freertos" *//* SDK_FREERTOS_DRIVER_VERSION (MAKE_VERSION(1, 0, 0)) */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/**
* @page misra_violations MISRA-C:2012 violations
*
 * @section [global]
* Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
* The macro is used for development validation.
*
* @section [global]
* Violates MISRA 2012 Required Rule 3.1, C comment contains C++ comment.
* Detections are URL links from FreeRTOS header text.
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
* The global macro will be used in function call of the module.
*/

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
*
* See http://www.freertos.org/a00110.html.
*----------------------------------------------------------*/


#define configCPU_CLOCK_HZ                          ( 48000000UL )
#define configTICK_RATE_HZ                          ((TickType_t) 1000 )
#define configMAX_PRIORITIES                        5
#define configMINIMAL_STACK_SIZE                    ((unsigned short) 90 )
#define configMAX_TASK_NAME_LEN                     10
#define configUSE_16_BIT_TICKS                      0
#define configIDLE_SHOULD_YIELD                     1
#define configUSE_PREEMPTION                        1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     1
#define configUSE_TASK_NOTIFICATIONS                1
#define configUSE_TIME_SLICING                      1
#define configUSE_NEWLIB_REENTRANT                  0
#define configENABLE_BACKWARD_COMPATIBILITY         1
#define configUSE_POSIX_ERRNO                       0

/* Definition assert() function. */
#define configASSERT(x)                             if((x)==0) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY ( 1 )

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION             0
#define configSUPPORT_DYNAMIC_ALLOCATION            1
#define configTOTAL_HEAP_SIZE                       (( size_t ) 4096 )
#define configAPPLICATION_ALLOCATED_HEAP            0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                         1
#define configUSE_TICK_HOOK                         1
#define configUSE_MALLOC_FAILED_HOOK                1
#define configCHECK_FOR_STACK_OVERFLOW              2
#define configUSE_DAEMON_TASK_STARTUP_HOOK          0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS               0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    
#define portGET_RUN_TIME_COUNTER_VALUE()            xTaskGetTickCount()
#define configUSE_TRACE_FACILITY                    0
#define configUSE_STATS_FORMATTING_FUNCTIONS        1

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             2

/* SEMAPHORES and MUTEXS */
#define configUSE_MUTEXES                           1
#define configUSE_RECURSIVE_MUTEXES                 1
#define configUSE_COUNTING_SEMAPHORES               1

/* Software timer related definitions. */
#define configUSE_TIMERS                            1
#define configTIMER_TASK_PRIORITY                   2
#define configTIMER_QUEUE_LENGTH                    10
#define configTIMER_TASK_STACK_DEPTH                180

/* Tickless Idle Mode */
#define configUSE_TICKLESS_IDLE                     0

/* QUEUE */
#define configQUEUE_REGISTRY_SIZE                   8
#define configUSE_QUEUE_SETS                        0

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                    1
#define INCLUDE_uxTaskPriorityGet                   1
#define INCLUDE_vTaskDelete                         1
#define INCLUDE_vTaskSuspend                        1
#define INCLUDE_vTaskDelayUntil                     1
#define INCLUDE_vTaskDelay                          1
#define INCLUDE_xTaskGetSchedulerState              1
#define INCLUDE_xTaskGetCurrentTaskHandle           1
#define INCLUDE_uxTaskGetStackHighWaterMark         1
#define INCLUDE_xTaskGetIdleTaskHandle              0
#define INCLUDE_eTaskGetState                       1
#define INCLUDE_xEventGroupSetBitFromISR            1
#define INCLUDE_xTimerPendFunctionCall              1
#define INCLUDE_xTaskAbortDelay                     1
#define INCLUDE_xTaskGetHandle                      1
#define INCLUDE_xTaskResumeFromISR                  1
#define INCLUDE_xQueueGetMutexHolder                1


/* Run time stats gathering definitions. */
#ifdef __GNUC__
    /* The #ifdef just prevents this C specific syntax from being included in
    assembly files. */
    void vMainConfigureTimerForRunTimeStats( void );
    unsigned long ulMainGetRunTimeCounterValue( void );
#endif

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
  #define configPRIO_BITS                         __NVIC_PRIO_BITS
#else
  #define configPRIO_BITS                         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY  (0x0F)

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
  #define configKERNEL_INTERRUPT_PRIORITY         (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8-configPRIO_BITS))  
#endif
  
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#ifndef configMAX_SYSCALL_INTERRUPT_PRIORITY
  #define configMAX_SYSCALL_INTERRUPT_PRIORITY    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8-configPRIO_BITS))
#endif

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler                             SVC_Handler
#define xPortPendSVHandler                          PendSV_Handler
#define xPortSysTickHandler                         SysTick_Handler   


#endif /* FREERTOS_CONFIG_H */

