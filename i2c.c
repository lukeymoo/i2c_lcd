#include "i2c.h"

uint8_t success_message[] = ">Start condition ACK!";
uint8_t fail_message[] = ">Sniffer failed!";

/**
  HERE ARE THE FUNCTIONS FOR CONTROLLING THE LCD DISPLAY
  HELPS TO NOT NEED TO WRITE 0X80, 0X03 ETC FOR COMMANDS EVERYTIME
*/

// Attempt to discover all devices on bus
void sniff(void)
{
  // start at address 0x01 -> 0x7f
  // we send start commands & then request addresses; followed by stop commands
  uint8_t addr_count = 0;
  unsigned char buffer[8];
  clear_display();
  lcd_string(">Sniffing...", 13);
  _delay_ms(1500);
  for(int i = 0x01; i < 0x7f; i++) {
    if(twi_nop(i))
      addr_count++;
  }
  clear_display();
  itoa(addr_count, buffer, 10);
  lcd_string(buffer, 8);
  lcd_string(" Address(es)\nfound!", 21);
  return;
}

/**
  This only broadcasts i2c address range 1 at a time to discover devices on a bus
*/
uint8_t twi_nop(uint8_t address)
{
  uint8_t status = 0;
  uint8_t tmp = bus.d_addr; // save current addr
  bus.data = 0x00;
  bus.d_addr = address; // load new one
  twi_start(); // attempt to write 0x00
  status = twi_slaw();
  // restore old destination address
  bus.d_addr = tmp;
  char buffer[8];
  itoa(address, buffer, 16);
  if(status) {
    clear_display();
    lcd_string("Device found!\n", 15);
    lcd_string("0x", 3);
    lcd_string(buffer, 8);
    _delay_ms(2000);
  } else { // no ack
    clear_display();
    lcd_string("No device found\n", 17);
    lcd_string("0x", 3);
    lcd_string(buffer, 8);
    _delay_ms(75);
  }
  return status;
}

//test function to fill screen with characters
void fill_screen(void)
{
  int a = 0x41;
  for(int j = 0; j < 2; j++) {
    for(int i = 0; i < 16; i++) {
      // print I
      lcd_dsend(a);
      a++;
      _delay_ms(20);
    }
    set_position(2, 0);
  }
  return;
}

/**
  Show/hide characters
*/
void display_on(void)
{
  lcd_send(0x08 | 0x04);
  return;
}

/**
  Show/hide characters
*/
void display_off(void)
{
  lcd_send(0x08);
  return;
}

/**
  Turns on character display & the LED backlight
*/
void screen_on(void)
{
  lcd.l_backlight = 1;
  lcd_send(0x08 | 0x04);
  return;
}

/**
  Turns off character display & the LED backlight
*/
void screen_off(void)
{
  lcd.l_backlight = 0;
  lcd_send(0x08);
  return;
}

/**
  Clears display and sets cursor position to 0,0
*/
void clear_display(void)
{
  // 0x01 is the clear display function
  // Takes a little longer for this function to complete so we include delay
  lcd_send(0x01);
  _delay_ms(10);
  return;
}

void set_position(uint8_t row, uint8_t column)
{
  // ensure row is less than 3, greater than 0
  if(row < 1) // min
    row = 1;
  if(row > 2) // max
    row = 2;

  if(column < 0) // min
    column = 0;
  if(column > 15) // max
    column = 15;

  // 0x80 is the set position function
  unsigned char cmd = 0x80;
  if(row > 1)
    cmd |= 0x40;
  cmd |= column;
  lcd_send(cmd);
  return;
}

void cursor_on(void)
{
  // use display control function 0x08
  // set bits 0 & 1 for blink & cursor correspondingly
  lcd_send(0x08 | 0x04 | 0x03);
  return;
}

void cursor_off(void)
{
  // use display control function 0x08
  // clear bits 0 & 1 for blink & cursor correspondingly
  lcd_send(0x08 | 0x04);
  return;
}








