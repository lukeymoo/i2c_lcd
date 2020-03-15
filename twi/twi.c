#include "twi.h"

/**
  Initialize I2C bus
  */
void twi_init(uint8_t addr)
{
	//defaults
	bus.d_addr 	= addr;
	bus.d_reg 	= 0x00;
	bus.data 	= 0x00;
	bus.retval 	= 0x00;

	/* Set up i2c hardware */
	TWSR = 0;   // No prescaler -- Prescaler is the lower 2 bits of status register
	TWBR = 72;  // baud rate target = 100kbs, divisor is 72 at 16 mhz
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);  // enable i2c
	return;
}

uint8_t twi_start(void)
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
  Write byte of data onto bus
  */
uint8_t twi_write(uint8_t byte)
{
	/* Produce START condition */
	bus.retval = twi_start();
	if(bus.retval == 0) { return 0; } // failed, stop operation
	/* Send the address from buffer */
	bus.retval = twi_slaw();

	if(!bus.retval) { return 0; } // failed, stop operation

	/* Send the data from buffer */
	bus.retval = _send_byte(byte);
	if(!bus.retval) { return 0; } // failed, stop operation
	return 1; // success
}

/**
  Sends address on bus with WRITE bit (0)
  */
uint8_t twi_slaw(void)
{
	bus.retval = 0;
	TWDR = bus.d_addr << 1;
	TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
	while(!(TWCR & (1 << TWINT))); // wait
	if((TWSR & 0xf8) == TW_MT_SLA_ACK) { // on success ret 1
		return 1;
	}
	return 0;
}

// Sends address on bus with READ bit(1)
uint8_t twi_slar(void)
{
	bus.retval = 0;
	TWDR = (bus.d_addr << 1) | 0x01;
	TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
	while(!(TWCR & (1 << TWINT))); // wait
	if((TWSR & 0xf8) == TW_MR_SLA_ACK) {
		return 1;
	}
	return 0;
}

/**
  Private function
  Sends byte of data on bus
  */
uint8_t _send_byte(uint8_t byte)
{
	TWDR = byte; // load buffer
	TWCR = (1 << TWEN) | (1 << TWINT); // hardware operation
	while(!(TWCR & (1 << TWINT))); // wait
	if((TWSR & 0xf8) == TW_MT_DATA_ACK) { // good data, return success
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
