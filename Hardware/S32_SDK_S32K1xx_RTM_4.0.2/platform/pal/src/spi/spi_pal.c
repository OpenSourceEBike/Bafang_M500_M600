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
 * @spi_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type
 * This is needed for the conversion between generic SPI_PAL data types to other drivers data types.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast;
 * This is needed for the conversion between generic SPI_PAL data types to other drivers data types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 * This is needed for using this driver for various buffer data type (uint8_t, uint16_t, uint32_t).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 * This is needed for using this driver for various buffer data type (uint8_t, uint16_t, uint32_t).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code
 * structure and better readability.
 *
 */
 
#include "spi_pal.h"
#include "device_registers.h"

/* Include PD files */
#if (defined (SPI_OVER_LPSPI))
    #include "lpspi_master_driver.h"
    #include "lpspi_slave_driver.h"
#endif

#if (defined (SPI_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_spi_driver.h"
#endif

#if (defined (SPI_OVER_DSPI))
    #include "dspi_driver.h"
#endif


/* Define state structures for LPSPI */
#if (defined(SPI_OVER_LPSPI))
    /*! @brief LPSPI state structures */
    static lpspi_state_t LpspiState[NO_OF_LPSPI_INSTS_FOR_SPI];
    /*! @brief LPSPI state-instance matching */
    static uint32_t LpspiStateInstanceMapping[NO_OF_LPSPI_INSTS_FOR_SPI];
    /*! @brief LPSPI  available resources table */
    static bool LpspiStateIsAllocated[NO_OF_LPSPI_INSTS_FOR_SPI];
#endif

/* Define state structures for SPI over FLEXIO */
#if (defined(SPI_OVER_FLEXIO))
    /*! @brief FLEXIO state structures */
    static flexio_spi_master_state_t FlexioSpiState[NO_OF_FLEXIO_INSTS_FOR_SPI];
    static flexio_device_state_t FlexioState;
    /*! @brief FLEXIO state-instance matching */
    static uint32_t FlexioSpiStateInstanceMapping[NO_OF_FLEXIO_INSTS_FOR_SPI];
    /*! @brief FLEXIO  available resources table */
    static bool FlexioSpiStateIsAllocated[NO_OF_FLEXIO_INSTS_FOR_SPI];
#endif

/* Define state structures for DSPI */
#if (defined(SPI_OVER_DSPI))
    /*! @brief DSPI state structures */
    static dspi_state_t dspiState[NO_OF_DSPI_INSTS_FOR_SPI];
    /*! @brief DSPI state-instance matching */
    static uint32_t DspiStateInstanceMapping[NO_OF_DSPI_INSTS_FOR_SPI];
    /*! @brief DSPI  available resources table */
    static bool DspiStateIsAllocated[NO_OF_DSPI_INSTS_FOR_SPI];
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : SpiAllocateState
 * Description   : Allocates one of the available state structure.
 *
 *END**************************************************************************/
static uint8_t SpiAllocateState(bool* isAllocated, uint32_t* instanceMapping, uint32_t instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the LPSPI state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance;
            isAllocated[i] = true;
            break;
        }
    }
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SpiFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void SpiFreeState(bool* isAllocated, const uint32_t* instanceMapping, uint32_t instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the LPSPI state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (instanceMapping[i] == instance)
        {
            isAllocated[i] = false;
            break;
        }
    }
}

#if (defined(SPI_OVER_LPSPI))
/*FUNCTION**********************************************************************
 *
 * Function Name : FindLpspiState
 * Description   : Search the state structure of the lpspi instance
 *
 *END**************************************************************************/
static uint8_t FindLpspiState(const spi_instance_t * const instance)
{
    uint8_t i = 0;
    for (i = 0; i < NO_OF_LPSPI_INSTS_FOR_SPI; i++)
    {
        if (LpspiStateIsAllocated[i] && (LpspiStateInstanceMapping[i] == instance->instIdx))
        {
            break;
        }
    }
    
    return i;
}
#endif



#if (defined(SPI_OVER_FLEXIO))
/*FUNCTION**********************************************************************
 *
 * Function Name : FindFlexioState
 * Description   : Search the state structure of the flexio instance
 *
 *END**************************************************************************/
