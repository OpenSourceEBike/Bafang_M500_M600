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

#ifndef FLEXIO_I2C_DRIVER_H
#define FLEXIO_I2C_DRIVER_H

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
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * The macro in question is used for development time checking of the maximum buffer size
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
 * @addtogroup flexio_i2c_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* Length of the DMA scatter-gather chain for Tx. Two blocks are needed, but as the fist one is 
   loaded directly in the TCD, only one block needs to be in RAM  */
#define FLEXIO_I2C_DMA_TX_CHAIN_LENGTH      (1U)
/* Length of the DMA scatter-gather chain for Rx. Six blocks are needed, but as the fist one is 
   loaded directly in the TCD, only five blocks needs to be in RAM  */
#define FLEXIO_I2C_DMA_RX_CHAIN_LENGTH      (5U)
/* Total size of the DMA scatter-gather buffer. Add 1 because STCD_SIZE already compensates for the 
   first stcd not being loaded in RAM */
#define FLEXIO_I2C_DMA_BUF_SIZE   (STCD_SIZE(FLEXIO_I2C_DMA_TX_CHAIN_LENGTH + FLEXIO_I2C_DMA_RX_CHAIN_LENGTH + 1U))
/*! @endcond */


/*******************************************************************************
* Definitions
******************************************************************************/

 /*!
 * @brief Master configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_i2c master at initialization time.
 * Implements : flexio_i2c_master_user_config_t_Class
 */
typedef struct
{
    uint16_t slaveAddress;              /*!< Slave address, 7-bit */
    flexio_driver_type_t driverType;    /*!< Driver type: interrupts/polling/DMA */
    uint32_t baudRate;                  /*!< Baud rate in hertz */
    uint8_t sdaPin;                     /*!< Flexio pin to use as I2C SDA pin */
    uint8_t sclPin;                     /*!< Flexio pin to use as I2C SCL pin */
    i2c_master_callback_t callback;         /*!< User callback function. Note that this function will be
                                            called from the interrupt service routine, so its 
                                            execution time should be as small as possible. It can be 
                                            NULL if it is not needed */
    void *callbackParam;                /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;               /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;               /*!< Tx DMA channel number. Only used in DMA mode */
} flexio_i2c_master_user_config_t;


/*!
 * @brief Master internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the FLEXIO_I2C_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_I2C_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    flexio_common_state_t flexioCommon;    /* Common flexio drivers structure */
    const uint8_t *txData;                 /* Transmit buffer */
    uint8_t *rxData;                       /* Receive buffer */
    uint32_t txRemainingBytes;             /* Number of remaining bytes to be transmitted */
    uint32_t rxRemainingBytes;             /* Number of remaining bytes to be received */
    uint8_t rxDMAChannel;                  /* Rx DMA channel number */
    uint8_t txDMAChannel;                  /* Tx DMA channel number */
    i2c_master_callback_t callback;        /* User callback function */
    void *callbackParam;                   /* Parameter for the callback function */
    uint16_t slaveAddress;                 /* Slave address */
    uint16_t eventCount;                   /* Number of timer events, needed for long transfers */
    flexio_driver_type_t driverType;       /* Driver type: interrupts/polling/DMA */
    status_t status;                       /* Current status of the driver */
    bool receive;                          /* Transfer direction, true = receive, false = transmit */
    bool addrReceived;                     /* Indicated start of receive (after address transmission) */
    bool driverIdle;                       /* Idle/busy state of the driver */
    semaphore_t idleSemaphore;             /* Semaphore used by blocking functions */
    bool blocking;                         /* Specifies if the current transfer is blocking */
    bool sendStop;                         /* Specifies if STOP condition must be generated after current transfer */
    uint8_t sdaPin;                        /* Flexio pin to use as I2C SDA pin */
    uint8_t sclPin;                        /* Flexio pin to use as I2C SCL pin */
    uint8_t dummyDmaIdle;                  /* Dummy location for DMA transfers */
    uint8_t dummyDmaStop;                  /* Dummy location for DMA transfers */
    uint8_t dummyDmaReceive;               /* Dummy location for DMA transfers */
    uint8_t dmaReceiveTxStop0;             /* Stores setting for setting Tx shifter stop bit to 0 */
    uint8_t dmaReceiveTxStop1;             /* Stores setting for setting Tx shifter stop bit to 1 */
    uint8_t dmaReceiveRxStop1;             /* Stores setting for setting Rx shifter stop bit to 1 */
    uint8_t lastReload;                    /* Counter value for the last timer reload. */
    uint8_t stcd[FLEXIO_I2C_DMA_BUF_SIZE]; /* Buffer for DMA scatter-gather operations */
/*! @endcond */
} flexio_i2c_master_state_t;


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLEXIO_I2C Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the FLEXIO_I2C master mode driver
 *
 * This function initializes the FLEXIO_I2C driver in master mode.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_I2C master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the FLEXIO_I2C master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_I2C_DRV_MasterDeinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterInit(uint32_t instance,
                                   const flexio_i2c_master_user_config_t * userConfigPtr,
                                   flexio_i2c_master_state_t * master);

                                              
