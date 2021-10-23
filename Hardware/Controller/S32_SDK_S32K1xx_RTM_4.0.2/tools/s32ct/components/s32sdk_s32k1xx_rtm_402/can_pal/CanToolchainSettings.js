/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Can pal component toolchain settings class */
function CanToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "CanToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            profile.addComponents("platform.driver.flexcan", true, true);

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}