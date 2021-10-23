/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

 scriptApi.requireScript("codegeneratorUtils.js");
/// @function MPU_Modulo
/// Returns The converted number (uint32_t).
/// @param {a} the number value (uint32_t).
/// @param {b} the modulo (uint32_t).
function MPU_Modulo(a, b) {
    return (a - Math.floor(a / b) * b);
}

/// @function MPU_ToUint32
/// Returns The converted number (uint32_t).
/// @param {x} the number (uint32_t).
function MPU_ToUint32(x) {
    return MPU_Modulo(x, Math.pow(2, 32));
}

/// @function MPU_DecToHex
/// Returns The heximal number (string).
/// @param {x} the decimal number (uint32_t).
function MPU_DecToHex(x) {
    return ('0x' + Math.abs(x).toString(16).toUpperCase());
}
/// @function MPU_StartAddrModulo
/// Returns The modulo 0-x address (uint32_t).
/// @param {startAddr} the start address (uint32_t).
/// @param {modulo} the modulo value (uint32_t).
function MPU_StartAddrModulo(startAddr, modulo) {
    var addr = startAddr;

    if (modulo) {
        addr = MPU_DecToHex(MPU_ToUint32(addr & (0xFFFFFFFF - modulo + 1)));
    }

    return addr;
}
/// @function MPU_EndAddrModulo
/// Returns The modulo 31-x address (uint32_t).
/// @param {endAddr} the end address (uint32_t).
/// @param {modulo} the modulo value (uint32_t).
function MPU_EndAddrModulo(endAddr, modulo) {
    var addr = endAddr;

    if (modulo) {
        addr = MPU_DecToHex(MPU_ToUint32(addr | (modulo - 1)));
    }

    return addr;
}

