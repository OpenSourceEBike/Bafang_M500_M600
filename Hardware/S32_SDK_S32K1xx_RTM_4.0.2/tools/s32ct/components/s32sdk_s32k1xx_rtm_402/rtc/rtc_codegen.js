/*
 * Copyright 2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// RTC component code generator class.
function RTC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "RTC_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function (hFileSections) {
        hFileSections[MISRA_COMMENTS] = "*\n * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n * @section [global]\n * Violates MISRA 2012 Required Rule 8.5, object/function previously declared\n * It is safe, because the file has include guards to prevent multiple declarations of the same object/function.\n";
        try {
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var instanceName = childContext.getComponentInstanceConfig().getName();

            // Generate Initialization Tab configuration
            var rtcInitConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcInitConfig").getChildren();
            var rtcInitConfig_Size = rtcInitConfig_Items.size();
            if ((rtcInitConfig_Items != null) && (rtcInitConfig_Size > 0)) {
                // Add Includes info.
                hFileSections[INCLUDES_SECTION] += addInclude("rtc_driver.h");

                //Add Defines info.
                hFileSections[DEFINES_SECTION] += "\n/* Device instance number */\n";
                hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase(), "(0U)");
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/*\n * RTC Initialization configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcInitConfig_Size; i++) {
                    //get config name
                    var rtcCfgName = rtcInitConfig_Items[i].getChildById("init.name").getStringValue();
                    //get config readonly
                    var rtcCfgReadOnly = (rtcInitConfig_Items[i].getChildById("init.readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_init_config_t", rtcCfgName);
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_timedate_t", rtcCfgName + "_StartTime");
                }
            }
            else {
                throw "No Initialization configuration found!";
            }

            // Generate Register Lock Tab configuration
            var rtcLockConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcLockConfig").getChildren();
            var rtcLockConfig_Size = rtcLockConfig_Items.size();
            if ((rtcLockConfig_Items != null) && (rtcLockConfig_Size > 0)) {
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*\n * RTC Register Lock configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcLockConfig_Size; i++) {
                    if (rtcLockConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcLockConfig_Items[i].getChildById("name").getStringValue();
                        //get config readonly
                        var rtcCfgReadOnly = (rtcLockConfig_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";
                        //add config to header file
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Register Lock configuration " + i + " */" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_register_lock_config_t", rtcCfgName);
                    }
                }
            }
            else {
                throw "No Register Lock configuration found!";
            }

            // Generate Alarm Tab configuration
            var rtcAlarmConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcAlarmConfig").getChildren();
            var rtcAlarmConfig_Size = rtcAlarmConfig_Items.size();
            if ((rtcAlarmConfig_Items != null) && (rtcAlarmConfig_Size > 0)) {
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*\n * RTC Alarm configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcAlarmConfig_Size; i++) {
                    if (rtcAlarmConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcAlarmConfig_Items[i].getChildById("name").getStringValue();
                        //get config readonly
                        var rtcCfgReadOnly = "";
                        //add config to header file
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Alarm configuration " + i + " */" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_alarm_config_t", rtcCfgName);

                        var rtcCfgCallback = rtcAlarmConfig_Items[i].getChildById("alarmCallback").getStringValue()
                        if (rtcCfgCallback != "NULL") {
                            //declare extern callbacks
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Callback declaration */\n";
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", rtcCfgCallback + "(void * callbackParam)");
                        }
                    }
                }
            }
            else {
                throw "No Alarm configuration found!";
            }

            // Generate Seconds Interrupt Tab configuration
            var rtcTSIConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcTSIConfig").getChildren();
            var rtcTSIConfig_Size = rtcTSIConfig_Items.size();
            if ((rtcTSIConfig_Items != null) && (rtcTSIConfig_Size > 0)) {
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*\n * RTC Time Seconds Interrupt configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcTSIConfig_Size; i++) {
                    if (rtcTSIConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcTSIConfig_Items[i].getChildById("name").getStringValue();
                        //get config readonly
                        var rtcCfgReadOnly = "";
                        //add config to header file
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Time Seconds Interrupt configuration " + i + " */" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_seconds_int_config_t", rtcCfgName);

                        var rtcCfgCallback = rtcTSIConfig_Items[i].getChildById("rtcSecondsCallback").getStringValue()
                        if (rtcCfgCallback != "NULL") {
                            //declare extern callbacks
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Callback declaration */\n";
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", rtcCfgCallback + "(void * callbackParam)");
                        }
                    }
                }
            }
            else {
                throw "No Seconds Interrupt configuration found!";
            }

            // Generate Fault interrupt Tab configuration
            var rtcFaultConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcFaultConfig").getChildren();
            var rtcFaultConfig_Size = rtcFaultConfig_Items.size();
            if ((rtcFaultConfig_Items != null) && (rtcFaultConfig_Size > 0)) {
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*\n * RTC Fault interrupt configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcFaultConfig_Size; i++) {
                    if (rtcFaultConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcFaultConfig_Items[i].getChildById("name").getStringValue();
                        //get config readonly
                        var rtcCfgReadOnly = "";
                        //add config to header file
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Fault interrupt configuration " + i + " */" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + rtcCfgReadOnly + "rtc_interrupt_config_t", rtcCfgName);

                        var rtcCfgCallback = rtcFaultConfig_Items[i].getChildById("rtcCallback").getStringValue()
                        if (rtcCfgCallback != "NULL") {
                            //declare extern callbacks
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Callback declaration */\n";
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", rtcCfgCallback + "(void * callbackParam)");
                        }
                    }
                }
            }
            else {
                throw "No Fault interrupt configuration found!";
            }

        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections
    this.genCFile = function (cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = "*\n * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";

        try {
            // Generate Initialization Tab configuration
            var rtcInitConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcInitConfig").getChildren();
            var rtcInitConfig_Size = rtcInitConfig_Items.size();
            if ((rtcInitConfig_Items != null) && (rtcInitConfig_Size > 0)) {
                //Add external global variables
                cFileSections[GLOBAL_VARS_SECTION] = "\n/*\n * RTC Initialization configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcInitConfig_Size; i++) {
                    //get config name
                    var rtcCfgName = rtcInitConfig_Items[i].getChildById("init.name").getStringValue();
                    //get config readonly
                    var rtcCfgReadOnly = (rtcInitConfig_Items[i].getChildById("init.readonly").getValue() == true) ? "const " : "";
                    // Add RTC configuration structure
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + " instance initialization configuration */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcInitConfig_Items[i].getChildById("init"), "rtc_init_config_t", "");
                    var rtc_apiCfg_timeVariable = rtcCfgReadOnly + "rtc_timedate_t " + rtcCfgName + "_StartTime";
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + "_StartTime Initial Time and Date */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcInitConfig_Items[i].getChildById("date"), rtc_apiCfg_timeVariable, "");
                }
            }
            else {
                throw "No Initialization configuration found!";
            }

            // Generate Register Lock Tab configuration
            var rtcLockConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcLockConfig").getChildren();
            var rtcLockConfig_Size = rtcLockConfig_Items.size();
            if ((rtcLockConfig_Items != null) && (rtcLockConfig_Size > 0)) {
                //Add external global variables
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*\n * RTC Register Lock configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcLockConfig_Size; i++) {
                    if (rtcLockConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcLockConfig_Items[i].getChildById("name").getStringValue();
                        // Add RTC configuration structure
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + " register lock configuration */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcLockConfig_Items[i], "rtc_register_lock_config_t", "");
                    }
                }
            }
            else {
                throw "No Register Lock configuration found!";
            }

            // Generate Alarm Tab configuration
            var rtcAlarmConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcAlarmConfig").getChildren();
            var rtcAlarmConfig_Size = rtcAlarmConfig_Items.size();
            if ((rtcAlarmConfig_Items != null) && (rtcAlarmConfig_Size > 0)) {
                //Add external global variables
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*\n * RTC Alarm configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcAlarmConfig_Size; i++) {
                    if (rtcAlarmConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcAlarmConfig_Items[i].getChildById("name").getStringValue();
                        // Add RTC configuration structure
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + " alarm configuration */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcAlarmConfig_Items[i], "rtc_alarm_config_t", "");
                        // Find where .alarmTime needs to be
                        var configPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(rtcCfgName);
                        var pos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".repeat", configPos);
                        var rtcAlarmTime = printConfigStruct(rtcAlarmConfig_Items[i].getChildById("alarmTime"), "", "").replace(/^\n  /g, "").replace(/\./g, "  .").replace(/};/g, "  },");
                        // Add .alarmTime field to config structure
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, pos) + ".alarmTime " + rtcAlarmTime + "  " + cFileSections[GLOBAL_VARS_SECTION].substr(pos);
                        // Find where .callbackParams needs to be
                        var configPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(rtcCfgName);
                        var pos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".alarmCallback", configPos);
                        var rtcCallParam = ".callbackParams = NULL,\n";
                        // Add .alarmTime field to config structure
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, pos) + rtcCallParam + "  " + cFileSections[GLOBAL_VARS_SECTION].substr(pos);
                    }
                }
            }
            else {
                throw "No Alarm configuration found!";
            }

            // Generate Seconds Interrupt Tab configuration
            var rtcTSIConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcTSIConfig").getChildren();
            var rtcTSIConfig_Size = rtcTSIConfig_Items.size();
            if ((rtcTSIConfig_Items != null) && (rtcTSIConfig_Size > 0)) {
                //Add external global variables
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*\n * RTC Time Seconds Interrupt configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcTSIConfig_Size; i++) {
                    if (rtcTSIConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcTSIConfig_Items[i].getChildById("name").getStringValue();
                        // Add RTC configuration structure
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + " time seconds configuration */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcTSIConfig_Items[i], "rtc_seconds_int_config_t", "");
                        // Find where .callbackParams needs to be
                        var configPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(rtcCfgName);
                        var pos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".rtcSecondsCallback", configPos);
                        var rtcCallParam = ".secondsCallbackParams = NULL,\n";
                        // Add .alarmTime field to config structure
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, pos) + rtcCallParam + "  " + cFileSections[GLOBAL_VARS_SECTION].substr(pos);
                    }
                }
            }
            else {
                throw "No Seconds Interrupt configuration found!";
            }

            // Generate Fault interrupt Tab configuration
            var rtcFaultConfig_Items = getSetting(this.uiConfig, this.uiConfig.getName() + ".rtcFaultConfig").getChildren();
            var rtcFaultConfig_Size = rtcFaultConfig_Items.size();
            if ((rtcFaultConfig_Items != null) && (rtcFaultConfig_Size > 0)) {
                //Add external global variables
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*\n * RTC Fault interrupt configurations\n */" + NEW_LINE;
                // Iterate through all configurations
                for (var i = 0; i < rtcFaultConfig_Size; i++) {
                    if (rtcFaultConfig_Items[i].getChildById("enableCfg").getValue()) {
                        //get config name
                        var rtcCfgName = rtcFaultConfig_Items[i].getChildById("name").getStringValue();
                        // Add RTC configuration structure
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! " + rtcCfgName + " fault configuration */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rtcFaultConfig_Items[i], "rtc_interrupt_config_t", "");
                        // Find where .callbackParams needs to be
                        var configPos = cFileSections[GLOBAL_VARS_SECTION].indexOf(rtcCfgName);
                        var pos = cFileSections[GLOBAL_VARS_SECTION].indexOf(".rtcCallback", configPos);
                        var rtcCallParam = ".callbackParams = NULL,\n";
                        // Add .alarmTime field to config structure
                        cFileSections[GLOBAL_VARS_SECTION] = cFileSections[GLOBAL_VARS_SECTION].substr(0, pos) + rtcCallParam + "  " + cFileSections[GLOBAL_VARS_SECTION].substr(pos);
                    }
                }
            }
            else {
                throw "No Fault interrupt configuration found!";
            }

        } catch (error) {
            scriptApi.logError(error);
        }
    }
}

