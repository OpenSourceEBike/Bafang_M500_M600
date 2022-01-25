/*
 * Copyright 2019-2020 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/// @function MPU_PrintComment
function MPU_PrintComment(comment)
{
    var out = NEW_LINE;
    out += "/*******************************************************************************\n * ";
    out += comment;
    out += "\n *******************************************************************************/\n";
    return out;
}
/// @function MPU_PrintArrayStruct
/// Returns Array structure string based on UI settings (string).
/// @param {name} the array name (string).
/// @param {readOnly} read only attribute (bool).
/// @param {type} the array type (string).
/// @param {array} the array object that needs to be converted (string).
function MPU_PrintArrayStruct(childContext, name, readOnly, type, array) {
    var out = "";//output string
    var constStruct = "\n";//const qualifier for the structure
    var struct = array.getChildren();

    if (struct.length > 0) {
        if (readOnly) {
            constStruct = "\nconst ";
        }

        out = constStruct + type + " " + name + "[] = {";
        for (var i = 0; i < struct.length; i++) {
            var children = struct[i].getChildren();
            var firstStructField = true;
            out += NEW_LINE + "    /* Configuration " + i + " */";
            out += NEW_LINE + "    {" + NEW_LINE;

            for (var j in children) {
                var chld = children[j];
                var settingValClass = chld.getClass();
                var uint_t = "";
                var pointer = "";
                if (chld.isOptionSet("cStructField") && chld.isAvailable()) {
                    if (firstStructField) {
                        firstStructField = false;
                    }
                    else {
                        out += "," + NEW_LINE;
                    }

                    if ((childContext.getComponentInstanceConfig().getPeripheral().substring(0, 7) == "MPU_R52") || (childContext.getComponentInstanceConfig().getPeripheral().substring(0, 6) == "MPU_M7")) {
                        if (chld.getName() == "masterAccRight") {
                            pointer = "&";
                        }
                    }
                    if ((chld.getName() == "extension") && (chld.getStringValue() != "NULL")) {
                        pointer = "&";
                    }
                    if (chld.getType() == "INTEGER") {
                        uint_t = "U"
                    }

                    out += "        ." + chld.getName() + ' = ' + pointer + chld.getStringValue() + uint_t;
                }
            }

            out += NEW_LINE + "    }";
            if (i < (struct.length - 1)) {
                out += ",";
            }
        }

        out += NEW_LINE + "};" + NEW_LINE;
    }

    return out;
}

