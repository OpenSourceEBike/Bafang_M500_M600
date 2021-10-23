/**
 * GDFLIB_FilterIIR1_SF_F32.c
 *
 *    ABSTRACT:
 *      The purpose of this sfunction is to call a simple legacy
 *      function during simulation:
 *
 *         int32 y1 = GDFLIB_FilterIIR1_w_F32(int32 u1, int32 u2, int32 u3, int32 u4, int32 u5, int32 u6, GDFLIB_FILTER_IIR1_T_F32 work1[1], int32 y2[1], int32 y3[1], int32 p1)
 *
 *    Simulink version           : 9.3 (R2019a) 23-Nov-2018
 *    C source code generated on : 26-Jun-2020 20:19:31
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
#define S_FUNCTION_NAME  GDFLIB_FilterIIR1_SF_F32
#define S_FUNCTION_LEVEL 2

/**
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#include <string.h>
#include <stdlib.h>

/* Specific header file(s) required by the legacy code function */
#include "gdflib.h"
#include "GDFLIB_FilterIIR1_w.h"

/* Specific header file(s) required for data types declarations */
#include "SWLIBS_Typedefs.h"
#include "GDFLIB_FilterIIR1.h"

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
    ssSetNumSFcnParams(S, 4);

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

    /* Set the number of work vectors */
    if (!IS_SIMULATION_TARGET(S)) {
        ssSetNumPWork(S, 0);
        if (!ssSetNumDWork(S, 1)) return;
    } else {
        ssSetNumPWork(S, 1);
        if (!ssSetNumDWork(S, 3)) return;

        /* Configure the dwork 2 (__dtSizeInfo) */
        ssSetDWorkDataType(S, 1, SS_INT32);
        ssSetDWorkUsageType(S, 1, SS_DWORK_USED_AS_DWORK);
        ssSetDWorkName(S, 1, "dtSizeInfo");
        ssSetDWorkWidth(S, 1, 3);
        ssSetDWorkComplexSignal(S, 1, COMPLEX_NO);

        /* Configure the dwork 3 (__dtBusInfo) */
        ssSetDWorkDataType(S, 2, SS_INT32);
        ssSetDWorkUsageType(S, 2, SS_DWORK_USED_AS_DWORK);
        ssSetDWorkName(S, 2, "dtBusInfo");
        ssSetDWorkWidth(S, 2, 10);
        ssSetDWorkComplexSignal(S, 2, COMPLEX_NO);
    } /* if */

    /* Configure the dwork 1 (work1) */
    #if defined(MATLAB_MEX_FILE) 
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "GDFLIB_FILTER_IIR1_T_F32", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssSetDWorkDataType(S, 0, dataTypeIdReg);
    } /* if */
    #endif
    ssSetDWorkUsageType(S, 0, SS_DWORK_USED_AS_DWORK);
    ssSetDWorkName(S, 0, "work1");
    ssSetDWorkWidth(S, 0, 1);
    ssSetDWorkComplexSignal(S, 0, COMPLEX_NO);

    /* Set the number of input ports */
    if (!ssSetNumInputPorts(S, 6)) return;

    /* Configure the input port 1 */
    ssSetInputPortDataType(S, 0, SS_INT32);
    {
        int_T u1Width = 1;
        ssSetInputPortWidth(S, 0, u1Width);
    }
    ssSetInputPortComplexSignal(S, 0, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortAcceptExprInRTW(S, 0, 1);
    ssSetInputPortOverWritable(S, 0, 1);
    ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 0, 1);

    /* Configure the input port 2 */
    ssSetInputPortDataType(S, 1, SS_INT32);
    {
        int_T u2Width = 1;
        ssSetInputPortWidth(S, 1, u2Width);
    }
    ssSetInputPortComplexSignal(S, 1, COMPLEX_NO);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortAcceptExprInRTW(S, 1, 1);
    ssSetInputPortOverWritable(S, 1, 1);
    ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
    ssSetInputPortRequiredContiguous(S, 1, 1);

    /* Configure the input port 3 */
    ssSetInputPortDataType(S, 2, SS_INT32);
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
    ssSetInputPortDataType(S, 3, SS_INT32);
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
    ssSetInputPortDataType(S, 4, SS_INT32);
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
    ssSetInputPortDataType(S, 5, SS_INT32);
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

    /* Set the number of output ports */
    if (!ssSetNumOutputPorts(S, 3)) return;

    /* Configure the output port 1 */
    ssSetOutputPortDataType(S, 0, SS_INT32);
    {
        int_T y1Width = 1;
        ssSetOutputPortWidth(S, 0, y1Width);
    }
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 0, 0);

    /* Configure the output port 2 */
    ssSetOutputPortDataType(S, 1, SS_INT32);
    {
        int_T y2Width = 1;
        ssSetOutputPortWidth(S, 1, y2Width);
    }
    ssSetOutputPortComplexSignal(S, 1, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 1, 0);

    /* Configure the output port 3 */
    ssSetOutputPortDataType(S, 2, SS_INT32);
    {
        int_T y3Width = 1;
        ssSetOutputPortWidth(S, 2, y3Width);
    }
    ssSetOutputPortComplexSignal(S, 2, COMPLEX_NO);
    ssSetOutputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
    ssSetOutputPortOutputExprInRTW(S, 2, 0);

    /* Register reserved identifiers to avoid name conflict */
    if (ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) {

        /* Register reserved identifier for  */
        ssRegMdlInfo(S, "GDFLIB_FilterIIR1Init_w_F32", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

        /* Register reserved identifier for  */
        ssRegMdlInfo(S, "GDFLIB_FilterIIR1_w_F32", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

        /* Register reserved identifier for wrappers */
        if (isSimulationTarget) {

            /* Register reserved identifier for  */
            ssRegMdlInfo(S, "GDFLIB_FilterIIR1Init_w_F32_wrapper_InitializeConditions", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for  */
            ssRegMdlInfo(S, "GDFLIB_FilterIIR1_w_F32_wrapper_Output", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for allocating PWork for SimulationTarget */
            ssRegMdlInfo(S, "GDFLIB_FilterIIR1_SF_F32_wrapper_allocmem", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));

            /* Register reserved identifier for freeing PWork for SimulationTarget */
            ssRegMdlInfo(S, "GDFLIB_FilterIIR1_SF_F32_wrapper_freemem", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
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

        {
            DTypeId dataTypeIdReg;

            ssRegisterTypeFromNamedObject(S, "tFrac32", &dataTypeIdReg);
            if(dataTypeIdReg == INVALID_DTYPE_ID) return;
        }
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
    if (!ssSetNumRunTimeParams(S, 4)) return;

    /* Register the run-time parameter 1 */
    ssRegDlgParamAsRunTimeParam(S, 0, 0, "p1", ssGetDataTypeId(S, "int32"));

    /* Register the run-time parameter 2 */
    {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "tFrac32", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssRegDlgParamAsRunTimeParam(S, 1, 1, "p2", dataTypeIdReg);
    }

    /* Register the run-time parameter 3 */
    {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "tFrac32", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssRegDlgParamAsRunTimeParam(S, 2, 2, "p3", dataTypeIdReg);
    }

    /* Register the run-time parameter 4 */
    {
        DTypeId dataTypeIdReg;

        ssRegisterTypeFromNamedObject(S, "tFrac32", &dataTypeIdReg);
        if(dataTypeIdReg == INVALID_DTYPE_ID) return;

        ssRegDlgParamAsRunTimeParam(S, 3, 3, "p4", dataTypeIdReg);
    }
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
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 1);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 2);


        /* Get common data type Id */
        DTypeId __GDFLIB_FILTER_IIR1_T_F32Id = ssGetDataTypeId(S, "GDFLIB_FILTER_IIR1_T_F32");
        DTypeId __GDFLIB_FILTER_IIR1_COEFF_T_F32Id = ssGetDataTypeId(S, "GDFLIB_FILTER_IIR1_COEFF_T_F32");
        DTypeId __int32Id = ssGetDataTypeId(S, "int32");

        /* Get common data type size */
        __dtSizeInfo[0] = ssGetDataTypeSize(S, __GDFLIB_FILTER_IIR1_T_F32Id);
        __dtSizeInfo[1] = ssGetDataTypeSize(S, __GDFLIB_FILTER_IIR1_COEFF_T_F32Id);
        __dtSizeInfo[2] = ssGetDataTypeSize(S, __int32Id);

        /* Get information for accessing GDFLIB_FILTER_IIR1_T_F32.trFiltCoeff.f32B0 */
        __dtBusInfo[0] = ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_T_F32Id, 0) + ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_COEFF_T_F32Id, 0);
        __dtBusInfo[1] = __dtSizeInfo[2];

        /* Get information for accessing GDFLIB_FILTER_IIR1_T_F32.trFiltCoeff.f32B1 */
        __dtBusInfo[2] = ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_T_F32Id, 0) + ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_COEFF_T_F32Id, 1);
        __dtBusInfo[3] = __dtSizeInfo[2];

        /* Get information for accessing GDFLIB_FILTER_IIR1_T_F32.trFiltCoeff.f32A1 */
        __dtBusInfo[4] = ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_T_F32Id, 0) + ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_COEFF_T_F32Id, 2);
        __dtBusInfo[5] = __dtSizeInfo[2];

        /* Get information for accessing GDFLIB_FILTER_IIR1_T_F32.f32FiltBufferX */
        __dtBusInfo[6] = ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_T_F32Id, 1);
        __dtBusInfo[7] = __dtSizeInfo[2];

        /* Get information for accessing GDFLIB_FILTER_IIR1_T_F32.f32FiltBufferY */
        __dtBusInfo[8] = ssGetBusElementOffset(S, __GDFLIB_FILTER_IIR1_T_F32Id, 2);
        __dtBusInfo[9] = __dtSizeInfo[2];


        /* Alloc memory for the pwork 1 (__work1BUS) */
        {
            GDFLIB_FILTER_IIR1_T_F32* __work1BUS = (GDFLIB_FILTER_IIR1_T_F32*)calloc(sizeof(GDFLIB_FILTER_IIR1_T_F32), ssGetDWorkWidth(S, 0));
            if (__work1BUS==NULL) {
                ssSetErrorStatus(S, "Unexpected error during the memory allocation for __work1BUS");
                return;
            } /* if */
            ssSetPWorkValue(S, 0, __work1BUS);
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
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 1);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 2);


        /* Get access to Parameter/Input/Output/DWork data */
        tFrac32* p2 = (tFrac32*) ssGetRunTimeParamInfo(S, 1)->data;
        tFrac32* p3 = (tFrac32*) ssGetRunTimeParamInfo(S, 2)->data;
        tFrac32* p4 = (tFrac32*) ssGetRunTimeParamInfo(S, 3)->data;
        char* work1 = (char*) ssGetDWork(S, 0);

        GDFLIB_FILTER_IIR1_T_F32* __work1BUS = (GDFLIB_FILTER_IIR1_T_F32*) ssGetPWorkValue(S, 0);

        /* Assign the Simulink structure work1 to user structure __work1BUS */
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32B0, work1 +  __dtBusInfo[0],  __dtBusInfo[1]);
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32B1, work1 +  __dtBusInfo[2],  __dtBusInfo[3]);
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32A1, work1 +  __dtBusInfo[4],  __dtBusInfo[5]);
        (void) memcpy(&__work1BUS[0].f32FiltBufferX, work1 +  __dtBusInfo[6],  __dtBusInfo[7]);
        (void) memcpy(&__work1BUS[0].f32FiltBufferY, work1 +  __dtBusInfo[8],  __dtBusInfo[9]);

        /* Call the legacy code function */
        GDFLIB_FilterIIR1Init_w_F32(*p2, *p3, *p4, __work1BUS);

        /* Assign the user structure __work1BUS to the Simulink structure work1 */
        (void) memcpy(work1 +  __dtBusInfo[0], &__work1BUS[0].trFiltCoeff.f32B0,  __dtBusInfo[1]);
        (void) memcpy(work1 +  __dtBusInfo[2], &__work1BUS[0].trFiltCoeff.f32B1,  __dtBusInfo[3]);
        (void) memcpy(work1 +  __dtBusInfo[4], &__work1BUS[0].trFiltCoeff.f32A1,  __dtBusInfo[5]);
        (void) memcpy(work1 +  __dtBusInfo[6], &__work1BUS[0].f32FiltBufferX,  __dtBusInfo[7]);
        (void) memcpy(work1 +  __dtBusInfo[8], &__work1BUS[0].f32FiltBufferY,  __dtBusInfo[9]);
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
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 1);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 2);


        /* Get access to Parameter/Input/Output/DWork data */
        int32_T* y1 = (int32_T*) ssGetOutputPortSignal(S, 0);
        int32_T* u1 = (int32_T*) ssGetInputPortSignal(S, 0);
        int32_T* u2 = (int32_T*) ssGetInputPortSignal(S, 1);
        int32_T* u3 = (int32_T*) ssGetInputPortSignal(S, 2);
        int32_T* u4 = (int32_T*) ssGetInputPortSignal(S, 3);
        int32_T* u5 = (int32_T*) ssGetInputPortSignal(S, 4);
        int32_T* u6 = (int32_T*) ssGetInputPortSignal(S, 5);
        char* work1 = (char*) ssGetDWork(S, 0);
        int32_T* y2 = (int32_T*) ssGetOutputPortSignal(S, 1);
        int32_T* y3 = (int32_T*) ssGetOutputPortSignal(S, 2);
        int32_T* p1 = (int32_T*) ssGetRunTimeParamInfo(S, 0)->data;

        GDFLIB_FILTER_IIR1_T_F32* __work1BUS = (GDFLIB_FILTER_IIR1_T_F32*) ssGetPWorkValue(S, 0);

        /* Assign the Simulink structure work1 to user structure __work1BUS */
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32B0, work1 +  __dtBusInfo[0],  __dtBusInfo[1]);
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32B1, work1 +  __dtBusInfo[2],  __dtBusInfo[3]);
        (void) memcpy(&__work1BUS[0].trFiltCoeff.f32A1, work1 +  __dtBusInfo[4],  __dtBusInfo[5]);
        (void) memcpy(&__work1BUS[0].f32FiltBufferX, work1 +  __dtBusInfo[6],  __dtBusInfo[7]);
        (void) memcpy(&__work1BUS[0].f32FiltBufferY, work1 +  __dtBusInfo[8],  __dtBusInfo[9]);

        /* Call the legacy code function */
        *y1 = GDFLIB_FilterIIR1_w_F32(*u1, *u2, *u3, *u4, *u5, *u6, __work1BUS, y2, y3, *p1);

        /* Assign the user structure __work1BUS to the Simulink structure work1 */
        (void) memcpy(work1 +  __dtBusInfo[0], &__work1BUS[0].trFiltCoeff.f32B0,  __dtBusInfo[1]);
        (void) memcpy(work1 +  __dtBusInfo[2], &__work1BUS[0].trFiltCoeff.f32B1,  __dtBusInfo[3]);
        (void) memcpy(work1 +  __dtBusInfo[4], &__work1BUS[0].trFiltCoeff.f32A1,  __dtBusInfo[5]);
        (void) memcpy(work1 +  __dtBusInfo[6], &__work1BUS[0].f32FiltBufferX,  __dtBusInfo[7]);
        (void) memcpy(work1 +  __dtBusInfo[8], &__work1BUS[0].f32FiltBufferY,  __dtBusInfo[9]);
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
        int32_T* __dtSizeInfo = (int32_T*) ssGetDWork(S, 1);
        int32_T* __dtBusInfo = (int32_T*) ssGetDWork(S, 2);


        /* Free memory for the pwork 1 (__work1BUS) */
        {
            GDFLIB_FILTER_IIR1_T_F32* __work1BUS = (GDFLIB_FILTER_IIR1_T_F32*)ssGetPWorkValue(S, 0);
            if (__work1BUS!=NULL) {
                free(__work1BUS);
                ssSetPWorkValue(S, 0, NULL);
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

    /* Verify AliasType/NumericType 'tFrac32' specification */
    {
        uint32_T chk[] = {3148693762, 4028971480, 3566515793, 2138309135};
        int_T status;
        status = CheckDataTypeChecksum(S, "tFrac32", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink AliasType/NumericType 'tFrac32'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink AliasType/NumericType 'tFrac32' definition has changed since the S-Function was generated");
        }
    }

    /* Verify Bus/StructType 'GDFLIB_FILTER_IIR1_T_F32', specification */
    {
        uint32_T chk[] = {3801752083, 3177101575, 3769668879, 2141988428};
        int_T status;
        status = CheckDataTypeChecksum(S, "GDFLIB_FILTER_IIR1_T_F32", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink Bus/StructType 'GDFLIB_FILTER_IIR1_T_F32'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink Bus/StructType 'GDFLIB_FILTER_IIR1_T_F32' definition has changed since the S-Function was generated");
        }
    }

    /* Verify Bus/StructType 'GDFLIB_FILTER_IIR1_COEFF_T_F32', specification */
    {
        uint32_T chk[] = {2388288581, 2451367563, 2631596071, 3055935293};
        int_T status;
        status = CheckDataTypeChecksum(S, "GDFLIB_FILTER_IIR1_COEFF_T_F32", &chk[0]);
        if (status==-1) {
          ssSetErrorStatus(S, "Unexpected error when checking the validity of the Simulink Bus/StructType 'GDFLIB_FILTER_IIR1_COEFF_T_F32'");
        } else if (status==0) {
          ssSetErrorStatus(S, "The Simulink Bus/StructType 'GDFLIB_FILTER_IIR1_COEFF_T_F32' definition has changed since the S-Function was generated");
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

