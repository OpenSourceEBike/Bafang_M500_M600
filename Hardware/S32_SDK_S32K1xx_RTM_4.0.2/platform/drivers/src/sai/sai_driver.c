/*
 * Copyright 2017, 2021 NXP
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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, cast performed between a pointer to object
 * type and a pointer to a different object type.
 * Driver support many data element sizes (1, 2, 4 byte), the cast is required to be able to
 * work on correct data size.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 18.4, pointer arithmetic other than array indexing used
 * Driver support many data element sizes (1, 2, 4 byte), pointer arithmetic is required to
 * be able to work on correct data size.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.8, attempt to cast away const/volatile
 * from a pointer or reference
 * To reduce complexity of driver, transmit and receive use the same state structure, which hold
 * user data pointer that might be read from or write to.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * Local variable is required to get neccesary value from function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'data' could be declared as pointing to const
 * Memory pointed by 'data' is used for receive.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 * Required to cast to void* to used with edma function
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced
 * These macro are used by user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External function could be made static
 * These functions are used by user.
 */

#include "sai_driver.h"
#include "sai_hw_access.h"

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/* default values set by driver */
#define TX_WATERMARK 5U
#define RX_WATERMARK 5U
#define FIFO_SIZE 8U
#define BIT_CLK_AS_EXTERNAL false
#define WORD_FLAG_INDEX 0
#define CONT_ON_ERROR true
#define PACK_MODE 0
#define SYNC_ON_DEMAND true
#define TX_FIFO_SIZE (FIFO_SIZE-TX_WATERMARK)
#define RX_FIFO_SIZE (RX_WATERMARK+1U)

#ifdef FEATURE_SAI_BIG_ENDIAN
#define DATA_REG_SIZE 4U
#endif

/*******************************************************************************
 * Private Functions
 ******************************************************************************/

static void SAI_DRV_TxResetVar(uint32_t instNum);
static void SAI_DRV_RxResetVar(uint32_t instNum);
static void SAI_DRV_CompleteSendDataUsingDma(void * parameter,
                                             edma_chn_status_t status);
static void SAI_DRV_CompleteReceiveDataUsingDma(void * parameter,
                                             edma_chn_status_t status);
static void SAI_DRV_TxIRQ(uint32_t instNum);
static void SAI_DRV_RxIRQ(uint32_t instNum);
static void SAI_DRV_SendInt(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count);
static void SAI_DRV_SendDma(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count);
static void SAI_DRV_ReceiveDma(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count);
static void SAI_DRV_ReceiveInt(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count);
static bool SAI_DRV_TxFillFifoInterrupt(uint32_t instNum);
static bool SAI_DRV_TxFillFifoDma(uint32_t instNum);
static void SAI_DRV_TxMuxLineInterrupt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish);
static void SAI_DRV_TxMuxMemInterrupt(uint32_t instNum,
                                      SAI_Type* inst,
                                      sai_state_t* pState,
                                      bool* finish);
static void SAI_DRV_TxMuxDisableInterrupt(uint32_t instNum,
                                          SAI_Type* inst,
                                          sai_state_t* pState,
                                          bool* finish);
static void SAI_DRV_RxMuxLineInterrupt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish);
static void SAI_DRV_RxMuxMemInterrupt(uint32_t instNum,
                                      SAI_Type* inst,
                                      sai_state_t* pState,
                                      bool* finish);
static void SAI_DRV_RxMuxDisableInterrupt(uint32_t instNum,
                                          SAI_Type* inst,
                                          sai_state_t* pState,
                                          bool* finish);
static void SAI_DRV_TxDmaInit(uint32_t instNum,
                              SAI_Type* inst,
                              sai_state_t* pState);
static void SAI_DRV_RxDmaInit(uint32_t instNum,
                              SAI_Type* inst,
                              sai_state_t* pState);
static void SAI_DRV_InternalClock(uint32_t instNum,
                                  const sai_user_config_t* saiUserConfig,
                                  bool *clkInternal,
                                  uint32_t *divisor,
                                  uint32_t *SourceClock);
static void SAI_DRV_TxFillMuxLineInt(uint32_t instNum,
                                     SAI_Type* inst,
                                     sai_state_t* pState,
                                     bool* finish);
static void SAI_DRV_TxFillMuxMemInt(uint32_t instNum,
                                    SAI_Type* inst,
                                    sai_state_t* pState,
                                    bool* finish);
static void SAI_DRV_TxFillMuxDisableInt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish);
static void SAI_DRV_TxFillMuxLineDma(uint32_t instNum,
                                     SAI_Type* inst,
                                     sai_state_t* pState,
                                     bool* finish);
static void SAI_DRV_TxFillMuxMemDma(uint32_t instNum,
                                    SAI_Type* inst,
                                    sai_state_t* pState,
                                    bool* finish);
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void SAI_DRV_RxInitCheck(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig,
                                sai_state_t* StateAlloc);
static void SAI_DRV_RxInitCheckSync(uint32_t instNum,
                                    const sai_user_config_t* saiUserConfig);
static void SAI_DRV_TxInitCheck(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig,
                                sai_state_t* StateAlloc);
static void SAI_DRV_TxInitCheckSync(uint32_t instNum,
                                    const sai_user_config_t* saiUserConfig);
#endif /* DEV_ERROR_DETECT */
static void SAI_DRV_GetRxBusyCount(uint32_t instNum,
                                 uint32_t *countRemain);
static void SAI_DRV_AbortSendingDma(uint32_t instNum);
static void SAI_DRV_AbortReceivingDma(uint32_t instNum);
static void SAI_DRV_TxIRQBusy(uint32_t instNum);
static void SAI_DRV_RxIRQBusy(uint32_t instNum);
static void SAI_DRV_FlushRxFifo(uint32_t instNum);
static uint8_t SAI_DRV_GetFirstEnabledRxChannel(uint32_t instNum);

#if (SAI_INSTANCE_COUNT > 0U)
void SAI0_Tx_IRQHandler(void);
void SAI0_Rx_IRQHandler(void);
#endif
#if (SAI_INSTANCE_COUNT > 1U)
void SAI1_Tx_IRQHandler(void);
void SAI1_Rx_IRQHandler(void);
#endif
#if (SAI_INSTANCE_COUNT > 2U)
void SAI2_Tx_IRQHandler(void);
void SAI2_Rx_IRQHandler(void);
#endif

#if (SAI_INSTANCE_COUNT > 0U)
/* Implementation of SAI0 handler named in startup code. */
void SAI0_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(0U);
}
void SAI0_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(0U);
}
#endif
#if (SAI_INSTANCE_COUNT > 1U)
/* Implementation of SAI1 handler named in startup code. */
void SAI1_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(1U);
}
void SAI1_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(1U);
}
#endif
#if (SAI_INSTANCE_COUNT > 2U)
/* Implementation of SAI1 handler named in startup code. */
void SAI2_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(2U);
}
void SAI2_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(2U);
}
#endif

/* Channel count for each instance */
#if (SAI_INSTANCE_COUNT == 2U)
    static const uint8_t sai_channel_count[SAI_INSTANCE_COUNT] = {SAI0_CHANNEL_COUNT, SAI1_CHANNEL_COUNT};
#elif (SAI_INSTANCE_COUNT == 3U)
    static const uint8_t sai_channel_count[SAI_INSTANCE_COUNT] = {SAI0_CHANNEL_COUNT, SAI1_CHANNEL_COUNT, SAI2_CHANNEL_COUNT};
#endif
#if (defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT) || (defined(FEATURE_SAI_MSEL_BUS_CLK) && defined(MPC574x_SERIES)))
#if (SAI_INSTANCE_COUNT == 2U)
    static const clock_names_t SAIClkNames[SAI_INSTANCE_COUNT] = {SAI0_CLK, SAI1_CLK};
#elif (SAI_INSTANCE_COUNT == 3U)
    static const clock_names_t SAIClkNames[SAI_INSTANCE_COUNT] = {SAI0_CLK, SAI1_CLK, SAI2_CLK};
#endif
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
static SAI_Type * const SAIBase[SAI_INSTANCE_COUNT] = SAI_BASE_PTRS;
static const IRQn_Type SAITxIrqId[SAI_INSTANCE_COUNT] = SAI_TX_IRQS;
static const IRQn_Type SAIRxIrqId[SAI_INSTANCE_COUNT] = SAI_RX_IRQS;

/* state variables */
static sai_state_t* TxState[SAI_INSTANCE_COUNT];
static sai_state_t* RxState[SAI_INSTANCE_COUNT];

/*FUNCTION**********************************************************************
 *
 * Function Name : TO_BIT
 * Description   : Convert bool to interger
 *
 *END**************************************************************************/
