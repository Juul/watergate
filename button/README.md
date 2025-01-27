
Firmware for the button that sends HTTP requests to the solenoid valve controller.

# Install dependencies

First, install [arduino-cli](https://arduino.github.io/arduino-cli/0.35/installation/) which you probably already have if you previously compiled the solenoid controller firmware.

Then:

```
# update list of installable platforms, addin ESP32
arduino-cli core update-index --additional-urls https://dl.espressif.com/dl/package_esp32_index.json

# install core for ESP32
arduino-cli core install esp32:esp32
```

Now do:

```
cp arduino-cli.yaml.example arduino-cli.yaml
```

Then edit `arduino-cli.yaml` to fit your system. You can get the serial device for the currently connected dev board like so:

```
arduino-cli board list
```

# Compile

```
./build.sh
```

# Upload

```
./upload.sh
```

# Monitor

To see the debug output:

```
./monitor.sh
```

# Hooking up the buttons

ToDO