/*
 * Copyright 2017-2019 NXP
 * To be used with UCT Tools. See Software License Agreement of UCT Tools.
 */

 // Header sections.
var FILE_NAME_SECTION = 'file_name';
var INCLUDES_SECTION = 'includes';
var DEFINES_SECTION = 'defines';
var PAL_INST_DEFINES_SECTION = 'inst_defines';
var GLOBAL_VARS_EXTERN_SECTION = 'global_vars_extern';
var MISRA_COMMENTS = 'misra_comments';
var INIT_FUNCTION_SECTION = 'init_functions';
var EXTERN_FUNCTION_SECTION = 'callback_function_extern';

// C Source sections.
var GLOBAL_VARS_SECTION  = 'global_vars';
var INIT_FUNCTION_VARS_SECTION =  'init_function_vars';
var MISRA_COMMENTS_C = 'misra_comments';
var INIT_FUNCTION_PREINIT_SECTION = 'init_function_preinit';
var INIT_FUNCTION_BODY_SECTION = 'init_function_body';
var INIT_FUNCTION_POSTINIT0_SECTION = 'init_function_postinit0';
var INIT_FUNCTION_POSTINIT1_SECTION = 'init_function_postinit1';
var INIT_FUNCTION_POSTINIT2_SECTION = 'init_function_postinit2';
var INIT_FUNCTION_POSTINIT3_SECTION = 'init_function_postinit3';

var NEW_LINE = "\n";
var TAB_SPACE = "\t";
//var ENABLE_DEBUG = true;


/// @function logDebugInfo
/// Log debug information if global variable ENABLE_DEBUG is defined and true.
/// @param {String} message.
/// Log debug information if global variable ENABLE_DEBUG is defined and true.
function logDebugInfo(message)
{
    // Using PExScriptContextAPI class we log debug information if
    // global variable ENABLE_DEBUG is defined and true.
    if (typeof ENABLE_DEBUG === 'undefined' || ENABLE_DEBUG === null || ENABLE_DEBUG == false)
    {
        return;
    }
    // Just pass the info further.
    scriptApi.logInfo(message);
}

/// @function addInclude
/// Generates include directive in C format.
/// @param {String} header file name.
/// Generates include directive in C format.
function addInclude(hFileName)
{
    return "#include \"" + hFileName + "\"\n";
}

/// @function addStdInclude
/// Generates include directive of C standard lib header.
/// @param {String} header file name.
/// Generates include directive of C standard lib header.
function addStdInclude(hFileName)
{
    return "#include <" + hFileName + ">\n";
}

/// @function addPalInstDefine
/// Encodes number of instances for PAL
/// @param {String} file name.
/// @param {String} symbol name.
/// Generates define directive in C format.
function addPalInstDefine(file, sym)
{
    return file + "_" + sym + "\n";
}

/// @function addPalDefine
/// Encodes pal cfg defines
/// @param {String} file name.
/// @param {String} symbol name.
/// @param {String} symbol value.
/// Generates define directive in C format.
function addPalDefine(file, sym, value)
{
    return file + "_" + sym + "  " + value + "\n";
}

/// @function addPalComment
/// Encodes pal cfg comments
/// @param {String} file name.
/// @param {String} comment.
/// Generates a comment line.
function addPalComment(file, comment)
{
    return "comment_" + file + "_" + comment + "\n";
}

/// @function addDefine 
/// Generates define directive in C format.
/// @param {String} symbol name.
/// @param {String} value.
/// Generates define directive in C format.
function addDefine(sym, val)
{
    return "#define " + sym + "  " + val + "\n";
}

/// @function declareVar
/// Declares a variable in C format.
/// @param {String} variable type.
/// @param {String} varibale name.
/// Declares a variable in C format.
function declareVar(type, sym)
{
    return type + " " + sym + ";" + NEW_LINE;
}

/// @function declareArray
/// Declares an array in C format.
/// @param {String} array type.
/// @param {String} array name.
/// @param {String} array length.
/// Declares a variable in C format.
function declareArray(type, sym, len)
{
    return type + " " + sym + "[" + len + "]" + ";" + NEW_LINE;
}

