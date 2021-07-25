; ---------------------------------------------------------------------------------------
;  @file:    startup_S32K142.s
;  @purpose: ARM Compiler Collection Startup File
;            S32K142
;  @version: 2.0
;  @date:    2017-1-10
;  @build:   b170107
; ---------------------------------------------------------------------------------------
;
; Copyright (c) 1997 - 2016 , Freescale Semiconductor, Inc.
; Copyright 2016-2017 NXP
; All rights reserved.
;
; NXP Confidential. This software is owned or controlled by NXP and may only be
; used strictly in accordance with the applicable license terms. By expressly
; accepting such terms or by downloading, installing, activating and/or otherwise
; using the software, you are agreeing that you have read, and that you agree to
; comply with and are bound by, such license terms. If you do not agree to be
; bound by the applicable license terms, then you may not retain, install,
; activate or otherwise use the software. The production use license in
; Section 2.3 is expressly granted for this software.
;***************************************************************************
; Version: ARMC Compiler Collection
;***************************************************************************


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    |.text|, CODE, READONLY
                EXPORT  __VECTOR_TABLE

                EXPORT __RAM_VECTOR_TABLE_SIZE
                EXPORT __VECTOR_RAM
                EXPORT __VECTOR_ROM

                EXPORT __BSS_START
                EXPORT __BSS_END

                EXPORT __CODE_ROM
                EXPORT __CODE_END
                EXPORT __CODE_RAM

                EXPORT __DATA_ROM
                EXPORT __DATA_END
                EXPORT __DATA_RAM
                EXPORT __VECTOR_ROM_START
                EXPORT __VECTOR_ROM_END
                EXPORT __DATA_RAM_START
                EXPORT __DATA_RAM_END
                EXPORT __CUSTOM_SECTION_START
                EXPORT __CUSTOM_SECTION_END
                EXPORT __CODE_RAM_START
                EXPORT __CODE_RAM_END
                
                EXPORT __RAM_START
                EXPORT __RAM_END

                IMPORT init_data_bss
                IMPORT  main
                IMPORT  __main
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|
                IMPORT  |Image$$RW_m_data_2$$ZI$$Base|
                IMPORT  |Image$$RW_m_data_2$$ZI$$Limit|

                IMPORT  |Image$$ER_m_text_const$$Base|
                IMPORT  |Image$$ER_m_text_const$$Limit|

                IMPORT  |Image$$RW_m_data$$Base|

                IMPORT  |Image$$ER_m_text_const$$Base|
                IMPORT  |Image$$ER_m_text_const$$Limit|

                IMPORT  |Image$$RW_m_data$$Length|
                IMPORT |Image$$RW_m_code$$Base|
                IMPORT |Image$$RW_m_code$$Length|
                IMPORT |Image$$ER_m_text$$Limit|

                IMPORT |Image$$VECTOR_RAM$$Length|
                IMPORT |Image$$VECTOR_RAM$$Base|

                IMPORT |Image$$VECTOR_ROM$$Length|
                IMPORT |Image$$VECTOR_ROM$$Base|
                IMPORT |Image$$RW_m_custom_section$$Base|
                IMPORT |Image$$RW_m_custom_section$$Limit|
                
                IMPORT |Image$$RW_m_data$$Limit|
                IMPORT |Image$$RW_m_code$$Limit|
                IMPORT |Image$$VECTOR_ROM$$Limit|
                
                IMPORT |Image$$ARM_LIB_STACK$$Limit|
__RAM_VECTOR_TABLE_SIZE DCD |Image$$VECTOR_ROM$$Length|
__VECTOR_RAM DCD |Image$$VECTOR_RAM$$Base|
__VECTOR_ROM DCD |Image$$VECTOR_ROM$$Base|
__VECTOR_ROM_START DCD |Image$$VECTOR_ROM$$Base|
__VECTOR_ROM_END DCD |Image$$VECTOR_ROM$$Limit|

__CODE_ROM DCD  |Image$$RW_m_data$$Length|+ |Image$$ER_m_text$$Limit|
__CODE_END DCD  |Image$$RW_m_data$$Length|+ |Image$$ER_m_text$$Limit| + |Image$$RW_m_code$$Length|
__CODE_RAM DCD |Image$$RW_m_code$$Base|
__CODE_RAM_START DCD |Image$$RW_m_code$$Base|
__CODE_RAM_END DCD |Image$$RW_m_code$$Limit|

__BSS_START DCD  |Image$$RW_m_data_2$$ZI$$Base|
__BSS_END DCD  |Image$$RW_m_data_2$$ZI$$Limit|

__DATA_RAM DCD  |Image$$RW_m_data$$Base|
__DATA_RAM_START DCD  |Image$$RW_m_data$$Base|
__DATA_RAM_END DCD  |Image$$RW_m_data$$Limit|
__DATA_ROM DCD  |Image$$ER_m_text$$Limit|
__DATA_END DCD  |Image$$RW_m_data$$Length|+ |Image$$ER_m_text$$Limit|
__CUSTOM_SECTION_START DCD |Image$$RW_m_custom_section$$Base|
__CUSTOM_SECTION_END DCD |Image$$RW_m_custom_section$$Limit|

