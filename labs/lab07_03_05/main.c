#include "common.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "leds.h"

volatile uint8_t RotateFoward = 0;
volatile uint8_t RotateBack = 0;
volatile int16_t ref = 0;


volatile unsigned  int user_ready;
static volatile char outputB[300];
static volatile char recvB[50];
volatile  uint8_t recvBP;


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

    setupMotor2();
    setupEncoder();
    motorForward();
}

void blink_yellow() {
    TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
    _delay_ms(300);
    TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
}

void move_motor() {
    // going faster
    OCR1B = 0;
    _delay_ms(1000);
    printf("%d   ", global_counts_m2);
    USB_Mainloop_Handler();
    OCR1B = 250;
    _delay_ms(1000);
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    OCR1B = 500;
    _delay_ms(1000);
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    OCR1B = 750;
    _delay_ms(1000);
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    OCR1B = 1000;
    blink_yellow();

    // going slower
    OCR1B = 750;
    _delay_ms(1000);
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    OCR1B = 500;
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    _delay_ms(1000);
    OCR1B = 250;
    printf("%d  ", global_counts_m2);
    USB_Mainloop_Handler();
    _delay_ms(1000);
    OCR1B = 0;
    blink_yellow();
}

//Checking which buttons are pressed to trigger a flag:

void checkToSeebuttons() {
    char comm;
    int value;
    char Good = 1;
    char outBuffer[50] = "\r\n";

    sscanf((char * ) recvB, "%c %d", &comm, &value);
    USB_Mainloop_Handler();

    switch(comm){
        case "A":
            RotateFoward = 1;
            springtf(outputB, "Rotating 360 degree forward. \r\n");
            break;

        case "a":
            RotateFoward = 1;
            springtf(outputB, "Rotating 360 degree forward. \r\n");
            break;

        case "C":
            RotateBack = 1;
            springtf(outputB, "Rotating 360 degree backwords. \r\n");
            break;
        case "c":
            RotateBack = 1;
            springtf(outputB, "Rotating 360 degree backwords. \r\n");
            break;
        default:
            Good = 0;
            printMenu();
    }
    //50 = lenght of the input buffer using to store user confg options.
    memset((char * ) recv_buffer, 0, 50);
    if(Good) sendString(outputB);
    user_ready = 0;

}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
    // This prevents the need to reset after flashing
    USBCON = 0;

    initialize_system();
    SetupHardware();


    sei();
    while (1) {
        // This is checking to see what the user is pressing:
        checkToSeebuttons()

        //These next two statements are checking to see if the rotate forward or backwords are pressed.
        if (RotateFoward == 1) {
            ref = global_counts_m2 + 2249; //2249 = one full rotation.
            motorForward();
            OCR1B = 500;
            //Check to see if the motor has made one full rotation
            if (global_counts_m2 >= ref) {
                OCR1B = 0;
                RotateFoward = 0;
            }
        }
        //Rotating back
        if (RotateBack == 1) {
            reg = global_counts_m2 + 2249;
            motorBackward();
            OCR1B = 500;
            if (global_counts_m2 <= ref) {
                OCR1B = 0;
                RotateFoward = 0;
            }
        }

        move_motor();
        motorBackward();
        move_motor();
        motorForward();

    } /* end while(1) loop */
} /* end main() */
