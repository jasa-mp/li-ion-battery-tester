#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/delay.h>
#include <stdbool.h>

#include <pin_io.h>
#include <adc.h>
#include <oled_lib.h>

#include <rtos.h>
#include <rtos_tasks.h>

#define testpin10 2
#define testpinADC A0

int main(void) {
  // setup

  // funcion initialization
  adc_init();
  i2c_enable();
  OLED_init();

  // start screen setup
  OLED_set_cursor(0,1);
  OLED_send_string("Testing time: ");

  OLED_set_cursor(0,3);
  OLED_send_string("Battery status: ");
  OLED_set_cursor(0,4);
  OLED_send_string("Voltage:        V");
  OLED_set_cursor(0,5);
  OLED_send_string("Current:        mA");
  OLED_set_cursor(0,6);
  OLED_send_string("Capacity:       mAh");

  // pin setup
  set_pin_mode(6, INPUT_PULLUP);      // Reset
  set_pin_mode(7, INPUT_PULLUP);      // Start/Stop

  set_pin_mode(11, OUTPUT);
  set_pin_mode(13, OUTPUT);
  
  set_pin_state(11, LOW);

  // RTOS setup
  rtos_init();
  rtos_enable();

  // loop
  while(1){


  
  }
  // safety 
  while(1){}
  
}
