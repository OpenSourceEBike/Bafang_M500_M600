/*
 * Copyright 2020 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");
scriptApi.requireScript("../system/add_osif_to_project.js");

function addADCPALDrivers(uiConfig) {
    //scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    // returns the class name
    this.toString = function () {
        return "addADCPALDrivers";
    }

    this.applyToolchainSettings = function(profile, configSets, isComponentPresent) {
       try {
            var configStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".adcPalConfig");
            var instanceType = configStruct.getChildById("instanceType").getValue();

            /* add peripheral driver code used by the ADC PAL component */
            if (instanceType == "ADC_INST_TYPE_ADC_S32K1xx") {
                profile.add_C_Compiler_Includes("SDK/platform/drivers/src/adc");
                if (!isComponentPresent("adc")) {
                    profile.addComponents("platform.driver.adc", true, true);
                }

                if (!isComponentPresent("pdb_driver")) {
                    profile.addComponents("platform.driver.pdb", true, true);
                }

                if (!isComponentPresent("trgmux")) {
                    profile.addComponents("platform.driver.trgmux", true, true);
                }
            } else if ((instanceType == "ADC_INST_TYPE_ADC_SAR") || (instanceType == "ADC_INST_TYPE_ADC_SAR_CTU") ||
                (instanceType == "ADC_INST_TYPE_ADC_SAR_BCTU")) {
                if (!isComponentPresent("adc_sar_driver")) {
                    profile.addComponents("platform.driver.adc_sar", true, true);
                }

                if ((instanceType == "ADC_INST_TYPE_ADC_SAR_CTU") && !isComponentPresent("ctu_driver")) {
                    profile.addComponents("platform.driver.ctu", true, true);
                }
            }

            /* add osif */
            var osifClassName = "addOsifDefault";
            var settingsClass = (Function('return new ' + osifClassName))()
            settingsClass.constructor.apply(settingsClass, new Array(configSets));
            settingsClass.applyToolchainSettings(profile, configSets, isComponentPresent);

        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}