/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// lpuart component code generator class.
function LpUartCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "LpUartCodeGen";
    }


    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName()
        var fnGroupName = childContext.getFunctionalGroup().getName();
        var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".lpuart_configuration");

        var initStructArray = initStruct.getChildren();
        var initStructArraySize = initStructArray.size();
        var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".lpuart_state_name");

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("lpuart_driver.h");

        if(initStruct != null)
        {
            //Add Defines info.
            hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), peripheralName.match(/\d+$/)[0]);
            hFileSections[DEFINES_SECTION] += declareVar("\nextern lpuart_state_t", stateStruct);
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of LPUART configuration structure */";

            for (var idx = 0; idx < initStructArraySize; idx++)
            {
                var configStructName = initStructArray[idx].getChildById("name").getStringValue();
                var constStruct = initStructArray[idx].getChildById("readonly").getValue();
                if (constStruct)
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern const lpuart_user_config_t", configStructName);
                }
                else
                {
                    hFileSections[DEFINES_SECTION] += declareVar("\nextern lpuart_user_config_t", configStructName);
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            var initStruct = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".lpuart_configuration");
            var initStructArray = initStruct.getChildren();
            var initStructArraySize = initStructArray.size();
            var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".lpuart_state_name");
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var fnGroupName = childContext.getFunctionalGroup().getName();
            // Add lpuart state structure.
            cFileSections[GLOBAL_VARS_SECTION] += declareVar("\lpuart_state_t", stateStruct);

            if(initStruct != null)
            {
                for (var index = 0; index < initStructArraySize; index++)
                {
                    var clockValue = initStructArray[index].getChildById("lpuartClock").getValue();
                    var desiredBaudRate = initStructArray[index].getChildById("baudRate").getValue();
                    //Calculate actual baudrate
                    var actBaud = 0;
                    var idx = 0;
                    var osr = 4;
                    var sbr = parseInt(clockValue / (desiredBaudRate * osr));
                    var calculatedBaud = parseInt(clockValue / (osr * sbr));
                    var baudDiff, idx, sbrTemp, tempDiff;
                    if (calculatedBaud > desiredBaudRate)
                    {
                        baudDiff = calculatedBaud - desiredBaudRate;
                    }
                    else
                    {
                        baudDiff = desiredBaudRate - calculatedBaud;
                    }

                    maxOsr = parseInt(clockValue / desiredBaudRate);
                    if (maxOsr > 32)
                    {
                        maxOsr = 32;
                    }
                    if (maxOsr >= 5)
                    {
                        for (idx = 5; idx <= maxOsr; idx++)
                        {
                            sbrTemp = parseInt(clockValue / (desiredBaudRate * idx));
                            calculatedBaud = parseInt(clockValue / (idx * sbrTemp));

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
                                osr = idx;
                                sbr = sbrTemp;
                            }
                        }
                    }

                    actBaud = parseInt(clockValue / (osr * sbr));
                    initStructArray[index].getChildById("actBaudRate").setValue(actBaud);

                    // Add lpuart config structure.
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(initStructArray[index], "lpuart_user_config_t", "");
                }
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}