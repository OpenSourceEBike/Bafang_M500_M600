/**
 * AMCLIB_BemfObsrvDQ_SF_FLT.c
 *
 *    ABSTRACT:
 *      The purpose of this sfunction is to call a simple legacy
 *      function during simulation:
 *
 *         single y1 = AMCLIB_BemfObsrvDQ_w_FLT(SWLIBS_2Syst_FLT u1[1], SWLIBS_2Syst_FLT u2[1], single u3, single u4, AMCLIB_BEMF_OBSRV_DQ_T_FLT work1[1], int32 work2[1], single u5, single u6, single u7, single u8, single u9, single u10, single u11, single u12, single u13, single u14, single u15, single u16, single u17, single u18, single u19, single u20, single u21, single u22, single u23, single u24, single y2[1], single y3[1], single y4[1], single y5[1], single y6[1], single y7[1], single y8[1], single y9[1], single y10[1], single y11[1], int32 p1, int32 p2)
 *
 *    Simulink version           : 9.3 (R2019a) 23-Nov-2018
 *    C source code generated on : 26-Jun-2020 20:15:55
 *
 */

/*  Copyright 2019-2020 NXP
 * 
 *  NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in
 *  accordance with the applicable license terms.  By expressly accepting such terms or by
 *  downloading, installing, activating and/or otherwise using the software, you are agreeing that
 *  you have read, and that you agree to comply with and are bound by, such license terms.  If you
 *  do not agree to be bound by the applicable license terms, then you may not retain, install,
 *  activate or otherwise use the software. */ 

/* Must specify the S_FUNCTION_NAME as the name of the S-function */
#define S_FUNCTION_NAME  AMCLIB_BemfObsrvDQ_SF_FLT
#define S_FUNCTION_LEVEL 2

/**
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#include <string.h>
#include <stdlib.h>

/* Specific header file(s) required by the legacy code function */
#include "amclib.h"
#include "AMCLIB_BemfObsrvDQ_w.h"
#include "gflib.h"
#include "gmclib.h"

/* Specific header file(s) required for data types declarations */
#include "SWLIBS_Typedefs.h"
#include "GFLIB_ControllerPIrAW.h"
#include "AMCLIB_BemfObsrvDQ.h"

#define EDIT_OK(S, P_IDX) \
       (!((ssGetSimMode(S)==SS_SIMMODE_SIZES_CALL_ONLY) && mxIsEmpty(ssGetSFcnParam(S, P_IDX))))

#define IS_SIMULATION_TARGET(S) (ssRTWGenIsAccelerator(S) || ssIsRapidAcceleratorActive(S) || ssRTWGenIsModelReferenceSimTarget(S) || (ssGetSimMode(S)==SS_SIMMODE_NORMAL) || (ssGetSimMode(S)==SS_SIMMODE_SIZES_CALL_ONLY) || !((ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) && GetRTWEnvironmentMode(S)==0))


/* Utility function prototypes */
static void CheckDataTypes(SimStruct *S);
static int_T GetRTWEnvironmentMode(SimStruct *S);

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
/* Function: mdlCheckParameters ===========================================
 * Abstract:
 *   mdlCheckParameters verifies new parameter settings whenever parameter
 *   change or are re-evaluated during a simulation. When a simulation is
 *   running, changes to S-function parameters can occur at any time during
 *   the simulation loop.
 */
