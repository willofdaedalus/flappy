TARGET = main
MCU = atmega32u4
F_CPU = 16000000UL

# --- Compiler & Tools ---
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# --- Source Files ---
SRC = $(wildcard *.c)
# SRC = main.c
LIB = libavr_i2c.a

# --- Compiler Flags ---
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99 -Wall -Wextra -Werror -Os -Iinclude

# --- Target Definitions ---
all: $(TARGET).hex

# Link all object files into the final ELF executable
$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LIB)

# Convert the ELF file to a HEX file for flashing
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# --- Flashing and Cleanup ---
# Flash the hex file to the MCU using AVRDUDE
flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c avr109 -P /dev/ttyACM0 -b 57600 -D -U flash:w:$<

# Clean up all generated files
clean:
	rm -f *.elf *.hex

.PHONY: all clean flash
