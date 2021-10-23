"use strict";

/*
 * Copyright 2016-2017 NXP
 * Copyright 2018 NXP
 * To be used with Configuration Tools. See Software License Agreement of Configuration Tools.
 */

/* Script to generate code for Pins tool */

// Main part of generated files name
var chFileName = "pin_mux";

// Preprocessor condition is header file, typically #ifndef _PIN_MUX_H_ , "#define _PIN_MUX_H_"
var moduleName = "pin_mux";

// Register database Object
var registerDatabaseObject = PExProcessor.getRegistersDB();

// Variable to store index of currently processed component
var currentComponent = -1;

/* c indentation 4 */
var cIndent4 = '    ';
/* c indentation 8 */
var cIndent8 = '        ';

// Global array of messages
var messageList = new Array();

// Definitions of couple of comments, that delimites fragments of code ignored by clang-format tool
var clang_off = '/* clang-format off */';
var clang_on = '/* clang-format on */';
var numOfPins = [];
var portPeripheralName = "PORT";
var gpioPrefixName = "PT";

//Global structure to store values for each pin
var pinSettings = new Object();

//Default value for pins config structure fields
var notAvailable = "n/a";

pinSettings.base = notAvailable;
pinSettings.pinPortIdx = notAvailable;
pinSettings.pullConfig = notAvailable;
pinSettings.passiveFilter = notAvailable;
pinSettings.driveSelect = notAvailable;
pinSettings.mux = notAvailable;
pinSettings.pinLock = notAvailable;
pinSettings.intConfig = notAvailable;
pinSettings.clearIntFlag = notAvailable;
pinSettings.gpioBase = notAvailable;
pinSettings.direction = notAvailable;
pinSettings.digitalFilter = notAvailable;
pinSettings.initValue = notAvailable;

//Printed functions (not empty functional groups)
var printedFunctions = new Array();

/**
 * Function prints to file comment, that delimites fragments of code ignored by clang-format tool
 */
function clangOff() {
    PExOut_gen(clang_off);
}

/**
 * Function prints to file comment, that delimites fragments of code not supposed to be ignored by clang-format tool
 */
function clangOn() {
    PExOut_gen(clang_on);
}

/**
 * Function creates count long string consisting of characters. Example: makeString('*', 5) returns '*****'
 */
function makeString(character, count) {
    if (count > 0) {
        return (Array(count + 1).join(character[0]));
    } else {
        return "";
    }
}

/**
 * Function finds, if subString (2nd param) is contained sourceString (1st param)
 */
function Contains(sourceString, subString) {
    return ((sourceString.indexOf(subString)) != -1);
}

/* Return boolean value if given coreId is 'enabled' within the tool UI */
function isEnabledCoreId(coreId) {
  var coreList = JSON.parse(coreListTxt);
  return (coreList[coreId]['enabled']);
}

/**
 * Function prints multiline string to output including newlines (PExOut_gen is not able to print newlines in string content)
 */
function PExOut_gen(stringToPrint) {
    var lineEnding = PExProject.getLineEnding();
    if (stringToPrint != null){
        var linesToPrint = stringToPrint.split(lineEnding);
        for (var i = 0; i < linesToPrint.length; i++) {
            PExOut.gen(linesToPrint[i]);
        }
    }
}

/**
 * Prints a message on console
 * repetitively - if false the same message is not printed more than once
 * returns nothing
 */
function logMessage(message, repetitively) {
    var index = messageList.indexOf(message);
    if (index == -1) {
        messageList.push(message);
    }
    var variant = "Unknown processor";
    if (cpuVariant != null) {
        variant = cpuVariant;
    }
    if (repetitively || (index == -1)) {
        PExOut.log(variant + ": " + message);
    }
}

/**
 * Gets bitwise or. Native bitwise operation in Javascript works only with 31-bit numbers because of implicit conversion to signed 32-bit integer.
 * a - value 1
 * b - value 2
 * returns result of bitwise OR
 */
function getBitwiseOr(a, b) {
    if (a < 0 || b < 0) {
        logMessage("One of operands (" + a + ", " + b + ") is negative number.", true);
    }
    var result = 0;
    var mask = 0xFFFF;
    var divisor = mask + 1;
    var i = 0;
    while ((a != 0) || (b != 0)) {
        var r = ((a & mask) | (b & mask));
        for (var j = 0; j < i; j++) {
            r *= divisor;
        }
        result += r;
        a = Math.floor(a / divisor);
        b = Math.floor(b / divisor);
        i++;
    }
    return result;
}

