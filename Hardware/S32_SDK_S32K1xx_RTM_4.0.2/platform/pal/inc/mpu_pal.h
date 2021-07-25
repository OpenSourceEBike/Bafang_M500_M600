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
 * @file mpu_pal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Typedef is used for SMPU configuration structure.
 */

#ifndef MPU_PAL_H
#define MPU_PAL_H

#include "status.h"
#include "mpu_pal_mapping.h"

/*!
 * @defgroup mpu_pal_code MPU PAL
 * @ingroup mpu_pal
 * @brief Memory Protection Unit Peripheral Abstraction Layer.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief MPU access error
 * Implements : mpu_error_access_type_t_Class
 */
typedef enum
{
    MPU_ERROR_TYPE_READ  = 0U, /*!< Error type: read */
    MPU_ERROR_TYPE_WRITE = 1U  /*!< Error type: write */
} mpu_error_access_type_t;

/*!
 * @brief MPU access error attributes
 * Implements : mpu_error_attributes_t_Class
 */
typedef enum
{
    MPU_ERROR_USER_MODE_INSTRUCTION_ACCESS       = 0U, /*!< Instruction access error in user mode */
    MPU_ERROR_USER_MODE_DATA_ACCESS              = 1U, /*!< Data access error in user mode */
    MPU_ERROR_SUPERVISOR_MODE_INSTRUCTION_ACCESS = 2U, /*!< Instruction access error in supervisor mode */
    MPU_ERROR_SUPERVISOR_MODE_DATA_ACCESS        = 3U  /*!< Data access error in supervisor mode */
} mpu_error_attributes_t;

/*!
 * @brief MPU detail error access info
 * Implements : mpu_error_info_t_Class
 */
typedef struct
{
    uint8_t                 master;     /*!< Access error master */
    bool                    overrun;    /*!< Access error master overrun */
    mpu_error_attributes_t  attributes; /*!< Access error attributes */
    mpu_error_access_type_t accessType; /*!< Access error type */
    uint32_t                accessCtr;  /*!< Access error control */
    uint32_t                addr;       /*!< Access error address */
    uint8_t                 processId;  /*!< Access error process identification */
} mpu_error_info_t;

