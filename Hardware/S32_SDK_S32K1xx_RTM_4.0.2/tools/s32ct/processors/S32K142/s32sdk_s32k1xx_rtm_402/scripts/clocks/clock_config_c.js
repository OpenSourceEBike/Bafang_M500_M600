/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */

/**
 * Code generation script producing clock_config.c
 *
 */

// SDK header includes
var sdkIncludes = {
  //#include fsl_common.h moved into h file
  //fsl_common:   {code: "#include \"fsl_common.h\"",   condition: true},
  fsl_smc:      {name: "smc", code: "#include \"fsl_smc.h\"",      configurations: new Array(),   condition: false},
  clock_config: {name: "",    code: "#include \"clock_config.h\"", configurations: "",            condition: true},
}

/**
 * Writes How to setup comment block
 *
 */
function writeHowToSetupComment() {
  CWriter.writeLines([
  "/*",
  " * How to setup clock using clock driver functions:",
  " *",
  " * 1. Call CLOCK_InitXXX() to configure corresponding SCG clock source.",
  " *    Note: The clock could not be set when it is being used as system clock.",
  " *    In default out of reset, the CPU is clocked from FIRC(IRC48M),",
  " *    so before setting FIRC, change to use another available clock source.",
  " *",
  " * 2. Call CLOCK_SetXtal0Freq() to set XTAL0 frequency based on board settings.",
  " *",
  " * 3. Call CLOCK_SetXxxModeSysClkConfig() to set SCG mode for Xxx run mode.",
  " *    Wait until the system clock source is changed to target source.",
  " *",
  " * 4. If power mode change is needed, call SMC_SetPowerModeProtection() to allow",
  " *    corresponding power mode and SMC_SetPowerModeXxx() to change to Xxx mode.",
  " *    Supported run mode and clock restrictions could be found in Reference Manual.",
  " */"]);
}

function writeMisraViolations()
{
  CWriter.writeLines([
"/**",
"* @page misra_violations MISRA-C:2012 violations",
"*",
"* @section [global]",
"* Violates MISRA 2012 Required Rule 9.4, Duplicate initialization of object element.",
"* It's the only way to initialize an array that is member of struct.",
"*",
"* @section [global]",
"* Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.",
"* The external variables will be used in other source files in application code.",
"*/"]);
}

// get function called from BOARD_InitBootClocks() function and create BOARD_InitBootClocks() function body
function getInitBootClocksFunctions() {
  var initFunctions = new Array();
  initFunctions.push('/*******************************************************************************');
  initFunctions.push(' ************************ BOARD_InitBootClocks function ************************');
  initFunctions.push(' ******************************************************************************/');

  initFunctions.push('void BOARD_InitBootClocks(void)');
  initFunctions.push('{');

  for (var cfgIndex = 0; cfgIndex < Gen.configs.length; cfgIndex++) {
    var cfg = Gen.configs[cfgIndex];
    if (cfg.isCalledFromDefaultInit()) {
      initFunctions.push('    ' + Gen.getConfigID(cfg) + '();');
    }
  }

  initFunctions.push('}');
  initFunctions.push('');
  return initFunctions;
}

/**
 * Generate code which calls sdk function for configuration of given peripheral clock selector
 *
 * return value - string lines in array
 */
function generateSetPeripheralClock(cfg, elementId, bitFieldIds, functionName, comment) {
  if (HwAbstr.clockElementExist(cfg, elementId)) {
    if (HwAbstr.isClockElementUsed(cfg, elementId)) {
      CWriter.line("    /* " + comment + " */");
      var code = "    " + functionName + "(";
      var parameters = new Array();
      for (elementIndex = 0; elementIndex < bitFieldIds.length; elementIndex++) {
        parameters.push(OutputUtils.getFieldValue(cfg, bitFieldIds[elementIndex], ""));
      }
      code += parameters.join(", ") + ");";
      CWriter.line(code);
    }
  }
}

function countPccClocks(cfg){
    var totalPccClocks=0;
for (elementId in HwAbstr.PccClockDesciption){
    if (HwAbstr.clockElementExist(cfg, elementId)) {
        if (HwAbstr.isClockElementUsed(cfg, elementId)) {
totalPccClocks++;
}
}
}
return totalPccClocks;
}
/**
 * Generate code which calls sdk function for configuration of given peripheral clock in PPCx
 *
 * return value - string lines in array
 */
