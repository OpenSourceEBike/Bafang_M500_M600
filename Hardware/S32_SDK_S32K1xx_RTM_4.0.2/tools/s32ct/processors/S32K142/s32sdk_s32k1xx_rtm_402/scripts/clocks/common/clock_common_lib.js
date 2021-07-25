/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */
// Prepare global parent object with properties and functions related to clock code generation
ClockCodeGen = function() {
  // clock profile
  this.profile = scriptApi.getProfile();
  // set of configurations
  this.configs = this.profile.getConfigurations();
  // creates ID from a general string
  this.makeID = function (name) {
    return name.replace(/[^\w\d]/g, "_");
  }
  // names of the functions
  this.getConfigID = function(clkconfig) {
      return this.makeID(clkconfig.getName());
  }
} // CGen object prototype


// This object opens a file and writes into the file 
OutputFile = function(fileName) {
  // name of the generated file
  this.fileName = fileName;
  // output stream 
  this.output = scriptApi.createFile(fileName);
  // write file content and "close" file object
  this.close = function(content) {
    for (var i = 0; i < content.length; i++) {
      this.output.write(content[i]);
    }
    this.fileName = "";
    this.output = null;
  }
} // OutputFile object prototype


// This object collects outputs, implements insertion and getting function in order to write into file 
OutputWriter = function(fileOutput) {
  // default position of the comment on the right side
  this.defaultCommentPos = 51;
  // write queue initialization
  this.writeQueue = new Array();
  // Start index of block which has not flushed yet
  this.notFlushedStartPos = 0;
  // dummy string as marker for "no line" position reservation used for code insertion
  this.noLineMarker = "  /* No line. This string is marker for code generation and should not been written in the code. */";
  // file output
  this.fileOutput = fileOutput;
  // This method writes to the output
  this.write = function (text, position) {
    if (text == null) {                                                                       
      text = this.noLineMarker;
    }
    else {
      //text += "\n";
    }
    if (position == null) {
      return this.writeQueue.push(text) - 1;
    }
    else {
      if (position >= this.notFlushedStartPos && position < this.writeQueue.length) {
        if (this.writeQueue[position] == this.noLineMarker) {
          this.writeQueue[position] = text;
        }
        else {
          this.writeQueue[position] += "\n" + text;
        }
        return position;
      }
      else {
        scriptApi.logError("[DEBUG] Reference passed to write function is wrong.");
        return null;
      }
    }
  }
  // This method to the output array of lines
  this.writeLines = function (linesArr, position) {
    //return this.write(linesArr.join("\n") + "\n", position);
    return this.write(linesArr.join("\n"), position);
  }
  // writes complete line with code text and added optional comment
  // commentPos and commentType are optional                                                  
  this.line = function(text, comment, commentType, commentPos) {
    var str = text; 
    if (text != null && comment != undefined) {
      var commstr;
      if (commentType == undefined) {
        commentType = "/*";
      }            
      commstr = commentType.concat(" ").concat(comment);
      if (commentType != "//") {
        commstr = commstr.concat(" */");
      }
      if (commentPos == undefined) {
        commentPos = this.defaultCommentPos;
      }
      // add speces if needed to match comment position
      if (str.length < commentPos) {
        str = str.concat(Array(commentPos - str.length).join(" "));
      }
      str = str.concat(commstr);
    }  
    // write line to the output
    return this.write(str);
  }
  // get collected content
  this.getContent = function() {
    var content = new Array();
    if (this.notFlushedStartPos < this.writeQueue.length) {
      for (var i = this.notFlushedStartPos; i < this.writeQueue.length; i++) {
        if (this.writeQueue[i] != this.noLineMarker) {
          content.push(this.writeQueue[i]);
        }
      }
      this.writeQueue = new Array();
      //this.notFlushedStartPos = this.writeQueue.length;
    }
    return content;
  }
  // return whether the position is empty or not
  this.isPositionEmpty = function(position) {
    return (this.writeQueue[position] == this.noLineMarker);
  }
} // OutputWriter object prototype


