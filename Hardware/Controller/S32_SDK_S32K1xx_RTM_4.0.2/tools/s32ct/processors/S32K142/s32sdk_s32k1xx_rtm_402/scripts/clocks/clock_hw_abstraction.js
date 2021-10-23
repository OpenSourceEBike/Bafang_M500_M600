/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */

var HwAbstr = {

  /**
   * Returns Mcu series
   *
   * @param gen Clock code generation object
   * @return Mcu series, e.g. "Kinetis L"
   */
  getMcuFamily: function(gen) {
    return gen.profile.getMcuInfo().getSeries();
  },

  /**
   * Returns currently set Power mode for the configuration
   *
   * @param cfg Current configuration.
   * @return Power mode setting
   */
  getPowerMode: function(cfg) {
    // return Power mode for a given configuration
    return cfg.getValueAsText("powerMode");
  },

  /**
   * Returns currently set SCG mode for the configuration
   *
   * @param cfg Current configuration.
   * @return SCG mode setting
   */
  getSCGMode: function(cfg) {
    var powerMode = this.getPowerMode(cfg);
    // return MCG mode for a given configuration
    if (powerMode == "STOP" || powerMode == "VLPS" || powerMode == "LLS" || powerMode == "VLLS0" || powerMode == "VLLS1" || powerMode == "VLLS2" || powerMode == "VLLS3" || powerMode == "BAT") {
      return "N/A";
    }
    else {
      return cfg.getBitFieldValueAsBigInteger("SCG::VCCR","SCS").toString();
    }
  },

  /* Checks all configurations on general settings and reports errors, warnings and info.
   * Typically external oscillator settings related to balast connected to the processor.
   * E.g. settings of capacitors should be the same for all configurations.
   * return value - no data
   */
  checkGeneralSettings: function() {

    var theSame = HwAbstr.isSettingSameInAllConfigurations("SCG.SOSC_CLK.outFreq", "sosc", "asInteger");
    if (theSame != null && !theSame) {
      scriptApi.logWarning("System OSC frequency is not the same in all configurations in which the oscillator is used. Constant BOARD_XTAL0_CLK_HZ cannot be defined.");
    }
    var theSame = HwAbstr.isSettingSameInAllConfigurations("SCG_SOSCCFG_SYS_CAP_LOAD_CFG", "sosc");
    if (theSame != null && !theSame) {
      scriptApi.logWarning("OSC capacitors load is not the same in all configurations in which the oscillator is used.");
    }
  },


  /* Checks all configurations and reports errors, warnings and info.
   * return value - no data
   */
  checkConfigurations: function() {
    for (var i = 0; i < Gen.configs.length; i++) {
      var tempCfg = Gen.configs[i];
      var powerMode = this.getPowerMode(tempCfg);
      var scgMode = this.getSCGMode(tempCfg);
      // SOSC or SIRC are used for VLPR and VLPW
      if ((powerMode == "VLPR" || powerMode == "VLPW") && scgMode != "2") {
        scriptApi.logError(tempCfg.getName() + " configuration: Entry to " + powerMode + " power mode is not allowed in other source except SIRC.");
      }
    }
  },


  /* Checks if the setting is the same in all configurations where the setting exists and valid.
   * The validity is determined by second parameter
   * Parameter settingId - clock setting
   * Parameter enableId - the setting of each configuration is checked only if this element is enabled
   * Parameter getMethod - asInteger - getValueAsInteger function is used; otherwise getValueAsText is used
   * return - true/false/null
   */
  isSettingSameInAllConfigurations: function(settingId, enableId, getMethod) {
    var value = null;
    var result = true;
    for (var cfgIndex = 0; cfgIndex < Gen.configs.length; cfgIndex++) {
      var tempCfg = Gen.configs[cfgIndex];
      if (enableId == null || (HwAbstr.clockElementExist(tempCfg, enableId) && HwAbstr.isClockElementUsed(tempCfg, enableId))) {
        var setting = tempCfg.getValueAsText(settingId);
        if (setting != null) {
          if (getMethod == "asInteger") {
            setting = tempCfg.getValueAsInteger(settingId);
          }
          if (value == null) {
            value = setting;
          }
          else {
            if (value != setting) {
              result = false;
            }
          }
        }
      }
    }
    if (value == null) {
      return null;
    }
    return result;
  },


  /* Get array of setting values for all configurations where the setting exists and valid.
   * The validity is determined by second parameter
   * Parameter settingId - clock setting
   * Parameter enableId - the setting of each configuration is checked only if this element is enabled
   * Parameter getMethod - asInteger - getValueAsInteger function is used; otherwise getValueAsText is used
   * return - array of values
   */
  getSettingValuesForAllConfigurations: function(settingId, enableId, getMethod) {
    var result = new Array();
    for (var cfgIndex = 0; cfgIndex < Gen.configs.length; cfgIndex++) {
      var tempCfg = Gen.configs[cfgIndex];
      if (enableId == null || (HwAbstr.clockElementExist(tempCfg, enableId) && HwAbstr.isClockElementUsed(tempCfg, enableId))) {
        var setting = tempCfg.getValueAsText(settingId);
        if (setting != null) {
          if (getMethod == "asInteger") {
            setting = tempCfg.getValueAsInteger(settingId);
          }
          if (result.indexOf(setting) == -1) {
            result.push(setting);
          }
        }
      }
    }
    return result;
  },

  /**
   * Returns whether LPFLL Trim is enabled
   *
   * @param cfg Clock configuration object
   * return value - true or false
   */
  isLpFllTrimEnabled: function(cfg) {
    return (cfg.getBitFieldValueAsBigInteger("SCG::LPFLLCSR","LPFLLTREN") == 1) ? true : false;
  },

  /**
   * Returns Firc trim mode selection
   *
   * @param cfg Clock configuration object
   * return value - trim selection
   */
  getFircTrimModeSelection: function(cfg) {
    return cfg.getValueAsText("SCG_FIRCCSR_TRIM_CFG");
  },

 /**
   * Returns whether SIM has clock output pin
   *
   * @param cfg Clock configuration object
   * return value - true or false
   */
  hasClkOutputPin: function() {
    return HwAbstr.clockElementExist(Gen.configs[0], "clkoutClock")
  },

  /**
   * Returns bit-field value
   *
   * @param cfg Clock configuration object
   * @param bitFieldId Bit-field id, see this.bitFields
   * return Bit-field value
   */
  getBitFieldValue: function(cfg, bitFieldId) {
    var bitField = this.bitFields[bitFieldId];
    if (bitField == null) {
      scriptApi.logError("[DEBUG] Function HwAbstr.getBitFieldValue() doesn't support bit-field " + bitField);
      return -1;
    }
    var value = cfg.getBitFieldValueAsBigInteger(bitField[0], bitField[1]);
    if (value == null) {
      scriptApi.logError("[DEBUG] Unknown value for " + bitField[0] + "[" + bitField[1] + "]");
    }
    return (value);
  },

  // Object which access element/component ids. It is used in functions clockElementExist and isClockElementUsed
  clockIds: {
      "scgPll" : "SCG.SPLL",
      "scgFirc" : "SCG.FIRC",
      "scgFircDiv1" : "SCG.FIRCDIV1",
      "scgDivBus" : "SCG.DIVBUS",
      "scgSirc" : "SCG.SIRC",
      "scgSircDiv1" : "SCG.SIRCDIV1",
      "scgLpFll" : "SCG.LPFLL",
      "scgClkOut" : "SCG.SCGCLKOUT_CLK",
      "scgSoscDiv1" : "SCG.SOSCDIV1",
      "sosc" : "SCG.SOSC",
      "soscErClk" : "SCG.SOSCER_CLK",

      // Clocks IDs for PCC
      "pccADC0Clk" : "ADC0_CLK",
      "pccADC1Clk" : "ADC1_CLK",
      "pccLPSPI0Clk" : "LPSPI0_CLK",
      "pccLPSPI1Clk" : "LPSPI1_CLK",
      "pccLPUART0Clk" :"LPUART0_CLK",
      "pccLPUART1Clk" :"LPUART1_CLK",
      "pccLPI2C0Clk" : "LPI2C0_CLK",
      "pccLPITClk" : "LPIT0_CLK",
      "pccLPTMR0Clk" : "LPTMR0_CLK",
      "pccFTM0Clk" :"FTM0_CLK",
      "pccFTM1Clk" :"FTM1_CLK",
      "pccFTM2Clk" : "FTM2_CLK",
      "pccFTM3Clk" : "FTM3_CLK",
      "pccFlexIOClk" : "FlexIO0_CLK",
      "pccCMP0Clk" : "CMP0_CLK",
      "pccCRC0Clk" : "CRC0_CLK",
      "pccDMAMUX0Clk" : "DMAMUX0_CLK",
      "pccEWM0Clk" : "EWM0_CLK",
      "pccFTFC0Clk" : "FTFC0_CLK",
      "pccPDB0Clk" : "PDB0_CLK",
      "pccPDB1Clk" : "PDB1_CLK",
      "pccRTC0Clk" : "RTC0_CLK",
      "pccFlexCAN0Clk" : "FLEXCAN0_CLK",
      "pccFlexCAN1Clk" : "FLEXCAN1_CLK",
      "pccFlexCAN2Clk" : "FLEXCAN2_CLK",
      "pccPORTAClk" : "PORTA_CLK",
      "pccPORTBClk" : "PORTB_CLK",
      "pccPORTCClk" : "PORTC_CLK",
      "pccPORTDClk" : "PORTD_CLK",
      "pccPORTEClk" : "PORTE_CLK"
  },

  /**
   * Returns whether a clock element/component exists
   *
   * @param cfg Clock configuration object
   * @param id Setting id, see this.clockIds
   * return value - true or false
   */
  clockElementExist: function(cfg, id) {
    if (cfg == null) {
      scriptApi.logError("[DEBUG] Function HwAbstr.clockElementExist() doesn't get right configuration (" + id + ")");
    }
    var settingId = this.clockIds[id];
    if (settingId == null) {
      scriptApi.logError("[DEBUG] Function HwAbstr.clockElementExist() doesn't support setting " + id);
      return false;
    }
    //settingId +=  ".enable";
    return cfg.existsId(settingId);
  },


  /**
   * Returns whether a clock element is used
   *
   * @param cfg Clock configuration object
   * @param id Setting id, see this.clockIds
   * return value - true or false
   */
  isClockElementUsed: function(cfg, id) {
    var settingId = this.clockIds[id];
    if (settingId == null) {
      scriptApi.logError("[DEBUG] Function HwAbstr.isClockElementUsed() doesn't support setting " + id);
      return false;
    }
    settingId +=  ".enable";
    var setting = cfg.getValueAsText(settingId);
    if (setting == null) {
      scriptApi.logError("[DEBUG] Unknown id: " + settingId);
      return false;
    }
    return (setting == "true");
  },

  //@clock alias (same is used in clockIDs object):[@clock enum in SDK,@register name,@peripheral name in CM]
  PccClockDesciption: {
    "pccADC0Clk" :    ["ADC0_CLK","PCC::ADC0","ADC0"],
    "pccADC1Clk" :    ["ADC1_CLK","PCC::ADC1","ADC1"],
    "pccLPSPI0Clk" :    ["LPSPI0_CLK","PCC::LPSPI0","LPSPI0"],
    "pccLPSPI1Clk" :    ["LPSPI1_CLK","PCC::LPSPI1","LPSPI1"],
    "pccLPUART0Clk" :    ["LPUART0_CLK","PCC::LPUART0","LPUART0"],
    "pccLPUART1Clk" :    ["LPUART1_CLK","PCC::LPUART1","LPUART1"],
    "pccLPI2C0Clk" :    ["LPI2C0_CLK","PCC::LPI2C0","LPI2C0"],
    "pccLPITClk" :    ["LPIT0_CLK","PCC::LPIT","LPIT"],
    "pccLPTMR0Clk" :    ["LPTMR0_CLK","PCC::LPTMR0","LPTMR0"],
    "pccFTM0Clk" :    ["FTM0_CLK","PCC::FTM0","FTM0"],
    "pccFTM1Clk" :    ["FTM1_CLK","PCC::FTM1","FTM1"],
    "pccFTM2Clk" :    ["FTM2_CLK","PCC::FTM2","FTM2"],
    "pccFTM3Clk" :    ["FTM3_CLK","PCC::FTM3","FTM3"],
    "pccFlexIOClk" :    ["FLEXIO0_CLK","PCC::FlexIO","FlexIO"],
    "pccCMP0Clk" :    ["CMP0_CLK","PCC::CMP0","CMP0_CGC"],
    "pccCRC0Clk" :    ["CRC0_CLK","PCC::CRC","CRC_CGC"],
    "pccDMAMUX0Clk" :    ["DMAMUX0_CLK","PCC::DMAMUX","DMAMUX_CGC"],
    "pccEWM0Clk" :    ["EWM0_CLK","PCC::EWM","EWM_CGC"],
    "pccFTFC0Clk" :    ["FTFC0_CLK","PCC::FTFC","FTFC_CGC"],
    "pccPDB0Clk" :    ["PDB0_CLK","PCC::PDB0","PDB0"],
    "pccPDB1Clk" :    ["PDB1_CLK","PCC::PDB1","PDB1"],
    "pccRTC0Clk" :    ["RTC0_CLK","PCC::RTC0","RTC0"],
    "pccFlexCAN0Clk" :    ["FlexCAN0_CLK","PCC::FlexCAN0","FLEXCAN0"],
    "pccFlexCAN1Clk" :    ["FlexCAN1_CLK","PCC::FlexCAN1","FLEXCAN1"],
    "pccPORTAClk" :    ["PORTA_CLK","PCC::PORTA","PORTA_CGC"],
    "pccPORTBClk" :    ["PORTB_CLK","PCC::PORTB","PORTB_CGC"],
    "pccPORTCClk" :    ["PORTC_CLK","PCC::PORTC","PORTC_CGC"],
    "pccPORTDClk" :    ["PORTD_CLK","PCC::PORTD","PORTD_CGC"],
    "pccPORTEClk" :    ["PORTE_CLK","PCC::PORTE","PORTE_CGC"]
    },
    ///////////////////////// PCC for peripherals ///////////////////////////////////
    PccClockIpSrc: {
            keyFunc : (function (cfg,register) { return register.indexOf("PORT")!==-1?0:cfg.getBitFieldValueAsBigInteger(register,"PCS"); }),
            enumMap:[
                 [0, "CLK_SRC_OFF",   "Clock is off or an external clock"],
                 [1, "CLK_SRC_SOSC_DIV2",   "System Oscillator Platform clock (divided)"],
                 [2, "CLK_SRC_SIRC_DIV2",   "Slow IRC clock (divided)"],
                 [3, "CLK_SRC_FIRC_DIV2",   "Fast IRC clock (divided)"],
                 [6, "CLK_SRC_SPLL_DIV2",   "System PLL clock (divided)"],
                 [7, "CLK_SRC_LPO",   "LPO clock (divided)"]
                 ]
    },
    PccClockIpFrac: {
   keyFunc : (function (cfg,register) { return register.indexOf("LPTMR")==-1?0:cfg.getBitFieldValueAsBigInteger(register,"FRAC"); }),
   enumMap:[
        [0, "MULTIPLY_BY_ONE",  "Fractional value is zero"],
        [1, "MULTIPLY_BY_TWO",  "Fractional value is one"],
      ]
    },
    PccClockIpDiv: {
   keyFunc : (function (cfg,register) { return register.indexOf("LPTMR")==-1?0:cfg.getBitFieldValueAsBigInteger(register,"PCD"); }),
   enumMap:[
        [0, "DIVIDE_BY_ONE",  "Slow Clock Divider: divided by 1"],
        [1, "DIVIDE_BY_TWO",  "Slow Clock Divider: divided by 2"],
        [2, "DIVIDE_BY_THREE",  "Slow Clock Divider: divided by 3"],
        [3, "DIVIDE_BY_FOUR",  "Slow Clock Divider: divided by 4"],
        [4, "DIVIDE_BY_FIVE",  "Slow Clock Divider: divided by 5"],
        [5, "DIVIDE_BY_SIX",  "Slow Clock Divider: divided by 6"],
        [6, "DIVIDE_BY_SEVEN",  "Slow Clock Divider: divided by 7"],
        [7, "DIVIDE_BY_EIGTH",  "Slow Clock Divider: divided by 8"]
      ]
    }
} // HwAbstr object prototype