#if (__RAM_VECTOR_TABLE_SIZE > 0)
__RAM_START DCD |Image$$VECTOR_RAM$$Base|
#else
__RAM_START DCD |Image$$RW_m_data$$Base|
#endif
__RAM_END   DCD |Image$$ARM_LIB_STACK$$Limit|

		AREA    RESET, DATA, READONLY
__VECTOR_TABLE  DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit| ; Top of Stack
                DCD     Reset_Handler  ; Reset Handler
                DCD     NMI_Handler                       ;NMI Handler
                DCD     HardFault_Handler                   ;Hard Fault Handler
                DCD     MemManage_Handler                   ;MPU Fault Handler
                DCD     BusFault_Handler                    ;Bus Fault Handler
                DCD     UsageFault_Handler                  ;Usage Fault Handler
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     SVC_Handler                         ;SVCall Handler
                DCD     DebugMon_Handler                    ;Debug Monitor Handler
                DCD     0                                   ;Reserved
                DCD     PendSV_Handler                      ;PendSV Handler
                DCD     SysTick_Handler                     ;SysTick Handler

                                                            ; External Interrupts
    DCD   DMA0_IRQHandler                                 ; DMA channel 0 transfer complete
    DCD   DMA1_IRQHandler                                 ; DMA channel 1 transfer complete
    DCD   DMA2_IRQHandler                                 ; DMA channel 2 transfer complete
    DCD   DMA3_IRQHandler                                 ; DMA channel 3 transfer complete
    DCD   DMA4_IRQHandler                                 ; DMA channel 4 transfer complete
    DCD   DMA5_IRQHandler                                 ; DMA channel 5 transfer complete
    DCD   DMA6_IRQHandler                                 ; DMA channel 6 transfer complete
    DCD   DMA7_IRQHandler                                 ; DMA channel 7 transfer complete
    DCD   DMA8_IRQHandler                                 ; DMA channel 8 transfer complete
    DCD   DMA9_IRQHandler                                 ; DMA channel 9 transfer complete
    DCD   DMA10_IRQHandler                                ; DMA channel 10 transfer complete
    DCD   DMA11_IRQHandler                                ; DMA channel 11 transfer complete
    DCD   DMA12_IRQHandler                                ; DMA channel 12 transfer complete
    DCD   DMA13_IRQHandler                                ; DMA channel 13 transfer complete
    DCD   DMA14_IRQHandler                                ; DMA channel 14 transfer complete
    DCD   DMA15_IRQHandler                                ; DMA channel 15 transfer complete
    DCD   DMA_Error_IRQHandler                            ; DMA error interrupt channels 0-15
    DCD   MCM_IRQHandler                                  ; FPU sources
    DCD   FTFC_IRQHandler                                 ; FTFC Command complete
    DCD   Read_Collision_IRQHandler                       ; FTFC Read collision
    DCD   LVD_LVW_IRQHandler                              ; PMC Low voltage detect interrupt
    DCD   FTFC_Fault_IRQHandler                           ; FTFC Double bit fault detect
    DCD   WDOG_EWM_IRQHandler                             ; Single interrupt vector for WDOG and EWM
    DCD   RCM_IRQHandler                                  ; RCM Asynchronous Interrupt
    DCD   LPI2C0_Master_IRQHandler                        ; LPI2C0 Master Interrupt
    DCD   LPI2C0_Slave_IRQHandler                         ; LPI2C0 Slave Interrupt
    DCD   LPSPI0_IRQHandler                               ; LPSPI0 Interrupt
    DCD   LPSPI1_IRQHandler                               ; LPSPI1 Interrupt
    DCD   Reserved44_IRQHandler                           ; Reserved Interrupt 44
    DCD   Reserved45_IRQHandler                           ; Reserved Interrupt 45
    DCD   Reserved46_IRQHandler                           ; Reserved Interrupt 46
    DCD   LPUART0_RxTx_IRQHandler                         ; LPUART0 Transmit / Receive Interrupt
    DCD   Reserved48_IRQHandler                           ; Reserved Interrupt 48
    DCD   LPUART1_RxTx_IRQHandler                         ; LPUART1 Transmit / Receive  Interrupt
    DCD   Reserved50_IRQHandler                           ; Reserved Interrupt 50
    DCD   Reserved51_IRQHandler                           ; Reserved Interrupt 51
    DCD   Reserved52_IRQHandler                           ; Reserved Interrupt 52
    DCD   Reserved53_IRQHandler                           ; Reserved Interrupt 53
    DCD   Reserved54_IRQHandler                           ; Reserved Interrupt 54
    DCD   ADC0_IRQHandler                                 ; ADC0 interrupt request.
    DCD   ADC1_IRQHandler                                 ; ADC1 interrupt request.
    DCD   CMP0_IRQHandler                                 ; CMP0 interrupt request
    DCD   Reserved58_IRQHandler                           ; Reserved Interrupt 58
    DCD   Reserved59_IRQHandler                           ; Reserved Interrupt 59
    DCD   ERM_single_fault_IRQHandler                     ; ERM single bit error correction
    DCD   ERM_double_fault_IRQHandler                     ; ERM double bit error non-correctable
    DCD   RTC_IRQHandler                                  ; RTC alarm interrupt
    DCD   RTC_Seconds_IRQHandler                          ; RTC seconds interrupt
    DCD   LPIT0_Ch0_IRQHandler                            ; LPIT0 channel 0 overflow interrupt
    DCD   LPIT0_Ch1_IRQHandler                            ; LPIT0 channel 1 overflow interrupt
    DCD   LPIT0_Ch2_IRQHandler                            ; LPIT0 channel 2 overflow interrupt
    DCD   LPIT0_Ch3_IRQHandler                            ; LPIT0 channel 3 overflow interrupt
    DCD   PDB0_IRQHandler                                 ; PDB0 interrupt
    DCD   Reserved69_IRQHandler                           ; Reserved Interrupt 69
    DCD   Reserved70_IRQHandler                           ; Reserved Interrupt 70
    DCD   Reserved71_IRQHandler                           ; Reserved Interrupt 71
    DCD   Reserved72_IRQHandler                           ; Reserved Interrupt 72
    DCD   SCG_IRQHandler                                  ; SCG bus interrupt request
    DCD   LPTMR0_IRQHandler                               ; LPTIMER interrupt request
    DCD   PORTA_IRQHandler                                ; Port A pin detect interrupt
    DCD   PORTB_IRQHandler                                ; Port B pin detect interrupt
    DCD   PORTC_IRQHandler                                ; Port C pin detect interrupt
    DCD   PORTD_IRQHandler                                ; Port D pin detect interrupt
    DCD   PORTE_IRQHandler                                ; Port E pin detect interrupt
    DCD   SWI_IRQHandler                                  ; Software interrupt
    DCD   Reserved81_IRQHandler                           ; Reserved Interrupt 81
    DCD   Reserved82_IRQHandler                           ; Reserved Interrupt 82
    DCD   Reserved83_IRQHandler                           ; Reserved Interrupt 83
    DCD   PDB1_IRQHandler                                 ; PDB1 interrupt
    DCD   FLEXIO_IRQHandler                               ; FlexIO Interrupt
    DCD   Reserved86_IRQHandler                           ; Reserved Interrupt 86
    DCD   Reserved87_IRQHandler                           ; Reserved Interrupt 87
    DCD   Reserved88_IRQHandler                           ; Reserved Interrupt 88
    DCD   Reserved89_IRQHandler                           ; Reserved Interrupt 89
    DCD   Reserved90_IRQHandler                           ; Reserved Interrupt 90
    DCD   Reserved91_IRQHandler                           ; Reserved Interrupt 91
    DCD   Reserved92_IRQHandler                           ; Reserved Interrupt 92
    DCD   Reserved93_IRQHandler                           ; Reserved Interrupt 93
    DCD   CAN0_ORed_IRQHandler                            ; CAN0 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]
    DCD   CAN0_Error_IRQHandler                           ; CAN0 Interrupt indicating that errors were detected on the CAN bus
    DCD   CAN0_Wake_Up_IRQHandler                         ; CAN0 Interrupt asserted when Pretended Networking operation is enabled, and a valid message matches the selected filter criteria during Low Power mode
    DCD   CAN0_ORed_0_15_MB_IRQHandler                    ; CAN0 OR'ed Message buffer (0-15)
    DCD   CAN0_ORed_16_31_MB_IRQHandler                   ; CAN0 OR'ed Message buffer (16-31)
    DCD   Reserved99_IRQHandler                           ; Reserved Interrupt 99
    DCD   Reserved100_IRQHandler                          ; Reserved Interrupt 100
    DCD   CAN1_ORed_IRQHandler                            ; CAN1 OR'ed [Bus Off OR Transmit Warning OR Receive Warning]
    DCD   CAN1_Error_IRQHandler                           ; CAN1 Interrupt indicating that errors were detected on the CAN bus
    DCD   Reserved103_IRQHandler                          ; Reserved Interrupt 103
    DCD   CAN1_ORed_0_15_MB_IRQHandler                    ; CAN1 OR'ed Interrupt for Message buffer (0-15)
    DCD   Reserved105_IRQHandler                          ; Reserved Interrupt 105
    DCD   Reserved106_IRQHandler                          ; Reserved Interrupt 106
    DCD   Reserved107_IRQHandler                          ; Reserved Interrupt 107
    DCD   Reserved108_IRQHandler                          ; Reserved Interrupt 108
    DCD   Reserved109_IRQHandler                          ; Reserved Interrupt 109
    DCD   Reserved110_IRQHandler                          ; Reserved Interrupt 110
    DCD   Reserved111_IRQHandler                          ; Reserved Interrupt 111
    DCD   Reserved112_IRQHandler                          ; Reserved Interrupt 112
    DCD   Reserved113_IRQHandler                          ; Reserved Interrupt 113
    DCD   Reserved114_IRQHandler                          ; Reserved Interrupt 114
    DCD   FTM0_Ch0_Ch1_IRQHandler                         ; FTM0 Channel 0 and 1 interrupt
    DCD   FTM0_Ch2_Ch3_IRQHandler                         ; FTM0 Channel 2 and 3 interrupt
    DCD   FTM0_Ch4_Ch5_IRQHandler                         ; FTM0 Channel 4 and 5 interrupt
    DCD   FTM0_Ch6_Ch7_IRQHandler                         ; FTM0 Channel 6 and 7 interrupt
    DCD   FTM0_Fault_IRQHandler                           ; FTM0 Fault interrupt
    DCD   FTM0_Ovf_Reload_IRQHandler                      ; FTM0 Counter overflow and Reload interrupt
    DCD   FTM1_Ch0_Ch1_IRQHandler                         ; FTM1 Channel 0 and 1 interrupt
    DCD   FTM1_Ch2_Ch3_IRQHandler                         ; FTM1 Channel 2 and 3 interrupt
    DCD   FTM1_Ch4_Ch5_IRQHandler                         ; FTM1 Channel 4 and 5 interrupt
    DCD   FTM1_Ch6_Ch7_IRQHandler                         ; FTM1 Channel 6 and 7 interrupt
    DCD   FTM1_Fault_IRQHandler                           ; FTM1 Fault interrupt
    DCD   FTM1_Ovf_Reload_IRQHandler                      ; FTM1 Counter overflow and Reload interrupt
    DCD   FTM2_Ch0_Ch1_IRQHandler                         ; FTM2 Channel 0 and 1 interrupt
    DCD   FTM2_Ch2_Ch3_IRQHandler                         ; FTM2 Channel 2 and 3 interrupt
    DCD   FTM2_Ch4_Ch5_IRQHandler                         ; FTM2 Channel 4 and 5 interrupt
    DCD   FTM2_Ch6_Ch7_IRQHandler                         ; FTM2 Channel 6 and 7 interrupt
    DCD   FTM2_Fault_IRQHandler                           ; FTM2 Fault interrupt
    DCD   FTM2_Ovf_Reload_IRQHandler                      ; FTM2 Counter overflow and Reload interrupt
    DCD   FTM3_Ch0_Ch1_IRQHandler                         ; FTM3 Channel 0 and 1 interrupt
    DCD   FTM3_Ch2_Ch3_IRQHandler                         ; FTM3 Channel 2 and 3 interrupt
    DCD   FTM3_Ch4_Ch5_IRQHandler                         ; FTM3 Channel 4 and 5 interrupt
    DCD   FTM3_Ch6_Ch7_IRQHandler                         ; FTM3 Channel 6 and 7 interrupt
    DCD   FTM3_Fault_IRQHandler                           ; FTM3 Fault interrupt
    DCD   FTM3_Ovf_Reload_IRQHandler                      ; FTM3 Counter overflow and Reload interrupt
    DCD   DefaultISR                                      ; Default ISR
