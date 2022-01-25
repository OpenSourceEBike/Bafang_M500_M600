/*
 * Copyright 2019-2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// SWT component code generator class.
function flash_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "flash_CodeGenerator";
    }

    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* There are global macros defined to be used by the integrator and another one used as include guard.\n *\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".flashConfig");
        var configList = initStruct.getChildren();
        var configListSize = configList.size();

        hFileSections[INCLUDES_SECTION] += addInclude("flash_driver.h");

        if (initStruct != null)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration structures */" + NEW_LINE;
            
            for (var i = 0; i < configListSize; i++)
            {
                var flashConfigName = configList[i].getChildById("Configuration").getStringValue();
                var flashConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                var flashConfigCallBack = configList[i].getChildById("CallBack");

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + flashConfigReadOnly + "flash_user_config_t", flashConfigName);

                if ((flashConfigCallBack.getStringValue() == "") || (flashConfigCallBack.getStringValue() == "NULL"))
                {
                    flashConfigCallBack.setValue("NULL_CALLBACK");
                }

                if(flashConfigCallBack.getValue() != "NULL_CALLBACK")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Callback function of configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + flashConfigCallBack.getValue() + "(void);" + NEW_LINE;
                }
            }
        }
    }

    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".flashConfig");
            var configList = initStruct.getChildren();
            var configListSize = configList.size();

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration structures */" + NEW_LINE;

            if(initStruct != null)
            {
                for (var i = 0; i < configListSize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* Flash user configuration " + i  + " */";

                    var flashConfigName = configList[i].getChildById("Configuration").getStringValue();
                    var flashConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    var flashConfigCallBack = configList[i].getChildById("CallBack");

                    if ((flashConfigCallBack.getStringValue() == "") || (flashConfigCallBack.getStringValue() == "NULL"))
                    {
                        flashConfigCallBack.setValue("NULL_CALLBACK");
                    }
                    
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + flashConfigReadOnly + "flash_user_config_t " + flashConfigName + " =";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "{";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .PFlashBase = 0x" + parseInt(configList[i].getChildById("PFlashBase").getValue()).toString(16).toUpperCase()+ "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .PFlashSize = 0x" + parseInt(configList[i].getChildById("PFlashSize").getValue()).toString(16).toUpperCase() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .DFlashBase = 0x" + parseInt(configList[i].getChildById("DFlashBase").getValue()).toString(16).toUpperCase() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .EERAMBase = 0x" + parseInt(configList[i].getChildById("EERAMBase").getValue()).toString(16).toUpperCase() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .CallBack = " + configList[i].getChildById("CallBack").getStringValue() ;
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
