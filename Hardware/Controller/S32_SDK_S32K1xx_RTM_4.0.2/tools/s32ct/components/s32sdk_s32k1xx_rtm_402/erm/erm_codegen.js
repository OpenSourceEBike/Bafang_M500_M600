/*
 * Copyright 2018-2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// SWT component code generator class.
function ERM_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "ERM_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var configList = getSetting(this.uiConfig, this.uiConfig.getName() + ".ermConfigArray").getChildren();
        var interruptList = getSetting(this.uiConfig, this.uiConfig.getName() + ".ermInterruptConfigArray").getChildren();
        var configListSize = configList.size();
        var interruptListSize = interruptList.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("erm_driver.h");

        if (configList != null)
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += "\n/* ERM instance number configured */\n";
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + 0 + "U)");
            hFileSections[DEFINES_SECTION] += "\n/* Number of configured channels for ERM instances */\n";

            for (var i = 0; i < configListSize; i++)
            {
                if(configList[i].getChildById("channelCount").getValue() > 0)
                {
                    hFileSections[DEFINES_SECTION] += addDefine("ERM_CHANNEL_COUNT" + i, "(" + configList[i].getChildById("channelCount").getValue() + "U)");
                }
            }
            
            //Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configurations */" + NEW_LINE;

            //Interrupt configurations
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Interrupt configurations */" + NEW_LINE;
            for (var i = 0; i < interruptListSize; i++)
            {
                var interruptConfigName = interruptList[i].getChildById("name").getStringValue();
                var interruptConfigReadOnly = (interruptList[i].getChildById("interruptConfigReadOnly").getValue() == true) ? "const " : "";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + interruptConfigReadOnly + "erm_interrupt_config_t", interruptConfigName);
            }

            //ERM user configurations
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* ERM configurations */" + NEW_LINE;
            for (var i = 0; i < configListSize; i++)
            {
                if(configList[i].getChildById("channelCount").getValue() > 0)
                {
                    var ermConfigName = configList[i].getChildById("name").getStringValue();
                    var ermConfigReadOnly = (configList[i].getChildById("userConfigReadOnly").getValue() == true) ? "const " : "";

                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + ermConfigReadOnly + "erm_user_config_t", ermConfigName + "[ERM_CHANNEL_COUNT" + i + "]");
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try
        {
            var childContext = uiConfig.getChildContext();
            var configList = getSetting(this.uiConfig, this.uiConfig.getName() + ".ermConfigArray").getChildren();
            var interruptList = getSetting(this.uiConfig, this.uiConfig.getName() + ".ermInterruptConfigArray").getChildren();
            var configListSize = configList.size();
            var interruptListSize = interruptList.size();

            if(configList != null)
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Interrupt configurations */\n";

                for (var i = 0 ; i < interruptListSize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Interrupt configuration " + i  + " */";
                    
                    var interruptConfigName = interruptList[i].getChildById("name").getStringValue();
                    var interruptConfigReadOnly = (interruptList[i].getChildById("interruptConfigReadOnly").getValue() == true) ? "const " : "";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + interruptConfigReadOnly + "erm_interrupt_config_t " + interruptConfigName + " =";

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "{";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .enableSingleCorrection = " + interruptList[i].getChildById("singleBitInterruptEnable").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .enableNonCorrectable = " + interruptList[i].getChildById("nonCorrectableInterruptEnable").getValue();
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};\n";
                }

                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* ERM configurations */";
                for (var i = 0 ; i < configListSize; i++)
                {
                    var ermChannelCount = configList[i].getChildById("channelCount").getValue();

                    if(ermChannelCount > 0)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "\n/* ERM configuration " + i  + " */";
                        
                        var ermConfigName = configList[i].getChildById("name").getStringValue();
                        var ermConfigReadOnly = (configList[i].getChildById("userConfigReadOnly").getValue() == true) ? "const " : "";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + ermConfigReadOnly + "erm_user_config_t " + ermConfigName + "[ERM_CHANNEL_COUNT" + i + "] =";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "{";

                        var ermChannelList = configList[i].getChildById("ermChannelConfigArray").getChildren();

                        for (var j = 0; j < ermChannelCount; j++)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* Channel configuration " + j + " */";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .channel = " + ermChannelList[j].getChildById("channel").getValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .interruptCfg = &" + ermChannelList[j].getChildById("interruptCfg").getValue();
                            
                            if (j == (ermChannelCount - 1))
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }";
                            }
                            else
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    },";
                            }
                        }

                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
                    }
                }
            }
        }catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
