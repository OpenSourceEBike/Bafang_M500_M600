/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function addHeaderCommentsFreeRTOSConfig(){
    var outputString = "";
        outputString += "/* ###################################################################\n";
        outputString += "**     Copyright 2018 NXP \n";
        outputString += "**     All Rights Reserved.\n";
        outputString += "**     \n";
        outputString += "**     NXP Confidential. This software is owned or controlled by NXP and may only be\n";
        outputString += "**     used strictly in accordance with the applicable license terms. By expressly\n";
        outputString += "**     accepting such terms or by downloading, installing, activating and/or otherwise\n";
        outputString += "**     using the software, you are agreeing that you have read, and that you agree to\n";
        outputString += "**     comply with and are bound by, such license terms. If you do not agree to be\n";
        outputString += "**     bound by the applicable license terms, then you may not retain, install,\n";
        outputString += "**     activate or otherwise use the software. The production use license in\n";
        outputString += "**     Section 2.3 is expressly granted for this software.\n";
        outputString += "** ###################################################################*/\n";
        outputString += "\n";
        outputString += "/* Component ID, used by S32 Configuration Tool */";
        outputString += "/* FSL_COMPONENT_ID \"platform.os.freertos\" */";
        outputString += "/* SDK_FREERTOS_DRIVER_VERSION (MAKE_VERSION(1, 0, 0)) */";
        outputString += "\n";
        outputString += "#ifndef FREERTOS_CONFIG_H\n";
        outputString += "#define FREERTOS_CONFIG_H\n";
        outputString += "\n";
        outputString += "/**\n";
        outputString += "* @page misra_violations MISRA-C:2012 violations\n";
        outputString += "*\n";
        outputString += " * @section [global]\n";
        outputString += "* Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.\n";
        outputString += "* The macro is used for development validation.\n";
        outputString += "*\n";
        outputString += "* @section [global]\n";
        outputString += "* Violates MISRA 2012 Required Rule 3.1, C comment contains C++ comment.\n";
        outputString += "* Detections are URL links from FreeRTOS header text.\n";
        outputString += "*\n";
        outputString += "* @section [global]\n"
        outputString += "* Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n"
        outputString += "* The global macro will be used in function call of the module.\n"
        outputString += "*/\n";
        outputString += "\n";
        outputString += "/*-----------------------------------------------------------\n";
        outputString += "* Application specific definitions.\n";
        outputString += "*\n";
        outputString += "* These definitions should be adjusted for your particular hardware and\n";
        outputString += "* application requirements.\n";
        outputString += "*\n";
        outputString += "* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE\n";
        outputString += "* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.\n";
        outputString += "*\n";
        outputString += "* See http://www.freertos.org/a00110.html.\n";
        outputString += "*----------------------------------------------------------*/\n";
        outputString += "\n";
    return outputString;
}

