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

#ifndef I2S_PAL_H
#define I2S_PAL_H

#include "i2s_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*!
 * @defgroup i2s_pal I2S PAL
 * @ingroup i2s_pal
 * @addtogroup i2s_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Defines the transfer type
 *
 * Implements : i2s_transfer_type_t_Class
 */
typedef enum
{
    I2S_USING_INTERRUPT  = 0U,    /*!< Driver uses interrupts for data transfers */
    I2S_USING_DMA         = 1U,    /*!< Driver uses DMA for data transfers */
} i2s_transfer_type_t;

/*!
 * @brief Master or slave
 *
 * Implements : i2s_mode_t_Class
 */
typedef enum
{
    I2S_MASTER = 0U,    /*!< Generate bit clock and word select signal */
    I2S_SLAVE  = 1U,    /*!< Receive bit clock and word select signal */
} i2s_mode_t;

/*!
 * @brief I2S user configuration structure
 *
 * Implements : i2s_user_config_t_Class
 */
typedef struct
{
    i2s_transfer_type_t transferType;       /*!< Driver type: interrupts/DMA */
    i2s_mode_t mode;                        /*!< Master or slave */
    uint32_t baudRate;                      /*!< Baud rate in hertz */
    uint8_t wordWidth;                      /*!< Number of bits in a word - multiple of 8. The word size in transfer functions depends on this parameter
                                                 Word size for each buffer read/write is 1 byte, 2 bytes or 4 byte, whichever larger and close to
                                                 wordWidth the most */
    i2s_callback_t callback;                /*!< User callback function. Can be null if not needed. */
    void *callbackParam;                    /*!< Parameter for the callback function */
    uint8_t rxDMAChannel;                   /*!< Rx DMA channel number. Only used in DMA mode */
    uint8_t txDMAChannel;                   /*!< Tx DMA channel number. Only used in DMA mode */
    void *extension;                        /*!< This field will be used to add extra settings to the basic configuration like FlexIO data pins */
} i2s_user_config_t;

#if (defined (I2S_OVER_FLEXIO))
/*!
 * @brief Defines the extension structure for the I2S over FLEXIO
 */
typedef struct
{
    uint8_t txPin;                          /*!< Flexio pin to use for transmit */
    uint8_t rxPin;                          /*!< Flexio pin to use for receive */
    uint8_t sckPin;                         /*!< Flexio pin to use for serial clock */
    uint8_t wsPin;                          /*!< Flexio pin to use for word select */
} extension_flexio_for_i2s_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initializes the I2S module
 *
 * This function initializes and enables the requested I2S module.
 * Note that when use I2S over SAI, tx and rx line are separated with SAI0,
 * with other SAI instance tx and rx share one line.
 *
 * @param[in] instance Instance number
 * @param[in] config The configuration structure
  */
status_t I2S_Init(const i2s_instance_t* instance, const i2s_user_config_t * config);

/*!
 * @brief De-initializes the I2S module
 *
 * This function de-initializes the I2S module.
 *
 * @param[in] instance Instance number
 */
status_t I2S_Deinit(const i2s_instance_t* instance);

/*!
 * @brief Returns the i2s baud rate.
 *
 * This function returns the i2s configured baud rate, only call this when instance is configured as master.
 *
 * @param instance  Instance number.
 * @param[out] configuredBaudRate configured baud rate.
 * @return STATUS_SUCCESS
 */
status_t I2S_GetBaudRate(const i2s_instance_t* instance,
                         uint32_t * configuredBaudRate);

/*!
 * @brief Keep sending.
 *
 * This function must be called in callback function when TX_EMPTY event is reported to ensure tx operation
 * working continuously.
 *
 * @param[in] instance Instance number
 * @param[in] txBuff pointer to the data to be transferred
 * @param[in] txSize length in words of the data to be transferred
 */
status_t I2S_SetTxBuffer(const i2s_instance_t* instance,
                         const uint8_t * txBuff,
                         uint32_t txSize);

/*!
 * @brief Keep receiving.
 *
 * This function must be called in callback function when RX_FULl event is reported to ensure rx operation
 * working continuously.
 *
 * @param[in] instance Instance number
 * @param[in] rxBuff pointer to the data to be transferred
 * @param[in] rxSize length in words of the data to be transferred
 */
status_t I2S_SetRxBuffer(const i2s_instance_t* instance,
                         uint8_t * rxBuff,
                         uint32_t rxSize);

/*!
 * @brief Perform a blocking I2S transmission
 *
 * This function sends a block of data and
 * only returns when the transmission is complete.
 *
 * @param[in] instance Instance number
 * @param[in] txBuff pointer to the data to be transferred
 * @param[in] txSize length in words of the data to be transferred
 * @param[in] timeout timeout value in milliseconds
 * @return    Error, success or timed out status
 */
status_t I2S_SendDataBlocking(
        const i2s_instance_t* instance,
        const uint8_t * txBuff,
        uint32_t txSize,
        uint32_t timeout);

/*!
 * @brief Perform a non-blocking I2S transmission
 *
 * This function sends a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode).
 *
 * @param[in] instance Instance number
 * @param[in] txBuff pointer to the data to be transferred
 * @param[in] txSize length in words of the data to be transferred
 */
status_t I2S_SendData(const i2s_instance_t* instance,
                      const uint8_t * txBuff,
                      uint32_t txSize);

/*!
 * @brief Get the status of the current I2S transfer
 *
 * @param instance  Instance number
 * @param countRemaining Pointer to value that is populated with the number of words that
 *      have been sent in the active transfer
 * @return The transmit status.
 * @retval STATUS_SUCCESS The transmit has completed successfully.
 * @retval STATUS_BUSY The transmit is still in progress. @countRemaining will be
 *     filled with the number of words that have been transferred so far.
 * @retval STATUS_I2S_ABORTED The transmit was aborted.
 * @retval STATUS_TIMEOUT A timeout was reached.
 * @retval STATUS_ERROR An error occurred.
 */
status_t I2S_GetStatus(const i2s_instance_t* instance,
                       uint32_t * countRemaining);

/*!
 * @brief Perform a blocking I2S reception
 *
 * This function receives a block of data and
 * only returns when the transmission is complete.
 *
 * @param[in] instance Instance number
 * @param rxBuff    pointer to the receive buffer
 * @param rxSize    length in words of the data to be received
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error, success or timed out status
 */
status_t I2S_ReceiveDataBlocking(
        const i2s_instance_t* instance,
        uint8_t * rxBuff,
        uint32_t rxSize,
        uint32_t timeout);

/*!
 * @brief Perform a non-blocking I2S reception
 *
 * This function receives a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode).
 *
 * @param[in] instance Instance number
 * @param[in] rxBuff pointer to the data to be transferred
 * @param[in] rxSize length in words of the data to be transferred
 */
status_t I2S_ReceiveData(const i2s_instance_t* instance,
                         uint8_t * rxBuff,
                         uint32_t rxSize);

/*!
 * @brief Terminates a non-blocking transfer early.
 *
 * @param instance  Instance number
 */
status_t I2S_Abort(const i2s_instance_t* instance);

/*!
 * @brief Return default configuration
 *
 * Return default config for 8 kHz sample rate, 16 bit sample width and 2 channels.
 *
 * @param[out] Pointer to configuration structure
 */
void I2S_GetDefaultConfig(i2s_user_config_t * const config);

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* I2S_PAL_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
