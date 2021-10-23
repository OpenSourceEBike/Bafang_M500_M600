/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* default value when blank on GUI */
function SetDefaultValue(parent, child, defvalue)
{
    var childvalue = parent.getChildById(child);

    if (childvalue.getStringValue() == "")
    {
        childvalue.setValue(defvalue);
    };
}

/* SAI component code generator class. */
function Sai_CodeGenerator(uiConfig) {
    this.uiCOnfig = uiConfig;

    /* Returns the class name. */
    this.toString = function () {
        return "Sai_CodeGenerator";
    }

    /* Adds component specific info to header file sections. */
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();

        // Get array user configuration
        var saiCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".saiCfgArray").getChildren();
        var saiCfgArray_Size = saiCfgArray_Items.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("sai_driver.h");
        hFileSections[INCLUDES_SECTION] += "#include <stddef.h>\n";

        //Add Defines info.
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), peripheralName.match(/\d+$/)[0]);

        for (var i =  0; i < saiCfgArray_Size; i++)
        {
            var commonsaiSetting = saiCfgArray_Items[i].getChildById("saiCommonConfig");
            var commonTxorRx = commonsaiSetting.getChildById("txOrRx").getValue();
            var commonName = commonsaiSetting.getChildById("name").getValue();
            var commonReadOnly = commonsaiSetting.getChildById("readOnly").getValue();
            var commonCallBack = commonsaiSetting.getChildById("callBack").getValue();

            if (commonCallBack != "NULL")
            {
                hFileSections[DEFINES_SECTION] += NEW_LINE + "/* User's callback function */" + NEW_LINE;
                hFileSections[DEFINES_SECTION] += "extern void " + commonCallBack + "(sai_report_type_t event, void *userData);" + NEW_LINE;
            }
            if (commonTxorRx == 'SAI_TX')
            {
                if(peripheralName == 'SAI0')
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern sai_state_t", "sai0TxState" + i);
                }
                else
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern sai_state_t", "sai1TxState" + i);
                }
            }
            else
            {
                if(peripheralName == 'SAI0')
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern sai_state_t", "sai0RxState" + i);
                }
                else
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern sai_state_t", "sai1RxState" + i);
                }
            }
            if(commonReadOnly)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const sai_user_config_t", commonName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern sai_user_config_t", commonName);
            }
        }
}

    /* Adds component specific info to source file sections. */
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configName = childContext.getComponentInstanceConfig().getName();

            // Get array user configuration
            var saiCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".saiCfgArray").getChildren();
            var saiCfgArray_Size = saiCfgArray_Items.size();

            /* Process parameters about extension */
            if ((saiCfgArray_Items != null) && (saiCfgArray_Size > 0))
            {
                /* User Configuration */

                for (var i =  0; i < saiCfgArray_Size; i++)
                {
                    // Common configuration
                    var commonSaiSetting = saiCfgArray_Items[i].getChildById("saiCommonConfig");
                    var commonTxorRx = commonSaiSetting.getChildById("txOrRx").getValue();
                    var commonName = commonSaiSetting.getChildById("name").getValue();
                    var commonReadOnly = commonSaiSetting.getChildById("readOnly").getValue();
                    var commonElementSize = commonSaiSetting.getChildById("elementSize").getValue();
                    var commondmaChannel0 = commonSaiSetting.getChildById("dmaChannel0").getValue();
                    var commondmaChannel1 = commonSaiSetting.getChildById("dmaChannel1").getValue();
                    var commondmaChannel2 = commonSaiSetting.getChildById("dmaChannel2").getValue();
                    var commondmaChannel3 = commonSaiSetting.getChildById("dmaChannel3").getValue();
                    var commonRunErr = commonSaiSetting.getChildById("runErr").getValue();
                    var commonSyncErr = commonSaiSetting.getChildById("syncErr").getValue();
                    var commonFrameStart = commonSaiSetting.getChildById("frameStart").getValue();
                    var commontransferType= commonSaiSetting.getChildById("transferType").getValue();
                    var commonCallBack= commonSaiSetting.getChildById("callBack").getValue();

                    // Specific configuration
                    var specificSaiSetting = saiCfgArray_Items[i].getChildById("saiSpecificConfig");
                    var specificBitClkDivisor = specificSaiSetting.getChildById("bitClkDivisor").getValue();
                    var specificMode = specificSaiSetting.getChildById("mode").getValue();
                    var specificSaiChannel0 = specificSaiSetting.getChildById("saiChannel0").getValue();
                    var specificSaiChannel1 = specificSaiSetting.getChildById("saiChannel1").getValue();
                    var specificSaiChannel2 = specificSaiSetting.getChildById("saiChannel2").getValue();
                    var specificSaiChannel3 = specificSaiSetting.getChildById("saiChannel3").getValue();
                    var specificBitClkPolar = specificSaiSetting.getChildById("bitClkPolar").getValue();
                    var specificFirstBitIndex = specificSaiSetting.getChildById("firstBitIndex").getValue();
                    var specificFrameSize = specificSaiSetting.getChildById("frameSize").getValue();
                    var specificMaskMode = specificSaiSetting.getChildById("maskMode").getValue();
                    var specificMasterClkSource = specificSaiSetting.getChildById("masterClkSource").getValue();
                    var specificMasterBitOrder = specificSaiSetting.getChildById("bitOrder").getValue();
                    var specificMuxMode = specificSaiSetting.getChildById("muxMode").getValue();
                    var specificNumberOfChannel = specificSaiSetting.getChildById("numberOfChannel").getValue();
                    var specificSyncEarly = specificSaiSetting.getChildById("syncEarly").getValue();
                    var specificSyncMode = specificSaiSetting.getChildById("syncMode").getValue();
                    var specificSyncPolar = specificSaiSetting.getChildById("syncPolar").getValue();
                    var specificSyncWidth = specificSaiSetting.getChildById("syncWidth").getValue();
                    var specificFirstWordWidth= specificSaiSetting.getChildById("firstWordWidth").getValue();
                    var specificOtherWordsWidth= specificSaiSetting.getChildById("otherWordsWidth").getValue();
                    var specificSampleRate = specificSaiSetting.getChildById("sampleRate").getValue();

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* User configuration */";
                    if(commonReadOnly)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const sai_user_config_t " + commonName + " = {";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "sai_user_config_t " + commonName + " = {";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkDiv = " + specificBitClkDivisor + "U,";
                    var bitClkFreq = ((specificFrameSize - 1) * specificOtherWordsWidth + specificFirstWordWidth) * specificSampleRate;
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkFreq = " + bitClkFreq + "U,";
                    if(specificMode == 'SAI_MASTER')
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkInternal = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkInternal = false,";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .ChannelEnable = ";
                    if(specificSaiChannel0)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "SAI_CHANNEL_0";
                        if(specificSaiChannel1 || specificSaiChannel2 || specificSaiChannel3)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += "|";
                        }
                    }
                    if(specificSaiChannel1)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "SAI_CHANNEL_1";
                        if(specificSaiChannel2 || specificSaiChannel3)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += "|";
                        }
                    }
                    if(specificSaiChannel2)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "SAI_CHANNEL_2";
                        if(specificSaiChannel3)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += "|";
                        }
                    }
                    if(specificSaiChannel3)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "SAI_CHANNEL_3";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += ",";
                    if(specificBitClkPolar == 'true')
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkNegPolar = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .BitClkNegPolar = false,";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .ElementSize = " + commonElementSize + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .FirstBitIndex = " + specificFirstBitIndex + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .FrameSize = " + specificFrameSize + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .MaskMode = " + specificMaskMode + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .MasterClkSrc = " + specificMasterClkSource + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .MsbFirst = " + specificMasterBitOrder + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .MuxMode = " + specificMuxMode + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .ChannelCount = " + specificNumberOfChannel + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .DmaChannel[0] = " + commondmaChannel0 + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .DmaChannel[1] = " + commondmaChannel1 + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .DmaChannel[2] = " + commondmaChannel2 + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .DmaChannel[3] = " + commondmaChannel3 + "U,";
                    if(commonRunErr)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .RunErrorReport = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .RunErrorReport = false,";
                    }
                    if(commonSyncErr)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncErrorReport = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncErrorReport = false,";
                    }
                    if(commonFrameStart)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .FrameStartReport = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .FrameStartReport = false,";
                    }
                    if(specificSyncEarly)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncEarly = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncEarly = false,";
                    }
                    if(specificMode == 'SAI_MASTER')
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncInternal = true,";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncInternal = false,";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncMode = " + specificSyncMode + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncNegPolar = " + specificSyncPolar + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .SyncWidth = " + specificSyncWidth + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .TransferType = " + commontransferType + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .Word0Width = " + specificFirstWordWidth + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .WordNWidth = " + specificOtherWordsWidth + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .callback = " + commonCallBack + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/*! Driver state structure */";
                for (var i =  0; i < saiCfgArray_Size; i++)
                {
                    // Common configuration
                    var commonSaiSetting = saiCfgArray_Items[i].getChildById("saiCommonConfig");
                    var commonTxorRx = commonSaiSetting.getChildById("txOrRx").getValue();
                    if (commonTxorRx == 'SAI_TX')
                    {
                        if(peripheralName == 'SAI0')
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "sai_state_t sai0TxState" + i + ";";
                        }
                        else
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "sai_state_t sai1TxState" + i + ";";
                        }
                    }
                    else
                    {
                        if(peripheralName == 'SAI0')
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "sai_state_t sai0RxState" + i + ";";
                        }
                        else
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "sai_state_t sai1RxState" + i + ";";
                        }
                    }
                }
            }
        }
        catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