/**
 * Gets bitwise and. Native bitwise operation in Javascript works only with 31-bit numbers because of implicit conversion to signed 32-bit integer.
 * a - value 1
 * b - value 2
 * returns result of bitwise AND
 */
function getBitwiseAnd(a, b) {
    if (a < 0 || b < 0) {
        logMessage("One of operands (" + a + ", " + b + ") is negative number.", true);
    }
    var result = 0;
    var mask = 0xFFFF;
    var divisor = mask + 1;
    var i = 0;
    while ((a != 0) && (b != 0)) {
        var r = ((a & mask) & (b & mask));
        for (var j = 0; j < i; j++) {
            r *= divisor;
        }
        result += r;
        a = Math.floor(a / divisor);
        b = Math.floor(b / divisor);
        i++;
    }
    return result;
}
// Preparation of other information, such as Clock gate generation enable/disable, configuration strategies for getting right set register configuration for given user function
var allComponents = PExProject.getAllComponents(); // Each component represents one user function
var configurationStrategies = new Array(); // Configuration strategies for getting right set register configuration for given user function
var componentCoreIds = new Array(); // Core IDs array. It says core id for each user function
var textOptions = null;
for (var pc = 0; pc < allComponents.length; pc++) {
    currentComponent = pc;
    var funcNameItem = allComponents[pc].findItemBySymbol("FunctionName");
    if (funcNameItem != null) {
        configurationStrategies[pc] = funcNameItem.getText();
    }
    textOptions = allComponents[pc].getComponentOptions();
    if (textOptions != null) {
        var options = JSON.parse(textOptions);
        componentCoreIds.push(options["coreID"]);
    }
}
var configurationStrategy = null;

// Getting core ids information
var coreIds = null;
var coreListTxt = PExProcessor.getCoresList();
if (coreListTxt != null) {
    coreIds = Object.keys(JSON.parse(coreListTxt));
}

// Filtering and finding cores with an assigned function
var notEmptyCoreIds = new Array();
for (var coreIndex in coreIds) {
    var coreId = coreIds[coreIndex];
    if (componentCoreIds.indexOf(coreId) > -1) {
        notEmptyCoreIds.push(coreId);
    }
}

/**
 * Function gets direction text from Routed Pins View and maps it
 * to values defined in SDK
 * @param config - current configuration index
 * @param index - register index in register list
 * @returns - pin's direction
 */
function getDirection(reg){
    var allComponents = PExProject.getAllComponents();
    var direction;
    var sdkDirection;
    for (var pc = 0; pc < allComponents.length; pc++) {
        var pin_listProperty = allComponents[pc].findItemBySymbol("pin_list");
        var numOfItems = pin_listProperty.getItemsCount();
        for (var i = 0; i < numOfItems; i++) {
            var pinItem = allComponents[pc].findItemBySymbol("pin_signal" + i);
            if (pinItem.getTextValue() == reg){
                direction = allComponents[pc].findItemBySymbol("direction" + i).getTextValue();
            }

        }
    }

    switch(direction){
        case "INPUT":
            sdkDirection = "GPIO_INPUT_DIRECTION";
            break;
        case "OUTPUT":
            sdkDirection = "GPIO_OUTPUT_DIRECTION";
            break;
        default :
            sdkDirection = "GPIO_UNSPECIFIED_DIRECTION";
            break;
    }
    return sdkDirection;
}

/**
 * Function returns port name from register name
 * @param regName - e.g. PORTB_PCR15
 * @returns port - e.g. PORTB
 */
function getPortFromRegName(regName){
    var arr = regName.split("_");
    return arr[0];
}

/**
 * Function returns port name from register name
 * @param regName - e.g. PORTB_PCR15
 * @returns port - e.g. PORTB
 */
function getPeriphFromRegName(regName){
    var arr = regName.split("_");
    return arr[0];
}

/**
 * Function returns pin number from register name
 * @param regName - e.g. PORTB_PCR15
 * @returns pin index - e.g. 15u
 */
function getPinFromRegName(regName){
    var arr = regName.split("_");
    var pcr = arr[1];
    return pcr.substr(3, pcr.length);
}

/**
 * Function returns GPIO base from register name
 * @param regName - e.g. PORTB_PCR15
 * @returns port - e.g. PTB
 */
