/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

/*!
 * @file mpu_driver.h
 */

#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include "status.h"
#include "device_registers.h"

/*!
 * @defgroup mpu_drv MPU Driver
 * @ingroup mpu
 * @brief Memory Protection Unit Peripheral Driver.
 * @addtogroup mpu_drv
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/
/*!
 * @brief MPU access error
 * Implements : mpu_err_access_type_t_Class
 */
typedef enum
{
    MPU_ERR_TYPE_READ  = 0U,    /*!< MPU error type: read  */
    MPU_ERR_TYPE_WRITE = 1U     /*!< MPU error type: write */
} mpu_err_access_type_t;

/*!
 * @brief MPU access error attributes
 * Implements : mpu_err_attributes_t_Class
 */
typedef enum
{
    MPU_INSTRUCTION_ACCESS_IN_USER_MODE       = 0U,    /*!< Access instruction error in user mode       */
    MPU_DATA_ACCESS_IN_USER_MODE              = 1U,    /*!< Access data error in user mode              */
    MPU_INSTRUCTION_ACCESS_IN_SUPERVISOR_MODE = 2U,    /*!< Access instruction error in supervisor mode */
    MPU_DATA_ACCESS_IN_SUPERVISOR_MODE        = 3U     /*!< Access data error in supervisor mode        */
} mpu_err_attributes_t;

/*!
 * @brief MPU detail error access info
 * Implements : mpu_access_err_info_t_Class
 */
typedef struct
{
    uint8_t                master;                     /*!< Access error master                   */
    mpu_err_attributes_t   attributes;                 /*!< Access error attributes               */
    mpu_err_access_type_t  accessType;                 /*!< Access error type                     */
    uint16_t               accessCtr;                  /*!< Access error control                  */
    uint32_t               addr;                       /*!< Access error address                  */
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    uint8_t                processorIdentification;    /*!< Access error processor identification */
#endif
} mpu_access_err_info_t;

