/* adc.c */

#include <adc.h>




void adc_init(void){
    // ATmega328p ADC config code

        ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);     // clkADC = sysclk/128
                                                            // must be between 50 kHz and 200 kHz
        /*
        REFS1 REFS0
         0     0	    AREF, internal Vref off
         0     1	    AVcc with external capacitor at AREF pin 			<-
         1     1	    Internal 1.1V ref with external capacitor at AREF pin
        */

        ADMUX  = (1<<REFS0);
}


/*
This function reads the analog value on pins A0-A5
int pin_number -> pin number
value = (Vin*1024)/Vref
*/
int adc_read(int pin_number){
    // variables
    int ADC_channel;
    int ADC_value;

    if ((pin_number>=14)&&(pin_number<=19)){
        ADC_channel = pin_number-14;

        ADMUX  = ADMUX & 0xF0;              //Clear MUX0-MUX3,Important in Multichannel conv
        ADMUX |= ADC_channel;

        ADCSRA |= (1<<ADEN)  | (1<<ADSC);   // enable and start 

        while(!(ADCSRA & (1<<ADIF)));       // waiting for conversion

        ADC_value = ADC;        // ADC varable contains all 10 bits of the

        ADCSRA |= (1<<ADIF);    // Clear the ADIF Flag , ADIF is cleared by writing a 1 to ADSCRA (According to ATmega328p datasheet)
		
    }
    else {
        // error
        return -1;
    }
    
    return ADC_value;

}

