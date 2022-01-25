/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");


// WDG PAL component code generator class.
function wdg_palCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "wdg_palCodeGen";
    }

    /* Generates the pal_cfg.h file */
    this.genHCfgFile = function(hCfgFileSections) {

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        hCfgFileSections[FILE_NAME_SECTION] += "wdg_pal_cfg.h\n";

        // Add define info
        if (peripheralName.indexOf("SWT") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("wdg_pal_cfg.h", "WDG_OVER_SWT", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("wdg_pal_cfg.h", "WDG_OVER_SWT_INSTANCE_COUNT");
        }

        if (peripheralName.indexOf("WDOG") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("wdg_pal_cfg.h", "WDG_OVER_WDOG", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("wdg_pal_cfg.h", "WDG_OVER_WDOG_INSTANCE_COUNT");
        }

        if (peripheralName.indexOf("EWM") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("wdg_pal_cfg.h", "WDG_OVER_EWM", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("wdg_pal_cfg.h", "WDG_OVER_EWM_INSTANCE_COUNT");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var initUserStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".wdg_pal_UserConfig");

        var initStruct = initUserStruct.getChildById("wdg_pal_Config");
        var initStructChildren = initStruct.getChildren();
        var initStructExtension = initUserStruct.getChildById("wdg_pal_ConfigExtension");
        var initStructExtensionChildren = initStructExtension.getChildren();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("wdg_pal.h");
        hFileSections[INCLUDES_SECTION] += addInclude("wdg_pal_mapping.h");

        if ((initStruct != null) && initStruct.isAvailable() && (initStructChildren.length > 0))
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_NUM_OF_USER_CFG", "(" + initStructChildren.length + "U)");

            /*! @brief Structure storing PAL instance information */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Structure storing PAL instance information */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const wdg_instance_t", String(instanceName + "_Instance"));

            /*! @brief Structure storing PAL configuration information */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User Configurations */" + NEW_LINE;

            for (var i = 0; i < initStructChildren.length; i++) {

                var cfgName = initStructChildren[i].getChildById("name").getStringValue();
                var cfgReadOnly = initStructChildren[i].getChildById("readonly").getValue();
                var isConst = cfgReadOnly ? "const " : "";

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + isConst + "wdg_config_t", cfgName);
            }
        }

        if ((initStructExtension != null) && initStructExtension.isAvailable() && (initStructExtensionChildren.length > 0))
        {
            /*! @brief Structure storing PAL extension information */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*  Extension configurations for EWM peripheral */" + NEW_LINE;

            for (var i = 0; i < initStructExtensionChildren.length; i++) {

                var cfgExtension = initStructExtensionChildren[i].getChildById("name").getValue();

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + "extension_ewm_for_wdg_t", cfgExtension);
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{

            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var initUserStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".wdg_pal_UserConfig");

            var initStruct = initUserStruct.getChildById("wdg_pal_Config");
            var initStructChildren = initStruct.getChildren();
            var initStructExtension = initUserStruct.getChildById("wdg_pal_ConfigExtension");
            var initStructExtensionChildren = initStructExtension.getChildren();

            var subNamePeripheral = peripheralName.slice(0,3);
            var typeInstance;
            var idInstance;

            /* Print user config struct */
            if ((initStruct != null) && initStruct.isAvailable() && (initStructChildren.length > 0))
            {
                if (subNamePeripheral == "SWT")
                {
                    typeInstance = String("WDG_INST_TYPE_" + peripheralName.slice(0,3));
                    idInstance = peripheralName.match(/\d+$/)[0];
                }

                if (subNamePeripheral == "EWM")
                {
                    typeInstance = String("WDG_INST_TYPE_" + peripheralName.slice(0,3));
                    idInstance = 0;
                }

                if (subNamePeripheral == "WDO")
                {
                    typeInstance = String("WDG_INST_TYPE_" + peripheralName.slice(0,4));
                    idInstance = 0;
                }

                // Add WDG PAL configuration instance structure.
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* WDG PAL instance information */" + NEW_LINE;

                cFileSections[GLOBAL_VARS_SECTION] += "const wdg_instance_t" + " " + String(instanceName + "_Instance") + " = {" + NEW_LINE;

                cFileSections[GLOBAL_VARS_SECTION] += "  .instType = " + typeInstance + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "  .instIdx  = " + idInstance + "U";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;


                cFileSections[GLOBAL_VARS_SECTION] += "\n/* Serial User Configurations */";
                for (var i = 0; i < initStructChildren.length; i++) {

                    var cfgName = initStructChildren[i].getChildById("name").getStringValue();
                    var cfgReadOnly = initStructChildren[i].getChildById("readonly").getValue();
                    var cfgExtension = initStructChildren[i].getChildById("extension").getValue();
                    var isConst = cfgReadOnly ? "const " : "";
                    var cfgWindowEnable = initStructChildren[i].getChildById("winEnable").getValue();
                    var cfgPercentWindow = initStructChildren[i].getChildById("percentWindow");
                    var cfgclkSource = initStructChildren[i].getChildById("clkSource");
                    var cfgTimeout = initStructChildren[i].getChildById("timeoutValue");

                    if (peripheralName.indexOf("SWT") == 0) {
                        cfgclkSource.setValue('WDG_PAL_IRC_CLOCK');
                        // Reset timeout value if it is out of admissible range! (256 - 4294967296)
                        if (cfgTimeout.getValue() < 256 ) {
                        cfgTimeout.setValue(256);
                        }
                    }

                    if (peripheralName.indexOf("EWM") == 0) {
                        cfgclkSource.setValue('WDG_PAL_LPO_CLOCK');
                        // Reset timeout value if it is out of admissible range! (0 - 255)
                        if (cfgTimeout.getValue() < 0 || cfgTimeout.getValue() > 255 ) {
                        cfgTimeout.setValue(255);
                        }
                    }

                    if (peripheralName.indexOf("WDOG") == 0) {
                        // Reset timeout value if it is out of admissible range! (0 - 65535)
                        if (cfgTimeout.getValue() < 0 || cfgTimeout.getValue() > 65535 ) {
                        cfgTimeout.setValue(65535);
                        }
                    }

                    // Add WDG PAL configuration structure.
                    if (cfgWindowEnable == false)
                    {
                        cfgPercentWindow.setValue(0);
                    }

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + isConst + "wdg_config_t" + " " + cfgName + " = {" + NEW_LINE;

                    cFileSections[GLOBAL_VARS_SECTION] += "  .clkSource        = " + initStructChildren[i].getChildById("clkSource").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  .opMode           = {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .wait           = " + initStructChildren[i].getChildById("opMode").getChildById("wait").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .stop           = " + initStructChildren[i].getChildById("opMode").getChildById("stop").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .debug          = " + initStructChildren[i].getChildById("opMode").getChildById("debug").getValue() + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  .intEnable        = " + initStructChildren[i].getChildById("intEnable").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  .prescalerEnable  = " + initStructChildren[i].getChildById("prescalerEnable").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  .winEnable        = " + initStructChildren[i].getChildById("winEnable").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "  .percentWindow    = " + initStructChildren[i].getChildById("percentWindow").getValue() + "U," + NEW_LINE;
                    if (initStructExtension.isAvailable()) {
                        cFileSections[GLOBAL_VARS_SECTION] += "  .timeoutValue     = " + initStructChildren[i].getChildById("timeoutValue").getValue() + "UL," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "  .extension        = &" + initStructChildren[i].getChildById("extension").getValue() + NEW_LINE;
                    }
                    else{
                        cFileSections[GLOBAL_VARS_SECTION] += "  .timeoutValue     = " + initStructChildren[i].getChildById("timeoutValue").getValue() + "UL" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

                    // cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(initStructChildren[i], "wdg_config_t", "");
                }
            }

            /* Print extension config struct */
            if ((initStructExtension != null) && initStructExtension.isAvailable() && (initStructExtensionChildren.length > 0))
            {
                // Add WDG PAL configuration extension structure.
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* Extension configurations for EWM peripheral */";

                for (var i = 0; i < initStructExtensionChildren.length; i++) {

                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(initStructExtensionChildren[i], "extension_ewm_for_wdg_t", "");
                }
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
