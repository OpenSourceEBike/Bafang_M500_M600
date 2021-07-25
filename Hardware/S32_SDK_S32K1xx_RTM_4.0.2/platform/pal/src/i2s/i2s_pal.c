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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * Local variable is required to use some API function.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, cast performed between a pointer to object
 * type and a pointer to a different object type.
 * Simplify code and increase performance
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.1, conversion between a pointer to function
 * and another type.
 * Necessary since user config structure's function pointer is used for both flexio and sai driver
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to
 * pointer to other type.
 * Simplify code and increase performance
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, could define variable
 * at block scope.
 * Those variables are state structures, required to be global.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, external function could be
 * made static
 * Those functions is API for user.
 */
#include "i2s_pal.h"

/* Include PD files */
#if (defined(I2S_OVER_SAI))
    #include "sai_driver.h"
#endif
#if (defined(I2S_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_i2s_driver.h"
#endif

/* Define state structures for SAI */
#if (defined(I2S_OVER_SAI))
    #define LAST_IS_TX 1U
    #define LAST_IS_RX 2U
    #define LAST_IS_NONE 0U
    /* sai state structures */
    static sai_state_t saiTxState[NO_OF_SAI_INSTS_FOR_I2S];
    static sai_state_t saiRxState[NO_OF_SAI_INSTS_FOR_I2S];
    /* sai state-instance matching */
    static uint32_t saiStateInstanceMapping[NO_OF_SAI_INSTS_FOR_I2S];
    /* sai available resources table */
    static bool saiStateIsAllocated[NO_OF_SAI_INSTS_FOR_I2S];
    /* record last transfer is tx or rx to call SelectMaster for sai */
    static uint8_t lastXfer[NO_OF_SAI_INSTS_FOR_I2S];
#endif

/* Define state structures for i2s over FLEXIO */
#if (defined(I2S_OVER_FLEXIO))
    #define NO_OF_FLEXIO_INSTS_FOR_I2S (NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S+NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S)
    /* FLEXIO state structures */
#if (NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S > 0U)
    static flexio_i2s_master_state_t flexioMasterState[NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S];
    static bool flexioMasterStateIsAllocated[NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S];
#endif
#if (NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S > 0U)
    static flexio_i2s_slave_state_t flexioSlaveState[NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S];
    static bool flexioSlaveStateIsAllocated[NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S];
#endif
    /* flexio device state */
    static flexio_device_state_t flexioDeviceState;
    /* state-instance matching table */
    static uint32_t flexioStateInstanceMapping[NO_OF_FLEXIO_INSTS_FOR_I2S];
    /* state is master or slave */
    static bool flexioIsMaster[NO_OF_FLEXIO_INSTS_FOR_I2S];
    /* pointer to master/slave state */
    static void* flexioState[NO_OF_FLEXIO_INSTS_FOR_I2S];
    /* available resources table */
    static bool flexioStateIsAllocated[NO_OF_FLEXIO_INSTS_FOR_I2S];
    static uint8_t flexioWordSize[NO_OF_FLEXIO_INSTS_FOR_I2S];
#endif

#ifdef I2S_OVER_FLEXIO
/*FUNCTION**********************************************************************
 *
 * Function Name : flexioAssignState
 * Description   : Assign master or slave state
 *
 *END**************************************************************************/