static uint8_t FindFlexioState(const spi_instance_t * const instance)
{
    uint8_t i;
    for (i = 0;i<NO_OF_FLEXIO_INSTS_FOR_SPI;i++)
    {
        if (FlexioSpiStateIsAllocated[i] && (FlexioSpiStateInstanceMapping[i] == instance->instIdx))
        {
            break;
        }
    }
    return i;
}
#endif

#if (defined(SPI_OVER_DSPI))
/*FUNCTION**********************************************************************
 *
 * Function Name : GetDspiIndex
 * Description   : Get DSPI instances.
 *
 *END**************************************************************************/
static uint32_t GetDspiIndex(const spi_instance_t * const instance)
{
    uint32_t inst;
    if (instance->instType == SPI_INST_TYPE_DSPI)
    {
        inst = instance->instIdx;
    }
    else
    {
        inst = instance->instIdx + DSPI_INSTANCE_COUNT;
    }
    return inst;
}
#endif

#if (defined(SPI_OVER_LPSPI))
/*FUNCTION**********************************************************************
 *
 * Function Name : SmallestPowerOfTwo
 * Description   : Calculate the smallest power of 2 greater than or equal to
 *                 the given number 
 *
 *END**************************************************************************/
static uint16_t SmallestPowerOfTwo(uint16_t number)
{
    uint16_t helper;

    helper = number - 1u;
    helper |= helper >> 1u;
    helper |= helper >> 2u;
    helper |= helper >> 4u;
    helper |= helper >> 8u;
    helper += 1u;
    helper /= 8u;

    return helper;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterInit
 * Description   : Configures the SPI in master mode
 * Implements    : SPI_MasterInit_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterInit(const spi_instance_t * const instance, const spi_master_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define SPI PAL over LPSPI */
    #if (defined (SPI_OVER_LPSPI))
    /*! @brief Clock names for LPSPI */
    const clock_names_t g_lpspiClock[LPSPI_INSTANCE_COUNT] = FEATURE_LPSPI_CLOCKS_NAMES
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        lpspi_master_config_t lpspiConfig;
        lpspiConfig.bitsPerSec = config->baudRate;
        lpspiConfig.whichPcs = (lpspi_which_pcs_t)config->ssPin;
        lpspiConfig.pcsPolarity = (lpspi_signal_polarity_t)(!(bool)(config->ssPolarity));
        lpspiConfig.bitcount = config->frameSize;
        (void)CLOCK_SYS_GetFreq(g_lpspiClock[(uint32_t)instance->instIdx] ,&lpspiConfig.lpspiSrcClk);
        lpspiConfig.clkPhase = (lpspi_clock_phase_t)config->clockPhase;
        lpspiConfig.clkPolarity = (lpspi_sck_polarity_t)config->clockPolarity;
        lpspiConfig.lsbFirst = config->bitOrder;
        lpspiConfig.transferType = (lpspi_transfer_type)config->transferType;
        lpspiConfig.rxDMAChannel = config->rxDMAChannel;
        lpspiConfig.txDMAChannel = config->txDMAChannel;
        lpspiConfig.callback = config->callback;
        lpspiConfig.callbackParam = config->callbackParam;
        lpspiConfig.isPcsContinuous = false;
        /* Allocate one of the LPSPI state structure for this instance */
        index = SpiAllocateState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance->instIdx, NO_OF_LPSPI_INSTS_FOR_SPI);
        status = LPSPI_DRV_MasterInit(instance->instIdx, (lpspi_state_t*)(&LpspiState[index]), &lpspiConfig);
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if (defined (SPI_OVER_FLEXIO))
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        /* FlexIO driver can be used only for the following frame sizes: 8,16 or 32 */
        DEV_ASSERT((config->frameSize == (uint32_t)8) || (config->frameSize == (uint32_t)16) || (config->frameSize == (uint32_t)32));
        /* FlexIO driver can be used only when SS is active low */
        DEV_ASSERT((config->ssPolarity) == SPI_ACTIVE_LOW);
        flexio_spi_master_user_config_t flexioConfig;
        flexioConfig.baudRate = config->baudRate;
        switch(config->transferType)
        {
            case SPI_USING_DMA:  flexioConfig.driverType = FLEXIO_DRIVER_TYPE_DMA; break;
            case SPI_USING_INTERRUPTS : flexioConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS; break;
            default : /*Nothing to do */ break;
        }
        flexioConfig.bitOrder = (flexio_spi_transfer_bit_order_t)config->bitOrder;
        switch (config->frameSize)
        {
            case 8U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_1BYTE; break; 
            case 16U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_2BYTE; break;
            case 32U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_4BYTE; break;
            default : /* Nothing to do */ break;
        }
        flexioConfig.clockPolarity = (uint8_t)config->clockPolarity;
        flexioConfig.clockPhase = (uint8_t)config->clockPhase;
        flexioConfig.mosiPin = ((extension_flexio_for_spi_t*)(config->extension))->mosiPin;
        flexioConfig.misoPin = ((extension_flexio_for_spi_t*)(config->extension))->misoPin;
        flexioConfig.sckPin = ((extension_flexio_for_spi_t*)(config->extension))->sckPin;
        flexioConfig.ssPin = ((extension_flexio_for_spi_t*)(config->extension))->ssPin;
        /* Link Flexio Callbacks to the callbacks defined in PAL */
        flexioConfig.callback = config->callback;
        /* Store the callbacks defined by the user in PAL internal callbacks vector */
        flexioConfig.callbackParam = config->callbackParam;
        flexioConfig.rxDMAChannel = config->rxDMAChannel;
        flexioConfig.txDMAChannel = config->txDMAChannel;
        status = FLEXIO_DRV_InitDevice(0U, &FlexioState);
        if (status == STATUS_SUCCESS)
        {
            /* Allocate one of the Flexio state structure for this instance */
            index = SpiAllocateState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance->instIdx, NO_OF_FLEXIO_INSTS_FOR_SPI);
            status = FLEXIO_SPI_DRV_MasterInit(0U, &flexioConfig, (flexio_spi_master_state_t*)&FlexioSpiState[index]);
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {
        uint32_t inst = GetDspiIndex(instance);
        
        dspi_master_config_t dspiConfig;
        dspiConfig.bitsPerSec = config->baudRate;
        dspiConfig.bitcount = config->frameSize;
        dspiConfig.clkPhase = (dspi_clock_phase_t)config->clockPhase;
        dspiConfig.clkPolarity = (dspi_polarity_t)config->clockPolarity;
        dspiConfig.isClkContinuous = false;
        dspiConfig.continuousPCS = false;
        dspiConfig.lsbFirst = config->bitOrder;
        /* If extended mode is not available DMA is not available in master mode */
        #if(defined(FEATURE_DSPI_HAS_EXTENDED_MODE))
        dspiConfig.transferType = (dspi_transfer_type_t)config->transferType;
        #else
        dspiConfig.transferType = DSPI_USING_INTERRUPTS;
        #endif
        dspiConfig.rxDMAChannel = config->rxDMAChannel;
        dspiConfig.txDMAChannel = config->txDMAChannel;
        dspiConfig.whichPCS = config->ssPin;
        dspiConfig.pcsPolarity = (dspi_polarity_t)config->ssPolarity;
        dspiConfig.callback = config->callback;
        dspiConfig.callbackParam = config->callbackParam;
        /* Allocate one of the DSPI state structure for this instance */
        index = SpiAllocateState(DspiStateIsAllocated, DspiStateInstanceMapping, inst, NO_OF_DSPI_INSTS_FOR_SPI);
        status = DSPI_MasterInit((dspi_instance_t)inst, (dspi_state_t*)&dspiState[index] ,&dspiConfig);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterTransfer
 * Description   : Initializes a non-blocking master transfer
 * Implements    : SPI_MasterTransfer_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterTransfer(const spi_instance_t * const instance, const void* txBuffer, void* rxBuffer, uint16_t numberOfFrames)
{
    status_t status = STATUS_ERROR;
    
    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        uint16_t bytesPerFrame;
        uint8_t indexLpspi;
        
        indexLpspi = FindLpspiState(instance);
        
        if (indexLpspi != NO_OF_LPSPI_INSTS_FOR_SPI)
        {
            /* Calculate the smallest power of 2 greater than or equal to bitsPerFrame */
            bytesPerFrame = SmallestPowerOfTwo(LpspiState[indexLpspi].bitsPerFrame);

            status = LPSPI_DRV_MasterTransfer(instance->instIdx, txBuffer, rxBuffer, numberOfFrames * bytesPerFrame);
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        uint8_t indexFlexio;
        indexFlexio = FindFlexioState(instance);
        
        if (indexFlexio != NO_OF_FLEXIO_INSTS_FOR_SPI)
        {
            uint16_t bytesPerFrame = FlexioSpiState[indexFlexio].transferSize;
            status = FLEXIO_SPI_DRV_MasterTransfer(&(FlexioSpiState[indexFlexio]), txBuffer, rxBuffer, (uint32_t)((uint32_t)bytesPerFrame * (uint32_t)numberOfFrames));
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {
        uint16_t inst = GetDspiIndex(instance);

        status = DSPI_MasterTransfer((dspi_instance_t)inst,  txBuffer, rxBuffer, numberOfFrames);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterTransferBlocking
 * Description   : Initializes a blocking master transfer with time-out
 * Implements    : SPI_MasterTransferBlocking_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterTransferBlocking(const spi_instance_t * const instance, const void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        uint16_t bytesPerFrame;
        uint8_t indexLpspi;

        indexLpspi = FindLpspiState(instance);
        
        if (indexLpspi != NO_OF_LPSPI_INSTS_FOR_SPI)
        {
            /* Calculate the smallest power of 2 greater than or equal to bitsPerFrame */
            bytesPerFrame = SmallestPowerOfTwo(LpspiState[indexLpspi].bitsPerFrame);

            status = LPSPI_DRV_MasterTransferBlocking(instance->instIdx, txBuffer, rxBuffer, (uint16_t)(numberOfFrames * bytesPerFrame), timeout);
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        uint8_t indexFlexio;
        indexFlexio = FindFlexioState(instance);
        
        if (indexFlexio != NO_OF_FLEXIO_INSTS_FOR_SPI)
        {
            uint16_t bytesPerFrame = FlexioSpiState[indexFlexio].transferSize;

            status = FLEXIO_SPI_DRV_MasterTransferBlocking(&(FlexioSpiState[indexFlexio]), txBuffer, rxBuffer, (uint32_t)((uint32_t)numberOfFrames * (uint32_t)bytesPerFrame), timeout);
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_MasterTransferBlocking((dspi_instance_t)inst,  txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveInit
 * Description   : Configures the SPI in slave mode
 * Implements    : SPI_SlaveInit_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveInit(const spi_instance_t * const instance, const spi_slave_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define SPI PAL over LPSPI */
    #if (defined (SPI_OVER_LPSPI))
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        lpspi_slave_config_t lpspiConfig;
        lpspiConfig.pcsPolarity = (lpspi_signal_polarity_t)(!(bool)(config->ssPolarity));
        lpspiConfig.bitcount = config->frameSize;
        lpspiConfig.clkPhase = (lpspi_clock_phase_t)config->clockPhase;
        lpspiConfig.clkPolarity = (lpspi_sck_polarity_t)config->clockPolarity;
        lpspiConfig.lsbFirst = config->bitOrder;
        lpspiConfig.transferType = (lpspi_transfer_type)config->transferType;
        lpspiConfig.rxDMAChannel = config->rxDMAChannel;
        lpspiConfig.txDMAChannel = config->txDMAChannel;
        lpspiConfig.callback = config->callback;
        lpspiConfig.callbackParam = config->callbackParam;
        lpspiConfig.whichPcs = (lpspi_which_pcs_t)0U;
        /* Allocate one of the LPSPI state structure for this instance */
        index = SpiAllocateState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance->instIdx, NO_OF_LPSPI_INSTS_FOR_SPI);
        status = LPSPI_DRV_SlaveInit(instance->instIdx, (lpspi_state_t*)(&LpspiState[index]), &lpspiConfig);
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if (defined (SPI_OVER_FLEXIO))
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        /* FlexIO driver can be used only for the following frame sizes: 8,16 or 32 */
        DEV_ASSERT((config->frameSize == 8U) || (config->frameSize == 16U) || (config->frameSize == 32U));
        /* FlexIO driver can be used only when SS is active low */
        DEV_ASSERT((config->ssPolarity) == SPI_ACTIVE_LOW);
        flexio_spi_slave_user_config_t flexioConfig;
        switch(config->transferType)
        {
            case SPI_USING_DMA:  flexioConfig.driverType = FLEXIO_DRIVER_TYPE_DMA; break;
            case SPI_USING_INTERRUPTS : flexioConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS; break;
            default : /*Nothing to do */ break;
        }
        flexioConfig.bitOrder = (flexio_spi_transfer_bit_order_t)config->bitOrder;
        switch (config->frameSize)
        {
            case 8U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_1BYTE; break; 
            case 16U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_2BYTE; break;
            case 32U : flexioConfig.transferSize = FLEXIO_SPI_TRANSFER_4BYTE; break;
            default: /* Nothing to do */ break;
        }
        flexioConfig.clockPolarity = (uint8_t)config->clockPolarity;
        flexioConfig.clockPhase = (uint8_t)config->clockPhase;
        flexioConfig.mosiPin = ((extension_flexio_for_spi_t*)(config->extension))->mosiPin;
        flexioConfig.misoPin = ((extension_flexio_for_spi_t*)(config->extension))->misoPin;
        flexioConfig.sckPin = ((extension_flexio_for_spi_t*)(config->extension))->sckPin;
        flexioConfig.ssPin = ((extension_flexio_for_spi_t*)(config->extension))->ssPin;
        /* Link Flexio Callbacks to the callbacks defined in PAL */
        flexioConfig.callback = config->callback;
        /* Store the callbacks defined by the user in PAL internal callbacks vector */
        flexioConfig.callbackParam = config->callbackParam;
        flexioConfig.rxDMAChannel = config->rxDMAChannel;
        flexioConfig.txDMAChannel = config->txDMAChannel;
        status = FLEXIO_DRV_InitDevice(0U, &FlexioState);
        if (status == STATUS_SUCCESS)
        {
            /* Allocate one of the Flexio state structure for this instance */
            index = SpiAllocateState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance->instIdx, NO_OF_FLEXIO_INSTS_FOR_SPI);
            status = FLEXIO_SPI_DRV_SlaveInit(0U, &flexioConfig, (flexio_spi_master_state_t*)&FlexioSpiState[index]);
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI)) 
    {    
        /* DSPI in slave mode works only in active low mode */
        DEV_ASSERT(config->ssPolarity == SPI_ACTIVE_LOW);
        uint32_t inst = GetDspiIndex(instance);
        dspi_slave_config_t dspiConfig;
        dspiConfig.bitcount = config->frameSize;
        dspiConfig.clkPhase = (dspi_clock_phase_t)config->clockPhase;
        dspiConfig.clkPolarity = (dspi_polarity_t)config->clockPolarity;
        dspiConfig.transferType = (dspi_transfer_type_t)config->transferType;
        dspiConfig.rxDMAChannel = config->rxDMAChannel;
        dspiConfig.txDMAChannel = config->txDMAChannel;
        dspiConfig.callback = config->callback;
        dspiConfig.callbackParam = config->callbackParam;
        /* Allocate one of the DSPI state structure for this instance */
        index = SpiAllocateState(DspiStateIsAllocated, DspiStateInstanceMapping, inst, NO_OF_DSPI_INSTS_FOR_SPI);
        status = DSPI_SlaveInit((dspi_instance_t)inst, (dspi_state_t*)&dspiState[index] ,&dspiConfig);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveTransfer
 * Description   : Initializes a non-blocking slave transfer
 * Implements    : SPI_SlaveTransfer_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveTransfer(const spi_instance_t * const instance, const void* txBuffer, void* rxBuffer, uint16_t numberOfFrames)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        uint16_t bytesPerFrame;
        uint8_t indexLpspi;
        
        indexLpspi = FindLpspiState(instance);
        
        if (indexLpspi != NO_OF_LPSPI_INSTS_FOR_SPI)
        {
            /* Calculate the smallest power of 2 greater than or equal to bitsPerFrame */
            bytesPerFrame = SmallestPowerOfTwo(LpspiState[indexLpspi].bitsPerFrame);

            status = LPSPI_DRV_SlaveTransfer(instance->instIdx, txBuffer, rxBuffer, (uint16_t)(numberOfFrames * bytesPerFrame));
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        uint8_t indexFlexio;
        indexFlexio = FindFlexioState(instance);
        
        if (indexFlexio != NO_OF_FLEXIO_INSTS_FOR_SPI)
        {
            uint16_t bytesPerFrame = FlexioSpiState[indexFlexio].transferSize;

            status = FLEXIO_SPI_DRV_SlaveTransfer(&(FlexioSpiState[indexFlexio]), txBuffer, rxBuffer, (uint32_t)((uint32_t)numberOfFrames * (uint32_t)bytesPerFrame));
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {    
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_SlaveTransfer((dspi_instance_t)inst,  txBuffer, rxBuffer, numberOfFrames);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveTransferBlocking
 * Description   : Initializes a blocking slave transfer with timeout
 * Implements    : SPI_SlaveTransferBlocking_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveTransferBlocking(const spi_instance_t * const instance, const void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        uint16_t bytesPerFrame;
        uint8_t indexLpspi;

        indexLpspi = FindLpspiState(instance);
        
        if (indexLpspi != NO_OF_LPSPI_INSTS_FOR_SPI)
        {
            /* Calculate the smallest power of 2 greater than or equal to bitsPerFrame */
            bytesPerFrame = SmallestPowerOfTwo(LpspiState[indexLpspi].bitsPerFrame);

            status = LPSPI_DRV_SlaveTransferBlocking(instance->instIdx, txBuffer, rxBuffer, (uint16_t)(numberOfFrames * bytesPerFrame), timeout);
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        uint8_t indexFlexio;
        indexFlexio = FindFlexioState(instance);
        
        if (indexFlexio != NO_OF_FLEXIO_INSTS_FOR_SPI)
        {
            uint16_t bytesPerFrame = FlexioSpiState[indexFlexio].transferSize;

            status = FLEXIO_SPI_DRV_SlaveTransferBlocking(&(FlexioSpiState[indexFlexio]), txBuffer, rxBuffer, (uint32_t)((uint32_t)numberOfFrames * (uint32_t)bytesPerFrame), timeout);
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {    
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_SlaveTransferBlocking((dspi_instance_t)inst,  txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetDefaultMasterConfig
 * Description   : Gets the default configuration structure for master
 * Implements    : SPI_GetDefaultMasterConfig_Activity
 *
 *END**************************************************************************/
status_t SPI_GetDefaultMasterConfig(spi_master_t *config)
{
    config->baudRate      = 100000U;
    config->ssPolarity    = SPI_ACTIVE_HIGH;
    config->frameSize     = 8U;
    config->clockPhase    = READ_ON_ODD_EDGE;
    config->clockPolarity = SPI_ACTIVE_HIGH;
    config->bitOrder      = SPI_TRANSFER_MSB_FIRST;
    config->transferType  = SPI_USING_INTERRUPTS;
    config->rxDMAChannel  = 255U;
    config->txDMAChannel  = 255U;
    config->callback      = NULL;
    config->callbackParam = NULL;
    config->ssPin         = 0;
    config->extension     = NULL;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetDefaultSlaveConfig
 * Description   : Gets the default configuration structure for slave
 * Implements    : SPI_GetDefaultSlaveConfig_Activity
 *
 *END**************************************************************************/
status_t SPI_GetDefaultSlaveConfig(spi_slave_t *config)
{
    config->ssPolarity    = SPI_ACTIVE_HIGH;
    config->frameSize     = 8U;
    config->clockPhase    = READ_ON_ODD_EDGE;
    config->clockPolarity = SPI_ACTIVE_HIGH;
    config->bitOrder      = SPI_TRANSFER_MSB_FIRST;
    config->transferType  = SPI_USING_INTERRUPTS;
    config->rxDMAChannel  = 255U;
    config->txDMAChannel  = 255U;
    config->callback      = NULL;
    config->callbackParam = NULL;
    config->extension     = NULL;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterDeinit
 * Description   : De-initializes the spi master module
 * Implements    : SPI_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterDeinit(const spi_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        status = LPSPI_DRV_MasterDeinit(instance->instIdx);
        if (status == STATUS_SUCCESS)
        {
            SpiFreeState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance->instIdx, NO_OF_LPSPI_INSTS_FOR_SPI);
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        uint8_t indexFlexio;
        indexFlexio = FindFlexioState(instance);
        
        if (indexFlexio != NO_OF_FLEXIO_INSTS_FOR_SPI)
        {
            status = FLEXIO_SPI_DRV_MasterDeinit(&(FlexioSpiState[indexFlexio]));
            if (status == STATUS_SUCCESS)
            {
                SpiFreeState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance->instIdx, NO_OF_FLEXIO_INSTS_FOR_SPI);
            }
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {    
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_Deinit((dspi_instance_t)inst);
        if (status == STATUS_SUCCESS)
        {
            SpiFreeState(DspiStateIsAllocated, DspiStateInstanceMapping, inst, NO_OF_DSPI_INSTS_FOR_SPI);
        }
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveDeinit
 * Description   : De-initializes the spi slave module
 * Implements    : SPI_SlaveDeinit_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveDeinit(const spi_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        status = LPSPI_DRV_SlaveDeinit(instance->instIdx);
        if (status == STATUS_SUCCESS)
        {
           SpiFreeState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance->instIdx, NO_OF_LPSPI_INSTS_FOR_SPI);
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        status = FLEXIO_SPI_DRV_SlaveDeinit(&(FlexioSpiState[FindFlexioState(instance)]));
        if (status == STATUS_SUCCESS)
        {
           SpiFreeState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance->instIdx, NO_OF_FLEXIO_INSTS_FOR_SPI);
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_Deinit((dspi_instance_t)inst);
        if (status == STATUS_SUCCESS)
        {
           SpiFreeState(DspiStateIsAllocated, DspiStateInstanceMapping, inst, NO_OF_DSPI_INSTS_FOR_SPI);
        }
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SetSS
 * Description   : Changes the SS, if this feature is available.
 * Implements    : SPI_SetSS_Activity
 *
 *END**************************************************************************/
status_t SPI_SetSS(const spi_instance_t * const instance, uint8_t ss)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        status = LPSPI_DRV_SetPcs(instance->instIdx, (lpspi_which_pcs_t)ss, LPSPI_ACTIVE_LOW);
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        (void)ss;
        status = STATUS_UNSUPPORTED;
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI)) 
    {
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_UpdateCS((dspi_instance_t)inst, ss);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetStatus
 * Description   : Gets the status of the last transfer
 * Implements    : SPI_GetStatus_Activity
 *
 *END**************************************************************************/
status_t SPI_GetStatus(const spi_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        uint8_t i;
        /* Find the state structure associated to current instance */
        for (i = 0; i < NO_OF_LPSPI_INSTS_FOR_SPI; i++)
        {
            if (LpspiStateInstanceMapping[i] == instance->instIdx)
            {
                return (status_t)(LpspiState[i].isTransferInProgress ? STATUS_BUSY : STATUS_SUCCESS);
            }
        }
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        if(STATUS_SUCCESS == FLEXIO_SPI_DRV_MasterGetStatus(&FlexioSpiState[FindFlexioState(instance)], NULL))
        {
            status = STATUS_SUCCESS;
        }
        else
        {
            status= STATUS_BUSY;
        }
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {    
        dspi_transfer_status_t dspiStatus;
        uint32_t inst = GetDspiIndex(instance);
        (void)DSPI_GetTransferStatus((dspi_instance_t)inst, &dspiStatus);
        if (dspiStatus == DSPI_TRANSFER_OK)
        {
            status = STATUS_SUCCESS;
        }
        else
        {
            status= STATUS_BUSY;
        }
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI_MasterSetDelay
 * Description   : Insert delays between transfer, or cs and clk using hardware features
 * Implements    : SPI_MasterSetDelay_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterSetDelay(const spi_instance_t * const instance, uint32_t delayBetweenTransfers,
        uint32_t delaySCKtoPCS, uint32_t delayPCStoSCK)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance->instType == SPI_INST_TYPE_LPSPI)
    {
        status = LPSPI_DRV_MasterSetDelay(instance->instIdx, delayBetweenTransfers, delaySCKtoPCS, delayPCStoSCK);
    }
    else
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if (instance->instType == SPI_INST_TYPE_FLEXIO)
    {
        (void)delayBetweenTransfers;
        (void)delaySCKtoPCS;
        (void)delayPCStoSCK;
        status = STATUS_UNSUPPORTED;
    }
    else
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    if ((instance->instType == SPI_INST_TYPE_SPI) || (instance->instType == SPI_INST_TYPE_DSPI))
    {   
        uint32_t inst = GetDspiIndex(instance);
        status = DSPI_MasterSetDelay((dspi_instance_t)inst, delayBetweenTransfers, delaySCKtoPCS, delayPCStoSCK);
    }
    else
    #endif
    {
        DEV_ASSERT(0);
    }
    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
