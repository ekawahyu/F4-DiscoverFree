=======================================
Demo Firmware for F4-DiscoverFree Board
=======================================

To build and install the demonstration firmware, you need to install:

1. Toolchains to generate binary for STM32F407IGT6.
2. Software flasher/programmer.
3. F4-DiscoverFree board by Aisenke.


Building the firmware and flashing it from terminal application:

1. export PATH=/where/you/install/your/toolchain:$PATH
2. export OPENOCD_SHARE=/where/your/openocd/share
3. make
4. make burn-ft2232d


For more information and tutorials, please visit:

http://www.aisenke.com/nodinorobotics