function addFooterCommentsFreeRTOSConfig(){
    var outputString = "";

    outputString += "/* Run time stats gathering definitions. */\n";
    outputString += "#ifdef __GNUC__\n";
    outputString += "    /* The #ifdef just prevents this C specific syntax from being included in\n";
    outputString += "    assembly files. */\n";
    outputString += "    void vMainConfigureTimerForRunTimeStats( void );\n";
    outputString += "    unsigned long ulMainGetRunTimeCounterValue( void );\n";
    outputString += "#endif\n";
    outputString += "\n";
    outputString += "/* Cortex-M specific definitions. */\n";
    outputString += "#ifdef __NVIC_PRIO_BITS\n";
    outputString += "  #define configPRIO_BITS                         __NVIC_PRIO_BITS\n";
    outputString += "#else\n";
    outputString += "  #define configPRIO_BITS                         4\n";
    outputString += "#endif\n";
    outputString += "\n";
    outputString += "/* The lowest interrupt priority that can be used in a call to a \"set priority\"\n";
    outputString += "function. */\n";
    outputString += "#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY  (0x0F)\n";
    outputString += "\n";
    outputString += "/* Interrupt priorities used by the kernel port layer itself.  These are generic\n";
    outputString += "to all Cortex-M ports, and do not rely on any particular library functions. */\n";
    outputString += "#ifndef configKERNEL_INTERRUPT_PRIORITY\n";
    outputString += "  #define configKERNEL_INTERRUPT_PRIORITY         (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8-configPRIO_BITS))  \n";
    outputString += "#endif\n";
    outputString += "  \n";
    outputString += "/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!\n";
    outputString += "See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */\n";
    outputString += "#ifndef configMAX_SYSCALL_INTERRUPT_PRIORITY\n";
    outputString += "  #define configMAX_SYSCALL_INTERRUPT_PRIORITY    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8-configPRIO_BITS))\n";
    outputString += "#endif\n";
    outputString += "\n";
    outputString += "/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS\n";
    outputString += "standard names. */\n";
    outputString += "#define vPortSVCHandler                             SVC_Handler\n";
    outputString += "#define xPortPendSVHandler                          PendSV_Handler\n";
    outputString += "#define xPortSysTickHandler                         SysTick_Handler   \n";
    outputString += "\n";
    outputString += "\n";
    outputString += "#endif /* FREERTOS_CONFIG_H */\n";
    return outputString;
}

function addDefineFreeRTOSConfig(what,value){
    var outputString = "";
    var noChars = 52;
    value +="";
    outputString += "#define "+what;
    if(noChars > (8 + what.length )){
        var N = noChars - 8 - what.length ;
        for(var i = 0; i< N ;i++){
            outputString += " ";
        }
    }else{
        outputString += " ";
    }

    outputString += value+"\n";
    return outputString;
}

function addMsgFreeRTOSConfig(msg){
    return msg+"\n";
}