/// @function setPrioIRQ
/// Set interrupt prority using cmsis NVIC.
/// @param {String} interrupt number.
/// @param {String} interrupt priority.
/// Set interrupt prority using cmsis NVIC.
function setPrioIRQ(IRQn, priority)
{
    var out = "";

    out = "  NVIC_SetPriority(" + IRQn +"," + priority +");"+ NEW_LINE;
    return out;
}

/// @function setPrioIRQ
/// Enables interrupt using cmsis NVIC.
/// @param {String} interrupt number.
/// Enables interrupt using cmsis NVIC.
function enableIRQ(IRQn)
{
    var out = "";
    out = "  NVIC_EnableIRQ(" + IRQn +");"+ NEW_LINE;
    return out;
}

/// @function getStructMember
/// Gets a member of configuration struct.
/// @param {StructConfig} structure that will be converted.
/// @param {String} member name.
/// Gets a member of configuration struct.
function getStructMember(struct, name) {
    logDebugInfo("[getStructMember] struct: " + struct.getName());
    var childrens = struct.getChildren();//struct members

    for(var idx in childrens){
        var chld = childrens[idx];
        logDebugInfo("[getStructMember] Child name: " +  chld.getName());
        if(String(chld.getName()).equals(name)){
            return chld;
        }
    }
}

/// @function getImbricatedStructs
/// Converts an imbricated structure to String using C format.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} the name of the structure.
/// @param {String} padding.
function getImbricatedStructs(struct, name, offset)
{
    var out ="";//output string
    var children = struct.getChildren();//struct members

    out = offset + "." + name + ' = {' + NEW_LINE;
    for(var idx in children){
        if(idx != 0)
        {
            out +="," + NEW_LINE;
        }
        var chld = children[idx];
        var settingValClass = chld.getClass();
        if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
        {
            out += getImbricatedStructs(chld, chld.getName(), offset + "  ");
        }
        else
        {
            out += offset + "  ." + chld.getName() + ' = ' + chld.getStringValue();
        }
    }
    out += NEW_LINE + offset +"}";
    return out;
}

/// @function getStructAsString
/// Converts a structure to String using C format.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} the type of the C structure.
/// @param {String} the name of the structure.
function getStructAsString(struct, type, name){
    var children = struct.getChildren();//struct members
    var out ="";//output string

    out = "const " + type + " " + name + " = {" + NEW_LINE;

    for(var idx in children){

        if(idx != 0)
        {
            out +="," + NEW_LINE;
        }
        var chld = children[idx];
        var settingValClass = chld.getClass();
        if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
        {
            out += getImbricatedStructs(chld, chld.getName(), "  ");
        }
        else
        {
            out += "  ." + chld.getName() + ' = ' + chld.getStringValue();
        }

    }

    out += NEW_LINE + "};" + NEW_LINE;

    return out;
}

/// @function printImbricatedStructs
/// Converts an imbricated structure to String using C format.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} alignment offset.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
/// @param {boolean} specifies whether the text should start with `,\n`.
function printImbricatedStructs(struct, offset, id_prefix, startWithNewLine)
{
    var out ="";//output string
    var children = struct.getChildren();//struct members

    if (startWithNewLine){
        out +="," + NEW_LINE;
    }

    if (struct.isOptionSet("cStructField") && struct.isAvailable()){
        out += offset + "." + struct.getName().substring(id_prefix.length) + ' = {' + NEW_LINE;
    }
    var firstStructField = true;

    for(var idx in children){
        var chld = children[idx];
        var settingValClass = chld.getClass();
        if (chld.isOptionSet("cStructField") && chld.isAvailable()){
            if (firstStructField) {
                firstStructField = false;
            }
            else {
                out +="," + NEW_LINE;
            }

            if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
            {
                out += printImbricatedStructs(chld, offset + "  ", id_prefix, false);
            }
            else
            {
                out += offset + printConfigElement(chld, id_prefix);
            }
        }
        else
        {
            if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
            {
                out += printImbricatedStructs(chld, "", id_prefix, true);
            }
        }
    }

    if (struct.isOptionSet("cStructField") && struct.isAvailable()){
        out += NEW_LINE + offset +"}";
    }

    return out;
}

