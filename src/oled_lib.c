/* oled_lib.c */

#include <oled_lib.h>
#include <pin_io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#include <normal_font.h>
#include <digital_font.h>


// declarations
uint_fast8_t init_sequence[] = {OLED_DISPLAYOFF, 
                                OLED_SETDISPLAYCLOCKDIV, 
                                0x80, 
                                OLED_SETMULTIPLEX,
                                OLED_HEIGHT-1,
                                OLED_SETDISPLAYOFFSET,
                                0x00,
                                OLED_SETSTARTLINE,
                                OLED_CHARGEPUMP,
                                0x14,                           // enable
                                OLED_MEMORYMODE,                // memory addressing mode
                                0x00,
                                OLED_SEGREMAP,                  // change this in oled_lib.h file to flip the displa horizontally
                                OLED_COMSCANDIR,                // change this in oled_lib.h file to flip the displa vertically
                                OLED_COLUMNADDR,                // Set column address
                                0x00,                           // start
                                0x7F,                           // stop
                                OLED_PAGEADDR,                  // Set page address
                                0x00,                           // start
                                0x07,                           // stop
                                OLED_SETCOMPINS,
                                comPins,
                                OLED_SETCONTRAST,
                                contrast,
                                OLED_SETPRECHARGE,
                                0xF1,
                                OLED_SETVCOMDETECT,
                                0x00,                           // normal 0x40, set to 0x00 for wider range of contrast
                                OLED_DISPLAYALLON_RESUME,
                                OLED_NORMALDISPLAY,
                                OLED_DEACTIVATE_SCROLL,
                                OLED_DISPLAYON};  


                    /* shorter init_sequence
                       {0xD9,
                        0xF1,
                        0x8D,
                        0x14,
                        0xDB,
                        0x40,
                        0xA1,
                        0xC8,
                        0xAF,
                        0xA4,
                        0x20,
                        0x00};*/



// I2C functions 
//-------------------------------------------------------------------------------------

void i2c_enable(void){
    // Enable the 2-wire serial interface
    
    // pins SCL and SDA need external PULLUP resistors or enabled internal ones
    set_pin_mode(SDA, INPUT_PULLUP);
    set_pin_mode(SCL, INPUT_PULLUP);

    // Bit rate generator unit:
    // SCL frequency = CPU_clk/(16+2TWBR*TWSR) 
    // TWBR -> 72   TWSR -> 0 (TWPS1 = 0 TWPS0 = 0 -> prescaler 1) -> 100k
    // TWBR -> 32   TWSR -> 0 (TWPS1 = 0 TWPS0 = 0 -> prescaler 1) -> 200k
    // TWBR -> 16   TWSR -> 0 (TWPS1 = 0 TWPS0 = 0 -> prescaler 1) -> 400k

    TWBR = 16;
    TWSR = 0;


    // TWEN -> must be set to 1 to enable the 2-wire serial interface
    TWCR = (1<<TWEN);
}


void i2c_disable(void){
    // Disable the 2-wire serial interface
        
    // TWEN -> must be set to 0 to disable the 2-wire serial interface
    TWCR = (0<<TWEN);
}


int i2c_start(int address){
    // This function sends the start command and sets the address of the slave

    if (TWCR&(1<<TWEN)){
        // START condition 
        /*  TWINT set to 1 to clear the flag
            TWSTA set to 1 to transmit a START condition
        */
        TWCR = (1<<TWINT)|(1<<TWSTA)|(0<<TWSTO)|(1<<TWEN);

        // Wait for TWINT flag set
        while (!(TWCR & (1<<TWINT)));

        if ((TWSR&0xF8)!= START){        // if START was not transmited exit
            return -1;
        }

        // TWI Data Register
        // sending SLA+W (address + WRITE)
        TWDR = (address<<1)+WRITE;        // address packet + READ/WRITE is called an SLA+R or SLA+W
                                        // this is loaded into the data register

        // SEND contiton
        /*  TWINT set to 1 to clear the flag
            TWSTA set to 0 to transmit end of START condition
        */
        TWCR = (1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEN);

        // Wait for TWINT flag set
        while (!(TWCR & (1<<TWINT)));

        if ((TWSR&0xF8)!= SLAW_ACK){        // if SLA+W was not transmited exit
            return -1;
        }
        return 1;
    }
    return 0;

}


int i2c_write(int data){
    // This function sends 8 bit data

    if (TWCR&(1<<TWEN)){
        // TWI Data Register
        TWDR = data;

        // SEND contiton
        /*  TWINT set to 1 to clear the flag
            TWSTA set to 0 to transmit end of START condition
        */
        TWCR = (1<<TWINT)|(0<<TWSTA)|(0<<TWSTO)|(1<<TWEN);

        // Wait for TWINT flag set
        while (!(TWCR & (1<<TWINT)));

        if ((TWSR&0xF8)!= DATA_ACK){        // if data was not transmited exit
            return -1;
        }

        return 1;
    }
    return 0;
    
}


int i2c_stop(void){

    if (TWCR&(1<<TWEN)){
        // STOP condition 
        /*  TWINT set to 1 to clear the flag
            TWSTA set to 0 
            TWSTO set to 1 to transmit a STOP condition
        */
        TWCR = (1<<TWINT)|(0<<TWSTA)|(1<<TWSTO)|(1<<TWEN);

        // Wait for TWSTO to be reset to 0
        while ((TWCR & (1<<TWSTO)));

        return 1;
    }
    return 0;
}


