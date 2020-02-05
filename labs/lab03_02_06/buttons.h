#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

#define PCINT_BUTTONA PCINT3
#define PCINT_BUTTONC PCINT0

typedef struct {
    uint8_t pcint;
    uint8_t mask;
    uint8_t enabled;
    unit8_t prev_state;
    void (*release_fn) (void);
    void (*press_fn) (void);
}; INTERRUPT_struct;
// These must be defined using initialize_button()
extern IO_struct _buttonA;
extern IO_struct _buttonC;

extern  INTERRUPT_struct _interruptA;
extern  INTERRUPT_struct _interruptC;

void enable_pcint(INTERRUPT_struct *state);


void setup_button_action(INTERRUPT_struct *state, int release, void(*callback)(void));

void initialize_button(IO_struct * button);

#endif