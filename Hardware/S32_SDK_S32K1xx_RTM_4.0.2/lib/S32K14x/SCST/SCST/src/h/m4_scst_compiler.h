/******************************************************************************
*
* Copyright 2015-2016 Freescale
* Copyright 2019-2020 NXP
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
#ifndef M4_SCST_COMPILER_H
#define M4_SCST_COMPILER_H


#ifdef __ghs__
    #define function $function
    #define object $object

    #define SCST_SYNTAX
    #define SCST_THUMB .thumb
    #define SCST_THUMB2 .thumb2

    #define SCST_TYPE(name, ident) .type name, ident

    #define SCST_OPCODE_START /*opcode start*/
    #define SCST_OPCODE_END /*opcode end*/

    #define SCST_LABEL
    #define SCST_CODE

    #define SCST_WORD .long
    #define SCST_SHORT .short

    #define SCST_EXPORT .global
    #define SCST_EXTERN .global

    #define SCST_ALIGN_POWER_OF_TWO(POWER) .align (1 << POWER)
    #define SCST_LTORG .ltorg
    
    #define SCST_ALLOC_BYTES(N) .space N
    
    /* has to be at the beginning of a line */
    #define SCST_SET(NAME,VALUE) .set NAME, VALUE
    /* has to be at the beginning of a line */
    #define SCST_DEFINE(NAME,VALUE) .set NAME, VALUE

    #define SCST_SECTION_EXEC(SECTION_NAME) .section .SECTION_NAME,"ax"
    #define SCST_SECTION_DATA(SECTION_NAME) .section .SECTION_NAME,"aw"
    #define SCST_SECTION_CONST(SECTION_NAME) .section .SECTION_NAME,"a"
    #define SCST_SECTION_EXEC_W(SECTION_NAME) .section .SECTION_NAME,"awx"
    #define SCST_SECTION_DATA_UNINIT(SECTION_NAME) .section .SECTION_NAME,"awb"
    
    #define SCST_SECTION_EXEC_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC(SECTION_NAME)
    #define SCST_SECTION_DATA_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA(SECTION_NAME)
    #define SCST_SECTION_CONST_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_CONST(SECTION_NAME)
    #define SCST_SECTION_EXEC_W_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC_W(SECTION_NAME)
    #define SCST_SECTION_DATA_UNINIT_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA_UNINIT(SECTION_NAME)

    #define SCST_COND_INSTRUCTION(INSTRUCTION,CONDITION,WIDTH,...) INSTRUCTION##.##WIDTH __VA_ARGS__
    
    #define SCST_FILE_END

    #define APSR_nzcvq APSR    
#endif
#ifdef __GNUC__
    #define SCST_THUMB .thumb
    #define SCST_THUMB2 .thumb ; .syntax unified

    #define SCST_TYPE(name, ident) .type name, %ident

    #define SCST_OPCODE_START /*opcode start*/
    #define SCST_OPCODE_END /*opcode end*/

    #define SCST_LABEL
    #define SCST_CODE

    #define SCST_WORD .long
    #define SCST_SHORT .short

    #define SCST_EXPORT .global
    #define SCST_EXTERN .global

    #define SCST_ALIGN_POWER_OF_TWO(POWER) .align POWER
    #define SCST_LTORG .ltorg
    
    #define SCST_ALLOC_BYTES(N) .space N
    
    /* has to be at the beginning of a line */
    #define SCST_SET(NAME,VALUE) .set NAME, VALUE
    /* has to be at the beginning of a line */
    #define SCST_DEFINE(NAME,VALUE) .set NAME, VALUE

    #define SCST_SECTION_EXEC(SECTION_NAME) .section .SECTION_NAME,"ax"
    #define SCST_SECTION_DATA(SECTION_NAME) .section .SECTION_NAME,"aw"
    #define SCST_SECTION_CONST(SECTION_NAME) .section .SECTION_NAME,"a"
    #define SCST_SECTION_EXEC_W(SECTION_NAME) .section .SECTION_NAME,"awx"
    #define SCST_SECTION_DATA_UNINIT(SECTION_NAME) .section .SECTION_NAME,"aw"
    
    #define SCST_SECTION_EXEC_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC(SECTION_NAME) ; SCST_ALIGN_POWER_OF_TWO(ALIGNMENT)
    #define SCST_SECTION_DATA_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA(SECTION_NAME) ; SCST_ALIGN_POWER_OF_TWO(ALIGNMENT)
    #define SCST_SECTION_CONST_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_CONST(SECTION_NAME) ; SCST_ALIGN_POWER_OF_TWO(ALIGNMENT)
    #define SCST_SECTION_EXEC_W_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC_W(SECTION_NAME) ; SCST_ALIGN_POWER_OF_TWO(ALIGNMENT)
    #define SCST_SECTION_DATA_UNINIT_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA_UNINIT(SECTION_NAME) ; SCST_ALIGN_POWER_OF_TWO(ALIGNMENT)

    #define SCST_COND_INSTRUCTION(INSTRUCTION,CONDITION,WIDTH,...) INSTRUCTION##CONDITION##.##WIDTH __VA_ARGS__
    
    #define SCST_FILE_END

    #define APSR_nzcvq APSR_nzcvq

#endif


