/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
 * To be used with KEx Tools. See Software License Agreement of KEx Tools.
 */

// util functions implemented in the tool
var util = Java.type("com.nxp.swtools.utils.scripting.JSUtils");
// load common objects
scriptApi.requireScript("common/clock_common_lib.js");
// load common helper functions
scriptApi.requireScript("common/clock_output_utils.js");

// create code generator instance 
var Gen = new ClockCodeGen();
// load processor register/configuration specific helper functions
scriptApi.requireScript("clock_hw_abstraction.js");
// read header from separate file
scriptApi.requireScript("common_header.js");
// load sdk mapping functions
scriptApi.requireScript("clock_sdk_mapper.js");

// check configurations and log errors, warnings, hints
HwAbstr.checkConfigurations();
// check settings which should be the same accross all configurations and log errors, warnings, hints
HwAbstr.checkGeneralSettings();

// load general configuration
//scriptApi.requireScript("clock_general_config.js");
// create file instance for c file
var CFile = new OutputFile("clock_config.c");
// create ouput writer instance for c file
var CWriter = new OutputWriter(CFile.output);
// create file instance for h file
var HFile = new OutputFile("clock_config.h");
// create ouput writer instance for h file
var HWriter = new OutputWriter(HFile.output);

// generate h file content
scriptApi.requireScript("clock_config_h.js");
ClockConfigH.generate(HWriter);
// generate c file content
scriptApi.requireScript("clock_config_c.js");

//complete h file content
ClockConfigH.complete();
// write content into h file
HFile.close(HWriter.getContent());
// write content into c file
CFile.close(CWriter.getContent());