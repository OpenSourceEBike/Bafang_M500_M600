/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

/**
 * @function printConfigElement
 * Returns element string based on UI settings.
 * @param {ScalarConfig} the configuration element object that needs to be printed.
 * @param {String} prefix of the field ID that will be truncated from the returned string. 
 */
function printConfigElement(element, id_prefix) {
    var name = element.getName().substring(id_prefix.length);
    var out = "  ." + name + ' = ' + element.getValue();
    if(!isNaN(element.getValue()))
    {
        if(name == "rxFifoDMAChannel")
        {
            out += "U";
        }
        else
        {
            if (!(String(element.getValue()) == "true") && !(String(element.getValue()) == "false"))
                out += "UL";
        }
    }
    return out;
}

/**  
 * @function printImbricatedStructs
 * Converts an imbricated structure to String using C format.
 * @param {StructSetting} the structure object that needs to be converted.
 * @param {String} alignment offset.
 * @param {String} prefix of the field ID that will be truncated from the returned string.
 * @param {boolean} specifies whether the text should start with `,\n`. 
 */
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
/** 
 * @function printConfigStruct
 * Returns configuration structure string based on UI settings.
 * @param {StructSetting} the structure object that needs to be converted.
 * @param {String} the type of the C structure.
 * @param {String} prefix of the field ID that will be truncated from the returned string. 
 */
