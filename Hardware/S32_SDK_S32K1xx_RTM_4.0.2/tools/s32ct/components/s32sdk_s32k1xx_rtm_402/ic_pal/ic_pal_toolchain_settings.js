/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* IC_PAL component toolchain settings class */
function IC_PAL_ToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "IC_PAL_ToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets, isComponentPresent) {
        try {
            if (isComponentPresent("platform.driver.ftm_ic") == false) {
                profile.addComponents("platform.driver.ftm_ic", true, true);
            }
            if (isComponentPresent("platform.driver.ftm_mc") == false) {
                profile.addComponents("platform.driver.ftm_mc", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}