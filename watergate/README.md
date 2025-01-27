
Web-enabled Arduino water hose controller.
Using Arduino Uno R3 and Arduino Ethernet Shield V1.1 (ENC28J60)

Adapted from [hearth watergate](https://github.com/hearthhouse/watergate) code.

# Install dependencies

First, install [arduino-cli](https://arduino.github.io/arduino-cli/0.35/installation/).

Then:

```
# update list of installable platforms
arduino-cli core update-index

# install core for relevant platform (assuming Arduino Uno R3)
arduino-cli core install arduino:avr

# install the EtherCard library
arduino-cli lib install EtherCard
```

# Compiling and uploading

ToDo - See `../button/`

# Usage

It uses DHCP to get an IP. The MAC address can be found at the top of watergate.ino

It runs a web server so control is via HTTP.

A simple control web page exists at http://<IP>/ but is mostly for testing.

## /sf/on/<n>

Turn the water on for <n> number of minutes where <n> is a positive integer.

## /sf/off

Turn the water off.

## /sf/on/w

Turn the water on for 1 minute the first time.

If there is at least 30 seconds remaining on the timer and this is hit again then turn it on for 5 minutes (not 5 extra minutes but exactly 5 minutes).

If there is at least 4 minutes remaining on the timer and this is hit again then turn it on for 20 minutes (not 20 extra minutes but exactly 20 minutes).
