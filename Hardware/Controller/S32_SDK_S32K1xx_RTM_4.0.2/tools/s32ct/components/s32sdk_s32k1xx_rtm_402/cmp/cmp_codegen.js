/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// CMP component code generator class.
function CMP_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "CMP_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var cmpCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".cmpConfig").getChildren();
            var cmpCfgArray_Size = cmpCfgArray_Items.size();

            if ((cmpCfgArray_Items != null) && (cmpCfgArray_Size > 0)) {
                // Add Includes info.
                hFileSections[INCLUDES_SECTION] += addInclude("cmp_driver.h");

                //Add Defines info.
                hFileSections[DEFINES_SECTION] += "\n/*Device instance number */\n";
                hFileSections[DEFINES_SECTION] += addDefine("INST_" + instanceName.toUpperCase(), "(0U)");
                //Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] = "\n/* User configurations */" + NEW_LINE;

                // Iterate through all configurations
                for (var i =  0; i < cmpCfgArray_Size; i++) {
                    //get config name
                    var cmpCfgArray_Name = cmpCfgArray_Items[i].getChildById("name").getStringValue();
                    //get config readonly
                    var cmpCfgArray_ReadOnly = (cmpCfgArray_Items[i].getChildById("readOnly").getValue() == true) ? "const " : "";
                    //add config to header file
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* User configuration " + i + " */" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + cmpCfgArray_ReadOnly + "cmp_module_t", cmpCfgArray_Name);
                }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global] \n * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be \n * visible when an object or function with external linkage is defined..\n *\n";
		cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
		try{
			// Get childContext class to extract peripheral name and functional group.
			var childContext = uiConfig.getChildContext();
			var peripheralName = childContext.getComponentInstanceConfig().getName();
			var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".cmpConfig");
			var initStructConfig = initStruct.getChildren();
			var numberOfConfigurations = initStructConfig.length;
			if (numberOfConfigurations != 0)
			{
				for(var i = 0; i < numberOfConfigurations;i++)
				{
					cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* CMP configuration structure " + i + " */" + NEW_LINE;
					var cmpConfigName = initStructConfig[i].getChildById("name");
					var readOnly = (initStructConfig[i].getChildById("readOnly").getValue() == true) ? "const " : "";
					var comparatorConfig = initStructConfig[i].getChildById("Comparator");
					var muxConfig = initStructConfig[i].getChildById("MUX");
					var dacConfig = initStructConfig[i].getChildById("DAC");
					var triggerConfig = initStructConfig[i].getChildById("TriggerMode");
					
					/* Get round robin channels */
					var roundRobinChannels = triggerConfig.getChildById("roundRobinChannelsState");
					var channel0 = Number(roundRobinChannels.getChildById("rr_channel0").getValue());
					var channel1 = Number(roundRobinChannels.getChildById("rr_channel1").getValue());
					var channel2 = Number(roundRobinChannels.getChildById("rr_channel2").getValue());
					var channel3 = Number(roundRobinChannels.getChildById("rr_channel3").getValue());
					var channel4 = Number(roundRobinChannels.getChildById("rr_channel4").getValue());
					var channel5 = Number(roundRobinChannels.getChildById("rr_channel5").getValue());
					var channel6 = Number(roundRobinChannels.getChildById("rr_channel6").getValue());
					var channel7 = Number(roundRobinChannels.getChildById("rr_channel7").getValue());
					
					/* Get value for roundRobinChannelsState */
					var value_rr_channels = (channel7 << 7) + (channel6 << 6) + (channel5 << 5) + (channel4 << 4) + (channel3 << 3) + (channel2 << 2) + (channel1 << 1) + channel0;
					
					/* Get pre-state for round robin check */
					var stateChannels = triggerConfig.getChildById("programedState");
					var state0 = Number(stateChannels.getChildById("state_channel0").getValue());
					var state1 = Number(stateChannels.getChildById("state_channel1").getValue());
					var state2 = Number(stateChannels.getChildById("state_channel2").getValue());
					var state3 = Number(stateChannels.getChildById("state_channel3").getValue());
					var state4 = Number(stateChannels.getChildById("state_channel4").getValue());
					var state5 = Number(stateChannels.getChildById("state_channel5").getValue());
					var state6 = Number(stateChannels.getChildById("state_channel6").getValue());
					var state7 = Number(stateChannels.getChildById("state_channel7").getValue());
					
					/* Get value for programedState*/
					var value_state = (state7 << 7) + (state6 << 6) + (state5 << 5) + (state4 << 4) + (state3 << 3) + (state2 << 2) + (state1 << 1) + state0;					
					
					cFileSections[GLOBAL_VARS_SECTION] += readOnly + "cmp_module_t " + cmpConfigName.getValue() + "=" + NEW_LINE;
					cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE + 
					                                       TAB_SPACE + "{" + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".dmaTriggerState        = " + comparatorConfig.getChildById("dmaTriggerState").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".outputInterruptTrigger = " + comparatorConfig.getChildById("outputInterruptTrigger").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".mode                   = " + comparatorConfig.getChildById("mode").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".filterSamplePeriod     = " + comparatorConfig.getChildById("filterSamplePeriod").getValue() + "," + NEW_LINE +
				                                           TAB_SPACE + TAB_SPACE + ".filterSampleCount      = " + comparatorConfig.getChildById("filterSampleCount").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".powerMode              = " + comparatorConfig.getChildById("powerMode").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".inverterState          = " + comparatorConfig.getChildById("inverterState").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".outputSelect           = " + comparatorConfig.getChildById("outputSelect").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".pinState               = " + comparatorConfig.getChildById("pinState").getValue() + "," + NEW_LINE +
				                                           TAB_SPACE + TAB_SPACE + ".offsetLevel            = " + comparatorConfig.getChildById("offsetLevel").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".hysteresisLevel        = " + comparatorConfig.getChildById("hysteresisLevel").getValue() + NEW_LINE +
														   TAB_SPACE + "}," + NEW_LINE +
														   TAB_SPACE + "{" + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".positivePortMux  = " + muxConfig.getChildById("positivePortMux").getValue() + "," + NEW_LINE +           
														   TAB_SPACE + TAB_SPACE + ".negativePortMux  = " + muxConfig.getChildById("negativePortMux").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".positiveInputMux = " + muxConfig.getChildById("positiveInputMux").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".negativeInputMux = " + muxConfig.getChildById("negativeInputMux").getValue() + NEW_LINE +
														   TAB_SPACE + "}," + NEW_LINE +
														   TAB_SPACE + "{" + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".voltageReferenceSource  = " + dacConfig.getChildById("voltageReferenceSource").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".voltage = " + dacConfig.getChildById("voltage").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".state = " + dacConfig.getChildById("state").getValue() + NEW_LINE +
														   TAB_SPACE + "}," + NEW_LINE +
														   TAB_SPACE + "{" + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".roundRobinState  = " + triggerConfig.getChildById("roundRobinState").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".roundRobinInterruptState = " + triggerConfig.getChildById("roundRobinInterruptState").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".fixedPort  = " + triggerConfig.getChildById("fixedPort").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".fixedChannel = " + triggerConfig.getChildById("fixedChannel").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".samples  = " + triggerConfig.getChildById("samples").getValue() + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".initializationDelay = " + triggerConfig.getChildById("initializationDelay").getValue() + "," + NEW_LINE +														   
														   TAB_SPACE + TAB_SPACE + ".roundRobinChannelsState = " + value_rr_channels + "," + NEW_LINE +
														   TAB_SPACE + TAB_SPACE + ".programedState = " + value_state + NEW_LINE +
														   TAB_SPACE + "}," + NEW_LINE +
														   "};" + NEW_LINE;
						
				}										   
			}
		} catch(err) {
            scriptApi.logError(err);
        }
	}
}

