/*
 * Copyright 2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");
// LPIT component code generator class.
function LPTMR_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "LPTMR_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections)
    {
         hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
         var readOnly = "";
        // Get childContext class to extract peripheral name and functional group.
         var childContext = uiConfig.getChildContext();
         var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
         var instanceName = childContext.getComponentInstanceConfig().getName();

        /* Get LPTMR Children in channel config list */
         var lptmrInitList = getSetting(this.uiConfig, this.uiConfig.getName()+".lptmrConfig").getChildren();

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("lptmr_driver.h");

        if(lptmrInitList != null)
        {
            var inst = peripheralName.charAt(peripheralName.length - 1);

            defVal = "INST_" + instanceName.toUpperCase();
            hFileSections[DEFINES_SECTION] += addDefine(defVal, "(" + inst + "U)");

            if(lptmrInitList.length > 0)
            {
                for (var i = 0; i < lptmrInitList.length; i++)
                {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/* LPTMR Configuration " + i + " */";
                    var lptmrCfgName = lptmrInitList[i].getChildById("name").getStringValue();
                    readOnly = (lptmrInitList[i].getChildById("readOnly").getValue() ? " const " : " ");
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("\nextern" + readOnly + "lptmr_config_t", lptmrCfgName);
                }
            }
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{

            /* Get LPTMR Children in channel config list */
            var lptmrInitList = getSetting(this.uiConfig, this.uiConfig.getName()+".lptmrConfig").getChildren();

            if(lptmrInitList != null)
            {
                if(lptmrInitList.length > 0)
                {
                    for (var i = 0; i < lptmrInitList.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += "\n/* LPTMR configuration structure " + i + " */";
                        readOnly = (lptmrInitList[i].getChildById("readOnly").getValue() ? "const " : "");
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(lptmrInitList[i], readOnly + "lptmr_config_t ", "" );
                    }
                }
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