static void flexioAssignState(bool isMaster,
                              uint8_t ind)
{
    uint8_t j;

    flexioStateIsAllocated[ind] = true;
    flexioIsMaster[ind] = isMaster;
    if (isMaster)
    {
#if (NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S > 0U)
        for (j = 0; j < NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S; j++)
        {
            if (!flexioMasterStateIsAllocated[j])
            {
                flexioMasterStateIsAllocated[j] = true;
                flexioState[ind] = &flexioMasterState[j];
                break;
            }
        }
#endif
    }
    else
    {
#if (NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S > 0U)
        for (j = 0; j < NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S; j++)
        {
            if (!flexioSlaveStateIsAllocated[j])
            {
                flexioSlaveStateIsAllocated[j] = true;
                flexioState[ind] = &flexioSlaveState[j];
                break;
            }
        }
#endif
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : flexioAllocateState
 * Description   : Allocates one of the available state structures.
 *
 *END**************************************************************************/
static uint8_t flexioAllocateState(uint32_t instId,
                                   bool isMaster)
{
    uint8_t i;

    /* Allocate one of the i2s state structures for this instance */
    for (i = 0; i < NO_OF_FLEXIO_INSTS_FOR_I2S; i++)
    {
        if (!flexioStateIsAllocated[i])
        {
            flexioStateInstanceMapping[i] = instId;
            flexioAssignState(isMaster, i);
            break;
        }
    }
    return i;
}
#endif /* I2S_OVER_FLEXIO */

#ifdef I2S_OVER_SAI
/*FUNCTION**********************************************************************
 *
 * Function Name : saiAllocateState
 * Description   : Allocates one of the available state structures.
 *
 *END**************************************************************************/
static uint8_t saiAllocateState(uint32_t instId)
{
    uint8_t i;
    for (i = 0; i < NO_OF_SAI_INSTS_FOR_I2S; i++)
    {
        if (!saiStateIsAllocated[i])
        {
            saiStateIsAllocated[i] = true;
            saiStateInstanceMapping[i] = instId;
            break;
        }
    }
    return i;
}
#endif /* I2S_OVER_SAI */

#ifdef I2S_OVER_FLEXIO
/*FUNCTION**********************************************************************
 *
 * Function Name : freeState
 * Description   : free allocated state
 *
 *END**************************************************************************/
static void freeFlexioState(uint8_t ind)
{
    uint8_t j;

    flexioStateIsAllocated[ind] = false;
    if (flexioIsMaster[ind])
    {
#if (NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S > 0U)
        for (j = 0; j < NO_OF_FLEXIO_MASTER_INSTS_FOR_I2S; j++)
        {
            if ((&flexioMasterState[j] == flexioState[ind]) && flexioMasterStateIsAllocated[j])
            {
                flexioMasterStateIsAllocated[j] = false;
            }
        }
#endif
    }
    else
    {
#if (NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S > 0U)
        for (j = 0; j < NO_OF_FLEXIO_SLAVE_INSTS_FOR_I2S; j++)
        {
            if ((&flexioSlaveState[j] == flexioState[ind]) && flexioSlaveStateIsAllocated[j])
            {
                flexioSlaveStateIsAllocated[j] = false;
            }
        }
#endif
    }
}
#endif /* I2S_OVER_FLEXIO */

/*FUNCTION**********************************************************************
 *
 * Function Name : freeState
 * Description   : free allocated state
 *
 *END**************************************************************************/
static void freeState(const i2s_instance_t* instance)
{
    uint8_t i;
#ifdef I2S_OVER_SAI
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        for (i = 0; i < NO_OF_SAI_INSTS_FOR_I2S; i++)
        {
            if ((saiStateInstanceMapping[i] == instance->instIdx) && saiStateIsAllocated[i])
            {
                saiStateIsAllocated[i] = false;
                break;
            }
        }
    }
#endif /* I2S_OVER_SAI */
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        for (i = 0; i < NO_OF_FLEXIO_INSTS_FOR_I2S; i++)
        {
            if ((flexioStateInstanceMapping[i] == instance->instIdx) && flexioStateIsAllocated[i])
            {
                freeFlexioState(i);
                break;
            }
        }
    }
#endif /* I2S_OVER_FLEXIO */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : findState
 * Description   : find state index from instance
 *
 *END**************************************************************************/
static uint8_t findState(uint32_t instance, const uint32_t* instMap, const bool* isAlloc, uint32_t noOfInst)
{
    uint8_t i;
    for (i = 0; i < noOfInst; i++)
    {
        if ((instMap[i] == instance) && isAlloc[i])
        {
            break;
        }
    }
    /* cannot find state */
    DEV_ASSERT(i < noOfInst);
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_Init
 * Description   : initialize driver
 *
 * Implements    : I2S_Init_Activity
 *END**************************************************************************/
status_t I2S_Init(const i2s_instance_t* instance, const i2s_user_config_t * config)
{
    uint8_t stateIndex;
    status_t ret = STATUS_UNSUPPORTED;
#ifdef I2S_OVER_SAI
    sai_user_config_t saiUserConfig;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = saiAllocateState(instance->instIdx);
        DEV_ASSERT(stateIndex < NO_OF_SAI_INSTS_FOR_I2S);
        lastXfer[stateIndex] = LAST_IS_NONE;
        saiUserConfig.BitClkNegPolar = true;
        saiUserConfig.ChannelCount = 1U;
        if (config->wordWidth <= 8U)
        {
            saiUserConfig.ElementSize = 1U;
        }
        else if (config->wordWidth <= 16U)
        {
            saiUserConfig.ElementSize = 2U;
        }
        else
        {
            saiUserConfig.ElementSize = 4U;
        }
        saiUserConfig.FirstBitIndex = (uint8_t)(config->wordWidth - 1U);
        saiUserConfig.FrameSize = 2U;
#ifdef FEATURE_SAI_HAS_CHMOD
        saiUserConfig.MaskMode = SAI_MASK_TRISTATE;
#endif /* FEATURE_SAI_HAS_CHMOD */

#if (defined(FEATURE_SAI_MSEL_FCD))
        saiUserConfig.MasterClkSrc = SAI_FCD_CLK;
#elif (defined(FEATURE_SAI_MSEL_BUS_CLK))
        saiUserConfig.MasterClkSrc = SAI_BUS_CLK;
#endif /* FEATURE_SAI_MSEL_BUS_CLK */
        saiUserConfig.MsbFirst = true;
        saiUserConfig.MuxMode = SAI_MUX_DISABLED;
        saiUserConfig.SyncEarly = true;
        saiUserConfig.SyncNegPolar = true;
        saiUserConfig.SyncWidth = config->wordWidth;
        saiUserConfig.Word0Width = config->wordWidth;
        saiUserConfig.WordNWidth = config->wordWidth;
        saiUserConfig.FrameStartReport = false;
        saiUserConfig.SyncErrorReport = false;
        saiUserConfig.RunErrorReport = false;
        if (config->transferType == I2S_USING_DMA)
        {
            saiUserConfig.TransferType = SAI_DMA;
        }
        else
        {
            saiUserConfig.TransferType = SAI_INTERRUPT;
        }
        saiUserConfig.callback = (sai_transfer_callback_t) config->callback;
        saiUserConfig.callbackParam = config->callbackParam;
        saiUserConfig.BitClkFreq = config->baudRate;
        saiUserConfig.SyncMode = SAI_ASYNC;
        /* saiUserConfig.BitClkDiv */
        saiUserConfig.DmaChannel[0] = config->txDMAChannel;
        saiUserConfig.ChannelEnable = 1U;
        if (config->mode == I2S_MASTER)
        {
#ifdef FEATURE_SAI_MSEL_FCD
            SAI_DRV_FCDInit(instance->instIdx, SAI_FCD_PLL, config->baudRate*2UL, false);
#endif /* FEATURE_SAI_MSEL_FCD */
            saiUserConfig.BitClkInternal = true;
            saiUserConfig.SyncInternal = true;
        }
        else
        {
            saiUserConfig.BitClkInternal = false;
            saiUserConfig.SyncInternal = false;
        }
        SAI_DRV_TxInit(instance->instIdx, &saiUserConfig, &saiTxState[stateIndex]);
        saiUserConfig.DmaChannel[0] = config->rxDMAChannel;
        if (instance->instIdx == 0UL)
        {
            saiUserConfig.ChannelEnable = 2U;
        }
        SAI_DRV_RxInit(instance->instIdx, &saiUserConfig, &saiRxState[stateIndex]);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
#ifdef I2S_OVER_FLEXIO
    flexio_i2s_master_user_config_t flexioMasterConfig;
    flexio_i2s_slave_user_config_t flexioSlaveConfig;
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = flexioAllocateState(instance->instIdx, ((config->mode == I2S_MASTER) ? true : false));
        DEV_ASSERT(stateIndex < NO_OF_FLEXIO_INSTS_FOR_I2S);
        ret = FLEXIO_DRV_InitDevice(0UL, &flexioDeviceState);
        if (config->wordWidth <= 8U)
        {
            flexioWordSize[stateIndex] = 1U;
        }
        else if (config->wordWidth <= 16U)
        {
            flexioWordSize[stateIndex] = 2U;
        }
        else
        {
            flexioWordSize[stateIndex] = 4U;
        }
        if (config->mode == I2S_MASTER)
        {
            flexioMasterConfig.baudRate = config->baudRate;
            flexioMasterConfig.bitsWidth = config->wordWidth;
            flexioMasterConfig.callback = config->callback;
            flexioMasterConfig.callbackParam = config->callbackParam;
            if (config->transferType == I2S_USING_DMA)
            {
                flexioMasterConfig.driverType = FLEXIO_DRIVER_TYPE_DMA;
            }
            if (config->transferType == I2S_USING_INTERRUPT)
            {
                flexioMasterConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
            }
            flexioMasterConfig.rxDMAChannel = config->rxDMAChannel;
            flexioMasterConfig.txDMAChannel = config->txDMAChannel;
            flexioMasterConfig.rxPin = ((extension_flexio_for_i2s_t*)config->extension)->rxPin;
            flexioMasterConfig.sckPin = ((extension_flexio_for_i2s_t*)config->extension)->sckPin;
            flexioMasterConfig.txPin = ((extension_flexio_for_i2s_t*)config->extension)->txPin;
            flexioMasterConfig.wsPin = ((extension_flexio_for_i2s_t*)config->extension)->wsPin;
            ret = FLEXIO_I2S_DRV_MasterInit(0, &flexioMasterConfig, flexioState[stateIndex]);
        }
        else
        {
            flexioSlaveConfig.bitsWidth = config->wordWidth;
            flexioSlaveConfig.callback = config->callback;
            flexioSlaveConfig.callbackParam = config->callbackParam;
            if (config->transferType == I2S_USING_DMA)
            {
                flexioSlaveConfig.driverType = FLEXIO_DRIVER_TYPE_DMA;
            }
            if (config->transferType == I2S_USING_INTERRUPT)
            {
                flexioSlaveConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
            }
            flexioSlaveConfig.rxDMAChannel = config->rxDMAChannel;
            flexioSlaveConfig.txDMAChannel = config->txDMAChannel;
            flexioSlaveConfig.rxPin = ((extension_flexio_for_i2s_t*)config->extension)->rxPin;
            flexioSlaveConfig.sckPin = ((extension_flexio_for_i2s_t*)config->extension)->sckPin;
            flexioSlaveConfig.txPin = ((extension_flexio_for_i2s_t*)config->extension)->txPin;
            flexioSlaveConfig.wsPin = ((extension_flexio_for_i2s_t*)config->extension)->wsPin;
            ret = FLEXIO_I2S_DRV_SlaveInit(0, &flexioSlaveConfig, flexioState[stateIndex]);
        }
    }
#endif /* I2S_OVER_FLEXIO */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_Deinit
 * Description   : deinit driver
 *
 * Implements    : I2S_Deinit_Activity
 *END**************************************************************************/
status_t I2S_Deinit(const i2s_instance_t* instance)
{
    status_t ret = STATUS_UNSUPPORTED;
#ifdef I2S_OVER_FLEXIO
    uint8_t stateIndex;
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret = FLEXIO_I2S_DRV_MasterDeinit(flexioState[stateIndex]);
        }
        else
        {
            ret = FLEXIO_I2S_DRV_SlaveDeinit(flexioState[stateIndex]);
        }
        if (ret == STATUS_SUCCESS)
        {
            freeState(instance);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        SAI_DRV_TxDeinit(instance->instIdx);
        SAI_DRV_RxDeinit(instance->instIdx);
        freeState(instance);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_GetBaudRate
 * Description   : return true baud rate
 *
 * Implements    : I2S_GetBaudRate_Activity
 *END**************************************************************************/
status_t I2S_GetBaudRate(const i2s_instance_t* instance,
                         uint32_t * configuredBaudRate)
{
    status_t ret = STATUS_UNSUPPORTED;
#ifdef I2S_OVER_FLEXIO
    uint8_t stateIndex;
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret = FLEXIO_I2S_DRV_MasterGetBaudRate(flexioState[stateIndex], configuredBaudRate);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        *configuredBaudRate = SAI_DRV_TxGetBitClockFreq(instance->instIdx);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_SendDataBlocking
 * Description   : Send blocking
 *
 * Implements    : I2S_SendDataBlocking_Activity
 *END**************************************************************************/
status_t I2S_SendDataBlocking(
        const i2s_instance_t* instance,
        const uint8_t * txBuff,
        uint32_t txSize,
        uint32_t timeout)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterSendDataBlocking(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize, timeout);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveSendDataBlocking(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize, timeout);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    const uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if ((lastXfer[stateIndex] == LAST_IS_NONE) || (lastXfer[stateIndex] == LAST_IS_RX))
        {
            lastXfer[stateIndex] = LAST_IS_TX;
            SAI_DRV_SetMaster(instance->instIdx, true);
        }
        addr = txBuff;
        ret = SAI_DRV_SendBlocking(instance->instIdx, &addr, txSize, timeout);
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_SetRxBuffer
 * Description   : Keep receiving data
 *
 * Implements    : I2S_SetRxBuffer_Activity
 *END**************************************************************************/
status_t I2S_SetRxBuffer(const i2s_instance_t* instance,
                      uint8_t * rxBuff,
                      uint32_t rxSize)
{
    status_t ret = STATUS_UNSUPPORTED;
#ifdef I2S_OVER_FLEXIO
    uint8_t stateIndex;
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterSetRxBuffer(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveSetRxBuffer(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        addr = rxBuff;
        SAI_DRV_Receive(instance->instIdx, &addr, rxSize);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_SetTxBuffer
 * Description   : Keep sending data
 *
 * Implements    : I2S_SetTxBuffer_Activity
 *END**************************************************************************/
status_t I2S_SetTxBuffer(const i2s_instance_t* instance,
                      const uint8_t * txBuff,
                      uint32_t txSize)
{
    status_t ret = STATUS_UNSUPPORTED;
#ifdef I2S_OVER_FLEXIO
    uint8_t stateIndex;
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterSetTxBuffer(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveSetTxBuffer(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    const uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        addr = txBuff;
        SAI_DRV_Send(instance->instIdx, &addr, txSize);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
    return ret;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_SendData
 * Description   : Send non-blocking
 *
 * Implements    : I2S_SendData_Activity
 *END**************************************************************************/
status_t I2S_SendData(const i2s_instance_t* instance,
                      const uint8_t * txBuff,
                      uint32_t txSize)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterSendData(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveSendData(flexioState[stateIndex], txBuff, flexioWordSize[stateIndex] * txSize);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    const uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if ((lastXfer[stateIndex] == LAST_IS_NONE) || (lastXfer[stateIndex] == LAST_IS_RX))
        {
            lastXfer[stateIndex] = LAST_IS_TX;
            SAI_DRV_SetMaster(instance->instIdx, true);
        }
        addr = txBuff;
        SAI_DRV_Send(instance->instIdx, &addr, txSize);
        ret = STATUS_SUCCESS;
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_Abort
 * Description   : Abort ongoing transfer
 *
 * Implements    : I2S_Abort_Activity
 *END**************************************************************************/
status_t I2S_Abort(const i2s_instance_t* instance)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret = FLEXIO_I2S_DRV_MasterTransferAbort((flexio_i2s_master_state_t*)flexioState[stateIndex]);
        }
        else
        {
            ret = FLEXIO_I2S_DRV_SlaveTransferAbort((flexio_i2s_slave_state_t*)flexioState[stateIndex]);
        }
    }
    else
    {
        /* avoid misra */
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if (lastXfer[stateIndex] == LAST_IS_TX)
        {
            SAI_DRV_AbortSending(instance->instIdx);
        }
        else if (lastXfer[stateIndex] == LAST_IS_RX)
        {
            SAI_DRV_AbortReceiving(instance->instIdx);
        }
        else
        {
            /* avoid misra */
        }
        ret = STATUS_SUCCESS;
    }
    else
    {
        /* avoid misra */
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_GetStatus
 * Description   : get status of driver
 *
 * Implements    : I2S_GetStatus_Activity
 *END**************************************************************************/
status_t I2S_GetStatus(const i2s_instance_t* instance,
                       uint32_t * countRemaining)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterGetStatus(flexioState[stateIndex], countRemaining);
            if (countRemaining != NULL)
            {
                *countRemaining /= flexioWordSize[stateIndex];
            }
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveGetStatus(flexioState[stateIndex], countRemaining);
            if (countRemaining != NULL)
            {
                *countRemaining /= flexioWordSize[stateIndex];
            }
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if (lastXfer[stateIndex] == LAST_IS_TX)
        {
            ret = SAI_DRV_GetSendingStatus(instance->instIdx, countRemaining);
        }
        else if (lastXfer[stateIndex] == LAST_IS_RX)
        {
            ret = SAI_DRV_GetReceivingStatus(instance->instIdx, countRemaining);
        }
        else
        {
            /* avoid misra */
        }
    }
    else
    {
        /* avoid misra */
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_ReceiveDataBlocking
 * Description   : receive blocking
 *
 * Implements    : I2S_ReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t I2S_ReceiveDataBlocking(
        const i2s_instance_t* instance,
        uint8_t * rxBuff,
        uint32_t rxSize,
        uint32_t timeout)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterReceiveDataBlocking(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize, timeout);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveReceiveDataBlocking(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize, timeout);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if ((lastXfer[stateIndex] == LAST_IS_NONE) || (lastXfer[stateIndex] == LAST_IS_TX))
        {
            lastXfer[stateIndex] = LAST_IS_RX;
            SAI_DRV_SetMaster(instance->instIdx, false);
        }
        addr = rxBuff;
        ret = SAI_DRV_ReceiveBlocking(instance->instIdx, &addr, rxSize, timeout);
    }
    else
    {
        /* avoid misra */
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_ReceiveData
 * Description   : receive non blocking
 *
 * Implements    : I2S_ReceiveData_Activity
 *END**************************************************************************/
status_t I2S_ReceiveData(const i2s_instance_t* instance,
                         uint8_t * rxBuff,
                         uint32_t rxSize)
{
    status_t ret = STATUS_UNSUPPORTED;
    uint8_t stateIndex;
#ifdef I2S_OVER_FLEXIO
    if (instance->instType == I2S_INST_TYPE_FLEXIO)
    {
        stateIndex = findState(instance->instIdx, flexioStateInstanceMapping, flexioStateIsAllocated, NO_OF_FLEXIO_INSTS_FOR_I2S);
        if (flexioIsMaster[stateIndex])
        {
            ret =  FLEXIO_I2S_DRV_MasterReceiveData(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize);
        }
        else
        {
            ret =  FLEXIO_I2S_DRV_SlaveReceiveData(flexioState[stateIndex], rxBuff, flexioWordSize[stateIndex] * rxSize);
        }
    }
#endif /* I2S_OVER_FLEXIO */
#ifdef I2S_OVER_SAI
    uint8_t* addr;
    if (instance->instType == I2S_INST_TYPE_SAI)
    {
        stateIndex = findState(instance->instIdx, saiStateInstanceMapping, saiStateIsAllocated, NO_OF_SAI_INSTS_FOR_I2S);
        if ((lastXfer[stateIndex] == LAST_IS_NONE) || (lastXfer[stateIndex] == LAST_IS_TX))
        {
            lastXfer[stateIndex] = LAST_IS_RX;
            SAI_DRV_SetMaster(instance->instIdx, false);
        }
        addr = rxBuff;
        SAI_DRV_Receive(instance->instIdx, &addr, rxSize);
        ret = STATUS_SUCCESS;
    }
    else
    {
        /* avoid misra */
    }
#endif /* I2S_OVER_SAI */
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2S_GetDefaultConfig
 * Description   : Return default config
 *
 * Implements    : I2S_GetDefaultConfig_Activity
 *END**************************************************************************/
void I2S_GetDefaultConfig(i2s_user_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->baudRate     = 256000U;
    config->mode         = I2S_MASTER;
    config->transferType = I2S_USING_INTERRUPT;
    config->wordWidth    = 16;
    config->rxDMAChannel = 0U;
    config->txDMAChannel = 0U;
    config->callback     = NULL;
    config->extension    = NULL;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