#if defined(MPU_OVER_MPU)
/*!
 * @brief MPU detail access permission
 * For specific master:
 * |      Code                     |  Supervisor  |     User    |  Description                                                                        |
 * |-------------------------------|--------------|-------------|-------------------------------------------------------------------------------------|
 * |  MPU_SUPERVISOR_RWX_USER_NONE |    r w x     |    - - -    | Allow Read, write, execute in supervisor mode; no access in user mode               |
 * |  MPU_SUPERVISOR_RWX_USER_X    |    r w x     |    - - x    | Allow Read, write, execute in supervisor mode; execute in user mode                 |
 * |  MPU_SUPERVISOR_RWX_USER_W    |    r w x     |    - w -    | Allow Read, write, execute in supervisor mode; write in user mode                   |
 * |  MPU_SUPERVISOR_RWX_USER_WX   |    r w x     |    - w x    | Allow Read, write, execute in supervisor mode; write and execute in user mode       |
 * |  MPU_SUPERVISOR_RWX_USER_R    |    r w x     |    r - -    | Allow Read, write, execute in supervisor mode; read in user mode                    |
 * |  MPU_SUPERVISOR_RWX_USER_RX   |    r w x     |    r - x    | Allow Read, write, execute in supervisor mode; read and execute in user mode        |
 * |  MPU_SUPERVISOR_RWX_USER_RW   |    r w x     |    r w -    | Allow Read, write, execute in supervisor mode; read and write in user mode          |
 * |  MPU_SUPERVISOR_RWX_USER_RWX  |    r w x     |    r w x    | Allow Read, write, execute in supervisor mode; read, write and execute in user mode |
 * |  MPU_SUPERVISOR_RX_USER_NONE  |    r - x     |    - - -    | Allow Read, execute in supervisor mode; no access in user mode                      |
 * |  MPU_SUPERVISOR_RX_USER_X     |    r - x     |    - - x    | Allow Read, execute in supervisor mode; execute in user mode                        |
 * |  MPU_SUPERVISOR_RX_USER_W     |    r - x     |    - w -    | Allow Read, execute in supervisor mode; write in user mode                          |
 * |  MPU_SUPERVISOR_RX_USER_WX    |    r - x     |    - w x    | Allow Read, execute in supervisor mode; write and execute in user mode              |
 * |  MPU_SUPERVISOR_RX_USER_R     |    r - x     |    r - -    | Allow Read, execute in supervisor mode; read in user mode                           |
 * |  MPU_SUPERVISOR_RX_USER_RX    |    r - x     |    r - x    | Allow Read, execute in supervisor mode; read and execute in user mode               |
 * |  MPU_SUPERVISOR_RX_USER_RW    |    r - x     |    r w -    | Allow Read, execute in supervisor mode; read and write in user mode                 |
 * |  MPU_SUPERVISOR_RX_USER_RWX   |    r - x     |    r w x    | Allow Read, execute in supervisor mode; read, write and execute in user mode        |
 * |  MPU_SUPERVISOR_RW_USER_NONE  |    r w -     |    - - -    | Allow Read, write in supervisor mode; no access in user mode                        |
 * |  MPU_SUPERVISOR_RW_USER_X     |    r w -     |    - - x    | Allow Read, write in supervisor mode; execute in user mode                          |
 * |  MPU_SUPERVISOR_RW_USER_W     |    r w -     |    - w -    | Allow Read, write in supervisor mode; write in user mode                            |
 * |  MPU_SUPERVISOR_RW_USER_WX    |    r w -     |    - w x    | Allow Read, write in supervisor mode; write and execute in user mode                |
 * |  MPU_SUPERVISOR_RW_USER_R     |    r w -     |    r - -    | Allow Read, write in supervisor mode; read in user mode                             |
 * |  MPU_SUPERVISOR_RW_USER_RX    |    r w -     |    r - x    | Allow Read, write in supervisor mode; read and execute in user mode                 |
 * |  MPU_SUPERVISOR_RW_USER_RW    |    r w -     |    r w -    | Allow Read, write in supervisor mode; read and write in user mode                   |
 * |  MPU_SUPERVISOR_RW_USER_RWX   |    r w -     |    r w x    | Allow Read, write in supervisor mode; read, write and execute in user mode          |
 * |  MPU_SUPERVISOR_USER_NONE     |    - - -     |    - - -    | No access allowed in user and supervisor modes                                      |
 * |  MPU_SUPERVISOR_USER_X        |    - - x     |    - - x    | Execute operation is allowed in user and supervisor modes                           |
 * |  MPU_SUPERVISOR_USER_W        |    - w -     |    - w -    | Write operation is allowed in user and supervisor modes                             |
 * |  MPU_SUPERVISOR_USER_WX       |    - w x     |    - w x    | Write and execute operations are allowed in user and supervisor modes               |
 * |  MPU_SUPERVISOR_USER_R        |    r - -     |    r - -    | Read operation is allowed in user and supervisor modes                              |
 * |  MPU_SUPERVISOR_USER_RX       |    r - x     |    r - x    | Read and execute operations are allowed in user and supervisor modes                |
 * |  MPU_SUPERVISOR_USER_RW       |    r w -     |    r w -    | Read and write operations are allowed in user and supervisor modes                  |
 * |  MPU_SUPERVISOR_USER_RWX      |    r w x     |    r w x    | Read write and execute operations are allowed in user and supervisor modes          |
 *
 * For normal master:
 * |      Code                     | Read/Write permission | Description                     |
 * |-------------------------------|-----------------------|---------------------------------|
 * |  MPU_NONE                     |          - -          | No Read/Write access permission |
 * |  MPU_W                        |          - w          | Write access permission         |
 * |  MPU_R                        |          r -          | Read access permission          |
 * |  MPU_RW                       |          r w          | Read/Write access permission    |
 * Implements : mpu_access_permission_t_Class
 */
typedef mpu_access_rights_t mpu_access_permission_t; /* Should be made enum inside MPU driver */
#elif defined(MPU_OVER_SMPU)
typedef enum
{
    MPU_NONE           = 0U, /*!< All accesses are not allowed */
    MPU_W_OR_SET_1     = 1U, /*!< Write allowed, no Read; Specific access set 1 */
    MPU_R_OR_SET_2     = 2U, /*!< Read allowed, no Write; Specific access set 2 */
    MPU_RW_OR_SET_3    = 3U  /*!< Both Read and Write allowed; Specific access set 3 */
} mpu_access_permission_t;
#endif /* defined(MPU_OVER_MPU) & defined(MPU_OVER_SMPU) */

