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

#ifndef FLEXIO_SPI_DRIVER_H
#define FLEXIO_SPI_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "flexio.h"
#include "edma_driver.h"
#include "callbacks.h"

/*!
 * @addtogroup flexio_spi_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*! @brief Order in which the data bits are transferred 
 * Implements : flexio_spi_transfer_bit_order_t_Class
 */
typedef enum
{
    FLEXIO_SPI_TRANSFER_MSB_FIRST        = 0U,  /*!< Transmit data starting with most significant bit */
    FLEXIO_SPI_TRANSFER_LSB_FIRST        = 1U,  /*!< Transmit data starting with least significant bit */
} flexio_spi_transfer_bit_order_t;

/*! @brief Size of transferred data in bytes 
 * Implements : flexio_spi_transfer_size_t_Class
 */
typedef enum
{
    FLEXIO_SPI_TRANSFER_1BYTE            = 1U,  /*!< Data size is 1-byte  */
    FLEXIO_SPI_TRANSFER_2BYTE            = 2U,  /*!< Data size is 2-bytes */
    FLEXIO_SPI_TRANSFER_4BYTE            = 4U,  /*!< Data size is 4-bytes */
} flexio_spi_transfer_size_t;


/*******************************************************************************
* Definitions
******************************************************************************/


 /*!
 * @brief Master configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_spi master at initialization time.
 * Implements : flexio_spi_master_user_config_t_Class
 */
typedef struct
{
    uint32_t baudRate;         /*!< Baud rate in hertz */
    flexio_driver_type_t driverType; /*!< Driver type: interrupts/polling/DMA */
    flexio_spi_transfer_bit_order_t bitOrder; /*!< Bit order: LSB-first / MSB-first */
    flexio_spi_transfer_size_t transferSize;  /*!< Transfer size in bytes: 1/2/4 */
    uint8_t clockPolarity;     /*!< Clock Polarity (CPOL) 0 = active-high clock; 1 = active-low clock */
    uint8_t clockPhase;        /*!< Clock Phase (CPHA) 0 = sample on leading clock edge; 1 = sample on trailing clock edge */
    uint8_t mosiPin;           /*!< Flexio pin to use as MOSI pin */
    uint8_t misoPin;           /*!< Flexio pin to use as MISO pin */
    uint8_t sckPin;            /*!< Flexio pin to use as SCK pin  */
    uint8_t ssPin;             /*!< Flexio pin to use as SS pin   */
    spi_callback_t callback;       /*!< User callback function. Note that this function will be 
                                            called from the interrupt service routine, so its 
                                            execution time should be as small as possible. It can be 
                                            NULL if it is not needed */
    void *callbackParam;       /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;      /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;      /*!< Tx DMA channel number. Only used in DMA mode */
} flexio_spi_master_user_config_t;

/*!
 * @brief Slave configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_spi slave at initialization time.
 * Implements : flexio_spi_slave_user_config_t_Class
 */
typedef struct
{
    flexio_driver_type_t driverType; /*!< Driver type: interrupts/polling/DMA */
    flexio_spi_transfer_bit_order_t bitOrder; /*!< Bit order: LSB-first / MSB-first */
    flexio_spi_transfer_size_t transferSize;  /*!< Transfer size in bytes: 1/2/4 */
    uint8_t clockPolarity;     /*!< Clock Polarity (CPOL) 0 = active-low clock; 1 = active-high clock */
    uint8_t clockPhase;        /*!< Clock Phase (CPHA) 0 = sample on leading clock edge; 1 = sample on trailing clock edge */
    uint8_t mosiPin;           /*!< Flexio pin to use as MOSI pin */
    uint8_t misoPin;           /*!< Flexio pin to use as MISO pin */
    uint8_t sckPin;            /*!< Flexio pin to use as SCK pin  */
    uint8_t ssPin;             /*!< Flexio pin to use as SS pin   */
    spi_callback_t callback;       /*!< User callback function. Note that this function will be 
                                            called from the interrupt service routine, so its 
                                            execution time should be as small as possible. It can be 
                                            NULL if it is not needed */
    void *callbackParam;       /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;      /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;      /*!< Tx DMA channel number. Only used in DMA mode */
} flexio_spi_slave_user_config_t;


/*!
 * @brief Master internal context structure
 *
 * This structure is used by the master-mode driver for its internal logic. It must
 * be provided by the application through the FLEXIO_SPI_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_SPI_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    flexio_common_state_t flexioCommon;        /* Common flexio drivers structure */
    const uint8_t *txData;                     /* Transmit buffer. */
    uint8_t *rxData;                           /* Receive buffer. */
    uint32_t txRemainingBytes;                 /* Number of remaining bytes to be transmitted. */
    uint32_t rxRemainingBytes;                 /* Number of remaining bytes to be received. */
    uint32_t dummyDmaData;                     /* Dummy location for DMA transfers. */
    uint8_t rxDMAChannel;                      /* Rx DMA channel number */
    uint8_t txDMAChannel;                      /* Tx DMA channel number */
    flexio_driver_type_t driverType;           /* Driver type: interrupts/polling/DMA */
    flexio_spi_transfer_bit_order_t bitOrder;  /* Bit order: LSB-first / MSB-first */
    flexio_spi_transfer_size_t transferSize;   /* Transfer size in bytes: 1/2/4 */
    status_t status;                           /* Current status of the driver */
    bool driverIdle;                           /* Idle/busy state of the driver */
    semaphore_t idleSemaphore;                 /* Semaphore used by blocking functions */
    bool blocking;                             /* Specifies if the current transfer is blocking */
    bool master;                               /* Specifies if the current instance was initialized as master */
    spi_callback_t callback;                   /* User callback function */
    void *callbackParam;                       /* Parameter for the callback function */
