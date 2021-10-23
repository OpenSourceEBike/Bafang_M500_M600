/*
 * Copyright 2018 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function addHeaderCommentsTCPIPConfig(){
    var outputString = "";
        outputString += "/* ###################################################################\n";
        outputString += "**     Copyright 2019-2020 NXP \n";
        outputString += "**     Copyright (c) 2001-2003 Swedish Institute of Computer Science.\n";
        outputString += "**     All rights reserved.\n";
        outputString += "**     \n";
        outputString += "**     Redistribution and use in source and binary forms, with or without modification,\n";
        outputString += "**     are permitted provided that the following conditions are met:\n";
        outputString += "*\n";
        outputString += "**     1. Redistributions of source code must retain the above copyright notice,\n";
        outputString += "**        this list of conditions and the following disclaimer.\n";
        outputString += "**     2. Redistributions in binary form must reproduce the above copyright notice,\n";
        outputString += "**        this list of conditions and the following disclaimer in the documentation\n";
        outputString += "**        and/or other materials provided with the distribution.\n";
        outputString += "**     3. The name of the author may not be used to endorse or promote products\n";
        outputString += "**        derived from this software without specific prior written permission.\n";
        outputString += "*\n";
        outputString += "**     This software is owned or controlled by NXP and may only be\n";
        outputString += "**     used strictly in accordance with the applicable license terms. By expressly\n";
        outputString += "**     accepting such terms or by downloading, installing, activating and/or otherwise\n";
        outputString += "**     using the software, you are agreeing that you have read, and that you agree to\n";
        outputString += "**     comply with and are bound by, such license terms. If you do not agree to be\n";
        outputString += "**     bound by the applicable license terms, then you may not retain, install,\n";
        outputString += "**     activate or otherwise use the software. The production use license in\n";
        outputString += "**     Section 2.3 is expressly granted for this software.\n";
        outputString += "*\n";
        outputString += "**     This file is part of the lwIP TCP/IP stack.\n";
        outputString += "*\n";
        outputString += "**     Author: Adam Dunkels <adam@sics.se>\n";
        outputString += "*\n";
        outputString += "** ###################################################################*/\n";
        outputString += "\n";
    return outputString;
}

function lwippoolString(){
    var outputString = "";

    outputString += addHeaderCommentsTCPIPConfig();

    outputString += "#ifndef LWIP_POOLS_H\n";
    outputString += "#define LWIP_POOLS_H\n";
    outputString += "\n";
    outputString += "/**\n";
    outputString += "* @page misra_violations MISRA-C:2012 violations\n";
    outputString += "*\n";
    outputString += "* @section [global]\n"
    outputString += "* Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n"
    outputString += "* The global macro will be used in function call of the module.\n"
    outputString += "*/\n";
    outputString += "\n";
    outputString += "/* OPTIONAL: Pools to replace heap allocation.\n";
    outputString += " * Pools can be used instead of the heap for mem_malloc. If so,\n";
    outputString += " * these should be defined here, in increasing order according to\n";
    outputString += " * the pool element size.\n";
    outputString += " *\n";
    outputString += " * LWIP_MALLOC_MEMPOOL(number_elements, element_size)\n";
    outputString += " *\n";
    outputString += " * Note: Your custom pools can go here if you would like to use\n";
    outputString += " * lwIP's memory pools for anything else.\n";
    outputString += " */\n";

    outputString += "\n";
    outputString += "#if MEM_USE_POOLS\n";
    outputString += "LWIP_MALLOC_MEMPOOL_START\n";
    outputString += "LWIP_MALLOC_MEMPOOL(100, 256)\n";
    outputString += "LWIP_MALLOC_MEMPOOL(50, 512)\n";
    outputString += "LWIP_MALLOC_MEMPOOL(20, 1024)\n";
    outputString += "LWIP_MALLOC_MEMPOOL(20, 1536)\n";
    outputString += "LWIP_MALLOC_MEMPOOL_END\n";
    outputString += "#endif /* MEM_USE_POOLS */\n";
    outputString += "\n";

    outputString += "#endif /* LWIP_POOLS_H */\n";

    outputString += "\n";
    return outputString
}

