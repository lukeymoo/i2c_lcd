#include "i2c.h"

/**
  Initialize I2C bus without setting an address to become slave
  Must receive bus & lcd structures
*/
void twi_init(struct i2c_bus *bus, struct i2c_lcd *lcd)
{
  /* Defaults for i2c bus */
  bus->b_addr = 0;            // default not capable of being slave
  bus->d_addr = 0x27;         // defaults to PCF8574T LCD pair
  bus->d_reg = 0x00;          // no register default
  bus->data = 0x00;           // no data
  bus->b_state = TWI_READY;   // idle, ready
  bus->retval = 0;            // default 0, nop

  /* Defaults for LCD screen */
  lcd->l_addr = 0x27;                   // default PCF8574T LCD pair addr
  lcd->l_backlight = LCD_BACKLIGHT_ON;
  lcd->l_cursor = LCD_CURSOR_ON;
  lcd->l_blink = LCD_BLINK_ON;
  lcd->l_rows = LCD_SINGLE_ROW;
  lcd->l_cols = LCD_16_COLS;

  /* Set up i2c hardware */
  TWSR = 0;   // No prescaler -- Prescaler is the lower 2 bits of status register
  TWBR = 72;  // baud rate target = 100kbs, divisor is 72 at 16 mhz
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);  // enable i2c
  return;
}

/**
  Attempt to become master via producing START condition
*/
unsigned char twi_start(void)
{
  TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT); // produce start condition
  while(!(TWCR & (1 << TWINT))); // wait

  if((TWSR & 0xf8) == TW_START) { // TW_START or TW_REP_START return 1
    return 1;
  }
  if((TWSR & 0xf8) == TW_REP_START) {
    return 1;
  }

  return 0; // fail if fall through
}

/**
  Set destination address of bus reads/writes
*/
void twi_set_addr(struct i2c_bus *bus, uint8_t addr)
{
  bus->d_addr = addr;
  return;
}

/**
  Fills bus buffer with byte of data
*/
void twi_loadbuf(struct i2c_bus *bus, uint8_t byte)
{
  bus->data = byte;
  return;
}

/**
  Sends address on bus with WRITE bit (0)
*/
unsigned char twi_slaw(struct i2c_bus *bus)
{
  bus->retval = 0;
  TWDR = bus->d_addr << 1;
  TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
  while(!(TWCR & (1 << TWINT))); // wait
  if((TWSR & 0xf8) == TW_MT_SLA_ACK) // on success ret 1
    return 1;
  return 0;
}

/**
  Write byte of data onto bus
*/
unsigned char twi_write(struct i2c_bus *bus)
{
  /* Produce START condition */
  bus->retval = twi_start();
  if(bus->retval == 0) { return 0; } // failed, stop operation
  /* Send the address from buffer */
  bus->retval = twi_slaw(bus);

  if(!bus->retval) { return 0; } // failed, stop operation

  /* Send the data from buffer */
  bus->retval = _send_byte(bus->data);
  if(!bus->retval) { return 0; } // failed, stop operation
  twi_stop(); // Send STOP condition
  return 1; // success
}

/**
  Private function
  Sends byte of data on bus
*/
unsigned char _send_byte(uint8_t byte)
{
  TWDR = byte; // load buffer
  TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
  while(!(TWCR & (1 << TWINT))); // wait
  if(TWSR & 0xf8 == TW_MT_DATA_ACK) { // good data, return success
    return 1;
  }
  return 0;
}

/**
  Sends stop condition & disables i2c
*/
void twi_stop(void)
{
  TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT); // stop i2c hardware
  return;
}

/**
  Initializes debugging
*/
void init_debug(struct debugging *bug)
{
  bug->red = 0;
  bug->green = 0;
  bug->blue = 0;
  DDRD = 0xff;
  PORTD = 0;
  return;
}

/**
  Set specified LED
*/
void set(uint8_t pin)
{
  DDRD |= (1 << pin);
  PORTD |= (1 << pin);
  return;
}

/**
  Clears specified LED
*/
void clear(uint8_t pin)
{
  PORTD &= ~(1 << pin);
  return;
}

// Outputs a byte of data to LEDs
void led_write(uint8_t byte)
{
  DDRD = 0xff; // OUTPUT
  PORTD = byte;
  return;
}