// This object collects, formats and sorts #defines
// endingValuePos - Ending position of #define value
// commentPos - Starting position of #define comment
DefineBlock = function(endingValuePos, commentPos) {
  // Default ending position of #define value
  var defaultEndingValuePos = 60;
  // Default starting position of #define comment
  var defaultcommentPos = 63;
  
  // Ending position of #define value
  this.endingValuePos = (endingValuePos != null)? endingValuePos: defaultEndingValuePos;
  // Starting position of #define comment
  this.commentPos = (commentPos != null)? commentPos: defaultcommentPos;
  // Container for collecting #defines 
  this.container = new Object();
  // This normalize name of define. Intended purpose is for example to extend numbers 
  // inside define name in order to better sorting (e.g. final order should be SOURCE_INPUT_2, SOURCE_INPUT_12)
  this.normalizeName = function(name) {
    // Not implemented at this moment. No use-case is known for its usage.
    return name;
  }
  // This adds a define into internal container
  // name - name of symbolic constant
  // value - value of symbolic constant
  // comment - comment for symbolic constant
  // formatOptions - determines format of result number, e.g. "hex" or "unsigned", see parameter options for function formatField
  this.addDefine = function (name, value, comment, formatOptions) {
    var normalizedName = this.normalizeName(name);
    var existingDefine = this.container[normalizedName];
    if (existingDefine == null) {
      this.container[normalizedName] = {
        name: name,
        value: value,
        comment: comment,
        formatOptions: formatOptions
      }
    }
    else {
      if (this.container[normalizedName].name == name && !BigNumber.equal(this.container[normalizedName].value, value) && this.container[normalizedName].comment != comment) {
        scriptApi.logError("[DEBUG] #define " + name + " " + BigNumber.toString(value) + "/* " + comment + " */ cannot be added into the container. It is already exist."); 
      }
    }
  }
  //This gets array formated strings. Each item contains one definition intended like one line in output file.
  this.getDefineBlock = function () {
    var defineArray = new Array();
    var normalizedNames = Object.keys(this.container).sort();
    for (var i = 0; i < normalizedNames.length; i++) {
      var def = this.container[normalizedNames[i]];
      defineArray.push(this.getDefineLine(def.name, def.value, def.comment, def.formatOptions)); 
    }
    return defineArray;
  }
  // gets complete line with code text and added optional comment
  this.getDefineLine = function(name, value, comment, formatOptions) {
    var str = "#define " + name + " "; 
    if (comment != undefined) {
      if (value==null)
        return "";
      var valueStr = BigNumber.toString(value);
      if (!isNaN(value)) {
        if (formatOptions == null) {
          formatOptions = ["unsigned"];
        }
        valueStr = OutputUtils.formatField(value, formatOptions);
      }
      if ((str.length + valueStr.length) < (this.endingValuePos + 1)) {
        str = str.concat(Array(this.endingValuePos + 1 - (str.length + valueStr.length)).join(" "));
      }
      str = str.concat(valueStr);
      if (str.length < this.commentPos) {
        str = str.concat(Array(this.commentPos - str.length).join(" "));
      }
      str = str.concat("/*!< " + comment + " */");
    }  
    // return the line
    return str;
  }
} //DefineBlock object prototype


