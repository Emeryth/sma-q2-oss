# SMAQ2 Open Source Firmware

> Open source firmware for the SMAQ2 smartwatch

As [featured on hackaday.io](https://hackaday.io/project/85463-color-open-source-smartwatch) this firmware aims to be a drop-in replacement for the original firmware. No additional hardware is required. It can be uploaded using the original OTA bootloader.To maintain compatibility with the softdevice and bootloader already on the watch, this firmware is based on nRF5 SDK version 11.0.

### Main Features ###
  * Basic support for all parts of the watch
  * Basic BLE support
  * Basic Gadgetbridge support (setting time)

### Build Instructions ###
#### Makefile Setup
1. install linux and python packages needed
  ```
  $ apt-get update && apt-get upgrade
  $ apt-get install git build-essential gcc-arm-none-eabi binutils-arm-none-eabi gdb-arm-none-eabi openocd nodejs npm python3-dbus bluetooth pipenv protobuf-compile python-protobuf python3-dbus python3-dev libdbus-glib-1-dev libgirepository1.0-dev libcairo2-dev cppcheck valgrind
  $ sudo pip3 install gatt jupyterlab jupyterlab-git
  ```
2.  initialize submodules
  ```
  $ git submodule init
  $ git submodule update
  ```
3. reconfigure bluez in experimental mode to enable all apis
  ```
  $ sudo vi /lib/systemd/system/bluetooth.service
  ...
  ExecStart=/usr/lib/bluetooth/bluetoothd --experimental
  ```
4. punch a hole for "pi" user
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
5. install JLink
  ```
  $ wget --post-data 'accept_license_agreement=accepted&non_emb_ctr=confirmed&submit=Download+software' https://www.segger.com/downloads/jlink/JLink_Linux_arm.tgz
  $ cd JLink_Linux_V646g_arm/
  $ more README.txt
  $ sudo cp 99-jlink.rules /etc/udev/rules.d/
  $ ln -s JLink_Linux_V646g_arm/JLinkExe ~/.local/bin
  $ sudo reboot
  ```
  5. (optional) configure jupyter to start by default
  /etc/systemd/system/jupyter.service
  ```
  [Unit]
  Description=Jupyter Workplace

  [Service]
  Type=simple
  PIDFile=/run/jupyter.pid
  ExecStart=/home/pi/.local/bin/jupyter-lab notebooks/ --allow-root --no-mathjax --ip=0.0.0.0 --port=8888 --no-browser --NotebookApp.token=''
  User=pi
  Group=pi
  WorkingDirectory=/home/pi/src/sma-q2-oss
  Restart=always
  RestartSec=10

  [Install]
  WantedBy=multi-user.target

  ```
  enable the service
  ```
  $ sudo systemctl enable jupyter.service
  $ sudo systemctl daemon-reload
  $ sudo systemctl restart jupyter.service
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
