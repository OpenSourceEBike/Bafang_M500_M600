/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// POWER_MANAGER component code generator class.
function POWER_MANAGER_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "POWER_MANAGER_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("power_manager.h");

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        var powerConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".POWER_CONFIGURATIONS");
        var powerConfigurationModeConfig = powerConfiguration.getChildById("powerModeConfiguration");
        var powerConfigurationChildModeConfig = powerConfigurationModeConfig.getChildren();
        var countPwrModeCfg = powerConfigurationChildModeConfig.length;

        var powerRgmConfigurationModeConfig = powerConfiguration.getChildById("Rgm");
        var powerRgmConfigurationChildModeConfig = powerRgmConfigurationModeConfig.getChildren();
        var countPwrRgmModeCfg = powerRgmConfigurationChildModeConfig.length;

        var powerConfigurationCallback = powerConfiguration.getChildById("Callback");
        var powerConfigurationChildCallback = powerConfigurationCallback.getChildren();
        var countCallbackCfg = powerConfigurationChildCallback.length;

        if (powerRgmConfigurationModeConfig != null)
        {

            for(var i = 0; i < countPwrRgmModeCfg;i++)
            {
                var powerRgm = powerRgmConfigurationChildModeConfig[i].getChildById("powerModeRgmConfig");
                if (powerRgm.isAvailable())
                {
                    var powerRgmModeCfg = powerRgmConfigurationChildModeConfig[i].getChildById("powerModeRgmConfig");

                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief User configuration structure " + powerRgmModeCfg.getValue() + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern power_manager_mc_rgm_extention_t " + powerRgmModeCfg.getValue() + ";" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                }
            }

        }

        if (powerConfigurationModeConfig != null)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Count of user configuration structures */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "#define POWER_MANAGER_CONFIG_CNT " + countPwrModeCfg +"U" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;

            if(countPwrModeCfg > 0)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Array of pointers to User configuration structures */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern power_manager_user_config_t * powerConfigsArr[" + countPwrModeCfg + "];" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
            }
            for(var i = 0; i < countPwrModeCfg;i++)
            {
                var powerModeCfg = powerConfigurationChildModeConfig[i].getChildById("powerModeConfig");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief User configuration structure " + powerModeCfg.getValue() + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern power_manager_user_config_t " + powerModeCfg.getValue() + ";" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
            }

        }

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
        if (powerConfigurationCallback != null)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Count of user Callbacks */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "#define POWER_MANAGER_CALLBACK_CNT " + countCallbackCfg +"U" + NEW_LINE;
            if(countCallbackCfg == 0)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Array of pointers to User defined static Callbacks configuration structures */"+ NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern power_manager_callback_user_config_t * powerStaticCallbacksConfigsArr[" + 1 + "];" + NEW_LINE;
            }
            else
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Array of pointers to User defined static Callbacks configuration structures */"+ NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern power_manager_callback_user_config_t * powerStaticCallbacksConfigsArr[" + countCallbackCfg + "];" + NEW_LINE;
            }
            for(var i = 0; i < countCallbackCfg;i++)
            {
                var callbacknameCfg = powerConfigurationChildCallback[i].getChildById("NameCallbackConfig");
                var callbackfunction = powerConfigurationChildCallback[i].getChildById("NameCallbackFunction");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Declaration of power manager static callback " + callbackfunction.getValue() + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern status_t " + callbackfunction.getValue() + "(power_manager_notify_struct_t * notify, power_manager_callback_data_t * dataPtr);" + NEW_LINE;
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        var powerConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".POWER_CONFIGURATIONS");
        var powerConfigurationModeConfig = powerConfiguration.getChildById("powerModeConfiguration");
        var powerConfigurationChildModeConfig = powerConfigurationModeConfig.getChildren();
        var countPwrModeCfg = powerConfigurationChildModeConfig.length;

        var powerRgmConfigurationModeConfig = powerConfiguration.getChildById("Rgm");
        var powerRgmConfigurationChildModeConfig = powerRgmConfigurationModeConfig.getChildren();
        var countPwrRgmModeCfg = powerRgmConfigurationChildModeConfig.length;

        var powerConfigurationCallback = powerConfiguration.getChildById("Callback");
        var powerConfigurationChildCallback = powerConfigurationCallback.getChildren();
        var countCallbackCfg = powerConfigurationChildCallback.length;

        if (powerRgmConfigurationModeConfig != null)
        {
            for(var i = 0; i < countPwrRgmModeCfg;i++)
            {

                var powerRgm = powerRgmConfigurationChildModeConfig[i].getChildById("powerModeRgmConfig");
                if (powerRgm.isAvailable())
                {

                    var powerRgmModeCfg = powerRgmConfigurationChildModeConfig[i].getChildById("powerModeRgmConfig");
                    cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief User Configuration structure " +  powerRgmModeCfg.getValue() + " */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "power_manager_mc_rgm_extention_t " + powerRgmModeCfg.getValue() + " = {" + NEW_LINE;

                    var rsRequest = powerRgmConfigurationChildModeConfig[i].getChildById("rsSourceRequest");
                    cFileSections[GLOBAL_VARS_SECTION] += "    .rsSourceRequest               = " + rsRequest.getValue() + ",                   /*!< Power manager: reset source reset */" + NEW_LINE;

                    var escalationThresholdValue = powerRgmConfigurationChildModeConfig[i].getChildById("escalationThreshold");
                    cFileSections[GLOBAL_VARS_SECTION] += "    .escalationThreshold           = " + escalationThresholdValue.getValue() + ",     /*!< Power manager: reset source reset */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "};\n" + NEW_LINE;
                }
            }

        }

        if (countPwrModeCfg != 0)
        {
            for(var i = 0; i < countPwrModeCfg;i++)
            {
                var powerModeCfg = powerConfigurationChildModeConfig[i].getChildById("powerModeConfig");
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief User Configuration structure " +  powerModeCfg.getValue() + " */" + NEW_LINE;

                cFileSections[GLOBAL_VARS_SECTION] += "power_manager_user_config_t " + powerModeCfg.getValue() + " = {" + NEW_LINE;
                var powerModeValue = powerConfigurationChildModeConfig[i].getChildById("powerMode");
                cFileSections[GLOBAL_VARS_SECTION] += "    .powerMode                = " + powerModeValue.getValue() + ",     /*!< Power manager mode */" + NEW_LINE;

                var powerRgm = powerConfigurationChildModeConfig[i].getChildById("extentionRgmcfg");
                if (powerRgm.isAvailable())
                {
                    if(powerModeValue.getValue() == "POWER_MANAGER_RESET")
                    {
                        var powerRgmModeCfg = powerRgmConfigurationChildModeConfig[0].getChildById("powerModeRgmConfig");
                        if (powerRgmConfigurationModeConfig != null)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += "    .powerMcRgmExtention      = " + "&" + powerRgmModeCfg.getValue() + ",          /*!< Configuration for reset generation module */" + NEW_LINE;
                        }
                    }
                    else
                    {
                            cFileSections[GLOBAL_VARS_SECTION] += "    .powerMcRgmExtention      = " + "(void *)0" + "," + NEW_LINE;
                    }

                }

                var sleepOnExitVal = powerConfigurationChildModeConfig[i].getChildById("sleepOnExitValue");
                if (sleepOnExitVal.isAvailable())
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .sleepOnExitValue         = " + sleepOnExitVal.getValue() + ",          /*!< Sleep on exit value */" + NEW_LINE;
                }

                var powerLevel = powerConfigurationChildModeConfig[i].getChildById("powerLevel");
                if (powerLevel.isAvailable())
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .powerLevel                       = " + powerLevel.getValue() + "          /*!< Relative power level indicator */" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "};\n" + NEW_LINE;


            }
            if(countPwrModeCfg > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Array of pointers to User configuration structures */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "power_manager_user_config_t * powerConfigsArr[] = {" + NEW_LINE;

                for (var i = 0; i < powerConfigurationChildModeConfig.length;i++) {
                    var powerModeCfg = powerConfigurationChildModeConfig[i].getChildById("powerModeConfig");
                    if (i != (countPwrModeCfg - 1))
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    &" + powerModeCfg.getValue() + "," + NEW_LINE;
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    &" + powerModeCfg.getValue() + NEW_LINE;
                    }
                }
                cFileSections[GLOBAL_VARS_SECTION] +="};\n" + NEW_LINE;
            }
        }

        if (countCallbackCfg != 0)
        {
            for(var i = 0; i < countCallbackCfg;i++)
            {
                var callbacknameCfg = powerConfigurationChildCallback[i].getChildById("NameCallbackConfig");
                var callbackfunction = powerConfigurationChildCallback[i].getChildById("NameCallbackFunction");
                var callbacktype = powerConfigurationChildCallback[i].getChildById("callbackType");
                var usePara = powerConfigurationChildCallback[i].getChildById("prameterEnable").getValue();;
                var para = powerConfigurationChildCallback[i].getChildById("callbackData");
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Configuration structure " + callbacknameCfg.getValue() + " */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "power_manager_callback_user_config_t " + callbacknameCfg.getValue() + " = {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callbackFunction = &" + callbackfunction.getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callbackType     = " + callbacktype.getValue() + "," + NEW_LINE;
                if (usePara != false)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .callbackData     = &" + para.getValue() + NEW_LINE;
                }
                else
                {
                    para.setValue("");
                    cFileSections[GLOBAL_VARS_SECTION] += "    .callbackData     = (void *)0" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
            }
            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Array of pointers to User defined Callbacks configuration structures */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "power_manager_callback_user_config_t * powerStaticCallbacksConfigsArr[] = {" + NEW_LINE;
            for(var i = 0; i < countCallbackCfg;i++)
            {
                var callbacknameCfg = powerConfigurationChildCallback[i].getChildById("NameCallbackConfig");
                if (i != (countCallbackCfg - 1))
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    &" + callbacknameCfg.getStringValue() + "," + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    &" + callbacknameCfg.getStringValue() + NEW_LINE;
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] +="};\n" + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] +="/*! @brief Array of pointers to User defined Callbacks configuration structures */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] +="power_manager_callback_user_config_t * powerStaticCallbacksConfigsArr[] = {(void *)0};\n";
        }
    }
}
