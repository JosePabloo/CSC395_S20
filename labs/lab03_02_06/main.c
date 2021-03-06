#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1
#define ZERO 0


enum buttonA_states {
    A_Starting, A_FirstPress, A_SecondPress, A_ThirdPress
} ButtonA_State;
enum buttonC_states {
    C_Starting, C_FirstPress, C_SecondPress, C_ThirdPress
} ButtonC_State;

//Adding the timers for the different states.
uint8_t yellowTimer = 0;
uint8_t greenTimer = 0;
uint32_t mainTimer = 0;
volatile uint8_t button_a_pressed_counter = 0;
volatile uint8_t button_c_pressed_counter = 0;

/***************************************************************************
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
    initialize_button(&_buttonA);
    initialize_button(&_buttonC);

    enable_pcint(&_interruptA);
    enable_pcint(&_interruptC);


    // Setting up the button action. when its pressed or released.
    // When it's released, called the release funciton.
    // when released call release function

//    setup_button_action(&_interruptA, 0);
//    setup_button_action(&_interruptC, 0);

    setup_button_action(&_interruptA, 1, button_a_pressed);
    setup_button_action(&_interruptC, 1, button_c_pressed);
}

//This is going to hold the state of the Yellow LED.=
void Yellow_LED_Cases() {
    switch (ButtonA_State) {   // Transitions
        // Initial transition
        //Stating off with the LED off.
        //If the Button A is pressed, Move the Flag counter to the first relase.
        case A_Starting:
            led_off(&_yellow, 0);
            if (1 == button_a_pressed_counter) {
                ButtonA_State = A_FirstPress;
            }
            break;
            //First press is the second  case is turning on the LED.
        case A_FirstPress:
            led_on(&_yellow, INVERTED);
            if (2 == button_a_pressed_counter) {
                ButtonA_State = A_SecondPress;
            }
            break;
            // Second Press is flashing at .4 hz
        case A_SecondPress:
            if (yellowTimer % 1250 == 0) {
                led_toggle(&_yellow);
            }
            if (3 == button_a_pressed_counter) {
                ButtonA_State = A_Starting;
            }
            break;
        default:
            ButtonA_State = A_Starting;
            break;
    } // Transitions


} // State actions



void Green_LED_Cases() {
    switch (ButtonC_State) {   // Transitions
        // Initial transition
        //Stating off with the LED off.
        //If the Button A is pressed, Move the Flag counter to the first relase.
        case C_Starting:
            led_off(&_green,1);
            if (1 == button_c_pressed_counter) {
                ButtonC_State = C_FirstPress;
            }
            break;
            //First press is the second  case is turning on the LED.
        case C_FirstPress:
            led_on(&_green, 0);
            if (2 == button_c_pressed_counter) {
                ButtonC_State = C_SecondPress;
            }
            break;
            // Second Press is flashing at .4 hz
        case C_SecondPress:
            if (greenTimer % 250 == 0) {
                led_toggle(&_green);
            }
            if (3 == button_c_pressed_counter) {
                ButtonC_State = C_Starting;
            }
            break;
        default:
            ButtonC_State = C_Starting;
            break;
    } // Transitions


} // State actions



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

    // Start with the system with both LED's off.
    // BUTTON A: modifies only rhe Yellow LED state.
    // BUTTON C: modifies only the Green LED state.

    // 1st release: LED ON (solidly, no blinking).
    // 2nd release: LED Blink at frequency provided below
    // 3rd release: LED OFF

    // When an LED is Blinking:
    // YELLOW LED - YELLOW LED blink at .4 Hz (ON at 1250 ms, OFF at 2500 ms)
    // GREEN LED - blink at 2 Hz (ON at 250 ms, OFF at 500 ms, ON at 750 ms, OFF at 1 sec)

    // ONLY USE THE PCINT ISR
    //Assiming that both buttons are in a not pressed state.

    //Set up the counter and the states for both Buttons
    //Need to initialize the counter flags for both buttons.
    //Ned to initalize the states for both LED's
    button_a_pressed_counter = 0;
    ButtonA_State = A_Starting;

    button_c_pressed_counter = 0;
    ButtonC_State = C_Starting;


    sei();  //calling this from main.

    while (1) {
        Yellow_LED_Cases();
        Green_LED_Cases();
        _delay_ms(250);
        mainTimer += 250;
    } // end while(1)

} /* end main() */
