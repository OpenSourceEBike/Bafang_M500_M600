/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */
var cIndent2 = "  ";
var cIndent4 = "    ";
var cIndent6 = "      ";
var cIndent8 = "        ";


scriptApi.requireScript("codegeneratorUtils.js");

function PWM_PAL_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "PWM_PAL_CodeGenerator";
    }
    
    // Adds component specific info to header file sections.
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "pwm_pal_cfg.h\n";
        
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if (peripheralName.indexOf("FTM_") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("pwm_pal_cfg.h", "PWM_OVER_FTM", "");
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("pwm_pal_cfg.h", "NO_OF_FTM_INSTS_FOR_PWM");
        }
    }
    
    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("pwm_pal.h");
        var childContext = uiConfig.getChildContext();
        var configName = childContext.getComponentInstanceConfig().getName();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Device instance configuration structure */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern pwm_instance_t " + peripheralName + "_instance;" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Channels configuration structure for " + peripheralName + "*/" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern pwm_global_config_t " + peripheralName + "_configs;" + NEW_LINE;
        
        var pwmPalTimebaseConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".pwmTimebaseConfigArray");
        var pwmPalTimebaseChannels = pwmPalTimebaseConfig.getChildren();
        var pwmPalTimebaseSize = pwmPalTimebaseChannels.length;
        
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief PWM_PAL Timebase configuration*/" +NEW_LINE;
        for (var i = 0; i < pwmPalTimebaseSize; i++)
        {
            var aux = pwmPalTimebaseChannels.get(i);
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern pwm_ftm_timebase_t " + peripheralName + "_timebase" + i + ";" + NEW_LINE;
        }
        
        var pwmPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".pwmGlobalConfigArray");
        var pwmPalConfigChannels = pwmPalConfig.getChildren();
        var pwmPalConfigSize = pwmPalConfigChannels.length;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief PWM_PAL Channel configuration */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern pwm_channel_t " + peripheralName + "_channels[" + pwmPalConfigSize + "];\n";
    }
    
    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
    
        var pwmPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".pwmGlobalConfigArray");
        var pwmPalConfigChannels = pwmPalConfig.getChildren();
        var pwmPalConfigSize = pwmPalConfigChannels.length;
        var pwmPalTimebaseConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".pwmTimebaseConfigArray");
        var pwmPalTimebaseChannels = pwmPalTimebaseConfig.getChildren();
        var pwmPalTimebaseSize = pwmPalTimebaseChannels.length;
        var pwmPalDeadtimeConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".pwmGlobalDeadtimeConfig");
        
        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PWM_PAL Timebase configuration*/" +NEW_LINE;
        for (var i = 0; i < pwmPalTimebaseSize; i++)
        {
            var aux = pwmPalTimebaseChannels.get(i);
            cFileSections[GLOBAL_VARS_SECTION] += "pwm_ftm_timebase_t " + peripheralName + "_timebase" + i + " = \n{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".sourceClock = " + aux.getChildById("sourceClock").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".prescaler = " + aux.getChildById("prescaler").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".deadtimePrescaler = " + aux.getChildById("deadtimePrescaler").getValue() + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE + NEW_LINE;
        }
        
        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PWM_PAL Channel configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "pwm_channel_t " + peripheralName + "_channels[" + pwmPalConfigSize + "] = \n{\n";
        
        for (var i = 0; i < pwmPalConfigSize; i++)
        {
            var aux = pwmPalConfigChannels.get(i);
            var timebaseIdx = aux.getChildById("timebase").getValue().split(" ")[1];
            cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".channel = " + aux.getChildById("channel").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".channelType = " + aux.getChildById("channelType").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".period = " + aux.getChildById("period").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".duty = " + aux.getChildById("duty").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".polarity = " + aux.getChildById("polarity").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".insertDeadtime = " + aux.getChildById("insertDeadtime").getValue() + "," + NEW_LINE;
            if (i == 0)
            {
                if ((pwmPalDeadtimeConfig.getChildById("deadtime").isEnabled()))
                {
                    cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".deadtime = " + pwmPalDeadtimeConfig.getChildById("deadtime").getValue() + "," + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".deadtime = 0," + NEW_LINE;
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".enableComplementaryChannel = " + aux.getChildById("enableComplementaryChannel").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".complementaryChannelPolarity = " + aux.getChildById("complementaryChannelPolarity").getValue() + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent8 + ".timebase = &" + peripheralName + "_timebase" + timebaseIdx + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + "}," + NEW_LINE;
        }
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE + NEW_LINE;
        
        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PWM_PAL Initialization configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "pwm_global_config_t " + peripheralName + "_configs = \n{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".pwmChannels = " + peripheralName + "_channels" + "," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".numberOfPwmChannels = " + pwmPalConfigSize + "," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        
        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PWM_PAL instance information */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "pwm_instance_t " + peripheralName + "_instance = \n{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".instType = PWM_INST_TYPE_FTM," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".instIdx = " + instanceName.split("_")[1] + "," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
    }
}