function printConfigStruct(struct, type, id_prefix){
    var children = struct.getChildren();//struct members
    var out ="";           //output string
    var constStruct = "\n";//const qualifier for the structure
    var structName = "";   //config structure name


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

function printPNConfigStruct(struct, type)
{
    var children = struct.getChildren();//struct members
    var out ="";           //output string
    var constStruct = "\n";//const qualifier for the structure
    var structName = "";   //config structure name
    var offset = "    ";
    for(var idx in children){
        var chld = children[idx];
        if (chld.getName().equals("name")){
            structName = chld.getStringValue();
        }
    }
    if (struct.getChildById("flexcan_pn_config").getChildById("readonly").getValue())
    {
            constStruct = "\nconst ";
    }
        
    out += constStruct + type + " " + structName + " = {" + NEW_LINE;
    out +=".wakeUpTimeout = "+struct.getChildById("flexcan_pn_config").getChildById("wakeUpTimeout").getValue()+ "," + NEW_LINE;
    out +=".wakeUpMatch = "+struct.getChildById("flexcan_pn_config").getChildById("wakeUpMatch").getValue()+ "," + NEW_LINE;
    out +=".numMatches = "+struct.getChildById("flexcan_pn_config").getChildById("numMatches").getValue()+ "UL," + NEW_LINE;
    out +=".matchTimeout = "+struct.getChildById("flexcan_pn_config").getChildById("matchTimeout").getValue()+ "UL," + NEW_LINE;
    if(struct.getChildById("flexcan_pn_config").getChildById("Ena_Payload").getValue()){
        if(struct.getChildById("flexcan_pn_config").getChildById("numMatches").getValue()>0)
            out +=".filterComb = FLEXCAN_FILTER_ID_PAYLOAD_NTIMES," + NEW_LINE;
        else
            out +=".filterComb = FLEXCAN_FILTER_ID_PAYLOAD," + NEW_LINE;
    }
    else
    {
        if(struct.getChildById("flexcan_pn_config").getChildById("numMatches").getValue()>0)
            out +=".filterComb = FLEXCAN_FILTER_ID_NTIMES," + NEW_LINE;
        else
            out +=".filterComb = FLEXCAN_FILTER_ID," + NEW_LINE;
    }
    out += ".idFilter1 = {" + NEW_LINE;
        if(struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("ID_Pn_Type").getValue() == "Extended")
        {
            out += offset+".extendedId = true,"+NEW_LINE;
        }
        else
        {
            out += offset+".extendedId = false,"+NEW_LINE;
        }
        if(struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("Frame_Pn_Type").getValue() == "Remote Frame")
        {
            out += offset+".remoteFrame = true,"+NEW_LINE;
        }
        else
        {
            out += offset+".remoteFrame = false,"+NEW_LINE;
        }
        out += offset+".id = "+struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("id1").getValue()+"UL,"+NEW_LINE;
    out += offset+"},"+ NEW_LINE;
    out += ".idFilter2 = {" + NEW_LINE;
        if(struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("ID_Pn_Type").getValue() == "Any")
           {
               out += offset+".extendedId = false,"+NEW_LINE;
           }
           else
           {
               out += offset+".extendedId = true,"+NEW_LINE;
           }
           if(struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("Frame_Pn_Type").getValue() == "Any")
           {
               out += offset+".remoteFrame = false,"+NEW_LINE;
           }
           else
           {
               out += offset+".remoteFrame = true,"+NEW_LINE;
           }
        if((struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("idFilterType").getValue() == "FLEXCAN_FILTER_MATCH_EXACT") ||
        (struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("idFilterType").getValue() == "FLEXCAN_FILTER_MATCH_RANGE"))
        {
             out += offset+".id = "+struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("id2").getValue()+"UL,"+NEW_LINE;
        }
        else
        {
            out += offset+".id = "+struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("id2_i").getValue()+"UL,"+NEW_LINE;
        }

    out +=  offset+"},"+ NEW_LINE;
    out += ".idFilterType = "+struct.getChildById("flexcan_pn_config").getChildById("idFilter").getChildById("idFilterType").getValue()+","+NEW_LINE;
    out += ".payloadFilterType = "+struct.getChildById("flexcan_pn_config").getChildById("payloadFilter").getChildById("PayloadFilterType").getValue()+","+NEW_LINE;
    out += ".payloadFilter = {" + NEW_LINE;    
        out += offset+".dlcLow = "+struct.getChildById("flexcan_pn_config").getChildById("payloadFilter").getChildById("dlcLow").getValue()+"UL,"+NEW_LINE;
        out += offset+".dlcHigh = "+struct.getChildById("flexcan_pn_config").getChildById("payloadFilter").getChildById("dlcHigh").getValue()+"UL,"+NEW_LINE;
        var Payload_Items = struct.getChildById("flexcan_pn_config").getChildById("payloadFilter").getChildById("Payload_1").getChildren()

         out += offset+".payload1 = {" +Payload_Items[0].getChildById("B0").getValue()+"UL,"
         out += Payload_Items[0].getChildById("B1").getValue()+"UL,"+Payload_Items[0].getChildById("B2").getValue()+"UL,"+Payload_Items[0].getChildById("B3").getValue()+"UL,"
         out += Payload_Items[0].getChildById("B4").getValue()+"UL,"+Payload_Items[0].getChildById("B5").getValue()+"UL,"+Payload_Items[0].getChildById("B6").getValue()+"UL,"
         out += +Payload_Items[0].getChildById("B7").getValue()+"UL},"+ NEW_LINE;
         
         var Payload_Items = struct.getChildById("flexcan_pn_config").getChildById("payloadFilter").getChildById("Payload_2").getChildren()
         out += offset+".payload2 = {" +Payload_Items[0].getChildById("B0").getValue()+"UL,"
         out += Payload_Items[0].getChildById("B1").getValue()+"UL,"+Payload_Items[0].getChildById("B2").getValue()+"UL,"+Payload_Items[0].getChildById("B3").getValue()+"UL,"
         out += Payload_Items[0].getChildById("B4").getValue()+"UL,"+Payload_Items[0].getChildById("B5").getValue()+"UL,"+Payload_Items[0].getChildById("B6").getValue()+"UL,"
         out += +Payload_Items[0].getChildById("B7").getValue()+"UL},"+ NEW_LINE;
    out += "}"+ NEW_LINE;
    
    out += "};" + NEW_LINE;
    return out;
}

/* FLEXCAN component code generator class. */
function FLEXCANCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    /* Returns the class name. */
    this.toString = function () {
        return "FLEXCANCodeGen";
    }

    /* Adds component specific info to header file sections. */
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        /* Get childContext class to extract peripheral name and functional group. */
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName()
        var fnGroupName = childContext.getFunctionalGroup().getName();
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".flexcanCfg");
        var configStructNameSetting = initStruct.getChildById("name");
        var configStructName = configStructNameSetting.getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();
        var stateStruct = getSettingsValue(this.uiConfig, this.uiConfig.getName()+".flexcan_state_name");
        var configPN = getSetting(this.uiConfig, this.uiConfig.getName()+".flexcanPN");
       
        
        /* Add Includes info. */
        hFileSections[INCLUDES_SECTION] += addInclude("flexcan_driver.h");
        
        var enablePN = configPN.getChildById("pnOn").getValue();
        var PNAvailable = getSetting(this.uiConfig, this.uiConfig.getName()+".instGet").getValue().get(3);
        
        
        if(initStruct != null)
        {
            /* Add Defines info. */
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(" + peripheralName.substring(3, 4) + "U)");
            hFileSections[DEFINES_SECTION] += declareVar("\nextern flexcan_state_t", stateStruct);
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexcan_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexcan_user_config_t", configStructName);
            }
        }
        
        if(enablePN == true && PNAvailable == true)
        {
            /* Check ReadOnly for PN config */
            if(configPN.getChildById("flexcan_pn_config").getChildById("readonly").getValue())
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexcan_pn_config_t", configPN.getChildById("name").getStringValue());
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexcan_pn_config_t", configPN.getChildById("name").getStringValue());
            }    
        }    
    }

    /* Adds component specific info to source file sections. */
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".flexcanCfg");
            var stateStruct = getSettingsValue(this.uiConfig, this.uiConfig.getName()+".flexcan_state_name");
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var fnGroupName = childContext.getFunctionalGroup().getName();
            var configPN = getSetting(this.uiConfig, this.uiConfig.getName()+".flexcanPN");
            var enablePN = configPN.getChildById("pnOn").getValue();
            var enableFD = initStruct.getChildById("flexcan_cfg_fd_enable").getValue();
            var FDAvailable = getSetting(this.uiConfig, this.uiConfig.getName()+".instGet").getValue().get(0);
            var PNAvailable = getSetting(this.uiConfig, this.uiConfig.getName()+".instGet").getValue().get(3);

            if (PNAvailable == false)
            {
                configPN.getChildById("pnOn").setValue('false');
            }

            if (FDAvailable == false)
            {
                initStruct.getChildById("flexcan_cfg_payload").setValue('FLEXCAN_PAYLOAD_SIZE_8');
                initStruct.getChildById("flexcan_cfg_fd_enable").setValue('false');
            }
            
            if(initStruct != null)
            {
                /* Add FLEXCAN state structure. */
                cFileSections[GLOBAL_VARS_SECTION] += declareVar("\nflexcan_state_t", stateStruct);
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(initStruct, "flexcan_user_config_t", "flexcan_cfg_");
            }
            
            if(enablePN == true && PNAvailable == true)
            {
                 cFileSections[GLOBAL_VARS_SECTION] += printPNConfigStruct(configPN, "flexcan_pn_config_t");
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
