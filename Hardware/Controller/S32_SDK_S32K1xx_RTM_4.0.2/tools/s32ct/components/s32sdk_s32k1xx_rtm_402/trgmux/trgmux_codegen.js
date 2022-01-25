/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* c indentation 4 */
var cIndent4 = '    ';

// TRGMUX component code generator class.
function TRGMUX_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "TRGMUX_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
                // Get childContext class to extract peripheral name and functional group.
                var InitArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".trgmuxCfg").getChildren();
                var InitArray_Size = InitArray_Items.size();
                var readOnly = "";
                var ioMappingcfgName = "";
                var instanceName = this.uiConfig.getName();
                if ((InitArray_Items != null) && (InitArray_Size > 0)) {
                    // Add Includes info.
                    hFileSections[INCLUDES_SECTION] += addInclude("trgmux_driver.h");
                    //Add Defines info.
                    hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(0U)");
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Declaration of InOut Mapping configuration */\n";
                    for(var i =0 ; i < InitArray_Size; i++)
                    {
                        readOnly = (InitArray_Items[i].getChildById("readonly").getValue() ? " const " : " ");
                        ioMappingcfgName = instanceName + "_InOutMappingConfig" + i;
                        var numInOutMappingConfigs = InitArray_Items[i].getChildById("mapping_cfg").getChildren().size();
                        hFileSections[DEFINES_SECTION] += addDefine("TRGMUX_NUM_IN_OUT_MAPPING_CONFIGS" + i, "(" + numInOutMappingConfigs + "U)");
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern" + readOnly + "trgmux_inout_mapping_config_t", ioMappingcfgName + "[TRGMUX_NUM_IN_OUT_MAPPING_CONFIGS" + i + "]");
                    }
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Configuration declaration */\n";
                    for(var i =0 ; i < InitArray_Size; i++)
                    {
                        readOnly = (InitArray_Items[i].getChildById("readonly").getValue() ? " const " : " ");
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern" + readOnly + "trgmux_user_config_t", InitArray_Items[i].getChildById("name").getValue());
                    }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            
            var InitArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".trgmuxCfg").getChildren();
            var InitArray_Size = InitArray_Items.size();
            var readOnly ="";
            var ioMappingcfgName = "";
            var instanceName = this.uiConfig.getName();
            if ((InitArray_Items != null) && (InitArray_Size > 0)) {
                for(var i =0 ; i < InitArray_Size; i++)
                {
                    var Mapping_list = InitArray_Items[i].getChildById("mapping_cfg").getChildren();
                    readOnly = (InitArray_Items[i].getChildById("readonly").getValue() ? "const " : "");
                    ioMappingcfgName = instanceName + "_InOutMappingConfig" + i;
                    cFileSections[GLOBAL_VARS_SECTION] += readOnly + "trgmux_inout_mapping_config_t " + ioMappingcfgName + "[TRGMUX_NUM_IN_OUT_MAPPING_CONFIGS" + i + "]" +" =\n";
                    cFileSections[GLOBAL_VARS_SECTION] += "{\n";
                    for(k = 0 ; k < Mapping_list.size(); k++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + "{";
                        cFileSections[GLOBAL_VARS_SECTION] += Mapping_list[k].getChildById("triggerSource").getValue() + ", ";
                        cFileSections[GLOBAL_VARS_SECTION] += Mapping_list[k].getChildById("targetModule").getValue() + ", ";
                        cFileSections[GLOBAL_VARS_SECTION] += Mapping_list[k].getChildById("lockreg").getValue() + "},\n";
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "};\n";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! trgmux configuration structure */\n";
                for(var i =0 ; i < InitArray_Size; i++)
                    {
                        readOnly = (InitArray_Items[i].getChildById("readonly").getValue() ? "const " : "");
                        cFileSections[GLOBAL_VARS_SECTION] += readOnly + "trgmux_user_config_t " + InitArray_Items[i].getChildById("name").getValue();
                        cFileSections[GLOBAL_VARS_SECTION] += " = {\n";
                        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".numInOutMappingConfigs" + " = ";
                        cFileSections[GLOBAL_VARS_SECTION] += "TRGMUX_NUM_IN_OUT_MAPPING_CONFIGS" + i + ",\n";
                        cFileSections[GLOBAL_VARS_SECTION] += cIndent4 + ".inOutMappingConfig" + cIndent4 + " = ";
                        cFileSections[GLOBAL_VARS_SECTION] += instanceName + "_InOutMappingConfig" + i + ",\n";
                        cFileSections[GLOBAL_VARS_SECTION] += "};\n";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                    }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
}