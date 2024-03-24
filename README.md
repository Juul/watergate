
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

# Compile

```
arduino-cli compile --fqbn arduino:avr:uno .
```

# Upload

Get the serial device for the connected board:

```
arduino-cli board list
```

Then, e.g:

```
arduino-cli upload --verbose -p /dev/ttyACM0 --fqbn arduino:avr:uno .
```

# Monitor

To see the debug output:

```
arduino-cli monitor -p /dev/ttyACM0 --config baudrate=57600
```