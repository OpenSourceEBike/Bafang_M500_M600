/*
 * Copyright 2018 NXP
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

/* I2S PAL component code generator class. */
function I2S_PAL_CodeGenerator(uiConfig) {
    this.uiCOnfig = uiConfig;

    /* Returns the class name. */
    this.toString = function () {
        return "I2S_PAL_CodeGenerator";
    }

    /* Generates the pal_cfg.h file */
    this.genHCfgFile = function(hCfgFileSections)
    {
        hCfgFileSections[FILE_NAME_SECTION] += "i2s_pal_cfg.h\n";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        /* Extern instance configuration */
        var i2sCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".i2sCfgArray").getChildren();
        var i2sCfgArray_Size = i2sCfgArray_Items.size();
        var numMaster = 0;
        var numSlave = 0;
        for (var i =  0; i < i2sCfgArray_Size; i++)
        {
            var commonI2sSetting = i2sCfgArray_Items[i].getChildById("i2sCommonConfig");
            var modeStruct = commonI2sSetting.getChildById("mode").getValue();
            if (modeStruct == "I2S_MASTER")
                numMaster++;
            else
                numSlave++;
        }
        if (peripheralName.indexOf("FLEXIO") == 0)
        {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2s_pal_cfg.h", "I2S_OVER_FLEXIO", "");
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2s_pal_cfg.h", "NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S",numSlave);
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2s_pal_cfg.h", "NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S", numMaster);
        }
        else if (peripheralName.indexOf("SAI") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2s_pal_cfg.h", "I2S_OVER_SAI", "");
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("i2s_pal_cfg.h", "NO_OF_SAI_INSTS_FOR_I2S");
        }
        else
        {
            // Do nothing
        }
    }

    /* Adds component specific info to header file sections. */
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        /* Get childContext class to extract peripheral name and functional group.*/
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
        var fnGroupName = childContext.getFunctionalGroup().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("i2s_pal.h");
        hFileSections[INCLUDES_SECTION] += "#include <stddef.h>\n";

        //Add external global variables
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration */" + NEW_LINE;

        /* Extern instance configuration */
        var instStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".i2sPalInst");
        var instStructNameSetting =instStruct.getChildById("name");
        var instStructName = instStructNameSetting.getStringValue();

        /* Extern initialize configuration */
        var i2sCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".i2sCfgArray").getChildren();
        var i2sCfgArray_Size = i2sCfgArray_Items.size();

        if (peripheralName == 'FLEXIO')
        {
            hFileSections[DEFINES_SECTION] += "#define INST_" + instanceName.toUpperCase() + " (&i2sOverFlexio0"+ "Instance)" + NEW_LINE;
        }
        else
        {
            hFileSections[DEFINES_SECTION] += "#define INST_" + instanceName.toUpperCase() + " (&i2sOverSai" + peripheralName.match(/\d+$/)[0] + "Instance)" + NEW_LINE;
        }
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const i2s_instance_t " + instStructName + ";\n"

        if ((i2sCfgArray_Items != null) && (i2sCfgArray_Size > 0))
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Serial module configurations */" + NEW_LINE;
            // Iterate through all configurations
            for (var i =  0; i < i2sCfgArray_Size; i++)
            {
                var commonI2sSetting = i2sCfgArray_Items[i].getChildById("i2sCommonConfig");
                var commonConfigName = commonI2sSetting.getChildById("name").getStringValue();
                var constInitStruct = commonI2sSetting.getChildById("readonly").getValue();
                var userCallback = commonI2sSetting.getChildById("callback").getStringValue();

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief I2S PAL User Configuration " + i + " */" + NEW_LINE;
                if (constInitStruct)
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const i2s_user_config_t " + commonConfigName + ";\n";
                }
                else
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern i2s_user_config_t " + commonConfigName + ";\n";
                }

                /* Extern callback function if they are available */
                if (userCallback != "NULL")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Define user's callbacks */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void "+ userCallback + "(i2s_event_t event, void *userData);" + NEW_LINE;
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            /* Get childContext class to extract peripheral name and functional group. */
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configName = childContext.getComponentInstanceConfig().getName()
            var fnGroupName = childContext.getFunctionalGroup().getName();

            /* Process parameters about instance configuration */
            var instStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".i2sPalInst");
            var instStructNameSetting =instStruct.getChildById("name");
            var instStructName = instStructNameSetting.getStringValue();
            var i2sCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".i2sCfgArray").getChildren();
            var i2sCfgArray_Size = i2sCfgArray_Items.size();

            if (instStruct != null)
            {
                var insttype = instStruct.getChildById("instType");
                var instidx = instStruct.getChildById("instIdx");
                if (peripheralName == 'FLEXIO')
                {
                    var newinstname = instStructName;
                    instidx.setValue(0);
                    insttype.setValue('I2S_INST_TYPE_FLEXIO');
                }
                else
                {
                    var newinstname = instStructName;
                    instidx.setValue(peripheralName.match(/\d+$/)[0]);
                    insttype.setValue('I2S_INST_TYPE_SAI');

                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(instStruct,"const i2s_instance_t","");

            /* Process parameters about extension */

            if ((i2sCfgArray_Items != null) && (i2sCfgArray_Size > 0))
            {
                for (var i =  0; i < i2sCfgArray_Size; i++)
                {
                    var commonI2sSetting = i2sCfgArray_Items[i].getChildById("i2sCommonConfig");
                    var commonConfigName = commonI2sSetting.getChildById("name");
                    /* Generate configuration structure code in c file */
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(commonI2sSetting, "i2s_user_config_t", "");
                }
            }
        } catch(err) {
            scriptApi.logInfo(err);
            }
    }
}
