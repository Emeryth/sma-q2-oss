# SMAQ2 Open Source Firmware

> Open source firmware for the SMAQ2 smartwatch


The Fluke / Philips PM630X line of LCR meters were a line of benchtop equipment that allowed the testing of components with a frequency up to 1MHZ. Even though the meter was designed over 20 years ago it is still highly capable and has specs similar to modern commercial meters. The major advantage modern meters have are their updated rich UIs which are much more featureful and powerful than the LED display leveraged in this device. Fortunately PM630X meters include an optional RS232 port which allows fully remote control of the unit. This project leverages this serial port to build a modern high resolution TFT-based UI
on this device that significantly expands the utility of the base unit.

### Main Features ###
TBD

### Build Instructions ###
#### Makefile Setup
1.  initialize submodules
  ```
  $ git submodule init
  $ git submodule update
  ```
2. install linux packages needed
  ```
  $ apt-get install build-essential pipenv python3-dbus git virtualenv build-essential python3-dev libdbus-glib-1-dev libgirepository1.0-dev libcairo2-dev python3-protobuf protobuf-compiler python-protobuf gcc-arm-none-eabi binutils-arm-none-eabi gdb-arm-none-eabi openocd
  pip3 install python3-pydbus python3-gi
  ```
3. reconfigure bluez in experimental mode to enable all apis
  ```
  $ sudo vi /lib/systemd/system/bluetooth.service
  ...
  ExecStart=/usr/lib/bluetooth/bluetoothd --experimental
  ```
4. install JLink
  ```
  $ wget --post-data 'accept_license_agreement=accepted&non_emb_ctr=confirmed&submit=Download+software' https://www.segger.com/downloads/jlink/JLink_Linux_arm.tgz
  $ cd JLink_Linux_V646g_arm/
  $ more README.txt
  $ sudo cp 99-jlink.rules /etc/udev/rules.d/
  $ ln -s JLink_Linux_V646g_arm/JLinkExe ~/.local/bin
  $ sudo reboot
  ```

## References
> Software tools, hardware, and useful articles

### Software ###
- [Electron](https://electronjs.org/)
- [ReactJS](https://reactjs.org/)
- [Bootstrap](https://getbootstrap.com/)

### Hardware ###
- [Odroid C2](https://wiki.odroid.com/odroid-c2/odroid-c2)
- [Waveshare 4.3inch HDMI LCD](https://www.waveshare.com/4.3inch-hdmi-lcd-b.htm)
# SMA-Q2-OSS

Open source firmware for the SMA-Q2 smartwatch.

[Project page on hackaday.io](https://hackaday.io/project/85463-color-open-source-smartwatch)

The firmware aims to be a drop-in replacement for the original firmware. No additional hardware is required. It can be uploaded using the original OTA bootloader.

To maintain compatibility with the softdevice and bootloader already on the watch, this firmware is based on nRF5 SDK version 11.0.

## Current status

  * Basic support for all parts of the watch
  * Basic BLE support
  * Basic Gadgetbridge support (setting time)

## Smartphone app

[Gadgetbridge fork](https://github.com/Emeryth/Gadgetbridge)

## Building

1. Download nRF5 SDK 11.0 and copy the sdk `components` folder to the project folder
2. Edit `components/toolchain/gcc/Makefile.posix` to point to your toolchain
2. Install `nrfutil` version **0.5.2**
3. Patch the TWI driver to allow transfers longer than 255 bytes:

        cd components/drivers_nrf/
        patch -s -p0 < ../../patch/twi.patch

4. Go to `armgcc` and run `make`
5. Resulting `app_dfu_package.zip` can be uploaded using regular Nordic DFU software

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

## License and Copyright

Copyright (c) 2019 Andrzej Surowiec,
released under MIT License

FreeRTOS Copyright (C) Amazon.com, Inc.

nRF GFX lib Copyright (c) Nordic Semiconductor ASA

CMSIS DSP lib Copyright (C) ARM Limited.

See headers for licenses.