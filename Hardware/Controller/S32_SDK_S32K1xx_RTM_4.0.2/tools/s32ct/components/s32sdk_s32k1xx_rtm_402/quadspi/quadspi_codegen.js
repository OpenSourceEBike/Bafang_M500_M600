/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

///@function printConfigElementQuadSPI
///Returns element string based on UI settings.
/// @param {ScalarConfig} the configuration element object that needs to be printed.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
function printConfigElementQuadSPI(element, id_prefix) {
    if (element.isOptionSet("overrideName"))
    {
        var out = "  ." + element.getOptionValue("overrideName").substring(id_prefix.length) + ' = ' + element.getValue();
    }
    else
    {
        var out = "  ." + element.getName().substring(id_prefix.length) + ' = ' + element.getValue();
    }
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


/// @function printImbricatedStructsQuadSPI
/// Converts an imbricated structure to String using C format.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} alignment offset.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
/// @param {boolean} specifies whether the text should start with `,\n`.
function printImbricatedStructsQuadSPI(struct, offset, id_prefix, startWithNewLine, noName)
{
    var out ="";//output string
    var children = struct.getChildren();//struct members

    if (startWithNewLine){
        out +="," + NEW_LINE;
    }

    if (noName) {
        out += offset + '{' + NEW_LINE;
    }
    else if (struct.isOptionSet("cStructField") && struct.isAvailable()){
        out += offset + "." + struct.getName().substring(id_prefix.length) + ' = {' + NEW_LINE;
    }
    var firstStructField = true;

    for(var idx in children){
        var chld = children[idx];
        var settingValClass = chld.getClass();
        //if (chld.isOptionSet("cStructField") && chld.isAvailable()){
        if ((String(struct.getClass().getName()).equals("com.nxp.swtools.periphs.model.config.ArrayConfig")) || 
            (chld.isOptionSet("cStructField") && chld.isAvailable())) {
            if (firstStructField) {
                firstStructField = false;
            }
            else {
                out +="," + NEW_LINE;
            }

            if ((String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig")) ||
                (String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.ArrayConfig")))
            {
                if (String(struct.getClass().getName()).equals("com.nxp.swtools.periphs.model.config.ArrayConfig"))
                {
                    out += printImbricatedStructsQuadSPI(chld, offset + "  ", id_prefix, false, true);
                }
                else
                {
                    out += printImbricatedStructsQuadSPI(chld, offset + "  ", id_prefix, false, false);
                }
            }
            else
            {
                out += offset + printConfigElementQuadSPI(chld, id_prefix);
            }
        }
        else
        {
            if ((String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig")) || 
                (String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.ArrayConfig")))
            {
                if (String(struct.getClass().getName()).equals("com.nxp.swtools.periphs.model.config.ArrayConfig"))
                {
                    out += printImbricatedStructsQuadSPI(chld, "", id_prefix, true, true);
                }
                else
                {
                    out += printImbricatedStructsQuadSPI(chld, "", id_prefix, true, false);
                }
            }
        }
    }

    if (noName || (struct.isOptionSet("cStructField") && struct.isAvailable())){
        out += NEW_LINE + offset +"}";
    }

    return out;
}

/// @function printConfigStructQuadSPI
/// Returns configuration structure string based on UI settings.
/// @param {StructSetting} the structure object that needs to be converted.
/// @param {String} the type of the C structure.
/// @param {String} prefix of the field ID that will be truncated from the returned string.
function printConfigStructQuadSPI(struct, type, id_prefix){
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
                out += printImbricatedStructsQuadSPI(chld, "  ", id_prefix, false, false);
            }
            else
            {
                out += printConfigElementQuadSPI(chld, id_prefix);
            }
        }
        else
        {
            if(String(settingValClass.getName()).equals("com.nxp.swtools.periphs.model.config.StructConfig"))
            {
                if (firstStructField) {
                    firstStructField = false;
                    out += printImbricatedStructsQuadSPI(chld, "", id_prefix, false, false);
                }
                else {
                    out += printImbricatedStructsQuadSPI(chld, "", id_prefix, true, false);
                }
            }
        }
    }

    out += NEW_LINE + "};" + NEW_LINE;

    return out;
}


// QSPI component code generator class.
function QuadSPICodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "QuadSPICodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
        var fnGroupName = childContext.getFunctionalGroup().getName();
        var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".qspiCfg");
        var configStructNameSetting = initStruct.getChildById("name");
        //scriptApi.logError(configStructNameSetting);
        var configStructName = configStructNameSetting.getStringValue();
        var constStruct = initStruct.getChildById("readonly").getValue();
        var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".qspi_state_name");

        /* Slave and Master callback definition */
        var configStructCallback = initStruct.getChildById("callback");
		var callbackName = configStructCallback.getStringValue();
		
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("quadspi_driver.h");

        if(initStruct != null)
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(0U)");
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of QuadSPI state structure */";
			hFileSections[DEFINES_SECTION] += declareVar("\nextern qspi_state_t", stateStruct);
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of QuadSPI configuration structure */";
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const qspi_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern qspi_user_config_t", configStructName);
            }
			
		    if((callbackName != 'NULL')&&(callbackName != ''))
			{
                hFileSections[DEFINES_SECTION] += "\n/* External declaration of user callback function */";
			    hFileSections[DEFINES_SECTION] += declareVar("\nextern void", callbackName+"(uint32_t instance, void * param)");
			}
        }

    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".qspiCfg");
            var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".qspi_state_name");
            var childContext = uiConfig.getChildContext();
            var configName = childContext.getComponentInstanceConfig().getName();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var fnGroupName = childContext.getFunctionalGroup().getName();

            if(initStruct != null)
            {
                // Add state structure.
			    cFileSections[GLOBAL_VARS_SECTION] += "/* QuadSPI state structure */\n";
                cFileSections[GLOBAL_VARS_SECTION] += declareVar("qspi_state_t", stateStruct);
			    cFileSections[GLOBAL_VARS_SECTION] += "\n/* QuadSPI configuration structure */";
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStructQuadSPI(initStruct, "qspi_user_config_t", "");
                // Add driver init function.
                // FLEXCAN_DRV_Init(BOARD_InitPeripherals_FLEXCAN0_BASE, &BOARD_InitPeripherals_FLEXCAN0_state, &BOARD_InitPeripherals_FLEXCAN0_init_config);
                //cFileSections[INIT_FUNCTION_BODY_SECTION] += "  FLEXCAN_DRV_Init(" + fnGroupName + "_" + peripheralName + "_BASE, &" + fnGroupName+"_"+ peripheralName + "_state, &" + fnGroupName + "_" + peripheralName + "_init_config);\n";
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}