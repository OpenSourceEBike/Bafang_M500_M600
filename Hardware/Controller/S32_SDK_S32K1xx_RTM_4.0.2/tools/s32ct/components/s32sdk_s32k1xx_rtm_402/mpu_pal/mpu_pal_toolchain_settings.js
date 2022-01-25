/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* MPU_PAL component toolchain settings class */
function MPU_PAL_AddDrivers(uiConfig) {
    // scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "MPU_PAL_AddDrivers";
    }

    this.applyToolchainSettings = function(profile) {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try{
            // XRDC
            if (peripheralName.indexOf("XRDC") == 0) {
                profile.addComponents("platform.driver.xrdc", true, true);
            }
            // MPU_R52
            else if (peripheralName.indexOf("MPU_R52") == 0) {
                profile.addComponents("platform.driver.mpu_r52", true, true);
            }
            // MPU_M7
            else if (peripheralName.indexOf("MPU_M7") == 0) {
                profile.addComponents("platform.driver.mpu_m7", true, true);
            }
            // SMPU
            else if (peripheralName.indexOf("SMPU") == 0) {
                profile.addComponents("platform.driver.smpu", true, true);
            }
            else if (peripheralName.indexOf("MPU") == 0) {
                profile.addComponents("platform.driver.mpu", true, true);
            }
            else {
                //Do nothing
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}