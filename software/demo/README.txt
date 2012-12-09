=======================================
Demo Firmware for F4-DiscoverFree Board
=======================================

F4-DiscoverFree board comes pre-installed with a demo firmware to show:

1. Turning on/off 4 LEDs.
2. Accelerometer navigation in 3 axis.
3. Loading and playing ogg/mp3 music from micro SD card.
4. RC/Servo motor control for up to 26 motors.


==================
System Requirement
==================

In case you want to build and modify the demo firmware, you need to:

1. A PC or Mac with Windows, Linux or MAC OS X.
2. An ARM toolchains to generate a binary for STM32F407IGT6 (GCC, IAR, Keil, etc).
3. OpenOCD to debug and program the demo firmware.


==============================
Building and Flashing Firmware
==============================

Building and flashing firmware from terminal (Linux and Mac OS X users):

1. export PATH=/where/your/toolchains/bin:$PATH
2. export OPENOCD_SHARE=/where/your/share/openocd
3. make
4. make burn-ft2232d

Building and flashing firmware from command prompt (Windows users):

1. PATH=C:\msys\1.0\bin;C:\where\your\toolchains;%PATH%
2. set OPENOCD_SHARE=C:\where\your\share\openocd
3. make
4. make burn-ft2232d


=====================
Serial Terminal Notes
=====================

1. Linux and Mac OS X users are suggested to install picocom with at least v1.7
   To start picocom: picocom -b 115200 /dev/<your_tty_usb_serial_number> --imap lfcrlf

2. Windows users are suggested to install TeraTerm with at least v4.75
   Double click TeraTerm icon to start it, then setup the terminal to receive LF and transmit CR+LF newline.
   Then setup the serial port number (COMx) and the baudrate to 115200.


For more information and tutorials, please visit:

http://www.aisenke.com/nodinorobotics/f4discoverfree
