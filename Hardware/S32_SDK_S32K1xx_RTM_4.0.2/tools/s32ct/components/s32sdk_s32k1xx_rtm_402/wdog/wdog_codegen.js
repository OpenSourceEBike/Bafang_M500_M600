/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// WDOG component code generator class.
function WDOG_CodeGen(uiConfig){
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function (){
        return "WDOG_CodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections){
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try{
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var userConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".WDOGConfig");
            // Declare variable for wdog config
            var wdogCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".WDOGConfig").getChildren();
            var wdogCfgArray_Size = wdogCfgArray_Items.size();

            if ((wdogCfgArray_Items != null) && (wdogCfgArray_Size > 0)) {
            // Add Includes info.
            hFileSections[INCLUDES_SECTION] += addInclude("wdog_driver.h");

            //Add Defines info
            hFileSections[DEFINES_SECTION] += "\n/* Device instance number */" + NEW_LINE;
            hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_INST", "(0U)");
            //Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configurations */" + NEW_LINE;
            // Iterate through all configurations
            for(var i = 0; i< wdogCfgArray_Size; i++)
            {
                //get config name
                var wdogCfgArray_Name = wdogCfgArray_Items[i].getChildById("name").getStringValue();
                //get config readonly
                var wdogCfgArray_ReadOnly = (wdogCfgArray_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + wdogCfgArray_ReadOnly + "wdog_user_config_t", wdogCfgArray_Name);
            }
        }
            else{
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections){
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".WDOGConfig");
            var configList = initStruct.getChildren();
            var configListSize = configList.size();
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration structures */" + NEW_LINE;

            if(initStruct != null)
            {
                for (var i = 0; i < configListSize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* WDOG user configuration " + i  + " */";

                    var wdogConfigName = configList[i].getChildById("name").getStringValue();
                    var wdogConfigReadOnly = (configList[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    var cfgWindowEnable = configList[i].getChildById("winEnable").getValue();
                    var cfgWindowValue = configList[i].getChildById("windowValue");
                    var cfgTimeoutValue = configList[i].getChildById("timeoutValue");
                    if(cfgWindowEnable == false)
                    {
                        cfgWindowValue.setValue(0);
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + wdogConfigReadOnly + "wdog_user_config_t " + wdogConfigName + " =";
                    cFileSections[GLOBAL_VARS_SECTION] += " {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .clkSource = " + configList[i].getChildById("clkSource").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .opMode = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        " + configList[i].getChildById("wait").getValue() + ",                   /* Wait Mode         */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        " + configList[i].getChildById("stop").getValue() + ",                   /* Stop Mode         */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        " + configList[i].getChildById("debug").getValue() + "                    /* Debug Mode        */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    },";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .updateEnable = " + configList[i].getChildById("updateEnable").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .intEnable = " + configList[i].getChildById("intEnable").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .winEnable = " + configList[i].getChildById("winEnable").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .windowValue = " + configList[i].getChildById("windowValue").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .timeoutValue = " + configList[i].getChildById("timeoutValue").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .prescalerEnable = " + configList[i].getChildById("prescalerEnable").getValue() + "     /* WDOG prescaler             */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};\n";
                }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
}