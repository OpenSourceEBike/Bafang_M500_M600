/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// FTM_MC component code generator class.
function FTM_QD_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FTM_QD_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ftm_qd_driver.h");
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        var ftmQDConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmUserConfig");
        var clockSelect = ftmQDConfiguration.getChildById("ClockSource").getValue();
        hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(4, 5) + "U") + NEW_LINE;

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_user_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;

        if (clockSelect != "FTM_CLOCK_SOURCE_NONE")
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\nextern ftm_quad_decode_config_t " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("configName").getValue() + ";" + NEW_LINE ;
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var ftmQDConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmUserConfig");
        //var powerMode = ftmQDConfiguration.getChildById("PowerMode").getValue();
        var prescaler = ftmQDConfiguration.getChildById("ClockSourcePrescaler").getValue();
        var clockSelect = ftmQDConfiguration.getChildById("ClockSource").getValue();
        //var clockSource = ftmQDConfiguration.getChildById("FrequencyValue").getValue();
        //var clockSourceExternal = ftmQDConfiguration.getChildById("ExternalClock").getValue();

        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_user_config_t " + peripheralName + "_InitConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        true,    /* Software trigger state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 1 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 2 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false,  /* Hardware trigger 3 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Max loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Min loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for INVCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for SWOCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for OUTMASK register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_SYSTEM_CLOCK, /* Update mode for CNTIN register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Automatic clear of the trigger*/" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_UPDATE_NOW, /* Synchronization point */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_MODE_QUADRATURE_DECODER, /*!< Mode of operation for FTM */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        FTM_CLOCK_DIVID_BY_" + prescaler + ", /* FTM clock prescaler */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + clockSelect + ",   /* FTM clock source */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("DebugMode").getValue() + ", /* FTM debug mode */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("TimerOverInterrupt").getValue() +",    /* Interrupt state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("InitializationTrigger").getValue() +"     /* Initialization trigger */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        if (clockSelect != "FTM_CLOCK_SOURCE_NONE")
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\nftm_quad_decode_config_t " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("configName").getValue() + " =" + NEW_LINE ;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadDecoderMode").getValue() + ", /* Quadrature decoder mode */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("initCountValue").getValue() + "U, /* Initial counter value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("maxCountValue").getValue() + "U, /* Maximum counter value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseAConfig").getChildById("phaseAInputFilter").getValue() + ", /* Phase A input filter */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseAConfig").getChildById("phaseAInputValue").getValue() + "U, /* Phase A input value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseAConfig").getChildById("ftm_quad_phaseA_polarity_t").getValue() + ", /* Phase A polarity */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseBConfig").getChildById("phaseBInputFilter").getValue() + ", /* Phase B input filter */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseBConfig").getChildById("phaseBInputValue").getValue() + "U, /* Phase B input value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmQDConfiguration.getChildById("quadDecoderConfiguration").getChildById("quadPhaseBConfig").getChildById("ftm_quad_phaseB_polarity_t").getValue() + ", /* Phase B polarity */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }

    }
}
