/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function getStreamMessage(inputStream){
    var inReader = new java.io.BufferedReader(new java.io.InputStreamReader(inputStream));
    var inputLine;
    var response = new java.lang.StringBuffer();
    
    while ((inputLine = inReader.readLine()) != null) {
           response.append(inputLine + "\n");
    }
    inReader.close();
    return response.toString();
}

function addDefineLinStackConfig(what,value){
    var outputString = "";
    var noChars = 52;
    value +="";
    outputString += "#define "+what;
    if(noChars > (8 + what.length )){
        var N = noChars - 8 - what.length ;
        for(var i = 0; i< N ;i++){
            outputString += " ";
        }
    }else{
        outputString += " ";
    }
    
    outputString += value+"\n";
    return outputString;
}

function addMsgLinStackConfig(msg){
    return msg+"\n";
}

function validateLdfFile(struct){
    var ArrayList = Java.type('java.util.ArrayList');
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var componentConfig = struct.getChildContext().getComponentConfig();

    /* Check error information in each network interface */
    var error_info = "";
    for (var idx in ArrListOfNetwork){
        var dir = null;
        if (componentConfig != null) {
            dir = componentConfig.getConfigCompTypeId().getFileLocation();
        }

        /* Get lin description file (*.ldf) path */
        var ldfPath = "";
        if (ArrListOfNetwork[idx].getChildById("RefNetworkDetail.IsLDFAbsolutePath").getValue()){
            ldfPath = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.LDFAbsolutePath").getValue().replace("LINK=","").toString();
        }
        else{
            ldfPath = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ProjectLocation").getValue() +
                    "\\" + ArrListOfNetwork[idx].getChildById("RefNetworkDetail.LDFRelativePath").getValue()
        }
        for (var i = 0; i <= ldfPath.length; i++){
            if (ldfPath[i] == "\\"){
                ldfPath = ldfPath.replace("\\","/");
            }
        }

        /* Validate ldf file and get errors if available */
        var pb = new java.lang.ProcessBuilder("java", "-jar", dir + "\\ParseGenerate.jar", "-ldfProcess", "-validate", ldfPath, "LDF0");
        var p = pb.start();
        var is = p.getInputStream();
        var inReader = new java.io.BufferedReader(new java.io.InputStreamReader(is));
        var inputLine;
        while ((inputLine = inReader.readLine()) != null){
            if (inputLine.search("error")){
                error_info += inputLine + "\n";
            }
        }
        inReader.close();
        p.destroy();
    }

    return error_info;
}

function checkListOfSrNum(struct){
    var ArrayList = Java.type('java.util.ArrayList');
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    /* Get slave infomation from LDF */
    for (var idx in ArrListOfNetwork){
        var listOfSlaveNode = new ArrayList();
        var ldfInfo = ArrListOfNetwork[idx].getChildById("StrLDFInfo").getValue();
        var temp = ldfInfo.slice(ldfInfo.search("Slave"));
        var temp = temp.substring(temp.search(" ") + 1, temp.search(";"));
        var slaveNum = parseInt(temp);
        for (var idx1 = 0; idx1 < slaveNum; idx1++){
            temp = ldfInfo.slice(ldfInfo.search("SlaveName" + idx1));
            var NodeName = temp.substring(temp.search(" ") + 1, temp.search(";"));
            listOfSlaveNode.add(NodeName);
        }
        /* Check list of serial number*/
        var ArrListOfSr = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ArrListOfSr").getChildren();
        if (ArrListOfSr.length < slaveNum){
            while (ArrListOfSr.length < slaveNum){
                ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ArrListOfSr").addNewItem();
            }
        }else if (ArrListOfSr.length > slaveNum){
            scriptApi.logError("Please add "+ slaveNum + " items into list of serial number. The network has only " + slaveNum + " slave(s)");
        }
        else{
            /* Do nothing */
        }
        /* Set slave node name to list */
        if (ArrListOfSr.length == slaveNum){
            for (var idx1 = 0; idx1 < slaveNum; idx1++){
                ArrListOfSr[idx1].getChildById("SlaveName").setValue(listOfSlaveNode[idx1]);
            }
        }
    }
}

function covertIntToHexString(num){
    var hexArray = ['0', '1', '2', '3','4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'];
    var hexOut = [];
    var hexString = '0x';
    var idx = 0;

    while(num > 0){
        hexOut[idx] = hexArray[num%16];
        idx++;
        remain = num/16;
        num = Math.floor(remain);
    }
    idx -= 1;
    while (idx >= 0){
        hexString += hexOut[idx];
        idx--;
    }
    if (hexString == '0x'){
        hexString += '0';
    }
    // hexString +='U';

    return hexString;
}

function convertToHex(struct){
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    for (var idx in ArrListOfNetwork){
        var ArrListOfSr = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ArrListOfSr").getChildren();
        for (var idx1 in ArrListOfSr){
            var sr0Temp = ArrListOfSr[idx1].getChildById("Sr0").getValue();
            var sr1Temp = ArrListOfSr[idx1].getChildById("Sr1").getValue();
            var sr2Temp = ArrListOfSr[idx1].getChildById("Sr2").getValue();
            var sr3Temp = ArrListOfSr[idx1].getChildById("Sr3").getValue();
            ArrListOfSr[idx1].getChildById("Sr0").setValue(covertIntToHexString(sr0Temp));
            ArrListOfSr[idx1].getChildById("Sr1").setValue(covertIntToHexString(sr1Temp));
            ArrListOfSr[idx1].getChildById("Sr2").setValue(covertIntToHexString(sr2Temp));
            ArrListOfSr[idx1].getChildById("Sr3").setValue(covertIntToHexString(sr3Temp));
        }

        var ArrUserService = ArrListOfNetwork[idx].getChildById("RefSupportedService.ArrListOfSIDs").getChildren();
        for (var idx1 in ArrUserService){
            var userServiceID = ArrUserService[idx1].getChildById("SID_ID").getValue()
            ArrUserService[idx1].getChildById("SID_ID").setValue(covertIntToHexString(userServiceID));
        }
    }
}

