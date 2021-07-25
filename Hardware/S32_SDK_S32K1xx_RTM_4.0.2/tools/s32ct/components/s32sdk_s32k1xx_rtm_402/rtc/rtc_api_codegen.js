/*
 * Copyright 2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// RTC_API component code generator class.
function RTC_API_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "RTC_API_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n * @section [global]\n * Violates MISRA 2012 Required Rule 8.5, object/function previously declared\n * The symbols are already declared since this file is included in Cpu.h, which is included here.\n * It is safe, because the file has include guards to prevent multiple declarations of the same object/function.\n";
        try {
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var rtc_apiCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".rtc_apiConfig").getChildren();
            var rtc_apiAlarmCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".rtc_apiAlarmConfig").getChildren();
            var rtc_apiCfgArray_Size = rtc_apiCfgArray_Items.size();
            var rtc_apiAlarmCfgArray_Size = rtc_apiAlarmCfgArray_Items.size();

            if ((rtc_apiCfgArray_Items != null) && (rtc_apiCfgArray_Size > 0) && (rtc_apiAlarmCfgArray_Items != null) && (rtc_apiAlarmCfgArray_Size > 0)) {
                // Add Includes info.
                hFileSections[INCLUDES_SECTION] += addInclude("rtc_api_driver.h");

                //Add Defines info.
                hFileSections[DEFINES_SECTION] += "\n/* Device instance number */\n";
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + peripheralName.replace(/[^0-9\.]+/g, "") + "U)");
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configurations */" + NEW_LINE;

                // Iterate through all configurations
                for (var i =  0; i < rtc_apiCfgArray_Size; i++) {
                    //get config name
                    var rtc_apiCfgArray_Name = rtc_apiCfgArray_Items[i].getChildById("init.name").getStringValue();
                    //get config readonly
                    var rtc_apiCfgArray_ReadOnly = (rtc_apiCfgArray_Items[i].getChildById("init.readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtc_apiCfgArray_ReadOnly + "rtc_init_config_t", rtc_apiCfgArray_Name);
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtc_apiCfgArray_ReadOnly + "rtc_timedate_t", rtc_apiCfgArray_Name + "_StartTime");

                    var rtc_apiCallback = rtc_apiCfgArray_Items[i].getChildById("init.periodicIntCallback").getStringValue()
                    if(rtc_apiCallback != "NULL")
                    {
                        //declare extern callbacks
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Callback declaration */\n";
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", rtc_apiCallback + "(void * callbackParam)");
                    }
                }
                // Iterate through all alarm configurations
                for (var i =  0; i < rtc_apiAlarmCfgArray_Size; i++) {
                    //get config name
                    var rtc_apiAlarmCfgArray_Name = rtc_apiAlarmCfgArray_Items[i].getChildById("name").getStringValue();
                    //get config readonly
                    var rtc_apiAlarmCfgArray_ReadOnly = (rtc_apiAlarmCfgArray_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User alarm configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtc_apiAlarmCfgArray_ReadOnly + "rtc_alarm_config_t", rtc_apiAlarmCfgArray_Name);

                    var rtc_apiCallback = rtc_apiAlarmCfgArray_Items[i].getChildById("alarmCallback").getStringValue()
                    if(rtc_apiCallback != "NULL")
                    {
                        //declare extern callbacks
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Callback declaration */\n";
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", rtc_apiCallback + "(void * callbackParam)");
                    }
                }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var rtc_apiCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".rtc_apiConfig").getChildren();
            var rtc_apiCfgArray_Size = rtc_apiCfgArray_Items.size();
            
            if ((rtc_apiCfgArray_Items != null) && (rtc_apiCfgArray_Size > 0)) {
                for (var i =  0; i < rtc_apiCfgArray_Size; i++) {
                    //get config name
                    var rtc_apiCfgArray_Name = rtc_apiCfgArray_Items[i].getChildById("init.name").getStringValue();
                    //get config readonly
                    var rtc_apiCfgArray_ReadOnly = (rtc_apiCfgArray_Items[i].getChildById("init.readonly").getValue() == true) ? "const " : "";
                    // Add RTC_API configuration structure
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtc_apiCfgArray_Name + " instance init configuration */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtc_apiCfgArray_Items[i].getChildById("init"), "rtc_init_config_t", "");
                    var rtc_apiCfg_timeVariable = rtc_apiCfgArray_ReadOnly + "rtc_timedate_t " + rtc_apiCfgArray_Name + "_StartTime";
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtc_apiCfgArray_Name + "_StartTime Initial Time and Date */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtc_apiCfgArray_Items[i].getChildById("dateTime"), rtc_apiCfg_timeVariable, "");
                }
            }
            else {
                throw "Configuration does not exist";
            }

            var rtc_apiAlarmCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".rtc_apiAlarmConfig").getChildren();
            var rtc_apiAlarmCfgArray_Size = rtc_apiAlarmCfgArray_Items.size();
            if ((rtc_apiAlarmCfgArray_Items != null) && (rtc_apiAlarmCfgArray_Size > 0)) {
                for (var i =  0; i < rtc_apiAlarmCfgArray_Size; i++) {
                    //get config name
                    var rtc_apiCfgArray_Name = rtc_apiAlarmCfgArray_Items[i].getChildById("name").getStringValue();
                    //get config readonly
                    var rtc_apiCfgArray_ReadOnly = (rtc_apiAlarmCfgArray_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";

                    // Add RTC_API configuration structure
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtc_apiCfgArray_Name + " Alarm Configuration */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtc_apiAlarmCfgArray_Items[i], "rtc_alarm_config_t", "");
                    // Find where .alarmTime needs to be
                    var configPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(rtc_apiCfgArray_Name);
                    var pos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".", configPos);
                    var rtc_apiAlarmTime = printConfigStruct(rtc_apiAlarmCfgArray_Items[i].getChildById("alarmTime"), "", "").replace(/^\n  /g,"").replace(/\./g,"  .").replace(/};/g,"  },");
                    // Add .alarmTime field to config structure
                    cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, pos) + ".alarmTime " + rtc_apiAlarmTime + "  " + cFileSections[GLOBAL_VARS_SECTION].substr(pos);

                    //If repetition is disable put repeat fields on default value
                    if (rtc_apiAlarmCfgArray_Items[i].getChildById("repeat").getValue() == false)
                    {
                        var fieldPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".repetitionInterval", configPos);
                        var commaPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(",", fieldPos);
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, fieldPos) + ".repetitionInterval = 0UL" + cFileSections[GLOBAL_VARS_SECTION].substr(commaPos);
                        fieldPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".numberOfRepeats", configPos);
                        commaPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(",", fieldPos);
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, fieldPos) + ".numberOfRepeats = 0UL" + cFileSections[GLOBAL_VARS_SECTION].substr(commaPos);
                        fieldPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".repeatForever", configPos);
                        commaPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(",", fieldPos);
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, fieldPos) + ".repeatForever = false" + cFileSections[GLOBAL_VARS_SECTION].substr(commaPos);
                    }
                }
            }
            else {
                throw "Alarm Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
}

