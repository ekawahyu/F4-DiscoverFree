=======================================
Demo Firmware for F4-DiscoverFree Board
=======================================

F4-DiscoverFree board comes pre-installed with a demo firmware to show:

1. Turning on/off 4 LEDs.
2. Accelerometer navigation in 3 axis.
3. Loading and playing ogg/mp3 music from micro SD card.
4. RC/Servo motor control up to 26 motors.


In case you want to build and modify the demonstration firmware, you need to:

1. Own a PC or Mac computer with Windows, Linux or Mac OS X.
2. Install an ARM Cortex-M4 toolchains to generate binary for F4-DiscoverFree board.
3. OpenOCD to flash the firmware to F4-DiscoverFree.


How to build the firmware and flashing it from terminal application:

1. export PATH=/where/you/install/your/toolchain:$PATH
2. export OPENOCD_SHARE=/where/your/openocd/share
3. make
4. make burn-ft2232d


For more information and tutorials, please visit:

http://www.aisenke.com/nodinorobotics
