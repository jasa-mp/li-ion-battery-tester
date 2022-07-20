/* oled_lib.h */

// header guard
#ifndef OLED_LIB_H
#define OLED_LIB_H

// include
#include <avr/io.h>

// I2C
// declarations
#define SDA A4
#define SCL A5

#define START 0x08
#define SLAW_ACK 0x18
#define DATA_ACK 0x28

#define WRITE 0
#define READ  1


// OLED SSD1306
// Co  = 1<<7 -> Continuation bit             
// D/C = 1<<6 -> Data / Command Selection bit

#define COMMAND_SINGLE  0x80   // Co = 1 and D/C = 0, tells the controller its next byte will be a command
#define COMMAND_STREAM  0x00   // Co = 0 and D/C = 0, tells the controller its next bytes will be commands
#define DATA_SINGLE     0xC0   // Co = 1 and D/C = 1, tells the controller its next byte will be data
#define DATA_STREAM     0x40   /* Co = 0 and D/C = 1, tells the controller to not expect any more commant 
                                         bytes and to only expect data*/


// OLED SSD1306 Dimensions
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define comPins 0x12
#define contrast 0xCF


#define BLACK OLED_BLACK     // 'off' pixels
#define WHITE OLED_WHITE     // 'on' pixels
#define INVERSE OLED_INVERSE // Invert pixels

// fit into the SSD1306_ naming scheme
#define OLED_BLACK 0   // Draw 'off' pixels
#define OLED_WHITE 1   // Draw 'on' pixels
#define OLED_INVERSE 2 // Invert pixels


#define OLED_MEMORYMODE 0x20          
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22            
#define OLED_SETCONTRAST 0x81        
#define OLED_CHARGEPUMP 0x8D          
#define OLED_SEGREMAP 0xA1                  // 0xA0 or 0xA1, to change starting side, switch this to flip the display horizontally
#define OLED_DISPLAYALLON_RESUME 0xA4 
#define OLED_DISPLAYALLON 0xA5        
#define OLED_NORMALDISPLAY 0xA6       
#define OLED_INVERTDISPLAY 0xA7       
#define OLED_SETMULTIPLEX 0xA8        
#define OLED_DISPLAYOFF 0xAE         
#define OLED_DISPLAYON 0xAF          
#define OLED_COMSCANINC 0xC0            
#define OLED_COMSCANDIR OLED_COMSCANDEC      // OLED_COMSCANINC or OLED_COMSCANDEC switch this to flip the display vertically
#define OLED_COMSCANDEC 0xC8    
#define OLED_SETDISPLAYOFFSET 0xD3    
#define OLED_SETDISPLAYCLOCKDIV 0xD5  
#define OLED_SETPRECHARGE 0xD9        
#define OLED_SETCOMPINS 0xDA          
#define OLED_SETVCOMDETECT 0xDB       

#define OLED_SETLOWCOLUMN 0x00  
#define OLED_SETHIGHCOLUMN 0x10 
#define OLED_SETSTARTLINE 0x40  

#define OLED_EXTERNALVCC 0x01  // External display voltage source
#define OLED_SWITCHCAPVCC 0x02 // Gen. display voltage from 3.3V

#define OLED_RIGHT_HORIZONTAL_SCROLL 0x26              // Init rt scroll
#define OLED_LEFT_HORIZONTAL_SCROLL 0x27               // Init left scroll
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 // Init diag scroll
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  // Init diag scroll
#define OLED_DEACTIVATE_SCROLL 0x2E                    // Stop scroll
#define OLED_ACTIVATE_SCROLL 0x2F                      // Start scroll
#define OLED_SET_VERTICAL_SCROLL_AREA 0xA3             // Set scroll range





// function prototypes 
// I2C
void i2c_enable(void);
int  i2c_start(int);
int  i2c_write(int);
int  i2c_stop(void);
void i2c_disable(void);

// OLED
int OLED_init(void);
int OLED_send_command(int);
int OLED_send_data_once(int);                   // for 1 column operations
int OLED_send_data(unsigned char);              // for multiple columns
int OLED_send_string(char *string);             // for writing text
int OLED_send_int(unsigned int numb);                    // for sending an int number
int OLED_send_int2(int numb);                   // for sending an int number and writhing it as a two-digit number
int OLED_send_int4(int numb);
int OLED_send_int_23(int numb);                // for sending an adc value mapped to voltage and then multiplied by 1000
                                               // and then displayed as a floating point number
int OLED_set_cursor(int colum, int page);       // for setting cursor to a specific location
int OLED_set_brightness(uint8_t level);         // set the brightness of the display
int OLED_clear(void);


#endif