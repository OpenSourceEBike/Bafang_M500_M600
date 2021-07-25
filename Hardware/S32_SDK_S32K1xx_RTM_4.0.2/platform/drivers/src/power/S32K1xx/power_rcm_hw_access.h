/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
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

#ifndef POWER_RCM_HW_ACCESS_H
#define POWER_RCM_HW_ACCESS_H

#include "device_registers.h"
#include "power_manager_S32K1xx.h"

/*! @file power_rcm_hw_access.h */

/*!
 * @ingroup power_rcm_hw_access
 * @defgroup power_rcm_hw_access
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name Reset Control Module APIs*/
/*@{*/

static inline bool RCM_GetSrcStatusCmd(const RCM_Type * const baseAddr,
                         const rcm_source_names_t srcName)
{
    bool retValue;
    uint32_t regValue = (uint32_t)baseAddr->SRS;

    DEV_ASSERT(srcName < RCM_SRC_NAME_MAX);

    switch (srcName)
    {
        case RCM_LOW_VOLT_DETECT:              /* low voltage detect reset */
            regValue = (regValue & RCM_SRS_LVD_MASK) >> RCM_SRS_LVD_SHIFT;
            break;
        case RCM_LOSS_OF_CLK:                  /* loss of clock reset */
            regValue = (regValue & RCM_SRS_LOC_MASK) >> RCM_SRS_LOC_SHIFT;
            break;
        case RCM_LOSS_OF_LOCK:                 /* loss of lock reset */
            regValue = (regValue & RCM_SRS_LOL_MASK) >> RCM_SRS_LOL_SHIFT;
            break;
#if FEATURE_RCM_HAS_CMU_LOSS_OF_CLOCK          /*!< CMU Loss of lock reset */
        case RCM_CMU_LOC:
            regValue = (regValue & RCM_SRS_CMU_LOC_MASK) >> RCM_SRS_CMU_LOC_SHIFT;
            break;     
#endif
        case RCM_WATCH_DOG:                    /* watch dog reset */
            regValue = (regValue & RCM_SRS_WDOG_MASK) >> RCM_SRS_WDOG_SHIFT;
            break;
        case RCM_EXTERNAL_PIN:                 /* external pin reset */
            regValue = (regValue & RCM_SRS_PIN_MASK) >> RCM_SRS_PIN_SHIFT;
            break;
        case RCM_POWER_ON:                     /* power on reset */
            regValue = (regValue & RCM_SRS_POR_MASK) >> RCM_SRS_POR_SHIFT;
            break;
        case RCM_SJTAG:                        /* JTAG generated reset */
            regValue = (regValue & RCM_SSRS_SJTAG_MASK) >> RCM_SSRS_SJTAG_SHIFT;
            break;
        case RCM_CORE_LOCKUP:                  /* core lockup reset */
            regValue = (regValue & RCM_SRS_LOCKUP_MASK) >> RCM_SRS_LOCKUP_SHIFT;
            break;
        case RCM_SOFTWARE:                     /* software reset */
            regValue = (regValue & RCM_SRS_SW_MASK) >> RCM_SRS_SW_SHIFT;
            break;
        case RCM_SMDM_AP:                      /* MDM-AP system reset */
            regValue = (regValue & RCM_SSRS_SMDM_AP_MASK) >> RCM_SSRS_SMDM_AP_SHIFT;
            break;
        case RCM_STOP_MODE_ACK_ERR:            /* stop mode ack error reset */
            regValue = (regValue & RCM_SRS_SACKERR_MASK) >> RCM_SRS_SACKERR_SHIFT;
            break;
        default:
            /* invalid command */
            regValue = 0U;
            break;
    }

    retValue = (regValue == 0UL) ? false : true;

    return retValue;
}

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* POWER_RCM_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
