/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* OSIF component toolchain settings class */
function scstToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "scstToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets) {
        try{
            var cpu = profile.getMcuInfo().getPartNumber();
            var compilerType = profile.get_Compiler();

            switch(compilerType) {
                case "gcc":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        var libBinary = "\":lib_m0_scst.a\"";
                    }
                    else
                    {
                        var libBinary = "\":lib_m4_scst.a\"";
                    }
                    profile.add_C_Linker_Library(libBinary);
                    profile.add_C_Linker_LibraryPath("../SDK/lib/SCST/SCST/src/lib");
                    break;
                case "ghs":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        var libBinary = "\"lib_m0_scst.a\"";
                    }
                    else
                    {
                        var libBinary = "\"lib_m4_scst.a\"";
                    }
                    profile.add_C_Option(libBinary, "ghs.managedmake.arm.executable.toolchain.cc_linker.project.libraries")
                    profile.add_C_Option("../SDK/lib/SCST/SCST/src/lib", "ghs.managedmake.arm.executable.toolchain.cc_linker.project.library_directories")
                    break;
                case "iar":
                    if (cpu.indexOf("S32K11") == 0) /* Add sources for S32K11x */
                    {
                        profile.add_C_Option("../SDK/lib/SCST/SCST/src/lib/lib_m0_scst.a", "iar.arm.toolchain.linker.option.additionalLibraries.v8.0.0")
                    }
                    else
                    {
                        profile.add_C_Option("../SDK/lib/SCST/SCST/src/lib/lib_m4_scst.a", "iar.arm.toolchain.linker.option.additionalLibraries.v8.0.0")
                    }
                    break;
                default:
                    break;
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}