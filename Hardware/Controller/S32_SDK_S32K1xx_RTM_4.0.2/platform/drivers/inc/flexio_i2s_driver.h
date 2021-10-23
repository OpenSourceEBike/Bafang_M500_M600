/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

#ifndef FLEXIO_I2S_DRIVER_H
#define FLEXIO_I2S_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "flexio.h"
#include "edma_driver.h"
#include "callbacks.h"

/**
 * @page misra_violations MISRA-C:2012 violations
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
 */

/*!
 * @addtogroup flexio_i2s_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*******************************************************************************
* Definitions
******************************************************************************/


/*!
 * @brief Master configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_i2s master at initialization time.
 * Implements : flexio_i2s_master_user_config_t_Class
 */
typedef struct
{
    flexio_driver_type_t driverType;        /*!< Driver type: interrupts/polling/DMA */
    uint32_t baudRate;                      /*!< Baud rate in hertz */
    uint8_t bitsWidth;                      /*!< Number of bits in a word - multiple of 8 */
    uint8_t txPin;                          /*!< Flexio pin to use for transmit */
    uint8_t rxPin;                          /*!< Flexio pin to use for receive */
    uint8_t sckPin;                         /*!< Flexio pin to use for serial clock */
    uint8_t wsPin;                          /*!< Flexio pin to use for word select */
    i2s_callback_t callback;                /*!< User callback function. Note that this function will be
                                                  called from the interrupt service routine, so its
                                                  execution time should be as small as possible. It can be
                                                  NULL if it is not needed */
    void *callbackParam;                     /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;                    /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;                    /*!< Tx DMA channel number. Only used in DMA mode */
} flexio_i2s_master_user_config_t;


/*!
 * @brief Slave configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_i2s slave at initialization time.
 * Implements : flexio_i2s_slave_user_config_t_Class
 */
typedef struct
{
    flexio_driver_type_t driverType;        /*!< Driver type: interrupts/polling/DMA */
    uint8_t bitsWidth;                      /*!< Number of bits in a word - multiple of 8 */
    uint8_t txPin;                          /*!< Flexio pin to use for transmit */
    uint8_t rxPin;                          /*!< Flexio pin to use for receive */
    uint8_t sckPin;                         /*!< Flexio pin to use for serial clock */
    uint8_t wsPin;                          /*!< Flexio pin to use for word select */
    i2s_callback_t callback;                /*!< User callback function. Note that this function will be
                                                  called from the interrupt service routine, so its
                                                  execution time should be as small as possible. It can be
                                                  NULL if it is not needed */
    void *callbackParam;                    /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;                   /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;                   /*!< Tx DMA channel number. Only used in DMA mode */
} flexio_i2s_slave_user_config_t;


/*!
 * @brief Master internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the FLEXIO_I2S_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_I2S_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    flexio_common_state_t flexioCommon;    /* Common flexio drivers structure */
    const uint8_t *txData;                 /* Transmit buffer. */
    uint8_t *rxData;                       /* Receive buffer. */
    uint32_t txRemainingBytes;             /* Number of remaining bytes to be transmitted. */
    uint32_t rxRemainingBytes;             /* Number of remaining bytes to be received. */
    uint32_t dummyDmaData;                 /* Dummy location for DMA transfers. */
    uint8_t rxDMAChannel;                  /* Rx DMA channel number */
    uint8_t txDMAChannel;                  /* Tx DMA channel number */
    uint8_t bitsWidth;                     /* Number of bits in a word */
    uint8_t byteWidth;                     /* Number of bytes in a word */
    bool master;                           /* Current instance is in master mode */
    flexio_driver_type_t driverType;       /* Driver type: interrupts/polling/DMA */
    status_t status;                       /* Current status of the driver */
    bool driverIdle;                       /* Idle/busy state of the driver */
    semaphore_t idleSemaphore;             /* Semaphore used by blocking functions */
    bool blocking;                         /* Specifies if the current transfer is blocking */
    i2s_callback_t callback;               /* User callback function */
    void *callbackParam;                   /* Parameter for the callback function */
