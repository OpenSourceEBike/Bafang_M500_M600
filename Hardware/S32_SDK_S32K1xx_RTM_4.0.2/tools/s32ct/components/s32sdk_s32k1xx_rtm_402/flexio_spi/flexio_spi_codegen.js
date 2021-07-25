/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function unique(value, index, self) { 
    return self.indexOf(value) === index;
}

// Flexio_spi component code generator class.
function FlexioSPICodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "FlexioSPICodeGen";
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
        var flexioSPIMasterConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_spi_master_Configuration");
        var flexioSPIMasterConfigurationsChildren = flexioSPIMasterConfigurations.getChildren();
        var flexioSPIMasterConfigurationsCount = flexioSPIMasterConfigurationsChildren.size();

		/* Get slave configurations */
        var flexioSPISlaveConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_spi_slave_Configuration");
        var flexioSPISlaveConfigurationsChildren = flexioSPISlaveConfigurations.getChildren();
        var flexioSPISlaveConfigurationsCount = flexioSPISlaveConfigurationsChildren.size();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("flexio_spi_driver.h");

        //Add Defines info.
        hFileSections[DEFINES_SECTION] += addDefine("INST_" + configName.toUpperCase(), "(0U)");
        //Master configurations.
        if(flexioSPIMasterConfigurationsCount > 0)
        {
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioSPI master configuration structure */";
        }
        for (var idx = 0; idx < flexioSPIMasterConfigurationsCount; idx++)
        {
            var configStructName = flexioSPIMasterConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioSPIMasterConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_spi_master_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_spi_master_user_config_t", configStructName);
            }
        }
        //Slave configurations.
        if(flexioSPISlaveConfigurationsCount > 0)
        {
            hFileSections[DEFINES_SECTION] += "\n/* External declaration of FlexioSPI slave configuration structure */";
        }
        for (var idx = 0; idx < flexioSPISlaveConfigurationsCount; idx++)
        {
            var configStructName = flexioSPISlaveConfigurationsChildren[idx].getChildById("name").getValue();
		    var constStruct = flexioSPISlaveConfigurationsChildren[idx].getChildById("readonly").getValue();
            if (constStruct)
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern const flexio_spi_slave_user_config_t", configStructName);
            }
            else
            {
                hFileSections[DEFINES_SECTION] += declareVar("\nextern flexio_spi_slave_user_config_t", configStructName);
            }
        }

        /* Master callback definitions */
        for (var idx = 0; idx < flexioSPIMasterConfigurationsCount; idx++)
        {
            var callbackName = flexioSPIMasterConfigurationsChildren[idx].getChildById("callback").getStringValue();
		    if((callbackName != 'NULL')&&(callbackName != ''))
			{
                callbacks.push(callbackName);
			}
        }
        /* Slave callback definitions */
        for (var idx = 0; idx < flexioSPISlaveConfigurationsCount; idx++)
        {
            var callbackName = flexioSPISlaveConfigurationsChildren[idx].getChildById("callback").getStringValue();
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
            hFileSections[DEFINES_SECTION] += declareVar("\nextern void", callbacks[i]+"(void *driverState, spi_event_t event, void *userData)");
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
            var flexioSPIMasterConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_spi_master_Configuration");
            var flexioSPIMasterConfigurationsChildren = flexioSPIMasterConfigurations.getChildren();
            var flexioSPIMasterConfigurationsCount = flexioSPIMasterConfigurationsChildren.size();
            /* Get slave configurations */
            var flexioSPISlaveConfigurations = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".flexio_spi_slave_Configuration");
            var flexioSPISlaveConfigurationsChildren = flexioSPISlaveConfigurations.getChildren();
            var flexioSPISlaveConfigurationsCount = flexioSPISlaveConfigurationsChildren.size();

            // Add master config structures
            if(flexioSPIMasterConfigurationsCount > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioSPI master configuration structure */";
            }
            for (var idx = 0; idx < flexioSPIMasterConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioSPIMasterConfigurationsChildren[idx], "flexio_spi_master_user_config_t", "");
            }
            // Add slave config structures
            if(flexioSPISlaveConfigurationsCount > 0)
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* FlexioSPI slave configuration structure */";
            }
            for (var idx = 0; idx < flexioSPISlaveConfigurationsCount; idx++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(flexioSPISlaveConfigurationsChildren[idx], "flexio_spi_slave_user_config_t", "");
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}