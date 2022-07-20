/* adc.h */

// header guard
#ifndef ADC_H
#define ADC_H

// include
#include <stdbool.h>
#include <avr/io.h>

// declarations 
#define ADC_Temp_Sensor 0b00001000  // Internal temp. sensor

void adc_init(void); // adc setup
int adc_read(int pin_number);   // read analog value on pin A0-A5



#endif