/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Security pal component toolchain settings class */
function add_components(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "add_components";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
            if (peripheralName.indexOf("FTFC") == 0  || peripheralName.indexOf("FTFM") == 0  || peripheralName.indexOf("FTFx") == 0) {
                profile.addComponents("platform.driver.csec", true, true);
            } else if (peripheralName.indexOf("CSE") == 0) {
				profile.addComponents("platform.driver.cse", true, true);
			}


        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