static inline uint32_t TO_BIT(bool x)
{
    return x ? 1UL : 0UL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxResetVar
 * Description   : Reset driver variable
 *
 *END**************************************************************************/
static void SAI_DRV_TxResetVar(uint32_t instNum)
{
    sai_state_t* pState = TxState[instNum];
    uint8_t j;

    for (j = 0; j < sai_channel_count[instNum]; j++)
    {
        pState->ChnState[j].data = NULL;
        pState->ChnState[j].count = 0U;
    }
    /* unsupported means uninitialized */
    pState->status = STATUS_UNSUPPORTED;
    pState->Blocking = false;
    pState->ChannelCount = 0U;
    pState->Callback = NULL;
    pState->NextChn = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxResetVar
 * Description   : Reset driver variable
 *
 *END**************************************************************************/
static void SAI_DRV_RxResetVar(uint32_t instNum)
{
    sai_state_t* pState = RxState[instNum];
    uint8_t j;

    for (j = 0; j < sai_channel_count[instNum]; j++)
    {
        pState->ChnState[j].data = NULL;
        pState->ChnState[j].count = 0U;
    }
    /* unsupported means uninitialized */
    pState->status = STATUS_UNSUPPORTED;
    pState->ChannelCount = 0U;
    pState->Blocking = false;
    pState->Callback = NULL;
    pState->NextChn = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_CompleteSendDataUsingDma
 * Description   : Finish sending data using dma
 *
 *END**************************************************************************/
static void SAI_DRV_CompleteSendDataUsingDma(void * parameter,
                                             edma_chn_status_t status)
{
    uint32_t instNum = (uint32_t)parameter;
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    sai_report_type_t event;

    /* Disable tx DMA requests for the current instance */
    SAI_DRV_TxDisableFifoReqDma(inst);
    /* Release the DMA channel */
    if (status != EDMA_CHN_NORMAL)
    {
        pState->status = STATUS_ERROR;
        event = SAI_ERROR;
    }
    else
    {
        pState->status = STATUS_SUCCESS;
        event = SAI_TX_COMPLETE;
    }
    if (pState->Callback != NULL)
    {
        pState->Callback(event, pState->callbackParam);
        /* no continue in callback */
        if (pState->Blocking)
        {
            if (pState->status != STATUS_BUSY)
            {
                (void)OSIF_SemaPost(&pState->Sema);
            }
        }
    }
    else
    {
        if (pState->Blocking)
        {
            (void)OSIF_SemaPost(&pState->Sema);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_CompleteReceiveDataUsingDma
 * Description   : Finish receiving data using dma
 *
 *END**************************************************************************/
static void SAI_DRV_CompleteReceiveDataUsingDma(void * parameter,
                                             edma_chn_status_t status)
{
    uint32_t instNum = (uint32_t)parameter;
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    sai_report_type_t event;

    /* Disable Rx DMA requests for the current instance */
    SAI_DRV_RxDisableFifoReqDma(inst);
    /* Release the DMA channel */
    if (status != EDMA_CHN_NORMAL)
    {
        pState->status = STATUS_ERROR;
        event = SAI_ERROR;
    }
    else
    {
        pState->status = STATUS_SUCCESS;
        event = SAI_RX_COMPLETE;
    }
    if (pState->Callback != NULL)
    {
        pState->Callback(event, pState->callbackParam);
        /* no continue in callback */
        if (pState->Blocking)
        {
            if (pState->status != STATUS_BUSY)
            {
                (void)OSIF_SemaPost(&pState->Sema);
            }
        }
    }
    else
    {
        if (pState->Blocking)
        {
            (void)OSIF_SemaPost(&pState->Sema);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AlmostCompleteReceiveDma
 * Description   : Completed count-1 data, set watermark to 0 to receive last data
 *
 *END**************************************************************************/
static void SAI_DRV_AlmostCompleteReceiveDma(void * parameter,
                                             edma_chn_status_t status)
{
    uint32_t instNum = (uint32_t)parameter;
    SAI_Type* inst = SAIBase[instNum];
    uint8_t* data[SAI_MAX_CHANNEL_COUNT];
    sai_state_t* pState = RxState[instNum];
    uint8_t c;
    uint8_t i;

    SAI_DRV_RxDisableFifoReqDma(inst);
    /* check if aborted */
    if (pState->status == STATUS_BUSY)
    {
        if (status != EDMA_CHN_NORMAL)
        {
            SAI_DRV_CompleteReceiveDataUsingDma(parameter, status);
        }
        else
        {
            c = 0;
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    data[c] = pState->ChnState[i].data + ((pState->ChnState[i].count-1UL) * pState->ElementSize);
                    c++;
                }
            }
            SAI_DRV_ReceiveDma(instNum, data, 1UL);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxMuxLineInterrupt
 * Description   : Manage interrupt transfer, separated from original txIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxMuxLineInterrupt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsTxChannelEnabled(inst, i))
        {
            if (pState->ChnState[0].count < TX_FIFO_SIZE)
            {
                temp = pState->ChnState[0].count;
            }
            else
            {
                temp = TX_FIFO_SIZE;
            }
            for (j = 0U; j < temp; j++)
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[pState->NextChn].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[pState->NextChn].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[pState->NextChn].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[pState->NextChn].data += pState->ElementSize;
                pState->NextChn++; /* alternate between data block */
                if (pState->NextChn == pState->ChannelCount)
                {
                    pState->NextChn = 0U;
                }
            }
            if (pState->ChnState[0].count <= TX_FIFO_SIZE)
            {
                *finish = true;
                pState->ChnState[0].count = 0;
            }
            else
            {
                pState->ChnState[0].count -= TX_FIFO_SIZE;
            }
            break; /* only one channle enabled in this mode */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxMuxMemInterrupt
 * Description   : Manage interrupt transfer, separated from original txIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxMuxMemInterrupt(uint32_t instNum,
                                      SAI_Type* inst,
                                      sai_state_t* pState,
                                      bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    if (pState->ChnState[0].count < TX_FIFO_SIZE)
    {
        temp = pState->ChnState[0].count;
    }
    else
    {
        temp = TX_FIFO_SIZE;
    }
    for (j = 0U; j < temp; j++)
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[0].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[0U].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[0U].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[0U].data += pState->ElementSize;
            }
        }
    }
    if (pState->ChnState[0].count <= TX_FIFO_SIZE)
    {
        *finish = true;
        pState->ChnState[0].count = 0;
    }
    else
    {
        pState->ChnState[0].count -= TX_FIFO_SIZE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxMuxDisableInterrupt
 * Description   : Manage interrupt transfer, separated from original txIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxMuxDisableInterrupt(uint32_t instNum,
                                          SAI_Type* inst,
                                          sai_state_t* pState,
                                          bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    if (pState->ChnState[0].count < TX_FIFO_SIZE)
    {
        temp = pState->ChnState[0].count;
    }
    else
    {
        temp = TX_FIFO_SIZE;
    }
    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsTxChannelEnabled(inst, i))
        {
            for (j = 0U; j < temp; j++)
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[i].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[i].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[i].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[i].data += pState->ElementSize;
            }
        }
    }
    if (pState->ChnState[0].count <= TX_FIFO_SIZE)
    {
        *finish = true;
        pState->ChnState[0].count = 0;
    }
    else
    {
        pState->ChnState[0].count -= TX_FIFO_SIZE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxIRQBusy
 * Description   : Manage interrupt transfer when in busy state
 *
 *END**************************************************************************/
static void SAI_DRV_TxIRQBusy(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    bool finish;

    finish = false;
    if (pState->mux == SAI_MUX_LINE)
    {
        SAI_DRV_TxMuxLineInterrupt(instNum, inst, pState, &finish);
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        SAI_DRV_TxMuxMemInterrupt(instNum, inst, pState, &finish);
    }
    else /* mux disabled */
    {
        SAI_DRV_TxMuxDisableInterrupt(instNum, inst, pState, &finish);
    }
    if (finish)
    {
        pState->status = STATUS_SUCCESS;
        SAI_DRV_TxDisableFifoReqInt(inst);
        if (pState->Callback != NULL)
        {
            pState->Callback(SAI_TX_COMPLETE, pState->callbackParam);
            /* No continue in callback */
            if (pState->status == STATUS_SUCCESS)
            {
                if (pState->Blocking)
                {
                    (void)OSIF_SemaPost(&pState->Sema);
                }
            }
        }
        else
        {
            if (pState->Blocking)
            {
                (void)OSIF_SemaPost(&pState->Sema);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxIRQ
 * Description   : Manage interrupt transfer using fifo request, execute
 *                 other user interrupt func (fifo error, word start,
 *                 sync error)
 *
 *END**************************************************************************/
static void SAI_DRV_TxIRQ(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    status_t CurStatus = pState->status;

    if (SAI_DRV_TxGetFifoReqFlag(inst))
    {
        if ((pState->XferType == SAI_INTERRUPT) && (CurStatus == STATUS_BUSY))
        {
            SAI_DRV_TxIRQBusy(instNum);
        }
    }
    if (pState->Callback != NULL)
    {
        if (SAI_DRV_TxGetFifoErrorFlag(inst))
        {
            pState->Callback(SAI_RUN_ERROR, pState->callbackParam);
            SAI_DRV_TxClearFlag (inst, SAI_TCSR_FEF_SHIFT);
        }
        if (SAI_DRV_TxGetWordStartFlag(inst))
        {
            pState->Callback(SAI_FRAME_START, pState->callbackParam);
            SAI_DRV_TxClearFlag (inst, SAI_TCSR_WSF_SHIFT);
        }
        if (SAI_DRV_TxGetSyncErrorFlag(inst))
        {
            pState->Callback(SAI_SYNC_ERROR, pState->callbackParam);
            SAI_DRV_TxClearFlag (inst, SAI_TCSR_SEF_SHIFT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxMuxLineInterrupt
 * Description   : Manage interrupt transfer, separated from original rxIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_RxMuxLineInterrupt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsRxChannelEnabled(inst, i))
        {
            if (pState->ChnState[0].count < RX_FIFO_SIZE)
            {
                temp = pState->ChnState[0].count;
            }
            else
            {
                temp = RX_FIFO_SIZE;
            }
            for (j = 0U; j < temp; j++)
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        *pState->ChnState[pState->NextChn].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 2U:
                        *(uint16_t*)pState->ChnState[pState->NextChn].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 4U:
                        *(uint32_t*)pState->ChnState[pState->NextChn].data = SAI_DRV_RxRead(inst, i);
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[pState->NextChn].data += pState->ElementSize;
                pState->NextChn++; /* alternate between data block */
                if (pState->NextChn == pState->ChannelCount)
                {
                    pState->NextChn = 0U;
                }
            }
            if (pState->ChnState[0].count <= RX_FIFO_SIZE)
            {
                *finish = true;
                pState->ChnState[0].count = 0;
            }
            else
            {
                pState->ChnState[0].count -= RX_FIFO_SIZE;
                if (pState->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
                {
                    SAI_DRV_RxSetWatermark(inst, (uint8_t)(pState->ChnState[0].count - 1UL));
                }
            }
            break; /* only one channel is enabled in this mode */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxMuxMemInterrupt
 * Description   : Manage interrupt transfer, separated from original rxIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_RxMuxMemInterrupt(uint32_t instNum,
                                      SAI_Type* inst,
                                      sai_state_t* pState,
                                      bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    if (pState->ChnState[0].count < RX_FIFO_SIZE)
    {
        temp = pState->ChnState[0].count;
    }
    else
    {
        temp = RX_FIFO_SIZE;
    }
    for (j = 0U; j < temp; j++)
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                switch(pState->ElementSize)
                {
                    case 1U:
                        *pState->ChnState[0U].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 2U:
                        *(uint16_t*)pState->ChnState[0U].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 4U:
                        *(uint32_t*)pState->ChnState[0U].data = SAI_DRV_RxRead(inst, i);
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[0U].data += pState->ElementSize;
            }
        }
    }
    if (pState->ChnState[0].count <= RX_FIFO_SIZE)
    {
        *finish = true;
        pState->ChnState[0].count = 0;
    }
    else
    {
        pState->ChnState[0].count -= RX_FIFO_SIZE;
        if (pState->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(pState->ChnState[0].count - 1UL));
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxMuxDisableInterrupt
 * Description   : Manage interrupt transfer, separated from original rxIrq
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_RxMuxDisableInterrupt(uint32_t instNum,
                                          SAI_Type* inst,
                                          sai_state_t* pState,
                                          bool* finish)
{
    uint8_t i,j;
    uint32_t temp;

    if (pState->ChnState[0].count < RX_FIFO_SIZE)
    {
        temp = pState->ChnState[0].count;
    }
    else
    {
        temp = RX_FIFO_SIZE;
    }
    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsRxChannelEnabled(inst, i))
        {
            for (j = 0U; j < temp; j++)
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        *pState->ChnState[i].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 2U:
                        *(uint16_t*)pState->ChnState[i].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                        break;
                    case 4U:
                        *(uint32_t*)pState->ChnState[i].data = SAI_DRV_RxRead(inst, i);
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[i].data += pState->ElementSize;
            }
        }
    }
    if (pState->ChnState[0].count <= RX_FIFO_SIZE)
    {
        *finish = true;
        pState->ChnState[0].count = 0;
    }
    else
    {
        pState->ChnState[0].count -= RX_FIFO_SIZE;
        if (pState->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(pState->ChnState[0].count - 1UL));
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxIRQBusy
 * Description   : Manage interrupt transfer when in busy state
 *
 *END**************************************************************************/
static void SAI_DRV_RxIRQBusy(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    bool finish;

    finish = false;
    if (pState->mux == SAI_MUX_LINE)
    {
        SAI_DRV_RxMuxLineInterrupt(instNum, inst, pState, &finish);
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        SAI_DRV_RxMuxMemInterrupt(instNum, inst, pState, &finish);
    }
    else /* mux disabled */
    {
        SAI_DRV_RxMuxDisableInterrupt(instNum, inst, pState, &finish);
    }
    if (finish)
    {
        pState->status = STATUS_SUCCESS;
        SAI_DRV_RxDisableFifoReqInt(inst);
        if (pState->Callback != NULL)
        {
            pState->Callback(SAI_RX_COMPLETE, pState->callbackParam);
            /* No continue in callback */
            if (pState->status == STATUS_SUCCESS)
            {
                if (pState->Blocking)
                {
                    (void)OSIF_SemaPost(&pState->Sema);
                }
            }
        }
        else
        {
            if (pState->Blocking)
            {
                (void)OSIF_SemaPost(&pState->Sema);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxIRQ
 * Description   : Manage interrupt transfer using fifo request, execute
 *                 other user interrupt func (fifo error, word start,
 *                 sync error)
 *
 *END**************************************************************************/
static void SAI_DRV_RxIRQ(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    status_t CurStatus = pState->status;

    if (SAI_DRV_RxGetFifoReqFlag(inst))
    {
        if ((pState->XferType == SAI_INTERRUPT) && (CurStatus == STATUS_BUSY))
        {
            SAI_DRV_RxIRQBusy(instNum);
        }
    }
    if (pState->Callback != NULL)
    {
        if (SAI_DRV_RxGetFifoErrorFlag(inst))
        {
            pState->Callback(SAI_RUN_ERROR, pState->callbackParam);
            SAI_DRV_RxClearFlag (inst, SAI_RCSR_FEF_SHIFT);
        }
        if (SAI_DRV_RxGetWordStartFlag(inst))
        {
            pState->Callback(SAI_FRAME_START, pState->callbackParam);
            SAI_DRV_RxClearFlag (inst, SAI_RCSR_WSF_SHIFT);
        }
        if (SAI_DRV_RxGetSyncErrorFlag(inst))
        {
            pState->Callback(SAI_SYNC_ERROR, pState->callbackParam);
            SAI_DRV_RxClearFlag (inst, SAI_RCSR_SEF_SHIFT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendInt
 * Description   : Start sending data using interrupt, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_SendInt(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    uint8_t c = 0;
    bool ret = false;
    uint8_t i;
    uint32_t tcsr;

    pState->status = STATUS_BUSY;
    if (pState->mux == SAI_MUX_LINE)
    {
        for (i = 0; i < pState->ChannelCount; i++)
        {
            DEV_ASSERT(data[i] != NULL);
            /* cast to non-const type to avoid compile error/warning */
            pState->ChnState[i].data = (uint8_t*) data[i];
        }
        pState->ChnState[0].count = count * pState->ChannelCount;
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        /* cast to non-const type to avoid compile error/warning */
        pState->ChnState[0].data = (uint8_t*) data[0];
        pState->ChnState[0].count = count;
    }
    else
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                /* cast to non-const type to avoid compile error/warning */
                pState->ChnState[i].data = (uint8_t*) data[c];
                c++;
            }
        }
        pState->ChnState[0].count = count;
    }
    /* Fill fifo before enable tx */
    tcsr = inst->TCSR;
    if ((tcsr & SAI_TCSR_TE_MASK) == 0UL)
    {
        ret = SAI_DRV_TxFillFifoInterrupt(instNum);
        /* if not finished all data after filling fifo operation */
        if (!ret)
        {
            SAI_DRV_TxEnableFifoReqInt(inst);
        }
    }
    else
    {
        SAI_DRV_TxEnableFifoReqInt(inst);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendDMA
 * Description   : Start sending data using dma, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_SendDma(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    uint8_t numChan = pState->ChannelCount;
    bool ret = false;
    uint8_t i;
    uint8_t c;
    uint32_t temp;
    uint32_t tcsr;

    pState->status = STATUS_BUSY;
    /* Setup state variable */
    if ((pState->mux == SAI_MUX_LINE) || (pState->mux == SAI_MUX_DISABLED))
    {
        c = 0;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                pState->ChnState[i].count = count;
                /* cast to non-const type to avoid compile error/warning */
                pState->ChnState[i].data = (uint8_t*) data[c];
                c++;
            }
        }
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        pState->ChnState[0].count = count;
        /* cast to non-const type to avoid compile error/warning */
        pState->ChnState[0].data = (uint8_t*) data[0];
    }
    else
    {
        /* do nothing */
    }
    /* Fill fifo before enable tx */
    tcsr = inst->TCSR;
    if ((tcsr & SAI_TCSR_TE_MASK) == 0UL)
    {
        ret = SAI_DRV_TxFillFifoDma(instNum);
    }
    if (((tcsr & SAI_TCSR_TE_MASK) != 0UL) || (!ret))
    {
        if ((pState->mux == SAI_MUX_LINE) || (pState->mux == SAI_MUX_DISABLED))
        {
            c = 0;
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    EDMA_DRV_SetSrcAddr(pState->DmaChannel[c], (uint32_t)pState->ChnState[i].data);
                    EDMA_DRV_SetMajorLoopIterationCount(pState->DmaChannel[c], pState->ChnState[i].count);
                    (void)EDMA_DRV_StartChannel(pState->DmaChannel[c]);
                    c++;
               }
            }
        }
        else if (pState->mux == SAI_MUX_MEM)
        {
            temp = pState->ChnState[0].count * numChan;
            EDMA_DRV_SetSrcAddr(pState->DmaChannel[0], (uint32_t)pState->ChnState[0].data);
            EDMA_DRV_SetMajorLoopIterationCount(pState->DmaChannel[0], temp);
            /* Start the DMA channel */
            (void)EDMA_DRV_StartChannel(pState->DmaChannel[0]);
        }
        else
        {
            /* do nothing */
        }
        /* Enable tx DMA requests for the current instance */
        SAI_DRV_TxEnableFifoReqDma(inst);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveInt
 * Description   : Start receiving data using interrupt, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_ReceiveInt(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    uint8_t c = 0;
    uint8_t i;

    pState->status = STATUS_BUSY;
    if (pState->mux == SAI_MUX_LINE)
    {
        for (i = 0; i < pState->ChannelCount; i++)
        {
            DEV_ASSERT(data[i] != NULL);
            pState->ChnState[i].data = data[i];
        }
        pState->ChnState[0].count = pState->ChannelCount * count;
        if (pState->ChnState[0].count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(pState->ChnState[0].count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        pState->ChnState[0].data = data[0];
        pState->ChnState[0].count = count;
        if (count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(pState->ChnState[0].count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    else
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                pState->ChnState[i].data = data[c];
                c++;
            }
        }
        pState->ChnState[0].count = count;
        if (count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t) (count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    SAI_DRV_RxEnableFifoReqInt(inst);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveDMA
 * Description   : Start receiving data using dma, return immediately
 * Because using dma with channel linking makes request source has a chance to
 * assert once more after data is read from fifo (fifo level is already equal
 * watermark, which shouldn't assert request source anymore but it did). So
 * value zero shouldn't be used to set watermark for dma. A workaround is done by
 * receive count-1 first with watermark 1, then set watermark to 0 and receive last
 * data.
 *
 *END**************************************************************************/
static void SAI_DRV_ReceiveDma(uint32_t instNum,
                               uint8_t* data[],
                               uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    uint8_t numChan = pState->ChannelCount;
    uint8_t i;
    uint8_t c;
    uint32_t temp;
    uint32_t tempCount;
    edma_callback_t tempFunc;

    pState->status = STATUS_BUSY;
    if ((pState->mux == SAI_MUX_DISABLED) || (pState->mux == SAI_MUX_LINE))
    {
        if (numChan > 1U)
        {
            /* work around for channel linking dma request assertion */
            if (count > 1UL)
            {
                SAI_DRV_RxSetWatermark(inst, 1U);
                tempCount = count - 1UL;
                tempFunc = SAI_DRV_AlmostCompleteReceiveDma;
            }
            else
            {
                SAI_DRV_RxSetWatermark(inst, 0U);
                tempCount = 1UL;
                tempFunc = SAI_DRV_CompleteReceiveDataUsingDma;
            }
            c = 0;
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    DEV_ASSERT(data[c] != NULL);
                    pState->ChnState[i].count = count;
                    pState->ChnState[i].data = data[c];
                    EDMA_DRV_SetDestAddr(pState->DmaChannel[c], (uint32_t) data[c]);
                    EDMA_DRV_SetMajorLoopIterationCount(pState->DmaChannel[c], tempCount);
                    (void)EDMA_DRV_InstallCallback(pState->DmaChannel[c],
                                                       (edma_callback_t)(tempFunc),
                                                       (void*)instNum);
                    /* Start the DMA channel */
                    (void)EDMA_DRV_StartChannel(pState->DmaChannel[c]);
                    c++;
                }
            }
        }
        else /* if there is only 1 channel enabled, no work around to increase speed */
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    EDMA_DRV_SetDestAddr(pState->DmaChannel[0], (uint32_t) data[0]);
                    EDMA_DRV_SetMajorLoopIterationCount(pState->DmaChannel[0], count);
                    /* Start the DMA channel */
                    (void)EDMA_DRV_StartChannel(pState->DmaChannel[0]);
                    break;
                }

            }
        }
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        temp = count * numChan;
        pState->ChnState[0].count = count;
        EDMA_DRV_SetDestAddr(pState->DmaChannel[0], (uint32_t) data[0]);
        EDMA_DRV_SetMajorLoopIterationCount(pState->DmaChannel[0], temp);
        /* Start the DMA channel */
        (void)EDMA_DRV_StartChannel(pState->DmaChannel[0]);
    }
    else
    {
        DEV_ASSERT(false);
    }
    /* Enable rx DMA requests for the current instance */
    SAI_DRV_RxEnableFifoReqDma(inst);
}

#ifdef FEATURE_SAI_MSEL_FCD
/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_FCDInit
* Description   : Init fractional clock divider
*                 Only DIVIDE in register is filled for 50% duty cycle
* Implements    : SAI_DRV_FCDInit_Activity
*END*************************************************************************/
void SAI_DRV_FCDInit(uint32_t instNum,
                     sai_fcd_clock_source_t fcdSource,
                     uint32_t freqDivisor,
                     bool MclkBypassFcd)
{
    SAI_Type* inst = SAIBase[instNum];
    clock_names_t clkName = END_OF_PERIPHERAL_CLKS;
    uint32_t divisor = 0U;
    uint32_t SourceClock;

    if (!MclkBypassFcd)
    {
        DEV_ASSERT(freqDivisor > 0UL);
        if ((fcdSource == SAI_FCD_PLL) || (fcdSource == SAI_FCD_FXOSC))
        {
            if (fcdSource == SAI_FCD_PLL)
            {
                clkName = PLL_PHI0_CLK;
            }
            else if (fcdSource == SAI_FCD_FXOSC)
            {
                clkName = FXOSC_CLK;
            }
            else
            {
                /* do nothing */
            }
            (void)CLOCK_SYS_GetFreq(clkName, &SourceClock);
            DEV_ASSERT(SourceClock > 0U);
            divisor = SourceClock / freqDivisor;
            if (divisor == 0UL)
            {
                divisor = 1UL;
            }
        }
        else if (fcdSource == SAI_FCD_MCLK)
        {
            DEV_ASSERT(freqDivisor > 0UL);
            divisor = freqDivisor;
        }
        else
        {
            /* do nothing */
        }
        if (divisor > 0x1000UL) /* DIVIDE field is 12 bit */
        {
            divisor = 0x1000UL;
        }
        inst->MDR = SAI_MDR_DIVIDE(divisor - 1UL);
        inst->MCR = SAI_MCR_MOE(1U) | SAI_MCR_MICS(fcdSource);
    }
    else
    {
        inst->MCR = 0;
    }
}
#endif /* FEATURE_SAI_MSEL_FCD */

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDmaInit
 * Description   : Setup dma, separated from original init function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxDmaInit(uint32_t instNum,
                              SAI_Type* inst,
                              sai_state_t* pState)
{
    edma_transfer_size_t xferSize = EDMA_TRANSFER_SIZE_1B;
    uint8_t numChan = 0U;
    edma_loop_transfer_config_t loopConfig;
    edma_transfer_config_t edmaConfig;
    uint8_t c;
    uint8_t i;

    numChan = pState->ChannelCount;
    if (pState->ElementSize == 1U)
    {
        xferSize = EDMA_TRANSFER_SIZE_1B;
    }
    else if (pState->ElementSize == 2U)
    {
        xferSize = EDMA_TRANSFER_SIZE_2B;
    }
    else if (pState->ElementSize == 4U)
    {
        xferSize = EDMA_TRANSFER_SIZE_4B;
    }
    else
    {
        DEV_ASSERT(false);
    }
    edmaConfig.srcTransferSize = xferSize;
    edmaConfig.destTransferSize = xferSize;
    edmaConfig.srcOffset = (int16_t) pState->ElementSize;
    edmaConfig.destOffset = 0;
    edmaConfig.srcLastAddrAdjust = 0;
    edmaConfig.destLastAddrAdjust = 0;
    edmaConfig.srcModulo = EDMA_MODULO_OFF;
    edmaConfig.destModulo = EDMA_MODULO_OFF;
    edmaConfig.minorByteTransferCount = pState->ElementSize;
    edmaConfig.scatterGatherEnable = false;
    edmaConfig.loopTransferConfig = &loopConfig;
    edmaConfig.interruptEnable = true;
    edmaConfig.srcAddr = 0;
    loopConfig.srcOffsetEnable = false;
    loopConfig.dstOffsetEnable = false;
    loopConfig.minorLoopOffset = 0;
    if ((pState->mux == SAI_MUX_LINE) || (pState->mux == SAI_MUX_DISABLED))
    {
        c = 0;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                if (c != (numChan - 1U))
                {
                    loopConfig.minorLoopChnLinkEnable = true;
                    loopConfig.majorLoopChnLinkEnable = true;
                    loopConfig.minorLoopChnLinkNumber = pState->DmaChannel[c+1U];
                    loopConfig.majorLoopChnLinkNumber = pState->DmaChannel[c+1U];
                    edmaConfig.interruptEnable = false;
                }
                else
                {
                    loopConfig.minorLoopChnLinkEnable = false;
                    loopConfig.majorLoopChnLinkEnable = false;
                    loopConfig.minorLoopChnLinkNumber = 0U;
                    loopConfig.majorLoopChnLinkNumber = 0U;
                    edmaConfig.interruptEnable = true;

                }
                (void)EDMA_DRV_InstallCallback(pState->DmaChannel[c],
                                               (edma_callback_t)(SAI_DRV_CompleteSendDataUsingDma),
                                               (void*)instNum);
#ifdef FEATURE_SAI_BIG_ENDIAN
                edmaConfig.destAddr = (uint32_t)(&(inst->TDR[i]))+DATA_REG_SIZE-pState->ElementSize;
#else
                edmaConfig.destAddr = (uint32_t)(&(inst->TDR[i]));
#endif
                (void)EDMA_DRV_ConfigLoopTransfer(pState->DmaChannel[c], &edmaConfig);
                EDMA_DRV_ConfigureInterrupt(pState->DmaChannel[c], EDMA_CHN_ERR_INT, true);
                EDMA_DRV_ConfigureInterrupt(pState->DmaChannel[c], EDMA_CHN_HALF_MAJOR_LOOP_INT, false);
                EDMA_DRV_DisableRequestsOnTransferComplete(pState->DmaChannel[c], true);
                /* Call driver function to end the transmission when the DMA transfer is done */
                c++;
           }
        }
    }
    else /* MUX MEM */
    {
#ifdef FEATURE_SAI_BIG_ENDIAN
        edmaConfig.destAddr = (uint32_t)(&(inst->TDR[0]))+DATA_REG_SIZE-pState->ElementSize;
#else
        edmaConfig.destAddr = (uint32_t)(&(inst->TDR[0]));
#endif
        loopConfig.minorLoopChnLinkEnable = false;
        loopConfig.majorLoopChnLinkEnable = false;
        loopConfig.minorLoopChnLinkNumber = 0U;
        loopConfig.majorLoopChnLinkNumber = 0U;
        (void)EDMA_DRV_ConfigLoopTransfer(pState->DmaChannel[0], &edmaConfig);
        EDMA_DRV_DisableRequestsOnTransferComplete(pState->DmaChannel[0], true);
        /* Call driver function to end the transmission when the DMA transfer is done */
        (void)EDMA_DRV_InstallCallback(pState->DmaChannel[0],
                                       (edma_callback_t)(SAI_DRV_CompleteSendDataUsingDma),
                                       (void*)(instNum));
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDmaInit
 * Description   : Setup dma, separated from original init function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_RxDmaInit(uint32_t instNum,
                              SAI_Type* inst,
                              sai_state_t* pState)
{
    edma_transfer_size_t xferSize = EDMA_TRANSFER_SIZE_1B;
    uint8_t numChan = 0U;
    edma_loop_transfer_config_t loopConfig;
    edma_transfer_config_t edmaConfig;
    uint8_t c;
    uint8_t i;

    numChan = pState->ChannelCount;
    if (pState->ElementSize == 1U)
    {
        xferSize = EDMA_TRANSFER_SIZE_1B;
    }
    else if (pState->ElementSize == 2U)
    {
        xferSize = EDMA_TRANSFER_SIZE_2B;
    }
    else if (pState->ElementSize == 4U)
    {
        xferSize = EDMA_TRANSFER_SIZE_4B;
    }
    else
    {
        DEV_ASSERT(false);
    }
    edmaConfig.srcTransferSize = xferSize;
    edmaConfig.destTransferSize = xferSize;
    edmaConfig.srcOffset = 0;
    edmaConfig.destOffset = (int16_t) pState->ElementSize;
    edmaConfig.srcLastAddrAdjust = 0;
    edmaConfig.destLastAddrAdjust = 0;
    edmaConfig.srcModulo = EDMA_MODULO_OFF;
    edmaConfig.destModulo = EDMA_MODULO_OFF;
    edmaConfig.minorByteTransferCount = pState->ElementSize;
    edmaConfig.scatterGatherEnable = false;
    edmaConfig.loopTransferConfig = &loopConfig;
    edmaConfig.interruptEnable = true;
    edmaConfig.destAddr = 0;
    loopConfig.srcOffsetEnable = false;
    loopConfig.dstOffsetEnable = false;
    loopConfig.minorLoopOffset = 0;
    c = 0;
    if ((pState->mux == SAI_MUX_DISABLED) || (pState->mux == SAI_MUX_LINE))
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                if (c != (numChan - 1U))
                {
                    loopConfig.minorLoopChnLinkEnable = true;
                    loopConfig.majorLoopChnLinkEnable = true;
                    loopConfig.minorLoopChnLinkNumber = pState->DmaChannel[c+1U];
                    loopConfig.majorLoopChnLinkNumber = pState->DmaChannel[c+1U];
                    edmaConfig.interruptEnable = false;
                }
                else
                {
                    loopConfig.minorLoopChnLinkEnable = false;
                    loopConfig.majorLoopChnLinkEnable = false;
                    loopConfig.minorLoopChnLinkNumber = 0U;
                    loopConfig.majorLoopChnLinkNumber = 0U;
                    edmaConfig.interruptEnable = true;
                }
#ifdef FEATURE_SAI_BIG_ENDIAN
                edmaConfig.srcAddr = (uint32_t)(&(inst->RDR[i]))+DATA_REG_SIZE-pState->ElementSize;
#else
                edmaConfig.srcAddr = (uint32_t)(&(inst->RDR[i]));
#endif
                (void)EDMA_DRV_ConfigLoopTransfer(pState->DmaChannel[c], &edmaConfig);
                EDMA_DRV_ConfigureInterrupt(pState->DmaChannel[c], EDMA_CHN_ERR_INT, true);
                EDMA_DRV_ConfigureInterrupt(pState->DmaChannel[c], EDMA_CHN_HALF_MAJOR_LOOP_INT, false);
                EDMA_DRV_DisableRequestsOnTransferComplete(pState->DmaChannel[c], true);
                if (numChan == 1U)
                {
                    SAI_DRV_RxSetWatermark(inst, 0U);
                    (void)EDMA_DRV_InstallCallback(pState->DmaChannel[0],
                                                   (edma_callback_t)(SAI_DRV_CompleteReceiveDataUsingDma),
                                                   (void*)instNum);
                    break;

                }
                c++;
            }
        }
    }
    else /* MUX_MEM */
    {
        SAI_DRV_RxSetWatermark(inst, 0U);
        loopConfig.minorLoopChnLinkEnable = false;
        loopConfig.majorLoopChnLinkEnable = false;
        loopConfig.minorLoopChnLinkNumber = 0U;
        loopConfig.majorLoopChnLinkNumber = 0U;
#ifdef FEATURE_SAI_BIG_ENDIAN
        edmaConfig.srcAddr = (uint32_t)(&(inst->RDR[0]))+DATA_REG_SIZE-pState->ElementSize;
#else
        edmaConfig.srcAddr = (uint32_t)(&(inst->RDR[0]));
#endif
        (void)EDMA_DRV_ConfigLoopTransfer(pState->DmaChannel[0], &edmaConfig);
        EDMA_DRV_DisableRequestsOnTransferComplete(pState->DmaChannel[0], true);
        /* Call driver function to end the transmission when the DMA transfer is done */
        (void)EDMA_DRV_InstallCallback(pState->DmaChannel[0],
                                       (edma_callback_t)(SAI_DRV_CompleteReceiveDataUsingDma),
                                       (void*)(instNum));
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_InternalClock
 * Description   : Setup internal clock, separated from original init function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_InternalClock(uint32_t instNum,
                                  const sai_user_config_t* saiUserConfig,
                                  bool *clkInternal,
                                  uint32_t *divisor,
                                  uint32_t *SourceClock)
{
    if (saiUserConfig->BitClkInternal)
    {
        switch(saiUserConfig->MasterClkSrc)
        {
#ifdef FEATURE_SAI_MSEL_FCD
            case SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, SourceClock);
                *clkInternal = ((*SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case SAI_BUS_CLK:
                *clkInternal = true;
    #if (defined(MPC574x_SERIES))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case SAI_SOSC_CLK:
                *clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, SourceClock);
    #endif
                break;
#endif
            default:
                /* do nothing */
                break;
        }
        if (*clkInternal)
        {
            DEV_ASSERT(*SourceClock > 0UL);
            DEV_ASSERT(saiUserConfig->BitClkFreq != 0U);
            *divisor = ((*SourceClock / saiUserConfig->BitClkFreq) / 2UL);
            if (*divisor > 0U)
            {
                *divisor -= 1UL;
            }
            if (*divisor > 255U)
            {
                *divisor = 255U;
            }
        }
        else
        {
            *divisor = ((uint32_t)saiUserConfig->BitClkDiv / 2UL);
            if (*divisor > 0U)
            {
                *divisor -= 1UL;
            }
            if (*divisor > 255U)
            {
                *divisor = 255U;
            }
        }
    }
    (void)instNum; /* avoid warning */
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxInitCheckSync
* Description   : Check dev assert for syncing
*END*************************************************************************/
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void SAI_DRV_TxInitCheckSync(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t rcsr = inst->RCSR;
    /* check if sync with other and other must be enabled later */
    DEV_ASSERT((saiUserConfig->SyncMode != SAI_SYNC_WITH_OTHER) ||
              ((saiUserConfig->SyncMode == SAI_SYNC_WITH_OTHER) && ((rcsr & SAI_RCSR_RE_MASK) == 0U)));
#ifdef FEATURE_SAI_SYNC_WITH_OTHER_INST
    uint32_t sai2Tcsr;
    uint32_t sai2Rcsr;
    /* sai2 cannot sync with sai2 */
    DEV_ASSERT(!((instNum == 2UL) &&
                 ((saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME) || (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER))));
    /* sync with sai2 and sai2 same part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME)
    {
        sai2Tcsr = SAIBase[2U]->TCSR;
        DEV_ASSERT((sai2Tcsr & SAI_TCSR_TE_MASK) == 0U);
    }
    /* sync with sai2 and sai2 other part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER)
    {
        sai2Rcsr = SAIBase[2U]->RCSR;
        DEV_ASSERT((sai2Rcsr & SAI_RCSR_RE_MASK) == 0U);
    }
#endif /* FEATURE_SAI_SYNC_WITH_OTHER_INST */
}
#endif /* DEV_ERROR_DETECT */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxInitCheck
* Description   : Check dev assert
*END*************************************************************************/
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void SAI_DRV_TxInitCheck(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig,
                                sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;

    DEV_ASSERT((saiUserConfig != NULL) && (StateAlloc != NULL) && (instNum < SAI_INSTANCE_COUNT));
    /* Check if current instance is clock gated off. */
    DEV_ASSERT(CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock) == STATUS_SUCCESS);
    DEV_ASSERT((saiUserConfig->FrameSize <= (1U << (SAI_DRV_GetParamFrameSize(inst)))) && (saiUserConfig->FrameSize > 0U));
    SAI_DRV_TxInitCheckSync(instNum, saiUserConfig);
    DEV_ASSERT((saiUserConfig->Word0Width > 0U) && (saiUserConfig->WordNWidth > 0U) && (saiUserConfig->SyncWidth > 0U) && (saiUserConfig->FirstBitIndex < 32U));
    DEV_ASSERT((saiUserConfig->ElementSize == 1U) || (saiUserConfig->ElementSize == 2U) || (saiUserConfig->ElementSize == 4U));
}
#endif /* DEV_ERROR_DETECT */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxInit
* Description   : Init tx core
* These params are set by driver:
*      bool BIT_CLK_AS_EXTERNAL = false;      < Delay internal clock as if clock is generated externally.>
*      uint8_t BitClkDiv = calculate for nearest value to user config value,
*                       only set by driver if master clock and bit clock is internal, otherwise set by user
*                                          < If bit clock is internal, it is divided from master clock by this.>
*      uint8_t WORD_FLAG_INDEX = 0;          < Index of word in frame that raise word start flag.>
*      bool CONT_ON_ERROR = true;            < Continue on fifo error.>
*      sai_combine_mode CombineMode = combine line or combine memory, user choice
*                                          < Select fifo combine mode.>
*      sai_packing_mode PACK_MODE = false;     < Select fifo pack mode.>
*      bool SYNC_ON_DEMAND; = true            < Generate frame sync only when fifo is not empty (transmit)
*                                             or not full (receive).>
* Implements    : SAI_DRV_TxInit_Activity
*END*************************************************************************/
void SAI_DRV_TxInit(uint32_t instNum,
                   const sai_user_config_t* saiUserConfig,
                   sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;
    uint32_t divisor = 0U;
    uint8_t CombineMode = 0U;
    uint8_t channel = 0U;
    bool clkInternal = false;
    status_t osifError;
    uint8_t i;
    sai_state_t* pState;

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    SAI_DRV_TxInitCheck(instNum, saiUserConfig, StateAlloc);
#endif
    TxState[instNum] = StateAlloc;
    pState = StateAlloc;
    SAI_DRV_TxResetVar(instNum);
    /* calculate divisor parameter */
    /* Get clock as configured in the clock manager */
    SAI_DRV_InternalClock(instNum, saiUserConfig, &clkInternal, &divisor, &SourceClock);
    pState->ElementSize = saiUserConfig->ElementSize;
    pState->XferType = saiUserConfig->TransferType;
    pState->mux = saiUserConfig->MuxMode;
    channel = saiUserConfig->ChannelEnable;
    if (saiUserConfig->TransferType == SAI_DMA)
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            pState->DmaChannel[i] = saiUserConfig->DmaChannel[i];
        }
        CombineMode = (uint8_t)saiUserConfig->MuxMode;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if ((channel & (1U << i)) != 0U)
            {
                pState->ChannelCount++;
            }
        }
    }
    else
    {
        CombineMode = 0U; /* emulate combine mode */
        pState->ChannelCount = saiUserConfig->ChannelCount;
    }
    osifError = OSIF_SemaCreate(&pState->Sema, 0U);
    DEV_ASSERT(osifError == STATUS_SUCCESS);
    (void) osifError;
    inst->TCSR = SAI_TCSR_SR(1U);  /* call reset bit, reset fifo and logic */
    inst->TCSR   = SAI_TCSR_FEIE(TO_BIT(saiUserConfig->RunErrorReport)) |
                   SAI_TCSR_SEIE(TO_BIT(saiUserConfig->SyncErrorReport)) |
                   SAI_TCSR_WSIE(TO_BIT(saiUserConfig->FrameStartReport));
    pState->Callback = saiUserConfig->callback;
    pState->callbackParam = saiUserConfig->callbackParam;
    inst->TCR1 = SAI_TCR1_TFW(TX_WATERMARK);
    inst->TCR2 = SAI_TCR2_SYNC(saiUserConfig->SyncMode) |
                   SAI_TCR2_BCI(TO_BIT(BIT_CLK_AS_EXTERNAL)) |
                   SAI_TCR2_MSEL(saiUserConfig->MasterClkSrc) |
                   SAI_TCR2_BCP(TO_BIT(saiUserConfig->BitClkNegPolar)) |
                   SAI_TCR2_BCD(TO_BIT(saiUserConfig->BitClkInternal)) |
                   SAI_TCR2_DIV((uint8_t) divisor);
    inst->TCR3 = SAI_TCR3_TCE(channel) |
                   SAI_TCR3_WDFL(WORD_FLAG_INDEX);
    inst->TCR4 = SAI_TCR4_FCONT(TO_BIT(CONT_ON_ERROR)) |
                   SAI_TCR4_FCOMB(CombineMode) |
                   SAI_TCR4_FPACK(PACK_MODE) |
                   SAI_TCR4_FRSZ((uint32_t)saiUserConfig->FrameSize - 1U) |
                   SAI_TCR4_SYWD((uint32_t)saiUserConfig->SyncWidth - 1U) |
#ifdef FEATURE_SAI_HAS_CHMOD
                   SAI_TCR4_CHMOD(saiUserConfig->MaskMode) |
#endif /* FEATURE_SAI_HAS_CHMOD */
                   SAI_TCR4_MF(TO_BIT(saiUserConfig->MsbFirst)) |
                   SAI_TCR4_FSE(TO_BIT(saiUserConfig->SyncEarly)) |
                   SAI_TCR4_ONDEM(TO_BIT(SYNC_ON_DEMAND)) |
                   SAI_TCR4_FSP(TO_BIT(saiUserConfig->SyncNegPolar)) |
                   SAI_TCR4_FSD(TO_BIT(saiUserConfig->SyncInternal));
    inst->TCR5 = SAI_TCR5_WNW((uint32_t)saiUserConfig->WordNWidth - 1U) |
                   SAI_TCR5_W0W((uint32_t)saiUserConfig->Word0Width - 1U) |
                   SAI_TCR5_FBT(saiUserConfig->MsbFirst ? ~(31U - (uint32_t)saiUserConfig->FirstBitIndex) : (uint32_t)saiUserConfig->FirstBitIndex);
    pState->cr4 = inst->TCR4;
    SAI_DRV_TxResetFifo(inst);
    /* Unset software reset bit */
    inst->TCSR &= ~SAI_TCSR_SR_MASK;
    if ((saiUserConfig->TransferType == SAI_INTERRUPT) || (saiUserConfig->RunErrorReport) ||
        (saiUserConfig->SyncErrorReport) || (saiUserConfig->FrameStartReport))
    {
        INT_SYS_EnableIRQ(SAITxIrqId[instNum]);
    }
    if (saiUserConfig->TransferType == SAI_DMA)
    {
        SAI_DRV_TxDmaInit(instNum, inst, pState);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxInitCheck
* Description   : Check dev assert
*END*************************************************************************/
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void SAI_DRV_RxInitCheck(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig,
                                sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;

    DEV_ASSERT((saiUserConfig != NULL) && (StateAlloc != NULL) && (instNum < SAI_INSTANCE_COUNT));
    /* Check if current instance is clock gated off. */
    DEV_ASSERT(CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock) == STATUS_SUCCESS);
    DEV_ASSERT((saiUserConfig->FrameSize <= (1U << (SAI_DRV_GetParamFrameSize(inst)))) && (saiUserConfig->FrameSize > 0U));
    SAI_DRV_RxInitCheckSync(instNum, saiUserConfig);
    DEV_ASSERT((saiUserConfig->Word0Width > 0U) && (saiUserConfig->WordNWidth > 0U) && (saiUserConfig->SyncWidth > 0U) && (saiUserConfig->FirstBitIndex < 32U));
    DEV_ASSERT((saiUserConfig->ElementSize == 1U) || (saiUserConfig->ElementSize == 2U) || (saiUserConfig->ElementSize == 4U));
}
#endif /* DEV_ERROR_DETECT */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxInitCheckSync
* Description   : Check dev assert for syncing
*END*************************************************************************/
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
static void SAI_DRV_RxInitCheckSync(uint32_t instNum,
                                const sai_user_config_t* saiUserConfig)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t tcsr = inst->TCSR;
    /* check if sync with other and other is not enabled */
    DEV_ASSERT((saiUserConfig->SyncMode != SAI_SYNC_WITH_OTHER) ||
              ((saiUserConfig->SyncMode == SAI_SYNC_WITH_OTHER) && ((tcsr & SAI_TCSR_TE_MASK) == 0U)));
#ifdef FEATURE_SAI_SYNC_WITH_OTHER_INST
    uint32_t sai2Tcsr;
    uint32_t sai2Rcsr;
    /* sai2 cannot sync with sai2 */
    DEV_ASSERT(!((instNum == 2UL) &&
                 ((saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME) || (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER))));
    /* if sync with sai2 then sai2 same part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER)
    {
        sai2Tcsr = SAIBase[2U]->TCSR;
        DEV_ASSERT((sai2Tcsr & SAI_TCSR_TE_MASK) == 0U);
    }
    /* if sync with sai2 then sai2 other part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME)
    {
        sai2Rcsr = SAIBase[2U]->RCSR;
        DEV_ASSERT((sai2Rcsr & SAI_RCSR_RE_MASK) == 0U);
    }
#endif /* FEATURE_SAI_SYNC_WITH_OTHER_INST */
}
#endif /* DEV_ERROR_DETECT */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxInit
* Description   : Init rx core
* These params are set by driver:
*      Others are the same as TxInit
* Implements    : SAI_DRV_RxInit_Activity
*END*************************************************************************/
void SAI_DRV_RxInit(uint32_t instNum,
                    const sai_user_config_t* saiUserConfig,
                    sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;
    uint32_t divisor = 0U;
    uint8_t CombineMode = 0U;
    uint8_t channel = 0U;
    bool clkInternal = false;
    status_t osifError;
    uint8_t i;
    sai_state_t* pState;

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    SAI_DRV_RxInitCheck (instNum, saiUserConfig, StateAlloc);
#endif
    RxState[instNum] = StateAlloc;
    pState = StateAlloc;
    SAI_DRV_RxResetVar(instNum);
    /* calculate divisor parameter */
    /* Get clock as configured in the clock manager */
    SAI_DRV_InternalClock(instNum, saiUserConfig, &clkInternal, &divisor, &SourceClock);
    pState->ElementSize = saiUserConfig->ElementSize;
    pState->XferType = saiUserConfig->TransferType;
    pState->mux = saiUserConfig->MuxMode;
    channel = saiUserConfig->ChannelEnable;
    if (saiUserConfig->TransferType == SAI_DMA)
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            pState->DmaChannel[i] = saiUserConfig->DmaChannel[i];
        }
        CombineMode = (uint8_t)saiUserConfig->MuxMode;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if ((channel & (1U << i)) != 0U)
            {
                pState->ChannelCount++;
            }
        }

    }
    else
    {
        CombineMode = 0U; /* emulate combine mode */
        pState->ChannelCount = saiUserConfig->ChannelCount;
    }
    osifError = OSIF_SemaCreate(&pState->Sema, 0U);
    DEV_ASSERT(osifError == STATUS_SUCCESS);
    (void) osifError;
    inst->RCSR |= SAI_RCSR_FR_MASK; /* reset rx fifo to clear junk data before receive */
    inst->RCSR = SAI_RCSR_SR(1UL);  /* call reset bit, reset fifo and logic */
    inst->RCSR   = SAI_RCSR_FEIE(TO_BIT(saiUserConfig->RunErrorReport)) |
                   SAI_RCSR_SEIE(TO_BIT(saiUserConfig->SyncErrorReport)) |
                   SAI_RCSR_WSIE(TO_BIT(saiUserConfig->FrameStartReport));
    pState->Callback = saiUserConfig->callback;
    pState->callbackParam = saiUserConfig->callbackParam;
    inst->RCR1 = SAI_RCR1_RFW(RX_WATERMARK);
    inst->RCR2 = SAI_RCR2_SYNC(saiUserConfig->SyncMode) |
                   SAI_RCR2_BCI(TO_BIT(BIT_CLK_AS_EXTERNAL)) |
                   SAI_RCR2_MSEL(saiUserConfig->MasterClkSrc) |
                   SAI_RCR2_BCP(TO_BIT(saiUserConfig->BitClkNegPolar)) |
                   SAI_RCR2_BCD(TO_BIT(saiUserConfig->BitClkInternal)) |
                   SAI_RCR2_DIV((uint8_t) divisor);
    inst->RCR3 = SAI_RCR3_RCE(channel) |
                   SAI_RCR3_WDFL(WORD_FLAG_INDEX);
    inst->RCR4 = SAI_RCR4_FCONT(TO_BIT(CONT_ON_ERROR)) |
                   SAI_RCR4_FCOMB(CombineMode) |
                   SAI_RCR4_FPACK(PACK_MODE) |
                   SAI_RCR4_FRSZ(saiUserConfig->FrameSize - 1UL) |
                   SAI_RCR4_SYWD(saiUserConfig->SyncWidth - 1UL) |
                   SAI_RCR4_MF(TO_BIT(saiUserConfig->MsbFirst)) |
                   SAI_RCR4_FSE(TO_BIT(saiUserConfig->SyncEarly)) |
                   SAI_RCR4_ONDEM(TO_BIT(SYNC_ON_DEMAND)) |
                   SAI_RCR4_FSP(TO_BIT(saiUserConfig->SyncNegPolar)) |
                   SAI_RCR4_FSD(TO_BIT(saiUserConfig->SyncInternal));
    inst->RCR5 = SAI_RCR5_WNW((uint32_t)saiUserConfig->WordNWidth - 1UL) |
                   SAI_RCR5_W0W((uint32_t)saiUserConfig->Word0Width - 1UL) |
                   SAI_RCR5_FBT(saiUserConfig->MsbFirst ? ~(31UL - (uint32_t)saiUserConfig->FirstBitIndex) : (uint32_t)saiUserConfig->FirstBitIndex);
    SAI_DRV_RxResetFifo(inst);
    pState->cr4 = inst->RCR4;
    inst->RCSR &= ~SAI_RCSR_SR_MASK; /* clear software reset bit */
    if ((saiUserConfig->TransferType == SAI_INTERRUPT) || (saiUserConfig->RunErrorReport) ||
        (saiUserConfig->SyncErrorReport) || (saiUserConfig->FrameStartReport))
    {
        INT_SYS_EnableIRQ(SAIRxIrqId[instNum]);
    }
    if (saiUserConfig->TransferType ==  SAI_DMA)
    {
        SAI_DRV_RxDmaInit(instNum, inst, pState);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDeinit
 * Description   : DeInit tx core
 * Implements    : SAI_DRV_TxDeinit_Activity
 *
 *END**************************************************************************/
void SAI_DRV_TxDeinit(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];

    inst->TCSR = 0; /* clear all interrupt enable bits */
    inst->TCSR |= SAI_TCSR_FR_MASK; /* reset tx fifo to avoid line is driven by tx even after disabled */
    inst->TCSR = SAI_TCSR_SR(1U);  /* call reset bit, reset fifo and logic */
    inst->TCR4 = 0; /* need this for rx/tx init after tx/rx deinit to work */
    inst->TCR2 = 0; /* need this for rx/tx init after tx/rx deinit to work */
    inst->TMR = 0; /* reset mask bits */
    if (pState->XferType == SAI_INTERRUPT)
    {
        /* Disable interrupt. */
        INT_SYS_DisableIRQ(SAITxIrqId[instNum]);
    }
    (void)OSIF_SemaDestroy(&pState->Sema);
    SAI_DRV_TxResetVar(instNum);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDeinit
 * Description   : DeInit rx core
 * Implements    : SAI_DRV_RxDeinit_Activity
 *
 *END**************************************************************************/
void SAI_DRV_RxDeinit(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    sai_state_t* pState = RxState[instNum];
    SAI_Type* inst = SAIBase[instNum];

    inst->RCSR = 0; /* clear all interrupt enable bits */
    inst->RCR4 = 0; /* need this for rx/tx init after tx/rx deinit to work */
    inst->RCR2 = 0; /* need this for rx/tx init after tx/rx deinit to work */
    inst->RMR = 0; /* reset mask bits */
    if (pState->XferType == SAI_INTERRUPT)
    {
        /* Disable interrupt. */
        INT_SYS_DisableIRQ(SAIRxIrqId[instNum]);
    }
    (void)OSIF_SemaDestroy(&pState->Sema);
    SAI_DRV_RxResetVar(instNum);
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxGetBitClockFreq
* Description   : Return true freq
* Implements    : SAI_DRV_RxGetBitClockFreq_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_RxGetBitClockFreq(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;
    bool clkInternal = false;
    uint32_t div;
    uint32_t clkSource;

    /* check if sync with other */
    DEV_ASSERT ((inst->RCR2 & SAI_RCR2_SYNC_MASK) == 0U);
    /* check if master clock is internal */
    clkSource = ((inst->RCR2 & SAI_RCR2_MSEL_MASK) >> SAI_RCR2_MSEL_SHIFT);
    switch (clkSource)
    {
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case (uint32_t)SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(MPC574x_SERIES))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK:
                clkInternal = false;
                break;
#endif
#ifdef FEATURE_SAI_MSEL_FCD
            case (uint32_t)SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case (uint32_t)SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_OTHER_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK_1:
            case (uint32_t)SAI_EXTERNAL_CLK_2:
                clkInternal = false;
                break;
#endif
            default:
                DEV_ASSERT(false);
                break;
    }
    /* check if external master clock or external bit clock */
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    uint32_t rcr2 = inst->RCR2;
    DEV_ASSERT (clkInternal && ((rcr2 & SAI_TCR2_BCD_MASK) == SAI_RCR2_BCD(1U)));
#endif /* DEV_ERROR_DETECT */
    (void) clkInternal;

    div = SAI_DRV_RxGetBitClockDiv(instNum);
    return SourceClock / div;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxGetBitClockFreq
* Description   : Return true freq
* Implements    : SAI_DRV_TxGetBitClockFreq_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_TxGetBitClockFreq(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;
    bool clkInternal = false;
    uint32_t div;
    uint32_t clkSource;

    /* check if sync with other */
    DEV_ASSERT ((inst->TCR2 & SAI_TCR2_SYNC_MASK) == 0U);
    /* check if master clock is internal */
    clkSource = (inst->TCR2 & SAI_TCR2_MSEL_MASK) >> SAI_TCR2_MSEL_SHIFT;
    switch (clkSource)
    {
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case (uint32_t)SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(MPC574x_SERIES))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK:
                clkInternal = false;
                break;
#endif
#ifdef FEATURE_SAI_MSEL_FCD
            case (uint32_t)SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case (uint32_t)SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_OTHER_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK_1:
            case (uint32_t)SAI_EXTERNAL_CLK_2:
                clkInternal = false;
                break;
#endif
            default:
                DEV_ASSERT(false);
                break;
    }
    /* check if external master clock or external bit clock */
#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    uint32_t tcr2 = inst->TCR2;
    DEV_ASSERT (clkInternal && ((tcr2 & SAI_TCR2_BCD_MASK) == SAI_TCR2_BCD(1U)));
#endif /* DEV_ERROR_DETECT */
    (void) clkInternal;

    div = SAI_DRV_TxGetBitClockDiv(instNum);
    return SourceClock / div;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxGetBitClockDiv
* Description   : Return true divisor
* Implements    : SAI_DRV_TxGetBitClockDiv_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_TxGetBitClockDiv(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    return (((inst->TCR2 & SAI_TCR2_DIV_MASK) >> SAI_TCR2_DIV_SHIFT) + 1U) * 2U;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxGetBitClockDiv
* Description   : Return true divisor
* Implements    : SAI_DRV_RxGetBitClockDiv_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_RxGetBitClockDiv(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    return (((inst->RCR2 & SAI_RCR2_DIV_MASK) >> SAI_RCR2_DIV_SHIFT) + 1U) * 2U;
}

#ifdef FEATURE_SAI_MSEL_FCD
/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_GetFCDFreq
* Description   : Return true fcd frequency and if clock is from internal
* Implements    : SAI_DRV_GetFCDFreq_Activity
*
*END**************************************************************************/
void SAI_DRV_GetFCDFreq(uint32_t instNum, uint32_t* freq)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    clock_names_t clkName = END_OF_PERIPHERAL_CLKS;
    uint32_t source;
    uint32_t SourceClock;
    uint32_t div;

    *freq = 0;
    /* check if fcd is bypassed */
    if ((inst->MCR & SAI_MCR_MOE_MASK) != 0UL)
    {
        source = ((inst->MCR & SAI_MCR_MICS_MASK) >> SAI_MCR_MICS_SHIFT);
        if ((source == (uint32_t)SAI_FCD_PLL) || (source == (uint32_t)SAI_FCD_FXOSC))
        {
            if (source == (uint32_t)SAI_FCD_PLL)
            {
                clkName = PLL_PHI0_CLK;
            }
            else if (source == (uint32_t)SAI_FCD_FXOSC)
            {
                clkName = FXOSC_CLK;
            }
            else
            {
                /* do nothing */
            }
            (void)CLOCK_SYS_GetFreq(clkName, &SourceClock);
            div = ((inst->MDR & SAI_MDR_DIVIDE_MASK) >> SAI_MDR_DIVIDE_SHIFT) + 1UL;
            *freq = SourceClock / div;
        }
    }
}
#endif /* FEATURE_SAI_MSEL_FCD */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxSetMaskWord
* Description   : Set next frame masked word index
* Implements    : SAI_DRV_TxSetNextMaskWords_Activity
*
*END**************************************************************************/
void SAI_DRV_TxSetNextMaskWords(uint32_t instNum, uint16_t Words)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->TMR = Words;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxSetMaskWord
* Description   : Set next frame masked word index
* Implements : SAI_DRV_RxSetNextMaskWords_Activity
*
*END**************************************************************************/
void SAI_DRV_RxSetNextMaskWords(uint32_t instNum, uint16_t Words)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->RMR = Words;
}

/*****************************SENDING FUNCTIONS************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendBlocking
 * Description   : Send block of data, return when finish sending
 * Implements    : SAI_DRV_SendBlocking_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_SendBlocking(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count,
                            uint32_t timeout)
{
    sai_state_t* pState = TxState[instNum];
    status_t osifError = STATUS_SUCCESS;
    status_t res;

    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(pState->status != STATUS_BUSY);

    pState->Blocking = true;
    SAI_DRV_Send (instNum, data, count);
    if (pState->status == STATUS_BUSY)
    {
        osifError = OSIF_SemaWait(&pState->Sema, timeout);
    }
    pState->Blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* abort current transfer */
        SAI_DRV_AbortSending (instNum);
        pState->status = STATUS_TIMEOUT;
        res = STATUS_TIMEOUT;
    }
    else
    {
        DEV_ASSERT(pState->status != STATUS_BUSY);
        /* not time out */
        res = pState->status;
    }
    return res;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillMuxLineInt
 * Description   : Fill fifo mux line interrupt, separated from original function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxFillMuxLineInt(uint32_t instNum,
                                     SAI_Type* inst,
                                     sai_state_t* pState,
                                     bool* finish)
{
    uint8_t i;
    bool isTxFull;

    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsTxChannelEnabled(inst, i))
        {
            isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            while (!((pState->ChnState[0].count == 0UL) || isTxFull))
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[pState->NextChn].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[pState->NextChn].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[pState->NextChn].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[pState->NextChn].data += pState->ElementSize;
                pState->NextChn++; /* alternate between data block */
                if (pState->NextChn == pState->ChannelCount)
                {
                    pState->NextChn = 0U;
                }
                pState->ChnState[0].count--;
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            }
            if (pState->ChnState[0].count == 0UL)
            {
                *finish = true;
            }
            break; /* only one channle enabled in this mode */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillMuxMemInt
 * Description   : Fill fifo mux mem interrupt, separated from original function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxFillMuxMemInt(uint32_t instNum,
                                    SAI_Type* inst,
                                    sai_state_t* pState,
                                    bool* finish)
{
    bool full = false;
    uint8_t i;

    while (!((pState->ChnState[0].count == 0UL) || (full)))
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            /* check the first enabled channel fifo */
            if (SAI_DRV_IsTxFifoFull(inst, i))
            {
                full = true;
                break;
            }
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[0].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[0U].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[0U].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[0U].data += pState->ElementSize;
            }
        }
        if (!full)
        {
            pState->ChnState[0U].count--;
        }
    }
    if (pState->ChnState[0].count == 0UL)
    {
        *finish = true;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillMuxMemInt
 * Description   : Fill fifo mux disable, interrupt, separated from original function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxFillMuxDisableInt(uint32_t instNum,
                                       SAI_Type* inst,
                                       sai_state_t* pState,
                                       bool* finish)
{
    uint32_t temp = 0UL;
    uint8_t i;
    bool isTxFull;

    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsTxChannelEnabled(inst, i))
        {
            isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            temp = pState->ChnState[0].count;
            while (!((temp == 0UL) || isTxFull))
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[i].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[i].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[i].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[i].data += pState->ElementSize;
                temp--;
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            }
        }
    }
    pState->ChnState[0].count = temp;
    if (pState->ChnState[0].count == 0UL)
    {
        *finish = true;
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillFifoInterrupt
 * Description   : Fill fifo before enable tx to avoid run error flag
 *
 *END**************************************************************************/
static bool SAI_DRV_TxFillFifoInterrupt(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    bool finish  = false;

    if (pState->mux == SAI_MUX_LINE)
    {
        SAI_DRV_TxFillMuxLineInt(instNum, inst, pState, &finish);
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        SAI_DRV_TxFillMuxMemInt(instNum, inst, pState, &finish);
    }
    else /* mux disabled */
    {
        SAI_DRV_TxFillMuxDisableInt(instNum, inst, pState, &finish);
    }
    if (finish)
    {
        pState->status = STATUS_SUCCESS;
        if (pState->Callback != NULL)
        {
            pState->Callback(SAI_TX_COMPLETE, pState->callbackParam);
        }
    }
    return finish;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillMuxLineDma
 * Description   : Fill fifo mux line dma, separated from original function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxFillMuxLineDma(uint32_t instNum,
                                     SAI_Type* inst,
                                     sai_state_t* pState,
                                     bool* finish)
{
    uint8_t enabledChannel = 0U;
    uint8_t i;
    bool isTxFull;

    for (i = 0U; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsTxChannelEnabled(inst, i))
        {
            enabledChannel = i;
            isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            /* all channel fifo should be in the same level */
            while (!((pState->ChnState[i].count == 0UL) || isTxFull))
            {
                switch(pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[i].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[i].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[i].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[i].data += pState->ElementSize;
                pState->ChnState[i].count--;
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
            }
        }
    }
    if (pState->ChnState[enabledChannel].count == 0UL)
    {
        *finish = true;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillMuxMemDma
 * Description   : Fill fifo mux mem dma, separated from original function
 *                 to reduce cyclomatic complexity
 *
 *END**************************************************************************/
static void SAI_DRV_TxFillMuxMemDma(uint32_t instNum,
                                    SAI_Type* inst,
                                    sai_state_t* pState,
                                    bool* finish)
{
    uint8_t i;
    bool full;

    full = false;
    while (!((pState->ChnState[0].count == 0UL) || (full)))
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            /* check the first enabled channel fifo */
            if (SAI_DRV_IsTxFifoFull(inst, i))
            {
                full = true;
                break;
            }
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                switch (pState->ElementSize)
                {
                    case 1U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*pState->ChnState[0].data);
                        break;
                    case 2U:
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) pState->ChnState[0U].data));
                        break;
                    case 4U:
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) pState->ChnState[0U].data));
                        break;
                    default:
                        DEV_ASSERT(false);
                        break;
                }
                pState->ChnState[0U].data += pState->ElementSize;
            }
        }
        if (!full)
        {
            pState->ChnState[0U].count--;
        }
    }
    if (pState->ChnState[0].count == 0UL)
    {
        *finish = true;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFillFifoDma
 * Description   : Fill fifo before enable tx to avoid run error
 *
 *END**************************************************************************/
static bool SAI_DRV_TxFillFifoDma(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = TxState[instNum];
    bool finish = false;

    if ((pState->mux == SAI_MUX_LINE) || (pState->mux == SAI_MUX_DISABLED))
    {
        SAI_DRV_TxFillMuxLineDma(instNum, inst, pState, &finish);
    }
    else if (pState->mux == SAI_MUX_MEM)
    {
        SAI_DRV_TxFillMuxMemDma(instNum, inst, pState, &finish);
    }
    else /* should not happen */
    {
        DEV_ASSERT(false);
    }
    if (finish)
    {
        pState->status = STATUS_SUCCESS;
        if (pState->Callback != NULL)
        {
            pState->Callback(SAI_TX_COMPLETE, pState->callbackParam);
        }
    }
    return finish;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SetMaster
 * Description   : disable tx,rx and save/restore tcr4/rcr4
 * Implements    : SAI_DRV_SetMaster_Activity
 *
 *END**************************************************************************/
void SAI_DRV_SetMaster(uint32_t instNum,
                       bool tx)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pTxState = TxState[instNum];
    sai_state_t* pRxState = RxState[instNum];

    inst->TCSR &= ~SAI_TCSR_TE(1UL);
    inst->RCSR &= ~SAI_RCSR_RE(1UL);
    /* in master mode when an instance disable rx and enable tx,
     * rcr4 must be set to zero for tx to work, otherwise sync signal
     * will not be generated. Same for rx.
     */
    if (tx)
    {
        inst->TCR4 = pTxState->cr4;
        pRxState->cr4 = inst->RCR4;
        inst->RCR4 = 0UL;
        inst->TCR2 &= ~SAI_TCR2_SYNC_MASK;
        inst->TCR2 |= SAI_TCR2_SYNC(SAI_ASYNC);
        inst->RCR2 &= ~SAI_RCR2_SYNC_MASK;
        inst->RCR2 |= SAI_RCR2_SYNC(SAI_SYNC_WITH_OTHER);
        inst->TCSR &= ~SAI_TCSR_SR_MASK;  /* clear reset bit */
    }
    else
    {
        inst->RCR4 = pRxState->cr4;
        pTxState->cr4 = inst->TCR4;
        inst->TCR4 = 0UL;
        inst->TCSR |= SAI_TCSR_FR_MASK; /* reset tx fifo to avoid line is driven by tx even after disabled */
        inst->TCSR |= SAI_TCSR_SR_MASK;  /* call reset bit to avoid line is driven by tx even after disable */
        inst->RCSR |= SAI_RCSR_FR_MASK; /* reset rx fifo to clear junk data before receive */
        inst->TCR2 &= ~SAI_TCR2_SYNC_MASK;
        inst->TCR2 |= SAI_TCR2_SYNC(SAI_SYNC_WITH_OTHER);
        inst->RCR2 &= ~SAI_RCR2_SYNC_MASK;
        inst->RCR2 |= SAI_RCR2_SYNC(SAI_ASYNC);

    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_Send
 * Description   : Send block of data, return immediately
 * Implements    : SAI_DRV_Send_Activity
 *
 *END**************************************************************************/
void SAI_DRV_Send(uint32_t instNum,
                  const uint8_t* data[],
                  uint32_t count)
{
    sai_state_t* pState = TxState[instNum];
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(count > 0UL);
    DEV_ASSERT(pState->status != STATUS_BUSY);

    SAI_Type* inst = SAIBase[instNum];

    if (pState->XferType == SAI_DMA)
    {
        SAI_DRV_SendDma(instNum, data, count);
    }
    else if (pState->XferType == SAI_INTERRUPT)
    {
        SAI_DRV_SendInt(instNum, data, count);
    }
    else
    {
        DEV_ASSERT(false);
    }
    /* Enable tx */
    inst->TCSR |= SAI_TCSR_TE(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetSendingStatus
 * Description   : Get sending status (dma or interrupt)
 * Implements    : SAI_DRV_GetSendingStatus_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_GetSendingStatus(uint32_t instNum,
                                  uint32_t *countRemain)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    sai_state_t* pState = TxState[instNum];
    status_t CurStatus = pState->status;
    status_t ret = STATUS_ERROR;
    uint32_t temp;

    switch (CurStatus)
    {
        case STATUS_UNSUPPORTED:
        case STATUS_SUCCESS:
            if (countRemain != NULL)
            {
                *countRemain = 0;
            }
            ret = STATUS_SUCCESS;
            break;
        case STATUS_ERROR:
            if (countRemain != NULL)
            {
                *countRemain = 0;
            }
            ret = STATUS_ERROR;
            break;
        case STATUS_BUSY:
            if (pState->XferType == SAI_DMA)
            {
                if (countRemain != NULL)
                {
                    temp = EDMA_DRV_GetRemainingMajorIterationsCount(pState->DmaChannel[0]);
                    if (pState->mux == SAI_MUX_MEM)
                    {
                        *countRemain = temp / pState->ChannelCount;
                    }
                    else
                    {
                        *countRemain = temp;
                    }
                }
            }
            else
            {
                if (countRemain != NULL)
                {
                    if (pState->mux == SAI_MUX_LINE)
                    {
                        *countRemain = pState->ChnState[0].count / pState->ChannelCount;
                    }
                    else
                    {
                        *countRemain = pState->ChnState[0].count;
                    }
    
                }
            }
            ret = STATUS_BUSY;
            break;
        case STATUS_SAI_ABORTED:
        case STATUS_TIMEOUT:
            if (countRemain != NULL)
            {
                *countRemain = pState->ChnState[0].count;
            }
            ret = CurStatus;
            break;
        default:
            /* undefined value */
            DEV_ASSERT(false);
            break;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortSendingDma
 * Description   : Abort ongoing sending dma
 *
 *END**************************************************************************/
static void SAI_DRV_AbortSendingDma(uint32_t instNum)
{
    sai_state_t* pState = TxState[instNum];
    SAI_Type* inst = SAIBase[instNum];
    uint8_t c = 0;
    uint8_t i;

    /* Disable tx DMA requests for the current instance */
    SAI_DRV_TxDisableFifoReqDma(inst);
    pState->ChnState[0].count = EDMA_DRV_GetRemainingMajorIterationsCount(pState->DmaChannel[0]);
    /* stop all dma channel*/
    if ((pState->mux == SAI_MUX_DISABLED) || (pState->mux == SAI_MUX_LINE))
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                (void)EDMA_DRV_StopChannel(pState->DmaChannel[c]);
                c++;
            }
        }
    }
    else
    {
        (void)EDMA_DRV_StopChannel(pState->DmaChannel[0]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortSending
 * Description   : Abort ongoing sending (dma or interrupt)
 * Implements    : SAI_DRV_AbortSending_Activity
 *
 *END**************************************************************************/
void SAI_DRV_AbortSending(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    sai_state_t* pState = TxState[instNum];
    SAI_Type* inst = SAIBase[instNum];

    if (pState->status == STATUS_BUSY)
    {
        pState->status = STATUS_SAI_ABORTED;
        if (pState->XferType == SAI_DMA)
        {
            SAI_DRV_AbortSendingDma(instNum);
        }
        else
        {
            SAI_DRV_TxDisableFifoReqInt(inst);
        }
        if (pState->Blocking)
        {
            (void)OSIF_SemaPost(&pState->Sema);
        }
    }
}

/*****************************RECEIVING FUNCTIONS*********************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveBlocking
 * Description   : Receive block of data, return when finish Receiving
 * Implements    : SAI_DRV_ReceiveBlocking_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_ReceiveBlocking(uint32_t instNum,
                                 uint8_t* data[],
                                 uint32_t count,
                                 uint32_t timeout)
{
    sai_state_t* pState = RxState[instNum];
    status_t res;
    status_t osifError;

    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(pState->status != STATUS_BUSY);

    pState->Blocking = true;
    SAI_DRV_Receive (instNum, data, count);
    osifError = OSIF_SemaWait(&pState->Sema, timeout);
    pState->Blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* abort current transfer */
        SAI_DRV_AbortReceiving (instNum);
        pState->status = STATUS_TIMEOUT;
        res = STATUS_TIMEOUT;
    }
    else
    {
        /* not time out */
        res = pState->status;
    }

    return res;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_FlushRxFifo
 * Description   : Flush enabled rx fifo
 *
 *END**************************************************************************/
static void SAI_DRV_FlushRxFifo(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;

    for (i = 0; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsRxChannelEnabled(inst, i))
        {
            while (!SAI_DRV_IsRxFifoEmpty(inst, i))
            {
                (void)SAI_DRV_RxRead(inst, i);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_Receive
 * Description   : Receive block of data, return immediately
 * Implements    : SAI_DRV_Receive_Activity
 *
 *END**************************************************************************/
void SAI_DRV_Receive(uint32_t instNum,
                     uint8_t* data[],
                     uint32_t count)
{
    sai_state_t* pState = RxState[instNum];
    SAI_Type* inst = SAIBase[instNum];

    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(count > 0UL);
    DEV_ASSERT(pState->status != STATUS_BUSY);

    /* if last transfer is aborted then flush fifo all enabled channel */
    if (pState->status == STATUS_SAI_ABORTED)
    {
        SAI_DRV_FlushRxFifo(instNum);
    }
    if (pState->XferType == SAI_DMA)
    {
        SAI_DRV_ReceiveDma(instNum, data, count);
    }
    else if (pState->XferType == SAI_INTERRUPT)
    {
        SAI_DRV_ReceiveInt(instNum, data, count);
    }
    else
    {
        DEV_ASSERT(false);
    }
    inst->RCSR |= (1UL << SAI_RCSR_RE_SHIFT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetFirstEnabledRxChannel
 * Description   : Get first enabled rx channel
 *
 *END**************************************************************************/
static uint8_t SAI_DRV_GetFirstEnabledRxChannel(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;

    for (i = 0; i < sai_channel_count[instNum]; i++)
    {
        if (SAI_DRV_IsRxChannelEnabled(inst, i))
        {
            break;
        }
    }
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetBusyCount
 * Description   : Get count when busy
 *
 *END**************************************************************************/
static void SAI_DRV_GetRxBusyCount(uint32_t instNum,
                                 uint32_t *countRemain)
{
    sai_state_t* pState = RxState[instNum];
    uint32_t temp;
    uint8_t i;

    if (pState->XferType == SAI_DMA)
    {
        if (countRemain != NULL)
        {
            temp = EDMA_DRV_GetRemainingMajorIterationsCount(pState->DmaChannel[0]);
            if((pState->ChannelCount > 1U) && (pState->mux != SAI_MUX_MEM))
            {
                i = SAI_DRV_GetFirstEnabledRxChannel(instNum);
                if (pState->ChnState[i].count > 1UL)
                {
                    *countRemain = temp + 1UL;
                }
                else
                {
                    *countRemain = temp;
                }
            }
            else
            {
                if (pState->mux == SAI_MUX_MEM)
                {
                    *countRemain = temp / pState->ChannelCount;
                }
                else
                {
                    *countRemain = temp;
                }
            }
        }
    }
    else
    {
        if (countRemain != NULL)
        {
            if (pState->mux == SAI_MUX_LINE)
            {
                *countRemain = pState->ChnState[0].count / pState->ChannelCount;
            }
            else
            {
                *countRemain = pState->ChnState[0].count;
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetReceivingStatus
 * Description   : Get receiving status (dma or interrupt)
 * Implements    : SAI_DRV_GetReceivingStatus_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_GetReceivingStatus(uint32_t instNum,
                                    uint32_t *countRemain)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    sai_state_t* pState = RxState[instNum];
    status_t ret = STATUS_ERROR;
    /* temp variable to avoid status change while checking */
    status_t CurStatus = pState->status;

    switch (CurStatus)
    {
        case STATUS_UNSUPPORTED:
        case STATUS_SUCCESS:
            if (countRemain != NULL)
            {
                *countRemain = 0;
            }
            ret = STATUS_SUCCESS;
            break;
        case STATUS_ERROR:
            if (countRemain != NULL)
            {
                *countRemain = 0;
            }
            ret = STATUS_ERROR;
            break;
        case STATUS_BUSY:
            SAI_DRV_GetRxBusyCount(instNum, countRemain);
            ret = STATUS_BUSY;
            break;
        case STATUS_SAI_ABORTED:
        case STATUS_TIMEOUT:
            if (countRemain != NULL)
            {
                *countRemain = pState->ChnState[0].count;
            }
            ret = CurStatus;
            break;
        default:
            /* undefined value */
            DEV_ASSERT(false);
            break;
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortReceivingDma
 * Description   : Abort ongoing receiving dma
 *
 *END**************************************************************************/
static void SAI_DRV_AbortReceivingDma(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];
    uint8_t c = 0;
    uint8_t i;
    uint32_t temp;

    /* Disable Rx DMA requests for the current instance */
    SAI_DRV_RxDisableFifoReqDma(inst);
    (void)SAI_DRV_GetReceivingStatus(instNum, &temp);
    pState->ChnState[0].count = temp;
    /* stop all dma channel*/
    if ((pState->mux == SAI_MUX_DISABLED) || (pState->mux == SAI_MUX_LINE))
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                (void)EDMA_DRV_StopChannel(pState->DmaChannel[c]);
                c++;
            }
        }
    }
    else
    {
        (void)EDMA_DRV_StopChannel(pState->DmaChannel[0]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortReceiving
 * Description   : Abort ongoing receiving (dma or interrupt)
 * Implements    : SAI_DRV_AbortReceiving_Activity
 *
 *END**************************************************************************/
void SAI_DRV_AbortReceiving(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    sai_state_t* pState = RxState[instNum];

    if (pState->status == STATUS_BUSY)
    {
        pState->status = STATUS_SAI_ABORTED;
        if (pState->XferType == SAI_DMA)
        {
            SAI_DRV_AbortReceivingDma(instNum);
        }
        else
        {
            SAI_DRV_RxDisableFifoReqInt(inst);
        }
        if (pState->Blocking)
        {
            (void)OSIF_SemaPost(&pState->Sema);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetDefaultConfig
 * Description   : Init config structure for I2S interface, interrupt,
 *                 internal generated bit clock, 44.1 kHz sample rate, 16 bit word width,
 *                 2 channel 1 data line (data line 0)
 * Implements    : SAI_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
void SAI_DRV_GetDefaultConfig(sai_user_config_t* uc)
{
    uc->BitClkInternal = true;
    uc->BitClkNegPolar = true;
    uc->BitClkFreq = 1411200U;
    uc->ChannelCount = 2U;              /* 2 buffer, one for each channel */
    uc->ChannelEnable = SAI_CHANNEL_0;  /* select data line 0 */
    uc->ElementSize = 2U;               /* 2 byte each read/write */
    uc->FirstBitIndex = 15U;            /* first bit to transfer is 15 because of msb first */
    uc->FrameSize = 2U;                 /* 2 word per frame */
    uc->FrameStartReport = false;
#ifdef FEATURE_SAI_MSEL_FCD
    uc->MasterClkSrc = SAI_FCD_CLK;
#else
    uc->MasterClkSrc = SAI_BUS_CLK;
#endif
#ifdef FEATURE_SAI_HAS_CHMOD
    uc->MaskMode = SAI_MASK_TRISTATE;
#endif
    uc->MsbFirst = true;
    uc->MuxMode = SAI_MUX_LINE;         /* 2 data buffer is muxed in to one data line */
    uc->RunErrorReport = false;
    uc->SyncEarly = true;
    uc->SyncErrorReport = false;
    uc->SyncMode = SAI_ASYNC;
    uc->SyncNegPolar = true;
    uc->SyncInternal = true;
    uc->SyncWidth = 16U;                /* sync width is first word */
    uc->TransferType = SAI_INTERRUPT;
    uc->Word0Width = 16U;
    uc->WordNWidth = 16U;
    uc->callback = NULL;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
