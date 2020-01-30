#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void) {
	// initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
  initialize_button(BUTTONA);
	initialize_button(BUTTONC);
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();

  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO MEET FUNCTIONAL REQUIREMENTS:

	// Always toggle yellow LED every 1000ms
	// Toggle greed LED every 500ms, but only if is in an on state
	// Press and release button A switches green to on state.
	// Press and release button C switches green to off state.

	// Assume both buttons start in a not pressed state.

	uint8_t green_on = 1;
    uint8_t green_off = 1;

  while(1) {


      _delay_ms(1000);
      green_on = is_button_pressed(&_buttonA);
      green_off = is_button_pressed(&_buttonC);
      led_on(&_yellow,INVERTED);
      green_on = is_button_pressed(&_buttonA);
      green_off = is_button_pressed(&_buttonC);
      _delay_ms(1000);
      green_on = is_button_pressed(&_buttonA);
      green_off = is_button_pressed(&_buttonC);
      led_off(&_yellow,0);
      green_on = is_button_pressed(&_buttonA);
      green_off = is_button_pressed(&_buttonC);

      if (green_on) {
          while(green_on){
              led_on(&_green,0);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_off(&_green,1);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_on(&_green,0);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_off(&_green,1);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_on(&_yellow,INVERTED);
              green_off = is_button_pressed(&_buttonC);
              led_on(&_green,0);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_off(&_green,1);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_on(&_green,0);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_off(&_green,1);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
              green_off = is_button_pressed(&_buttonC);
              led_on(&_green,0);
              green_off = is_button_pressed(&_buttonC);
              led_off(&_yellow,0);
              green_off = is_button_pressed(&_buttonC);
              _delay_ms(250);
          }
          if (green_off){
              green_on = 1;
              green_off = 1;
          }

      }






  } // end while(1)

} /* end main() */
