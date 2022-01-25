/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
 * @file lpi2c_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 *
 */

#if !defined(LPI2C_DRIVER_H)
#define LPI2C_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"
#include "edma_driver.h"
#include "osif.h"
#include "callbacks.h"


/*!
 * @addtogroup lpi2c_drv LPI2C Driver
 * @ingroup lpi2c
 * @brief Low Power Inter-Integrated Circuit Driver
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* Size of the master command queue. Worst case: 5 commands in High-Speed receive with 10-bit address:
   START + master code, REP START + addr_1 + tx, addr_2, REP START + addr_1 + rx, receive command */
#define LPI2C_MASTER_CMD_QUEUE_SIZE   5U
/*! @endcond */

/*! @brief I2C operating modes
 * Implements : lpi2c_mode_t_Class
 */
typedef enum
{
    LPI2C_STANDARD_MODE      = 0x0U,   /*!< Standard-mode (Sm), bidirectional data transfers up to 100 kbit/s */
    LPI2C_FAST_MODE          = 0x1U,   /*!< Fast-mode (Fm), bidirectional data transfers up to 400 kbit/s */
#if(LPI2C_HAS_FAST_PLUS_MODE)
    LPI2C_FASTPLUS_MODE      = 0x2U,   /*!< Fast-mode Plus (Fm+), bidirectional data transfers up to 1 Mbit/s */
#endif
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    LPI2C_HIGHSPEED_MODE     = 0x3U,   /*!< High-speed Mode (Hs-mode), bidirectional data transfers up to 3.4 Mbit/s */
#endif
#if(LPI2C_HAS_ULTRA_FAST_MODE)
    LPI2C_ULTRAFAST_MODE     = 0x4U    /*!< Ultra Fast Mode (UFm), unidirectional data transfers up to 5 Mbit/s */
#endif
} lpi2c_mode_t;

/*! @brief Type of LPI2C transfer (based on interrupts or DMA).
 * Implements : lpi2c_transfer_type_t_Class
 */
typedef enum
{
   LPI2C_USING_DMA         = 0,    /*!< The driver will use DMA to perform I2C transfer */
   LPI2C_USING_INTERRUPTS  = 1,    /*!< The driver will use interrupts to perform I2C transfer */
} lpi2c_transfer_type_t;

/*******************************************************************************
* Definitions
******************************************************************************/

/*!
 * @brief Defines the example structure
 *
 * This structure is used as an example.
 */

 /*!
 * @brief Master configuration structure
 *
 * This structure is used to provide configuration parameters for the LPI2C master at initialization time.
 * Implements : lpi2c_master_user_config_t_Class
 */
typedef struct
{
    uint16_t slaveAddress;                      /*!< Slave address, 7-bit or 10-bit */
    bool is10bitAddr;                           /*!< Selects 7-bit or 10-bit slave address */
    lpi2c_mode_t operatingMode;                 /*!< I2C Operating mode */
    uint32_t baudRate;                          /*!< The baud rate in hertz to use with current slave device */
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    uint32_t baudRateHS;                        /*!< Baud rate for High-speed mode. Unused in other operating modes */
    uint8_t masterCode;                         /*!< Master code for High-speed mode. Valid range: 0-7. Unused in other operating modes */
#endif
    lpi2c_transfer_type_t transferType;         /*!< Type of LPI2C transfer */
    uint8_t dmaChannel;                         /*!< Channel number for DMA channel. If DMA mode isn't used this field will be ignored. */
    i2c_master_callback_t masterCallback;     /*!< Master callback function. Note that this function will be
                                                     called from the interrupt service routine at the end of a transfer,
                                                     so its execution time should be as small as possible. It can be
                                                     NULL if you want to check manually the status of the transfer. */
    void *callbackParam;                        /*!< Parameter for the master callback function */
} lpi2c_master_user_config_t;

/*!
 * @brief Slave configuration structure
 *
 * This structure is used to provide configuration parameters for the LPI2C slave at initialization time.
 * Implements : lpi2c_slave_user_config_t_Class
 */
