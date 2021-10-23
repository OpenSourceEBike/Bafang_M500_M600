/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// FTM_PWM component code generator class.
function FTM_PWM_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FTM_PWM_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("ftm_pwm_driver.h");
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmPwmUserConfig");
        var ftmPwmConfig = ftmConfig.getChildById("ftmPwmConfiguration");
        var ftmPwmArrConfig = ftmPwmConfig.getChildById("FtmPwmConfigArr");
        var ftmPwmIndptConfig = ftmPwmArrConfig.getChildById("ftmPwmIndptChnCfg");
        var ftmPwmCombConfig = ftmPwmArrConfig.getChildById("ftmPwmCombChnCfg");
        var ftmPwmFaultConfig = ftmPwmArrConfig.getChildById("faultConfig")

        var ftmPwmIndptChnArr = ftmPwmIndptConfig.getChildById("pwmIndependentChannelConfig");
        var ftmPwmIndptChnDetail = ftmPwmIndptChnArr.getChildren();
        var ftmPwmNumOfIndptChn = ftmPwmIndptChnDetail.length;
        var ftmPwmCombChnArr = ftmPwmCombConfig.getChildById("pwmCombinedChannelConfig");
        var ftmPwmCombChnDetail = ftmPwmCombChnArr.getChildren();
        var ftmPwmNumOfCombChn = ftmPwmCombChnDetail.length;

        hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(4, 5) + "U") + NEW_LINE;

        if (ftmPwmFaultConfig.available)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Fault configuration structure for " + peripheralName + " */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_pwm_fault_param_t " + peripheralName + "_FaultConfig;" + NEW_LINE;
        }

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* PWM configuration for " + peripheralName + " */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_pwm_param_t " + peripheralName + "_PwmConfig;" + NEW_LINE;
        if (ftmPwmNumOfIndptChn > 0)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for the independent channels */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_independent_ch_param_t " + peripheralName + "_IndependentChannelsConfig[" + ftmPwmNumOfIndptChn + "];" + NEW_LINE;
        }
        
        if (ftmPwmNumOfCombChn > 0)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Channels configuration structure for the combined channels */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_combined_ch_param_t " + peripheralName + "_CombinedChannelsConfig[" + ftmPwmNumOfCombChn + "];" + NEW_LINE;
        }

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Global configuration of flexTimer */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern ftm_user_config_t " + peripheralName + "_InitConfig;" + NEW_LINE;
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getName();
        var instanceName = childContext.getComponentInstanceConfig().getName();
        var ftmConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".ftmPwmUserConfig");

        var ftmInitConfig = ftmConfig.getChildById("ftmPwmInitConfig");
        var ftmPwmRegSyncConfig = ftmInitConfig.getChildById("ftmPwmRegSyncCfg");
        var ftmPwmGlobalConfig = ftmInitConfig.getChildById("ftmPwmGlobalCfg");
        var ftmPwmClockConfig = ftmInitConfig.getChildById("ftmPwmClkCfg");
        var ftmPwmConfig = ftmConfig.getChildById("ftmPwmConfiguration");
        var ftmInitConfigChild = ftmInitConfig.getChildren();
        var countftmInitConfigChild = ftmInitConfigChild.length;

        cFileSections[GLOBAL_VARS_SECTION] += "\n/* Global configuration of " + peripheralName + " InitConfig */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_user_config_t " + peripheralName + "_InitConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("softTrigger").getValue() + ", /* Software trigger state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("hardwareTrigger1").getValue() + ", /* Hardware trigger 1 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("hardwareTrigger2").getValue() + ", /* Hardware trigger 2 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("hardwareTrigger3").getValue() + ", /* Hardware trigger 3 state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("maxload").getValue() + ", /* Max loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("minload").getValue() + ", /* Min loading point state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("ftmRegisterUpdate").getValue() + ", /* Update mode for INVCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("ftmSoftwareOutControl").getValue() + ", /* Update mode for SWOCTRL register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("ftmOutMaskSync").getValue() + ", /* Update mode for OUTMASK register */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("ftmCounterInitSync").getValue() + ", /* Update mode for CNTIN register */" + NEW_LINE;
        if (ftmPwmRegSyncConfig.getChildById("AutoTrigClear").getValue() == true)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "        false, /* Automatic clear of the trigger*/" + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] += "        true, /* Automatic clear of the trigger*/" + NEW_LINE;
        }
        cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmRegSyncConfig.getChildById("ftmSyncPoint").getValue() + ", /* Synchronization point */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGlobalConfig.getChildById("ftmMode").getValue() + ", /* Mode of operation for FTM */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    FTM_CLOCK_DIVID_BY_" + ftmPwmClockConfig.getChildById("ftmprescaler").getValue() + ", /* FTM clock prescaler */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmClockConfig.getChildById("ClockSource").getValue() + ",   /* FTM clock source */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGlobalConfig.getChildById("debugMode").getValue() + ", /* FTM debug mode */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGlobalConfig.getChildById("timerOverInter").getValue() + ",    /* Interrupt state */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGlobalConfig.getChildById("initTrigger").getValue() + "     /* Initialization trigger */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;

        var ftmPwmGeneralConfig = ftmPwmConfig.getChildById("ftmPwmGeneralCfg");
        var ftmPwmArrConfig = ftmPwmConfig.getChildById("FtmPwmConfigArr");
        var ftmPwmIndptConfig = ftmPwmArrConfig.getChildById("ftmPwmIndptChnCfg");
        var ftmPwmCombConfig = ftmPwmArrConfig.getChildById("ftmPwmCombChnCfg");
        var ftmPwmIndptChnArr = ftmPwmIndptConfig.getChildById("pwmIndependentChannelConfig");
        var ftmPwmIndptChnDetail = ftmPwmIndptChnArr.getChildren();
        var ftmPwmNumOfIndptChn = ftmPwmIndptChnDetail.length;
        var ftmPwmCombChnArr = ftmPwmCombConfig.getChildById("pwmCombinedChannelConfig");
        var ftmPwmCombChnDetail = ftmPwmCombChnArr.getChildren();
        var ftmPwmNumOfCombChn = ftmPwmCombChnDetail.length;
        //Set the number of configuration for FTM_PWM
        var ftmPwmNumOfIndptChnCfg = ftmPwmIndptConfig.getChildById("ftmPwmNumOfIndeptChn");
        ftmPwmNumOfIndptChnCfg.setValue(ftmPwmNumOfIndptChn);
        var ftmPwmNumOfCombChnCfg = ftmPwmCombConfig.getChildById("ftmPwmNumOfCombChn");
        ftmPwmNumOfCombChnCfg.setValue(ftmPwmNumOfCombChn);

        // FaultConfig
        var ftmPwmFaultConfig = ftmPwmArrConfig.getChildById("faultConfig")
        var pwmOutputStateOnFault = ftmPwmFaultConfig.getChildById("pwmOutputStateOnFault")
        var pwmFaultInterrupt = ftmPwmFaultConfig.getChildById("pwmFaultInterrupt")
        var faultFilterValue = ftmPwmFaultConfig.getChildById("faultFilterValue")
        var faultMode = ftmPwmFaultConfig.getChildById("faultMode")
        var ftmFaultChannelArray = ftmPwmFaultConfig.getChildById("ftmFaultChannelParam")
        var ftmFaultChannelDetail = ftmFaultChannelArray.getChildren();
        var ftmPwmNumOfFaultChnCfg = ftmFaultChannelDetail.size();

        if (ftmPwmFaultConfig.available)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* Fault configuration structure for " + peripheralName + "*/"+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_pwm_fault_param_t " + peripheralName + "_FaultConfig =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;

            cFileSections[GLOBAL_VARS_SECTION] += "    " + pwmOutputStateOnFault.getValue() + ", /* Output pin state on fault */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + pwmFaultInterrupt.getValue() + ", /* PWM fault interrupt state */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + faultFilterValue.getValue() + "U, /* Fault filter value */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    " + faultMode.getValue() + ", /* Fault mode */" + NEW_LINE;

            cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;

            if (ftmPwmNumOfFaultChnCfg == 1)
            {
                for (var i = 0; i < 2; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel filter state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "FTM_POLARITY_LOW" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                }

                    /* S32K116_FTM0_FLT2 */
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[0].getChildById("faultChannelEnabled").getValue() + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[0].getChildById("faultFilterEnabled").getValue() + ", /* Fault channel filter state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[0].getChildById("ftmFaultPinPolarity").getValue() + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;

                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel filter state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "FTM_POLARITY_LOW" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
            }
            else
            {
                if (ftmPwmNumOfFaultChnCfg > 0)
                {
                    for (var i = 0; i < ftmPwmNumOfFaultChnCfg; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[i].getChildById("faultChannelEnabled").getValue() + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[i].getChildById("faultFilterEnabled").getValue() + ", /* Fault channel filter state (Enabled/Disabled) */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            " + ftmFaultChannelDetail[i].getChildById("ftmFaultPinPolarity").getValue() + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    }
                }

                /* Default values */
                var faultChannelsDefault = ftmPwmFaultConfig.getChildById("faultChannelsDefault").getValue()
                for (var i = ftmPwmNumOfFaultChnCfg; i < faultChannelsDefault; i++)
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "false" + ", /* Fault channel filter state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            " + "FTM_POLARITY_LOW" + ", /* Fault channel state (Enabled/Disabled) */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                }
            }

            cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }

        if (ftmPwmNumOfIndptChn > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* The independent channels configuration structure for " + peripheralName + "_IndependentChannelsConfig */"+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_independent_ch_param_t " + peripheralName + "_IndependentChannelsConfig[" + ftmPwmNumOfIndptChn + "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;

            for (var i = 0; i < ftmPwmNumOfIndptChn; i++) 
            {
                var ftmPwmConfigName = ftmPwmIndptChnDetail[i].getChildById("ftmpwmIndptName");
                var ftmPwmInitName = peripheralName + "_IndependentChannelConfig" + i;
                ftmPwmConfigName.setValue(ftmPwmInitName);
                
                var ftmPwmHwChnId = ftmPwmIndptChnDetail[i].getChildById("ftmPwmIndptHwChnId");
                var ftmPwmPol = ftmPwmIndptChnDetail[i].getChildById("polarity");
                var ftmPwmDutyCycle = ftmPwmIndptChnDetail[i].getChildById("uDutyCyclePercent");
                var ftmPwmExtTrigEn = ftmPwmIndptChnDetail[i].getChildById("enableExternalTrigger");
                var ftmPwmSafeState = ftmPwmIndptChnDetail[i].getChildById("safeState");
                var ftmPwmSecChnOutEn = ftmPwmIndptChnDetail[i].getChildById("enableSecondChannelOutput");
                var ftmPwmSecChnPol = ftmPwmIndptChnDetail[i].getChildById("secondChannelPolarity");
                var ftmPwmDeadtimeEn = ftmPwmIndptChnDetail[i].getChildById("IndptdeadTimeEn");
                var ftmPwmIndtHwChnId = ftmPwmHwChnId.getValue().substring(0,1);
                ftmPwmIndtHwChnId++;

                for (var k = 0; k < ftmPwmNumOfIndptChn; k++)
                {
                    var ftmPwmIndtHwNextChnId = ftmPwmIndptChnDetail[k].getChildById("ftmPwmIndptHwChnId");
                    var ftmPwmIndtNextChnVal = ftmPwmIndtHwNextChnId.getValue().substring(0,1);

                    if (ftmPwmIndtHwChnId == ftmPwmIndtNextChnVal)
                    {
                        if (ftmPwmSecChnOutEn.getValue() == true)
                        scriptApi.logError("The channel has used in another configuration");
                    }
                }

                cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmHwChnId.getValue() + ", /* Hardware channel Id */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmPol.getValue() + ", /* Polarity of the PWM signal */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmDutyCycle.getValue() + "U, /* Duty cycle percent 0-0x8000 */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmExtTrigEn.getValue() + ", /* External Trigger */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSafeState.getValue() + ", /* Safe state of the PWM channel when faults are detected */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecChnOutEn.getValue() + ", /* Enabled/disabled the channel (n+1) output */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecChnPol.getValue() + ", /* Select channel (n+1) output relative to channel (n) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmDeadtimeEn.getValue() + ", /* Dead time enabled/disabled */" + NEW_LINE;
                
                if (i == (ftmPwmNumOfIndptChn-1))
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                }

                for (var j = 0; j < ftmPwmNumOfCombChn; j++)
                {
                    var ftmPwmCombineHwChnId = ftmPwmCombChnDetail[j].getChildById("ftmPwmCombHwChnId");
                    var ftmPwmIndtChnVal = ftmPwmHwChnId.getValue().substring(0,1);
                    var ftmPwmCombChnVal = ftmPwmCombineHwChnId.getValue().substring(0,1);
                    var ftmPwmTempVal = ftmPwmCombChnVal++;

                    if ((ftmPwmIndtChnVal == ftmPwmCombChnVal) || (ftmPwmTempVal == ftmPwmIndtChnVal))
                    {
                        scriptApi.logError("The channel is used in another mode");
                    }
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }
        
        if (ftmPwmNumOfCombChn > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* The combined channels configuration structure for  " + peripheralName + "CombinedChannelsConfig */"+ NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "ftm_combined_ch_param_t " + peripheralName + "_CombinedChannelsConfig[" + ftmPwmNumOfCombChn + "] =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;

            for (var i = 0; i < ftmPwmNumOfCombChn; i++) 
            {
                var ftmPwmConfigName = ftmPwmCombChnDetail[i].getChildById("ftmPwmCombName");
                var ftmPwmInitName = peripheralName + "_CombinedChannelConfig" + i;
                ftmPwmConfigName.setValue(ftmPwmInitName);

                var ftmPwmHwChnId = ftmPwmCombChnDetail[i].getChildById("ftmPwmCombHwChnId");
                var ftmPwmFirstEdge = ftmPwmCombChnDetail[i].getChildById("firstEdge");
                var ftmPwmSecEdge = ftmPwmCombChnDetail[i].getChildById("secondEdge");
                var ftmPwmDeadtimeEn = ftmPwmCombChnDetail[i].getChildById("CombDeadTimeEn");
                var ftmPwmModifCombEn = ftmPwmCombChnDetail[i].getChildById("enableModifiedCombine");
                var ftmPwmMainChnPol = ftmPwmCombChnDetail[i].getChildById("mainChannelPolarity");
                var ftmPwmSecChnOutEn = ftmPwmCombChnDetail[i].getChildById("enCombSecondChannelOutput");
                var ftmPwmSecChnPol = ftmPwmCombChnDetail[i].getChildById("secondCombChannelPolarity");
                var ftmPwmExtTrigEn = ftmPwmCombChnDetail[i].getChildById("enCombExternalTrigger");
                var ftmPwmExtTrigNextChnEn = ftmPwmCombChnDetail[i].getChildById("enCombExternalTriggerOnNextChn");
                var ftmPwmMainChnSafeState = ftmPwmCombChnDetail[i].getChildById("mainChannelSafeState");
                var ftmPwmSecChnSafeState = ftmPwmCombChnDetail[i].getChildById("secondChannelSafeState");

                cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmHwChnId.getValue() + ", /* Hardware channel for channel (n) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmFirstEdge.getValue() + "U, /* First edge time */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecEdge.getValue() + "U, /* Second edge time */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmDeadtimeEn.getValue() + ", /* Dead time enabled/disabled */" + NEW_LINE;
                if (ftmPwmModifCombEn.isAvailable())
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmModifCombEn.getValue() + ", /* The modified combine mode enabled/disabled */" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmMainChnPol.getValue() + ", /* Select channel (n) polarity */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecChnOutEn.getValue() + ", /* Enabled/disabled the channel (n+1) output */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecChnPol.getValue() + ", /* Select channel (n+1) output relative to channel (n) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmExtTrigEn.getValue() + ", /* External Trigger on the channel (n) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmExtTrigNextChnEn.getValue() + ", /* External Trigger on the channel (n+1) */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmMainChnSafeState.getValue() + ", /* Safe state of the PWM channel n when faults are detected */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "        " + ftmPwmSecChnSafeState.getValue() + ", /* Safe state of the PWM channel n+1 when faults are detected */" + NEW_LINE;
                
                if (i == (ftmPwmNumOfCombChn-1))
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }" + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                }
            }
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
        }

        cFileSections[GLOBAL_VARS_SECTION] += "\n/* PWM configuration for " + peripheralName + " */"+ NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "ftm_pwm_param_t " + peripheralName + "_PwmConfig =" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmNumOfIndptChn + "U, /* Number of independent PWM channels */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmNumOfCombChn + "U, /* Number of combined PWM channels */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGlobalConfig.getChildById("ftmMode").getValue() + ", /* PWM mode */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGeneralConfig.getChildById("deadTimeValue").getValue() + "U, /* Dead time value */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGeneralConfig.getChildById("deadTimePrescaler").getValue() + ", /* Dead time prescale */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "    " + ftmPwmGeneralConfig.getChildById("periodInFreq").getValue() + "U, /* PWM frequency */" + NEW_LINE;
        if (ftmPwmNumOfIndptChn > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    " + peripheralName + "_IndependentChannelsConfig, /* The independent PWM channels configuration structure */" + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    NULL, /* The independent PWM channels configuration structure */" + NEW_LINE;
        }
        if (ftmPwmNumOfCombChn > 0)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    " + peripheralName + "_CombinedChannelsConfig, /* Combined PWM channels configuration structure */" + NEW_LINE;
        }
        else
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    NULL, /* Combined PWM channels configuration structure */" + NEW_LINE;
        }

        if (ftmPwmFaultConfig.available)
        {
            cFileSections[GLOBAL_VARS_SECTION] += "    &" + peripheralName + "_FaultConfig /* PWM fault configuration structure */" + NEW_LINE;
        }


        cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
    }
}