/// @function MPU_GetSlaveProtected
/// Returns The slave protected and module(port number) (string).
/// @param {childContext} the childContext class (class).
/// @param {memInstance} the memory instance number (string).
/// @param {peripheralInstance} the instance of periferal (string).
function MPU_GetSlaveProtected(childContext, memInstance, peripheralInstance) {
    var slaveProtected = "`N/A`";

    switch (childContext.getComponentInstanceConfig().getMode()["id"]) {
        case 'mpu_pal_basic_s32v':
            switch (memInstance) {
                case '0':
                    slaveProtected = "`Up to 16 regions, protect DDR_0(0) and DDR_1(1)`";
                    break;
                case '1':
                    slaveProtected = "`Up to 4 regions, protect SRAM(No Error Capture) - Special for SRAM_STREAMDMA`";
                    break;
                case '2':
                    slaveProtected = "`Up to 16 regions, protect SRAM(128-bit masters, APEX0(0), APEX1(1), FastDMA(2), and Cores(3))`";
                    break;
                case '3':
                    slaveProtected = "`Up to 16 regions, protect SRAM(64-bit masters, PDI and H264(0), PCIE and ENET(1), Other(2))`";
                    break;
                case '4':
                    slaveProtected = "`Up to 4 regions, protect APEX0(0) and APEX1(1)`";
                    break;
                case '5':
                    slaveProtected = "`Up to 4 regions, protect PCIE(0)`";
                    break;
                case '6':
                    slaveProtected = "`Up to 4 regions, protect SEQ(0)`";
                    break;
                case '7':
                    slaveProtected = "`Up to 16 regions, protect TCM(0), NIC(1), and QSPI(2)`";
                    break;
                default:
                    break;
            }
            break;
        case 'mpu_pal_basic_s32s':
            switch (memInstance) {
                case '0':
                    slaveProtected = "`Up to 4 regions, protect FlexNoC_1 internal registers`";
                    break;
                case '2':
                    slaveProtected = "`Up to 16 regions, protect SRAM_1(0), SRAM_2(1), SRAM_3(2), SRAM_0(3)`";
                    break;
                case '3':
                    slaveProtected = "`Up to 16 regions, protect SRAM_5(0), SRAM_6(1), SRAM_7(2), SRAM_4(3)`";
                    break;
                case '4':
                    slaveProtected = "`Up to 16 regions, protect SRAM_9(0), SRAM_8(1), SRAM_11(2), SRAM_10(3)`";
                    break;
                case '5':
                    slaveProtected = "`Up to 16 regions, protect SRAM_12(0), SRAM_13(1), SRAM_14(2), SRAM_15(3)`";
                    break;
                case '7':
                    slaveProtected = "`Up to 16 regions, protect S_FLASH(0)(external)`";
                    break;
                case '12':
                    slaveProtected = "`Up to 4 regions, protect FlexNoC_0 registers(0)`";
                    break;
                case '13':
                    slaveProtected = "`Up to 16 regions, protect S_DBG_APB(0)`";
                    break;
                case '14':
                    slaveProtected = "`Up to 16 regions, protect S_DBG_APB(0)`";
                    break;
                case '15':
                    slaveProtected = "`Up to 16 regions, protect R52 Cluster 1 TCM(0), R52 Cluster 0 TCM(1)`";
                    break;
                default:
                    break;
            }
            break;
        case 'mpu_pal_basic_s32g':
        case 'mpu_pal_basic_s32r':
            if (peripheralInstance == "XRDC_0")
            {
                switch (memInstance)
                {
                    case '0':
                        slaveProtected = "`Up to 16 regions, Chip-specific`";
                        break;
                    case '2':
                        slaveProtected = "`Up to 16 regions, protect SRAM_1(0), SRAM_2(1), SRAM_3(2), SRAM_0(3)`";
                        break;
                    case '3':
                        slaveProtected = "`Up to 16 regions, protect SRAM_5(0), SRAM_6(1), SRAM_7(2), SRAM_4(3)`";
                        break;
                    case '4':
                        slaveProtected = "`Up to 16 regions, protect SRAM_9(0), SRAM_8(1), SRAM_11(2), SRAM_10(3)`";
                        break;
                    case '5':
                        slaveProtected = "`Up to 16 regions, protect SRAM_12(0), SRAM_13(1), SRAM_14(2), SRAM_15(3)`";
                        break;
                    case '7':
                        slaveProtected = "`Up to 16 regions, protect S_FLASH(0)(external)`";
                        break;
                    case '8':
                        slaveProtected = "`Up to 16 regions, protect S_LLCE(0)`";
                        break;
                    case '9':
                        slaveProtected = "`Up to 16 regions, protect S_GIC-500(0)`";
                        break;
                    case '11':
                        slaveProtected = "`Up to 12 regions, protect M7_TCM_0(0), M7_TCM_1(1), and M7_TCM_2(2)`";
                        break;
                    case '12':
                        slaveProtected = "`Up to 4 regions, protect NoC_0 registers(0)`";
                        break;
                    case '13':
                        slaveProtected = "`Up to 16 regions, protect S_DBG_APB(0)`";
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch (memInstance)
                {
                    case '0':
                        slaveProtected = "`Up to 4 regions, protect STDBY_SRAM`";
                        break;
                    case '1':
                        slaveProtected = "`Up to 4 regions, protect LLCE`";
                        break;
                    case '2':
                        slaveProtected = "`Up to 16 regions, protect Concerto`";
                        break;
                    case '3':
                        slaveProtected = "`Up to 4 regions, protect PCLe_1`";
                        break;
                    case '4':
                        slaveProtected = "`Up to 4 regions, protect PFE`";
                        break;
                    case '5':
                        slaveProtected = "`Up to 4 regions, protect Service_Regs`";
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

    return slaveProtected;
}

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

// MPU_PAL component code generator class.
function MPU_PAL_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "MPU_PAL_CodeGenerator";
    }

    /* Generates the pal_cfg.h file */
    this.genHCfgFile = function (hCfgFileSections) {

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        hCfgFileSections[FILE_NAME_SECTION] += "mpu_pal_cfg.h\n";

        // Add define info
        if (peripheralName.indexOf("XRDC") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("mpu_pal_cfg.h", "MPU_OVER_XRDC", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("mpu_pal_cfg.h", "MPU_OVER_XRDC_INSTANCE_COUNT");
        }
        else if (peripheralName.indexOf("MPU_R52") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("mpu_pal_cfg.h", "MPU_OVER_MPU_R52", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("mpu_pal_cfg.h", "MPU_OVER_MPU_R52_INSTANCE_COUNT");
        }
        else if (peripheralName.indexOf("MPU_M7") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("mpu_pal_cfg.h", "MPU_OVER_MPU_M7", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("mpu_pal_cfg.h", "MPU_OVER_MPU_M7_INSTANCE_COUNT");
        }
        else if (peripheralName.indexOf("SMPU") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("mpu_pal_cfg.h", "MPU_OVER_SMPU", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("mpu_pal_cfg.h", "MPU_OVER_SMPU_INSTANCE_COUNT");
        }
        else if (peripheralName.indexOf("MPU") == 0) {
            hCfgFileSections[DEFINES_SECTION] += addPalDefine("mpu_pal_cfg.h", "MPU_OVER_MPU", 1);
            hCfgFileSections[PAL_INST_DEFINES_SECTION] += addPalInstDefine("mpu_pal_cfg.h", "MPU_OVER_MPU_INSTANCE_COUNT");
        }
        else
        {
            // Do nothing
        }
    }

    // Adds component specific info to header file sections.
    this.genHFile = function (hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        hFileSections[MISRA_COMMENTS] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.1, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        hFileSections[MISRA_COMMENTS] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.2, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        hFileSections[MISRA_COMMENTS] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.4, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        hFileSections[MISRA_COMMENTS] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.5, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        // General variables
        var readOnly;
        var define;

        // Get childContext class to extract peripheral name and functional group.
        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var instanceName = childContext.getComponentInstanceConfig().getName();

        // Get hardware configuration childContext class
        var hwConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Hardware");
        // Get memory configuration childContext class
        var memConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Configuration");
        // Get user configuration
        var userConfig = memConfig.getChildById("mpu_pal_UserConfig").getChildren();
        // Get master configuration
        var masterConfig = memConfig.getChildById("mpu_pal_MasterConfig").getChildren();
        // Extension configuration enable
        var extConfigEnable = true;

        // Add Includes info.
        hFileSections[INCLUDES_SECTION] += addInclude("stddef.h");
        hFileSections[INCLUDES_SECTION] += addInclude("mpu_pal.h");

        // Add memory instances
        if (peripheralName.substring(0, 4) == "XRDC") {
            // Get instance configuration childContext class
            var memInstance = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Memory").getChildren();
            hFileSections[DEFINES_SECTION] += "/* Memory instance count */" + NEW_LINE;
            hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_INSTANCE_CNT", "(" + memInstance.length + "U)");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*\n * Memory instances\n */" + NEW_LINE;
            for (var i = 0; i < memInstance.length; i++) {
                // Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Instance " + memInstance[i].getChildById("memInstance").getStringValue() + " - " + memInstance[i].getChildById("memInfo").getStringValue() + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const " + "mpu_instance_t", instanceName + "_Instance" + memInstance[i].getChildById("memInstance").getStringValue());
            }
            // Get extension configuration
            var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig").getChildren();
            var extensionType = "mpu_extension_xrdc_region_t";
        }
        else if (peripheralName.substring(0, 7) == "MPU_R52") {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/*\n * Memory instances\n */" + NEW_LINE;
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const " + "mpu_instance_t", instanceName + "_Instance");

            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE + NEW_LINE + "/*\n * Hardware configuration\n */" + NEW_LINE;
            readOnly = (hwConfig.getChildById("readOnly").getValue() ? " const " : " ");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern" + readOnly + "mpu_hw_config_t", hwConfig.getChildById("name").getStringValue());
            if (hwConfig.getChildById("attrCnt").getValue() > 0) {
                hFileSections[DEFINES_SECTION] += "/* Memory attribute count */" + NEW_LINE;
                define = instanceName.toUpperCase() + "_MEM_ATTR_CNT";
                hFileSections[DEFINES_SECTION] += addDefine(define, "(" + hwConfig.getChildById("attrCnt").getValue() + "U)");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Memory attribute configuration */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern mpu_extension_mpu_r52_hardware_t", hwConfig.getChildById("name").getStringValue() + "Memory");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern mpu_r52_mem_attr_t", hwConfig.getChildById("name").getStringValue() + "MemoryAttr[" + define + "]");
            }

            // Get extension configuration
            var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig_Mpu_R52").getChildren();
            var extensionType = "mpu_extension_mpu_r52_region_t";
        }
        else if (peripheralName.substring(0, 6) == "MPU_M7")
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Memory instances");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const " + "mpu_instance_t", instanceName + "_Instance");
            var cacheConfig = memConfig.getChildById("mpu_pal_ExtensionConfigurations_Mpu_M7").getChildById("mpu_pal_cacheConfig_Mpu_M7").getChildren();
            var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfigurations_Mpu_M7").getChildById("mpu_pal_extensionConfig_Mpu_M7").getChildren();
            var extensionType = "mpu_extension_mpu_m7_region_t";
        }
        else if (peripheralName.substring(0, 4) == "SMPU")
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Memory instances");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const " + "mpu_instance_t", instanceName + "_Instance");
            // Get extension configuration
            var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig").getChildren();
            var extensionType = "mpu_extension_smpu_region_t";
        }
        else if (peripheralName.substring(0, 3) == "MPU")
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Memory instances");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern const " + "mpu_instance_t", instanceName + "_Instance");
            // Get extension configuration
            extConfigEnable = false;
        }
        else
        {
            // Do nothing
        }

        // Add user configuration
        hFileSections[DEFINES_SECTION] += NEW_LINE + "/* Region configuration count */" + NEW_LINE;
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE + NEW_LINE + "/*\n * User configurations\n */" + NEW_LINE;
        var userConfigCnt = 0;
        for (var i = 0; i < userConfig.length; i++) {
            var regionConfig = userConfig[i].getChildById("mpu_pal_RegionConfig").getChildren();
            // Add define variables
            if (regionConfig.length > 0) {
                userConfigCnt++;
                define = instanceName.toUpperCase() + "_REGION_CFG_CNT" + i;
                hFileSections[DEFINES_SECTION] += addDefine(define, "(" + regionConfig.length + "U)");
                // Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Configuration " + i + " */" + NEW_LINE;
                readOnly = (userConfig[i].getChildById("readOnly").getValue() ? " const " : " ");
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern" + readOnly + "mpu_region_config_t", userConfig[i].getChildById("name").getStringValue() + "[" + define + "]");
            }
        }
        hFileSections[DEFINES_SECTION] += NEW_LINE + "/* User configuration count */" + NEW_LINE;
        hFileSections[DEFINES_SECTION] += addDefine(instanceName.toUpperCase() + "_USER_CFG_CNT", "(" + userConfigCnt + "U)");

        // Add domain configuration
        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Domain configurations");
        var masterCnt = "";
        if (peripheralName.substring(0, 7) == "MPU_R52") {
            // Do nothing
        }
        else if (peripheralName.substring(0, 6) == "MPU_M7") {
            // Do nothing
        }
        else if (peripheralName.substring(0, 4) == "XRDC") {
            masterCnt = "[FEATURE_XRDC_DOMAIN_COUNT]";
        }
        else if (peripheralName.substring(0, 4) == "SMPU") {
            masterCnt = "[FEATURE_SMPU_MASTER_COUNT]";
        }
        else if (peripheralName.substring(0, 3) == "MPU") {
            masterCnt = "[FEATURE_MPU_MASTER_COUNT]";
        }
        else {
            // Do nothing
        }
        for (var i = 0; i < masterConfig.length; i++) {
            // Add external global variables
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Domain Configuration " + i + " */" + NEW_LINE;
            readOnly = (masterConfig[i].getChildById("readOnly").getValue() ? " const " : " ");
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern" + readOnly + "mpu_master_access_permission_t", masterConfig[i].getChildById("name").getStringValue() + masterCnt);
        }

        // Add extension configuration
        if (extConfigEnable == true)
        {
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Extension configurations");
            for (var i = 0; i < extensionConfig.length; i++) {
                // Add external global variables
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Extension Configuration " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + extensionType, extensionConfig[i].getChildById("name").getStringValue());
            }
        }
        if (peripheralName.substring(0, 6) == "MPU_M7")
        {
            // Add extension configuration
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Cache Policy Configuration Ptr");
            for (var i = 0; i < cacheConfig.length; i++) {
                // Add Cache Policy Config
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Cache Policy Config " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + "mpu_m7_cache_config_t", cacheConfig[i].getChildById("name").getStringValue());
            }
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Hardware Configuration");
            var hwMpuM7Config = memConfig.getChildById("mpu_pal_hardwareExtensionConfig_Mpu_M7").getChildById("mpu_pal_Hardware_Mpu_M7").getChildren();
            var hwMpuM7ExtensionConfig = memConfig.getChildById("mpu_pal_hardwareExtensionConfig_Mpu_M7").getChildById("mpu_pal_HardwareExtension_Mpu_M7").getChildren();
            for (var i = 0; i < hwMpuM7Config.length; i++) {
                // Add Cache Policy Config
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Hardware Config " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + "mpu_hw_config_t", hwMpuM7Config[i].getChildById("name").getStringValue());
            }
            hFileSections[GLOBAL_VARS_EXTERN_SECTION] += MPU_PrintComment("Hardware extension");
            for (var i = 0; i < hwMpuM7ExtensionConfig.length; i++) {
                // Add Cache Policy Config
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* Hardware extension Config " + i + " */" + NEW_LINE;
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += declareVar("extern " + "mpu_extension_mpu_m7_hardware_t", hwMpuM7ExtensionConfig[i].getChildById("name").getStringValue());
            }
        }

    }


    // Adds component specific info to source file sections.
    this.genCFile = function (cFileSections) {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.1, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.2, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.4, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        cFileSections[MISRA_COMMENTS_C] += " * @section [global]\n * Violates MISRA 2012 Required Rule 5.5, identifier clash.\n * The supported compilers use more than 31 significant characters for identifiers.\n *\n";
        try {
            // Variables
            var readOnly = "";
            var arrayConfig = "";
            var arrayName = "";
            var arrayType = "";
            var modulo = 32;
            var extConfigEnable = true;

            // Get childContext class to extract peripheral name and functional group.
            var childContext = uiConfig.getChildContext();
            var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
            var instanceName = childContext.getComponentInstanceConfig().getName();

            // Add memory instances
            cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Memory instances");
            if (peripheralName.substring(0, 4) == "XRDC")
            {
                var inst = peripheralName.substring(5, 6);
                if (inst == "") {
                    inst = 0;
                }
                var arrayConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Memory").getChildren();
                for (var i = 0; i < arrayConfig.length; i++)
                {
                    arrayConfig[i].getChildById("memInfo").setValue(MPU_GetSlaveProtected(childContext, arrayConfig[i].getChildById("memInstance").getStringValue(), peripheralName.substring(0, 6)));
                    var instanc = (inst << 4) | arrayConfig[i].getChildById("memInstance").getValue();
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Configuration " + i + " */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const mpu_instance_t " + instanceName + "_Instance" + arrayConfig[i].getChildById("memInstance").getStringValue() + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instType = " + "MPU_INST_TYPE_XRDC,";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instIdx = " + instanc + "U";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE;
                }
            }
            else if (peripheralName.substring(0, 7) == "MPU_R52")
            {
                var inst = peripheralName.substring(8, 9);
                if (inst == "") {
                    inst = 0;
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const mpu_instance_t " + instanceName + "_Instance = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instType = " + "MPU_INST_TYPE_MPU_R52,";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instIdx = " + inst + "U";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"

                // Get hardware configuration childContext class
                var hwConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Hardware");
                hwConfig.getChildById("attrCnt").setValue(hwConfig.getChildById("mpu_pal_MemoryAttr").getChildren().length);
                if (hwConfig.getChildById("name").getStringValue() == "") {
                    hwConfig.getChildById("name").setValue(instanceName + "_Hardware");
                }
                var extConfigName = "NULL";
                if (hwConfig.getChildById("attrCnt").getValue() > 0) {
                    extConfigName = "&" + hwConfig.getChildById("name").getStringValue() + "Memory";
                }
                readOnly = (hwConfig.getChildById("readOnly").getValue() ? "const " : "");
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + NEW_LINE + "/*\n * Hardware configuration\n */";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_hw_config_t " + hwConfig.getChildById("name").getStringValue() + " = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .defaultMapEn = " + hwConfig.getChildById("defaultMapEn").getValue() + ",";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .extension = " + extConfigName;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"

                if (hwConfig.getChildById("attrCnt").getValue() > 0)
                {
                    extConfigName = hwConfig.getChildById("name").getStringValue() + "Memory";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Memory attribute configuration */";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "mpu_extension_mpu_r52_hardware_t " + extConfigName + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .attrCnt = " + hwConfig.getChildById("attrCnt").getValue() + "U,";
                    extConfigName = hwConfig.getChildById("name").getStringValue() + "MemoryAttr";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .memSetAttr = " + extConfigName;
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"

                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Attribute configuration */";
                    arrayConfig = hwConfig.getChildById("mpu_pal_MemoryAttr");
                    arrayName = extConfigName;
                    readOnly = false;
                    arrayType = "mpu_r52_mem_attr_t";
                    cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintArrayStruct(childContext, arrayName, readOnly, arrayType, arrayConfig);
                }
            }
            else if (peripheralName.substring(0, 6) == "MPU_M7")
            {
                var inst = 0;
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const mpu_instance_t " + instanceName + "_Instance = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instType = " + "MPU_INST_TYPE_MPU_M7,";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instIdx = " + inst + "U";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
            }
            else if (peripheralName.substring(0, 4) == "SMPU")
            {
                var inst = peripheralName.substring(5, 6);
                if (inst == "") {
                    inst = 0;
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const mpu_instance_t " + instanceName + "_Instance = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instType = " + "MPU_INST_TYPE_SMPU,";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instIdx = " + inst + "U";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
            }
            else if (peripheralName.substring(0, 3) == "MPU")
            {
                extConfigEnable = false;
                var inst = peripheralName.substring(4, 5);
                if (inst == "") {
                    inst = 0;
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "const mpu_instance_t " + instanceName + "_Instance = {";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instType = " + "MPU_INST_TYPE_MPU,";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .instIdx = " + inst + "U";
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
            }
            else
            {
                // Do nothing
            }

            // Get memory configuration childContext class
            var memConfig = getSetting(this.uiConfig, this.uiConfig.getName() + ".mpu_pal_Configuration");

            // Add domain configurations
            cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Domain configurations");
            var masterConfig = memConfig.getChildById("mpu_pal_MasterConfig").getChildren();
            for (var i = 0; i < masterConfig.length; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Domain Configuration " + i + " */";
                readOnly = (masterConfig[i].getChildById("readOnly").getValue() ? "const " : "");
                if (peripheralName.substring(0, 4) == "XRDC")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_master_access_permission_t " + masterConfig[i].getChildById("name").getStringValue() + "[] = {";
                    var firstStructField = true;
                    for (var j = 0; j < 8; j++) {
                        var domainId = "domain" + j;
                        if (firstStructField) {
                            firstStructField = false;
                        }
                        else {
                            cFileSections[GLOBAL_VARS_SECTION] += ",";
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {"
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .masterNum = " + j + "U,"
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .accessRight = " + masterConfig[i].getChildById(domainId).getStringValue()
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }"
                    }
                }
                else if (peripheralName.substring(0, 7) == "MPU_R52")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_master_access_permission_t " + masterConfig[i].getChildById("name").getStringValue() + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .masterNum = 0U,"
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .accessRight = " + masterConfig[i].getChildById("accessRight").getStringValue()
                }
                else if (peripheralName.substring(0, 6) == "MPU_M7")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_master_access_permission_t " + masterConfig[i].getChildById("name").getStringValue() + " = {";
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .masterNum = 0U,"
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .accessRight = " + masterConfig[i].getChildById("coreDomain").getStringValue()
                }
                else if (peripheralName.substring(0, 4) == "SMPU")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_master_access_permission_t " + masterConfig[i].getChildById("name").getStringValue() + "[] = {";
                    var children = masterConfig[i].getChildren();
                    var firstStructField = true;
                    for (var j in children) {
                        var chld = children[j];
                        if (chld.isOptionSet("cStructField") && chld.isAvailable()) {
                            if (firstStructField) {
                                firstStructField = false;
                            }
                            else {
                                cFileSections[GLOBAL_VARS_SECTION] += ",";
                            }
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {"
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .masterNum = FEATURE_SMPU_MASTER_" + chld.getName() + ","
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .accessRight = " + chld.getStringValue()
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }"
                        }
                    }
                }
                else if (peripheralName.substring(0, 3) == "MPU")
                {
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + "mpu_master_access_permission_t " + masterConfig[i].getChildById("name").getStringValue() + "[] = {";
                    var masterAccConfig = masterConfig[i].getChildById("mpu_pal_Master").getChildren();
                    var firstStructField = true;
                    for (var j = 0; j < masterAccConfig.length; j++) {
                        if (firstStructField) {
                            firstStructField = false;
                        }
                        else {
                            cFileSections[GLOBAL_VARS_SECTION] += ",";
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {"
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .masterNum = " + masterAccConfig[j].getChildById("masterNum").getValue() + ","
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .accessRight = " + masterAccConfig[j].getChildById("supervisorAccessRight").getStringValue() + "_" + masterAccConfig[j].getChildById("userAccessRight").getStringValue();
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }"
                    }
                }
                else
                {
                    // Do nothing
                }
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};" + NEW_LINE
            }

            // Add extension configurations
            if (extConfigEnable == true)
            {
                cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Extension configurations");
                if (peripheralName.substring(0, 4) == "XRDC")
                {
                    var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig").getChildren();
                    // var extensionID = "mpu_pal_ExtensionConfig";
                    var extensionType = "mpu_extension_xrdc_region_t";
                    modulo = 32;

                    for (var i = 0; i < extensionConfig.length; i++)
                    {
                        if (extensionConfig[i].getChildById("hStartAddr") != null)
                        {
                            if ((childContext.getComponentInstanceConfig().getMode()["id"] == "mpu_pal_basic_s32s") && (peripheralName == "XRDC_1"))
                            {
                                extensionConfig[i].getChildById("hStartAddr").setValue(0);
                            }
                            extensionConfig[i].getChildById("hStartAddr").setValue(MPU_StartAddrModulo(extensionConfig[i].getChildById("hStartAddr").getValue(), 1));
                        }
                        if (extensionConfig[i].getChildById("hEndAddr") != null)
                        {
                            if ((childContext.getComponentInstanceConfig().getMode()["id"] == "mpu_pal_basic_s32s") && (peripheralName == "XRDC_1"))
                            {
                                extensionConfig[i].getChildById("hEndAddr").setValue(0);
                            }
                            extensionConfig[i].getChildById("hEndAddr").setValue(MPU_EndAddrModulo(extensionConfig[i].getChildById("hEndAddr").getValue(), 1));
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(extensionConfig[i], extensionType, "");
                    }
                } 
                else if (peripheralName.substring(0, 7) == "MPU_R52")
                {
                    var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig_Mpu_R52").getChildren();
                    // var extensionID = "mpu_pal_ExtensionConfig_Mpu_R52";
                    var extensionType = "mpu_extension_mpu_r52_region_t";
                    modulo = 64;
                    for (var i = 0; i < extensionConfig.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(extensionConfig[i], extensionType, "");
                    }
                }
                else if (peripheralName.substring(0, 6) == "MPU_M7")
                {
                    var extensionType = "mpu_extension_mpu_m7_region_t";
                    var hwExtensionID = "";
                    var hwExtensionType = "";
                    modulo = 32;

                    // Print extension Configuration
                    var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfigurations_Mpu_M7").getChildById("mpu_pal_extensionConfig_Mpu_M7").getChildren();
                    var cacheConfig = memConfig.getChildById("mpu_pal_ExtensionConfigurations_Mpu_M7").getChildById("mpu_pal_cacheConfig_Mpu_M7").getChildren();
                    var pointControl = "";
                    for (var i = 0; i < extensionConfig.length; i++)
                    {
                        if ((extensionConfig[i].getChildById("cachePtr").getStringValue() == "") || (extensionConfig[i].getChildById("cachePtr").getStringValue() == "NULL"))
                        {
                            extensionConfig[i].getChildById("cachePtr").setValue("NULL");
                        }
                        else
                        {
                            var checkPtrCache = false;
                            pointControl = "&";
                            for (var j = 0; j < cacheConfig.length; j++)
                            {
                                if (extensionConfig[i].getChildById("cachePtr").getStringValue() == cacheConfig[j].getChildById("name").getStringValue())
                                {
                                    checkPtrCache = true;
                                    break;
                                }
                            }
                            if (checkPtrCache != true)
                            {
                                scriptApi.logError("The name of Cache Configuration are not available.");
                            }
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Extension Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "mpu_extension_mpu_m7_region_t " + extensionConfig[i].getChildById("name").getStringValue() + " = {";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .memType     = " + extensionConfig[i].getChildById("memType").getStringValue() + ",";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .cachePtr    = " + pointControl + extensionConfig[i].getChildById("cachePtr").getStringValue() + ",";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .subRegMask  = " + extensionConfig[i].getChildById("subRegMask").getStringValue() + ",";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .shareable   = " + extensionConfig[i].getChildById("shareable").getStringValue();
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
                    }
                    // Cache extension config
                    cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Cache configurations");
                    for (var i = 0; i < cacheConfig.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Cache Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(cacheConfig[i], "mpu_m7_cache_config_t", "");
                    }
                    // Hardware configuration
                    cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Hardware Configuration");
                    var hwMpuM7Config = memConfig.getChildById("mpu_pal_hardwareExtensionConfig_Mpu_M7").getChildById("mpu_pal_Hardware_Mpu_M7").getChildren();
                    var hwMpuM7ExtensionConfig = memConfig.getChildById("mpu_pal_hardwareExtensionConfig_Mpu_M7").getChildById("mpu_pal_HardwareExtension_Mpu_M7").getChildren();
                    for (var i = 0; i < hwMpuM7Config.length; i++)
                    {
                        var pointControl = "";
                        if ((hwMpuM7Config[i].getChildById("extension").getStringValue() == "") || (hwMpuM7Config[i].getChildById("extension").getStringValue() == "NULL"))
                        {
                            hwMpuM7Config[i].getChildById("extension").setValue("NULL");
                        }
                        else
                        {
                            var checkPtrHwConfig = false;
                            pointControl = "&";
                            for (var j = 0; j < hwMpuM7ExtensionConfig.length; j++)
                            {
                                if (hwMpuM7Config[i].getChildById("extension").getStringValue() == hwMpuM7ExtensionConfig[j].getChildById("name").getStringValue())
                                {
                                    checkPtrHwConfig = true;
                                    break;
                                }
                            }
                            if (checkPtrHwConfig != true)
                            {
                                scriptApi.logError("The name of Hardware extension are not available.");
                            }
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Hardware Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "mpu_hw_config_t " + hwMpuM7Config[i].getChildById("name").getStringValue() + " = {";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .defaultMapEn   = " + hwMpuM7Config[i].getChildById("defaultMapEn").getStringValue() + ",";
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    .extension      = " + pointControl + hwMpuM7Config[i].getChildById("extension").getStringValue();
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};"
                    }
                    // Hardware extension configuration
                    cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("Hardware extension configuration");
                    for (var i = 0; i < hwMpuM7ExtensionConfig.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Hardware extension Configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(hwMpuM7ExtensionConfig[i], "mpu_extension_mpu_m7_hardware_t", "");
                    }
                }
                else if (peripheralName.substring(0, 4) == "SMPU")
                {
                    var extensionConfig = memConfig.getChildById("mpu_pal_ExtensionConfig").getChildren();
                    var extensionType = "mpu_extension_smpu_region_t";
                    modulo = 4;
                    for (var i = 0; i < extensionConfig.length; i++)
                    {
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Extension configuration " + i + " */";
                        cFileSections[GLOBAL_VARS_SECTION] += printConfigStruct(extensionConfig[i], extensionType, "");
                    }
                }
                else
                {
                    // Do nothing
                }
            }
            // Add user configurations
            cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintComment("User configurations");
            var userConfig = memConfig.getChildById("mpu_pal_UserConfig").getChildren();
            for (var i = 0; i < userConfig.length; i++)
            {
                cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "/* Configuration " + i + " */";
                arrayConfig = userConfig[i].getChildById("mpu_pal_RegionConfig");
                arrayName = userConfig[i].getChildById("name").getStringValue();
                readOnly = userConfig[i].getChildById("readOnly").getValue();
                arrayType = "mpu_region_config_t";
                var regionConfig = arrayConfig.getChildren();
                userConfig[i].getChildById("regionCnt").setValue(regionConfig.length);

                for (var j = 0; j < regionConfig.length; j++)
                {
                    regionConfig[j].getChildById("regionNum").setValue(j);
                    regionConfig[j].getChildById("startAddr").setValue(MPU_StartAddrModulo(regionConfig[j].getChildById("startAddr").getValue(), modulo));
                    regionConfig[j].getChildById("endAddr").setValue(MPU_EndAddrModulo(regionConfig[j].getChildById("endAddr").getValue(), modulo));
                    if (extConfigEnable == true)
                    {
                        if (regionConfig[j].getChildById("extension").getStringValue() == "")
                        {
                            regionConfig[j].getChildById("extension").setValue("NULL");
                        }
                    }
                }

                if (childContext.getComponentInstanceConfig().getMode()["id"] == "mpu_pal_basic_s32k")
                {
                    if (regionConfig.length > 0)
                    {
                        readOnly = (userConfig[i].getChildById("readOnly").getValue() ? "const " : "");
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + readOnly + arrayType + " " + arrayName + "[] = {";
                        for (var j = 0; j < regionConfig.length; j++)
                        {
                            // Get PID enable
                            var pid = 0;
                            for (var k = 0; k < masterConfig.length; k++)
                            {
                                if (masterConfig[k].getChildById("name").getStringValue() == regionConfig[j].getChildById("masterAccRight").getStringValue())
                                {
                                    var masterAccConfig = masterConfig[k].getChildById("mpu_pal_Master").getChildren();
                                    for (var l = 0; l < masterAccConfig.length; l++)
                                    {
                                        if ((j == 0) && (masterAccConfig[l].getChildById("masterNum").getValue() == "FEATURE_MPU_MASTER_DEBUGGER"))
                                        {
                                            masterAccConfig[l].getChildById("supervisorAccessRight").setValue("MPU_SUPERVISOR");
                                            masterAccConfig[l].getChildById("userAccessRight").setValue("USER_RWX");
                                            masterAccConfig[l].getChildById("pidEnable").setValue(false);
                                        }
                                        if (masterAccConfig[l].getChildById("pidEnable").getValue() == true)
                                        {
                                            pid |= (1 << l);
                                        }
                                    }
                                    break;
                                }
                            }

                            // Region 0 default configuration
                            regionConfig[j].getChildById("processIdentifier").setValue(MPU_DecToHex(regionConfig[j].getChildById("processIdentifier").getValue()));
                            regionConfig[j].getChildById("processIdMask").setValue(MPU_DecToHex(regionConfig[j].getChildById("processIdMask").getValue()));
                            if (j == 0)
                            {
                                regionConfig[j].getChildById("startAddr").setValue("0x0");
                                regionConfig[j].getChildById("endAddr").setValue("0xFFFFFFFF");
                                regionConfig[j].getChildById("processIdentifier").setValue("0x0");
                                regionConfig[j].getChildById("processIdMask").setValue("0x0");
                            }

                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    /* Region number " + j + " */";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    {";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .startAddr     = " + regionConfig[j].getChildById("startAddr").getStringValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .endAddr    = " + regionConfig[j].getChildById("endAddr").getStringValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .masterAccRight  = " + regionConfig[j].getChildById("masterAccRight").getStringValue() + ",";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .processIdEnable   = " + MPU_DecToHex(pid) + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .processIdentifier   = " + regionConfig[j].getChildById("processIdentifier").getStringValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "        .processIdMask   = " + regionConfig[j].getChildById("processIdMask").getStringValue() + "U,";
                            cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "    }";
                            if (j < (regionConfig.length - 1))
                            {
                                cFileSections[GLOBAL_VARS_SECTION] += ",";
                            }
                        }
                        cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE + "};";
                    }
                }
                else
                {
                    cFileSections[GLOBAL_VARS_SECTION] += MPU_PrintArrayStruct(childContext, arrayName, readOnly, arrayType, arrayConfig);
                }
            }
        }
        catch (err) {
            scriptApi.logInfo(err);
        }
    }
}
