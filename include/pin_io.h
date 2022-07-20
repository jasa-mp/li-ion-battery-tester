/* pin_io.h */


// header guard
#ifndef PIN_IO_H    // random variable, that we are looking 
#define PIN_IO_H    // at, if its defined or not

// include
#include <stdbool.h>
#include <avr/io.h>

// declarations
#define OUTPUT 0
#define INPUT  1
#define INPUT_PULLUP 2
#define LOW    0
#define HIGH   1

// analog pins
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

// other
#define RESET 29




// function prototypes 
void set_pin_mode(int, int);    // pinMode
void set_pin_state(int, bool);  // digitalWrite
int  get_pin_state(int);        // digitalRead
void toggle_pin_state(int);     // toggle pin x


#endif 