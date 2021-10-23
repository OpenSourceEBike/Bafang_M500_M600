/*
 * Copyright 2017 NXP
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

#ifndef QUADSPI_DRIVER_H
#define QUADSPI_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "status.h"

/*!
 * @ingroup qspi_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/

/*! @brief Number of AHB buffers in the device
 */
#define QSPI_AHB_BUFFERS   4U

/*! @brief Key to lock/unlock LUT
 */
#define QSPI_LUT_LOCK_KEY       0x5AF05AF0U

/*! @brief Lut commands 
 * Implements : qspi_lut_commands_t_Class
 */
typedef enum
{
    QSPI_LUT_CMD_STOP            = 0U,    /*!<  End of sequence              */
    QSPI_LUT_CMD_CMD             = 1U,    /*!<  Command                      */
    QSPI_LUT_CMD_ADDR            = 2U,    /*!<  Address                      */
    QSPI_LUT_CMD_DUMMY           = 3U,    /*!<  Dummy cycles                 */
    QSPI_LUT_CMD_MODE            = 4U,    /*!<  8-bit mode                   */
    QSPI_LUT_CMD_MODE2           = 5U,    /*!<  2-bit mode                   */
    QSPI_LUT_CMD_MODE4           = 6U,    /*!<  4-bit mode                   */
    QSPI_LUT_CMD_READ            = 7U,    /*!<  Read data                    */
    QSPI_LUT_CMD_WRITE           = 8U,    /*!<  Write data                   */
    QSPI_LUT_CMD_JMP_ON_CS       = 9U,    /*!<  Jump on chip select deassert */
    QSPI_LUT_CMD_ADDR_DDR        = 10U,   /*!<  Address - DDR mode           */
    QSPI_LUT_CMD_MODE_DDR        = 11U,   /*!<  8-bit mode - DDR mode        */
    QSPI_LUT_CMD_MODE2_DDR       = 12U,   /*!<  2-bit mode - DDR mode        */
    QSPI_LUT_CMD_MODE4_DDR       = 13U,   /*!<  4-bit mode - DDR mode        */
    QSPI_LUT_CMD_READ_DDR        = 14U,   /*!<  Read data - DDR mode         */
    QSPI_LUT_CMD_WRITE_DDR       = 15U,   /*!<  Write data - DDR mode        */
    QSPI_LUT_CMD_CMD_DDR         = 17U,   /*!<  Command - DDR mode           */
    QSPI_LUT_CMD_CADDR           = 18U,   /*!<  Column address               */
    QSPI_LUT_CMD_CADDR_DDR       = 19U,   /*!<  Column address - DDR mode    */
} qspi_lut_commands_t;

/*! @brief Lut pad options 
 * Implements : qspi_lut_pads_t_Class
 */
typedef enum
{
    QSPI_LUT_PADS_1              = 0U,    /*!<  1 Pad      */
    QSPI_LUT_PADS_2              = 1U,    /*!<  2 Pads     */
    QSPI_LUT_PADS_4              = 2U,    /*!<  4 Pads     */
    QSPI_LUT_PADS_8              = 3U,    /*!<  8 Pads     */
} qspi_lut_pads_t;

/*! @brief Driver type
 * Implements : qspi_transfer_type_t_Class
 */
typedef enum
{
    QSPI_TRANSFER_TYPE_SYNC         = 0U,   /*!< Synchronous transfer using polling    */
    QSPI_TRANSFER_TYPE_ASYNC_INT    = 1U,   /*!< Interrupt-based asynchronous transfer */
    QSPI_TRANSFER_TYPE_ASYNC_DMA    = 2U,   /*!< DMA-based asynchronous transfer       */
} qspi_transfer_type_t;


/*! @brief Read mode
 * Implements : qspi_read_mode_t_Class
 */
typedef enum
{
    QSPI_READ_MODE_INTERNAL_SAMPLING   = 0U,  /*!< Sample on internal reference clock edge      */  
    QSPI_READ_MODE_INTERNAL_DQS        = 1U,  /*!< Use internally generated strobe signal       */  
    QSPI_READ_MODE_LOOPBACK_DQS        = 2U,  /*!< Use loopback clock from PAD as strobe signal */  
    QSPI_READ_MODE_EXTERNAL_DQS        = 3U,  /*!< Use external strobe signal                   */  
} qspi_read_mode_t;


/*! @brief Endianess options
 * Implements : qspi_endianess_t_Class
 */