/*!
 * @brief MPU master access permission.
 * Implements : mpu_master_access_permission_t_Class
 */
typedef struct
{
    uint8_t                masterNum;    /*!< Master number */
    mpu_access_permission_t accessRight; /*!< Privilege right */
} mpu_master_access_permission_t;

/*!
 * @brief MPU region configuration structure.
 * Implements : mpu_region_config_t_Class
 */
typedef struct
{
    uint32_t                             startAddr;         /*!< Memory region start address */
    uint32_t                             endAddr;           /*!< Memory region end address */
    const mpu_master_access_permission_t *masterAccRight;   /*!< Access permission for masters */
    uint8_t                              processIdEnable;   /*!< Process identifier enable
                                                            For MPU: the bit index corresponding with masters
                                                            For SMPU: disable if equal zero, otherwise enable */
    uint8_t                              processIdentifier; /*!< Process identifier*/
    uint8_t                              processIdMask;     /*!< Process identifier mask. The setting bit will
                                                            ignore the same bit in process identifier */
    void                                 *extension;        /*!< This field will be used to add extra settings
                                                            to the basic region configuration */
} mpu_region_config_t;

#if defined(MPU_OVER_SMPU)
    #if (FEATURE_SMPU_HAS_SPECIFIC_ACCESS_RIGHT_COUNT != 0U)
    /*!
     * @brief Specific access permission.
     * Specifies separate access rights for supervisor mode (privilege) and user mode (unprivileged).
     * - Read (r) permission refers to the ability to access the referenced memory address
     * using an operand (data) fetch.
     * - Write (w) permission refers to the ability to update the referenced memory address
     * using a store (data) operation.
     * - Execute (x) permission refers to the ability to read the referenced memory address
     * using an instruction fetch
     * Implements : mpu_specific_access_permission_t_Class
     */
    typedef enum
    {
        MPU_SUPERVISOR_NONE_USER_NONE = 0x00U, /*!< All accesses are not allowed */
        MPU_SUPERVISOR_NONE_USER_X    = 0x01U, /*!< No access in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_NONE_USER_W    = 0x02U, /*!< No access in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_NONE_USER_WX   = 0x03U, /*!< No access in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_NONE_USER_R    = 0x04U, /*!< No access in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_NONE_USER_RX   = 0x05U, /*!< No access in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_NONE_USER_RW   = 0x06U, /*!< No access in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_NONE_USER_RWX  = 0x07U, /*!< No access in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_X_USER_NONE    = 0x08U, /*!< Allow Execute in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_X_USER_X       = 0x09U, /*!< Allow Execute in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_X_USER_W       = 0x0AU, /*!< Allow Execute in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_X_USER_WX      = 0x0BU, /*!< Allow Execute in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_X_USER_R       = 0x0CU, /*!< Allow Execute in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_X_USER_RX      = 0x0DU, /*!< Allow Execute in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_X_USER_RW      = 0x0EU, /*!< Allow Execute in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_X_USER_RWX     = 0x0FU, /*!< Allow Execute in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_W_USER_NONE    = 0x10U, /*!< Allow Write in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_W_USER_X       = 0x11U, /*!< Allow Write in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_W_USER_W       = 0x12U, /*!< Allow Write in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_W_USER_WX      = 0x13U, /*!< Allow Write in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_W_USER_R       = 0x14U, /*!< Allow Write in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_W_USER_RX      = 0x15U, /*!< Allow Write in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_W_USER_RW      = 0x16U, /*!< Allow Write in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_W_USER_RWX     = 0x17U, /*!< Allow Write in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_WX_USER_NONE   = 0x18U, /*!< Allow Write, Execute in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_WX_USER_X      = 0x19U, /*!< Allow Write, Execute in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_WX_USER_W      = 0x1AU, /*!< Allow Write, Execute in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_WX_USER_WX     = 0x1BU, /*!< Allow Write, Execute in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_WX_USER_R      = 0x1CU, /*!< Allow Write, Execute in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_WX_USER_RX     = 0x1DU, /*!< Allow Write, Execute in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_WX_USER_RW     = 0x1EU, /*!< Allow Write, Execute in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_WX_USER_RWX    = 0x1FU, /*!< Allow Write, Execute in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_R_USER_NONE    = 0x20U, /*!< Allow Read in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_R_USER_X       = 0x21U, /*!< Allow Read in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_R_USER_W       = 0x22U, /*!< Allow Read in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_R_USER_WX      = 0x23U, /*!< Allow Read in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_R_USER_R       = 0x24U, /*!< Allow Read in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_R_USER_RX      = 0x25U, /*!< Allow Read in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_R_USER_RW      = 0x26U, /*!< Allow Read in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_R_USER_RWX     = 0x27U, /*!< Allow Read in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_RX_USER_NONE   = 0x28U, /*!< Allow Read, Execute in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_RX_USER_X      = 0x29U, /*!< Allow Read, Execute in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_RX_USER_W      = 0x2AU, /*!< Allow Read, Execute in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_RX_USER_WX     = 0x2BU, /*!< Allow Read, Execute in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_RX_USER_R      = 0x2CU, /*!< Allow Read, Execute in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_RX_USER_RX     = 0x2DU, /*!< Allow Read, Execute in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_RX_USER_RW     = 0x2EU, /*!< Allow Read, Execute in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_RX_USER_RWX    = 0x2FU, /*!< Allow Read, Execute in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_RW_USER_NONE   = 0x30U, /*!< Allow Read, Write in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_RW_USER_X      = 0x31U, /*!< Allow Read, Write in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_RW_USER_W      = 0x32U, /*!< Allow Read, Write in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_RW_USER_WX     = 0x33U, /*!< Allow Read, Write in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_RW_USER_R      = 0x34U, /*!< Allow Read, Write in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_RW_USER_RX     = 0x35U, /*!< Allow Read, Write in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_RW_USER_RW     = 0x36U, /*!< Allow Read, Write in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_RW_USER_RWX    = 0x37U, /*!< Allow Read, Write in supervisor mode; Allow Read, Write, Execute in user mode */
        MPU_SUPERVISOR_RWX_USER_NONE  = 0x38U, /*!< Allow Read, Write, Execute in supervisor mode; No access in user mode */
        MPU_SUPERVISOR_RWX_USER_X     = 0x39U, /*!< Allow Read, Write, Execute in supervisor mode; Allow Execute in user mode */
        MPU_SUPERVISOR_RWX_USER_W     = 0x3AU, /*!< Allow Read, Write, Execute in supervisor mode; Allow Write in user mode */
        MPU_SUPERVISOR_RWX_USER_WX    = 0x3BU, /*!< Allow Read, Write, Execute in supervisor mode; Allow Write, Execute in user mode */
        MPU_SUPERVISOR_RWX_USER_R     = 0x3CU, /*!< Allow Read, Write, Execute in supervisor mode; Allow Read in user mode */
        MPU_SUPERVISOR_RWX_USER_RX    = 0x3DU, /*!< Allow Read, Write, Execute in supervisor mode; Allow Read, Execute in user mode */
        MPU_SUPERVISOR_RWX_USER_RW    = 0x3EU, /*!< Allow Read, Write, Execute in supervisor mode; Allow Read, Write in user mode */
        MPU_SUPERVISOR_RWX_USER_RWX   = 0x3FU  /*!< Allow Read, Write, Execute in supervisor mode; Allow Read, Write, Execute in user mode */
    } mpu_specific_access_permission_t;
    #endif

