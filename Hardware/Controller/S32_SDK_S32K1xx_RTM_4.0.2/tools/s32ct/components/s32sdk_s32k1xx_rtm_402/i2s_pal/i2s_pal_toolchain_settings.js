/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function I2sPalToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "I2sPalToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        try{
            this.uiConfig = configSets;

            profile.addComponents("platform.driver.clock", true, true);
            profile.addComponents("platform.driver.interrupt_manager ", true, true);
            profile.addComponents("platform.driver.edma ", true, true);
            profile.addComponents("platform.driver.pins", true, true);
            if (peripheralName.indexOf("SAI") == 0) {
            profile.addComponents("platform.driver.sai", true, true);
            }
            if (peripheralName.indexOf("FLEXIO") == 0) {
            profile.addComponents("platform.driver.flexio_i2s", true, true);
            }
            profile.addComponents("platform.driver.osif", true, true);

            scriptApi.logInfo("applyToolchainSettings");
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
