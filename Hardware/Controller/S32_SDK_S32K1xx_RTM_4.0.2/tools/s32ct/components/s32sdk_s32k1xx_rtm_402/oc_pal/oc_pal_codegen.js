/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// OC_PAL component code generator class.
function OC_PAL_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "OC_PAL_CodeGenerator";
    }

    // Generate the configuration file
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "oc_pal_cfg.h\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if (peripheralName.indexOf("FTM") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("oc_pal_cfg.h", "OC_PAL_OVER_FTM", "");
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("oc_pal_cfg.h", "NO_OF_FTM_INSTS_FOR_OC");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("oc_pal.h");
        hFileSections[INCLUDES_SECTION] += addStdInclude("stddef.h");
        var ocPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ocPalConfig");
        var ocPalConfigList = ocPalConfig.getChildById("ocPalChnConfigList");
        var ocPalCfgChnDetail = ocPalConfigList.getChildren();
        var ocPalNumOfChnCfg = ocPalCfgChnDetail.length;
        var ocPalInstConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ocPalInstanceCfg");
        var ocPalConst = ocPalInstConfig.getChildById("readOnly").getValue() ? "const " : "";
        var childContext = uiConfig.getChildContext();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();

        var peripheralName = ocPalInstConfig.getChildById("name").getValue() + "_instance_" + instanceName.substring(4,5);

        hFileSections[DEFINES_SECTION] += "/*! @brief PAL instance information */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern " + ocPalConst + "oc_instance_t " + peripheralName + ";" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Channels configuration structure for " + peripheralName + "*/" + NEW_LINE;
        if(ocPalNumOfChnCfg > 0)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern oc_output_ch_param_t  " + peripheralName + "_ChnConfig[" + ocPalNumOfChnCfg + "];" + NEW_LINE;
        }
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief The extension structure for the " + peripheralName + " */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern extension_ftm_for_oc_t " + peripheralName + "_ftmExtension;" + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Global configuration of " + peripheralName + " */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern oc_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;

        for (var i = 0; i < ocPalNumOfChnCfg; i++)
        {
            if(ocPalCfgChnDetail[i].getChildById("channelCallbacks").getValue() != "NULL")
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Callback function of channel configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + ocPalCfgChnDetail[i].getChildById("channelCallbacks").getValue() + "(oc_event_t event, void * userData);" + NEW_LINE;
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();

        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();

        var ocPalConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ocPalConfig");
        var ocPalConfigList = ocPalConfig.getChildById("ocPalChnConfigList");
        var ocPalCfgChnDetail = ocPalConfigList.getChildren();
        var ocPalNumOfChnCfg = ocPalCfgChnDetail.length;
        var ocPalMaxCount = ocPalConfig.getChildById("ocPalMaxCountValue");

        var ocPalClkConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ocPalClockConfig");
        var ocPalInpClkSrc = ocPalClkConfig.getChildById("ocPalInputClkSrc");
        var ocPalPrescal = ocPalClkConfig.getChildById("ocPalPrescaler");
        var ocPalInstConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ocPalInstanceCfg");
        var peripheralName = ocPalInstConfig.getChildById("name").getValue() + "_instance_" + instanceName.substring(4,5);
        var ocPalConst = ocPalInstConfig.getChildById("readOnly").getValue() ? "const " : "";

        cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief PAL instance information */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += ocPalConst + "oc_instance_t " + peripheralName + " = { OC_INST_TYPE_FTM, " + instanceName.substring(4, 5) + "U };" + NEW_LINE;
        if(ocPalNumOfChnCfg > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Channel configuration array */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "oc_output_ch_param_t " + peripheralName + "_ChnConfig[" + ocPalNumOfChnCfg+ "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;

            for (var i = 0; i < ocPalNumOfChnCfg; i++)
            {
                if (ocPalCfgChnDetail[i].getChildById("comparedValue").getValue() > ocPalMaxCount.getValue())
                {
                    scriptApi.logError(" The compare value of channel " + ocPalCfgChnDetail[i].getChildById("hwChannelId").getValue() + " must be lesser than the maximum count value");
                }

                cFileSections[GLOBAL_VARS_SECTION] += "    /* Channel configuration " + i + " */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .hwChannelId           = " + ocPalCfgChnDetail[i].getChildById("hwChannelId").getValue() + ", /* Hardware channel ID */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .chMode                = " + ocPalCfgChnDetail[i].getChildById("chMode").getValue() + ", /* Output Compare mode */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .comparedValue         = " + ocPalCfgChnDetail[i].getChildById("comparedValue").getValue() + "U, /* compared value */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelExtension      = " + "NULL" + ", /* channel extension */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelCallbackParams = " + ocPalCfgChnDetail[i].getChildById("channelCallbackParams").getValue() + ", /* callback parameter */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        .channelCallbacks      = " + ocPalCfgChnDetail[i].getChildById("channelCallbacks").getValue() + " /* Callback */" + NEW_LINE;

                if (i == (ocPalNumOfChnCfg - 1))
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
        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief IP specific extension configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "extension_ftm_for_oc_t  " + peripheralName + "_ftmExtension =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .maxCountValue        = " + ocPalMaxCount.getValue() + ", /* Max count value of output compared counter */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .ftmClockSource       = " + ocPalInpClkSrc.getValue() + ", /* The clock source */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .ftmPrescaler         = " + ocPalPrescal.getValue() + " /* The prescaler for output compare counter */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Output compare configuration */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "oc_config_t  " + peripheralName + "_InitConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    .nNumChannels   = " + ocPalNumOfChnCfg + "U," + NEW_LINE;
        if(ocPalNumOfChnCfg > 0) {
            cFileSections[GLOBAL_VARS_SECTION] += "    .outputChConfig = " + peripheralName + "_ChnConfig," + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    .outputChConfig = NULL," + NEW_LINE;
        }
        cFileSections[GLOBAL_VARS_SECTION] += "    .extension      = &" + peripheralName + "_ftmExtension" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
    }
}