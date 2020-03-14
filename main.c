#define F_CPU 16000000UL

#include "i2c.h"

int main(void)
{
	char buffer[2] = {'\0'};
	char retval[4] = {'\0'};
	bus.r_addr = 0x3a;
	twi_init();
	lcd_init();
	itoa(bus.r_addr, buffer, 16);
	lcd_string("Sending request\nto 0x", 22);
	lcd_dsend(buffer[0]);
	lcd_dsend(buffer[1]);
	_delay_ms(700);
	// send start condition
	twi_start();
	// send read request to device
	// output status of SLA R
	uint8_t status = twi_slar();
	status = (TWSR & 0xf8);

	buffer[0] = 0;
	buffer[1] = 0;

	switch(status) {
		case TW_MR_SLA_ACK: // SLA R sent ACK received
			TWCR = (1 << TWINT) | (1 << TWEN); // read data and return NACK
			while(!(TWCR & (1 << TWINT))); // wait for data

			status = TWSR & 0xf8; // we should be checking this status
			retval[0] = 'L';
			retval[1] = 'L';
			switch(status) { // checking status of READ operation
				case TW_MR_DATA_ACK: // data received ACK sent
					retval[0] = TWDR; // read data
					retval[1] = 'X';
					TWCR = (1 << TWINT) | (1 << TWEN); // read again
					while(!(TWCR & (1 << TWINT))); // wait
					retval[1] = TWDR; // read 2nd value
					// send stop
					TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
					while(!(TWCR & (1 << TWSTO))); // wait
					break;
				case TW_MR_DATA_NACK: // data received NACK sent
					retval[0] = TWDR;
					retval[1] = 'O';
					// send stop
					TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
					while(!(TWCR & (1 << TWSTO))); // wait
					break;
			}
			break;
		case TW_MR_SLA_NACK: // SLA R sent NACK received
			break;
	}

	// output status code
	if(status == 0x40) { // SLA R sent ACK received

	}
	if(status == 0x48) { // SLA R sent NACK received
		// send stop and write status
		status = TWSR & 0xf8;
		TWCR = (1 << TWSTO) | (1 << TWINT);
		while(!(TWCR & (1 << TWSTO))); // wait
		clear_display();
		lcd_string("SLANACK>", 9);
		itoa(status, buffer, 16);
		lcd_dsend(buffer[0]);
		lcd_dsend(buffer[1]);
		_delay_ms(3000);
	}


	clear_display();

	// f8 at this point means status has been read already
	lcd_string("Output>", 8);
	lcd_dsend(retval[0]);
	lcd_dsend(retval[1]);


	while(1) {
	}

	return 0;
}
