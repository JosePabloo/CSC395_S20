#include "common.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "leds.h"

volatile uint64_t ms_ticks = 0;
volatile int16_t global_last_m2a_val = 0;
volatile int16_t global_last_m2b_val = 0;
uint64_t time_task = 0;
uint64_t time_loop = 0;


// constants for task
#define TASK_RED 0
#define TASK_YELLOW 1
#define TASK_BUTTON_A 2
#define TASK_BUTTON_C 3
#define TASK_PRINT 4

//task structure
typedef struct {
    uint8_t ready;
    uint32_t nextRelease;
    uint32_t period;
    void (*tick_fn) (void);
} task;

// declaring the task
extern task _task_red;
extern task _task_yellow;
extern task _task_button_a;
extern task _task_button_c;
extern task _print;
// define tasks
task _task_red = {0, 0, 0, empty_function};
task _task_yellow = {0, 0, 0, empty_function};
task _task_button_a = {0, 0, 0, empty_function};
task _task_button_c = {0, 0, 0, empty_function};
task _print = {0, 0, 0, empty_function};
//flags
volatile uint8_t on;
volatile uint16_t pressed_times;
volatile uint8_t implementedA;
volatile uint8_t implementedC;

// deginding those task:







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

}

void initialize_task(int task_type){
    switch(task_type){
        case(TASK_RED):
            // update my task period
            _task_red.period = 500;
            // update my task function
            _taskRed.tick_fn = task_red;
            break;
        case(TASK_YELLOW):
            _task_yellow.period = 250;
            _taskYellow.tick_fn = task_yellow;
            break;
        case(TASK_BUTTON_A):
            _task_button_a.period = 1000;
            _task_button_a.tick_fn = _task_button_a;
            break;
        case(TASK_BUTTON_C):
            _task_button_c.period = 2000;
            _task_button_c.tick_fn = _task_button_c;
            break;
        case(TASK_PRINT):
            _print.period = 3000;
            _print.tick_fn = counter_print;
            break;
        default:
            break;
    }
}
//The task of the LED Lights
void task_red() {
    led_toggle(&_redbb);
}
void task_yellow() {
    led_toggle(&_yellowbb);
}

//Task of buttons:
void task_button_a() {
    // if button A pressed
    if(is_button_pressed(&_buttonA)) {
        // if the task hasn't been implemented
        if(!implementedA) {
            // count
            pressed_times++;
            // refresh the flag
            implementedA = 1;
        }
    }
}

void task_button_c() {
    if (is_button_pressed(&_buttonC)) {
        //Check to see  if the task hasn't been implemented
        //Then check to see if its not on. If then go from 0 to 100
        //rest thr flag
        //Else the red led is turned on
        if(!implementedC) {
            if (!on) {
                OCR1B += OCR1A / 4;
                if (OCR1B == OCR1A) {
                    on = 1;
                }
            }
            else {
                OCR1B -= OCR1A / 4;
                if (OCR1B == 0) {
                    on = 0;
                }
            }
            implementedC = 1;
        }
    }
}

void counter_print() {
    printf("Button A pressed %d times.\n", pressed_times);

//Setting up timmer 0
//Prescaller 64
//Match number 249

void setup_timmer_0(){
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCNT0 = 0;
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
}
//ISR is every 1ms,
ISR(TIMER0_COMPA_vect) {
        ms_ticks++;
}

void task_scheduler() {
    if (ms_ticks >= _taskRed.nextRelease) {
        _task_red.ready = 1;
        _task_red.nextRelease = ms_ticks + _task_red.period;
    }
    // if time for yellow led task, make it ready
    if (ms_ticks >= _taskYellow.nextRelease) {
        _taskYellow.ready = 1;
        _taskYellow.nextRelease = ms_ticks + _taskYellow.period;
    }
    // if time for button A task, make it ready
    if (ms_ticks >= _taskButtonA.nextRelease) {
        _taskButtonA.ready = 1;
        _taskButtonA.nextRelease = ms_ticks + _taskButtonA.period;
    }
    // if time for button C task, make it ready
    if (ms_ticks >= _taskButtonC.nextRelease) {
        _taskButtonC.ready = 1;
        _taskButtonC.nextRelease = ms_ticks + _taskButtonC.period;
    }
    // if time to print, make it ready
    if (ms_ticks >= _print.nextRelease) {
        _print.ready = 1;
        _print.nextRelease = ms_ticks + _print.period;
    }
}

void task_potentiometer(uint8_t channel){
    ADMUX &= 0b11100000;
    if (channel >= 8) {
        ADCSRB |= (1<<MUX5);
        ADMUX |= (channel - 8);
    }
    else {
        ADCSRB &= ~(1<<MUX5);
        ADMUX |= channel;
    }
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
}



/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
    // This prevents the need to reset after flashing
    USBCON = 0;

    initialize_system();
    setup_timmer_0();
    sei()
    uint32_t iterations = 100000;

    cli();
    uint64_t start = ms_ticks;
    sei();

    //Potentiometer Task
    cli();
    time_task = ms_ticks - start - time_loop;
    char encoder_string[50];
    sprintf(encoder_string, "%d", (int)encoder_string);
    strcat(encoder_string, " <- loop time\n");
    sendString(encoder_string);
    led_on(&_red, 0);
    start = ms_ticks;
    time_task = 0
    sei();
    for(i=0; i<iterations; i++){
        task_potentiometer();
    }
    //Scheduler Task
    cli();
    time_loop = ms_ticks - start;
    char loopstring[50];
    sprintf(loopstring, "%d", (int)time_loop);
    strcat(loopstring, " <- loop time\n");
    sendString(loopstring);
    start = ms_ticks
    sei();
    led_on(&_yellow, 0);
    for(i=0; i<iterations; i++){
        task_scheduler();
    }

    //Endcoder Task
    //TODO: Code task_encoder();
 




} /* end main() */
