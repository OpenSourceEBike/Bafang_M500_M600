/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// eDMA component code generator class.
function eDMA_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "eDMA_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {

        /* User Configuration Structures */
        var edmaUserCfg_InitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_edmaUserCfg");
        var edmaUserCfg_State = edmaUserCfg_InitStruct.getChildById("userStateStruct").getValue();
        var edmaUserCfg_InitArray = edmaUserCfg_InitStruct.getChildById("array_userCfgStructs");
        var edmaUserCfg_ArrayChildren = edmaUserCfg_InitArray.getChildren();
        var edmaUserCfg_ArrayDimension =edmaUserCfg_ArrayChildren.size();
        var edmaUserCfg_Name = new Array(edmaUserCfg_ArrayDimension);
        var edmaUserCfg_ReadOnly = new Array(edmaUserCfg_ArrayDimension);
        var edmaUserCfg_GroupArb = new Array(edmaUserCfg_ArrayDimension);
        var edmaUserCfg_GroupPrio = new Array(edmaUserCfg_ArrayDimension);
        var edmaUserCfg_ChArb = new Array(edmaUserCfg_ArrayDimension);
        var edmaUserCfg_HaltBit = new Array(edmaUserCfg_ArrayDimension);
        for(var idx = 0; idx < edmaUserCfg_ArrayDimension; idx++)
        {
            edmaUserCfg_Name[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("userCfgName").getValue();
            edmaUserCfg_ReadOnly[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("readOnly").getValue();
            edmaUserCfg_GroupArb[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("groupArbitration").getValue();
            edmaUserCfg_GroupPrio[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("groupPriority").getValue();
            edmaUserCfg_ChArb[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("chnArbitration").getValue();
            edmaUserCfg_HaltBit[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("haltOnError").getValue();
        }

        /* Channel Configuration Structures */
        var edmaChCfg_InitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_edmaChCfg");
        var edmaChCfg_InitArray = edmaChCfg_InitStruct.getChildById("array_chCfgStructs");
        var edmaChCfg_ArrayChildren = edmaChCfg_InitArray.getChildren();
        var edmaChCfg_ArrayDimension =edmaChCfg_ArrayChildren.size();
        var edmaChCfg_StateName = new Array(edmaChCfg_ArrayDimension);
        var edmaChCfg_ChConfigName = new Array(edmaChCfg_ArrayDimension);
        var edmaChCfg_VirtCh = new Array(edmaChCfg_ArrayDimension);
        var edmaChCfg_Call = new Array(edmaChCfg_ArrayDimension);
        var edmaChCfg_CallParam = new Array(edmaChCfg_ArrayDimension);
        for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
        {
            edmaChCfg_StateName[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chStateStructName").getStringValue();
            edmaChCfg_ChConfigName[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chConfigName").getStringValue();
            edmaChCfg_VirtCh[idx] = edmaChCfg_ArrayChildren[idx].getChildById("virtCh").getStringValue();
            edmaChCfg_Call[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chCallback").getStringValue();
            edmaChCfg_CallParam[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chCallbackParam").getStringValue();
        }

        /* DMA_CRC_0 User Configuration Structures */
        var crcUserCfg_InitStructInst0 = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_crcChCfgInst0");
        var crcUserCfg_StateInst0 = crcUserCfg_InitStructInst0.getChildById("crcStateStructNameInst0").getValue();
        var crcUserCfg_NameInst0 = crcUserCfg_InitStructInst0.getChildById("crcUserCfgInst0").getValue();

        /* DMA_CRC_0 Channel Configuration Structures */
        var crcChCfg_InitArrayInst0 = crcUserCfg_InitStructInst0.getChildById("array_crcChnCfgStructs");
        var crcChCfg_ArrayChildrenInst0 = crcChCfg_InitArrayInst0.getChildren();
        var crcChCfg_ArrayDimensionInst0 =crcChCfg_ArrayChildrenInst0.size();
        var crcChCfg_StateNameInst0 = new Array(crcChCfg_ArrayDimensionInst0);
        var crcChCfg_ChConfigNameInst0 = new Array(crcChCfg_ArrayDimensionInst0);
        var crcChCfg_CrcChInst0 = new Array(crcChCfg_ArrayDimensionInst0);
        for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
        {
            crcChCfg_StateNameInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChnStateStructName").getStringValue();
            crcChCfg_ChConfigNameInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChnConfigName").getStringValue();
            crcChCfg_CrcChInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChn").getStringValue();
        }

        /* DMA_CRC_1 User Configuration Structures */
        var crcUserCfg_InitStructInst1 = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_crcChCfgInst1");
        var crcUserCfg_StateInst1 = crcUserCfg_InitStructInst1.getChildById("crcStateStructNameInst1").getValue();
        var crcUserCfg_NameInst1 = crcUserCfg_InitStructInst1.getChildById("crcUserCfgInst1").getValue();

        /* DMA_CRC_1 Channel Configuration Structures */
        var crcChCfg_InitArrayInst1 = crcUserCfg_InitStructInst1.getChildById("array_crcChnCfgStructs");
        var crcChCfg_ArrayChildrenInst1 = crcChCfg_InitArrayInst1.getChildren();
        var crcChCfg_ArrayDimensionInst1 =crcChCfg_ArrayChildrenInst1.size();
        var crcChCfg_StateNameInst1 = new Array(crcChCfg_ArrayDimensionInst1);
        var crcChCfg_ChConfigNameInst1 = new Array(crcChCfg_ArrayDimensionInst1);
        var crcChCfg_CrcChInst1 = new Array(crcChCfg_ArrayDimensionInst1);
        for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
        {
            crcChCfg_StateNameInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChnStateStructName").getStringValue();
            crcChCfg_ChConfigNameInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChnConfigName").getStringValue();
            crcChCfg_CrcChInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChn").getStringValue();
        }

        if((edmaUserCfg_InitStruct != null) && (edmaChCfg_InitStruct != null))
        {
            // Add Includes info.
            hFileSections[INCLUDES_SECTION] += addInclude("edma_driver.h");
            /*************************************************************************************************/
            /* Add Defines */
            /* Channel Number */
            for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
            {
                hFileSections[DEFINES_SECTION] += "/*! @brief Channel number for channel configuration #" + edmaChCfg_VirtCh[idx] + " */\n";
                hFileSections[DEFINES_SECTION] += addDefine("EDMA_CHN" + edmaChCfg_VirtCh[idx] + "_NUMBER ", edmaChCfg_VirtCh[idx] + "U");
            }
            /*************************************************************************************************/
            /* Total channels */
            hFileSections[DEFINES_SECTION] += "\n/*! @brief The total number of configured channels */\n";
            hFileSections[DEFINES_SECTION] += addDefine("EDMA_CONFIGURED_CHANNELS_COUNT", edmaChCfg_ArrayDimension + "U");
            /*************************************************************************************************/
            /* Add external global variables */
            /* Controller state */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Driver state structure which holds driver runtime data */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_state_t " + edmaUserCfg_State + ";" + NEW_LINE;
            /*************************************************************************************************/
            /* Controller channels state */
            for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief eDma channel state structure " + edmaChCfg_VirtCh[idx] +". Holds channel runtime data */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_chn_state_t " + edmaChCfg_StateName[idx] + ";" + NEW_LINE;
            }
            /*************************************************************************************************/
            /* Controller channels callback */
            for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
            {
                if(edmaChCfg_Call[idx] != 'NULL')
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief eDma channel callback " + edmaChCfg_VirtCh[idx] + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + edmaChCfg_Call[idx] + "(void *parameter, edma_chn_status_t status);" + NEW_LINE;
                }
            }
            /*************************************************************************************************/
            if(edmaChCfg_ArrayDimension > 0)
            {
                /* Controller array of channels state */
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of channel state structures */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_chn_state_t * const edmaChnStateArray[EDMA_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
            }
            /*************************************************************************************************/
            /* User Configuration */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief User configuration */" + NEW_LINE;
            for(var idx = 0; idx < edmaUserCfg_ArrayDimension; idx++)
            {
                if(edmaUserCfg_ReadOnly[idx])
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const edma_user_config_t", edmaUserCfg_Name[idx]);
                }
                else
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern edma_user_config_t", edmaUserCfg_Name[idx]);
                }
            }
            /*************************************************************************************************/
            /* Channels configuration */
            for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief eDma channel " + edmaChCfg_VirtCh[idx] + " configuration */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_channel_config_t " + edmaChCfg_ChConfigName[idx] + ";" + NEW_LINE;
            }
            /*************************************************************************************************/
            if(edmaChCfg_ArrayDimension > 0)
            {
                /* Array of channels configuration */
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of channel configuration structures */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const edma_channel_config_t * const edmaChnConfigArray[EDMA_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
            }
            /*************************************************************************************************/
        }

        /* DMA_CRC instance 0 */
        if(crcUserCfg_InitStructInst0 != null && crcChCfg_ArrayDimensionInst0 > 0)
        {
            /*************************************************************************************************/
            /* Add Defines */
            /* DMA CRC channel Number */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
            {
                hFileSections[DEFINES_SECTION] += "\n/*! @brief Channel number for DMA_CRC_0 channel configuration #" + crcChCfg_CrcChInst0[idx] + " */\n";
                hFileSections[DEFINES_SECTION] += addDefine("DMA_CRC_0_CHN" + crcChCfg_CrcChInst0[idx] + "_NUMBER ", crcChCfg_CrcChInst0[idx] + "U");
            }
            /*************************************************************************************************/
            /* Total DMA CRC channels */
            hFileSections[DEFINES_SECTION] += "\n/*! @brief The total number of configured DMA_CRC_0 channels */\n";
            hFileSections[DEFINES_SECTION] += addDefine("DMA_CRC_0_CONFIGURED_CHANNELS_COUNT", crcChCfg_ArrayDimensionInst0 + "U");
            /*************************************************************************************************/
            /* Add external global variables */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Runtime state structure for the DMA_CRC_0 */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_state_t " + crcUserCfg_StateInst0 + ";" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief The user configuration structure for DMA_CRC_0 */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_user_config_t " + crcUserCfg_NameInst0 + ";" + NEW_LINE;
            /* DMA CRC channels state */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief DMA_CRC_0 channel " + crcChCfg_CrcChInst0[idx] + " state structure . Holds channel runtime data */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_chn_state_t " + crcChCfg_StateNameInst0[idx] + ";" + NEW_LINE;
            }
            /* Controller array of DMA CRC channels state */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of DMA_CRC_0 channel state structures */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_chn_state_t * const dmaCrc0ChnStateArray[DMA_CRC_0_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
            /* DMA CRC channel configurations */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief DMA_CRC_0 channel " + crcChCfg_CrcChInst0[idx] + " configuration */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_channel_config_t " + crcChCfg_ChConfigNameInst0[idx] + ";" + NEW_LINE;
            }
            /* Array of DMA CRC channels configuration */
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of DMA_CRC_0 channel configuration structures */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const edma_crc_channel_config_t * const dmaCrc0ChnConfigArray[DMA_CRC_0_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
        }

        /* DMA CRC instance 1 */
        if(crcUserCfg_InitStructInst1 != null && crcChCfg_ArrayDimensionInst1 > 0)
        {
            /*************************************************************************************************/
            /* Add Defines */
            /* DMA CRC channel Number */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
            {
                hFileSections[DEFINES_SECTION] += "\n/*! @brief Channel number for DMA_CRC_1 channel configuration #" + crcChCfg_CrcChInst1[idx] + " */\n";
                hFileSections[DEFINES_SECTION] += addDefine("DMA_CRC_1_CHN" + crcChCfg_CrcChInst1[idx] + "_NUMBER ", crcChCfg_CrcChInst1[idx] + "U");
            }
            /*************************************************************************************************/
            /* Total DMA CRC channels */
            hFileSections[DEFINES_SECTION] += "\n/*! @brief The total number of configured DMA_CRC_1 channels */\n";
            hFileSections[DEFINES_SECTION] += addDefine("DMA_CRC_1_CONFIGURED_CHANNELS_COUNT", crcChCfg_ArrayDimensionInst1 + "U");
            /*************************************************************************************************/
            /* Add external global variables */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Runtime state structure for the DMA_CRC_1 */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_state_t " + crcUserCfg_StateInst1 + ";" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief The user configuration structure for DMA_CRC_1 */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_user_config_t " + crcUserCfg_NameInst1 + ";" + NEW_LINE;
            /* DMA CRC channels state */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief DMA_CRC_1 channel " + crcChCfg_CrcChInst1[idx] + " state structure . Holds channel runtime data */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_chn_state_t " + crcChCfg_StateNameInst1[idx] + ";" + NEW_LINE;
            }
            /* Controller array of DMA CRC channels state */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of DMA_CRC_1 channel state structures */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_chn_state_t * const dmaCrc1ChnStateArray[DMA_CRC_1_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
            /* DMA CRC channel configurations */
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief DMA_CRC_1 channel " + crcChCfg_CrcChInst1[idx] + " configuration */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern edma_crc_channel_config_t " + crcChCfg_ChConfigNameInst1[idx] + ";" + NEW_LINE;
            }
            /* Array of DMA CRC channels configuration */
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Array of DMA_CRC_1 channel configuration structures */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const edma_crc_channel_config_t * const dmaCrc1ChnConfigArray[DMA_CRC_1_CONFIGURED_CHANNELS_COUNT];" + NEW_LINE;
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        try{
            cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
            /* User Configuration Structures */
            var edmaUserCfg_InitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_edmaUserCfg");
            var edmaUserCfg_State = edmaUserCfg_InitStruct.getChildById("userStateStruct").getValue();
            var edmaUserCfg_InitArray = edmaUserCfg_InitStruct.getChildById("array_userCfgStructs");
            var edmaUserCfg_ArrayChildren = edmaUserCfg_InitArray.getChildren();
            var edmaUserCfg_ArrayDimension =edmaUserCfg_ArrayChildren.size();
            var edmaUserCfg_Name = new Array(edmaUserCfg_ArrayDimension);
            var edmaUserCfg_ReadOnly = new Array(edmaUserCfg_ArrayDimension);
            var edmaUserCfg_GroupArb = new Array(edmaUserCfg_ArrayDimension);
            var edmaUserCfg_GroupPrio = new Array(edmaUserCfg_ArrayDimension);
            var edmaUserCfg_ChArb = new Array(edmaUserCfg_ArrayDimension);
            var edmaUserCfg_HaltBit = new Array(edmaUserCfg_ArrayDimension);
            for(var idx = 0; idx < edmaUserCfg_ArrayDimension; idx++)
            {
                edmaUserCfg_Name[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("userCfgName").getValue();
                edmaUserCfg_ReadOnly[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("readOnly").getValue();
                if (edmaUserCfg_ArrayChildren[idx].getChildById("groupArbitration").available)
                {
                    edmaUserCfg_GroupArb[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("groupArbitration").getValue();
                }
                if (edmaUserCfg_ArrayChildren[idx].getChildById("groupPriority").available)
                {
                    edmaUserCfg_GroupPrio[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("groupPriority").getValue();
                }
                edmaUserCfg_ChArb[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("chnArbitration").getValue();
                edmaUserCfg_HaltBit[idx] = edmaUserCfg_ArrayChildren[idx].getChildById("haltOnError").getValue();
            }

            /* Channel Configuration Structures */
            var edmaChCfg_InitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_edmaChCfg");
            var edmaChCfg_InitArray = edmaChCfg_InitStruct.getChildById("array_chCfgStructs");
            var edmaChCfg_ArrayChildren = edmaChCfg_InitArray.getChildren();
            var edmaChCfg_ArrayDimension = edmaChCfg_ArrayChildren.size();
            var edmaChCfg_StateName = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_ChConfigName = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_ChConfigType = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_VirtCh = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_GrPrio = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_ChPrio = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_Req = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_Call = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_CallParam = new Array(edmaChCfg_ArrayDimension);
            var edmaChCfg_PerTrig = new Array(edmaChCfg_ArrayDimension);
            for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
            {
                edmaChCfg_StateName[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chStateStructName").getStringValue();
                edmaChCfg_ChConfigName[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chConfigName").getStringValue();
                edmaChCfg_ChConfigType[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chType").getStringValue();
                edmaChCfg_VirtCh[idx] = edmaChCfg_ArrayChildren[idx].getChildById("virtCh").getStringValue();
                if (edmaChCfg_ArrayChildren[idx].getChildById("grPrio").available)
                {
                    edmaChCfg_GrPrio[idx] = edmaChCfg_ArrayChildren[idx].getChildById("grPrio").getStringValue();
                }
                edmaChCfg_ChPrio[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chPrio").getStringValue();
                edmaChCfg_Call[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chCallback").getStringValue();
                edmaChCfg_CallParam[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chCallbackParam").getStringValue();
                edmaChCfg_PerTrig[idx] = edmaChCfg_ArrayChildren[idx].getChildById("enableTrigger").getStringValue();
                if (edmaChCfg_ArrayChildren[idx].getChildById("chReqNoMux").available)
                {
                    edmaChCfg_ArrayChildren[idx].getChildById("chReqNoMux").setValue(EDMA_GetChannelRequest(edmaChCfg_ArrayChildren[idx].getChildById("virtCh").getStringValue()));
                    edmaChCfg_Req[idx] = (edmaChCfg_ArrayChildren[idx].getChildById("chReqNoMux").getStringValue());
                }
                else
                {
                    edmaChCfg_Req[idx] = edmaChCfg_ArrayChildren[idx].getChildById("chReq").getStringValue();
                }
            }

            /* DMA_CRC_0 User Configuration Structures */
            var crcUserCfg_InitStructInst0 = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_crcChCfgInst0");
            var crcUserCfg_StateInst0 = crcUserCfg_InitStructInst0.getChildById("crcStateStructNameInst0").getValue();
            var crcUserCfg_NameInst0 = crcUserCfg_InitStructInst0.getChildById("crcUserCfgInst0").getValue();
            var crcUserCfg_SwapByteInst0 = crcUserCfg_InitStructInst0.getChildById("swapByte").getValue();
            var crcUserCfg_SwapBitInst0 = crcUserCfg_InitStructInst0.getChildById("swapBit").getValue();

            /* DMA_CRC_0 Channel Configuration Structures */
            var crcChCfg_InitArrayInst0 = crcUserCfg_InitStructInst0.getChildById("array_crcChnCfgStructs");
            var crcChCfg_ArrayChildrenInst0 = crcChCfg_InitArrayInst0.getChildren();
            var crcChCfg_ArrayDimensionInst0 =crcChCfg_ArrayChildrenInst0.size();
            var crcChCfg_StateNameInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_ChConfigNameInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_CrcChInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_ChConfigTypeInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_DmaVirtChInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_PolynomialInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_InitValueInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_SeedValueInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            var crcChCfg_EnableCrcInst0 = new Array(crcChCfg_ArrayDimensionInst0);
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
            {
                crcChCfg_StateNameInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChnStateStructName").getStringValue();
                crcChCfg_ChConfigNameInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChnConfigName").getStringValue();
                crcChCfg_CrcChInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChn").getStringValue();
                crcChCfg_ChConfigTypeInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("crcChnType").getStringValue();
                crcChCfg_DmaVirtChInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("dmaVirtChn").getStringValue();
                crcChCfg_PolynomialInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("polynomial").getStringValue();
                crcChCfg_InitValueInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("initValue").getStringValue();
                crcChCfg_SeedValueInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("seedValue").getStringValue();
                crcChCfg_EnableCrcInst0[idx] = crcChCfg_ArrayChildrenInst0[idx].getChildById("enableCRC").getStringValue();
            }

            /* DMA_CRC_1 User Configuration Structures */
            var crcUserCfg_InitStructInst1 = getSetting(this.uiConfig, this.uiConfig.getName()+".settings_array_crcChCfgInst1");
            var crcUserCfg_StateInst1 = crcUserCfg_InitStructInst1.getChildById("crcStateStructNameInst1").getValue();
            var crcUserCfg_NameInst1 = crcUserCfg_InitStructInst1.getChildById("crcUserCfgInst1").getValue();
            var crcUserCfg_SwapByteInst1 = crcUserCfg_InitStructInst1.getChildById("swapByte").getValue();
            var crcUserCfg_SwapBitInst1 = crcUserCfg_InitStructInst1.getChildById("swapBit").getValue();

            /* DMA_CRC_1 Channel Configuration Structures */
            var crcChCfg_InitArrayInst1 = crcUserCfg_InitStructInst1.getChildById("array_crcChnCfgStructs");
            var crcChCfg_ArrayChildrenInst1 = crcChCfg_InitArrayInst1.getChildren();
            var crcChCfg_ArrayDimensionInst1 =crcChCfg_ArrayChildrenInst1.size();
            var crcChCfg_StateNameInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_ChConfigNameInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_CrcChInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_ChConfigTypeInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_DmaVirtChInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_PolynomialInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_InitValueInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_SeedValueInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            var crcChCfg_EnableCrcInst1 = new Array(crcChCfg_ArrayDimensionInst1);
            for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
            {
                crcChCfg_StateNameInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChnStateStructName").getStringValue();
                crcChCfg_ChConfigNameInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChnConfigName").getStringValue();
                crcChCfg_CrcChInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChn").getStringValue();
                crcChCfg_ChConfigTypeInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("crcChnType").getStringValue();
                crcChCfg_DmaVirtChInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("dmaVirtChn").getStringValue();
                crcChCfg_PolynomialInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("polynomial").getStringValue();
                crcChCfg_InitValueInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("initValue").getStringValue();
                crcChCfg_SeedValueInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("seedValue").getStringValue();
                crcChCfg_EnableCrcInst1[idx] = crcChCfg_ArrayChildrenInst1[idx].getChildById("enableCRC").getStringValue();
            }

            if((edmaUserCfg_InitStruct != null) && (edmaChCfg_InitStruct != null))
            {
                /* User Configuration */
                cFileSections[GLOBAL_VARS_SECTION] += "edma_state_t " + edmaUserCfg_State + ";" + NEW_LINE;
                /*************************************************************************************************/
                /* Channels state */
                for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_chn_state_t " + edmaChCfg_StateName[idx] + ";" + NEW_LINE;
                }
                /*************************************************************************************************/
                if(edmaChCfg_ArrayDimension > 0)
                {
                    /* Channels state array */
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_chn_state_t * const edmaChnStateArray[] = {";
                    for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + edmaChCfg_StateName[idx] + ",";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /*************************************************************************************************/
                /* Channels configuration */
                for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + edmaChCfg_ChConfigType[idx] + " " + edmaChCfg_ChConfigName[idx] + " = {";
                    if (edmaChCfg_ArrayChildren[idx].getChildById("grPrio").available)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .groupPriority = " + edmaChCfg_GrPrio[idx] + ",";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .channelPriority = " + edmaChCfg_ChPrio[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .virtChnConfig = " + "EDMA_CHN" + edmaChCfg_VirtCh[idx] + "_NUMBER,";
                    if (edmaChCfg_ArrayChildren[idx].getChildById("chReqNoMux").available)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .source = " + edmaChCfg_Req[idx].replace(/`/g, '') + ",";
                    }
                    else
                    {
                        var request = edmaChCfg_ArrayChildren[idx].getChildById("chReq").getValueDescription()
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .source = " + request + ",";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .callback = " + edmaChCfg_Call[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .callbackParam = " + edmaChCfg_CallParam[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .enableTrigger = " + edmaChCfg_PerTrig[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /*************************************************************************************************/
                if(edmaChCfg_ArrayDimension > 0)
                {
                    /* Channels configuration array */
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const edma_channel_config_t * const edmaChnConfigArray[] = {";
                    for(var idx = 0; idx < edmaChCfg_ArrayDimension; idx++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + edmaChCfg_ChConfigName[idx] + ",";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /*************************************************************************************************/
                /* User Configuration */
                for(var idx = 0; idx < edmaUserCfg_ArrayDimension; idx++)
                {
                    if(edmaUserCfg_ReadOnly[idx])
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const edma_user_config_t " + edmaUserCfg_Name[idx] + " = {";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_user_config_t " + edmaUserCfg_Name[idx] + " = {";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .chnArbitration = " + edmaUserCfg_ChArb[idx] + ",";
                    if (edmaUserCfg_ArrayChildren[idx].getChildById("groupArbitration").available)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .groupArbitration = " + edmaUserCfg_GroupArb[idx] + ",";
                    }
                    if (edmaUserCfg_ArrayChildren[idx].getChildById("groupPriority").available)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .groupPriority = " + edmaUserCfg_GroupPrio[idx] + ",";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .haltOnError = " + edmaUserCfg_HaltBit[idx];
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /*************************************************************************************************/
            }

            /* DMA_CRC instance 0 */
            if(crcUserCfg_InitStructInst0 != null && crcChCfg_ArrayDimensionInst0 > 0)
            {
                /* User Configuration State */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_state_t " + crcUserCfg_StateInst0 + ";" + NEW_LINE;
                /*************************************************************************************************/
                /* User Configuration */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_user_config_t " + crcUserCfg_NameInst0 + " = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isSwapByte = " + crcUserCfg_SwapByteInst0 + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isSwapBit = " + crcUserCfg_SwapBitInst0;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                /*************************************************************************************************/
                /* Channels state */
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_chn_state_t " + crcChCfg_StateNameInst0[idx] + ";" + NEW_LINE;
                }
                /*************************************************************************************************/
                /* Channels state array */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_chn_state_t * const dmaCrc0ChnStateArray[] = {";
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + crcChCfg_StateNameInst0[idx] + ",";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                /*************************************************************************************************/
                /* Channels configuration */
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + crcChCfg_ChConfigTypeInst0[idx] + " " + crcChCfg_ChConfigNameInst0[idx] + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .crcChannel = " + "DMA_CRC_0_CHN" + crcChCfg_CrcChInst0[idx] + "_NUMBER,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .virtualChannel = " + crcChCfg_DmaVirtChInst0[idx] + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .polynomial = " + crcChCfg_PolynomialInst0[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .initSel = " + crcChCfg_InitValueInst0[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .seedValue = " + crcChCfg_SeedValueInst0[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isEnabled = " + crcChCfg_EnableCrcInst0[idx];
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /* Channels configuration array */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const edma_crc_channel_config_t * const dmaCrc0ChnConfigArray[] = {";
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst0; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + crcChCfg_ChConfigNameInst0[idx] + ",";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
            }

            /* DMA_CRC instance 1 */
            if(crcUserCfg_InitStructInst1 != null && crcChCfg_ArrayDimensionInst1 > 0)
            {
                /* User Configuration State */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_state_t " + crcUserCfg_StateInst1 + ";" + NEW_LINE;
                /*************************************************************************************************/
                /* User Configuration */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_user_config_t " + crcUserCfg_NameInst1 + " = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isSwapByte = " + crcUserCfg_SwapByteInst1 + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isSwapBit = " + crcUserCfg_SwapBitInst1;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                /*************************************************************************************************/
                /* Channels state */
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_chn_state_t " + crcChCfg_StateNameInst1[idx] + ";" + NEW_LINE;
                }
                /*************************************************************************************************/
                /* Channels state array */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "edma_crc_chn_state_t * const dmaCrc1ChnStateArray[] = {";
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + crcChCfg_StateNameInst1[idx] + ",";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                /*************************************************************************************************/
                /* Channels configuration */
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + crcChCfg_ChConfigTypeInst1[idx] + " " + crcChCfg_ChConfigNameInst1[idx] + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .crcChannel = " + "DMA_CRC_1_CHN" + crcChCfg_CrcChInst1[idx] + "_NUMBER,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .virtualChannel = " + crcChCfg_DmaVirtChInst1[idx] + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .polynomial = " + crcChCfg_PolynomialInst1[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .initSel = " + crcChCfg_InitValueInst1[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .seedValue = " + crcChCfg_SeedValueInst1[idx] + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .isEnabled = " + crcChCfg_EnableCrcInst1[idx];
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
                /* Channels configuration array */
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const edma_crc_channel_config_t * const dmaCrc1ChnConfigArray[] = {";
                for(var idx = 0; idx < crcChCfg_ArrayDimensionInst1; idx++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    &" + crcChCfg_ChConfigNameInst1[idx] + ",";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}

/// @function EDMA_GetChannelRequest
function EDMA_GetChannelRequest(channel)
{
    var channelRequest = "`N/A`";
    switch (channel)
    {
        case '0':
            channelRequest = "`EDMA_REQ_QSPI_RX`";
            break;
        case '1':
            channelRequest = "`EDMA_REQ_QSPI_TX`";
            break;
        case '2':
            channelRequest = "`EDMA_REQ_SPI_HOST_TX`";
            break;
        case '3':
            channelRequest = "`EDMA_REQ_SPI_HOST_RX`";
            break;
        case '4':
            channelRequest = "`EDMA_REQ_SPI_PER_TX`";
            break;
        case '5':
            channelRequest = "`EDMA_REQ_SPI_PER_RX`";
            break;
        case '6':
            channelRequest = "`EDMA_REQ_LTC_SHA256`";
            break;
        case '7':
            channelRequest = "`EDMA_REQ_CHANNEL_7`";
            break;
        case '8':
            channelRequest = "`EDMA_REQ_CHANNEL_8`";
            break;
        case '9':
            channelRequest = "`EDMA_REQ_CHANNEL_9`";
            break;
        case '10':
            channelRequest = "`EDMA_REQ_CHANNEL_10`";
            break;
        case '11':
            channelRequest = "`EDMA_REQ_CHANNEL_11`";
            break;
        case '12':
            channelRequest = "`EDMA_REQ_CHANNEL_12`";
            break;
        case '13':
            channelRequest = "`EDMA_REQ_CHANNEL_13`";
            break;
        case '14':
            channelRequest = "`EDMA_REQ_CHANNEL_14`";
            break;
        case '15':
            channelRequest = "`EDMA_REQ_CHANNEL_15`";
            break;
    }
    return channelRequest;
}
