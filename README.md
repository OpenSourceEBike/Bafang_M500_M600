# Bafang M500 and M600

Tecnhical notes about Bafang M500 and M600, see the [forum message](https://endless-sphere.com/forums/viewtopic.php?f=28&t=100777).

## How to colaborate
Please fork this repository, make your changes and then submit a pull request.

## The CANBUS protocol
- Newer Bafang hardware uses a CANBUS protocol for all internal communications.
- A short overview of the currently known CANBUS registers is available here (sniffed from a Bafang display with an Ananda Controller): https://github.com/stancecoke/BionX_Minimal_Translator/blob/Sniffer_for_Bafang/Inc/CAN-Registers.h

## Notes on the Bafang BESST Software

- WORST security ever.
One could write a complete alternative backend for BEST in about a week or two, Spoofing the login is easily and the whole UI is just an Electron App, so you can just read the Javascript sourcecode yourself. However:
- BESST does not contain the UI elements required to modify firmware
- The settings that can be altered with the right login, are limited. There is no UI or backend element to change the current limit or voltage for example. However: Reading the code I cannot exclude that it is, in fact, possible to use the canbus to alter these settings.
- The BESST code does seem to include some references where certain values can be found on the canbus for it's GET, the users working at reverse enginering the CANBUS should be able to use the Javascript UI to reverse engineer the correct bussaddresses for those values and start trying to SET those.
- There is no firmware validation in the BESST software.


## Notes on the Bafang BESST hardware tool
- It looks like the BESST tool (the physical thing) connects to the PC using UART which uses a generic Silicon Labs CP210x chip
- The BESST hardware tool translates those UART commands to CANBUS commands
- We could try to test the responses if we try to set or get values we are not supposed to, by simply editing the BESST software source

## Notes on reverse enginering the official firmware

- According to entrophy scanners, the firmware files are not encrypted.
- Comparing different versions, indicate some obfustication might be going on, but we can clearly deff the actual hex differences.
- After running Binwalk it seems the code is for ARM, little endian based cortex hardware with THUMB instructions
- Initial tries with GHIDRA or IDA Pro to decompile where not (yet) successfull

## Basic reprogramming using CAN commands

- [CiDi](https://endless-sphere.com/forums/memberlist.php?mode=viewprofile&u=74090) was able to reverse engineer the commands for changing 25/45/60kmh mode, wheel size, circumference, tourque and position calibration without genuine Bafang hardware (BESST tool). See [ES#p1595302](https://endless-sphere.com/forums/viewtopic.php?f=28&t=100777&start=400#p1595302) Works for M500 & M600

## Python trick for BEEST software

- [CiDi](https://endless-sphere.com/forums/memberlist.php?mode=viewprofile&u=74090) also wrote a python script to fool the BESST software, to use it without any licenced Bafang account. Script from [ES#p1617500](https://endless-sphere.com/forums/viewtopic.php?f=28&t=100777&start=500#p1617500). Software from [Bafang OEM](https://bafang-e.com/en/oem-area/service/besst/) Works with currently latest 1.2.26