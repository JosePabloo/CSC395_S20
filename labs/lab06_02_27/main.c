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
// this is the flag and brightness
uint8_t on = 0;
uint16_t brightness;


volatile uint8_t button_a_pressed_counter = 0;
volatile uint8_t button_c_pressed_counter = 0;
volatile uint32_t ms_ticks = 0;

/***************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void) {

    initialize_led(GREEN);
    initialize_led(YELLOW);
    initialize_led(RED);
    initialize_led(GREENBREADBOARD);
    initialize_led(YELLOWBREADBOARD);
    initialize_led(REDBREADBOARD);
    initialize_led(REDLABFIVEBREADBOARD);
    //Initializing Task
    initialize_task(REDTASK);
    initialize_task(YELLOWTASK);
    initialize_task(BUTTONATASK);
    initialize_task(BUTTONCTASK);
    initialize_task(PRINTTASK);

    // The "sanity check".
    // When you see this pattern of lights you know the board has reset
    light_show();
    external_light_show();

    set_up_timmer();
    set_up_timmer_zero();

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


void set_up_timmer_zero() {
    cli();
    /**
     * Set up wavefrom to CTC. (Clear Timer on Compare Match)
     * CTC = 010
     * 01 = TCCR3B
     */

    TCCR0A |= (1 << WGM01);
    /**
     *  Setting up the Clock_select_bits to set up the prescaler.
     *  Set CS01 bits for 64 prescaler:
     */
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCNT0 = 0; //Timmer initialize
    OCR0A = 249;
    //enable the timer interrupt.
    TIMSK0 |= (1 << OCIE0A);

}

void set_up_timmer_one() {

    TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1<<CS10);
    OCR1A = 0xFFFF;

}

ISR(TIMER0_COMPA_vect){
        ++ms_ticks;
        //if the task is ready for the red:
        if(ms_ticks >= _redTask.nextRelease){
            _redTask.ready = 1;
            _redTask.nextRelease = ms_ticks + _redTask.period;
        }
        //yellow task
        if(ms_ticks >= _yellowTask.nextRelease){
            _yellowTask.ready = 1;
            _yellowTask.nextRelease = ms_ticks + _yellowTask.period;
        }
        //button A task
        if(ms_ticks >= _buttonATask.nextRelease){
            _buttonATask.ready = 1;
            _buttonATask.nextRelease = ms_ticks + _buttonATask.period;
        }
        //button C task
        if(ms_ticks >= _buttonCTask.nextRelease){
            _buttonCTask.ready = 1;
            _buttonCTask.nextRelease = ms_ticks + _buttonCTask.period;
        }
        //print time
        if(ms_ticks >= _print.nextRelease){
            _print.ready = 1;
            _print.nextRelease = ms_ticks + _print.period;
        }
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
            led_off(&_green, 1);
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

void timer1_setup() {
    //WGE to set fast PWM
    TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 0xFFFF;
}

int main(void) {
    // This prevents the need to reset after flashing
    USBCON = 0;


    initialize_system();
    //calling the setup function
    set_up_timmer_zero();
    set_up_timmer_one();
    //*************************************************************//
    //*******         THE CYCLIC CONTROL LOOP            **********//
    //*************************************************************//


//    button_a_pressed_counter = 0;
//    ButtonA_State = A_Starting;
//
//    button_c_pressed_counter = 0;
//    ButtonC_State = C_Starting;


    //setting up the match number
    OCR1B = 0;
    on = 0;
    number_of_times_pressed = 0;
    aImplemented = 0;
    cImplemented = 0;
    sei();  //calling this from main.


    while (1) {
        //changing the fq. if button c was pressed.

    } // end while

} //end main



