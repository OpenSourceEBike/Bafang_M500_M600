/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* OSIF component toolchain settings class */
function OsifToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "OsifToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets, isComponentPresent) {
       try{
            var freertosPresent = isComponentPresent("freertos_driver");

            scriptApi.logInfo("applyToolchainSettings");
            // scriptApi.logInfo("configSets "+configSets);
            // scriptApi.logInfo("this.uiConfig "+ this.uiConfig);
            profile.addComponents("platform.driver.osif", true, true);
            if (freertosPresent)
                profile.addComponents("platform.driver.osif_freertos", true, true);
            else
                profile.addComponents("platform.driver.osif_baremetal", true, true);
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}