__Vectors_End


__Vectors_Size  EQU     __Vectors_End - __VECTOR_TABLE

; <h> Flash Configuration
;   <i> 16-byte flash configuration field that stores default protection settings (loaded on reset)
;   <i> and security information that allows the MCU to restrict access to the FTFL module.
;   <h> Backdoor Comparison Key
;     <o0>  Backdoor Comparison Key 0.  <0x0-0xFF:2>
;     <o1>  Backdoor Comparison Key 1.  <0x0-0xFF:2>
;     <o2>  Backdoor Comparison Key 2.  <0x0-0xFF:2>
;     <o3>  Backdoor Comparison Key 3.  <0x0-0xFF:2>
;     <o4>  Backdoor Comparison Key 4.  <0x0-0xFF:2>
;     <o5>  Backdoor Comparison Key 5.  <0x0-0xFF:2>
;     <o6>  Backdoor Comparison Key 6.  <0x0-0xFF:2>
;     <o7>  Backdoor Comparison Key 7.  <0x0-0xFF:2>
BackDoorK0      EQU     0xFF
BackDoorK1      EQU     0xFF
BackDoorK2      EQU     0xFF
BackDoorK3      EQU     0xFF
BackDoorK4      EQU     0xFF
BackDoorK5      EQU     0xFF
BackDoorK6      EQU     0xFF
BackDoorK7      EQU     0xFF
;   </h>
;   <h> Program flash protection bytes (FPROT)
;     <i> Each program flash region can be protected from program and erase operation by setting the associated PROT bit.
;     <i> Each bit protects a 1/32 region of the program flash memory.
;     <h> FPROT0
;       <i> Program Flash Region Protect Register 0
;       <i> 1/32 - 8/32 region
;       <o.0>   FPROT0.0
;       <o.1>   FPROT0.1
;       <o.2>   FPROT0.2
;       <o.3>   FPROT0.3
;       <o.4>   FPROT0.4
;       <o.5>   FPROT0.5
;       <o.6>   FPROT0.6
;       <o.7>   FPROT0.7
nFPROT0         EQU     0x00
FPROT0          EQU     nFPROT0:EOR:0xFF
;     </h>
;     <h> FPROT1
;       <i> Program Flash Region Protect Register 1
;       <i> 9/32 - 16/32 region
;       <o.0>   FPROT1.0
;       <o.1>   FPROT1.1
;       <o.2>   FPROT1.2
;       <o.3>   FPROT1.3
;       <o.4>   FPROT1.4
;       <o.5>   FPROT1.5
;       <o.6>   FPROT1.6
;       <o.7>   FPROT1.7
nFPROT1         EQU     0x00
FPROT1          EQU     nFPROT1:EOR:0xFF
;     </h>
;     <h> FPROT2
;       <i> Program Flash Region Protect Register 2
;       <i> 17/32 - 24/32 region
;       <o.0>   FPROT2.0
;       <o.1>   FPROT2.1
;       <o.2>   FPROT2.2
;       <o.3>   FPROT2.3
;       <o.4>   FPROT2.4
;       <o.5>   FPROT2.5
;       <o.6>   FPROT2.6
;       <o.7>   FPROT2.7
nFPROT2         EQU     0x00
FPROT2          EQU     nFPROT2:EOR:0xFF
;     </h>
;     <h> FPROT3
;       <i> Program Flash Region Protect Register 3
;       <i> 25/32 - 32/32 region
;       <o.0>   FPROT3.0
;       <o.1>   FPROT3.1
;       <o.2>   FPROT3.2
;       <o.3>   FPROT3.3
;       <o.4>   FPROT3.4
;       <o.5>   FPROT3.5
;       <o.6>   FPROT3.6
;       <o.7>   FPROT3.7
nFPROT3         EQU     0x00
FPROT3          EQU     nFPROT3:EOR:0xFF
;     </h>
;   </h>
;   <h> Data flash protection byte (FDPROT)
;     <i> Each bit protects a 1/8 region of the data flash memory.
;     <i> (Program flash only devices: Reserved)
;       <o.0>   FDPROT.0
;       <o.1>   FDPROT.1
;       <o.2>   FDPROT.2
;       <o.3>   FDPROT.3
;       <o.4>   FDPROT.4
;       <o.5>   FDPROT.5
;       <o.6>   FDPROT.6
;       <o.7>   FDPROT.7
nFDPROT         EQU     0x00
FDPROT          EQU     nFDPROT:EOR:0xFF
;   </h>
;   <h> EEPROM protection byte (FEPROT)
;     <i> FlexNVM devices: Each bit protects a 1/8 region of the EEPROM.
;     <i> (Program flash only devices: Reserved)
;       <o.0>   FEPROT.0
;       <o.1>   FEPROT.1
;       <o.2>   FEPROT.2
;       <o.3>   FEPROT.3
;       <o.4>   FEPROT.4
;       <o.5>   FEPROT.5
;       <o.6>   FEPROT.6
;       <o.7>   FEPROT.7
nFEPROT         EQU     0x00
FEPROT          EQU     nFEPROT:EOR:0xFF
;   </h>
;   <h> Flash nonvolatile option byte (FOPT)
;     <i> Allows the user to customize the operation of the MCU at boot time.
;     <o.0> LPBOOT
;       <0=> Low-power boot
;       <1=> Normal boot
;     <o.1> EZPORT_DIS
;       <0=> EzPort operation is disabled
;       <1=> EzPort operation is enabled
FOPT          EQU     0xFF
;   </h>
;   <h> Flash security byte (FSEC)
;     <i> WARNING: If SEC field is configured as "MCU security status is secure" and MEEN field is configured as "Mass erase is disabled",
;     <i> MCU's security status cannot be set back to unsecure state since Mass erase via the debugger is blocked !!!
;     <o.0..1> SEC
;       <2=> MCU security status is unsecure
;       <3=> MCU security status is secure
;         <i> Flash Security
;     <o.2..3> FSLACC
;       <2=> Freescale factory access denied
;       <3=> Freescale factory access granted
;         <i> Freescale Failure Analysis Access Code
;     <o.4..5> MEEN
;       <2=> Mass erase is disabled
;       <3=> Mass erase is enabled
;     <o.6..7> KEYEN
;       <2=> Backdoor key access enabled
;       <3=> Backdoor key access disabled
;         <i> Backdoor Key Security Enable
FSEC          EQU     0xFE
;   </h>
; </h>
                IF      :LNOT::DEF:RAM_TARGET
                AREA    FlashConfig, DATA, READONLY