function generateSetPeripheralClockPCC(cfg, cfgIndex) {
  var elementId, comment="";
  CWriter.writeLines([
    "/* *************************************************************************",
 "* Configuration structure for peripheral clock configuration "+cfgIndex,
 "* ************************************************************************* */",
"/*! @brief peripheral clock configuration "+cfgIndex+" */",
"peripheral_clock_config_t peripheralClockConfig"+cfgIndex+"[NUM_OF_PERIPHERAL_CLOCKS_" + cfgIndex + "] = {"]);
for (elementId in HwAbstr.PccClockDesciption){
    if (HwAbstr.clockElementExist(cfg, elementId)) {
        if (HwAbstr.isClockElementUsed(cfg, elementId)) {
            CWriter.line("    {");
            CWriter.line("        .clockName = " + HwAbstr.PccClockDesciption[elementId][0] + ",");
            CWriter.line("        .clkGate = true,");
            var regPCC = HwAbstr.PccClockDesciption[elementId][1];
            var map = HwAbstr.PccClockIpSrc;
            var numClkSrc = "";
            var map2 = HwAbstr.PccClockIpFrac;
            var numClkFrac = map2.keyFunc(cfg, regPCC);
            var map3 = HwAbstr.PccClockIpDiv;
            var numClkDiv = map3.keyFunc(cfg, regPCC);

            if (regPCC.indexOf("PORT") == -1 && regPCC.indexOf("EWM") == -1 && regPCC.indexOf("CMP") == -1 && regPCC.indexOf("CRC") == -1 && regPCC.indexOf("DMA") == -1 && regPCC.indexOf("FTFC") == -1 && regPCC.indexOf("FlexCAN") == -1 && regPCC.indexOf("PDB") == -1 && regPCC.indexOf("RTC") == -1)
            {
                switch (cfg.getBitFieldValueAsBigInteger(regPCC,"PCS"))
                {
                    case 1:
                        numClkSrc = (regPCC.indexOf("FTM") == -1) ? "CLK_SRC_SOSC_DIV2" : "CLK_SRC_SOSC_DIV1";
                        break;
                    case 2:
                        numClkSrc = (regPCC.indexOf("FTM") == -1) ? "CLK_SRC_SIRC_DIV2" : "CLK_SRC_SIRC_DIV1";
                        break;
                    case 3:
                        numClkSrc = (regPCC.indexOf("FTM") == -1) ? "CLK_SRC_FIRC_DIV2" : "CLK_SRC_FIRC_DIV1";
                        break;
                    case 6:
                        numClkSrc = (regPCC.indexOf("FTM") == -1) ? "CLK_SRC_SPLL_DIV2" : "CLK_SRC_SPLL_DIV1";
                        break;
                    case 7:
                        numClkSrc = (regPCC.indexOf("FTM") == -1) ? "CLK_SRC_LPO" : "CLK_SRC_OFF";
                        break;
                    default:
                        numClkSrc = "CLK_SRC_OFF";
                        break;
                }
                CWriter.line("        .clkSrc = " + numClkSrc + ",");
            }
            else
            {
                CWriter.line("        .clkSrc = CLK_SRC_OFF,");
            }
            // CWriter.line("        .clkSrc = " + map.enumMap[numClkSrc][1] + ",");
            CWriter.line("        .frac = " + map2.enumMap[numClkFrac][1] + ",");
            CWriter.line("        .divider = " + map3.enumMap[numClkDiv][1] + ",");
            CWriter.line("    },");
        }
    }
}
CWriter.line("};");
}

