#define F_CPU 16000000UL

#include "i2c.h"

int main(void)
{
  twi_init();
  lcd_init();
  lcd_string("[+] I2C Master\n", 34);

  while(1) {
  }

  return 0;
}
