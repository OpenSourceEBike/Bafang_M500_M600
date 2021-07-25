/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Timing pal component toolchain settings class */
function I2cToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "I2cToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            scriptApi.logInfo("applyToolchainSettings");
		if (peripheralName.indexOf("I2C") == 0) 
		{
            profile.addComponents("platform.driver.i2c", true, true);		
        }
        if (peripheralName.indexOf("LPI2C") == 0) {
			profile.addComponents("platform.driver.lpi2c", true, true);
        }
        if (peripheralName.indexOf("FLEXIO") == 0) {
			profile.addComponents("platform.driver.flexio_i2c", true, true);
        }
		if (peripheralName.indexOf("SWI2C") == 0) {
			profile.addComponents("platform.driver.swi2c", true, true);
        }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}