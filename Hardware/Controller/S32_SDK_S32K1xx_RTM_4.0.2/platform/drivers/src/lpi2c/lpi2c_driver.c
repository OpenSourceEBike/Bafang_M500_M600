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
 * @lpi2c_driver.c
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
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
 * visible when an object or function with external linkage is defined.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variables are defined in the common source file and this rule can't be
 * applied.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type with many values.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially Boolean'
 * to 'essentially unsigned'. This is required by the conversion of a bool into a bit.
 * Impermissible cast; cannot cast from 'essentially unsigned' to 'essentially enum<i>'.
 * This is required by the conversion of a bitfield of a register into a enum.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 * This conversion is required because the converted values are the addresses used in DMA transfer.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long, cast from unsigned long to pointer
 * and cast from unsigned int to pointer. The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa. The cast is required to initialize a DMA
 * transfer. The converted value is the address of a buffer.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.8, Attempt to cast away const/volatile from a pointer or reference.
 * The cast is required to initialize a DMA transfer. The converted value is the address of a register.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code
 * structure and better readability.
 */

#include "lpi2c_driver.h"
#include "interrupt_manager.h"
#include "lpi2c_hw_access.h"
#include "clock_manager.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/* Constraints used for baud rate computation */
#define CLKHI_MIN_VALUE 1U
#define CLKLO_MIN_VALUE 3U
#define CLKHI_MAX_VALUE ((1U << LPI2C_MCCR0_CLKHI_WIDTH) - 1U)
#define CLKLO_MAX_VALUE CLKHI_MAX_VALUE
#define DATAVD_MIN_VALUE 1U
#define SETHOLD_MIN_VALUE 2U
#define BUSIDLE_MAX_VALUE 0xFFFU

/* Table of base addresses for LPI2C instances. */
static LPI2C_Type * const g_lpi2cBase[LPI2C_INSTANCE_COUNT] = LPI2C_BASE_PTRS;

/* LPI2C DMA request sources */
static const uint8_t g_lpi2cDMASrc[LPI2C_INSTANCE_COUNT][2] = LPI2C_EDMA_REQ;

/* Pointer to runtime state structure.*/
static lpi2c_master_state_t* g_lpi2cMasterStatePtr[LPI2C_INSTANCE_COUNT] = {NULL};
static lpi2c_slave_state_t* g_lpi2cSlaveStatePtr[LPI2C_INSTANCE_COUNT] = {NULL};

/* Table for lpi2c IRQ numbers */
static const IRQn_Type g_lpi2cMasterIrqId[LPI2C_INSTANCE_COUNT] = LPI2C_MASTER_IRQS;
static const IRQn_Type g_lpi2cSlaveIrqId[LPI2C_INSTANCE_COUNT] = LPI2C_SLAVE_IRQS;

/* PCC clock sources, for getting the input clock frequency */
static const clock_names_t g_lpi2cClock[LPI2C_INSTANCE_COUNT] = LPI2C_PCC_CLOCKS;

/* Callback for master DMA transfer done.*/
static void LPI2C_DRV_MasterCompleteDMATransfer(void* parameter, edma_chn_status_t status);

/*! @brief Direction of a LPI2C transfer - transmit or receive. */
typedef enum
{
   LPI2C_TX_REQ = 0,    /*!< The driver will perform an I2C transmit transfer */
   LPI2C_RX_REQ = 1,    /*!< The driver will perform an I2C receive transfer */
} lpi2c_transfer_direction_t;

/*!
 * @brief DMA internal parameters structure
 *
 * This structure is used in DMA transfers. It contains different
 * variables required for setting and maintaining a DMA transfer.
 */
typedef struct
{
    /*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint8_t dmaChannel;                             /* Channel number for the DMA channel */
    edma_transfer_type_t dmaTransferType;           /* Type for the DMA transfer */
    uint32_t i2cDataRegAddr;                        /* An i2c data register address */
    uint8_t *bufferTransfer;                        /* Buffer used for transfer */
    uint32_t transferSize;                          /* Size of the data to be transfered */
    lpi2c_transfer_direction_t transferDirection;   /* Tells if the driver will make a receive or transmit DMA transfer */
    /*! @endcond */
} lpi2c_dma_transfer_params_t;


/*******************************************************************************
 * Private Functions
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterCmdQueueEmpty
 * Description   : checks if there are any commands in the master software queue
 *
 *END**************************************************************************/
