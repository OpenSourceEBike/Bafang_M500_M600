/*
 * Copyright 2018-2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function flash_mx25l6433f_CodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name
    this.toString = function () {
        return "flash_mx25l6433f_CodeGen";
    }

    var UserConfigurationList = ".flash_mx25l6433f_configurations"

    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* There are global macros defined to be used by the integrator and another one used as include guard.\n *\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + UserConfigurationList);
        var configList = initStruct.getChildren();
        var configListSize = configList.size();

        hFileSections[INCLUDES_SECTION] += addInclude("flash_mx25l6433f_driver.h");

        if (initStruct != null)
        {
            for (var i = 0; i < configListSize; i++)
            {
                var configName = configList[i].getChildById("configName").getStringValue();
                var readOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + readOnly + "flash_mx25l6433f_user_config_t", configName);
            }
        }
    }

    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + UserConfigurationList);
            var configList = initStruct.getChildren();
            var configListSize = configList.size();

            if (initStruct != null)
            {
                for (var i = 0; i < configListSize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* User configuration " + i  + " */";

                    var configName = configList[i].getChildById("configName").getStringValue();
                    var readOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "flash_mx25l6433f_user_config_t " + configName + " = {";

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .dmaSupport           = " + configList[i].getChildById("dmaSupport").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .outputDriverStrength = " + configList[i].getChildById("outputDriverStrength").getValue();

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};\n";
                }
            }
        }
        catch (error)
        {
            scriptApi.logError(error);
        }
    }
}