__FlashConfig
                DCB     BackDoorK0, BackDoorK1, BackDoorK2, BackDoorK3
                DCB     BackDoorK4, BackDoorK5, BackDoorK6, BackDoorK7
                DCB     FPROT0    , FPROT1    , FPROT2    , FPROT3
                DCB     FSEC      , FOPT      , FEPROT    , FDPROT
                ENDIF


                AREA    |.text|, CODE, READONLY


Reset_Handler   PROC
                ALIGN   4
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit

                IF      :LNOT::DEF:RAM_TARGET
                REQUIRE FlashConfig
                ENDIF


                CPSID   I               ; Mask interrupts
#ifdef START_FROM_FLASH    
                ; Init ECC RAM
                LDR     R1, =__RAM_START
                LDR     R1, [R1]
                LDR     R2, =__RAM_END
                LDR     R2, [R2]
                SUBS    R2, R1
                SUBS    R2, #1
                BLE LC5
            
                MOVS    R0, #0
                MOVS    R3, #4
LC4
                STR R0, [R1]
                ADD	R1, R1, R3
                SUBS R2, #4
                BGE LC4
LC5
                NOP
#endif                
                LDR     R0, =0xE000ED08
                LDR     R1, =__VECTOR_TABLE
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =init_data_bss
                BLX     R0
                CPSIE   i               ; Unmask interrupts
                LDR     R0, =__main
                BLX      R0
                ENDP

