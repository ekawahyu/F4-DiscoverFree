# put your all of your source code *.c here

SRCS = 	$(wildcard usr/*.c)

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=develop

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

CFLAGS  = -g -O3 -Wall -Tstm32f4xx.ld
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
#CFLAGS += -specs=nano.specs
#CFLAGS += -u _printf_float -u _scanf_float
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

DEFINES = -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS

###################################################

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib/cmsis/inc -Ilib/peripherals/inc -Ilib/usb/inc
CFLAGS += -Isys/include -Iusr
CFLAGS += -Isys/f4df -Iapps/nsh -Iapps/nsh/examples

SRCS += lib/startup_stm32f4xx.s
SRCS += $(wildcard src/*.c)
SRCS += $(wildcard sys/*.c)
SRCS += $(wildcard sys/dev/*.c)
SRCS += $(wildcard sys/cmd/*.c)
SRCS += $(wildcard sys/f4df/*.c)

SRCS += $(wildcard apps/nsh/*.c)
SRCS += $(wildcard apps/nsh/examples/*.c)
SRCS += $(wildcard apps/nplay/*.c)
SRCS += $(wildcard apps/bldc/*.c)

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

# Flash the F4-DiscoverFree using FT2232D
burn-ft2232d: all
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
