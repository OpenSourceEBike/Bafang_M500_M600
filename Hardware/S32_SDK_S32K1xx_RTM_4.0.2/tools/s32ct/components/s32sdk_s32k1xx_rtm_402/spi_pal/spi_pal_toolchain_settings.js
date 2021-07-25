/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* Spi pal component toolchain settings class */
function SpiToolchainSettings(uiConfig) 
{
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () 
    {
        return "SpiToolchainSettings";
    }

    this.applyToolchainSettings = function(profile) 
    {
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();

        try
        {
            scriptApi.logInfo("applyToolchainSettings");

            if (peripheralName.indexOf("LPSPI") == 0) 
            {
                profile.addComponents("platform.driver.lpspi", true, true);
                profile.addComponents("platform.driver.clock", true, true);
                profile.addComponents("platform.driver.interrupt_manager ", true, true);
                profile.addComponents("platform.driver.pins", true, true);
                profile.addComponents("platform.driver.osif", true, true);
            }
            else if (peripheralName.indexOf("FLEXIO") == 0) 
            {
                profile.addComponents("platform.driver.flexio_spi", true, true);
                profile.addComponents("platform.driver.clock", true, true);
                profile.addComponents("platform.driver.interrupt_manager ", true, true);
                profile.addComponents("platform.driver.pins", true, true);
                profile.addComponents("platform.driver.osif", true, true);
            }
            else if (peripheralName.indexOf("DSPI") == 0) 
            {
                profile.addComponents("platform.driver.dspi", true, true);
                profile.addComponents("platform.driver.clock", true, true);
                profile.addComponents("platform.driver.interrupt_manager ", true, true);
                profile.addComponents("platform.driver.pins", true, true);
                profile.addComponents("platform.driver.osif", true, true);
            }
            
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}