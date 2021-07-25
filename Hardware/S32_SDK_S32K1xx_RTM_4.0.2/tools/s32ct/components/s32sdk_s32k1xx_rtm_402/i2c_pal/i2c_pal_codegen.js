/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");
    
// I2C component code generator class.
function I2C_PALCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "I2C_PALCodeGen";
    }
    
    this.genHCfgFile = function(hCfgFileSections) {
        hCfgFileSections[FILE_NAME_SECTION] += "i2c_pal_cfg.h\n";
        try{    
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            
			if(peripheralName != null)
			{
			    if (peripheralName.indexOf("I2C_") == 0) {
                    hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2c_pal_cfg.h", "I2C_OVER_I2C", 1);
                    hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("i2c_pal_cfg.h", "NO_OF_I2C_INSTS_FOR_I2C");
                }
				else if(peripheralName.indexOf("LPI2C") == 0)
				{   
			        hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2c_pal_cfg.h", "I2C_OVER_LPI2C", 1);
                    hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("i2c_pal_cfg.h", "NO_OF_LPI2C_INSTS_FOR_I2C");
				}
				else if(peripheralName.indexOf("FLEXIO") == 0)
				{   
			        hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2c_pal_cfg.h", "I2C_OVER_FLEXIO", 1);
                    hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("i2c_pal_cfg.h", "NO_OF_FLEXIO_INSTS_FOR_I2C");
				}
	    	}
			else
			{
	    	    hCfgFileSections[DEFINES_SECTION] += addPalDefine("i2c_pal_cfg.h", "I2C_OVER_SWI2C", 1);
                hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("i2c_pal_cfg.h", "NO_OF_SWI2C_INSTS_FOR_I2C");	
			}
			
	    }catch(error)
		{
			scriptApi.logError(error);
		}
    }
    
    
    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";

        // Add Includes info 
        hFileSections[INCLUDES_SECTION] += addInclude("i2c_pal.h");
       
		//PAL instance information
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* PAL instance information */" + NEW_LINE;
		
	    var childContext = uiConfig.getChildContext();
        var configName = childContext.getComponentInstanceConfig().getName();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
		peripheralNameLength = peripheralName.length();
		
		if(peripheralName != null)
		{			
            var i2cPal_masterConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palMasterConfiguration");
            var i2cPal_slaveConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palSlaveConfiguration");
	
            //PAL instance information			
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern i2c_instance_t "+ peripheralName.toLowerCase() +"_instance;"+ NEW_LINE;
            
            // Get master configuration 
            var i2cPal_masterConfigDetailChild = i2cPal_masterConfig.getChildren();
            var count_masterArrayConfiguration = i2cPal_masterConfigDetailChild.size();
            
            //Get slave configuration 
            var i2cPal_slaveConfigDetailChild = i2cPal_slaveConfig.getChildren();
            var count_slaveArrayConfiguration = i2cPal_slaveConfigDetailChild.size();
            
            var count = 0;
            
            //Master configurations 
            for (var idx = 0; idx < count_masterArrayConfiguration; idx++)
            {
                if(count == 0)
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Master module configurations */"+NEW_LINE;
                    count++;
                }
		    	
		    	 //Get master configuration name
                var masterConfigName = i2cPal_masterConfigDetailChild[idx].getChildById("i2c_MasterConfiguration").getChildById("name").getValue();
		        var readonlyField = i2cPal_masterConfigDetailChild[idx].getChildById("i2c_MasterConfiguration").getChildById("readonly").getValue();
		    	
		        if(readonlyField == true)
		    	{
		    		hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const i2c_master_t " + masterConfigName +";"+NEW_LINE;
		    	}
		    	else
		    	{
		    		hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern i2c_master_t " + masterConfigName +";"+NEW_LINE;
		    	}	    
            }
            
            //Slave configurations
            count = 0;
            for (var idx = 0; idx < count_slaveArrayConfiguration; idx++)
            {
		    	
                if(count == 0)
                {
                     hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Slave module configurations */"+NEW_LINE;
                     count++;
                }
                //Get slave configuration name
                var slaveConfigName = i2cPal_slaveConfigDetailChild[idx].getChildById("name").getValue();
		    	var readonlyField = i2cPal_slaveConfigDetailChild[idx].getChildById("readonly").getValue();
		    	
		    	if(readonlyField == true)
		    	{
		    		hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const i2c_slave_t " + slaveConfigName +";"+ NEW_LINE;
		    	}
		    	else
		    	{
					if(peripheralName != "FLEXIO")
					{
		    		    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern i2c_slave_t " + slaveConfigName +";"+ NEW_LINE;
					}
		    	}
            }
    
            //Master callbacks
            count = 0;
            for(var idx = 0; idx < count_masterArrayConfiguration; idx++)
            {
                //var configName_idx = i2cPal_masterConfigDetailChild[idx].getChildById("name").setValue("i2c_MasterConfig"+idx);
                var masterCallbackName = i2cPal_masterConfigDetailChild[idx].getChildById("i2c_MasterConfiguration").getChildById("i2c_pal_master_cfg_callback").getValue();
                
                if((masterCallbackName != 'NULL')&&(masterCallbackName != ''))
                {   if (count == 0)
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Define external callbacks for master devices */"+NEW_LINE;
                    }
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", masterCallbackName + "(i2c_master_event_t masterEvent,void *userData)");
                    count++;
                }
            }
            
            //Slave callbacks
            count = 0; 
            for(var idx = 0; idx < count_slaveArrayConfiguration; idx++)
            {
                var slaveCallbackName = i2cPal_slaveConfigDetailChild[idx].getChildById("i2c_pal_slave_cfg_callback").getValue();
                 
                if((slaveCallbackName != 'NULL')&&(slaveCallbackName != ''))
                {   
                    if(count == 0)
                    {                   
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Define external callbacks for slave devices */"+NEW_LINE;
                    }
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern void", slaveCallbackName + "(i2c_slave_event_t slaveEvent,void *userData)");
                    count++;
                }
            }
	    }
		else
		{
			/* SWI2C is used */
            /* Add Includes info */
			try
			{
                var initStructswi2cMaster = getSetting(this.uiConfig, this.uiConfig.getName()+".swi2cMasterConfig");
                var initStructswi2cMasterConfig = initStructswi2cMaster.getChildren();
                var countswi2cMasterCfg = initStructswi2cMasterConfig.length;    
			    
                if (countswi2cMasterCfg != 0)
                {
					for(var i = 0; i < countswi2cMasterCfg;i++)
					{
						hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* PAL instance information */" + NEW_LINE;
						hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern i2c_instance_t i2c"+i+"_instance; "+ NEW_LINE;
						hFileSections[GLOBAL_VARS_EXTERN_SECTION] += ""+NEW_LINE;					
					}
					
                    for(var i = 0; i < countswi2cMasterCfg;i++)
                    {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += ""+NEW_LINE;
                        var swi2cNameMasterCfg = initStructswi2cMasterConfig[i].getChildById("swi2cModeConfig");
                        if (swi2cNameMasterCfg.getStringValue() == "")
                        {
                            swi2cNameMasterCfg.setValue(peripheralName + "_MasterInitConfig" + i);
                        }
						
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += " extern" + " " + "swi2c_pin_t " + "swi2c_sda" +  i + ";" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += " extern" + " " + "swi2c_pin_t " + "swi2c_scl" +  i + ";" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += " extern" + " " + "swi2c_pin_t " + "swi2c_sdaRead" +  i + ";" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += " extern" + " " + "swi2c_pin_t " + "swi2c_sclRead" +  i + ";" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += " extern" + " " + "i2c_master_t " + swi2cNameMasterCfg.getValue() + ";" + NEW_LINE;
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                
                    }
                }
			}catch(error){
				scriptApi.logError(error);
			}			
		}
    }
    
    /* Adds component specific info to source file sections */
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
			
            var childContext = uiConfig.getChildContext();
            var configName = childContext.getComponentInstanceConfig().getName();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
			var peripheralNameLength = peripheralName.length();
            var i2cPal_masterConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palMasterConfiguration");
			var i2cPal_slaveConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palSlaveConfiguration");
		    if(peripheralName != null)
			{
				var i2cPal_masterConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palMasterConfiguration");
			    var i2cPal_slaveConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".i2c_palSlaveConfiguration");
				
                /* Get master configuration */
                var i2cPal_masterConfigDetailChild = i2cPal_masterConfig.getChildren();
                var count_masterArrayConfiguration = i2cPal_masterConfigDetailChild.size();
			    
                /* PAL instance information */
                cFileSections[GLOBAL_VARS_SECTION] += "\n/* PAL instance information */" + NEW_LINE;
				
				var peripheralType;
				
				if (peripheralName.indexOf("I2C_") == 0) 
				{
					peripheralType =  "I2C";
				}
				else if(peripheralName.indexOf("LPI2C") == 0)
				{
					peripheralType =  "LPI2C";
				}else if(peripheralName.indexOf("FLEXIO") == 0)
				{
					peripheralType =  "FLEXIO";
				}
				
				if(peripheralName.indexOf("FLEXIO") == 0)
				{
					cFileSections[GLOBAL_VARS_SECTION] += "i2c_instance_t " + peripheralName.toLowerCase() + "_instance = {I2C_INST_TYPE_"+peripheralType+", "+"0U};" + NEW_LINE;
				}
				else
				{
                    cFileSections[GLOBAL_VARS_SECTION] += "i2c_instance_t "+ peripheralName.toLowerCase() +"_instance = {I2C_INST_TYPE_"+peripheralType+", "+peripheralName.substring(Number(peripheralNameLength)-1, Number(peripheralNameLength))+"U};" + NEW_LINE;
                }

                for(var idx = 0; idx < count_masterArrayConfiguration; idx++)
                {
                    var masterStuct = i2cPal_masterConfigDetailChild[idx].getChildById("i2c_MasterConfiguration");
                    var extStruct = i2cPal_masterConfigDetailChild[idx].getChildById("i2cExtConfig");
                    var readonlyField = masterStuct.getChildById("readonly").getValue();
                    var ext = masterStuct.getChildById("i2c_pal_master_cfg_extension");
                    var newextname = extStruct.getChildById("name").getValue();
                    if((extStruct != null) && (peripheralName == 'FLEXIO'))
                    {
                        /* Generate extension structure code in c file */
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(extStruct, "static extension_flexio_for_i2c_t", "");
                    }
                    if (peripheralName == 'FLEXIO')
                    {
                        ext.setValue(newextname);
                    }
                    else
                    {
                        ext.setValue("NULL");
                    }

                    if (ext.getValue() != "NULL")
                    {
                        ext.setValue("&" + newextname);
                    }

                    var readOnly = (readonlyField ? "const " : "");

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "i2c_master_t " + masterStuct.getChildById("name").getValue() + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .slaveAddress = " + masterStuct.getChildById("i2c_pal_master_cfg_slaveAddress").getValue() + "U,";
                    if(peripheralType != "FLEXIO")
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .is10bitAddr = " + masterStuct.getChildById("i2c_pal_master_cfg_is10bitAddr").getValue() + ",";
                    }
                    else
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .is10bitAddr = false,"
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .baudRate = " + masterStuct.getChildById("i2c_pal_master_cfg_baudRate").getValue() + "UL,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .dmaChannel1 = " + masterStuct.getChildById("i2c_pal_master_cfg_dmaChannel1").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .dmaChannel2 = " + masterStuct.getChildById("i2c_pal_master_cfg_dmaChannel2").getValue() + "U,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .transferType = " + masterStuct.getChildById("i2c_pal_master_cfg_transferType").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .operatingMode = " + masterStuct.getChildById("i2c_pal_master_cfg_operatingMode").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .callback = " + masterStuct.getChildById("i2c_pal_master_cfg_callback").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .callbackParam = " + masterStuct.getChildById("i2c_pal_master_cfg_callbackParam").getValue() + ",";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "  .extension = " + masterStuct.getChildById("i2c_pal_master_cfg_extension").getValue();
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;

                    if (ext.getValue() != "NULL")
                    {
                        ext.setValue(newextname);
                    }

                }
                /* Get slave configuration */
                var i2cPal_slaveConfigDetailChild = i2cPal_slaveConfig.getChildren();
                var count_slaveArrayConfiguration = i2cPal_slaveConfigDetailChild.size();
                if(peripheralType != "FLEXIO")
                {
                    for(var idx = 0; idx < count_slaveArrayConfiguration; idx++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(i2cPal_slaveConfigDetailChild[idx], "i2c_slave_t", "i2c_pal_slave_cfg_");               
                    }
                }
		    }
            else
			{
				/* SWI2C is used */
                // Get childContext class to extract peripheral name and functional group.
		        
                var initStructswi2cMaster = getSetting(this.uiConfig, this.uiConfig.getName()+".swi2cMasterConfig");
                var initStructswi2cMasterConfig = initStructswi2cMaster.getChildren();
                var countswi2cMasterCfg = initStructswi2cMasterConfig.length;
                if (countswi2cMasterCfg != 0)
                {   
			        cFileSections[GLOBAL_VARS_SECTION]+= "/* PAL instance information */" + NEW_LINE;
                    for(var i = 0; i < countswi2cMasterCfg;i++)
                    {
						cFileSections[GLOBAL_VARS_SECTION] += "extern i2c_instance_t i2c"+i+"_instance = {I2C_INST_TYPE_SWI2C, "+i+"U};"+ NEW_LINE;
					}
					cFileSections[GLOBAL_VARS_SECTION] += ""+NEW_LINE;
			
                    for(var i = 0; i < countswi2cMasterCfg;i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "/* I2C pin configurations " + i + " */" + NEW_LINE;
                        var swi2cNameMasterCfg = initStructswi2cMasterConfig[i].getChildById("swi2cModeConfig");
                
                        if (swi2cNameMasterCfg.getStringValue() == "")
                        {
                            swi2cNameMasterCfg.setValue(peripheralName + "_MasterInitConfig" + i);
                        }
                
                        var sdaPinConvertStrToNumber = parseInt(initStructswi2cMasterConfig[i].getChildById("sdaPin").getStringValue(), 10);
                        var sclPinConvertStrToNumber = parseInt(initStructswi2cMasterConfig[i].getChildById("sclPin").getStringValue(), 10);
                
                        var sdaReadPinConvertStrToNumber = parseInt(initStructswi2cMasterConfig[i].getChildById("sdaReadPin").getStringValue(), 10);
                        var sclReadPinConvertStrToNumber = parseInt(initStructswi2cMasterConfig[i].getChildById("sclReadPin").getStringValue(), 10);
                
                        cFileSections[GLOBAL_VARS_SECTION] += "swi2c_pin_t " + "swi2c_sda" +  i + " =" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .port              = " + parseInt(sdaPinConvertStrToNumber/16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .pinNumber         = " + parseInt(sdaPinConvertStrToNumber%16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                
                        cFileSections[GLOBAL_VARS_SECTION] += "swi2c_pin_t " + "swi2c_scl" +  i + " =" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .port              = " + parseInt(sclPinConvertStrToNumber/16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .pinNumber         = " + parseInt(sclPinConvertStrToNumber%16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                
                        cFileSections[GLOBAL_VARS_SECTION] += "swi2c_pin_t " + "swi2c_sdaRead" +  i + " =" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .port              = " + parseInt(sdaReadPinConvertStrToNumber/16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .pinNumber         = " + parseInt(sdaReadPinConvertStrToNumber%16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                
                        cFileSections[GLOBAL_VARS_SECTION] += "swi2c_pin_t " + "swi2c_sclRead" +  i + " =" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .port              = " + parseInt(sclReadPinConvertStrToNumber/16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .pinNumber         = " + parseInt(sclReadPinConvertStrToNumber%16, 10) + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;               
                    }
   
                    for(var i = 0; i < countswi2cMasterCfg;i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "/* I2C Master Configurations " + i + " */" + NEW_LINE;
                        var swi2cNameMasterCfg = initStructswi2cMasterConfig[i].getChildById("swi2cModeConfig");
                        if (swi2cNameMasterCfg.getStringValue() == "")
                        {
                            swi2cNameMasterCfg.setValue(peripheralName + "_MasterInitConfig" + i);
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += "i2c_master_t " + swi2cNameMasterCfg.getValue() + " =" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .slaveAddress              = " + initStructswi2cMasterConfig[i].getChildById("slaveAddress").getValue() + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "    .baudRate                  = " + initStructswi2cMasterConfig[i].getChildById("Baudratecounter").getValue() + "," + NEW_LINE;
                
                        cFileSections[GLOBAL_VARS_SECTION] += "    .extension                 = NULL" + NEW_LINE;
                
                        cFileSections[GLOBAL_VARS_SECTION] += "}" + ";" + NEW_LINE;
                    }
                
                }
			}				
            
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}