function getGpioBaseFromRegName(regName){
    var port = getPortFromRegName(regName);
    var tmp = port.replace(portPeripheralName,gpioPrefixName);
    return tmp;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapMuxFieldToSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = "PORT_PIN_DISABLED";
            break;
        case 1:
            val = "PORT_MUX_AS_GPIO";
            break;
        case 2:
            val = "PORT_MUX_ALT2";
            break;
        case 3:
            val = "PORT_MUX_ALT3";
            break;
        case 4:
            val = "PORT_MUX_ALT4";
            break;
        case 5:
            val = "PORT_MUX_ALT5";
            break;
        case 6:
            val = "PORT_MUX_ALT6";
            break;
        case 7:
            val = "PORT_MUX_ALT7";
            break;
        case 8:
            val = "PORT_MUX_ADC_INTERLEAVE";
            break;
        default:
            val = "ERROR";
            break;
    }
    return val;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapInterruptFlagToSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = false;
            break;
        case 1:
            val = true;
            break;
        default:
            break;
    }
    return val;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapLockFieldtoSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = false;
            break;
        case 1:
            val = true;
            break;
        default: break;
    }
    return val;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapInteruptConfigToSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = "PORT_DMA_INT_DISABLED";
            break;
        case 1:
            val = "PORT_DMA_RISING_EDGE";
            break;
        case 2:
            val = "PORT_DMA_FALLING_EDGE";
            break;
        case 3:
            val = "PORT_DMA_EITHER_EDGE";
            break;
        case 8:
            val = "PORT_INT_LOGIC_ZERO";
            break;
        case 9:
            val = "PORT_INT_RISING_EDGE";
            break;
        case 10:
            val = "PORT_INT_FALLING_EDGE";
            break;
        case 11:
            val = "PORT_INT_EITHER_EDGE";
            break;
        case 12:
            val = "PORT_INT_LOGIC_ONE";
            break;
        default: break;
    }
    return val;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapDriveStrengthToSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = "PORT_LOW_DRIVE_STRENGTH";
            break;
        case 1:
            val = "PORT_HIGH_DRIVE_STRENGTH";
            break;
        default: break;
    }
    return val;
}

/**
 * Function maps register value to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapPassiveFilterToSDK(bfValue){
    var val;
    switch(bfValue){
        case 0:
            val = false;
            break;
        case 1:
            val = true;
            break;
        default: break;
    }
    return val;
}

/**
 * Function maps register bitfield values (PE, PS) to SDK value
 * @param bfValue - value from register
 * @returns - value from SDK
 */
function mapPullConfigToSDK(pullEnable, pullSelect){
    var val;
    if(pullEnable != 0){
        if(pullSelect == 0){
            val = "PORT_INTERNAL_PULL_DOWN_ENABLED";
        }else if (pullSelect == 1){
            val = "PORT_INTERNAL_PULL_UP_ENABLED";
        }
    }else{
        val = "PORT_INTERNAL_PULL_NOT_ENABLED";
    }
    return val;
}

/**
 * Create a map between the register name and the corresponding register modification
 * @param {String} functionName
 * @returns {map<String, RegisterModification>} registerName, registerModification
 */
// Map with all register modifications
var registerModifications = new Array();
function getAllRegisterModifications(functionName) {
    var registerModificationsMap = new Array();
    var configurationSteps = PExProcessor.getRegisterConfigurationSequence(true, functionName, null);
    for (var stepIdx = 0; stepIdx < configurationSteps.length; stepIdx++) {
        var registerModifications = configurationSteps[stepIdx].getRegistersConfigurations();
        for (var regModIdx = 0; regModIdx < registerModifications.length; regModIdx++) {
            var regMod = registerModifications[regModIdx];
            registerModificationsMap[regMod.getRegisterName()] = regMod;
        }
    }
    return registerModificationsMap;
}

/**
 * Get the bit field value from register modification for the registers that use bit 31th
 * Native bitwise operation in Javascript works only with 31-bit numbers because of implicit conversion to signed 32-bit integer
 * @param {String} registerName
 * @param {integer} mask, shift, default of bit field
 * @returns {integer} current bit field value
 */
function getValueBitField(registerName, mask, shift, dft)
{
    /* the init value is default value */
    var val = dft;
    for (var reg in registerModifications) {
        if (reg == registerName) {
            var c = (registerModifications[reg].getClrRegMask() / Math.pow(2, shift)) & mask;
            var s = (registerModifications[reg].getSetRegMask() / Math.pow(2, shift)) & mask;
            if (s > 0) {
                /* val is set value */
                val = s;
            } else if (c > 0 && s == 0) {
                /* val is cleared */
                val = 0;
            }
            break;
        }
    }
    return val;
}

/**
 * Function re-update bitfield values, which have diff with 0
 */