// TCPIP component code generator class.
function tcpip_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    this.applyToolchainSettings = function(profile) {
       try{
            scriptApi.logInfo("applyToolchainSettings");
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }

    // Returns the class name.
    this.toString = function () {
        return "tcpip_CodeGenerator";
    }

    // Generate the configuration file
    this.genHCfgFile = function(hCfgFileSections) {
        var outputString = "";
        var outputString_lwipcfg = "";
        var outputString_lwipopts = "";
        var outputString_netifcfgHeader = "";
        var outputString_netifcfgSource = "";

        var childContext = uiConfig.getChildContext();
        var peripheralName = childContext.getComponentInstanceConfig().getPeripheral();
        var coreId = childContext.getFunctionalGroup().getCore();
        var profile = scriptApi.getProfile();
        var functionalGroups = profile.getFunctionalGroups();

        var genericStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".generic_tab");
        var osChoice = genericStruct.getChildById("osEnable");
        var lowlevelChoice = genericStruct.getChildById("lowlevelIface");
        var boolEnableHostname = genericStruct.getChildById("boolEnableHostname").getValue();

        var numberOfTcpipComponents = functionalGroups[0].getComponentInstances("tcpip_config").length;
        if (numberOfTcpipComponents > 1)
        {
            scriptApi.logError("Please enable and use only one TCPIP_STACK component!")
        }

        var netifArray = getSetting(this.uiConfig, this.uiConfig.getName()+".netifcfg_tab");
        var netifUsed = 0;
        var allDhcp = false;
        var allAutoip = false;
        var allIpv6 = false;
        if (null != netifArray)
        {
            var netifArrayList = netifArray.getChildren();
            var netifArraySize = netifArrayList.size();
            for (var i = 0; i < netifArraySize; i++)
            {
                if (netifArrayList[i].getChildById("boolIface").getValue() == true)
                {
                    netifUsed++;
                    if (netifArrayList[i].getChildById("boolDhcp").getValue() == true)
                    {
                        allDhcp = true;
                    }
                    if (netifArrayList[i].getChildById("boolAutoip").getValue() == true)
                    {
                        allAutoip = true;
                    }
                    if (netifArrayList[i].getChildById("boolIpv6").getValue() == true)
                    {
                        allIpv6 = true;
                    }
                }
            }
        }

        /* DataLinkStruct */
        var datalinkStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".datalink_tab");
        var miiMode = datalinkStruct.getChildById("miiMode").getValue();
        var miiSpeed = datalinkStruct.getChildById("miiSpeed").getValue();
        var boolARP = datalinkStruct.getChildById("boolARP").getValue();
        var arpTableSize = datalinkStruct.getChildById("arpTableSize").getValue();
        var boolARPQueue = datalinkStruct.getChildById("boolARPQueue").getValue();

        /* NetworkStruct */
        var networkStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".network_tab");
        var boolIpv4 = networkStruct.getChildById("ipv4_id").getChildById("boolIpv4").getValue();
        var dhcp2autoipTries = networkStruct.getChildById("ipv4_id").getChildById("ipv4_auto_id").getChildById("dhcp2autoipTries").getValue();
        var ipforward = networkStruct.getChildById("ipv4_id").getChildById("ipv4_extensions_id").getChildById("ipforward").getValue();
        var ipreassembly = networkStruct.getChildById("ipv4_id").getChildById("ipv4_extensions_id").getChildById("ipreassembly").getValue();
        var ipfrag = networkStruct.getChildById("ipv4_id").getChildById("ipv4_extensions_id").getChildById("ipfrag").getValue();
        var boolIcmp = networkStruct.getChildById("ipv4_id").getChildById("ipv4_icmp").getChildById("icmp_enable").getValue();
        var icmpttl = networkStruct.getChildById("ipv4_id").getChildById("ipv4_icmp").getChildById("icmp_ttl").getValue();
        var boolIgmp = networkStruct.getChildById("ipv4_id").getChildById("ipv4_igmp").getChildById("igmp_enable").getValue();

        var transportStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".transport_tab");
        var boolTcp = transportStruct.getChildById("tcp_id").getChildById("boolTcp").getValue();
        var boolQueue = transportStruct.getChildById("tcp_id").getChildById("boolQueue").getValue();
        var tcpttl =  transportStruct.getChildById("tcp_id").getChildById("ttl").getValue();
        var tcpmss =  transportStruct.getChildById("tcp_id").getChildById("mss").getValue();
        var tcpsndbuf =  transportStruct.getChildById("tcp_id").getChildById("snd_buf").getValue();
        var tcpwnd =  transportStruct.getChildById("tcp_id").getChildById("wnd").getValue();
        var tcpmaxrtx =  transportStruct.getChildById("tcp_id").getChildById("maxrtx").getValue();
        var tcpsynmaxrtx =  transportStruct.getChildById("tcp_id").getChildById("synmaxrtx").getValue();
        var boolUdp = transportStruct.getChildById("udp_id").getChildById("boolUdp").getValue();
        var udpttl =  transportStruct.getChildById("udp_id").getChildById("ttl").getValue();

        var appStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".application_tab");
        var enableRaw = appStruct.getChildById("raw").getChildById("rawSettings").getChildById("enableRaw").getValue();
        var boolHttpdRaw = appStruct.getChildById("raw").getChildById("boolHttpdRaw").getValue();
        var boolTcpechoRaw = appStruct.getChildById("raw").getChildById("boolTcpechoRaw").getValue();
        var boolUdpechoRaw = appStruct.getChildById("raw").getChildById("boolUdpechoRaw").getValue();
        var boolLwiperf = appStruct.getChildById("raw").getChildById("boolLwiperf").getValue();
        var boolNetbios = appStruct.getChildById("raw").getChildById("boolNetbios").getValue();
        var boolSntp = appStruct.getChildById("raw").getChildById("boolSntp").getValue();
        var boolNetio = appStruct.getChildById("raw").getChildById("boolNetio").getValue();

        if (enableRaw == false)
        {
            boolHttpdRaw = false
            boolTcpechoRaw = false
            boolUdpechoRaw = false
            boolLwiperf = false
            boolNetbios = false
            boolSntp = false
            boolNetio = false
        }

        var netconn_semPerThread = appStruct.getChildById("netconn").getChildById("netconnSettings").getChildById("netconn_semPerThread").getValue();
        var boolHttpdNetconn = appStruct.getChildById("netconn").getChildById("boolHttpdNetconn").getValue();
        var boolTcpechoNetconn = appStruct.getChildById("netconn").getChildById("boolTcpechoNetconn").getValue();
        var boolUdpechoNetconn = appStruct.getChildById("netconn").getChildById("boolUdpechoNetconn").getValue();
        var boolDns = appStruct.getChildById("netconn").getChildById("boolDns").getValue();
        var boolShell = appStruct.getChildById("netconn").getChildById("boolShell").getValue();
        var boolChargenSocket = appStruct.getChildById("socket").getChildById("boolChargenSocket").getValue();
        var boolSocketExamples = appStruct.getChildById("socket").getChildById("boolSocketExamples").getValue();
        var boolRtp = appStruct.getChildById("socket").getChildById("boolRtp").getValue();
        var boolSslechoSocket = appStruct.getChildById("socket").getChildById("boolSslechoSocket").getValue();

        var checksumStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".checksum_tab");
        var boolCksumPerNetif = checksumStruct.getChildById("checksum_id.boolCksumPerNetif").getValue();
        var boolCksumIPGen = checksumStruct.getChildById("checksum_id").getChildById("boolCksumIPGen").getValue();
        var boolCksumUDPGen = checksumStruct.getChildById("checksum_id").getChildById("boolCksumUDPGen").getValue();
        var boolCksumTCPGen = checksumStruct.getChildById("checksum_id").getChildById("boolCksumTCPGen").getValue();
        var boolCksumICMPGen = checksumStruct.getChildById("checksum_id").getChildById("boolCksumICMPGen").getValue();
        var boolCksumIPValid = checksumStruct.getChildById("checksum_id").getChildById("boolCksumIPValid").getValue();
        var boolCksumUDPValid = checksumStruct.getChildById("checksum_id").getChildById("boolCksumUDPValid").getValue();
        var boolCksumTCPValid = checksumStruct.getChildById("checksum_id").getChildById("boolCksumTCPValid").getValue();
        var boolCksumICMPValid = checksumStruct.getChildById("checksum_id").getChildById("boolCksumICMPValid").getValue();
        var boolCksumOnCopy = checksumStruct.getChildById("checksum_id").getChildById("boolCksumOnCopy").getValue();

        /* Disable all checksum/validate feature when disable software checksum */
        if (false == checksumStruct.getChildById("boolEnableCksum").getValue())
        {
           var childChkSum = checksumStruct.getChildById("checksum_id").getChildren();

           for each (child in childChkSum) {
              child.setValue(false);
           }
        }

        var cpu = profile.getMcuInfo().getPartNumber();
        var userValue = getSetting(this.uiConfig, this.uiConfig.getName()+".user_tab").getChildById("configUSER_SETTINGS").getValue();
        var boolTimevalPrivate = getSetting(this.uiConfig, this.uiConfig.getName()+".user_tab").getChildById("boolTimevalPrivate").getValue();

        /* get value from UI for lwipcfg */
        outputString_lwipcfg += addHeaderCommentsTCPIPConfig();
        outputString_lwipcfg += "#ifndef LWIP_CFG_H\n";
        outputString_lwipcfg += "#define LWIP_CFG_H\n";
        outputString_lwipcfg += "\n";
        outputString_lwipcfg += "/**\n";
        outputString_lwipcfg += "* @page misra_violations MISRA-C:2012 violations\n";
        outputString_lwipcfg += "*\n";
        outputString_lwipcfg += "* @section [global]\n"
        outputString_lwipcfg += "* Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n"
        outputString_lwipcfg += "* The global macro will be used in function call of the module.\n"
        outputString_lwipcfg += "*/\n";
        outputString_lwipcfg += "\n";

        outputString_lwipcfg += "#if defined(__NEWLIB__)\n";
        outputString_lwipcfg += "#include <sys/time.h>\n";
        outputString_lwipcfg += "#elif defined(__EWL__)\n";
        outputString_lwipcfg += "#pragma GCC diagnostic push\n";
        outputString_lwipcfg += "#pragma GCC diagnostic ignored \"-Wunknown-pragmas\"\n";
        outputString_lwipcfg += "#include <time.h>\n";
        outputString_lwipcfg += "#pragma GCC diagnostic pop\n";
        outputString_lwipcfg += "#endif /* defined(__NEWLIB__) */\n";
        outputString_lwipcfg += "\n";

        /* RTOS */
        if (osChoice.getValue() == true)
        {
            var indexChoice = 0;
            var freertosComponent = functionalGroups[0].getComponentInstances("freertos");
            /* Get user choices */
            var rtosUserChoice = genericStruct.getChildById("rtosChoice").getValue();
            for (var index = 0; index < freertosComponent.length; index++)
            {
                if (rtosUserChoice == freertosComponent[index].getName())
                {
                    indexChoice = index;
                    break;
                }
            }
            if (freertosComponent[indexChoice].getChildById("freertos_driver.general_tab.configNUM_THREAD_LOCAL_STORAGE_POINTERS").getValue() == 0)
            {
                scriptApi.logWarning("[tcpip-freertos]`num thread local storage pointers` value is suggested to be more than `0`.")
            }

            if (enableRaw == true)
            {
                if ((boolHttpdRaw == true) && (boolHttpdNetconn == true))
                {
                    scriptApi.logError("TCPIP_ERROR: Cannot enable both HTTPD using RAW and NETCONN API at the same time");
                }
                if ((boolTcpechoRaw == true) && (boolTcpechoNetconn == true))
                {
                    scriptApi.logError("TCPIP_ERROR: Cannot enable both TCPECHO using RAW and NETCONN API at the same time");
                }
                if ((boolUdpechoRaw == true) && (boolUdpechoNetconn == true))
                {
                    scriptApi.logError("TCPIP_ERROR: Cannot enable both UDPECHO using RAW and NETCONN API at the same time");
                }
            }

            outputString_lwipcfg += "#define LWIP_HTTPD_APP              "+Number((boolHttpdRaw == true) || (boolHttpdNetconn == true))+"\n";
            outputString_lwipcfg += "#define LWIP_HTTPD_APP_NETCONN      "+Number(boolHttpdNetconn)+"\n";
            outputString_lwipcfg += "#define LWIP_TCPECHO_APP            "+Number((boolTcpechoRaw == true) || (boolTcpechoNetconn == true))+"\n";
            outputString_lwipcfg += "#define LWIP_TCPECHO_APP_NETCONN    "+Number(boolTcpechoNetconn)+"\n";
            outputString_lwipcfg += "#define LWIP_UDPECHO_APP            "+Number((boolUdpechoRaw == true) || (boolUdpechoNetconn == true))+"\n";
            outputString_lwipcfg += "#define LWIP_UDPECHO_APP_NETCONN    "+Number(boolUdpechoNetconn)+"\n";

            outputString_lwipcfg += "#define LWIP_DNS_APP                "+Number(boolDns)+"\n";
            outputString_lwipcfg += "#define LWIP_SHELL_APP              "+Number(boolShell)+"\n";

            outputString_lwipcfg += "\n";
            outputString_lwipcfg += "#define LWIP_LWIPERF_APP            "+Number(boolLwiperf)+"\n";
            outputString_lwipcfg += "#define LWIP_NETBIOS_APP            "+Number(boolNetbios)+"\n";
            outputString_lwipcfg += "#define LWIP_CHARGEN_APP            "+Number(boolChargenSocket)+"\n";
            outputString_lwipcfg += "#define LWIP_SOCKET_EXAMPLES_APP    "+Number(boolSocketExamples)+"\n";
            outputString_lwipcfg += "#define LWIP_RTP_APP                "+Number(boolRtp)+"\n";
            outputString_lwipcfg += "#define LWIP_SSL_ECHO_APP           "+Number(boolSslechoSocket)+"\n";

            outputString_lwipcfg += "#define LWIP_SNTP_APP               "+Number(boolSntp)+"\n";
            outputString_lwipcfg += "#define LWIP_NETIO_APP              "+Number(boolNetio)+"\n";
        }
        else
        {
            genericStruct.getChildById("baremetalChoice").setValue("USING_OS_BAREMETAL")

            outputString_lwipcfg += "#define LWIP_HTTPD_APP              "+Number(boolHttpdRaw)+"\n";
            outputString_lwipcfg += "#define LWIP_HTTPD_APP_NETCONN      0"+"\n";
            outputString_lwipcfg += "#define LWIP_TCPECHO_APP            "+Number(boolTcpechoRaw)+"\n";
            outputString_lwipcfg += "#define LWIP_TCPECHO_APP_NETCONN    0"+"\n";
            outputString_lwipcfg += "#define LWIP_UDPECHO_APP            "+Number(boolUdpechoRaw)+"\n";
            outputString_lwipcfg += "#define LWIP_UDPECHO_APP_NETCONN    0"+"\n";
            outputString_lwipcfg += "#define LWIP_LWIPERF_APP            "+Number(boolLwiperf)+"\n";
            outputString_lwipcfg += "#define LWIP_NETBIOS_APP            "+Number(boolNetbios)+"\n";
            outputString_lwipcfg += "#define LWIP_SNTP_APP               "+Number(boolSntp)+"\n";
            outputString_lwipcfg += "#define LWIP_NETIO_APP              "+Number(boolNetio)+"\n";
        }

        outputString_lwipcfg += "\n";
        outputString_lwipcfg += "#define NETIF_CHECKSUM_SETTING        NETIF_CHECKSUM_DISABLE_ALL"+"\n";

        outputString_lwipcfg += "\n";
        outputString_lwipcfg += "#ifdef BYTE_ORDER\n";
        outputString_lwipcfg += "#undef BYTE_ORDER\n";
        outputString_lwipcfg += "#define BYTE_ORDER                     LITTLE_ENDIAN\n";
        outputString_lwipcfg += "#else\n";
        outputString_lwipcfg += "#define BYTE_ORDER                     LITTLE_ENDIAN\n";
        outputString_lwipcfg += "#endif /* BYTE_ORDER */\n";

        outputString_lwipcfg += "\n";
        outputString_lwipcfg += "/****************************************************************************\n";
        outputString_lwipcfg += " * ADDITIONAL USERS SETTINGS\n";
        outputString_lwipcfg += " ****************************************************************************/\n";
        outputString_lwipcfg += userValue;
        outputString_lwipcfg += "\n";

        outputString_lwipcfg += "\n";
        outputString_lwipcfg += "#endif /* LWIP_CFG_H */\n";

        genCustomHeaderFile("lwipcfg", outputString_lwipcfg, coreId)

        /* get value from UI for lwipopts */
        outputString_lwipopts += addHeaderCommentsTCPIPConfig();
        outputString_lwipopts += "#ifndef LWIP_OPTS_H\n";
        outputString_lwipopts += "#define LWIP_OPTS_H\n";

        outputString_lwipopts += "/**\n";
        outputString_lwipopts += "* @page misra_violations MISRA-C:2012 violations\n";
        outputString_lwipopts += "*\n";
        outputString_lwipopts += "* @section [global]\n"
        outputString_lwipopts += "* Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n"
        outputString_lwipopts += "* The global macro will be used in function call of the module.\n"
        outputString_lwipopts += "*/\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- Application APIs ---------- */\n";

        /* RTOS */
        if (osChoice.getValue() == true)
        {
            outputString_lwipopts += "#define NO_SYS                     0\n";
        }
        else
        {
            outputString_lwipopts += "#define NO_SYS                     1\n";
        }
        outputString_lwipopts += "#define LWIP_RAW                   " + Number(enableRaw) + "\n";
        outputString_lwipopts += "#define LWIP_SOCKET               (NO_SYS==0)\n";
        outputString_lwipopts += "#define LWIP_NETCONN              (NO_SYS==0)\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- ARP options ---------- */\n";
        outputString_lwipopts += "#define LWIP_ARP                "+Number(boolARP)+"\n";
        outputString_lwipopts += "#define ARP_TABLE_SIZE          "+arpTableSize+"\n";
        outputString_lwipopts += "#define ARP_QUEUEING            "+Number(boolARPQueue)+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- DHCP options ---------- */\n";
        outputString_lwipopts += "/* Define LWIP_DHCP to 1 if you want DHCP configuration of interfaces */\n";
        outputString_lwipopts += "#define LWIP_DHCP               "+Number(allDhcp)+"\n";
        outputString_lwipopts += "/* 1 if you want to do an ARP check on the offered address (recommended) */\n";
        outputString_lwipopts += "#define DHCP_DOES_ARP_CHECK    (LWIP_DHCP)\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- AUTOIP options ------- */\n";
        outputString_lwipopts += "#define LWIP_AUTOIP            "+Number(allAutoip)+"\n";
        outputString_lwipopts += "#define LWIP_DHCP_AUTOIP_COOP  (LWIP_DHCP && LWIP_AUTOIP)\n";
        outputString_lwipopts += "#define LWIP_DHCP_AUTOIP_COOP_TRIES     "+dhcp2autoipTries+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- IP options ---------- */\n";
        outputString_lwipopts += "#define LWIP_IPV4                  "+Number(boolIpv4)+"\n";
        outputString_lwipopts += "#define LWIP_IPV6                  "+Number(allIpv6)+"\n";
        if (boolIpv4 == true) {
            outputString_lwipopts += "#define LWIP_ICMP                  "+Number(boolIcmp)+"\n";
            outputString_lwipopts += "#define LWIP_IGMP                  "+Number(boolIgmp)+"\n";
        } else {
            outputString_lwipopts += "/* IPv4 is needed for ICMP and IGMP operations */\n";
            outputString_lwipopts += "#define LWIP_ICMP                  0\n";
            outputString_lwipopts += "#define LWIP_IGMP                  0\n";
        }

        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* Define IP_FORWARD to 1 if you wish to have the ability to forward\n";
        outputString_lwipopts += "   IP packets across network interfaces. If you are going to run lwIP\n";
        outputString_lwipopts += "   on a device with only one network interface, define this to 0. */\n";
        outputString_lwipopts += "#define IP_FORWARD              "+Number(ipforward)+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* IP reassembly and segmentation.These are orthogonal even\n";
        outputString_lwipopts += " * if they both deal with IP fragments */\n";
        outputString_lwipopts += "#define IP_REASSEMBLY           "+Number(ipreassembly)+"\n";
        outputString_lwipopts += "#define IP_REASS_MAX_PBUFS      10\n";
        outputString_lwipopts += "#define MEMP_NUM_REASSDATA      10\n";
        outputString_lwipopts += "#define IP_FRAG                 "+Number(ipfrag)+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- ICMP options ---------- */\n";
        outputString_lwipopts += "#define ICMP_TTL                "+icmpttl+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- TCP options ---------- */\n";
        outputString_lwipopts += "#define LWIP_TCP                "+Number(boolTcp)+"\n";
        outputString_lwipopts += "#define TCP_TTL                 "+tcpttl+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* Controls if TCP should queue segments that arrive out of\n";
        outputString_lwipopts += "   order. Define to 0 if your device is low on memory. */\n";
        outputString_lwipopts += "#define TCP_QUEUE_OOSEQ         "+Number(boolQueue)+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* TCP Maximum segment size. */\n";
        outputString_lwipopts += "#define TCP_MSS                 "+tcpmss+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* TCP sender buffer space (bytes). */\n";
        outputString_lwipopts += "#define TCP_SND_BUF             "+tcpsndbuf+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* TCP sender buffer space (pbufs). This must be at least = 2 *\n";
        outputString_lwipopts += "   TCP_SND_BUF/TCP_MSS for things to work. */\n";
        outputString_lwipopts += "#define TCP_SND_QUEUELEN       (4 * TCP_SND_BUF/TCP_MSS)\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* TCP writable space (bytes). This must be less than or equal\n";
        outputString_lwipopts += "   to TCP_SND_BUF. It is the amount of space which must be\n";
        outputString_lwipopts += "   available in the tcp snd_buf for select to return writable */\n";
        outputString_lwipopts += "#define TCP_SNDLOWAT           (TCP_SND_BUF/2)\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* TCP receive window. */\n";
        outputString_lwipopts += "#define TCP_WND                 "+tcpwnd+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* Maximum number of retransmissions of data segments. */\n";
        outputString_lwipopts += "#define TCP_MAXRTX              "+tcpmaxrtx+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* Maximum number of retransmissions of SYN segments. */\n";
        outputString_lwipopts += "#define TCP_SYNMAXRTX           "+tcpsynmaxrtx+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- UDP options ---------- */\n";
        outputString_lwipopts += "#define LWIP_UDP                "+Number(boolUdp)+"\n";
        outputString_lwipopts += "#define LWIP_UDPLITE            LWIP_UDP\n";
        outputString_lwipopts += "#define UDP_TTL                 "+udpttl+"\n";
        outputString_lwipopts += "\n";

        var memoryStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".memory_tab");
        var memAlign = memoryStruct.getChildById("memAlign").getValue();
        var memHeapSize = memoryStruct.getChildById("memHeapSize").getValue();
        var noPacketBuf = memoryStruct.getChildById("noPacketBuf").getValue();
        var noRawPcb = memoryStruct.getChildById("noRawPcb").getValue();
        var noUdpPcb = memoryStruct.getChildById("noUdpPcb").getValue();
        var noTcpPcb = memoryStruct.getChildById("noTcpPcb").getValue();
        var noListenTcpPcb = memoryStruct.getChildById("noListenTcpPcb").getValue();
        var noSegmentTcpPcb = memoryStruct.getChildById("noSegmentTcpPcb").getValue();
        var noActiveTimeouts = memoryStruct.getChildById("noActiveTimeouts").getValue();
        var noStructNetbufs = memoryStruct.getChildById("noStructNetbufs").getValue();
        var noStructNetconns = memoryStruct.getChildById("noStructNetconns").getValue();
        var noStructTcpipMsgApi = memoryStruct.getChildById("noStructTcpipMsgApi").getValue();
        var noStructTcpipMsgInpkt = memoryStruct.getChildById("noStructTcpipMsgInpkt").getValue();
        var noBufferInPool = memoryStruct.getChildById("noBufferInPool").getValue();

        outputString_lwipopts += "/* ---------- Memory options ---------- */\n";
        outputString_lwipopts += "/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which\n";
        outputString_lwipopts += "   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2\n";
        outputString_lwipopts += "   byte alignment -> define MEM_ALIGNMENT to 2. */\n";
        outputString_lwipopts += "/* MSVC port: intel processors don't need 4-byte alignment,\n";
        outputString_lwipopts += "   but are faster that way! */\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define MEM_ALIGNMENT           "+memAlign+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEM_SIZE: the size of the heap memory. If the application will send\n";
        outputString_lwipopts += "a lot of data that needs to be copied, this should be set high. */\n";
        outputString_lwipopts += "#define MEM_SIZE               "+memHeapSize+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application\n";
        outputString_lwipopts += "   sends a lot of data out of ROM (or other static memory), this\n";
        outputString_lwipopts += "   should be set high. */\n";
        outputString_lwipopts += "#define MEMP_NUM_PBUF           "+noPacketBuf+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_RAW_PCB: the number of UDP protocol control blocks. One\n";
        outputString_lwipopts += "   per active RAW connection. */\n";
        outputString_lwipopts += "#define MEMP_NUM_RAW_PCB        "+noRawPcb+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One\n";
        outputString_lwipopts += "   per active UDP connection. */\n";
        outputString_lwipopts += "#define MEMP_NUM_UDP_PCB        "+noUdpPcb+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_TCP_PCB: the number of simultaneously active TCP connections. */\n";
        outputString_lwipopts += "#define MEMP_NUM_TCP_PCB        "+noTcpPcb+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP connections. */\n";
        outputString_lwipopts += "#define MEMP_NUM_TCP_PCB_LISTEN     "+noListenTcpPcb+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.*/\n";
        outputString_lwipopts += "#define MEMP_NUM_TCP_SEG        "+noSegmentTcpPcb+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active timeouts. */\n";
        outputString_lwipopts += "#define MEMP_NUM_SYS_TIMEOUT    "+noActiveTimeouts+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* The following four are used only with the sequential API and can be\n";
        outputString_lwipopts += "   set to 0 if the application only will use the raw API. */\n";
        outputString_lwipopts += "/* MEMP_NUM_NETBUF: the number of struct netbufs. */\n";
        outputString_lwipopts += "#define MEMP_NUM_NETBUF         "+noStructNetbufs+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_NETCONN: the number of struct netconns. */\n";
        outputString_lwipopts += "#define MEMP_NUM_NETCONN        "+noStructNetconns+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* MEMP_NUM_TCPIP_MSG_*: the number of struct tcpip_msg, which is used\n";
        outputString_lwipopts += "   for sequential API communication and incoming packets. Used in\n";
        outputString_lwipopts += "   src/api/tcpip.c. */\n";
        outputString_lwipopts += "#define MEMP_NUM_TCPIP_MSG_API   "+noStructTcpipMsgApi+"\n";
        outputString_lwipopts += "#define MEMP_NUM_TCPIP_MSG_INPKT "+noStructTcpipMsgInpkt+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- Pbuf options ---------- */\n";
        outputString_lwipopts += "/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */\n";
        outputString_lwipopts += "#define PBUF_POOL_SIZE          "+noBufferInPool+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/** SYS_LIGHTWEIGHT_PROT\n";
        outputString_lwipopts += "* define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection\n";
        outputString_lwipopts += "* for certain critical regions during buffer allocation, deallocation and memory\n";
        outputString_lwipopts += "* allocation and deallocation. */\n";
        outputString_lwipopts += "#define SYS_LIGHTWEIGHT_PROT    (NO_SYS==0)\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- Statistics options ---------- */\n";
        outputString_lwipopts += "#define LWIP_STATS              1\n";
        outputString_lwipopts += "#define LWIP_STATS_DISPLAY      0\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#if LWIP_STATS\n";
        outputString_lwipopts += "#define LINK_STATS              1\n";
        outputString_lwipopts += "#define IP_STATS                1\n";
        outputString_lwipopts += "#define ICMP_STATS              1\n";
        outputString_lwipopts += "#define IGMP_STATS              1\n";
        outputString_lwipopts += "#define IPFRAG_STATS            1\n";
        outputString_lwipopts += "#define UDP_STATS               1\n";
        outputString_lwipopts += "#define TCP_STATS               1\n";
        outputString_lwipopts += "#define MEM_STATS               1\n";
        outputString_lwipopts += "#define MEMP_STATS              1\n";
        outputString_lwipopts += "#define PBUF_STATS              1\n";
        outputString_lwipopts += "#define SYS_STATS               1\n";
        outputString_lwipopts += "#endif /* LWIP_STATS */\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- PPP options ---------- */\n";
        outputString_lwipopts += "#define PPP_SUPPORT             0\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- TIMEVAL options ---------- */\n";
        outputString_lwipopts += "#define LWIP_TIMEVAL_PRIVATE        " + Number(boolTimevalPrivate) + "\n";
        outputString_lwipopts += "\n";

        var debugStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".debug_tab");
        var enableDebug = debugStruct.getChildById("enableDebug").getValue();
        var minDebugLevel = debugStruct.getChildById("minDebugLevel").getValue();
        var etharpDebug = debugStruct.getChildById("protocolDebug").getChildById("etharpDebug").getValue();
        var netifDebug = debugStruct.getChildById("protocolDebug").getChildById("netifDebug").getValue();
        var pbufDebug = debugStruct.getChildById("protocolDebug").getChildById("pbufDebug").getValue();
        var apiLibDebug = debugStruct.getChildById("protocolDebug").getChildById("apiLibDebug").getValue();
        var apiMsgDebug = debugStruct.getChildById("protocolDebug").getChildById("apiMsgDebug").getValue();
        var socketsDebug = debugStruct.getChildById("protocolDebug").getChildById("socketsDebug").getValue();
        var icmpDebug = debugStruct.getChildById("protocolDebug").getChildById("icmpDebug").getValue();
        var igmpDebug = debugStruct.getChildById("protocolDebug").getChildById("igmpDebug").getValue();
        var inetDebug = debugStruct.getChildById("protocolDebug").getChildById("inetDebug").getValue();
        var ipDebug = debugStruct.getChildById("protocolDebug").getChildById("ipDebug").getValue();
        var ipReassDebug = debugStruct.getChildById("protocolDebug").getChildById("ipReassDebug").getValue();
        var rawDebug = debugStruct.getChildById("protocolDebug").getChildById("rawDebug").getValue();
        var memDebug = debugStruct.getChildById("protocolDebug").getChildById("memDebug").getValue();
        var mempDebug = debugStruct.getChildById("protocolDebug").getChildById("mempDebug").getValue();
        var sysDebug = debugStruct.getChildById("protocolDebug").getChildById("sysDebug").getValue();
        var timersDebug = debugStruct.getChildById("protocolDebug").getChildById("timersDebug").getValue();
        var tcpDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpDebug").getValue();
        var tcpInputDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpInputDebug").getValue();
        var tcpFrDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpFrDebug").getValue();
        var tcpRtoDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpRtoDebug").getValue();
        var tcpCwndDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpCwndDebug").getValue();
        var tcpWndDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpWndDebug").getValue();
        var tcpOutputDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpOutputDebug").getValue();
        var tcpRstDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpRstDebug").getValue();
        var tcpQlenDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpQlenDebug").getValue();
        var udpDebug = debugStruct.getChildById("protocolDebug").getChildById("udpDebug").getValue();
        var tcpipDebug = debugStruct.getChildById("protocolDebug").getChildById("tcpipDebug").getValue();
        var slipDebug = debugStruct.getChildById("protocolDebug").getChildById("slipDebug").getValue();
        var dhcpDebug = debugStruct.getChildById("protocolDebug").getChildById("dhcpDebug").getValue();
        var autoipDebug = debugStruct.getChildById("protocolDebug").getChildById("autoipDebug").getValue();
        var dnsDebug = debugStruct.getChildById("protocolDebug").getChildById("dnsDebug").getValue();
        var ip6Debug = debugStruct.getChildById("protocolDebug").getChildById("ip6Debug").getValue();
        var pppDebug = debugStruct.getChildById("protocolDebug").getChildById("pppDebug").getValue();
        var mdnsDebug = debugStruct.getChildById("protocolDebug").getChildById("mdnsDebug").getValue();
        var mqttDebug = debugStruct.getChildById("protocolDebug").getChildById("mqttDebug").getValue();
        var snmpDebug = debugStruct.getChildById("protocolDebug").getChildById("snmpDebug").getValue();
        var sntpDebug = debugStruct.getChildById("protocolDebug").getChildById("sntpDebug").getValue();
        var tftpDebug = debugStruct.getChildById("protocolDebug").getChildById("tftpDebug").getValue();
        var pingDebug = debugStruct.getChildById("protocolDebug").getChildById("pingDebug").getValue();
        var httpdDebug = debugStruct.getChildById("protocolDebug").getChildById("httpdDebug").getValue();
        var rtpDebug = debugStruct.getChildById("protocolDebug").getChildById("rtpDebug").getValue();
        var smtpDebug = debugStruct.getChildById("protocolDebug").getChildById("smtpDebug").getValue();

        var lwipDebugTypesOn = ""
        var debugTrace = debugStruct.getChildById("messageTypeDebug").getChildById("debugTrace").getValue();
        var debugState = debugStruct.getChildById("messageTypeDebug").getChildById("debugState").getValue();
        var debugFresh = debugStruct.getChildById("messageTypeDebug").getChildById("debugFresh").getValue();
        var debugHalt = debugStruct.getChildById("messageTypeDebug").getChildById("debugHalt").getValue();

        if (enableDebug == true) {
            outputString_lwipopts += "/* ---------- Debugging options ---------- */\n";
            outputString_lwipopts += "#define LWIP_DEBUG\n";
            outputString_lwipopts += "\n";

            lwipDebugTypesOn += "LWIP_DBG_ON"
            if (debugTrace) {
                lwipDebugTypesOn += "|LWIP_DBG_TRACE"
            }
            if (debugState) {
                lwipDebugTypesOn += "|LWIP_DBG_STATE"
            }
            if (debugFresh) {
                lwipDebugTypesOn += "|LWIP_DBG_FRESH"
            }
            if (debugHalt) {
                lwipDebugTypesOn += "|LWIP_DBG_HALT"
            }

            outputString_lwipopts += "#define LWIP_DBG_TYPES_ON         ("+lwipDebugTypesOn+")\n";
            outputString_lwipopts += "\n";
            outputString_lwipopts += "#ifdef LWIP_DEBUG\n";
            outputString_lwipopts += "#define LWIP_DBG_MIN_LEVEL         "+minDebugLevel+"\n";
            outputString_lwipopts += "#define ETHARP_DEBUG               "+convertLwipDebugValue(etharpDebug)+"\n";
            outputString_lwipopts += "#define NETIF_DEBUG                "+convertLwipDebugValue(netifDebug)+"\n";
            outputString_lwipopts += "#define PBUF_DEBUG                 "+convertLwipDebugValue(pbufDebug)+"\n";
            outputString_lwipopts += "#define API_LIB_DEBUG              "+convertLwipDebugValue(apiLibDebug)+"\n";
            outputString_lwipopts += "#define API_MSG_DEBUG              "+convertLwipDebugValue(apiMsgDebug)+"\n";
            outputString_lwipopts += "#define SOCKETS_DEBUG              "+convertLwipDebugValue(socketsDebug)+"\n";
            outputString_lwipopts += "#define ICMP_DEBUG                 "+convertLwipDebugValue(icmpDebug)+"\n";
            outputString_lwipopts += "#define IGMP_DEBUG                 "+convertLwipDebugValue(igmpDebug)+"\n";
            outputString_lwipopts += "#define INET_DEBUG                 "+convertLwipDebugValue(inetDebug)+"\n";
            outputString_lwipopts += "#define IP_DEBUG                   "+convertLwipDebugValue(ipDebug)+"\n";
            outputString_lwipopts += "#define IP_REASS_DEBUG             "+convertLwipDebugValue(ipReassDebug)+"\n";
            outputString_lwipopts += "#define RAW_DEBUG                  "+convertLwipDebugValue(rawDebug)+"\n";
            outputString_lwipopts += "#define MEM_DEBUG                  "+convertLwipDebugValue(memDebug)+"\n";
            outputString_lwipopts += "#define MEMP_DEBUG                 "+convertLwipDebugValue(mempDebug)+"\n";
            outputString_lwipopts += "#define SYS_DEBUG                  "+convertLwipDebugValue(sysDebug)+"\n";
            outputString_lwipopts += "#define TIMERS_DEB                 "+convertLwipDebugValue(timersDebug)+"\n";
            outputString_lwipopts += "#define TCP_DEBUG                  "+convertLwipDebugValue(tcpDebug)+"\n";
            outputString_lwipopts += "#define TCP_INPUT_DEBUG            "+convertLwipDebugValue(tcpInputDebug)+"\n";
            outputString_lwipopts += "#define TCP_FR_DEBUG               "+convertLwipDebugValue(tcpFrDebug)+"\n";
            outputString_lwipopts += "#define TCP_RTO_DEBUG              "+convertLwipDebugValue(tcpRtoDebug)+"\n";
            outputString_lwipopts += "#define TCP_CWND_DEBUG             "+convertLwipDebugValue(tcpCwndDebug)+"\n";
            outputString_lwipopts += "#define TCP_WND_DEBUG              "+convertLwipDebugValue(tcpWndDebug)+"\n";
            outputString_lwipopts += "#define TCP_OUTPUT_DEBUG           "+convertLwipDebugValue(tcpOutputDebug)+"\n";
            outputString_lwipopts += "#define TCP_RST_DEBUG              "+convertLwipDebugValue(tcpRstDebug)+"\n";
            outputString_lwipopts += "#define TCP_QLEN_DEBUG             "+convertLwipDebugValue(tcpQlenDebug)+"\n";
            outputString_lwipopts += "#define UDP_DEBUG                  "+convertLwipDebugValue(udpDebug)+"\n";
            outputString_lwipopts += "#define TCPIP_DEBUG                "+convertLwipDebugValue(tcpipDebug)+"\n";
            outputString_lwipopts += "#define SLIP_DEBUG                 "+convertLwipDebugValue(slipDebug)+"\n";
            outputString_lwipopts += "#define DHCP_DEBUG                 "+convertLwipDebugValue(dhcpDebug)+"\n";
            outputString_lwipopts += "#define AUTOIP_DEBUG               "+convertLwipDebugValue(autoipDebug)+"\n";
            outputString_lwipopts += "#define DNS_DEBUG                  "+convertLwipDebugValue(dnsDebug)+"\n";
            outputString_lwipopts += "#define IP6_DEBUG                  "+convertLwipDebugValue(ip6Debug)+"\n";
            outputString_lwipopts += "#define PPP_DEBUG                  "+convertLwipDebugValue(pppDebug)+"\n";
            outputString_lwipopts += "#define MDNS_DEBUG                 "+convertLwipDebugValue(mdnsDebug)+"\n";
            outputString_lwipopts += "#define MQTT_DEBUG                 "+convertLwipDebugValue(mqttDebug)+"\n";
            outputString_lwipopts += "#define SNMP_DEBUG                 "+convertLwipDebugValue(snmpDebug)+"\n";
            outputString_lwipopts += "#define SNTP_DEBUG                 "+convertLwipDebugValue(sntpDebug)+"\n";
            outputString_lwipopts += "#define TFTP_DEBUG                 "+convertLwipDebugValue(tftpDebug)+"\n";
            outputString_lwipopts += "#define PING_DEBUG                 "+convertLwipDebugValue(pingDebug)+"\n";
            outputString_lwipopts += "#define HTTPD_DEBUG                "+convertLwipDebugValue(httpdDebug)+"\n";
            outputString_lwipopts += "#define RTP_DEBUG                  "+convertLwipDebugValue(rtpDebug)+"\n";
            outputString_lwipopts += "#define SMTP_DEBUG                 "+convertLwipDebugValue(smtpDebug)+"\n";
            outputString_lwipopts += "#endif /* LWIP_DEBUG */\n";
            outputString_lwipopts += "\n";
        }

        outputString_lwipopts += "/* ---------- Software checksum options ---------- */\n";
        outputString_lwipopts += "#define LWIP_CHECKSUM_CTRL_PER_NETIF    "+Number(boolCksumPerNetif)+"\n";
        outputString_lwipopts += "#define CHECKSUM_GEN_IP                 "+Number(boolCksumIPGen)+"\n";
        outputString_lwipopts += "#define CHECKSUM_GEN_UDP                "+Number(boolCksumUDPGen)+"\n";
        outputString_lwipopts += "#define CHECKSUM_GEN_TCP                "+Number(boolCksumTCPGen)+"\n";
        outputString_lwipopts += "#define CHECKSUM_GEN_ICMP               "+Number(boolCksumICMPGen)+"\n";
        outputString_lwipopts += "#define CHECKSUM_GEN_ICMP6              LWIP_IPV6\n";
        outputString_lwipopts += "#define CHECKSUM_CHECK_IP               "+Number(boolCksumIPValid)+"\n";
        outputString_lwipopts += "#define CHECKSUM_CHECK_UDP              "+Number(boolCksumUDPValid)+"\n";
        outputString_lwipopts += "#define CHECKSUM_CHECK_TCP              "+Number(boolCksumTCPValid)+"\n";
        outputString_lwipopts += "#define CHECKSUM_CHECK_ICMP             "+Number(boolCksumICMPValid)+"\n";
        outputString_lwipopts += "#define CHECKSUM_CHECK_ICMP6            LWIP_IPV6\n";
        outputString_lwipopts += "#define LWIP_CHECKSUM_ON_COPY           "+Number(boolCksumOnCopy)+"\n";
        outputString_lwipopts += "\n";

        outputString_lwipopts += "/* ---------- Other options ---------- */\n";
        outputString_lwipopts += "#define LWIP_NETIF_HOSTNAME           "+Number(boolEnableHostname)+"\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "/* (LWIP_ETHERNET == 1) Enable ETHERNET support even though ARP might be disabled */\n";
        outputString_lwipopts += "#define LWIP_ETHERNET                 1\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define TCPIP_MBOX_SIZE              10\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define DEFAULT_UDP_RECVMBOX_SIZE       10\n";
        outputString_lwipopts += "#define DEFAULT_TCP_RECVMBOX_SIZE       10\n";
        outputString_lwipopts += "#define DEFAULT_RAW_RECVMBOX_SIZE       10\n";
        outputString_lwipopts += "#define DEFAULT_ACCEPTMBOX_SIZE         10\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_NETIF_TX_SINGLE_PBUF  1\n";
        outputString_lwipopts += "#define LWIP_SUPPORT_CUSTOM_PBUF   1\n";
        outputString_lwipopts += "#define MEMP_USE_CUSTOM_POOLS      0\n";
        outputString_lwipopts += "#define MEM_USE_POOLS              0\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_SNMP                  LWIP_UDP\n";
        outputString_lwipopts += "#define MIB2_STATS                 LWIP_SNMP\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_DNS                   LWIP_UDP\n";
        outputString_lwipopts += "#define LWIP_MDNS_RESPONDER        LWIP_UDP\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_NUM_NETIF_CLIENT_DATA (LWIP_MDNS_RESPONDER)\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_HAVE_LOOPIF           0\n";
        outputString_lwipopts += "#define LWIP_NETIF_LOOPBACK        0\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define TCP_LISTEN_BACKLOG         1\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_COMPAT_SOCKETS        1\n";
        outputString_lwipopts += "#define LWIP_SO_RCVTIMEO           1\n";
        outputString_lwipopts += "#define LWIP_SO_RCVBUF             1\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_TCPIP_CORE_LOCKING    1\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_NETIF_LINK_CALLBACK   0\n";
        outputString_lwipopts += "#define LWIP_NETIF_STATUS_CALLBACK 0\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_NETCONN_SEM_PER_THREAD " + netconn_semPerThread + "\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#define LWIP_SOCKET_SET_ERRNO       1\n";
        outputString_lwipopts += "\n";
        outputString_lwipopts += "#endif /* LWIP_OPTS_H */\n";

        genCustomHeaderFile("lwipopts", outputString_lwipopts, coreId)

        /* get value from UI for lwippools */
        genCustomHeaderFile("lwippools", lwippoolString(), coreId)

        /* generating netifcfg header files */
        outputString_netifcfgHeader += addHeaderCommentsTCPIPConfig();
        outputString_netifcfgHeader += "#ifndef LWIP_NETIFCFG_H_\n";
        outputString_netifcfgHeader += "#define LWIP_NETIFCFG_H_\n";
        outputString_netifcfgHeader += "\n";
        outputString_netifcfgHeader += "#include \"lwip/netif.h\"\n";
        outputString_netifcfgHeader += "\n";
        outputString_netifcfgHeader += "/* Structure containing network interfaces configuration. */\n";
        outputString_netifcfgHeader += "typedef struct {\n";
        outputString_netifcfgHeader += "    u8_t num;                               /* Interface number */\n";
        outputString_netifcfgHeader += "    uint8_t hwaddr[NETIF_MAX_HWADDR_LEN];   /* MAC address */\n";
        outputString_netifcfgHeader += "    bool has_dhcp;                          /* Variable containing information whether dhcp is enabled or not */\n";
        outputString_netifcfgHeader += "    bool has_auto_ip;                       /* Variable containing information whether auto ip is enabled or not */\n";
        outputString_netifcfgHeader += "    bool has_IPv6;                          /* Variable containing information whether ipv6 is enabled or not */\n";
        outputString_netifcfgHeader += "    u8_t ip_addr[4];                        /* Board's ip address. Needs to be set if dhcp is turned off. */\n";
        outputString_netifcfgHeader += "    u8_t netmask[4];                        /* Board's netmask. Needs to be set if dhcp is turned off. */\n";
        outputString_netifcfgHeader += "    u8_t gw[4];                             /* Board's network gateway. Needs to be set if dhcp is turned off. */\n";
        outputString_netifcfgHeader += "    const char *hostname;                   /* Board's hostname used for dns. */\n";
        outputString_netifcfgHeader += "    char name[2];                           /* Interface name */\n";
        outputString_netifcfgHeader += "} netif_custom_t;\n";
        outputString_netifcfgHeader += "\n";

        var parseMiiMode = "null";
        var parseMiiSpeed = "null";

        if (lowlevelChoice.getValue() == "ENET")
        {
            outputString_netifcfgHeader += "#include \"enetif.h\"\n";
            outputString_netifcfgHeader += "\n";

            outputString_netifcfgHeader += "/* Number of active network interfaces. */\n";
            outputString_netifcfgHeader += "#define ENETIF_NUMBER "+netifUsed+"\n";
            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Number of Ethernet Interfaces for the stack */\n";
            outputString_netifcfgHeader += "#define ETHIF_NUMBER ENETIF_NUMBER\n";
            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Network interfaces configuration. */\n";
            outputString_netifcfgHeader += "extern netif_custom_t *netif_cfg[ENETIF_NUMBER];\n";

            if (miiMode == "MII") { parseMiiMode = "ENET_MII_MODE"; }
            if (miiMode == "RMII") { parseMiiMode = "ENET_RMII_MODE"; }
            if (miiMode == "RGMII") { parseMiiMode = "ENET_RGMII_MODE"; }

            if (miiSpeed == "10M") { parseMiiSpeed = "ENET_MII_SPEED_10M"; }
            if (miiSpeed == "100M") { parseMiiSpeed = "ENET_MII_SPEED_100M"; }
            if (miiSpeed == "1G") { parseMiiSpeed = "ENET_MII_SPEED_1000M"; }

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Ethernet MII mode - speed. */\n";
            outputString_netifcfgHeader += "#define ENET_MIIMODE                  "+parseMiiMode+"\n";
            outputString_netifcfgHeader += "#define ENET_MIISPEED                 "+parseMiiSpeed+"\n";

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "#ifndef FEATURE_ENET_RX_CONFIG_PAYLOAD_LEN_CHECK\n";
            outputString_netifcfgHeader += "#define FEATURE_ENET_RX_CONFIG_PAYLOAD_LEN_CHECK    1U"+"\n";
            outputString_netifcfgHeader += "#endif /* FEATURE_ENET_RX_CONFIG_PAYLOAD_LEN_CHECK */\n";

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "#ifndef FEATURE_ENET_RX_CONFIG_STRIP_CRC_FIELD\n";
            outputString_netifcfgHeader += "#define FEATURE_ENET_RX_CONFIG_STRIP_CRC_FIELD      1U"+"\n";
            outputString_netifcfgHeader += "#endif /* FEATURE_ENET_RX_CONFIG_STRIP_CRC_FIELD */\n";

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "#ifndef FEATURE_ENET_RX_CONFIG_REMOVE_PADDING\n";
            outputString_netifcfgHeader += "#define FEATURE_ENET_RX_CONFIG_REMOVE_PADDING    1U"+"\n";
            outputString_netifcfgHeader += "#endif /* FEATURE_ENET_RX_CONFIG_REMOVE_PADDING */\n";

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "#ifndef FEATURE_ENET_HAS_TX_CONFIG\n";
            outputString_netifcfgHeader += "#define FEATURE_ENET_HAS_TX_CONFIG      1U"+"\n";
            outputString_netifcfgHeader += "#endif /* FEATURE_ENET_HAS_TX_CONFIG */\n";

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "#ifndef FEATURE_ENET_HAS_ACCELERATOR\n";
            outputString_netifcfgHeader += "#define FEATURE_ENET_HAS_ACCELERATOR      1U"+"\n";
            outputString_netifcfgHeader += "#endif /* FEATURE_ENET_HAS_ACCELERATOR */\n";
        }
        else if (lowlevelChoice.getValue() == "GMAC")
        {
            outputString_netifcfgHeader += "#include \"gmacif.h\"\n";
            outputString_netifcfgHeader += "\n";

            outputString_netifcfgHeader += "/* Number of active network interfaces. */\n";
            outputString_netifcfgHeader += "#define GMACIF_NUMBER "+netifUsed+"\n";
            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Number of Ethernet Interfaces for the stack */\n";
            outputString_netifcfgHeader += "#define ETHIF_NUMBER GMACIF_NUMBER\n";
            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Network interfaces configuration. */\n";
            outputString_netifcfgHeader += "extern netif_custom_t *netif_cfg[GMACIF_NUMBER];\n";

            if (miiMode == "MII") { parseMiiMode = "GMAC_MII_MODE"; }
            if (miiMode == "RMII") { parseMiiMode = "GMAC_RMII_MODE"; }
            if (miiMode == "RGMII") { parseMiiMode = "GMAC_RGMII_MODE"; }
            if (miiMode == "SGMII") { parseMiiMode = "GMAC_SGMII_MODE"; }

            if (miiSpeed == "10M") { parseMiiSpeed = "GMAC_SPEED_10M"; }
            if (miiSpeed == "100M") { parseMiiSpeed = "GMAC_SPEED_100M"; }
            if (miiSpeed == "1G") { parseMiiSpeed = "GMAC_SPEED_1G"; }
            if (miiSpeed == "2.5G") { parseMiiSpeed = "GMAC_SPEED_2P5G"; }

            outputString_netifcfgHeader += "\n";
            outputString_netifcfgHeader += "/* Ethernet MII mode - speed. */\n";
            outputString_netifcfgHeader += "#define GMAC_MODE                  "+parseMiiMode+"\n";
            outputString_netifcfgHeader += "#define GMAC_SPEED                 "+parseMiiSpeed+"\n";
        }

        outputString_netifcfgHeader += "\n";
        outputString_netifcfgHeader += "#endif /* LWIP_NETIFCFG_H_ */\n";

        /* generating netifcfg source files */
        outputString_netifcfgSource += addHeaderCommentsTCPIPConfig();
        outputString_netifcfgSource += "#include \"netifcfg.h\"\n";
        outputString_netifcfgSource += "\n";

        for (var i = 0; i < netifArraySize; i++)
        {
            if (netifArrayList[i].getChildById("boolIface").getValue() == true)
            {
                var macString = "0x"+(netifArrayList[i].getChildById("macAddress").getValue()).replace(/:/g, ",0x");
                var hostname = netifArrayList[i].getChildById("hostname").getValue();

                outputString_netifcfgSource += "#ifndef LWIP_MAC_ADDR_BASE_"+i+"\n";
                outputString_netifcfgSource += "#define LWIP_MAC_ADDR_BASE_"+i+"          "+macString+"\n";
                outputString_netifcfgSource += "#endif /* LWIP_MAC_ADDR_BASE_"+i+" */\n";
                outputString_netifcfgSource += "\n";
                outputString_netifcfgSource += "#ifndef LWIP_NETIF_HOSTNAME_TEXT_"+i+"\n";
                outputString_netifcfgSource += "#define LWIP_NETIF_HOSTNAME_TEXT_"+i+"          (\""+hostname+"\")\n";
                outputString_netifcfgSource += "#endif /* LWIP_NETIF_HOSTNAME_TEXT_"+i+" */\n";
                outputString_netifcfgSource += "\n";
                outputString_netifcfgSource += "#ifndef IF_NAME_"+i+"\n";
                outputString_netifcfgSource += "#define IF_NAME_"+i+"          'i','"+eval(i+1)+"'\n";
                outputString_netifcfgSource += "#endif /* IF_NAME_"+i+" */\n";
                outputString_netifcfgSource += "\n";
            }
        }

        for (var i = 0; i < netifArraySize; i++)
        {
            if (netifArrayList[i].getChildById("boolIface").getValue() == true)
            {
                var boolDhcp = netifArrayList[i].getChildById("boolDhcp").getValue();
                var boolAutoip = netifArrayList[i].getChildById("boolAutoip").getValue();
                var boolIpv6 = netifArrayList[i].getChildById("boolIpv6").getValue();
                var address = netifArrayList[i].getChildById("ipv4Address").getValue();
                var subnet = netifArrayList[i].getChildById("ipv4Subnet").getValue();
                var gateway = netifArrayList[i].getChildById("ipv4Gateway").getValue();

                outputString_netifcfgSource += "/* Each netif configuration */\n";
                outputString_netifcfgSource += "netif_custom_t netifCfg_"+i+" = {\n";
                outputString_netifcfgSource += "    .num = "+i+",\n";
                outputString_netifcfgSource += "    .hwaddr = { LWIP_MAC_ADDR_BASE_"+i+" },\n";
                outputString_netifcfgSource += "    .has_dhcp = "+boolDhcp+",\n";
                outputString_netifcfgSource += "    .has_auto_ip = "+boolAutoip+",\n";
                outputString_netifcfgSource += "    .has_IPv6 = "+boolIpv6+",\n";
                outputString_netifcfgSource += "    .ip_addr = { "+address.replace(/\./g, ',')+" },\n";
                outputString_netifcfgSource += "    .netmask = { "+subnet.replace(/\./g, ',')+" },\n";
                outputString_netifcfgSource += "    .gw = { "+gateway.replace(/\./g, ',')+" },\n";
                outputString_netifcfgSource += "    .hostname = LWIP_NETIF_HOSTNAME_TEXT_"+i+",\n";
                outputString_netifcfgSource += "    .name =  { IF_NAME_"+i+" },\n";
                outputString_netifcfgSource += "};\n";
                outputString_netifcfgSource += "\n";
            }
        }

        outputString_netifcfgSource += "/* Array of netif configurations */\n";
        outputString_netifcfgSource += "netif_custom_t *netif_cfg[] = {\n";
        for (var i = 0; i < netifArraySize; i++)
        {
            if (netifArrayList[i].getChildById("boolIface").getValue() == true)
            {
                if (i == (netifArraySize-1))
                {
                    outputString_netifcfgSource += "          &netifCfg_"+i+"\n";
                }
                else
                {
                    outputString_netifcfgSource += "          &netifCfg_"+i+",\n";
                }
            }
        }
        outputString_netifcfgSource += "};\n";

        genCustomHeaderFile("netifcfg", outputString_netifcfgHeader, coreId)
        genCustomSourceFile("netifcfg", outputString_netifcfgSource, coreId)
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        try {
            // Add Includes info.
        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        try{
        cFileSections[GLOBAL_VARS_SECTION] += "/* dummy_t type to bypass warning of forbids an empty translation unit */" + NEW_LINE;
        cFileSections[GLOBAL_VARS_SECTION] += "typedef unsigned int dummy_t;" + NEW_LINE;
        } catch(err) {
            scriptApi.logError(err);
        }
    }

}

function convertLwipDebugValue(enableValue) {
    if (enableValue == true) {
        return "LWIP_DBG_ON"
    } else {
        return "LWIP_DBG_OFF"
    }
}

function genCustomHeaderFile(fileName, string, coreId){
    try {
        var output = scriptApi.createFile(fileName+".h", coreId);
        output.write(string);
        output.close();
    } catch (error) {
        scriptApi.logError("creating "+fileName+".h" + error);
    }
}

function genCustomSourceFile(fileName, string, coreId){
    try {
        var output = scriptApi.createFile(fileName+".c", coreId);
        output.write(string);
        output.close();
    } catch (error) {
        scriptApi.logError("creating "+fileName+".c" + error);
    }
}
