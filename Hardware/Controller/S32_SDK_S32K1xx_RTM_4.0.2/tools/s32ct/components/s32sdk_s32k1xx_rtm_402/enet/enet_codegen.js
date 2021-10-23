/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

var ringsNum = 0;

var maskMapping = {
    "enetCfg_rxInt" :     ["ENET_RX_FRAME_INTERRUPT",
                           "ENET_RX_FRAME_INTERRUPT | ENET_RX_FRAME_1_INTERRUPT",
                           "ENET_RX_FRAME_INTERRUPT | ENET_RX_FRAME_1_INTERRUPT | ENET_RX_FRAME_2_INTERRUPT"],
    "enetCfg_txInt" :     ["ENET_TX_FRAME_INTERRUPT",
                           "ENET_TX_FRAME_INTERRUPT | ENET_TX_FRAME_1_INTERRUPT",
                           "ENET_TX_FRAME_INTERRUPT | ENET_TX_FRAME_1_INTERRUPT | ENET_TX_FRAME_2_INTERRUPT"],
    "enetCfg_errInt" :    ["ENET_BABR_INTERRUPT | ENET_BABT_INTERRUPT | ENET_EBERR_INTERRUPT | ENET_LATE_COLLISION_INTERRUPT | " +
                           "ENET_RETRY_LIMIT_INTERRUPT | ENET_UNDERRUN_INTERRUPT | ENET_PAYLOAD_RX_INTERRUPT",
                           "ENET_BABR_INTERRUPT | ENET_BABT_INTERRUPT | ENET_EBERR_INTERRUPT | ENET_LATE_COLLISION_INTERRUPT | " +
                           "ENET_RETRY_LIMIT_INTERRUPT | ENET_UNDERRUN_INTERRUPT | ENET_PAYLOAD_RX_INTERRUPT",
                           "ENET_BABR_INTERRUPT | ENET_BABT_INTERRUPT | ENET_EBERR_INTERRUPT | ENET_LATE_COLLISION_INTERRUPT | " +
                           "ENET_RETRY_LIMIT_INTERRUPT | ENET_UNDERRUN_INTERRUPT | ENET_PAYLOAD_RX_INTERRUPT"],
    "enetCfg_parserInt" : ["ENET_PARSERERR_INTERRUPT | ENET_PARSRF_INTERRUPT",
                           "ENET_PARSERERR_INTERRUPT | ENET_PARSRF_INTERRUPT",
                           "ENET_PARSERERR_INTERRUPT | ENET_PARSRF_INTERRUPT"],
    "enetCfg_timerInt" :  ["ENET_TS_AVAIL_INTERRUPT | ENET_TS_TIMER_INTERRUPT",
                           "ENET_TS_AVAIL_INTERRUPT | ENET_TS_TIMER_INTERRUPT",
                           "ENET_TS_AVAIL_INTERRUPT | ENET_TS_TIMER_INTERRUPT"],
    "enetCfg_wakeInt" :   ["ENET_WAKEUP_INTERRUPT",
                           "ENET_WAKEUP_INTERRUPT",
                           "ENET_WAKEUP_INTERRUPT"]
};

function genMask(elem, type) {
    var mask = "";
    var elemChildren = elem.getChildren();

    for (var i in elemChildren)
    {
        var idArray = elemChildren.get(i).getId().split(".");
        var id = idArray[idArray.length - 1];
        if (elemChildren.get(i).getValue())
        {
            var maskName;
            if (maskMapping[id] != null)
            {
                maskName = maskMapping[id][ringsNum];
            }
            else
            {
                maskName = id;
            }
            if (mask == "")
            {
                mask = "(" + type + ")" + maskName;
            }
            else
            {
                mask = mask + " | " + "(" + type + ")" + maskName;
            }
        }
    }

    if (mask == "")
        mask = "0";

    return mask;
}

