#ifndef COMMON_H_
#define COMMON_H_


#ifndef F_CPU
#define F_CPU 16000000UL
#endif //for the F_CPU

#include <avr/io.h>
#include <inttypes.h>

//Adding definitions for each pin.
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7


#define SET_BIT(reg, pin) (reg |= (1<<pin))
#define CLEAR_BIT(reg, pin) (reg &= ~(1<<pin))
#define TOGGLE_BIT(reg, pin) (reg ^= (1<<pin))


//Macros that make is easy to set, clear, and toggle a single bit within a register.
//A way to set this could be -> SET_BIT(DDRB,2) will set the bit 2 in DDRB register to 1.
#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))



//Definding a user-defined type called IO_struct which
//contains 5 variables.
// & is used to the the address of a variable.
// * is a type of specifier in a variable declaration.
typedef struct {
    volatile uint8_t * ddr;
    volatile uint8_t * port;
    uint8_t pin;
    volatile uint8_t * portin;
} IO_struct;






#endif //for the Common_H
