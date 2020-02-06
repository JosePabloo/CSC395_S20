#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1

enum buttonA_states { A_Starting, A_FirstPress, A_SecondPress, A_ThirdPress } ButtonA_State;

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

    enable_pcint(&_interruptA);
    enable_pcint(&_interruptC);


    // Setting up the button action. when its pressed or released.
    // When it's released, called the release funciton.
    // when released call release function

//    setup_button_action(&_interruptA, PRESS);
//    setup_button_action(&_interruptC, PRESS);

    setup_button_action(&_interruptA, 1, buttonAPressed);
    setup_button_action(&_interruptC,1,toggleGreen);
}

void buttonAPressed()
{
    switch(ButtonA_State) {   // Transitions
        case LA_SMStart:  // Initial transition
            LA_State = LA_s0;
            break;

        case LA_s0:
            if (!A0) {
                LA_State = LA_s0;
            }
            else if (A0) {
                LA_State = LA_s1;
            }
            break;

        case LA_s1:
            if (!A0) {
                LA_State = LA_s0;
            }
            else if (A0) {
                LA_State = LA_s1;
            }
            break;

        default:
            Button_A_State = LA_SMStart;
            break;
    } // Transitions

    switch(ButtonA_State) {   // State actions
        case LA_s0:
            break;

        case LA_s1:
            B0 = A1;
            break;

        default:
            break;
    } // State actions
}





/****************************************************************************
   MAIN
****************************************************************************/


int main(void) {
    // This prevents the need to reset after flashing
    USBCON = 0;
    LA_State = LA_SMStart; // Indicates initial call

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

    sei();  //calling this from main.



    while (1) {
        TickFct_Latch();




    } // end while(1)

} /* end main() */
