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
 * @file i2c_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Typedef is used for Flexio configuration structure.
 *
 *
 */

#ifndef SOURCES_I2C_PAL_H_
#define SOURCES_I2C_PAL_H_

#include "i2c_pal_cfg.h"
#include "i2c_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*! @file I2C_PAL.h */

/*!
 * @defgroup i2c_pal I2C PAL
 * @ingroup i2c_pal
 * @addtogroup i2c_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the mechanism to update the rx or tx buffers
 * Implements : i2c_pal_transfer_type_t_Class
 */
typedef enum
{
    I2C_PAL_USING_DMA         = 0U,    /*!< The driver will use DMA to perform I2C transfer */
    I2C_PAL_USING_INTERRUPTS  = 1U,    /*!< The driver will use interrupts to perform I2C transfer */
} i2c_pal_transfer_type_t;

/*!
 * @brief Defines the extension structure for the I2C over FLEXIO
 * Implements : extension_flexio_for_i2c_t_Class
 */
typedef struct
{
    uint8_t sclPin;    /*!< FlexIO pin for SCL */
    uint8_t sdaPin;    /*!< FlexIO pin for SDA */
} extension_flexio_for_i2c_t;


#if defined(I2C_OVER_SWI2C)
/*!
 * @brief Defines the extension structure for the I2C over SWI2C
 * Implements : extension_swi2c_for_i2c_t_Class
 */
typedef struct
{
    swi2c_pin_t *sclPin;       /* SCL pin for SWI2C */
    swi2c_pin_t *sdaPin;       /* SDA pin for SWI2C */
    swi2c_pin_t *sdaReadPin;   /* SDA read pin for SWI2C */
    swi2c_pin_t *sclReadPin;   /* SCL read pin for SWI2C */
}extension_swi2c_for_i2c_t;
#endif

/*!
 * @brief Defines the operation mode of the i2c pal 
 * Implements : i2c_operating_mode_t_Class 
 */
typedef enum
{
    I2C_PAL_STANDARD_MODE      = 0x0U,   /*!< Standard-mode (Sm), bidirectional data transfers up to 100 kbit/s */
    I2C_PAL_FAST_MODE          = 0x1U,   /*!< Fast-mode (Fm), bidirectional data transfers up to 400 kbit/s */
    I2C_PAL_FASTPLUS_MODE      = 0x2U,   /*!< Fast-mode Plus (Fm+), bidirectional data transfers up to 1 Mbit/s */
    I2C_PAL_HIGHSPEED_MODE     = 0x3U,   /*!< High-speed Mode (Hs-mode), bidirectional data transfers up to 3.4 Mbit/s */
    I2C_PAL_ULTRAFAST_MODE     = 0x4U    /*!< Ultra Fast Mode (UFm), unidirectional data transfers up to 5 Mbit/s */
} i2c_operating_mode_t;


/*!
 * @brief Defines the configuration structure for I2C master
 * Implements : i2c_master_t_Class
 */
typedef struct
{
    uint16_t slaveAddress;                        /*!< Slave address, 7-bit or 10-bit */
    bool is10bitAddr;                             /*!< Selects 7-bit or 10-bit slave address */
    uint32_t baudRate;                            /*!< Baud rate in hertz */
    uint8_t dmaChannel1;                          /*!< DMA channel number. Only used in DMA mode */
    uint8_t dmaChannel2;                          /*!< DMA channel used only by Flexio I2C which needs two DMA channels,
                                                       one for receiving and one for transmitting. */
    i2c_pal_transfer_type_t transferType;         /*!< Type of I2C transfer (interrupts or DMA) */
    i2c_operating_mode_t operatingMode;           /*!< I2C Operating mode */
    i2c_master_callback_t callback;               /*!< User callback function. Note that this function will be
                                                       called from the interrupt service routine, so its
                                                       execution time should be as small as possible. It can be
                                                       NULL if it is not needed */
    void *callbackParam;                          /*!< Parameter for the callback function */
    void *extension;                              /*!< This field will be used to add extra settings to the basic configuration like FlexIO pins */

} i2c_master_t;

/*!
 * @brief Defines the configuration structure for I2C slave
 * Implements : i2c_slave_t_Class
 */
typedef struct
{
    uint16_t slaveAddress;                        /*!< Slave address, 7-bit or 10-bit */
    bool is10bitAddr;                             /*!< Selects 7-bit or 10-bit slave address */
    bool slaveListening;                          /*!< Slave mode (always listening or on demand only) */
    i2c_operating_mode_t operatingMode;           /*!< I2C Operating mode */
    i2c_pal_transfer_type_t transferType;         /*!< Type of the I2C transfer */
    uint8_t dmaChannel;                           /*!< Channel number for DMA channel. If DMA mode is not supported or
                                                       is not used this field will be ignored. */
    i2c_slave_callback_t callback;                /*!< Callback function. */
    void *callbackParam;                          /*!< Parameter for the slave callback function */

}i2c_slave_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the I2C module in master mode
 *
 * This function initializes and enables the requested I2C module in master mode,
 * configuring the bus parameters.
 *
 * @param[in] instance The name of the instance
 * @param[in] config The configuration structure
 * @return    Error or success status returned by API
 */
status_t I2C_MasterInit(const i2c_instance_t * const instance, const i2c_master_t *config);

/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * This function starts the transmission of a block of data to the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine.
 *
 * @param instance  The name of the instance
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @return    Error or success status returned by API
 */
status_t I2C_MasterSendData(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop);