function generateSetClockManagerConfig(cfg, cfgIndex) {
    var powerMode = HwAbstr.getPowerMode(cfg);
    CWriter.writeLines([
"/* *************************************************************************",
 "* Configuration structure for Clock Configuration "+cfgIndex+"",
 "* ************************************************************************* */",
"/*! @brief User Configuration structure clock_managerCfg_"+cfgIndex+" */",
"clock_manager_user_config_t clockMan1_InitConfig"+cfgIndex+" = {"]);
    CWriter.line("    .scgConfig =");
    CWriter.line("    {");
    CWriter.line("        .sircConfig =");
    CWriter.line("        {");
    if (cfg.getBitFieldValueAsBigInteger("SCG::SIRCCSR","SIRCEN") != 0)
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.enableInStop");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.enableInLowPower");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.locked");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.range");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.div1");
        OutputUtils.writeStructItemInitLine(cfg, 13, "sircConfig.div2");
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .fircConfig =");
    CWriter.line("        {");
    if (cfg.getBitFieldValueAsBigInteger("SCG::FIRCCSR","FIRCEN") != 0)
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "fircConfig.regulator");
        OutputUtils.writeStructItemInitLine(cfg, 13, "fircConfig.locked");
        // OutputUtils.writeStructItemInitLine(cfg, 13, "fircConfig.range");
        CWriter.line("            .range = SCG_FIRC_RANGE_48M,           /*!< RANGE      */");
        OutputUtils.writeStructItemInitLine(cfg, 13, "fircConfig.div1");
        OutputUtils.writeStructItemInitLine(cfg, 13, "fircConfig.div2");
    }
    else
    {
         CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .rtcConfig =");
    CWriter.line("        {");
    if (cfg.getBitFieldValueAsBigInteger("RTC::CR","CPE") != 0 && cfg.getValueAsDecimal("RTC.RTC_CLKIN.outFreq") != 0)
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "rtcConfig.rtcClkInFreq");
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .soscConfig =");
    CWriter.line("        {");

    if (cfg.getBitFieldValueAsBigInteger("SCG::SOSCCSR","SOSCEN") != 0 && cfg.getValueAsDecimal("SCG.SOSC.outFreq") != 0)
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.freq");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.monitorMode");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.locked");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.extRef");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.gain");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.range");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.div1");
        OutputUtils.writeStructItemInitLine(cfg, 13, "soscConfig.div2");
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .spllConfig =");
    CWriter.line("        {");
    if (cfg.getBitFieldValueAsBigInteger("SCG::SPLLCSR","SPLLEN") != 0)
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.monitorMode");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.locked");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.prediv");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.mult");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.src");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.div1");
        OutputUtils.writeStructItemInitLine(cfg, 13, "spllConfig.div2");
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .clockOutConfig =");
    CWriter.line("        {");


    if (cfg.getValueAsText("CLKOUTConfig") == "yes")
    {
        CWriter.line("            .initialize = true,");
        OutputUtils.writeStructItemInitLine(cfg, 13, "scgClockOutConfig.source");
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }
    CWriter.line("        },");
    CWriter.line("        .clockModeConfig =");
    CWriter.line("        {");
    CWriter.line("            .initialize = true,");

    getRunModeConf(cfg,"rccrConfig");
    getRunModeConf(cfg,"vccrConfig");
    getRunModeConf(cfg,"hccrConfig");
    
    CWriter.line("        },");
    CWriter.line("    },");
    CWriter.line("    .pccConfig =");
    CWriter.line("    {");
    CWriter.line("        .peripheralClocks = peripheralClockConfig" + cfgIndex + ", /*!< Peripheral clock control configurations  */");
    CWriter.line("        .count = NUM_OF_PERIPHERAL_CLOCKS_" + cfgIndex +", /*!< Number of the peripheral clock control configurations  */");
    CWriter.line("    },");
    CWriter.line("    .simConfig =");
    CWriter.line("    {");
    CWriter.line("        .clockOutConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsText("CLKOUTConfig") == "yes")
    {
        CWriter.line("            .initialize = true, /*!< Initialize    */");
        OutputUtils.writeStructItemInitLine(cfg, 13, "clockOutConfig.enable");
        OutputUtils.writeStructItemInitLine(cfg, 13, "clockOutConfig.source");
        OutputUtils.writeStructItemInitLine(cfg, 13, "clockOutConfig.divider");
    }
    else
    {
        CWriter.line("            .initialize = false, /*!< Initialize    */");
    }
    CWriter.line("        },");
    CWriter.line("        .lpoClockConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsText("LPOInitialize") == "yes")
    {
        CWriter.line("            .initialize = true, /*!< Initialize    */");

        if (cfg.getBitFieldValueAsBigInteger("SIM::LPOCLKS","LPO1KCLKEN") != 0)
        {
            CWriter.line("            .enableLpo1k = true, /*!< LPO1KCLKEN    */");
        }
        else
        {
            CWriter.line("            .enableLpo1k = false, /*!< LPO1KCLKEN    */");
        }
        if (cfg.getBitFieldValueAsBigInteger("SIM::LPOCLKS","LPO32KCLKEN") != 0)
        {
            CWriter.line("            .enableLpo32k = true, /*!< LPO32KCLKEN   */");
        }
        else
        {
            CWriter.line("            .enableLpo32k = false, /*!< LPO32KCLKEN   */");
        }


        OutputUtils.writeStructItemInitLine(cfg, 13, "lpoClockConfig.sourceLpoClk");
        OutputUtils.writeStructItemInitLine(cfg, 13, "lpoClockConfig.sourceRtcClk");
    }
    else
    {
        CWriter.line("            .initialize = false, /*!< Initialize    */");
    }
    CWriter.line("        },");
    CWriter.line("        .platGateConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsText("SimInitialize") == "yes")
    {
        CWriter.line("            .initialize = true, /*!< Initialize    */");
        if (cfg.getBitFieldValueAsBigInteger("SIM::PLATCGC","CGCEIM") != 0)
        {
            CWriter.line("            .enableEim = true, /*!< CGCEIM        */");
        }
        else
        {
            CWriter.line("            .enableEim = false, /*!< CGCEIM        */");
        }
        if (cfg.getBitFieldValueAsBigInteger("SIM::PLATCGC","CGCERM") != 0)
        {
            CWriter.line("            .enableErm = true, /*!< CGCERM        */");
        }
        else
        {
            CWriter.line("            .enableErm = false, /*!< CGCERM        */");
        }
        if (cfg.getBitFieldValueAsBigInteger("SIM::PLATCGC","CGCDMA") != 0)
        {
            CWriter.line("            .enableDma = true, /*!< CGCDMA        */");
        }
        else
        {
            CWriter.line("            .enableDma = false, /*!< CGCDMA        */");
        }
        if (cfg.getBitFieldValueAsBigInteger("SIM::PLATCGC","CGCMPU") != 0)
        {
            CWriter.line("            .enableMpu = true, /*!< CGCMPU        */");
        }
        else
        {
            CWriter.line("            .enableMpu = false, /*!< CGCMPU        */");
        }
        if (cfg.getBitFieldValueAsBigInteger("SIM::PLATCGC","CGCMSCM") != 0)
        {
            CWriter.line("            .enableMscm = true, /*!< CGCMSCM       */");
        }
        else
        {
            CWriter.line("            .enableMscm = false, /*!< CGCMSCM       */");
        }
    }
    else
    {
        CWriter.line("            .initialize = false,");
    }

    CWriter.line("        },");
    CWriter.line("        .tclkConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsText("TCLKConfig") == "yes")
    {
        CWriter.line("            .initialize = true, /*!< Initialize    */");
        CWriter.line("            .tclkFreq[0] = " + cfg.getValueAsDecimal("external_clocks.TCLK0EXTREFOUT.outFreq") + "U, /*!< TCLK0         */");
        CWriter.line("            .tclkFreq[1] = " + cfg.getValueAsDecimal("external_clocks.TCLK1EXTREFOUT.outFreq") + "U, /*!< TCLK1         */");
        CWriter.line("            .tclkFreq[2] = " + cfg.getValueAsDecimal("external_clocks.TCLK2EXTREFOUT.outFreq") + "U, /*!< TCLK2         */");
        CWriter.line("            .extPinSrc[0] = " + cfg.getBitFieldValueAsBigInteger("SIM::FTMOPT0","FTM0CLKSEL") + "U, /*!< FTM0 ext pin source   */");
        CWriter.line("            .extPinSrc[1] = " + cfg.getBitFieldValueAsBigInteger("SIM::FTMOPT0","FTM1CLKSEL") + "U, /*!< FTM1 ext pin source   */");
        CWriter.line("            .extPinSrc[2] = " + cfg.getBitFieldValueAsBigInteger("SIM::FTMOPT0","FTM2CLKSEL") + "U, /*!< FTM2 ext pin source   */");
        CWriter.line("            .extPinSrc[3] = " + cfg.getBitFieldValueAsBigInteger("SIM::FTMOPT0","FTM3CLKSEL") + "U, /*!< FTM3 ext pin source   */");
    }
    else
    {
        CWriter.line("            .initialize = false, /*!< Initialize    */");
    }
    CWriter.line("        },");
    CWriter.line("        .traceClockConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsText("TRACEInitialize") == "yes")
    {
        CWriter.line("            .initialize = true, /*!< Initialize    */");
        if (cfg.getBitFieldValueAsBigInteger("SIM::CLKDIV4","TRACEDIVEN") != 0)
        {
            CWriter.line("            .divEnable = true, /*!< TRACEDIVEN    */");
        }
        else
        {
            CWriter.line("            .divEnable = false, /*!< TRACEDIVEN    */");
        }
        CWriter.line("            .source = CLOCK_TRACE_SRC_CORE_CLK, /*!< TRACECLK_SEL  */");

        CWriter.line("            .divider = " + cfg.getBitFieldValueAsBigInteger("SIM::CLKDIV4","TRACEDIV") + "U, /*!< TRACEDIV      */");
        if (cfg.getBitFieldValueAsBigInteger("SIM::CLKDIV4","TRACEFRAC") != 0)
        {
            CWriter.line("            .divFraction = true, /*!< TRACEFRAC     */");
        }
        else
        {
            CWriter.line("            .divFraction = false, /*!< TRACEFRAC     */");
        }
    }
    else
    {
        CWriter.line("            .initialize = false, /*!< Initialize    */");
    }
    CWriter.line("        },");
    CWriter.line("    },");
    CWriter.line("    .pmcConfig =");
    CWriter.line("    {");
    CWriter.line("        .lpoClockConfig =");
    CWriter.line("        {");
    if (cfg.getValueAsDecimal("PMC.LPO128k_CLK.outFreq") != 0)
    {
        if (cfg.getValueAsText("LPOInitialize") == "yes")
        {
            CWriter.line("        .initialize = true,  /*!< Initialize    */");
            if (cfg.getBitFieldValueAsBigInteger("PMC::REGSC","LPODIS") == 0)
            {
                CWriter.line("        .enable = true, /*!< Enable/disable LPO     */");
            }
            else
            {
                CWriter.line("        .enable = false, /*!< Enable/disable LPO     */");
            }
            CWriter.line("        .trimValue = " + cfg.getValueAsText("LPO_Trimming") + ", /*!< Trimming value for LPO */");
        }
        else
        {
            CWriter.line("         .initialize = false,  /*!< Initialize    */");
        }
    }
    else
    {
        CWriter.line("         .initialize = false,  /*!< Initialize    */");
    }
    CWriter.line("        },");
    CWriter.line("    },");
    CWriter.line("};");

}
function getDividerValue(mode,type)
{
    var str = "'" + mode + ":SCG." + type + ".scale', value: ";
    var temp;
    var re_val = "1";

    if (yaml.search(str) > 0)
    {
        temp = yaml.substr(yaml.search(str) + str.length,5);
        re_val = temp.substr(temp.indexOf("'") + 1, temp.lastIndexOf("'") - 1);
    }
    else
    {
        switch(mode)
        {
            case "HSRUN":
                switch (type)
                {
                    case "DIVCORE":
                        re_val = "1";
                        break;
                    case "DIVBUS":
                        re_val = "2";
                        break;
                    case "DIVSLOW":
                        re_val = "4";
                        break;
                }
                break;
            case "RUN":
                switch (type)
                {
                    case "DIVCORE":
                        re_val = "1";
                        break;
                    case "DIVBUS":
                        re_val = "1";
                        break;
                    case "DIVSLOW":
                        re_val = "2";
                        break;
                }
                break;
            case "VLPR":
                switch (type)
                {
                    case "DIVCORE":
                        re_val = "2";
                        break;
                    case "DIVBUS":
                        re_val = "1";
                        break;
                    case "DIVSLOW":
                        re_val = "4";
                        break;
                }
                break;
        }
    }

   return re_val;
}

function getRunModeConf(cfg, registerToConfig) {
    var divider = "1";
    var mode_cfg_space = 17;

    CWriter.line("            ."+registerToConfig+" =");
    CWriter.line("            {");
    switch (registerToConfig)
    {
        case "hccrConfig":

            if (yaml.lastIndexOf("'HSRUN:SCG.SCSSEL.sel', value: SCG.SPLL_CLK") > 0)
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_SYS_PLL,/* System PLL */");
            }
            else
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_FIRC,/*Fast FIRC */");
            }

            divider = getDividerValue("HSRUN", "DIVCORE");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divCore = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Core Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("HSRUN", "DIVBUS");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divBus = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Bus Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("HSRUN", "DIVSLOW");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divSlow = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Slow Clock Divider: divided by " + divider + " */");
            break;
        case "rccrConfig":

            if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SPLL_CLK") > 0)
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_SYS_PLL,/* System PLL */");
            }
            else if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SIRC") > 0)
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_SIRC,/* Slow SIRC */");
            }
            else if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SOSC") > 0)
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_SYS_OSC,/* SOSC */");
            }
            else
            {
                CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_FIRC,/* Fast FIRC */");
            }

            divider = getDividerValue("RUN", "DIVCORE");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divCore = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Core Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("RUN", "DIVBUS");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divBus = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Bus Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("RUN", "DIVSLOW");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divSlow = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Slow Clock Divider: divided by " + divider + " */");
            break;
        case "vccrConfig":

            CWriter.line(Array(mode_cfg_space).join(" ") + ".src = SCG_SYSTEM_CLOCK_SRC_SIRC,/* Slow SIRC */");
            divider = getDividerValue("VLPR", "DIVCORE");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divCore = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Core Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("VLPR", "DIVBUS");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divBus = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Bus Clock Divider: divided by " + divider + " */");
            divider = getDividerValue("VLPR", "DIVSLOW");
            CWriter.line(Array(mode_cfg_space).join(" ") + ".divSlow = SCG_SYSTEM_CLOCK_DIV_BY_" + divider + ",/* Slow Clock Divider: divided by " + divider + " */");
            break;
    }

    CWriter.line("            },");
}
function getRunSourceCock()
{
    var srcMode = 3;
    if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SPLL_CLK") > 0)
    {
        srcMode = 6;
    }
    else if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SIRC") > 0)
    {
        srcMode = 2;
    }
    else if (yaml.lastIndexOf("'RUN:SCG.SCSSEL.sel', value: SCG.SOSC") > 0)
    {
        srcMode = 1;
    }
    else
    {
        srcMode = 3;
    }

return srcMode;
}

