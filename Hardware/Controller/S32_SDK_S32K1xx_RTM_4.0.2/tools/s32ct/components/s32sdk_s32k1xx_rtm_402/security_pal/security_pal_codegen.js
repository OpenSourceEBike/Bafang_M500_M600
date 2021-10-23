/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");


// Security PAL component code generator class.
function security_palCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "security_palCodeGen";
    }

    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "security_pal_cfg.h\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if (peripheralName.indexOf("CSE") == 0 ) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("security_pal_cfg.h", "SECURITY_OVER_CSE", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("security_pal_cfg.h", "NO_OF_CSE_INSTS_FOR_SECURITY");
        } else if (peripheralName.indexOf("FTFC") == 0  || peripheralName.indexOf("FTFM") == 0  || peripheralName.indexOf("FTFx") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("security_pal_cfg.h", "SECURITY_OVER_CSEC", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("security_pal_cfg.h", "NO_OF_CSEC_INSTS_FOR_SECURITY");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".security_pal_user_config");
        var configStructNameSetting = initStruct.getChildById("name");
        var configStructName = configStructNameSetting.getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("stddef.h");
        hFileSections[INCLUDES_SECTION] += addInclude("security_pal.h");

        if(initStruct != null)
        {
            //Add Defines info.
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const security_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern security_user_config_t", configStructName);
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".security_pal_user_config");

            if(initStruct != null)
            {
                // Add linflexd_uart config structure.
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(initStruct, "security_user_config_t", "");
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
