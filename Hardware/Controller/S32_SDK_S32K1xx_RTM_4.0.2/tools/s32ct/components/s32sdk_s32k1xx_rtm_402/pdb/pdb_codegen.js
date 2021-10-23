/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// PDB component code generator class.
function PDB_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "PDB_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections)
    {    
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();         
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var adcTrigCfgList = getSetting(this.uiConfig, this.uiConfig.getName()+".adcTrigCfgs").getChildren();
        var pdbCfgList = getSetting(this.uiConfig, this.uiConfig.getName()+".pdbCfgs").getChildren();

        var configName = "";

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("pdb_driver.h");

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Device instance number */\n";
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "#define INST_" + peripheralName + " " + peripheralName.slice(peripheralName.length - 1) + "U\n";

        if((adcTrigCfgList != null) && (adcTrigCfgList.length > 0))
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief ADC pre-trigger configuration declarations */\n";
            for (var i = 0; i < adcTrigCfgList.length; i++)
            {
                configName = adcTrigCfgList[i].getChildById("name").getStringValue();
                readOnly = (adcTrigCfgList[i].getChildById("readOnly").getValue() ? "const " : "");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + readOnly + "pdb_adc_pretrigger_config_t", configName);
            }
        }

        if((pdbCfgList != null) && (pdbCfgList.length > 0))
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief PDB timer configuration declarations */\n";
            for (var i = 0; i < pdbCfgList.length; i++)
            {
                configName = pdbCfgList[i].getChildById("name").getStringValue();
                readOnly = (pdbCfgList[i].getChildById("readOnly").getValue() ? "const " : "");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + readOnly + "pdb_timer_config_t", configName);
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";        
        try {
            var adcTrigCfgList = getSetting(this.uiConfig, this.uiConfig.getName()+".adcTrigCfgs").getChildren();
            var pdbCfgList = getSetting(this.uiConfig, this.uiConfig.getName()+".pdbCfgs").getChildren();

            if((adcTrigCfgList != null) && (adcTrigCfgList.length > 0))
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief ADC pre-trigger configurations */\n";
                for (var i = 0; i < adcTrigCfgList.length; i++)
                {
                    configName = adcTrigCfgList[i].getChildById("name").getStringValue();
                    readOnly = (adcTrigCfgList[i].getChildById("readOnly").getValue() ? "const " : "");
                    cFileSections[GLOBAL_VARS_SECTION] += readOnly + "pdb_adc_pretrigger_config_t "+ configName +" = {\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .adcPreTriggerIdx = " + adcTrigCfgList[i].getChildById("adcPreTriggerIdx").getValue() + "U,\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .preTriggerEnable = " + adcTrigCfgList[i].getChildById("preTriggerEnable").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .preTriggerOutputEnable = " + adcTrigCfgList[i].getChildById("preTriggerOutputEnable").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .preTriggerBackToBackEnable = " + adcTrigCfgList[i].getChildById("preTriggerBackToBackEnable").getValue() + "\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "};\n\n";
                }
            }

            if((pdbCfgList != null) && (pdbCfgList.length > 0))
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief PDB timer configurations */\n";
                for (var i = 0; i < pdbCfgList.length; i++)
                {
                    configName = pdbCfgList[i].getChildById("name").getStringValue();
                    readOnly = (pdbCfgList[i].getChildById("readOnly").getValue() ? "const " : "");
                    cFileSections[GLOBAL_VARS_SECTION] += readOnly + "pdb_timer_config_t "+ configName +" = {\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .loadValueMode = " + pdbCfgList[i].getChildById("loadValueMode").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .seqErrIntEnable = " + pdbCfgList[i].getChildById("seqErrIntEnable").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .clkPreDiv = " + pdbCfgList[i].getChildById("clkPreDiv").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .clkPreMultFactor = " + pdbCfgList[i].getChildById("clkPreMultFactor").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .triggerInput = " + pdbCfgList[i].getChildById("triggerInput").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .continuousModeEnable = " + pdbCfgList[i].getChildById("continuousModeEnable").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .dmaEnable = " + pdbCfgList[i].getChildById("dmaEnable").getValue() + ",\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "    .intEnable = " + pdbCfgList[i].getChildById("intEnable").getValue() + ",\n";
                    if (pdbCfgList[i].getChildById("instanceBackToBackEnable").isAvailable())
                    {
						cFileSections[GLOBAL_VARS_SECTION] += "    .instanceBackToBackEnable = " + pdbCfgList[i].getChildById("instanceBackToBackEnable").getValue() + ",\n";
					}
                    if (pdbCfgList[i].getChildById("interchannelBackToBackEnable").isAvailable())
                    {
						cFileSections[GLOBAL_VARS_SECTION] += "    .interchannelBackToBackEnable = " + pdbCfgList[i].getChildById("interchannelBackToBackEnable").getValue() + "\n";
					}
                    cFileSections[GLOBAL_VARS_SECTION] += "};\n\n";
                }
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}