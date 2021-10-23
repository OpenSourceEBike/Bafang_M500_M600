/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// FTM_OC component code generator class.
function FTM_OC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FTM_OC_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ftm_oc_driver.h");
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".totalConfiguration");
        var ftmInitConfig = ftmConfig.getChildById("ftmUserConfig");
        var ftmOcConfig = ftmConfig.getChildById("ocConfiguration");
        var ftmOcConfigDetail = ftmOcConfig.getChildById("arrayocConfig");
        var ftmOcConfigDetailChild = ftmOcConfigDetail.getChildren();
        var countftmOcConfigDetailChild = ftmOcConfigDetailChild.length;

        hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(4, 5) + "U") + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_output_cmp_param_t " + peripheralName + "_OutputCompareConfig;" + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of flexTimer_oc1 */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_user_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;
        
        if(countftmOcConfigDetailChild > 0)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for " + peripheralName + " output compare */"+ NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_output_cmp_ch_param_t " + peripheralName + "_OutputCompareChannelConfig[" + countftmOcConfigDetailChild + "];" + NEW_LINE;
        }
        
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".totalConfiguration");
        var ftmInitConfig = ftmConfig.getChildById("ftmUserConfig");
        var ftmInitConfigChild = ftmInitConfig.getChildren();
        var countftmInitConfigChild = ftmInitConfigChild.length;

             //var powerMode = ftmInitConfig.getChildById("PowerMode").getValue();
             var prescaler = ftmInitConfig.getChildById("ClockSourcePrescaler").getValue();
             var clockSelect = ftmInitConfig.getChildById("ClockSource").getValue();
            //var clockSourceExternal = ftmInitConfig.getChildById("ExternalClock").getValue();

            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_user_config_t " + peripheralName + "_InitConfig =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("softTrigger").getValue() + ",    /* Software trigger state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("hardwareTrigger1").getValue() + ",  /* Hardware trigger 1 state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("hardwareTrigger2").getValue() + ",  /* Hardware trigger 2 state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("hardwareTrigger3").getValue() + ",  /* Hardware trigger 3 state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("maxload").getValue() + ", /* Max loading point state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("minload").getValue() + ", /* Min loading point state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("ftmRegisterUpdate").getValue() + ", /* Update mode for INVCTRL register */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("ftmSoftwareOutControl").getValue() + ", /* Update mode for SWOCTRL register */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("ftmOutMaskSync").getValue() + ", /* Update mode for OUTMASK register */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("ftmCounterInitSync").getValue() + ", /* Update mode for CNTIN register */" + NEW_LINE;
            if (ftmInitConfig.getChildById("autoTriggerClear").getValue() == true)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Automatic clear of the trigger*/" + NEW_LINE;
            }
            else
            {
                cFileSections[GLOBAL_VARS_SECTION] += "        true, /* Automatic clear of the trigger*/" + NEW_LINE;
            }
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("ftmSyncPoint").getValue() + ", /* Synchronization point */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        FTM_MODE_OUTPUT_COMPARE, /* Mode of operation for FTM */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        FTM_CLOCK_DIVID_BY_" + prescaler + ", /* FTM clock prescaler */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + clockSelect + ",   /* FTM clock source */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("DebugMode").getValue() + ", /* FTM debug mode */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("TimerOverInterrupt").getValue() +",    /* Interrupt state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmInitConfig.getChildById("InitializationTrigger").getValue() +"     /* Initialization trigger */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        var ftmOcConfig = ftmConfig.getChildById("ocConfiguration");
        var ftmOcConfigDetail = ftmOcConfig.getChildById("arrayocConfig");
        var ftmOcConfigDetailChild = ftmOcConfigDetail.getChildren();
        var countftmOcConfigDetailChild = ftmOcConfigDetailChild.length;
        var numConfig = ftmOcConfig.getChildById("numofChannel").setValue(countftmOcConfigDetailChild);
        var maxTimerConfig = ftmOcConfig.getChildById("maxCountVal");
        if (maxTimerConfig.getValue() == 0)
        {
            maxTimerConfig.setValue(65535);
        }

        if (countftmOcConfigDetailChild > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Channels configuration structure for " + peripheralName + " output compare */"+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_output_cmp_ch_param_t " + peripheralName + "_OutputCompareChannelConfig[" + countftmOcConfigDetailChild + "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            var checkConflictChannel = false;
            for (var i = 0; i < countftmOcConfigDetailChild; i++) 
            {
                var ocConfigName = ftmOcConfigDetailChild[i].getChildById("configName");
                var ochwid = ftmOcConfigDetailChild[i].getChildById("hwID");
                var ochwidmode = ftmOcConfigDetailChild[i].getChildById("outputMode");
                var compareV = ftmOcConfigDetailChild[i].getChildById("CompareValue");
                var enableExternalTrig = ftmOcConfigDetailChild[i].getChildById("externalTrigger");
                ocConfigName.setValue(peripheralName + "_ChannelConfig" + i);

                if (maxTimerConfig.getValue() < compareV.getValue())
                {
                    scriptApi.logError(" The compare value of " + ochwid.getValue() + " must be lesser than the maximum count value");
                }

                if (checkConflictChannel == false)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + ochwid.getValue() + "U, /* Channel id */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + ochwidmode.getValue() + ", /* Output mode */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + compareV.getValue() + "U, /* Compared value */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + enableExternalTrig.getValue() + " /* External Trigger */" + NEW_LINE;
                    
                    if (i == countftmOcConfigDetailChild-1)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    }
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }

        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Output compare configuration for " + peripheralName + " */"+ NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_output_cmp_param_t " + peripheralName + "_OutputCompareConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + countftmOcConfigDetailChild + "U, /* Number of channel configurations */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    FTM_MODE_OUTPUT_COMPARE, /* FTM mode */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + maxTimerConfig.getValue() + "U, /* Maximum count value */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + peripheralName + "_OutputCompareChannelConfig /* Channels configuration*/" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
    }
}