/*!
 * @brief MPU access rights.
 * |      Code                     |  Supervisor  |     User    |  Description                                                                       |
 * |-------------------------------|--------------|-------------|------------------------------------------------------------------------------------|
 * |  MPU_SUPERVISOR_RWX_USER_NONE |    r w x     |    - - -    | Allow Read, write, execute in supervisor mode; no access in user mode              |
 * |  MPU_SUPERVISOR_RWX_USER_X    |    r w x     |    - - x    | Allow Read, write, execute in supervisor mode; execute in user mode                |
 * |  MPU_SUPERVISOR_RWX_USER_W    |    r w x     |    - w -    | Allow Read, write, execute in supervisor mode; write in user mode                  |
 * |  MPU_SUPERVISOR_RWX_USER_WX   |    r w x     |    - w x    | Allow Read, write, execute in supervisor mode; write and execute in user mode      |
 * |  MPU_SUPERVISOR_RWX_USER_R    |    r w x     |    r - -    | Allow Read, write, execute in supervisor mode; read in user mode                   |
 * |  MPU_SUPERVISOR_RWX_USER_RX   |    r w x     |    r - x    | Allow Read, write, execute in supervisor mode; read and execute in user mode       |
 * |  MPU_SUPERVISOR_RWX_USER_RW   |    r w x     |    r w -    | Allow Read, write, execute in supervisor mode; read and write in user mode         |
 * |  MPU_SUPERVISOR_RWX_USER_RWX  |    r w x     |    r w x    | Allow Read, write, execute in supervisor mode; read, write and execute in user mode|
 * |  MPU_SUPERVISOR_RX_USER_NONE  |    r - x     |    - - -    | Allow Read, execute in supervisor mode; no access in user mode                     |
 * |  MPU_SUPERVISOR_RX_USER_X     |    r - x     |    - - x    | Allow Read, execute in supervisor mode; execute in user mode                       |
 * |  MPU_SUPERVISOR_RX_USER_W     |    r - x     |    - w -    | Allow Read, execute in supervisor mode; write in user mode                         |
 * |  MPU_SUPERVISOR_RX_USER_WX    |    r - x     |    - w x    | Allow Read, execute in supervisor mode; write and execute in user mode             |
 * |  MPU_SUPERVISOR_RX_USER_R     |    r - x     |    r - -    | Allow Read, execute in supervisor mode; read in user mode                          |
 * |  MPU_SUPERVISOR_RX_USER_RX    |    r - x     |    r - x    | Allow Read, execute in supervisor mode; read and execute in user mode              |
 * |  MPU_SUPERVISOR_RX_USER_RW    |    r - x     |    r w -    | Allow Read, execute in supervisor mode; read and write in user mode                |
 * |  MPU_SUPERVISOR_RX_USER_RWX   |    r - x     |    r w x    | Allow Read, execute in supervisor mode; read, write and execute in user mode       |
 * |  MPU_SUPERVISOR_RW_USER_NONE  |    r w -     |    - - -    | Allow Read, write in supervisor mode; no access in user mode                       |
 * |  MPU_SUPERVISOR_RW_USER_X     |    r w -     |    - - x    | Allow Read, write in supervisor mode; execute in user mode                         |
 * |  MPU_SUPERVISOR_RW_USER_W     |    r w -     |    - w -    | Allow Read, write in supervisor mode; write in user mode                           |
 * |  MPU_SUPERVISOR_RW_USER_WX    |    r w -     |    - w x    | Allow Read, write in supervisor mode; write and execute in user mode               |
 * |  MPU_SUPERVISOR_RW_USER_R     |    r w -     |    r - -    | Allow Read, write in supervisor mode; read in user mode                            |
 * |  MPU_SUPERVISOR_RW_USER_RX    |    r w -     |    r - x    | Allow Read, write in supervisor mode; read and execute in user mode                |
 * |  MPU_SUPERVISOR_RW_USER_RW    |    r w -     |    r w -    | Allow Read, write in supervisor mode; read and write in user mode                  |
 * |  MPU_SUPERVISOR_RW_USER_RWX   |    r w -     |    r w x    | Allow Read, write in supervisor mode; read, write and execute in user mode         |
 * |  MPU_SUPERVISOR_USER_NONE     |    - - -     |    - - -    | No access allowed in user and supervisor modes                                     |
 * |  MPU_SUPERVISOR_USER_X        |    - - x     |    - - x    | Execute operation is allowed in user and supervisor modes                          |
 * |  MPU_SUPERVISOR_USER_W        |    - w -     |    - w -    | Write operation is allowed in user and supervisor modes                            |
 * |  MPU_SUPERVISOR_USER_WX       |    - w x     |    - w x    | Write and execute operations are allowed in user and supervisor modes              |
 * |  MPU_SUPERVISOR_USER_R        |    r - -     |    r - -    | Read operation is allowed in user and supervisor modes                             |
 * |  MPU_SUPERVISOR_USER_RX       |    r - x     |    r - x    | Read and execute operations are allowed in user and supervisor modes               |
 * |  MPU_SUPERVISOR_USER_RW       |    r w -     |    r w -    | Read and write operations are allowed in user and supervisor modes                 |
 * |  MPU_SUPERVISOR_USER_RWX      |    r w x     |    r w x    | Read write and execute operations are allowed in user and supervisor modes         |
 *
 *
 * |      Code                     | Read/Write permission   | Description                       |
 * |-------------------------------|-------------------------|-----------------------------------|
 * |  MPU_NONE                     |          - -            | No Read/Write access permission   |
 * |  MPU_W                        |          - w            | Write access permission           |
 * |  MPU_R                        |          r -            | Read access permission            |
 * |  MPU_RW                       |          r w            | Read/Write access permission      |
 * Implements : mpu_access_rights_t_Class
 */
