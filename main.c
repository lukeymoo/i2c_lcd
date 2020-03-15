#include "lcd/lcd_pcf8574t.h"

int main(void)
{
	twi_init(0x27); // initialize TWI interface
	lcd_init(0x27); // initialize the LCD
	lcd_nibble.string((uint8_t *)"hello world", 12);

	// LEDs ON
	DDRD = (1 << PD2) | (1 << PD4);
	PORTD = (1 << PD2) | (1 << PD4);

	while(1) {
		_delay_us(1);
	}
	return 0;
}