/*!
 * @brief Lock configuration.
 * Implements : mpu_lock_t_Class
 */
typedef enum
{
    MPU_UNLOCK     = 0U, /*!< Unlocked */
#if FEATURE_SMPU_HAS_OWNER_LOCK
    MPU_OWNER_LOCK = 1U, /*!< Locked by the master wrote this register and LCK bit
                         Attempted writes by other masters are ignored */
#endif/* FEATURE_SMPU_HAS_OWNER_LOCK */
    MPU_ALL_LOCK   = 3U  /*!< Attempted writes to any location in the region descriptor are ignored */
} mpu_lock_t;

/*!
 * @brief Region lock configuration structure.
 * Implements : mpu_region_lock_t_Class
 */
typedef struct
{
    uint8_t    regionNum;   /*!< Region number */
#if FEATURE_SMPU_HAS_OWNER_LOCK
    uint8_t    masterOwner; /*!< Master number */
#endif /* FEATURE_SMPU_HAS_OWNER_LOCK */
    mpu_lock_t lockConfig;  /*!< Lock configuration */
} mpu_region_lock_t;

/*!
 * @brief Defines the region extension structure for the MPU over SMPU
 * Implements : mpu_extension_smpu_region_t_Class
 */
typedef struct
{
#if (FEATURE_SMPU_HAS_SPECIFIC_ACCESS_RIGHT_COUNT != 0U)
    bool                                   specAccessEnable;   /*!< Specific access enable */
    const mpu_specific_access_permission_t *specAccessSet;     /*!< Specific access configurations */
#endif
    bool                                   cacheInhibitEnable; /*!< Cache Inhibit */
    mpu_lock_t                             lockConfig;         /*!< Lock configuration */
} mpu_extension_smpu_region_t;
#endif /* defined(MPU_OVER_SMPU) */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name MPU PAL API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes memory protection unit by allocating regions
 * and granting access rights for masters.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] regionCnt The number of regions configured.
 * @param[in] configPtr The pointer to regions configuration structure, see #mpu_region_config_t.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_ERROR       : Operation failed due to invalid master number
 *                               or the region was locked by another master
 *                               or all masters are locked.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_Init(const mpu_instance_t * const instance,
                  uint8_t regionCnt,
                  const mpu_region_config_t * configPtr);

