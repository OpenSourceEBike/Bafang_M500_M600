/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// Creates a string representation of the polynomial generator
function CRC_SW_PolyRepr(polyType, polyValue)
{
    var polyRepr = "";
    var counter = 0;
    switch (polyType)
    {
        case "CRC_BITS_16" :
            polyRepr += "x^16";
            counter = 15
            break;
        case "CRC_BITS_32" :
            polyRepr += "x^32";
            counter = 31
            break;
    }
    for (; counter >= 0; counter--)
    {
        if ((polyValue & (0x1 << counter)) != 0x0)
        {
            polyRepr += " + x^" + counter;
        }
    }
    return polyRepr
}

// CRC SW component code generator class.
function CRC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "CRC_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var crcCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".crcConfig").getChildren();
            var crcCfgArray_Size = crcCfgArray_Items.size();

            if ((crcCfgArray_Items != null) && (crcCfgArray_Size > 0)) {
                // Add Includes info.
                hFileSections[INCLUDES_SECTION] += addInclude("crc_driver.h");

                //Add Defines info.
                hFileSections[DEFINES_SECTION] += "\n/*Device instance number */\n";
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(0U)");
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configurations */" + NEW_LINE;

                // Iterate through all configurations
                for (var i =  0; i < crcCfgArray_Size; i++) {
                    //get config name
                    var crcCfgArray_Name = crcCfgArray_Items[i].getChildById("name").getStringValue();
                    //get config readonly
                    var crcCfgArray_ReadOnly = (crcCfgArray_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + crcCfgArray_ReadOnly + "crc_user_config_t", crcCfgArray_Name);
                }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var crcCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".crcConfig").getChildren();
            var crcCfgArray_Size = crcCfgArray_Items.size();

            if ((crcCfgArray_Items != null) && (crcCfgArray_Size > 0)) {
                for (var i =  0; i < crcCfgArray_Size; i++) {
                    // Update polynomial representation
                    var crcCfgArray_PolyLabel = crcCfgArray_Items[i].getChildById("polyRepresentation");
                    crcCfgArray_PolyLabel.setValue("`" + CRC_SW_PolyRepr(crcCfgArray_Items[i].getChildById("crcWidth").getValue(), crcCfgArray_Items[i].getChildById("polynomial").getValue()) + "`");

                    // Add crc configuration structure.
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(crcCfgArray_Items[i], "crc_user_config_t", "");
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
