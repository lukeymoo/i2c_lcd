#include "lcd_pcf8574t.h"

/**
	I2C operations
*/

// calls HD44780U initialization procedures
void lcd_init(uint8_t addr, uint8_t cols, uint8_t rows)
{
	// Set functions
	lcd_nibble.data = _lcd_write_nibble_data;
	lcd_nibble.command = _lcd_write_nibble_cmd;
	lcd_nibble.string = _lcd_write_nibble_string;

	lcd_byte.data = _lcd_write_byte_data;
	lcd_byte.command = _lcd_write_byte_cmd;

	// default LCD traits
	lcd.addr = addr;
	lcd.cursor = 1;
	lcd.blink = 1;
	lcd.characters = 1;
	lcd.backlight = 1;
	lcd.rows = rows;
	lcd.columns = cols;

	_delay_ms(50); // wait for LCD power on

	lcd_byte.command(0x00);

	lcd_byte.command(0x03 << 4); // first we spam 8-bit function set
	_delay_ms(5);

	lcd_byte.command(0x03 << 4); // spam 8-bit function set
	_delay_ms(5);

	lcd_byte.command(0x03 << 4); // spam 8-bit function set
	_delay_us(150);

	lcd_byte.command(0x02 << 4); // after 3 tries we can do a 4-bit function set

	if(rows < 2) {
		lcd_nibble.command(0x24);
	} else {
		lcd_nibble.command(0x2c); // function set command :: 4-bits, 2 lines, all fonts || 0x24 for 1 line 5X10 dot
	}

	// FUNCTION -- display on, entry mode, cursor, blink
	lcd_nibble.command(0x0f); // display control :: display on, cursor on, blink on

	// FUNCTION -- Clear Screen -- This function needs AT LEAST 2
	lcd_nibble.command(0x01); // This command takes more time
	_delay_ms(5);

	// FUNCTION -- MOVE TO LOCATION, 0x80 | 0x00 first spot on screen || RowxColumn .> 0x00 1x1 :: 0x40 2x1
	lcd_nibble.command(0x80); // set ddgram address (move cursor to beginning)
	return;
}

/**
	I2C . LCD Byte size operations
*/
uint8_t _lcd_write_byte_cmd(uint8_t byte)
{
	twi_write(byte | PCF_ENABLE);
	_delay_us(1);
	twi_write(byte & ~PCF_ENABLE);
	twi_stop();
	return 1;
}

uint8_t _lcd_write_byte_data(uint8_t byte)
{
	twi_write((byte | PCF_RS) | PCF_ENABLE);
	_delay_us(1);
	twi_write((byte | PCF_RS) & ~PCF_ENABLE);
	twi_stop();
	return 1;
}


// Write byte specified as DATA or CMD in half bytes(2 transmissions)
uint8_t _lcd_write_nibble_cmd(uint8_t byte)
{
	uint8_t byte_h = (byte & 0xf0); // mask high bits
	uint8_t byte_l = ((byte<<4) & 0xf0); // shift lower 4 up & mask high
	uint8_t displayctrl = 0;
	uint8_t status = 0;

	// check if display is enabled
	if(lcd.backlight) {
		displayctrl |= 0x08;
	}
	// issue start command
	status = twi_start();
	if(!status) { // bad start
		twi_stop();
		return 0;
	}

	status = twi_slaw(); // send SLA + W
	if(!status) { // NACK, bad addr/not rdy for receiving
		twi_stop();
		return 0;
	}

	// HIGH BITS
	twi_send_byte((byte_h | PCF_ENABLE) | displayctrl); 	// enable high

	status = twi_start(); 								// repeated start
	if(!status) { // bad repeated start
		twi_stop();
		return 0;
	}

	status = twi_slaw(); // send SLA + W
	if(!status) { // NACK, bad addr/not rdy for receiving
		twi_stop();
		return 0;
	}

	twi_send_byte((byte_h | displayctrl) & ~PCF_ENABLE); 	// enable low

	status = twi_start();								// repeated start
	if(!status) { // bad repeated start
		twi_stop();
		return 0;
	}

	status = twi_slaw(); // send SLA + W
	if(!status) { // NACK, bad addr/not rdy for receiving
		twi_stop();
		return 0;
	}

	// LOW BITS
	twi_send_byte((byte_l | PCF_ENABLE) | displayctrl); 	// enable high

	status = twi_start();								// repeated start
	if(!status) { // bad repeated start
		twi_stop();
		return 0;
	}

	status = twi_slaw(); // send SLA + W
	if(!status) { // NACK, bad addr/not rdy for receiving
		twi_stop();
		return 0;
	}

	twi_send_byte((byte_l | displayctrl) & ~PCF_ENABLE); 	// enable low
	twi_stop();
	return 1;
}

