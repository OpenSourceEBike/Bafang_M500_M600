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
1 Orange: VCC
2 White	: CL
3 Brown	: P+
4 Green	: CH
5 Black	: GND

Bit Rate 250 kBit/s

Level matching:
L9  L5  L3	
0   0   0	: FF FF
1-2 1 		: 40 1F
3-4 2   1	: 2C 1A
5-6 3   2	: EC 13
7-8 4		: AC 0D
9   5   3	: 8C 0A
```


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

##### source and target devices on the canbus.

BESST normally sends from BST (0x05) to a target, most often HMI (0x03)
It's not 100% sure how these translate to the CANBUS frameid and data packages

```
/**
 * @enum {hex}
 * @readonly
 */
module.exports.DEVICE = {
    SEN: 0x01,
    CTL: 0x02,
    HMI: 0x03,
    BAT: 0x04,
    BST: 0x05,
    ADV: 0x1f,
}
```


##### References for the BESST USB tool used to send certain CANBUS commands

```
/**
 * @enum {hex}
 * @readonly
 */
module.exports.CAN = {
    WRITE_CMD: 0x00,
    READ_CMD: 0x01,
    NORMAL_ACK: 0x02,
    ERROR_ACK: 0x03,
    LONG_START_CMD: 0x04,
    LONG_TRANG_CMD: 0x05,
    LONG_END_CMD: 0x06,
    LONG_WARNING_CMD: 0x07,
}
```

```
/**
 * @enum {hex}
 * @readonly
 */
module.exports.OPT = {
    OPT_HARDWARE_VERSION: 0x00,
    OPT_SOFTWARE_VERSION: 0x01,
    OPT_MODEL_VERSION: 0x02,
    OPT_SN: 0x03,
    OPT_CLIENT_NO: 0x04
}
```

These can be used using our CANBUS frameid calculator here:
https://ornias1993.github.io/Bafang_M500_M600/index.html

## Known working CANBUS commands

##### BESST Speed/Wheel/Circumference Setup

```
ID: 05103203
Numbers Byte: 6
Speed Limit Byte 0/1	: 60.00km/h(1770Hex) = 70 17 / 25.00km/h(9C4Hex) = C4 09
Wheel Size Byte 2/3		: 29.0(1DHex) = D0 01 / 27.5(1B5Hex) = B5 01
Circumference Byte 4/5	: 2280(8E8Hex) = E8 08 / 2240mm(8C0Hex) = C0 08
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

##### Controller Level Informations

```
ID: 02F83200
Numbers Byte: 8
Byte 0/1				: 
Byte 2/3				: 
Byte 4/5				: 
Level Byte 6/7			: 0 = FF FF / 1= 40 1F / 2= 2C 1A / 3 = EC 13 / 4 = AC 0D / 5 = 8C 0A
```

##### Controller Speed/Current/Temperature/Voltage Informations

```
ID: 02F83201
Numbers Byte: 8
Speed Byte 0/1			: 25.00km/h(9C4Hex) = C4 09
Current					: 10.00A(3E8Hex) = E8 03
Voltage Byte 6/7		: 53.46V(14E2Hex) = E2 14
Temperature Byte 4/5	: 10.00°C = 10+40=50(32Hex) = 00 32
```

##### Controller Speed/Wheel/Circumference Informations

```
ID: 02F83203
Numbers Byte: 6
Speed Limit Byte 0/1	: 60.00km/h(1770Hex) = 70 17 / 25.00km/h(9C4Hex) = C4 09
Wheel Size Byte 2/3		: 29.0(1DHex) = D0 01 / 27.5(1B5Hex) = B5 01
Circumference Byte 4/5	: 2280(8E8Hex) = E8 08 / 2240mm(8C0Hex) = C0 08
```


##### HMI Level/Light Setup

```
ID: 03106300
Numbers Byte: 4
Level number Byte 0		: 05
Set Level Byte 1		: 0 = 00 / 1 = 0B / 2 = 0D / 3 = 15 / 4 = 17 / 5 = 03
Button Set level Byte 2	: Change Off = 00 / Change On = 02 / Change Off with light = 01 / Change On with light = 03
On/Off Byte 3			: Off = 00 / On = 01
```

##### HMI Speed/Wheel/Circumference Setup

```
ID: 03103203
Numbers Byte: 6
Speed Limit Byte 0/1	: 60.00km/h(1770Hex) = 70 17 / 25.00km/h(9C4Hex) = C4 09
Wheel Size Byte 2/3		: 29.0(1DHex) = D0 01 / 27.5(1B5Hex) = B5 01
Circumference Byte 4/5	: 2280(8E8Hex) = E8 08 / 2240mm(8C0Hex) = C0 08
```

## CANBUS controller update procedure

**1. Start continuesly announcing that the host (pc) is ready to transfer a firmware update:**

m500
``
05FF3005 - 00 ( BESST "waiting in firmware update mode" or "Trying to transfer")
05112000 - 88 45 02 00 (Polling: "Is Controller ready for firmware?")
`` 

m600
``
05FF3005 - 00 ( BESST "waiting in firmware update mode" or "Trying to transfer")
05112000 - 89 45 02 00 (Polling: "Is Controller ready for firmware?")
`` 

**2. Wait for Controller response:**
Once you recieve this confirmation, you can stop announcing as stated in step 1.

m500
```
022A2000 - 88 45 82 (Response: "Controller ready to recieve firmware")
```

m600
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

