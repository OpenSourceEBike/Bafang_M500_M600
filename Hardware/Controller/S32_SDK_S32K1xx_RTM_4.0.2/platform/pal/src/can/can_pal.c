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
 * @file can_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type
 * This is needed for the conversion between generic CAN types to FlexCAN types.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast
 * from 'essentially enum<i>' to 'essentially enum<i>'
 * This is needed for the conversion between generic CAN types to FlexCAN types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to
 * object type and a pointer to a different object type. This is needed for the
 * extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to
 * pointer to other type. This is needed for the extension of the user
 * configuration structure, for which the actual type cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.8, attempt to cast away const/volatile
 * from a pointer or reference
 * This is needed for the conversion between generic CAN types to FlexCAN types.
 */

#include "can_pal.h"
#include "device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (defined(CAN_OVER_FLEXCAN))

/* Internal FlexCAN Rx FIFO state information */
typedef struct
{
    bool rxFifoEn;
    flexcan_rx_fifo_id_filter_num_t numIdFilters;
} flexcan_rx_fifo_state_t;

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(CAN_OVER_FLEXCAN))

/*! @brief FlexCAN state structures */
static flexcan_state_t s_flexcanState[NO_OF_FLEXCAN_INSTS_FOR_CAN];
/*! @brief FlexCAN state-instance matching */
static uint32_t s_flexcanStateInstanceMapping[NO_OF_FLEXCAN_INSTS_FOR_CAN];
/*! @brief FlexCAN available resources table */
static bool s_flexcanStateIsAllocated[NO_OF_FLEXCAN_INSTS_FOR_CAN];
/*! @brief FlexCAN buffer configs */
static const can_buff_config_t *s_hwObjConfigs[NO_OF_FLEXCAN_INSTS_FOR_CAN][FEATURE_CAN_MAX_MB_NUM];
/*! @brief FlexCAN Rx FIFO state structures */
static flexcan_rx_fifo_state_t s_flexcanRxFifoState[NO_OF_FLEXCAN_INSTS_FOR_CAN];
/*! @brief Callback function provided by user for each instance*/
static can_callback_t userCallbacks[NO_OF_FLEXCAN_INSTS_FOR_CAN];

#endif

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_AllocateState
 * Description   : Allocates one of the available state structures.
 *
 *END**************************************************************************/
