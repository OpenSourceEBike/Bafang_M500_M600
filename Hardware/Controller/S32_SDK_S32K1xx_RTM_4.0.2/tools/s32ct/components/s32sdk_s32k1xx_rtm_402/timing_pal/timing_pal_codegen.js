/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");


// WDG PAL component code generator class.
function timing_palCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "timing_palCodeGen";
    }

    /* Generates the pal_cfg.h file */
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "timing_pal_cfg.h\n";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        // Add define info
        if (peripheralName.indexOf("PIT") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("timing_pal_cfg.h", "TIMING_OVER_PIT", 1);
        }

        if (peripheralName.indexOf("STM") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("timing_pal_cfg.h", "TIMING_OVER_STM", 1);
        }

        if (peripheralName.indexOf("FTM") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("timing_pal_cfg.h", "TIMING_OVER_FTM", 1);
        }

        if (peripheralName.indexOf("LPTMR") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("timing_pal_cfg.h", "TIMING_OVER_LPTMR", 1);
        }

        if (peripheralName.indexOf("LPIT") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("timing_pal_cfg.h", "TIMING_OVER_LPIT", 1);
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

         var readOnly = "";
        // Get childContext class to extract peripheral name and functional group.
         var childContext = uiConfig.getChildContext();
         var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
         var instanceName = childContext.getComponentInstanceConfig().getName();

        //var fnGroupName = childContext.getFunctionalGroup().getName();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("timing_pal.h");

        /* Extern instance configuration */
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".timingPalInst");
        var instanceCfgName = initStruct.getChildById("name").getStringValue();
        var cfgReadOnly = initStruct.getChildById("readonly").getValue();

        if(initStruct != null)
        {
            var instanceCfgName = initStruct.getChildById("name").getStringValue();
            //Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* PAL instance information */";
            readOnly = (cfgReadOnly ? " const " : " ");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + readOnly + "timing_instance_t", instanceCfgName);
        }

        /* Extern channel configuration, timing configuration, and callback function */
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".timingPalConfig");
        var cfgName = initStruct.getChildById("name").getStringValue();
        var cfgReadOnly = initStruct.getChildById("readonly").getValue();
        var chanList = initStruct.getChildById("chanConfigList").getChildren();

        if(initStruct != null)
        {
            var cfgName = initStruct.getChildById("name").getStringValue();
            //Add external global variables
            readOnly = (cfgReadOnly ? " const " : " ");

            if(chanList.length > 0)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channel configuration array */";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + readOnly + "timer_chan_config_t", instanceCfgName + "_" + "chanConfig" + "[" + chanList.length + "]");
            }

            var stmExten = initStruct.getChildById("stmExt");
            var ftmExten = initStruct.getChildById("ftmExt");
            var lptmrExten = initStruct.getChildById("lptmrExt");

            if(stmExten != null && stmExten.isAvailable())
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* STM specific extension configuration */";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern " + "extension_stm_for_timer_t", instanceCfgName + "_" + "extension");
            }

            if(ftmExten != null && ftmExten.isAvailable())
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* FTM specific extension configuration */";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern " + "extension_ftm_for_timer_t", instanceCfgName + "_" + "extension");
            }

            if(lptmrExten != null && lptmrExten.isAvailable())
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* LPTMR specific extension configuration */";
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern " + "extension_lptmr_for_timer_t", instanceCfgName + "_" + "extension");
            }

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Timing configuration */";
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + readOnly + "timer_config_t", cfgName);

            if(chanList.length > 0)
            {
                for (var i = 0; i < chanList.length; i++)
                {
                    var chanCfgName = chanList[i].getChildById("timerCallback").getStringValue();
                    if (chanCfgName != "NULL")
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Callback function of channel configuration " + i + " */";
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + " void", chanCfgName + "(void * userData)");
                    }
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
        var readOnly = "";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        /* Extern instance configuration */
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".timingPalInst");
        var instanceCfgName = initStruct.getChildById("name").getStringValue();

        if(initStruct != null)
        {
            var readOnly = (initStruct.getChildById("readonly").getValue() ? "const " : " ");
            var instidx = initStruct.getChildById("instIdx");
            var insttype = "";
            if ((peripheralName != "PIT") && (peripheralName != "STM_TS"))
            {
                var periphnumb = peripheralName.match(/\d+$/)[0];
				var periphtype = peripheralName.replace(periphnumb,'');
            }
            if (periphtype == "PIT_")
            {
                instidx.setValue(periphnumb);
                insttype = 'TIMING_INST_TYPE_PIT';
            }
            else if (periphtype == "STM_")
            {
                instidx.setValue(periphnumb);
                insttype = 'TIMING_INST_TYPE_STM';
            }
            else if (periphtype == "FTM_")
            {
                instidx.setValue(periphnumb);
                insttype = 'TIMING_INST_TYPE_FTM';
            }
            else if (peripheralName == "PIT")
            {
                instidx.setValue(0);
                insttype = 'TIMING_INST_TYPE_PIT';
            }
			else if (peripheralName == "STM_TS")
            {
                instidx.setValue(8);
                insttype = 'TIMING_INST_TYPE_STM';
            }
            else if (periphtype == "LPTMR_")
            {
                instidx.setValue(periphnumb);
                insttype = 'TIMING_INST_TYPE_LPTMR';
            }
            else if (periphtype == "LPIT_")
            {
                instidx.setValue(periphnumb);
                insttype = 'TIMING_INST_TYPE_LPIT';
            }
            else
            {
                /* do nothing */
            }

            //Add external global variables
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* PAL instance information */";
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "timing_instance_t " + instanceCfgName + " = {";
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .instType = " + insttype + ",";
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .instIdx = " + instidx.getValue();
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
        }


        /* Extern channel configuration, timing configuration, and callback function */
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".timingPalConfig");
        var cfgName = initStruct.getChildById("name").getStringValue();
        var cfgReadOnly = initStruct.getChildById("readonly").getValue();
        var chanList = initStruct.getChildById("chanConfigList").getChildren();

        if(initStruct != null)
        {

            if(chanList.length > 0)
            {
                for(var i = 0; i < chanList.length; i++)
                {
                    if(chanList[i].getChildById("timerCallback").getStringValue() == "")
                    {
                        chanList[i].getChildById("timerCallback").setValue("NULL");
                    }

                    if(chanList[i].getChildById("callbackParameter").getStringValue() == "")
                    {
                        chanList[i].getChildById("callbackParameter").setValue("NULL");
                    }
                }
            }
            //Add external global variables
            readOnly = (cfgReadOnly ? "const " : " ");

            if (chanList.length > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* Channel configuration array */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "timer_chan_config_t " + instanceCfgName + "_" + "chanConfig" + "[" + chanList.length + "]"+ " = {";
                for (var i = 0; i < chanList.length; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* Channel configuration " + i  + " */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "       .channel = " + chanList[i].getChildById("channel").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "       .chanType = " + chanList[i].getChildById("chanType").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "       .callback = " + chanList[i].getChildById("timerCallback").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "       .callbackParam = " + chanList[i].getChildById("callbackParameter").getValue();
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }";
                    if (i < (chanList.length - 1))
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += ",";
                    }
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
            }

            var stmExten = initStruct.getChildById("stmExt");
            var ftmExten = initStruct.getChildById("ftmExt");
            var lptmrExten = initStruct.getChildById("lptmrExt");

            if(stmExten != null && stmExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* STM specific extension configuration */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "extension_stm_for_timer_t " + instanceCfgName + "_" + "extension" + " = {";
                if(stmExten.getChildById("stmClockSource").isAvailable())
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .clockSelect = " + stmExten.getChildById("stmClockSource").getValue() + ",";
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .prescaler = " + stmExten.getChildById("stmPrescaler").getValue();
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
            }

            if(ftmExten != null && ftmExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* FTM specific extension configuration */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "extension_ftm_for_timer_t " + instanceCfgName + "_" + "extension" + " = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .clockSelect = " + ftmExten.getChildById("ftmClockSource").getValue() + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .prescaler = " + ftmExten.getChildById("ftmPrescaler").getValue() + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .finalValue = " + ftmExten.getChildById("finalValue").getValue();
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
            }

            if(lptmrExten != null && lptmrExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* LPTMR specific extension configuration */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "extension_lptmr_for_timer_t " + instanceCfgName + "_" + "extension" + " = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .clockSelect = " + lptmrExten.getChildById("lptmrClockSource").getValue() + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .prescaler = " + lptmrExten.getChildById("lptmrPrescaler").getValue() + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .bypassPrescaler = " + lptmrExten.getChildById("bypassPrescaler").getValue();
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
            }

            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Timing configuration */";
            var cfgName = initStruct.getChildById("name").getStringValue();
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "timer_config_t " + cfgName + " = {";
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .chanConfigArray = " + instanceCfgName + "_" + "chanConfig" + ",";
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .numChan = " + chanList.length + "U" + ",";
            if(stmExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .extension = &" + instanceCfgName + "_" + "extension";
            }
            else if(ftmExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .extension = &" + instanceCfgName + "_" + "extension";
            }
            else if(lptmrExten.isAvailable())
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .extension = &" + instanceCfgName + "_" + "extension";
            }
            else
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "   .extension = " + "NULL";
            }

            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
        }


        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