// This object enhancing bit-field functionality, e.g. getting ored bit-fields register mask, getting ored bit-fields values,
// getting ored bit-fields configuration values,...
ScriptBitFields = {
  // Gets IRegBitFieldAPI object for bit field specified in two string parameters "<peripheral_name>::<register_name>","bit-field_name", e.g. "MCG::C2","EREFS"
  // Exact peripheral, register, bit-field names can be used as well as their aliases named "id"
  getBitField: function(peripheralRegisterName, bitFieldName) {
    var peripheralName = peripheralRegisterName.split("::")[0];
    var registerName = peripheralRegisterName.split("::")[1];
    return scriptApi.getProfile().getRegistersDB().findBitField("id", peripheralName, registerName, bitFieldName);
  },

  // Gets register mask or bit field specified in two string parameters "<peripheral_name>::<register_name>","bit-field_name", e.g. "MCG::C2","EREFS"
  // Exact peripheral, register, bit-field names can be used as well as their aliases named "id"
  // Example: it returns 4 for "MCG::C2","EREFS" 
  getMask: function (registerName, bitFieldName) {
    return this.getBitField(registerName, bitFieldName).getRegisterMask();
  },

  // Gets shifted value specified in 3rd parameter on position of bit-field specified in 1st and 2nd parameters
  // Exact peripheral, register, bit-field names can be used as well as their aliases named "id"
  // Example: it returns 4 for "MCG::C2","EREFS",1 or 0 for "MCG::C2","EREFS",0 
  getShiftedValue: function (registerName, bitFieldName, value) {
    var bitField = this.getBitField(registerName, bitFieldName);
    if (bitField == null) {
      return 0;
    }
    return BigNumber.bitwiseAnd(BigNumber.shiftLeft(value, bitField.getOffset()), bitField.getRegisterMask());
  },

  // Gets ored shifted values specified in 2nd string array parameter on positions of bit-fields specified in 1st string array parameters
  // Exact peripheral, register, bit-field names can be used as well as their aliases named "id"
  // Example: it returns 12 for [["MCG::C2","EREFS"],["MCG::C2","HGO"]],[1,1] or 8 for [["MCG::C2","EREFS"],["MCG::C2","HGO"]],[0,1] 
  getMultiShiftedValues: function (bitFields, values) {
    if (bitFields.length != values.length) {
      scriptApi.logError("[DEBUG] Array parameters bitFields and values passed into ScriptBitFields.getMultiShiftedValues() function don't have the same length.");
    }
    var result = 0;
    for (bfIndex = 0; bfIndex < bitFields.length; bfIndex++) {
      result = BigNumber.bitwiseOr(result, this.getShiftedValue(bitFields[bfIndex][0], bitFields[bfIndex][1], values[bfIndex]));
    }
    return result;
  },

  // Gets ored shifted values as configured in the tool for bit-fields specified in 2nd string array parameters and given configuration specified in 1st parameter
  // Exact peripheral, register, bit-field names can be used as well as their aliases named "id"
  // Example: it returns 12 for [["MCG::C2","EREFS"],["MCG::C2","HGO"]] if bit-fields are configured on [1,1] 
  //          or 8 for [["MCG::C2","EREFS"],["MCG::C2","HGO"]] if bit-fields are configured on [0,1] 
  getMultiShiftedBitFieldConfigValues: function (cfg, bitFields) {
    var result = 0;
    for (bfIndex = 0; bfIndex < bitFields.length; bfIndex++) {
      var getVal = cfg.getShiftedBitFieldValue(bitFields[bfIndex][0], bitFields[bfIndex][1]);
      if (BigNumber.equal(getVal, -1)) {
        scriptApi.logError("[DEBUG] " + bitFields[bfIndex][0] + "[" + bitFields[bfIndex][1] + "] is not configured (discovered by ScriptBitFields.getMultiShiftedBitFieldConfigValues())");
        return result;
      }
      result = BigNumber.bitwiseOr(result, getVal);
    }
    return result;
  }
} // ScriptBitFields prototype

// This object supports compare, math and bitwise functions based on java BigInteger.
// The object is intended for work with register and bit-field values.
// There are also functions for creation BigInteger number, conversion to string, etc.
// Input parameters of most functions are primarily java.math.BigInteger (created by create function of this object), 
// javascript number or java.lang.Long which is automatically converted on the BigInteger.
// Result numbers are typically BigIntegers.