/**
  Initialize I2C bus without setting an address to become slave
  Must receive bus & lcd structures
*/
void twi_init(void)
{
  /* Defaults for i2c bus */
  bus.b_addr = 0;            // default not capable of being slave
  bus.d_addr = 0x27;         // defaults to PCF8574T LCD pair
  bus.d_reg = 0x00;          // no register default
  bus.data = 0x00;           // no data
  bus.b_state = TWI_READY;   // idle, ready
  bus.retval = 0;            // default 0, nop

  /* Defaults for LCD screen */
  lcd.l_addr = 0x27;                   // default PCF8574T LCD pair addr
  lcd.l_backlight = 0x04;
  lcd.l_cursor = LCD_CURSOR_ON;
  lcd.l_blink = LCD_BLINK_ON;
  lcd.l_rows = LCD_SINGLE_ROW;
  lcd.l_cols = LCD_16_COLS;

  /* Set up i2c hardware */
  TWSR = 0;   // No prescaler -- Prescaler is the lower 2 bits of status register
  TWBR = 72;  // baud rate target = 100kbs, divisor is 72 at 16 mhz
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);  // enable i2c
  return;
}

/**
  LCD Write
  Encapsulation for I2C writing to LCD
*/
void lcd_write(uint8_t byte)
{
  twi_loadbuf(byte | PCF_ENABLE);
  twi_write();
  _delay_us(1);
  twi_loadbuf(byte & ~PCF_ENABLE);
  twi_write();
  return;
}

/**
  LCD Write - 4 bit
  Encapsulation for I2C writing to LCD
*/
void lcd_send(uint8_t byte)
{
  unsigned char byte_h = (byte & 0xf0);
  unsigned char byte_l = ((byte<<4) & 0xf0);
  unsigned char _displaycontrol = 0;
  if(lcd.l_backlight) { _displaycontrol |= 0x08; } // set PCF backlight control bit

  twi_loadbuf(byte_h | PCF_ENABLE | _displaycontrol);
  twi_write();
  twi_loadbuf(byte_h & ~PCF_ENABLE | _displaycontrol);
  twi_write();

  twi_loadbuf(byte_l | PCF_ENABLE | _displaycontrol);
  twi_write();
  twi_loadbuf(byte_l & ~PCF_ENABLE | _displaycontrol);
  twi_write();
  return;
}

/**
  LCD Write - 4 bit
  Encapsulation for I2C writing DATA to LCD screen
*/
void lcd_dsend(uint8_t byte)
{
  // if it is data we must set the Register Select pin(RS)
  unsigned char byte_h = (byte & 0xf0);
  unsigned char byte_l = ((byte<<4) & 0xf0);
  unsigned char _displaycontrol = 0;
  if(lcd.l_backlight) { _displaycontrol |= 0x08; } // set PCF backlight control bit

  twi_loadbuf(byte_h | PCF_RS | PCF_ENABLE | _displaycontrol);
  twi_write();
  twi_loadbuf(byte_h | PCF_RS & ~PCF_ENABLE | _displaycontrol);
  twi_write();

  twi_loadbuf(byte_l | PCF_RS | PCF_ENABLE | _displaycontrol);
  twi_write();
  twi_loadbuf(byte_l | PCF_RS & ~PCF_ENABLE | _displaycontrol);
  twi_write();
  return;
}

/**
  LCD Write a string of characters - 4 bits
  Writes string characters with some formatting
*/
void lcd_string(char *words, uint8_t num_chars)
{
  for(int i = 0;i < (num_chars-1); i++) { // -1 to avoid the \0 to terminate strings on PC
    // Check for new line
    if(words[i] == '\n') {
      // Set location 0x40
      lcd_send(0x80 | 0x40);
      continue;
    }
    if(words[i] == '\0') {
      break;
    }
    lcd_dsend(words[i]);
  }
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
void twi_set_addr(uint8_t addr)
{
  bus.d_addr = addr;
  return;
}

/**
  Fills bus buffer with byte of data
*/
void twi_loadbuf(uint8_t byte)
{
  bus.data = byte;
  return;
}

/**
  Write byte of data onto bus
*/
unsigned char twi_write(void)
{
  /* Produce START condition */
  bus.retval = twi_start();
  if(bus.retval == 0) { return 0; } // failed, stop operation
  /* Send the address from buffer */
  bus.retval = twi_slaw();

  if(!bus.retval) { return 0; } // failed, stop operation

  /* Send the data from buffer */
  bus.retval = _send_byte(bus.data);
  if(!bus.retval) { return 0; } // failed, stop operation
  twi_stop(); // Send STOP condition
  return 1; // success
}

/**
  Sends address on bus with WRITE bit (0)
*/
unsigned char twi_slaw(void)
{
  bus.retval = 0;
  TWDR = bus.d_addr << 1;
  TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
  while(!(TWCR & (1 << TWINT))); // wait
  if((TWSR & 0xf8) == TW_MT_SLA_ACK) { return 1; } // on success ret 1
  return 0;
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