/*! @endcond */
} flexio_i2s_master_state_t;

/*!
 * @brief Slave internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the FLEXIO_I2S_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_I2S_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef flexio_i2s_master_state_t flexio_i2s_slave_state_t;


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLEXIO_I2S Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the FLEXIO_I2S master mode driver
 *
 * This function initializes the FLEXIO_I2S driver in master mode.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_I2S master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the FLEXIO_I2S master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_I2S_DRV_MasterDeinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterInit(uint32_t instance,
                                   const flexio_i2s_master_user_config_t * userConfigPtr,
                                   flexio_i2s_master_state_t * master);


/*!
 * @brief De-initialize the FLEXIO_I2S master mode driver
 *
 * This function de-initializes the FLEXIO_I2S driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterDeinit(flexio_i2s_master_state_t * master);


/*!
 * @brief Set the baud rate and bit width for any subsequent I2S communication
 *
 * This function sets the baud rate (SCK frequency) and bit width for the I2S master.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency FlexIO clock. The application should call
 * FLEXIO_I2S_DRV_MasterGetBaudRate() after FLEXIO_I2S_DRV_MasterSetConfig() to check
 * what baud rate was actually set.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param baudRate  the desired baud rate in hertz
 * @param bitsWidth  number of bits per word
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterSetConfig(flexio_i2s_master_state_t * master,
                                        uint32_t baudRate,
                                        uint8_t bitsWidth);


/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured I2S baud rate.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param baudRate  the current baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterGetBaudRate(flexio_i2s_master_state_t * master, uint32_t *baudRate);


/*!
 * @brief Perform a non-blocking send transaction on the I2S bus
 *
 * This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_MasterGetStatus() to check the progress of the transmission.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterSendData(flexio_i2s_master_state_t * master,
                                       const uint8_t * txBuff,
                                       uint32_t txSize);



/*!
 * @brief Perform a blocking send transaction on the I2S bus
 *
 * This function sends a block of data, and
 * only returns when the transmission is complete.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterSendDataBlocking(flexio_i2s_master_state_t * master,
                                               const uint8_t * txBuff,
                                               uint32_t txSize,
                                               uint32_t timeout);


/*!
 * @brief Perform a non-blocking receive transaction on the I2S bus
 *
 * This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_MasterGetStatus() to check the progress of the reception.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterReceiveData(flexio_i2s_master_state_t * master,
                                          uint8_t * rxBuff,
                                          uint32_t rxSize);


/*!
 * @brief Perform a blocking receive transaction on the I2S bus
 *
 * This function receives a block of data and only returns when the reception is complete.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterReceiveDataBlocking(flexio_i2s_master_state_t * master,
                                                  uint8_t * rxBuff,
                                                  uint32_t rxSize,
                                                  uint32_t timeout);


/*!
 * @brief Aborts a non-blocking I2S master transaction
 *
 * This function aborts a non-blocking I2S transfer.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterTransferAbort(flexio_i2s_master_state_t * master);


/*!
 * @brief Get the status of the current non-blocking I2S master transaction
 *
 * This function returns the current status of a non-blocking I2S master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterGetStatus(flexio_i2s_master_state_t * master, uint32_t *bytesRemaining);


/*!
 * @brief Provide a buffer for receiving data
 *
 * This function can be used to provide a new buffer for receiving data to the driver.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterSetRxBuffer(flexio_i2s_master_state_t *master,
                                          uint8_t * rxBuff,
                                          uint32_t rxSize);


/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function can be used to provide a new buffer for transmitting data to the driver.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param master    Pointer to the FLEXIO_I2S master driver context structure.
 * @param txBuff    pointer to the buffer containing transmit data
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_MasterSetTxBuffer(flexio_i2s_master_state_t *master,
                                          const uint8_t * txBuff,
                                          uint32_t txSize);


/*!
 * @brief Initialize the FLEXIO_I2S slave mode driver
 *
 * This function initializes the FLEXIO_I2S driver in slave mode.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_I2S slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param slave     Pointer to the FLEXIO_I2S slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_I2S_DRV_SlaveDeinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveInit(uint32_t instance,
                                  const flexio_i2s_slave_user_config_t * userConfigPtr,
                                  flexio_i2s_slave_state_t * slave);


/*!
 * @brief De-initialize the FLEXIO_I2S slave mode driver
 *
 * This function de-initializes the FLEXIO_I2S driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveDeinit(flexio_i2s_slave_state_t * slave);


/*!
 * @brief Set the bit width for any subsequent I2S communication
 *
 * This function sets the bit width for the I2S slave.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param bitsWidth  number of bits per word
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveSetConfig(flexio_i2s_slave_state_t * slave,
                                       uint8_t bitsWidth);


/*!
 * @brief Perform a non-blocking send transaction on the I2S bus
 *
 * This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_SlaveGetStatus() to check the progress of the transmission.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveSendData(flexio_i2s_slave_state_t * slave,
                                                    const uint8_t * txBuff,
                                                    uint32_t txSize);


/*!
 * @brief Perform a blocking send transaction on the I2S bus
 *
 * This function sends a block of data, and
 * only returns when the transmission is complete.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveSendDataBlocking(flexio_i2s_slave_state_t * slave,
                                                            const uint8_t * txBuff,
                                                            uint32_t txSize,
                                                            uint32_t timeout);

/*!
 * @brief Perform a non-blocking receive transaction on the I2S bus
 *
 * This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use FLEXIO_I2S_DRV_SlaveGetStatus() to check the progress of the reception.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveReceiveData(flexio_i2s_slave_state_t * slave,
                                                       uint8_t * rxBuff,
                                                       uint32_t rxSize);


/*!
 * @brief Perform a blocking receive transaction on the I2S bus
 *
 * This function receives a block of data and only returns when the reception is complete.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveReceiveDataBlocking(flexio_i2s_slave_state_t * slave,
                                                               uint8_t * rxBuff,
                                                               uint32_t rxSize,
                                                               uint32_t timeout);


/*!
 * @brief Aborts a non-blocking I2S slave transaction
 *
 * This function aborts a non-blocking I2S transfer.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveTransferAbort(flexio_i2s_slave_state_t * slave);


/*!
 * @brief Get the status of the current non-blocking I2S slave transaction
 *
 * This function returns the current status of a non-blocking I2S slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveGetStatus(flexio_i2s_slave_state_t * slave, uint32_t *bytesRemaining);


/*!
 * @brief Provide a buffer for receiving data
 *
 * This function can be used to provide a driver with a new buffer for receiving data.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveSetRxBuffer(flexio_i2s_slave_state_t *slave,
                                                       uint8_t * rxBuff,
                                                       uint32_t rxSize);


/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function can be used to provide a driver with a new buffer for transmitting data.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param slave    Pointer to the FLEXIO_I2S slave driver context structure.
 * @param txBuff    pointer to the buffer containing transmit data
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2S_DRV_SlaveSetTxBuffer(flexio_i2s_slave_state_t *slave,
                                                       const uint8_t * txBuff,
                                                       uint32_t txSize);


/*!
 * @brief Returns default configuration structure for FLEXIO_I2S master
 *
 * @param userConfigPtr    Pointer to the FLEXIO_I2S user configuration structure.
 */
void FLEXIO_I2S_DRV_MasterGetDefaultConfig(flexio_i2s_master_user_config_t * userConfigPtr);


/*!
 * @brief Returns default configuration structure for FLEXIO_I2S slave
 *
 * @param userConfigPtr    Pointer to the FLEXIO_I2S user configuration structure.
 */
void FLEXIO_I2S_DRV_SlaveGetDefaultConfig(flexio_i2s_slave_user_config_t * userConfigPtr);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLEXIO_I2S_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
