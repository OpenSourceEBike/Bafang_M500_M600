/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

/// @function LIN_DecToHex
/// Returns The heximal number (string).
/// @param {x} the decimal number (uint32_t).
function LIN_DecToHex(x) {
    return ('0x' + Math.abs(x).toString(16).toUpperCase());
}

scriptApi.requireScript("codegeneratorUtils.js");

// LIN component code generator class.
function LIN_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "LIN_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext      = uiConfig.getChildContext();
        var peripheralName    = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName      = "lin" + peripheralName.substring(7, 8);
        // Get LIN user configuration class
        var initLinStruct     = getSetting(this.uiConfig, this.uiConfig.getName()+".linUserConfiguation");
        // Get LIN state structure
        var getLinState       = initLinStruct.getChildById("linStateStructure").getStringValue();
        // Get LIN configuration array class
        var initLinConfig     = initLinStruct.getChildById("linConfiguration");
        // Get LIN configuration childContext class
        var linConfigChildren = initLinConfig.getChildren();
        // Check LIN state structure name
        if (getLinState == "")
        {
            initLinStruct.getChildById("linStateStructure").setValue(instanceName + "_State");
            getLinState       = initLinStruct.getChildById("linStateStructure").getStringValue();
        }

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("stddef.h");
        hFileSections[INCLUDES_SECTION] += addInclude("lin_driver.h");
        //Add Defines info.
        hFileSections[DEFINES_SECTION] += "\n/*! @brief Device instance number */\n";
        var linLpuartInstance = peripheralName.substring(7, 8);
        if (linLpuartInstance == "")
        {
            linLpuartInstance = 0;
        }
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(" + linLpuartInstance + "U)");

        /* LIN user configuration is available */
        if ((initLinConfig.length != 0) && initLinConfig.isAvailable() && (linConfigChildren.length > 0))
        {
            /*! Driver state structure */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE + "/*! @brief LIN State structure */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern lin_state_t", getLinState);
            /*! @brief Structure storing LIN user configuration information */
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE + "/*! @brief Extern LIN user configuration structure */" + NEW_LINE;
            for (var i = 0; i < linConfigChildren.length; i++)
            {
                var userNameConfig     = linConfigChildren[i].getChildById("name").getStringValue();
                /* Extern LIN user configuration  */
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Extern LIN configuration " + userNameConfig + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + "lin_user_config_t", userNameConfig);
            }
            /*! @brief Extern Timer get time interval callback function */
            for (var i = 0; i < linConfigChildren.length; i++)
            {
                if (linConfigChildren[i].getChildById("timerGetTimeIntervalCallback").getValue() != "NULL")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Extern Timer get time interval callback function" + i + " */" + NEW_LINE;
                    var nameCallbackFunction = linConfigChildren[i].getChildById("timerGetTimeIntervalCallback").getValue() + "(uint32_t *ns)";
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern uint32_t ", nameCallbackFunction);
                }
            }
        }
     }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            // Variables
            var childContext      = uiConfig.getChildContext();
            var peripheralName    = childContext.getComponentInstanceConfig().getPeripheral();
            var linInstanceName   = "lin" + peripheralName.substring(7, 8);
            // Get LIN user configuration class
            var initLinStruct     = getSetting(this.uiConfig, this.uiConfig.getName()+".linUserConfiguation");
            // Get LIN state structure
            var getLinState       = initLinStruct.getChildById("linStateStructure").getStringValue();
            // Get LIN configuration array class
            var initLinConfig     = initLinStruct.getChildById("linConfiguration");
            // Get LIN configuration childContext class
            var linConfigChildren = initLinConfig.getChildren();
            // Check LIN state structure name
            if (getLinState == "")
            {
                initLinStruct.getChildById("linStateStructure").setValue(linInstanceName + "_State");
                getLinState       = initLinStruct.getChildById("linStateStructure").getStringValue();
            }
            /* LIN user configuration is available */
            if ((initLinConfig.length != 0) && initLinConfig.isAvailable() && (linConfigChildren.length > 0))
            {
                var classicPidArr;
                /*! Driver state structure */
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief LIN State structure */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "lin_state_t " + getLinState + ";";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "\n/*! @brief LIN User Configurations structure */";
                for (var i = 0; i < linConfigChildren.length; i++)
                {
                    var getNumOfClassicPid = linConfigChildren[i].getChildById("numOfClassicPID").getChildren();

                    if (linConfigChildren[i].getChildById("classicPidType").getValue() == "Mixed")
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief LIN classicPIDS" + i + " */" + NEW_LINE;
                        classicPidArr = linInstanceName + "ClassicPIDS" + i
                        cFileSections[GLOBAL_VARS_SECTION] += "uint8_t " + classicPidArr + "["
                                                           + getNumOfClassicPid.length + "] = { "
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    "
                        for (var j = 0; j < getNumOfClassicPid.length; j++)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += LIN_DecToHex(getNumOfClassicPid[j].getChildById("numberclassicPID").getValue()) + ","
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
                    }
                }

                for (var i = 0; i < linConfigChildren.length; i++)
                {
                    var userNameConfig     = linConfigChildren[i].getChildById("name").getStringValue();
                    classicPidArr = linInstanceName + "ClassicPIDS" + i;
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/*! @brief LIN configuration " + userNameConfig + " */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "lin_user_config_t" + " " + userNameConfig + " = " + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .baudRate                     = " + linConfigChildren[i].getChildById("baudRate").getValue() + "UL,"
                                                                                                 + "                        /* LPUART baudRate */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .nodeFunction                 = " + linConfigChildren[i].getChildById("nodeFunction").getValue() + ","
                                                                                                 + "                    /* true - MASTER, false - SLAVE */"+ NEW_LINE;
                    if (linConfigChildren[i].getChildById("nodeFunction").getValue() == "(bool)MASTER")
                    {
                        linConfigChildren[i].getChildById("autobaudEnable").setValue(0);
                        cFileSections[GLOBAL_VARS_SECTION] += "    .autobaudEnable               = " +"false," + "                          /* Disable auto baudRate */" + NEW_LINE;
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .autobaudEnable               = " + linConfigChildren[i].getChildById("autobaudEnable").getValue() + ","
                                                                                                     + "                                    /* true - Enbale auto baudRate; false - Disable auto baudRate*/" + NEW_LINE;
                    }
                    if (linConfigChildren[i].getChildById("timerGetTimeIntervalCallback").getStringValue() == "")
                    {
                        linConfigChildren[i].getChildById("timerGetTimeIntervalCallback").setValue(linInstanceName + 'TimerGetTimeIntervalCallback' + i);
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "    .timerGetTimeIntervalCallback = " + linConfigChildren[i].getChildById("timerGetTimeIntervalCallback").getValue() + "," + NEW_LINE;
                    if (linConfigChildren[i].getChildById("classicPidType").getValue() == "Classic")
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .classicPID                   = NULL," + "                               /* ClassicPID */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .numOfClassicPID              = 255U" + "                                /* Number of classicPID */" + NEW_LINE;
                    }
                    else if (linConfigChildren[i].getChildById("classicPidType").getValue() == "Enhanced")
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .classicPID                   = NULL," + "                               /* ClassicPID */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .numOfClassicPID              = 0U" + "                                  /* Number of classicPID */" + NEW_LINE;
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .classicPID                   = " + classicPidArr + "," + "                             /* ClassicPID */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .numOfClassicPID              = " + linConfigChildren[i].getChildById("numOfClassicPID").getChildren().length + "U" + "                  /* Number of classicPID */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
                }
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
