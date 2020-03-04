#define F_CPU 16000000UL

#include "i2c.h"

uint8_t words[] = ">Listening...";

int main(void)
{
  twi_init();

  _delay_ms(50); // wait for LCD power on
  lcd_write(0x00);
  lcd_write(0x03 << 4); // first we spam 8-bit function set
  _delay_ms(5);
  lcd_write(0x03 << 4); // first we spam 8-bit function set
  _delay_ms(5);
  lcd_write(0x03 << 4); // first we spam 8-bit function set
  _delay_us(150);
  lcd_write(0x02 << 4); // after 3 tries we can do a 4-bit function set
  lcd_send(0x2c); // function set command :: 4-bits, 2 lines, all fonts || 0x24 for 1 line 5X10 dot

  // FUNCTION -- display on, entry mode, cursor, blink
  lcd_send(0x0f); // display control :: display on, cursor on, blink on

  // FUNCTION -- Clear Screen -- This function needs AT LEAST 2
  lcd_send(0x01); // This command takes more time
  _delay_ms(5);

  // FUNCTION -- MOVE TO LOCATION, 0x80 | 0x00 first spot on screen || RowxColumn ->> 0x00 1x1 :: 0x40 2x1
  lcd_send(0x80); // set ddgram address (move cursor to beginning)

  // launch logo
  lcd_string(words, (sizeof(words)/sizeof(char)));

  while(1) {
  }

  return 0;
}