/*!
 * @brief De-initializes memory protection unit by reseting all regions
 * and masters to default and disable module.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_ERROR       : Operation failed due to the region was locked by another master
 *                               or all masters are locked.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_Deinit(const mpu_instance_t * const instance);

/*!
 * @brief Gets default region configuration. Grants all access rights for masters;
 * disable PID and cache; unlock region descriptor.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[out] masterAccRight The pointer to master configuration structure, see #mpu_master_access_permission_t.
 *                            The length of array should be defined by number of masters supported by hardware.
 * @param[out] regionConfig The pointer to default region configuration structure, see #mpu_region_config_t.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_GetDefautRegionConfig(const mpu_instance_t * const instance,
                                   mpu_master_access_permission_t * masterAccRight,
                                   mpu_region_config_t * regionConfig);

/*!
 * @brief Updates region configuration.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] regionNum The region number.
 * @param[in] configPtr The pointer to region configuration structure, see #mpu_region_config_t.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_ERROR       : Operation failed due to invalid master number
 *                               or the region was locked by another master
 *                               or all masters are locked.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_UpdateRegion(const mpu_instance_t * const instance,
                          uint8_t regionNum,
                          const mpu_region_config_t * configPtr);

/*!
 * @brief Enables or disables an exist region configuration.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] regionNum The region number.
 * @param[in] enable Valid state
 *            - true  : Enable region.
 *            - false : Disable region.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_ERROR       : Operation failed due to the region was locked by another master
 *                               or all masters are locked.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_EnableRegion(const mpu_instance_t * const instance,
                          uint8_t regionNum,
                          bool enable);

/*!
 * @brief Checks and gets the access error detail information
 * then clear error flag if the error caused by a master.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] channel The error capture channel
 *                    For MPU: corresponding with the slave port number
 *                    For SMPU: corresponding with the the master number
 * @param[out] errPtr The pointer to access error info structure, see #mpu_error_info_t.
 * @return operation status
 *        - true  : An error has occurred.
 *        - false : No error has occurred
 *                  or the operation was unsupported.
 */
bool MPU_GetError(const mpu_instance_t * const instance,
                  uint8_t channel,
                  mpu_error_info_t * errPtr);

#if defined(MPU_OVER_SMPU)
/*!
 * @brief Updates lock configuration on a region.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] regionNum The region number.
 * @param[in] lockConfig The lock configuration, see #mpu_lock_t.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_ERROR       : Operation failed due to the region was locked by another master
 *                               or all masters are locked.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_UpdateRegionLock(const mpu_instance_t * const instance,
                              uint8_t regionNum,
                              mpu_lock_t lockConfig);

/*!
 * @brief Reports the region lock status.
 *
 * @param[in] instance The pointer to MPU instance number.
 * @param[in] regionNum The region number.
 * @param[out] regionLock The pointer to region lock status, see mpu_region_lock_t.
 * @return operation status
 *        - STATUS_SUCCESS     : Operation was successful.
 *        - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t MPU_GetRegionLockInfo(const mpu_instance_t * const instance,
                               uint8_t regionNum,
                               mpu_region_lock_t * regionLock);
#endif /* defined(MPU_OVER_SMPU) */


/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* MPU_PAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