// SWT component code generator class.
function freertos_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    this.applyToolchainSettings = function(profile) {
       try{
            scriptApi.logInfo("applyToolchainSettings");
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }

    // Returns the class name.
    this.toString = function () {
        return "freertos_CodeGenerator";
    }

    // Generate the configuration file
    this.genHCfgFile = function(hCfgFileSections) {
        var outputString = "";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var coreId = childContext.getFunctionalGroup().getCore();

        var initStructGeneral = getSetting(this.uiConfig, this.uiConfig.getName()+".general_tab");
        var  configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY = initStructGeneral.getChildById("configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY").getValue();
        if (initStructGeneral.getChildById("configCPU_CLOCK_HZ").getValue() == "0"){
            initStructGeneral.getChildById("configCPU_CLOCK_HZ").setValue(initStructGeneral.getChildById("configCPU_CLOCK_HZ_Default").getValue());
        }
        var  configCPU_CLOCK_HZ = initStructGeneral.getChildById("configCPU_CLOCK_HZ").getValue();
        var  configTICK_RATE_HZ = initStructGeneral.getChildById("configTICK_RATE_HZ").getValue();
        var  configMAX_PRIORITIES = initStructGeneral.getChildById("configMAX_PRIORITIES").getValue();
        var  configMINIMAL_STACK_SIZE = initStructGeneral.getChildById("configMINIMAL_STACK_SIZE").getValue();
        var  configMAX_TASK_NAME_LEN = initStructGeneral.getChildById("configMAX_TASK_NAME_LEN").getValue();
        var  configUSE_16_BIT_TICKS = initStructGeneral.getChildById("configUSE_16_BIT_TICKS").getValue();
        var  configIDLE_SHOULD_YIELD = initStructGeneral.getChildById("configIDLE_SHOULD_YIELD").getValue();
        var  configUSE_PREEMPTION = initStructGeneral.getChildById("configUSE_PREEMPTION").getValue();
        var  configNUM_THREAD_LOCAL_STORAGE_POINTERS = initStructGeneral.getChildById("configNUM_THREAD_LOCAL_STORAGE_POINTERS").getValue();
        var  configUSE_PORT_OPTIMISED_TASK_SELECTION = initStructGeneral.getChildById("configUSE_PORT_OPTIMISED_TASK_SELECTION").getValue();
        var  configUSE_TASK_NOTIFICATIONS = initStructGeneral.getChildById("configUSE_TASK_NOTIFICATIONS").getValue();
        var  configUSE_TIME_SLICING = initStructGeneral.getChildById("configUSE_TIME_SLICING").getValue();
        var  configUSE_NEWLIB_REENTRANT = initStructGeneral.getChildById("configUSE_NEWLIB_REENTRANT").getValue();
        var  configENABLE_BACKWARD_COMPATIBILITY = initStructGeneral.getChildById("configENABLE_BACKWARD_COMPATIBILITY").getValue();
        var  configUSE_POSIX_ERRNO = initStructGeneral.getChildById("configUSE_POSIX_ERRNO").getValue();
        var  configMESSAGE_BUFFER_LENGTH_TYPE = initStructGeneral.getChildById("configMESSAGE_BUFFER_LENGTH_TYPE").getValue();
        var  boolConfigASSERT = initStructGeneral.getChildById("configASSERT_id").getChildById("boolConfigASSERT").getValue();
        var  configASSERT = initStructGeneral.getChildById("configASSERT_id").getChildById("configASSERT").getValue();

        var initStructMEMORY = getSetting(this.uiConfig, this.uiConfig.getName()+".memory_tab");
        var configSUPPORT_STATIC_ALLOCATION = initStructMEMORY.getChildById("configSUPPORT_STATIC_ALLOCATION").getValue();
        var configSUPPORT_DYNAMIC_ALLOCATION= initStructMEMORY.getChildById("configSUPPORT_DYNAMIC_ALLOCATION").getValue();
        var configTOTAL_HEAP_SIZE= initStructMEMORY.getChildById("configTOTAL_HEAP_SIZE").getValue();
        var configAPPLICATION_ALLOCATED_HEAP= initStructMEMORY.getChildById("configAPPLICATION_ALLOCATED_HEAP").getValue();
        var memory_scheme= initStructMEMORY.getChildById("memory_scheme_id").getValue();

        var initStructHFRD = getSetting(this.uiConfig, this.uiConfig.getName()+".hook_tab");
        var configUSE_IDLE_HOOK = initStructHFRD.getChildById("configUSE_IDLE_HOOK").getValue();
        var configUSE_TICK_HOOK= initStructHFRD.getChildById("configUSE_TICK_HOOK").getValue();
        var configCHECK_FOR_STACK_OVERFLOW= initStructHFRD.getChildById("configCHECK_FOR_STACK_OVERFLOW").getValue();
        var configUSE_MALLOC_FAILED_HOOK= initStructHFRD.getChildById("configUSE_MALLOC_FAILED_HOOK").getValue();
        var configUSE_DAEMON_TASK_STARTUP_HOOK = initStructHFRD.getChildById("configUSE_DAEMON_TASK_STARTUP_HOOK").getValue();

        var initStructRttsgrd = getSetting(this.uiConfig, this.uiConfig.getName()+".rttsgrd_tab");
        var configGENERATE_RUN_TIME_STATS = initStructRttsgrd.getChildById("configGENERATE_RUN_TIME_STATS_id").getChildById("configGENERATE_RUN_TIME_STATS").getValue();
        var portCONFIGURE_TIMER_FOR_RUN_TIME_STATS= initStructRttsgrd.getChildById("configGENERATE_RUN_TIME_STATS_id").getChildById("portCONFIGURE_TIMER_FOR_RUN_TIME_STATS").getValue();
        var portGET_RUN_TIME_COUNTER_VALUE= initStructRttsgrd.getChildById("configGENERATE_RUN_TIME_STATS_id").getChildById("portGET_RUN_TIME_COUNTER_VALUE").getValue();
        var configUSE_TRACE_FACILITY= initStructRttsgrd.getChildById("configUSE_TRACE_FACILITY").getValue();
        var configUSE_STATS_FORMATTING_FUNCTIONS= initStructRttsgrd.getChildById("configUSE_STATS_FORMATTING_FUNCTIONS").getValue();

        var initStructCOroutines = getSetting(this.uiConfig, this.uiConfig.getName()+".co_routines_tab");
        var configUSE_CO_ROUTINES = initStructCOroutines.getChildById("configUSE_CO_ROUTINES").getValue();
        var configMAX_CO_ROUTINE_PRIORITIES= initStructCOroutines.getChildById("configMAX_CO_ROUTINE_PRIORITIES").getValue();

        var initStructSandM = getSetting(this.uiConfig, this.uiConfig.getName()+".sandm_tab");
        var configUSE_MUTEXES = initStructSandM.getChildById("configUSE_MUTEXES").getValue();
        var configUSE_RECURSIVE_MUTEXES= initStructSandM.getChildById("configUSE_RECURSIVE_MUTEXES").getValue();
        var configUSE_COUNTING_SEMAPHORES= initStructSandM.getChildById("configUSE_COUNTING_SEMAPHORES").getValue();

        var initStructST = getSetting(this.uiConfig, this.uiConfig.getName()+".st_tab");
        var configUSE_TIMERS = initStructST.getChildById("configUSE_TIMERS").getValue();
        var configTIMER_TASK_PRIORITY= initStructST.getChildById("configTIMER_TASK_PRIORITY").getValue();
        var configTIMER_QUEUE_LENGTH= initStructST.getChildById("configTIMER_QUEUE_LENGTH").getValue();
        var configTIMER_TASK_STACK_DEPTH= initStructST.getChildById("configTIMER_TASK_STACK_DEPTH").getValue();

        var initStructTidleM = getSetting(this.uiConfig, this.uiConfig.getName()+".tidlem_tab");
        var configUSE_TICKLESS_IDLE = initStructTidleM.getChildById("configUSE_TICKLESS_IDLE").getValue();
        var configEXPECTED_IDLE_TIME_BEFORE_SLEEP= initStructTidleM.getChildById("configEXPECTED_IDLE_TIME_BEFORE_SLEEP").getValue();
        var configPRE_SLEEP_PROCESSING= initStructTidleM.getChildById("configPRE_SLEEP_PROCESSING").getValue();
        var configPOST_SLEEP_PROCESSING= initStructTidleM.getChildById("configPOST_SLEEP_PROCESSING").getValue();

        var initStructQUEUE = getSetting(this.uiConfig, this.uiConfig.getName()+".queue_tab");
        var configQUEUE_REGISTRY_SIZE = initStructQUEUE.getChildById("configQUEUE_REGISTRY_SIZE").getValue();
        var configUSE_QUEUE_SETS= initStructQUEUE.getChildById("configUSE_QUEUE_SETS").getValue();

        var initStructAPI = getSetting(this.uiConfig, this.uiConfig.getName()+".api_tab");
        var INCLUDE_vTaskPrioritySet = initStructAPI.getChildById("INCLUDE_vTaskPrioritySet").getValue();
        var INCLUDE_uxTaskPriorityGet = initStructAPI.getChildById("INCLUDE_uxTaskPriorityGet").getValue();
        var INCLUDE_vTaskDelete = initStructAPI.getChildById("INCLUDE_vTaskDelete").getValue();
        var INCLUDE_vTaskSuspend = initStructAPI.getChildById("INCLUDE_vTaskSuspend").getValue();
        var INCLUDE_vTaskDelayUntil = initStructAPI.getChildById("INCLUDE_vTaskDelayUntil").getValue();
        var INCLUDE_vTaskDelay = initStructAPI.getChildById("INCLUDE_vTaskDelay").getValue();
        var INCLUDE_xTaskGetSchedulerState = initStructAPI.getChildById("INCLUDE_xTaskGetSchedulerState").getValue();
        var INCLUDE_xTaskGetCurrentTaskHandle = initStructAPI.getChildById("INCLUDE_xTaskGetCurrentTaskHandle").getValue();
        var INCLUDE_uxTaskGetStackHighWaterMark = initStructAPI.getChildById("INCLUDE_uxTaskGetStackHighWaterMark").getValue();
        var INCLUDE_xTaskGetIdleTaskHandle = initStructAPI.getChildById("INCLUDE_xTaskGetIdleTaskHandle").getValue();
        var INCLUDE_eTaskGetState = initStructAPI.getChildById("INCLUDE_eTaskGetState").getValue();
        var INCLUDE_xEventGroupSetBitFromISR = initStructAPI.getChildById("INCLUDE_xEventGroupSetBitFromISR").getValue();
        var INCLUDE_xTimerPendFunctionCall = initStructAPI.getChildById("INCLUDE_xTimerPendFunctionCall").getValue();
        var INCLUDE_xTaskAbortDelay = initStructAPI.getChildById("INCLUDE_xTaskAbortDelay").getValue();
        var INCLUDE_xTaskGetHandle = initStructAPI.getChildById("INCLUDE_xTaskGetHandle").getValue();
        var INCLUDE_xTaskResumeFromISR = initStructAPI.getChildById("INCLUDE_xTaskResumeFromISR").getValue();
        var INCLUDE_xQueueGetMutexHolder = initStructAPI.getChildById("INCLUDE_xQueueGetMutexHolder").getValue();

        var initStructUS = getSetting(this.uiConfig, this.uiConfig.getName()+".us_tab");
        var configUSER_SETTINGS = initStructUS.getChildById("configUSER_SETTINGS").getValue();


        outputString += addHeaderCommentsFreeRTOSConfig();

        outputString += "\n"; //GENERAL tab
        outputString += addDefineFreeRTOSConfig("configCPU_CLOCK_HZ", "( "+configCPU_CLOCK_HZ+"UL )" , 51);
        outputString += addDefineFreeRTOSConfig("configTICK_RATE_HZ", "((TickType_t) "+configTICK_RATE_HZ+" )", 51);
        outputString += addDefineFreeRTOSConfig("configMAX_PRIORITIES", configMAX_PRIORITIES, 51);
        outputString += addDefineFreeRTOSConfig("configMINIMAL_STACK_SIZE", "((unsigned short) "+configMINIMAL_STACK_SIZE+" )", 51);
        outputString += addDefineFreeRTOSConfig("configMAX_TASK_NAME_LEN", configMAX_TASK_NAME_LEN, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_16_BIT_TICKS", configUSE_16_BIT_TICKS?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configIDLE_SHOULD_YIELD", configIDLE_SHOULD_YIELD?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_PREEMPTION", configUSE_PREEMPTION?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configNUM_THREAD_LOCAL_STORAGE_POINTERS", configNUM_THREAD_LOCAL_STORAGE_POINTERS, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_PORT_OPTIMISED_TASK_SELECTION", configUSE_PORT_OPTIMISED_TASK_SELECTION?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_TASK_NOTIFICATIONS", configUSE_TASK_NOTIFICATIONS?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_TIME_SLICING", configUSE_TIME_SLICING?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_NEWLIB_REENTRANT", configUSE_NEWLIB_REENTRANT?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configENABLE_BACKWARD_COMPATIBILITY", configENABLE_BACKWARD_COMPATIBILITY?1:0, 51);
        outputString += addDefineFreeRTOSConfig("configUSE_POSIX_ERRNO", configUSE_POSIX_ERRNO?1:0, 51);
        if (configMESSAGE_BUFFER_LENGTH_TYPE){
            outputString += addDefineFreeRTOSConfig("configMESSAGE_BUFFER_LENGTH_TYPE", configMESSAGE_BUFFER_LENGTH_TYPE, 51);
        }
        outputString += "\n";
        if(boolConfigASSERT == true){
            outputString += addMsgFreeRTOSConfig("/* Definition assert() function. */");
            outputString += addDefineFreeRTOSConfig("configASSERT(x)",configASSERT);
        }

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* The highest interrupt priority that can be used by any interrupt service");
        outputString += addMsgFreeRTOSConfig("routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL");
        outputString += addMsgFreeRTOSConfig("INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER");
        outputString += addMsgFreeRTOSConfig("PRIORITY THAN THIS! (higher priorities are lower numeric values. */");
        outputString += addDefineFreeRTOSConfig("configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY", "( "+configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+" )");

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Memory allocation related definitions. */");
        outputString += addDefineFreeRTOSConfig("configSUPPORT_STATIC_ALLOCATION", configSUPPORT_STATIC_ALLOCATION?1:0);
        outputString += addDefineFreeRTOSConfig("configSUPPORT_DYNAMIC_ALLOCATION", configSUPPORT_DYNAMIC_ALLOCATION?1:0);
        if ( (memory_scheme != 'heap3') && (memory_scheme != 'heap5') ){
            outputString += addDefineFreeRTOSConfig("configTOTAL_HEAP_SIZE", "(( size_t ) "+configTOTAL_HEAP_SIZE+" )" , 51);
        }
        outputString += addDefineFreeRTOSConfig("configAPPLICATION_ALLOCATED_HEAP", configAPPLICATION_ALLOCATED_HEAP?1:0);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Hook function related definitions. */");
        outputString += addDefineFreeRTOSConfig("configUSE_IDLE_HOOK", configUSE_IDLE_HOOK?1:0);
        outputString += addDefineFreeRTOSConfig("configUSE_TICK_HOOK", configUSE_TICK_HOOK?1:0);
        outputString += addDefineFreeRTOSConfig("configUSE_MALLOC_FAILED_HOOK", configUSE_MALLOC_FAILED_HOOK?1:0);
        outputString += addDefineFreeRTOSConfig("configCHECK_FOR_STACK_OVERFLOW",configCHECK_FOR_STACK_OVERFLOW);
        outputString += addDefineFreeRTOSConfig("configUSE_DAEMON_TASK_STARTUP_HOOK", configUSE_DAEMON_TASK_STARTUP_HOOK?1:0);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Run time and task stats gathering related definitions. */");
        outputString += addDefineFreeRTOSConfig("configGENERATE_RUN_TIME_STATS", configGENERATE_RUN_TIME_STATS?1:0);
        outputString += addDefineFreeRTOSConfig("portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()", portCONFIGURE_TIMER_FOR_RUN_TIME_STATS);
        outputString += addDefineFreeRTOSConfig("portGET_RUN_TIME_COUNTER_VALUE()", portGET_RUN_TIME_COUNTER_VALUE);
        outputString += addDefineFreeRTOSConfig("configUSE_TRACE_FACILITY", configUSE_TRACE_FACILITY?1:0);
        outputString += addDefineFreeRTOSConfig("configUSE_STATS_FORMATTING_FUNCTIONS", configUSE_STATS_FORMATTING_FUNCTIONS?1:0);


        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Co-routine related definitions. */");
        outputString += addDefineFreeRTOSConfig("configUSE_CO_ROUTINES", configUSE_CO_ROUTINES?1:0);
        outputString += addDefineFreeRTOSConfig("configMAX_CO_ROUTINE_PRIORITIES", configMAX_CO_ROUTINE_PRIORITIES);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* SEMAPHORES and MUTEXS */");
        outputString += addDefineFreeRTOSConfig("configUSE_MUTEXES", configUSE_MUTEXES?1:0);
        outputString += addDefineFreeRTOSConfig("configUSE_RECURSIVE_MUTEXES", configUSE_RECURSIVE_MUTEXES?1:0);
        outputString += addDefineFreeRTOSConfig("configUSE_COUNTING_SEMAPHORES", configUSE_COUNTING_SEMAPHORES?1:0);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Software timer related definitions. */");
        outputString += addDefineFreeRTOSConfig("configUSE_TIMERS", configUSE_TIMERS?1:0);
        outputString += addDefineFreeRTOSConfig("configTIMER_TASK_PRIORITY", configTIMER_TASK_PRIORITY);
        outputString += addDefineFreeRTOSConfig("configTIMER_QUEUE_LENGTH", configTIMER_QUEUE_LENGTH);
        outputString += addDefineFreeRTOSConfig("configTIMER_TASK_STACK_DEPTH", configTIMER_TASK_STACK_DEPTH);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Tickless Idle Mode */");
        outputString += addDefineFreeRTOSConfig("configUSE_TICKLESS_IDLE", configUSE_TICKLESS_IDLE?1:0);
        if(configUSE_TICKLESS_IDLE == true){
            outputString += addDefineFreeRTOSConfig("configEXPECTED_IDLE_TIME_BEFORE_SLEEP", configEXPECTED_IDLE_TIME_BEFORE_SLEEP);
            outputString += addDefineFreeRTOSConfig("configPRE_SLEEP_PROCESSING()", configPRE_SLEEP_PROCESSING);
            outputString += addDefineFreeRTOSConfig("configPOST_SLEEP_PROCESSING()", configPOST_SLEEP_PROCESSING);
        }

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* QUEUE */");
        outputString += addDefineFreeRTOSConfig("configQUEUE_REGISTRY_SIZE", configQUEUE_REGISTRY_SIZE);
        outputString += addDefineFreeRTOSConfig("configUSE_QUEUE_SETS", configUSE_QUEUE_SETS?1:0);

        outputString += "\n";
        outputString += addMsgFreeRTOSConfig("/* Optional functions - most linkers will remove unused functions anyway. */");
        outputString += addDefineFreeRTOSConfig("INCLUDE_vTaskPrioritySet", INCLUDE_vTaskPrioritySet?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_uxTaskPriorityGet", INCLUDE_uxTaskPriorityGet?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_vTaskDelete", INCLUDE_vTaskDelete?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_vTaskSuspend", INCLUDE_vTaskSuspend?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_vTaskDelayUntil", INCLUDE_vTaskDelayUntil?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_vTaskDelay", INCLUDE_vTaskDelay?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskGetSchedulerState", INCLUDE_xTaskGetSchedulerState?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskGetCurrentTaskHandle", INCLUDE_xTaskGetCurrentTaskHandle?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_uxTaskGetStackHighWaterMark", INCLUDE_uxTaskGetStackHighWaterMark?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskGetIdleTaskHandle", INCLUDE_xTaskGetIdleTaskHandle?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_eTaskGetState", INCLUDE_eTaskGetState?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xEventGroupSetBitFromISR", INCLUDE_xEventGroupSetBitFromISR?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTimerPendFunctionCall", INCLUDE_xTimerPendFunctionCall?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskAbortDelay", INCLUDE_xTaskAbortDelay?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskGetHandle", INCLUDE_xTaskGetHandle?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xTaskResumeFromISR", INCLUDE_xTaskResumeFromISR?1:0);
        outputString += addDefineFreeRTOSConfig("INCLUDE_xQueueGetMutexHolder", INCLUDE_xQueueGetMutexHolder?1:0);


        outputString += "\n";
        outputString += addMsgFreeRTOSConfig(configUSER_SETTINGS);


        outputString += addFooterCommentsFreeRTOSConfig();
        try {
            var output = scriptApi.createFile("FreeRTOSConfig.h", coreId);
            output.write(outputString);
            output.close();
        } catch (error) {
            scriptApi.logError("creating FreeRTOSConfig : " + error);
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            hFileSections[INCLUDES_SECTION] += addInclude("FreeRTOS.h");
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        try{
        } catch(err) {
            scriptApi.logError(err);
        }
    }

}