BigNumber = {
  // Maximum number for checking input parameters if js number is passed
  maxJsNumber: 0x1FFFFFFFFFFFFF,
  
  create: function(num, radix) {
    if (radix == null) {
      radix = 10;
    }
    return new java.math.BigInteger(num, radix);
  },
  
  equal: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].equals(r[1]);
    }
    else {
      return (r[0] == r[1]);
    }
  }, 

  compare: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].compareTo(r[1]);
    }
    else {
      return (r[0] - r[1]);
    }
  }, 

  add: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].add(r[1]);
    }
    else {
      return (r[0] + r[1]);
    }
  }, 

  subtract: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].subtract(r[1]);
    }
    else {
      return (r[0] - r[1]);
    }
  }, 

  multiply: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].multiply(r[1]);
    }
    else {
      return (r[0] * r[1]);
    }
  }, 

  divide: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].divide(r[1]);
    }
    else {
      return (r[0] / r[1]);
    }
  }, 

  bitwiseOr: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].or(r[1]);
    }
    else {
      return (r[0] | r[1]);
    }
  }, 

  bitwiseXor: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].xor(r[1]);
    }
    else {
      return (r[0] ^ r[1]);
    }
  }, 

  bitwiseAnd: function(a, b) {
    var r = this.convertOperands(a, b);
    if (this.isBigIntegerObject(r[0]) && this.isBigIntegerObject(r[1])) {
      return r[0].and(r[1]);
    }
    else {
      return (r[0] & r[1]);
    }
  }, 

  bitwiseNeg: function(a) {
    var r = this.convertOperand(a);
    if (this.isBigIntegerObject(r)) {
      return r.negate();
    }
    else {
      return (~r);
    }
  }, 

  shiftLeft: function(a, n) {
    var r = this.convertOperand(a);
    if (this.isBigIntegerObject(r)) {
      return r.shiftLeft(n);
    }
    else {
      return (r << n);
    }
  }, 

  shiftRight: function(a, n) {
    var r = this.convertOperand(a);
    if (this.isBigIntegerObject(r)) {
      return r.shiftRight(n);
    }
    else {
      return (r >>> n);
    }
  }, 

  isParamPositiveNumber: function(o) {
    if (o instanceof java.math.BigInteger) {
      if (r[0].compareTo(this.create(0)) < 0) {
        scriptApi.logError("[DEBUG] Parameter value (" + this.toString(o) + ") is negative number.");
        return false;
      }
    } else if (o instanceof String || typeof(o) == "string") {
    } else if ( o instanceof Number || typeof(o) == "number") {
      if (o < 0) {
        scriptApi.logError("[DEBUG] Parameter value (" + this.toString(o) + ") is negative number.");
        return false;
      }
    }
    return true;
  },
  
  isParamCorrect: function(o) {
    if (o instanceof java.math.BigInteger) {
    } else if (o instanceof java.lang.Long) {
    } else if (o instanceof String || typeof(o) == "string") {
    } else if ( o instanceof Number || typeof(o) == "number") {
      if (o > this.maxJsNumber) {
        scriptApi.logError("[DEBUG] JS number " + o.toString() + " is over js number limit (precision). Use BigNumber.create() function.");
        return false;
      }
    }
    return true; 
  },
  
  convertNumberToBigInteger: function (num) {
    if (num instanceof java.lang.Long) {
      var strNum = util.toString(num, 10);
    }
    else {
      var strNum = num.toString(10);
    }
    if (util.isStringNumber(strNum)) {
      return this.create(strNum,10);
    }
    scriptApi.logError("[DEBUG] JS number " + num.toString() + " is not convertable to java.math.BigInteger.");
    return num.toString();
  },
  
  convertOperand: function (a) {
    this.isParamCorrect(a);
    var c = a;
    if (this.isJsNumber(a) || this.isJavaLong(a)) {
      c = this.convertNumberToBigInteger(a);
    }
    return c;
  },
  
  convertOperands: function (a, b) {
    this.isParamCorrect(a);
    this.isParamCorrect(b);
    var c = a;
    var d = b;
    if (this.isJsNumber(a) || this.isJavaLong(a)) {
      c = this.convertNumberToBigInteger(a);
    }
    if (this.isJsNumber(b) || this.isJavaLong(b)) {
      d = this.convertNumberToBigInteger(b);
    }
    if (this.isString(a) || this.isString(b)) {
      c = this.toString(a);
      d = this.toString(b);
    } 
    return [c, d];
  },
  
  isBigIntegerObject: function (o) {
    if (o instanceof java.math.BigInteger) {
      return true;
    }
    return false;
  },
  
  isJsNumber: function (o) {
    if (o instanceof java.math.BigInteger) {
      return false;
    } else if (o instanceof java.lang.Long) {
      return false;
    } else if (o instanceof Number || typeof(o) == "number") {
      return true;
    }
    return false;
  },
  
  isJavaLong: function (o) {
    if (o instanceof java.lang.Long) {
      return true;
    }
    return false;
  },
  
  isString: function(o) {
    if (o instanceof java.math.BigInteger || o instanceof java.lang.Long || o instanceof Number) {
      return false;
    } else if (o instanceof String || typeof(o) == "string") {
      return true;
    }
    return false;
  },
  
  toString: function (o, radix) {
    var System = Java.type("java.lang.System");
    var Thread = Java.type("java.lang.Thread");
    if (radix == null) {
      radix = 10;
    }
    if (o instanceof java.math.BigInteger) {
      return util.toString(o, radix);
    } else if (o instanceof java.lang.Long) {
      return util.toString(o, radix);
    } else if (o instanceof Number || typeof(o) == "number") {
      return o.toString(radix);
    }
    return o.toString();
  },
  
} //BigNumber prototype                                                                                                                                                             

                                                     