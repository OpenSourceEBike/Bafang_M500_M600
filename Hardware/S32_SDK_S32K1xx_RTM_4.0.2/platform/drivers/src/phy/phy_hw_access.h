/*
 * Copyright 2020 NXP
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

#ifndef PHY_HW_ACCESS_H
#define PHY_HW_ACCESS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "phy.h"

/*!
 * @file phy_hw_access.h
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * @brief Clause 22 PHY write access.
 *
 * @param[in] instance Instance number
 * @param[in] phyAddr PHY device address
 * @param[in] phyReg PHY register address
 * @param[in] data Data to be written in the specified register of the PHY
 * @param[in] timeoutMs Timeout for the write operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed successfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t PHY_HW_WriteCl22(uint8_t instance, uint8_t phyAddr, uint8_t phyReg, uint16_t data, uint32_t timeoutMs);

/*!
 * @brief Clause 22 PHY read access.
 *
 * @param[in]  instance Instance number
 * @param[in]  phyAddr PHY device address
 * @param[in]  phyReg PHY register address
 * @param[out] data Data read from the PHY
 * @param[in] timeoutMs Timeout for the read operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed successfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t PHY_HW_ReadCl22(uint8_t instance, uint8_t phyAddr, uint8_t phyReg, uint16_t *data, uint32_t timeoutMs);

/*!
 * @brief Clause 45 PHY write access.
 *
 * @param[in] instance Instance number
 * @param[in] phyAddr PHY device address
 * @param[in] phyReg PHY register address
 * @param[in] data Data to be written in the specified register of the PHY
 * @param[in] timeoutMs Timeout for the write operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed successfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t PHY_HW_WriteCl45(uint8_t instance, uint8_t phyAddr, uint8_t mmd, uint16_t phyReg, uint16_t data, uint32_t timeoutMs);

/*!
 * @brief Clause 45 PHY read access.
 *
 * @param[in]  instance Instance number
 * @param[in]  phyAddr PHY device address
 * @param[in]  mmd The MMD index of the target register
 * @param[in]  phyReg PHY register address
 * @param[out] data Data read from the PHY
 * @param[in]  timeoutMs Timeout for the read operation (in milliseconds)
 * @return STATUS_SUCCESS if the operation completed successfully, STATUS_TIMEOUT
 * if the specified timeout expired before completing the operation.
 */
status_t PHY_HW_ReadCl45(uint8_t instance, uint8_t phyAddr, uint8_t mmd, uint16_t phyReg, uint16_t *data, uint32_t timeoutMs);


#ifdef __cplusplus
}
#endif


#endif /* PHY_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
