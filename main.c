#define F_CPU 16000000UL

#include "i2c.h"

int main(void)
{
  twi_init();
  lcd_init();
  lcd_string("Waiting...\n>", 14);

  while(1) {
  }

  return 0;
}
