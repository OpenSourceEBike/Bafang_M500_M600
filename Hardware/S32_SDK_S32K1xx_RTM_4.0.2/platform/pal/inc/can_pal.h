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

#ifndef CAN_PAL_H
#define CAN_PAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "can_pal_cfg.h"
#include "can_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*!
 * @defgroup can_pal CAN PAL
 * @ingroup can_pal
 * @addtogroup can_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief CAN controller operation modes
 * Implements : can_operation_modes_t_Class
 */
typedef enum {
    CAN_NORMAL_MODE = 0U,        /*!< Normal mode or user mode */
    CAN_LOOPBACK_MODE = 2U,      /*!< Loop-back mode */
    CAN_DISABLE_MODE = 4U        /*!< Module disable mode */
} can_operation_modes_t;

/*! @brief CAN buffer payload sizes
 * Implements : can_fd_payload_size_t_Class
 */
typedef enum {
    CAN_PAYLOAD_SIZE_8 = 0,  /*!< CAN message buffer payload size in bytes */
    CAN_PAYLOAD_SIZE_16 ,    /*!< CAN message buffer payload size in bytes */
    CAN_PAYLOAD_SIZE_32 ,    /*!< CAN message buffer payload size in bytes */
    CAN_PAYLOAD_SIZE_64      /*!< CAN message buffer payload size in bytes */
} can_fd_payload_size_t;

/*! @brief CAN bit timing variables
 * Implements : can_time_segment_t_Class
 */
typedef struct {
    uint32_t propSeg;         /*!< Propagation segment */
    uint32_t phaseSeg1;       /*!< Phase segment 1 */
    uint32_t phaseSeg2;       /*!< Phase segment 2 */
    uint32_t preDivider;      /*!< Clock prescaler division factor */
    uint32_t rJumpwidth;      /*!< Resync jump width */
} can_time_segment_t;

/*! @brief CAN bitrate phase (nominal/data)
 * Implements : can_bitrate_phase_t_Class
 */
typedef enum {
    CAN_NOMINAL_BITRATE,        /*!< Nominal (FD arbitration) bitrate */
    CAN_FD_DATA_BITRATE         /*!< FD data bitrate */
} can_bitrate_phase_t;

/*! @brief CAN Message Buffer ID type
 * Implements : can_msg_id_type_t_Class
 */
typedef enum {
    CAN_MSG_ID_STD,         /*!< Standard ID */
    CAN_MSG_ID_EXT          /*!< Extended ID */
} can_msg_id_type_t;

/*! @brief CAN PE clock sources
 * Implements : can_clk_source_t_Class
 */
typedef enum {
    CAN_CLK_SOURCE_OSC    = 0U,  /*!< The CAN engine clock source is the oscillator clock. */
    CAN_CLK_SOURCE_PERIPH = 1U   /*!< The CAN engine clock source is the peripheral clock. */
} can_clk_source_t;

/*! @brief CAN buffer configuration
 * Implements : can_buff_config_t_Class
 */
typedef struct {
    bool enableFD;               /*!< Enable flexible data rate */
    bool enableBRS;              /*!< Enable bit rate switch inside a CAN FD frame */
    uint8_t fdPadding;           /*!< Value used for padding when the data length code (DLC)
                                     specifies a bigger payload size than the actual data length */
    can_msg_id_type_t idType;    /*!< Specifies whether the frame format is standard or extended */
    bool isRemote;               /*!< Specifies if the frame is standard or remote */
} can_buff_config_t;

/*! @brief CAN message format
 * Implements : can_message_t_Class
 */
typedef struct {
	uint32_t cs;       /*!< Code and Status*/
    uint32_t id;       /*!< ID of the message */
    uint8_t data[64];  /*!< Data bytes of the CAN message*/
    uint8_t length;    /*!< Length of payload in bytes */
} can_message_t;

/*! @brief CAN controller configuration
 * Implements : can_user_config_t_Class
 */
