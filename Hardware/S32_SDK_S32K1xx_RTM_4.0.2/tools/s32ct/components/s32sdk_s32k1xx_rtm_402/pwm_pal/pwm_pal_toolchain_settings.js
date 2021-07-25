/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* PWM pal component toolchain settings class */
function PwmpalToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "PwmpalToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            if (peripheralName.indexOf("FTM") == 0) {
                profile.addComponents("platform.driver.ftm_pwm", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}