//-------------------------------------------------------------------------------------


// OLED functions

int OLED_init(void){
    /* starts i2c communication and sends one control byte telling the controller
       to expect only command bytes after it, it then starts sending the initcialization 
       sequence and stops i2c communication afterwards 

       if any of the commands sent are not recieved it returns an error (-1)
    */
    i2c_start(0x3C);                // address
    i2c_write(COMMAND_STREAM);     // control byte
    for (int i = 0; i < sizeof(init_sequence); i++){
        // sizeof() returns number of bytes
        if ( -1 == i2c_write(init_sequence[i])){
            return -1;
        }
    }
    i2c_stop();

    // clear the display 
    for (int i = 0; i < 8192; i++){
        OLED_send_data_once(0);
    }

    return 1;

}


int OLED_send_data_once(int data){
    // for 1 column operations
    i2c_start(0x3C);

    i2c_write(DATA_SINGLE);     // control byte

    if ( -1 == i2c_write(data)){
            return -1;
    }

    i2c_stop();


    return 1;
}


int OLED_send_command(int command){
    i2c_start(0x3C);

    i2c_write(COMMAND_SINGLE);     // control byte

    if ( -1 == i2c_write(command)){
            return -1;
    }

    i2c_stop();


    return 1;
}


int OLED_send_data(unsigned char character){
    /* for multiple columns
    the recieved character is found in the font array and 
    sent to the display one colum at a time
    */
    int array_index = (character - ' ')*5;
    i2c_start(0x3C);

    i2c_write(DATA_STREAM);     // control byte

    for (int i = 0; i < 5; i++){
        if ( -1 == i2c_write(fontData[array_index+i])){
            return -1;
        }
    }

    i2c_stop();


    return 1;
}



int OLED_send_string(char *string){
    // function parameter string is sent character by character
    // only sends characters, can not send variables, for that check OLED_send_number
    for (int i = 0; string[i] != 0; i++){
        if (-1 == OLED_send_data(string[i])){
            return -1;
        }
    }

    return 1;
}


int OLED_send_int(unsigned int numb){
    // function parameter number is writen as a string with the help of sprintf function
    // and then sent to the display one character at a time

    char OLED_buffer[8]; 
    sprintf(OLED_buffer, "%u", numb);

    for (int i = 0; OLED_buffer[i] != 0; i++){
        if (-1 == OLED_send_data(OLED_buffer[i])){
            return -1;
        }
    }

    return 1;
}


int OLED_send_int2(int numb){
    // function parameter number is writen as a string with the help of sprintf function
    // because stopwatches have two-digit numbers, this function uses %2d to write the number
    // change the number infront of "d" if need be 
    // and then sent to the display one character at a time

    char OLED_buffer[8]; 
    sprintf(OLED_buffer, "%02d", numb);

    for (int i = 0; OLED_buffer[i] != 0; i++){
        if (-1 == OLED_send_data(OLED_buffer[i])){
            return -1;
        }
    }

    return 1;
}


int OLED_send_int4(int numb){
    // function parameter number is writen as a string with the help of sprintf function
    // because stopwatches have two-digit numbers, this function uses %2d to write the number
    // change the number infront of "d" if need be 
    // and then sent to the display one character at a time

    char OLED_buffer[8]; 
    sprintf(OLED_buffer, "%4d", numb);

    for (int i = 0; OLED_buffer[i] != 0; i++){
        if (-1 == OLED_send_data(OLED_buffer[i])){
            return -1;
        }
    }

    return 1;
}




int OLED_send_int_23(int numb){
    // function parameter number is writen as a string with the help of sprintf function
    // and then sent to the display one character at a time

    char OLED_buffer[8]; 

    // numb to two integers, to be written as a "float"
    int int_integers = numb/1000;
    int int_decimals  = numb%1000;

    sprintf(OLED_buffer, "%2d.%03d", int_integers, int_decimals);

    for (int i = 0; OLED_buffer[i] != 0; i++){
        if (-1 == OLED_send_data(OLED_buffer[i])){
            return -1;
        }
    }

    return 1;
}


int OLED_set_cursor(int colum, int page){
    // for setting cursor to a specific location
    uint8_t cursor_command[] = {
        OLED_COLUMNADDR,                // Set column address
        colum,                          // start
        0x7F,                           // stop
        OLED_PAGEADDR,                  // Set page address
        page,                           // start
        0x07                            // stop
    };

    i2c_start(0x3C);                // address
    i2c_write(COMMAND_STREAM);     // control byte

    for (int i = 0; i < 6; i++){
        if ( -1 == i2c_write(cursor_command[i])){
            return -1;
        }
    }
    i2c_stop();

    return 1;                           
}


int OLED_set_brightness(uint8_t level){
    /* set brightness value according to function parameter level,
       it has to be a value between 0 and 255
    */
    i2c_start(0x3C);                // address
    i2c_write(COMMAND_STREAM);     // control byte

    i2c_write(OLED_SETCONTRAST);
    i2c_write(level);

    i2c_stop();

    return 1;
}


int OLED_clear(void){
    // clear the display 
    for (int i = 0; i < 8192; i++){
        if( -1 == OLED_send_data_once(0)){
            return -1;
        }
    }
    
    return 1;

}

//-------------------------------------------------------------------------------------