function UpdateDefaultValueRegister(reg){
    var pcr = reg.registerName;
    var pullEnable = -1;
    var pullSelect = -1;
    if ((pcr == "PORTA_PCR4") || (pcr == "PORTA_PCR5") || (pcr == "PORTC_PCR5")) {
        pullSelect = getValueBitField(reg.registerName, 1, 0, 1);
        pullEnable = getValueBitField(reg.registerName, 1, 1, 1);
        pinSettings.pullConfig = mapPullConfigToSDK(pullEnable,pullSelect);
        if (pcr == "PORTA_PCR5") {
            pinSettings.passiveFilter = mapPassiveFilterToSDK(getValueBitField(reg.registerName, 1, 4, 1));
        }
    }
    if (pcr == "PORTC_PCR4") {
        pullSelect = getValueBitField(reg.registerName, 1, 0, 0);
        pullEnable = getValueBitField(reg.registerName, 1, 1, 1);
        pinSettings.pullConfig = mapPullConfigToSDK(pullEnable,pullSelect);
    }
    if (pcr == "PORTA_PCR10") {
        pinSettings.driveSelect = mapDriveStrengthToSDK(getValueBitField(reg.registerName, 1, 6, 1));
    }
}

/* Reset all pinSettings to not Available*/
function ResetGlobalPinSetings(){
    pinSettings.base = notAvailable;
    pinSettings.pinPortIdx = notAvailable;
    pinSettings.pullConfig = notAvailable;
    pinSettings.passiveFilter = notAvailable;
    pinSettings.driveSelect = notAvailable;
    pinSettings.mux = notAvailable;
    pinSettings.pinLock = notAvailable;
    pinSettings.intConfig = notAvailable;
    pinSettings.clearIntFlag = notAvailable;
    pinSettings.gpioBase = notAvailable;
    pinSettings.direction = notAvailable;
    pinSettings.digitalFilter = notAvailable;
    pinSettings.initValue = notAvailable;
}

/**
 * Function processes bitfield values from input register and
 * assigns correponding value to pinSetting structure fields.
 * @param reg - current register
 */
