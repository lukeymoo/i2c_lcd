#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

/**
  ~ Frequently played ~     :)
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
  PINOUT for PCF8574T to LCD control pins
*/
#define PCF_RS                    0b00000001  //P0 - PCF8574T Pin connected to RS
#define PCF_RW                    0b00000010  //P1 - PCF8574T Pin connected to RW
#define PCF_ENABLE                0b00000100  //P2 - PCF8574T Pin connected to EN
#define PCF_BACKLIGHT             0b00001000  //P3 - PCF8574T Pin connected to BACKLIGHT LED

/**
  i2c LCD settings
*/
struct i2c_lcd {
  uint8_t l_addr;          /* Slave device address                               || default 0x27 */
  uint8_t l_backlight;   /* Backlight on/off control                           || default 1 */
} lcd;

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
} bus;


/**
  HERE ARE THE FUNCTIONS FOR CONTROLLING THE LCD DISPLAY
  HELPS TO NOT NEED TO WRITE 0X80, 0X03 ETC FOR COMMANDS EVERYTIME
*/
void clear_display(void);
void set_position(uint8_t row, uint8_t column);
void cursor_on(void);
void cursor_off(void);
void display_on(void);  // not the actual backlight
void display_off(void); // not the actual backlight
void screen_on(void);   // actual backlight -- turns on character display & backlight
void screen_off(void); // actual backlight  -- turns off character display & backlight
void lcd_init(void); // Initializes LCD screen
void lcd_write(uint8_t byte); // LCD Write Command - 8 Bit
void lcd_send(uint8_t byte); // LCD Write Command - 4 bit
void lcd_dsend(uint8_t byte); // LCD Write Data - 4 bits
void lcd_string(char *words, uint8_t num_chars); // LCD Write a string of characters - 4 bits
void led_write(uint8_t byte); // Outputs a byte of data to LEDs

/**
  I2C specific functions
  These functions aren't focused around controlling
  an LCD display but the bus itself
*/
void twi_init(void); // Initialize I2C bus
unsigned char twi_start(void); // Sends start condition on bus
void twi_set_addr(uint8_t addr); // Set destination address of bus reads/writes
void twi_loadbuf(uint8_t byte); // Places specifed byte into buffer
unsigned char twi_write(void); // Write byte of data onto bus
unsigned char twi_slaw(void); // Send contained address on bus with specified WRITE bit(0)
unsigned char _send_byte(uint8_t byte); // Private function sends byte of data on bus
void twi_stop(void); // Shutdown I2C hardware & exit the bus
