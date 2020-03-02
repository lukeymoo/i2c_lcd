#define F_CPU 16000000UL

#include "i2c.h"

void send(uint8_t byte)
{
  // write data 3 times
  // 2->3 we are pulsing enable
  return;
}

int main(void)
{
  // create bus & lcd objects
  struct i2c_bus bus;
  struct i2c_lcd lcd;
  struct debugging bug;
  init_debug(&bug);
  twi_init(&bus, &lcd);
  // load buffer with data

  _delay_ms(50); // wait for LCD power on

  twi_loadbuf(&bus, 0x00);
  twi_write(&bus);

  twi_loadbuf(&bus, (0x03 << 4));
  twi_write(&bus);
  twi_loadbuf(&bus, (0x03 << 4) | PCF_ENABLE);
  twi_write(&bus);
  _delay_us(1);
  twi_loadbuf(&bus, (0x03 << 4) & ~PCF_ENABLE);
  twi_write(&bus);

  _delay_ms(5);

  twi_loadbuf(&bus, (0x03 << 4));
  twi_write(&bus);
  twi_loadbuf(&bus, (0x03 << 4) | PCF_ENABLE);
  twi_write(&bus);
  _delay_us(1);
  twi_loadbuf(&bus, (0x03 << 4) & ~PCF_ENABLE);
  twi_write(&bus);

  _delay_ms(5);

  twi_loadbuf(&bus, (0x03 << 4));
  twi_write(&bus);
  twi_loadbuf(&bus, (0x03 << 4) | PCF_ENABLE);
  twi_write(&bus);
  _delay_us(1);
  twi_loadbuf(&bus, (0x03 << 4) & ~PCF_ENABLE);
  twi_write(&bus);

  _delay_us(150);

  // 4 bit mode selection
  twi_loadbuf(&bus, (0x02 << 4));
  twi_write(&bus);
  twi_loadbuf(&bus, (0x02 << 4) | PCF_ENABLE);
  twi_write(&bus);
  _delay_us(1);
  twi_loadbuf(&bus, (0x02 << 4) & ~PCF_ENABLE);
  twi_write(&bus);

  // function set -- display on, entry mode, cursor, blink
  unsigned char fset = (0x2c);// function set command :: 4-bits, 2 lines, all fonts
  unsigned char fset_h = (fset & 0xf0);
  unsigned char fset_l = ((fset<<4) & 0xf0);

  twi_loadbuf(&bus, fset_h | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_h & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);

  twi_loadbuf(&bus, fset_l | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_l & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);

  // FUNCTION -- display on, entry mode, cursor, blink
  fset = (0x0f); // display control :: display on, cursor on, blink on
  fset_h = (fset & 0xf0);
  fset_l = ((fset<<4) & 0xf0);

  twi_loadbuf(&bus, fset_h | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_h & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);

  twi_loadbuf(&bus, fset_l | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_l & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);

  // FUNCTION -- MOVE TO LOCATION, 0x80 | 0x00 first spot on screen
  fset = (0x80); // set DDGRAM Address                               | 0x40 is the beginning of next line
  fset_h = (fset & 0xf0);
  fset_l = ((fset<<4) & 0xf0);

  twi_loadbuf(&bus, fset_h | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_h & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);

  twi_loadbuf(&bus, fset_l | PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);
  twi_loadbuf(&bus, fset_l & ~PCF_ENABLE|PCF_BACKLIGHT);
  twi_write(&bus);


  while(1) {
  }
  return 0;
}
