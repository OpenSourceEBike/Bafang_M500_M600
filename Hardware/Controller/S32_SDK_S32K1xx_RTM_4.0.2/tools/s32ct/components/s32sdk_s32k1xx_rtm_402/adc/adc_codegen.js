/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// SWT component code generator class.
function ADC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "adc_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var converterConfigChildren = getSetting(this.uiConfig, this.uiConfig.getName()+".adcConverterCfg").getChildren();
            var compareConfigChildren   = getSetting(this.uiConfig, this.uiConfig.getName()+".adcCompareCfg").getChildren();
            var averageConfigChildren = getSetting(this.uiConfig, this.uiConfig.getName()+".adcAverageCfg").getChildren();
            var chanConfigChildren = getSetting(this.uiConfig, this.uiConfig.getName()+".adcChanCfg").getChildren();

            // Add Includes info.
            hFileSections[INCLUDES_SECTION] += addInclude("adc_driver.h");

            //Add Defines info.
            hFileSections[DEFINES_SECTION] += "\n/*Device instance number */\n";
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + peripheralName.substring(4, 5) + "U)");
            //Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configurations */" + NEW_LINE;
            if ((converterConfigChildren != null) && (converterConfigChildren.size() > 0)) {
                // Iterate through all configurations
                for (var i =  0; i < converterConfigChildren.size(); i++) {
                    //get config name
                    var crcCfgArray_Name = converterConfigChildren[i].getChildById("name").getStringValue();
                    //get config readonly
                    var crcCfgArray_ReadOnly = (converterConfigChildren[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Converter configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + crcCfgArray_ReadOnly + "adc_converter_config_t", crcCfgArray_Name);
                }
            }
            if ((compareConfigChildren != null) && (compareConfigChildren.size() > 0)) {
                // Iterate through all configurations
                for (var i =  0; i < compareConfigChildren.size(); i++) {
                    //get config name
                    var crcCfgArray_Name = compareConfigChildren[i].getChildById("name").getStringValue();
                    //get config readonly
                    var crcCfgArray_ReadOnly = (compareConfigChildren[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Hw Compare configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + crcCfgArray_ReadOnly + "adc_compare_config_t", crcCfgArray_Name);
                }
            }
            if ((averageConfigChildren != null) && (averageConfigChildren.size() > 0)) {
                // Iterate through all configurations
                for (var i =  0; i < averageConfigChildren.size(); i++) {
                    //get config name
                    var crcCfgArray_Name = averageConfigChildren[i].getChildById("name").getStringValue();
                    //get config readonly
                    var crcCfgArray_ReadOnly = (averageConfigChildren[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Hw Average configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + crcCfgArray_ReadOnly + "adc_average_config_t", crcCfgArray_Name);
                }
            }
            if ((chanConfigChildren != null) && (chanConfigChildren.size() > 0)) {
                // Iterate through all configurations
                for (var i =  0; i < chanConfigChildren.size(); i++) {
                    //get config name
                    var crcCfgArray_Name = chanConfigChildren[i].getChildById("name").getStringValue();
                    //get config readonly
                    var crcCfgArray_ReadOnly = (chanConfigChildren[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channel configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + crcCfgArray_ReadOnly + "adc_chan_config_t", crcCfgArray_Name);
                }
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n \* The external variables will be used in other source files in application code.\n *\n";

        var converterConfig   = getSetting(this.uiConfig, this.uiConfig.getName()+".adcConverterCfg");
        var converterConfigChildren = converterConfig.getChildren();

        /* Print adc converter configuration structure */
        if ((converterConfig != null) && converterConfig.isAvailable() && (converterConfigChildren.length > 0))
        {
            for (var i = 0; i < converterConfigChildren.length; i++) {

                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(converterConfigChildren[i], "adc_converter_config_t", "");
            }
        }

        var compareConfig   = getSetting(this.uiConfig, this.uiConfig.getName()+".adcCompareCfg");
        var compareConfigChildren = compareConfig.getChildren();

        /* Print adc converter configuration structure */
        if ((compareConfig != null) && compareConfig.isAvailable() && (compareConfigChildren.length > 0))
        {
            for (var i = 0; i < compareConfigChildren.length; i++) {

                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(compareConfigChildren[i], "adc_compare_config_t", "");
            }
        }

        var averageConfig   = getSetting(this.uiConfig, this.uiConfig.getName()+".adcAverageCfg");
        var averageConfigChildren = averageConfig.getChildren();

        /* Print adc converter configuration structure */
        if ((averageConfig != null) && averageConfig.isAvailable() && (averageConfigChildren.length > 0))
        {
            for (var i = 0; i < averageConfigChildren.length; i++) {

                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(averageConfigChildren[i], "adc_average_config_t", "");
            }
        }

        var chanConfig   = getSetting(this.uiConfig, this.uiConfig.getName()+".adcChanCfg");
        var chanConfigChildren = chanConfig.getChildren();

        /* Print adc converter configuration structure */
        if ((chanConfig != null) && chanConfig.isAvailable() && (chanConfigChildren.length > 0))
        {
            for (var i = 0; i < chanConfigChildren.length; i++) {

                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(chanConfigChildren[i], "adc_chan_config_t", "");
            }
        }
    }
}
