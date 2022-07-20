#include <rtos.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <pin_io.h>

uint32_t rtos_init(void){
    /* timer config
        COMPARE MODE
        WGM13:0 = 4 -> WHM12 = 1
        OCR1A = reset value
     */

    // tick/slice = RTOS_SLICE_US/T_tick = RTOS_SLICE_US/(1/F_timer) = 
    //              RTOS_SLICE_US*F_CPU_MH/8 = RTOS_SLICE_US * 2

    if((2*RTOS_SLICE_US) > (65536-1)){
        /*  if the ticks for the slice are more than the counter can count -> it would overflow
            return -1 as an error
         */
        return -1;
    }

    TIMSK1 |= 1<<OCIE1A;                // enable interrupt
    TCCR1B |= 1<<WGM12;                 // set mode
    OCR1A = 2*RTOS_SLICE_US;            // set reset value


    sei();                 // enable global interrupts, set the bit in SREG 

    return 1;
}


void rtos_enable(void){
    // to enable the timer set the clock
    
    TCCR1B |= (0<<CS12)|(1<<CS11)|(0<<CS10);    // set clk prescaler -> 8
}


void rtos_disable(void){
    // to disable the timer set al the CSxx bits to 0
    
    TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));    
}




// extern, because the definition is not in this file
extern rtos_task_t *rtos_task_list[];

ISR(TIMER1_COMPA_vect) { 
    static uint8_t idx = 0;

    rtos_task_list[idx]->fun();
    idx++;

    if ( rtos_task_list[idx] == 0){
        idx = 0;
    }


    
    if (TIFR1 & (1 << OCF1A)) {
        //  if there is already another interrupt while in ISR

        // stop the rtos sistem
        while(1){
            set_pin_state(13, LOW);
        }
    }
    
    

}