#define F_CPU 16000000UL

#include "i2c.h"

int main(void)
{
  init_debug();
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

  lcd_send(0x2c); // function set command :: 4-bits, 2 lines, all fonts

  // FUNCTION -- display on, entry mode, cursor, blink

  lcd_send(0x0f); // display control :: display on, cursor on, blink on

  // FUNCTION -- MOVE TO LOCATION, 0x80 | 0x00 first spot on screen

  lcd_send(0x80); // set ddgram address (move cursor to beginning)


  while(1) {
  }
  return 0;
}
