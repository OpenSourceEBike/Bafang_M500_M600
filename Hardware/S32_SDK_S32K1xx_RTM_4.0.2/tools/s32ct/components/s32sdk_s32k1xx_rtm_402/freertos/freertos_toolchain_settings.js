/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* OSIF component toolchain settings class */
function FreeRtosToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "FreeRtosToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets) {
        try{
             this.uiConfig = configSets;

            // var configStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".osif_os_selSet");
            // var os_type = configStruct.getValue();

            var initStructMEMORY = getSetting(this.uiConfig, this.uiConfig.getName()+".memory_tab");
            var memory_scheme= initStructMEMORY.getChildById("memory_scheme_id").getValue();
            var isConfigSUPPORT_DYNAMIC_ALLOCATION= initStructMEMORY.getChildById("configSUPPORT_DYNAMIC_ALLOCATION").getValue();
            //add also the current driver dependencies
            profile.addComponents("platform.os.freertos", true, true);

            var cpu = profile.getMcuInfo().getPartNumber();
            if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
            {
                profile.addComponents("platform.os.freertosS32K11x", true, true);
            }
            else if (cpu.indexOf("S32K14") == 0) /* Add sources for S32K14x */
            {
                profile.addComponents("platform.os.freertosS32K14x", true, true);
            }

            if(isConfigSUPPORT_DYNAMIC_ALLOCATION){
                switch(memory_scheme) {
                    case "heap1":
                        profile.addComponents("platform.os.freertos_heap_1", true, true);
                        break;
                    case "heap2":
                        profile.addComponents("platform.os.freertos_heap_2", true, true);
                        break;
                    case "heap3":
                        profile.addComponents("platform.os.freertos_heap_3", true, true);
                        break;
                    case "heap4":
                        profile.addComponents("platform.os.freertos_heap_4", true, true);
                        break;
                    case "heap5":
                        profile.addComponents("platform.os.freertos_heap_5", true, true);
                        break;
                     default:
                        scriptApi.logError("that memory_scheme is not avaible : " + memory_scheme);
                }
            }

            var compilerType = profile.get_Compiler();
            switch(compilerType) {
                case "gcc":
                    /* These commands only work for GCC */
                    profile.add_C_Compiler_Symbol("USING_OS_FREERTOS");
                    profile.add_C_Assembler_Symbol("USING_OS_FREERTOS");
                    /* The following commands are similar to the above ones */
                    /* profile.add_C_Option("USING_OS_FREERTOS", "gnu.c.compiler.option.preprocessor.def.symbols"); */
                    /* profile.add_C_Option("USING_OS_FREERTOS", "com.freescale.s32ds.cross.gnu.tool.assembler.option.defs"); */
                    break;
                case "iar":
                    profile.add_C_Option("USING_OS_FREERTOS", "iar.arm.toolchain.compiler.option.defines.v8.0.0");
                    profile.add_C_Option("USING_OS_FREERTOS", "iar.arm.toolchain.assembler.option.definedSymbols.v8.0.0");
                    break;
                 default:
                    scriptApi.logError("The current compiler is not supported by FreeRTOS");
                    break;
            }

            /* scriptApi.logInfo("get_Compiler : " + compilerType); */
            /* scriptApi.logInfo("get_Compiler_Name : " + profile.get_Compiler_Name()); */
            /* scriptApi.logInfo("get_Compiler_Id : " + profile.get_Compiler_Id()); */
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