typedef enum
{
    /* Format: M_R_W_SS_UUU
     * M  : 1 bit  - Specify that access right is for masters which have separated
     * privilege rights for user and supervisor mode accesses (e.g. master0~3)
     * R  : 1 bit  - Read access permission
     * W  : 1 bit  - Write access permission
     * SS : 2 bits - Supervisor Mode Access Control
     * UUU: 3 bits - User Mode Access Control
     */
    MPU_SUPERVISOR_RWX_USER_NONE    = 0x00U,    /*!< 0b00000000U : rwx|--- */
    MPU_SUPERVISOR_RWX_USER_X       = 0x01U,    /*!< 0b00000001U : rwx|--x */
    MPU_SUPERVISOR_RWX_USER_W       = 0x02U,    /*!< 0b00000010U : rwx|-w- */
    MPU_SUPERVISOR_RWX_USER_WX      = 0x03U,    /*!< 0b00000011U : rwx|-wx */
    MPU_SUPERVISOR_RWX_USER_R       = 0x04U,    /*!< 0b00000100U : rwx|r-- */
    MPU_SUPERVISOR_RWX_USER_RX      = 0x05U,    /*!< 0b00000101U : rwx|r-x */
    MPU_SUPERVISOR_RWX_USER_RW      = 0x06U,    /*!< 0b00000110U : rwx|rw- */
    MPU_SUPERVISOR_RWX_USER_RWX     = 0x07U,    /*!< 0b00000111U : rwx|rwx */
    MPU_SUPERVISOR_RX_USER_NONE     = 0x08U,    /*!< 0b00001000U : r-x|--- */
    MPU_SUPERVISOR_RX_USER_X        = 0x09U,    /*!< 0b00001001U : r-x|--x */
    MPU_SUPERVISOR_RX_USER_W        = 0x0AU,    /*!< 0b00001010U : r-x|-w- */
    MPU_SUPERVISOR_RX_USER_WX       = 0x0BU,    /*!< 0b00001011U : r-x|-wx */
    MPU_SUPERVISOR_RX_USER_R        = 0x0CU,    /*!< 0b00001100U : r-x|r-- */
    MPU_SUPERVISOR_RX_USER_RX       = 0x0DU,    /*!< 0b00001101U : r-x|r-x */
    MPU_SUPERVISOR_RX_USER_RW       = 0x0EU,    /*!< 0b00001110U : r-x|rw- */
    MPU_SUPERVISOR_RX_USER_RWX      = 0x0FU,    /*!< 0b00001111U : r-x|rwx */
    MPU_SUPERVISOR_RW_USER_NONE     = 0x10U,    /*!< 0b00010000U : rw-|--- */
    MPU_SUPERVISOR_RW_USER_X        = 0x11U,    /*!< 0b00010001U : rw-|--x */
    MPU_SUPERVISOR_RW_USER_W        = 0x12U,    /*!< 0b00010010U : rw-|-w- */
    MPU_SUPERVISOR_RW_USER_WX       = 0x13U,    /*!< 0b00010011U : rw-|-wx */
    MPU_SUPERVISOR_RW_USER_R        = 0x14U,    /*!< 0b00010100U : rw-|r-- */
    MPU_SUPERVISOR_RW_USER_RX       = 0x15U,    /*!< 0b00010101U : rw-|r-x */
    MPU_SUPERVISOR_RW_USER_RW       = 0x16U,    /*!< 0b00010110U : rw-|rw- */
    MPU_SUPERVISOR_RW_USER_RWX      = 0x17U,    /*!< 0b00010111U : rw-|rwx */
    MPU_SUPERVISOR_USER_NONE        = 0x18U,    /*!< 0b00011000U : ---|--- */
    MPU_SUPERVISOR_USER_X           = 0x19U,    /*!< 0b00011001U : --x|--x */
    MPU_SUPERVISOR_USER_W           = 0x1AU,    /*!< 0b00011010U : -w-|-w- */
    MPU_SUPERVISOR_USER_WX          = 0x1BU,    /*!< 0b00011011U : -wx|-wx */
    MPU_SUPERVISOR_USER_R           = 0x1CU,    /*!< 0b00011100U : r--|r-- */
    MPU_SUPERVISOR_USER_RX          = 0x1DU,    /*!< 0b00011101U : r-x|r-x */
    MPU_SUPERVISOR_USER_RW          = 0x1EU,    /*!< 0b00011110U : rw-|rw- */
    MPU_SUPERVISOR_USER_RWX         = 0x1FU,    /*!< 0b00011111U : rwx|rwx */
    MPU_NONE                        = 0x80U,    /*!< 0b10000000U : --      */
    MPU_W                           = 0xA0U,    /*!< 0b10100000U : w-      */
    MPU_R                           = 0xC0U,    /*!< 0b11000000U : -r      */
    MPU_RW                          = 0xE0U     /*!< 0b11100000U : wr      */
} mpu_access_rights_t;

/*!
 * @brief MPU master access rights.
 * Implements : mpu_master_access_right_t_Class
 */
typedef struct
{
    uint8_t                     masterNum;                  /*!< Master number                          */
    mpu_access_rights_t         accessRight;                /*!< Access right                           */
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    bool                        processIdentifierEnable;    /*!< Enables or disables process identifier */
#endif
} mpu_master_access_right_t;

/*!
 * @brief MPU user region configuration structure.
 * This structure is used when calling the MPU_DRV_Init function.
 * Implements : mpu_user_config_t_Class
 */
