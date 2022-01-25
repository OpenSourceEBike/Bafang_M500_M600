/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// CSEc component code generator class.
function CSEcCodeGen(uiConfig) {
	this.uiCOnfig = uiConfig;

	// Returns the class name.
	this.toString = function () {
		return "CSEcCodeGen";
	}

	// Adds component specific info to header file sections.
	this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
		var childContext = uiConfig.getChildContext();
		var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".csec_state_name");

		hFileSections[INCLUDES_SECTION] += addInclude("csec_driver.h");

		if(stateStruct != null)
		{
			hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*! @brief Driver state structure which holds driver runtime data */\n";
			hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern csec_state_t", stateStruct);
		}
	}

	// Adds component specific info to source file sections.
	this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
		var childContext = uiConfig.getChildContext();
		var stateStruct = getSettingsValue(this.uiCOnfig, this.uiCOnfig.getName()+".csec_state_name");

		if (stateStruct != null)
		{
			cFileSections[GLOBAL_VARS_SECTION] += "/*! @brief Driver state structure which holds driver runtime data */\n";
			cFileSections[GLOBAL_VARS_SECTION] += declareVar("csec_state_t", stateStruct);
		}
	}
}