typedef enum
{
    QSPI_END_64BIT_BE      = 0U,  /*!< 64-bit, Big Endian     */
    QSPI_END_32BIT_LE      = 1U,  /*!< 32-bit, Little Endian  */
    QSPI_END_32BIT_BE      = 2U,  /*!< 32-bit, Big Endian     */
    QSPI_END_64BIT_LE      = 3U,  /*!< 64-bit, Little Endian  */
} qspi_endianess_t;

/*! @brief Source of QuadSPI internal reference clock
 * Implements : qspi_clock_src_t_Class
 */
typedef enum
{
    QSPI_CLK_SRC_PLL_DIV1   = 0U,  /*!< PLL_DIV1 is clock source of QuadSPI internal reference clock  */
    QSPI_CLK_SRC_FIRC_DIV1  = 1U,  /*!< FIRC_DIV1 is clock source of QuadSPI internal reference clock */
} qspi_clock_src_t;

/*! @brief Clock phase used for sampling Rx data
 * Implements : qspi_date_rate_t_Class
 */
typedef enum
{
    QSPI_DATE_RATE_SDR         = 0U,    /*!<  Single data rate    */
    QSPI_DATE_RATE_DDR         = 1U,    /*!<  Double data rate    */
} qspi_date_rate_t;


/*! @brief External flash connection options (side A/B)
 * Implements : qspi_flash_side_t_Class
  */
typedef enum
{
    QSPI_FLASH_SIDE_A          = 0U,    /*!<  Serial flash connected to A-side  */
    QSPI_FLASH_SIDE_B          = 1U,    /*!<  Serial flash connected to B-side  */
} qspi_flash_side_t;


 /*! @brief Delay used for sampling Rx data
 * Implements : qspi_sample_delay_t_Class
  */
typedef enum
{
    QSPI_SAMPLE_DELAY_1        = 0U,    /*!<  One clock cycle delay   */
    QSPI_SAMPLE_DELAY_2        = 1U,    /*!<  Two clock cycles delay  */
} qspi_sample_delay_t;

 /*! @brief Clock phase used for sampling Rx data
 * Implements : qspi_sample_phase_t_Class
  */
typedef enum
{
    QSPI_SAMPLE_PHASE_NON_INVERTED    = 0U,    /*!<  Sampling at non-inverted clock  */
    QSPI_SAMPLE_PHASE_INVERTED        = 1U,    /*!<  Sampling at inverted clock      */
} qspi_sample_phase_t;


/*******************************************************************************
* Definitions
******************************************************************************/

/*! @brief QuadSPI callback function type. */
typedef void (* qspi_callback_t)(uint32_t instance, void * param);

 /*!
 * @brief Driver configuration structure
 *
 * This structure is used to provide configuration parameters for the qspi driver
 * at initialization time.
 * Implements : qspi_user_config_t_Class
 */
typedef struct
{
    qspi_date_rate_t dataRate;          /*!< Single/double data rate                              */
    bool dmaSupport;                    /*!< Enables DMA support in the driver                    */
#if (FEATURE_QSPI_FIXED_DMA_CHANNELS == 0)
    uint8_t dmaChannel;                 /*!< DMA channel number. Only used if dmaSupport is true  */
#endif
    qspi_callback_t callback;           /*!< User callback for reporting asynchronous events      */
    void * callbackParam;               /*!< Parameter for user callback                          */
    qspi_flash_side_t side;             /*!< Side on which the serial flash is connected          */
    uint32_t memSize;                   /*!< Size of serial flash                                 */
    uint8_t csHoldTime;                 /*!< CS hold time, expressed in serial clock cycles       */
    uint8_t csSetupTime;                /*!< CS setup time, expressed in serial clock cycles      */
    uint8_t columnAddr;                 /*!< Width of the column address, 0 if not used           */
    bool wordAddresable;                /*!< True if serial flash is word addressable             */
    qspi_read_mode_t readMode;          /*!< Read mode for incoming data from serial flash        */
    qspi_sample_delay_t sampleDelay;    /*!< Delay (in clock cycles) used for sampling Rx data    */
    qspi_sample_phase_t samplePhase;    /*!< Clock phase used for sampling Rx data                */
    uint8_t dqsDelay;                   /*!< Fine delay for internal or loopback DQS mode         */
    bool dqsInvert;                     /*!< Use inverted clock for internal or loopback DQS mode */
    qspi_endianess_t endianess;         /*!< Endianess configuration                              */
    qspi_clock_src_t clock_src;         /*!< Clock source for QuadSPI device                      */
    uint8_t divider;                    /*!< (1 - 8) Divider value for input clock                */
    uint8_t io2IdleValue;               /*!< (0 / 1) Logic level of IO[2] signal when not used    */
    uint8_t io3IdleValue;               /*!< (0 / 1) Logic level of IO[3] signal when not used    */
    bool dozeMode;                      /*!< Enable Doze Mode                                     */
} qspi_user_config_t;

 /*!
 * @brief AHB configuration structure
 *
 * This structure is used to provide configuration parameters for AHB access 
 * to the external flash
 * Implements : qspi_ahb_config_t_Class
 */
