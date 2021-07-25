/*
 * Copyright 2017 NXP
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

#ifndef FLASH_MX25L6433F_DRIVER_H
#define FLASH_MX25L6433F_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"
#include "quadspi_driver.h"

/*!
 * @addtogroup flash_mx25l6433f_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*! @brief Flash protection direction
 * Implements :  flash_mx25l6433f_prot_dir_t_Class
 */
typedef enum
{
    FLASH_MX25L6433F_PROT_DIR_TOP        = 0x00U,  /*!< Top flash area protected.    */
    FLASH_MX25L6433F_PROT_DIR_BOTTOM     = 0x01U,  /*!< Bottom flash area protected. */
} flash_mx25l6433f_prot_dir_t;


/*! @brief Flash device drive strength
 * Implements :  flash_mx25l6433f_drv_strength_t_Class
 */
typedef enum
{
    FLASH_MX25L6433F_DRV_STRENGTH_HIGH       = 0x00U,  /*!< Full driver strength.      */
    FLASH_MX25L6433F_DRV_STRENGTH_LOW        = 0x01U,  /*!< Low (1/4) driver strength. */
} flash_mx25l6433f_drv_strength_t;


/*! @brief Size of flash protected area
 * Implements :  flash_mx25l6433f_prot_size_t_Class
 */
typedef enum
{
    FLASH_MX25L6433F_PROT_SIZE_0         = 0x00U,  /*!< Entire flash unprotected.    */
    FLASH_MX25L6433F_PROT_SIZE_64K       = 0x01U,  /*!< 64k flash area protected.    */
    FLASH_MX25L6433F_PROT_SIZE_128K      = 0x02U,  /*!< 128k flash area protected.   */
    FLASH_MX25L6433F_PROT_SIZE_256K      = 0x03U,  /*!< 256k flash area protected.   */
    FLASH_MX25L6433F_PROT_SIZE_512K      = 0x04U,  /*!< 512k flash area protected.   */
    FLASH_MX25L6433F_PROT_SIZE_1M        = 0x05U,  /*!< 1M flash area protected.     */
    FLASH_MX25L6433F_PROT_SIZE_2M        = 0x06U,  /*!< 2M flash area protected.     */
    FLASH_MX25L6433F_PROT_SIZE_4M        = 0x07U,  /*!< 4M flash area protected.     */
    FLASH_MX25L6433F_PROT_SIZE_8M        = 0x08U,  /*!< Entire flash area protected. */
} flash_mx25l6433f_prot_size_t;


/*******************************************************************************
* Definitions
******************************************************************************/

 /*!
 * @brief Driver configuration structure
 *
 * This structure is used to provide configuration parameters for the mx25l6433f flash driver
 * at initialization time.
 * Implements : flash_mx25l6433f_user_config_t_Class
 */
typedef struct
{
    bool dmaSupport;                                        /*!< Enables DMA support in the driver                    */
    flash_mx25l6433f_drv_strength_t outputDriverStrength;   /*!< Output driver level of the device                    */
} flash_mx25l6433f_user_config_t;


/*!
 * @brief Driver internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the FLASH_MX25L6433F_DRV_Init() function, then
 * it cannot be freed until the driver is de-initialized using FLASH_MX25L6433F_DRV_Deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint8_t lastCommand;                                    /* Last command executed by the driver   */
    uint8_t suspendedCommand;                               /* Suspended command                     */
    flash_mx25l6433f_drv_strength_t outputDriverStrength;   /* Output driver level of the device     */
    bool dmaSupport;                                        /* Enables DMA support in the driver     */
/*! @endcond */
} flash_mx25l6433f_state_t;



/*! @brief Flash protection settings
 * Implements :  flash_mx25l6433f_secure_lock_t_Class
 */
typedef struct
{
    bool userAreaLock;               /*!< User secured area (1st 4KB region) is locked.     */
    bool factoryAreaLock;            /*!< Factory secured area (2nd 4KB region) is locked.  */
} flash_mx25l6433f_secure_lock_t;


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLASH_MX25L6433F Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initializes the serial flash memory driver
 *
 * This function initializes the MX25L6433F flash driver and prepares it for operation.
 *
 * @param instance     QuadSPI peripheral instance number
 * @param userConfigPtr    Pointer to the MX25L6433F flash driver user configuration structure. The
 *                         function reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the MX25L6433F flash driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLASH_MX25L6433F_DRV_Deinit().
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Init(uint32_t instance,
                                   const flash_mx25l6433f_user_config_t * userConfigPtr,
                                   flash_mx25l6433f_state_t * state);


