/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// I2C component code generator class.
function LPI2CCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "LPI2CCodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";


	    // Add Includes info 
        hFileSections[INCLUDES_SECTION] += addInclude("lpi2c_driver.h");
		
        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var configName = childContext.getComponentInstanceConfig().getName();
        var fnGroupName = childContext.getFunctionalGroup().getName();
		
		/* Get total configuration */
		var lpi2c_masterConfig = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".masterConfigurationLPI2C");
		var lpi2c_slaveConfig = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".slaveConfigurationLPI2C");
		
		/* Get master configuration */
        var lpi2c_masterConfigDetailChild = lpi2c_masterConfig.getChildren();
        var count_masterArrayConfiguration = lpi2c_masterConfigDetailChild.size();
		
		//Get slave configuration 
        var lpi2c_slaveConfigDetailChild = lpi2c_slaveConfig.getChildren();
        var count_slaveArrayConfiguration = lpi2c_slaveConfigDetailChild.size();
		
		var count = 0;
		
       //Add Defines info.
		hFileSections[DEFINES_SECTION] += "\n/* LPI2C instance */"+NEW_LINE;
		hFileSections[DEFINES_SECTION] += addDefine("INST_" + peripheralName.toUpperCase(), peripheralName.match(/\d+$/)[0] + "U");
		//Master configurations 
        for (var idx = 0; idx < count_masterArrayConfiguration; idx++)
        {
            if(count == 0)
            {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* Master module configurations */"+NEW_LINE;
                count++;
            }
			
			 //Get master configuration name
            var masterConfigName = lpi2c_masterConfigDetailChild[idx].getChildById("name").getValue();
		    var readonlyField = lpi2c_masterConfigDetailChild[idx].getChildById("readonly").getValue();
			
		    if(readonlyField == true)
			{
				hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const lpi2c_master_user_config_t " + masterConfigName +";"+NEW_LINE;
			}
			else
			{
				hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern lpi2c_master_user_config_t " + masterConfigName +";"+NEW_LINE;
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
            var slaveConfigName = lpi2c_slaveConfigDetailChild[idx].getChildById("name").getValue();
			var readonlyField = lpi2c_slaveConfigDetailChild[idx].getChildById("readonly").getValue();
			
			if(readonlyField == true)
			{
				hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern const lpi2c_slave_user_config_t " + slaveConfigName +";"+ NEW_LINE;
			}
			else
			{
				 hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern lpi2c_slave_user_config_t " + slaveConfigName +";"+ NEW_LINE;
			}    

        }
		
        
        //Master callbacks
        count = 0;
        for(var idx = 0; idx < count_masterArrayConfiguration; idx++)
        {
            //var configName_idx = i2cPal_masterConfigDetailChild[idx].getChildById("name").setValue("i2c_MasterConfig"+idx);
            var masterCallbackName = lpi2c_masterConfigDetailChild[idx].getChildById("lpi2c_master_cfg_masterCallback").getValue();
            
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
            var slaveCallbackName = lpi2c_slaveConfigDetailChild[idx].getChildById("lpi2c_slave_cfg_slaveCallback").getValue();
             
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

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
			
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var configName = childContext.getComponentInstanceConfig().getName();
            var fnGroupName = childContext.getFunctionalGroup().getName();
		    
		    /* Get total configuration */
		    var lpi2c_masterConfig = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".masterConfigurationLPI2C");
			var lpi2c_slaveConfig = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".slaveConfigurationLPI2C");
		    
		    /* Get master configuration */
            var lpi2c_masterConfigDetailChild = lpi2c_masterConfig.getChildren();
            var count_masterArrayConfiguration = lpi2c_masterConfigDetailChild.size();
		    
		    //Get slave configuration 
            var lpi2c_slaveConfigDetailChild = lpi2c_slaveConfig.getChildren();
            var count_slaveArrayConfiguration = lpi2c_slaveConfigDetailChild.size();
		
            //Master configurations structure
            for(var idx = 0; idx < count_masterArrayConfiguration; idx++)
            {   
                var readonlyField = lpi2c_masterConfigDetailChild[idx].getChildById("readonly");
                
                if(readonlyField == "true")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(lpi2c_masterConfigDetailChild[idx], "const lpi2c_master_user_config_t", "lpi2c_master_cfg_");                
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(lpi2c_masterConfigDetailChild[idx], "lpi2c_master_user_config_t", "lpi2c_master_cfg_");
                }
            }
            
             //Master configurations structure
            for(var idx = 0; idx < count_slaveArrayConfiguration; idx++)
            {   
                var readonlyField = lpi2c_slaveConfigDetailChild[idx].getChildById("readonly");
                
                if(readonlyField == "true")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(lpi2c_slaveConfigDetailChild[idx], "const lpi2c_slave_user_config_t", "i2c_slave_cfg_");                
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(lpi2c_slaveConfigDetailChild[idx], "lpi2c_slave_user_config_t", "lpi2c_slave_cfg_");
                }
            }
            

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}