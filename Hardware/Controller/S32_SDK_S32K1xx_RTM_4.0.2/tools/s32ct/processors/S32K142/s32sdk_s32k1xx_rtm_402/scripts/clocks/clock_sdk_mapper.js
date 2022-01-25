/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */

/**
 * Functions for making SDK structure field values from clock model state
 *
 */

// FieldVal object specifies object which is returned by SDKMapper.getFieldValue function.
var FieldVal = function (value, description, number) {
   this.value = value;
   this.description = description;
   this.number = number;
};

// SDKMapper object allows mapping a id to sdk enumeration symbol, a number or #define symbol (including "#define <symbol> ..." generation).
SDKMapper = {
      // Reference on define container in c file
      setCWriterDefineContainer: null,

      // Internal function of SDKMapper object. This function adds a #define into container specified by setCWriterDefineContainer
      addCDefine: function (defineSymbol, defineValue, defineComment, formatOptions) {
        if (this.setCWriterDefineContainer != null) {
          this.setCWriterDefineContainer.addDefine(defineSymbol, defineValue, defineComment, formatOptions);
        }
      },

      /* Gets value FieldVal (value, comment and number) for specified id. FieldVal.value can be formated according to parameter formatOptions.
       * Founded value (FieldVal.value) can be replaced by a symbolic constant according to defineSymbolname parameter.
       * cfg - reference to configuration object
       * fieldID - ID of item which should be mapped to number or enumaration or define symbol based on a setting
       * defineSymbolName - if null then no define is added into container specified by setCWriterDefineContainer
       *                  - if "" then define is added into the container named according to data in "mapping" part of this SDKMapper object and the alternative symbol is returned as .value
       *                  - if "<not_empty_string>" then define is added into the container named according to this parameter and the alternative symbol is returned as .value
       * formatOptions - determines format of result number in #define, e.g. ["hex"] or ["hex","unsigned"], see parameter options for function OutputUtils.formatField
       *                 it is ignored if defineSymbolName is null
       * return value - object FieldVal
       */
      getFieldValue: function (cfg, fieldID, defineSymbolName, formatOptions) {
         var map = this.mapping[fieldID];               //default reference to the ID mapping object
         if (map == null) {
           scriptApi.logError("[DEBUG] Unsupported field ID " + fieldID + ".");
           return null;
         }
         if (map[HwAbstr.getMcuFamily(Gen)] != null) {  //use the default reference if there is no Mcu series mapping object
           map = map[HwAbstr.getMcuFamily(Gen)];
         }
         if (formatOptions == null && map.formatOptions) {
           if ("formatOptions" in map) {
             formatOptions = map.formatOptions;
           }
         }
         switch (map.type) {
           case "enum":
             var kval = map.keyFunc(cfg);
             if (kval == null) {
               scriptApi.logError("[DEBUG] " + map.keyFunc.toString() + " returns null.");
               return null;
             }
             for (var i=0; i< map.enumMap.length; i++) {
               if (BigNumber.equal(map.enumMap[i][0], kval)) {
                 var ret = new FieldVal(map.enumMap[i][1], map.enumMap[i][2], map.enumMap[i][0]);
                 if (defineSymbolName != null) {
                   if (defineSymbolName == "") {
                     if (map.enumMap[i][3] != null && map.enumMap[i][3] != "") {
                       this.addCDefine(map.enumMap[i][1], map.enumMap[i][0], map.enumMap[i][2], formatOptions);
                     }
                   }
                   else {
                     this.addCDefine(defineSymbolName, map.enumMap[i][0], map.enumMap[i][2], formatOptions);
                     ret.value = defineSymbolName;
                   }
                 }
                 return ret;
               }
             }
             scriptApi.logError("[DEBUG] " + "enuMap for " + fieldID + " does not contain value " + kval + ".");
             return null;
           case "number":
                 var ret = map.expr(cfg);
                 ret.number = ret.value;
                 if (defineSymbolName != null) {
                   if (defineSymbolName == "") {
                     if ("defineSymbol" in map) {
                       var defineSymbol = map.defineSymbol(cfg, ret);
                       this.addCDefine(defineSymbol, ret.value, ret.description, formatOptions);
                       ret.value = defineSymbol;
                     }
                   }
                   else {
                     this.addCDefine(defineSymbolName, ret.value, ret.description, formatOptions);
                   }
                 }
                 return ret;
           case "multiEnum":
             // Usable for "mask" type enumerations. There can be enumerations which doesn't define anything for state =0. Then in case of there is no enum state in final code, a define needs to be created
             // Examples result code: "kMCG_PllEnableIndependent | kMCG_PllEnableInStop", "kMCG_PllEnableIndependent", "MCG_PLL_DISABLE_INDEPENDENT_DISABLE_IN_STOP"
             var code = "";
             var comment = "";
             // Iterate throught all functions in funcMap, create expression/comment from corresponding row in enumMap and add defines if there is "defineSymbol" in the row
             for (var i=0; i< map.funcMap.length; i++) {
               if (map.funcMap[i][0](cfg)) {
                 var enm = map.enumMap[map.funcMap[i][1]];
                 code = code + ((code == "") ? "" : " | ") + enm[1];
                 comment = comment + ((comment == "") ? "" : ", ") + enm[2];
                 if (enm[3] != null && enm[3] != "") {
                   if (defineSymbolName != null) {
                     this.addCDefine(enm[1], enm[0], enm[2], formatOptions);
                   }
                 }
               }
             }
             // If all functions returned zeros, use symbol "0"
             if (code == "") {
               if (map.enumMap[0] != null) {
                 code = code + ((code == "") ? "" : " | ") + map.enumMap[0][1];
                 comment = comment + ((comment == "") ? "" : ", ") + map.enumMap[0][2];
                 if (map.enumMap[0][3] != null && map.enumMap[0][3] != "") {
                   if (defineSymbolName != null) {
                     this.addCDefine(map.enumMap[0][1], map.enumMap[0][0], map.enumMap[0][2], formatOptions);
                   }
                 }
               }
               else {
                 scriptApi.logError("[DEBUG] " + "enuMap for " + fieldID + " does not contain value 0.");
                 return null;
               }
             }
             if ("comment" in map) {
               comment = map.comment(cfg);
             }
             return new FieldVal(code, comment);
           default:
             return null;
         }
      },

      // Data part of this SDKMapper object. "mapping" contains field ids of certain type and theirs settings.
      // Type of the id can be "number", "enum" or "multiEnum".
      // defineSymbol in an enumMap table or defineSymbol function in <id> object ensures definiton of symbolic constant
      // e.g. there is defined "#define MCG_IRCLK_DISABLE   0U" for [0, "MCG_IRCLK_DISABLE",      "MCGIRCLK disabled", "defineSymbol"],
      //      or "#define RTC_OSC_CAP_LOAD_30PF   0x3c00" for "rtc.oscCapLoad" id
      mapping : {
        "scgConfig.source" : {
              type: "enum",
              keyFunc: (function (cfg) { return cfg.getValueAsText("SCGMode") }),
              enumMap:[
                ["SOSC", "SCG_CLOCKOUT_SRC_SCG_SLOW",  "System OSC is selected as System Clock Source"],
                ["SIRC", "SCG_CLOCKOUT_SRC_SIRC", "Slow IRC is selected as System Clock Source"],
                ["FIRC", "SCG_SYSTEM_CLOCK_SRC_FIRC",  "Fast IRC is selected as System Clock Source"],
                ["SPLL", "SCG_CLOCKOUT_SRC_SPLL",  "System PLL is selected as System Clock Source"],
              ]
        },
         ///////////////////////// Run Clock Control Register ///////////////////////////////////
         "rccrConfig.src": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::RCCR","SCS") }),
            enumMap:[
                 [1, "SCG_SYSTEM_CLOCK_SRC_SYS_OSC",   "System OSC"],
                 [2, "SCG_SYSTEM_CLOCK_SRC_SIRC",   "Slow IRC"],
                 [3, "SCG_SYSTEM_CLOCK_SRC_FIRC",   "Fast IRC"],
                 [6, "SCG_SYSTEM_CLOCK_SRC_SYS_PLL",   "System PLL"],
                 [7, "SCG_SYSTEM_CLOCK_SRC_NONE",  "MAX value"],
           ]
        },
        "rccrConfig.divSlow": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVSLOW") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Slow Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Slow Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Slow Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Slow Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Slow Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Slow Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Slow Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Slow Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Slow Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Slow Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Slow Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Slow Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Slow Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Slow Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Slow Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Slow Clock Divider: divided by 16"],
              ]
        },
        "rccrConfig.divBus": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVBUS") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Bus Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Bus Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Bus Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Bus Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Bus Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Bus Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Bus Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Bus Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Bus Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Bus Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Bus Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Bus Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Bus Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Bus Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Bus Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Bus Clock Divider: divided by 16"],
              ]
        },
        "rccrConfig.divCore": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::RCCR", "DIVCORE") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Core Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Core Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Core Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Core Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Core Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Core Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Core Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Core Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Core Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Core Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Core Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Core Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Core Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Core Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Core Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Core Clock Divider: divided by 16"],
              ]
        },
         ///////////////////////// VLPR Clock Control Register ///////////////////////////////////
         "vccrConfig.src": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::VCCR","SCS") }),
            enumMap:[
                 [1, "SCG_SYSTEM_CLOCK_SRC_SYS_OSC",   "System OSC"],
                 [2, "SCG_SYSTEM_CLOCK_SRC_SIRC",   "Slow IRC"],
                 [3, "SCG_SYSTEM_CLOCK_SRC_FIRC",   "Fast IRC"],
                 [6, "SCG_SYSTEM_CLOCK_SRC_SYS_PLL",   "System PLL"],
                 [7, "SCG_SYSTEM_CLOCK_SRC_NONE",  "MAX value"],
           ]
        },
        "vccrConfig.divSlow": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::VCCR", "DIVSLOW") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Slow Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Slow Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Slow Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Slow Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Slow Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Slow Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Slow Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Slow Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Slow Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Slow Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Slow Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Slow Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Slow Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Slow Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Slow Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Slow Clock Divider: divided by 16"],
              ]
        },
        "vccrConfig.divBus": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::VCCR", "DIVBUS") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Bus Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Bus Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Bus Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Bus Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Bus Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Bus Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Bus Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Bus Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Bus Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Bus Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Bus Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Bus Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Bus Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Bus Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Bus Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Bus Clock Divider: divided by 16"],
              ]
        },
        "vccrConfig.divCore": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::VCCR", "DIVCORE") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Core Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Core Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Core Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Core Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Core Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Core Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Core Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Core Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Core Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Core Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Core Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Core Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Core Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Core Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Core Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Core Clock Divider: divided by 16"],
              ]
        },
         ///////////////////////// HSRUN Clock Control Register ///////////////////////////////////
         "hccrConfig.src": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::HCCR","SCS") }),
            enumMap:[
                 [1, "SCG_SYSTEM_CLOCK_SRC_SYS_OSC",   "System OSC"],
                 [2, "SCG_SYSTEM_CLOCK_SRC_SIRC",   "Slow IRC"],
                 [3, "SCG_SYSTEM_CLOCK_SRC_FIRC",   "Fast IRC"],
                 [6, "SCG_SYSTEM_CLOCK_SRC_SYS_PLL",   "System PLL"],
                 [7, "SCG_SYSTEM_CLOCK_SRC_NONE",  "MAX value"],
           ]
        },
        "hccrConfig.divSlow": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVSLOW") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Slow Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Slow Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Slow Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Slow Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Slow Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Slow Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Slow Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Slow Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Slow Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Slow Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Slow Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Slow Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Slow Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Slow Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Slow Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Slow Clock Divider: divided by 16"],
              ]
        },
        "hccrConfig.divBus": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVBUS") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Bus Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Bus Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Bus Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Bus Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Bus Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Bus Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Bus Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Bus Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Bus Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Bus Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Bus Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Bus Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Bus Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Bus Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Bus Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Bus Clock Divider: divided by 16"],
              ]
        },
        "hccrConfig.divCore": {
           type: "enum",
           keyFunc: (function (cfg) {return cfg.getBitFieldValueAsBigInteger("SCG::HCCR", "DIVCORE") }),
           enumMap:[
                [0, "SCG_SYSTEM_CLOCK_DIV_BY_1",  "Core Clock Divider: divided by 1"],
                [1, "SCG_SYSTEM_CLOCK_DIV_BY_2",  "Core Clock Divider: divided by 2"],
                [2, "SCG_SYSTEM_CLOCK_DIV_BY_3",  "Core Clock Divider: divided by 3"],
                [3, "SCG_SYSTEM_CLOCK_DIV_BY_4",  "Core Clock Divider: divided by 4"],
                [4, "SCG_SYSTEM_CLOCK_DIV_BY_5",  "Core Clock Divider: divided by 5"],
                [5, "SCG_SYSTEM_CLOCK_DIV_BY_6",  "Core Clock Divider: divided by 6"],
                [6, "SCG_SYSTEM_CLOCK_DIV_BY_7",  "Core Clock Divider: divided by 7"],
                [7, "SCG_SYSTEM_CLOCK_DIV_BY_8",  "Core Clock Divider: divided by 8"],
                [8, "SCG_SYSTEM_CLOCK_DIV_BY_9",  "Core Clock Divider: divided by 9"],
                [9, "SCG_SYSTEM_CLOCK_DIV_BY_10", "Core Clock Divider: divided by 10"],
                [10, "SCG_SYSTEM_CLOCK_DIV_BY_11", "Core Clock Divider: divided by 11"],
                [11, "SCG_SYSTEM_CLOCK_DIV_BY_12", "Core Clock Divider: divided by 12"],
                [12, "SCG_SYSTEM_CLOCK_DIV_BY_13", "Core Clock Divider: divided by 13"],
                [13, "SCG_SYSTEM_CLOCK_DIV_BY_14", "Core Clock Divider: divided by 14"],
                [14, "SCG_SYSTEM_CLOCK_DIV_BY_15", "Core Clock Divider: divided by 15"],
                [15, "SCG_SYSTEM_CLOCK_DIV_BY_16", "Core Clock Divider: divided by 16"],
              ]
        },

		 ///////////////////////// sircConfig ///////////////////////////////////

		"sircConfig.locked": {
            type: "enum",
       		  keyFunc: (function (cfg) {return  cfg.getBitFieldValue("SCG::SIRCCSR","LK") }),
          	 enumMap: [
              	[0, "false",   "unlocked"],
                [1, "true", "locked"],
           	 ]
         },

    "sircConfig.enableInStop": {
          type: "enum",
          keyFunc: (function(cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SIRCCSR","SIRCSTEN")}),
          enumMap: [
                   [0, "false", "Slow IRC disabled in stop mode"],
                   [1, "true", "Enable SIRC in stop mode"],
          ]
        },

     "sircConfig.enableInLowPower": {
          type: "enum",
          keyFunc: (function(cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SIRCCSR","SIRCLPEN")}),
          enumMap: [
                   [0, "false", "Slow IRC disabled in low power mode"],
                   [1, "true", "Enable SIRC in low power mode"],
          ]
        },

        "sircConfig.div1": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SIRCDIV","SIRCDIV1") }),
  		   enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "Slow IRC Clock Divider 1: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "Slow IRC Clock Divider 1: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "Slow IRC Clock Divider 1: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "Slow IRC Clock Divider 1: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "Slow IRC Clock Divider 1: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "Slow IRC Clock Divider 1: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "Slow IRC Clock Divider 1: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "Slow IRC Clock Divider 1: divided by 64"],
           ]
        },
        "sircConfig.div2": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SIRCDIV","SIRCDIV2") }),
  		   enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "Slow IRC Clock Divider 3: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "Slow IRC Clock Divider 3: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "Slow IRC Clock Divider 3: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "Slow IRC Clock Divider 3: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "Slow IRC Clock Divider 3: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "Slow IRC Clock Divider 3: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "Slow IRC Clock Divider 3: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "Slow IRC Clock Divider 3: divided by 64"],
           ]
        },
		"sircConfig.range": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SIRCCFG","RANGE") }),
  		   enumMap:[
                 [0, "SCG_SIRC_RANGE_LOW",   "Slow IRC low range clock (2 MHz)"],
                 [1, "SCG_SIRC_RANGE_HIGH",   "Slow IRC high range clock (8 MHz)"],
           ]
        },

    ///////////////////////// fircConfig ///////////////////////////////////
    "fircConfig.locked": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValue("SCG::FIRCCSR","LK") }),
             enumMap: [
                [0, "false",   "unlocked"],
                [1, "true", "locked"],
             ]
         },

    "fircConfig.regulator": {
          type: "enum",
          keyFunc: (function(cfg) { return cfg.getBitFieldValue("SCG::FIRCCSR","FIRCREGOFF")}),
          enumMap: [
                   [0, "false", "FIRC regulator is disabled"],
                   [1, "true", "FIRC regulator is enabled"],
          ]
        },

        "fircConfig.div1": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::FIRCDIV","FIRCDIV1") }),
         enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "Fast IRC Clock Divider 1: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "Fast IRC Clock Divider 1: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "Fast IRC Clock Divider 1: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "Fast IRC Clock Divider 1: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "Fast IRC Clock Divider 1: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "Fast IRC Clock Divider 1: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "Fast IRC Clock Divider 1: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "Fast IRC Clock Divider 1: divided by 64"],
           ]
        },
        "fircConfig.div2": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::FIRCDIV","FIRCDIV2") }),
         enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "Fast IRC Clock Divider 3: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "Fast IRC Clock Divider 3: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "Fast IRC Clock Divider 3: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "Fast IRC Clock Divider 3: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "Fast IRC Clock Divider 3: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "Fast IRC Clock Divider 3: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "Fast IRC Clock Divider 3: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "Fast IRC Clock Divider 3: divided by 64"],
           ]
        },
    "fircConfig.range": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::FIRCCFG","RANGE") }),
         enumMap:[
                 [0, "SCG_FIRC_RANGE_48M",   "Fast IRC is trimmed to 48MHz"],
                 [1, "SCG_FIRC_RANGE_52M",   "Fast IRC is trimmed to 52MHz"],
                 [2, "SCG_FIRC_RANGE_56M",   "Fast IRC is trimmed to 56MHz"],
                 [3, "SCG_FIRC_RANGE_60M",   "Fast IRC is trimmed to 60MHz"],
           ]
        },

       ///////////////////////// rtcConfig ///////////////////////////////////
        "rtcConfig.rtcClkInFreq": {
           type: "number",
           expr: (function (cfg) {
                  var freq = cfg.getValueAsDecimal("RTC.RTC_CLKIN.outFreq");
                    return new FieldVal(freq, "Core clock frequency: " + freq + "Hz");
              })
        },

       ///////////////////////// soscConfig ///////////////////////////////////
        "soscConfig.freq": {
           type: "number",
           expr: (function (cfg) {
                     var freq = cfg.getValueAsText("SCG.SOSC.outFreq");
                     if (freq != "N/A") {
                       freq = cfg.getValueAsInteger("SCG.SOSC.outFreq");
                     }
                     return new FieldVal(freq + "U", "System Oscillator frequency: " + freq + "Hz");
                  })
        },
        "soscConfig.monitorMode": {
              type: "enum",
              keyFunc: (function (cfg) { return (ScriptBitFields.getMultiShiftedBitFieldConfigValues(cfg, [["SCG::SOSCCSR", "SOSCCMRE"],["SCG::SOSCCSR", "SOSCCM"]])) }),
              enumMap:[
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SOSCCSR", "SOSCCMRE"],["SCG::SOSCCSR", "SOSCCM"]],[0,0]), "SCG_SOSC_MONITOR_DISABLE", "Monitor disabled"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SOSCCSR", "SOSCCMRE"],["SCG::SOSCCSR", "SOSCCM"]],[0,1]), "SCG_SOSC_MONITOR_INT", "Interrupt when the system OSC error is detected"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SOSCCSR", "SOSCCMRE"],["SCG::SOSCCSR", "SOSCCM"]],[1,0]), "SCG_SOSC_MONITOR_DISABLE",       "Monitor disabled"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SOSCCSR", "SOSCCMRE"],["SCG::SOSCCSR", "SOSCCM"]],[1,1]), "SCG_SOSC_MONITOR_RESET", "Reset when the system OSC error is detected"],
              ]
        },
        "soscConfig.locked": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValue("SCG::SOSCCSR","LK") }),
             enumMap: [
                [0, "false",   "SOSC disabled"],
                [1, "true", "Enable SOSC clock"],
             ]
         },
        "soscConfig.extRef": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValueAsBigInteger("SCG::SOSCCFG","EREFS") }),
             enumMap: [
                [0, "SCG_SOSC_REF_EXT",   "External reference clock requested"],
                [1, "SCG_SOSC_REF_OSC", "Internal oscillator of OSC requested."],
             ]
         },
        "soscConfig.gain": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValueAsBigInteger("SCG::SOSCCFG","HGO") }),
             enumMap: [
                [0, "SCG_SOSC_GAIN_LOW",   "Configure crystal oscillator for low-gain operation"],
                [1, "SCG_SOSC_GAIN_HIGH", "Configure crystal oscillator for high-gain operation"],
             ]
         },
        "soscConfig.range": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValueAsBigInteger("SCG::SOSCCFG","RANGE") }),
             enumMap: [
                [2, "SCG_SOSC_RANGE_MID", "Medium frequency range selected for the crystal oscillator of 4 MHz to 8MHz."],
                [3, "SCG_SOSC_RANGE_HIGH", "High frequency range selected for the crystal oscillator of 8 MHz to 40 MHz."],
             ]
         },
        "soscConfig.div1": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SOSCDIV","SOSCDIV1") }),
         enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "System OSC Clock Divider 1: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "System OSC Clock Divider 1: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "System OSC Clock Divider 1: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "System OSC Clock Divider 1: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "System OSC Clock Divider 1: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "System OSC Clock Divider 1: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "System OSC Clock Divider 1: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "System OSC Clock Divider 1: divided by 64"],
           ]
        },
        "soscConfig.div2": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SOSCDIV","SOSCDIV2") }),
         enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "System OSC Clock Divider 3: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "System OSC Clock Divider 3: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "System OSC Clock Divider 3: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "System OSC Clock Divider 3: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "System OSC Clock Divider 3: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "System OSC Clock Divider 3: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "System OSC Clock Divider 3: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "System OSC Clock Divider 3: divided by 64"],
           ]
        },

		///////////////////////// spll_RunConfig ///////////////////////////////////

		// "spllConfig.enableMode": {
  //             type: "multiEnum",
  //      		  funcMap:[
  //               [(function (cfg) {return  cfg.getBitFieldValueAsBigInteger("SCG::SPLLCSR","SPLLEN") }), "spllen"],
  //               [(function (cfg) {return  cfg.getBitFieldValueAsBigInteger("SCG::SPLLCSR","SPLLSTEN") }), "spllten"],
  //             ],
  //         	 enumMap: {
  //             	0 :      [0, "SCG_SPLL_DISABLE",   "System PLL disabled", "defineSymbol"],
  //             	"spllen" : [ScriptBitFields.getShiftedValue("SCG::SPLLCSR","SPLLEN",1), "kSCG_SysPllEnable",  "Enable SPLL clock"],
  //             	"spllten":  [ScriptBitFields.getShiftedValue("SCG::SPLLCSR","SPLLSTEN",1), "kSCG_SysPllEnableInStop",  "Enable SPLL in stop mode"],
  //          	 }
  //        },
        "spllConfig.monitorMode": {
              type: "enum",
              keyFunc: (function (cfg) { return (ScriptBitFields.getMultiShiftedBitFieldConfigValues(cfg, [["SCG::SPLLCSR", "SPLLCMRE"],["SCG::SPLLCSR", "SPLLCM"]])) }),
              enumMap:[
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SPLLCSR", "SPLLCMRE"],["SCG::SPLLCSR", "SPLLCM"]],[0,0]), "SCG_SPLL_MONITOR_DISABLE", "Monitor disabled"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SPLLCSR", "SPLLCMRE"],["SCG::SPLLCSR", "SPLLCM"]],[0,1]), "SCG_SPLL_MONITOR_INT", "Interrupt when the System PLL error is detected"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SPLLCSR", "SPLLCMRE"],["SCG::SPLLCSR", "SPLLCM"]],[1,0]), "SCG_SPLL_MONITOR_DISABLE",       "Monitor disabled"],
                 [ScriptBitFields.getMultiShiftedValues([["SCG::SPLLCSR", "SPLLCMRE"],["SCG::SPLLCSR", "SPLLCM"]],[1,1]), "SCG_SPLL_MONITOR_RESET", "Reset when the System PLL error is detected"],
              ]
         },
        "spllConfig.locked": {
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValue("SCG::SPLLCSR","LK") }),
             enumMap: [
                [0, "false",   "unlocked"],
                [1, "true", "locked"],
             ]
         },
        "spllConfig.div1": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SPLLDIV","SPLLDIV1") }),
  		   enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "System PLL Clock Divider 1: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "System PLL Clock Divider 1: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "System PLL Clock Divider 1: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "System PLL Clock Divider 1: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "System PLL Clock Divider 1: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "System PLL Clock Divider 1: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "System PLL Clock Divider 1: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "System PLL Clock Divider 1: divided by 64"],
           ]
        },
        "spllConfig.div2": {
           type: "enum",
           keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::SPLLDIV","SPLLDIV2") }),
            enumMap:[
                 [0, "SCG_ASYNC_CLOCK_DISABLE",   "System PLL Clock Divider 3: Clock output is disabled"],
                 [1, "SCG_ASYNC_CLOCK_DIV_BY_1",   "System PLL Clock Divider 3: divided by 1"],
                 [2, "SCG_ASYNC_CLOCK_DIV_BY_2",   "System PLL Clock Divider 3: divided by 2"],
                 [3, "SCG_ASYNC_CLOCK_DIV_BY_4",   "System PLL Clock Divider 3: divided by 4"],
                 [4, "SCG_ASYNC_CLOCK_DIV_BY_8",  "System PLL Clock Divider 3: divided by 8"],
                 [5, "SCG_ASYNC_CLOCK_DIV_BY_16",  "System PLL Clock Divider 3: divided by 16"],
                 [6, "SCG_ASYNC_CLOCK_DIV_BY_32",  "System PLL Clock Divider 3: divided by 32"],
                 [7, "SCG_ASYNC_CLOCK_DIV_BY_64", "System PLL Clock Divider 3: divided by 64"],
           ]
        },
		"spllConfig.src": {
           type: "number",
           expr : (function (cfg) { var tmp = 0 + "U";
                    return new FieldVal(tmp);
               }),
        },
		"spllConfig.prediv": {
           type: "number",
           expr: (function (cfg) {
  		            var tmp = cfg.getBitFieldValueAsBigInteger("SCG::SPLLCFG", "PREDIV") + 1;
                    return new FieldVal("(uint8_t)SCG_SPLL_CLOCK_PREDIV_BY_" + tmp, "Divided by " + cfg.getValueAsText("SCG.PREDIV.scale"));
		          })
        },
		"spllConfig.mult": {
           type: "number",
           expr: (function (cfg) {
  		            var tmp = cfg.getBitFieldValueAsBigInteger("SCG::SPLLCFG", "MULT") + 16;
                    return new FieldVal("(uint8_t)SCG_SPLL_CLOCK_MULTIPLY_BY_" + tmp, "Multiply Factor is " + cfg.getValueAsText("SCG.SPLL_mul.scale"));
		          })
        },
        "scgClockOutConfig.source" : {
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::CLKOUTCNFG","CLKOUTSEL") }),
              enumMap:[
                [0, "SCG_CLOCKOUT_SRC_SCG_SLOW",  "SCG SLOW."],
                [1, "SCG_CLOCKOUT_SRC_SOSC",  "System OSC."],
                [2, "SCG_CLOCKOUT_SRC_SIRC", "Slow IRC."],
                [3, "SCG_CLOCKOUT_SRC_FIRC",  "Fast IRC."],
                [6, "SCG_CLOCKOUT_SRC_SPLL",  "System PLL."],
              ]
        },


        ///////////////////////// sim for peripherals ///////////////////////////////////

         "clockOutConfig.enable":{
            type: "enum",
            keyFunc: (function (cfg) {return  cfg.getBitFieldValue("SIM::CHIPCTL","CLKOUTEN") }),
             enumMap: [
                [0, "false", "disabled"],
                [1, "true", "enabled"],
             ]
         },
         "clockOutConfig.source":{
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SIM::CHIPCTL", "CLKOUTSEL") }),
              enumMap:[
                 [0, "SIM_CLKOUT_SEL_SYSTEM_SCG_CLKOUT",    "SCG CLKOUT clock select: SCG slow clock", "defineSymbol"],
                 [2, "SIM_CLKOUT_SEL_SYSTEM_SOSC_DIV2_CLK",    "SCG CLKOUT clock select: System OSC", "defineSymbol"],
                 [4, "SIM_CLKOUT_SEL_SYSTEM_SIRC_DIV2_CLK",    "SCG CLKOUT clock select: Slow IRC", "defineSymbol"],
                 [6, "SIM_CLKOUT_SEL_SYSTEM_FIRC_DIV2_CLK",    "SCG CLKOUT clock select: Fast IRC", "defineSymbol"],
                 [7, "SIM_CLKOUT_SEL_SYSTEM_HCLK",    "SCG CLKOUT clock select: HCLK", "defineSymbol"],
                 [8, "SIM_CLKOUT_SEL_SYSTEM_SPLL_DIV2_CLK",    "SCG CLKOUT clock select: Low Power FLL", "defineSymbol"],
                 [9, "SIM_CLKOUT_SEL_SYSTEM_BUS_CLK",    "SCG CLKOUT clock select: Bus clock", "defineSymbol"],
                 [10,"SIM_CLKOUT_SEL_SYSTEM_LPO_128K_CLK",    "LPO CLK 128 Khz ", "defineSymbol"],
                 [12,"SIM_CLKOUT_SEL_SYSTEM_LPO_CLK",    "LPO CLK as selected by SIM LPO CLK Select ", "defineSymbol"],
                 [14,"SIM_CLKOUT_SEL_SYSTEM_RTC_CLK",    "RTC CLK as selected by SIM CLK 32 KHz Select", "defineSymbol"],
               ],
         },
         "clockOutConfig.divider":{
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SIM::CHIPCTL", "CLKOUTDIV") }),
              enumMap:[
                [0, "SIM_CLKOUT_DIV_BY_1",  "Divided by 1"],
                [1, "SIM_CLKOUT_DIV_BY_2",  "Divided by 2"],
                [2, "SIM_CLKOUT_DIV_BY_3",  "Divided by 3"],
                [3, "SIM_CLKOUT_DIV_BY_4",  "Divided by 4"],
                [4, "SIM_CLKOUT_DIV_BY_5",  "Divided by 5"],
                [5, "SIM_CLKOUT_DIV_BY_6",  "Divided by 6"],
                [6, "SIM_CLKOUT_DIV_BY_7",  "Divided by 7"],
                [7, "SIM_CLKOUT_DIV_BY_8",  "Divided by 8"],
              ],
         },
         "lpoClockConfig.sourceLpoClk":{
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SIM::LPOCLKS", "LPOCLKSEL") }),
              enumMap:[
                 [0, "SIM_LPO_CLK_SEL_LPO_128K",   "128 kHz LPO clock", "defineSymbol"],
                 [1, "SIM_LPO_CLK_SEL_NO_CLOCK",     "No clock", "defineSymbol"],
                 [2, "SIM_LPO_CLK_SEL_LPO_32K",       "32 kHz LPO clock which is divided by the 128 kHz LPO clock", "defineSymbol"],
                 [3, "SIM_LPO_CLK_SEL_LPO_1K",       "1 kHz LPO clock which is divided by the 128 kHz LPO clock", "defineSymbol"],
              ],
         },
         "lpoClockConfig.sourceRtcClk":{
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SIM::LPOCLKS", "RTCCLKSEL") }),
              enumMap:[
                 [0, "SIM_RTCCLK_SEL_SOSCDIV1_CLK",   "SOSCDIV1 clock", "defineSymbol"],
                 [1, "SIM_RTCCLK_SEL_LPO_32K",     "32 kHz LPO clock", "defineSymbol"],
                 [2, "SIM_RTCCLK_SEL_RTC_CLKIN",       "RTC_CLKIN clock", "defineSymbol"],
                 [3, "SIM_RTCCLK_SEL_FIRCDIV1_CLK",       "FIRCDIV1 clock", "defineSymbol"],
              ],
         },



    "scg.scgClkoutSel":{
              type: "enum",
              keyFunc : (function (cfg) { return cfg.getBitFieldValueAsBigInteger("SCG::CLKOUTCNFG", "CLKOUTSEL") }),
              enumMap:[
                 [0, "SCG_CLKOUTCNFG_SCGSLOW",   "SCG CLKOUT clock select: SCG slow clock", "defineSymbol"],
                 [1, "SCG_CLKOUTCNFG_SOSC",     "SCG CLKOUT clock select: System OSC", "defineSymbol"],
                 [2, "SCG_CLKOUTCNFG_SIRC",       "SCG CLKOUT clock select: Slow IRC", "defineSymbol"],
                 [3, "SCG_CLKOUTCNFG_FIRC",       "SCG CLKOUT clock select: Fast IRC", "defineSymbol"],
                 [6, "SCG_CLKOUTCNFG_SPLL",       "SCG CLKOUT clock select: System PLL", "defineSymbol"],
              ],
         },
         "osc32.osc32config":{
              type: "enum",
              keyFunc: (function (cfg) { return (ScriptBitFields.getMultiShiftedBitFieldConfigValues(cfg, [["OSC32::CR", "ROSCSTPEN"],["OSC32::CR", "ROSCEREFS"]])) }), //Only used when OSC32::CR ROSCEN is set.
              enumMap:[
                 [0, "OSC32, kOSC32_Bypass",      "Bypass mode, use external input clock directly"],
                 [ScriptBitFields.getMultiShiftedValues([["OSC32::CR", "ROSCSTPEN"],["OSC32::CR", "ROSCEREFS"]],[1,0]), "OSC32, kOSC32_Bypass", "Bypass mode, use external input clock directly"],
                 [ScriptBitFields.getMultiShiftedValues([["OSC32::CR", "ROSCSTPEN"],["OSC32::CR", "ROSCEREFS"]],[0,1]), "OSC32, kOSC32_Crystal",       "OSC32 works in crystal mode, does not work in stop mode"],
                 [ScriptBitFields.getMultiShiftedValues([["OSC32::CR", "ROSCSTPEN"],["OSC32::CR", "ROSCEREFS"]],[1,1]), "OSC32, kOSC32_CrystalEnableInStop", "OSC32 works in crystal mode, still work in stop mode"],
              ],
         },

         "coreClock": {
           type: "number",
           expr: (function (cfg) {
                     var freq = cfg.getValueAsText("SCGCLKOUT.outFreq");
                     if (freq != "N/A") {
                       freq = cfg.getValueAsBigInteger("SCGCLKOUT.outFreq");
                     }
                     return new FieldVal(freq, "Core clock frequency: " + freq + "Hz");
                  })
        },

      } // mapping
};
