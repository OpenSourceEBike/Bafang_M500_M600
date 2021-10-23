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
 * @file i2c_pal.c
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
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * State structures are not part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Some functions are part of API and are called by user's application and can't be made static
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type.
 * The expression converts the pal type to specific driver type. The types have
 * the same size.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially enum<i>' to 'essentially enum<i>'.
 * The expression converts the pal type to specific driver type. The types have
 * the same size.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned int, representing an instance number.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned int, representing an instance number.
 *
 */

#include "i2c_pal.h"
#include "device_registers.h"

/* Include PD files */
#if (defined (I2C_OVER_LPI2C))
    #include "lpi2c_driver.h"
#endif

#if (defined (I2C_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_i2c_driver.h"
#endif

#if (defined (I2C_OVER_I2C))
    #include "i2c_driver.h"
#endif

#if (defined (I2C_OVER_SWI2C))
    #include "swi2c_driver.h"
#endif

/* Define state structures for LPI2C */
#if (defined(I2C_OVER_LPI2C))
    /*! @brief I2C state structures */
    static lpi2c_master_state_t Lpi2cMasterState[NO_OF_LPI2C_INSTS_FOR_I2C];
    static lpi2c_slave_state_t Lpi2cSlaveState[NO_OF_LPI2C_INSTS_FOR_I2C];
    /*! @brief LPI2C state-instance matching */
    static uint32_t Lpi2cStateInstanceMapping[NO_OF_LPI2C_INSTS_FOR_I2C];
    /*! @brief LPI2C  available resources table */
    static bool Lpi2cStateIsAllocated[NO_OF_LPI2C_INSTS_FOR_I2C];
#endif

/* Define state structure for FLEXIO */
#if (defined(I2C_OVER_FLEXIO))
    /*! @brief FLEXIO state structures */
    static flexio_i2c_master_state_t FlexioI2CState[NO_OF_FLEXIO_INSTS_FOR_I2C];
    static flexio_device_state_t FlexioState;
    /*! @brief FLEXIO state-instance matching */
    static uint32_t FlexioI2CStateInstanceMapping[NO_OF_FLEXIO_INSTS_FOR_I2C];
    /*! @brief FLEXIO  available resources table */
    static bool FlexioI2CStateIsAllocated[NO_OF_FLEXIO_INSTS_FOR_I2C];
#endif

/* Define state structure for I2C */
#if (defined(I2C_OVER_I2C))
    /*! @brief I2C state structures */
    i2c_master_state_t I2CMasterState[NO_OF_I2C_INSTS_FOR_I2C];
    i2c_slave_state_t I2CSlaveState[NO_OF_I2C_INSTS_FOR_I2C];
    /*! @brief I2C state-instance matching */
    static uint32_t I2CStateInstanceMapping[NO_OF_I2C_INSTS_FOR_I2C];
    /*! @brief I2C available resources table */
    static bool I2CStateIsAllocated[NO_OF_I2C_INSTS_FOR_I2C];
#endif

/* Define state structure for SWI2C */
#if (defined(I2C_OVER_SWI2C))
    /*! @brief SWI2C state structures */
    swi2c_master_state_t SWI2CMasterState[NO_OF_SWI2C_INSTS_FOR_I2C];
#endif

#if defined(I2C_OVER_I2C) || defined(I2C_OVER_LPI2C) || defined (I2C_OVER_FLEXIO)
/*FUNCTION**********************************************************************
*
* Function Name : I2CAllocateState
* Description   : Allocates one of the available state structure.
*
*END**************************************************************************/
static uint8_t I2CAllocateState(bool* isAllocated, uint32_t* instanceMapping, const i2c_instance_t * const instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the I2C state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance->instIdx;
            isAllocated[i] = true;
            break;
        }
     }

    return i;
}
#endif

#if defined(I2C_OVER_I2C) || defined(I2C_OVER_LPI2C) || defined (I2C_OVER_FLEXIO)
/*FUNCTION**********************************************************************
 *
 * Function Name : I2CFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void I2CFreeState(bool* isAllocated, const uint32_t* instanceMapping, const i2c_instance_t * const instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Deallocate one of the available state structure*/
    for (i = 0;i < numberOfinstances;i++)
    {
        if (instanceMapping[i] == instance->instIdx)
        {
            isAllocated[i] = false;
            break;
        }
    }
}
#endif

