/* pin_io.c */

#include <pin_io.h>


void set_pin_mode(int pin, int mode){ 
    // pinMode
    /*
        int pin -> pin number
            port D from pin 0 to 7 
            port B from pin 8 to 13
            port C from pin 14 to 19 (analog)

        int mode -> pin mode
            OUTPUT = 0 = output
            INPUT  = 1 = input
            INPUT_PULLUP  = 2 = input with pullup enabled 

        Sets the mode of the pin, INPUT/OUTPUT

    */

    if ((pin >= 0)&&(pin <= 7)){
        if (mode == OUTPUT){
            DDRD |= (1<<pin);    // set as OUTPUT
        }
        else if (mode == INPUT){
            DDRD &= ~(1<<pin);   // set as INPUT
        }
        else if (mode == INPUT_PULLUP){
            DDRD &= ~(1<<pin);   // set as INPUT
            PORTD |= (1<<pin);   // enable PULLUP
        }
    }
    else if ((pin >= 8)&&(pin <= 13)){
        if (mode == OUTPUT){
            DDRB |= (1<<(pin-8)); // set as OUTPUT
        }
        else if (mode == INPUT){
            DDRB &= ~(1<<(pin-8));// set as INPUT
        }
        else if (mode == INPUT_PULLUP){
            DDRD &= ~(1<<(pin-8));   // set as INPUT
            PORTD |= (1<<(pin-8));   // enable PULLUP
        }
    }
    else if ((pin >= 14)&&(pin <= 19)){
        if (mode == OUTPUT){
            DDRC |= (1<<(pin-14)); // set as OUTPUT
        }
        else if (mode == INPUT){
            DDRC &= ~(1<<(pin-14));// set as INPUT
        }
        else if (mode == INPUT_PULLUP){
            DDRC &= ~(1<<(pin-14));   // set as INPUT
            PORTC |= (1<<(pin-14));   // enable PULLUP
        }
    }
    else {
        // might add a warning
    }

}


void set_pin_state(int pin, bool state){
    // digitalWrite
    /*
    int pin -> pin number
        port D from pin 0 to 7 
        port B from pin 8 to 13
        port C from pin 14 to 19 (analog)

    bool mode -> pin mode
        HIGH  = 1 
        LOW = 0 

    Sets the pin to 5V, if state = HIGH or to 0V, if the state = LOW

    */

   if ((pin >= 0)&&(pin <= 7)){
       if (state == 1){
           PORTD |= (1<<pin);
       }
       else if (state == 0){
           PORTD &= ~(1<<pin);
       }
    }
    else if ((pin >= 8)&&(pin <= 13)){
       if (state == 1){
           PORTB |= (1<<(pin-8));
       }
       else if (state == 0){
           PORTB &= ~(1<<(pin-8));
       }
    }
    else if ((pin >= 14)&&(pin <= 19)){
       if (state == 1){
           PORTC |= (1<<(pin-14));
       }
       else if (state == 0){
           PORTC &= ~(1<<(pin-14));
       }
    }
    else {
        // might add a warning
        // return = 1?
    }



}


int  get_pin_state(int pin){
    // digitalRead
    /*
    int pin -> pin number
        port D from pin 0 to 7 
        port B from pin 8 to 13
        port C from pin 14 to 19 (analog)        

    Reads the value on pin, returns 1, if HIGH and 0, if LOW 

    */
    
    // variables 
    int state;
    int shift;

    if ((pin >= 0)&&(pin <= 7)){
        state = (PIND & (1<<pin))>>pin;
    }
    else if ((pin >= 8)&&(pin <= 13)){
        shift = pin-8;
        state = (PINB & (1<<shift))>>shift;
    }
    else if ((pin >= 14)&&(pin <= 19)){
        shift = pin-14;
        state = (PINC & (1<<shift))>>shift;
    }
    else {
        // might add a warning
        // return = -1?
    }

    return state;

}


void  toggle_pin_state(int pin){
    // toggle pin x
    /*
    int pin -> pin number
        port D from pin 0 to 7 
        port B from pin 8 to 13
        port C from pin 14 to 19 (analog)        

    Writing a 1 to a bit in register PINX toggles the state of that pin

    */
    
    // variables 
    int shift;

    if ((pin >= 0)&&(pin <= 7)){
        PIND = (1<<pin);
    }
    else if ((pin >= 8)&&(pin <= 13)){
        shift = pin-8;
        PINB = (1<<shift);
    }
    else if ((pin >= 14)&&(pin <= 19)){
        shift = pin-14;
        PINC = (1<<shift);
    }
    else {
        // might add a warning
        // return = -1?
    }
    
}