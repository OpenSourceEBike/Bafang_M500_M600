/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */
scriptApi.requireScript("codegeneratorUtils.js");

// CMP component code generator class.
function LPSPI_CodeGenerator(uiConfig) 
{
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function() 
    {
        return "LPSPI_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) 
    {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try 
        {
            // Add Includes info.
            hFileSections[INCLUDES_SECTION] += addInclude("lpspi_master_driver.h");
            hFileSections[INCLUDES_SECTION] += addInclude("lpspi_slave_driver.h");
            hFileSections[INCLUDES_SECTION] += addInclude("lpspi_shared_function.h");
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getName();
            var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
            var initStructspiMaster = getSetting(this.uiConfig, this.uiConfig.getName() + ".lpspiMasterConfig");
            var initStructspiMasterConfig = initStructspiMaster.getChildren();
            var initStructspiSlave = getSetting(this.uiConfig, this.uiConfig.getName() + ".lpspiSlaveConfig");
            var initStructspiSlaveConfig = initStructspiSlave.getChildren();
            var countspiMasterCfg = initStructspiMasterConfig.length;
            var countspiSlaveCfg = initStructspiSlaveConfig.length;

            if ((countspiMasterCfg != 0) || (countspiSlaveCfg != 0)) 
            {
                hFileSections[DEFINES_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), instanceName.substring(6, 7) + "U");

                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Define state structure for current SPI instance */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern" + " " + " lpspi_state_t" + " " + peripheralName + "State" + ";" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
            }
            if (countspiMasterCfg != 0) 
            {
                for (var i = 0; i < countspiMasterCfg; i++) 
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* LPSPI Master Configurations " + i + " */" + NEW_LINE;
                    var spiNameMasterCfg = initStructspiMasterConfig[i].getChildById("name");
                    var spiNameMasterReadOnly = (initStructspiMasterConfig[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern " + spiNameMasterReadOnly + "lpspi_master_config_t " + spiNameMasterCfg.getValue() + ";" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                    if (initStructspiMasterConfig[i].getChildById("callback").getValue() != "NULL") 
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* LPSPI callback for Master Configuration " + i + "*/" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + initStructspiMasterConfig[i].getChildById("callback").getValue() + "(void *driverState, spi_event_t event, void *userData);" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                    }
                }
            }

            if (countspiSlaveCfg != 0) 
            {
                for (var i = 0; i < countspiSlaveCfg; i++) 
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* LPSPI Slave Configurations " + i + " */" + NEW_LINE;
                    var spiNameSlaveCfg = initStructspiSlaveConfig[i].getChildById("name");
                    var spiNameSlaveReadOnly = (initStructspiSlaveConfig[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern " + spiNameSlaveReadOnly + "lpspi_slave_config_t " + spiNameSlaveCfg.getValue() + ";" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                    if (initStructspiSlaveConfig[i].getChildById("callback").getValue() != "NULL") 
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* LPSPI callback for Slave Configuration " + i + "*/" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + initStructspiSlaveConfig[i].getChildById("callback").getValue() + "(void *driverState, spi_event_t event, void *userData);" + NEW_LINE;
                    }
                }
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global] \n * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be \n * visible when an object or function with external linkage is defined..\n *\n";
        cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getName();
            var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
            var initStructspiMaster = getSetting(this.uiConfig, this.uiConfig.getName() + ".lpspiMasterConfig");
            var initStructspiMasterConfig = initStructspiMaster.getChildren();
            var initStructspiSlave = getSetting(this.uiConfig, this.uiConfig.getName() + ".lpspiSlaveConfig");
            var initStructspiSlaveConfig = initStructspiSlave.getChildren();
            var countspiMasterCfg = initStructspiMasterConfig.length;
            var countspiSlaveCfg = initStructspiSlaveConfig.length;

            if ((countspiMasterCfg != 0) || (countspiSlaveCfg != 0))
            {                
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "/* Define state structure for current SPI instance */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "lpspi_state_t" + " " + peripheralName + "State" + ";" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
            }
            if (countspiMasterCfg != 0) 
            {
                for (var i = 0; i < countspiMasterCfg; i++) 
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "/* LPSPI Master Configurations " + i + " */" + NEW_LINE;
                    var spiNameMasterCfg = initStructspiMasterConfig[i];
                    var spiNameMasterReadOnly = (initStructspiMasterConfig[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(spiNameMasterCfg, spiNameMasterReadOnly + "lpspi_master_config_t", "");
					cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                }
            }

            if (countspiSlaveCfg != 0) 
            {
                for (var i = 0; i < countspiSlaveCfg; i++) 
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "/* LPSPI Slave Configurations " + i + " */" + NEW_LINE;
                    var spiNameSlaveCfg = initStructspiSlaveConfig[i];
					var spiNameSlaveReadOnly = (initStructspiSlaveConfig[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(spiNameSlaveCfg, spiNameSlaveReadOnly + "lpspi_slave_config_t", "");
					cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                }
            }
        } catch (err) {
            scriptApi.logError(err);
        }
    }
}