function getHSRunSourceCock()
{
    var srcMode = 3;
    if (yaml.lastIndexOf("'HSRUN:SCG.SCSSEL.sel', value: SCG.SPLL_CLK") > 0)
    {
        srcMode = 6;
    }
    else
    {
        srcMode = 3;
    }

return srcMode;
}



/**
 * Generates one boot function for a given clock configuration
 *
 * @param cfg Clock configuration object
 */
function GenerateBootFunction(cfg) {
  CWriter.line("void " + Gen.getConfigID(cfg) + "(void)");
  CWriter.line("{");
  CWriter.line("    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,");
  CWriter.line("                    g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);");
  CWriter.line("    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);");
  CWriter.line("}");
  CWriter.line("");
}

function getStructs(length){
     var inits = new Array();
  inits.push("/*! @brief Array of pointers to User configuration structures */");
inits.push("clock_manager_user_config_t const * g_clockManConfigsArr[] = {");
for (var i = 0; i < length; i++) {
    inits.push("&clockMan1_InitConfig" + i + ((i<length-1) ? "," : ""));
    // (i<length-1) ? inits.push("&clock_InitConfig" + i + ",") : inits.push("&clock_InitConfig" + i);
}
inits.push("};");
inits.push("");
inits.push("/*! @brief Array of pointers to User defined Callbacks configuration structures */");
inits.push("/* The tool do not support generate Callbacks configuration. It's always empty. */");
inits.push("clock_manager_callback_user_config_t * g_clockManCallbacksArr[] = {(void*)0};");
inits.push("");

return inits;
}

