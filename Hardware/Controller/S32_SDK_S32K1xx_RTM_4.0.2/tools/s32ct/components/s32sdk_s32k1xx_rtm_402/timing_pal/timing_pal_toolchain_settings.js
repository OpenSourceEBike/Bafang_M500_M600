/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Timing pal component toolchain settings class */
function TimingToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "TimingToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            if (peripheralName.indexOf("PIT") == 0) {
                profile.addComponents("platform.driver.pit", true, true);
            }
            if (peripheralName.indexOf("STM") == 0) {
                profile.addComponents("platform.driver.stm", true, true);
            }
            if (peripheralName.indexOf("FTM") == 0) {
                profile.addComponents("platform.driver.ftm_oc", true, true);
                profile.addComponents("platform.driver.ftm_mc", true, true);
            }
            if (peripheralName.indexOf("LPTMR") == 0) {
                profile.addComponents("platform.driver.lptmr", true, true);
            }
            if (peripheralName.indexOf("LPIT") == 0) {
                profile.addComponents("platform.driver.lpit", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
