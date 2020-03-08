default: all

upload:
	sudo avrdude -carduino -patmega328p -P /dev/ttyACM0 -D -Uflash:w:main.hex:i -v

all: main

compile:
	avr-gcc -Wall -g -mmcu=atmega328p -c -O1 main.c i2c.c
	avr-gcc -Wall -g -mmcu=atmega328p -o main.elf i2c.o main.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

main: main.c
	avr-gcc -Wall -g -mmcu=atmega328p -c -O1 main.c i2c.c
	avr-gcc -Wall -g -mmcu=atmega328p -o main.elf i2c.o main.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

	sudo avrdude -carduino -patmega328p -P /dev/ttyACM0 -D -Uflash:w:main.hex:i -v
