/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function addOsifDefault(uiConfig) {
    //scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "addOsifDefault";
    }

    this.applyToolchainSettings = function(profile, configSets, isComponentPresent) {
       try{
            this.uiConfig = configSets;
            
            var osifPresent = isComponentPresent("osif");
            var freertosPresent = isComponentPresent("freertos_driver");

            if (!osifPresent) {
                profile.addComponents("platform.driver.osif", true, true);
                if (freertosPresent)
                    profile.addComponents("platform.driver.osif_freertos", true, true);
                else
                    profile.addComponents("platform.driver.osif_baremetal", true, true);
            }
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}