static void mdlCheckParameters(SimStruct *S)
{
    /* Check the parameter 1 */
    if (EDIT_OK(S, 0)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 0, "P1", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 2 */
    if (EDIT_OK(S, 1)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 1, "P2", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 3 */
    if (EDIT_OK(S, 2)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 2, "P3", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 4 */
    if (EDIT_OK(S, 3)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 3, "P4", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 5 */
    if (EDIT_OK(S, 4)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 4, "P5", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 6 */
    if (EDIT_OK(S, 5)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 5, "P6", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 7 */
    if (EDIT_OK(S, 6)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 6, "P7", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 8 */
    if (EDIT_OK(S, 7)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 7, "P8", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 9 */
    if (EDIT_OK(S, 8)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 8, "P9", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 10 */
    if (EDIT_OK(S, 9)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 9, "P10", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 11 */
    if (EDIT_OK(S, 10)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 10, "P11", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 12 */
    if (EDIT_OK(S, 11)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 11, "P12", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 13 */
    if (EDIT_OK(S, 12)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 12, "P13", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

    /* Check the parameter 14 */
    if (EDIT_OK(S, 13)) {
        int_T dimsArray[2] = {1, 1};

        /* Check the parameter attributes */
        ssCheckSFcnParamValueAttribs(S, 13, "P14", DYNAMICALLY_TYPED, 2, dimsArray, 0);
    } /* if */

}
#endif

#define MDL_PROCESS_PARAMETERS
#if defined(MDL_PROCESS_PARAMETERS) && defined(MATLAB_MEX_FILE)
/* Function: mdlProcessParameters =========================================
 * Abstract:
 *   Update run-time parameters.
 */
static void mdlProcessParameters(SimStruct *S)
{
    ssUpdateAllTunableParamsAsRunTimeParams(S);
}
#endif

/* Function: mdlInitializeSizes ===========================================
 * Abstract:
 *   The sizes information is used by Simulink to determine the S-function
 *   block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    /* Flag for detecting standalone or simulation target mode */
    boolean_T isSimulationTarget = IS_SIMULATION_TARGET(S);

    /* Number of expected parameters */
    ssSetNumSFcnParams(S, 14);

    #if defined(MATLAB_MEX_FILE) 
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        /**
         * If the number of expected input parameters is not equal
         * to the number of parameters entered in the dialog box return.
         * Simulink will generate an error indicating that there is a
         * parameter mismatch.
         */
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) return;
    } else {
        /* Return if number of expected != number of actual parameters */
        return;
    } /* if */
    #endif 

    /* Set the parameter's tunability */
    ssSetSFcnParamTunable(S, 0, 1);
    ssSetSFcnParamTunable(S, 1, 1);
    ssSetSFcnParamTunable(S, 2, 1);
    ssSetSFcnParamTunable(S, 3, 1);
    ssSetSFcnParamTunable(S, 4, 1);
    ssSetSFcnParamTunable(S, 5, 1);
    ssSetSFcnParamTunable(S, 6, 1);
    ssSetSFcnParamTunable(S, 7, 1);
    ssSetSFcnParamTunable(S, 8, 1);
    ssSetSFcnParamTunable(S, 9, 1);
    ssSetSFcnParamTunable(S, 10, 1);
    ssSetSFcnParamTunable(S, 11, 1);
    ssSetSFcnParamTunable(S, 12, 1);
    ssSetSFcnParamTunable(S, 13, 1);

    /* Set the number of work vectors */
    if (!IS_SIMULATION_TARGET(S)) {
        ssSetNumPWork(S, 0);
        if (!ssSetNumDWork(S, 2)) return;
    } else {
        ssSetNumPWork(S, 3);
        if (!ssSetNumDWork(S, 4)) return;

        /* Configure the dwork 3 (__dtSizeInfo) */
        ssSetDWorkDataType(S, 2, SS_INT32);
        ssSetDWorkUsageType(S, 2, SS_DWORK_USED_AS_DWORK);
        ssSetDWorkName(S, 2, "dtSizeInfo");
        ssSetDWorkWidth(S, 2, 4);
        ssSetDWorkComplexSignal(S, 2, COMPLEX_NO);

        /* Configure the dwork 4 (__dtBusInfo) */
        ssSetDWorkDataType(S, 3, SS_INT32);
        ssSetDWorkUsageType(S, 3, SS_DWORK_USED_AS_DWORK);
        ssSetDWorkName(S, 3, "dtBusInfo");
        ssSetDWorkWidth(S, 3, 48);
        ssSetDWorkComplexSignal(S, 3, COMPLEX_NO);
    } /* if */

    /* Configure the dwork 1 (work1) */
    #if defined(MATLAB_MEX_FILE) 
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "AMCLIB_BEMF_OBSRV_DQ_T_FLT", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssSetDWorkDataType(S, 0, dataTypeIdReg);
    } /* if */
    #endif
    ssSetDWorkUsageType(S, 0, SS_DWORK_USED_AS_DWORK);
    ssSetDWorkName(S, 0, "work1");
    ssSetDWorkWidth(S, 0, 1);
    ssSetDWorkComplexSignal(S, 0, COMPLEX_NO);

    /* Configure the dwork 2 (work2) */
    ssSetDWorkDataType(S, 1, SS_INT32);
    ssSetDWorkUsageType(S, 1, SS_DWORK_USED_AS_DWORK);
    ssSetDWorkName(S, 1, "work2");
    ssSetDWorkWidth(S, 1, 1);
    ssSetDWorkComplexSignal(S, 1, COMPLEX_NO);

    /* Set the number of input ports */
    if (!ssSetNumInputPorts(S, 24)) return;

    /* Configure the input port 1 */
    #if defined(MATLAB_MEX_FILE) 
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "SWLIBS_2Syst_FLT", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssSetInputPortDataType(S, 0, dataTypeIdReg);
    } /* if */
    #endif
    ssSetBusInputAsStruct(S, 0, 1);
    {
        int_T u1Width = 1;
        ssSetInputPortWidth(S, 0, u1Width);
    }
    ssSetInputPortComplexSignal(S, 0, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortAcceptExprInRTW(S, 0, 0);
    ssSetInputPortOverWritable(S, 0, 0);
    ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 0, 1);

    /* Configure the input port 2 */
    #if defined(MATLAB_MEX_FILE) 
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "SWLIBS_2Syst_FLT", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssSetInputPortDataType(S, 1, dataTypeIdReg);
    } /* if */
    #endif
    ssSetBusInputAsStruct(S, 1, 1);
    {
        int_T u2Width = 1;
        ssSetInputPortWidth(S, 1, u2Width);
    }
    ssSetInputPortComplexSignal(S, 1, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortAcceptExprInRTW(S, 1, 0);
    ssSetInputPortOverWritable(S, 1, 0);
    ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 1, 1);

    /* Configure the input port 3 */
    ssSetInputPortDataType(S, 2, SS_SINGLE);
    {
        int_T u3Width = 1;
        ssSetInputPortWidth(S, 2, u3Width);
    }
    ssSetInputPortComplexSignal(S, 2, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortAcceptExprInRTW(S, 2, 1);
    ssSetInputPortOverWritable(S, 2, 1);
    ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 2, 1);

    /* Configure the input port 4 */
    ssSetInputPortDataType(S, 3, SS_SINGLE);
    {
        int_T u4Width = 1;
        ssSetInputPortWidth(S, 3, u4Width);
    }
    ssSetInputPortComplexSignal(S, 3, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    ssSetInputPortAcceptExprInRTW(S, 3, 1);
    ssSetInputPortOverWritable(S, 3, 1);
    ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 3, 1);

    /* Configure the input port 5 */
    ssSetInputPortDataType(S, 4, SS_SINGLE);
    {
        int_T u5Width = 1;
        ssSetInputPortWidth(S, 4, u5Width);
    }
    ssSetInputPortComplexSignal(S, 4, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 4, 1);
    ssSetInputPortAcceptExprInRTW(S, 4, 1);
    ssSetInputPortOverWritable(S, 4, 1);
    ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 4, 1);

    /* Configure the input port 6 */
    ssSetInputPortDataType(S, 5, SS_SINGLE);
    {
        int_T u6Width = 1;
        ssSetInputPortWidth(S, 5, u6Width);
    }
    ssSetInputPortComplexSignal(S, 5, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 5, 1);
    ssSetInputPortAcceptExprInRTW(S, 5, 1);
    ssSetInputPortOverWritable(S, 5, 1);
    ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 5, 1);

    /* Configure the input port 7 */
    ssSetInputPortDataType(S, 6, SS_SINGLE);
    {
        int_T u7Width = 1;
        ssSetInputPortWidth(S, 6, u7Width);
    }
    ssSetInputPortComplexSignal(S, 6, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 6, 1);
    ssSetInputPortAcceptExprInRTW(S, 6, 1);
    ssSetInputPortOverWritable(S, 6, 1);
    ssSetInputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 6, 1);

    /* Configure the input port 8 */
    ssSetInputPortDataType(S, 7, SS_SINGLE);
    {
        int_T u8Width = 1;
        ssSetInputPortWidth(S, 7, u8Width);
    }
    ssSetInputPortComplexSignal(S, 7, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 7, 1);
    ssSetInputPortAcceptExprInRTW(S, 7, 1);
    ssSetInputPortOverWritable(S, 7, 1);
    ssSetInputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 7, 1);

    /* Configure the input port 9 */
    ssSetInputPortDataType(S, 8, SS_SINGLE);
    {
        int_T u9Width = 1;
        ssSetInputPortWidth(S, 8, u9Width);
    }
    ssSetInputPortComplexSignal(S, 8, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 8, 1);
    ssSetInputPortAcceptExprInRTW(S, 8, 1);
    ssSetInputPortOverWritable(S, 8, 1);
    ssSetInputPortOptimOpts(S, 8, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 8, 1);

    /* Configure the input port 10 */
    ssSetInputPortDataType(S, 9, SS_SINGLE);
    {
        int_T u10Width = 1;
        ssSetInputPortWidth(S, 9, u10Width);
    }
    ssSetInputPortComplexSignal(S, 9, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 9, 1);
    ssSetInputPortAcceptExprInRTW(S, 9, 1);
    ssSetInputPortOverWritable(S, 9, 1);
    ssSetInputPortOptimOpts(S, 9, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 9, 1);

    /* Configure the input port 11 */
    ssSetInputPortDataType(S, 10, SS_SINGLE);
    {
        int_T u11Width = 1;
        ssSetInputPortWidth(S, 10, u11Width);
    }
    ssSetInputPortComplexSignal(S, 10, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 10, 1);
    ssSetInputPortAcceptExprInRTW(S, 10, 1);
    ssSetInputPortOverWritable(S, 10, 1);
    ssSetInputPortOptimOpts(S, 10, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 10, 1);

    /* Configure the input port 12 */
    ssSetInputPortDataType(S, 11, SS_SINGLE);
    {
        int_T u12Width = 1;
        ssSetInputPortWidth(S, 11, u12Width);
    }
    ssSetInputPortComplexSignal(S, 11, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 11, 1);
    ssSetInputPortAcceptExprInRTW(S, 11, 1);
    ssSetInputPortOverWritable(S, 11, 1);
    ssSetInputPortOptimOpts(S, 11, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 11, 1);

    /* Configure the input port 13 */
    ssSetInputPortDataType(S, 12, SS_SINGLE);
    {
        int_T u13Width = 1;
        ssSetInputPortWidth(S, 12, u13Width);
    }
    ssSetInputPortComplexSignal(S, 12, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 12, 1);
    ssSetInputPortAcceptExprInRTW(S, 12, 1);
    ssSetInputPortOverWritable(S, 12, 1);
    ssSetInputPortOptimOpts(S, 12, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 12, 1);

    /* Configure the input port 14 */
    ssSetInputPortDataType(S, 13, SS_SINGLE);
    {
        int_T u14Width = 1;
        ssSetInputPortWidth(S, 13, u14Width);
    }
    ssSetInputPortComplexSignal(S, 13, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 13, 1);
    ssSetInputPortAcceptExprInRTW(S, 13, 1);
    ssSetInputPortOverWritable(S, 13, 1);
    ssSetInputPortOptimOpts(S, 13, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 13, 1);

    /* Configure the input port 15 */
    ssSetInputPortDataType(S, 14, SS_SINGLE);
    {
        int_T u15Width = 1;
        ssSetInputPortWidth(S, 14, u15Width);
    }
    ssSetInputPortComplexSignal(S, 14, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 14, 1);
    ssSetInputPortAcceptExprInRTW(S, 14, 1);
    ssSetInputPortOverWritable(S, 14, 1);
    ssSetInputPortOptimOpts(S, 14, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 14, 1);

    /* Configure the input port 16 */
    ssSetInputPortDataType(S, 15, SS_SINGLE);
    {
        int_T u16Width = 1;
        ssSetInputPortWidth(S, 15, u16Width);
    }
    ssSetInputPortComplexSignal(S, 15, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 15, 1);
    ssSetInputPortAcceptExprInRTW(S, 15, 1);
    ssSetInputPortOverWritable(S, 15, 1);
    ssSetInputPortOptimOpts(S, 15, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 15, 1);

    /* Configure the input port 17 */
    ssSetInputPortDataType(S, 16, SS_SINGLE);
    {
        int_T u17Width = 1;
        ssSetInputPortWidth(S, 16, u17Width);
    }
    ssSetInputPortComplexSignal(S, 16, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 16, 1);
    ssSetInputPortAcceptExprInRTW(S, 16, 1);
    ssSetInputPortOverWritable(S, 16, 1);
    ssSetInputPortOptimOpts(S, 16, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 16, 1);

    /* Configure the input port 18 */
    ssSetInputPortDataType(S, 17, SS_SINGLE);
    {
        int_T u18Width = 1;
        ssSetInputPortWidth(S, 17, u18Width);
    }
    ssSetInputPortComplexSignal(S, 17, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 17, 1);
    ssSetInputPortAcceptExprInRTW(S, 17, 1);
    ssSetInputPortOverWritable(S, 17, 1);
    ssSetInputPortOptimOpts(S, 17, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 17, 1);

    /* Configure the input port 19 */
    ssSetInputPortDataType(S, 18, SS_SINGLE);
    {
        int_T u19Width = 1;
        ssSetInputPortWidth(S, 18, u19Width);
    }
    ssSetInputPortComplexSignal(S, 18, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 18, 1);
    ssSetInputPortAcceptExprInRTW(S, 18, 1);
    ssSetInputPortOverWritable(S, 18, 1);
    ssSetInputPortOptimOpts(S, 18, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 18, 1);

    /* Configure the input port 20 */
    ssSetInputPortDataType(S, 19, SS_SINGLE);
    {
        int_T u20Width = 1;
        ssSetInputPortWidth(S, 19, u20Width);
    }
    ssSetInputPortComplexSignal(S, 19, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 19, 1);
    ssSetInputPortAcceptExprInRTW(S, 19, 1);
    ssSetInputPortOverWritable(S, 19, 1);
    ssSetInputPortOptimOpts(S, 19, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 19, 1);

    /* Configure the input port 21 */
    ssSetInputPortDataType(S, 20, SS_SINGLE);
    {
        int_T u21Width = 1;
        ssSetInputPortWidth(S, 20, u21Width);
    }
    ssSetInputPortComplexSignal(S, 20, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 20, 1);
    ssSetInputPortAcceptExprInRTW(S, 20, 1);
    ssSetInputPortOverWritable(S, 20, 1);
    ssSetInputPortOptimOpts(S, 20, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 20, 1);

    /* Configure the input port 22 */
    ssSetInputPortDataType(S, 21, SS_SINGLE);
    {
        int_T u22Width = 1;
        ssSetInputPortWidth(S, 21, u22Width);
    }
    ssSetInputPortComplexSignal(S, 21, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 21, 1);
    ssSetInputPortAcceptExprInRTW(S, 21, 1);
    ssSetInputPortOverWritable(S, 21, 1);
    ssSetInputPortOptimOpts(S, 21, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 21, 1);

    /* Configure the input port 23 */
    ssSetInputPortDataType(S, 22, SS_SINGLE);
    {
        int_T u23Width = 1;
        ssSetInputPortWidth(S, 22, u23Width);
    }
    ssSetInputPortComplexSignal(S, 22, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 22, 1);
    ssSetInputPortAcceptExprInRTW(S, 22, 1);
    ssSetInputPortOverWritable(S, 22, 1);
    ssSetInputPortOptimOpts(S, 22, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 22, 1);

    /* Configure the input port 24 */
    ssSetInputPortDataType(S, 23, SS_SINGLE);
    {
        int_T u24Width = 1;
        ssSetInputPortWidth(S, 23, u24Width);
    }
    ssSetInputPortComplexSignal(S, 23, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 23, 1);
    ssSetInputPortAcceptExprInRTW(S, 23, 1);
    ssSetInputPortOverWritable(S, 23, 1);
    ssSetInputPortOptimOpts(S, 23, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 23, 1);

    /* Set the number of output ports */
    if (!ssSetNumOutputPorts(S, 11)) return;

    /* Configure the output port 1 */
    ssSetOutputPortDataType(S, 0, SS_SINGLE);
    {
        int_T y1Width = 1;
        ssSetOutputPortWidth(S, 0, y1Width);
    }
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 0, 0);

    /* Configure the output port 2 */
    ssSetOutputPortDataType(S, 1, SS_SINGLE);
    {
        int_T y2Width = 1;
        ssSetOutputPortWidth(S, 1, y2Width);
    }
    ssSetOutputPortComplexSignal(S, 1, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 1, 0);

    /* Configure the output port 3 */
    ssSetOutputPortDataType(S, 2, SS_SINGLE);
    {
        int_T y3Width = 1;
        ssSetOutputPortWidth(S, 2, y3Width);
    }
    ssSetOutputPortComplexSignal(S, 2, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 2, 0);

    /* Configure the output port 4 */
    ssSetOutputPortDataType(S, 3, SS_SINGLE);
    {
        int_T y4Width = 1;
        ssSetOutputPortWidth(S, 3, y4Width);
    }
    ssSetOutputPortComplexSignal(S, 3, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 3, 0);

    /* Configure the output port 5 */
    ssSetOutputPortDataType(S, 4, SS_SINGLE);
    {
        int_T y5Width = 1;
        ssSetOutputPortWidth(S, 4, y5Width);
    }
    ssSetOutputPortComplexSignal(S, 4, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 4, 0);

    /* Configure the output port 6 */
    ssSetOutputPortDataType(S, 5, SS_SINGLE);
    {
        int_T y6Width = 1;
        ssSetOutputPortWidth(S, 5, y6Width);
    }
    ssSetOutputPortComplexSignal(S, 5, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 5, 0);

    /* Configure the output port 7 */
    ssSetOutputPortDataType(S, 6, SS_SINGLE);
    {
        int_T y7Width = 1;
        ssSetOutputPortWidth(S, 6, y7Width);
    }
    ssSetOutputPortComplexSignal(S, 6, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 6, 0);

    /* Configure the output port 8 */
    ssSetOutputPortDataType(S, 7, SS_SINGLE);
    {
        int_T y8Width = 1;
        ssSetOutputPortWidth(S, 7, y8Width);
    }
    ssSetOutputPortComplexSignal(S, 7, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 7, 0);

    /* Configure the output port 9 */
    ssSetOutputPortDataType(S, 8, SS_SINGLE);
    {
        int_T y9Width = 1;
        ssSetOutputPortWidth(S, 8, y9Width);
    }
    ssSetOutputPortComplexSignal(S, 8, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 8, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 8, 0);

    /* Configure the output port 10 */
    ssSetOutputPortDataType(S, 9, SS_SINGLE);
    {
        int_T y10Width = 1;
        ssSetOutputPortWidth(S, 9, y10Width);
    }
    ssSetOutputPortComplexSignal(S, 9, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 9, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 9, 0);

    /* Configure the output port 11 */
    ssSetOutputPortDataType(S, 10, SS_SINGLE);
    {
        int_T y11Width = 1;
        ssSetOutputPortWidth(S, 10, y11Width);
    }
    ssSetOutputPortComplexSignal(S, 10, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 10, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 10, 0);

    /* Register reserved identifiers to avoid name conflict */
    if (ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) {

        /* Register reserved identifier for  */
        ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQInit_w_FLT", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

        /* Register reserved identifier for  */
        ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQ_w_FLT", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

        /* Register reserved identifier for wrappers */
        if (isSimulationTarget) {

            /* Register reserved identifier for  */
            ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQInit_w_FLT_wrapper_InitializeConditions", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for  */
            ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQ_w_FLT_wrapper_Output", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for allocating PWork for SimulationTarget */
            ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQ_SF_FLT_wrapper_allocmem", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for freeing PWork for SimulationTarget */
            ssRegMdlInfo(S, "AMCLIB_BemfObsrvDQ_SF_FLT_wrapper_freemem", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
        } /* if */
    } /* if */

    /* This S-function can be used in referenced model simulating in normal mode */
    ssSetModelReferenceNormalModeSupport(S, MDL_START_AND_MDL_PROCESS_PARAMS_OK);

    /* Set the number of sample time */
    ssSetNumSampleTimes(S, 1);

    /* Set the compliance for the operating point save/restore. */
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

    ssSetArrayLayoutForCodeGen(S, SS_ALL);

    /* Set the Simulink version this S-Function has been generated in */
    ssSetSimulinkVersionGeneratedIn(S, "9.3");

    /**
     * All options have the form SS_OPTION_<name> and are documented in
     * matlabroot/simulink/include/simstruc.h. The options should be
     * bitwise or'd together as in
     *    ssSetOptions(S, (SS_OPTION_name1 | SS_OPTION_name2))
     */
    ssSetOptions(S,
        SS_OPTION_USE_TLC_WITH_ACCELERATOR |
        SS_OPTION_CAN_BE_CALLED_CONDITIONALLY |
        SS_OPTION_EXCEPTION_FREE_CODE |
        SS_OPTION_WORKS_WITH_CODE_REUSE |
        SS_OPTION_SFUNCTION_INLINED_FOR_RTW |
        SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME
    );

    /* Verify Data Type consistency with specification */
    #if defined(MATLAB_MEX_FILE)
    if ((ssGetSimMode(S)!=SS_SIMMODE_SIZES_CALL_ONLY)) {
        CheckDataTypes(S);
    } /* if */
    #endif
}

/* Function: mdlInitializeSampleTimes =====================================
 * Abstract:
 *   This function is used to specify the sample time(s) for your
 *   S-function. You must register the same number of sample times as
 *   specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);

    #if defined(ssSetModelReferenceSampleTimeDefaultInheritance)
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
    #endif
}

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
/* Function: mdlSetWorkWidths =============================================
 * Abstract:
 *   The optional method, mdlSetWorkWidths is called after input port
 *   width, output port width, and sample times of the S-function have
 *   been determined to set any state and work vector sizes which are
 *   a function of the input, output, and/or sample times. 
 *   Run-time parameters are registered in this method using methods 
 *   ssSetNumRunTimeParams, ssSetRunTimeParamInfo, and related methods.
 */
static void mdlSetWorkWidths(SimStruct *S)
{
    #if defined(ssSupportsMultipleExecInstances)
    ssSupportsMultipleExecInstances(S, 1);
    #endif

    /* Set number of run-time parameters */
    if (!ssSetNumRunTimeParams(S, 14)) return;

    /* Register the run-time parameter 1 */
    ssRegDlgParamAsRunTimeParam(S, 0, 0, "p1", ssGetDataTypeId(S, "int32"));

    /* Register the run-time parameter 2 */
    ssRegDlgParamAsRunTimeParam(S, 1, 1, "p2", ssGetDataTypeId(S, "int32"));

    /* Register the run-time parameter 3 */
    ssRegDlgParamAsRunTimeParam(S, 2, 2, "p3", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 4 */
    ssRegDlgParamAsRunTimeParam(S, 3, 3, "p4", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 5 */
    ssRegDlgParamAsRunTimeParam(S, 4, 4, "p5", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 6 */
    ssRegDlgParamAsRunTimeParam(S, 5, 5, "p6", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 7 */
    ssRegDlgParamAsRunTimeParam(S, 6, 6, "p7", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 8 */
    ssRegDlgParamAsRunTimeParam(S, 7, 7, "p8", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 9 */
    ssRegDlgParamAsRunTimeParam(S, 8, 8, "p9", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 10 */
    ssRegDlgParamAsRunTimeParam(S, 9, 9, "p10", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 11 */
    ssRegDlgParamAsRunTimeParam(S, 10, 10, "p11", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 12 */
    ssRegDlgParamAsRunTimeParam(S, 11, 11, "p12", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 13 */
    ssRegDlgParamAsRunTimeParam(S, 12, 12, "p13", ssGetDataTypeId(S, "single"));

    /* Register the run-time parameter 14 */
    ssRegDlgParamAsRunTimeParam(S, 13, 13, "p14", ssGetDataTypeId(S, "single"));
}
#endif

#define MDL_START
#if defined(MDL_START)
/* Function: mdlStart =====================================================
 * Abstract:
 *   This function is called once at start of model execution. If you
 *   have states that should be initialized once, this is the place
 *   to do it.
 */
static void mdlStart(SimStruct *S)
{
    if (IS_SIMULATION_TARGET(S)) {

        /* Access bus/struct information */
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 2);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 3);


        /* Get common data type Id */
        DTypeId __SWLIBS_2Syst_FLTId = ssGetDataTypeId(S, "SWLIBS_2Syst_FLT");
        DTypeId __singleId = ssGetDataTypeId(S, "single");
        DTypeId __AMCLIB_BEMF_OBSRV_DQ_T_FLTId = ssGetDataTypeId(S, "AMCLIB_BEMF_OBSRV_DQ_T_FLT");
        DTypeId __GFLIB_CONTROLLER_PIAW_R_T_FLTId = ssGetDataTypeId(S, "GFLIB_CONTROLLER_PIAW_R_T_FLT");

        /* Get common data type size */
        __dtSizeInfo[0] = ssGetDataTypeSize(S, __SWLIBS_2Syst_FLTId);
        __dtSizeInfo[1] = ssGetDataTypeSize(S, __singleId);
        __dtSizeInfo[2] = ssGetDataTypeSize(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId);
        __dtSizeInfo[3] = ssGetDataTypeSize(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId);

        /* Get information for accessing SWLIBS_2Syst_FLT.fltArg1 */
        __dtBusInfo[0] = ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 0);
        __dtBusInfo[1] = __dtSizeInfo[1];

        /* Get information for accessing SWLIBS_2Syst_FLT.fltArg2 */
        __dtBusInfo[2] = ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 1);
        __dtBusInfo[3] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pEObsrv.fltArg1 */
        __dtBusInfo[4] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 0) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 0);
        __dtBusInfo[5] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pEObsrv.fltArg2 */
        __dtBusInfo[6] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 0) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 1);
        __dtBusInfo[7] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pIObsrv.fltArg1 */
        __dtBusInfo[8] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 1) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 0);
        __dtBusInfo[9] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pIObsrv.fltArg2 */
        __dtBusInfo[10] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 1) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 1);
        __dtBusInfo[11] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltCC1sc */
        __dtBusInfo[12] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 0);
        __dtBusInfo[13] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltCC2sc */
        __dtBusInfo[14] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 1);
        __dtBusInfo[15] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltAcc */
        __dtBusInfo[16] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 2);
        __dtBusInfo[17] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltInErrK1 */
        __dtBusInfo[18] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 3);
        __dtBusInfo[19] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltUpperLimit */
        __dtBusInfo[20] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 4);
        __dtBusInfo[21] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamD.fltLowerLimit */
        __dtBusInfo[22] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 2) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 5);
        __dtBusInfo[23] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltCC1sc */
        __dtBusInfo[24] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 0);
        __dtBusInfo[25] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltCC2sc */
        __dtBusInfo[26] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 1);
        __dtBusInfo[27] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltAcc */
        __dtBusInfo[28] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 2);
        __dtBusInfo[29] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltInErrK1 */
        __dtBusInfo[30] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 3);
        __dtBusInfo[31] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltUpperLimit */
        __dtBusInfo[32] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 4);
        __dtBusInfo[33] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pParamQ.fltLowerLimit */
        __dtBusInfo[34] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 3) + ssGetBusElementOffset(S, __GFLIB_CONTROLLER_PIAW_R_T_FLTId, 5);
        __dtBusInfo[35] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pIObsrvIn_1.fltArg1 */
        __dtBusInfo[36] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 4) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 0);
        __dtBusInfo[37] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.pIObsrvIn_1.fltArg2 */
        __dtBusInfo[38] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 4) + ssGetBusElementOffset(S, __SWLIBS_2Syst_FLTId, 1);
        __dtBusInfo[39] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.fltIGain */
        __dtBusInfo[40] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 5);
        __dtBusInfo[41] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.fltUGain */
        __dtBusInfo[42] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 6);
        __dtBusInfo[43] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.fltWIGain */
        __dtBusInfo[44] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 7);
        __dtBusInfo[45] = __dtSizeInfo[1];

        /* Get information for accessing AMCLIB_BEMF_OBSRV_DQ_T_FLT.fltEGain */
        __dtBusInfo[46] = ssGetBusElementOffset(S, __AMCLIB_BEMF_OBSRV_DQ_T_FLTId, 8);
        __dtBusInfo[47] = __dtSizeInfo[1];


        /* Alloc memory for the pwork 1 (__u1BUS) */
        {
            SWLIBS_2Syst_FLT* __u1BUS = (SWLIBS_2Syst_FLT*)calloc(sizeof(SWLIBS_2Syst_FLT), ssGetInputPortWidth(S, 0));
            if (__u1BUS==NULL) {
                ssSetErrorStatus(S, "Unexpected error during the memory allocation for __u1BUS");
                return;
            } /* if */
            ssSetPWorkValue(S, 0, __u1BUS);
        }

        /* Alloc memory for the pwork 2 (__u2BUS) */
        {
            SWLIBS_2Syst_FLT* __u2BUS = (SWLIBS_2Syst_FLT*)calloc(sizeof(SWLIBS_2Syst_FLT), ssGetInputPortWidth(S, 1));
            if (__u2BUS==NULL) {
                ssSetErrorStatus(S, "Unexpected error during the memory allocation for __u2BUS");
                return;
            } /* if */
            ssSetPWorkValue(S, 1, __u2BUS);
        }

        /* Alloc memory for the pwork 3 (__work1BUS) */
        {
            AMCLIB_BEMF_OBSRV_DQ_T_FLT* __work1BUS = (AMCLIB_BEMF_OBSRV_DQ_T_FLT*)calloc(sizeof(AMCLIB_BEMF_OBSRV_DQ_T_FLT), ssGetDWorkWidth(S, 0));
            if (__work1BUS==NULL) {
                ssSetErrorStatus(S, "Unexpected error during the memory allocation for __work1BUS");
                return;
            } /* if */
            ssSetPWorkValue(S, 2, __work1BUS);
        }

    } /* if */
}
#endif

