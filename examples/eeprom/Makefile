DEVICE     = atmega328p
CLOCK      = 9830400
PROGRAMMER = -c usbtiny -P usb
OBJECTS    = eeprom-test.o ../../clock.o ../../i2c.o ../../raspberry_pi.o \
						  ../../rs232.o ../../settings.o ../../system.o ../../usart.o \
						  ../../zone_controller.o ../../helper.o
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0xe0:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex main.eep 

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

flash_eeprom:
	$(AVRDUDE) -U eeprom:w:main.eep:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf main.eep $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

main.eep: main.elf
	rm -f main.eep
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex main.elf main.eep

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