typedef struct
{
    uint16_t slaveAddress;                      /*!< Slave address, 7-bit or 10-bit */
    bool is10bitAddr;                           /*!< Selects 7-bit or 10-bit slave address */
    lpi2c_mode_t operatingMode;                 /*!< I2C Operating mode */
    bool slaveListening;                        /*!< Slave mode (always listening or on demand only) */
    lpi2c_transfer_type_t transferType;         /*!< Type of LPI2C transfer */
    uint8_t dmaChannel;                       /*!< Channel number for DMA rx channel. If DMA mode isn't used this field will be ignored. */
    i2c_slave_callback_t slaveCallback;       /*!< Slave callback function. Note that this function will be
                                                  called from the interrupt service routine, so its
                                                  execution time should be as small as possible. It can be
                                                  NULL if the slave is not in listening mode
                                                  (slaveListening = false) */
    void *callbackParam;                        /*!< Parameter for the slave callback function */
} lpi2c_slave_user_config_t;

/*!
 * @brief Baud rate structure
 *
 * This structure is used for setting or getting the baud rate.
 * Implements : lpi2c_baud_rate_params_t_Class
 */
typedef struct
{
    uint32_t baudRate;
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    uint32_t baudRateHS;
#endif
} lpi2c_baud_rate_params_t;

/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* LPI2C master commands */
typedef enum
{
    LPI2C_MASTER_COMMAND_TRANSMIT         = 0U,  /*!< Transmit DATA[7:0] */
    LPI2C_MASTER_COMMAND_RECEIVE          = 1U,  /*!< Receive (DATA[7:0] + 1) bytes */
    LPI2C_MASTER_COMMAND_STOP             = 2U,  /*!< Generate STOP condition */
    LPI2C_MASTER_COMMAND_RECEIVE_DISCARD  = 3U,  /*!< Receive and discard (DATA[7:0] + 1) bytes */
    LPI2C_MASTER_COMMAND_START            = 4U,  /*!< Generate START and transmit address in DATA[7:0] */
    LPI2C_MASTER_COMMAND_START_NACK       = 5U,  /*!< Generate START and transmit address in DATA[7:0], expect a NACK to be returned */
    LPI2C_MASTER_COMMAND_START_HS         = 6U,  /*!< Generate START and transmit address in DATA[7:0] in high speed mode */
    LPI2C_MASTER_COMMAND_START_NACK_HS    = 7U,  /*!< Generate START and transmit address in DATA[7:0] in high speed mode, expect a NACK to be returned */
} lpi2c_master_command_t;
/*! @endcond */

/*! @cond DRIVER_INTERNAL_USE_ONLY */
/* Master software command queue */
typedef struct
{
    lpi2c_master_command_t cmd[LPI2C_MASTER_CMD_QUEUE_SIZE];
    uint8_t data[LPI2C_MASTER_CMD_QUEUE_SIZE];
    uint8_t writeIdx;
    uint8_t readIdx;
} lpi2c_master_cmd_queue_t;
/*! @endcond */

/*!
 * @brief Master internal context structure
 *
 * This structure is used by the master-mode driver for its internal logic. It must
 * be provided by the application through the LPI2C_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using LPI2C_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    lpi2c_master_cmd_queue_t cmdQueue;      /* Software queue for commands, when LPI2C FIFO is not big enough */
    uint8_t * rxBuff;                       /* Pointer to receive data buffer */
    uint32_t rxSize;                        /* Size of receive data buffer */
    const uint8_t * txBuff;                 /* Pointer to transmit data buffer */
    uint32_t txSize;                        /* Size of transmit data buffer */
    volatile status_t status;               /* Status of last driver operation */
    lpi2c_mode_t operatingMode;             /* I2C Operating mode */
    uint16_t slaveAddress;                  /* Slave address */
    volatile bool i2cIdle;                  /* Idle/busy state of the driver */
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    uint8_t masterCode;                     /* Master code for High-speed mode */
    bool highSpeedInProgress;               /* High-speed communication is in progress */
    uint32_t baudRateHS;                    /*!< Baud rate for High-speed mode. Unused in other operating modes */