#define MDL_INITIALIZE_CONDITIONS
#if defined(MDL_INITIALIZE_CONDITIONS)
/* Function: mdlInitializeConditions ======================================
 * Abstract:
 *   In this function, you should initialize the states for your S-function block.
 *   You can also perform any other initialization activities that your
 *   S-function may require. Note, this routine will be called at the
 *   start of simulation and if it is present in an enabled subsystem
 *   configured to reset states, it will be call when the enabled subsystem
 *   restarts execution to reset the states.
 */
static void mdlInitializeConditions(SimStruct *S)
{
    if (IS_SIMULATION_TARGET(S)) {

        /* Access bus/struct information */
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 2);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 3);


        /* Get access to Parameter/Input/Output/DWork data */
        char* work1 = (char*) ssGetDWork(S, 0);
        int32_T* work2 = (int32_T*) ssGetDWork(S, 1);
        real32_T* p3 = (real32_T*) ssGetRunTimeParamInfo(S, 2)->data;
        real32_T* p4 = (real32_T*) ssGetRunTimeParamInfo(S, 3)->data;
        real32_T* p5 = (real32_T*) ssGetRunTimeParamInfo(S, 4)->data;
        real32_T* p6 = (real32_T*) ssGetRunTimeParamInfo(S, 5)->data;
        real32_T* p7 = (real32_T*) ssGetRunTimeParamInfo(S, 6)->data;
        real32_T* p8 = (real32_T*) ssGetRunTimeParamInfo(S, 7)->data;
        real32_T* p9 = (real32_T*) ssGetRunTimeParamInfo(S, 8)->data;
        real32_T* p10 = (real32_T*) ssGetRunTimeParamInfo(S, 9)->data;
        real32_T* p11 = (real32_T*) ssGetRunTimeParamInfo(S, 10)->data;
        real32_T* p12 = (real32_T*) ssGetRunTimeParamInfo(S, 11)->data;
        real32_T* p13 = (real32_T*) ssGetRunTimeParamInfo(S, 12)->data;
        real32_T* p14 = (real32_T*) ssGetRunTimeParamInfo(S, 13)->data;
        int32_T* p2 = (int32_T*) ssGetRunTimeParamInfo(S, 1)->data;

        AMCLIB_BEMF_OBSRV_DQ_T_FLT* __work1BUS = (AMCLIB_BEMF_OBSRV_DQ_T_FLT*) ssGetPWorkValue(S, 2);

        /* Assign the Simulink structure work1 to user structure __work1BUS */
        (void) memcpy(&__work1BUS[0].pEObsrv.fltArg1, work1 +  __dtBusInfo[4],  __dtBusInfo[5]);
        (void) memcpy(&__work1BUS[0].pEObsrv.fltArg2, work1 +  __dtBusInfo[6],  __dtBusInfo[7]);
        (void) memcpy(&__work1BUS[0].pIObsrv.fltArg1, work1 +  __dtBusInfo[8],  __dtBusInfo[9]);
        (void) memcpy(&__work1BUS[0].pIObsrv.fltArg2, work1 +  __dtBusInfo[10],  __dtBusInfo[11]);
        (void) memcpy(&__work1BUS[0].pParamD.fltCC1sc, work1 +  __dtBusInfo[12],  __dtBusInfo[13]);
        (void) memcpy(&__work1BUS[0].pParamD.fltCC2sc, work1 +  __dtBusInfo[14],  __dtBusInfo[15]);
        (void) memcpy(&__work1BUS[0].pParamD.fltAcc, work1 +  __dtBusInfo[16],  __dtBusInfo[17]);
        (void) memcpy(&__work1BUS[0].pParamD.fltInErrK1, work1 +  __dtBusInfo[18],  __dtBusInfo[19]);
        (void) memcpy(&__work1BUS[0].pParamD.fltUpperLimit, work1 +  __dtBusInfo[20],  __dtBusInfo[21]);
        (void) memcpy(&__work1BUS[0].pParamD.fltLowerLimit, work1 +  __dtBusInfo[22],  __dtBusInfo[23]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltCC1sc, work1 +  __dtBusInfo[24],  __dtBusInfo[25]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltCC2sc, work1 +  __dtBusInfo[26],  __dtBusInfo[27]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltAcc, work1 +  __dtBusInfo[28],  __dtBusInfo[29]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltInErrK1, work1 +  __dtBusInfo[30],  __dtBusInfo[31]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltUpperLimit, work1 +  __dtBusInfo[32],  __dtBusInfo[33]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltLowerLimit, work1 +  __dtBusInfo[34],  __dtBusInfo[35]);
        (void) memcpy(&__work1BUS[0].pIObsrvIn_1.fltArg1, work1 +  __dtBusInfo[36],  __dtBusInfo[37]);
        (void) memcpy(&__work1BUS[0].pIObsrvIn_1.fltArg2, work1 +  __dtBusInfo[38],  __dtBusInfo[39]);
        (void) memcpy(&__work1BUS[0].fltIGain, work1 +  __dtBusInfo[40],  __dtBusInfo[41]);
        (void) memcpy(&__work1BUS[0].fltUGain, work1 +  __dtBusInfo[42],  __dtBusInfo[43]);
        (void) memcpy(&__work1BUS[0].fltWIGain, work1 +  __dtBusInfo[44],  __dtBusInfo[45]);
        (void) memcpy(&__work1BUS[0].fltEGain, work1 +  __dtBusInfo[46],  __dtBusInfo[47]);

        /* Call the legacy code function */
        AMCLIB_BemfObsrvDQInit_w_FLT(__work1BUS, work2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, *p11, *p12, *p13, *p14, *p2);

        /* Assign the user structure __work1BUS to the Simulink structure work1 */
        (void) memcpy(work1 +  __dtBusInfo[4], &__work1BUS[0].pEObsrv.fltArg1,  __dtBusInfo[5]);
        (void) memcpy(work1 +  __dtBusInfo[6], &__work1BUS[0].pEObsrv.fltArg2,  __dtBusInfo[7]);
        (void) memcpy(work1 +  __dtBusInfo[8], &__work1BUS[0].pIObsrv.fltArg1,  __dtBusInfo[9]);
        (void) memcpy(work1 +  __dtBusInfo[10], &__work1BUS[0].pIObsrv.fltArg2,  __dtBusInfo[11]);
        (void) memcpy(work1 +  __dtBusInfo[12], &__work1BUS[0].pParamD.fltCC1sc,  __dtBusInfo[13]);
        (void) memcpy(work1 +  __dtBusInfo[14], &__work1BUS[0].pParamD.fltCC2sc,  __dtBusInfo[15]);
        (void) memcpy(work1 +  __dtBusInfo[16], &__work1BUS[0].pParamD.fltAcc,  __dtBusInfo[17]);
        (void) memcpy(work1 +  __dtBusInfo[18], &__work1BUS[0].pParamD.fltInErrK1,  __dtBusInfo[19]);
        (void) memcpy(work1 +  __dtBusInfo[20], &__work1BUS[0].pParamD.fltUpperLimit,  __dtBusInfo[21]);
        (void) memcpy(work1 +  __dtBusInfo[22], &__work1BUS[0].pParamD.fltLowerLimit,  __dtBusInfo[23]);
        (void) memcpy(work1 +  __dtBusInfo[24], &__work1BUS[0].pParamQ.fltCC1sc,  __dtBusInfo[25]);
        (void) memcpy(work1 +  __dtBusInfo[26], &__work1BUS[0].pParamQ.fltCC2sc,  __dtBusInfo[27]);
        (void) memcpy(work1 +  __dtBusInfo[28], &__work1BUS[0].pParamQ.fltAcc,  __dtBusInfo[29]);
        (void) memcpy(work1 +  __dtBusInfo[30], &__work1BUS[0].pParamQ.fltInErrK1,  __dtBusInfo[31]);
        (void) memcpy(work1 +  __dtBusInfo[32], &__work1BUS[0].pParamQ.fltUpperLimit,  __dtBusInfo[33]);
        (void) memcpy(work1 +  __dtBusInfo[34], &__work1BUS[0].pParamQ.fltLowerLimit,  __dtBusInfo[35]);
        (void) memcpy(work1 +  __dtBusInfo[36], &__work1BUS[0].pIObsrvIn_1.fltArg1,  __dtBusInfo[37]);
        (void) memcpy(work1 +  __dtBusInfo[38], &__work1BUS[0].pIObsrvIn_1.fltArg2,  __dtBusInfo[39]);
        (void) memcpy(work1 +  __dtBusInfo[40], &__work1BUS[0].fltIGain,  __dtBusInfo[41]);
        (void) memcpy(work1 +  __dtBusInfo[42], &__work1BUS[0].fltUGain,  __dtBusInfo[43]);
        (void) memcpy(work1 +  __dtBusInfo[44], &__work1BUS[0].fltWIGain,  __dtBusInfo[45]);
        (void) memcpy(work1 +  __dtBusInfo[46], &__work1BUS[0].fltEGain,  __dtBusInfo[47]);
    } /* if */
}
#endif

