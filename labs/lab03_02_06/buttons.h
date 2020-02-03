#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

// These must be defined using initialize_button()
IO_struct _buttonA;
IO_struct _buttonC;


void initialize_button(int button);

int is_button_pressed(IO_struct * button);

#endif