#if (defined(I2C_OVER_FLEXIO))
/*FUNCTION**********************************************************************
 *
 * Function Name : FindFlexioState
 * Description   : Search the state structure of the flexio instance
 *
 *END**************************************************************************/
static uint8_t FindFlexioState(const i2c_instance_t * const instance)
{
    uint8_t i;
    for (i = 0;i<NO_OF_FLEXIO_INSTS_FOR_I2C;i++)
    {
        if (FlexioI2CStateInstanceMapping[i] == instance->instIdx)
        {
            break;
        }
    }
    return i;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterInit
 * Description   : Configures the I2C in master mode
 * Implements    : I2C_MasterInit_Activity
 *END**************************************************************************/
status_t I2C_MasterInit(const i2c_instance_t * const instance, const i2c_master_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define I2C PAL over I2C */
    #if (defined (I2C_OVER_LPI2C))
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
         lpi2c_master_user_config_t lpi2cConfig;
         lpi2cConfig.slaveAddress = config->slaveAddress;
         lpi2cConfig.is10bitAddr = config->is10bitAddr;
         switch(config->transferType)
         {
             case I2C_PAL_USING_DMA:      lpi2cConfig.transferType = LPI2C_USING_DMA; break;
             case I2C_PAL_USING_INTERRUPTS : lpi2cConfig.transferType = LPI2C_USING_INTERRUPTS; break;
             default:
                 /* Transfer type not available for PAL */
                 break;
         }
         lpi2cConfig.masterCallback = config->callback;
         lpi2cConfig.callbackParam = config->callbackParam;

         switch(config->operatingMode)
         {
             case I2C_PAL_STANDARD_MODE: lpi2cConfig.operatingMode = LPI2C_STANDARD_MODE; break;
             case I2C_PAL_FAST_MODE: lpi2cConfig.operatingMode = LPI2C_FAST_MODE; break;
             #if (LPI2C_HAS_FAST_PLUS_MODE)
             case I2C_PAL_FASTPLUS_MODE:
                     lpi2cConfig.operatingMode = LPI2C_FASTPLUS_MODE;
             break;
             #endif
             #if (LPI2C_HAS_HIGH_SPEED_MODE)
             case I2C_PAL_HIGHSPEED_MODE:
                     lpi2cConfig.operatingMode = LPI2C_HIGHSPEED_MODE;
             break;
             #endif
             #if (LPI2C_HAS_ULTRA_FAST_MODE)
             case I2C_PAL_ULTRAFAST_MODE:
                     lpi2cConfig.operatingMode = LPI2C_ULTRAFAST_MODE;
             break;
			 #endif
             default:
                 /* Operating mode not available for PAL */
                 DEV_ASSERT(0);
                 break;
         }

         lpi2cConfig.baudRate = config->baudRate;
         /* DMA channel */
         lpi2cConfig.dmaChannel = config->dmaChannel1;
         /* Allocate one of the LPI2C state structure for this instance */
         index = I2CAllocateState(Lpi2cStateIsAllocated, Lpi2cStateInstanceMapping, instance, (uint8_t)NO_OF_LPI2C_INSTS_FOR_I2C);
         status = LPI2C_DRV_MasterInit((uint32_t)instance->instIdx, &lpi2cConfig, &Lpi2cMasterState[index]);
    }
    #endif

    #if(defined (I2C_OVER_I2C))
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        i2c_master_user_config_t i2cConfig;
        i2cConfig.slaveAddress = config->slaveAddress;
        switch(config->transferType)
        {
            case I2C_PAL_USING_DMA:      i2cConfig.transferType = I2C_USING_DMA; break;
            case I2C_PAL_USING_INTERRUPTS : i2cConfig.transferType = I2C_USING_INTERRUPTS; break;
            default:
                /* Transfer type not available for PAL */
                break;
        }
        i2cConfig.masterCallback = config->callback;
        i2cConfig.callbackParam = (uint8_t *)instance->instIdx;
        i2cConfig.dmaChannel = config->dmaChannel1;
        i2cConfig.baudRate = config->baudRate;
        /*Allocate one of the I2C state structure for this instance */
        index = I2CAllocateState(I2CStateIsAllocated, I2CStateInstanceMapping, instance, NO_OF_I2C_INSTS_FOR_I2C);
        status = I2C_DRV_MasterInit((uint8_t)instance->instIdx, &i2cConfig, &I2CMasterState[index]);
    }
    #endif

