#include <rtos_tasks.h>
#include <rtos.h>

#include <pin_io.h>
#include <adc.h>
#include <oled_lib.h>

// declarations
uint32_t time_seconds = 0;
uint64_t seconds;
uint64_t minutes;
uint64_t hours;

uint8_t update_flag = 1;            // a flag to signal the program to calculate new values and display them
uint8_t enable = 0;                /*   - when the button start/stop is pressed it sets enable to 1
                                        - it is then set to 0 either when the test is finished when the battery
                                          is at battery_voltage_LOW or when the button pressed again
                                    */
uint8_t emptybat_flag = 0;          


int battery_voltage;
int battery_current;
uint64_t battery_capacity = 0;

const float shunt_resistance = 0.5;         // resistance of the shunt resistor in use
float battery_voltage_LOW = 3.0;            // default voltage at which the battery is considered empty 
const float battery_voltage_offset = 0.2;   // battery voltage offset because of the resistance of the wires and other factors

// tasks
rtos_task_t task_time = {
    .last_tick = 0,     // could be used to remember when it was last called
    .name = "time",
    .fun = stopwatch

};

rtos_task_t task_battery = {
    .last_tick = 0,
    .name = "buttons",
    .fun = battery_checker

};

rtos_task_t task_OLED = {
    .last_tick = 0,
    .name = "lcd",
    .fun = write_to_oled

};

rtos_task_t task_buttons = {
    .last_tick = 0,
    .name = "buttons",
    .fun = button_status

};

rtos_task_t task_blink = {
    .last_tick = 0,     
    .name = "blink",
    .fun = blink_led

};

void stopwatch(void){
    // 10 ms between tasks, 5 tasks currently -> 50 ms to do a fool loop
    static int i = 0;

    if (enable == 1){
        if (i == 20){
            i = 0;
            time_seconds++;            
            update_flag = 1;
        }
        else {
            update_flag = 0;
        }

        i++;
    }

    if (update_flag == 1){
        seconds = time_seconds % 60;
        minutes = (time_seconds / 60) % 60;
        hours = (time_seconds / 3600) % 24;
    }
}


void battery_checker(void){
    uint32_t V_reading;          // analog value of the ADC pin for battery voltage measurement
    uint32_t I_reading;          // analog value of the ADC pin for battery current measurement

    /* uint16_t would be enough for storing the analog value, which it 10 bit, but
       the calculations for battery_voltage and battery_current require more bits 
       to store the big value 
     */
    
    if (update_flag){
        V_reading = adc_read(A0); 
        I_reading = adc_read(A2);   

        battery_voltage = (5*1000*V_reading)/1023;
        if (battery_voltage > 600){
            battery_voltage = battery_voltage-(int)(battery_voltage_offset*1000);
        }
        battery_current = (5*1000*I_reading)/(1023*shunt_resistance);   // in mA
        battery_capacity += ((uint64_t)battery_current*1*1000)/3600;    // in uAh

        if (battery_voltage <= (battery_voltage_LOW*1000)){
            emptybat_flag = 1;
        }
    }
}


void write_to_oled(void){
    
    if (update_flag){
        // time display setup
        OLED_set_cursor(70,1);
        OLED_send_int2(hours);
        OLED_send_string(":");
        OLED_send_int2(minutes);
        OLED_send_string(":");
        OLED_send_int2(seconds);

        // battery status display setup
        OLED_set_cursor(45,4);
        OLED_send_int_23(battery_voltage);
        OLED_set_cursor(45,5);
        OLED_send_int_23(battery_current);
        OLED_set_cursor(55,6);
        OLED_send_int4((unsigned int)(battery_capacity/1000));

        update_flag = 0;
    }
    
    

    

}


void button_status(void){
    static int previous_state = 1;

    // start / stop
    if (((get_pin_state(7) == 0) && (previous_state == 1)) || (emptybat_flag == 1)){

        if ((enable == 0) && (emptybat_flag == 0)){
            enable = 1;
            set_pin_state(11, HIGH);
        }
        else {                      
            enable = 0;
            set_pin_state(11, LOW);
            update_flag = 1;
        }

    }

    previous_state = get_pin_state(7);

    // reset
    if (get_pin_state(6) == 0){  
        enable = 0;   
        time_seconds = 0;
        battery_capacity = 0;

        emptybat_flag = 0;

        set_pin_state(11, LOW);
        update_flag = 1;
    }

}


void blink_led(void){
    toggle_pin_state(13);   // heartbeat 



}





// array of strucs pointers type rtos_task_t
// 0 -> watchdog
rtos_task_t *rtos_task_list[] = {&task_time, &task_battery, &task_OLED, &task_buttons, &task_blink, 0};
