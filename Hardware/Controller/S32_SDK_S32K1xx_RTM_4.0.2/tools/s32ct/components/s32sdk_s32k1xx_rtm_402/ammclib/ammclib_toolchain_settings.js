/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* OSIF component toolchain settings class */
function ammclibToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "ammclibToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets) {
        try{
            var cpu = profile.getMcuInfo().getPartNumber();
            var compilerType = profile.get_Compiler();

            switch(compilerType) {
                case "gcc":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        var libBinary = "\":S32K11x_AMMCLIB.a\"";
                    }
                    else
                    {
                        var libBinary = "\":S32K14x_AMMCLIB.a\"";
                    }
                    profile.add_C_Linker_Library(libBinary);
                    profile.add_C_Linker_LibraryPath("../SDK/lib/AMMCLIB/lib/s32ds_arm32");
                    break;
                case "ghs":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        var libBinary = "\"S32K11x_AMMCLIB.a\"";
                    }
                    else
                    {
                        var libBinary = "\"S32K14x_AMMCLIB.a\"";
                    }
                    profile.add_C_Option(libBinary, "ghs.managedmake.arm.executable.toolchain.cc_linker.project.libraries")
                    profile.add_C_Option("../SDK/lib/AMMCLIB/lib/ghs", "ghs.managedmake.arm.executable.toolchain.cc_linker.project.library_directories")
                    break;
                case "iar":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        scriptApi.logError("The current compiler is not supported by ammclib");
                    }
                    else
                    {
                        profile.add_C_Option("../SDK/lib/AMMCLIB/lib/iar/S32K14x_AMMCLIB.a", "iar.arm.toolchain.linker.option.additionalLibraries.v8.0.0")
                    }
                    break;
                default:
                    scriptApi.logError("The current compiler is not supported by ammclib");
                    break;
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}