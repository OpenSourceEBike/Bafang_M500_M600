/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function unique(value, index, self) { 
    return self.indexOf(value) === index;
}

// Flexio_i2s component code generator class.
function FlexioI2SCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FlexioI2SCodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
		var callbacks = [];
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
		/* Get master configurations */
        var flexioI2SMasterConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2s_master_Configuration");
        var flexioI2SMasterConfigurationsChildren = flexioI2SMasterConfigurations.getChildren();
        var flexioI2SMasterConfigurationsCount = flexioI2SMasterConfigurationsChildren.size();

		/* Get slave configurations */
        var flexioI2SSlaveConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2s_slave_Configuration");
        var flexioI2SSlaveConfigurationsChildren = flexioI2SSlaveConfigurations.getChildren();
        var flexioI2SSlaveConfigurationsCount = flexioI2SSlaveConfigurationsChildren.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("flexio_i2s_driver.h");

        //Add Defines info.
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(0U)");
        //Master configurations.
        if(flexioI2SMasterConfigurationsCount > 0)
        {
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioI2S master configuration structure */";
        }
        for (var idx = 0; idx < flexioI2SMasterConfigurationsCount; idx++)
        {
            var configStructName = flexioI2SMasterConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioI2SMasterConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_i2s_master_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_i2s_master_user_config_t", configStructName);
            }
        }
        //Slave configurations.
        if(flexioI2SSlaveConfigurationsCount > 0)
        {
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioI2S slave configuration structure */";
        }
        for (var idx = 0; idx < flexioI2SSlaveConfigurationsCount; idx++)
        {
            var configStructName = flexioI2SSlaveConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioI2SSlaveConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_i2s_slave_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_i2s_slave_user_config_t", configStructName);
            }
        }

        /* Master callback definitions */
        for (var idx = 0; idx < flexioI2SMasterConfigurationsCount; idx++)
        {
            var callbackName = flexioI2SMasterConfigurationsChildren[idx].getChildById("callback").getStringValue();
		    if((callbackName != 'NULL')&&(callbackName != ''))
			{
                callbacks.push(callbackName);
			}
        }
        /* Slave callback definitions */
		count = 0;
        for (var idx = 0; idx < flexioI2SSlaveConfigurationsCount; idx++)
        {
            var callbackName = flexioI2SSlaveConfigurationsChildren[idx].getChildById("callback").getStringValue();
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
            hFileSections[DEFINES_SECTION] += declareVar("\nextern void", callbacks[i]+"(i2s_event_t event, void *userData)");
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
            /* Get master configurations */
            var flexioI2SMasterConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2s_master_Configuration");
            var flexioI2SMasterConfigurationsChildren = flexioI2SMasterConfigurations.getChildren();
            var flexioI2SMasterConfigurationsCount = flexioI2SMasterConfigurationsChildren.size();
            /* Get slave configurations */
            var flexioI2SSlaveConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_i2s_slave_Configuration");
            var flexioI2SSlaveConfigurationsChildren = flexioI2SSlaveConfigurations.getChildren();
            var flexioI2SSlaveConfigurationsCount = flexioI2SSlaveConfigurationsChildren.size();

            // Add master config structures
            if(flexioI2SMasterConfigurationsCount > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioI2S master configuration structure */";
            }
            for (var idx = 0; idx < flexioI2SMasterConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioI2SMasterConfigurationsChildren[idx], "flexio_i2s_master_user_config_t", "");
            }
            // Add slave config structures
            if(flexioI2SSlaveConfigurationsCount > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioI2S slave configuration structure */";
            }
            for (var idx = 0; idx < flexioI2SSlaveConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioI2SSlaveConfigurationsChildren[idx], "flexio_i2s_slave_user_config_t", "");
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}