function createDependency(module, component, coreID) {
  var dependency = {
    resourceType: "SWComponent",
    resourceId: "platform.drivers." + module,
    description: "Clocks initialization requires the " + module.toUpperCase() + " Driver in the project.",
    problem_level: 2,
    source: "Clocks." + component,
    features:
    [{ feature_name: "enabled",
        evaluation: "equal",
        configuration: coreID,
        data: [{
          value: "true",
          unit: null,
          min:  null,
          max: null
       }]
    }]
  }
  return dependency;
}
function checkRatioCoreClkHsrunToRun(cfg,currentMode)
{
    currentFreqMode = cfg.getValueAsDecimal("SCG.DIVCORE.outFreq");
    freqMode = 0;
    srcMode = 0;
    if (currentMode == "HSRUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::RCCR","SCS");

        srcMode = getRunSourceCock();
        switch (srcMode)
        {
            case 1:
                freqMode = cfg.getValueAsDecimal("SCG.SOSC.outFreq");
                break;
            case 2:
                freqMode = cfg.getValueAsDecimal("SCG.SIRC.outFreq");
                break;
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("RUN mode supports the SOSC, SIRC and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVCORE") + 1);
        freqMode = freqMode / (getDividerValue("RUN", "DIVCORE"));
        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for SYSTEM clock respectively must be less than or equal to 3.");
        }
    }
    else if (currentMode == "RUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::HCCR","SCS");
        srcMode = getHSRunSourceCock();

        switch (srcMode)
        {
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("HSRUN mode only supports the SPLL and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVCORE") + 1);
        freqMode = freqMode / (getDividerValue("HSRUN", "DIVCORE"));

        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for SYSTEM clock respectively must be less than or equal to 3.");
        }
    }
    else
    {
        /* Do nothing */
    }
}

