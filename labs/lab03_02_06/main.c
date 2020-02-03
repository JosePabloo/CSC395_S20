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

    while (1) {

        /*
         * this first while loop is toggling the yellow LED every 1000ms looking for a state change
         * in between every delay.
         * If button A is pressed, the State is changed for green_on.
         * Then it goes into the second while look.
         */


        _delay_ms(1000);
        green_on = is_button_pressed(&_buttonA);

        led_on(&_yellow, INVERTED);
        green_on = is_button_pressed(&_buttonA);

        _delay_ms(1000);
        green_on = is_button_pressed(&_buttonA);

        led_off(&_yellow, 0);
        green_on = is_button_pressed(&_buttonA);

        while (green_on) {
/*
 * This is the second while loop. One is in here and is toggling the green LED on, then checking for the state.
 * After it checks for the state, its delaying 500ms. After 1000ms, it toggles the yellow LED.
 * If the state is changed, then it gets kicked out of this while loop back to the first one.
 */
            led_toggle(&_green);
            green_on = !is_button_pressed(&_buttonC);
            _delay_ms(500);

            led_toggle(&_green);
            green_on = !is_button_pressed(&_buttonC);
            _delay_ms(500);

            led_toggle(&_green);
            led_on(&_yellow, INVERTED);
            green_on = !is_button_pressed(&_buttonC);
            _delay_ms(500);

            led_toggle(&_green);
            green_on = !is_button_pressed(&_buttonC);
            _delay_ms(500);

            led_off(&_yellow, 0);

        }//End of while(green_on)






    } // end while(1)

} /* end main() */
