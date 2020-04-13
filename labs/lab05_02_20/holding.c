#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include "common.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1
#define ZERO 0


/**
 * To set up the PWM -- Set up  Timer 1. TO control that pin.
 *
 *
 */
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
// this is the flag and brightness
uint8_t on = 0;
uint16_t brightness;


volatile uint8_t button_a_pressed_counter = 0;
volatile uint8_t button_c_pressed_counter = 0;
volatile uint32_t ms_ticks = 0;

/***************************************************************************
   ALL INITIALIZATION
****************************************************************************/


ISR(TIMER3_COMPA_vect){
        led_toggle(&_yellowbb);
}

ISR(TIMER0_COMPA_vect){
        ++ms_ticks;
}


void initialize_system(void) {
    // initalize green and yellow only.
    // initialization defines the IO_structs and sets DDR
    initialize_led(GREEN);
    initialize_led(YELLOW);
    initialize_led(RED);

    initialize_led(GREENBREADBOARD);
    initialize_led(YELLOWBREADBOARD);
    initialize_led(REDBREADBOARD);
    initialize_led(REDLABFIVEBREADBOARD);
    // The "sanity check".
    // When you see this pattern of lights you know the board has reset
    light_show();
    external_light_show();



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




void timer_one_setup() {
    //WGE to set fast PWM
    //The pin is connected to the
    TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 0xFFFF;
}

void init(){
    SetupHardware(); //This is setting up the USB Hardware and stdio
    sei(); //enabling interrupts -- needed for the USB.
}
int main() {
    // This prevents the need to reset after flashing
    USBCON = 0;
    char c;
    init();


    initialize_system();
    //calling the setup function
    timer_one_setup();


    //setting up the match number
    OCR1B = 0;
    while (1) {
        USB_Mainloop_Handler();
        if ((c=fgetc(stdin)) != EOF ){
            printf("Hello! Hit any key, including %c to see this again!\r\n", c);
        }
//        //changing the fq. if button c was pressed.
//        if (button_released((&_buttonC))) {
//            if (!on) {
//                //go to 100 then reset flag
//                OCR1B += OCR1A / 4;
//                if (OCR1B == OCR1A) {
//                    on = 1;
//                }
//            } else {
//                //going back to 0 from 100 then reseting flag
//                OCR1B -= OCR1A / 4;
//                if (OCR1B == 0) {
//                    on = 0;
//                }
//            }
//        }

//        if (button_released((&_buttonA))) {
//            if (!on) {
//                //making it 100
//                for (brightness = 0; brightness < OCR1A; brightness++) {
//                    OCR1B = brightness;
//                    _delay_ms(10);
//                }
//                on = 1;
//            } else {
//                //making it drop down to 0
//                for (brightness = OCR1A; brightness > 0; brightness--) {
//                    OCR1B = brightness;
//                    _delay_ms(10);
//                }
//                on = 0;
//
//            }


    } // end while

} //end main






