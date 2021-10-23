/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#ifndef LIN_COMMON_API_H
#define LIN_COMMON_API_H

/*!
 * @addtogroup common_core_api_group
 * @{
 */

#include "lin.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define word status macro */
#define SAVE_CONFIG_SET             0x0040U       /*!< Save configuration */
#define EVENT_TRIGGER_COLLISION_SET 0x0020U       /*!< Event triggered frame collision */
#define BUS_ACTIVITY_SET            0x0010U       /*!< Bus activity */
#define GO_TO_SLEEP_SET             0x0008U       /*!< Go to sleep */
#define OVERRUN                     0x0004U       /*!< Overrun */
#define SUCCESSFULL_TRANSFER        0x0002U       /*!< Successful transfer */
#define ERROR_IN_RESPONSE           0x0001U       /*!< Error in response */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup driver_cluster_group
 * @{
 */
/*!
 * @brief This function performs the initialization of the LIN core;
 * is the first call a user must use in the LIN core before using any other API functions.
 * The implementation of this function can be replaced by user if needed.
 *
 * @return Operation status = Zero, which is equivalent to 'Initialization was successful'.
 */
l_bool l_sys_init(void);

/*! @} */

#if (SUPPORT_MASTER_MODE == 1U)
/*!
 * @addtogroup schedule_management_group
 * @{
 */
/*!
 * @brief This function follows a schedule. When a frame becomes due, its
 * transmission is initiated. When the end of the current schedule is reached,
 * this function starts again at the beginning of the schedule.
 *
 * @param[in] Interface name
 * @return Operation status
 *        - Zero: if the next call of l_sch_tick will not start transmission of a frame.
 *        - Non-Zero: if the next call of l_sch_tick will start transmission of a frame.
 * The return value will in this case be the next schedule table entry's number (counted from the
 * beginning of the schedule table) in the schedule table. The return value will be in range 1 to N
 * if the schedule table has N entries.
 */
l_u8 l_sch_tick(l_ifc_handle iii);

/*!
 * @brief Set up the next schedule to be followed by the l_sch_tick
 *  function for a certain interface. The new schedule will be
 *  activated as soon as the current schedule reaches
 *  its next schedule entry point.
 *
 * @param[in] iii Interface name
 * @param[in] schedule_iii Schedule table for interface
 * @param[in] entry Entry to be set
 * @return void
 */
void l_sch_set(l_ifc_handle iii,
               l_schedule_handle schedule_iii,
               l_u8 entry);

#endif /* End (SUPPORT_MASTER_MODE == 1U) */
/*! @} */

/*!
 * @addtogroup interface_management_group
 * @{
 */
/*!
 * @brief Initialize the controller specified by name, i.e. sets up internal
 *  functions such as the baud rate. The default schedule set by the l_ifc_init
 *  call will be the L_NULL_SCHEDULE where no frames will be sent and received.
 *  This is the first call a user must perform, before using any other interface
 *  related LIN API functions.
 *  The function returns zero if the initialization was successful and non-zero if failed.
 *
 * @param[in] iii Interface name
 * @return Operation status
 *        - Zero:       Initialization was successful.
 *        - Non-zero:   Initialization failed.
 */
l_bool l_ifc_init(l_ifc_handle iii);

#if (SUPPORT_MASTER_MODE == 1U)
/*!
 * @brief Request slave nodes on the cluster connected to the interface to enter
 *  bus sleep mode by issuing one go to sleep command.
 *  This API is available only for Master nodes.
 *
 *  @note After sending go to sleep command successfully, the master node sets go to sleep flag
 *  to 1 and goes to sleep mode. At the end of Go to sleep schedule table, at the end of frame slot of
 *  go to sleep command, in l_sch_tick() the master node actually switches its active schedule table
 *  to Null to stop all communication. To start LIN communication, the master node shall call
 *  l_ifc_wake_up() to wake up LIN cluster and l_sch_set() to activate normal schedule table.
 *
 * @param[in] iii Interface name
 * @return void
 */
void l_ifc_goto_sleep(l_ifc_handle iii);

#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*!
 * @brief Transmit the wake up signal.
 *
 * @param[in] iii Interface name
 * @return void
 */
void l_ifc_wake_up(l_ifc_handle iii);

/*!
 * @brief This function will return the status of the previous communication.
 *
 * @param[in] iii Interface name
 * @return l_u16
 */
l_u16 l_ifc_read_status(l_ifc_handle iii);

/*! @} */

/*!
 * @addtogroup call_out_group
 * @{
 */
/*!
 * @brief Disable LIN related IRQ.
 *
 * @param[in] iii Interface name
 * @return l_u16
 */
l_u16 l_sys_irq_disable(l_ifc_handle iii);

/*!
 * @brief Enable LIN related IRQ.
 *
 * @param[in] iii Interface name
 * @return void
 */
void l_sys_irq_restore(l_ifc_handle iii);

/*! @} */

#endif /* LIN_COMMON_API_H */

/*! @} */

/******************************************************************************/
/* EOF */
/******************************************************************************/