#endif
    bool sendStop;                          /* Specifies if STOP condition must be generated after current transfer */
    bool is10bitAddr;                       /* Selects 7-bit or 10-bit slave address */
    semaphore_t idleSemaphore;              /* Semaphore used by blocking functions */
    bool blocking;                          /* Specifies if the current transfer is blocking */
    lpi2c_transfer_type_t transferType;     /* Type of LPI2C transfer */
    uint8_t dmaChannel;                     /* Channel number for DMA rx channel */
    i2c_master_callback_t masterCallback; /* Master callback function */
    void *callbackParam;                    /* Parameter for the master callback function */
    bool abortedTransfer;                   /* Specifies if master has aborted transfer */
    uint32_t baudrate;                      /* Baud rate in Hz*/
/*! @endcond */
} lpi2c_master_state_t;

/*!
 * @brief Slave internal context structure
 *
 * This structure is used by the slave-mode driver for its internal logic. It must
 * be provided by the application through the LPI2C_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using LPI2C_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    status_t status;                  /* The I2C slave status */
    volatile bool isTransferInProgress;     /* Slave is busy because of an ongoing transfer */
    uint32_t txSize;                        /* Size of the TX buffer*/
    uint32_t rxSize;                        /* Size of the RX buffer*/
    const uint8_t * txBuff;                 /* Pointer to Tx Buffer*/
    uint8_t * rxBuff;                       /* Pointer to Rx Buffer*/
    lpi2c_mode_t operatingMode;             /* I2C Operating mode */
    bool slaveListening;                    /* Slave mode (always listening or on demand only) */
    bool is10bitAddress;                    /* Specifies if 10-bit or 7-bit address */
    uint8_t repeatedStarts;                 /* Specifies the number of repeated starts */
    bool txUnderrunWarning;                 /* Possible slave tx underrun */
    semaphore_t idleSemaphore;              /* Semaphore used by blocking functions */
    bool blocking;                          /* Specifies if the current transfer is blocking */
    lpi2c_transfer_type_t transferType;     /* Type of LPI2C transfer */
    uint8_t dmaChannel;                   /* Channel number for DMA channel */                  /* Channel number for DMA tx channel */
    i2c_slave_callback_t slaveCallback;   /* Slave callback function */
    void *callbackParam;                    /* Parameter for the slave callback function */
/*! @endcond */
} lpi2c_slave_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name LPI2C Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initialize the LPI2C master mode driver
 *
 * This function initializes the LPI2C driver in master mode.
 *
 * @param instance  LPI2C peripheral instance number
 * @param userConfigPtr    Pointer to the LPI2C master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the LPI2C master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using LPI2C_DRV_MasterDeinit().
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterInit(uint32_t instance,
                                    const lpi2c_master_user_config_t * userConfigPtr,
                                    lpi2c_master_state_t * master);


/*!
 * @brief De-initialize the LPI2C master mode driver
 *
 * This function de-initializes the LPI2C driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterDeinit(uint32_t instance);


/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured baud rate.
 *
 * @param instance  LPI2C peripheral instance number
 * @param baudRate  structure that contains the current baud rate in hertz
 *                  and the baud rate in hertz for High-speed mode (unused
 *                  in other modes, can be NULL)
 */
void LPI2C_DRV_MasterGetBaudRate(uint32_t instance, lpi2c_baud_rate_params_t *baudRate);


/*!
 * @brief Set the baud rate for any subsequent I2C communication
 *
 * This function sets the baud rate (SCL frequency) for the I2C master. It can also
 * change the operating mode. If the operating mode is High-Speed, a second baud rate
 * must be provided for high-speed communication.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency protocol clock for the LPI2C module. The application should
 * call LPI2C_DRV_MasterGetBaudRate() after LPI2C_DRV_MasterSetBaudRate() to check
 * what baud rate was actually set.
 *
 * @param instance  LPI2C peripheral instance number
 * @param operatingMode  I2C operating mode
 * @param baudRate  structure that contains the baud rate in hertz to use by current slave device
 *                  and also the baud rate in hertz for High-speed mode (unused in other modes)
 *
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterSetBaudRate(uint32_t instance,
                                const lpi2c_mode_t operatingMode,
                                const lpi2c_baud_rate_params_t baudRate);


/*!
 * @brief Set the slave address for any subsequent I2C communication
 *
 * This function sets the slave address which will be used for any future
 * transfer initiated by the LPI2C master.
 *
 * @param instance  LPI2C peripheral instance number
 * @param address   slave address, 7-bit or 10-bit
 * @param is10bitAddr   specifies if provided address is 10-bit
 */
