/*
** ###################################################################
**     Abstract:
**         Common include file for CMSIS register access layer headers.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2020 NXP
**     All rights reserved.
**
**     NXP Confidential. This software is owned or controlled by NXP and may only be
**     used strictly in accordance with the applicable license terms. By expressly
**     accepting such terms or by downloading, installing, activating and/or otherwise
**     using the software, you are agreeing that you have read, and that you agree to
**     comply with and are bound by, such license terms. If you do not agree to be
**     bound by the applicable license terms, then you may not retain, install,
**     activate or otherwise use the software. The production use license in
**     Section 2.3 is expressly granted for this software.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
** ###################################################################
*/

#ifndef DEVICE_REGISTERS_H
#define DEVICE_REGISTERS_H

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced.
* The macro defines the device currently in use and may be used by components for specific checks.
*
*/


/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */

#if (defined(CPU_S32K148) || defined(CPU_S32K146) || defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || defined(CPU_S32K142) || defined(CPU_S32K142W) || defined(CPU_S32K144W))

    #define S32K14x_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm4.h"

    #if defined(CPU_S32K148)

        #define S32K148_SERIES

        /* Register definitions */
        #include "S32K148/include/S32K148.h"
        /* CPU specific feature definitions */
        #include "S32K148/include/S32K148_features.h"

    #elif defined(CPU_S32K146)

        #define S32K146_SERIES

        /* Register definitions */
        #include "S32K146/include/S32K146.h"
        /* CPU specific feature definitions */
        #include "S32K146/include/S32K146_features.h"

    #elif (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT))

        #define S32K144_SERIES

        /* Register definitions */
        #include "S32K144/include/S32K144.h"
        /* CPU specific feature definitions */
        #include "S32K144/include/S32K144_features.h"

    #elif defined(CPU_S32K142)

        #define S32K142_SERIES

        /* Register definitions */
        #include "S32K142/include/S32K142.h"
        /* CPU specific feature definitions */
        #include "S32K142/include/S32K142_features.h"

    #elif defined(CPU_S32K142W)

        #define S32K142W_SERIES

        /* Register definitions */
        #include "S32K142W/include/S32K142W.h"
        /* CPU specific feature definitions */
        #include "S32K142W/include/S32K142W_features.h"

    #elif defined(CPU_S32K144W)

        #define S32K144W_SERIES

        /* Register definitions */
        #include "S32K144W/include/S32K144W.h"
        /* CPU specific feature definitions */
        #include "S32K144W/include/S32K144W_features.h"

    #endif

#elif defined(CPU_S32V234)

    #define S32V234_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm4.h"
    /* Register definitions */
    #include "S32V234/include/S32V234.h"
    /* CPU specific feature definitions */
    #include "S32V234/include/S32V234_features.h"

#elif (defined(CPU_S32K116) || defined(CPU_S32K118))

    #define S32K11x_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm0.h"

    #if defined(CPU_S32K116)

        #define S32K116_SERIES
        /* Register definitions */
        #include "S32K116/include/S32K116.h"
        /* CPU specific feature definitions */
        #include "S32K116/include/S32K116_features.h"
    #elif defined(CPU_S32K118)

        #define S32K118_SERIES
        /* Register definitions */
        #include "S32K118/include/S32K118.h"
        /* CPU specific feature definitions */
        #include "S32K118/include/S32K118_features.h"

    #endif

#elif defined(CPU_MPC5777C)

    #define MPC5777C_SERIES

    /* Specific core definitions */
    #include "common/s32_core_e200.h"
    /* Register definitions */
    #include "MPC5777C/include/MPC5777C.h"
    /* CPU specific feature definitions */
    #include "MPC5777C/include/MPC5777C_features.h"