typedef struct
{
    uint32_t maxBuffNum;                  /*!< Set maximum number of buffers */
    can_operation_modes_t mode;           /*!< Set operation mode */
    can_clk_source_t peClkSrc;            /*!< The clock source of the CAN Protocol Engine (PE). */
    bool enableFD;                        /*!< Enable flexible data rate */
    can_fd_payload_size_t payloadSize;    /*!< Set size of buffer payload */
    can_time_segment_t nominalBitrate;    /*!< Bit timing segments for nominal bitrate */
    can_time_segment_t dataBitrate;       /*!< Bit timing segments for data bitrate */
    void *extension;                      /*!< This field will be used to add extra settings to the
                                               basic configuration like FlexCAN Rx FIFO settings */
} can_user_config_t;

#if (defined(CAN_OVER_FLEXCAN))
/*! @brief FlexCAN Rx FIFO configuration
 * Implements : extension_flexcan_rx_fifo_t_Class
 */
typedef struct
{
    flexcan_rx_fifo_id_filter_num_t numIdFilters;   /*!< The number of Rx FIFO ID filters needed */
    flexcan_rx_fifo_id_element_format_t idFormat;   /*!< RX FIFO ID format */
    flexcan_id_table_t *idFilterTable;              /*!< Rx FIFO ID table */
} extension_flexcan_rx_fifo_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the CAN module
 *
 * This function initializes and enables the requested CAN module.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver.
 *
 * @param[in] instance Instance information structure
 * @param[in] config The configuration structure
 * @return    STATUS_SUCCESS if successful;
 *            STATUS_ERROR if unsuccessful or invalid instance number;
 */
status_t CAN_Init(const can_instance_t * const instance,
                  const can_user_config_t *config);

/*!
 * @brief De-initializes the CAN module
 *
 * This function de-initializes the CAN module.
 *
 * @param[in] instance Instance information structure
 * @return    STATUS_SUCCESS if successful;
 *            STATUS_ERROR if unsuccessful or invalid instance number;
 */
status_t CAN_Deinit(const can_instance_t * const instance);

/*!
 * @brief Configures the CAN bitrate.
 *
 * This function configures the CAN bit timing variables.
 *
 * @param[in] instance Instance information structure.
 * @param[in] phase selects between nominal/data phase bitrate.
 * @param[in] bitTiming bit timing variables.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_SetBitrate(const can_instance_t * const instance,
                        can_bitrate_phase_t phase,
                        const can_time_segment_t *bitTiming);

/*!
 * @brief Returns the CAN bitrate.
 *
 * This function returns the CAN configured bitrate.
 *
 * @param[in] instance Instance information structure.
 * @param[in] phase selects between nominal/data phase bitrate.
 * @param[out] bitTiming configured bit timing variables.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_GetBitrate(const can_instance_t * const instance,
                        can_bitrate_phase_t phase,
                        can_time_segment_t *bitTiming);

/*!
 * @brief Configures a buffer for transmission.
 *
 * This function configures a buffer for transmission.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should not reconfigure this buffer for transmission.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[in] config buffer configuration.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_ConfigTxBuff(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_buff_config_t *config);

/*!
 * @brief Configures a transmit buffer for remote frame response.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[in] config buffer configuration.
 * @param[in] message frame to be sent as remote response.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_ConfigRemoteResponseBuff(const can_instance_t * const instance,
                                      uint32_t buffIdx,
                                      const can_buff_config_t *config,
                                      const can_message_t *message);

/*!
 * @brief Configures a buffer for reception.
 *
 * This function configures a buffer for reception.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should not reconfigure this buffer for classical
 *       buffer reception.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[in] config buffer configuration.
 * @param[in] acceptedId ID used for accepting frames.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_ConfigRxBuff(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_buff_config_t *config,
                          uint32_t acceptedId);

/*!
 * @brief Sends a CAN frame using the specified buffer.
 *
 * This function sends a CAN frame using a configured buffer. The function
 * returns immediately. If a callback is installed, it will be invoked after
 * the frame was sent.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should not use this buffer for transmission.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[in] message message to be sent.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_BUSY if the current buffer is involved in another transfer;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_Send(const can_instance_t * const instance,
                  uint32_t buffIdx,
                  const can_message_t *message);

/*!
 * @brief Sends a CAN frame using the specified buffer, in a blocking manner.
 *
 * This function sends a CAN frame using a configured buffer. The function
 * blocks until either the frame was sent, or the specified timeoutMs expired.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should not use this buffer for transmission.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[in] message message to be sent.
 * @param[in] timeoutMs A timeout for the transfer in milliseconds.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_BUSY if the current buffer is involved in another transfer;
 *         STATUS_TIMEOUT if the timeout is reached;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_SendBlocking(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_message_t *message,
                          uint32_t timeoutMs);

/*!
 * @brief Receives a CAN frame using the specified message buffer.
 *
 * This function receives a CAN frame using a configured buffer. The function
 * returns immediately. If a callback is installed, it will be invoked after
 * the frame was received and read into the specified buffer.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should use this buffer to receive frames in the FIFO.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[out] message received message.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_BUSY if the current buffer is involved in another transfer;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_Receive(const can_instance_t * const instance,
                     uint32_t buffIdx,
                     can_message_t *message);

/*!
 * @brief Receives a CAN frame using the specified buffer, in a blocking manner.
 *
 * This function receives a CAN frame using a configured buffer. The function
 * blocks until either a frame was received, or the specified timeout expired.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should use this buffer to receive frames in the FIFO.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @param[out] message received message.
 * @param[in] timeoutMs A timeout for the transfer in milliseconds.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_BUSY if the current buffer is involved in another transfer;
 *         STATUS_TIMEOUT if the timeout is reached;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_ReceiveBlocking(const can_instance_t * const instance,
                             uint32_t buffIdx,
                             can_message_t *message,
                             uint32_t timeoutMs);

/*!
 * @brief Ends a non-blocking CAN transfer early.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_CAN_NO_TRANSFER_IN_PROGRESS if no transfer was running
 */
