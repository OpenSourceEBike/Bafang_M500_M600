# Hardware Information for m500 and m600

## Connectors

The connectors use a so called "Higo Mini f" connector for all wiring
See:
https://www.higoconnector.com/news/compact-transmission-connector-range


##### Connection diagram

![connectiondiagram](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/Bafang%20Diagram.png)

### m600 Teardown

A good teardown video for the m600 is available on youtube:
https://www.youtube.com/watch?v=muSNIUXNNaQ


## Specifications

##### m500
![m500specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/m500%20specs.PNG)



##### m600
![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/m600%20specs.PNG)



## Battery

For both models the official Bafang battery includes a connector that might in the future include CANBUS integration for the battery.
However: The motor controller, as of July 2021, does not require the battery to be CANBUS connected to function. This means connecting the battery power-wires to the motor controller is enough.  

A second side effect of the unused CANBUS protocol is that the battery connector (and mating bicyle connector) both contain 3 unused terminals which are identical to the terminals used for + and - battery power.  With minimal modification, two of the extra terminals on both the battery and bicycle side can be 'wired in' allowing two connectors to carry current for bat+ and two for bat-, effectively halving the current and IR^2 heating any one connector would see. 

![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/battery.jpg)

## Power

##### m600

The m600 is available in the following variants:
- 36v/15A
- 36v/25A
- 43v/20A
- 48v/10A 
- 48v/18A 


##### m500

The m500 is available in the following variants:
- 36v/15a
- 36v/17a
- 48v/10A 
- 48v/12A 

##### Custom power setups

Most of these differences are in firmware and not in hardware, so there might be more or less variants out there than listed.
It is, however, possible to run the 43v/20A firmware with a 48v battery, this would increase the max power output, but you might risk the battery being deep-drained, due to a too low cutoff voltage in the controller.

It's also possible to run the 48v/18a variant on a 52v battery, as indicated here:
https://www.youtube.com/watch?v=X2IPtda_LFo

It should be noted that there is a safety cutoff for 48v controllers, that disables the motor if the battery voltage is above 58v. You should charge the 52v cells accordingly.  It should further be noted the 'weakest' (voltage tolerance) components found so far (on an M600 board) are driver chips for the on-board SMPS.  Each of the three SMPS use an MP2459 driver with a maximum rated voltage of 55V and an 'absolute maximum' rating of 60V.

The reason Bafang does not supply a 52v battery (and you also don't have to try and ask them for it), is because the EU does not allow more than 48v charger connections  CE certification.
The difference between 52v or 48v are, however, very small so it wouldn't actually pose a risk. But for Bafang they cannot supply it within regulations.

## Gearing

The m500 and m600 are mostly similair, with some exceptions:

- The m500 seems to still be using 1 plastic gear, where the m600 is not.
- The gears of the two motors have a slightly different pitch

Gear ratio's:
m600: 6:27
m500: 7:32

Here are a few pictures of people trying to find the stock gearing part numbers:
`TODO: Insert images`

## Controller hardware module


##### Teardown

These pictures highlight the most important components on the Controller Hardware Module


![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/X1_Ludicrous_Controller_M600_-_Annotated_Top.jpg)

![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/X1_Ludicrous_Controller_M600_-_Annotated_Bottom.jpg)



##### Controller SoC

The controller SoC seems to be a S32K142H NXP SoC

Datasheet:
https://github.com/OpenSourceEBike/Bafang_M500_M600/blob/main/Hardware/S32K-DS.pdf

##### MOSFETS

The M500 board contains 2 MOSFETS (one high, one low) for each PHASE for a total of 6.  The M600 board uses 4 MOSFETS for each phase (two high, two low) for a total of 12.  The second parallel MOSFET effectively doubles current capacity of the M600 board.

The MOSFET(s) seem to be HGN036N08S, which are 80V/60A rated.  Questions of adequate / optimal cooling arise, especially since the mosfets are mainly cooled through the bottom of the circuit board and several are topped by a blob of hot glue retaining electrolytic capacitors. However, the datasheet lists a typical RDSon (resistance drain-to-sink) of ~3 milliohms.  Meaning at full rated current the mosfet is generating approximately 10 watts of heat.  Considering even the M600 board may only push 30-40amps for short periods, and that amperage is spread across two mosfets, it is clear each mosfet only needs to dissipate low-to-mid single digit watts worth of heat under the most demanding conditions.  'Through the board' cooling is likely adequate for most intended purposes.

Datasheet:
https://github.com/OpenSourceEBike/Bafang_M500_M600/blob/main/Hardware/HGN036N08S_V1.0-061417.pdf

##### Power measurement and phase control

The board contains 1 or 2 shunt resistors in parallel measuring the total powerconsumption.  The M600 'ludicrous' board appears to use 
two 0.01 ohm, 3 watt, 2512 SMD resistors in parallel for a total resistance of 0.005 ohms @ 6 watts.

However the board does not contain any shunt resistors on the indivudual phases. How the single shunt resistor(s) can be used in this regard, is highlighted in the following documentation:

![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/3-phase_bridge_motor_driver_example.jpg)
https://github.com/OpenSourceEBike/Bafang_M500_M600/blob/main/Hardware/AN5327.pdf

##### m500 vs m600

It should technically be possible to transplant a m600 module into a m500, however: one would need to change the wiring from the board to the motor (phase wires) in such a way that the colors line up.
The differences are mostly the fact that the m500 has 1 instead of 2 shunt resistors in the m600 and the consideration of the M600 board
having double the mosfets for double the current carrying capacity.

![controllercomparison](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/controllercomparison.jpg)
