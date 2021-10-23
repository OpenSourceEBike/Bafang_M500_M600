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

#ifndef SPI_PAL_mapping_H
#define SPI_PAL_mapping_H
#include "device_registers.h"

/*!
 * @brief Enumeration with the types of peripherals supported by SPI PAL
 *
 * This enumeration contains the types of peripherals supported by SPI PAL.
 * Implements : spi_inst_type_t_Class
 */
typedef enum{
#if defined(LPSPI_INSTANCE_COUNT)
    SPI_INST_TYPE_LPSPI = 0u,
#endif

#if defined(FLEXIO_INSTANCE_COUNT)
    SPI_INST_TYPE_FLEXIO = 1u,
#endif

#if defined(DSPI_INSTANCE_COUNT)
    SPI_INST_TYPE_DSPI = 2u,
#endif

#if defined(SPI_INSTANCE_COUNT)
    SPI_INST_TYPE_SPI = 3u,
#endif

} spi_inst_type_t;

/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : spi_instance_t_Class
 */
typedef struct{
    spi_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
} spi_instance_t;

#endif /* SPI_PAL_mapping_H */
