/*
 * Copyright 2019 NXP
 * To be used with S32 Configuration Too. See Software License Agreement of S32 Configuration Tool.
 */

scriptApi.requireScript("codegeneratorUtils.js");

// AMMCLIB component code generator class.
function ammclib_CodeGenerator(uiConfig) {
    this.uiConfig = uiConfig;

    // Returns the class name.
    this.toString = function () {
        return "ammclib_CodeGenerator";
    }

    // Adds component specific info to header file sections.
    this.genHFile = function(hFileSections) {
        hFileSections[MISRA_COMMENTS] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.\n \* The global macro will be used in function call of the module.\n *\n";
        try {
            // #include "SWLIBS_Config.h"
            // #include "amclib.h"
            // #include "adflib.h"
            // #include "gdflib.h"
            // #include "gflib.h"
            // #include "gmclib.h"
            // #include "mlib.h"
            hFileSections[INCLUDES_SECTION] += addInclude("SWLIBS_Config.h");
            hFileSections[INCLUDES_SECTION] += addInclude("amclib.h");
            hFileSections[INCLUDES_SECTION] += addInclude("gdflib.h");
            hFileSections[INCLUDES_SECTION] += addInclude("gflib.h");
            hFileSections[INCLUDES_SECTION] += addInclude("gmclib.h");
            hFileSections[INCLUDES_SECTION] += addInclude("mlib.h");

            /* Generate SWLIBS_Config.h file needed by the AMMCLib */
            ammclib_genFile("SWLIBS_Config.h", uiConfig);

        } catch (error) {
            scriptApi.logError(error);
        }
    }

    // Adds component specific info to source file sections.
    this.genCFile = function(cFileSections)
    {
        cFileSections[MISRA_COMMENTS_C] = " * @section [global]\n * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.\n * The external variables will be used in other source files in application code.\n *\n";
        try{
            cFileSections[GLOBAL_VARS_SECTION] += "/*" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " * This file is generated automatically by AMMCLib component" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " * and is intentionally left empty" + NEW_LINE;
            cFileSections[GLOBAL_VARS_SECTION] += " */";
        } catch(err) {
            scriptApi.logError(err);
        }
    }
}

/* Function used to generate SWLIBS_Config.h file needed by the AMMCLib */
function ammclib_genFile(fileName, uiConfig)
{
    var coreID = uiConfig.getChildContext().getFunctionalGroup().getCore();
    var fixedPoint = getSetting(uiConfig, uiConfig.getName()+".ammclib_fixedPoint").getValue()?"ON":"OFF";
    var floatEn = getSetting(uiConfig, uiConfig.getName()+".ammclib_float").getValue()?"ON":"OFF";
    var defaultImpl = getSetting(uiConfig, uiConfig.getName()+".ammclib_defaultImpl").getValue();
    var profile = scriptApi.getProfile();
    var cpu = profile.getMcuInfo().getPartNumber();

    var outputString = ammclibHeaderPart1;
    outputString += "#define SWLIBS_SUPPORT_F32    SWLIBS_STD_" + fixedPoint + "   /**< Enables/disables support of 32-bit fractional implementation. */\n";
    outputString += ammclibHeaderPart1_1;
    outputString += "#define SWLIBS_SUPPORT_F16    SWLIBS_STD_" + fixedPoint + "   /**< Enables/disables support of 16-bit fractional implementation. */\n";
    if ((cpu.substring(0, 4) == "S32V") ) {
      outputString += ammclibHeaderPart2;
      outputString += "  #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_" + floatEn + "   /**< Enables/disables support of single precision floating point implementation. */\n";
      outputString += ammclibHeaderPart3;
      outputString += "  #define SWLIBS_DEFAULT_IMPLEMENTATION    " + defaultImpl + "   /**< Selection of default implementation. */\n";
      outputString += ammclibHeaderPart4;
    }
    else {
      outputString += ammclibHeaderPart2_1;
      outputString += "#define SWLIBS_SUPPORT_FLT    SWLIBS_STD_" + floatEn + "   /**< Enables/disables support of single precision floating point implementation. */\n";
      outputString += ammclibHeaderPart3_1;
      outputString += "  #define SWLIBS_DEFAULT_IMPLEMENTATION    " + defaultImpl + "   /**< Selection of default implementation. */\n";
      outputString += ammclibHeaderPart4_1;
    }

    new OutputFile(fileName, coreID).close(outputString);
}

