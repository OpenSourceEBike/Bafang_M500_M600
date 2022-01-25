/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// SWT component code generator class.
function osif_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;
    
    this.applyToolchainSettings = function(profile) {
       try{
            scriptApi.logInfo("applyToolchainSettings");

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }

    // Returns the class name.
    this.toString = function () {
        return "osif_CodeGenerator";
    }
    
    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";                
        try {
            // Add Includes info.
            hFileSections[INCLUDES_SECTION] += addInclude("osif.h");
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) 
    {        
        try{
        } catch(err) {
            scriptApi.logError(err);
        }
    }
}