/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * This function sends a block of data to the currently configured slave address, and
 * only returns when the transmission is complete.
 *
 * @param instance  The name of the instance
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t I2C_MasterSendDataBlocking(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop, uint32_t timeout);

/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * This function starts the reception of a block of data from the currently
 * configured slave address and returns immediately.
 * The rest of the reception is handled by the interrupt service routine.
 *
 * @param instance  The name of the instance
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @return    Error or success status returned by API
 */
status_t I2C_MasterReceiveData(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop);

/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * This function receives a block of data from the currently configured slave address,
 * and only returns when the transmission is complete.
 *
 * @param instance  The name of the instance
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t I2C_MasterReceiveDataBlocking(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop, uint32_t timeout);

/*!
 * @brief Set the slave address for the I2C communication
 *
 * This function sets the slave address which will be used for any future
 * transfer initiated by the I2C master.
 *
 * @param instance  I2C peripheral instance number
 * @param address   slave 7-bit or 10-bit address
 */
status_t I2C_MasterSetSlaveAddress(const i2c_instance_t * const instance, const uint16_t address, const bool is10bitAddr);

/*!
 * @brief De-initializes the I2C master module
 *
 * This function de-initialized the I2C master module.
 *
 * @param[in] instance The name of the instance
 * @return    Error or success status returned by API
 */
status_t I2C_MasterDeinit(const i2c_instance_t * const instance);

/*!
 * @brief Gets the default configuration structure for master
 *
 * The default configuration structure is:
 *
 * @param[out] config Pointer to configuration structure
 * @return    Error or success status returned by API
 */
status_t I2C_GetDefaultMasterConfig(i2c_master_t *config);

/*!
 * @brief Gets the default configuration structure for slave
 *
 * The default configuration structure is:
 *
 * @param[out] config Pointer to configuration structure
 * @return    Error or success status returned by API
 */
status_t I2C_GetDefaultSlaveConfig(i2c_slave_t *config);
/*!
 * @brief Initializes the I2C module in slave mode
 *
 * This function initializes and enables the requested I2C module in slave mode,
 * configuring the bus parameters.
 *
 * @param[in] instance The name of the instance
 * @param[in] config The configuration structure
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveInit(const i2c_instance_t * const instance, const i2c_slave_t *config);

/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * Performs a non-blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the transmission and returns immediately. The rest of the transmission is
 * handled by the interrupt service routine.
 *
 * @param instance  The name of the instance
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveSendData(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize);

/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * Performs a blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the transmission and then waits for the transfer to complete before
 * returning.
 *
 * @param instance  The name of the instance
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveSendDataBlocking(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout);

/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * Performs a non-blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the reception and returns immediately. The rest of the reception is
 * handled by the interrupt service routine.
 *
 * @param instance  The name of the instance
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveReceiveData(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize);

/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * Performs a blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the reception and then waits for the transfer to complete before
 * returning.
 *
 * @param instance  The name of the instance
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveReceiveDataBlocking(const i2c_instance_t * const instance, uint8_t *rxBuff,uint32_t rxSize, uint32_t timeout);

/*!
 * @brief Provide a buffer for receiving data.
 *
 * This function provides a buffer in which the I2C slave-mode driver can
 * store received data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events I2C_SLAVE_EVENT_RX_REQ or
 * I2C_SLAVE_EVENT_RX_FULL.
 *
 * @param instance  I2C peripheral instance number
 * @param rxBuff    pointer to the data to be transferred
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveSetRxBuffer(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize);

/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function provides a buffer from which the I2C slave-mode driver can
 * transmit data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events I2C_SLAVE_EVENT_TX_REQ or
 * I2C_SLAVE_EVENT_TX_EMPTY.
 *
 * @param instance  I2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveSetTxBuffer(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize);

/*!
 * @brief De-initializes the i2c slave module
 *
 * This function de-initialized the i2c slave module.
 *
 * @param[in] instance The name of the instance
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveDeinit(const i2c_instance_t * const instance);

/*!
 * @brief Return the current status of the I2C master transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param instance  I2C peripheral instance number
 * @param bytesRemaining   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t I2C_MasterGetTransferStatus(const i2c_instance_t * const instance, uint32_t *bytesRemaining);

/*!
 * @brief Return the current status of the I2C slave transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param instance  I2C peripheral instance number
 * @param bytesRemaining   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveGetTransferStatus(const i2c_instance_t * const instance, uint32_t *bytesRemaining);

/*!
 * @brief Set the master baud rate for the I2C communication
 *
 * This function sets the master baud rate of the I2C master module.
 *
 * @param instance  I2C peripheral instance number
 * @param baudRate  the desired baud rate in Hz
 */
status_t I2C_MasterSetBaudRate(const i2c_instance_t * const instance, const i2c_master_t *config, uint32_t baudRate);

/*!
 * @brief Get the master baud rate for the I2C communication
 *
 * This function returns the master baud rate of the I2C master module.
 *
 * @param instance  I2C peripheral instance number
 * @return    the baud rate in Hz
 */
status_t I2C_MasterGetBaudRate(const i2c_instance_t * const instance, uint32_t *baudRate);

/*!
 * @brief Abort a non-blocking I2C Master transmission or reception
 *
 * @param instance  I2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t I2C_MasterAbortTransfer(const i2c_instance_t * const instance);

/*!
 * @brief Abort a non-blocking I2C slave transmission or reception
 *
 * @param instance  I2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t I2C_SlaveAbortTransfer(const i2c_instance_t * const instance);

/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* SOURCES_I2C_PAL_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/