typedef struct
{
    uint8_t masters[QSPI_AHB_BUFFERS];    /*!< List of AHB masters assigned to each buffer          */
    uint16_t sizes[QSPI_AHB_BUFFERS];     /*!< List of buffer sizes                                 */
    bool allMasters;                      /*!< Indicates that any master may access the last buffer */
    bool highPriority;                    /*!< Indicates that the first buffer has high priority    */
} qspi_ahb_config_t;


/*!
 * @brief Driver internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the QSPI_DRV_Init() function, then
 * it cannot be freed until the driver is de-initialized using QSPI_DRV_Deinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    uint32_t instance;                  /* QuadSPI instance number                              */
    bool dmaSupport;                    /* Enables DMA support in the driver                    */
    uint8_t dmaChannel;                 /* DMA channel number. Only used if dmaSupport is true  */
    status_t status;                    /* Status of the current operation                      */
    bool driverBusy;                    /* Driver is busy with an operation                     */
    qspi_callback_t callback;           /* User callback for reporting asynchronous events      */
    void * callbackParam;               /* Parameter for user callback                          */
    uint8_t * data;                     /* Buffer for data being currently received             */
    const uint8_t * roData;             /* Read-only data (for program or verify operations)    */
    uint32_t size;                      /* Size of data being currently transmitted or received */
    uint8_t operation;                  /* True if receiving in interrupt mode                  */
/*! @endcond */
} qspi_state_t;


/*!
 * @brief Table of base addresses for QuadSPI instances
 */
extern QuadSPI_Type * const g_qspiBase[QuadSPI_INSTANCE_COUNT];


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name QuadSPI Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initializes the qspi driver
 *
 * This function initializes the qspi driver and prepares it for operation.
 *
 * @param instance     QuadSPI peripheral instance number
 * @param userConfigPtr    Pointer to the qspi user configuration structure. The
 *                         function reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the qspi context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using QSPI_DRV_Deinit().
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_Init(uint32_t instance,
                       const qspi_user_config_t * userConfigPtr,
                       qspi_state_t * state);


/*!
 * @brief De-initialize the qspi driver
 *
 * This function de-initializes the qspi driver. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_Deinit(uint32_t instance);


/*!
 * @brief Returns default configuration structure for QuadSPI
 *
 * @param userConfigPtr    Pointer to the qspi user configuration structure.
 * @return    Always returns STATUS_SUCCESS
 */
status_t QSPI_DRV_GetDefaultConfig(qspi_user_config_t * userConfigPtr);


/*!
 * @brief Sets up AHB accesses to the serial flash
 *
 * @param instance   QuadSPI peripheral instance number
 * @param config     AHB configuration structure
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_AhbSetup(uint32_t instance, const qspi_ahb_config_t *config);



/*!
 * @brief Configures LUT commands
 *
 * This function configures a pair of LUT commands in the specified LUT register.
 * LUT sequences start at index multiple of 4 and can have up to 8 commands
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lut          Index of LUT register
 * @param instr0       First instruction
 * @param pad0         Number of pads to use for first instruction
 * @param oprnd0       Operand for first instruction
 * @param instr1       Second instruction
 * @param pad1         Number of pads to use for second instruction
 * @param oprnd1       Operand for second instruction
 * Implements : QSPI_DRV_SetLut_Activity
 */
static inline void QSPI_DRV_SetLut(uint32_t instance,
                                   uint8_t lut,
                                   qspi_lut_commands_t instr0,
                                   qspi_lut_pads_t pad0,
                                   uint8_t oprnd0,
                                   qspi_lut_commands_t instr1,
                                   qspi_lut_pads_t pad1,
                                   uint8_t oprnd1)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    DEV_ASSERT(lut < QuadSPI_LUT_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->LUT[lut] = QuadSPI_LUT_INSTR0(instr0)
                       | QuadSPI_LUT_PAD0(pad0)
                       | QuadSPI_LUT_OPRND0(oprnd0)
                       | QuadSPI_LUT_INSTR1(instr1)
                       | QuadSPI_LUT_PAD1(pad1)
                       | QuadSPI_LUT_OPRND1(oprnd1);
}


