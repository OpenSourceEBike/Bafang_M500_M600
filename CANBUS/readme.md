# CANBUS

## Summary

These documents, dumps and projects explain everything we currently know about the Bafang CANBUS protocol

## Pinout

```
HIGO-B5-Female:
    ────────┐
 /    1   5 │
│ 2         │
 \    3   4 │
    ────────┘
1 Orange: VCC/Ctrl
2 White	: CL
3 Brown	: P+
4 Green	: CH
5 Black	: GND

Bit Rate 250 kBit/s
```

##### Level matching:
|L9|L5|L3|Byte|
|---|---|---|---|
|Walk|Walk|Walk|06|
|0|0|0|00|
|1|||01|
|2|1||0B|
|3||1|0C|
|4|2||0D|
|5||2|02|
|6|3||15|
|7|||16|
|8|4||17|
|9|5|3|03|


##### The following Canbus frameid's have been identified:

|Function|GET/SET|second-last-digit|last-digit|
|---|---|---|---|
|announceUpgrade|20|00|
|startUpgradeTransfer|20|01|
|pulse|GET|30|00|
|unknown|?|30|05|
|sensorSignal|GET|31|00|
|controllerInformation|GET|32|00|
|controllerInformation_1|GET|32|01|
|controllerInformation_2|GET|32|02|
|controllerInformation_3|GET/SET|32|03|
|controllerInformation_4|GET|32|04|
|controllerInformation_5|GET|32|05|
|batteryInformation|GET|34|00|
|batteryInformation_1|GET|34|01|
|batteryInformation_2|GET|34|02|
|besst|GET|35|00|
|transmissionInformation|GET|36|00|
|transmissionInformation_1|GET|36|01|
|transmissionInformation_2|GET|36|02|
|transmissionInformation_3|GET|36|03|
|eletronicLockInformation|GET|37|00|
|hardwareVersionNumber|GET/SET|60|00|
|softwareVersionNumber|GET|60|01|
|modelNumber|GET/SET|60|02|
|serialNumber|GET/SET|60|03|
|customerNumber|GET/SET|60|04|
|manufacturer|GET/SET|60|05|
|params|GET/SET|60|06|
|errorCode|GET/SET|60|07|
|bootLoad|GET/SET|60|08|
|conParams_1|GET/SET|60|11|
|conParams_2|GET/SET|60|12|
|conParams_3|GET/SET|60|13|
|conParams_4|GET/SET|60|14|
|conParams_5|GET/SET|60|15|
|conParams_6|GET/SET|60|16|
|conParams_7|GET/SET|60|17|
|conParams_8|GET/SET|60|18|
|sensorCalibration|GET|61|00|
|positionSensorCalibration|SET|62|00|
|clear_Hmi_Total_Mileage|SET|62|01|
|set_Hmi_Time|SET|62|02|
|clear_Hmi_Single_Mileage|SET|62|03|
|controllerFeatures|GET|62|15|
|dataOfHmi|GET|63|00|
|dataOfHmi_1|GET|63|01|
|dataOfHmi_2|GET/SET|63|02|
|dataOfHmi_3|GET|63|03|
|dataOfHmi_4|GET|63|04|
|designInformation|GET|64|?|
|chargingInformation|GET|64|01|
|batteryCellsInformation|GET|64|02|
|batteryCellsInformation_1|GET|64|03|
|batteryCellsInformation_2|GET|64|04|
|batteryCellsInformation_3|GET|64|05|
|batteryCellsInformation_4|GET|64|06|
|batteryCellsInformation_5|GET|64|07|
|batteryCellsInformation_6|GET|64|08|
|batteryCellsInformation_7|GET|64|09|
|batteryCharacteristics|GET|64|15|
|set_Hmi_Card|SET|65|00|

## Global Enums in BESST code

##### Source and target devices on the canbus.

```
Torque sensor   : 0x01
Controller      : 0x02
HMI             : 0x03
Battery         : 0x04
BESST interface : 0x05
Advanced        : 0x1f
```

##### References for the BESST USB tool used to send certain CANBUS commands

```
WRITE_CMD           : 0x00
READ_CMD            : 0x01
NORMAL_ACK          : 0x02
ERROR_ACK           : 0x03
LONG_START_CMD      : 0x04
LONG_TRANG_CMD      : 0x05
LONG_END_CMD        : 0x06
LONG_WARNING_CMD    : 0x07
```

