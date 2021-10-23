/*
 * Copyright 2018-2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function EWM_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    this.toString = function () {
        return "EWM_CodeGenerator";
    }

    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* There are global macros defined to be used by the integrator and another one used as include guard.\n *\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".ewmConfig");
        var configList = initStruct.getChildren();
        var configListSize = configList.size();

        hFileSections[INCLUDES_SECTION] += addInclude("ewm_driver.h");

        if (initStruct != null)
        {
            hFileSections[DEFINES_SECTION] += "\n/* EWM instance number configured */\n";
            
            if (instanceName.toUpperCase() == "EWM")
            {
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + 0 + "U)");
            }
            else
            {
                hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase(), "(" + 0 + "U)");
            }

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration structures */" + NEW_LINE;
            
            for (var i = 0; i < configListSize; i++)
            {
                var ewmConfigName = configList[i].getChildById("name").getStringValue();
                var ewmConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + ewmConfigReadOnly + "ewm_init_config_t", ewmConfigName);
            }
        }
    }

    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".ewmConfig");
            var configList = initStruct.getChildren();
            var configListSize = configList.size();

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration structures */" + NEW_LINE;

            if(initStruct != null)
            {
                for (var i = 0; i < configListSize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* EWM user configuration " + i  + " */";

                    var ewmConfigName = configList[i].getChildById("name").getStringValue();
                    var ewmConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + ewmConfigReadOnly + "ewm_init_config_t " + ewmConfigName + " =";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "{";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .interruptEnable = " + configList[i].getChildById("interrupt").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .prescaler = " + configList[i].getChildById("prescaler").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .compareLow = " + configList[i].getChildById("compareLow").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .compareHigh = " + configList[i].getChildById("compareHigh").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .assertLogic = " + configList[i].getChildById("inputPin").getValue();
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
