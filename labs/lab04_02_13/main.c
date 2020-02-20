#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1
#define ZERO 0


volatile uint32_t ms_ticks = 0;

/***************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void set_up_timmer() {
    /**
     * Set up wavefrom to CTC. (Clear Timer on Compare Match)
     * CTC = 0100
     * 01 = TCCR3B
     * 00 = TCCR3A
     */
    TCCR3B |= (1 << WGM32);
    TCCR0A |= (1 << WGM01);
    /**
     * Setting up the Clock_select_bits to set up the prescaler.
     * Set CS32 bits for 256 prescaler:
     */
    TCCR3B |= (1 << CS32);
    TCCR3B |= (1 << CS01);
    //set match to achive 250 ms periods aka 4 hz.
    OCR3A = 15625;
    OCR0A = 2000;

    //enable the timer interrupt.
    TIMSK3 |= (1 << OCIE3A);
    TIMSK0 |= (1 << OCR0A);
}

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

    // The "sanity check".
    // When you see this pattern of lights you know the board has reset
    light_show();
    external_light_show();

    set_up_timmer();




}






/****************************************************************************
   MAIN
****************************************************************************/


int main(void) {
    // This prevents the need to reset after flashing
    USBCON = 0;


    initialize_system();

    sei();  //calling this from main.

    while (1) {

        if (ms_ticks % 500 == 0) {
            led_toggle(&_redbb);
            led_toggle(&_red);
        }
    } // end while(1)

} /* end main() */