function populatePinSettings(reg){
    var dbRegister = registerDatabaseObject.getRegisterByFullName(reg.registerName);
    var registerClrSetMask = getBitwiseOr(reg.registerClrMask, reg.registerSetMask);
    // default values -1 for PS and PE, because we need to process them together
    var pullEnable = -1;
    var pullSelect = -1;
    if (registerClrSetMask != 0) {
        if (dbRegister != null) {
            var dbBitFields = dbRegister.getBitFields();
            //iterate though register bitfields
            for (var i = 0; i< dbBitFields.length;i++){
                var bitField = dbBitFields[i];
                var bfName = bitField.getName();
                var bitFieldMask = bitField.getRegisterMask().doubleValue();
                var bfValue = getBitwiseAnd(reg.registerSetMask, bitFieldMask) >>> bitField.getOffset();
                // if register is not reserved
                if(bfName!=""){
                    switch(bfName){
                        case "MUX":
                            pinSettings.mux = mapMuxFieldToSDK(bfValue);
                            break;
                        case "ISF":
                            pinSettings.clearIntFlag = mapInterruptFlagToSDK(bfValue);
                            break;
                        case "LK":
                            pinSettings.pinLock = mapLockFieldtoSDK(bfValue);
                            break;
                        case "IRQC":
                            pinSettings.intConfig = mapInteruptConfigToSDK(bfValue);
                            break;
                        case "PFE":
                            pinSettings.passiveFilter = mapPassiveFilterToSDK(bfValue);
                            break;
                        case "DSE":
                            pinSettings.driveSelect = mapDriveStrengthToSDK(bfValue);
                            break;
                        case "PS":
                            pullSelect = bfValue;
                            break;
                        case "PE":
                            pullEnable = bfValue;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    // if we found values for PE and PS, call the appropiate function
    if(pullSelect != -1 && pullEnable != -1){
        pinSettings.pullConfig = mapPullConfigToSDK(pullEnable,pullSelect);
    }

    // default value for drive strength
    if(pinSettings.driveSelect == notAvailable){
        pinSettings.driveSelect = "PORT_LOW_DRIVE_STRENGTH";
    }

    // default value for passiveFilter
    if(pinSettings.passiveFilter == notAvailable){
        pinSettings.passiveFilter = false;
    }

    // Update the default value for some bitfields that diff with 0
    UpdateDefaultValueRegister(reg);

    //populate remaining fields from register name
    pinSettings.base = getPortFromRegName(reg.registerName);
    pinSettings.pinPortIdx = getPinFromRegName(reg.registerName);
    //gpio base must be set only if pin is muxed as gpio
    if(pinSettings.mux == "PORT_MUX_AS_GPIO"){
        pinSettings.gpioBase = getGpioBaseFromRegName(reg.registerName);
        pinSettings.initValue = (reg.initValue >> pinSettings.pinPortIdx) & 1;
    }else{
        pinSettings.gpioBase = "NULL";
    }
    pinSettings.digitalFilter = ((reg.digitalFilter >> pinSettings.pinPortIdx) & 1) ? true : false;
    if((pinSettings.mux == "PORT_PIN_DISABLED") && (reg.adcInterleave != 0))
        pinSettings.mux = mapMuxFieldToSDK(8);
}

/**
 * Function prints current values in pinSettings structure
 * @param reg - current register
 * @param index - register index(used for variable name in generated code)
 */
function printPinSettingsConfig(reg){
    //fill in structure fields
    populatePinSettings(reg);
    //print output code
    PExOut_gen(cIndent4 + "{");
    PExOut_gen(cIndent8 + ".base            = " + pinSettings.base + ",");
    PExOut_gen(cIndent8 + ".pinPortIdx      = " + pinSettings.pinPortIdx + "U,");
    PExOut_gen(cIndent8 + ".pullConfig      = " + pinSettings.pullConfig + ",");
    PExOut_gen(cIndent8 + ".driveSelect     = " + pinSettings.driveSelect + ",");
    PExOut_gen(cIndent8 + ".passiveFilter   = " + pinSettings.passiveFilter + ",");
    PExOut_gen(cIndent8 + ".mux             = " + pinSettings.mux + ",");
    PExOut_gen(cIndent8 + ".pinLock         = " + pinSettings.pinLock + ",");
    PExOut_gen(cIndent8 + ".intConfig       = " + pinSettings.intConfig + ",");
    PExOut_gen(cIndent8 + ".clearIntFlag    = " + pinSettings.clearIntFlag + ",");
    PExOut_gen(cIndent8 + ".gpioBase        = " + pinSettings.gpioBase + ",");
    // only print direction if pin has gpio function
    if(pinSettings.mux == "PORT_MUX_AS_GPIO"){
        PExOut_gen(cIndent8 + ".direction       = " + pinSettings.direction + ",");
    }
    PExOut_gen(cIndent8 + ".digitalFilter   = " + pinSettings.digitalFilter + ",");
    // only print initValue if pin has gpio function
    if(pinSettings.mux == "PORT_MUX_AS_GPIO"){
        PExOut_gen(cIndent8 + ".initValue       = " + pinSettings.initValue + "U,");
    }
    PExOut_gen(cIndent4 + "},");
    /* Reset Global PinSettings */
    ResetGlobalPinSetings();
}

/* Calculate value in SIM_CHIPCTL register for ADC muxing */
var adcInterleaveValue = 0;
function getPinsAdcInterleaved(regName)
{
    var value = 0;

    if (((regName == "PORTB_PCR14") && ((adcInterleaveValue & 0x8) != 0)) || ((regName == "PORTC_PCR1") && ((adcInterleaveValue & 0x8) == 0)))
    {
        value = 8;
        return value;
    }

    if (((regName == "PORTB_PCR13") && ((adcInterleaveValue & 0x4) != 0)) || ((regName == "PORTC_PCR0") && ((adcInterleaveValue & 0x4) == 0)))
    {
        value = 4;
        return value;
    }

    if (((regName == "PORTB_PCR1") && ((adcInterleaveValue & 0x2) != 0)) || ((regName == "PORTB_PCR16") && ((adcInterleaveValue & 0x2) == 0)))
    {
        value = 2;
        return value;
    }

    if (((regName == "PORTB_PCR0") && ((adcInterleaveValue & 0x1) != 0)) || ((regName == "PORTB_PCR15") && ((adcInterleaveValue & 0x1) == 0)))
    {
        value = 1;
        return value;
    }

    return value;
}

/* Get PORT/GPIO index for digitial filter and initValue fields */
var initValueRegister = [0,0,0,0,0];
var digitalFilterRegister = [0,0,0,0,0];
function getIndexFromName(regName){
    var val = 0;
    if ((regName == "PTA") || (regName == "PORTA"))
    {
        val = 0;
    }
    else if ((regName == "PTB") || (regName == "PORTB"))
    {
        val = 1;
    }
    else if ((regName == "PTC") || (regName == "PORTC"))
    {
        val = 2;
    }
    else if ((regName == "PTD") || (regName == "PORTD"))
    {
        val = 3;
    }
    else if ((regName == "PTE") || (regName == "PORTE"))
    {
        val = 4;
    }
    return val;
}
/**
 * Analyzes register settings for pin muxing and functional pin properties (electrical features) from the tool.
 * It iterates through all steps and registers and finding and collecting clock gate configurations
 * pc - index of configuration/function (internally component)
 * configurationSteps - array of configuration register steps
 */
function analyzeRegisterConfigurationSequence(pc, configurationSteps) {
    /* Analyzing returned data. Iteration through all registers and finding and collecting clock gate configurations */
    registerList[pc] = new Object();
    var configurationRegisterList = registerList[pc];
    for (var si = 0; si < configurationSteps.length; si++) {
        var configurationStepName = configurationSteps[si].getName();
        var configurationStepDescription = configurationSteps[si].getDescription();
        var configurationRegisters = configurationSteps[si].getRegistersConfigurations();
        for (var ri = 0; ri < configurationRegisters.length; ri++) {
            var registerName = configurationRegisters[ri].getRegisterName();
            var dbPeripheral = registerName.split('_')[0];
            if (Contains(registerName,"PDOR"))
            {
                initValueRegister[getIndexFromName(dbPeripheral)] = configurationRegisters[ri].getSetRegMask();
            }
            if (Contains(registerName,"DFER"))
            {
                digitalFilterRegister[getIndexFromName(dbPeripheral)] = configurationRegisters[ri].getSetRegMask();
            }
            if (Contains(registerName,"SIM"))
            {
                adcInterleaveValue = configurationRegisters[ri].getSetRegMask();
            }
        }
    }
    for (var si = 0; si < configurationSteps.length; si++) {
        var configurationStepName = configurationSteps[si].getName();
        var configurationStepDescription = configurationSteps[si].getDescription();
        var configurationRegisters = configurationSteps[si].getRegistersConfigurations();
        for (var ri = 0; ri < configurationRegisters.length; ri++) {
            var registerName = configurationRegisters[ri].getRegisterName();
            var dbRegister = registerDatabaseObject.getRegisterByFullName(registerName);
            if (dbRegister != null) {
                if(Contains(registerName, "PCR"))
                {
                    var dbPeripheral = registerName.split('_')[0];
                    configurationRegisterList[registerName] = new Object();
                    configurationRegisterList[registerName].registerName = registerName;
                    configurationRegisterList[registerName].registerClrMask = configurationRegisters[ri].getClrRegMask();
                    configurationRegisterList[registerName].registerSetMask = configurationRegisters[ri].getSetRegMask();
                    configurationRegisterList[registerName].initValue = initValueRegister[getIndexFromName(dbPeripheral)];
                    configurationRegisterList[registerName].digitalFilter = digitalFilterRegister[getIndexFromName(dbPeripheral)];
                    configurationRegisterList[registerName].adcInterleave = getPinsAdcInterleaved(registerName);
                }
            }
        }
    }
}

/**
 * Processes register configuration sequence for all pin function configurations
 * coreId - cpu core id
 */
function processRegisterConfigurationSequence(coreId,identifierNames) {
    for (var pc = 0; pc < allComponents.length; pc++) { // Pin configuration represented by tables in the UI of the tool
        currentComponent = pc;
        var pin_listProperty = allComponents[pc].findItemBySymbol("pin_list");
        var numOfItems = pin_listProperty.getItemsCount();
        for (var i = 0; i < numOfItems; i++) {
            var identifierItem = allComponents[pc].findItemBySymbol("identifier" + i);
            var pinItem = allComponents[pc].findItemBySymbol("pin_signal" + i);
            if(identifierItem != null && pinItem !=null){
                if(identifierItem.getError() == null && pinItem.getError() == null){
                    var identifierText = identifierItem.getTextValue();
                    var pinItemText = pinItem.getTextValue();
                    if (identifierText != "") {
                        identifierNames.push(pc + '.' + pinItemText + '.' + identifierText);
                    }
                }
            }
        }
        numOfPins[pc] = numOfItems;
        if (coreId == componentCoreIds[pc]) {
            var configurationSteps = PExProcessor.getRegisterConfigurationSequence(true, configurationStrategies[pc], null); // exclude automatic, given function, all steps
            analyzeRegisterConfigurationSequence(pc, configurationSteps);
        }
    }
}

/**
 * Prints register configurations for one configuration
 * configuration - given configuration where constant definitions is added
 * return value - no data; just printing into output
 */
function printOneRegisterConfiguration(configuration) {
    var printedAnything = false;
    var regIdx = 0;
      /* ***** Pin muxing, functional property Register configurations ***** */
    var configurationRegisterList = registerList[configuration];

    //only generate function call if we have at least one register configured
    if(Object.keys(configurationRegisterList).length !== 0){
        PExOut_gen("/* Generate array of configured pin structures */");
        PExOut_gen("pin_settings_config_t g_pin_mux_InitConfigArr"+configuration+"[NUM_OF_CONFIGURED_PINS"
                    + configuration +"] = {");
    }
    // update the register modifications map for this function
    registerModifications = getAllRegisterModifications(configurationStrategies[configuration]);
    for (var r in configurationRegisterList) {
        // populate direction field from UI
        var reg = getGpioBaseFromRegName(configurationRegisterList[r].registerName);
        reg += configurationRegisterList[r].registerName.split('_')[1].substr(3);
        pinSettings.direction = getDirection(reg);
        //populate and print pinSettings structure for current register
        printPinSettingsConfig(configurationRegisterList[r]);
        regIdx++;
        if(!printedAnything){
            printedAnything = true;
        }
    }

    //end the function call - if we have at least one register configured
    if(Object.keys(configurationRegisterList).length !== 0){
        PExOut_gen("};")
    }
}

var registerList = null; // List of registers for pin muxing and pin functional properties; register is defined by name, clear mask and set mask

function getEnabledCoreId(coreListTxt) {
    var coreList = JSON.parse(coreListTxt);
    var coreIds = Object.keys(coreList);
    for (var coreId = 0; coreId < coreIds.length; coreId++) {
        if (coreList[coreIds[coreId]]['enabled']) {
            return coreIds[coreId];
        }
    }
    return null;
}

function printYamlState(yamlState) {
    PExOut_gen('');
    clangOff();
    PExOut_gen('/*');
    PExOut_gen(yamlState);
    PExOut_gen(' */');
    clangOn();
    PExOut_gen('');
}

function getInitFunctions() {
    var initFunctions = [];

    for (var pc = 0; pc < allComponents.length; pc++) {
        var functionJSON = JSON.parse(allComponents[pc].getComponentOptions());

        if (functionJSON['callFromInitBoot']) {
            initFunctions.push(functionJSON['FunctionName']);
        }
    }
    return initFunctions;
}

/* Returns the number of rows from RoutedPins for the specified component (functional group) */
function getNumberOfRoutedPins(component){
    var allPropertiesAPI = component.getAllProperties();
    var cnt = allPropertiesAPI.count();
    var routedPins = allPropertiesAPI.getItem(cnt-1);
    return parseInt(routedPins.getTextValue());
}

/* Creates h and c file and prints theirs body.
 * return value - no data; just printing into output
 */
function printDriver() {
    for (var coreIndex in notEmptyCoreIds) {
        var coreId = notEmptyCoreIds[coreIndex];
        if (isEnabledCoreId(coreId)) {
            var moduleNameSuffix = "";
            PExOut.setOutputFile(chFileName + moduleNameSuffix + ".c");
            registerList = new Object();
            var identifierNames=[];
            processRegisterConfigurationSequence(coreId,identifierNames);

            PExOut.setOutputFile(chFileName + moduleNameSuffix + ".h");
            PExOut.gen('#ifndef _' + (chFileName + moduleNameSuffix).toUpperCase() + '_H_');
            PExOut.gen('#define _' + (chFileName + moduleNameSuffix).toUpperCase() + '_H_');
            PExOut.gen('');
            PExOut.gen('#include "pins_driver.h"');
            PExOut.gen('');
            PExOut.gen('/' + makeString('*', 119));
            PExOut.gen(' * Definitions');
            PExOut.gen(' ' + makeString('*', 118) + '/');
            PExOut.gen('');
            PExOut.gen('/*!');
            PExOut.gen(' * @addtogroup ' + chFileName + moduleNameSuffix);
            PExOut.gen(' * @{');
            PExOut.gen(' */');
            PExOut.gen('');
            PExOut.gen('/' + makeString('*', 119));
            PExOut.gen(' * API');
            PExOut.gen(' ' + makeString('*', 118) + '/');
            PExOut.gen('');
            PExOut.gen('#if defined(__cplusplus)');
            PExOut.gen('extern \"C\" {');
            PExOut.gen('#endif');
            for (var pc = 0; pc < allComponents.length; pc++) { // Pin configuration represented by tables in the UI of the tool
                currentComponent = pc;
                if (coreId == componentCoreIds[pc]) {
                    configurationStrategy = configurationStrategies[pc];

                    PExOut.gen('');
                    PExOut.gen('');
                    PExOut.gen('/*! @brief Definitions/Declarations for ' + configurationStrategy + ' Functional Group */');
                    if(identifierNames[0] != null)
                    {
                        var IdentifierTitle = false;
                        for(var i =0; i < identifierNames.length; i++)
                        {
                            var identifierGroup = identifierNames[i].split(".")[0];
                            if (parseInt(identifierGroup) == pc) {
                                if (IdentifierTitle == false) {
                                    PExOut.gen('/*! @brief User definition pins */');
                                    IdentifierTitle = true;
                                }
                                var identifierString = identifierNames[i].split(".")[2];
                                var identifierValue = identifierNames[i].split(".")[1];
                                PExOut.gen("#define " + identifierString + "_PORT    " + identifierValue.substr(0,3));
                                PExOut.gen("#define " + identifierString + "_PIN     " + identifierValue.substr(3) + "U");
                            }
                        }
                    }

                    if (numOfPins[pc] > 0) {
                        PExOut.gen('/*! @brief User number of configured pins */');
                        PExOut.gen('#define NUM_OF_CONFIGURED_PINS' + pc + ' ' + numOfPins[pc]);
                        PExOut.gen('/*! @brief User configuration structure */');
                        PExOut.gen('extern pin_settings_config_t g_pin_mux_InitConfigArr'+ pc +'[NUM_OF_CONFIGURED_PINS'+ pc +'];');
                    } else {
                        PExOut.gen('/*! @brief No pin was configured for this group or no need any configuration */');
                    }
                }
            }
            PExOut.gen('');
            PExOut.gen('');
            PExOut.gen('#if defined(__cplusplus)');
            PExOut.gen('}');
            PExOut.gen('#endif');
            PExOut.gen('');
            PExOut.gen('/*!');
            PExOut.gen(' * @}');
            PExOut.gen(' */');
            PExOut.gen('#endif /* _' + (chFileName + moduleNameSuffix).toUpperCase() + '_H_ */');
            PExOut.gen('');
            PExOut.gen('/' + makeString('*', 119));
            PExOut.gen(' * EOF');
            PExOut.gen(' ' + makeString('*', 118) + '/');
            PExOut.gen('');

            PExOut.setOutputFile(chFileName + moduleNameSuffix + ".c");
            clangOff();
            PExOut.gen('/*');
            PExOut.gen(PExProject.getYamlState());
            PExOut.gen(' */');
            clangOn();
            PExOut.gen("\n/**\n * @page misra_violations MISRA-C:2012 violations\n *");
            PExOut.gen(" * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *");
            PExOut.gen(" * @section [global]\n * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.\n * The cast is required to initialize a pointer with an unsigned long define, representing an address.\n *");
            PExOut.gen(" * @section [global]\n * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.\n * The cast is required to initialize a pointer with an unsigned long define, representing an address.\n *");
            PExOut.gen(" */\n");
            PExOut.gen('#include \"' + chFileName + moduleNameSuffix + '.h\"');
            for (var pc = 0; pc < allComponents.length; pc++) { // Pin configuration represented by tables in the UI of the tool
                var pin_listProperty = allComponents[pc].findItemBySymbol("pin_list");
                var numOfItems = pin_listProperty.getItemsCount();
                numOfPins[pc] = numOfItems;

                currentComponent = pc;
                if (coreId == componentCoreIds[pc]) {
                    configurationStrategy = configurationStrategies[pc];
                    // print yaml and function if at least 1 pin is configured, otherwise print ony yaml info
                    if( getNumberOfRoutedPins(allComponents[pc]) > 0) {
                        printYamlState(allComponents[pc].getYamlState());
                        if (registerDatabaseObject != null) {
                            if (allComponents[pc].isErrorInProperty()) {
                                PExOut.gen('    /* There are conflicts or other incorrect settings in the configuration, the code below is generated only for ');
                                PExOut.gen('     * those registers which are set correctly and without a conflict. Open this file in Pins Tool for more details. */');
                                PExOut.gen('');
                            }
                        }
                        printOneRegisterConfiguration(pc);
                        // mark that function has been printed
                        printedFunctions.push(configurationStrategy);
                    } else {
                        printYamlState(allComponents[pc].getYamlState());
                    }
                }
            }
            PExOut_gen('/' + makeString('*', 119));
            PExOut_gen(' * EOF');
            PExOut_gen(' ' + makeString('*', 118) + '/');
        }
    }
}

    // calls main function
    printDriver();