#ifdef __IAR_SYSTEMS_ASM__
    #define SCST_SYNTAX
    #define SCST_THUMB CODE16
    #define SCST_THUMB2 THUMB

    #define SCST_TYPE(name, ident) /*not needed*/

    #define SCST_OPCODE_START DATA
    #define SCST_OPCODE_END THUMB

    #define SCST_LABEL DATA
    #define SCST_CODE THUMB

    #define SCST_WORD DC32
    #define SCST_SHORT DC16

    #define SCST_EXPORT EXPORT
    #define SCST_EXTERN EXTERN

    #define SCST_ALIGN_POWER_OF_TWO(POWER) ALIGNROM POWER
    #define SCST_LTORG LTORG
    
    #define SCST_ALLOC_BYTES(N) DS8 N
    
    /* has to be at the beginning of a line */
    #define SCST_SET(NAME,VALUE) NAME SET VALUE
    /* has to be at the beginning of a line */
    #define SCST_DEFINE(NAME,VALUE) NAME DEFINE VALUE 

    #define SCST_SECTION_EXEC(SECTION_NAME) section .SECTION_NAME :CODE
    #define SCST_SECTION_DATA(SECTION_NAME) section .SECTION_NAME :DATA
    #define SCST_SECTION_CONST(SECTION_NAME) section .SECTION_NAME :CONST
    #define SCST_SECTION_EXEC_W(SECTION_NAME) section .SECTION_NAME :DATA
    #define SCST_SECTION_DATA_UNINIT(SECTION_NAME) section .SECTION_NAME :DATA

    #define SCST_SECTION_EXEC_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC(SECTION_NAME) (ALIGNMENT)
    #define SCST_SECTION_DATA_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA(SECTION_NAME) (ALIGNMENT)
    #define SCST_SECTION_CONST_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_CONST(SECTION_NAME) (ALIGNMENT)
    #define SCST_SECTION_EXEC_W_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_EXEC_W(SECTION_NAME) (ALIGNMENT)
    #define SCST_SECTION_DATA_UNINIT_ALIGN(SECTION_NAME, ALIGNMENT) SCST_SECTION_DATA_UNINIT(SECTION_NAME) (ALIGNMENT)
    
    #define SCST_FILE_END END

    #define APSR_nzcvq APSR    
#endif /* __IAR_SYSTEMS_ASM__ */

#if defined (__ghs__) || defined (__GNUC__) || defined (__IAR_SYSTEMS_ASM__)
#define SCST_BYTES_2   1
#define SCST_BYTES_4   2
#define SCST_BYTES_8   3
#define SCST_BYTES_16  4
#define SCST_BYTES_32  5
#define SCST_BYTES_64  6
#define SCST_BYTES_128 7

#define SCST_ALIGN_BYTES_2 SCST_ALIGN_POWER_OF_TWO(1)
#define SCST_ALIGN_BYTES_4 SCST_ALIGN_POWER_OF_TWO(2)
#define SCST_ALIGN_BYTES_8 SCST_ALIGN_POWER_OF_TWO(3)
#define SCST_ALIGN_BYTES_16 SCST_ALIGN_POWER_OF_TWO(4)
#define SCST_ALIGN_BYTES_32 SCST_ALIGN_POWER_OF_TWO(5)
#define SCST_ALIGN_BYTES_64 SCST_ALIGN_POWER_OF_TWO(6)
#define SCST_ALIGN_BYTES_128 SCST_ALIGN_POWER_OF_TWO(7)

/**
* use this to write 32bit opcode. This is one (HIGH) is the first half and it should go first.
*/
#define SCST_OPCODE32_HIGH(DATA) SCST_SHORT DATA
/**
* use this to write 32bit opcode. This is one (LOW) is the second half and it should go after SCST_OPCODE32_HIGH.
*/
#define SCST_OPCODE32_LOW(DATA) SCST_SHORT DATA
/**
* use this to write 16bit opcode.
*/
#define SCST_OPCODE16(DATA) SCST_SHORT DATA
#endif 

/*==============================================================================*/
/* C/C++ compiler macros */
#ifdef __IAR_SYSTEMS_ICC__
#ifndef __IAR_SYSTEMS_ASM__
/* C/C++ compiler macros for placing a variable in an assembly defined section */
#define SCST_PLACE_IN_SECTION_HELPER_0(X) #X
#define SCST_PLACE_IN_SECTION_HELPER_1(SECTION_NAME) SCST_PLACE_IN_SECTION_HELPER_0(location = #SECTION_NAME)
#define SCST_PLACE_IN_SECTION_HELPER_2(SECTION_NAME) SCST_PLACE_IN_SECTION_HELPER_1(.SECTION_NAME)

#define SCST_PLACE_IN_SECTION(SECTION_NAME) _Pragma(SCST_PLACE_IN_SECTION_HELPER_2(SECTION_NAME)) 

#define SCST_INLINE /*there's a bug in IAR compiler preventing from linking in certain cases*/
#endif  /* #ifndef __IAR_SYSTEMS_ASM__ */
#endif  /* __IAR_SYSTEMS_ICC__*/
#if defined (__ghs__) || defined (__GNUC__)
/* C/C++ compiler macros for placing a variable in an assembly defined section */
#define SCST_PLACE_IN_SECTION_HELPER(SECTION_NAME) __attribute__ (( section(#SECTION_NAME) ))

#define SCST_PLACE_IN_SECTION(SECTION_NAME) SCST_PLACE_IN_SECTION_HELPER(.SECTION_NAME) 

#define SCST_INLINE inline
#endif /* __ghs__ __GNUC__ */

#endif /* #ifndef M4_SCST_COMPILER_H */

