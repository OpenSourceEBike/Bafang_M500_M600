/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function unique(value, index, self) { 
    return self.indexOf(value) === index;
}

// Flexio_i2c component code generator class.
function FlexioI2CCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FlexioI2CCodeGen";
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
        var flexioI2CConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2c_Configuration");
        var flexioI2CConfigurationsChildren = flexioI2CConfigurations.getChildren();
        var flexioI2CConfigurationsCount = flexioI2CConfigurationsChildren.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("flexio_i2c_driver.h");

        //Add Defines info.
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(0U)");
        hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioI2C configuration structure */";

        for (var idx = 0; idx < flexioI2CConfigurationsCount; idx++)
        {
            var configStructName = flexioI2CConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioI2CConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_i2c_master_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_i2c_master_user_config_t", configStructName);
            }
        }

        /* Callback definitions */
        for (var idx = 0; idx < flexioI2CConfigurationsCount; idx++)
        {
            var callbackName = flexioI2CConfigurationsChildren[idx].getChildById("callback").getStringValue();
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
			hFileSections[DEFINES_SECTION] += declareVar("\nextern void", callbacks[i]+"(i2c_master_event_t event, void *userData)");
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
            var flexioI2CConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2c_Configuration");
            var flexioI2CConfigurationsChildren = flexioI2CConfigurations.getChildren();
            var flexioI2CConfigurationsCount = flexioI2CConfigurationsChildren.size();

            // Add config structures
            cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioI2C configuration structure */";
            for (var idx = 0; idx < flexioI2CConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioI2CConfigurationsChildren[idx], "flexio_i2c_master_user_config_t", "");
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}