function checkRatioBusClkHsrunToRun(cfg,currentMode)
{
    currentFreqMode = cfg.getValueAsDecimal("SCG.DIVBUS.outFreq");
    freqMode = 0;
    srcMode = 0;
    if (currentMode == "HSRUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::RCCR","SCS");
        srcMode = getRunSourceCock();

        switch (srcMode)
        {
            case 1:
                freqMode = cfg.getValueAsDecimal("SCG.SOSC.outFreq");
                break;
            case 2:
                freqMode = cfg.getValueAsDecimal("SCG.SIRC.outFreq");
                break;
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("RUN mode supports the SOSC, SIRC and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVCORE") + 1);
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVBUS") + 1);
        freqMode = freqMode / (getDividerValue("RUN", "DIVCORE"));
        freqMode = freqMode / (getDividerValue("RUN", "DIVBUS"));
        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for BUS clock respectively must be less than or equal to 3.");
        }
    }
    else if (currentMode == "RUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::HCCR","SCS");
        srcMode = getHSRunSourceCock();
        switch (srcMode)
        {
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("HSRUN mode only supports the SPLL and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVCORE") + 1);
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVBUS") + 1);
        freqMode = freqMode / (getDividerValue("HSRUN", "DIVCORE"));
        freqMode = freqMode / (getDividerValue("HSRUN", "DIVBUS"));

        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for BUS clock respectively must be less than or equal to 3.");
        }
    }
    else
    {
        /* Do nothing */
    }

}

