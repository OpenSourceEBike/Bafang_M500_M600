/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// CRC HW component code generator class.
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
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + peripheralName.substring(4, 5) + "U)");
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

    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var crcCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".crcConfig").getChildren();
            var crcCfgArray_Size = crcCfgArray_Items.size();

            if ((crcCfgArray_Items != null) && (crcCfgArray_Size > 0)) {
                for (var i =  0; i < crcCfgArray_Size; i++) {
                    var crcCfgArray_SeedValue = crcCfgArray_Items[i].getChildById("seed").getValue();

                    // Update polynomial representation and validate seed
                    var crcCfgArray_PolyLabel = crcCfgArray_Items[i].getChildById("polyRepresentation");
                    var crcCfgArray_SelectedPoly = crcCfgArray_Items[i].getChildById("polynomial");
                    switch (crcCfgArray_SelectedPoly.getValue()) {
                        case "CRC_BITS_8":
                            crcCfgArray_PolyLabel.setValue("`x^8+x^4+x^3+x^2+x^0`");
                            if ((crcCfgArray_SeedValue < 0x00) || (crcCfgArray_SeedValue > 0xFF)) {
                                throw "Seed value must be in range [0x00 - 0xFF].";
                            }
                            break;
                        case "CRC_BITS_8_H2F":
                            crcCfgArray_PolyLabel.setValue("`x^5+x^3+x^2+x^1+x^0`");
                            if ((crcCfgArray_SeedValue < 0x00) || (crcCfgArray_SeedValue > 0xFF)) {
                                throw "Seed value must be in range [0x00 - 0xFF].";
                            }
                            break;
                        case "CRC_BITS_16_CCITT":
                            crcCfgArray_PolyLabel.setValue("`x^16+x^12+x^5+x^0`");
                            if ((crcCfgArray_SeedValue < 0x0000) || (crcCfgArray_SeedValue > 0xFFFF)) {
                                throw "Seed value must be in range [0x0000 - 0xFFFF].";
                            }
                            break;
                        case "CRC_BITS_32":
                            crcCfgArray_PolyLabel.setValue("`x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x^1+x^0`");
                            if ((crcCfgArray_SeedValue < 0x00000000) || (crcCfgArray_SeedValue > 0xFFFFFFFF)) {
                                throw "Seed value must be in range [0x00000000 - 0xFFFFFFFF].";
                            }
                            break;
                        default:
                            crcCfgArray_PolyLabel.setValue("`N/A`");
                            throw "No polynomial selected. Something went really wrong";
                            break;
                    }

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