/* Function: mdlOutputs ===================================================
 * Abstract:
 *   In this function, you compute the outputs of your S-function
 *   block. Generally outputs are placed in the output vector(s),
 *   ssGetOutputPortSignal.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    if (IS_SIMULATION_TARGET(S)) {

        /* Access bus/struct information */
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 2);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 3);


        /* Get access to Parameter/Input/Output/DWork data */
        real32_T* y1 = (real32_T*) ssGetOutputPortSignal(S, 0);
        char* u1 = (char*) ssGetInputPortSignal(S, 0);
        char* u2 = (char*) ssGetInputPortSignal(S, 1);
        real32_T* u3 = (real32_T*) ssGetInputPortSignal(S, 2);
        real32_T* u4 = (real32_T*) ssGetInputPortSignal(S, 3);
        char* work1 = (char*) ssGetDWork(S, 0);
        int32_T* work2 = (int32_T*) ssGetDWork(S, 1);
        real32_T* u5 = (real32_T*) ssGetInputPortSignal(S, 4);
        real32_T* u6 = (real32_T*) ssGetInputPortSignal(S, 5);
        real32_T* u7 = (real32_T*) ssGetInputPortSignal(S, 6);
        real32_T* u8 = (real32_T*) ssGetInputPortSignal(S, 7);
        real32_T* u9 = (real32_T*) ssGetInputPortSignal(S, 8);
        real32_T* u10 = (real32_T*) ssGetInputPortSignal(S, 9);
        real32_T* u11 = (real32_T*) ssGetInputPortSignal(S, 10);
        real32_T* u12 = (real32_T*) ssGetInputPortSignal(S, 11);
        real32_T* u13 = (real32_T*) ssGetInputPortSignal(S, 12);
        real32_T* u14 = (real32_T*) ssGetInputPortSignal(S, 13);
        real32_T* u15 = (real32_T*) ssGetInputPortSignal(S, 14);
        real32_T* u16 = (real32_T*) ssGetInputPortSignal(S, 15);
        real32_T* u17 = (real32_T*) ssGetInputPortSignal(S, 16);
        real32_T* u18 = (real32_T*) ssGetInputPortSignal(S, 17);
        real32_T* u19 = (real32_T*) ssGetInputPortSignal(S, 18);
        real32_T* u20 = (real32_T*) ssGetInputPortSignal(S, 19);
        real32_T* u21 = (real32_T*) ssGetInputPortSignal(S, 20);
        real32_T* u22 = (real32_T*) ssGetInputPortSignal(S, 21);
        real32_T* u23 = (real32_T*) ssGetInputPortSignal(S, 22);
        real32_T* u24 = (real32_T*) ssGetInputPortSignal(S, 23);
        real32_T* y2 = (real32_T*) ssGetOutputPortSignal(S, 1);
        real32_T* y3 = (real32_T*) ssGetOutputPortSignal(S, 2);
        real32_T* y4 = (real32_T*) ssGetOutputPortSignal(S, 3);
        real32_T* y5 = (real32_T*) ssGetOutputPortSignal(S, 4);
        real32_T* y6 = (real32_T*) ssGetOutputPortSignal(S, 5);
        real32_T* y7 = (real32_T*) ssGetOutputPortSignal(S, 6);
        real32_T* y8 = (real32_T*) ssGetOutputPortSignal(S, 7);
        real32_T* y9 = (real32_T*) ssGetOutputPortSignal(S, 8);
        real32_T* y10 = (real32_T*) ssGetOutputPortSignal(S, 9);
        real32_T* y11 = (real32_T*) ssGetOutputPortSignal(S, 10);
        int32_T* p1 = (int32_T*) ssGetRunTimeParamInfo(S, 0)->data;
        int32_T* p2 = (int32_T*) ssGetRunTimeParamInfo(S, 1)->data;

        SWLIBS_2Syst_FLT* __u1BUS = (SWLIBS_2Syst_FLT*) ssGetPWorkValue(S, 0);
        SWLIBS_2Syst_FLT* __u2BUS = (SWLIBS_2Syst_FLT*) ssGetPWorkValue(S, 1);
        AMCLIB_BEMF_OBSRV_DQ_T_FLT* __work1BUS = (AMCLIB_BEMF_OBSRV_DQ_T_FLT*) ssGetPWorkValue(S, 2);

        /* Assign the Simulink structure u1 to user structure __u1BUS */
        (void) memcpy(&__u1BUS[0].fltArg1, u1 +  __dtBusInfo[0],  __dtBusInfo[1]);
        (void) memcpy(&__u1BUS[0].fltArg2, u1 +  __dtBusInfo[2],  __dtBusInfo[3]);

        /* Assign the Simulink structure u2 to user structure __u2BUS */
        (void) memcpy(&__u2BUS[0].fltArg1, u2 +  __dtBusInfo[0],  __dtBusInfo[1]);
        (void) memcpy(&__u2BUS[0].fltArg2, u2 +  __dtBusInfo[2],  __dtBusInfo[3]);

        /* Assign the Simulink structure work1 to user structure __work1BUS */
        (void) memcpy(&__work1BUS[0].pEObsrv.fltArg1, work1 +  __dtBusInfo[4],  __dtBusInfo[5]);
        (void) memcpy(&__work1BUS[0].pEObsrv.fltArg2, work1 +  __dtBusInfo[6],  __dtBusInfo[7]);
        (void) memcpy(&__work1BUS[0].pIObsrv.fltArg1, work1 +  __dtBusInfo[8],  __dtBusInfo[9]);
        (void) memcpy(&__work1BUS[0].pIObsrv.fltArg2, work1 +  __dtBusInfo[10],  __dtBusInfo[11]);
        (void) memcpy(&__work1BUS[0].pParamD.fltCC1sc, work1 +  __dtBusInfo[12],  __dtBusInfo[13]);
        (void) memcpy(&__work1BUS[0].pParamD.fltCC2sc, work1 +  __dtBusInfo[14],  __dtBusInfo[15]);
        (void) memcpy(&__work1BUS[0].pParamD.fltAcc, work1 +  __dtBusInfo[16],  __dtBusInfo[17]);
        (void) memcpy(&__work1BUS[0].pParamD.fltInErrK1, work1 +  __dtBusInfo[18],  __dtBusInfo[19]);
        (void) memcpy(&__work1BUS[0].pParamD.fltUpperLimit, work1 +  __dtBusInfo[20],  __dtBusInfo[21]);
        (void) memcpy(&__work1BUS[0].pParamD.fltLowerLimit, work1 +  __dtBusInfo[22],  __dtBusInfo[23]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltCC1sc, work1 +  __dtBusInfo[24],  __dtBusInfo[25]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltCC2sc, work1 +  __dtBusInfo[26],  __dtBusInfo[27]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltAcc, work1 +  __dtBusInfo[28],  __dtBusInfo[29]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltInErrK1, work1 +  __dtBusInfo[30],  __dtBusInfo[31]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltUpperLimit, work1 +  __dtBusInfo[32],  __dtBusInfo[33]);
        (void) memcpy(&__work1BUS[0].pParamQ.fltLowerLimit, work1 +  __dtBusInfo[34],  __dtBusInfo[35]);
        (void) memcpy(&__work1BUS[0].pIObsrvIn_1.fltArg1, work1 +  __dtBusInfo[36],  __dtBusInfo[37]);
        (void) memcpy(&__work1BUS[0].pIObsrvIn_1.fltArg2, work1 +  __dtBusInfo[38],  __dtBusInfo[39]);
        (void) memcpy(&__work1BUS[0].fltIGain, work1 +  __dtBusInfo[40],  __dtBusInfo[41]);
        (void) memcpy(&__work1BUS[0].fltUGain, work1 +  __dtBusInfo[42],  __dtBusInfo[43]);
        (void) memcpy(&__work1BUS[0].fltWIGain, work1 +  __dtBusInfo[44],  __dtBusInfo[45]);
        (void) memcpy(&__work1BUS[0].fltEGain, work1 +  __dtBusInfo[46],  __dtBusInfo[47]);

        /* Call the legacy code function */
        *y1 = AMCLIB_BemfObsrvDQ_w_FLT(__u1BUS, __u2BUS, *u3, *u4, __work1BUS, work2, *u5, *u6, *u7, *u8, *u9, *u10, *u11, *u12, *u13, *u14, *u15, *u16, *u17, *u18, *u19, *u20, *u21, *u22, *u23, *u24, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11, *p1, *p2);

        /* Assign the user structure __work1BUS to the Simulink structure work1 */
        (void) memcpy(work1 +  __dtBusInfo[4], &__work1BUS[0].pEObsrv.fltArg1,  __dtBusInfo[5]);
        (void) memcpy(work1 +  __dtBusInfo[6], &__work1BUS[0].pEObsrv.fltArg2,  __dtBusInfo[7]);
        (void) memcpy(work1 +  __dtBusInfo[8], &__work1BUS[0].pIObsrv.fltArg1,  __dtBusInfo[9]);
        (void) memcpy(work1 +  __dtBusInfo[10], &__work1BUS[0].pIObsrv.fltArg2,  __dtBusInfo[11]);
        (void) memcpy(work1 +  __dtBusInfo[12], &__work1BUS[0].pParamD.fltCC1sc,  __dtBusInfo[13]);
        (void) memcpy(work1 +  __dtBusInfo[14], &__work1BUS[0].pParamD.fltCC2sc,  __dtBusInfo[15]);
        (void) memcpy(work1 +  __dtBusInfo[16], &__work1BUS[0].pParamD.fltAcc,  __dtBusInfo[17]);
        (void) memcpy(work1 +  __dtBusInfo[18], &__work1BUS[0].pParamD.fltInErrK1,  __dtBusInfo[19]);
        (void) memcpy(work1 +  __dtBusInfo[20], &__work1BUS[0].pParamD.fltUpperLimit,  __dtBusInfo[21]);
        (void) memcpy(work1 +  __dtBusInfo[22], &__work1BUS[0].pParamD.fltLowerLimit,  __dtBusInfo[23]);
        (void) memcpy(work1 +  __dtBusInfo[24], &__work1BUS[0].pParamQ.fltCC1sc,  __dtBusInfo[25]);
        (void) memcpy(work1 +  __dtBusInfo[26], &__work1BUS[0].pParamQ.fltCC2sc,  __dtBusInfo[27]);
        (void) memcpy(work1 +  __dtBusInfo[28], &__work1BUS[0].pParamQ.fltAcc,  __dtBusInfo[29]);
        (void) memcpy(work1 +  __dtBusInfo[30], &__work1BUS[0].pParamQ.fltInErrK1,  __dtBusInfo[31]);
        (void) memcpy(work1 +  __dtBusInfo[32], &__work1BUS[0].pParamQ.fltUpperLimit,  __dtBusInfo[33]);
        (void) memcpy(work1 +  __dtBusInfo[34], &__work1BUS[0].pParamQ.fltLowerLimit,  __dtBusInfo[35]);
        (void) memcpy(work1 +  __dtBusInfo[36], &__work1BUS[0].pIObsrvIn_1.fltArg1,  __dtBusInfo[37]);
        (void) memcpy(work1 +  __dtBusInfo[38], &__work1BUS[0].pIObsrvIn_1.fltArg2,  __dtBusInfo[39]);
        (void) memcpy(work1 +  __dtBusInfo[40], &__work1BUS[0].fltIGain,  __dtBusInfo[41]);
        (void) memcpy(work1 +  __dtBusInfo[42], &__work1BUS[0].fltUGain,  __dtBusInfo[43]);
        (void) memcpy(work1 +  __dtBusInfo[44], &__work1BUS[0].fltWIGain,  __dtBusInfo[45]);
        (void) memcpy(work1 +  __dtBusInfo[46], &__work1BUS[0].fltEGain,  __dtBusInfo[47]);
    } /* if */
}

