/*
 * Copyright 2020 NXP
 * To be used with S32 Configuration Tool. See Software License Agreement of S32 Configuration Tool.
 */

function runLDFProcess(uiConfig){
    var configSet = child.getChildContext().getConfigSetConfig();
    var componentConfig = child.getChildContext().getComponentConfig();
    this.uiConfig = uiConfig;

    var initStruct = configSet.getChildById("linstackCfg");
    var ArrListOfNetwork = configSet.getChildById("linstackCfg.RefNetworkConfig.ArrListOfNetwork").getChildren();
    var ArrayList = Java.type('java.util.ArrayList');
    var response = new ArrayList();
    var temp = "";

    /* Validate ldf file and get ldf information */
    LDFReading(initStruct);

    for(var idx in ArrListOfNetwork){
        var ldfInfo = ArrListOfNetwork[idx].getChildById("StrLDFInfo").getValue();
        var nodeArray = new ArrayList();
        if (ldfInfo.length != 0){
            /* Get Node name */
            /* Get all slave node name if node function is slave node */
            if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == "SLAVE"){
                temp = ldfInfo.slice(ldfInfo.search("Slave"));
                var temp = temp.substring(temp.search(" ") + 1, temp.search(";"));
                var slaveNum = parseInt(temp);
                for (var idx1 = 0; idx1 < slaveNum; idx1++){
                    temp = ldfInfo.slice(ldfInfo.search("SlaveName" + idx1));
                    nodeArray.add(temp.substring(temp.search(" ") + 1, temp.search(";")));
                }
            }
            /* Get master node name if node function is master node */
            else if (ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvNodeFunc").getValue() == "MASTER"){
                temp = ldfInfo.slice(ldfInfo.search("MasterName"));
                nodeArray.add(temp.substring(temp.search(" ") + 1, temp.search(";")));
            }
            /* Get all node name if don't know node function */
            else{
                temp = ldfInfo.slice(ldfInfo.search("MasterName"));
                nodeArray.add(temp.substring(temp.search(" ") + 1, temp.search(";")));
                temp = ldfInfo.slice(ldfInfo.search("Slave"));
                var temp = temp.substring(temp.search(" ") + 1, temp.search(";"));
                var slaveNum = parseInt(temp);
                for (var idx1 = 0; idx1 < slaveNum; idx1++){
                    temp = ldfInfo.slice(ldfInfo.search("SlaveName" + idx1));
                    nodeArray.add(temp.substring(temp.search(" ") + 1, temp.search(";")));
                }
            }
        }
        else
        {
            nodeArray.add("");
        }
        response.add(nodeArray);
        ArrListOfNetwork[idx].getChildById("RefNetworkDetail.NetworkNum").setValue(idx);
    }
    return response;
}

function LDFReading(struct){
    var componentConfig = child.getChildContext().getComponentConfig();
    var ArrListOfNetwork = struct.getChildById("RefNetworkConfig.ArrListOfNetwork").getChildren();
    var temp = "";
    var error_info = "";
    for (var idx in ArrListOfNetwork){
        var ldfInfo = "";
        var ldfPath = "";
        /* Clean error information before read ldf file */
        ArrListOfNetwork[idx].getChildById("RefNetworkDetail.StrErrorInfo").setValue("");
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
        var linstackComponentDir = null;
        /* Get component directory path which contain ncf tool (java file) */
        if (componentConfig != null) {
            linstackComponentDir = componentConfig.getConfigCompTypeId().getFileLocation();
        }

        /* Validate ldf file and get errors if avaiable */
        var javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-ldfProcess", "-validate", ldfPath, "LDF0");
        var p = javaCommand.start();
        var javaLog = p.getInputStream();
        var inReader = new java.io.BufferedReader(new java.io.InputStreamReader(javaLog));
        var inputLine;

        while ((inputLine = inReader.readLine()) != null){
            if (inputLine.search("error"))
            {
                error_info += inputLine + "\n";
            }
        }
        inReader.close();
        p.destroy();
        /* Get information from ldf file if have not errors */
        if (error_info == ""){
            javaCommand = new java.lang.ProcessBuilder("java", "-jar", linstackComponentDir + "\\ParseGenerate.jar", "-ldfProcess", "-getLDF", ldfPath);
            var p = javaCommand.start();
            var javaLog = p.getInputStream();
            inReader = new java.io.BufferedReader(new java.io.InputStreamReader(javaLog));
            while ((inputLine = inReader.readLine()) != null) 
            {
                ldfInfo += inputLine + "; ";
            }
            inReader.close();
            p.destroy();
            /* Return LDF information into ldfInfo*/
            ArrListOfNetwork[idx].getChildById("StrLDFInfo").setValue(ldfInfo);

            /* Get Master Protocol Version */
            temp = ldfInfo.slice(ldfInfo.search("ProtocolVersion"));
            ArrListOfNetwork[idx].getChildById("RefNetworkDetail.ProtocolVersion").setValue(temp.substring(temp.search(" ") + 1, temp.search(";")));
            /* Get LIN Baudrate */
            temp = ldfInfo.slice(ldfInfo.search("LinDrvBaudRateConfig"));
            ArrListOfNetwork[idx].getChildById("RefLinDriverConfig.LinDrvBaudRateConfig").setValue(temp.substring(temp.search(" ") + 1, temp.search(";")));
        }
        /* Write error information into StrErrorInfo */
        ArrListOfNetwork[idx].getChildById("RefNetworkDetail.StrErrorInfo").setValue(error_info);
        error_info = "";
    }
}

runLDFProcess();