function getLinComponent(struct){
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var childContext = struct.getChildContext();
    var profile = scriptApi.getProfile();
    var functionalGroups = profile.getFunctionalGroups();
    var instanceName = childContext.getComponentInstanceConfig().getName();
    var ArrayList = Java.type('java.util.ArrayList');
    var temp = "";
    for (var idx in ArrListOfNetwork){
        var LinDrvLinDriver = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvLinDriver").getValue();
        var linDrvLinConfig = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvLinConfig").getValue();

        /* Find functional group that this component belongs to */
        for (var index = 0; index < functionalGroups.length; index++){
            var comps = functionalGroups[index].getComponentInstances();
            for (var idComp = 0; idComp < comps.length; idComp++){
                if(comps[idComp].getName() == instanceName){
                    component = functionalGroups[index].getComponentInstances("lin");
                    /* Stop search */
                    index = functionalGroups.length;
                    break;
                }
            }
        }

        for (var index = 0; index < component.length; index++){
            if (component[index].getName() == LinDrvLinDriver){
                /* Get LPUART Device from Lin Driver */
                var peripheralName = component[index].getPeripheral();
                ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvDevice").setValue(peripheralName);

                var arrLinConfiguration = component[index].getChildById("lin_driver.linUserConfiguation.linConfiguration");
                var linConfigurationChildren = arrLinConfiguration.getChildren();
                for (var idx1 in linConfigurationChildren){
                    if (linConfigurationChildren[idx1].getChildById("name").getValue() == linDrvLinConfig){
                        /* Set BaudRate Lin Driver follow LDF file*/
                        linConfigurationChildren[idx1].getChildById("baudRate").setValue(ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvBaudRateConfig").getValue());
                        /* Get Node Function from Lin Driver */
                        if (linConfigurationChildren[idx1].getChildById("nodeFunction").getStringValue() == "(bool)MASTER"){
                            ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").setValue("MASTER");
                        }
                        else{
                            ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").setValue("SLAVE");
                        }
                        /* Get Autobaud from Lin Driver */
                        var nodeProtocol = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NodeProtocol").getValue();
                        if (nodeProtocol == "J2602"){
                            linConfigurationChildren[idx1].getChildById("autobaudEnable").setValue(false);
                        }
                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvAutobaud").setValue(linConfigurationChildren[idx1].getChildById("autobaudEnable").getValue());
                        /* Get Time Interval Callback from Lin Driver */
                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvIntCallback").setValue(linConfigurationChildren[idx1].getChildById("timerGetTimeIntervalCallback").getValue());

                        /* Set PIDs Type and check list of PIDs in to LIN Driver*/
                        var ldfInfo = ArrListOfNetwork[idx].getChildById("StrLDFInfo").getValue();
                        /* Clear list of PIDs of Linstack before process*/
                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidClassicChecksum").setValue("");
                        if (ldfInfo.length != 0){
                            if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == "MASTER"){
                                var nodeName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefNodeName").getValue();
                                if (ldfInfo.search("ListClassic") == -1){
                                    linConfigurationChildren[idx1].getChildById("classicPidType").setValue("Enhanced");
                                    ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidType").setValue("Enhanced");
                                }
                                else{
                                    temp = ldfInfo.slice(ldfInfo.search("Slave"), ldfInfo.search(nodeName + "Protocol"));
                                    if (temp.search("Protocol") == -1){
                                        linConfigurationChildren[idx1].getChildById("classicPidType").setValue("Classic");
                                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidType").setValue("Classic");
                                    }
                                    else{
                                        linConfigurationChildren[idx1].getChildById("classicPidType").setValue("Mixed");
                                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidType").setValue("Mixed");
                                        temp = ldfInfo.slice(ldfInfo.search(nodeName + "ListClassic"));
                                        var temp = temp.substring(temp.search(" ") + 1, temp.search(";"));
                                        var frameClassicNum = parseInt(temp);
                                        var listOfPIDs = new ArrayList();
                                        var PIDs = "";
                                        for (var idx2 = 0; idx2 < frameClassicNum; idx2++){
                                            temp = ldfInfo.slice(ldfInfo.search("SeatECUFrame" + idx2));
                                            listOfPIDs.add(temp.substring(temp.search(" ") + 1, temp.search(";")));
                                            PIDs += temp.substring(temp.search(" ") + 1, temp.search(";")) + "; ";
                                        }
                                        ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidClassicChecksum").setValue(PIDs);
                                        var arrListOfPIDs = linConfigurationChildren[idx1].getChildById("numOfClassicPID").getChildren();
                                        /* Check list of PIDs of Lin Driver and Linstack*/
                                        if (arrListOfPIDs.length != listOfPIDs.length){
                                            scriptApi.logError("Please change " + linConfigurationChildren[idx1].getChildById("name").getValue() + " and fill " + listOfPIDs + " into list of PIDs");
                                        }
                                        else{
                                            for (var i = 0; i < arrListOfPIDs.length; i++){
                                                var isFound = false;
                                                for (var j = 0; j < listOfPIDs.length; j++){
                                                    var PIDValue = arrListOfPIDs[i].getChildById("numberclassicPID").getValue();
                                                    if (PIDValue == listOfPIDs[j]){
                                                        isFound = true;
                                                    }
                                                }
                                                if (!isFound){
                                                    scriptApi.logError(linConfigurationChildren[idx1].getChildById("name").getValue() + ": PID " + PIDValue + " not found in LinStack. List of PIDs is " + listOfPIDs);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else{
                                var nodeName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefNodeName").getValue();
                                if (ldfInfo.search(nodeName + "ListClassic") == -1){
                                    linConfigurationChildren[idx1].getChildById("classicPidType").setValue("Enhanced");
                                    ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidType").setValue("Enhanced");
                                }
                                else{
                                    linConfigurationChildren[idx1].getChildById("classicPidType").setValue("Classic");
                                    ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvPidType").setValue("Classic");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

function setHideField(struct){
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var temp = "";
    for(var idx in ArrListOfNetwork){
        /* Get node protocol */
        var ldfInfo = ArrListOfNetwork[idx].getChildById("StrLDFInfo").getValue();
        var NodeName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefNodeName").getValue();
        temp = ldfInfo.slice(ldfInfo.search(NodeName + "Protocol"));
        ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NodeProtocol").setValue(temp.substring(temp.search(" ") + 1, temp.search(";")));
        ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeProtocol").setValue(temp.substring(temp.search(" ") + 1, temp.search(";")));

        if (ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NodeProtocol").getValue() == "1.3"){
            ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").setValue("Class1");
        }
        /* Get DiagClass from RefNetworkDetail to RefSupportedService */
        temp = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").getValue();
        ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceClassInfo").setValue(temp);
        temp = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue();
        ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeFunc").setValue(temp);
    }
}

function setSupportedService(struct){
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var ArrayList = Java.type('java.util.ArrayList');

    var serviceList = new ArrayList();
    serviceList.add("ServiceAssignNAD");
    serviceList.add("ServiceAssignFrameIdentifier");
    serviceList.add("ServiceReadByIdentifier");
    serviceList.add("ServiceConditionalChangeNAD");
    serviceList.add("ServiceDataDump");
    serviceList.add("ServiceSaveConfiguration");
    serviceList.add("ServiceAssignFrameIdenRange");
    serviceList.add("ServiceReadDataByIdentifier");
    serviceList.add("ServiceWriteDataByIdentifier");
    serviceList.add("ServiceSessionControl");
    serviceList.add("ServiceIOControlByIdentifier");
    serviceList.add("ServiceFaultMemoryRead");
    serviceList.add("ServiceFaultMemoryClear");
    serviceList.add("ServiceTargetReset");

    var serviceMandatoryClass1 = new ArrayList();
    serviceMandatoryClass1.add("ServiceReadByIdentifier");
    var serviceOptionalClass1 = new ArrayList();
    serviceOptionalClass1.add("ServiceConditionalChangeNAD");
    serviceOptionalClass1.add("ServiceDataDump");
    serviceOptionalClass1.add("ServiceAssignNAD");

    var serviceMandatoryClass2 = new ArrayList();
    serviceMandatoryClass2.add("ServiceAssignFrameIdenRange");
    serviceMandatoryClass2.add("ServiceReadDataByIdentifier");
    serviceMandatoryClass2.add("ServiceReadByIdentifier");
    var serviceOptionalClass2 = new ArrayList();
    serviceOptionalClass2.add("ServiceConditionalChangeNAD");
    serviceOptionalClass2.add("ServiceDataDump");
    serviceOptionalClass2.add("ServiceSaveConfiguration");
    serviceOptionalClass2.add("ServiceAssignNAD");
    serviceOptionalClass2.add("ServiceWriteDataByIdentifier");

    var serviceMandatoryClass3 = new ArrayList();
    serviceMandatoryClass3.add("ServiceReadByIdentifier");
    serviceMandatoryClass3.add("ServiceSaveConfiguration");
    serviceMandatoryClass3.add("ServiceAssignFrameIdenRange");
    serviceMandatoryClass3.add("ServiceReadDataByIdentifier");
    serviceMandatoryClass3.add("ServiceSessionControl");
    serviceMandatoryClass3.add("ServiceIOControlByIdentifier");
    serviceMandatoryClass3.add("ServiceFaultMemoryRead");
    serviceMandatoryClass3.add("ServiceFaultMemoryClear");
    var serviceOptionalClass3 = new ArrayList();
    serviceOptionalClass3.add("ServiceAssignNAD");
    serviceOptionalClass3.add("ServiceConditionalChangeNAD");
    serviceOptionalClass3.add("ServiceWriteDataByIdentifier");
    serviceOptionalClass3.add("ServiceDataDump");

    var serviceMandatoryJ2602 = new ArrayList();
    serviceMandatoryJ2602.add("ServiceTargetReset");
    var serviceOptionalJ2602 = new ArrayList();
    serviceOptionalJ2602.add("ServiceAssignFrameIdentifier");
    serviceOptionalJ2602.add("ServiceReadByIdentifier");
    serviceOptionalJ2602.add("ServiceAssignNAD");
    serviceOptionalJ2602.add("ServiceConditionalChangeNAD");
    serviceOptionalJ2602.add("ServiceDataDump");

    var serviceMandatoryLIN20 = new ArrayList();
    serviceMandatoryLIN20.add("ServiceAssignFrameIdentifier");
    serviceMandatoryLIN20.add("ServiceReadByIdentifier");
    var serviceOptionalLIN20 = new ArrayList();
    serviceOptionalLIN20.add("ServiceAssignNAD");
    serviceOptionalLIN20.add("ServiceConditionalChangeNAD");
    serviceOptionalLIN20.add("ServiceDataDump");

    var serviceMandatoryLIN21 = new ArrayList();
    serviceMandatoryLIN21.add("ServiceAssignFrameIdenRange");
    var serviceOptionalLIN21 = new ArrayList();
    serviceOptionalLIN21.add("ServiceSaveConfiguration");

    var protocol2x = 0;
    var protocol20 = 0;
    var protocolj2602 = 0;

    for(var idx in ArrListOfNetwork){
        /* Set mandatory service is true and not is false */
        var serviceMandatoryList = new ArrayList();
        var serviceOptionalList = new ArrayList();

        var nodeClass = ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceClassInfo").getValue();
        var nodeProtocol = ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeProtocol").getValue();
        var nodeFunc = ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeFunc").getValue();

        if (nodeClass != "Class3" || nodeProtocol == "2.0" || nodeProtocol == "1.3" || nodeFunc == "MASTER"){
            ArrListOfNetwork[idx].getChildById("RefSupportedService.SupportSID").setValue(false);
        }

        if (nodeFunc == "SLAVE"){
            if (nodeProtocol == "2.x"){
                if (nodeClass == "Class1"){
                    serviceMandatoryList = serviceMandatoryLIN21 + serviceMandatoryClass1;
                    serviceOptionalList = serviceOptionalLIN21 + serviceOptionalClass1;
                }
                else if(nodeClass == "Class2"){
                    serviceMandatoryList = serviceMandatoryLIN21 + serviceMandatoryClass2;
                    serviceOptionalList = serviceOptionalLIN21 + serviceOptionalClass2;
                }
                else{
                    serviceMandatoryList = serviceMandatoryLIN21 + serviceMandatoryClass3;
                    serviceOptionalList = serviceOptionalLIN21 + serviceOptionalClass3;
                }
            }
            else if (nodeProtocol == "2.0"){
                serviceMandatoryList = serviceMandatoryLIN20;
                serviceOptionalList = serviceOptionalLIN20;
            }
            else if (nodeProtocol == "J2602"){
                serviceMandatoryList = serviceMandatoryJ2602;
                serviceOptionalList = serviceOptionalJ2602;
            }
            else{
                /* Do nothing */
            }
        }
        /* Set service support of node */
        for (var idx1 in serviceList){
            if (serviceMandatoryList.indexOf(serviceList[idx1]) != -1){
                ArrListOfNetwork[idx].getChildById("RefSupportedService." + serviceList[idx1]).setValue(true);
            }
            else if (serviceOptionalList.indexOf(serviceList[idx1]) !=-1){
                /* Do nothing */
            }
            else{
                ArrListOfNetwork[idx].getChildById("RefSupportedService." + serviceList[idx1]).setValue(false);
            }
        }

        /* Get all node protocol */
        if (nodeProtocol == "2.x"){
            protocol2x = 1;
        }
        else if (nodeProtocol == "2.0"){
            protocol20 = 1;
        }
        else if (nodeProtocol == "J2602"){
            protocolj2602 = 1;
        }
        else{
            /* Do nothing */
        }
    }
    /* Set Transport Layer and Support Diagnotic Service Item */
    if ((protocol2x == 0) && (protocol20 == 0) && (protocolj2602 == 0))
    {
        struct.getChildById("RefNetworkConfig.UseTransportLayer").setValue(false);
        struct.getChildById("RefNetworkConfig.SupportDiagnoticService").setValue(false);
    }
}

function arrayDiagService(struct){
    var ArrayList = Java.type('java.util.ArrayList');
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var NetworkName = "";
    var response = "";

    var serviceList = new ArrayList();
    serviceList.add("ServiceAssignNAD");
    serviceList.add("ServiceAssignFrameIdentifier");
    serviceList.add("ServiceReadByIdentifier");
    serviceList.add("ServiceConditionalChangeNAD");
    serviceList.add("ServiceDataDump");
    serviceList.add("ServiceSaveConfiguration");
    serviceList.add("ServiceAssignFrameIdenRange");
    serviceList.add("ServiceReadDataByIdentifier");
    serviceList.add("ServiceWriteDataByIdentifier");
    serviceList.add("ServiceSessionControl");
    serviceList.add("ServiceIOControlByIdentifier");
    serviceList.add("ServiceFaultMemoryRead");
    serviceList.add("ServiceFaultMemoryClear");
    serviceList.add("ServiceTargetReset");

    var sidSupported = new ArrayList();
    sidSupported.add("0xB0");
    sidSupported.add("0xB1");
    sidSupported.add("0xB2");
    sidSupported.add("0xB3");
    sidSupported.add("0xB4");
    sidSupported.add("0xB6");
    sidSupported.add("0xB7");
    sidSupported.add("0x22");
    sidSupported.add("0x2E");
    sidSupported.add("0x10");
    sidSupported.add("0x2F");
    sidSupported.add("0x19");
    sidSupported.add("0x14");
    sidSupported.add("0xB5");

    for (var idx in ArrListOfNetwork)
    {
        var sidList = new ArrayList();
        var index = 0;
        var idx1 = 0;
        if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == "SLAVE"){
            NetworkName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NetworkName").getValue();
            for (idx1 in serviceList)
            {
                if (ArrListOfNetwork[idx].getChildById("RefSupportedService." + serviceList[idx1]).getValue())
                {
                    sidList.add(sidSupported[idx1]);
                    index++;
                }
            }
            if (ArrListOfNetwork[idx].getChildById("RefSupportedService.SupportSID").getValue())
            {
                var ArrUserService = ArrListOfNetwork[idx].getChildById("RefSupportedService.ArrListOfSIDs").getChildren();
                for (idx1 in ArrUserService)
                {
                    if (sidList.indexOf(ArrUserService[idx1].getChildById("SID_ID").getValue()) != -1){
                        scriptApi.logError("There are some duplicated SID ID");
                    }
                    else{
                        sidList.add(ArrUserService[idx1].getChildById("SID_ID").getValue());
                        index++;
                    }
                }
            }
            if (index > 0){
                response += "\n";
                response += "static const l_u8 " + NetworkName + "_lin_diag_services_supported[" + NetworkName + "_DIAG_NUMBER_OF_SERVICES] = {";
                for (idx1 = 0; idx1 < index - 1; idx1++)
                {
                    response += sidList[idx1] + ", ";
                }
                response += sidList[index - 1] + "};\n";
                response += "static l_u8 " + NetworkName + "_lin_diag_services_flag[" + NetworkName + "_DIAG_NUMBER_OF_SERVICES] = {";
                for (idx1 = 0; idx1 < index - 1; idx1++)
                {
                    response += 0 + ", ";
                }
                response += 0 + "};\n";
            }
            index = 0;
            sidList.clear();
        }
    }
    response += "\n";
    return response;
}

function defineDiagService(struct){
    var ArrayList = Java.type('java.util.ArrayList');
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();

    var serviceList = new ArrayList();
    serviceList.add("ServiceAssignNAD");
    serviceList.add("ServiceAssignFrameIdentifier");
    serviceList.add("ServiceReadByIdentifier");
    serviceList.add("ServiceConditionalChangeNAD");
    serviceList.add("ServiceDataDump");
    serviceList.add("ServiceSaveConfiguration");
    serviceList.add("ServiceAssignFrameIdenRange");
    serviceList.add("ServiceReadDataByIdentifier");
    serviceList.add("ServiceWriteDataByIdentifier");
    serviceList.add("ServiceSessionControl");
    serviceList.add("ServiceIOControlByIdentifier");
    serviceList.add("ServiceFaultMemoryRead");
    serviceList.add("ServiceFaultMemoryClear");
    serviceList.add("ServiceTargetReset");

    var serviceName = new ArrayList();
    serviceName.add("ASSIGN_NAD");
    serviceName.add("ASSIGN_FRAME_IDENTIFIER");
    serviceName.add("READ_BY_IDENTIFIER");
    serviceName.add("CONDITIONAL_CHANGE_NAD");
    serviceName.add("DATA_DUMP");
    serviceName.add("SAVE_CONFIGURATION");
    serviceName.add("ASSIGN_FRAME_ID_RANGE");
    serviceName.add("READ_DATA_BY_IDENTIFIER");
    serviceName.add("WRITE_DATA_BY_IDENTIFIER");
    serviceName.add("SESSION_CONTROL");
    serviceName.add("IO_CONTROL_BY_IDENTIFIER");
    serviceName.add("FAULT_MEMORY_READ");
    serviceName.add("FAULT_MEMORY_CLEAR");
    serviceName.add("TARGET_RESET");

    var NetworkName = "";
    var response = "";
    var maxMessLength = 0;
    var index = 0;
    var idx1 = 0;

    for (var idx in ArrListOfNetwork){
        var refDiagClass = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").getValue();
        var linDrvNodeFunc = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue();

        NetworkName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NetworkName").getValue();

        if (maxMessLength < ArrListOfNetwork[idx].getChildById("RefNetworkDetail.MaxMessageLength").getValue()){
            maxMessLength = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.MaxMessageLength").getValue();
        }
        response += "\n";

        var serviceNameList = new ArrayList();
        for (idx1 in serviceList){
            if (ArrListOfNetwork[idx].getChildById("RefSupportedService." + serviceList[idx1]).getValue())
            {
                response += "#define " + NetworkName + "_DIAGSRV_" + serviceName[idx1] + "_ORDER       " + index + "U\n";
                serviceNameList.add(serviceName[idx1]);
                index++;
            }
        }

        if (ArrListOfNetwork[idx].getChildById("RefSupportedService.SupportSID").getValue()){
            var ArrUserService = ArrListOfNetwork[idx].getChildById("RefSupportedService.ArrListOfSIDs").getChildren();
            for (idx1 in ArrUserService){
                if (serviceNameList.indexOf(ArrUserService[idx1].getChildById("SID_Name").getValue()) != -1){
                    scriptApi.logError("There are some duplicated SID name");
                }
                else{
                    response += "#define " + NetworkName + "_DIAGSRV_" + ArrUserService[idx1].getChildById("SID_Name").getValue() + "_ORDER       " + index + "U\n";
                    serviceNameList.add(ArrUserService[idx1].getChildById("SID_Name").getValue());
                    index++;
                }
            }
        }
        if (index != 0){
            response += "\n";
            response += "#define " + NetworkName + "_DIAG_NUMBER_OF_SERVICES        " + index + "U\n";
        }
        index = 0;
    }
    response += "\n";
    response += "#define MAX_LENGTH_SERVICE     " + maxMessLength + "U\n";

    return response;
}

function genNPF (struct){
    var ArrayList = Java.type('java.util.ArrayList');
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var linstackComponentDir = null;

    var componentConfig = struct.getChildContext().getComponentConfig();
    var profile = scriptApi.getProfile();
    var TimeOutUnit = struct.getChildById("RefNetworkConfig.TimeOutUnit").getValue();
    var cpu = profile.getMcuInfo().getPartNumber();
    var idx = 0;
    var outputGenCode = "";
    outputGenCode += "/*** GENERAL DEFINITION ***/" + NEW_LINE;
    outputGenCode += "LIN_node_config_file;" + NEW_LINE;
    outputGenCode += "/*** GENERAL DEFINITION ***/" + NEW_LINE;
    outputGenCode += "mcu {" + NEW_LINE;
    outputGenCode += "    mcu_name                      = " + cpu + ";" + NEW_LINE;
    outputGenCode += "    bus_clock                     = 48000000;" + "                 /* Frequency bus of system Hz*/" + NEW_LINE;
    outputGenCode += "    xgate_support                 = no;" + "                       /* Support XGATE Co-Processor */" + NEW_LINE;
    outputGenCode += "}" + NEW_LINE;
    outputGenCode += "/*** LIN HARDWARE DEFINITION ***/" + NEW_LINE;
    outputGenCode += "sci {" + NEW_LINE;
    for (idx in ArrListOfNetwork)
    {
        var PeriphDevice = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvDevice").getValue();

        if (PeriphDevice == "LPUART_0")
        {
            index = 0;
        }
        else if (PeriphDevice == "LPUART_1")
        {
            index = 1;
        }
        else
        {
            index = 2;
        }
        outputGenCode += "    " + PeriphDevice + "{"+ NEW_LINE;
        outputGenCode += "        sci_channel               = " + index + ";" + NEW_LINE;
        outputGenCode += "    }" + NEW_LINE;
    }
    outputGenCode += "}" + NEW_LINE;
    outputGenCode += "/*** NETWORK DEFINITION ***/" + NEW_LINE;
    outputGenCode += "network {" + NEW_LINE;
    outputGenCode += "    idle_timeout                  = 0s;" + NEW_LINE;
    outputGenCode += "    resynchronization_support     = no;" + NEW_LINE;
    var ldfPathList = new ArrayList();
    var ldfPath = "";
    for (idx in ArrListOfNetwork)
    {
        var networkname = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NetworkName").getValue();

        /* Get node name and show error log if it is not available */
        var NodeName = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefNodeName").getValue();
        if (NodeName == ""){
            scriptApi.logError("Node name is not available. Please re-select or check the LDF file");
        }

        /* Get LDF path and convert to linux path type */
        if (ArrListOfNetwork[idx].getChildById("RefNetworkDetail.IsLDFAbsolutePath").getValue()){
            ldfPath = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.LDFAbsolutePath").getValue().replace("LINK=","").toString();
        }
        else{
            ldfPath = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ProjectLocation").getValue() +
                    "\\" + ArrListOfNetwork[idx].getChildById("RefNetworkDetail.LDFRelativePath").getValue()
        }
        for (var i = 0; i <= ldfPath.length; i++){
            if (ldfPath[i] == "\\"){
                ldfPath = ldfPath.replace("\\","/");
            }
        }

        /* Check node name and ldf file name must not same */
        for (var idx1 = 0; idx1 < ldfPathList.length; idx1++){
            if (ldfPathList[idx1] == ldfPath){
                scriptApi.logError("Networks must not have the same node name and ldf file name");
            }
        }
        ldfPathList.add(ldfPath);

        var PeriphDevice = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvDevice").getValue();
        var DiagnosticClassTemp = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").getValue();
        if (DiagnosticClassTemp == "Class1"){
            DiagnosticClass = 1;
        }
        else if (DiagnosticClassTemp == "Class2"){
            DiagnosticClass = 2;
        }
        else{
            DiagnosticClass = 3;
        }

        var AutobaudTemp = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvAutobaud").getValue();
        if (AutobaudTemp){
            Autobaud = "yes";
        }
        else{
            Autobaud = "no";
        }
        var MaxMessageLength = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.MaxMessageLength").getValue();

        /* Get serial number of node*/
        var SerialNumber = 0;
        if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == "SLAVE"){
            var ArrListOfSr = ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ArrListOfSr").getChildren();
            for (var idx1 = 0; idx1 < ArrListOfSr.length; idx1++){
                if (NodeName == ArrListOfSr[idx1].getChildById("SlaveName").getValue()){
                    var sr0 = ArrListOfSr[idx1].getChildById("Sr0").getValue();
                    var sr1 = ArrListOfSr[idx1].getChildById("Sr1").getValue();
                    var sr2 = ArrListOfSr[idx1].getChildById("Sr2").getValue();
                    var sr3 = ArrListOfSr[idx1].getChildById("Sr3").getValue();
                    SerialNumber = sr0 + "-" + sr1 + "-" + sr2 + "-" + sr3;
                    break;
                }
            }
        }

        var MaxIdleTimeout = ((ArrListOfNetwork[idx].getChildById("RefNetworkDetail.IdleTimeout").getValue() * 1000000) / TimeOutUnit).toFixed(0);
        var NameConfiguration = ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvLinConfig").getValue();

        outputGenCode += "    " + networkname + "{" + NEW_LINE;
        outputGenCode += "        node                      = " + NodeName + ";                   /* Name of node described in LDF (must check consistence with LDF) */" +  NEW_LINE;
        outputGenCode += '        file                      = ""' + ldfPath + '"";               /* Path to LDF file */' +  NEW_LINE;
        outputGenCode += "        device                    = " + PeriphDevice + ";" +  NEW_LINE;
        outputGenCode += "        diagnostic_class          = " + DiagnosticClass + ";" +  NEW_LINE;
        outputGenCode += "        autobaud_support          = " + Autobaud + ";" +  NEW_LINE;
        outputGenCode += "        max_message_length        = " + MaxMessageLength + ";" +  NEW_LINE;
        outputGenCode += '        serial_number             = ""' + SerialNumber + '"";' +  NEW_LINE;
        outputGenCode += "        max_idle_timeout_cnt      = " + MaxIdleTimeout + ";" +  NEW_LINE;
        outputGenCode += "        lin_user_config_t         = " + NameConfiguration + ";" +  NEW_LINE;
        outputGenCode += "        lin_time_out_unit_t       = " + TimeOutUnit + ";" +  NEW_LINE;
        outputGenCode += "    }" + NEW_LINE;
    }
    outputGenCode += "}" + NEW_LINE;

    /* Get linstack component location to run java script */
    if (componentConfig != null) {
        linstackComponentDir = componentConfig.getConfigCompTypeId().getFileLocation();
    }

    var javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-npfProcess", "-create", linstackComponentDir + "\\LinStackCfg.npf", outputGenCode);
    var javaLog = getStreamMessage(javaCommand.start().getInputStream());
    if ((javaLog.search("%error") != -1) || (javaLog.search("%define _ParseError") != -1)){
        scriptApi.logError(javaLog);
    }
}

function copyrightHeader(){
    var outputString = "";

    outputString += "/*\n";
    outputString += " * Copyright 2019 - 2020 NXP\n";
    outputString += " * All rights reserved.\n";
    outputString += " *\n";
    outputString += " * This software is owned or controlled by NXP and may only be\n";
    outputString += " * used strictly in accordance with the applicable license terms. By expressly\n";
    outputString += " * accepting such terms or by downloading, installing, activating and/or otherwise\n";
    outputString += " * using the software, you are agreeing that you have read, and that you agree to\n";
    outputString += " * comply with and are bound by, such license terms. If you do not agree to be\n";
    outputString += " * bound by the applicable license terms, then you may not retain, install,\n";
    outputString += " * activate or otherwise use the software. The production use license in\n";
    outputString += " * Section 2.3 is expressly granted for this software.\n";
    outputString += " */\n";

    return outputString
}

function copyrightMisraCHeader(){
    var outputString = "";

    outputString += "/*!\n"
    outputString += " * @file lin_cfg.c\n"
    outputString += " *\n"
    outputString += " * @page misra_violations MISRA-C:2012 violations\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.\n"
    outputString += " * Function ld_read_by_id_callout is API and shall not be made static.\n"
    outputString += " * This API is declared in lin_cfg.c so that users can configure the response\n"
    outputString += " * for  Read by ID request.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block\n"
    outputString += " * scope if its identifier only appears in a single function.\n"
    outputString += " * An object with static storage duration declared at block scope cannot be\n"
    outputString += " * accessed directly from outside the block. g_lin_hardware_ifc, g_lin_tl_descriptor_array,\n"
    outputString += " * timerGetTimeIntervalCallback are dependent on user configurations,\n"
    outputString += " * so they shall be declared in lin_cfg.c.\n"
    outputString += " */\n"

    return outputString
}

function copyrightMisraHHeader(){
    var outputString = "";

    outputString += "/*!\n"
    outputString += " * @file lin_cfg.h\n"
    outputString += " *\n"
    outputString += " * @page misra_violations MISRA-C:2012 violations\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Rule 2.3, A project should not contain unused type declarations.\n"
    outputString += " * The type can be used by user application.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declarations.\n"
    outputString += " * The macro can be used by user application.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Required Rule 5.1, External identifiers shall be distinct.\n"
    outputString += " * The identifiers are generated from signal names. The signal name is defined by users and we can not control this.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Required Rule 5.2, Identifiers declared in the same scope and name\n"
    outputString += " * space shall be distinct.\n"
    outputString += " * The name of signal is defined by user and we can not control this.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Required Rule 5.4, Macro identifiers shall be distinct.\n"
    outputString += " * The name of signal is defined by user and we can not control this.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Required Rule 5.5, Identifier shall be distinct from macro names.\n"
    outputString += " * The name of signal is defined by user and we can not control this.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Directive 4.9, A function should be used in preference\n"
    outputString += " * to a function-like macro where they are interchangeable.\n"
    outputString += " * The macro is used to define static or dynamic implementation of API functions.\n"
    outputString += " * This way is more efficient.\n"
    outputString += " *\n"
    outputString += " * @section [global]\n"
    outputString += " * Violates MISRA 2012 Advisory Rule 20.10, The #/ ## preprocessor operator should not be used.\n"
    outputString += " * The operator is used to define dynamic implementation of API functions of signal interaction.\n"
    outputString += " * This way is more efficient.\n"
    outputString += " */\n"

    return outputString
}

function linstackCfgHeader(struct){
    var outputString = "";

    /* Get childContext class to extract peripheral name and functional group */
    var componentConfig = struct.getChildContext().getComponentConfig();
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var TimeOutUnit = struct.getChildById("RefNetworkConfig.TimeOutUnit").getValue();
    var InterleaveTimeOut = struct.getChildById("RefNetworkConfig.InterleaveTimeOut").getValue();
    var master = 0;
    var slave = 0;
    var class1 = 0;
    var class2 = 0;
    var class3 = 0;
    var protocol21 = 0;
    var protocol20 = 0;
    var protocol13 = 0;
    var protocolj2602 = 0;
    var transport = 0;
    var diagservice = 0;
    var linstackComponentDir = null;
    var ArrayList = Java.type('java.util.ArrayList');
    var linCompList = new ArrayList();

    if (componentConfig != null) {
        linstackComponentDir = componentConfig.getConfigCompTypeId().getFileLocation();
    }

    if (struct.getChildById("RefNetworkConfig.UseTransportLayer").getValue()){
        transport = 1;
    }

    if (struct.getChildById("RefNetworkConfig.SupportDiagnoticService").getValue()){
        diagservice = 1;
    }

    for (var idx in ArrListOfNetwork){
        linCompList.add(ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvLinDriver").getValue());

        if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == 'MASTER'){
            master = 1;
        }
        else{
            slave = 1;
        }

        if (ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").getValue() == 'Class1'){
            class1 = 1;
        }
        else if (ArrListOfNetwork[idx].getChildById("RefNetworkDetail.RefDiagClass").getValue() == 'Class2'){
            class2 = 1;
        }
        else{
            class3 = 1;
            class2 = 1;
        }

        if (ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeProtocol").getValue() == '2.x'){
            protocol21 = 1;
        }
        else if (ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeProtocol").getValue() == '2.0'){
            protocol20 = 1;
        }
        else if (ArrListOfNetwork[idx].getChildById("RefSupportedService.ServiceNodeProtocol").getValue() == '1.3'){
            protocol13 = 1;
        }
        else{
            protocolj2602 = 1;
        }
    }

    outputString += copyrightHeader();
    outputString += copyrightMisraHHeader();
    outputString += "\n";

    outputString += "#ifndef LIN_CFG_H\n";
    outputString += "#define LIN_CFG_H\n";
    outputString += "\n";

    outputString += "#include <stdint.h>\n";
    outputString += "#include \"lin_driver.h\"\n";
    for (var idx = 0; idx < linCompList.length; idx++){
        outputString += "#include \"peripherals_"+ linCompList[idx] +".h\"\n";
    }
    outputString += "\n";

    outputString += "/* Implements    : SUPPORT_MASTER_MODE_Class */\n"
    outputString += "#define SUPPORT_MASTER_MODE     " + master + "U " + "/* Equal to 1 when have at least one master interface */\n"
    outputString += "/* Implements    : SUPPORT_SLAVE_MODE_Class */\n"
    outputString += "#define SUPPORT_SLAVE_MODE      " + slave + "U " + "/* Equal to 1 when have at least one slave interface */\n"
    outputString += "\n"
    outputString += "/* Implements    : SUPPORT_PROTOCOL_21_Class */\n"
    outputString += "#define SUPPORT_PROTOCOL_21     " + protocol21 + "U " + "/* Equal to 1 when have at least one LIN2.1 or LIN2.2 interface */\n"
    outputString += "/* Implements    : SUPPORT_PROTOCOL_20_Class */\n"
    outputString += "#define SUPPORT_PROTOCOL_20     " + protocol20 + "U " + "/* Equal to 1 when have at least one LIN2.0 interface */\n"
    outputString += "/* Implements    : SUPPORT_PROTOCOL_13_Class */\n"
    outputString += "#define SUPPORT_PROTOCOL_13     " + protocol13 + "U " + "/* Equal to 1 when have at least one LIN1.3 interface */\n"
    outputString += "/* Implements    : SUPPORT_PROTOCOL_J2602_Class */\n"
    outputString += "#define SUPPORT_PROTOCOL_J2602  " + protocolj2602 + "U " + "/* Equal to 1 when have at least one J2602 interface */\n"
    outputString += "\n"
    outputString += "/* Implements    : SUPPORT_DIAG_CLASS_III_Class */\n"
    outputString += "#define SUPPORT_DIAG_CLASS_III  " + class3 + "U " + "/* Equal to 1 when have at least one ifc with diagnostic class III */\n"
    outputString += "/* Implements    : SUPPORT_DIAG_CLASS_II_Class */\n"
    outputString += "#define SUPPORT_DIAG_CLASS_II   " + class2 + "U " + "/* Equal to 1 when have at least one ifc with diagnostic class II or III */\n"
    outputString += "\n"
    outputString += "/* Implements    : SUPPORT_TRANSPORT_LAYER_Class */\n"
    outputString += "#define SUPPORT_TRANSPORT_LAYER " + transport + "U " + "/* Equal to 1 to use transport layer */\n"
    outputString += "/* Implements    : SUPPORT_DIAG_SERVICE_Class */\n"
    outputString += "#define SUPPORT_DIAG_SERVICE    " + diagservice + "U " + "/* Equal to 1 to use diagnostic service layer */\n"
    outputString += "#define TIME_OUT_UNIT_US        " + TimeOutUnit +"U\n"
    if (0 != master)
    {
        InterleaveTimeOut = ((InterleaveTimeOut * 1000) / TimeOutUnit).toFixed(0);
        outputString += "#define INTERLEAVE_MAX_TIMEOUT  (l_u16)" + InterleaveTimeOut + "U " + "/* Interleaved diagnostic response counter timeout (Interleave Time Out(ms) * 1000 / Time Out Unit(us)) */\n"
    }

    var javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-npf", linstackComponentDir + "\\LinStackCfg.npf", "-o", "cfgHeader", "-u", "header");
    var javaLog = getStreamMessage(javaCommand.start().getInputStream());
    if ((javaLog.search("%error") != -1) || (javaLog.search("%define _ParseError") != -1)){
        scriptApi.logError(javaLog);
    }
    else
    {
        outputString += NEW_LINE + javaLog + NEW_LINE;
    }
    outputString += defineDiagService(struct) + "\n";

    outputString += "\n";
    outputString += "#endif /* LIN_CFG_H_ */";

    return outputString
}

function linstackCfgSource(struct){
    var outputString = "";
    var componentConfig = struct.getChildContext().getComponentConfig();
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var TimeOutUnit = struct.getChildById("RefNetworkConfig.TimeOutUnit").getValue();

    outputString += copyrightHeader();
    outputString += copyrightMisraCHeader();
    outputString += "\n";

    outputString += "#include \"lin.h\"\n";

    var linstackComponentDir = null;
    if (componentConfig != null) {
        linstackComponentDir = componentConfig.getConfigCompTypeId().getFileLocation();
    }

    if(struct != null)
    {
        outputString += arrayDiagService(struct);
        var javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-npf", linstackComponentDir + "\\LinStackCfg.npf", "-o", "cfgSource", "-u", "source");
        var javaLog = getStreamMessage(javaCommand.start().getInputStream());
        if ((javaLog.search("%error") != -1) || (javaLog.search("%define _ParseError") != -1)){
            scriptApi.logError(javaLog);
        }
        else{
            outputString += javaLog;
        }
        javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-npfProcess", "-delete", linstackComponentDir + "\\LinStackCfg.npf");
        var javaLog = getStreamMessage(javaCommand.start().getInputStream());
        if ((javaLog.search("%error") != -1) || (javaLog.search("%define _ParseError") != -1)){
            scriptApi.logError(javaLog);
        }
    }

    return outputString
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

function LINSTACKCodeGen(uiConfig) {
    this.uiConfig = uiConfig;

    /* Returns the class name */
    this.toString = function () {
        return "LINSTACKCodeGen";
    }

    this.genHCfgFile = function(hCfgFileSections) {
        var profile = scriptApi.getProfile();
        var functionalGroups = profile.getFunctionalGroups();
        var childContext = uiConfig.getChildContext();
        var coreId = childContext.getFunctionalGroup().getCore();
        var initStruct = getSetting(this.uiConfig, this.uiConfig.getName()+".linstackCfg");

        checkListOfSrNum(initStruct);
        convertToHex(initStruct);
        getLinComponent(initStruct);
        setHideField(initStruct);
        setSupportedService(initStruct);

        /* Check error information in each network interface */
        var error_info = validateLdfFile(initStruct);

        /* Generate confirguration when make sure do not error */
        if (error_info == ""){
            /* Create npf file to generate configuration file and will be deleted when generate done*/
            genNPF(initStruct);
            /* Generate lin_cfg.h and lin_cfg.c */
            genCustomHeaderFile("lin_cfg", linstackCfgHeader(initStruct), coreId);
            genCustomSourceFile("lin_cfg", linstackCfgSource(initStruct), coreId);
        }
        else{
            scriptApi.logError(error_info);
        }
    }

    /* Adds component specific info to header file sections */
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
}
    this.genCFile = function(cFileSections) {
        cFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced\n \* Variable is referenced in a different file by the user.\n *\n";
        try{
            cFileSections[GLOBAL_VARS_SECTION] += "typedef int make_iso_compilers_happy;"
        } catch(err) {
            scriptApi.logInfo(err);
        }
    }
}