/* Function: mdlTerminate =================================================
 * Abstract:
 *   In this function, you should perform any actions that are necessary
 *   at the termination of a simulation.
 */
static void mdlTerminate(SimStruct *S)
{
    if (IS_SIMULATION_TARGET(S)) {

        /* Access bus/struct information */
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 2);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 3);


        /* Free memory for the pwork 1 (__u1BUS) */
        {
            SWLIBS_2Syst_FLT* __u1BUS = (SWLIBS_2Syst_FLT*)ssGetPWorkValue(S, 0);
            if (__u1BUS!=NULL) {
                free(__u1BUS);
                ssSetPWorkValue(S, 0, NULL);
            } /* if */
        }

        /* Free memory for the pwork 2 (__u2BUS) */
        {
            SWLIBS_2Syst_FLT* __u2BUS = (SWLIBS_2Syst_FLT*)ssGetPWorkValue(S, 1);
            if (__u2BUS!=NULL) {
                free(__u2BUS);
                ssSetPWorkValue(S, 1, NULL);
            } /* if */
        }

        /* Free memory for the pwork 3 (__work1BUS) */
        {
            AMCLIB_BEMF_OBSRV_DQ_T_FLT* __work1BUS = (AMCLIB_BEMF_OBSRV_DQ_T_FLT*)ssGetPWorkValue(S, 2);
            if (__work1BUS!=NULL) {
                free(__work1BUS);
                ssSetPWorkValue(S, 2, NULL);
            } /* if */
        }

    } /* if */
}