// ENET component code generator class.
function ENETCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "ENETCodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetCfg");
        var timerInitStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg");
        var timerInitStructEnable = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg_enable").getValue();
        var configStructName = initStruct.getChildById("name").getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();
        var configTimerStructName = timerInitStruct.getChildById("name").getStringValue();
        var constTimerStruct = timerInitStruct.getChildById("readonly").getValue();
        var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".enet_state_name");
        var ringsArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetCfg_ringConfig");
        var chanArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg_channels");
        var chanArrayList = chanArray.getChildren();
        var chanArraySize = chanArrayList.size();

        hFileSections[INCLUDES_SECTION] += addInclude("enet_driver.h");

        if((initStruct != null) && (ringsArray != null))
        {
            hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */\n";
            if (peripheralName.substring(4, 5) != "")
            {
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), "(" + peripheralName.substring(4, 5) + "U)");
            }
            else
            {
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), "(0U)");
            }
            hFileSections[DEFINES_SECTION] += "\n/*! @brief Driver state structure which holds driver runtime data */"
            hFileSections[DEFINES_SECTION] += declareVar("\nextern enet_state_t", stateStruct);
            hFileSections[DEFINES_SECTION] += "\n";

            hFileSections[DEFINES_SECTION] += "/*! @brief Number of rings */\n";
            hFileSections[DEFINES_SECTION] += addDefine(peripheralName.toUpperCase() + "_NUM_RINGS", "(" + ringsArray.getChildren().size() + "U)");
            hFileSections[DEFINES_SECTION] += "\n";

            hFileSections[DEFINES_SECTION] += "/*! @brief The MAC address of the module */\n";
            hFileSections[DEFINES_SECTION] += "extern uint8_t " + peripheralName.toLowerCase() + "_MacAddr[6];\n";
            hFileSections[DEFINES_SECTION] += "\n";

            hFileSections[DEFINES_SECTION] += "/*! @brief Module configuration structure */";
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const enet_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern enet_config_t", configStructName);
            }
            hFileSections[DEFINES_SECTION] += "\n";

            hFileSections[DEFINES_SECTION] += "/*! @brief Buffer configuration structures */\n";
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += "extern const ";
            }
            else
            {
                hFileSections[DEFINES_SECTION] += "extern ";
            }
            hFileSections[DEFINES_SECTION] += "enet_buffer_config_t " + peripheralName.toLowerCase() + "_buffConfigArr[" + peripheralName.toUpperCase() + "_NUM_RINGS" + "];";

            if ((initStruct.getChildById("callback").getValue() != "") && (initStruct.getChildById("callback").getValue() != "NULL"))
            {
                hFileSections[DEFINES_SECTION] += "\n\n";
                hFileSections[DEFINES_SECTION] += "/*! @brief Module callback external declaration */\n";
                hFileSections[DEFINES_SECTION] += "extern void " + initStruct.getChildById("callback").getValue() + "(uint8_t instance, enet_event_t event, uint8_t ring);";
            }

            if ((timerInitStruct != null) && timerInitStructEnable)
            {
                hFileSections[DEFINES_SECTION] += "\n\n/*! @brief Adjustable timer configuration structure */";
                if (constTimerStruct)
                {
                   hFileSections[DEFINES_SECTION] += declareVar("\nextern const enet_timer_config_t", configTimerStructName);
                }
                else
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern enet_timer_config_t", configTimerStructName);
                }

                if ((timerInitStruct.getChildById("enetTimerCfg_callback").getValue() != "") && (timerInitStruct.getChildById("enetTimerCfg_callback").getValue() != "NULL"))
                {
                    hFileSections[DEFINES_SECTION] += "\n";
                    hFileSections[DEFINES_SECTION] += "/*! @brief Timer callback external declaration */\n";
                    hFileSections[DEFINES_SECTION] += "extern void " + timerInitStruct.getChildById("enetTimerCfg_callback").getValue() + "(uint8_t instance, uint8_t channels);\n";
                }

                for (i = 0; i < chanArraySize; i++)
                {
                    hFileSections[DEFINES_SECTION] += "\n/*! @brief Timer channel " + i + " configuration structure */";
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern enet_timer_channel_config_t", chanArrayList[i].getChildById("name").getStringValue());
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetCfg");
        var timerInitStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg");
        var timerInitStructEnable = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg_enable").getValue();
        var configStructName = initStruct.getChildById("name").getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();
        var constTimerStruct = timerInitStruct.getChildById("readonly").getValue();
        var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".enet_state_name");
        var ringsArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetCfg_ringConfig");
        var ringsArrayList = ringsArray.getChildren();
        var ringsArraySize = ringsArrayList.size();
        var chanArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".enetTimerCfg_channels");
        var chanArrayList = chanArray.getChildren();
        var chanArraySize = chanArrayList.size();

        ringsNum = ringsArraySize - 1;

        if ((initStruct != null) && (ringsArray != null))
        {
            var i;
            var macString = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".enet_mac");
            var macArray = macString.split(":");

            for (i = 0; i < ringsArraySize; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Number of receive buffer descriptors for ring  " + i + " */\n";
                cFileSections[GLOBAL_VARS_SECTION] += addDefine(peripheralName.toUpperCase() + "_RXBD_NUM_" + i, "(" + ringsArrayList.get(i).getChildById("enet_rx_size").getValue() + "U)");
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Number of transmit buffer descriptors for ring  " + i + " */\n";
                cFileSections[GLOBAL_VARS_SECTION] += addDefine(peripheralName.toUpperCase() + "_TXBD_NUM_" + i, "(" + ringsArrayList.get(i).getChildById("enet_tx_size").getValue() + "U)");
                cFileSections[GLOBAL_VARS_SECTION] += "\n";
            }

            cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Driver state structure which holds driver runtime data */\n"
            cFileSections[GLOBAL_VARS_SECTION] += declareVar("enet_state_t", stateStruct);

            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief The MAC address of the module */\n";
            cFileSections[GLOBAL_VARS_SECTION] += "uint8_t " + peripheralName.toLowerCase() + "_MacAddr[6] = {";
            for (i = 0; i < 5; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "0x" + macArray[i] + ", ";
            }
            cFileSections[GLOBAL_VARS_SECTION] += "0x" + macArray[5] + "};\n";

            cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Module configuration structures */\n";
            if (constStruct)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "const ";
            }
            cFileSections[GLOBAL_VARS_SECTION] += "enet_config_t " + configStructName + " = {\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.interrupts      = " + genMask(initStruct.getChildById("enetCfg_interrupts"), "uint32_t") + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.maxFrameLen     = " + initStruct.getChildById("enetCfg_maxFrameLen").getValue() + "U,\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.miiMode         = " + initStruct.getChildById("enetCfg_miiConfig").getChildById("enetCfg_miiMode").getValue() + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.miiSpeed        = " + initStruct.getChildById("enetCfg_miiConfig").getChildById("enetCfg_miiSpeed").getValue() + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.miiDuplex       = " + initStruct.getChildById("enetCfg_miiConfig").getChildById("enetCfg_miiDuplex").getValue() + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.rxAccelerConfig = " + genMask(initStruct.getChildById("enetCfg_rxAccelerConfig"), "uint8_t") + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.txAccelerConfig = " + genMask(initStruct.getChildById("enetCfg_txAccelerConfig"), "uint8_t") + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.rxConfig        = " + genMask(initStruct.getChildById("enetCfg_rxSpecialConfig"), "uint8_t") + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.txConfig        = " + genMask(initStruct.getChildById("enetCfg_txSpecialConfig"), "uint8_t") + ",\n";
            cFileSections[GLOBAL_VARS_SECTION] += "\t.ringCount       = " + peripheralName.toUpperCase() + "_NUM_RINGS,\n";
            if (initStruct.getChildById("callback").getValue() == "")
            {
                initStruct.getChildById("callback").setValue("NULL");
            }
            cFileSections[GLOBAL_VARS_SECTION] += "\t.callback        = " + initStruct.getChildById("callback").getValue() + "\n";
            cFileSections[GLOBAL_VARS_SECTION] += "};\n";

            for (i = 0; i < ringsArraySize; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Receive buffer descriptors for ring  " + i + " */\n";
                cFileSections[GLOBAL_VARS_SECTION] += "ALIGNED(FEATURE_ENET_BUFFDESCR_ALIGNMENT) static enet_buffer_descriptor_t " + peripheralName.toLowerCase() +
                                                      "_rxBuffDescrip" + i + "[" + peripheralName.toUpperCase() + "_RXBD_NUM_" + i + "];\n";
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Transmit buffer descriptors for ring  " + i + " */\n";
                cFileSections[GLOBAL_VARS_SECTION] += "ALIGNED(FEATURE_ENET_BUFFDESCR_ALIGNMENT) static enet_buffer_descriptor_t " + peripheralName.toLowerCase() +
                                                      "_txBuffDescrip" + i + "[" + peripheralName.toUpperCase() + "_TXBD_NUM_" + i + "];\n";
                cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Receive data buffers for ring  " + i + " */\n";
                cFileSections[GLOBAL_VARS_SECTION] += "ALIGNED(FEATURE_ENET_BUFF_ALIGNMENT) static uint8_t " + peripheralName.toLowerCase() + "_rxDataBuff" + i
                                                      + "[" + peripheralName.toUpperCase() + "_RXBD_NUM_" + i + " * ENET_BUFF_ALIGN("
                                                      + initStruct.getChildById("enetCfg_maxFrameLen").getValue() + "U)];\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\n";
            }

            cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Buffer configuration structures */\n";
            if (constStruct)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "const ";
            }
            cFileSections[GLOBAL_VARS_SECTION] += "enet_buffer_config_t " +  peripheralName.toLowerCase() + "_buffConfigArr[] = {" + "\n";
            for (i = 0; i < ringsArraySize; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\t{ \n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t\t" + peripheralName.toUpperCase() + "_RXBD_NUM_" + i + ",\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t\t" + peripheralName.toUpperCase() + "_TXBD_NUM_" + i + ",\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t\t&" + peripheralName.toLowerCase() + "_rxBuffDescrip" + i + "[0],\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t\t&" + peripheralName.toLowerCase() + "_txBuffDescrip" + i + "[0],\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t\t&" + peripheralName.toLowerCase() + "_rxDataBuff"+ i + "[0]\n";
                cFileSections[GLOBAL_VARS_SECTION] += "\t}, \n";
            }
            cFileSections[GLOBAL_VARS_SECTION] += "};\n";

            if ((timerInitStruct != null) && timerInitStructEnable)
            {
                if (timerInitStruct.getChildById("enetTimerCfg_callback").getValue() == "")
                {
                    timerInitStruct.getChildById("enetTimerCfg_callback").setValue("NULL");
                }

                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Adjustable timer configuration structure */";
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(timerInitStruct, "enet_timer_config_t", "enetTimerCfg_");

                for (i = 0; i < chanArraySize; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Timer channel " + i + " configuration structure */";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(chanArrayList[i], "enet_timer_channel_config_t", "enetChnCfg_");
                }
            }
        }
    }
}
