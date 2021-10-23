/*
 * Copyright 2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// sCST component code generator class.
function scst_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "scst_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        try {
            var headerFile = "";
            /* Select header file depends on CPU */
            var profile = scriptApi.getProfile();
            var cpu = profile.getMcuInfo().getPartNumber();
            if ((cpu == "S32K116") || (cpu == "S32K118")) {
                headerFile = "m0_scst_test_shell.h";
            }
            else {
                headerFile = "m4_scst_test_shell.h";
            }

            // #include "m4_scst_test_shell.h" is needed only
            hFileSections[INCLUDES_SECTION] += addInclude(headerFile);
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        try{
            cFileSections[GLOBAL_VARS_SECTION] += "/*" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " * This file is generated automatically by sCST component" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " * and is intentionally left empty" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " */";
        } catch(err) {
            scriptApi.logError(err);
        }
    }
}