/* Function: CheckDataTypeChecksum ========================================
 * Abstract:
 *   CheckDataTypeChecksum invokes a MATLAB helper for checking the consistency
 *   between the data type definition used when this S-Function was generated
 *   and the data type used when calling the S-Function.
 */
static int_T CheckDataTypeChecksum(SimStruct *S, const char* dtypeName, uint32_T* chkRef)
{
    mxArray *plhs[1] = {NULL};
    mxArray *prhs[3];
    mxArray *err = NULL;
    const char *bpath = ssGetPath(S);
    int_T status = -1;

    prhs[0] = mxCreateString(bpath);
    prhs[1] = mxCreateString(dtypeName);
    prhs[2] = mxCreateDoubleMatrix(1, 4, mxREAL);
    mxGetPr(prhs[2])[0] = chkRef[0];
    mxGetPr(prhs[2])[1] = chkRef[1];
    mxGetPr(prhs[2])[2] = chkRef[2];
    mxGetPr(prhs[2])[3] = chkRef[3];

    err = mexCallMATLABWithTrap(1, plhs, 3, prhs, "legacycode.LCT.getOrCompareDataTypeChecksum");
    mxDestroyArray(prhs[0]);
    mxDestroyArray(prhs[1]);
    mxDestroyArray(prhs[2]);

    if (err==NULL && plhs[0]!=NULL) {
        status = mxIsEmpty(plhs[0]) ? -1 : (int_T) (mxGetScalar(plhs[0]) != 0);
        mxDestroyArray(plhs[0]);
    } /* if */

    return status;
}

