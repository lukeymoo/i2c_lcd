#include "lcd_pcf8574t.h"

int main(void)
{
	twi_init(0x27); // initialize TWI interface
	lcd_init(0x27, 16, 2); // initialize the LCD
	lcd_nibble.string((unsigned char *)"hello world", 11);
	set_cursor_pos(9, 2);
	lcd_nibble.string((unsigned char *)"- by jr", 7);

	// LEDs ON
	DDRD = (1 << PD2) | (1 << PD4);
	PORTD = (1 << PD2) | (1 << PD4);

	while(1) {
		_delay_us(1);
	}
	return 0;
}
