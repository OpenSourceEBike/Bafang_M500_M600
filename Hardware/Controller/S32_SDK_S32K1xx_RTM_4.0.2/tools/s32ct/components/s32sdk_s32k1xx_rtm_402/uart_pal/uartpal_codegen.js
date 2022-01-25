/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* default value when blank on GUI */
function SetDefaultValue(parent, child, defvalue)
{
    var childvalue = parent.getChildById(child);

    if (childvalue.getStringValue() == "")
    {
        childvalue.setValue(defvalue);
    };
}

/* UART PAL component code generator class. */
function UART_PALCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    /* Returns the class name. */
    this.toString = function () {
        return "UART_PALCodeGen";
    }

    /* Generates the pal_cfg.h file */
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "uart_pal_cfg.h\n";
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        if (peripheralName.indexOf("LINFlexD") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("uart_pal_cfg.h", "UART_OVER_LINFLEXD", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("uart_pal_cfg.h", "NO_OF_LINFLEXD_INSTS_FOR_UART");
        }
        else if (peripheralName.indexOf("LPUART") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("uart_pal_cfg.h", "UART_OVER_LPUART", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("uart_pal_cfg.h", "NO_OF_LPUART_INSTS_FOR_UART");
        }
        else if (peripheralName.indexOf("FLEXIO") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("uart_pal_cfg.h", "UART_OVER_FLEXIO", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("uart_pal_cfg.h", "NO_OF_FLEXIO_INSTS_FOR_UART");
        }
    }

    /* Adds component specific info to header file sections. */
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        /* Get childContext class to extract peripheral name and functional group.*/
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
        var fnGroupName = childContext.getFunctionalGroup().getName();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("uart_pal.h");
        hFileSections[INCLUDES_SECTION] += "#include <stddef.h>\n";

        //Add external global variables
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configuration */" + NEW_LINE;

        /* Extern instance configuration */
        var instStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".uartPalInst");
        var instStructNameSetting =instStruct.getChildById("name");
        var instStructName = instStructNameSetting.getStringValue();
        var constInstStruct = instStruct.getChildById("readonly").getValue();

        if(instStruct != null)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Device instance information */" + NEW_LINE;
            if (constInstStruct)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const uart_instance_t", instStructName);
            }
            else
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern uart_instance_t", instStructName);
            }
        }

        /* Extern initialize configuration */
        var uartCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".uartCfgArray").getChildren();
        var uartCfgArray_Size = uartCfgArray_Items.size();

        if ((uartCfgArray_Items != null) && (uartCfgArray_Size > 0))
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Serial module configurations */" + NEW_LINE;
            // Iterate through all configurations
            for (var i =  0; i < uartCfgArray_Size; i++)
            {
                var commonUartSetting = uartCfgArray_Items[i].getChildById("uartCommonConfig");
                var commonConfigName = commonUartSetting.getChildById("name").getStringValue();;
                var constInitStruct = commonUartSetting.getChildById("readonly").getValue();
                var txCallback = commonUartSetting.getChildById("txCallback");
                var rxCallback = commonUartSetting.getChildById("rxCallback");

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief UART PAL User Configuration " + i + " */" + NEW_LINE;
                if (constInitStruct)
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const uart_user_config_t " + commonConfigName + ";\n";
                }
                else
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern uart_user_config_t " + commonConfigName + ";\n";
                }

                /* Extern callback function if they are available */
                if((txCallback.getStringValue() != "NULL") || ((rxCallback.getStringValue() != "NULL")))
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Define external callbacks for RX and TX */" + NEW_LINE;
                    if (rxCallback.getStringValue() != "NULL")
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void "+ rxCallback.getStringValue() + "(void *driverState, uart_event_t event, void *userData);" + NEW_LINE;
                    }
                    if (txCallback.getStringValue() != "NULL")
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void "+ txCallback.getStringValue() + "(void *driverState, uart_event_t event, void *userData);" + NEW_LINE;
                    }
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            /* Get childContext class to extract peripheral name and functional group. */
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configName = childContext.getComponentInstanceConfig().getName()
            var fnGroupName = childContext.getFunctionalGroup().getName();

            if (peripheralName != 'FLEXIO')
            {
                var periphnumb = peripheralName.match(/\d+$/)[0];
                var periphtype = peripheralName.replace(periphnumb,'');
            }

            /* Process parameters about instance configuration */
            var instStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".uartPalInst");

            if (instStruct != null)
            {
                var instidx = instStruct.getChildById("instIdx");
                var insttype = instStruct.getChildById("instType");

                if (periphtype == 'LPUART_')
                {
                    var newinstname = "uart_instance" + "_" + "lu" + periphnumb;

                    instidx.setValue(periphnumb);
                    insttype.setValue('UART_INST_TYPE_LPUART');
                }
                else if (peripheralName == 'FLEXIO')
                {
                    var newinstname = "uart_instance" + "_" + "fi0";

                    instidx.setValue(0);
                    insttype.setValue('UART_INST_TYPE_FLEXIO_UART');
                }
                else if (periphtype == 'LINFlexD_')
                {
                    var newinstname = "uart_instance" + "_" + "lf" + periphnumb;

                    instidx.setValue(periphnumb);
                    insttype.setValue('UART_INST_TYPE_LINFLEXD');
                }
                else
                {
                    /* Do nothing */
                }

                /* Generate instance structure code in c file */
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(instStruct, "uart_instance_t", "");
            }

            /* Process parameters about extension */
            var uartCfgArray_Items = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".uartCfgArray").getChildren();
            var uartCfgArray_Size = uartCfgArray_Items.size();

            if ((uartCfgArray_Items != null) && (uartCfgArray_Size > 0))
            {
                for (var i =  0; i < uartCfgArray_Size; i++)
                {
                    var commonUartSetting = uartCfgArray_Items[i].getChildById("uartCommonConfig");
                    var commonConfigName = commonUartSetting.getChildById("name");
                    var extStruct = uartCfgArray_Items[i].getChildById("uartExtConfig");
                    var txpin = extStruct.getChildById("dataPinTx");
                    var rxpin = extStruct.getChildById("dataPinRx");
                    var extStructNameSetting = extStruct.getChildById("name");

                    var desiredBaudRate = commonUartSetting.getChildById("baudRate").getValue();

                    if(peripheralName.indexOf("LPUART") == 0)
                    {
                        var lpuartClk = commonUartSetting.getChildById("lpuartClock").getValue();
                        var sbrTemp, j;
                        var tempDiff, baudDiff, maxOsr;
                        var actualBaud = 0;

                        var osr = 4;
                        var sbr = parseInt(lpuartClk / (desiredBaudRate * osr));

                        var calculatedBaud = parseInt(lpuartClk / (osr * sbr));

                        if (calculatedBaud > desiredBaudRate)
                        {
                            baudDiff = calculatedBaud - desiredBaudRate;
                        }
                        else
                        {
                            baudDiff = desiredBaudRate - calculatedBaud;
                        }
                        /* find maximum osr */
                        maxOsr = lpuartClk / desiredBaudRate;
                        if (maxOsr > 32)
                        {
                            maxOsr = 32;
                        }
                        /* loop to find the best osr value possible, one that generates minimum baudDiff
                         * iterate through the rest of the supported values of osr */
                        if (maxOsr >= 5)
                        {
                            for (j = 5; j <= maxOsr; j++)
                            {
                                /* calculate the temporary sbr value   */
                                sbrTemp = parseInt(lpuartClk / (desiredBaudRate * j));
                                /* calculate the baud rate based on the temporary osr and sbr values */
                                calculatedBaud = parseInt(lpuartClk / (j * sbrTemp));

                                if (calculatedBaud > desiredBaudRate)
                                {
                                    tempDiff = calculatedBaud - desiredBaudRate;
                                }
                                else
                                {
                                    tempDiff = desiredBaudRate - calculatedBaud;
                                }

                                if (tempDiff <= baudDiff)
                                {
                                    baudDiff = tempDiff;
                                    osr = j;  /* update and store the best osr value calculated */
                                    sbr = sbrTemp;  /* update store the best sbr value calculated */
                                }
                            }
                        }

                        actualBaud = parseInt(lpuartClk / (osr * sbr));
                        commonUartSetting.getChildById("actualLpuartBaud").setValue(actualBaud);
                    }

                    if ((extStruct != null) && (peripheralName == 'FLEXIO'))
                    {
                        /* Generate extension structure code in c file */
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(extStruct, "static extension_flexio_for_uart_t", "");
                    }

                    /* set extension value */
                    var ext = commonUartSetting.getChildById("extension");
                    var newextname = extStruct.getChildById("name").getValue();

                    if (peripheralName == 'FLEXIO')
                    {
                        ext.setValue(newextname);
                    }
                    else
                    {
                        ext.setValue("NULL");
                    }

                    /* set default value when blank on GUI */
                    SetDefaultValue(commonUartSetting, "txCallback", "NULL");
                    SetDefaultValue(commonUartSetting, "txCallbackParam", "NULL");
                    SetDefaultValue(commonUartSetting, "rxCallback", "NULL");
                    SetDefaultValue(commonUartSetting, "rxCallbackParam", "NULL");

                    if (ext.getValue() != "NULL")
                    {
                        ext.setValue("&" + newextname);
                    }

                    /* Generate configuration structure code in c file */
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(commonUartSetting, "uart_user_config_t", "");

                    if (ext.getValue() != "NULL")
                    {
                        ext.setValue(newextname);
                    }
                }
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
