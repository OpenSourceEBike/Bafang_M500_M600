/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Oc pal component toolchain settings class */
function OcToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "OcToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            // FTM
            if (peripheralName.indexOf("FTM") == 0) {
                profile.addComponents("platform.driver.ftm_oc", true, true);
                profile.addComponents("platform.driver.ftm_mc", true, true);
            }

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}