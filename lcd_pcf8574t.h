#include "twi/twi.h"

/**
	Music to remember
	Lorn - Acid Rain
*/

/**
  PINOUT for PCF8574T to LCD control pins
*/
#define PCF_RS 			0x01  //P0 - PCF8574T Pin connected to RS
#define PCF_RW 			0x02  //P1 - PCF8574T Pin connected to RW
#define PCF_ENABLE 		0x04  //P2 - PCF8574T Pin connected to EN
#define PCF_BACKLIGHT 	0x08  //P3 - PCF8574T Pin connected to BACKLIGHT LED

struct i2c_lcd {
	uint8_t addr;

	// LCD traits
	uint8_t rows;
	uint8_t columns;
	uint8_t backlight:1;
	uint8_t characters:1;
	uint8_t cursor:1;
	uint8_t blink:1;
} lcd;

struct LCD_BYTE {
	uint8_t (*data)(uint8_t byte);
	uint8_t (*command)(uint8_t byte);
	uint8_t (*string)(uint8_t bytes[], uint8_t size);
} lcd_byte;

struct LCD_NIBBLE {
	uint8_t (*data)(uint8_t byte);
	uint8_t (*command)(uint8_t byte);
	uint8_t (*string)(uint8_t bytes[], uint8_t size);
} lcd_nibble;

/**
	LCD Commands
*/
void cursor_on(void);
void cursor_off(void);
void screen_on(void);   // turn off display and characters
void screen_off(void); // turn off display and characters
void clear_display(void);
void characters_on(void);  // turn on/off character display
void characters_off(void); // turn on/off character display
void set_cursor_pos(uint8_t col, uint8_t row);
void cursor_home(void); // returns cursor to beginning of DDGRAM

/**
	I2C operations
*/
void lcd_init(uint8_t addr, uint8_t columns, uint8_t rows);

uint8_t _lcd_write_byte_data(uint8_t byte);
uint8_t _lcd_write_byte_cmd(uint8_t byte);

uint8_t _lcd_write_nibble_data(uint8_t byte);
uint8_t _lcd_write_nibble_cmd(uint8_t byte);

// Write a string of characters
uint8_t _lcd_write_nibble_string(uint8_t bytes[], uint8_t size);
