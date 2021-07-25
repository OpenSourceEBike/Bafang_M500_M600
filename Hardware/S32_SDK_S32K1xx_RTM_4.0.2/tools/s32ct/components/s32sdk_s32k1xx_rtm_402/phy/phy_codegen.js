/*
 * Copyright 2018,2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

var driverStructMapping = {
    'PHY_TYPE_GENERIC' : '&PHY_driver_generic',
    'PHY_TYPE_TJA110X' : '&PHY_driver_tja110x'
};

function unique(value, index, self) {
    return self.indexOf(value) === index;
}

// PHY component code generator class.
function PHYCodeGen(uiConfig) {
    this.uiCOnfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "PHYCodeGen";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        var childContext = uiConfig.getChildContext();
        var configsArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".phy_configs");
        var configsArrayList = configsArray.getChildren();
        var configsArraySize = configsArrayList.size();
        var driversSet = [];
        var callbacks = [];

        // Go through all the devices and discover driver types
        for (i = 0; i < configsArraySize; i++)
        {
            driversSet.push(configsArrayList[i].getChildById("phyStdCfg").getChildById("type").getValue());
        }
        driversSet = driversSet.filter(unique);

        hFileSections[INCLUDES_SECTION] += addInclude("stddef.h");
        hFileSections[INCLUDES_SECTION] += addInclude("phy.h");
        hFileSections[INCLUDES_SECTION] += addInclude("phy_generic.h");
        if (driversSet.indexOf('PHY_TYPE_TJA110X') != -1)
        {
            hFileSections[INCLUDES_SECTION] += addInclude("phy_tja110x.h");
        }

        hFileSections[DEFINES_SECTION] += "\n/*! @brief Number of PHY devices */\n";
        hFileSections[DEFINES_SECTION] += addDefine("N_PHY_DEVS", "(" + configsArraySize + "U)");
        hFileSections[DEFINES_SECTION] += "/*! @brief Number of PHY drivers */\n";
        hFileSections[DEFINES_SECTION] += addDefine("N_PHY_DRIVERS", "(" + driversSet.length + "U)");

        hFileSections[DEFINES_SECTION] += "\n/*! @brief Definition of names for the PHY instances */\n";
        for (i = 0; i < configsArraySize; i++)
        {
            if (configsArrayList[i].getChildById("phyStdCfg").getChildById("name").getValue() == '')
                hFileSections[DEFINES_SECTION] += addDefine("PHY_INSTANCE_" + i, "(" +i + "U)");
            else
                hFileSections[DEFINES_SECTION] += addDefine(configsArrayList[i].getChildById("phyStdCfg").getChildById("name").getValue(), "(" +i + "U)");
        }

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\nextern phy_config_t  phyConfig[N_PHY_DEVS];      /*! @brief Array of PHY configuration structures */";
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\nextern phy_driver_t *phyDrivers[N_PHY_DRIVERS];  /*! @brief Array of pointers towards the driver structures */\n";

        for (i = 0; i < configsArraySize; i++)
        {
            var cfg = configsArrayList[i].getChildById("phyStdCfg");
            if ((cfg.getChildById("linkUpEventCB").getValue() != "") && (cfg.getChildById("linkUpEventCB").getValue() != "NULL"))
            {
                callbacks.push(cfg.getChildById("linkUpEventCB").getValue());
            }
            if ((cfg.getChildById("linkDownEventCB").getValue() != "") && (cfg.getChildById("linkDownEventCB").getValue() != "NULL"))
            {
                callbacks.push(cfg.getChildById("linkDownEventCB").getValue());
            }
            if ((cfg.getChildById("autonegCompleteCB").getValue() != "") && (cfg.getChildById("autonegCompleteCB").getValue() != "NULL"))
            {
                callbacks.push(cfg.getChildById("autonegCompleteCB").getValue());
            }
            if ((cfg.getChildById("autonegFailedCB").getValue() != "") && (cfg.getChildById("autonegFailedCB").getValue() != "NULL"))
            {
                callbacks.push(cfg.getChildById("autonegFailedCB").getValue());
            }
            if ((cfg.getChildById("sleepRequestCB").getValue() != "") && (cfg.getChildById("sleepRequestCB").getValue() != "NULL"))
            {
                callbacks.push(cfg.getChildById("sleepRequestCB").getValue());
            }
        }
        callbacks = callbacks.filter(unique);

        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief PHY framework callbacks */\n";
        for (i = 0; i < callbacks.length; i++)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "void " + callbacks[i] + "(uint8_t phy);\n";
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        var childContext = uiConfig.getChildContext();
        var configsArray = getSetting(this.uiCOnfig, this.uiCOnfig.getName()+".phy_configs");
        var configsArrayList = configsArray.getChildren();
        var configsArraySize = configsArrayList.size();
        var driversSet = [];
        var callbacks = [];
        var phyDriversArr = '\n/*! @brief Array of pointers towards the driver structures */\nphy_driver_t *phyDrivers[N_PHY_DRIVERS] = {';
        var phyConfigsArr = '';

        // Go through all the devices and discover driver types
        for (i = 0; i < configsArraySize; i++)
        {
            driversSet.push(configsArrayList[i].getChildById("phyStdCfg").getChildById("type").getValue());
        }
        driversSet = driversSet.filter(unique);

        for (i = 0; i < driversSet.length; i++)
        {
            cFileSections[GLOBAL_VARS_SECTION] += addDefine(driversSet[i], "(" +i + "U)");
            phyDriversArr += driverStructMapping[driversSet[i]];
            if (i != driversSet.length - 1)
            {
                phyDriversArr += ", ";
            }
        }
        phyDriversArr += '};\n';

        cFileSections[GLOBAL_VARS_SECTION] += phyDriversArr;

        // Extended configurations
        for (i = 0; i < configsArraySize; i++)
        {
            if (configsArrayList[i].getChildById("phyExtCfg_enable").isEnabled() && configsArrayList[i].getChildById("phyExtCfg_enable").getValue())
            {
                cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Extended configuration for PHY " + i + " */";
                configsArrayList[i].getChildById("phyExtCfg").getChildById("name").setValue(configsArrayList[i].getChildById("phyStdCfg").getChildById("extendedConfig").getValue().substring(1));
                cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(configsArrayList[i].getChildById("phyExtCfg"), "static phy_tja110x_config_t", "phyExtCfgTJA110x_");
            }
        }

        cFileSections[GLOBAL_VARS_SECTION] += "\n/*! @brief Array of PHY configuration structures */\n";
        for (i = 0; i < configsArraySize; i++)
        {
            phyConfigsArr += printConfigStruct(configsArrayList[i].getChildById("phyStdCfg"), "", "");
        }
        phyConfigsArr = phyConfigsArr.replace(/\n.* = {/g, "{");
        phyConfigsArr = phyConfigsArr.replace(/};/g, "},");
        phyConfigsArr = 'phy_config_t  phyConfig[N_PHY_DEVS] = {\n' + phyConfigsArr;
        phyConfigsArr = phyConfigsArr.replace(/\n/g, "\n\t");
        phyConfigsArr = phyConfigsArr.slice(0, -1) + '};\n';

        cFileSections[GLOBAL_VARS_SECTION] += phyConfigsArr;
    }
}