/*! @endcond */
} flexio_spi_master_state_t;

/*!
 * @brief Slave internal context structure
 *
 * This structure is used by the slave-mode driver for its internal logic. It must
 * be provided by the application through the FLEXIO_SPI_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_SPI_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef flexio_spi_master_state_t flexio_spi_slave_state_t;



/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLEXIO_SPI Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the FLEXIO_SPI master mode driver
 *
 * This function initializes the FLEXIO_SPI driver in master mode.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_SPI master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the FLEXIO_SPI master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_SPI_DRV_MasterDeinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterInit(uint32_t instance,
                                   const flexio_spi_master_user_config_t * userConfigPtr,
                                   flexio_spi_master_state_t * master);

                                              
/*!
 * @brief De-initialize the FLEXIO_SPI master mode driver
 *
 * This function de-initializes the FLEXIO_SPI driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterDeinit(flexio_spi_master_state_t *master);


/*!
 * @brief Set the baud rate for any subsequent SPI communication
 *
 * This function sets the baud rate for the SPI master.
 * Note that due to module limitation not any baud rate can be achieved. The driver 
 * will set a baud rate as close as possible to the requested baud rate, but there may 
 * still be substantial differences, for example if requesting a high baud rate while 
 * using a low-frequency FlexIO clock. The application should call 
 * FLEXIO_SPI_DRV_MasterGetBaudRate() after FLEXIO_SPI_DRV_MasterSetBaudRate() to check 
 * what baud rate was actually set.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @param baudRate  the desired baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterSetBaudRate(flexio_spi_master_state_t *master, uint32_t baudRate);


/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured SPI baud rate.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @param baudRate  the current baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterGetBaudRate(flexio_spi_master_state_t *master, uint32_t *baudRate);


/*!
 * @brief Perform a non-blocking SPI master transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer 
 * and then returns, leaving the transfer to complete asynchronously).
 * FLEXIO_SPI_DRV_MasterGetStatus() can be called to check the status of the transfer.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterTransfer(flexio_spi_master_state_t *master,
                                       const uint8_t *txData,
                                       uint8_t *rxData,
                                       uint32_t dataSize);


/*!
 * @brief Perform a blocking SPI master transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is blocking, the function only returns when the transfer 
 * is complete.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterTransferBlocking(flexio_spi_master_state_t *master,
                                               const uint8_t *txData,
                                               uint8_t *rxData,
                                               uint32_t dataSize,
                                               uint32_t timeout);


                                                             
/*!
 * @brief Aborts a non-blocking SPI master transaction
 *
 * This function aborts a non-blocking SPI transfer.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterTransferAbort(flexio_spi_master_state_t *master);


/*!
 * @brief Get the status of the current non-blocking SPI master transaction
 *
 * This function returns the current status of a non-blocking SPI master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master    Pointer to the FLEXIO_SPI master driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_MasterGetStatus(flexio_spi_master_state_t *master, uint32_t *bytesRemaining);


/*!
 * @brief Initialize the FLEXIO_SPI slave mode driver
 *
 * This function initializes the FLEXIO_SPI driver in slave mode.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_SPI slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param slave     Pointer to the FLEXIO_SPI slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_SPI_DRV_SlaveDeinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveInit(uint32_t instance,
                                  const flexio_spi_slave_user_config_t * userConfigPtr,
                                  flexio_spi_slave_state_t * slave);

/*!
 * @brief De-initialize the FLEXIO_SPI slave mode driver
 *
 * This function de-initializes the FLEXIO_SPI driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveDeinit(flexio_spi_slave_state_t *slave);


/*!
 * @brief Perform a non-blocking SPI slave transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer 
 * and then returns, leaving the transfer to complete asynchronously).
 * FLEXIO_SPI_DRV_SlaveGetStatus() can be called to check the status of the transfer.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveTransfer(flexio_spi_slave_state_t *slave,
                                                    const uint8_t *txData,
                                                    uint8_t *rxData,
                                                    uint32_t dataSize);


/*!
 * @brief Perform a blocking SPI slave transaction
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for 
 * txData or rxData. The transfer is blocking, the function only returns when the transfer 
 * is complete.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param txData  pointer to the data to be transmitted
 * @param rxData  pointer to the buffer where to store received data
 * @param dataSize  length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveTransferBlocking(flexio_spi_slave_state_t *slave,
                                                            const uint8_t *txData,
                                                            uint8_t *rxData,
                                                            uint32_t dataSize,
                                                            uint32_t timeout);


/*!
 * @brief Aborts a non-blocking SPI slave transaction
 *
 * This function aborts a non-blocking SPI transfer.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveTransferAbort(flexio_spi_slave_state_t *slave);


/*!
 * @brief Get the status of the current non-blocking SPI slave transaction
 *
 * This function returns the current status of a non-blocking SPI slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the FLEXIO_SPI slave driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_SPI_DRV_SlaveGetStatus(flexio_spi_slave_state_t *slave, uint32_t *bytesRemaining);


/*!
 * @brief Returns default configuration structure for FLEXIO_SPI master
 *
 * @param userConfigPtr    Pointer to the FLEXIO_SPI user configuration structure.
 */
void FLEXIO_SPI_DRV_MasterGetDefaultConfig(flexio_spi_master_user_config_t * userConfigPtr);


/*!
 * @brief Returns default configuration structure for FLEXIO_SPI slave
 *
 * @param userConfigPtr    Pointer to the FLEXIO_SPI user configuration structure.
 */
void FLEXIO_SPI_DRV_SlaveGetDefaultConfig(flexio_spi_slave_user_config_t * userConfigPtr);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLEXIO_SPI_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
