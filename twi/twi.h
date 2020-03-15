#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>


/**
  ~ Frequently played ~     :)
  Eminem - Venom
  A Boogie Wit da Hoodie - No Comparison
  A Himitsu - Easier To Fade feat. Madi Larson
  Whatever happens, happens... // Verzache -> YouTube
  A Boogie Wit da Hoodie - Another Day Gone feat. Khalid
  A Boogie Wit da Hoodie - Me and My Guitar
  XXXTENTACION - MOONLIGHT
  Lil Peep - Save That Shit
  */

/**
  TWI operation return codes
  */
#define TW_START                  0x08
#define TW_REP_START              0x10
#define TW_MT_SLA_ACK             0x18
#define TW_MT_SLA_NACK            0x20
#define TW_MT_DATA_ACK            0x28
#define TW_MT_DATA_NACK           0x30
#define TW_MT_ARB_LOST            0x38
#define TW_MR_ARB_LOST            0x38
#define TW_MR_SLA_ACK             0x40
#define TW_MR_SLA_NACK            0x48
#define TW_MR_DATA_ACK            0x50
#define TW_MR_DATA_NACK           0x58
#define TW_ST_SLA_ACK             0xA8
#define TW_ST_ARB_LOST_SLA_ACK    0xB0
#define TW_ST_DATA_ACK            0xB8
#define TW_ST_DATA_NACK           0xC0
#define TW_ST_LAST_DATA           0xC8
#define TW_SR_SLA_ACK             0x60
#define TW_SR_ARB_LOST_SLA_ACK    0x68
#define TW_SR_GCALL_ACK           0x70
#define TW_SR_ARB_LOST_GCALL_ACK  0x78
#define TW_SR_DATA_ACK            0x80
#define TW_SR_DATA_NACK           0x88
#define TW_SR_GCALL_DATA_ACK      0x90
#define TW_SR_GCALL_DATA_NACK     0x98
#define TW_SR_STOP                0xA0
#define TW_NO_INFO                0xF8
#define TW_BUS_ERROR              0x00

/**
  i2c bus settings
  */
struct i2c_bus {
	uint8_t d_addr;    /* Destination address */
	uint8_t d_reg;     /* Destination register */
	uint8_t data;      /* Contains data to be written or data received */
	uint8_t retval;    /* Holds success/fail of last bus activity */
} bus;

/**
  I2C specific functions
*/
void twi_init(uint8_t addr); // Initialize I2C bus

uint8_t twi_start(void); // Sends start condition on bus
void twi_stop(void); // Shutdown I2C hardware & exit the bus

uint8_t twi_slaw(void); // Send contained address on bus with specified WRITE bit(0)
uint8_t twi_slar(void); // Send contained address on bus with specified READ bit(0)

uint8_t twi_write(uint8_t byte); // Send specified byte to contained address on bus struct

uint8_t _send_byte(uint8_t byte); // Private function sends byte of data on bus
