#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/**
  ~ Frequently played ~     :)
  Whatever happens, happens... // Verzache -> YouTube
  A Boogie Wit da Hoodie - Another Day Gone feat. Khalid
  A Boogie Wit da Hoodie - Me and My Guitar
  XXXTENTACION - MOONLIGHT
  Lil Peep - Save That Shit
*/

/**
  Debugging pins
*/
#define DEBUG_BLUE                PD2
#define DEBUG_RED                 PD3
#define DEBUG_GREEN               PD4

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
  PINOUT for PCF8574T to LCD control pins
*/
#define PCF_RS                    0b00000001  //P0 - PCF8574T Pin connected to RS
#define PCF_RW                    0b00000010  //P1 - PCF8574T Pin connected to RW
#define PCF_ENABLE                0b00000100  //P2 - PCF8574T Pin connected to EN
#define PCF_BACKLIGHT             0b00001000  //P3 - PCF8574T Pin connected to BACKLIGHT LED

/**
  TWI bus state list
*/
#define TWI_READY         0x01
#define TWI_TRANSMITTING  0x02
#define TWI_RECEIVING     0x03

/**
  LCD struct info, limited to 1 bit
*/

#define LCD_BACKLIGHT_ON  0x01
#define LCD_BACKLIGHT_OFF 0x00

#define LCD_DOUBLE_ROW    0x01
#define LCD_SINGLE_ROW    0x00

#define LCD_16_COLS       0x00
#define LCD_20_COLS       0x01

#define LCD_CURSOR_ON     0x01
#define LCD_CURSOR_OFF    0x00

#define LCD_BLINK_ON      0x01
#define LCD_BLINK_OFF     0x00

/**
  Debugging LEDS
*/
struct debugging {
  uint8_t red; // defaults all off
  uint8_t green;
  uint8_t blue;
};

// Outputs a byte of data to LEDs
void led_write(uint8_t byte);

/**
  Initializes debugging
*/
void init_debug(struct debugging *bug);

/**
  Set specified LED
*/
void set(uint8_t pin);

/**
  Clears specified LED
*/
void clear(uint8_t pin);

/**
  i2c LCD settings
*/
struct i2c_lcd {
  uint8_t l_addr;          /* Slave device address                               || default 0x27 */
  uint8_t l_backlight:1;   /* Backlight on/off control                           || default 1 */
  uint8_t l_cursor:1;      /* Cursor on/off control                              || default 1 */
  uint8_t l_blink:1;       /* Cursor blinking on/off control                     || default 1 */
  uint8_t l_rows:1;        /* number of rows :: 0 = single row, 1 = double row   || default 0 */
  uint8_t l_cols:1;        /* Number of columns :: 0 = 16 cols , 1 = 20 cols     || default 0 */
};

/**
  i2c bus settings
*/
struct i2c_bus {
  uint8_t b_addr;    /* Contains address to be addressed as       || default 0 */
  uint8_t d_addr;    /* Destination address */
  uint8_t d_reg;     /* Destination register */
  uint8_t data;      /* Contains data to be written or data received */
  uint8_t b_state;   /* Tracks idle, transmitting, recieving, etc || default TW_READY */
  uint8_t retval;    /* Holds success/fail of last bus activity */
};

/**
  Initialize I2C bus without setting an address to become slave
  Must receive bus & lcd structures
*/
void twi_init(struct i2c_bus *bus, struct i2c_lcd *lcd);

/**
  Sends start condition on bus
*/
unsigned char twi_start(void);

/**
  Set destination address of bus reads/writes
*/
void twi_set_addr(struct i2c_bus *bus, uint8_t addr);

/**
  Places specifed byte into buffer
*/
void twi_loadbuf(struct i2c_bus *bus, uint8_t byte);

/**
  Write byte of data onto bus
*/
unsigned char twi_write(struct i2c_bus *bus);

/**
  Send contained address on bus with specified WRITE bit(0)
*/
unsigned char twi_slaw(struct i2c_bus *bus);

/**
  Private function
  Sends byte of data on bus
*/
unsigned char _send_byte(uint8_t byte);

/**
  Shutdown I2C hardware & exit the bus
*/
void twi_stop(void);

/**
  Set pin ON
*/
void set(uint8_t pin);

/**
  Set pin OFF
*/
void clear(uint8_t pin);