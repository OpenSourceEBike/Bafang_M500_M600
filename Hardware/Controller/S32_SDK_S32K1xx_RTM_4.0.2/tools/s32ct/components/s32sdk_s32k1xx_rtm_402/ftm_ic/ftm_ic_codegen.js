/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// FTM_IC component code generator class.
function FTM_IC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FTM_IC_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ftm_ic_driver.h");
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".totalConfiguration");
        var ftmInitConfig = ftmConfig.getChildById("ftmUserConfig");

        var ftmIcConfig = ftmConfig.getChildById("icConfiguration");
        var ftmIcConfigDetail = ftmIcConfig.getChildById("arrayicConfig");
        var ftmIcConfigDetailChild = ftmIcConfigDetail.getChildren();
        var countftmIcConfigDetailChild = ftmIcConfigDetailChild.length;
        hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(4, 5) + "U") + NEW_LINE;
        
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for flexTimer_ic1* input capture channels */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_input_ch_param_t " + peripheralName + "_InputCaptureChannelConfig[" + countftmIcConfigDetailChild + "];" + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for flexTimer_ic1 input capture */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_input_param_t " + peripheralName + "_InputCaptureConfig;" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of flexTimer_ic1 */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_user_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;

    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".totalConfiguration");
        var ftmInitConfig = ftmConfig.getChildById("ftmUserConfig");
        //var powerMode = ftmInitConfig.getChildById("PowerMode").getValue();
        var prescaler = ftmInitConfig.getChildById("ClockSourcePrescaler").getValue();
        var clockSelect = ftmInitConfig.getChildById("ClockSource").getValue();
        //var clockSourceExternal = ftmInitConfig.getChildById("ExternalClock").getValue();

        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_user_config_t " + peripheralName + "_InitConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        true,    /* Software trigger state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 1 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 2 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 3 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Max loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Min loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for INVCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for SWOCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for OUTMASK register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for CNTIN register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Automatic clear of the trigger */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_UPDATE_NOW, /* Synchronization point */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_MODE_INPUT_CAPTURE, /* Mode of operation for FTM */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_CLOCK_DIVID_BY_" + prescaler + ", /* FTM clock prescaler */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + clockSelect + ",   /* FTM clock source */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("DebugMode").getValue() + ", /* FTM debug mode */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("TimerOverInterrupt").getValue() +",    /* Interrupt state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("InitializationTrigger").getValue() +"     /* Initialization trigger */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        var ftmIcConfig = ftmConfig.getChildById("icConfiguration");
        var ftmIcConfigDetail = ftmIcConfig.getChildById("arrayicConfig");
        var ftmIcConfigDetailChild = ftmIcConfigDetail.getChildren();
        var countftmIcConfigDetailChild = ftmIcConfigDetailChild.length;
        var numConfig = ftmIcConfig.getChildById("numofChannel").setValue(countftmIcConfigDetailChild);
        var maxTimerConfig = ftmIcConfig.getChildById("maxCountVal");
        if (maxTimerConfig.getValue() == 0)
        {
            maxTimerConfig.setValue(65535);
        }
        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Input capture configuration for " + peripheralName + " */"+ NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_input_param_t " + peripheralName + "_InputCaptureConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + countftmIcConfigDetailChild + "U, /* Number of channel configurations */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + maxTimerConfig.getValue() + "U, /* Maximum count value */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + peripheralName + "_InputCaptureChannelConfig /* Channels configuration*/" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        if (countftmIcConfigDetailChild > 0)
        {

            for (var i = 0; i < countftmIcConfigDetailChild; i++) 
            {
                var callback = ftmIcConfigDetailChild[i].getChildById("callback");
                var callbackPara = ftmIcConfigDetailChild[i].getChildById("callbackParameter");
                if (callback.getValue() == "")
                {
                    callback.setValue("NULL");
                }
                if (callbackPara.getValue() == "")
                {
                    callbackPara.setValue("NULL");
                }
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
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Channels configuration structure for " + peripheralName + " input capture */"+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_input_ch_param_t " + peripheralName + "_InputCaptureChannelConfig[" + countftmIcConfigDetailChild + "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            var checkConflictChannel = false;
            for (var i = 0; i < countftmIcConfigDetailChild; i++) 
            {
                var icConfigName = ftmIcConfigDetailChild[i].getChildById("configName");
                var ichwid = ftmIcConfigDetailChild[i].getChildById("hwID");
                var ichwidmode = ftmIcConfigDetailChild[i].getChildById("operationMode");
                var edgeMode = ftmIcConfigDetailChild[i].getChildById("edgeDetectMode");
                var measureMode = ftmIcConfigDetailChild[i].getChildById("measureMode");
                var enableContinousMode = ftmIcConfigDetailChild[i].getChildById("continousMode");
                var enableFilter = ftmIcConfigDetailChild[i].getChildById("inputCaptureFilter");
                var filterValue = ftmIcConfigDetailChild[i].getChildById("inputCaptureFilterValue");
                var callback = ftmIcConfigDetailChild[i].getChildById("callback");
                var callbackPara = ftmIcConfigDetailChild[i].getChildById("callbackParameter");

                var callbackParaVal = "NULL"
                var callbackParaIdx = callbackPara.getValue().indexOf(" ")
                if (callbackParaIdx > 1)
                {
                    callbackParaVal = "&" + callbackPara.getValue().substring(callbackParaIdx + 1)
                }

                icConfigName.setValue(peripheralName + "_ChannelConfig" + i);

                cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ichwid.getValue() + "U, /* Channel id */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ichwidmode.getValue() + ", /* Input capture operation mode */" + NEW_LINE;
                if (ichwidmode.getValue() == "FTM_EDGE_DETECT")
                {
                    measureMode.setValue("FTM_NO_MEASUREMENT");
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + edgeMode.getValue() + ", /* Edge alignment mode */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        FTM_NO_MEASUREMENT, /* Signal measurement operation type */" + NEW_LINE;
                }
                else
                {
                    edgeMode.setValue("FTM_RISING_EDGE");
                    cFileSections[GLOBAL_VARS_SECTION] += "        FTM_RISING_EDGE, /* Edge alignment mode */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + measureMode.getValue() + ", /* Signal measurement operation type */" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "        " + filterValue.getValue() + "U, /* Filter value */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + enableFilter.getValue() + ", /* Filter state (enabled/disabled) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + enableContinousMode.getValue() + ", /* Continuous measurement state */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + callbackParaVal + ", /* Vector of callbacks  parameters for channels events */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + callback.getValue() + " /* Vector of callbacks for channels events */" + NEW_LINE;

                if (i == countftmIcConfigDetailChild - 1)
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
    }
}