/*!
 * @brief Locks LUT table
 *
 * @param instance     QuadSPI peripheral instance number
 * Implements : QSPI_DRV_LockLut_Activity
 */
static inline void QSPI_DRV_LockLut(uint32_t instance)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->LUTKEY = QSPI_LUT_LOCK_KEY;
    baseAddr->LCKCR = QuadSPI_LCKCR_LOCK_MASK;
}



/*!
 * @brief Unlocks LUT table
 *
 * @param instance     QuadSPI peripheral instance number
 * Implements : QSPI_DRV_UnlockLut_Activity
 */
static inline void QSPI_DRV_UnlockLut(uint32_t instance)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->LUTKEY = QSPI_LUT_LOCK_KEY;
    baseAddr->LCKCR = QuadSPI_LCKCR_UNLOCK_MASK;
}


/*!
 * @brief Clears IP sequence pointer
 *
 * @param instance     QuadSPI peripheral instance number
 * Implements : QSPI_DRV_ClearIpSeqPointer_Activity
 */
static inline void QSPI_DRV_ClearIpSeqPointer(uint32_t instance)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->SPTRCLR = QuadSPI_SPTRCLR_IPPTRC_MASK;
}


/*!
 * @brief Clears AHB sequence pointer
 *
 * @param instance     QuadSPI peripheral instance number
 * Implements : QSPI_DRV_ClearAHBSeqPointer_Activity
 */
static inline void QSPI_DRV_ClearAHBSeqPointer(uint32_t instance)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->SPTRCLR = QuadSPI_SPTRCLR_BFPTRC_MASK;
}


/*!
 * @brief Sets sequence ID for AHB operations
 *
 * @param instance     QuadSPI peripheral instance number
 * @param seqID        Sequence ID in LUT for read operation
 * Implements : QSPI_DRV_SetAhbSeqId_Activity
 */
static inline void QSPI_DRV_SetAhbSeqId(uint32_t instance, 
                                        uint8_t seqID)
{
    QuadSPI_Type *baseAddr;
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    baseAddr = g_qspiBase[instance];
    baseAddr->BFGENCR =  QuadSPI_BFGENCR_SEQID(seqID);
}


/*!
 * @brief Launches a simple IP command
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lut          Index of LUT register
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_IpCommand(uint32_t instance, uint8_t lut, uint32_t timeout);


/*!
 * @brief Launches an IP read command
 *
 * This function can launch a read command in 3 modes:
 * - normal read (dataRead != NULL): Data is read from serial flash and placed in the buffer
 * - verify (dataRead == NULL, dataCmp != NULL): Data is read from serial flash and compared to the reference buffer
 * - blank check (dataRead == NULL, dataCmp == NULL): Data is read from serial flash and compared to 0xFF
 * Only normal read mode can use DMA.
 *
 * @param instance       QuadSPI peripheral instance number
 * @param lut            Index of LUT register
 * @param addr           Start address for read operation in serial flash
 * @param dataRead       Buffer where to store read data
 * @param dataCmp        Buffer to be compared to read data
 * @param size           Size of data buffer
 * @param transferType   Type of transfer
 * @param timeout  timeout for the transfer in milliseconds; only applies for synchronous transfers
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_IpRead(uint32_t instance, 
                         uint8_t lut, 
                         uint32_t addr, 
                         uint8_t * dataRead,
                         const uint8_t * dataCmp,
                         uint32_t size,
                         qspi_transfer_type_t transferType,
                         uint32_t timeout);


/*!
 * @brief Launches an IP write command
 *
 * @param instance       QuadSPI peripheral instance number
 * @param lut            Index of LUT register
 * @param addr           Start address for write operation in serial flash
 * @param data           Data to be programmed in flash
 * @param size           Size of data buffer
 * @param transferType   Type of transfer
 * @param timeout  timeout for the transfer in milliseconds; only applies for synchronous transfers
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_IpWrite(uint32_t instance, 
                          uint8_t lut, 
                          uint32_t addr, 
                          const uint8_t * data, 
                          uint32_t size,
                          qspi_transfer_type_t transferType,
                          uint32_t timeout);


/*!
 * @brief Launches an IP erase command
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lut          Index of LUT register
 * @param addr         Start address of erased sector
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_IpErase(uint32_t instance, 
                          uint8_t lut, 
                          uint32_t addr);


/*!
 * @brief Checks the status of the currently running IP command
 *
 * @param instance     QuadSPI peripheral instance number
 * @return    Error or success status returned by API
 */
status_t QSPI_DRV_IpGetStatus(uint32_t instance);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* QUADSPI_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
