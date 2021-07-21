# Hardware Information for m500 and m600

## Connectors

The connectors use a so called "Higo Mini f" connector for all wiring
See:
https://www.higoconnector.com/news/compact-transmission-connector-range


##### Connection diagram

`TODO: insert image`

### m600 Teardown

A good teardown video for the m600 is available on youtube:
https://www.youtube.com/watch?v=muSNIUXNNaQ


## Specifications

##### m500
`TODO: Insert image`


##### m600
`TODO: Insert image`


## Battery

For both models the official Bafang battery includes a connector that might in the future include CANBUS integration for the battery.
However: The motor controller, as of July 2021, does not require the battery to be CANBUS connected to function. This means connecting the battery power-wires to the motor controller is enough.

##### m600

The m600 is available in a 43v/20A and a 48v/18A variant.
It is, however, possible to run the 43v/20A firmware with a 48v battery, this would increase the max power output, but you might risk the battery being deep-drained, due to a too low cutoff voltage in the controller.

It's also possible to run the 48v/18a variant on a 52v battery, as indicated here:
https://www.youtube.com/watch?v=X2IPtda_LFo

It should be noted that there is a safety cutoff for 48v controllers, that disables the motor if the battery voltage is above 58v. You should charge the 52v cells accordingly.

The reason Bafang does not supply a 52v battery (and you also don't have to try and ask them for it), is because the EU does not allow more than 48v charger connections  CE certification.
The difference between 52v or 48v are, however, very small so it wouldn't actually pose a risk. But for Bafang they cannot supply it within regulations.

##### m500

The m500 is available in a 43v/?A and a 48v/?A varaiant.


## Gearing

The m500 and m600 are mostly similair, with some exceptions:

- The m500 seems to still be using 1 plastic gear, where the m600 is not.
- The gears of the two motors have a slightly different pitch

Gear ratio's:
m600: 6:27
m500: 7:32

Here are a few pictures of people trying to find the stock gearing part numbers:
`TODO: Insert images`
