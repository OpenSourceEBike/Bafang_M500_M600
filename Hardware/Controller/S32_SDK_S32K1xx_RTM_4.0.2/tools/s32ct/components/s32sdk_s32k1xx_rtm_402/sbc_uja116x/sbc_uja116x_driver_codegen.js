/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");


function SBC_UJA116X_DRIVER_GetDeviceName(sbcModel) {
    var sbcModelText = "";
    
    switch (sbcModel)
    {
        case "0_LV":
            sbcModelText = "UJA1169ATK";
            break;
        case "1_LV":
            sbcModelText = "UJA1169ATK/X";
            break;
        case "2_LFPV":
            sbcModelText = "UJA1169ATK/F";
            break;
        case "3_LFPV":
            sbcModelText = "UJA1169ATK/X/F";
            break;
        case "4_L":
            sbcModelText = "UJA1169ATK/3";
            break;
        case "5_LFP":
            sbcModelText = "UJA1169ATK/F/3";
            break;
        case "6_PV":
            sbcModelText = "UJA1168ATK";
            break;
        case "7_PV":
            sbcModelText = "UJA1168ATK/VX";
            break;
        case "8_FPV":
            sbcModelText = "UJA1168ATK/FD";
            break;
        case "9_FPV":
            sbcModelText = "UJA1168ATK/VX/FD";
            break;
        default:
            break;
    }
    
    return sbcModelText;
}

