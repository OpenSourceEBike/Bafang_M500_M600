### S32 SDK Build Makefile ###
# This makefile will build a bare metal S32 SDK application.
# Some make variables need to be defined here so make targets can be built correctly.
#
# Prerequisites:
#   - an arm toolchain
#   - GNU Make or an Unix-like environment
#
# Application specific variables that must be defined:
#   - CSOURCES, ASMSOURCES: C and assembly sources, respectively. Note: only files, without paths
#   - SOURCEDIR: Locations for all source and header files. The paths defined here will be used:
#       1)to match source files defined in CSOURCES and ASMSOURCES 
#       2)as include path for the compiler
#   - OBJECTDIR: object directory to save object and intermediate files. Objects will be generated
#       as a flat list (no subdirectories).
# Common make variables. These are recommended to be defined, but can be omitted:
#   - CFLAGS: flags to pass to the C compiler
#   - ASFLAGS: flags to pass to the assembler
#   - LDFLAGS: flags to pass to the linker
#   - EXECUTABLE: name and location of the executable (.elf file)
# Variables that define the toolchain:
#   - CC: C compiler executable
#   - AS: Assembler executable
#   - LD: Linker executable
#   - RM: removed command (default 'rm -f')
#   - CCSIZE: Size executable (prints sections sizes: text, data, bss)
#   - CCOBJDUMP: Objdump executable
#
# Most variables have default values defined, but it is recommended to define 

## Define CSOURCES and ASMSOURCES variables ##
CSOURCES:=main.c
CSOURCES+=system_S32K142.c
CSOURCES+=startup.c
CSOURCES+=pins_driver.c
CSOURCES+=pins_port_hw_access.c
CSOURCES+=clock_S32K1xx.c
CSOURCES+=interrupt_manager.c

ASMSOURCES:=startup_S32K142.s

## Define SOURCEDIR variable ##
# Define two helper vars, BASEDIR (SDK base) and SOURCEDIRSDK (paths inside SDK to be included/searched)
BASEDIR:=../../../../../
SOURCEDIRSDK:=platform/
SOURCEDIRSDK+=platform/drivers/src/
SOURCEDIRSDK+=platform/drivers/src/clock/
SOURCEDIRSDK+=platform/drivers/src/interrupt/
SOURCEDIRSDK+=platform/drivers/src/clock/S32K1xx/
SOURCEDIRSDK+=platform/devices/
SOURCEDIRSDK+=platform/devices/S32K142/
SOURCEDIRSDK+=platform/devices/S32K142/startup/
SOURCEDIRSDK+=platform/devices/S32K142/startup/dcc/
SOURCEDIRSDK+=platform/devices/S32K142/include/
SOURCEDIRSDK+=platform/devices/common
SOURCEDIRSDK+=platform/drivers/inc/
SOURCEDIRSDK+=platform/drivers/src/adc/
SOURCEDIRSDK+=platform/drivers/src/flash/
SOURCEDIRSDK+=platform/drivers/src/lpuart/
SOURCEDIRSDK+=platform/drivers/src/lpi2c/
SOURCEDIRSDK+=platform/drivers/src/pins/
# Location of objects and final source directories

OBJECTDIR_NAME:=obj
SOURCEDIR_NAME:=../Sources

ASM_EXT:=s

# Map file names
MAP_RAM:=app_ram.map
MAP_FLASH:=app_flash.map

## Elf file names ##
EXECUTABLE_RAM:=app_ram.elf
EXECUTABLE_FLASH:=app_flash.elf

## Toolchain executables ##
CC:=dcc
AS:=dcc
LD:=dld
CCSIZE:=ddump -Z

## Toolchain flags ##
TARGET = -tARMCORTEXM4LV
ASFLAGS_RAM:= $(TARGET) -g3 -Xpreprocess-assembly
ASFLAGS_FLASH:= $(TARGET) -g3 -Xpreprocess-assembly -DSTART_FROM_FLASH
ASFLAGS_RAM+= -Xdebug-dwarf2
ASFLAGS_RAM+= -Xdebug-local-all
ASFLAGS_RAM+= -Xdebug-local-cie
ASFLAGS_RAM+= -Xdebug-struct-all
ASFLAGS_FLASH+= -Xdebug-dwarf2
ASFLAGS_FLASH+= -Xdebug-local-all
ASFLAGS_FLASH+= -Xdebug-local-cie
ASFLAGS_FLASH+= -Xdebug-struct-all

CFLAGS:= $(TARGET) -mthumb
CFLAGS+=-Xdialect-c99
CFLAGS+=-g3
CFLAGS+=-DCPU_S32K142
CFLAGS+= -Xdebug-dwarf2
CFLAGS+= -Xdebug-local-all
CFLAGS+= -Xdebug-local-cie
CFLAGS+= -Xdebug-struct-all
CFLAGS+= -ei5388,5387,1824

LDFLAGS_RAM:=$(TARGET) -Xremove-unused-sections -lc -Xpreprocess-lecl
LDFLAGS_RAM+= "$(BASEDIR)\platform\devices\S32K142\linker\dcc\S32K142_32_ram.dld" -e Reset_Handler
LDFLAGS_RAM+= -m6 > $(MAP_RAM)
LDFLAGS_FLASH:=$(TARGET) -Xremove-unused-sections -lc -Xpreprocess-lecl
LDFLAGS_FLASH+="$(BASEDIR)\platform\devices\S32K142\linker\dcc\S32K142_32_flash.dld" -e Reset_Handler
LDFLAGS_FLASH+= -m6 > $(MAP_FLASH)

include Makefile-arm
