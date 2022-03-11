# Bafang M500 and M600 Modification Project

This project aims to provide guidance for modifying the Bafang m500 and m600 motors. Eventually we hope to be able to provide a fully opensource firmware, but for the time being we provide hacked firmwares and documentation

## How to colaborate

Help test our firmwares, join discussions, fork the repository or send us relevant information either by github-issue or email directly.

## Ideas for projects
### DIY OpenSource display
Adapt the [EBike DIY OpenSource display](https://opensourceebike.github.io/) to control and show the motor data. We know the CAN commands to read data from the controller and to change max speed and wheel diameter. It should also be possible to flash the motor firmware. A display button combination can be used to quick change max speed (offroad mode).

Needed steps:
* Adapt the [EBike DIY OpenSource display](https://opensourceebike.github.io/) hardware by adding a SPI CAN module (cheap Arduino CAN module) to be able to communicate with Bafang M500/M600 motors
* Add the CAN commands to the firmware and adapt the information being shown on the screen 

Pictures pf [EBike DIY OpenSource display](https://opensourceebike.github.io/):
![](diy_display.png)


## Project structure

- `/BESST` -  Provides sourcecode and older versions to research the BESST software package
- `/CANBUS` - Provides documentation on the inner workings of the Bafang CANBUS protocol
- `/Firmwares/Official` - Provides firmwares originally provided by Bafang themselves
- `/Firmwares/Custom` - Provides hacked firmwares created by this projects, that contain customised settings
- `/hardware` - Provides Documentation for everything we know about the Bafang hardware and it's controller

## Disclaimer

All the code, programs, dumps, images and information in this repository is supplied solely for educational use.
