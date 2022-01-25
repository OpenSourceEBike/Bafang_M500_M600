/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * NXP Confidential. This software is owned or controlled by NXP and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */
/*!
 * @file ic_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 */

#ifndef IC_PAL_MAPPING_H
#define IC_PAL_MAPPING_H

#include "ic_pal_cfg.h"

/* Include peripheral drivers */
#if defined(IC_PAL_OVER_FTM)
    #include "ftm_ic_driver.h"
    #include "ftm_mc_driver.h"

    /* The maximum of channel in each instance */
    #define IC_PAL_NUM_OF_CHANNEL_MAX  FEATURE_FTM_CHANNEL_COUNT
    /* The maximum of counter */
    #define MAX_COUNTER_VALUE          FTM_FEATURE_CNT_MAX_VALUE_U32
    /* The number of instances are available */
    #define NUMBER_OF_IC_PAL_INSTANCES NO_OF_FTM_INSTS_FOR_IC
    /* The maximum of instance in FTM */
    #define IC_PAL_INSTANCE_MAX        FTM_INSTANCE_COUNT
#endif /* End of definition for IC_PAL_OVER_FTM */

#if defined(IC_PAL_OVER_EMIOS)
    #include "emios_mc_driver.h"
    #include "emios_ic_driver.h"

    /* The maximum of channel in each instance */
    #define IC_PAL_NUM_OF_CHANNEL_MAX  eMIOS_UC_COUNT
    /* The maximum of counter */
    #define MAX_COUNTER_VALUE          (0xFFFFU)
    /* The number of instances are available */
    #define NUMBER_OF_IC_PAL_INSTANCES NO_OF_EMIOS_INSTS_FOR_IC
    /* The maximum of instance in eMIOS */
    #define IC_PAL_INSTANCE_MAX        eMIOS_INSTANCE_COUNT
#endif /* End of definition for IC_PAL_OVER_EMIOS */

#if defined(IC_PAL_OVER_ETIMER)

    #include "etimer_driver.h"

    /* The maximum of channel in each instance */
    #define IC_PAL_NUM_OF_CHANNEL_MAX_OVER_ETIMER  ETIMER_CH_COUNT
    /* The number of instances are available */
    #define NUMBER_OF_IC_PAL_INSTANCES_OVER_ETIMER NO_OF_ETIMER_INSTS_FOR_IC
    /* The maximum of instance in ETIMER */
    #define IC_PAL_ETIMER_INSTANCE_COUNT           ETIMER_INSTANCE_COUNT
#endif /* End of definition for IC_PAL_OVER_ETIMER */

#if defined(IC_PAL_OVER_FLEXPWM)

    #include "flexpwm_driver.h"

    /* The maximum of channel in each instance */
    #define IC_PAL_NUM_OF_CHANNEL_MAX_OVER_FLEXPWM  FlexPWM_SUB_COUNT
    /* The maximum of counter */
    #define MAX_COUNTER_VALUE                       (0xFFFFU)
    /* The number of instances are available */
    #define NUMBER_OF_IC_PAL_INSTANCES_OVER_FLEXPWM NO_OF_FLEXPWM_INSTS_FOR_IC
    /* The maximum of instance in FLEXPWM */
    #define IC_PAL_FLEXPWM_INSTANCE_COUNT           FlexPWM_INSTANCE_COUNT
#endif /* End of definition for IC_PAL_OVER_FLEXPWM */

/*!
 * @brief Enumeration with the types of peripherals supported by IC PAL
 *
 * This enumeration contains the types of peripherals supported by IC PAL.
 * Implements : ic_inst_type_t_Class
 */
typedef enum{
#if defined(FTM_INSTANCE_COUNT)
    IC_INST_TYPE_FTM    = 0U,
#endif /* defined(FTM_INSTANCE_COUNT)  */

#if defined(eMIOS_INSTANCE_COUNT)
    IC_INST_TYPE_EMIOS  = 1U,
#endif /* defined(eMIOS_INSTANCE_COUNT) */

#if defined(ETIMER_INSTANCE_COUNT)
    IC_INST_TYPE_ETIMER = 2U,
#endif /* defined(ETIMER_INSTANCE_COUNT) */

#if defined(FlexPWM_INSTANCE_COUNT)
    IC_INST_TYPE_FLEXPWM = 3U
#endif /* defined(FLEXPWM_INSTANCE_COUNT) */
} ic_inst_type_t;


/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : ic_instance_t_Class
 */
typedef struct{
    ic_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;           /*!< Instance index of the peripheral over which the PAL is used */
} ic_instance_t;

#endif /* IC_PAL_MAPPING_H */