/*!
 * @brief De-initialize the FLEXIO_I2C master mode driver
 *
 * This function de-initializes the FLEXIO_I2C driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterDeinit(flexio_i2c_master_state_t * master);


/*!
 * @brief Set the baud rate for any subsequent I2C communication
 *
 * This function sets the baud rate (SCL frequency) for the I2C master.
 * Note that due to module limitation not any baud rate can be achieved. The driver 
 * will set a baud rate as close as possible to the requested baud rate, but there may 
 * still be substantial differences, for example if requesting a high baud rate while 
 * using a low-frequency FlexIO clock. The application should call 
 * FLEXIO_I2C_DRV_MasterGetBaudRate() after FLEXIO_I2C_DRV_MasterSetBaudRate() to check 
 * what baud rate was actually set.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param baudRate  the desired baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterSetBaudRate(flexio_i2c_master_state_t * master, uint32_t baudRate);


/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured I2C baud rate.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param baudRate  the current baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterGetBaudRate(flexio_i2c_master_state_t * master, uint32_t *baudRate);


/*!
 * @brief Set the slave address for any subsequent I2C communication
 *
 * This function sets the slave address which will be used for any future transfer.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param address   slave address, 7-bit
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterSetSlaveAddr(flexio_i2c_master_state_t * master, const uint16_t address);


/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * This function starts the transmission of a block of data to the currently 
 * configured slave address and returns immediately. 
 * The rest of the transmission is handled by the interrupt service routine (if the driver 
 * is initialized in interrupt mode) or by the FLEXIO_I2C_DRV_MasterGetStatus function (if 
 * the driver is initialized in polling mode). 
 * Use FLEXIO_I2C_DRV_MasterGetStatus() to check the progress of the transmission.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterSendData(flexio_i2c_master_state_t * master,
                                       const uint8_t * txBuff,
                                       uint32_t txSize,
                                       bool sendStop);



/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * This function sends a block of data to the currently configured slave address, and 
 * only returns when the transmission is complete.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterSendDataBlocking(flexio_i2c_master_state_t * master,
                                               const uint8_t * txBuff,
                                               uint32_t txSize,
                                               bool sendStop,
                                               uint32_t timeout);


/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * This function starts the reception of a block of data from the currently 
 * configured slave address and returns immediately. 
 * The rest of the transmission is handled by the interrupt service routine (if the driver 
 * is initialized in interrupt mode) or by the FLEXIO_I2C_DRV_MasterGetStatus function (if 
 * the driver is initialized in polling mode). 
 * Use FLEXIO_I2C_DRV_MasterGetStatus() to check the progress of the reception.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterReceiveData(flexio_i2c_master_state_t * master,
                                          uint8_t * rxBuff,
                                          uint32_t rxSize,
                                          bool sendStop);


/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * This function receives a block of data from the currently configured slave address, 
 * and only returns when the transmission is complete.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterReceiveDataBlocking(flexio_i2c_master_state_t * master,
                                                  uint8_t * rxBuff,
                                                  uint32_t rxSize,
                                                  bool sendStop,
                                                  uint32_t timeout);


/*!
 * @brief Aborts a non-blocking I2C master transaction
 *
 * This function aborts a non-blocking I2C transfer.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterTransferAbort(flexio_i2c_master_state_t * master);


/*!
 * @brief Get the status of the current non-blocking I2C master transaction
 *
 * This function returns the current status of a non-blocking I2C master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master          Pointer to the FLEXIO_I2C master driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_I2C_DRV_MasterGetStatus(flexio_i2c_master_state_t * master, uint32_t *bytesRemaining);


/*!
 * @brief Returns default configuration structure for FLEXIO_I2C
 *
 * @param userConfigPtr    Pointer to the FLEXIO_I2C user configuration structure.
 */
void FLEXIO_I2C_DRV_GetDefaultConfig(flexio_i2c_master_user_config_t * userConfigPtr);


/*!
 * @brief Generate nine clock on SCL line to free SDA line
 *
 * This function should be called when SDA line be stuck in low.
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @return    STATUS_BUSY: Driver is transfering data, 
 *            STATUS_SUCCESS: Function started generating clock
 */
status_t FLEXIO_I2C_DRV_GenerateNineClock(flexio_i2c_master_state_t *master);

/*!
 *
 * @brief Indicate the generation nine clock is done or not
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @return    STATUS_BUSY: Clock generation not done yet, 
 *            STATUS_SUCCESS: Device finished generating nine clock
 */
status_t FLEXIO_I2C_DRV_StatusGenerateNineClock(flexio_i2c_master_state_t *master);


/*!
 *
 * @brief Check status whether SDA or SCL line be low or high
 *
 * @param master    Pointer to the FLEXIO_I2C master driver context structure.
 * @param sdaLine   true - function return status of SDA line.
 *                  false - function return status of SCL line.
 * @return    true: Pin selected is high,  
 *            false: Pin selected is low
 */
bool FLEXIO_I2C_DRV_GetBusStatus(const flexio_i2c_master_state_t *master, bool sdaLine);

/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLEXIO_I2C_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
