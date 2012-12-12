F4-DiscoverFree Project Template
================================


NOTE TO BEGINNERS:
------------------

This template is a very basic one and suitable for most Arduino users who want
to become familiar with the real programming environment in C. It provides a
similar function calls setup() and loop() as in Arduino IDE, however, programming
is done in pure C language. The setup() function gets called only once and loop()
function gets called repeatedly until exit(0) gets called within the loop().

The real entry point of the program exists in main.c, called main(). The main()
function calls system_init() which is required to set some newlib's stubs
functions. Then it calls setup() and finally calling loop() forever.

This templates is also intended for beginner at entry level of embedded system
development who wish to understand programming ARM Cortex-M4 in C language.

If you love Arduino, you should love F4-DiscoverFree even more since your code
will become more portable to any microcontroller of your choice since it is
written in C language. Aside from that, now you can debug the code step by step
to find bugs!


NOTE TO ADVANCED USERS:
-----------------------

You may delete user.c and write your program directly under main.c without any
doubt. Please make sure to remove user.c from the build in the Makefile.


System Requirement
------------------

1. A PC or Mac with Windows, Linux or MAC OS X.
2. An ARM toolchains to generate a binary for STM32F407IGT6 (GCC, IAR, Keil, etc).
3. OpenOCD to debug and program the firmware.


How To Use This Template
------------------------

1. Get a copy of myproject
2. Get one of example you want to build by copying main.c and user.c into
   myproject/usr folder. You may override them safely.
3. Build the project.


Building and Flashing
---------------------

Building and flashing your project from terminal (Linux and Mac OS X users):

1. export PATH=/where/your/toolchains/bin:$PATH
2. export OPENOCD_SHARE=/where/your/share/openocd
3. make all
4. make burn-ft2232d

Building and flashing your project from command prompt (Windows users):

1. PATH=C:\msys\1.0\bin;C:\where\your\toolchains;%PATH%
2. set OPENOCD_SHARE=C:\where\your\share\openocd
3. make all
4. make burn-ft2232d


How To Use This Template with Eclipse IDE
-----------------------------------------
1. Copy and paste myproject folder into your Eclipse workspace.
2. Create a new makefile project pointing to myproject.
3. Open project's properties to add PATH and OPENOCD_SHARE variables.
4. Create some make target under Make Target window (all, burn-ft2232d and clean).
5. To build the project, double click "all" target.
6. To burn the binary to F4-DiscoverFree, double click "burn-ft2232d" target.
7. To clean the project, double click "clean" target.


More Info
---------

For more information and tutorial, please visit:

http://www.aisenke.com/nodinorobotics/f4discoverfree
