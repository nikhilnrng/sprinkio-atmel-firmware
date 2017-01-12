# Sprinkio Sprinkler Controller

### Overview

Sprinkio is a smart, WiFi-connected household sprinkler controller. Sprinkio gives users the ability to control their sprinklers and reduce their water bills, all from a smartphone app. The controller uses environmental temperature and moisture sensor inputs to build a custom schedule that waters just the right amount. 

The controller was built as part of a senior capstone project. The hardware combines an Atmel ATmega 328p microcontroller and a Raspberry Pi to provide manual zone controls, zone status, WiFi connectivity, and a real-time clocking mechanism. The project also utilizes an iOS app to give users control of zone status remotely, on a shared WiFi network. 

This project utilizes three repositories: 

- [sprinkio-atmel-firmware](https://github.com/nikhilnrng/sprinkio-atmel-firmware): the firmware for the Atmel ATmega 328p microcontroller
- [sprinkio-raspberry-pi-firmware](https://github.com/nikhilnrng/sprinkio-atmel-firmware): the firmware for the Raspberry Pi
- [sprinkio-iphone-app](https://github.com/nikhilnrng/sprinkio-iphone-app): an iOS app that provides user with zone control over WiFi

### Atmel ATmega 328p Firmware

The Atmel firmware was written in C code and is separated in pairs of C files and header files that allow for individual handling of hardware components such as the real time clock, temperature sensors, moisture sensors, zone control, and so on. The individual files are brought together in a single main file, which initializes the system loop. When the system loop begins, the microcontroller will remain in this loop as long as power is continually supplied to the device. The Atmel firmware uses three different forms of data communication: (1) serial (USART) protocol, (2) I2C protocol and (3) digital and analog GPIO. These forms of data communication allow the Atmel to interface with all the individual hardware components:

- Raspberry Pi - interface for user inputs and WiFi module
- Relays - a go-between for low-power controller signals the zone circuitry to raise and lower sprinkler heads
- Moisture Sensor - detects moisture level of soil in a zone and can result in early shutoff
- Temperature and Humidity Sensor - detects weather patterns, used to adjust watering schedule accordingly
- RS-232 Interface - for debugging purposes

Two structs are critical to the operation of the ATmega328p firmware. These structs are the `system_t` struct and the `settings_t` struct. The `system_t` struct is used to track the current zone status, manual zone status, system state, current date and time, and the system settings. The `settings_t` struct tracks settings that are at the heart of the Sprinkio operation. When a zone is manually setup, the zone settings are stored in a `settings_t` object and backed up to the EEPROM. Additional structs are defined to track the date, time, zone, and modes during microcontroller operation.

For a detailed report on the system operation, message me at nikhilnrng@gmail.com. 