/*!
 * @brief De-initialize the MX25L6433F flash driver
 *
 * This function de-initializes the MX25L6433F flash driver. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Deinit(uint32_t instance);


/*!
 * @brief Configure protected area of the device
 *
 * This function configures the size and location (top or bottom) of protected area.
 * Note that due to device limitations, once the protected area is set to BOTTOM it
 * cannot be changed back (the setting is "one time program")
 *
 * @param instance     QuadSPI peripheral instance number
 * @param direction    Location of protected area (top or bottom)
 * @param size         Size of protected area
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_SetProtection(uint32_t instance, 
                                            flash_mx25l6433f_prot_dir_t direction,
                                            flash_mx25l6433f_prot_size_t size);


/*!
 * @brief Get protected area of the device
 *
 * @param instance     QuadSPI peripheral instance number
 * @param direction    Location of protected area (top or bottom)
 * @param size         Size of protected area
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_GetProtection(uint32_t instance, 
                                            flash_mx25l6433f_prot_dir_t * direction,
                                            flash_mx25l6433f_prot_size_t * size);


/*!
 * @brief Locks the customer sector of the secured OTP area
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_SetSecureLock(uint32_t instance);


/*!
 * @brief Get lock status of the secured OTP area
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lock         Lock status of the secured OTP area
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_GetSecureLock(uint32_t instance,  
                                            flash_mx25l6433f_secure_lock_t *lock);


/*!
 * @brief Read data from serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Start address for read operation
 * @param data         Buffer where to store read data
 * @param size         Size of data buffer
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Read(uint32_t instance, 
                                   uint32_t address,
                                   uint8_t * data,
                                   uint32_t size);


/*!
 * @brief Erase a 4k sector in the serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Address of sector to be erased
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Erase4K(uint32_t instance, uint32_t address);


/*!
 * @brief Erase a 32k block in the serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Address of block to be erased
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Erase32K(uint32_t instance, uint32_t address);


/*!
 * @brief Erase a 64k block in the serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Address of block to be erased
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Erase64K(uint32_t instance, uint32_t address);


/*!
 * @brief Erases the entire serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_EraseAll(uint32_t instance);


/*!
 * @brief Checks whether or not an area in the serial flash is erased
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Start address of area to be verified
 * @param size         Size of area to be verified
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_EraseVerify(uint32_t instance, uint32_t address, uint32_t size);


/*!
 * @brief Writes data in serial flash
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Start address of area to be programmed
 * @param data         Data to be programmed in flash
 * @param size         Size of data buffer
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Program(uint32_t instance,
                                      uint32_t address,
                                      const uint8_t * data,
                                      uint32_t size);


/*!
 * @brief Verifies the correctness of the programmed data
 *
 * @param instance     QuadSPI peripheral instance number
 * @param address      Start address of area to be verified
 * @param data         Data to be verified
 * @param size         Size of data buffer
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_ProgramVerify(uint32_t instance, 
                                            uint32_t address,
                                            const uint8_t * data,
                                            uint32_t size);


/*!
 * @brief Get the status of the last operation
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_GetStatus(uint32_t instance);


/*!
 * @brief Reset the serial flash device
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Reset(uint32_t instance);


/*!
 * @brief Enters OTP mode
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_EnterOTP(uint32_t instance);


/*!
 * @brief Exits OTP mode
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_ExitOTP(uint32_t instance);


/*!
 * @brief Enters Deep Power Down mode
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_EnterDPD(uint32_t instance);


/*!
 * @brief Exits Deep Power Down mode
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_ExitDPD(uint32_t instance);


/*!
 * @brief Suspends current program or erase operation
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Suspend(uint32_t instance);


/*!
 * @brief Resume a suspended program or erase operation
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t FLASH_MX25L6433F_DRV_Resume(uint32_t instance);


/*!
 * @brief Returns default configuration structure for FLASH_MX25L6433F
 *
 * @param userConfigPtr    Pointer to the FLASH_MX25L6433F user configuration structure.
 */
void FLASH_MX25L6433F_DRV_GetDefaultConfig(flash_mx25l6433f_user_config_t * userConfigPtr);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLASH_MX25L6433F_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
