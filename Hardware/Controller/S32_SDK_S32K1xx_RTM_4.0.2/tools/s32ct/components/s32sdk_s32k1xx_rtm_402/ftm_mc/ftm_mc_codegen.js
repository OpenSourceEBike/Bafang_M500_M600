/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// FTM_MC component code generator class.
function FTM_MC_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FTM_MC_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ftm_mc_driver.h");
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        var ftmMCConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmUserConfig");
        var ftmMCConfigChild = ftmMCConfiguration.getChildren();
        var countftmMCConfigChild = ftmMCConfigChild.length;
        hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(4, 5) + "U") + NEW_LINE;
        for(var i = 0;i < countftmMCConfigChild; i++)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig " + i + " */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_user_config_t " + peripheralName + "_InitConfig_" + i + ";" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Global configuration of " + peripheralName + " TimerConfig " + i + " */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_timer_param_t " + peripheralName + "_TimerConfig_" + i + ";" + NEW_LINE ;
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var ftmMCConfiguration = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmUserConfig");
        var ftmMCConfigChild = ftmMCConfiguration.getChildren();
        var countftmMCConfigChild = ftmMCConfigChild.length;

        // cFileSections[GLOBAL_VARS_SECTION] += countftmMCConfigChild + NEW_LINE;
        for (var i = 0; i < countftmMCConfigChild; i++) {
            var prescaler = ftmMCConfigChild[i].getChildById("ClockSourcePrescaler").getValue();
            var clockSelect = ftmMCConfigChild[i].getChildById("ClockSource").getValue();
            //var clockSourceExternal = ftmMCConfigChild[i].getChildById("ExternalClock").getValue();
            var timerConfig = ftmMCConfigChild[i].getChildById("timerConfiguration");
            var upTimerConfig = timerConfig.getChildById("FinalCounter").getValue();
            var downTimerConfig = timerConfig.getChildById("InitialCounter").getValue();
            if (upTimerConfig == 0)
            {
                upTimerConfig = 65535;
                timerConfig.getChildById("FinalCounter").setValue(upTimerConfig);
            }
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig " + i + " */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_user_config_t " + peripheralName + "_InitConfig_" + i + " =" + NEW_LINE;
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
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmMCConfigChild[i].getChildById("TimerOperationMode").getValue() + ", /* Mode of operation for FTM */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        FTM_CLOCK_DIVID_BY_" + prescaler + ", /* FTM clock prescaler */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + clockSelect + ",   /* FTM clock source */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmMCConfigChild[i].getChildById("DebugMode").getValue() + ", /* FTM debug mode */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmMCConfigChild[i].getChildById("TimerOverInterrupt").getValue() +",    /* Interrupt state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmMCConfigChild[i].getChildById("InitializationTrigger").getValue() +"     /* Initialization trigger */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Timer mode configuration for " + peripheralName + " TimerConfig " + i + " */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_timer_param_t " + peripheralName + "_TimerConfig_" + i + " =" + NEW_LINE ;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmMCConfigChild[i].getChildById("TimerOperationMode").getValue() + ", /* Counter mode */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + downTimerConfig + ", /* Initial counter value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "        " + upTimerConfig + " /* Final counter value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }
    }
}