uint8_t _lcd_write_nibble_data(uint8_t byte)
{
	uint8_t byte_h = (byte & 0xf0); // mask high bits
	uint8_t byte_l = ((byte<<4) & 0xf0); // shift lower 4 up & mask high
	uint8_t displayctrl = 0;
	uint8_t status = 0;

	// check if display is enabled
	if(lcd.backlight) {
		displayctrl |= 0x08;
	}

										/**			HIGH BITS 			**/
	status = twi_start();
	if(!status) { // bad start, abort
		return 0;
	}
	status = twi_slaw(); // send SLA + W
	if(!status) { // bad addr or device not rdy to receive
		return 0;
	}
	twi_send_byte(byte_h | displayctrl | PCF_RS | PCF_ENABLE); // enable high
	twi_stop();

	status = twi_start();
	if(!status) { // bad start
		return 0;
	}
	status = twi_slaw();
	if(!status) { // bad addr or device not rdy to receive
		return 0;
	}
	twi_send_byte((byte_h | displayctrl | PCF_RS) & ~PCF_ENABLE); // enable low
	twi_stop();

										/**			LOW BITS			**/
	status = twi_start();
	if(!status) { // bad start
		return 0;
	}
	status = twi_slaw();
	if(!status) { // bad addr or device not rdy to receive
		return 0;
	}
	twi_send_byte(byte_l | displayctrl | PCF_RS | PCF_ENABLE); // enable high
	twi_stop();

	status = twi_start();
	if(!status) { // bad start
		return 0;
	}
	status = twi_slaw();
	if(!status) { // bad addr or device not rdy to receive
		return 0;
	}
	twi_send_byte((byte_l | displayctrl | PCF_RS) & ~PCF_ENABLE ); // enable low
	twi_stop();
	return 1;
}

// Write string of characters as DATA
uint8_t _lcd_write_nibble_string(uint8_t bytes[], uint8_t size)
{
	for(int i = 0; i < size; i++) {
		// handle special chars
		if(bytes[i] == '\0') {
			bytes[i] = ' ';
		}
		if(bytes[i] == '\n') {
			// ensure not on 2nd row already
		}
		lcd_nibble.data(bytes[i]);
	}
	return 1;
}

void cursor_on(void)
{
	uint8_t cmd = 0x08 | 0x02;
	lcd.cursor = 1;
	(lcd.characters) 	? cmd |= 0x04 : (0);
	(lcd.blink) 		? cmd |= 0x01 : (0);
	// turn on cursor
	lcd_nibble.command(cmd);
	return;
}

void cursor_off(void)
{
	uint8_t cmd = 0x08;
	lcd.cursor = 0;
	(lcd.characters) 	? cmd |= 0x04 : (0);
	(lcd.blink) 		? cmd |= 0x01 : (0);
	lcd_nibble.command(cmd);
	return;
}

void screen_on(void)
{
	uint8_t cmd = 0x08;
	lcd.backlight = 1;
	(lcd.characters) 	? cmd |= 0x04 : (0);
	(lcd.cursor) 		? cmd |= 0x02 : (0);
	(lcd.blink) 		? cmd |= 0x01 : (0);
	lcd_nibble.command(cmd);
	return;
}

void screen_off(void)
{
	uint8_t cmd = 0x08;
	lcd.backlight = 0;
	lcd_nibble.command(cmd);
	return;
}

void clear_display(void)
{
	uint8_t cmd = 0x01;
	lcd_nibble.command(cmd);
	_delay_ms(5);
	return;
}

void characters_on(void)
{
	uint8_t cmd = 0x08 | 0x04;
	lcd.characters = 1;
	(lcd.cursor) 	? cmd |= 0x02 : (0);
	(lcd.blink) 	? cmd |= 0x01 : (0);
	lcd_nibble.command(cmd);
	return;
}

void characters_off(void)
{
	uint8_t cmd = 0x08;
	lcd.characters = 0;
	lcd_nibble.command(cmd);
	return;
}

void set_cursor_pos(uint8_t column, uint8_t row)
{
	uint8_t cmd = 0x80;
	// check bounds
	if(column > lcd.columns) {
		column = lcd.columns;
	}
	if(column < 1) {
		column = 1;
	}

	if(row > lcd.rows) {
		row = lcd.rows;
	}
	if(row < 1) {
		row = 1;
	}

	(row == 2) ? cmd |= 0x40 : (0);
	cmd |= (column - 1);
	lcd_nibble.command(cmd);
	return;
}

void cursor_home(void)
{
	uint8_t cmd = 0x02;
	lcd_nibble.command(cmd);
	return;
}
