/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// IC_PAL component code generator class.
function IC_PAL_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "IC_PAL_CodeGenerator";
    }
    
    // Generate the configuration file
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "ic_pal_cfg.h\n";
        
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if (peripheralName.indexOf("FTM_") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("ic_pal_cfg.h", "IC_PAL_OVER_FTM", "");
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("ic_pal_cfg.h", "NO_OF_FTM_INSTS_FOR_IC");
        }
    }
    
    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ic_pal.h");
        hFileSections[INCLUDES_SECTION] += addStdInclude("stddef.h");
        var childContext = uiConfig.getChildContext();
        var configName = childContext.getComponentInstanceConfig().getName();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        
        var icPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".icPalConfig");
        var icPalConfigList = icPalConfig.getChildById("icPalChnConfigList");
        var icPalCfgChnDetail = icPalConfigList.getChildren();
        var icPalNumOfChnCfg = icPalCfgChnDetail.length;
        var icPalInstConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".icPalInstanceCfg");
        var icPalConst = icPalInstConfig.getChildById("readOnly").getValue() ? "const " : "";
        var instanceCfg = icPalInstConfig.getChildById("name").getValue();

        hFileSections[DEFINES_SECTION] += "/*! @brief PAL instance information */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern " + icPalConst + "ic_instance_t " + peripheralName + "_" + instanceCfg + ";" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for " + peripheralName + "*/" + NEW_LINE;
        if(icPalNumOfChnCfg > 0)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ic_input_ch_param_t  " + peripheralName + "_ChnConfig[" + icPalNumOfChnCfg +"];" + NEW_LINE;
        }
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* The extension structure for the " + peripheralName + " */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern extension_ftm_for_ic_t " + peripheralName + "_ftmExtension;" + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* The channel extension structure for " + peripheralName + " */" + NEW_LINE;
        for (var i = 0; i < icPalNumOfChnCfg; i++)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern channel_extension_ftm_for_ic_t " + peripheralName + "_ftmChnExtension" + i + ";" + NEW_LINE;
        }
        
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of " + peripheralName + " */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ic_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();

        var icPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".icPalConfig");
        var icPalConfigList = icPalConfig.getChildById("icPalChnConfigList");
        var icPalCfgChnDetail = icPalConfigList.getChildren();
        var icPalNumOfChnCfg = icPalCfgChnDetail.length;
        var icPalNumOfChn = icPalConfig.getChildById("icPalNumOfChnConfig");
        icPalNumOfChn.setValue(icPalNumOfChnCfg);
        
        var icPalClkConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".icPalClockConfig");
        var icPalInpClkSrc = icPalClkConfig.getChildById("ClockSource");
        var icPalPrescal = icPalClkConfig.getChildById("icPalPrescaler");
        var icPalInstConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".icPalInstanceCfg");
        var icPalConst = icPalInstConfig.getChildById("readOnly").getValue() ? "const " : "";
        var instanceCfg = icPalInstConfig.getChildById("name").getValue();

        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PAL instance information */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += icPalConst + "ic_instance_t " + peripheralName + "_" + instanceCfg+ " = { IC_INST_TYPE_FTM, " + instanceName.substring(4, 5) + "U };" + NEW_LINE;
        if(icPalNumOfChnCfg > 0)
        {
            for (var i = 0; i < icPalNumOfChnCfg; i++)
            {
                var callback = icPalCfgChnDetail[i].getChildById("channelCallbacks");
                var callbackPara = icPalCfgChnDetail[i].getChildById("channelCallbackParams");

                if (callback.getValue() != "NULL")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/* Callback function of channel configuration " + i + " */";
                    cFileSections[GLOBAL_VARS_SECTION] += "\nextern void " + callback.getValue() + "(ic_event_t event, void *userData);" + NEW_LINE;
                }
                if (callbackPara.getValue() != "NULL")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/* Callback parameter of channel configuration " + i + " */";
                    cFileSections[GLOBAL_VARS_SECTION] += "\nextern " + callbackPara.getValue() + ";" + NEW_LINE;
                }
            }

            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Channel configuration array */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ic_input_ch_param_t " + peripheralName + "_ChnConfig[" + icPalNumOfChnCfg+ "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;

            for (var i = 0; i < icPalNumOfChnCfg; i++)
            {
                var icPalCfgName = icPalCfgChnDetail[i].getChildById("icPalName");
                icPalCfgName.setValue(peripheralName + "_ChannelConfig" + i);

                var callbackPara = icPalCfgChnDetail[i].getChildById("channelCallbackParams")

                var callbackParaVal = "NULL"
                var callbackParaIdx = callbackPara.getValue().indexOf(" ")
                if (callbackParaIdx > 1)
                {
                    callbackParaVal = "&" + callbackPara.getValue().substring(callbackParaIdx + 1)
                }

                cFileSections[GLOBAL_VARS_SECTION] += "    /* Channel configuration " + i + " */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .hwChannelId           = " + icPalCfgChnDetail[i].getChildById("hwChannelId").getValue() + ", /* Hardware channel ID */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .inputCaptureMode      = " + icPalCfgChnDetail[i].getChildById("inputCaptureMode").getValue() + ", /* Input capture mode */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .filterEn              = " + icPalCfgChnDetail[i].getChildById("filterEn").getValue() + ", /* Enable/Disable filter */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .filterValue           = " + icPalCfgChnDetail[i].getChildById("filterValue").getValue() + "U, /* filter value */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelExtension      = &" + peripheralName + "_ftmChnExtension" + i + ", /* channel extension */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelCallbackParams = " + callbackParaVal + ", /* callback parameter */" + NEW_LINE;
                if (icPalCfgChnDetail[i].getChildById("channelCallbacks").getStringValue() == "")
                {
                    icPalCfgChnDetail[i].getChildById("channelCallbacks").setValue("NULL");
                }
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelCallbacks      = " + icPalCfgChnDetail[i].getChildById("channelCallbacks").getValue() + " /* Callback */" + NEW_LINE;

                if (i == (icPalNumOfChnCfg - 1))
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }
        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief The channel extension configurations */" + NEW_LINE;
        for (var i = 0; i < icPalNumOfChnCfg; i++)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\nchannel_extension_ftm_for_ic_t " + peripheralName + "_ftmChnExtension" + i + " ="+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    /* Continuous measurement for configuration " + i + " */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    .continuousModeEn = " + icPalCfgChnDetail[i].getChildById("continuousMode").getValue() + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }
        
        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief IP specific extension configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "extension_ftm_for_ic_t  " + peripheralName + "_ftmExtension =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .ftmClockSource       = " + icPalInpClkSrc.getValue() + ", /* The clock source */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .ftmPrescaler         = FTM_CLOCK_DIVID_BY_" + icPalPrescal.getValue() + " /* The prescaler for input capture counter */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Input capture configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ic_config_t  " + peripheralName + "_InitConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .nNumChannels  = " + icPalNumOfChnCfg + "U," + NEW_LINE;
        if(icPalNumOfChnCfg > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    .inputChConfig = " + peripheralName + "_ChnConfig," + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    .inputChConfig = NULL," + NEW_LINE;
        }
        cFileSections[GLOBAL_VARS_SECTION] += "    .extension     = &" + peripheralName + "_ftmExtension" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
    }
}