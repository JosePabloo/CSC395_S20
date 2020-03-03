#include "task.h"
#include "buttons.h"
#include "leds.h"
#include <stdio.h>

//definding task

//LEDs
task _redTask = {0,0,0, empty_function};
task _yellowTask = {0,0,0, empty_function};
//buttons
task _buttonATask = {0,0,0, empty_function};
task _buttonCTask = {0,0,0, empty_function};
//print
task _print = {0,0,0, empty_function};

void intialize_task(int taskType){
    switch(taskType){
        case(REDTASK):
            _redTask.period = 500;
            _redTask.tick_fn = red_task;
            break;
        case(YELLOWTASK):
            _yellowTask.period = 500;
            _yellowTask.tick_fn = yellow_task;
            break;
        case(BUTTONATASK):
            _buttonATask.period = 500;
            _buttonATask.tick_fn = button_A_Task;
            break;
        case(BUTTONCTASK):
            _buttonCTask.period = 500;
            _buttonCTask.tick_fn = button_C_Task;
            break;
        case(PRINT):
            _print.period = 500;
            _print.tick_fn = print_counter;
            break;
        default:
            break;
    }
}
//toggle the LED every 500ms
void red_task(){
    led_toggle(&_redbb)
}
//toggle the LED every 500ms
void yellow_task(){
    led_toggle(&_yellowbb)
}
//task of button A
void button_A_Task(){
    if(button_pressed(&_buttonA)){
        if(!aImplemented){
            times_pressed++;
            aImplemented=1;
        }
    }
}