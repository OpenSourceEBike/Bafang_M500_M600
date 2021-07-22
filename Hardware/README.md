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

![m600specs](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/battery.jpg)

## Power

##### m600

The m600 is available in the following variants:
- 36v/15A, 
- 36v/25A, 
- 43v/20A
- 48v/10A 
- 48v/18A 

Most of these differences are in firmware and not in hardware, so there might be more or less variants out there than listed.
It is, however, possible to run the 43v/20A firmware with a 48v battery, this would increase the max power output, but you might risk the battery being deep-drained, due to a too low cutoff voltage in the controller.

It's also possible to run the 48v/18a variant on a 52v battery, as indicated here:
https://www.youtube.com/watch?v=X2IPtda_LFo

It should be noted that there is a safety cutoff for 48v controllers, that disables the motor if the battery voltage is above 58v. You should charge the 52v cells accordingly.

The reason Bafang does not supply a 52v battery (and you also don't have to try and ask them for it), is because the EU does not allow more than 48v charger connections  CE certification.
The difference between 52v or 48v are, however, very small so it wouldn't actually pose a risk. But for Bafang they cannot supply it within regulations.

##### m500

The m500 is available in a 36v/15a, 43v/?A and a 48v/?A varaiant.


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

It should technically be possible to transplant a m600 module into a m500, however: one would need to change the wiring from the board to the motor (phase wires) in such a way that the colors line up.
The differences are mostly the fact that the m500 has 1 instead of 2 shunt resistors in the m600 and the m600 seems to have one or more extra MOSFETS

![controllercomparison](https://github.com/OpenSourceEBike/Bafang_M500_M600/raw/main/Hardware/img/controllercomparison.jpg)