// SBC component code generator class.
function SBC_UJA116X_DRIVER_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function() {
        return "SBC_UJA116X_DRIVER_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        // Add Includes info.
        try {
            hFileSections[INCLUDES_SECTION] += addInclude("sbc_uja116x_driver.h");
            var initStructSbc = getSetting(this.uiConfig, this.uiConfig.getName() + ".sbcCfgArray");
            var initStructSbcConfig = initStructSbc.getChildren();            
            var countSbcCfg = initStructSbcConfig.length;
            if (countSbcCfg != 0) {
                hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                for (var i = 0; i < countSbcCfg; i++) {
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "/* SBC UJA116xA Configuration " + i + ". */" + NEW_LINE;
                    var sbcNameCfg = initStructSbcConfig[i].getChildById("name");
                    if (sbcNameCfg == null) {
                        hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "FAIL";
                    }
                    
                    var sbcCfgRo = (initStructSbcConfig[i].getChildById("ro").getValue() == true) ? "const " : "";

                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += "extern " + sbcCfgRo + "sbc_int_config_t " + sbcNameCfg.getValue() + ";" + NEW_LINE;
                    hFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                }
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections) {
        // Get childContext class to extract peripheral name and functional group.
        try {
            var sbcModel = getSetting(this.uiConfig, this.uiConfig.getName() + ".sbcDevModel").getValue();
            var sbcModelText = SBC_UJA116X_DRIVER_GetDeviceName(sbcModel);
            var initStructSbc = getSetting(this.uiConfig, this.uiConfig.getName() + ".sbcCfgArray");
            var initStructSbcConfig = initStructSbc.getChildren();
            var countSbcCfg = initStructSbcConfig.length;
            if (countSbcCfg != 0) {
                cFileSections[GLOBAL_VARS_EXTERN_SECTION] += NEW_LINE;
                for (var i = 0; i < countSbcCfg; i++) {
                    cFileSections[GLOBAL_VARS_SECTION] += "/* SBC UJA116xA Configuration " + i + " - device: " + sbcModelText + ". */" + NEW_LINE;
                    var sbcNameCfg = initStructSbcConfig[i].getChildById("name");
                    if (sbcNameCfg == null) {
                        cFileSections[GLOBAL_VARS_EXTERN_SECTION] += "FAIL";
                    }

                    var sbcCfgRo = (initStructSbcConfig[i].getChildById("ro").getValue() == true) ? "const " : "";
                    
                    cFileSections[GLOBAL_VARS_SECTION] += sbcCfgRo + "sbc_int_config_t " + sbcNameCfg.getValue() + " =" + NEW_LINE;                    
                    cFileSections[GLOBAL_VARS_SECTION] += "{" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* Regulator control register group. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .regulatorCtr =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* Regulator control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .regulator =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .pdc   = " + initStructSbcConfig[i].getChildById("regulatorCtr.regulator.pdc").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .v2c   = " + initStructSbcConfig[i].getChildById("regulatorCtr.regulator.v2c").getValue() + "," + NEW_LINE;
                    if (sbcModel == "0_LV" || sbcModel == "1_LV" || sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .v1rtc = " + initStructSbcConfig[i].getChildById("regulatorCtr.regulator.v1rtc").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .v1rtc = SBC_UJA_REGULATOR_V1RTC_90, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* Supply event capture enable register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .supplyEvnt =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .v2oe = " + initStructSbcConfig[i].getChildById("regulatorCtr.supplyEvnt.v2oe").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .v2ue = " + initStructSbcConfig[i].getChildById("regulatorCtr.supplyEvnt.v2ue").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .v1ue = " + initStructSbcConfig[i].getChildById("regulatorCtr.supplyEvnt.v1ue").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* Watchdog control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .watchdog =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .modeControl   = " + initStructSbcConfig[i].getChildById("watchdog.modeControl").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .nominalPeriod = " + initStructSbcConfig[i].getChildById("watchdog.nominalPeriod").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* Mode control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .mode = " + initStructSbcConfig[i].getChildById("mode").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* LIMP home control. */" + NEW_LINE;
                    if (sbcModel == "0_LV" || sbcModel == "1_LV" || sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "4_L" || sbcModel == "5_LFP") {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .lhc = " + initStructSbcConfig[i].getChildById("lhc").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "    .lhc = SBC_UJA_FAIL_SAFE_LHC_FLOAT, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* System event capture enable registers. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .sysEvnt =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .owte  = " + initStructSbcConfig[i].getChildById("sysEvnt.owte").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .spife = " + initStructSbcConfig[i].getChildById("sysEvnt.spife").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* Lock control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .lockMask = (sbc_lock_t)( SBC_UJA_LOCK_LK0C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK0C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK1C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK1C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK2C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK2C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK3C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK3C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK4C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK4C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK5C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK5C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") \\" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            | SBC_UJA_LOCK_LK6C_F(";
                    cFileSections[GLOBAL_VARS_SECTION] += (initStructSbcConfig[i].getChildById("lockMask.LK6C").getValue()) ? "1U" : "0U";
                    cFileSections[GLOBAL_VARS_SECTION] += ") )," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* CAN configuration group. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .can =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* CAN control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .canConf =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .cfdc  = " + initStructSbcConfig[i].getChildById("can.canConf.cfdc").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .cfdc  = SBC_UJA_CAN_CFDC_DIS,  /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .pncok = " + initStructSbcConfig[i].getChildById("can.canConf.pncok").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .pncok = SBC_UJA_CAN_PNCOK_DIS, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .cpnc  = " + initStructSbcConfig[i].getChildById("can.canConf.cpnc").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .cpnc  = SBC_UJA_CAN_CPNC_DIS,  /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "            .cmc   = " + initStructSbcConfig[i].getChildById("can.canConf.cmc").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* Transceiver event capture enable register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .canTransEvnt =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .cbse = " + initStructSbcConfig[i].getChildById("can.canTransEvnt.cbse").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .cfe  = " + initStructSbcConfig[i].getChildById("can.canTransEvnt.cfe").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "            .cwe  = " + initStructSbcConfig[i].getChildById("can.canTransEvnt.cwe").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* CAN data rate selection. */" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "        .datRate = " + initStructSbcConfig[i].getChildById("can.datRate").getValue() + "," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "        .datRate = SBC_UJA_DAT_RATE_CDR_50KB, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* ID registers. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .identif =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) " + initStructSbcConfig[i].getChildById("can.identif.id0").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) " + initStructSbcConfig[i].getChildById("can.identif.id1").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) " + initStructSbcConfig[i].getChildById("can.identif.id2").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) " + initStructSbcConfig[i].getChildById("can.identif.id3").getValue() + "U," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identifier_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* ID mask registers. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .mask =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) " + initStructSbcConfig[i].getChildById("can.mask.idMask0").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) " + initStructSbcConfig[i].getChildById("can.mask.idMask1").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) " + initStructSbcConfig[i].getChildById("can.mask.idMask2").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) " + initStructSbcConfig[i].getChildById("can.mask.idMask3").getValue() + "U," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_identif_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* Frame control register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .frame =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .ide  = " + initStructSbcConfig[i].getChildById("can.frame.ide").getValue() + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            .pndm = " + initStructSbcConfig[i].getChildById("can.frame.pndm").getValue() + "," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            .dlc  = (sbc_frame_ctr_dlc_t) " + initStructSbcConfig[i].getChildById("can.frame.dlc").getValue() + "U," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            .ide  = SBC_UJA_FRAME_CTR_IDE_11B,    /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            .pndm = SBC_UJA_FRAME_CTR_PNDM_DCARE, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            .dlc  = (sbc_frame_ctr_dlc_t) 0U,     /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        /* Data mask 0 - 7 configuration. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .dataMask =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        {" + NEW_LINE;
                    if (sbcModel == "2_LFPV" || sbcModel == "3_LFPV" || sbcModel == "5_LFP" || sbcModel == "6_PV" || sbcModel == "7_PV" || sbcModel == "8_FPV" || sbcModel == "9_FPV") {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask0").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask1").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask2").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask3").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask4").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask5").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask6").getValue() + "U," + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) " + initStructSbcConfig[i].getChildById("can.dataMask.dataMask7").getValue() + "U," + NEW_LINE;
                    } else {
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                        cFileSections[GLOBAL_VARS_SECTION] += "            (sbc_data_mask_t) 0U, /* Not available on " + sbcModelText + ". */" + NEW_LINE;
                    }
                    cFileSections[GLOBAL_VARS_SECTION] += "        }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    /* WAKE pin event capture enable register. */" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    .wakePin =" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    {" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .wpre = " + initStructSbcConfig[i].getChildById("wakePin.wpre").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "        .wpfe = " + initStructSbcConfig[i].getChildById("wakePin.wpfe").getValue() + "," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "    }," + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += "};" + NEW_LINE;
                    cFileSections[GLOBAL_VARS_SECTION] += NEW_LINE;
                }
            }
        } catch (error) {
            scriptApi.logError(error);
        }
    }
}