// MPU component code generator class.
function MPU_CodeGen(uiConfig){
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function (){
        return "MPU_CodeGen";
    }
    // Adds component specific info to header file sections.
        this.genHFile = function(hFileSections){
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
            try{
                var childContext = uiConfig.getChildContext();
                var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
                var instanceName = childContext.getComponentInstanceConfig().getName();
                var userConfig = getSetting(this.uiConfig, this.uiConfig.getName()+".MPUConfig");
                // Declare variable for mpu config
                var mpuCfgArray_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".MPUConfig").getChildren();
                var mpuCfgArray_Size = mpuCfgArray_Items.size();
                var NumberOfConfig = 0;
                var mpuMasterAccess_Items = getSetting(this.uiConfig, this.uiConfig.getName()+".mpuMasterAccessRight").getChildren();
                var mpuMasterAccess_Size = mpuMasterAccess_Items.size();
                if ((mpuCfgArray_Items != null) && (mpuCfgArray_Size > 0)) {
                    // Add Includes info.
                    hFileSections[INCLUDES_SECTION] += addInclude("mpu_driver.h");
                    // Add Defines info
                    hFileSections[DEFINES_SECTION] += "\n/*! @brief Device instance number */" + NEW_LINE;
                    hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_INST", "(0U)");

                    hFileSections[DEFINES_SECTION] += "\n/*! @brief Number of region in user configuration */" + NEW_LINE;
                    // Iterate through all configurations
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief User configuration */" + NEW_LINE;
                    var mpuNumberConfig = 0;
                    for(var i = 0; i< mpuCfgArray_Size; i++)
                    {
                        NumberOfConfig = 0;
                        var mpuRegionConfiguration_Items = mpuCfgArray_Items[i].getChildById("mpuRegionConfig").getChildren();
                        var mpuRegionConfiguration_Size = mpuRegionConfiguration_Items.size();
                        for(var j = 0; j< mpuRegionConfiguration_Size; j++)
                        {
                            if(mpuRegionConfiguration_Items[j].getChildById("regionEnable").getValue() == true)
                            {
                                NumberOfConfig ++;
                            }
                        }
                        var mpuRegionCfgArray_Items = mpuCfgArray_Items[i].getChildById("mpuRegionConfig").getChildren();
                        var mpuRegionCfgArray_Size = mpuRegionCfgArray_Items.size();
                        if(mpuRegionCfgArray_Size > 0){
                            hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_NUM_OF_REGION_CFG"+ i ,"(" + NumberOfConfig + "U)");
                            //get config name
                            var mpuCfgArray_Name = mpuCfgArray_Items[i].getChildById("name").getStringValue();
                            //get config readonly
                            var mpuCfgArray_ReadOnly = (mpuCfgArray_Items[i].getChildById("readonly").getValue() == true) ? "const " : "";
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + mpuCfgArray_ReadOnly + "mpu_user_config_t", mpuCfgArray_Name + "[" + instanceName.toUpperCase() + "_NUM_OF_REGION_CFG"+i + "]");
                            mpuNumberConfig++;
                        }
                    }
                    hFileSections[DEFINES_SECTION] += "\n/*! @brief Number of user configuration */" + NEW_LINE;
                    hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_NUM_OF_USER_CFG", "(" + mpuNumberConfig + "U)");

                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "\n/*! @brief Master access rights */" + NEW_LINE;
                    for(var i = 0; i< mpuMasterAccess_Size; i++)
                    {
                        var mpuAccessRightCfgArray_Items = mpuMasterAccess_Items[i].getChildById("mpuAccessRight").getChildren();
                        var mpuAccessRightCfgArray_Size = mpuAccessRightCfgArray_Items.size();
                        if(mpuAccessRightCfgArray_Size > 0){
                            if(i == 0)
                            {
                                mpuConfigNameMaster = mpuMasterAccess_Items[i].getChildById("accessRightName").setValue("AccessRightConfig0");
                            }
                            var mpuConfigNameMaster = mpuMasterAccess_Items[i].getChildById("accessRightName").getStringValue();
                            var mpuMasterAccess_ReadOnly = (mpuMasterAccess_Items[i].getChildById("MasterAccessRightReadOnly").getValue() == true) ? "const " : "";
                            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + mpuMasterAccess_ReadOnly + "mpu_master_access_right_t",mpuConfigNameMaster + "[FEATURE_MPU_MASTER_COUNT]");
                        }
                    }
                }
                else{
                    throw "Configuration does not exist";
                }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
    // Adds component specific info to source file sections
    this.genCFile = function(cFileSections){
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try {
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();
            var userConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".MPUConfig");

            var configUserStruct = getSetting(this.uiConfig, this.uiConfig.getName() + ".MPUConfig").getChildren();
            var configUserStruct_Size = configUserStruct.size();
            var configList = userConfig.getChildren();
            var configListSize = configList.size();
            /* Select Modulo of MPU depends on CPU */
            var profile = scriptApi.getProfile();
            var cpu = profile.getMcuInfo().getPartNumber();
            if ((cpu == "S32K116") || (cpu == "S32K118") || (cpu == "S32K142") || (cpu == "S32K142W") || (cpu == "S32K144") || (cpu == "S32K144W") || (cpu = "S32K146") || (cpu = "S32K148")) {
                 var mpuModulo = 32;
            }
            if(userConfig != null){
                if(configListSize > 0)
                {
                    for (var i = 0; i < configListSize; i++)
                    {
                        var mpuRegionConfiguration_Items = configUserStruct[i].getChildById("mpuRegionConfig").getChildren();
                        var mpuRegionConfiguration_Size = mpuRegionConfiguration_Items.size();
                        if (mpuRegionConfiguration_Size > 0)
                        {
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* MPU user configuration " + i  + " */";
                            var mpuConfigName = configList[i].getChildById("name").getStringValue();
                            var mpuConfigReadOnly = (configList[i].getChildById("readonly").getValue() == true) ? "const " : "";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + mpuConfigReadOnly + "mpu_user_config_t " + mpuConfigName + "[]" + " =";
                            cFileSections[GLOBAL_VARS_SECTION] += "\n{";
                            for(var j = 0; j < mpuRegionConfiguration_Size; j++)
                            {
                                if (j == 0)
                                {
                                    mpuRegionConfiguration_Items[j].getChildById("masterAccRight").setValue("AccessRightConfig0");
                                }
                                mpuRegionConfiguration_Items[j].getChildById("regionName").setValue("Region " + j);
                                var mpuEnableRegion = mpuRegionConfiguration_Items[j].getChildById("regionEnable").getValue();
                                // Check region enable
                                if(mpuEnableRegion)
                                {
                                var mpuStartAddr = mpuRegionConfiguration_Items[j].getChildById("startAddr").getValue();
                                var mpuEndAddr = mpuRegionConfiguration_Items[j].getChildById("endAddr").getValue()
                                var mpuOffsetStartEnd = mpuStartAddr + 31;
                                if(mpuStartAddr < 31)
                                {
                                    mpuRegionConfiguration_Items[j].getChildById("startAddr").setValue(MPU_DecToHex(0));
                                }
                                if(mpuEndAddr < mpuStartAddr + 31)
                                {
                                    mpuRegionConfiguration_Items[j].getChildById("endAddr").setValue(MPU_DecToHex(mpuOffsetStartEnd));
                                }

                                mpuRegionConfiguration_Items[j].getChildById("startAddr").setValue(MPU_StartAddrModulo(mpuRegionConfiguration_Items[j].getChildById("startAddr").getValue(),mpuModulo));
                                mpuRegionConfiguration_Items[j].getChildById("endAddr").setValue(MPU_EndAddrModulo(mpuRegionConfiguration_Items[j].getChildById("endAddr").getValue(),mpuModulo));
                                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/*!" + mpuRegionConfiguration_Items[j].getChildById("regionName").getValue()  + " */";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n       {";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n          .startAddr = 0x" + parseInt(mpuRegionConfiguration_Items[j].getChildById("startAddr").getValue()).toString(16).toUpperCase() + "U,";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n          .endAddr = 0x" + parseInt(mpuRegionConfiguration_Items[j].getChildById("endAddr").getValue()).toString(16).toUpperCase() + "U,";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n          .masterAccRight = " + mpuRegionConfiguration_Items[j].getChildById("masterAccRight").getValue() + ",";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n          .processIdentifier = 0x" + parseInt(mpuRegionConfiguration_Items[j].getChildById("processIdentifier").getValue()).toString(16).toUpperCase() + "U,";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n          .processIdMask = 0x" + parseInt(mpuRegionConfiguration_Items[j].getChildById("processIdMask").getValue()).toString(16).toUpperCase() + "U";
                                cFileSections[GLOBAL_VARS_SECTION] += "\n       }";
                                if (j != (mpuRegionConfiguration_Items.length - 1))
                                {
                                    cFileSections[GLOBAL_VARS_SECTION] += "," + NEW_LINE;
                                }
                                }
                            }
                            cFileSections[GLOBAL_VARS_SECTION] += "\n};";
                        }
                    }
                }
            }
            else {
                throw "Configuration does not exist";
            }
            var userConfigMaster = getSetting(this.uiConfig, this.uiConfig.getName()+".mpuMasterAccessRight");
            var configUserStructMaster = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpuMasterAccessRight").getChildren();
            var configUserStructMaster_Size = configUserStructMaster.size();
            var configListMaster = userConfigMaster.getChildren();
            var configListSizeMaster = configListMaster.size();
            if(userConfigMaster != null){
                if(configListSizeMaster > 0){
                    for (var i = 0; i < configListSizeMaster; i++)
                    {
                        var mpuRegionConfigurationMaster_Items = configUserStructMaster[i].getChildById("mpuAccessRight").getChildren();
                        var mpuRegionConfigurationMaster_Size = mpuRegionConfigurationMaster_Items.size();
                        if (mpuRegionConfigurationMaster_Size > 0){
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /*! Master access rights configuration " + i  + " */";
                        var mpuConfigNameMaster = configListMaster[i].getChildById("accessRightName").getStringValue();
                        var mpuConfigReadOnlyMaster = (configListMaster[i].getChildById("MasterAccessRightReadOnly").getValue() == true) ? "const " : "";
                        var mpuRegionConfigurationMaster_Items = configUserStructMaster[i].getChildById("mpuAccessRight").getChildren();
                        var mpuRegionConfigurationMaster_Size = mpuRegionConfigurationMaster_Items.size();

                        var masterNameConfig = configListMaster[i].getChildById("mpuAccessRight").getChildren();
                        var masterArr = ["CORE", "DEBUGGER", "DMA", "ENET"];

                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + mpuConfigReadOnlyMaster + "mpu_master_access_right_t " + mpuConfigNameMaster + "[]" + " =";
                        cFileSections[GLOBAL_VARS_SECTION] += "\n{";
                        for(var j = 0; j < mpuRegionConfigurationMaster_Size; j++)
                        {
                            var setMasterName = masterNameConfig[j].getChildById("master").setValue(masterArr[j]);
                            if(mpuRegionConfigurationMaster_Items[j].getChildById("master").getValue() == "DMA")
                            {
                                mpuRegionConfigurationMaster_Items[j].getChildById("processIdentifierEnable").setValue(false);
                            }
                            if(mpuRegionConfigurationMaster_Items[j].getChildById("master").getValue() == "ENET")
                            {
                                mpuRegionConfigurationMaster_Items[j].getChildById("processIdentifierEnable").setValue(false);
                            }
                            if(mpuRegionConfigurationMaster_Items[j].getChildById("master").getValue() == "DEBUGGER" && i==0)
                            {
                                mpuRegionConfigurationMaster_Items[j].getChildById("processIdentifierEnable").setValue(false);
                                mpuRegionConfigurationMaster_Items[j].getChildById("idTypeSuppervisor").setValue("MPU_SUPERVISOR");
                                mpuRegionConfigurationMaster_Items[j].getChildById("idTypeUser").setValue("USER_RWX");
                            }
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "      /*! " + mpuRegionConfigurationMaster_Items[j].getChildById("master").getValue() + " */";
                            cFileSections[GLOBAL_VARS_SECTION] += "\n       {";
                            cFileSections[GLOBAL_VARS_SECTION] += "\n          .masterNum = FEATURE_MPU_MASTER_" + mpuRegionConfigurationMaster_Items[j].getChildById("master").getValue() + ",";
                            cFileSections[GLOBAL_VARS_SECTION] += "\n          .accessRight = " + mpuRegionConfigurationMaster_Items[j].getChildById("idTypeSuppervisor").getValue() + "_" + mpuRegionConfigurationMaster_Items[j].getChildById("idTypeUser").getValue() + ",";
                            cFileSections[GLOBAL_VARS_SECTION] += "\n          .processIdentifierEnable = " + mpuRegionConfigurationMaster_Items[j].getChildById("processIdentifierEnable").getValue();
                            cFileSections[GLOBAL_VARS_SECTION] += "\n       }";
                            if (j != (mpuRegionConfigurationMaster_Items.length - 1))
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += "," + NEW_LINE;
                            }
                        }
                            cFileSections[GLOBAL_VARS_SECTION] += "\n};";
                        }
                    }
                }
            }
            else {
                throw "Configuration does not exist";
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
}