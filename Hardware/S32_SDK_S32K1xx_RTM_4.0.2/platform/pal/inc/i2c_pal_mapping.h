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
 * @file i2c_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef PAL_mapping_H
#define PAL_mapping_H

#include "device_registers.h"
#include "i2c_pal_cfg.h"

/* Include PD files */
#if (defined(I2C_OVER_LPI2C))
    #include "lpi2c_driver.h"
#endif

#if (defined(I2C_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_i2c_driver.h"
#endif

#if (defined(I2C_OVER_I2C))
    #include "i2c_driver.h"
#endif

#if (defined(I2C_OVER_SWI2C))
    #include "swi2c_driver.h"
#endif


 /*!
  * @brief Enumeration with the types of peripherals supported by I2C PAL
  *
  * This enumeration contains the types of peripherals supported by I2C PAL.
  * Implements : i2c_inst_type_t_Class
  */
 typedef enum{
 #if defined(LPI2C_INSTANCE_COUNT)
     I2C_INST_TYPE_LPI2C    = 0u,
 #endif /* defined(LPI2C_INSTANCE_COUNT)  */

 #if defined(FLEXIO_INSTANCE_COUNT)
     I2C_INST_TYPE_FLEXIO   = 1u,
 #endif /* defined(FLEXIO_INSTANCE_COUNT) */

 #if defined(I2C_INSTANCE_COUNT)
     I2C_INST_TYPE_I2C     = 2u,
 #endif /* defined(I2C_INSTANCE_COUNT)   */

 #if defined(SWI2C_INSTANCE_COUNT)
     I2C_INST_TYPE_SWI2C     = 3u,
 #endif /* defined(SWI2C_INSTANCE_COUNT)   */

 } i2c_inst_type_t;

 /*!
  * @brief Structure storing PAL instance information
  *
  * This structure is used for storing PAL instance information.
  * Implements : i2c_instance_t_Class
  */
 typedef struct{
     i2c_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
     uint32_t instIdx;            /*!< Instance index of the peripheral over which the PAL is used */
 } i2c_instance_t;


#endif /* PAL_mapping_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/

 
