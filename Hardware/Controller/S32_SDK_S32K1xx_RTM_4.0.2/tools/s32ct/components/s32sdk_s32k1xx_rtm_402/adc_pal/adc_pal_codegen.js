/*
 * Copyright 2018-2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function adc_pal_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "adc_pal_CodeGenerator";
    }

    // Prepare component for code generation
    this.prepareComponent = function() {
        var childContext         = uiConfig.getChildContext();
        var configStruct         = getSetting(this.uiConfig, this.uiConfig.getName()+".adcPalConfig");
        var instanceName         = childContext.getComponentInstanceConfig().getName();
        var peripheralName       = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceType         = configStruct.getChildById("instanceType").getValue();
        var conversionGroupArray = configStruct.getChildById("conversionGroupArray").getChildren();
        var functionalGroup      = childContext.getFunctionalGroup();

        /* Update default names */
        configStruct.getChildById("name").setValue(instanceName + "_config");
        configStruct.getChildById("extensionName").setValue(instanceName + "_extensionConfig");
        configStruct.getChildById("conversionGroupArrayName").setValue(instanceName + "_groupArray");

        for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
        {
            conversionGroupArray[groupIdx].getChildById("resultsName").setValue(instanceName + "_results" + groupIdx);
            conversionGroupArray[groupIdx].getChildById("chansArrayName").setValue(instanceName + "_channelsArray" + groupIdx);
        }

        /* Check if used instances are present in the project and raise error if they are */
        if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx") {
            /* PDB is mapped 1:1 to ADC instances when used on ADC PAL so if
             * ADC_PAL is used on ADC0, then PDB0 must not be used independently
             */
            var usedPDBInst = "PDB" + peripheralName.charAt(peripheralName.length - 1);
            if (functionalGroup.isPeripheralUsed(usedPDBInst)) {
                scriptApi.logError(usedPDBInst + " is already in use by ADC PAL on instance " + peripheralName + ".");
            }
        } else if ((instanceType == "ADC_INST_TYPE_ADC_SAR") ||
            (instanceType == "ADC_INST_TYPE_ADC_SAR_CTU") || (instanceType == "ADC_INST_TYPE_ADC_SAR_BCTU")) {
            if (((peripheralName == "ADC_0") && (functionalGroup.isPeripheralUsed("ADC_1"))) || ((peripheralName == "ADC_1") && (functionalGroup.isPeripheralUsed("ADC_0")))) {
                scriptApi.logError("ADC PAL uses ADC SAR exclusively. Other ADC instances shall not be used. Only one instance of ADC PAL shall be used.");
            }

            if ((instanceType == "ADC_INST_TYPE_ADC_SAR_CTU") && functionalGroup.isPeripheralUsed("CTU")) {
                scriptApi.logError("ADC PAL uses CTU exclusively. The CTU instance shall not be used.");
            }
        }
    }

    // Adds component specific info to pal_cfg.h header file sections
    this.genHCfgFile = function(hCfgFileSections) {
        var configStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".adcPalConfig");
        var conversionGroupArray = configStruct.getChildById("conversionGroupArray").getChildren();
        var instanceType = configStruct.getChildById("instanceType").getValue();
        // Count HW triggered groups
        var hwTriggeredGroups = 0;
        for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
        {
            if(conversionGroupArray[groupIdx].getChildById("hwTriggerSupport").getValue() == "true")
                hwTriggeredGroups += 1;
        }

        hCfgFileSections[FILE_NAME_SECTION] += "adc_pal_cfg.h\n";

        if (instanceType == "ADC_INST_TYPE_ADC_SAR_CTU")
        {
            hCfgFileSections[DEFINES_SECTION] +=
                addPalInstDefine("adc_pal_cfg.h", "ADC_PAL_TYPE_ADC_SAR_CTU /* Define for selecting one of the ADC PAL type to be used */") +
                addPalDefine("adc_pal_cfg.h", "ADC_PAL_MAX_NUM_HW_GROUPS_EN", hwTriggeredGroups + "u /* Define the maximum number of HW triggered groups which may be enabled simultaneously */") +
                addPalDefine("adc_pal_cfg.h", "ADC_PAL_TOTAL_NUM_GROUPS", conversionGroupArray.size() + "u /* Define the total number of configured groups */");
        }
        else if (instanceType == "ADC_INST_TYPE_ADC_SAR")
        {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("adc_pal_cfg.h", "ADC_PAL_SAR /* Define for selecting one of the ADC PAL type to be used */", "");
        }
        else if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx")
        {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("adc_pal_cfg.h", "ADC_PAL_S32K1xx /* Define for selecting one of the ADC PAL type to be used */", "") +
            addPalDefine("adc_pal_cfg.h", "ADC_PAL_MAX_NUM_HW_GROUPS_EN", "1u /* Define the maximum number of HW triggered groups which may be enabled simultaneously */");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            /* User Configuration Structures */
            var childContext         = uiConfig.getChildContext();
            var instanceName         = childContext.getComponentInstanceConfig().getName();
            var configStruct         = getSetting(this.uiConfig, this.uiConfig.getName()+".adcPalConfig");

            var configStructName     = configStruct.getChildById("name").getValue();
            var configStructReadOnly = configStruct.getChildById("readOnly").getValue();
            var conversionGroupArray     = configStruct.getChildById("conversionGroupArray").getChildren();
            var conversionGroupArrayName = configStruct.getChildById("conversionGroupArrayName").getValue();

            /**** File includes ****/
            hFileSections[INCLUDES_SECTION] += addInclude("adc_pal.h");
            hFileSections[INCLUDES_SECTION] += addStdInclude("stddef.h");

            hFileSections[DEFINES_SECTION] += "\n/*! @brief PAL instance information */\n";
            hFileSections[DEFINES_SECTION] += declareVar("extern const adc_instance_t", instanceName + "_instance");

            if(configStructReadOnly)
            {
                var constAttribute = "const";
            }
            else
            {
                var constAttribute = "";
            }
            hFileSections[DEFINES_SECTION] += "\n/*! @brief configuration structure */\n";
            hFileSections[DEFINES_SECTION] += declareVar("extern " + constAttribute + " adc_config_t", configStructName);

            hFileSections[DEFINES_SECTION] += "\n/*! @brief array of conversion groups */\n";
            hFileSections[DEFINES_SECTION] += declareArray("extern " + constAttribute + " adc_group_config_t", conversionGroupArrayName, conversionGroupArray.size() + "u");

            for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
            {
                var resultsName = conversionGroupArray[groupIdx].getChildById("resultsName").getValue();
                var callback = conversionGroupArray[groupIdx].getChildById("callback").getValue();
                var callbackUserData = conversionGroupArray[groupIdx].getChildById("callbackUserData").getValue();
                var inputChannelsArray = conversionGroupArray[groupIdx].getChildById("inputChannelArray").getChildren();
                var inputChannelsArrayName = conversionGroupArray[groupIdx].getChildById("chansArrayName").getValue();
                var inputChannelsArraySize = inputChannelsArray.size();
                var numSetsResults = conversionGroupArray[groupIdx].getChildById("numSetsResults").getValue();

                if(inputChannelsArraySize > 0)
                {
                    hFileSections[DEFINES_SECTION] += NEW_LINE +
                        "/*! @brief group " + groupIdx + ": input channels array */\n" +
                        declareArray("extern adc_input_chan_t", inputChannelsArrayName, inputChannelsArraySize + "u") +
                        "/*! @brief group " + groupIdx + ": results buffer */\n" +
                        declareArray("extern uint16_t", resultsName, numSetsResults + "u * " + inputChannelsArraySize + "u");
                }

                if(callback != "NULL")
                {
                    hFileSections[DEFINES_SECTION] += "/*! @brief group " + groupIdx + ": callback function */\n" +
                        "void " + callback + "(const adc_callback_info_t * const callbackInfo, void * userData);\n";
                }
                if(callbackUserData != "NULL")
                {
                    hFileSections[DEFINES_SECTION] += "/*! @brief group " + groupIdx + ": callback user data */\n" +
                        declareVar("extern void * ", callbackUserData);
                }
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            /* User Configuration Structures */
            var childContext = uiConfig.getChildContext();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".adcPalConfig");
            var configStructName = configStruct.getChildById("name").getValue();
            var configStructReadOnly = configStruct.getChildById("readOnly").getValue();
            var sampleTicks = configStruct.getChildById("sampleTicks").getValue();
            /* Extension configuration items */
            var instanceType        = configStruct.getChildById("instanceType").getValue();
            var extensionName       = configStruct.getChildById("extensionName").getValue();
            var extensionType       = configStruct.getChildById("extensionType").getValue();
            var conversionGroupArray     = configStruct.getChildById("conversionGroupArray").getChildren();
            var conversionGroupArrayName = configStruct.getChildById("conversionGroupArrayName").getValue();

            /* Get instance index.
             * Only S32K1xx platforms have triggering unit instances mapped 1:1
             * with ADC instances so this is the same as peripheral instance index.
             * Other platforms either have none or a single triggering unit for
             * all ADC instances so this instance is always 0 for them.
             */
            var instanceIndex = 0;
            if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx")
            {
                instanceIndex = peripheralName.charAt(peripheralName.length - 1);
            }

            /**** Generate instance information ****/
            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief PAL instance information */\n" +
                "const adc_instance_t " + instanceName + "_instance = { " + instanceType + ", " +  instanceIndex + "u }; " + NEW_LINE;

            /**** Generate PAL extension ****/
            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief PAL extension */\n" +
                "static " + extensionType + " " + extensionName + " = { " + NEW_LINE;
            if (instanceType == "ADC_INST_TYPE_ADC_SAR_CTU" || instanceType == "ADC_INST_TYPE_ADC_SAR")
            {
                var extensionClkSelect  = configStruct.getChildById("adcClkSel").getValue();
                var extensionRefSelect  = configStruct.getChildById("adcRefSel").getValue();
                var extensionAutoClkOff = configStruct.getChildById("autoClockOff").getValue();
                var extensionCTUPrescaler = configStruct.getChildById("ctuPrescaler").getValue();

                cFileSections[GLOBAL_VARS_SECTION] +=
                    TAB_SPACE + ".clkSelect       = " + extensionClkSelect + "," + NEW_LINE +
                    TAB_SPACE + ".refSelect       = " + extensionRefSelect + "," + NEW_LINE +
                    TAB_SPACE + ".autoClockOff    = " + extensionAutoClkOff;
                if (instanceType == "ADC_INST_TYPE_ADC_SAR_CTU") {
                    cFileSections[GLOBAL_VARS_SECTION] += "," + NEW_LINE + TAB_SPACE + ".ctuPrescaler    = CTU_PRESCALER_" + extensionCTUPrescaler;
                }
            }
            if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx")
            {
                var extensionADCClkDiv  = configStruct.getChildById("adcClkDiv").getValue();
                var extensionADCResolution  = configStruct.getChildById("adcResolution").getValue();
                var extensionADCInputClock = configStruct.getChildById("adcInputClock").getValue();
                var extensionADCVoltageRef = configStruct.getChildById("adcVoltageRef").getValue();
                var extensionADCSupplyMonitoringEnable = configStruct.getChildById("adcSupplyMonitoringEnable").getValue();
                var extensionADCPDBPrescaler = configStruct.getChildById("adcPDBPrescaler").getValue();

                /* If supply monitoring is not available for the current instance then set it to false */
                if (!configStruct.getChildById("adcSupplyMonitoringEnable").isEnabled())
                {
                    extensionADCSupplyMonitoringEnable = false;
                }

                cFileSections[GLOBAL_VARS_SECTION] +=
                    TAB_SPACE + ".clockDivide              = " + extensionADCClkDiv + "," + NEW_LINE +
                    TAB_SPACE + ".resolution               = " + extensionADCResolution + "," + NEW_LINE +
                    TAB_SPACE + ".inputClock               = " + extensionADCInputClock + "," + NEW_LINE +
                    TAB_SPACE + ".voltageRef               = " + extensionADCVoltageRef + "," + NEW_LINE +
                    TAB_SPACE + ".supplyMonitoringEnable   = " + extensionADCSupplyMonitoringEnable + "," + NEW_LINE +
                    TAB_SPACE + ".pdbPrescaler             = " + extensionADCPDBPrescaler;
            }
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;

            /**** Generate Configuration Structure ****/
            if(configStructReadOnly)
            {
                var constAttribute = "const";
            }
            else
            {
                var constAttribute = "";
            }
            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief configuration structure */\n" +
                constAttribute + " adc_config_t " + configStructName + " = { " + NEW_LINE +
                TAB_SPACE + ".groupConfigArray  = " + conversionGroupArrayName + "," + NEW_LINE +
                TAB_SPACE + ".numGroups         = " + conversionGroupArray.size() + "u," + NEW_LINE +
                TAB_SPACE + ".sampleTicks       = " + sampleTicks + "u," + NEW_LINE +
                TAB_SPACE + ".extension         = &" + extensionName + NEW_LINE +
            "};" + NEW_LINE;

            /**** Generate Conversion Group Array Delay Values ****/
            var additionalNewlineAdded = false;
            for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
            {
                var inputChannelsArray = conversionGroupArray[groupIdx].getChildById("inputChannelArray").getChildren();
                var inputChannelsArraySize = inputChannelsArray.size();
                var delayTypeEnable = conversionGroupArray[groupIdx].getChildById("delayType").isEnabled();
                if (delayTypeEnable)
                {
                    var delayType = conversionGroupArray[groupIdx].getChildById("delayType").getValue();
                    if (delayType == "ADC_DELAY_TYPE_GROUP_DELAY")
                    {
                        if (!additionalNewlineAdded) {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                            additionalNewlineAdded = true;
                        }
                        var groupDelay = conversionGroupArray[groupIdx].getChildById("groupDelay").getValue();
                        cFileSections[GLOBAL_VARS_SECTION] += "/* Group delay value for conversion group array " + groupIdx +
                            " */" + NEW_LINE + "uint16_t group" + groupIdx + "_delayValue = " + groupDelay + "u;" + NEW_LINE;
                    }

                    if (delayType == "ADC_DELAY_TYPE_INDIVIDUAL_DELAY")
                    {
                        if (!additionalNewlineAdded) {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                            additionalNewlineAdded = true;
                        }

                        cFileSections[GLOBAL_VARS_SECTION] += "/* Channel delay values for conversion group " + groupIdx +
                            " */" + NEW_LINE + "uint16_t group" + groupIdx + "_delayArray[" + inputChannelsArraySize + "u] = {" + NEW_LINE;

                        for(var chanIdx = 0; chanIdx < inputChannelsArraySize; chanIdx++)
                        {
                            var channelDelay = inputChannelsArray[chanIdx].getChildById("chanDelay").getValue();
                            cFileSections[GLOBAL_VARS_SECTION] += TAB_SPACE + "/* " + chanIdx + " */  " + channelDelay + "u," + NEW_LINE;
                        }

                        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

                    }
                }
            }

            /**** Generate Conversion Group Array ****/
            if(conversionGroupArray.size() > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief array of conversion groups */\n" +
                    constAttribute + " adc_group_config_t " + conversionGroupArrayName + "[" + conversionGroupArray.size() + "u] = {" + NEW_LINE;

                for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
                {
                    var resultsName = conversionGroupArray[groupIdx].getChildById("resultsName").getValue();
                    var callback = conversionGroupArray[groupIdx].getChildById("callback").getValue();
                    var callbackUserData = conversionGroupArray[groupIdx].getChildById("callbackUserData").getValue();
                    var inputChannelsArray = conversionGroupArray[groupIdx].getChildById("inputChannelArray").getChildren();
                    var inputChannelsArrayName = conversionGroupArray[groupIdx].getChildById("chansArrayName").getValue();
                    var inputChannelsArraySize = inputChannelsArray.size();
                    var numSetsResults = conversionGroupArray[groupIdx].getChildById("numSetsResults").getValue();
                    var hwTriggerSupport = conversionGroupArray[groupIdx].getChildById("hwTriggerSupport").getValue();
                    var triggerSource = "0u";
                    var delayType = "ADC_DELAY_TYPE_NO_DELAY";
                    var delayArray = "NULL";
                    var continuousConvEn = conversionGroupArray[groupIdx].getChildById("continuousConvEn").getValue();

                    if (hwTriggerSupport == "true")
                    {
                        triggerSource = conversionGroupArray[groupIdx].getChildById("triggerSource").getValue();
                        delayType = conversionGroupArray[groupIdx].getChildById("delayType").getValue();
                        var delayTypeEnable = conversionGroupArray[groupIdx].getChildById("delayType").isEnabled();
                        if (delayType == "ADC_DELAY_TYPE_GROUP_DELAY" && delayTypeEnable)
                        {
                            delayArray = "&group" + groupIdx + "_delayValue";
                        }
                        else if (delayType == "ADC_DELAY_TYPE_INDIVIDUAL_DELAY" && delayTypeEnable)
                        {
                            delayArray = "group" + groupIdx + "_delayArray";
                        }
                        else
                        {
                            delayArray = conversionGroupArray[groupIdx].getChildById("delayArray").getValue();
                            if (delayArray == "")
                                delayArray = "NULL";
                        }
                        continuousConvEn = false;
                    }
                    else if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx")
                    {
                        triggerSource = "TRGMUX_TRIG_SOURCE_DISABLED";
                    }

                    cFileSections[GLOBAL_VARS_SECTION] += "    /* Conversion group " + groupIdx + " */" + NEW_LINE +
                        TAB_SPACE + "{" + NEW_LINE;

                    if(inputChannelsArraySize > 0)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] +=
                            TAB_SPACE + TAB_SPACE + ".inputChannelArray   = " + inputChannelsArrayName + "," + NEW_LINE +
                            TAB_SPACE + TAB_SPACE + ".resultBuffer        = " + resultsName + "," + NEW_LINE;
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] +=
                            TAB_SPACE + TAB_SPACE + ".inputChannelArray   = " + "NULL," + NEW_LINE +
                            TAB_SPACE + TAB_SPACE + ".resultBuffer        = " + "NULL," + NEW_LINE;
                    }

                    cFileSections[GLOBAL_VARS_SECTION] +=
                        TAB_SPACE + TAB_SPACE + ".numChannels         = " + inputChannelsArraySize + "u," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".numSetsResultBuffer = " + numSetsResults + "u," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".hwTriggerSupport    = " + hwTriggerSupport + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".triggerSource       = " + triggerSource + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".delayType           = " + delayType + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".delayArray          = " + delayArray + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".continuousConvEn    = " + continuousConvEn + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".callback            = " + callback + "," + NEW_LINE +
                        TAB_SPACE + TAB_SPACE + ".callbackUserData    = " + callbackUserData + NEW_LINE +
                        TAB_SPACE + "}," + NEW_LINE;
                }

                cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE + NEW_LINE;
            }

            /**** Generate input channel arrays and result buffers  ****/
            for(var groupIdx = 0; groupIdx < conversionGroupArray.size(); groupIdx++)
            {
                var inputChannelsArray = conversionGroupArray[groupIdx].getChildById("inputChannelArray").getChildren();
                var inputChannelsArraySize = inputChannelsArray.size();
                if(inputChannelsArraySize > 0)
                {
                    var inputChannelsArrayName = conversionGroupArray[groupIdx].getChildById("chansArrayName").getValue();
                    var resultsName = conversionGroupArray[groupIdx].getChildById("resultsName").getValue();
                    var numSetsResults = conversionGroupArray[groupIdx].getChildById("numSetsResults").getValue();
                    var resultsLength = numSetsResults * inputChannelsArraySize;

                    cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief group " + groupIdx + ": input channels array */" + NEW_LINE +
                        "adc_input_chan_t " + inputChannelsArrayName + "[" + inputChannelsArraySize + "u] = {" + NEW_LINE;
                    for(var chanIdx = 0; chanIdx < inputChannelsArraySize; chanIdx++)
                    {
                        var channel = inputChannelsArray[chanIdx].getChildById("adcInputChannel").getValue();
                        cFileSections[GLOBAL_VARS_SECTION] += TAB_SPACE + "/* " + chanIdx + " */  " + channel + "," + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

                    cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief group " + groupIdx + ": results buffer */" + NEW_LINE +
                        "uint16_t " + resultsName + "[" + resultsLength + "u];" + NEW_LINE;

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                }
            }

        } catch(err) {
            scriptApi.logError(err);
        }
    }
}
