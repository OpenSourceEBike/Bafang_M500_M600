/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */

/////////////////// helper functions ///////////////////////////////////

var OutputUtils = {
  /* Gets introduction comment for whole configuration. "Configuration <configuration_name>" is centered and covered by "*"
   * cfg - reference to configuration object
   * return value - the introduction comment 
   */
  getConfigurationIntro: function(cfg) {
    var lineLength = 80;
    var confStr = " Configuration " + cfg.getName() + " ";
    var midLine = " ";
    var lines = [
    "/*******************************************************************************",
    ];
    //" ********************* Configuration " + cfg.getName() + "*********************",
    lineLength += 1;
    if ((confStr.length + midLine.length) < lineLength) {
      var numberOfStars = (lineLength - confStr.length - midLine.length) >>> 1;
      midLine = midLine.concat(Array(numberOfStars).join("*"))
    }
    midLine += confStr;
    if (midLine.length < lineLength) {
      midLine = midLine.concat(Array(lineLength - midLine.length).join("*"))
    }
    lines = lines.concat(midLine);
    lines = lines.concat([
    " ******************************************************************************/",
    ]);
    return (lines);
  },
  
  /* Ges introduction comment for definitions.
   * cfg - reference to configuration object
   * return value - the comment as array of strings 
   */
  getDefinitionIntro: function(cfg) {
    var result = [
    "/*******************************************************************************"];
    if (cfg == null) {
      result = result.concat([" * Definitions"]);
    }
    else {
      result = result.concat([" * Definitions for " + cfg.getName() + " configuration"]);
    }
    result = result.concat([" ******************************************************************************/"]);
    return result;
  },
  
  /* Gets introduction comment for vaiables.
   * cfg - reference to configuration object
   * return value - the comment as array of strings 
   */
  getVariablesIntro: function(cfg) {
    var result = [
    "/*******************************************************************************"];
    if (cfg == null) {
      result = result.concat([" * Variables"]);
    }
    else {
      result = result.concat([" * Variables for " + cfg.getName() + " configuration"]);
    }
    result = result.concat([" ******************************************************************************/"]);
    return result;
  },
  
  /* Gets introduction comment for api.
   * cfg - reference to configuration object
   * return value - the comment as array of strings 
   */
  getApiIntro: function(cfg) {
    var result = [
    "/*******************************************************************************"];
    if (cfg == null) {
      result = result.concat([" * API"]);
    }
    else {
      result = result.concat([" * API for " + cfg.getName() + " configuration"]);
    }
    result = result.concat([" ******************************************************************************/"]);
    return result;
  },
  
  //"/*! @brief Clock configuration structure. */",
  //"typedef struct _clock_config",
  //"{",
  //"    mcg_config_t mcgConfig;       /*!< MCG configuration.      */",
  //"    sim_clock_config_t simConfig; /*!< SIM configuration.      */",
  //"    osc_config_t oscConfig;       /*!< OSC configuration.      */",
  //"    uint32_t coreClock;           /*!< core clock frequency.   */",
  //"} clock_config_t;",
  //"",
  
  /* Gets introduction comment for code.
   * cfg - reference to configuration object
   * return value - no data 
   */
  getCodeIntro: function(cfg) {
    var result = [
    "/*******************************************************************************"];
    if (cfg == null) {
      result = result.concat([" * Code"]);
    }
    else {
      result = result.concat([" * Code for " + cfg.getName() + " configuration"]);
    }
    result = result.concat([" ******************************************************************************/"]);
    return result;
  },
  
  
  /* Formats a value.
   * val - value which should be formatted
   * options - determines format of result number, e.g. "hex" or "unsigned"
   * return value - formatted value 
   */
  formatField: function(val, options) {
    if (options == null) {
      return val;
    }
    var opt = {};
    for (var i = 0; i < options.length; i++) {
      opt[options[i]] = true;
    }
    var tmp = "";
    if (opt["hex"]) {
      tmp = "0x" + BigNumber.toString(val, 16); 
    } else {
      tmp = BigNumber.toString(val, 10);
    }
    if (opt["unsigned"]) {
      tmp += "U";
    } 
    return tmp;
  },
  
  /* Gets fieldName mapping (value) for requested fieldID. If a number is returned, format options can be specified.
   * If a define is created, symbolic name is returned according to settings in SDKMapper.mapper.<id> and parameter defineSymbolName.
   * cfg - reference to configuration object
   * fieldID - ID of item which should be mapped to number or enumaration or define symbol based on a setting, see SDKMapper.mapping
   * defineSymbolName - if null then no define is added into container specified by setCWriterDefineContainer
   *                  - if "" then define is added into the container named according to data in "mapping" part of this SDKMapper object and the alternative symbol is returned as .value
   *                  - if "<not_empty_string>" then define is added into the container named according to this parameter and the alternative symbol is returned as .value                   
   * formatOptions - determines format of result number or format number in created #define, e.g. ["hex"] or ["hex","unsigned"] or null if returned FieldVal.value is a number; formatField function is used for the formatting
   * return value - object FieldVal 
   */
  getField: function(cfg, fieldID, defineSymbolName, formatOptions) {
    var tmp = SDKMapper.getFieldValue(cfg, fieldID, defineSymbolName, formatOptions);
    if (tmp == null || tmp.value == null) {
      tmp = {"value": "N/A", "description": "<<=  <<=  <<=  <<=  <<="};
    }
    if (formatOptions != null && !isNaN(tmp.value)) {
      // pass through formatter function
      tmp.value = this.formatField(tmp.value, formatOptions);
    }
    return tmp;
  },

  /* Writes line with initialization structure item ".<fieldName> = <mapping value for requested fieldID>,".
   * cfg - reference to configuration object
   * fieldID - ID of item which should be mapped to number or enumaration or define symbol based on a setting or bit field
   * fieldName - name of item in a structure
   * isLast - determines if there should be "," or not at the end of expression
   * formatOptions - determines format of result number, e.g. "hex" or "unsigned", see parameter options for function formatField
   * return value - no data 
   */
  writeStructItemInitLine: function(cfg, indentLevel, fieldID, fieldName, isLast, formatOptions) {
    var tmp = this.getField(cfg, fieldID, "", formatOptions);
    if (fieldName == null || fieldName == "") {
      var splitFieldID = fieldID.split(".");
      fieldName = splitFieldID[splitFieldID.length - 1];
    }
    CWriter.line(Array(indentLevel).join(" ") + "." + fieldName + " = " + tmp.value + (isLast ? "" : ","), tmp.description);
  },

  /* This function is the same as getField function but this function returns directly FieldVal.value (converted to string if a number)
   */
  getFieldValue: function(cfg, fieldID, defineSymbolName, formatOptions) {
    var tmp = this.getField(cfg, fieldID, defineSymbolName, formatOptions);
    return BigNumber.toString(tmp.value);
  },

  /* Gets fieldName mapping value and optionally create define for requested fieldID. The define creation is intended for H file
   * At this moment this function is not used.
   * cfg - reference to configuration object
   * fieldID - ID of item which should be mapped to number or enumaration or define symbol based on a setting or bit field
   * defineSymbolName - this symbolic value is returned and given #define generated under container defineBlock; if "" then symbol from SDK mapper for appropriate   
   * defineBlock - instance of define block object   
   * formatOptions - determines format of result number or format number in created #define, e.g. ["hex"] or ["hex","unsigned"] or null if returned FieldVal.value is a number; formatField function is used for the formatting
   * return value - object FieldVal 
   */
  getFieldValueAndCreateDefine: function(cfg, fieldID, defineSymbolName, defineBlock, formatOptions) {
    if (defineSymbolName == null || defineSymbolName == "") {
      var tmp = this.getField(cfg, fieldID, defineSymbolName, formatOptions);
      return tmp.value;
    }
    else {
      var tmp = this.getField(cfg, fieldID, null, formatOptions);
      defineBlock.addDefine(defineSymbolName, tmp.number, tmp.description, formatOptions);
      return defineSymbolName;
    }
  },
  
} // OutputUtils object prototype