status_t CAN_AbortTransfer(const can_instance_t * const instance,
                           uint32_t buffIdx);

/*!
 * @brief Configures an ID filter for a specific reception buffer.
 *
 * This function configures an ID filter for each reception buffer.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver. The user should not reconfigure the Rx filter for this buffer.
 *
 * @param[in] instance Instance information structure.
 * @param[in] idType selects between standard and extended ID.
 * @param[in] buffIdx buffer index.
 * @param[in] mask mask value for ID filtering.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_CAN_BUFF_OUT_OF_RANGE if the buffer index is out of range;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_SetRxFilter(const can_instance_t * const instance,
                         can_msg_id_type_t idType,
                         uint32_t buffIdx,
                         uint32_t mask);

/*!
 * @brief Returns the state of the previous CAN transfer.
 *
 * When performing an async transfer, call this function to ascertain the state
 * of the current transfer: in progress or complete.
 *
 * @note When the Rx FIFO extension is used, buffer 0 (zero) is used to read the
 *       contents of the FIFO and is configured at the initialization of the
 *       driver.
 *
 * @param[in] instance Instance information structure.
 * @param[in] buffIdx buffer index.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_BUSY if a resource is busy;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_GetTransferStatus(const can_instance_t * const instance,
                               uint32_t buffIdx);

/*!
 * @brief Installs a callback function for the IRQ handler.
 *
 * @param[in] instance Instance information structure.
 * @param[in] callback The callback function.
 * @param[in] callbackParam User parameter passed to the callback function
 *            through the state parameter.
 * @return STATUS_SUCCESS if successful;
 *         STATUS_ERROR if invalid instance number is used;
 */
status_t CAN_InstallEventCallback(const can_instance_t * const instance,
                                  can_callback_t callback,
                                  void *callbackParam);

/**
 *  @brief Returns the Default configuration for CAN_PAL instance 0
 *  over FlexCan with a 500K Baud in normal mode, without flexible datarate
 *  with oscillator clock source for PE and 8 Bytes payload size.
 *  
 *  @param [out] instance Pointer for can_instance structure.
 *  @param [out] config Pointer for can_user_config structure. 
 */
void CAN_GetDefaultConfig(can_instance_t * instance,
        	 	 	 	  can_user_config_t * config);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* CAN_PAL_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