function checkRatioFlashClkHsrunToRun(cfg,currentMode)
{
    currentFreqMode = cfg.getValueAsDecimal("SCG.DIVSLOW.outFreq");
    freqMode = 0;
    srcMode = 0;
    if (currentMode == "HSRUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::RCCR","SCS");
        srcMode = getRunSourceCock();
        switch (srcMode)
        {
            case 1:
                freqMode = cfg.getValueAsDecimal("SCG.SOSC.outFreq");
                break;
            case 2:
                freqMode = cfg.getValueAsDecimal("SCG.SIRC.outFreq");
                break;
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("RUN mode supports the SOSC, SIRC and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVCORE") + 1);
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVSLOW") + 1);
        freqMode = freqMode / (getDividerValue("RUN", "DIVCORE"));
        freqMode = freqMode / (getDividerValue("RUN", "DIVSLOW"));

        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for FLASH clock respectively must be less than or equal to 3.");
        }
    }
    else if (currentMode == "RUN")
    {
        // srcMode = cfg.getBitFieldValueAsBigInteger("SCG::HCCR","SCS");
        srcMode = getHSRunSourceCock();
        switch (srcMode)
        {
            case 3:
                freqMode = cfg.getValueAsDecimal("SCG.FIRC.outFreq");
                break;
            case 6:
                freqMode = cfg.getValueAsDecimal("SCG.SPLL_CLK.outFreq");
                break;
            default:
                scriptApi.logError("HSRUN mode only supports the SPLL and FIRC source.");
                break;
        }
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVCORE") + 1);
        // freqMode = freqMode / (cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVSLOW") + 1);
        freqMode = freqMode / (getDividerValue("HSRUN", "DIVCORE"));
        freqMode = freqMode / (getDividerValue("HSRUN", "DIVSLOW"));
        if (((currentFreqMode / freqMode) > 3) || ((currentFreqMode / freqMode) < (1/3)))
        {
            scriptApi.logError("The ratio of frequency between HSRUN mode and RUN mode for FLASH clock respectively must be less than or equal to 3.");
        }
    }
    else
    {
        /* Do nothing */
    }
}

function checkRatioClockHsrunToRun(cfg) {
    currentMode = cfg.getValueAsText("powerMode");
    checkRatioCoreClkHsrunToRun(cfg,currentMode);
    checkRatioBusClkHsrunToRun(cfg,currentMode);
    checkRatioFlashClkHsrunToRun(cfg,currentMode);
}
function pushDependency(dependencyHelper, dependency) {
  dependencyHelper.addDependency(dependency);
}

/////////////////// code generation  //////////////////////////////////