#elif (defined(CPU_MPC5746R) || defined(CPU_MPC5748G) || defined(CPU_MPC5746C) || defined(CPU_MPC5741P)|| defined(CPU_MPC5742P)|| defined(CPU_MPC5743P)|| defined(CPU_MPC5744P) || defined(CPU_MPC5744B) || defined(CPU_MPC5745B) || defined(CPU_MPC5746B) || defined(CPU_MPC5744C) || defined(CPU_MPC5745C) || defined(CPU_MPC5747C) || defined(CPU_MPC5748C) || defined(CPU_MPC5746G) || defined(CPU_MPC5747G))

    #define MPC574x_SERIES

    /* Specific core definitions */
    #include "common/s32_core_e200.h"

    #if (defined(CPU_MPC5746R))

        #define MPC5746R_SERIES

        /* Register definitions */
        #include "MPC5746R/include/MPC5746R.h"
        /* CPU specific feature definitions */
        #include "MPC5746R/include/MPC5746R_features.h"

    #elif (defined(CPU_MPC5748G))

        #define MPC5748G_SERIES

        /* Register definitions */
        #include "MPC5748G/include/MPC5748G.h"
        /* CPU specific feature definitions */
        #include "MPC5748G/include/MPC5748G_features.h"

    #elif (defined(CPU_MPC5747C))

        #define MPC5747C_SERIES

        /* Register definitions */
        #include "MPC5747C/include/MPC5747C.h"
        /* CPU specific feature definitions */
        #include "MPC5747C/include/MPC5747C_features.h"

    #elif (defined(CPU_MPC5748C))

        #define MPC5748C_SERIES

        /* Register definitions */
        #include "MPC5748C/include/MPC5748C.h"
        /* CPU specific feature definitions */
        #include "MPC5748C/include/MPC5748C_features.h"

    #elif (defined(CPU_MPC5746G))

        #define MPC5746G_SERIES

        /* Register definitions */
        #include "MPC5746G/include/MPC5746G.h"
        /* CPU specific feature definitions */
        #include "MPC5746G/include/MPC5746G_features.h"

    #elif (defined(CPU_MPC5747G))

        #define MPC5747G_SERIES

        /* Register definitions */
        #include "MPC5747G/include/MPC5747G.h"
        /* CPU specific feature definitions */
        #include "MPC5747G/include/MPC5747G_features.h"

    #elif defined(CPU_MPC5746C)

        #define MPC5746C_SERIES

        /* Register definitions */
        #include "MPC5746C/include/MPC5746C.h"
        /* CPU specific feature definitions */
        #include "MPC5746C/include/MPC5746C_features.h"

    #elif defined(CPU_MPC5744B)

        #define MPC5744B_SERIES

        /* Register definitions */
        #include "MPC5744B/include/MPC5744B.h"
        /* CPU specific feature definitions */
        #include "MPC5744B/include/MPC5744B_features.h"

    #elif defined(CPU_MPC5745B)

        #define MPC5745B_SERIES

        /* Register definitions */
        #include "MPC5745B/include/MPC5745B.h"
        /* CPU specific feature definitions */
        #include "MPC5745B/include/MPC5745B_features.h"

    #elif defined(CPU_MPC5746B)

        #define MPC5746B_SERIES

        /* Register definitions */
        #include "MPC5746B/include/MPC5746B.h"
        /* CPU specific feature definitions */
        #include "MPC5746B/include/MPC5746B_features.h"

    #elif defined(CPU_MPC5744C)

        #define MPC5744C_SERIES

        /* Register definitions */
        #include "MPC5744C/include/MPC5744C.h"
        /* CPU specific feature definitions */
        #include "MPC5744C/include/MPC5744C_features.h"

    #elif defined(CPU_MPC5745C)

        #define MPC5745C_SERIES

        /* Register definitions */
        #include "MPC5745C/include/MPC5745C.h"
        /* CPU specific feature definitions */
        #include "MPC5745C/include/MPC5745C_features.h"

    #elif defined(CPU_MPC5741P)

        #define MPC5741P_SERIES

        /* Register definitions */
        #include "MPC5741P/include/MPC5741P.h"
        /* CPU specific feature definitions */
        #include "MPC5741P/include/MPC5741P_features.h"

    #elif defined(CPU_MPC5742P)

        #define MPC5742P_SERIES

        /* Register definitions */
        #include "MPC5742P/include/MPC5742P.h"
        /* CPU specific feature definitions */
        #include "MPC5742P/include/MPC5742P_features.h"

    #elif defined(CPU_MPC5743P)

        #define MPC5743P_SERIES

        /* Register definitions */
        #include "MPC5743P/include/MPC5743P.h"
        /* CPU specific feature definitions */
        #include "MPC5743P/include/MPC5743P_features.h"

    #elif defined(CPU_MPC5744P)

        #define MPC5744P_SERIES

        /* Register definitions */
        #include "MPC5744P/include/MPC5744P.h"
        /* CPU specific feature definitions */
        #include "MPC5744P/include/MPC5744P_features.h"

    #endif
#elif (defined(CPU_S32R274) || defined(CPU_S32R372))

    #define S32R_SERIES

    /* Specific core definitions */
    #include "common/s32_core_e200.h"

    #if (defined(CPU_S32R274))

        #define S32R274_SERIES

        /* Register definitions */
        #include "S32R274/include/S32R274.h"
        /* CPU specific feature definitions */
        #include "S32R274/include/S32R274_features.h"

    #elif (defined(CPU_S32R372))

        #define S32R372_SERIES

        /* Register definitions */
        #include "S32R372/include/S32R372.h"
        /* CPU specific feature definitions */
        #include "S32R372/include/S32R372_features.h"
    #endif
#elif (defined(CPU_S32MTV))

    #define S32MTV_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm4.h"

    /* Register definitions */
    #include "S32MTV/include/S32MTV.h"
    /* CPU specific feature definitions */
    #include "S32MTV/include/S32MTV_features.h"

#elif defined(CPU_SJA1110)

    #define SJA1110_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm7.h"

    /* Register definitions */
    #include "SJA1110/include/SJA1110.h"
    /* CPU specific feature definitions */
    #include "SJA1110/include/SJA1110_features.h"

#elif defined(CPU_S32S247)

    #define S32S247_SERIES

    /* Specific core definitions */
    #include "common/s32_core_cm7.h"

    /* Register definitions */
    #include "S32S247/include/S32S247.h"
    /* CPU specific feature definitions */
    #include "S32S247/include/S32S247_features.h"

#else
    #error "No valid CPU defined!"
#endif

#include "devassert.h"

#endif /* DEVICE_REGISTERS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