static inline bool LPI2C_DRV_MasterCmdQueueEmpty(const lpi2c_master_state_t * master)
{
    DEV_ASSERT(master != NULL);

    return (master->cmdQueue.writeIdx == master->cmdQueue.readIdx);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterResetQueue
 * Description   : resets the master software queue
 *
 *END**************************************************************************/
static inline void LPI2C_DRV_MasterResetQueue(lpi2c_master_state_t * master)
{
    DEV_ASSERT(master != NULL);

    master->cmdQueue.readIdx = 0U;
    master->cmdQueue.writeIdx = 0U;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterQueueCmd
 * Description   : queues a command in the hardware FIFO or in the master software queue
 *
 *END**************************************************************************/
static inline void LPI2C_DRV_MasterQueueCmd(LPI2C_Type *baseAddr,
                                            lpi2c_master_state_t * master,
                                            lpi2c_master_command_t cmd,
                                            uint8_t data)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = LPI2C_Get_MasterTxFIFOCount(baseAddr);
    uint16_t txFIFOSize = LPI2C_Get_MasterTxFIFOSize(baseAddr);

    /* Check if there is room in the hardware FIFO */
    if (txFIFOCount < txFIFOSize)
    {
        LPI2C_Cmd_MasterTransmit(baseAddr, cmd, data);
    }
    else
    {
        /* Hardware FIFO full, use software FIFO */
        DEV_ASSERT(master->cmdQueue.writeIdx < LPI2C_MASTER_CMD_QUEUE_SIZE);

        master->cmdQueue.cmd[master->cmdQueue.writeIdx] = cmd;
        master->cmdQueue.data[master->cmdQueue.writeIdx] = data;
        master->cmdQueue.writeIdx++;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSendQueuedCmd
 * Description   : transfers commands from the master software queue to the hardware FIFO
 *
 *END**************************************************************************/
static inline void LPI2C_DRV_MasterSendQueuedCmd(LPI2C_Type *baseAddr, lpi2c_master_state_t * master)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = LPI2C_Get_MasterTxFIFOCount(baseAddr);
    uint16_t txFifoSize = LPI2C_Get_MasterTxFIFOSize(baseAddr);

    while ((!LPI2C_DRV_MasterCmdQueueEmpty(master)) && (txFIFOCount < txFifoSize))
    {
        LPI2C_Cmd_MasterTransmit(baseAddr,
                                    master->cmdQueue.cmd[master->cmdQueue.readIdx],
                                    master->cmdQueue.data[master->cmdQueue.readIdx]);
        master->cmdQueue.readIdx++;

        txFIFOCount = LPI2C_Get_MasterTxFIFOCount(baseAddr);
    }

    if (LPI2C_DRV_MasterCmdQueueEmpty(master))
    {
        /* Reset queue */
        LPI2C_DRV_MasterResetQueue(master);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSendAddress
 * Description   : send start event and slave address
 *                 parameter receive specifies the direction of the transfer
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterSendAddress(LPI2C_Type *baseAddr,
                                        lpi2c_master_state_t * master,
                                        bool receive)
{
    uint8_t addrByte;
    lpi2c_master_command_t startCommand;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baseAddr != NULL);

#if(LPI2C_HAS_HIGH_SPEED_MODE)
    if ((master->operatingMode == LPI2C_HIGHSPEED_MODE) && (master->highSpeedInProgress == false))
    {
        /* Initiating High-speed mode - send master code first */
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, LPI2C_MASTER_COMMAND_START_NACK, master->masterCode);
        master->highSpeedInProgress = true;
    }

    if (master->highSpeedInProgress == true)
    {
        /* Use high-speed settings after start event in High Speed mode */
        startCommand = LPI2C_MASTER_COMMAND_START_HS;
    }
    else
#endif
    {
        /* Normal START command */
        startCommand = LPI2C_MASTER_COMMAND_START;
    }

    if (master->is10bitAddr)
    {
        /* 10-bit addressing */
        /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 0(transmit) */
        addrByte = (uint8_t)(0xF0U + ((master->slaveAddress >> 7U) & 0x6U) + (uint8_t)0U);
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
        /* Second address byte: Remaining 8 bits of address */
        addrByte = (uint8_t)(master->slaveAddress & 0xFFU);
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, LPI2C_MASTER_COMMAND_TRANSMIT, addrByte);
        if (receive == true)
        {
            /* Receiving from 10-bit slave - must send repeated start and resend first address byte */
            /* First address byte: 1111 0XXD, where XX are bits 10 and 9 of address, and D = 1 (receive) */
            addrByte = (uint8_t)(0xF0U + ((master->slaveAddress >> 7U) & 0x6U) + (uint8_t)1U);
            LPI2C_DRV_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
        }
    }
    else
    {
        /* 7-bit addressing */
        /* Address byte: slave 7-bit address + D = 0(transmit) or 1 (receive) */
        addrByte = (uint8_t)((master->slaveAddress << 1U) + (uint8_t)receive);
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, startCommand, addrByte);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterQueueData
 * Description   : queues transmit data in the LPI2C tx fifo until it is full
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterQueueData(LPI2C_Type *baseAddr,
                                      lpi2c_master_state_t * master)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baseAddr != NULL);

    uint16_t txFIFOCount = LPI2C_Get_MasterTxFIFOCount(baseAddr);
    uint16_t txFifoSize = LPI2C_Get_MasterTxFIFOSize(baseAddr);

    /* Don't queue any data if there are commands in the software queue */
    if (LPI2C_DRV_MasterCmdQueueEmpty(master))
    {
        while ((master->txSize > 0U) && (txFIFOCount < txFifoSize))
        {
            LPI2C_Cmd_MasterTransmit(baseAddr, LPI2C_MASTER_COMMAND_TRANSMIT, master->txBuff[0U]);
            master->txBuff++;
            master->txSize--;

            txFIFOCount = LPI2C_Get_MasterTxFIFOCount(baseAddr);
        }
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterEndTransfer
 * Description   : ends current transmission or reception
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterEndTransfer(LPI2C_Type *baseAddr,
                                        lpi2c_master_state_t * master,
                                        bool sendStop,
                                        bool resetFIFO)
{
    DEV_ASSERT(master != NULL);
    DEV_ASSERT(baseAddr != NULL);

    /* Disable all events */
    LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                     LPI2C_MASTER_ARBITRATION_LOST_INT |
                                     LPI2C_MASTER_NACK_DETECT_INT |
                                     LPI2C_MASTER_TRANSMIT_DATA_INT |
                                     LPI2C_MASTER_RECEIVE_DATA_INT,
                           false);

    if (master->transferType == LPI2C_USING_DMA)
    {
        /* Stop DMA channel */
        (void)EDMA_DRV_StopChannel(master->dmaChannel);
        
        /* Disable LPI2C DMA request. */
        if (master->rxSize != (uint16_t)0)
        {
            (void)LPI2C_Set_MasterRxDMA(baseAddr, false);
        }
        else
        {
            (void)LPI2C_Set_MasterTxDMA(baseAddr, false);
        }
    }
    
    if (resetFIFO == true)
    {
        /* Reset FIFOs if requested */
        LPI2C_Reset_MasterTxFIFOCmd(baseAddr);
        LPI2C_Reset_MasterRxFIFOCmd(baseAddr);
    }

    /* Queue STOP command if requested */
    if (sendStop == true)
    {
        LPI2C_Cmd_MasterTransmit(baseAddr, LPI2C_MASTER_COMMAND_STOP, 0U);
#if(LPI2C_HAS_HIGH_SPEED_MODE)
        master->highSpeedInProgress = false; /* High-speed transfers end at STOP condition */
#endif
    }

    master->txBuff = NULL;
    master->txSize = 0;
    master->rxBuff = NULL;
    master->rxSize = 0;
    master->i2cIdle = true;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveEndTransfer
 * Description   : ends current transmission or reception
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveEndTransfer(LPI2C_Type *baseAddr,
                                    lpi2c_slave_state_t *slave)
{
    DEV_ASSERT(slave != NULL);
    DEV_ASSERT(baseAddr != NULL);

    /* Deactivate events */

    LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                    LPI2C_SLAVE_FIFO_ERROR_INT |
                                    LPI2C_SLAVE_STOP_DETECT_INT |
                                    LPI2C_SLAVE_REPEATED_START_INT |
                                    LPI2C_SLAVE_ADDRESS_VALID_INT |
                                    LPI2C_SLAVE_RECEIVE_DATA_INT |
                                    LPI2C_SLAVE_TRANSMIT_DATA_INT,
                          false);

    /* For DMA we must disable the DMA request. */
    if (slave->transferType == LPI2C_USING_DMA)
    {
        if (slave->rxSize != (uint16_t)0)
        {
            (void)LPI2C_Set_SlaveRxDMA(baseAddr, false);
        }
        else
        {
            (void)LPI2C_Set_SlaveTxDMA(baseAddr, false);
        }
    }

    /* Disable LPI2C slave */
    LPI2C_Set_SlaveEnable(baseAddr, false);

    slave->isTransferInProgress = false;
    slave->rxBuff = NULL;
    slave->rxSize = 0U;
    slave->txBuff = NULL;
    slave->txSize = 0U;
    slave->repeatedStarts = 0U;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSetOperatingMode
 * Description   : sets the operating mode of the I2C master
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterSetOperatingMode(uint32_t instance, lpi2c_mode_t operatingMode)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t * master;

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

#if(LPI2C_HAS_ULTRA_FAST_MODE)
    if (operatingMode == LPI2C_ULTRAFAST_MODE)
    {
        LPI2C_Set_MasterPinConfig(baseAddr, LPI2C_CFG_2PIN_OUTPUT_ONLY);
        LPI2C_Set_MasterNACKConfig(baseAddr, LPI2C_NACK_IGNORE);
    }
    else
#endif
    {
        LPI2C_Set_MasterPinConfig(baseAddr, LPI2C_CFG_2PIN_OPEN_DRAIN);
        LPI2C_Set_MasterNACKConfig(baseAddr, LPI2C_NACK_RECEIVE);
    }

    master->operatingMode = operatingMode;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveSetOperatingMode
 * Description   : sets the operating mode of the I2C slave
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveSetOperatingMode(uint32_t instance, lpi2c_mode_t operatingMode)
{
    LPI2C_Type *baseAddr;
    lpi2c_slave_state_t * slave;

    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

#if(LPI2C_HAS_ULTRA_FAST_MODE)
    if (operatingMode == LPI2C_ULTRAFAST_MODE)
    {
        LPI2C_Set_SlaveIgnoreNACK(baseAddr, LPI2C_SLAVE_NACK_CONTINUE_TRANSFER);
        LPI2C_Set_SlaveTransmitNACK(baseAddr, LPI2C_SLAVE_TRANSMIT_NACK);
        /* Disable all clock stretching in ultra-fast mode */
        LPI2C_Set_SlaveACKStall(baseAddr, false);
        LPI2C_Set_SlaveTXDStall(baseAddr, false);
        LPI2C_Set_SlaveRXStall(baseAddr, false);
        LPI2C_Set_SlaveAddrStall(baseAddr, false);
    }
    else
#endif
    {
        LPI2C_Set_SlaveIgnoreNACK(baseAddr, LPI2C_SLAVE_NACK_END_TRANSFER);
        LPI2C_Set_SlaveTransmitNACK(baseAddr, LPI2C_SLAVE_TRANSMIT_ACK);
        /* Enable clock stretching except ACKSTALL (we don't need to send ACK/NACK manually) */
        LPI2C_Set_SlaveACKStall(baseAddr, false);
        LPI2C_Set_SlaveTXDStall(baseAddr, true);
        LPI2C_Set_SlaveRXStall(baseAddr, true);
        LPI2C_Set_SlaveAddrStall(baseAddr, true);
    }

#if(LPI2C_HAS_HIGH_SPEED_MODE)
    if (operatingMode == LPI2C_HIGHSPEED_MODE)
    {
        /* Enable detection of the High-speed Mode master code */
        LPI2C_Set_SlaveHighSpeedModeDetect(baseAddr, true);
    }
    else
#endif
    {
        /* Disable detection of the High-speed Mode master code */
        LPI2C_Set_SlaveHighSpeedModeDetect(baseAddr, false);
    }

    slave->operatingMode = operatingMode;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_ConfigureDmaTransfer
 * Description   : configures the DMA transfer
 *
 *END**************************************************************************/
static void LPI2C_DRV_ConfigureDmaTransfer(uint32_t instance, const lpi2c_dma_transfer_params_t *dmaTransParams)
{
    /* Configure DMA channel */
    if (dmaTransParams->transferDirection == LPI2C_TX_REQ)
    {
        (void)EDMA_DRV_SetChannelRequestAndTrigger(dmaTransParams->dmaChannel,  g_lpi2cDMASrc[instance][LPI2C_TX_REQ], false);
        (void)EDMA_DRV_ConfigMultiBlockTransfer(dmaTransParams->dmaChannel, dmaTransParams->dmaTransferType, (uint32_t)dmaTransParams->bufferTransfer,
                                                (uint32_t)dmaTransParams->i2cDataRegAddr, EDMA_TRANSFER_SIZE_1B, (uint32_t)1U,
                                                (uint32_t)dmaTransParams->transferSize, false);
    }
    else
    {
        (void)EDMA_DRV_SetChannelRequestAndTrigger(dmaTransParams->dmaChannel,  g_lpi2cDMASrc[instance][LPI2C_RX_REQ], false);
        (void)EDMA_DRV_ConfigMultiBlockTransfer(dmaTransParams->dmaChannel, dmaTransParams->dmaTransferType, (uint32_t)dmaTransParams->i2cDataRegAddr,
                                                (uint32_t)dmaTransParams->bufferTransfer, EDMA_TRANSFER_SIZE_1B, (uint32_t)1U,
                                                (uint32_t)dmaTransParams->transferSize, false);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterStartDmaTransfer
 * Description   : starts the DMA transfer for master
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterStartDmaTransfer(uint32_t instance)
{
    LPI2C_Type *baseAddr = g_lpi2cBase[instance];
    lpi2c_master_state_t *master = g_lpi2cMasterStatePtr[instance];
    lpi2c_dma_transfer_params_t dmaTransParams;
    bool receive = false;

    dmaTransParams.dmaChannel = master->dmaChannel;
    if (master->txSize > 0U)
    {
        /* Configure watermarks for transmit DMA for master */
        uint32_t txBytes = LPI2C_Get_MasterTxFIFOSize(baseAddr);
        if (txBytes > master->txSize)
        {
            txBytes = master->txSize;
        }
        LPI2C_Set_MasterTxFIFOWatermark(baseAddr, (uint16_t)(txBytes - 1U));

        dmaTransParams.dmaTransferType = EDMA_TRANSFER_MEM2PERIPH;
        dmaTransParams.i2cDataRegAddr = (uint32_t)(&(baseAddr->MTDR));
        dmaTransParams.bufferTransfer = (uint8_t *)master->txBuff;
        dmaTransParams.transferDirection = LPI2C_TX_REQ;
        dmaTransParams.transferSize = master->txSize;

    }
    else
    {
        /* Configure watermarks for receive DMA for master */
        LPI2C_Set_MasterRxFIFOWatermark(baseAddr, 0U);

        receive = true;

        dmaTransParams.dmaTransferType = EDMA_TRANSFER_PERIPH2MEM;
        dmaTransParams.i2cDataRegAddr = (uint32_t)(&(baseAddr->MRDR));
        dmaTransParams.bufferTransfer = master->rxBuff;
        dmaTransParams.transferDirection = LPI2C_RX_REQ;
        dmaTransParams.transferSize = master->rxSize;
    }

    (void)LPI2C_DRV_ConfigureDmaTransfer(instance, &dmaTransParams);

    /* Disable DMA requests for channel when transfer is done */
    EDMA_DRV_DisableRequestsOnTransferComplete(dmaTransParams.dmaChannel, true);

    /* Call callback function when all the bytes were transfered. */
    (void)EDMA_DRV_InstallCallback(dmaTransParams.dmaChannel, (LPI2C_DRV_MasterCompleteDMATransfer), (void*)(instance));

    /* Start channel */
    (void)EDMA_DRV_StartChannel(dmaTransParams.dmaChannel);

    LPI2C_DRV_MasterSendAddress(baseAddr, master, receive);

    /* Enable transmit/receive DMA requests */
    if (master->txSize > (uint32_t)0U)
    {
        (void)LPI2C_Set_MasterTxDMA(baseAddr, true);
    }
    else
    {
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, LPI2C_MASTER_COMMAND_RECEIVE, (uint8_t)(master->rxSize - 1U));
        (void)LPI2C_Set_MasterRxDMA(baseAddr, true);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveStartDmaTransfer
 * Description   : starts the DMA transfer for slave
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveStartDmaTransfer(uint32_t instance)
{
    LPI2C_Type *baseAddr = g_lpi2cBase[instance];
    const lpi2c_slave_state_t *slave = g_lpi2cSlaveStatePtr[instance];
    lpi2c_dma_transfer_params_t dmaTransParams;

    if (slave->txSize > (uint32_t)0U)
    {
        dmaTransParams.dmaChannel = slave->dmaChannel;
        dmaTransParams.dmaTransferType = EDMA_TRANSFER_MEM2PERIPH;
        dmaTransParams.i2cDataRegAddr = (uint32_t)(&(baseAddr->STDR));
        dmaTransParams.bufferTransfer = (uint8_t*)slave->txBuff;
        dmaTransParams.transferDirection = LPI2C_TX_REQ;
        dmaTransParams.transferSize = slave->txSize;
    }
    else
    {
        dmaTransParams.dmaChannel = slave->dmaChannel;
        dmaTransParams.dmaTransferType = EDMA_TRANSFER_PERIPH2MEM;
        dmaTransParams.i2cDataRegAddr = (uint32_t)(&(baseAddr->SRDR));
        dmaTransParams.bufferTransfer = slave->rxBuff;
        dmaTransParams.transferDirection = LPI2C_RX_REQ;
        dmaTransParams.transferSize = slave->rxSize;
    }

    (void)LPI2C_DRV_ConfigureDmaTransfer(instance, &dmaTransParams);
    /* Adjustment added to source address at the beginning of TX buffer */
    EDMA_DRV_SetSrcLastAddrAdjustment(dmaTransParams.dmaChannel, -(int32_t)(slave->txSize));

    /* Start channel */
    (void)EDMA_DRV_StartChannel(dmaTransParams.dmaChannel);

    /* Enable transmit/receive DMA requests */
    if (slave->txSize > (uint32_t)0U)
    {
        (void)LPI2C_Set_SlaveTxDMA(baseAddr, true);
    }
    else
    {
        (void)LPI2C_Set_SlaveRxDMA(baseAddr, true);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterCompleteDMATransfer
 * Description   : Finish up a transfer DMA for master. The main purpose of
 *                 this function is to create a function compatible with DMA
 *                 callback type
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterCompleteDMATransfer(void* parameter, edma_chn_status_t status)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t *master;

    uint32_t instance = (uint32_t)parameter;

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];

    if((master->txSize > 0U) && (status != EDMA_CHN_ERROR))
    {
        master->txSize = 0U;

        LPI2C_Set_MasterTxFIFOWatermark(baseAddr, 0U);

        /* Disable transmit data DMA requests */
        (void)LPI2C_Set_MasterTxDMA(baseAddr, false);

        /* Activate transmit data events */
        LPI2C_Set_MasterInt(baseAddr, (uint32_t)LPI2C_MASTER_TRANSMIT_DATA_INT, true);
    }
    else
    {
        /* Signal transfer end for blocking transfers */
        LPI2C_DRV_MasterEndTransfer(baseAddr, master, master->sendStop, false);

        if (master->blocking == true)
        {
            (void)OSIF_SemaPost(&(master->idleSemaphore));
        }

        /* Report status error if an error occurred in EDMA channel */
        if(status == EDMA_CHN_ERROR)
        {
            master->status = STATUS_ERROR;
        }
        else
        {
            master->status = STATUS_SUCCESS;
        }

        if (master->masterCallback != NULL)
        {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterReinit
 * Description   : re-initialize the I2C master
 *
 *END**************************************************************************/
static status_t LPI2C_DRV_MasterReinit(uint32_t instance, lpi2c_master_state_t * master)
{
    LPI2C_Type *baseAddr;
    lpi2c_baud_rate_params_t baudRate;

    baseAddr = g_lpi2cBase[instance];
    g_lpi2cMasterStatePtr[instance] = master;

    /* Re-initialize driver status structure */
    master->txBuff = NULL;
    master->txSize = 0;
    master->rxBuff = NULL;
    master->rxSize = 0;
    master->i2cIdle = true;

    LPI2C_DRV_MasterResetQueue(master);

    /* Initialize module */
    LPI2C_Init(baseAddr);

    /* Set baud rate */
    baudRate.baudRate = master->baudrate;
    #if(LPI2C_HAS_HIGH_SPEED_MODE)
        baudRate.baudRateHS = master->baudRateHS;
    #endif
    (void)LPI2C_DRV_MasterSetBaudRate(instance, master->operatingMode, baudRate);

     /* Set slave address */
     LPI2C_DRV_MasterSetSlaveAddr(instance, master->slaveAddress, master->is10bitAddr);

     /* Enable LPI2C master */
     LPI2C_Set_MasterEnable(baseAddr, true);

     return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t LPI2C_DRV_MasterWaitTransferEnd(uint32_t instance, uint32_t timeout)
{
    LPI2C_Type *baseAddr;
    status_t osifError = STATUS_SUCCESS;
    lpi2c_master_state_t *master;
    uint16_t rxFifoFill = 0;

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];

    /* Wait for transfer to be completed by the IRQ */
    osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);

    if (osifError == STATUS_TIMEOUT)
    {
        /* If master is sending data transfer is aborted now in case timeout occurred */
        if(master->txSize != 0U)
        {
            LPI2C_DRV_MasterEndTransfer(baseAddr, master, false, true);

            master->status = STATUS_TIMEOUT;
        }
        else
        {
            if(master->transferType == LPI2C_USING_DMA)
            {
                /* Stop DMA channel and activate interrupts */
                (void)EDMA_DRV_StopChannel(master->dmaChannel);
            }

            /* Disable interrupts to check number of bytes in rx fifo */
            LPI2C_Set_MasterInt(baseAddr, (uint32_t)LPI2C_MASTER_RECEIVE_DATA_INT, false);

            /* Check number of bytes in rx fifo */
            rxFifoFill = LPI2C_Get_MasterRxFIFOCount(baseAddr);

            /* In case both rx size and number of bytes in rx fifo is 0, then the transfer ended successfully */
            if((rxFifoFill == master->rxSize) && (master->rxSize ==  0U))
            {
                master->status = STATUS_SUCCESS;

                /* Blocking transfer is over */
                master->blocking = false;

                return master->status;
            }

            /* Set watermark to rxFifoFill in case the rx size is grater than the number of bytes in the rx  fifo */
            if(rxFifoFill < master->rxSize)
            {
                master->abortedTransfer = true;
                LPI2C_Set_MasterRxFIFOWatermark(baseAddr, rxFifoFill);

                master->status = STATUS_TIMEOUT;
            }

            LPI2C_Set_MasterInt(baseAddr, (uint32_t)LPI2C_MASTER_RECEIVE_DATA_INT, true);

            osifError = OSIF_SemaWait(&(master->idleSemaphore), timeout);
            if(osifError == STATUS_TIMEOUT)
            {
                (void)LPI2C_DRV_MasterReinit(instance, master);
                master->status = STATUS_TIMEOUT;
            }

            master->abortedTransfer = false;
        }
    }

    /* Blocking transfer is over */
    master->blocking = false;
    return master->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 *END**************************************************************************/
static status_t LPI2C_DRV_SlaveWaitTransferEnd(uint32_t instance, uint32_t timeout)
{
    status_t osifError = STATUS_SUCCESS;
    LPI2C_Type *baseAddr;
    lpi2c_slave_state_t *slave;

    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];

    /* Wait for transfer to be completed by the IRQ */
    osifError = OSIF_SemaWait(&(slave->idleSemaphore), timeout);

    if (osifError == STATUS_TIMEOUT)
    {
        LPI2C_DRV_SlaveEndTransfer(baseAddr, slave);
        slave->status = STATUS_TIMEOUT;
    }

    /* Blocking transfer is over */
    slave->blocking = false;
    return slave->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterHandleTransmitDataRequest
 * Description   : handle a transmit request for master
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterHandleTransmitDataRequest(LPI2C_Type *baseAddr, lpi2c_master_state_t *master)
{
    /* More data needed for transmission */
    if (!LPI2C_DRV_MasterCmdQueueEmpty(master))
    {
        /* If there are queued commands, send them */
        LPI2C_DRV_MasterSendQueuedCmd(baseAddr, master);
    }
    else if (master->txBuff != NULL)
    {
        /* A transmission is in progress */
        if (master->txSize == 0U)
        {
            /* There is no more data in buffer, the transmission is over */
            LPI2C_DRV_MasterEndTransfer(baseAddr, master, master->sendStop, false);

            /* Signal transfer end for blocking transfers */
            if (master->blocking == true)
            {
                (void)OSIF_SemaPost(&(master->idleSemaphore));
            }

            master->status = STATUS_SUCCESS;

            if (master->masterCallback != NULL)
            {
                master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
        }
        else
        {
            /* Queue data bytes to fill tx fifo */
            LPI2C_DRV_MasterQueueData(baseAddr, master);
        }
    }
    else
    {
        /* No more commands and no transmission in progress - disable tx event */
        LPI2C_Set_MasterInt(baseAddr, (uint32_t)LPI2C_MASTER_TRANSMIT_DATA_INT, false);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterHandleReceiveDataRequest
 * Description   : handle a receive request for master
 *
 *END**************************************************************************/
static void LPI2C_DRV_MasterHandleReceiveDataReadyEvent(LPI2C_Type *baseAddr, lpi2c_master_state_t *master)
{
    /* Received data ready */
    DEV_ASSERT(master->rxBuff != NULL);

    /* Transfer received data to user buffer */
    while ((LPI2C_Get_MasterRxFIFOCount(baseAddr) > 0U) && (master->rxSize > 0U))
    {
        master->rxBuff[0U] = LPI2C_Get_MasterRxData(baseAddr);
        master->rxBuff++;
        master->rxSize--;
    }
    if (master->rxSize == 0U)
    {
        /* Done receiving */
        LPI2C_DRV_MasterEndTransfer(baseAddr, master, master->sendStop, false);

        /* Signal transfer end for blocking transfers */
        if (master->blocking == true)
        {
            (void)OSIF_SemaPost(&(master->idleSemaphore));
        }

        master->status = STATUS_SUCCESS;

        if (master->masterCallback != NULL)
        {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
    else if (master->rxSize <= LPI2C_Get_MasterRxFIFOWatermark(baseAddr))
    {
        /* Reduce rx watermark to receive the last few bytes */
        LPI2C_Set_MasterRxFIFOWatermark(baseAddr, (uint16_t)(master->rxSize - 1U));
    }
    else
    {
        /* Continue reception */
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveHandleAddressValidEvent
 * Description   : handle an address valid event for slave
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveHandleAddressValidEvent(uint32_t instance, const LPI2C_Type *baseAddr, lpi2c_slave_state_t *slave)
{
    uint16_t receivedAddr;

    receivedAddr = LPI2C_Get_SlaveReceivedAddr(baseAddr);
    if ((receivedAddr & 1U) != (uint16_t)0U)
    {
        /* Request from master to transmit data */
        if ((slave->slaveCallback != NULL) && slave->slaveListening)
        {
            slave->slaveCallback(I2C_SLAVE_EVENT_TX_REQ, slave->callbackParam);
        }

#if defined(ERRATA_E10792)
        if (slave->transferType == LPI2C_USING_INTERRUPTS) {
            /* Enable interrupt for transmitting data */
            LPI2C_Set_SlaveInt(g_lpi2cBase[instance], (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, true);
        }
#endif

        slave->txUnderrunWarning = false;

        if ((slave->transferType == LPI2C_USING_DMA) && slave->slaveListening)
        {
            (void)LPI2C_DRV_SlaveStartDmaTransfer(instance);
        }

    }
    else
    {
        /* Request from master to receive data */
        if ((slave->slaveCallback != NULL) && slave->slaveListening)
        {
            slave->slaveCallback(I2C_SLAVE_EVENT_RX_REQ, slave->callbackParam);
        }

        if ((slave->transferType == LPI2C_USING_DMA) && slave->slaveListening)
        {
            (void)LPI2C_DRV_SlaveStartDmaTransfer(instance);
        }
    }

    slave->status = STATUS_BUSY;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveHandleTransmitDataEvent
 * Description   : handle a transmit data event for slave
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveHandleTransmitDataEvent(LPI2C_Type *baseAddr, lpi2c_slave_state_t *slave)
{
    if (slave->txUnderrunWarning == true)
    {
        /* Another Tx event after underflow warning means the dummy char was sent */
        slave->status = STATUS_I2C_TX_UNDERRUN;
    }

    if (slave->txSize == 0U)
    {
        /* Out of data, call callback to allow user to provide a new buffer */
        if (slave->slaveCallback != NULL)
        {
            slave->slaveCallback(I2C_SLAVE_EVENT_TX_EMPTY, slave->callbackParam);
        }
    }

    if (slave->txSize == 0U)
    {
        /*
         * Still no data, record tx underflow event and send dummy char.
         * Special case after the last tx byte: the device will ask for more data
         * but the dummy char will not be sent if NACK and then STOP condition are
         * received from master. So only record a "warning" for now.
         */
        slave->txUnderrunWarning = true;
        LPI2C_Transmit_SlaveData(baseAddr, (uint8_t)0xFFU);
    }
    else
    {
        LPI2C_Transmit_SlaveData(baseAddr, slave->txBuff[0U]);
        slave->txBuff++;
        slave->txSize--;
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveHandleReceiveDataEvent
 * Description   : handle a receive data event for slave
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveHandleReceiveDataEvent(const LPI2C_Type *baseAddr, lpi2c_slave_state_t *slave)
{
    if (slave->rxSize == 0U)
    {
        /* No more room for data, call callback to allow user to provide a new buffer */
        if (slave->slaveCallback != NULL)
        {
            slave->slaveCallback(I2C_SLAVE_EVENT_RX_FULL, slave->callbackParam);
        }
    }

    if (slave->rxSize == 0U)
    {
        /* Still no room for data, record rx overrun event and dummy read data */
        slave->status = STATUS_I2C_RX_OVERRUN;
        (void)LPI2C_Get_SlaveData(baseAddr);
    }
    else
    {
        slave->rxBuff[0U] = LPI2C_Get_SlaveData(baseAddr);
        slave->rxBuff++;
        slave->rxSize--;
    }
}

/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterInit
 * Description   : initialize the I2C master mode driver
 *
 * Implements : LPI2C_DRV_MasterInit_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterInit(uint32_t instance,
                                    const lpi2c_master_user_config_t * userConfigPtr,
                                    lpi2c_master_state_t * master)
{
    LPI2C_Type *baseAddr;
    status_t retVal;
    uint32_t inputClock;
    lpi2c_baud_rate_params_t baudRate;

    DEV_ASSERT(master != NULL);
    DEV_ASSERT(userConfigPtr != NULL);
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    /* Check to see if the LPI2C master instance is already initialized */
    DEV_ASSERT(g_lpi2cMasterStatePtr[instance] == NULL);

    /* Check the protocol clock frequency */
    retVal = CLOCK_SYS_GetFreq(g_lpi2cClock[instance], &inputClock);
    DEV_ASSERT(retVal == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    baseAddr = g_lpi2cBase[instance];
    g_lpi2cMasterStatePtr[instance] = master;

    /* Initialize driver status structure */
    master->rxBuff = NULL;
    master->rxSize = 0U;
    master->txBuff = NULL;
    master->txSize = 0U;
    master->status = STATUS_SUCCESS;
    master->i2cIdle = true;
    master->slaveAddress = userConfigPtr->slaveAddress;
    master->is10bitAddr = userConfigPtr->is10bitAddr;
    master->transferType = userConfigPtr->transferType;
    /* Store DMA channel number used in transfer */
    master->dmaChannel = userConfigPtr->dmaChannel;
    master->masterCallback = userConfigPtr->masterCallback;
    master->callbackParam = userConfigPtr->callbackParam;
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    master->masterCode = userConfigPtr->masterCode;
    master->highSpeedInProgress = false;
#endif
    master->blocking = false;
    master->baudrate = userConfigPtr->baudRate;
    master->abortedTransfer = false;

    /* Initialize the semaphore */
    retVal = OSIF_SemaCreate(&(master->idleSemaphore), 0);
    DEV_ASSERT(retVal == STATUS_SUCCESS);

    LPI2C_DRV_MasterResetQueue(master);

    /* Enable lpi2c interrupt */
    INT_SYS_EnableIRQ(g_lpi2cMasterIrqId[instance]);

    /* Initialize module */
    LPI2C_Init(baseAddr);

    /* Set baud rate */
    baudRate.baudRate = userConfigPtr->baudRate;
#if(LPI2C_HAS_HIGH_SPEED_MODE)
    master->baudRateHS = userConfigPtr->baudRateHS;
    baudRate.baudRateHS = userConfigPtr->baudRateHS;
#endif
    (void)LPI2C_DRV_MasterSetBaudRate(instance, userConfigPtr->operatingMode, baudRate);

    /* Set slave address */
    LPI2C_DRV_MasterSetSlaveAddr(instance, userConfigPtr->slaveAddress, userConfigPtr->is10bitAddr);

    /* Enable LPI2C master */
    LPI2C_Set_MasterEnable(baseAddr, true);
    
    (void)retVal;
    
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterDeinit
 * Description   : deinitialize the I2C master mode driver
 *
 * Implements : LPI2C_DRV_MasterDeinit_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterDeinit(uint32_t instance)
{
    LPI2C_Type *baseAddr;
    const lpi2c_master_state_t *master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Destroy the semaphore */
    (void)OSIF_SemaDestroy(&(master->idleSemaphore));

    g_lpi2cMasterStatePtr[instance] = NULL;

    /* Disable master */
    LPI2C_Set_MasterEnable(baseAddr, false);

    /* Disable i2c interrupt */
    INT_SYS_DisableIRQ(g_lpi2cMasterIrqId[instance]);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SetMasterBusIdleTimeout
 * Description   : Set bus idle timeout period for I2C master mode driver
 *
 * Implements : LPI2C_DRV_SetMasterBusIdleTimeout_Activity
 *END**************************************************************************/
void LPI2C_DRV_SetMasterBusIdleTimeout(uint32_t instance, uint16_t timeout)
{
    LPI2C_Type *baseAddr;
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    uint16_t timeout_temp = timeout;
    
    /* Limit timeout value since BUSIDLE field is 12-bit long */
    if(timeout_temp > BUSIDLE_MAX_VALUE)
    {
        timeout_temp = BUSIDLE_MAX_VALUE;
    }

    baseAddr = g_lpi2cBase[instance];
    
    /* Disable master */
    LPI2C_Set_MasterEnable(baseAddr, false);

    /*Set BUSIDLE value for MCFGR2*/
    LPI2C_Set_MasterBusIdleTimeout(baseAddr, timeout_temp);

    /* Ensable master */
    LPI2C_Set_MasterEnable(baseAddr, true);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterGetBaudRate
 * Description   : returns the currently configured baud rate
 *
 * Implements : LPI2C_DRV_MasterGetBaudRate_Activity
 *END**************************************************************************/
void LPI2C_DRV_MasterGetBaudRate(uint32_t instance, lpi2c_baud_rate_params_t *baudRate)
{
    const LPI2C_Type *baseAddr;
    const lpi2c_master_state_t *master;
    status_t retVal;
    uint32_t prescaler;
    uint32_t clkLo;
    uint32_t clkHi;
    uint32_t inputClock;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Get the protocol clock frequency */
    retVal = CLOCK_SYS_GetFreq(g_lpi2cClock[instance], &inputClock);
    DEV_ASSERT(retVal == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Ignoring the glitch filter, the baud rate formula is:
            SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2))
    */
    prescaler = (uint32_t)LPI2C_Get_MasterPrescaler(baseAddr);
    clkHi = (uint32_t)LPI2C_Get_MasterClockHighPeriod(baseAddr);
    clkLo = (uint32_t)LPI2C_Get_MasterClockLowPeriod(baseAddr);

    baudRate->baudRate = inputClock / (((uint32_t)1U << prescaler) * (clkLo + clkHi + (uint32_t)2U));

#if(LPI2C_HAS_HIGH_SPEED_MODE)
    if (master->operatingMode == LPI2C_HIGHSPEED_MODE)
    {
        clkHi = LPI2C_Get_MasterClockHighPeriodHS(baseAddr);
        clkLo = LPI2C_Get_MasterClockLowPeriodHS(baseAddr);

        baudRate->baudRateHS = inputClock / (((uint32_t)1U << prescaler) * (clkLo + clkHi + (uint32_t)2U));
    }
#endif

    (void)retVal;
    (void)master;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSetBaudRate
 * Description   : set the baud rate for any subsequent I2C communication
 *
 * Implements : LPI2C_DRV_MasterSetBaudRate_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterSetBaudRate(uint32_t instance,
                                const lpi2c_mode_t operatingMode,
                                const lpi2c_baud_rate_params_t baudRate)
{
    LPI2C_Type *baseAddr;
    const lpi2c_master_state_t * master;
    status_t retVal;
    uint32_t inputClock;
    uint32_t minPrescaler = 0U;
    uint32_t prescaler;
    uint32_t clkTotal;
    uint32_t clkLo;
    uint32_t clkHi;
    uint32_t setHold;
    uint32_t dataVd;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if(!master->i2cIdle)
    {
        return STATUS_BUSY;
    }

    /* Get the protocol clock frequency */
    retVal = CLOCK_SYS_GetFreq(g_lpi2cClock[instance], &inputClock);
    DEV_ASSERT(retVal == STATUS_SUCCESS);
    DEV_ASSERT(inputClock > 0U);

    /* Disable master */
    LPI2C_Set_MasterEnable(baseAddr, false);

    /* Ignoring the glitch filter, the baud rate formula is:
            SCL_freq = Input_freq / (2^PRESCALER * (CLKLO + CLKHI + 2))
            Assume CLKLO = CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2
    */

    if (baudRate.baudRate != 0U)
    {
        /* Compute minimum prescaler for which CLKLO and CLKHI values are in valid range. Always round up. */
        minPrescaler = ((inputClock - 1U) / ((baudRate.baudRate) * (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))) + (uint32_t)1U;
        for (prescaler = 0U; prescaler < 7U; prescaler++)
        {
            if (((uint32_t)1U << prescaler) >= minPrescaler)
            {
                break;
            }
        }

        /* Compute CLKLO and CLKHI values for this prescaler. Round to nearest integer. */
        clkTotal = (inputClock + ((baudRate.baudRate << prescaler) >> 1U)) / (baudRate.baudRate << prescaler);
    }
    else
    {
        prescaler = 7U;
        clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    if (clkTotal > (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))
    {
        clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
    }

    /*
     * If we try to compute clk high and low values using clkTotal equal with 0
     * (this is the case when the baudrate is 0), we will get negative values for
     * them, so we set them to 0 for this case.
     */
    if (clkTotal <= 1U)
    {
        clkHi = 0U;
        clkLo = 0U;
    }
    else
    {
        clkHi = (clkTotal - 2U) / 2U;
        clkLo = clkTotal - 2U - clkHi;
    }

    if (clkHi < CLKHI_MIN_VALUE)
    {
        clkHi = CLKHI_MIN_VALUE;
    }
    if (clkLo < CLKLO_MIN_VALUE)
    {
        clkLo = CLKLO_MIN_VALUE;
    }

    /* Compute DATAVD and SETHOLD */
    setHold = clkHi;
    dataVd = clkHi >> 1U;
    if (setHold < SETHOLD_MIN_VALUE)
    {
        setHold = SETHOLD_MIN_VALUE;
    }
    if (dataVd < DATAVD_MIN_VALUE)
    {
        dataVd = DATAVD_MIN_VALUE;
    }

    /* Apply settings */
    LPI2C_Set_MasterPrescaler(baseAddr, (lpi2c_master_prescaler_t)prescaler);
    LPI2C_Set_MasterDataValidDelay(baseAddr, (uint8_t)dataVd);
    LPI2C_Set_MasterSetupHoldDelay(baseAddr, (uint8_t)setHold);
    LPI2C_Set_MasterClockHighPeriod(baseAddr, (uint8_t)clkHi);
    LPI2C_Set_MasterClockLowPeriod(baseAddr, (uint8_t)clkLo);

#if(LPI2C_HAS_HIGH_SPEED_MODE)
    if (operatingMode == LPI2C_HIGHSPEED_MODE)
    {
        /* Compute settings for High-speed baud rate */
        /* Compute High-speed CLKLO and CLKHI values for the same prescaler. Round to nearest integer. */
        clkTotal = (inputClock + ((baudRate.baudRateHS << prescaler) >> 1U)) / (baudRate.baudRateHS << prescaler);
        if (clkTotal > (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U))
        {
            clkTotal = (CLKHI_MAX_VALUE + CLKLO_MAX_VALUE + 2U);
        }

        clkHi = (clkTotal - 2U) / 3U;
        clkLo = clkTotal - 2U - clkHi;
        if (clkHi < CLKHI_MIN_VALUE)
        {
            clkHi = CLKHI_MIN_VALUE;
        }
        if (clkLo < CLKLO_MIN_VALUE)
        {
            clkLo = CLKLO_MIN_VALUE;
        }

        /* Compute High-speed DATAVD and SETHOLD */
        setHold = clkHi;
        dataVd = clkHi >> 1U;
        if (setHold < SETHOLD_MIN_VALUE)
        {
            setHold = SETHOLD_MIN_VALUE;
        }
        if (dataVd < DATAVD_MIN_VALUE)
        {
            dataVd = DATAVD_MIN_VALUE;
        }

        /* Apply High-speed settings */
        LPI2C_Set_MasterDataValidDelayHS(baseAddr, (uint8_t)dataVd);
        LPI2C_Set_MasterSetupHoldDelayHS(baseAddr, (uint8_t)setHold);
        LPI2C_Set_MasterClockHighPeriodHS(baseAddr, (uint8_t)clkHi);
        LPI2C_Set_MasterClockLowPeriodHS(baseAddr, (uint8_t)clkLo);
    }
#endif

    /* Perform other settings related to the chosen operating mode */
    LPI2C_DRV_MasterSetOperatingMode(instance, operatingMode);

    /* Re-enable master */
    LPI2C_Set_MasterEnable(baseAddr, true);
    
    (void) minPrescaler;
    (void)master;
    (void)retVal;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSetSlaveAddr
 * Description   : set the slave address for any subsequent I2C communication
 *
 * Implements : LPI2C_DRV_MasterSetSlaveAddr_Activity
 *END**************************************************************************/
void LPI2C_DRV_MasterSetSlaveAddr(uint32_t instance, const uint16_t address, const bool is10bitAddr)
{
    lpi2c_master_state_t * master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    master->slaveAddress = address;
    master->is10bitAddr = is10bitAddr;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSendData
 * Description   : perform a non-blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterSendData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterSendData(uint32_t instance,
                                            const uint8_t * txBuff,
                                            uint32_t txSize,
                                            bool sendStop)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t *master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if(!master->i2cIdle)
    {
        return STATUS_BUSY;
    }

    /* Copy parameters to driver state structure */
    master->txBuff = txBuff;
    master->txSize = txSize;
    master->sendStop = sendStop;
    master->i2cIdle = false;
    master->status = STATUS_BUSY;

    if (master->transferType == LPI2C_USING_DMA)
    {
        LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                         LPI2C_MASTER_ARBITRATION_LOST_INT |
                                         LPI2C_MASTER_NACK_DETECT_INT,
                               true);

        LPI2C_DRV_MasterStartDmaTransfer(instance);
    }
    else
    {
        /* Initiate communication */
        LPI2C_DRV_MasterSendAddress(baseAddr, master, false);

        /* Queue data bytes to fill tx fifo */
        LPI2C_DRV_MasterQueueData(baseAddr, master);

        /* Set tx FIFO watermark */
        LPI2C_Set_MasterTxFIFOWatermark(baseAddr, 0U);

        /* Enable relevant events */
#if(LPI2C_HAS_ULTRA_FAST_MODE)
        if (master->operatingMode == LPI2C_ULTRAFAST_MODE)
        {
            /* Do not enable NACK event reporting in ultra-fast mode */
            LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                             LPI2C_MASTER_ARBITRATION_LOST_INT |
                                             LPI2C_MASTER_TRANSMIT_DATA_INT,
                                   true);
        }
        else
#endif
        {
            LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                             LPI2C_MASTER_ARBITRATION_LOST_INT |
                                             LPI2C_MASTER_NACK_DETECT_INT |
                                             LPI2C_MASTER_TRANSMIT_DATA_INT,
                                   true);
        }
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterSendDataBlocking
 * Description   : perform a blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterSendDataBlocking_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterSendDataBlocking(uint32_t instance,
                                    const uint8_t * txBuff,
                                    uint32_t txSize,
                                    bool sendStop,
                                    uint32_t timeout)
{

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    lpi2c_master_state_t *master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if(!master->i2cIdle)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)OSIF_SemaWait(&(master->idleSemaphore), 0);

    (void)LPI2C_DRV_MasterSendData(instance, txBuff, txSize, sendStop);

    /* Wait for transfer to end */
    return LPI2C_DRV_MasterWaitTransferEnd(instance, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterAbortTransferData
 * Description   : abort a non-blocking I2C Master transmission or reception
 *
 * Implements : LPI2C_DRV_MasterAbortTransferData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterAbortTransferData(uint32_t instance)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t * master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    if (master->rxBuff != NULL)
    {
        /* Aborting a reception not supported because hardware will continue the
           current command even if the FIFO is reset and this could last indefinitely */
        return STATUS_UNSUPPORTED;
    }

    /* End transfer: force stop generation, reset FIFOs */
    master->status = STATUS_I2C_ABORTED;
    LPI2C_DRV_MasterEndTransfer(baseAddr, master, true, true);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterReceiveData
 * Description   : perform a non-blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterReceiveData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterReceiveData(uint32_t  instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize,
                                       bool sendStop)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t * master;
    uint16_t rxBytes;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    DEV_ASSERT(rxSize <= 256U);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if(!master->i2cIdle)
    {
        return STATUS_BUSY;
    }

#if(LPI2C_HAS_ULTRA_FAST_MODE)
    if (master->operatingMode == LPI2C_ULTRAFAST_MODE)
    {
        /* No reception possible in ultra-fast mode */
        return STATUS_ERROR;
    }
#endif

    /* Copy parameters to driver state structure */
    master->rxSize = rxSize;
    master->i2cIdle = false;
    master->sendStop = sendStop;
    master->rxBuff = rxBuff;
    master->status = STATUS_BUSY;

    if (master->transferType == LPI2C_USING_DMA)
    {
        LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                         LPI2C_MASTER_ARBITRATION_LOST_INT |
                                         LPI2C_MASTER_NACK_DETECT_INT,
                               true);

        LPI2C_DRV_MasterStartDmaTransfer(instance);
    }
    else
    {
        /* Initiate communication */
        LPI2C_DRV_MasterSendAddress(baseAddr, master, true);
        /* Queue receive command for rxSize bytes */
        LPI2C_DRV_MasterQueueCmd(baseAddr, master, LPI2C_MASTER_COMMAND_RECEIVE, (uint8_t)(rxSize - 1U));

        /* Set rx FIFO watermark */
        rxBytes = LPI2C_Get_MasterRxFIFOSize(baseAddr);
        if (rxBytes > rxSize)
        {
            rxBytes = (uint8_t)rxSize;
        }
        LPI2C_Set_MasterRxFIFOWatermark(baseAddr, (uint16_t)(rxBytes - 1U));

        /* Enable relevant events */
        if (!LPI2C_DRV_MasterCmdQueueEmpty(master))
        {
            /* Enable tx event too if there are commands in the software FIFO */
            LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                             LPI2C_MASTER_ARBITRATION_LOST_INT |
                                             LPI2C_MASTER_NACK_DETECT_INT |
                                             LPI2C_MASTER_TRANSMIT_DATA_INT |
                                             LPI2C_MASTER_RECEIVE_DATA_INT,
                                   true);
        }
        else
        {
            LPI2C_Set_MasterInt(baseAddr, LPI2C_MASTER_FIFO_ERROR_INT |
                                             LPI2C_MASTER_ARBITRATION_LOST_INT |
                                             LPI2C_MASTER_NACK_DETECT_INT |
                                             LPI2C_MASTER_RECEIVE_DATA_INT,
                                   true);
        }
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterReceiveDataBlocking
 * Description   : perform a blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_MasterReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterReceiveDataBlocking(uint32_t instance,
                                           uint8_t * rxBuff,
                                           uint32_t rxSize,
                                           bool sendStop,
                                           uint32_t timeout)
{
    status_t retVal = STATUS_SUCCESS;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    lpi2c_master_state_t *master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check if driver is busy */
    if(!master->i2cIdle)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    master->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)OSIF_SemaWait(&(master->idleSemaphore), 0);

    retVal = LPI2C_DRV_MasterReceiveData(instance, rxBuff, rxSize, sendStop);

#if(LPI2C_HAS_ULTRA_FAST_MODE)
    if (retVal != STATUS_SUCCESS)
    {
        master->blocking = false;
        return retVal;
    }
#endif

    (void) retVal;

    /* Wait for transfer to end */
    return LPI2C_DRV_MasterWaitTransferEnd(instance, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterGetTransferStatus
 * Description   : return the current status of the I2C master transfer
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 * Implements : LPI2C_DRV_MasterGetTransferStatus_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_MasterGetTransferStatus(uint32_t instance,
                                                uint32_t *bytesRemaining)
{
    const LPI2C_Type *baseAddr;
    const lpi2c_master_state_t * master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    if ((bytesRemaining != NULL) && (master->transferType == LPI2C_USING_INTERRUPTS))
    {
        if (master->txSize > 0U)
        {
            /* Send data */
            /* Remaining bytes = bytes in buffer + bytes in tx FIFO */
            *bytesRemaining = master->txSize + (uint32_t)LPI2C_Get_MasterTxFIFOCount(baseAddr);
        }
        else if (master->rxSize > 0U)
        {
            /* Receive data */
            /* Remaining bytes = free space in buffer - bytes in rx FIFO */
            *bytesRemaining = master->rxSize - (uint32_t)LPI2C_Get_MasterRxFIFOCount(baseAddr);
        }
        else
        {
            *bytesRemaining = 0U;
        }
    }

    return master->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterGetDefaultConfig
 * Description   : Gets the default configuration structure for master
 *
 * Implements : LPI2C_DRV_MasterGetDefaultConfig_Activity
 *END**************************************************************************/
void LPI2C_DRV_MasterGetDefaultConfig(lpi2c_master_user_config_t * config)
{
      config->slaveAddress = 32U;
      config->is10bitAddr = false;
      config->operatingMode = LPI2C_STANDARD_MODE;
      config->baudRate = 100000U;
      config->transferType = LPI2C_USING_INTERRUPTS;
      config->dmaChannel = 0U;
      config->masterCallback = NULL;
      config->callbackParam = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_MasterIRQHandler
 * Description   : handle non-blocking master operation when I2C interrupt occurs
 *
 *END**************************************************************************/
void LPI2C_DRV_MasterIRQHandler(uint32_t instance)
{
    LPI2C_Type *baseAddr;
    lpi2c_master_state_t * master;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    master = g_lpi2cMasterStatePtr[instance];
    DEV_ASSERT(master != NULL);

    /* Check which event caused the interrupt */
    if (LPI2C_Get_MasterTransmitDataRequestEvent(baseAddr))
    {
        LPI2C_DRV_MasterHandleTransmitDataRequest(baseAddr, master);
    }

    if ((LPI2C_Get_MasterReceiveDataReadyEvent(baseAddr)))
    {
        if(master->abortedTransfer)
        {
             /* Signal transfer end for blocking transfers */
             if (master->blocking == true)
             {
                 (void)OSIF_SemaPost(&(master->idleSemaphore));
             }

             master->status = STATUS_TIMEOUT;

            (void)LPI2C_DRV_MasterReinit(instance, master);

        }
        else
        {
             LPI2C_DRV_MasterHandleReceiveDataReadyEvent(baseAddr, master);
        }

    }

    if (LPI2C_Get_MasterFIFOErrorEvent(baseAddr))
    {
        /* FIFO error */
        LPI2C_Clear_MasterFIFOErrorEvent(baseAddr);

#if(LPI2C_HAS_HIGH_SPEED_MODE)
        /* High-speed transfers end at STOP condition */
        master->highSpeedInProgress = false;
#endif
        master->status = STATUS_ERROR;

        /* End transfer: no stop generation (the module will handle that by itself
           if needed), reset FIFOs */
        LPI2C_DRV_MasterEndTransfer(baseAddr, master, false, true);

        /* Signal transfer end for blocking transfers */
        if (master->blocking == true)
        {
            (void)OSIF_SemaPost(&(master->idleSemaphore));
        }

        if (master->masterCallback != NULL)
        {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }

    if (LPI2C_Get_MasterArbitrationLostEvent(baseAddr))
    {
        /* Arbitration lost */
        LPI2C_Clear_MasterArbitrationLostEvent(baseAddr);

        /* End transfer: no stop generation (the module will handle that by itself
           if needed), reset FIFOs */
        LPI2C_DRV_MasterEndTransfer(baseAddr, master, false, true);

        /* Signal transfer end for blocking transfers */
        if (master->blocking == true)
        {
            (void)OSIF_SemaPost(&(master->idleSemaphore));
        }

        master->status = STATUS_I2C_ARBITRATION_LOST;

        if (master->masterCallback != NULL)
        {
            master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
    }

    if (LPI2C_Get_MasterNACKDetectEvent(baseAddr))
    {
        /* Received NACK */

#if(LPI2C_HAS_ULTRA_FAST_MODE)
        /* Ignore NACK in Ultra Fast mode */
        if (master->operatingMode != LPI2C_ULTRAFAST_MODE)
        {
#endif
            /* Signal transfer end for blocking transfers */
            if (master->blocking == true)
            {
                (void)OSIF_SemaPost(&(master->idleSemaphore));
            }

#if(LPI2C_HAS_HIGH_SPEED_MODE)
            /* High-speed transfers end at STOP condition */
            master->highSpeedInProgress = false;
#endif
            master->status = STATUS_I2C_RECEIVED_NACK;

            /* End transfer: no stop generation (the module will handle that by itself
               if needed), reset FIFOs */
            LPI2C_DRV_MasterEndTransfer(baseAddr, master, false, true);

            if (master->masterCallback != NULL)
            {
                master->masterCallback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
            
            /* Clear NACK flag */
            LPI2C_Clear_MasterNACKDetectEvent(baseAddr);
#if(LPI2C_HAS_ULTRA_FAST_MODE)
        }
#endif
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveInit
 * Description   : initialize the I2C slave mode driver
 *
 * Implements : LPI2C_DRV_SlaveInit_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveInit(uint32_t instance,
                                   const lpi2c_slave_user_config_t * userConfigPtr,
                                   lpi2c_slave_state_t * slave)
{
    LPI2C_Type *baseAddr;
    status_t retVal;
    uint32_t inputClock;

    DEV_ASSERT(slave != NULL);
    DEV_ASSERT(userConfigPtr != NULL);
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    DEV_ASSERT(g_lpi2cSlaveStatePtr[instance] == NULL);

    /*
     * Check the protocol clock frequency.
     * LPI2C slave remains operational, even when the LPI2C functional
     * clock is disabled, so we don't need to check if inputClock is 0.
     */
    retVal = CLOCK_SYS_GetFreq(g_lpi2cClock[instance], &inputClock);
    DEV_ASSERT(retVal == STATUS_SUCCESS);

    baseAddr = g_lpi2cBase[instance];
    g_lpi2cSlaveStatePtr[instance] = slave;

    /* Initialize driver status structure */
    slave->status = STATUS_SUCCESS;
    slave->slaveListening = userConfigPtr->slaveListening;
    slave->slaveCallback = userConfigPtr->slaveCallback;
    slave->callbackParam = userConfigPtr->callbackParam;
    slave->txBuff = NULL;
    slave->rxBuff = NULL;
    slave->txSize = 0U;
    slave->rxSize = 0U;
    slave->transferType = userConfigPtr->transferType;
    /* Store DMA channel number used in transfer */
    slave->dmaChannel = userConfigPtr->dmaChannel;
    slave->isTransferInProgress = false;
    slave->blocking = false;
    slave->is10bitAddress = userConfigPtr->is10bitAddr;
    slave->repeatedStarts = 0U;

    /* Initialize the semaphore */
    retVal = OSIF_SemaCreate(&(slave->idleSemaphore), 0);
    DEV_ASSERT(retVal == STATUS_SUCCESS);

    /* Enable lpi2c interrupt */
    INT_SYS_EnableIRQ(g_lpi2cSlaveIrqId[instance]);

    /* Initialize module */
    LPI2C_Init(baseAddr);

    /* Configure slave address */
    LPI2C_Set_SlaveAddr0(baseAddr, userConfigPtr->slaveAddress);
    if (userConfigPtr->is10bitAddr)
    {
        LPI2C_Set_SlaveAddrConfig(baseAddr, LPI2C_SLAVE_ADDR_MATCH_0_10BIT);
    }
    else
    {
        LPI2C_Set_SlaveAddrConfig(baseAddr, LPI2C_SLAVE_ADDR_MATCH_0_7BIT);
    }

    /* Configure operating mode */
    LPI2C_DRV_SlaveSetOperatingMode(instance, userConfigPtr->operatingMode);

    if (userConfigPtr->slaveListening)
    {
        if (slave->transferType == LPI2C_USING_DMA)
        {
            /* Activate events */
            LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                            LPI2C_SLAVE_FIFO_ERROR_INT |
                                            LPI2C_SLAVE_STOP_DETECT_INT |
                                            LPI2C_SLAVE_REPEATED_START_INT |
                                            LPI2C_SLAVE_ADDRESS_VALID_INT,
                                  true);
        }
        if (slave->transferType == LPI2C_USING_INTERRUPTS)
        {
            /* Activate events */
#if defined(ERRATA_E10792)
            LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                            LPI2C_SLAVE_FIFO_ERROR_INT |
                                            LPI2C_SLAVE_STOP_DETECT_INT |
                                            LPI2C_SLAVE_REPEATED_START_INT |
                                            LPI2C_SLAVE_ADDRESS_VALID_INT |
                                            LPI2C_SLAVE_RECEIVE_DATA_INT,
                                   true);

#else
            LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                            LPI2C_SLAVE_FIFO_ERROR_INT |
                                            LPI2C_SLAVE_STOP_DETECT_INT |
                                            LPI2C_SLAVE_REPEATED_START_INT |
                                            LPI2C_SLAVE_ADDRESS_VALID_INT |
                                            LPI2C_SLAVE_RECEIVE_DATA_INT |
                                            LPI2C_SLAVE_TRANSMIT_DATA_INT,
                                  true);

#endif

        }

        /* Enable LPI2C slave */
        LPI2C_Set_SlaveEnable(baseAddr, true);
    }
    
    (void)retVal;
   
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveDeinit
 * Description   : de-initialize the I2C slave mode driver
 *
 * Implements : LPI2C_DRV_SlaveDeinit_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveDeinit(uint32_t instance)
{
    LPI2C_Type *baseAddr;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    const lpi2c_slave_state_t *slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    /* Destroy the semaphore */
    (void)OSIF_SemaDestroy(&(slave->idleSemaphore));

    if ((slave->transferType == LPI2C_USING_DMA) && (slave->slaveListening))
    {
        /* Disable LPI2C DMA requests. */
        (void)LPI2C_Set_SlaveRxDMA(baseAddr, false);
        (void)LPI2C_Set_SlaveTxDMA(baseAddr, false);
    }

    g_lpi2cSlaveStatePtr[instance] = NULL;

    /* Disable LPI2C slave */
    LPI2C_Set_SlaveEnable(baseAddr, false);

    /* Disable i2c interrupt */
    INT_SYS_DisableIRQ(g_lpi2cSlaveIrqId[instance]);

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveSetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 *
 * Implements : LPI2C_DRV_SlaveSetTxBuffer_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveSetTxBuffer(uint32_t instance,
                                                 const uint8_t * txBuff,
                                                 uint32_t txSize)
{
    lpi2c_slave_state_t * slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    slave->txBuff = txBuff;
    slave->txSize = txSize;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveSetRxBuffer
 * Description   : Provide a buffer for receiving data.
 *
 * Implements : LPI2C_DRV_SlaveSetRxBuffer_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveSetRxBuffer(uint32_t instance,
                                                 uint8_t * rxBuff,
                                                 uint32_t  rxSize)
{
    lpi2c_slave_state_t * slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    slave->rxBuff = rxBuff;
    slave->rxSize = rxSize;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveSendData
 * Description   : perform a non-blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveSendData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveSendData(uint32_t instance,
                                   const uint8_t * txBuff,
                                   uint32_t txSize)
{
    LPI2C_Type *baseAddr;
    lpi2c_slave_state_t * slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);


    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    /* If the slave is in listening mode the user should not use this function or the blocking counterpart. */
    DEV_ASSERT(slave->slaveListening == false);

    /* Check if slave is busy */
    if(slave->isTransferInProgress)
    {
        return STATUS_BUSY;
    }

    slave->txBuff = txBuff;
    slave->txSize = txSize;
    slave->status = STATUS_BUSY;

    if (slave->transferType == LPI2C_USING_DMA)
    {
        /* Activate events */
        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                        LPI2C_SLAVE_FIFO_ERROR_INT |
                                        LPI2C_SLAVE_STOP_DETECT_INT |
                                        LPI2C_SLAVE_REPEATED_START_INT |
                                        LPI2C_SLAVE_ADDRESS_VALID_INT,
                              true);

        /* Enable LPI2C slave */
        LPI2C_Set_SlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;

        LPI2C_DRV_SlaveStartDmaTransfer(instance);
    }
    else
    {
        /* Activate events */
#if defined(ERRATA_E10792)

        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                        LPI2C_SLAVE_FIFO_ERROR_INT |
                                        LPI2C_SLAVE_STOP_DETECT_INT |
                                        LPI2C_SLAVE_REPEATED_START_INT |
                                        LPI2C_SLAVE_ADDRESS_VALID_INT,
                              true);

#else
        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                        LPI2C_SLAVE_FIFO_ERROR_INT |
                                        LPI2C_SLAVE_STOP_DETECT_INT |
                                        LPI2C_SLAVE_REPEATED_START_INT |
                                        LPI2C_SLAVE_ADDRESS_VALID_INT |
                                        LPI2C_SLAVE_TRANSMIT_DATA_INT,
                              true);
#endif


        /* Enable LPI2C slave */
        LPI2C_Set_SlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveSendDataBlocking
 * Description   : perform a blocking send transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveSendDataBlocking_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveSendDataBlocking(uint32_t    instance,
                                           const uint8_t *  txBuff,
                                           uint32_t txSize,
                                           uint32_t timeout)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(txSize > 0U);

    lpi2c_slave_state_t *slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    /* Check if slave is busy */
    if(slave->isTransferInProgress)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)OSIF_SemaWait(&(slave->idleSemaphore), 0);

    (void)LPI2C_DRV_SlaveSendData(instance, txBuff, txSize);

    /* Wait for transfer to end */
    return LPI2C_DRV_SlaveWaitTransferEnd(instance, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveReceiveData
 * Description   : perform a non-blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveReceiveData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveReceiveData(uint32_t   instance,
                                       uint8_t * rxBuff,
                                       uint32_t  rxSize)
{
    LPI2C_Type *baseAddr;
    lpi2c_slave_state_t * slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    /* If the slave is in listening mode the user should not use this function or the blocking counterpart. */
    DEV_ASSERT(slave->slaveListening == false);

    /* Check if slave is busy */
    if(slave->isTransferInProgress)
    {
        return STATUS_BUSY;
    }

    slave->rxBuff = rxBuff;
    slave->rxSize = rxSize;
    slave->status = STATUS_BUSY;

    if (slave->transferType == LPI2C_USING_DMA)
    {
        /* Activate events */
        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                        LPI2C_SLAVE_FIFO_ERROR_INT |
                                        LPI2C_SLAVE_STOP_DETECT_INT |
                                        LPI2C_SLAVE_REPEATED_START_INT |
                                        LPI2C_SLAVE_ADDRESS_VALID_INT,
                              true);

        /* Enable LPI2C slave */
        LPI2C_Set_SlaveEnable(baseAddr, true);

        slave->isTransferInProgress = true;

        LPI2C_DRV_SlaveStartDmaTransfer(instance);
    }
    else
    {
        slave->isTransferInProgress = true;

        /* Activate events */
        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_BIT_ERROR_INT |
                                        LPI2C_SLAVE_FIFO_ERROR_INT |
                                        LPI2C_SLAVE_STOP_DETECT_INT |
                                        LPI2C_SLAVE_REPEATED_START_INT |
                                        LPI2C_SLAVE_ADDRESS_VALID_INT |
                                        LPI2C_SLAVE_RECEIVE_DATA_INT,
                              true);

        /* Enable LPI2C slave */
        LPI2C_Set_SlaveEnable(baseAddr, true);
    }

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveReceiveDataBlocking
 * Description   : perform a blocking receive transaction on the I2C bus
 *
 * Implements : LPI2C_DRV_SlaveReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveReceiveDataBlocking(uint32_t instance,
                                               uint8_t  *rxBuff,
                                               uint32_t rxSize,
                                               uint32_t timeout)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(rxSize > 0U);

    lpi2c_slave_state_t *slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    /* Check if slave is busy */
    if(slave->isTransferInProgress)
    {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    slave->blocking = true;

    /* Dummy wait to ensure the semaphore is 0, no need to check result */
    (void)OSIF_SemaWait(&(slave->idleSemaphore), 0);

    (void)LPI2C_DRV_SlaveReceiveData(instance, rxBuff, rxSize);

    /* Wait for transfer to end */
    return LPI2C_DRV_SlaveWaitTransferEnd(instance, timeout);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveGetTransferStatus
 * Description   : return the current status of the I2C slave transfer
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function
 * to ascertain the state of the current transfer. In addition, if the transfer is still
 * in progress, the user can get the number of words that should be receive.
 *
 * Implements : LPI2C_DRV_SlaveGetTransferStatus_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveGetTransferStatus(uint32_t instance,
                                               uint32_t *bytesRemaining)
{
    const lpi2c_slave_state_t *slave;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    if ((bytesRemaining != NULL) && (slave->transferType == LPI2C_USING_INTERRUPTS))
    {
        if (slave->txSize > 0U)
        {
            /* Send data */
            *bytesRemaining = slave->txSize;
        }
        else if (slave->rxSize > 0U)
        {
            /* Receive data */
            *bytesRemaining = slave->rxSize;
        }
        else
        {
            *bytesRemaining = 0U;
        }
    }

    return slave->status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveAbortTransferData
 * Description   : abort a non-blocking I2C Master transmission or reception
 *
 * Implements : LPI2C_DRV_SlaveAbortTransferData_Activity
 *END**************************************************************************/
status_t LPI2C_DRV_SlaveAbortTransferData(uint32_t instance)
{
    lpi2c_slave_state_t * slave;
    LPI2C_Type *baseAddr;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);

    if (!slave->slaveListening)
    {
        slave->status = STATUS_I2C_ABORTED;
        LPI2C_DRV_SlaveEndTransfer(baseAddr, slave);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveGetDefaultConfig
 * Description   : Gets the default configuration structure for slave
 *
 * Implements : LPI2C_DRV_SlaveGetDefaultConfig_Activity
 *END**************************************************************************/
void LPI2C_DRV_SlaveGetDefaultConfig(lpi2c_slave_user_config_t * config)
{
      config->slaveAddress = 32U;
      config->is10bitAddr = false;
      config->slaveListening = true;
      config->operatingMode = LPI2C_STANDARD_MODE;
      config->transferType = LPI2C_USING_INTERRUPTS;
      config->dmaChannel = 0U;
      config->slaveCallback = NULL;
      config->callbackParam = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveEndTransferHandler
 * Description   : handle slave end transfer operations
 *
 *END**************************************************************************/
static void LPI2C_DRV_SlaveEndTransferHandler(lpi2c_slave_state_t *slave, LPI2C_Type *baseAddr)
{
    /* Check slave state */
    DEV_ASSERT(slave != NULL);

    /* Stop DMA channel if slave is transferring data in DMA mode */
    if (slave->transferType == LPI2C_USING_DMA)
    {
        (void)EDMA_DRV_StopChannel(slave->dmaChannel);
    }

    if (!slave->slaveListening)
    {

        LPI2C_DRV_SlaveEndTransfer(baseAddr, slave);

        /* Signal transfer end for blocking transfers */
        if (slave->blocking == true)
        {
            (void)OSIF_SemaPost(&(slave->idleSemaphore));
        }
    }

    if(slave->slaveCallback != NULL)
    {
        slave->slaveCallback(I2C_SLAVE_EVENT_STOP, slave->callbackParam);
    }

}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_DRV_SlaveIRQHandler
 * Description   : handle non-blocking slave operation when I2C interrupt occurs
 *
 *END**************************************************************************/
void LPI2C_DRV_SlaveIRQHandler(uint32_t instance)
{
    LPI2C_Type *baseAddr;
    lpi2c_slave_state_t * slave;
    bool stopDetect = false, repeatStartDetect = false;

    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    baseAddr = g_lpi2cBase[instance];
    slave = g_lpi2cSlaveStatePtr[instance];
    DEV_ASSERT(slave != NULL);
    
    /* Check for error event */
    if (LPI2C_Get_SlaveBitErrorEvent(baseAddr))
    {
        slave->status = STATUS_ERROR;
        LPI2C_Clear_SlaveBitErrorEvent(baseAddr);

#if defined(ERRATA_E10792)
        /* Deactivate interrupts for transmitting data */
        LPI2C_Set_SlaveInt(baseAddr, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
#endif

        LPI2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
    }
#if(LPI2C_HAS_ULTRA_FAST_MODE)
    else if (LPI2C_Get_SlaveFIFOErrorEvent(baseAddr))
    {
        /* In Ultra-Fast mode clock stretching is disabled, so it is possible to get
        this event if the slave can't keep up */
        slave->status = STATUS_I2C_RX_OVERRUN;
        LPI2C_Clear_SlaveFIFOErrorEvent(baseAddr);

#if defined(ERRATA_E10792)
        /* Deactivate interrupts for transmitting data */
        LPI2C_Set_SlaveInt(baseAddr, LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
#endif

        LPI2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
    }
#endif
    else
    {
        /* Check for repeated start or stop condition */
        stopDetect =  LPI2C_Get_SlaveSTOPDetectEvent(baseAddr);
        repeatStartDetect = LPI2C_Get_SlaveRepeatedStartEvent(baseAddr);

        if (repeatStartDetect)
        {
            slave->repeatedStarts++;

            if ((slave->repeatedStarts == 1U) && (slave->is10bitAddress))
            {
                repeatStartDetect = false;
                LPI2C_Clear_SlaveRepeatedStartEvent(baseAddr);
            }
        }

        if ((stopDetect == true) || (repeatStartDetect == true))
        {
            /* Either STOP or repeated START have the same meaning here: the current transfer is over */
            LPI2C_Clear_SlaveSTOPDetectEvent(baseAddr);
            LPI2C_Clear_SlaveRepeatedStartEvent(baseAddr);

    #if defined(ERRATA_E10792)
            /* Deactivate interrupts for transmitting data */
            LPI2C_Set_SlaveInt(baseAddr, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT, false);
    #endif

            if (slave->status == STATUS_BUSY)
            {
                /* Report success if no error was recorded */
                slave->status = STATUS_SUCCESS;
            }

            LPI2C_DRV_SlaveEndTransferHandler(slave, baseAddr);
        }
        else
        {
            /* Check which event caused the interrupt */
            if (LPI2C_Get_SlaveAddressValidEvent(baseAddr))
            {
                LPI2C_DRV_SlaveHandleAddressValidEvent(instance, baseAddr, slave);
            }

            if (LPI2C_Get_SlaveTransmitDataEvent(baseAddr))
            {
                if (LPI2C_Get_SlaveInt(baseAddr, (uint32_t)LPI2C_SLAVE_TRANSMIT_DATA_INT))
                {
                    LPI2C_DRV_SlaveHandleTransmitDataEvent(baseAddr, slave);
                }
            }
            
            if (LPI2C_Get_SlaveReceiveDataEvent(baseAddr))
            {
                if (LPI2C_Get_SlaveInt(baseAddr, (uint32_t)LPI2C_SLAVE_RECEIVE_DATA_INT))
                {
                    LPI2C_DRV_SlaveHandleReceiveDataEvent(baseAddr, slave);
                }
            }
        }
    }
}


#if defined(S32K11x_SERIES)
void LPI2C_DRV_ModuleIRQHandler(uint32_t instance)
{
    DEV_ASSERT(instance < LPI2C_INSTANCE_COUNT);

    /* Check if module is master or slave */
    if (g_lpi2cSlaveStatePtr[instance] == NULL)
    {
        LPI2C_DRV_MasterIRQHandler(instance);
    }
    else
    {
        LPI2C_DRV_SlaveIRQHandler(instance);
    }

}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
