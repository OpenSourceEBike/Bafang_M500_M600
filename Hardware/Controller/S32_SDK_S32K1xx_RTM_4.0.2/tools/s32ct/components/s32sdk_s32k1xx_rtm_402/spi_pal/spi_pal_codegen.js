/*
 * Copyright 2018-2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */
scriptApi.requireScript("codegeneratorUtils.js");

// SPI_PAL component code generator class.
function spi_pal_CodeGenerator(uiConfig)
{
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function() 
    {
        return "spi_pal_CodeGenerator";
    }

    this.genHCfgFile = function(hCfgFileSections) 
    {
        hCfgFileSections[FILE_NAME_SECTION] += "spi_pal_cfg.h\n";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        
        if (peripheralName.indexOf("DSPI") == 0) 
        {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("spi_pal_cfg.h", "SPI_OVER_DSPI", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("spi_pal_cfg.h", "NO_OF_DSPI_INSTS_FOR_SPI");
        }
        else if(peripheralName.indexOf("LPSPI") == 0)
        {   
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("spi_pal_cfg.h", "SPI_OVER_LPSPI", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("spi_pal_cfg.h", "NO_OF_LPSPI_INSTS_FOR_SPI");
        }
        else if(peripheralName.indexOf("FLEXIO") == 0)
        {   
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("spi_pal_cfg.h", "SPI_OVER_FLEXIO", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("spi_pal_cfg.h", "NO_OF_FLEXIO_INSTS_FOR_SPI");
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) 
    {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("spi_pal.h");
        var childContext = uiConfig.getChildContext();
        var configName = childContext.getComponentInstanceConfig().getName();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getPeripheral();
        var initStructspi_palMaster = getSetting(this.uiConfig, this.uiConfig.getName() + ".spi_palMasterConfig");
        var initStructspi_palMasterConfig = initStructspi_palMaster.getChildren();
        var initStructspi_palSlave = getSetting(this.uiConfig, this.uiConfig.getName() + ".spi_palSlaveConfig");
        var initStructspi_palSlaveCfg = initStructspi_palSlave.getChildren();
        var countspi_palMasterCfg = initStructspi_palMasterConfig.length;
        var countspi_palSlaveCfg = initStructspi_palSlaveCfg.length;

        if ((countspi_palMasterCfg != 0) || (countspi_palSlaveCfg != 0)) 
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Device instance number */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern spi_instance_t", configName + "_Instance");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
        }

        if (countspi_palMasterCfg != 0) 
        {
            for (var i = 0; i < countspi_palMasterCfg; i++) 
            {                
                var spi_palNameMasterCfg = initStructspi_palMasterConfig[i].getChildById("name");
                if (peripheralName == "FLEXIO")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* FLEXIO extension for SPI_PAL */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern" + " " + "extension_flexio_for_spi_t " + spi_palNameMasterCfg.getValue() + "Extension;" + NEW_LINE;
                }
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* SPI_PAL Master Configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern" + " " + "spi_master_t " + spi_palNameMasterCfg.getValue() + ";" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                if (initStructspi_palMasterConfig[i].getChildById("callback").getValue() != "NULL")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* SPI_PAL callback for Master Configuration " + i + "*/" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + initStructspi_palMasterConfig[i].getChildById("callback").getValue() + "(void *driverState, spi_event_t event, void *userData);" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                }
            }

        }

        if (countspi_palSlaveCfg != 0) 
        {

            for (var i = 0; i < countspi_palSlaveCfg; i++) 
            {
                var spi_palNameSlaveCfg = initStructspi_palSlaveCfg[i].getChildById("name");
                if (peripheralName == "FLEXIO")
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* FLEXIO extension for SPI_PAL */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern" + " " + "extension_flexio_for_spi_t " + spi_palNameSlaveCfg.getValue() + "Extension;" + NEW_LINE;
                }
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* SPI_PAL Slave Configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern" + " " + "spi_slave_t " + spi_palNameSlaveCfg.getValue() + ";" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                if (initStructspi_palSlaveCfg[i].getChildById("callback").getValue() != "NULL") 
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* SPI_PAL callback for Slave Configuration " + i + "*/" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern void " + initStructspi_palSlaveCfg[i].getChildById("callback").getValue() + "(void *driverState, spi_event_t event, void *userData);" + NEW_LINE;
                }
            }
        }
    }

    this.genCFile = function(cFileSections) 
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var initStructspi_palMaster = getSetting(this.uiConfig, this.uiConfig.getName() + ".spi_palMasterConfig");
        var initStructspi_palMasterConfig = initStructspi_palMaster.getChildren();
        var countspi_palMasterCfg = initStructspi_palMasterConfig.length;
        var initStructspi_palSlave = getSetting(this.uiConfig, this.uiConfig.getName() + ".spi_palSlaveConfig");
        var initStructspi_palSlaveConfig = initStructspi_palSlave.getChildren();
        var countspi_palSlaveCfg = initStructspi_palSlaveConfig.length;
        var configName = childContext.getComponentInstanceConfig().getName();
        var spiPalInstanceType = "SPI_INST_TYPE_DSPI";
        var spiPalInstanceIndex = 0;

        if (peripheralName.indexOf("DSPI") !== -1)
        {
            spiPalInstanceType = "SPI_INST_TYPE_DSPI";
            spiPalInstanceIndex = peripheralName.replace(/[^0-9]/g, "");
        }
        else if (peripheralName.indexOf("LPSPI") !== -1)
        {
            spiPalInstanceType = "SPI_INST_TYPE_LPSPI";
            spiPalInstanceIndex = peripheralName.replace(/[^0-9]/g, "");
        }
        else if (peripheralName.indexOf("SPI") !== -1)
        {
            spiPalInstanceType = "SPI_INST_TYPE_SPI";
            spiPalInstanceIndex = peripheralName.replace(/[^0-9]/g, "");
        }
        else if (peripheralName.indexOf("FLEXIO") !== -1)
        {
            spiPalInstanceType = "SPI_INST_TYPE_FLEXIO";
            spiPalInstanceIndex = 0;
        }       
        
        if ((countspi_palMasterCfg != 0) || (countspi_palSlaveCfg != 0))
        {
            cFileSections[GLOBAL_VARS_SECTION] += "/* Define instance structure */" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "spi_instance_t " + configName + "_Instance" + " =" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    .instType = " + spiPalInstanceType + "," + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "    .instIdx = " + spiPalInstanceIndex + "U" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
        }

        if (countspi_palMasterCfg != 0) 
        {
            for (var i = 0; i < countspi_palMasterCfg; i++) 
            {
                
                var spi_palNameMasterCfg = initStructspi_palMasterConfig[i].getChildById("name");
                if (spi_palNameMasterCfg.getStringValue() == "") 
                {
                    spi_palNameMasterCfg.setValue(peripheralName + "_MasterInitConfig" + i);
                }
                
                if (peripheralName == "FLEXIO")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "/* FLEXIO extension for SPI_PAL */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "extension_flexio_for_spi_t " + spi_palNameMasterCfg.getValue() + "Extension =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .mosiPin = " + initStructspi_palMasterConfig[i].getChildById("mosiPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .misoPin = " + initStructspi_palMasterConfig[i].getChildById("misoPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .sckPin  = " + initStructspi_palMasterConfig[i].getChildById("sckPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .ssPin   = " + initStructspi_palMasterConfig[i].getChildById("ssPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE + NEW_LINE;
                }
                
                cFileSections[GLOBAL_VARS_SECTION] += "/* SPI_PAL Master Configurations " + i + " */" + NEW_LINE;

                cFileSections[GLOBAL_VARS_SECTION] += "spi_master_t " + spi_palNameMasterCfg.getValue() + " =" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .baudRate         = " + initStructspi_palMasterConfig[i].getChildById("baudRate").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .frameSize        = " + initStructspi_palMasterConfig[i].getChildById("frameSize").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .bitOrder         = " + initStructspi_palMasterConfig[i].getChildById("bitOrder").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .clockPolarity    = " + initStructspi_palMasterConfig[i].getChildById("clockPolarity").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .ssPolarity       = " + initStructspi_palMasterConfig[i].getChildById("ssPolarity").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .clockPhase       = " + initStructspi_palMasterConfig[i].getChildById("clockPhase").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .ssPin            = " + initStructspi_palMasterConfig[i].getChildById("whichPcs").getValue() + "," + NEW_LINE;
                if (initStructspi_palMasterConfig[i].getChildById("continuousSS").isAvailable())
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .continuousSS     = " + initStructspi_palMasterConfig[i].getChildById("continuousSS").getValue() + "," + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "    .transferType     = " + initStructspi_palMasterConfig[i].getChildById("transferType").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .rxDMAChannel     = " + initStructspi_palMasterConfig[i].getChildById("rxDMAChannel").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .txDMAChannel     = " + initStructspi_palMasterConfig[i].getChildById("txDMAChannel").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callback         = " + initStructspi_palMasterConfig[i].getChildById("callback").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callbackParam    = " + initStructspi_palMasterConfig[i].getChildById("callbackParam").getValue() + "," + NEW_LINE;
                if (peripheralName == "FLEXIO")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .extension        = &"+spi_palNameMasterCfg.getValue() + "Extension" + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .extension        = NULL" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE + NEW_LINE;
            }
        }

        if (countspi_palSlaveCfg != 0) 
        {
            for (var i = 0; i < countspi_palSlaveCfg; i++) 
            {
                var spi_palNameSlaveCfg = initStructspi_palSlaveConfig[i].getChildById("name");
                if (spi_palNameSlaveCfg.getStringValue() == "") 
                {
                    spi_palNameSlaveCfg.setValue(peripheralName + "_SlaveInitConfig" + i);
                }
                
                if (peripheralName == "FLEXIO")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "/*FLEXIO extension for SPI_PAL */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "extension_flexio_for_spi_t " + spi_palNameSlaveCfg.getValue() + "Extension =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .mosiPin = " + initStructspi_palSlaveConfig[i].getChildById("mosiPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .misoPin = " + initStructspi_palSlaveConfig[i].getChildById("misoPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .sckPin  = " + initStructspi_palSlaveConfig[i].getChildById("sckPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .ssPin   = " + initStructspi_palSlaveConfig[i].getChildById("ssPin").getValue() + "U," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE + NEW_LINE;
                }
                
                cFileSections[GLOBAL_VARS_SECTION] += "/* SPI_PAL Slave Configurations " + i + " */" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "spi_slave_t " + spi_palNameSlaveCfg.getValue() + " =" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .frameSize        = " + initStructspi_palSlaveConfig[i].getChildById("frameSize").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .bitOrder         = " + initStructspi_palSlaveConfig[i].getChildById("bitOrder").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .clockPolarity    = " + initStructspi_palSlaveConfig[i].getChildById("clockPolarity").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .ssPolarity       = " + initStructspi_palSlaveConfig[i].getChildById("ssPolarity").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .clockPhase       = " + initStructspi_palSlaveConfig[i].getChildById("clockPhase").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .transferType     = " + initStructspi_palSlaveConfig[i].getChildById("transferType").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .rxDMAChannel     = " + initStructspi_palSlaveConfig[i].getChildById("rxDMAChannel").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .txDMAChannel     = " + initStructspi_palSlaveConfig[i].getChildById("txDMAChannel").getValue() + "U," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callback         = " + initStructspi_palSlaveConfig[i].getChildById("callback").getValue() + "," + NEW_LINE;
                cFileSections[GLOBAL_VARS_SECTION] += "    .callbackParam    = " + initStructspi_palSlaveConfig[i].getChildById("callbackParam").getValue() + "," + NEW_LINE;
                if (peripheralName == "FLEXIO")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .extension        = &"+spi_palNameSlaveCfg.getValue() + "Extension"  + NEW_LINE;
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += "    .extension        = NULL" + NEW_LINE;
                }
                cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE + NEW_LINE;
            }
        }

    }
}