STRICTMODE = -Wextra  -Wstrict-aliasing -pedantic -fmax-errors=5 -Werror -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option


default: all

upload:
	avrdude -carduino -patmega328p -P /dev/ttyACM0 -D -Uflash:w:main.hex:i -v

all: main

compile:
	avr-gcc -Wall -g -mmcu=atmega328p -c -O1 main.c lcd_pcf8574t.c twi/twi.c -D F_CPU 16000000UL
	avr-gcc -Wall -g -mmcu=atmega328p -o main.elf lcd_pcf8574t.o twi.o main.o -D F_CPU 16000000UL
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

main: lcd_pcf8574t.c twi/twi.c
	avr-gcc -Wall $(STRICTMODE) -g -mmcu=atmega328p -c -O2 main.c lcd_pcf8574t.c twi/twi.c -D F_CPU=16000000UL
	avr-gcc -Wall -g -mmcu=atmega328p -o main.elf lcd_pcf8574t.o twi.o main.o -D F_CPU=16000000UL
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

	avrdude -carduino -patmega328p -P /dev/ttyACM0 -D -Uflash:w:main.hex:i -v
