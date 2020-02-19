#ifndef LEDS_H_
#define  LEDS_H_
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>


#define GREEN 5
#define YELLOW 7
#define RED 0

#define GREENBREADBOARD 0
#define YELLOWBREADBOARD 0
#define REDBREADBOARD 0



IO_struct _yellow;
IO_struct _red;
IO_struct _green;
//Setting up the LED's From the breadboard
IO_struct _yellowbb;
IO_struct _redbb;
IO_struct _greenbb;

/*
 * initializing the data struct for the specific on-board led
 */
void initialize_led(int color);

/*
 * Flashing the designated on-board led for 250ms on and off.
 * assumes that the led is initialized.
 */

void flash_led(IO_struct * color, int inverted);

/*
 * flash all the leds that have been initialized for a sanity check light show
 */

void light_show();

/*
 * turn on specified on board LED. indicate if inverted (0 turns led on)
 */
void led_on(IO_struct * color, int inverted);

/*
 * turn off specified on board LED. indicate if inverted (1 turns led off)
 */
void led_off(IO_struct * color, int inverted);

/*
 * toggle the led between on and off
 */
void led_toggle(IO_struct * color);








#endif