static uint8_t CAN_AllocateState(bool *isAllocated,
                                 uint32_t *instanceMapping,
                                 uint32_t instance)
{
    uint8_t i;
    /* Allocate one of the CAN state structures for this instance */
    for (i = 0; i < NO_OF_FLEXCAN_INSTS_FOR_CAN; i++)
    {
		if (isAllocated[i] == false)
		{
			instanceMapping[i] = instance;
			isAllocated[i] = true;
			break;
		}
    }

    if (i >= NO_OF_FLEXCAN_INSTS_FOR_CAN)
    {
    	/* Should Never Reach Here */
    	DEV_ASSERT(false);
    	i = (NO_OF_FLEXCAN_INSTS_FOR_CAN-1U);
    }

    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_FreeState
 * Description   : Deallocates one of the available state structures.
 *
 *END**************************************************************************/
static void CAN_FreeState(bool *isAllocated,
                          const uint32_t *instanceMapping,
                          uint32_t instance)
{
    uint8_t i;
    /* Free one of the CAN state structures for this instance */
    for (i = 0; i < NO_OF_FLEXCAN_INSTS_FOR_CAN; i++)
    {
		if (instanceMapping[i] == instance)
		{
			isAllocated[i] = false;
			break;
		}
    }
}

#if (defined(CAN_OVER_FLEXCAN))

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_FindFlexCANState
 * Description   : Search the state structure of the FlexCAN instance
 *
 *END**************************************************************************/
static uint8_t CAN_FindFlexCANState(uint32_t instance)
{
    uint8_t i;

    for (i = 0; i < NO_OF_FLEXCAN_INSTS_FOR_CAN; i++)
    {
		if (s_flexcanStateInstanceMapping[i] == instance)
		{
			break;
		}
    }

    if (i >= NO_OF_FLEXCAN_INSTS_FOR_CAN)
    {
    	/* Should Never Reach Here */
    	DEV_ASSERT(false);
    	i = (NO_OF_FLEXCAN_INSTS_FOR_CAN-1U);
    }

    return i;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_GetVirtualBuffIdx
 * Description   : Determines the index of the last buffer occupied by Rx
 *                 FIFO filters
 *
 *END**************************************************************************/
static inline uint32_t CAN_GetVirtualBuffIdx(uint32_t x)
{
    return (5U + ((((x) + 1U) * 8U) / 4U));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_InternalCallback
 * Description   : Internal callback used to translate buffer indices
 *                 and event types
 *
 *END**************************************************************************/
static void CAN_InternalCallback(uint8_t instance,
                                 flexcan_event_type_t eventType,
                                 uint32_t buffIdx,
                                 flexcan_state_t *state)
{
    uint8_t index = CAN_FindFlexCANState(instance);
    can_callback_t callback = userCallbacks[index];

    /* If FlexCAN Rx FIFO is enabled, translate real buffer index to virtual index */
    if ((s_flexcanRxFifoState[index].rxFifoEn == true) && (buffIdx != 0U))
    {
        buffIdx -= CAN_GetVirtualBuffIdx((uint32_t) s_flexcanRxFifoState[index].numIdFilters);
    }

    /* Translate FlexCAN events to CAN PAL events and invoke the callback provided by user */
    switch (eventType)
    {
        case FLEXCAN_EVENT_TX_COMPLETE:
            callback(instance,
                     CAN_EVENT_TX_COMPLETE,
                     (uint8_t) buffIdx,
                     (flexcan_state_t *) state);
            break;
        case FLEXCAN_EVENT_RX_COMPLETE:
            callback(instance,
                     CAN_EVENT_RX_COMPLETE,
                     (uint8_t) buffIdx,
                     (flexcan_state_t *) state);
            break;
        case FLEXCAN_EVENT_RXFIFO_COMPLETE:
            callback(instance,
                     CAN_EVENT_RX_COMPLETE,
                     (uint8_t) buffIdx,
                     (flexcan_state_t *) state);
            break;
        default:
            /* Event types not implemented in PAL */
            break;
    }
}

#endif

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_Init
 * Description   : Configures the CAN module
 *
 * Implements    : CAN_Init_Activity
 *END**************************************************************************/
status_t CAN_Init(const can_instance_t * const instance,
                  const can_user_config_t *config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index = 0U;

    /* Define CAN PAL over FLEXCAN */
    #if (defined (CAN_OVER_FLEXCAN))
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        flexcan_user_config_t flexcanConfig;

        /* Allocate one of the FLEXCAN state structure for this instance */
        index = CAN_AllocateState(s_flexcanStateIsAllocated,
                                  s_flexcanStateInstanceMapping,
                                  instance->instIdx);

        /* Clear Rx FIFO state */
        s_flexcanRxFifoState[index].rxFifoEn = false;

        /* Configure features implemented by PAL */
        flexcanConfig.max_num_mb = config->maxBuffNum;
        flexcanConfig.flexcanMode = (flexcan_operation_modes_t) config->mode;
#if FEATURE_CAN_HAS_FD
        flexcanConfig.fd_enable = config->enableFD;
        flexcanConfig.payload = (flexcan_fd_payload_size_t) config->payloadSize;
#endif

        flexcanConfig.bitrate.phaseSeg1 = config->nominalBitrate.phaseSeg1;
        flexcanConfig.bitrate.phaseSeg2 = config->nominalBitrate.phaseSeg2;
        flexcanConfig.bitrate.preDivider = config->nominalBitrate.preDivider;
        flexcanConfig.bitrate.propSeg = config->nominalBitrate.propSeg;
        flexcanConfig.bitrate.rJumpwidth = config->nominalBitrate.rJumpwidth;

#if FEATURE_CAN_HAS_FD
        flexcanConfig.bitrate_cbt.phaseSeg1 = config->dataBitrate.phaseSeg1;
        flexcanConfig.bitrate_cbt.phaseSeg2 = config->dataBitrate.phaseSeg2;
        flexcanConfig.bitrate_cbt.preDivider = config->dataBitrate.preDivider;
        flexcanConfig.bitrate_cbt.propSeg = config->dataBitrate.propSeg;
        flexcanConfig.bitrate_cbt.rJumpwidth = config->dataBitrate.rJumpwidth;
#endif

#if FEATURE_CAN_HAS_PE_CLKSRC_SELECT
        flexcanConfig.pe_clock = (flexcan_clk_source_t) config->peClkSrc;
#endif

        /* If extension is used, configure Rx FIFO */
        if (config->extension != NULL)
        {
            flexcanConfig.is_rx_fifo_needed = true;
            flexcanConfig.num_id_filters = ((extension_flexcan_rx_fifo_t *)
                                           (config->extension))->numIdFilters;
#if FEATURE_CAN_HAS_DMA_ENABLE
            flexcanConfig.rxFifoDMAChannel = 0U;
#endif
            flexcanConfig.transfer_type = FLEXCAN_RXFIFO_USING_INTERRUPTS;

            /* Compute maximum number of virtual buffers */
            flexcanConfig.max_num_mb += CAN_GetVirtualBuffIdx(flexcanConfig.num_id_filters) + 1U;

            /* Update Rx FIFO state */
            s_flexcanRxFifoState[index].rxFifoEn = true;
            s_flexcanRxFifoState[index].numIdFilters = flexcanConfig.num_id_filters;
        }
        else
        {
            flexcanConfig.is_rx_fifo_needed = false;
            flexcanConfig.num_id_filters = FLEXCAN_RX_FIFO_ID_FILTERS_8;
#if FEATURE_CAN_HAS_DMA_ENABLE
            flexcanConfig.rxFifoDMAChannel = 0U;
#endif
            flexcanConfig.transfer_type = FLEXCAN_RXFIFO_USING_INTERRUPTS;
        }

        /* Initialize FLEXCAN instance */
        status = FLEXCAN_DRV_Init((uint8_t) instance->instIdx,
                                  &s_flexcanState[index],
                                  &flexcanConfig);

        /* Configure Rx FIFO if needed */
        if ((status == STATUS_SUCCESS) &&
            (s_flexcanRxFifoState[index].rxFifoEn == true))
        {
            FLEXCAN_DRV_ConfigRxFifo(
                    (uint8_t) instance->instIdx,
                    ((extension_flexcan_rx_fifo_t *) (config->extension))->idFormat,
                    ((extension_flexcan_rx_fifo_t *) (config->extension))->idFilterTable);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_Deinit
 * Description   : De-initializes the CAN module
 *
 * Implements    : CAN_Deinit_Activity
 *END**************************************************************************/
status_t CAN_Deinit(const can_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        /* De-initialize the FlexCAN module */
        status = FLEXCAN_DRV_Deinit((uint8_t) instance->instIdx);

        if (status == STATUS_SUCCESS)
        {
            /* Clear FlexCAN instance mapping */
            CAN_FreeState(s_flexcanStateIsAllocated,
                          s_flexcanStateInstanceMapping,
                          instance->instIdx);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_SetBitrate
 * Description   : Configures the CAN bit timing variables.
 *
 * Implements    : CAN_SetBitrate_Activity
 *END**************************************************************************/
status_t CAN_SetBitrate(const can_instance_t * const instance,
                        can_bitrate_phase_t phase,
                        const can_time_segment_t *bitTiming)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(bitTiming != NULL);

    status_t status = STATUS_ERROR;
    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)

    flexcan_time_segment_t flexcanBitTime;
    flexcanBitTime.phaseSeg1 = bitTiming->phaseSeg1;
    flexcanBitTime.phaseSeg2 = bitTiming->phaseSeg2;
    flexcanBitTime.preDivider = bitTiming->preDivider;
    flexcanBitTime.propSeg = bitTiming->propSeg;
    flexcanBitTime.rJumpwidth = bitTiming->rJumpwidth;

    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        status = STATUS_SUCCESS;

        if (phase == CAN_NOMINAL_BITRATE)
        {
            FLEXCAN_DRV_SetBitrate((uint8_t) instance->instIdx,
            						&flexcanBitTime);
        }
#if FEATURE_CAN_HAS_FD
        else
        {
            FLEXCAN_DRV_SetBitrateCbt((uint8_t) instance->instIdx,
            						  &flexcanBitTime);
        }
#endif
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_GetBitrate
 * Description   : Returns the CAN configured bit timing variables.
 *
 * Implements    : CAN_GetBitrate_Activity
 *END**************************************************************************/
status_t CAN_GetBitrate(const can_instance_t * const instance,
                        can_bitrate_phase_t phase,
                        can_time_segment_t *bitTiming)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(bitTiming != NULL);

    status_t status = STATUS_ERROR;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        status = STATUS_SUCCESS;

        if (phase == CAN_NOMINAL_BITRATE)
        {
            FLEXCAN_DRV_GetBitrate((uint8_t) instance->instIdx,
                                   (flexcan_time_segment_t *) bitTiming);
        }
#if FEATURE_CAN_HAS_FD
        else
        {
            FLEXCAN_DRV_GetBitrateFD((uint8_t) instance->instIdx,
                                     (flexcan_time_segment_t *) bitTiming);
        }
#endif
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_ConfigTxBuff
 * Description   : Configures a buffer for transmission.
 *
 * Implements    : CAN_ConfigTxBuff_Activity
 *END**************************************************************************/
status_t CAN_ConfigTxBuff(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_buff_config_t *config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) can only be used for reception */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        flexcan_data_info_t dataInfo = {
            .msg_id_type = (flexcan_msgbuff_id_type_t) (config->idType),
			/* Used only to initialize the MB, when send data will be updated */
            .data_length = (uint32_t) 8U,
#if FEATURE_CAN_HAS_FD
            .fd_enable = config->enableFD,
            .fd_padding = config->fdPadding,
            .enable_brs = config->enableBRS,
#endif
            .is_remote = false
        };

        /* Save buffer config  for later use */
        s_hwObjConfigs[index][buffIdx] = config;

        /* Compute virtual buffer index */
        if (s_flexcanRxFifoState[index].rxFifoEn)
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        /* Configure FlexCAN MB for transmission */
        status = FLEXCAN_DRV_ConfigTxMb((uint8_t) instance->instIdx,
                                        (uint8_t) buffIdx,
                                        &dataInfo,
                                        0U);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_ConfigRemoteResponseBuff
 * Description   : Configures a transmit buffer for remote frame response. This
 * function will set up the buffer fields, configure the buffer code for Tx
 * buffer as RX_RANSWER, and enable the buffer interrupt.
 *
 * Implements    : CAN_ConfigRemoteResponseBuff_Activity
 *END**************************************************************************/
status_t CAN_ConfigRemoteResponseBuff(const can_instance_t * const instance,
                                      uint32_t buffIdx,
                                      const can_buff_config_t *config,
                                      const can_message_t *message)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) can only be used for reception */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        flexcan_data_info_t dataInfo = {
            .msg_id_type = (flexcan_msgbuff_id_type_t) (config->idType),
            .data_length = message->length,
#if FEATURE_CAN_HAS_FD
            .fd_enable = config->enableFD,
            .fd_padding = config->fdPadding,
            .enable_brs = config->enableBRS,
#endif
            .is_remote = config->isRemote
        };

        /* Save buffer config  for later use */
        s_hwObjConfigs[index][buffIdx] = config;

        /* Compute virtual buffer index */
        if (s_flexcanRxFifoState[index].rxFifoEn)
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        /* Configure FlexCAN MB for transmission */
        status = FLEXCAN_DRV_ConfigRemoteResponseMb((uint8_t) instance->instIdx,
                                                    (uint8_t) buffIdx,
                                                    &dataInfo,
                                                    message->id,
                                                    message->data);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_ConfigRxBuff
 * Description   : Configures a buffer for reception.
 *
 * Implements    : CAN_ConfigRxBuff_Activity
 *END**************************************************************************/
status_t CAN_ConfigRxBuff(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_buff_config_t *config,
                          uint32_t acceptedId)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) is configured at init time */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        flexcan_data_info_t dataInfo = {
            .msg_id_type = (flexcan_msgbuff_id_type_t) (config->idType),
            .data_length = (config->enableFD ? (uint32_t) 64U :  (uint32_t) 8U),
#if FEATURE_CAN_HAS_FD
            .fd_enable = config->enableFD,
            .fd_padding = config->fdPadding,
            .enable_brs = config->enableBRS,
#endif
            .is_remote = config->isRemote
        };

        /* Save buffer config for later use */
        s_hwObjConfigs[index][buffIdx] = config;

        /* Compute virtual buffer index */
        if (s_flexcanRxFifoState[index].rxFifoEn)
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        /* Configure FlexCAN MB for reception */
        status = FLEXCAN_DRV_ConfigRxMb((uint8_t) instance->instIdx,
                                        (uint8_t) buffIdx,
                                        &dataInfo,
                                        acceptedId);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_Send
 * Description   : Sends a CAN frame using the specified buffer.
 *
 * Implements    : CAN_Send_Activity
 *END**************************************************************************/
status_t CAN_Send(const can_instance_t * const instance,
                  uint32_t buffIdx,
                  const can_message_t *message)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(message != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) can only be used for reception */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        flexcan_data_info_t dataInfo = {
            .msg_id_type = (flexcan_msgbuff_id_type_t) s_hwObjConfigs[index][buffIdx]->idType,
            .data_length = message->length,
#if FEATURE_CAN_HAS_FD
            .fd_enable = s_hwObjConfigs[index][buffIdx]->enableFD,
            .fd_padding = s_hwObjConfigs[index][buffIdx]->fdPadding,
            .enable_brs = s_hwObjConfigs[index][buffIdx]->enableBRS,
#endif
            .is_remote = s_hwObjConfigs[index][buffIdx]->isRemote
        };

        /* Compute virtual buffer index */
        if (s_flexcanRxFifoState[index].rxFifoEn)
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        status = FLEXCAN_DRV_Send((uint8_t) instance->instIdx,
                                  (uint8_t) buffIdx,
                                  &dataInfo,
                                  message->id,
                                  message->data);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_SendBlocking
 * Description   : Sends a CAN frame using the specified buffer,
 *                 in a blocking manner.
 *
 * Implements    : CAN_SendBlocking_Activity
 *END**************************************************************************/
status_t CAN_SendBlocking(const can_instance_t * const instance,
                          uint32_t buffIdx,
                          const can_message_t *message,
                          uint32_t timeoutMs)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(message != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) can only be used for reception */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        flexcan_data_info_t dataInfo = {
            .msg_id_type = (flexcan_msgbuff_id_type_t) s_hwObjConfigs[index][buffIdx]->idType,
            .data_length = message->length,
#if FEATURE_CAN_HAS_FD
            .fd_enable = s_hwObjConfigs[index][buffIdx]->enableFD,
            .fd_padding = s_hwObjConfigs[index][buffIdx]->fdPadding,
            .enable_brs = s_hwObjConfigs[index][buffIdx]->enableBRS,
#endif
            .is_remote = s_hwObjConfigs[index][buffIdx]->isRemote
        };

        /* Compute virtual buffer index */
        if (s_flexcanRxFifoState[index].rxFifoEn)
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        status = FLEXCAN_DRV_SendBlocking((uint8_t) instance->instIdx,
                                          (uint8_t) buffIdx,
                                          &dataInfo,
                                          message->id,
                                          message->data,
                                          timeoutMs);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_Receive
 * Description   : Receives a CAN frame using the specified message buffer.
 *
 * Implements    : CAN_Receive_Activity
 *END**************************************************************************/
status_t CAN_Receive(const can_instance_t * const instance,
                     uint32_t buffIdx,
                     can_message_t *message)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(message != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) is used to read frames received in FIFO */
        if ((s_flexcanRxFifoState[index].rxFifoEn == true) && (buffIdx == 0U))
        {
            status = FLEXCAN_DRV_RxFifo((uint8_t) instance->instIdx,
                                        (flexcan_msgbuff_t *) message);
        }
        else
        {
            /* Compute virtual buffer index */
            if (s_flexcanRxFifoState[index].rxFifoEn)
            {
                buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
            }

            status = FLEXCAN_DRV_Receive((uint8_t) instance->instIdx,
                                         (uint8_t) buffIdx,
                                         (flexcan_msgbuff_t *) message);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_ReceiveBlocking
 * Description   : Receives a CAN frame using the specified buffer,
 *                 in a blocking manner.
 *
 * Implements    : CAN_ReceiveBlocking_Activity
 *END**************************************************************************/
status_t CAN_ReceiveBlocking(const can_instance_t * const instance,
                             uint32_t buffIdx,
                             can_message_t *message,
                             uint32_t timeoutMs)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(message != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) is used to read frames received in FIFO */
        if ((s_flexcanRxFifoState[index].rxFifoEn == true) && (buffIdx == 0U))
        {
            status = FLEXCAN_DRV_RxFifoBlocking((uint8_t) instance->instIdx,
                                                (flexcan_msgbuff_t *) message,
                                                timeoutMs);
        }
        else
        {
            /* Compute virtual buffer index */
            if (s_flexcanRxFifoState[index].rxFifoEn)
            {
                buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
            }

            status = FLEXCAN_DRV_ReceiveBlocking((uint8_t) instance->instIdx,
                                                 (uint8_t) buffIdx,
                                                 (flexcan_msgbuff_t *) message,
                                                 timeoutMs);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_AbortTransfer
 * Description   : This function ends a non-blocking CAN transfer early.
 *
 * Implements    : CAN_AbortTransfer_Activity
 *END**************************************************************************/
status_t CAN_AbortTransfer(const can_instance_t * const instance,
                           uint32_t buffIdx)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

       index = CAN_FindFlexCANState(instance->instIdx);

       /* Compute virtual buffer index */
       if (s_flexcanRxFifoState[index].rxFifoEn)
       {
           buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
       }

        status = FLEXCAN_DRV_AbortTransfer((uint8_t) instance->instIdx,
                                            (uint8_t)buffIdx);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_SetRxFilter
 * Description   : Configures an ID filter for a specific reception buffer.
 *
 * Implements    : CAN_SetRxFilter_Activity
 *END**************************************************************************/
status_t CAN_SetRxFilter(const can_instance_t * const instance,
                         can_msg_id_type_t idType,
                         uint32_t buffIdx,
                         uint32_t mask)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* If Rx FIFO is enabled, buffer 0 (zero) filters are configured at init time */
        DEV_ASSERT((s_flexcanRxFifoState[index].rxFifoEn == false) || (buffIdx != 0U));
        (void) index;
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);

        FLEXCAN_DRV_SetRxMaskType((uint8_t) instance->instIdx,
                                  FLEXCAN_RX_MASK_INDIVIDUAL);
        status = FLEXCAN_DRV_SetRxIndividualMask((uint8_t) instance->instIdx,
                                                 (flexcan_msgbuff_id_type_t) idType,
                                                 (uint8_t) buffIdx,
                                                 mask);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_GetTransferStatus
 * Description   : Returns the state of the previous CAN transfer.
 *
 * Implements    : CAN_GetTransferStatus_Activity
 *END**************************************************************************/
status_t CAN_GetTransferStatus(const can_instance_t * const instance,
                               uint32_t buffIdx)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        /* Check buffer index to avoid overflow */
        DEV_ASSERT(buffIdx < FEATURE_CAN_MAX_MB_NUM);
        DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

        index = CAN_FindFlexCANState(instance->instIdx);

        /* Compute virtual buffer index */
        if ((s_flexcanRxFifoState[index].rxFifoEn) && (buffIdx != 0U))
        {
            buffIdx += CAN_GetVirtualBuffIdx(s_flexcanRxFifoState[index].numIdFilters);
        }

        status = FLEXCAN_DRV_GetTransferStatus((uint8_t) instance->instIdx,
                                               (uint8_t) buffIdx);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_InstallEventCallback
 * Description   : Installs a callback function for the IRQ handler.
 *
 * Implements    : CAN_InstallEventCallback_Activity
 *END**************************************************************************/
status_t CAN_InstallEventCallback(const can_instance_t * const instance,
                                  can_callback_t callback,
                                  void *callbackParam)
{
    DEV_ASSERT(instance != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index;

    /* Define CAN PAL over FLEXCAN */
    #if defined(CAN_OVER_FLEXCAN)
    if (instance->instType == CAN_INST_TYPE_FLEXCAN)
    {
        if (callback != NULL)
        {
            DEV_ASSERT(instance->instIdx < CAN_INSTANCE_COUNT);

            index = CAN_FindFlexCANState(instance->instIdx);

            /* Save user callback */
            userCallbacks[index] = callback;
            /* Install internal FlexCAN callback */
            FLEXCAN_DRV_InstallEventCallback((uint8_t) instance->instIdx,
                                             CAN_InternalCallback,
                                             callbackParam);
        }
        else
        {
            /* Uninstall internal FlexCAN callback */
            FLEXCAN_DRV_InstallEventCallback((uint8_t) instance->instIdx,
                                             NULL,
                                             callbackParam);
        }

        status = STATUS_SUCCESS;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_GetDefaultConfig
 * Description   : Return Default configuration for CAN Pal component
 *
 * Implements    : CAN_GetDefaultConfig_Activity
 *END**************************************************************************/
 void CAN_GetDefaultConfig(can_instance_t * instance,
         	 	 	 	   can_user_config_t * config)
 {
	 flexcan_user_config_t flexcanConfig;
	 /* Get Default configuration and calculate cbt segments based on clock value */
     (void) FLEXCAN_DRV_GetDefaultConfig(&flexcanConfig);

	 instance->instType = CAN_INST_TYPE_FLEXCAN;
	 instance->instIdx = 0UL;

	 config->maxBuffNum = flexcanConfig.max_num_mb;
	 config->mode = CAN_NORMAL_MODE;
	 config->peClkSrc = CAN_CLK_SOURCE_OSC;
	 config->enableFD = false;
	 config->payloadSize = CAN_PAYLOAD_SIZE_8;

	 config->nominalBitrate.phaseSeg1   = flexcanConfig.bitrate.phaseSeg1;
	 config->nominalBitrate.phaseSeg2   = flexcanConfig.bitrate.phaseSeg2;
	 config->nominalBitrate.preDivider  = flexcanConfig.bitrate.preDivider;
	 config->nominalBitrate.propSeg     = flexcanConfig.bitrate.propSeg;
	 config->nominalBitrate.rJumpwidth  = flexcanConfig.bitrate.rJumpwidth;

     config->dataBitrate.phaseSeg1  = flexcanConfig.bitrate.phaseSeg1;
	 config->dataBitrate.phaseSeg2  = flexcanConfig.bitrate.phaseSeg2;
	 config->dataBitrate.preDivider = flexcanConfig.bitrate.preDivider;
	 config->dataBitrate.propSeg    = flexcanConfig.bitrate.propSeg;
	 config->dataBitrate.rJumpwidth = flexcanConfig.bitrate.rJumpwidth;


	 config->extension = NULL;
 }

/*******************************************************************************
 * EOF
 ******************************************************************************/
