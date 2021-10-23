/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Can pal component toolchain settings class */
function UartToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "UartToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            // LINFlexD
            if (peripheralName.indexOf("LINFlexD") == 0) {
            profile.addComponents("platform.driver.linflexd_uart", true, true);
            }
            // LPUART
            if (peripheralName.indexOf("LPUART") == 0) {
            profile.addComponents("platform.driver.lpuart", true, true);
            }
            // FLEXIO
            if (peripheralName.indexOf("FLEXIO") == 0) {
            profile.addComponents("platform.driver.flexio_uart", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}