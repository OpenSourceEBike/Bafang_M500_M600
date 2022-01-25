/*
 * Copyright 2018 NXP
 * To be used with UCT Tools. See Software License Agreement of UCT Tools.
 */

// scriptApi.requireScript("codegeneratorUtils.js");

///@function printConfigElement
///Returns element string based on UI settings.
/// @param {ScalarConfig} the configuration element object that needs to be printed.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
function printConfigElement(element, id_prefix) {
    var name = element.getName().substring(id_prefix.length);
    var out = "  ." + name + ' = ' + element.getValue();
    if(!isNaN(element.getValue()))
    {
        if (!(String(element.getValue()) == "true") && !(String(element.getValue()) == "false"))
            out += "UL";
    }
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

    if (startWithNewLine && struct.isAvailable()){
        out +="," + NEW_LINE;
    }

    if (struct.isOptionSet("cStructField") && struct.isAvailable()){
        var name = struct.getName().substring(id_prefix.length);
        if (name[name.length -1] == "_")
            name = name.substring(0, name.length - 1);
        out += offset + "." + name + ' = {' + NEW_LINE;
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
                out += printImbricatedStructs(chld, "", id_prefix, true);
            }
        }
    }

    out += NEW_LINE + "};" + NEW_LINE;

    return out;
}

// CAN_PAL component code generator class.
function CAN_PALCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "CAN_PALCodeGen";
    }

    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "can_pal_cfg.h\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if ((peripheralName.indexOf("CAN") == 0) || (peripheralName.indexOf("FLEXCAN") == 0)) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("can_pal_cfg.h", "CAN_OVER_FLEXCAN", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("can_pal_cfg.h", "NO_OF_FLEXCAN_INSTS_FOR_CAN");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName()

        var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".can_user_config_t2");
        var configStructNameSetting = initStruct.getChildById("name");
        var configStructName = configStructNameSetting.getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();

        var instStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".can_instance_t2");
        var instStructNameSetting =instStruct.getChildById("name");
        var instStructName = instStructNameSetting.getStringValue();
        var constInitStruct = instStruct.getChildById("readonly").getValue();

        var rxFifoExtStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".can_fifo_ext2");
        var rxFifoStructNameSetting =rxFifoExtStruct.getChildById("name");
        var rxFifoStructName = rxFifoStructNameSetting.getStringValue();
        var rxFifoExtEnableStruct = rxFifoExtStruct.getChildById("isRxFIFO_Enable");
        var rxFifoExtEnable = rxFifoExtEnableStruct.getValue();
        var rxFifostructExt = rxFifoExtStruct.getChildById("struct_extension");
        var rxFifoReadOnly = rxFifostructExt.getChildById("readonly").getValue();

        hFileSections[INCLUDES_SECTION] += addInclude("can_pal.h");

                    //Add external global variables
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration */" + NEW_LINE;

        if(rxFifostructExt != null & rxFifoExtEnable)
        {
            if(rxFifoReadOnly)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern const extension_flexcan_rx_fifo_t", rxFifoStructName);
            }
            else
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern extension_flexcan_rx_fifo_t", rxFifoStructName);
            }
        }

        if(initStruct != null)
        {
            //Add Defines info.
            // hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), peripheralName.substring(4, 5));
            // hFileSections[DEFINES_SECTION] += declareVar("\nextern can_user_config_t", initStruct);
            // hFileSections[DEFINES_SECTION] += declareVar("\nextern can_instance_t", instStruct);

            if (constStruct)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern const can_user_config_t", configStructName);
            }
            else
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern can_user_config_t", configStructName);
            }
        }

        // Add Includes info.
        if(instStruct != null)
        {
            if (constInitStruct)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern const can_instance_t", instStructName);
            }
            else
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern can_instance_t", instStructName);
            }
        }

    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var confStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".can_user_config_t2");
            var instStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".can_instance_t2");
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var profile = scriptApi.getProfile();
            var cpuName = profile.getMcuInfo().getPartNumber();

            var rxFifoExtStruct =  getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".can_fifo_ext2")
            var rxFifoExtEnableStruct = rxFifoExtStruct.getChildById("isRxFIFO_Enable");
            var rxFifoExtEnable = rxFifoExtEnableStruct.getValue();
            var enableFD = confStruct.getChildById("enableFD");
            var maxMB = confStruct.getChildById("maxMB");
            var payloadSize = confStruct.getChildById("payloadSize");

            // Set instance idx value for CAN PAL
            var instidx = instStruct.getChildById("instIdx");
            instidx.setValue(peripheralName.match(/\d+$/)[0]);

            /* Calculate maximum message buffer for every instance */
            switch (cpuName)
            {
                case "S32K116":
                case "S32K118":
                case "S32K148":
                    maxMB.setValue(32);
                    break;
                case "S32K142W":
                case "S32K144W":
                    maxMB.setValue(64);
                    break;
                case "S32K142":
                case "S32K144":
                    if (instidx.getValue() == 0)
                    {
                        maxMB.setValue(32);
                    }
                    else
                    {
                        maxMB.setValue(16);
                        enableFD.setValue(false);
                    }
                    break;
                case "S32K146":
                    if (instidx.getValue() < 2)
                    {
                        maxMB.setValue(32);
                    }
                    else
                    {
                        maxMB.setValue(16);
                        enableFD.setValue(false);
                    }
                    break;
                default :
                    /* Do nothing */
                    break;
            }

            /* Payload size can be only 8 bytes if Flexible Data Rate not enabled */
            if (!enableFD.getValue())
            {
                payloadSize.setValue("CAN_PAYLOAD_SIZE_8");
            }

            if(rxFifoExtStruct != null)
            {
                if(rxFifoExtEnable == true)
                {
                    var rxFifoExtNameStruct = rxFifoExtStruct.getChildById("name");
                    var rxFifoExtName = rxFifoExtNameStruct.getStringValue();
                    var rxFifoStruct = rxFifoExtStruct.getChildById("struct_extension");
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(rxFifoStruct, "extension_flexcan_rx_fifo_t " + rxFifoExtName , "");
                }
            }

            if(instStruct != null)
            {
                // Add CAN PAL instance structure.
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(instStruct, "can_instance_t", "");
            }

            if(confStruct != null)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(confStruct, "can_user_config_t", "");
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