```
OPT_HARDWARE_VERSION    : 0x00
OPT_SOFTWARE_VERSION    : 0x01
OPT_MODEL_VERSION       : 0x02
OPT_SN                  : 0x03
OPT_CLIENT_NO           : 0x04
```

These can be used using our CANBUS frameid calculator here:
https://ornias1993.github.io/Bafang_M500_M600/index.html

## Known working CANBUS commands

##### BESST Speed/Wheel/Circumference Setup

```
Decoding example: 05103203 6 70 17 B5 01 C0 08
ID: 05103203
Speed Limit Byte   1/0	: 70 17 -> 0x1770              = 6000   => 60.00km/h
Wheel Size Byte    3/2	: B5 01 -> 0x01B5 -> 0x01B . 5 = 27 . 5 => 27.5"
Circumference Byte 5/4	: C0 08 -> 0x08C0              = 2240   => 2240mm

Decoding example: 05103203 6 C4 09 D0 01 C0 08
ID: 05103203
Speed Limit Byte   1/0	: C4 09 -> 0x09C4              = 2500   => 25.00km/h
Wheel Size Byte    3/2	: D0 01 -> 0x01D0 -> 0x01D . 0 = 29 . 0 => 29.0"
Circumference Byte 5/4	: E8 08 -> 0x08E8              = 2280   => 2280mm

Little-Endian byte order!
```

##### BESST Torque Sensor Calibration

```
ID: 05106101
Numbers Byte: 0
```

##### BESST Position Sensor Calibration

```
ID: 05116201
Numbers Bytes: 0
```

##### Controller % Battery/Cadans/Torque/Range Informations

```
Decoding example: 02F83200 8 32 0B 00 22 38 01 34 00
ID: 02F83200
Numbers Byte: 8
% Battery Byte 0	                    : 0x32        -> 0x32        = 50 => 50%
Distance since power on Byte 2/1        : 0x00 0x0B   -> 0x000B      = 11 => 0,11KM
Cadans Byte 3                           : 0x22        -> 0x22        = 34 => 34RPM
mV Torque Byte 4/5                      : 0x01 0x38   -> 0x138       = 312 => 312mV
KM range Byte 6/7			            : 0x00 0x34   -> 0x0034      = 52 => 52KM
```

##### Controller Speed/Current/Voltage/Temperature Informations

```
Decoding example: 02F83201 8 C4 09 E8 03 E2 14 32 3C
ID: 02F83201
Numbers Byte: 8
Speed Byte   1/0	: C4 09 -> 0x09C4 = 2500 => 25.00km/h
Current Byte 3/2	: E8 03 -> 0x03E8 = 1000 => 10.00A
Voltage Byte 5/4	: E2 14 -> 0x14E2 = 5346 => 53.46V
Temp. Control. Byte 6	: 32    -> 0x32   = 50   -> 50 - 40 = 10 => 10°C
Temp. Motor Byte 7	: 3C    -> 0x3C   = 60   -> 60 - 40 = 20 => 20°C

Little-Endian byte order!
```

##### Controller Speed/Wheel/Circumference Informations

```
Decoding example: 02F83203 6 70 17 B5 01 C0 08
ID: 02F83203
Speed Limit Byte   1/0 : 70 17 -> 0x1770              = 6000   => 60.00km/h
Wheel Size Byte    3/2 : B5 01 -> 0x01B5 -> 0x01B . 5 = 27 . 5 => 27.5"
Circumference Byte 5/4 : C0 08 -> 0x08C0              = 2240   => 2240mm

Decoding example: 02F83203 6 C4 09 D0 01 C0 08
ID: 02F83203
Speed Limit Byte   1/0 : C4 09 -> 0x09C4              = 2500   => 25.00km/h
Wheel Size Byte    3/2 : D0 01 -> 0x01D0 -> 0x01D . 0 = 29 . 0 => 29.0"
Circumference Byte 5/4 : E8 08 -> 0x08E8              = 2280   => 2280mm

Little-Endian byte order!

```

##### Controller State Informations

