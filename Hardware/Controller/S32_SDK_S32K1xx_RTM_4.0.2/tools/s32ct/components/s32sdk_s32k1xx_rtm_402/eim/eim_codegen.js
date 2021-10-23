/*
 * Copyright 2018-2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// SWT component code generator class.
function EIM_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "EIM_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".eimConfig");
        var configList = initStruct.getChildren();
        var configListSize = configList.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("eim_driver.h");

        if (initStruct != null)
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += "\n/* EIM instance number configured */\n";
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + 0 + "U)");

            hFileSections[DEFINES_SECTION] += "\n/* Number of configured channels for EIM instances */\n";

            for (var i = 0; i < configListSize; i++)
            {
                if(configList[i].getChildById("channelCount").getValue() > 0)
                {
                    hFileSections[DEFINES_SECTION] += addDefine("EIM_CHANNEL_COUNT" + i, "(" + configList[i].getChildById("channelCount").getValue() + "U)");
            
                }
            }

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User channel configuration structure list configuration */" + NEW_LINE;

            for (var i = 0; i < configListSize; i++)
            {
                if(configList[i].getChildById("channelCount").getValue() > 0)
                {
                    var eimConfigName = configList[i].getChildById("name").getStringValue();
                    var eimConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";

                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + eimConfigReadOnly + "eim_user_channel_config_t", eimConfigName + "[EIM_CHANNEL_COUNT" + i + "]");
                }
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
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".eimConfig");
            var configList = initStruct.getChildren();
            var configListSize = configList.size();

            if (initStruct != null)
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Channel configurations */"
            
                for (var i = 0; i < configListSize; i++)
                {
                    var eimConfigName = configList[i].getChildById("name").getStringValue();
                    var eimConfigReadOnly = (configList[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    var eimChannelCount =  configList[i].getChildById("channelCount").getValue();
                    
                    if(eimChannelCount > 0)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + eimConfigReadOnly + "eim_user_channel_config_t " + eimConfigName + "[EIM_CHANNEL_COUNT" + i + "] =";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "{";

                        for (var j = 0; j < eimChannelCount; j++)
                        {
                            var channelConfigList = configList[i].getChildById("channelConfig").getChildren();

                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* Channel configuration " + j + " */";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .channel = " + channelConfigList[j].getChildById("channel").getValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .checkBitMask = " + channelConfigList[j].getChildById("checkBitMask").getValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .dataMask = " + channelConfigList[j].getChildById("dataMask").getValue() + "U,";
                            
                            var channelNumber = channelConfigList[j].getChildById("channel").getValue();
                            if ((channelNumber == 6) || (channelNumber == 7) || (channelNumber == 10) || (channelNumber == 11) || (channelNumber == 12))
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .dataMask1 = " + channelConfigList[j].getChildById("dataMask1").getValue() + "U,";
                            }

                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .enable = " + channelConfigList[j].getChildById("enable").getValue();
                            if (j == (eimChannelCount - 1))
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }";
                            }
                            else
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    },";
                            }
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                    }
                }
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