/*
// write header to the output
CWriter.write(CommonHeaderText());
CWriter.write("");
*/
// write How to setup comment
// writeHowToSetupComment();
writeMisraViolations();
CWriter.write("");
// write global Yaml block
CWriter.write(Gen.profile.getYaml());
CWriter.write("");
// write block of #includes
var optionalIncludesPosition = CWriter.line(null);
CWriter.write("");

// allocate place for block of #defines
var defineBlockPosition = CWriter.line(null);
// create block instance of #defines
var defineBlockC = new DefineBlock();
// SDKMapper.setCWriterDefineContainer = defineBlockC;
var defineBlockLinesC = OutputUtils.getDefinitionIntro();

//write common variables introduction
CWriter.writeLines(OutputUtils.getVariablesIntro());
// CWriter.writeLines([
// "/* System clock frequency. */",
// "extern uint32_t SystemCoreClock;",
// ]);
CWriter.write("");

// allocate place for FllStableDelay, InitRtc,... functions
var internalFunctionHeaderPosition = CWriter.line(null);
var internalFunctionPosition = CWriter.line(null);
var fircSafeConfigFunctionReq = false;
var fircSafeConfigFunctionReq = false;
var getClkOutSelectFunctionReq = false;
var getScgClkOutSelFunctionReq = false;
var getRtc32KClkSrcFunctionReq = false;
var getCanFltClkSelFunctionReq = false;
var getSetTraceClockFunctionReq = false;

for (var cfgIndex = 0; cfgIndex < Gen.configs.length; cfgIndex++) {
  var cfg = Gen.configs[cfgIndex];
  // write configuration introduction
  // CWriter.writeLines(OutputUtils.getConfigurationIntro(cfg));
  // write Yaml block
  CWriter.write(cfg.getYaml());
  yaml =cfg.getYaml();

  CWriter.line("");

  /* Set peripherals clock which are set in PCC */
  generateSetPeripheralClockPCC(cfg, cfgIndex);

  generateSetClockManagerConfig(cfg, cfgIndex);
  checkRatioClockHsrunToRun(cfg);
  // write code introduction
  // CWriter.writeLines(OutputUtils.getCodeIntro(cfg));

}
CWriter.line("");
  // write config array
  CWriter.writeLines(getStructs(Gen.configs.length));
CWriter.line("");

// for (var cfgIndex = 0; cfgIndex < Gen.configs.length; cfgIndex++) {
//   var cfg = Gen.configs[cfgIndex];
//    // write boot function
//   GenerateBootFunction(cfg);
// }

// write clock initialization functions
 // CWriter.writeLines(getInitBootClocksFunctions());

// write "CODE" header if an internal function is generated
if (!CWriter.isPositionEmpty(internalFunctionPosition)) {
  CWriter.writeLines(OutputUtils.getCodeIntro(), internalFunctionHeaderPosition);
}

// complete defines block and write it
defineBlockLinesC = defineBlockLinesC.concat(defineBlockC.getDefineBlock()).concat("");
CWriter.writeLines(defineBlockLinesC, defineBlockPosition);

// generate #includes and register its dependencies
var sdkIncludesKeys = Object.keys(sdkIncludes);

var dependencyHelper = scriptApi.getDependencyHelper();
var coreListTxt = scriptApi.getProfile().getMcuInfo().getCoresList();
var coreList = JSON.parse(coreListTxt);
var coreIds = Object.keys(coreList);

// FIXME AF: check if this code is needed when SDK is available
/*for (var coreId = 0; coreId < coreIds.length; coreId++) {
  for(var configID = 0; configID < Gen.configs.length; configID++) {
    pushDependency(dependencyHelper, createDependency("common", Gen.getConfigID(Gen.configs[configID]), coreIds[coreId]));
  }
}*/

for (includeIndex = 0; includeIndex < sdkIncludesKeys.length; includeIndex++) {
  var includeItem = sdkIncludes[sdkIncludesKeys[includeIndex]];
  if (includeItem["condition"]) {
    CWriter.write(includeItem["code"], optionalIncludesPosition);
    if (includeItem["name"] != "") {
      for (var coreId = 0; coreId < coreIds.length; coreId++) {
        for(var configID = 0; configID < includeItem["configurations"].length; configID ++) {
          pushDependency(dependencyHelper, createDependency(includeItem["name"], includeItem["configurations"][configID], coreIds[coreId]));
        }
      }
    }
  }
}
