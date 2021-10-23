/*
** ###################################################################
**     Processor:           S32K142 with 32 KB SRAM
**     Compiler:            GreenHills ARM Compiler
**
**     Abstract:
**         Linker file for the GreenHills ARM Compiler
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2017-2018 NXP
**     All rights reserved.
**
**     NXP Confidential. This software is owned or controlled by NXP and may only be
**     used strictly in accordance with the applicable license terms. By expressly
**     accepting such terms or by downloading, installing, activating and/or otherwise
**     using the software, you are agreeing that you have read, and that you agree to
**     comply with and are bound by, such license terms. If you do not agree to be
**     bound by the applicable license terms, then you may not retain, install,
**     activate or otherwise use the software. The production use license in
**     Section 2.3 is expressly granted for this software.
**
**     http:                 www.nxp.com
**
** ###################################################################
*/

/* Specify the memory areas */
MEMORY
{
  /* Flash */
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_flash_config        (RX)  : ORIGIN = 0x00000400, LENGTH = 0x00000010
  m_text                (RX)  : ORIGIN = 0x00000410, LENGTH = 0x0003FBF0

  /* SRAM_L */
  m_data                (RW)  : ORIGIN = 0x1FFFC000, LENGTH = 0x00004000

  /* SRAM_U */
  m_data_2              (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00003000
}

/* Define output sections */
SECTIONS
{
  // Defaults
  HEAP_SIZE  = isdefined(__heap_size__)  ? __heap_size__  : 0x00000400;
  STACK_SIZE = isdefined(__stack_size__) ? __stack_size__ : 0x00000400;
  
/* If symbol __flash_vector_table__=1 is defined at link time 
 * the interrupt vector will not be copied to RAM. 
 * Warning: Using the interrupt vector from Flash will not allow
 * INT_SYS_InstallHandler because the section is Read Only. 
 */
  M_VECTOR_RAM_SIZE = isdefined(__flash_vector_table__) ? 0x0 : 0x0400;

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __interrupts_start__ = .;
    . = ALIGN(4);
    "*(.isr_vector)"         /* Startup code */   
    __interrupts_end__ = .;
    . = ALIGN(4);
  } > m_interrupts

  .flash_config :
  {
    . = ALIGN(4);
    "*(.FlashConfig)"        /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    "*(.text)"               /* .text sections (code) */
    "*(.text*)"              /* .text* sections (code) */
    "*(.rodata)"             /* .rodata sections (constants, strings, etc.) */
    "*(.rodata*)"            /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } > m_text

  __etext = .;    /* Define a global symbol at end of code. */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization. */

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __RAM_START = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start. */
    *(.m_interrupts_ram)          /* This is a user defined section. */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .;   /* Define a global symbol at data end. */
  } > m_data

  __VECTOR_RAM = isdefined(__flash_vector_table__) ? addr(m_interrupts) : __VECTOR_RAM__ ;
  __RAM_VECTOR_TABLE_SIZE = isdefined(__flash_vector_table__) ? 0x0 : (__interrupts_ram_end__ - __interrupts_ram_start__);

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* Create a global symbol at data start. */
    "*(.data)"               /* .data sections */
    "*(.data*)"              /* .data* sections */
    . = ALIGN(4);
    __data_end__ = .;        /* Define a global symbol at data end. */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  __CODE_ROM = __DATA_END; /* Symbol is used by code initialization. */

  .code : AT(__CODE_ROM)
  {
    . = ALIGN(4);
    __CODE_RAM = .;
    __code_start__ = .;      /* Create a global symbol at code start. */
    __code_ram_start__ = .;
    "*(.code_ram)"           /* Custom section for storing code in RAM */
    . = ALIGN(4);
    __code_end__ = .;        /* Define a global symbol at code end. */
    __code_ram_end__ = .;
  } > m_data

  __CODE_END = __CODE_ROM + (__code_end__ - __code_start__);
  __CUSTOM_ROM = __CODE_END;

  /* Custom Section Block that can be used to place data at absolute address. */
  /* Use __attribute__((section (".customSection"))) to place data here. */
  .customSectionBlock : AT(__CUSTOM_ROM)
  {
    __customSection_start__ = .;
    "*(.customSection)"      /* Keep section even if not referenced. */
    __customSection_end__ = .;
  } > m_data_2
  __CUSTOM_END = __CUSTOM_ROM + (__customSection_end__ - __customSection_start__);

  /* Uninitialized data section. */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section. */
    . = ALIGN(4);
    __BSS_START = .;
    __bss_start__ = .;
    "*(.bss)"
    "*(.bss*)"
    "*(COMMON)"
    . = ALIGN(4);
    __bss_end__ = .;
    __BSS_END = .;
  } > m_data_2

   /* Put heap section after the program data */
  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    __heap_start__ = .;
    PROVIDE(end = .);
    PROVIDE(_end = .);
    PROVIDE(__end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .;
    __heap_end__ = .;
  } > m_data_2

  /* Initializes stack on the end of block */
  __StackTop   = addr(m_data_2) + sizeof(m_data_2);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);
  __RAM_END = __StackTop;

  .stack __StackLimit :
  {
    . = ALIGN(8);
    __stack_start__ = .;
    . += STACK_SIZE;
    __stack_end__ = .;
  } > m_data_2

  .ARM.attributes :
  {
    *(.ARM.attributes)
  }

}

