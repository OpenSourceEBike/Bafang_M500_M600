/******************************************************************************
*
* Copyright 2015-2016 Freescale
* Copyright 2019 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only 
* be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing, activating
* and/or otherwise using the software, you are agreeing that you have read, 
* and that you agree to comply with and are bound by, such license terms. 
* If you do not agree to be bound by the applicable license terms, 
* then you may not retain, install, activate or otherwise use the software.
*
******************************************************************************/
#ifndef M4_SCST_TYPEDEFS_H
#define M4_SCST_TYPEDEFS_H


    typedef signed char m4_scst_int8_t;
    typedef unsigned char m4_scst_uint8_t;
    typedef volatile signed char m4_scst_vint8_t;
    typedef volatile unsigned char m4_scst_vuint8_t;

    typedef signed short m4_scst_int16_t;
    typedef unsigned short m4_scst_uint16_t;
    typedef volatile signed short m4_scst_vint16_t;
    typedef volatile unsigned short m4_scst_vuint16_t;

    typedef signed int m4_scst_int32_t;
    typedef unsigned int m4_scst_uint32_t;
    typedef volatile signed int m4_scst_vint32_t;
    typedef volatile unsigned int m4_scst_vuint32_t;
    
    #ifndef TRUE                /* conditional check */
        #define TRUE      1
    #endif
    
    #ifndef FALSE               /* conditional check */
        #define FALSE     0
    #endif

#endif    /* _M4_SCST_TYPEDEFS_H */