#if (defined(I2C_OVER_SWI2C))
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        swi2c_master_user_config_t swi2cConfig;
        swi2cConfig.slaveAddress = config->slaveAddress;
        swi2cConfig. baudRate = config->baudRate;

        /* Initialize pins */
        swi2cConfig.sclPin = ((extension_swi2c_for_i2c_t *) config->extension)->sclPin;
        swi2cConfig.sdaPin = ((extension_swi2c_for_i2c_t *) config->extension)->sdaPin;
        swi2cConfig.sclReadPin = ((extension_swi2c_for_i2c_t *) config->extension)->sclReadPin;
        swi2cConfig.sdaReadPin = ((extension_swi2c_for_i2c_t *) config->extension)->sdaReadPin;

        status = SWI2C_DRV_MasterInit(&SWI2CMasterState[instance->instIdx], &swi2cConfig);

        /* Cast to avoid compiler warnings */
        (void) index;
    }
#endif

    #if(defined (I2C_OVER_FLEXIO))
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_user_config_t flexioI2CConfig;
        flexioI2CConfig.slaveAddress = config->slaveAddress;
        switch(config->transferType)
        {
            case I2C_PAL_USING_DMA:      flexioI2CConfig.driverType = FLEXIO_DRIVER_TYPE_DMA; break;
            case I2C_PAL_USING_INTERRUPTS : flexioI2CConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS; break;
            default:
                /* Transfer type not available for PAL */
                break;
        }
        flexioI2CConfig.sdaPin = ((extension_flexio_for_i2c_t*)(config->extension))->sdaPin;
        flexioI2CConfig.sclPin = ((extension_flexio_for_i2c_t*)(config->extension))->sclPin;
        flexioI2CConfig.callback = config->callback;
        flexioI2CConfig.callbackParam = config->callbackParam;
        flexioI2CConfig.baudRate = config->baudRate;
        flexioI2CConfig.rxDMAChannel = config -> dmaChannel2;
        flexioI2CConfig.txDMAChannel = config -> dmaChannel1;
        status = FLEXIO_DRV_InitDevice(0U, &FlexioState);

        if(status == STATUS_SUCCESS)
        {
            /* Allocate one of the Flexio state structure for this instance */
            index = I2CAllocateState(FlexioI2CStateIsAllocated, FlexioI2CStateInstanceMapping, instance, NO_OF_FLEXIO_INSTS_FOR_I2C);
            status = FLEXIO_I2C_DRV_MasterInit(0U, &flexioI2CConfig, (flexio_i2c_master_state_t*)&FlexioI2CState[index]);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveInit
 * Description   : Configures the I2C in slave mode
 * Implements    : I2C_SlaveInit_Activity
 *END**************************************************************************/
status_t I2C_SlaveInit(const i2c_instance_t * const instance, const i2c_slave_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define I2C PAL over LPI2C */
    #if (defined (I2C_OVER_LPI2C))
        if(instance->instType == I2C_INST_TYPE_LPI2C)
        {
            lpi2c_slave_user_config_t lpi2cConfig;
            lpi2cConfig.slaveAddress = config->slaveAddress;
            lpi2cConfig.transferType = (lpi2c_transfer_type_t)config->transferType;
            lpi2cConfig.dmaChannel = config->dmaChannel;
            lpi2cConfig.is10bitAddr = config->is10bitAddr;
            lpi2cConfig.slaveListening = config->slaveListening;
			lpi2cConfig.operatingMode = (lpi2c_mode_t)config->operatingMode;
            lpi2cConfig.slaveCallback = config->callback;
            lpi2cConfig.callbackParam = config->callbackParam;
            /*Allocate one of the LPI2C state structure for this instance */
            index = I2CAllocateState(Lpi2cStateIsAllocated, Lpi2cStateInstanceMapping, instance, (uint8_t)NO_OF_LPI2C_INSTS_FOR_I2C);
            status = LPI2C_DRV_SlaveInit((uint32_t)instance->instIdx, &lpi2cConfig, &Lpi2cSlaveState[index]);
        }
    #endif

    /* Define I2C PAL over I2C */
    #if (defined (I2C_OVER_I2C))
        if(instance->instType == I2C_INST_TYPE_I2C)
        {
            i2c_slave_user_config_t i2cConfig;
            i2cConfig.slaveAddress = config->slaveAddress;
            i2cConfig.slaveListening = config->slaveListening;
            i2cConfig.slaveCallback = config->callback;
            i2cConfig.callbackParam = config->callbackParam;
            /*Allocate one of the LPI2C state structure for this instance */
            index = I2CAllocateState(I2CStateIsAllocated, I2CStateInstanceMapping, instance, (uint8_t)NO_OF_I2C_INSTS_FOR_I2C);
            status = I2C_DRV_SlaveInit((uint8_t)instance->instIdx, &i2cConfig, (i2c_slave_state_t*)(&I2CSlaveState[index]));
        }
    #endif

    /* Define I2C PAL over SWI2C */
    #if (defined (I2C_OVER_SWI2C))
        if(instance->instType == I2C_INST_TYPE_SWI2C)
        {
            status = STATUS_UNSUPPORTED;

            /* Cast to void compiler warnings */
            (void) index;
            (void) config;
        }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if (defined (I2C_OVER_FLEXIO))
        if(instance->instType == I2C_INST_TYPE_FLEXIO)
        {
            /* Cast to void to avoid compiler warnings */
            (void) config;
            (void) index;
            status = STATUS_UNSUPPORTED;
        }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterSendData
 * Description   : Initializes a non-blocking master send data transfer
 * Implements    : I2C_MasterSendData_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterSendData(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over LPI2C */
    #if defined (I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterSendData((uint32_t)instance->instIdx, txBuff, txSize, sendStop);
    }
    #endif

    /* Define I2C PAL over I2C */
    #if defined (I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterSendData((uint8_t)instance->instIdx, txBuff, txSize, sendStop);
    }
    #endif

    /* Define I2C PAL over SWI2C */
    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;

        /* Cast to avoid compiler warnings */
        (void) txBuff;
        (void) txSize;
        (void) sendStop;

    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined (I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint8_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterSendData(master, txBuff, txSize, sendStop);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterSendDataBlocking
 * Description   : Initializes a blocking master send data transfer with time-out
 * Implements    : I2C_MasterSendDataBlocking_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterSendDataBlocking(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop, uint32_t timeout)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over LPI2C */
    #if defined (I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterSendDataBlocking((uint32_t)instance->instIdx, txBuff, txSize, sendStop, timeout);
    }
    #endif

    /* Define I2C PAL over I2C */
    #if defined (I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterSendDataBlocking((uint8_t)instance->instIdx, txBuff, txSize, sendStop, timeout);
    }
    #endif

    /* Define I2C PAL over SWI2C */
    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = SWI2C_DRV_MasterSendDataBlocking(&SWI2CMasterState[instance->instIdx], txBuff, txSize, sendStop);

        /* Cast to avoid compiler warnings */
        (void) timeout;
    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined (I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint8_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterSendDataBlocking(master, txBuff, txSize, sendStop, timeout);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterReceiveData
 * Description   : Initializes a non-blocking master receive transfer
 * Implements    : I2C_MasterReceiveData_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterReceiveData(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterReceiveData((uint32_t)instance->instIdx, rxBuff, rxSize, sendStop);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterReceiveData((uint8_t)instance->instIdx, rxBuff, rxSize, sendStop);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;

        /* Cast to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;
        (void) sendStop;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint8_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterReceiveData(master, rxBuff, rxSize, sendStop);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterReceiveDataBlocking
 * Description   : Initializes a blocking master receive transfer
 * Implements    : I2C_MasterReceiveDataBlocking_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterReceiveDataBlocking(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop, uint32_t timeout)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterReceiveDataBlocking((uint32_t)instance->instIdx, rxBuff, rxSize, sendStop, timeout);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterReceiveDataBlocking((uint8_t)instance->instIdx, rxBuff, rxSize, sendStop, timeout);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = SWI2C_DRV_MasterReceiveDataBlocking(&SWI2CMasterState[instance->instIdx], rxBuff, rxSize, sendStop);

        /* Cast to avoid compiler warnings */
        (void) timeout;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterReceiveDataBlocking(master, rxBuff, rxSize, sendStop, timeout);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterDeinit
 * Description   : De-initializes the i2c master module
 * Implements    : I2C_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterDeinit(const i2c_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterDeinit((uint32_t)instance->instIdx);
        I2CFreeState(Lpi2cStateIsAllocated, Lpi2cStateInstanceMapping, instance, (uint8_t)NO_OF_LPI2C_INSTS_FOR_I2C);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterDeinit((uint8_t)instance->instIdx);
        I2CFreeState(I2CStateIsAllocated, I2CStateInstanceMapping, instance, (uint8_t)NO_OF_I2C_INSTS_FOR_I2C);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;

    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterDeinit(master);
        if (status == STATUS_SUCCESS)
        {
            I2CFreeState(FlexioI2CStateIsAllocated, FlexioI2CStateInstanceMapping, instance, NO_OF_FLEXIO_INSTS_FOR_I2C);
        }
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterSetSlaveAddress
 * Description   : set the slave address for any subsequent I2C communication
 * Implements : I2C_MasterSetSlaveAddress_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterSetSlaveAddress(const i2c_instance_t * const instance, const uint16_t address, const bool is10bitAddr)
{
    status_t status = STATUS_ERROR;

    #if defined (I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        (void) LPI2C_DRV_MasterSetSlaveAddr((uint32_t) instance->instIdx, address, is10bitAddr);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined (I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        (void) I2C_DRV_MasterSetSlaveAddress((uint32_t) instance->instIdx, address);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        (void) SWI2C_DRV_MasterSetSlaveAddress(&SWI2CMasterState[instance->instIdx], address);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined (I2C_OVER_FLEXIO)
    if (instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterSetSlaveAddr(master, address);
    }
    #endif

    /* Cast to void to avoid compiler warnings for the cases where is10bitAddr is not used */
    (void) is10bitAddr;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_MasterSetBaudRate
 * Description   : set the baud rate for any subsequent I2C communication
 * Implements : I2C_MasterSetBaudRate_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterSetBaudRate(const i2c_instance_t * const instance, const i2c_master_t *config, uint32_t baudRate)
{
    status_t status = STATUS_ERROR;
    uint32_t counter = 0U;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        lpi2c_baud_rate_params_t baudrateLpi2c;

        baudrateLpi2c.baudRate = baudRate;
        (void)LPI2C_DRV_MasterSetBaudRate((uint32_t)instance->instIdx, (lpi2c_mode_t) config->operatingMode, baudrateLpi2c);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance -> instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterSetBaudRate((uint32_t)instance->instIdx, baudRate);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
    	/* SWI2C MAX baudRate is about 90KHz*/
    	if(baudRate >= 90000U)
    	{
    		counter = 0U;
    	}

    	/* BaudRate smaller that 100 Hz*/
    	if(baudRate <= 100U)
    	{
    		counter = 30000U;
    	}

    	/* Calculate SCL waiting cycles */
        if((baudRate < 90000U) && (baudRate > 100U))
        {
        	counter = (uint32_t)((1000000U/baudRate) - 11U)*(5U);
        }


        (void) SWI2C_DRV_SetWaitTimeForSCLTransition(&SWI2CMasterState[instance->instIdx], counter);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *masterFlexio;
        uint32_t instFlexio;

        /* Cast to void to avoid compiler warnings */
        (void) config;

        instFlexio = FindFlexioState(instance);
        masterFlexio = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterSetBaudRate(masterFlexio, baudRate);
    }
    #endif

    /* Cast to void to avoid compiler warnings */
    (void) config;
    (void) counter;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_MasterGetBaudRate
 * Description   : get the baud rate for any subsequent I2C communication
 * Implements : I2C_MasterGetBaudRate_Activity 
 *
 *END**************************************************************************/
status_t I2C_MasterGetBaudRate(const i2c_instance_t * const instance, uint32_t *baudRate)
{
    status_t status = STATUS_ERROR;

    #if defined (I2C_OVER_LPI2C)
    if (instance->instType == I2C_INST_TYPE_LPI2C)
    {
        lpi2c_baud_rate_params_t baudrateLpi2c;

        (void) LPI2C_DRV_MasterGetBaudRate((uint32_t)instance->instIdx, &baudrateLpi2c);
        *baudRate = baudrateLpi2c.baudRate;
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if (instance->instType == I2C_INST_TYPE_I2C)
    {
        *baudRate = I2C_DRV_MasterGetBaudRate((uint32_t)instance->instIdx);
        status = STATUS_SUCCESS;
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;

        /* Cast to avoid compiler warnings */
        (void) baudRate;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if (instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];
        status = FLEXIO_I2C_DRV_MasterGetBaudRate(master, baudRate);
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_GetDefaultMasterConfig
 * Description   : Gets the default configuration structure for master
 * Implements    : I2C_GetDefaultMasterConfig_Activity
 *
 *END**************************************************************************/
status_t I2C_GetDefaultMasterConfig(i2c_master_t *config)
{
    config->slaveAddress = 32U;
    config->is10bitAddr = false;

#if defined(I2C_OVER_SWI2C)
    config->baudRate = 0U;
#else
    config->baudRate = 100000U;
#endif

    config->transferType = I2C_PAL_USING_INTERRUPTS;
    config->operatingMode = I2C_PAL_STANDARD_MODE;
    config->callback = NULL;
    config->callbackParam = NULL;
    config->extension = NULL;

    return STATUS_SUCCESS;
}

#if (defined(I2C_OVER_I2C) || defined(I2C_OVER_LPI2C))
/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_GetDefaultSlaveConfig
 * Description   : Gets the default configuration structure for slave
 * Implements    : I2C_GetDefaultSlaveConfig_Activity
 *
 *END**************************************************************************/
status_t I2C_GetDefaultSlaveConfig(i2c_slave_t *config)
{
    config->slaveAddress = 32U;
    config->is10bitAddr = false;
    config->slaveListening = true;
    config->transferType = I2C_PAL_USING_INTERRUPTS;
    config->callback = NULL;
    config->callbackParam = NULL;

    return STATUS_SUCCESS;
}
#endif 

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveSendData
 * Description   : Initializes a non-blocking master transfer
 * Implements    : I2C_SlaveSendData_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveSendData(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveSendData((uint32_t)instance->instIdx, txBuff, txSize);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveSendData((uint8_t)instance->instIdx, txBuff, txSize);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;

        /* Cast to avoid compiler warnings */
        (void) txBuff;
        (void) txSize;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings for the unused parameters */
        (void) txBuff;
        (void) txSize;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveSendDataBlocking
 * Description   : perform a blocking receive transaction on the I2C bus
 * Implements : I2C_SlaveSendDataBlocking_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveSendDataBlocking(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveSendDataBlocking((uint32_t)instance->instIdx, txBuff, txSize, timeout);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveSendDataBlocking((uint8_t)instance->instIdx, txBuff, txSize, timeout);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to void to avoid compiler warnings for the unused parameter */
        (void) txBuff;
        (void) txSize;
        (void) timeout;

        status = STATUS_UNSUPPORTED;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings for the unused parameter */
        (void) txBuff;
        (void) txSize;
        (void) timeout;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveReceiveData
 * Description   : perform a non-blocking receive transaction on the I2C bus
 * Implements : I2C_SlaveReceiveData_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveReceiveData(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveReceiveData((uint32_t)instance->instIdx, rxBuff, rxSize);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveReceiveData((uint8_t)instance->instIdx, rxBuff, rxSize);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;

        status = STATUS_UNSUPPORTED;

    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveReceiveDataBlocking
 * Description   : perform a blocking receive transaction on the I2C bus
 * Implements : I2C_SlaveReceiveDataBlocking_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveReceiveDataBlocking(const i2c_instance_t * const instance, uint8_t *rxBuff,
        uint32_t rxSize, uint32_t timeout)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveReceiveDataBlocking((uint32_t)instance->instIdx, rxBuff, rxSize, timeout);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveReceiveDataBlocking((uint8_t)instance->instIdx, rxBuff, rxSize, timeout);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to void to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;
        (void) timeout;

        status = STATUS_UNSUPPORTED;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;
        (void) timeout;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveSetRxBuffer
 * Description   : Provide a buffer for receiving data.
 *
 * Implements : I2C_SlaveSetRxBuffer_Activity
 *END***************************************************************************/
status_t I2C_SlaveSetRxBuffer(const i2c_instance_t * const instance, uint8_t *rxBuff, uint32_t rxSize)
{
    status_t status = STATUS_ERROR;
    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveSetRxBuffer(instance->instIdx, rxBuff, rxSize);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveSetRxBuffer((uint8_t) instance->instIdx, rxBuff, rxSize);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to void to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;

        status = STATUS_UNSUPPORTED;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings */
        (void) rxBuff;
        (void) rxSize;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveSetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 *
 * Implements : I2C_SlaveSetTxBuffer_Activity
 *END***************************************************************************/
status_t I2C_SlaveSetTxBuffer(const i2c_instance_t * const instance, const uint8_t *txBuff, uint32_t txSize)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveSetTxBuffer(instance->instIdx, txBuff, txSize);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveSetTxBuffer((uint8_t) instance->instIdx, txBuff, txSize);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to void to avoid compiler warnings */
        (void) txBuff;
        (void) txSize;

        status = STATUS_UNSUPPORTED;
    }
    #endif


    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        /* Cast to void to avoid compiler warnings */
        (void) txBuff;
        (void) txSize;
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveDeinit
 * Description   : De-initializes the i2c slave module
 * Implements    : I2C_SlaveDeinit_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveDeinit(const i2c_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveDeinit((uint32_t)instance->instIdx);
        I2CFreeState(Lpi2cStateIsAllocated, Lpi2cStateInstanceMapping, instance, (uint8_t)NO_OF_LPI2C_INSTS_FOR_I2C);
    }
    #endif

    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveDeinit((uint8_t)instance->instIdx);
        I2CFreeState(I2CStateIsAllocated, I2CStateInstanceMapping, instance, (uint8_t)NO_OF_I2C_INSTS_FOR_I2C);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterGetTransferStatus
 * Description   : Get the status of the current non-blocking I2C master transaction
 * Implements    : I2C_MasterGetTransferStatus_Activity
 *END**************************************************************************/
status_t I2C_MasterGetTransferStatus(const i2c_instance_t * const instance, uint32_t *bytesRemaining)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over I2C */
    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterGetTransferStatus(instance->instIdx, bytesRemaining);
    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];

        status = FLEXIO_I2C_DRV_MasterGetStatus(master, bytesRemaining);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        /* Cast to void to avoid compiler warnings */
        (void)bytesRemaining;

        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define I2C PAL over LPI2C */
    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterGetTransferStatus(instance->instIdx);

        /* Cast to void to avoid compiler warnings */
        (void)bytesRemaining;
    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveGetTransferStatus
 * Description   : Get the status of the current non-blocking I2C slave transaction
 * Implements    : I2C_SlaveGetTransferStatus_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveGetTransferStatus(const i2c_instance_t * const instance, uint32_t *bytesRemaining)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over I2C */
    #if defined(I2C_OVER_LPI2C)
    if(instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveGetTransferStatus(instance->instIdx, bytesRemaining);
    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define I2C PAL over SWI2C */
    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define I2C PAL over LPI2C */
    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveGetTransferStatus(instance->instIdx);
    }
    #endif

    (void) bytesRemaining;

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_MasterAbortTransfer
 * Description   : abort a non-blocking I2C Master transmission or reception
 * Implements    : I2C_MasterAbortTransfer_Activity
 *
 *END**************************************************************************/
status_t I2C_MasterAbortTransfer(const i2c_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over I2C */
    #if defined(I2C_OVER_LPI2C)
    if (instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_MasterAbortTransferData(instance->instIdx);
    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        flexio_i2c_master_state_t *master;
        uint32_t instFlexio;

        instFlexio = FindFlexioState(instance);
        master = &FlexioI2CState[instFlexio];

        status = FLEXIO_I2C_DRV_MasterTransferAbort(master);
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;
    }

    #endif

    /* Define I2C PAL over LPI2C */
    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_MasterAbortTransferData(instance->instIdx);

    }
    #endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_SlaveAbortTransfer
 * Description   : abort a non-blocking I2C slave transmission or reception
 * Implements    : I2C_SlaveAbortTransfer_Activity
 *
 *END**************************************************************************/
status_t I2C_SlaveAbortTransfer(const i2c_instance_t * const instance)
{
    status_t status = STATUS_ERROR;

    /* Define I2C PAL over I2C */
    #if defined(I2C_OVER_LPI2C)
    if (instance->instType == I2C_INST_TYPE_LPI2C)
    {
        status = LPI2C_DRV_SlaveAbortTransferData(instance->instIdx);
    }
    #endif

    /* Define I2C PAL over FLEXIO */
    #if defined(I2C_OVER_FLEXIO)
    if(instance->instType == I2C_INST_TYPE_FLEXIO)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    #if defined(I2C_OVER_SWI2C)
    if(instance->instType == I2C_INST_TYPE_SWI2C)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define I2C PAL over LPI2C */
    #if defined(I2C_OVER_I2C)
    if(instance->instType == I2C_INST_TYPE_I2C)
    {
        status = I2C_DRV_SlaveAbortTransferData(instance->instIdx);
    }
    #endif

    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
