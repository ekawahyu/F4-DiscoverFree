# put your all of your source code *.c here

SRCS = 	main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c \
		ff.c diskio.c sdio.c sdio_sd.c \
		led.c button.c adc.c usart.c \
		vs1053b.c \

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

CFLAGS  = -g -O2 -Wall -Tstm32f4xx.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

DEFINES = -DUSE_STDPERIPH_DRIVER

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib/cmsis/inc -Ilib/peripherals/inc

SRCS += lib/startup_stm32f4xx.s

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

# Flash the F4-DiscoverFree using FT2232D
burn-ft2232d:
	openocd -s $(OPENOCD_SHARE)/scripts \
            -f interface/ft2232d.cfg \
            -f target/stm32f4x.cfg \
            -c "init" \
            -c "reset halt" \
            -c "flash write_image erase $(PROJ_NAME).bin 0x08000000" \
            -c "reset" \
            -c "shutdown"

lib:
	$(MAKE) -C lib

clean-lib:
	$(MAKE) -C lib clean

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $(DEFINES) $^ -o $@ -Llib -lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(SIZE) -d $(PROJ_NAME).elf

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
