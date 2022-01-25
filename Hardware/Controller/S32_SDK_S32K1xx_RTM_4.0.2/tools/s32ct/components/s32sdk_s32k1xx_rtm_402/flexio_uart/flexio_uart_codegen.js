/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function unique(value, index, self) { 
    return self.indexOf(value) === index;
}

// Flexio_uart component code generator class.
function FlexioUartCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FlexioUARTCodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
		var callbacks = [];
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
		/* Get configurations */
        var flexioUartConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_uart_Configuration");
        var flexioUartConfigurationsChildren = flexioUartConfigurations.getChildren();
        var flexioUartConfigurationsCount = flexioUartConfigurationsChildren.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("flexio_uart_driver.h");

        //Add Defines info.
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(0U)");
        hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioUART configuration structure */";

        for (var idx = 0; idx < flexioUartConfigurationsCount; idx++)
        {
            var configStructName = flexioUartConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioUartConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_uart_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_uart_user_config_t", configStructName);
            }
        }

        /* Callback definitions */
        for (var idx = 0; idx < flexioUartConfigurationsCount; idx++)
        {
            var callbackName = flexioUartConfigurationsChildren[idx].getChildById("callback").getStringValue();
		    if((callbackName != 'NULL')&&(callbackName != ''))
			{
                callbacks.push(callbackName);
			}
        }
        callbacks = callbacks.filter(unique);
        if(callbacks.length > 0)
        {
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of user callback function */";
        }
        for (i = 0; i < callbacks.length; i++)
        {
			hFileSections[DEFINES_SECTION] += declareVar("\nextern void", callbacks[i]+"(void *driverState, uart_event_t event, void *userData)");
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
        // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configName = childContext.getComponentInstanceConfig().getName();
            /* Get configurations */
            var flexioUartConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_uart_Configuration");
            var flexioUartConfigurationsChildren = flexioUartConfigurations.getChildren();
            var flexioUartConfigurationsCount = flexioUartConfigurationsChildren.size();

            // Add config structures
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioUART configuration structure */";
            for (var idx = 0; idx < flexioUartConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioUartConfigurationsChildren[idx], "flexio_uart_user_config_t", "");
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}