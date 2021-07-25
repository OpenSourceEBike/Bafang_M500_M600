/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* IPCF component toolchain settings class */
function LIN_ToolchainSettings(uiConfig) {
    this.uiConfig = uiConfig;

    /* returns the class name */
    this.toString = function () {
        return "LIN_ToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) {
       try{
            profile.addComponents("platform.driver.lpuart", true, true);
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
