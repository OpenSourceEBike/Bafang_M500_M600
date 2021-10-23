/*
 * Copyright 2018 NXP
 * To be used with S32 Config tools. See Software License Agreement of S32 Config tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

/* TCPIP component toolchain settings class */
function TcpipToolchainSettings(uiConfig) {
    scriptApi.logInfo("Constructor " + uiConfig);
    this.uiConfig = uiConfig;

    /* returns the class name */
    this.toString = function () {
        return "TcpipToolchainSettings";
    }

    this.applyToolchainSettings = function(profile, configSets, isComponentPresent) {
        try{
            this.uiConfig = configSets;

            var osifPresent = isComponentPresent("osif");
            var freertosPresent = isComponentPresent("freertos_driver");

            var genericStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".generic_tab");
            var osChoice = genericStruct.getChildById("osEnable");
            var lowlevelChoice = genericStruct.getChildById("lowlevelIface");
            var txRingSize = genericStruct.getChildById("txRingSize").getValue();
            var rxRingSize = genericStruct.getChildById("rxRingSize").getValue();

            var netifArray = getSetting(this.uiConfig, this.uiConfig.getName()+".netifcfg_tab");

            var appStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".application_tab");
            var boolHttpdRaw = appStruct.getChildById("raw").getChildById("boolHttpdRaw");
            var boolTcpechoRaw = appStruct.getChildById("raw").getChildById("boolTcpechoRaw");
            var boolUdpechoRaw = appStruct.getChildById("raw").getChildById("boolUdpechoRaw");
            var boolLwiperf = appStruct.getChildById("raw").getChildById("boolLwiperf");
            var boolNetbios = appStruct.getChildById("raw").getChildById("boolNetbios");
            var boolHttpdNetconn = appStruct.getChildById("netconn").getChildById("boolHttpdNetconn");
            var boolTcpechoNetconn = appStruct.getChildById("netconn").getChildById("boolTcpechoNetconn");
            var boolUdpechoNetconn = appStruct.getChildById("netconn").getChildById("boolUdpechoNetconn");
            var boolChargenSocket = appStruct.getChildById("socket").getChildById("boolChargenSocket");
            var boolSslechoSocket = appStruct.getChildById("socket").getChildById("boolSslechoSocket");

            var cpuName = profile.getMcuInfo().getPartNumber();

            /* Add OSIF in case not present */
            if (osifPresent != true)
            {
                if (freertosPresent != true)
                {
                    profile.addComponents("platform.driver.osif_baremetal", true, true);
                }
                else
                {
                    profile.addComponents("platform.driver.osif_freertos", true, true);
                }
            }

            /* Add common C include path for tcpip project */
            profile.add_C_Compiler_Includes("SDK/middleware/tcpip/lwip/src/include");
            profile.add_C_Compiler_Includes("SDK/middleware/tcpip/tcpip_stack");
            profile.add_C_Compiler_Includes("SDK/middleware/tcpip/tcpip_stack/ports/OS");
            profile.add_C_Compiler_Includes("SDK/middleware/tcpip/tcpip_stack/ports/platform/generic/gcc/setting");

            if (lowlevelChoice.getValue() == "ENET") {
                profile.add_C_Compiler_Symbol("ENET_TXBD_NUM="+txRingSize);
                profile.add_C_Compiler_Symbol("ENET_RXBD_NUM="+rxRingSize);
            } else if (lowlevelChoice.getValue() == "GMAC") {
                profile.add_C_Compiler_Symbol("GMAC_TXBD_NUM="+txRingSize);
                profile.add_C_Compiler_Symbol("GMAC_RXBD_NUM="+rxRingSize);
            }

            /* Specific compiler options for TCPIP stack */
            profile.add_C_Option("-Wno-address", "com.freescale.s32ds.cross.gnu.tool.c.compiler.option.warnings.other");
            profile.add_C_Option("false", "gnu.c.compiler.option.warnings.pedantic");

            if (lowlevelChoice.getValue() == "ENET")
            {
                profile.add_C_Compiler_Includes("SDK/middleware/tcpip/tcpip_stack/ports/netif/enetif");
                profile.addComponents("platform.middleware.tcpip_port_enet", true, true);
            }
            else if (lowlevelChoice.getValue() == "GMAC")
            {
                profile.add_C_Compiler_Includes("SDK/middleware/tcpip/tcpip_stack/ports/netif/gmacif");
                profile.addComponents("platform.middleware.tcpip_port_gmac", true, true);
            }
            else
            {
                scriptApi.logError("Unsupported lowlevel driver porting for tcpip");
            }

            /* RTOS */
            if (osChoice.getValue() == true)
            {
                if (boolHttpdRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW HTPPD");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_httpd", true, true);
                }
                if (boolTcpechoRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW TCPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_tcpecho", true, true);
                }
                if (boolUdpechoRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW UDPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_udpecho", true, true);
                }
                if (boolLwiperf.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW LWIPERF");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_lwiperf", true, true);
                }
                if (boolNetbios.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW NETBIOS");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_netbiosns", true, true);
                }

                if (boolHttpdNetconn.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING NETCONN HTPPD");
                    profile.addComponents("platform.middleware.tcpip_apps_netconn_httpd", true, true);
                }
                if (boolTcpechoNetconn.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING NETCONN TCPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_netconn_tcpecho", true, true);
                }
                if (boolUdpechoNetconn.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING NETCONN UDPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_netconn_udpecho", true, true);
                }

                if (boolChargenSocket.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING SOCKET CHARGEN");
                    profile.addComponents("platform.middleware.tcpip_apps_socket_chargen", true, true);
                }
                if (boolSslechoSocket.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING WOLFSSL LIBRARY");
                    profile.addComponents("platform.middleware.wolfssl", true, true);

                    /* Add dependencies of wolfssl for different platforms */
                    switch(cpuName)
                    {
                        case "S32V234":
                            profile.addComponents("platform.driver.cse", true, true);
                            profile.addComponents("platform.driver.pit", true, true);
                            break;

                        case "S32K148":
                            profile.add_C_Compiler_Symbol("NXP_SDK_S32Kxxx");
                            profile.addComponents("platform.driver.csec", true, true);
                            profile.addComponents("platform.driver.rtc", true, true);
                            break;

                        case "S32G274A":
                        case "S32R45":
                            profile.addComponents("platform.driver.hse", true, true);
                            profile.addComponents("platform.driver.pit", true, true);
                            break;

                        case "SJA1110":
                            profile.add_C_Compiler_Symbol("NXP_SDK_SJA1110x");
                            break;

                        default:
                            /* Do nothing */
                            break;
                    }

                    var allIpv6 = false;
                    if (null != netifArray)
                    {
                        var netifArrayList = netifArray.getChildren();
                        var netifArraySize = netifArrayList.size();
                        for (var i = 0; i < netifArraySize; i++)
                        {
                            if (netifArrayList[i].getChildById("boolIface").getValue() == true)
                            {
                                if (netifArrayList[i].getChildById("boolIpv6").getValue() == true)
                                {
                                    allIpv6 = true;
                                }
                            }
                        }
                    }

                    if (allIpv6 == true) {
                        profile.add_C_Compiler_Symbol("WOLFSSL_IPV6");
                    }

                    profile.add_C_Compiler_Includes("SDK/middleware/tcpip/wolfssl");
                    profile.add_C_Compiler_Includes("SDK/middleware/tcpip/wolfssl/wolfssl");
                    profile.add_C_Compiler_Includes("SDK/rtos/FreeRTOS/Source");

                    profile.add_C_Compiler_Symbol("NXP_SDK");
                    profile.add_C_Compiler_Symbol("SINGLE_THREADED");
                    profile.add_C_Compiler_Symbol("NDEBUG");
                    profile.add_C_Compiler_Symbol("TFM_TIMING_RESISTANT");
                    profile.add_C_Compiler_Symbol("ECC_TIMING_RESISTANT");
                    profile.add_C_Compiler_Symbol("WC_RSA_BLINDING");
                    profile.add_C_Compiler_Symbol("NO_DSA");
                    profile.add_C_Compiler_Symbol("NO_ERROR_STRINGS");
                    profile.add_C_Compiler_Symbol("NO_OLD_TLS");
                    profile.add_C_Compiler_Symbol("NO_RC4");
                    profile.add_C_Compiler_Symbol("NO_MD5");
                    profile.add_C_Compiler_Symbol("NO_HC128");
                    profile.add_C_Compiler_Symbol("NO_RABBIT");
                    profile.add_C_Compiler_Symbol("WC_NO_HASHDRBG");
                    profile.add_C_Compiler_Symbol("NO_FILESYSTEM");
                    profile.add_C_Compiler_Symbol("NO_PSK");
                    profile.add_C_Compiler_Symbol("NO_MD4");
                    profile.add_C_Compiler_Symbol("NO_PWDBASED");
                    profile.add_C_Compiler_Symbol("USE_FAST_MATH");
                    profile.add_C_Compiler_Symbol("NO_DES3");
                    profile.add_C_Compiler_Symbol("WOLFSSL_STATIC_RSA");
                    profile.add_C_Compiler_Symbol("RSA_LOW_MEM");
                    profile.add_C_Compiler_Symbol("SMALL_SESSION_CACHE");
                    profile.add_C_Compiler_Symbol("WOLFSSL_DH_CONST");
                    profile.add_C_Compiler_Symbol("HAVE_DH");
                    profile.add_C_Compiler_Symbol("NO_INLINE");

                    scriptApi.logInfo("TCPIP_LOG: ADDING SOCKET SECURE");
                    profile.addComponents("platform.middleware.tcpip_apps_socket_secure", true, true);
                }
            }
            else
            {
                if (boolHttpdRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW HTPPD");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_httpd", true, true);
                }
                if (boolTcpechoRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW TCPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_tcpecho", true, true);
                }
                if (boolUdpechoRaw.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW UDPECHO");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_udpecho", true, true);
                }
                if (boolLwiperf.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW LWIPERF");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_lwiperf", true, true);
                }
                if (boolNetbios.getValue() == true)
                {
                    scriptApi.logInfo("TCPIP_LOG: ADDING RAW NETBIOS");
                    profile.addComponents("platform.middleware.tcpip_apps_raw_netbiosns", true, true);
                }
            }

            scriptApi.logInfo("TCPIP_LOG: Adding generic applications--");
            profile.addComponents("platform.middleware.tcpip_apps_generic", true, true);

            scriptApi.logInfo("applyToolchainSettings");
        } catch(err) {
            scriptApi.logError(err);
        }
    }
}
