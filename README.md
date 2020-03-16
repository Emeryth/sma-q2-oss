# SMAQ2 Open Source Firmware

> Open source firmware for the SMAQ2 smartwatch

As [featured on hackaday.io](https://hackaday.io/project/85463-color-open-source-smartwatch) this firmware aims to be a drop-in replacement for the original firmware. No additional hardware is required. It can be uploaded using the original OTA bootloader.To maintain compatibility with the softdevice and bootloader already on the watch, this firmware is based on nRF5 SDK version 11.0.

### Main Features ###
  * Basic support for all parts of the watch
  * Basic BLE support
  * Basic Gadgetbridge support (setting time)

### Build Instructions ###
#### Basic Build Setup
1. install linux and python packages needed
  ```
  $ apt-get update && apt-get upgrade
  $ apt-get install git build-essential gcc-arm-none-eabi binutils-arm-none-eabi gdb-arm-none-eabi openocd nodejs npm python3-dbus bluetooth pipenv protobuf-compile python-protobuf python3-dbus python3-dev libdbus-glib-1-dev libgirepository1.0-dev libcairo2-dev cppcheck valgrind
  ```
2.  initialize submodules
  ```
  $ git submodule init
  $ git submodule update
  ```
3. build dfu image
  ```
  $ make dfu
  ```
#### Simulated Bluetooth Central
By default you can pair you watch with a phone running [gadgetbridge](https://github.com/Emeryth/Gadgetbridge) and use all features of the phone. One limitation of this approach is that it requires that you release a new version of the Android APK along side your watch each time you iterate on new messages. To get around this the project includes a set of Jupyter notebooks that use python to simulate a Bluetooth central device. In this way you can interract with your watch and iterate on new message formats without needing to recompile and install an android app. To set this up do the following:
1. Reconfigure bluez in experimental mode. Since some APIs have not yet been mainlined you need this flag to enable all apis.
  ```
  $ sudo vi /lib/systemd/system/bluetooth.service
  ...
  ExecStart=/usr/lib/bluetooth/bluetoothd --experimental
  ```
2. Ensure that you have appropriate permissions for the user you will be running the notebook as to interract with the bluetooth phy. In this case the below snippet allows the "pi" user access:
  ```
  $ sudo vi /etc/dbus-1/system.d/bluetooth.conf
  ...
  <policy user="pi">
    <allow own="org.bluez"/>
    <allow send_destination="org.bluez"/>
    <allow send_interface="org.bluez.GattCharacteristic1"/>
    <allow send_interface="org.bluez.GattDescriptor1"/>
    <allow send_interface="org.freedesktop.DBus.ObjectManager"/>
    <allow send_interface="org.freedesktop.DBus.Properties"/>
  </policy>

  ```
3. Start up the notebook. Note that once started you should be able to go to http://localhost:8888 and see demo notebooks that let you interract with the watch.
  ```
  $ make jupyter
  ```


## References
> Software tools, hardware, and useful articles
## Flashing

### Via DFU
The watch can be flashed using regular Nordic DFU software, like nRF Connect for Android.

To enter the built-in OTA bootloader:

**Original firmware:**
 hold UP and DOWN buttons while powering on the watch.

**This firmware:**
 press UP, DOWN and BACK at the same time


### Via SWD
Enabling SWD requires some work:

1. Upload a firmware that dumps the flash and configuration register contents
2. Disassemble watch
3. Connect to SWD and unlock device **this erases the entire flash!**
4. Restore the bootloader/softdevice/firmware and configuration register values
5. Disable CRC checking in bootloader

### Software ###
- [Gadgetbridge fork](https://github.com/Emeryth/Gadgetbridge)

### Hardware ###
- TBD

## License and Copyright

Copyright (c) 2019 Andrzej Surowiec,
released under MIT License

FreeRTOS Copyright (C) Amazon.com, Inc.

nRF GFX lib Copyright (c) Nordic Semiconductor ASA

CMSIS DSP lib Copyright (C) ARM Limited.

See headers for licenses.