/* Function: CheckDataTypes ===============================================
 * Abstract:
 *   CheckDataTypes verifies data type consistency between the data type 
 *   definition used when this S-Function was generated and the data type
 *   used when calling the S-Function.
 */
static void CheckDataTypes(SimStruct *S)
{

    /* Verify Bus/StructType 'SWLIBS_2Syst_FLT', specification */
    {
        uint32_T chk[] = {1910970754, 866280756, 1155391210, 492100123};
        int_T status;
        status = CheckDataTypeChecksum(S, "SWLIBS_2Syst_FLT", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink Bus/StructType 'SWLIBS_2Syst_FLT'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink Bus/StructType 'SWLIBS_2Syst_FLT' definition has changed since the S-Function was generated");
        }
    }

    /* Verify Bus/StructType 'AMCLIB_BEMF_OBSRV_DQ_T_FLT', specification */
    {
        uint32_T chk[] = {1650086140, 303493879, 1436903234, 267489762};
        int_T status;
        status = CheckDataTypeChecksum(S, "AMCLIB_BEMF_OBSRV_DQ_T_FLT", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink Bus/StructType 'AMCLIB_BEMF_OBSRV_DQ_T_FLT'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink Bus/StructType 'AMCLIB_BEMF_OBSRV_DQ_T_FLT' definition has changed since the S-Function was generated");
        }
    }

    /* Verify Bus/StructType 'GFLIB_CONTROLLER_PIAW_R_T_FLT', specification */
    {
        uint32_T chk[] = {1134103077, 708184969, 200444125, 813311757};
        int_T status;
        status = CheckDataTypeChecksum(S, "GFLIB_CONTROLLER_PIAW_R_T_FLT", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink Bus/StructType 'GFLIB_CONTROLLER_PIAW_R_T_FLT'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink Bus/StructType 'GFLIB_CONTROLLER_PIAW_R_T_FLT' definition has changed since the S-Function was generated");
        }
    }
}