```
ID: 02FF1200
Numbers Byte: 1
Bit0 Brake state          : 1=Brake
Bit1 Motor stopped        : 1=Stopped
Bit2 Battery undervoltage : 1=Undervoltage
```

##### HMI Level/Light Setup

```
ID: 03106300
Numbers Byte: 4
Levels number Byte 0	: 03 / 05 / 09
Set Level Byte 1		: Walk = 06 / 0 = 00 / 1 = 01 / 2 = 0B / 3 = 0C / 4 = 0D / 5 = 02 / 6 = 15 / 7 = 16 / 8 = 17 / 9 = 03
Button "+" Byte 2      	: Off = 00 / On = 02 / Off with light = 01 / On with light = 03
Boost mode Byte 3	    : Off = 01 / On = 00 (after two seconds of pressing the power button)
```

##### Torque Sensor

```
ID: 01F83100
Numbers Byte: 4
Torque Byte 0/1		: EE 02
Cadence Byte 2		: 00
Progressive Byte 3	: 01
```

## CAN messages during operations

##### HMI

```
03106300 every 100ms
```

##### BESST

```
05F83501 every 1016ms
```

##### Controller (Only with the presence of the HMI or BESST)

```
02F83200 every 1500ms
02F83201 every 280ms
02F83202 every 100ms
02F83203 every 450ms
02F83205 every 300ms
02FF1200 every 490ms
```

##### Torque Sensor

```
01F83100 every 10ms
```

##### Battery

```
04F83400 every 200ms
04F83401 every 100ms
04F83402 every 500ms
```

## CAN messages ondemand

#### Torque Sensor

```
03096000 Hardware Version Number
03096001 Software Version Number
03096002 Model Number
03096003 Serial Number
03096004 Customer Number
```

#### Controller

```
03116000 Hardware Version Number
03116001 Software Version Number
03116002 Model Number
03116003 Serial Number
```

#### Battery

```
03216000 Hardware Version Number
03216001 Software Version Number
03216002 Model Number
03216003 Serial Number
03216004 Customer Number
```

## CANBUS controller update procedure

**1. Start continuesly announcing that the host (pc) is ready to transfer a firmware update:**

M500
``
05FF3005 - 00 ( BESST "waiting in firmware update mode" or "Trying to transfer")
05112000 - 88 45 02 00 (Polling: "Is Controller ready for firmware?")
`` 

M600
``
05FF3005 - 00 ( BESST "waiting in firmware update mode" or "Trying to transfer")
05112000 - 89 45 02 00 (Polling: "Is Controller ready for firmware?")
`` 

**2. Wait for Controller response:**
Once you recieve this confirmation, you can stop announcing as stated in step 1.

M500
```
022A2000 - 88 45 82 (Response: "Controller ready to recieve firmware")
```

M600
```
022A2000 - 89 45 82 (Response: "Controller ready to recieve firmware")
```

**3. send first package:**

This package contains the length of the file, minus the first 16 hex bytes, transformed into hex.
We will use ## ## ## to represent this value.

``
05142001 - ## ## ## (Transfer started from BESST, contains length in hex)

**4. wait for response first package:**

We now need to wait for the response from the controller before actually starting to send the firmware

``
022A2001 - 00 (Transfer start ack from Controller)
`` 

**5. send data:**

We now send the firmware in 8 byte chunks, minus the first 16 hex bytes. 
Each chunk will be numbered in hexadecimal, starting from 0000. After each chunk we will recieve a response verifying the transfer has been completed successfully.
We will use #### to represent the ever incrementing chunk numbers. While we list only one response, ofcoarse this means multiple chunks get send with incrementing frame numbers.
We will use XX XX XX XX XX XX XX XX to represent the file content that is getting transfered.

Do not send the last data package this way though...

Data send:
```
0515#### - XX XX XX XX XX XX XX XX (Data transfer packages)
```

Response from controller:
```
022A#### - (Data transfer packages ack from controller)
```

**6. send last data and end transfer:**

This package needs to go towards a different frameID, to signal the controller that the transfer has been completed.
The content of the package can vary in length up to 8 bytes, which we will simulate by using XX.
The #### is still the autoincrement number from step 5.

Data send:
```
0516#### - XX (Transfer completed from BESST)
```

Response from controller:
```
022A#### - (Data transfer packages ack from controller)
```

