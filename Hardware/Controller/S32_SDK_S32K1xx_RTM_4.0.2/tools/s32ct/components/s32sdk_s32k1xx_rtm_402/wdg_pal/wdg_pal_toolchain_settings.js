/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* WDG_PAL component toolchain settings class */
function WDG_PAL_AddDrivers(uiConfig) {
    // scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "WDG_PAL_AddDrivers";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            // WDOG
            if (peripheralName.indexOf("WDOG") == 0) {
                profile.addComponents("platform.driver.wdog", true, true);
            }
            // EWM
            if (peripheralName.indexOf("EWM") == 0) {
                profile.addComponents("platform.driver.ewm", true, true);
            }
            // SWT
            if (peripheralName.indexOf("SWT") == 0) {
                profile.addComponents("platform.driver.swt", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}