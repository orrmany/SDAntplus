# BlueAnt
A Nordic SoftDevice powered ANT+ library for the Arduino framework, intended to coexist with Adafruit Bluefruit52, using a mixture of C- & C++-programming language files. 

## Credits
- This library is built on top of the [Adafruit Bluefruit52 library](https://github.com/adafruit/Adafruit_nRF52_Arduino/tree/master/libraries/Bluefruit52Lib), which his part of the [nRF52 Arduino Core](https://github.com/adafruit/Adafruit_nRF52_Arduino) of Adafruit Industries.
- This work was inspired by and produly stealed code from [@rtgree01's forked Bluefruit52 repo](https://github.com/rtgree01/Adafruit_nRF52_Arduino/tree/add_ant/libraries/Bluefruit52Lib/src)

## Dependencies and assumptions
- This library is assumed to be used with a nRF52 Series SoC from Nordic Semiconductor. IT has been developed and tested using an nRF52840 SoC from Adafruit: [Feather nRF52840 Express](https://www.adafruit.com/product/4062)
  - Note that most of the commercially available Arduino compatible SoC somes with no ANT+ softdevice installed. Adding such softdevice and modifying the booloader, etc. is the responsibility of the user. See, for example this blog entry about the necessary changes: [S340 SoftDevice & Adafruit nRF52840 Express Feather](https://blogarak.wordpress.com/2020/03/15/s340-softdevice-adafruit-nrf52840-express-feather/)
- This library is assumed to be used with the Arduino programming framework. It relies on the <Arduino.h> provided system classes and libraries. Adaption to other frameworks, such as mbed, relies on the availability of the Bluefruit library, see next major bullet.
  - Note that most of the available nRF52 Arduino Cores are _not_ prepared for ANT+ Adding support, such as modifying the nRF52 Ardiono Core and adding the modified bootloader, etc, is the responsibility of the user. See, for example these blog entries about the necessary changes:
    - [Arduino IDE integration for the nRF52840 Feather Express with S340](https://blogarak.wordpress.com/2020/03/29/arduino-ide-integration-for-the-nrf52840-feather-express-with-s340/)
    - [PlatformIO IDE integration for the nRF52840 Feather Express with S340](https://blogarak.wordpress.com/2020/03/29/platformio-ide-integration-for-the-nrf52840-feather-express-with-s340/)
- This library assumes the presence of a combined BLE+ANT multiprotocol softdevice from Nordic Semiconductor. It has been developed and tested against the [S340 Softdevice](https://www.nordicsemi.com/Software-and-tools/Software/S340-ANT), but it should be easy to use with other similar SoftDevices, such as [S312](https://www.nordicsemi.com/Software-and-tools/Software/S312-ANT) or [S332](https://www.nordicsemi.com/Software-and-tools/Software/S332-ANT). 
  - Note 
- This library is intended to be piggy-backed onto the Adafruit Bluefruit52 libray, to provide concurrent multiprotocol capabilities with both BLE and ANT+. As aconsequence of this it assumes a FreeRTOS environment.  

# W.I.P.
- Several ANT+ profiles are incomplete, or completely missing.
  - Some of the profiles support only RX mode currently, for example