typedef struct
{
    uint32_t                            startAddr;            /*!< Memory region start address                   */
    uint32_t                            endAddr;              /*!< Memory region end address                     */
    const mpu_master_access_right_t     *masterAccRight;      /*!< Access permission for masters                 */
#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
    uint8_t                             processIdentifier;    /*!< Process identifier                            */
    uint8_t                             processIdMask;        /*!< Process identifier mask. The setting bit will
                                                              ignore the same bit in process identifier          */
#endif /* FEATURE_MPU_HAS_PROCESS_IDENTIFIER */
} mpu_user_config_t;

/*******************************************************************************
 * API
 *******************************************************************************/
/*!
 * @name MPU Driver API
 * @{
 */
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief The function initializes the memory protection unit by setting the access configurations
 * of all available masters, process identifier and the memory location for the given regions;
 * and activate module finally.
 * Please note that access rights for region 0 will always be configured and
 * regionCnt takes values between 1 and the maximum region count supported by the hardware.
 * e.g. In S32K144 the number of supported regions is 8.
 * The user must make sure that the clock is enabled.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] regionCnt The number of configured regions.
 * @param[in] userConfigArr The pointer to the array of MPU user configure structure, see #mpu_user_config_t.
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 *        - STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t MPU_DRV_Init(uint32_t instance,
                      uint8_t regionCnt,
                      const mpu_user_config_t * userConfigArr);

/*!
 * @brief De-initializes the memory protection unit by reseting all regions to default
 * and disable module.
 *
 * @param[in] instance The MPU peripheral instance number.
 */
void MPU_DRV_Deinit(uint32_t instance);

/*!
 * @brief Sets the region start and end address.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] regionNum The region number.
 * @param[in] startAddr The region start address.
 * @param[in] endAddr The region end address.
 */
void MPU_DRV_SetRegionAddr(uint32_t instance,
                           uint8_t regionNum,
                           uint32_t startAddr,
                           uint32_t endAddr);

/*!
 * @brief Sets the region configuration.
 * Updates the access configuration of all available masters,
 * process identifier and memory location in a given region.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] regionNum The region number.
 * @param[in] userConfigPtr The region configuration structure pointer.
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 *        - STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t MPU_DRV_SetRegionConfig(uint32_t instance,
                                 uint8_t regionNum,
                                 const mpu_user_config_t * userConfigPtr);

/*!
 * @brief Configures access permission for bus master in region.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] regionNum The MPU region number.
 * @param[in] accessRightsPtr The pointer to access permission structure, #see mpu_master_access_right_t.
 * @return operation status
 *        - STATUS_SUCCESS:    Operation was successful.
 *        - STATUS_ERROR:      Operation failed due to master number is out of range supported by hardware.
 */
status_t MPU_DRV_SetMasterAccessRights(uint32_t instance,
                                       uint8_t regionNum,
                                       const mpu_master_access_right_t * accessRightsPtr);

/*!
 * @brief Checks and gets the MPU access error detail information for a slave port.
 * Clears bus error flag if an error occurs.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] slavePortNum The slave port number to get Error Detail.
 * @param[out] errInfoPtr The pointer to access error info structure, #see mpu_access_err_info_t.
 * @return operation status
 *        - true  : An error has occurred.
 *        - false : No error has occurred.
 */
bool MPU_DRV_GetDetailErrorAccessInfo(uint32_t instance,
                                      uint8_t slavePortNum,
                                      mpu_access_err_info_t * errInfoPtr);

/*!
 * @brief Gets default region configuration.
 * Grants all access rights for masters and disables PID on entire memory.
 *
 * @param[out] masterAccRight The pointer to master configuration structure, see #mpu_master_access_right_t.
 *                            The length of array should be defined by number of masters supported by hardware.
 * @return The default region configuration, see #mpu_user_config_t.
 */
mpu_user_config_t MPU_DRV_GetDefaultRegionConfig(mpu_master_access_right_t * masterAccRight);

/*!
 * @brief Enables/Disables region descriptor.
 * Please note that region 0 should not be disabled.
 *
 * @param[in] instance The MPU peripheral instance number.
 * @param[in] regionNum The region number.
 * @param[in] enable Valid state
 *            - true  : Enable region.
 *            - false : Disable region.
 */
void MPU_DRV_EnableRegion(uint32_t instance,
                          uint8_t regionNum,
                          bool enable);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* MPU_DRIVER_H */
/*******************************************************************************
 * EOF
 *******************************************************************************/
