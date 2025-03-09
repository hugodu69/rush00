# frequency of the CPU (in Hz)
F_CPU = 16000000UL
TARGET = main
CC = avr-gcc
INCLUDES = -I.

AVRGCC_MCU_TYPE = atmega328p
AVRDUDE_MCU_TYPE = m328p
BAUD_RATE = 115200
PROGRAMMER_ID = arduino

# original firmware dump
DUMP_ORI = ../../ressources/dump_atmega328p_ori.hex

# Find all C source files in the current directory
SRC = $(wildcard *.c)
# Generate object file names from source file names
OBJ = $(SRC:.c=.o)

# Compiler flags
CFLAGS = -mmcu=$(AVRGCC_MCU_TYPE) -D F_CPU=$(F_CPU) $(INCLUDES) -Os

# Detect OS
OS := $(shell uname -s)

# Set Serial Port based on OS
ifeq ($(OS), Darwin)  # macOS
    SERIAL_PORT := $(shell ls /dev/tty.usb* 2>/dev/null | head -n 1) # should be : /dev/tty.usbserial-310
else ifeq ($(OS), Linux)  # Ubuntu/Linux
    SERIAL_PORT = $(shell ls /dev/ttyUSB* 2>/dev/null | head -n 1) # should be : /dev/ttyUSB0
endif

##
## RULES
##

all: hex flash

hex: $(TARGET).hex

# Compile each .c file to corresponding .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files to create the binary
# avr-gcc : compiler for AVR microcontrollers, https://gcc.gnu.org/wiki/avr-gcc#Using_avr-gcc, https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
#	-mmcu : the target microcontrolle (ATmega328P)
#	-D : defines a preprocessor macro (#define F_CPU <value>)
#	-o : output filename
$(TARGET).bin: $(OBJ)
	$(CC) -mmcu=$(AVRGCC_MCU_TYPE) $(OBJ) -o $(TARGET).bin

# Convert binary to hex for flashing
# avr-objcopy : man avr-jobcopy, https://linux.die.net/man/1/avr-objcopy
#	-O : specifies output format, Intel HEX
#		some formats (list not found in any doc) :
#		-O ihex : Used for flashing AVR chips
#		-O binary : Raw binary file
#		-O elf32-avr : Used for debugging
$(TARGET).hex: $(TARGET).bin
	avr-objcopy -O ihex $(TARGET).bin $(TARGET).hex

# Flash the hex file to the microcontroller
# avr-dude : AVR Downloader/UploaDEr, upload firmware, https://avrdudes.github.io/avrdude, v6.3 https://download-mirror.savannah.gnu.org/releases/avrdude/avrdude-doc-6.3.pdf
#	-p : specifies the microcontroller -> m328p
#	-c : programmer-id
#	-b : baud rate for serial communication
#	-P : the serial port, found with `ls /dev/tty*` -> /dev/ttyUSB0
#	-U : Uploads (w = write) the HEX firmware file to the flash memory
#	(-v : verbose info dump)
flash:
	avrdude -p $(AVRDUDE_MCU_TYPE) -c $(PROGRAMMER_ID) -b $(BAUD_RATE) -P $(SERIAL_PORT) -U flash:w:$(TARGET).hex
	avrdude -p $(AVRDUDE_MCU_TYPE) -c $(PROGRAMMER_ID) -b $(BAUD_RATE) -P /dev/ttyUSB1 -U flash:w:$(TARGET).hex

restore:
	avrdude -p $(AVRDUDE_MCU_TYPE) -c $(PROGRAMMER_ID) -b $(BAUD_RATE) -P $(SERIAL_PORT) -U flash:w:$(DUMP_ORI)

clean:
	rm -f main.hex main.bin $(OBJ)

.PHONY : all clean hex flash restore