/// @function printConfigStruct
/// Returns configuration structure string based on UI settings.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} the type of the C structure.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
function printConfigStruct(struct, type, id_prefix){
    var children = struct.getChildren();//struct members
    var out ="";//output string
    var constStruct = "\n";//const qualifier for the structure
    var structName = "";//config structure name


    for(var idx in children){
        var chld = children[idx];
        if (chld.getName().equals("name")){
            structName = chld.getStringValue();
        }
        if (chld.getName().equals("readonly") && chld.getStringValue().equals("true")){
            constStruct = "\nconst ";
        }
    }

    out = constStruct + type + " " + structName + " = {" + NEW_LINE;
    var firstStructField = true;
    for(var idx in children){
        var chld = children[idx];
        var settingValClass = chld.getClass();
        if (chld.isOptionSet("cStructField") && chld.isAvailable()){
            if (firstStructField) {
                firstStructField = false;
            }
            else {
                out +="," + NEW_LINE;
            }

            if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
            {
                out += printImbricatedStructs(chld, "  ", id_prefix, false);
            }
            else
            {
                out += printConfigElement(chld, id_prefix);
            }
        }
        else
        {
            if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
            {
                if (firstStructField) {
                    firstStructField = false;
                    out += printImbricatedStructs(chld, "", id_prefix, false);
                }
                else {
                    out += printImbricatedStructs(chld, "", id_prefix, true);
                }
            }
        }
    }

    out += NEW_LINE + "};" + NEW_LINE;

    return out;
}

///@function printConfigElement
///Returns element string based on UI settings.
/// @param {ScalarConfig} the configuration element object that needs to be printed.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
function printConfigElement(element, id_prefix) {
    var out = "  ." + element.getName().substring(id_prefix.length) + ' = ' + element.getValue();
    if(element.getTypeName() == "integer")
    {
        var intType = String(element.getModelData().getType());
        if (intType.startsWith("u"))
        {
            out += "U";
        }
        if (intType.endsWith("32_t"))
        {
            out += "L";
        }
        if (intType.endsWith("64_t"))
        {
            out += "LL";
        }
    }
    return out;
}

/// @function getSetting
/// Gets a setting obj using  an ID.
/// @param {ConfigSet} UI settings collection.
/// @param {String} setting ID.
/// Gets a setting obj using  an ID.
function getSetting(uiConfig, settingId)
{
    var childrenList = uiConfig.getChildren();
    logDebugInfo("[getSetting] settingId: " + settingId);
    for(var idx in childrenList)
    {
        var children = childrenList[idx];
        logDebugInfo("[getSetting] children.getId(): " + children.getId());

        if(String(children.getId()).equals(settingId))
        {
            logDebugInfo("[getSetting] children: " + children);
            return children;
        }
    }

    return null;
}

/// @function getSetting
/// Gets a setting value using  an ID.
/// @param {ConfigSet} UI settings collection.
/// @param {String} setting ID.
/// Gets a setting value using  an ID.
function getSettingsValue(uiConfig, settingId)
{
    var childrenList = uiConfig.getChildren();
    logDebugInfo("[getSettingsValue] settingId: " + settingId);

    for(var idx in childrenList)
    {
        var children = childrenList[idx];
        logDebugInfo("[getSettingsValue] children.getId()" + children.getId());
        if(String(children.getId()).equals(settingId))
        {
            logDebugInfo("[getSetting] children.getValue(): " + children.getValue());
            return children.getValue();
        }
    }

    return null;
}

/// @function occurrences
/// Counts occurrences of a substring in a string.
/// @param {String} string - The string
/// @param {String} subString - The sub string to search for
/// Counts occurrences of a substring in a string.
function occurrences(string, subString, allowOverlapping) {

    string += "";
    subString += "";
    if (subString.length <= 0) return (string.length + 1);

    var n = 0,
        pos = 0,
        step = subString.length;

    while (true) {
        pos = string.indexOf(subString, pos);
        if (pos >= 0) {
            ++n;
            pos += step;
        } else break;
    }
    return n;
}
