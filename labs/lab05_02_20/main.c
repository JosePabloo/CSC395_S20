#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

// this is the flag and brightness controller.
uint8_t on = 0;
uint32_t brightness;

//Need to set up the timmer:

void timer_one_setup() {
    //The pin is connected to the
    TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 0xFFFF;
    OCR1B = 0
}

void init() {
    SetupHardware(); //This setups the USB hardware and stdio
    sei(); //Enables interrupts - needed for USB
}

int main() {
    char c;
    init();
    timer_one_setup()
    while (1) {
        USB_Mainloop_Handler(); //Handles USB communication
        if ((c = fgetc(stdin)) == "F") {
            brightness++
            if (brightness < OCR1A) {
                OCR1B = brightness;
            } else {
                printf("Hello, the brightness is already at the highest");
            }

        }
        if ((c = fgetc(stdin)) == "f") {
            //checking to see if the flag has been triggered.
            //making the brighness go up:
            brightness--
            if (brightness > 0) {
                OCR1B = brightness;
            } else {
                printf("Hello, the brightness is already at the lowest");
            }
        }
    }
}