{
var ammclibHeaderPart1 ="\
/******************************************************************************\n\
\*\n\
\*   Copyright 2013-2015 Freescale Semiconductor\n\
\*   Copyright 2016-2019 NXP\n\
\*\n\
\*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in\n\
\*   accordance with the applicable license terms.  By expressly accepting such terms or by\n\
\*   downloading, installing, activating and/or otherwise using the software, you are agreeing that\n\
\*   you have read, and that you agree to comply with and are bound by, such license terms.  If you\n\
\*   do not agree to be bound by the applicable license terms, then you may not retain, install,\n\
\*   activate or otherwise use the software.\n\
\*\n\
\******************************************************************************/\n\
\/**\n\
\*\n\
\* @file     SWLIBS_Config.h\n\
\*\n\
\* @version  1.0.34.0\n\
\*\n\
\* @date     17-April-2019\n\
\*\n\
\* @brief    AMMCLIB configuration header file.\n\
\*\n\
\******************************************************************************/\n\
\n\
\/**\n\
\* @page misra_violations MISRA-C:2004 violations\n\
\* \n\
\* @section SWLIBS_Config_h_REF_1\n\
\* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro. \n\
\* To allow the user utilize the benefits of using all three supported implementation of each \n\
\* function in user application, the macro dispatcher inevitably requires use of disallowed macro \n\
\* definition. \n\
\* \n\
\* @section SWLIBS_Config_h_REF_2\n\
\* Violates MISRA 2004 Required Rule 5.1, Identifier exceeds 31 characters. \n\
\* This is not a violation since all the used compilers interpret the identifiers correctly.\n\
\*\n\
\* @section SWLIBS_Config_h_REF_3\n\
\* Violates MISRA 2004 Required Rule 1.4, Identifier clash. \n\
\* This is not a violation since all the used compilers interpret the identifiers correctly.\n\
\*/\n\
\n\
\#ifndef SWLIBS_CONFIG_H\n\
\#define SWLIBS_CONFIG_H\n\
\n\
\/******************************************************************************\n\
\* Library version definition (format <major number>.<minor number>.<build number>)\n\
\******************************************************************************/\n\
\#define SWLIBS_VERSION {(unsigned char)1U,(unsigned char)1U,(unsigned char)21U}\n\
\n\
/******************************************************************************\n\
* Global definitions\n\
******************************************************************************/\n\
#define SWLIBS_STD_ON    0x01U\n\
#define SWLIBS_STD_OFF   0x00U\n\
\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n\
#define F32 F32\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n\
#define F16 F16\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n\
#define FLT FLT\n\
\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 \n\
* characters.\n\
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash. \n\
*/\n\
#define SWLIBS_DEFAULT_IMPLEMENTATION_F32   (1U) \n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 \n\
* characters. \n\
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash.\n\
*/\n\
#define SWLIBS_DEFAULT_IMPLEMENTATION_F16   (2U)\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_2 MISRA 2004 Required Rule 5.1, Identifier exceeds 31 \n\
* characters. \n\
* @violates @ref SWLIBS_Config_h_REF_3 MISRA 2004 Required Rule 1.4, Identifier clash.\n\
*/\n\
#define SWLIBS_DEFAULT_IMPLEMENTATION_FLT   (3U)\n\
\n\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n\
/******************************************************************************\n\
* Implementation supported\n\
******************************************************************************/\n";
var ammclibHeaderPart1_1 = "\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n";
var ammclibHeaderPart2 = "\
#if defined(__ghs__)\n\
  #if defined(__EFP_APU__)\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro. \n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_ON   /**< Enables/disables support of single precision floating point implementation. */\n\
  #else\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro. \n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_OFF   /**< Enables/disables support of single precision floating point implementation. */\n\
  #endif\n\
#elif defined(__DCC__)\n\
  #if defined(__nofp)\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro. \n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_OFF   /**< Enables/disables support of single precision floating point implementation. */\n\
  #else\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro. \n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_ON   /**< Enables/disables support of single precision floating point implementation. */\n\
  #endif\n\
#elif ((defined(__GNUC__)) && (defined(__PPC_EABI__)))\n\
  #if defined(_SOFT_FLOAT)\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro. \n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_OFF   /**< Enables/disables support of single precision floating point implementation. */\n\
  #else\n\
    /*\n\
    * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
    * macro.\n\
    */\n\
    #define SWLIBS_SUPPORT_FLT    SWLIBS_STD_ON   /**< Enables/disables support of single precision floating point implementation. */\n\
  #endif\n\
#else \n\
  /*\n\
  * @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
  * macro. \n\
  */\n";
var ammclibHeaderPart2_1 = "\
/*\n\
* @violates @ref SWLIBS_Config_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for \n\
* macro. \n\
*/\n";
var ammclibHeaderPart3 = "\
#endif\n\
\n\
#define SWLIBS_SUPPORTED_IMPLEMENTATION {SWLIBS_SUPPORT_F32,\\\n\
                                         SWLIBS_SUPPORT_F16,\\\n\
                                         SWLIBS_SUPPORT_FLT,\\\n\
                                         0,0,0,0,0,0} /**< Array of supported implementations. */\n\
\n\
/******************************************************************************\n\
* Selection of default implementation\n\
******************************************************************************/\n\
#ifndef SWLIBS_DEFAULT_IMPLEMENTATION\n";
var ammclibHeaderPart3_1 = "\
\n\
#define SWLIBS_SUPPORTED_IMPLEMENTATION {SWLIBS_SUPPORT_F32,\\\n\
                                         SWLIBS_SUPPORT_F16,\\\n\
                                         SWLIBS_SUPPORT_FLT,\\\n\
                                         0,0,0,0,0,0} /**< Array of supported implementations. */\n\
\n\
/******************************************************************************\n\
* Selection of default implementation\n\
******************************************************************************/\n\
#ifndef SWLIBS_DEFAULT_IMPLEMENTATION\n";
var ammclibHeaderPart4 ="\
#endif /* SWLIBS_DEFAULT_IMPLEMENTATION */\n\
\n\
\n\
#endif /* SWLIBS_CONFIG_H */\n\
";
var ammclibHeaderPart4_1 ="\
  #ifndef SWLIBS_DEFAULT_IMPLEMENTATION\n\
    #error \"Please select the default implementation in the file SWLIBS_Config.h.\"\n\
  #else\n\
    #if((SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_F32)&& \\\n\
        (SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_F16)&& \\\n\
        (SWLIBS_DEFAULT_IMPLEMENTATION != SWLIBS_DEFAULT_IMPLEMENTATION_FLT))\n\
      #error \"Please select the default implementation in the file SWLIBS_Config.h.\"\n\
    #endif\n\
  #endif /* SWLIBS_DEFAULT_IMPLEMENTATION */\n\
#endif /* SWLIBS_DEFAULT_IMPLEMENTATION */\n\
\n\
/******************************************************************************\n\
* Compiler version check\n\
******************************************************************************/\n\
#ifdef __IAR_SYSTEMS_ICC__\n\
  #if (__VER__ < 8011001)\n\
    #error ***ERROR***: You are using an incompatible version of the IAR compiler. Please update your compiler to the supported version. You can find the list of the supported compilers in the Release Notes.\n\
  #endif\n\
#endif\n\
\n\
#endif /* SWLIBS_CONFIG_H */\n\
";
}