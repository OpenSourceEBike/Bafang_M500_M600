/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// lPIT component code generator class.
function LPIT_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "LPIT_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections)
    {
         hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
         var readOnly = "";
         // Get childContext class to extract peripheral name and functional group.
         var childContext = uiConfig.getChildContext();
         var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
         var instanceName = childContext.getComponentInstanceConfig().getName();
         var globalInitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".lpitConfig");
         var globalcfgName = globalInitStruct.getChildById("name").getStringValue();

         var chanInitList = getSetting(this.uiConfig, this.uiConfig.getName()+".lpitChannelConfig").getChildren();
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("lpit_driver.h");

        if(globalInitStruct != null)
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += "\n/*Device instance number */\n";
            //Number of instance
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(0U)");

            //Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* LPIT global configuration */";
            globalcfgName = globalInitStruct.getChildById("name").getStringValue();
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern const " + "lpit_user_config_t", globalcfgName);
        }

        if(chanInitList != null)
        {

            if(chanInitList.length > 0)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* LPIT channel configuration */";
                for (var i = 0; i < chanInitList.length; i++)
                {
                    var chanCfgName = chanInitList[i].getChildById("name").getStringValue();
                    readOnly = (chanInitList[i].getChildById("cfgReadOnly").getValue() ? " const " : " ");
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + readOnly + "lpit_user_channel_config_t", chanCfgName);
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var readOnly = "";
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var globalInitStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".lpitConfig");
            var globalcfgName = globalInitStruct.getChildById("name").getStringValue();

            var chanInitList = getSetting(this.uiConfig, this.uiConfig.getName()+".lpitChannelConfig").getChildren();

            if(globalInitStruct != null)
            {
                // Add lPIT configuration structure.
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* lPIT global configuration */";
                globalcfgName = globalInitStruct.getChildById("name").getStringValue();
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(globalInitStruct, "const " + "lpit_user_config_t ", "" );
            }

            if(chanInitList != null)
            {
                for (var i = 0; i < chanInitList.length; i++)
                {
                    if (chanInitList[i].getChildById("timerMode").getValue() == "LPIT_INPUT_CAPTURE")
                    {
                        chanInitList[i].getChildById("period").setValue(4294967295);
                    }
                    if (chanInitList[i].getChildById("period").getValue() == 0)
                    {
                        chanInitList[i].getChildById("period").setValue(1);
                    }
                }

                if(chanInitList.length > 0)
                {
                    for (var i = 0; i < chanInitList.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Channel configuration " + i + "*/";
                        readOnly = (chanInitList[i].getChildById("cfgReadOnly").getValue() ? "const " : "");
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(chanInitList[i], readOnly + "lpit_user_channel_config_t ", "" );
                    }
                }
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