/* Function: GetRTWEnvironmentMode ========================================
 * Abstract:
 *   Must be called when ssRTWGenIsCodeGen(S)==true. This function
 *   returns the code generation mode:
 *       -1 if an error occurred
 *        0 for standalone code generation target
 *        1 for simulation target (Accelerator, RTW-SFcn,...)
 */
static int_T GetRTWEnvironmentMode(SimStruct *S)
{
    int_T status;
    mxArray * err;
    mxArray *plhs[1];
    mxArray *prhs[1];

    /* Get the name of the Simulink block diagram */
    prhs[0] = mxCreateString(ssGetModelName(ssGetRootSS(S)));
    plhs[0] = NULL;

    /* Call "isSimulationTarget = rtwenvironmentmode(modelName)" in MATLAB */
    err = mexCallMATLABWithTrap(1, plhs, 1, prhs, "rtwenvironmentmode");

    mxDestroyArray(prhs[0]);

    /* Set the error status if an error occurred */
    if (err) {
        if (plhs[0]) {
            mxDestroyArray(plhs[0]);
            plhs[0] = NULL;
        } /* if: } */
        ssSetErrorStatus(S, "Unknown error during call to 'rtwenvironmentmode'.");
        return -1;
    } /* if */

    /* Get the value returned by rtwenvironmentmode(modelName) */
    if (plhs[0]) {
        status = (int_T) (mxGetScalar(plhs[0]) != 0);
        mxDestroyArray(plhs[0]);
        plhs[0] = NULL;
    } /* if */

    return status;
}

/* Required S-function trailer */
#ifdef    MATLAB_MEX_FILE
# include "simulink.c"
#else
# include "cg_sfun.h"
#endif