void LPI2C_DRV_MasterSetSlaveAddr(uint32_t instance, const uint16_t address, const bool is10bitAddr);


/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * This function starts the transmission of a block of data to the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine.
 * Use LPI2C_DRV_MasterGetSendStatus() to check the progress of the transmission.
 *
 * @param instance  LPI2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterSendData(uint32_t instance,
                                            const uint8_t * txBuff,
                                            uint32_t txSize,
                                            bool sendStop);


/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * This function sends a block of data to the currently configured slave address, and
 * only returns when the transmission is complete.
 *
 * @param instance  LPI2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the transmission
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterSendDataBlocking(uint32_t instance,
                                    const uint8_t * txBuff,
                                    uint32_t txSize,
                                    bool sendStop,
                                    uint32_t timeout);


/*!
 * @brief Abort a non-blocking I2C Master transmission or reception
 *
 * @param instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterAbortTransferData(uint32_t instance);


/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * This function starts the reception of a block of data from the currently
 * configured slave address and returns immediately.
 * The rest of the reception is handled by the interrupt service routine.
 * Use LPI2C_DRV_MasterGetReceiveStatus() to check the progress of the reception.
 *
 * @param instance  LPI2C peripheral instance number
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterReceiveData(uint32_t  instance,
                                               uint8_t * rxBuff,
                                               uint32_t rxSize,
                                               bool sendStop);


/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * This function receives a block of data from the currently configured slave address,
 * and only returns when the transmission is complete.
 *
 * @param instance  LPI2C peripheral instance number
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param sendStop    specifies whether or not to generate stop condition after the reception
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterReceiveDataBlocking(uint32_t instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize,
                                       bool sendStop,
                                       uint32_t timeout);


/*!
 * @brief Return the current status of the I2C master transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param instance  LPI2C peripheral instance number
 * @param bytesRemaining   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t *bytesRemaining);


/*!
 * @brief Handle master operation when I2C interrupt occurs
 *
 * This is the interrupt service routine for the LPI2C master mode driver. It
 * handles the rest of the transfer started by one of the send/receive functions.
 *
 * @param instance  LPI2C peripheral instance number
 */
void LPI2C_DRV_MasterIRQHandler(uint32_t instance);