NMI_Handler\
                PROC
                EXPORT  NMI_Handler         [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler         [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler         [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler         [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler         [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP
DefaultISR\
                PROC
                EXPORT  DefaultISR         [WEAK]
                B       .
                ENDP
Default_Handler PROC
                EXPORT  DMA0_IRQHandler                              [WEAK]
                EXPORT  DMA1_IRQHandler                              [WEAK]
                EXPORT  DMA2_IRQHandler                              [WEAK]
                EXPORT  DMA3_IRQHandler                              [WEAK]
                EXPORT  DMA4_IRQHandler                              [WEAK]
                EXPORT  DMA5_IRQHandler                              [WEAK]
                EXPORT  DMA6_IRQHandler                              [WEAK]
                EXPORT  DMA7_IRQHandler                              [WEAK]
                EXPORT  DMA8_IRQHandler                              [WEAK]
                EXPORT  DMA9_IRQHandler                              [WEAK]
                EXPORT  DMA10_IRQHandler                             [WEAK]
                EXPORT  DMA11_IRQHandler                             [WEAK]
                EXPORT  DMA12_IRQHandler                             [WEAK]
                EXPORT  DMA13_IRQHandler                             [WEAK]
                EXPORT  DMA14_IRQHandler                             [WEAK]
                EXPORT  DMA15_IRQHandler                             [WEAK]
                EXPORT  DMA_Error_IRQHandler                             [WEAK]
                EXPORT  MCM_IRQHandler                           [WEAK]
                EXPORT  FTFC_IRQHandler                              [WEAK]
                EXPORT  Read_Collision_IRQHandler                            [WEAK]
                EXPORT  LVD_LVW_IRQHandler                           [WEAK]
                EXPORT  FTFC_Fault_IRQHandler                            [WEAK]
                EXPORT  WDOG_EWM_IRQHandler                              [WEAK]
                EXPORT  RCM_IRQHandler                           [WEAK]
                EXPORT  LPI2C0_Master_IRQHandler                             [WEAK]
                EXPORT  LPI2C0_Slave_IRQHandler                              [WEAK]
                EXPORT  LPSPI0_IRQHandler                            [WEAK]
                EXPORT  LPSPI1_IRQHandler                            [WEAK]
                EXPORT  Reserved44_IRQHandler                            [WEAK]
                EXPORT  Reserved45_IRQHandler                            [WEAK]
                EXPORT  Reserved46_IRQHandler                            [WEAK]
                EXPORT  LPUART0_RxTx_IRQHandler                              [WEAK]
                EXPORT  Reserved48_IRQHandler                            [WEAK]
                EXPORT  LPUART1_RxTx_IRQHandler                              [WEAK]
                EXPORT  Reserved50_IRQHandler                            [WEAK]
                EXPORT  Reserved51_IRQHandler                            [WEAK]
                EXPORT  Reserved52_IRQHandler                            [WEAK]
                EXPORT  Reserved53_IRQHandler                            [WEAK]
                EXPORT  Reserved54_IRQHandler                            [WEAK]
                EXPORT  ADC0_IRQHandler                              [WEAK]
                EXPORT  ADC1_IRQHandler                              [WEAK]
                EXPORT  CMP0_IRQHandler                              [WEAK]
                EXPORT  Reserved58_IRQHandler                            [WEAK]
                EXPORT  Reserved59_IRQHandler                            [WEAK]
                EXPORT  ERM_single_fault_IRQHandler                              [WEAK]
                EXPORT  ERM_double_fault_IRQHandler                              [WEAK]
                EXPORT  RTC_IRQHandler                           [WEAK]
                EXPORT  RTC_Seconds_IRQHandler                           [WEAK]
                EXPORT  LPIT0_Ch0_IRQHandler                             [WEAK]
                EXPORT  LPIT0_Ch1_IRQHandler                             [WEAK]
                EXPORT  LPIT0_Ch2_IRQHandler                             [WEAK]
                EXPORT  LPIT0_Ch3_IRQHandler                             [WEAK]
                EXPORT  PDB0_IRQHandler                              [WEAK]
                EXPORT  Reserved69_IRQHandler                            [WEAK]
                EXPORT  Reserved70_IRQHandler                            [WEAK]
                EXPORT  Reserved71_IRQHandler                            [WEAK]
                EXPORT  Reserved72_IRQHandler                            [WEAK]
                EXPORT  SCG_IRQHandler                           [WEAK]
                EXPORT  LPTMR0_IRQHandler                            [WEAK]
                EXPORT  PORTA_IRQHandler                             [WEAK]
                EXPORT  PORTB_IRQHandler                             [WEAK]
                EXPORT  PORTC_IRQHandler                             [WEAK]
                EXPORT  PORTD_IRQHandler                             [WEAK]
                EXPORT  PORTE_IRQHandler                             [WEAK]
                EXPORT  SWI_IRQHandler                           [WEAK]
                EXPORT  Reserved81_IRQHandler                            [WEAK]
                EXPORT  Reserved82_IRQHandler                            [WEAK]
                EXPORT  Reserved83_IRQHandler                            [WEAK]
                EXPORT  PDB1_IRQHandler                              [WEAK]
                EXPORT  FLEXIO_IRQHandler                            [WEAK]
                EXPORT  Reserved86_IRQHandler                            [WEAK]
                EXPORT  Reserved87_IRQHandler                            [WEAK]
                EXPORT  Reserved88_IRQHandler                            [WEAK]
                EXPORT  Reserved89_IRQHandler                            [WEAK]
                EXPORT  Reserved90_IRQHandler                            [WEAK]
                EXPORT  Reserved91_IRQHandler                            [WEAK]
                EXPORT  Reserved92_IRQHandler                            [WEAK]
                EXPORT  Reserved93_IRQHandler                            [WEAK]
                EXPORT  CAN0_ORed_IRQHandler                             [WEAK]
                EXPORT  CAN0_Error_IRQHandler                            [WEAK]
                EXPORT  CAN0_Wake_Up_IRQHandler                              [WEAK]
                EXPORT  CAN0_ORed_0_15_MB_IRQHandler                             [WEAK]
                EXPORT  CAN0_ORed_16_31_MB_IRQHandler                            [WEAK]
                EXPORT  Reserved99_IRQHandler                            [WEAK]
                EXPORT  Reserved100_IRQHandler                           [WEAK]
                EXPORT  CAN1_ORed_IRQHandler                             [WEAK]
                EXPORT  CAN1_Error_IRQHandler                            [WEAK]
                EXPORT  Reserved103_IRQHandler                           [WEAK]
                EXPORT  CAN1_ORed_0_15_MB_IRQHandler                             [WEAK]
                EXPORT  Reserved105_IRQHandler                           [WEAK]
                EXPORT  Reserved106_IRQHandler                           [WEAK]
                EXPORT  Reserved107_IRQHandler                           [WEAK]
                EXPORT  Reserved108_IRQHandler                           [WEAK]
                EXPORT  Reserved109_IRQHandler                           [WEAK]
                EXPORT  Reserved110_IRQHandler                           [WEAK]
                EXPORT  Reserved111_IRQHandler                           [WEAK]
                EXPORT  Reserved112_IRQHandler                           [WEAK]
                EXPORT  Reserved113_IRQHandler                           [WEAK]
                EXPORT  Reserved114_IRQHandler                           [WEAK]
                EXPORT  FTM0_Ch0_Ch1_IRQHandler                              [WEAK]
                EXPORT  FTM0_Ch2_Ch3_IRQHandler                              [WEAK]
                EXPORT  FTM0_Ch4_Ch5_IRQHandler                              [WEAK]
                EXPORT  FTM0_Ch6_Ch7_IRQHandler                              [WEAK]
                EXPORT  FTM0_Fault_IRQHandler                            [WEAK]
                EXPORT  FTM0_Ovf_Reload_IRQHandler                           [WEAK]
                EXPORT  FTM1_Ch0_Ch1_IRQHandler                              [WEAK]
                EXPORT  FTM1_Ch2_Ch3_IRQHandler                              [WEAK]
                EXPORT  FTM1_Ch4_Ch5_IRQHandler                              [WEAK]
                EXPORT  FTM1_Ch6_Ch7_IRQHandler                              [WEAK]
                EXPORT  FTM1_Fault_IRQHandler                            [WEAK]
                EXPORT  FTM1_Ovf_Reload_IRQHandler                           [WEAK]
                EXPORT  FTM2_Ch0_Ch1_IRQHandler                              [WEAK]
                EXPORT  FTM2_Ch2_Ch3_IRQHandler                              [WEAK]
                EXPORT  FTM2_Ch4_Ch5_IRQHandler                              [WEAK]
                EXPORT  FTM2_Ch6_Ch7_IRQHandler                              [WEAK]
                EXPORT  FTM2_Fault_IRQHandler                            [WEAK]
                EXPORT  FTM2_Ovf_Reload_IRQHandler                           [WEAK]
                EXPORT  FTM3_Ch0_Ch1_IRQHandler                              [WEAK]
                EXPORT  FTM3_Ch2_Ch3_IRQHandler                              [WEAK]
                EXPORT  FTM3_Ch4_Ch5_IRQHandler                              [WEAK]
                EXPORT  FTM3_Ch6_Ch7_IRQHandler                              [WEAK]
                EXPORT  FTM3_Fault_IRQHandler                            [WEAK]
                EXPORT  FTM3_Ovf_Reload_IRQHandler                           [WEAK]
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_Error_IRQHandler
MCM_IRQHandler
FTFC_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
FTFC_Fault_IRQHandler
WDOG_EWM_IRQHandler
RCM_IRQHandler
LPI2C0_Master_IRQHandler
LPI2C0_Slave_IRQHandler
LPSPI0_IRQHandler
LPSPI1_IRQHandler
Reserved44_IRQHandler
Reserved45_IRQHandler
Reserved46_IRQHandler
LPUART0_RxTx_IRQHandler
Reserved48_IRQHandler
LPUART1_RxTx_IRQHandler
Reserved50_IRQHandler
Reserved51_IRQHandler
Reserved52_IRQHandler
Reserved53_IRQHandler
Reserved54_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
Reserved58_IRQHandler
Reserved59_IRQHandler
ERM_single_fault_IRQHandler
ERM_double_fault_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
LPIT0_Ch0_IRQHandler
LPIT0_Ch1_IRQHandler
LPIT0_Ch2_IRQHandler
LPIT0_Ch3_IRQHandler
PDB0_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
Reserved72_IRQHandler
SCG_IRQHandler
LPTMR0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
SWI_IRQHandler
Reserved81_IRQHandler
Reserved82_IRQHandler
Reserved83_IRQHandler
PDB1_IRQHandler
FLEXIO_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
Reserved88_IRQHandler
Reserved89_IRQHandler
Reserved90_IRQHandler
Reserved91_IRQHandler
Reserved92_IRQHandler
Reserved93_IRQHandler
CAN0_ORed_IRQHandler
CAN0_Error_IRQHandler
CAN0_Wake_Up_IRQHandler
CAN0_ORed_0_15_MB_IRQHandler
CAN0_ORed_16_31_MB_IRQHandler
Reserved99_IRQHandler
Reserved100_IRQHandler
CAN1_ORed_IRQHandler
CAN1_Error_IRQHandler
Reserved103_IRQHandler
CAN1_ORed_0_15_MB_IRQHandler
Reserved105_IRQHandler
Reserved106_IRQHandler
Reserved107_IRQHandler
Reserved108_IRQHandler
Reserved109_IRQHandler
Reserved110_IRQHandler
Reserved111_IRQHandler
Reserved112_IRQHandler
Reserved113_IRQHandler
Reserved114_IRQHandler
FTM0_Ch0_Ch1_IRQHandler
FTM0_Ch2_Ch3_IRQHandler
FTM0_Ch4_Ch5_IRQHandler
FTM0_Ch6_Ch7_IRQHandler
FTM0_Fault_IRQHandler
FTM0_Ovf_Reload_IRQHandler
FTM1_Ch0_Ch1_IRQHandler
FTM1_Ch2_Ch3_IRQHandler
FTM1_Ch4_Ch5_IRQHandler
FTM1_Ch6_Ch7_IRQHandler
FTM1_Fault_IRQHandler
FTM1_Ovf_Reload_IRQHandler
FTM2_Ch0_Ch1_IRQHandler
FTM2_Ch2_Ch3_IRQHandler
FTM2_Ch4_Ch5_IRQHandler
FTM2_Ch6_Ch7_IRQHandler
FTM2_Fault_IRQHandler
FTM2_Ovf_Reload_IRQHandler
FTM3_Ch0_Ch1_IRQHandler
FTM3_Ch2_Ch3_IRQHandler
FTM3_Ch4_Ch5_IRQHandler
FTM3_Ch6_Ch7_IRQHandler
FTM3_Fault_IRQHandler
FTM3_Ovf_Reload_IRQHandler

                B       .

                ENDP

                ALIGN
                END