/*!
 * @brief Initialize the I2C slave mode driver
 *
 * @param instance  LPI2C peripheral instance number
 * @param userConfigPtr    Pointer to the LPI2C slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param slave     Pointer to the LPI2C slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using LPI2C_DRV_SlaveDeinit().
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveInit(uint32_t instance,
                               const lpi2c_slave_user_config_t * userConfigPtr,
                               lpi2c_slave_state_t * slave);


/*!
 * @brief De-initialize the I2C slave mode driver
 *
 * This function de-initializes the LPI2C driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveDeinit(uint32_t instance);


/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function provides a buffer from which the LPI2C slave-mode driver can
 * transmit data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events LPI2C_SLAVE_EVENT_TX_REQ or
 * LPI2C_SLAVE_EVENT_TX_EMPTY.
 *
 * @param instance  LPI2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveSetTxBuffer(uint32_t instance,
                                            const uint8_t * txBuff,
                                            uint32_t txSize);


/*!
 * @brief Provide a buffer for receiving data.
 *
 * This function provides a buffer in which the LPI2C slave-mode driver can
 * store received data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events LPI2C_SLAVE_EVENT_RX_REQ or
 * LPI2C_SLAVE_EVENT_RX_FULL.
 *
 * @param instance  LPI2C peripheral instance number
 * @param rxBuff    pointer to the data to be transferred
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveSetRxBuffer(uint32_t instance,
                                            uint8_t * rxBuff,
                                            uint32_t  rxSize);


/*!
 * @brief Perform a non-blocking send transaction on the I2C bus
 *
 * Performs a non-blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the transmission and returns immediately. The rest of the transmission is
 * handled by the interrupt service routine.
 * Use LPI2C_DRV_SlaveGetTransmitStatus() to check the progress of the transmission.
 *
 * @param instance  LPI2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveSendData(uint32_t instance,
                                   const uint8_t * txBuff,
                                   uint32_t txSize);


/*!
 * @brief Perform a blocking send transaction on the I2C bus
 *
 * Performs a blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the transmission and then waits for the transfer to complete before
 * returning.
 *
 * @param instance  LPI2C peripheral instance number
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveSendDataBlocking(uint32_t    instance,
                                           const uint8_t *  txBuff,
                                           uint32_t txSize,
                                           uint32_t timeout);


/*!
 * @brief Perform a non-blocking receive transaction on the I2C bus
 *
 * Performs a non-blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the reception and returns immediately. The rest of the reception is
 * handled by the interrupt service routine.
 * Use LPI2C_DRV_SlaveGetReceiveStatus() to check the progress of the reception.
 *
 * @param instance  LPI2C peripheral instance number
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveReceiveData(uint32_t   instance,
                                       uint8_t * rxBuff,
                                       uint32_t  rxSize);


/*!
 * @brief Perform a blocking receive transaction on the I2C bus
 *
 * Performs a blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the reception and then waits for the transfer to complete before
 * returning.
 *
 * @param instance  LPI2C peripheral instance number
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @param timeout   timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveReceiveDataBlocking(uint32_t instance,
                                                uint8_t  * rxBuff,
                                                uint32_t rxSize,
                                                uint32_t timeout);


/*!
 * @brief Return the current status of the I2C slave transfer
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param instance  LPI2C peripheral instance number
 * @param bytesRemaining   the number of remaining bytes in the active I2C transfer
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveGetTransferStatus(uint32_t instance,
                                            uint32_t *bytesRemaining);


/*!
 * @brief Abort a non-blocking I2C Master transmission or reception
 *
 * @param instance  LPI2C peripheral instance number
 * @return    Error or success status returned by API
 */
status_t LPI2C_DRV_SlaveAbortTransferData(uint32_t instance);


/*!
 * @brief Handle slave operation when I2C interrupt occurs
 *
 * This is the interrupt service routine for the LPI2C slave mode driver. It
 * handles any transfer initiated by an I2C master and notifies the application
 * via the provided callback when relevant events occur.
 *
 * @param instance  LPI2C peripheral instance number
 */
void LPI2C_DRV_SlaveIRQHandler(uint32_t instance);

/*!
 * @brief Gets the default configuration structure for master
 *
 * The default configuration structure is:
 *
 * @param config Pointer to configuration structure
 */
void LPI2C_DRV_MasterGetDefaultConfig(lpi2c_master_user_config_t * config);

/*!
 * @brief Gets the default configuration structure for slave
 *
 * The default configuration structure is:
 *
 * @param config Pointer to configuration structure
 */
void LPI2C_DRV_SlaveGetDefaultConfig(lpi2c_slave_user_config_t *config);

/*!
 * @brief Set bus idle timeout for LPI2C
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START condition
 *
 * @param baseAddr  base address of the LPI2C module
 * @param timeout   bus idle timeout period in clock cycle. Zero means no bus idle timeout
 */
void LPI2C_DRV_SetMasterBusIdleTimeout(uint32_t instance, uint16_t timeout);

#if defined(S32K11x_SERIES)
/*!
 * @brief Handler for both slave and master operation when I2C interrupt occurs
 *
 * This is the interrupt service routine for the LPI2C slave and master mode driver. It
 * handles any transfer initiated by an I2C master and notifies the application
 * via the provided callback when relevant events occur.
 *
 * @param instance  LPI2C peripheral instance number
 */
void LPI2C_DRV_ModuleIRQHandler(uint32_t instance);
#endif

/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* LPI2C_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
