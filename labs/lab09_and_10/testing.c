#include "common.h"
#include "motor.h"
#include "leds.h"
#include "timer.h"

// GLOBAL VARIABLES

void set_ref_delta( XXX );
void pd_control();

main() {
    // initialize motors
    // initialize communication
    // initialize 1 ms timer
    // initialize leds (optional)
    // initialize buttons (optional)
    while(1) {

        // LUFA
        USB_Mainloop_Handler(); //Handles USB communication
        if ((key=fgetc(stdin)) != EOF ) {
            //...
        }

        // SERIAL
        if (user_command_ready) {
            sscanf((char *)recv_buffer,"%c %d",&key, &value);
        }

        // if key 'f' go forward 1 rotation
        set_ref_delta(2249);
        // if key 'b' go backward 1 rotation
        set_ref_delta(-2249);

        pd_control();
        _delay_ms(pd_period);
    } // end while()
} // end main()
void pd_control() {
    // T = kP * P + kD * D;
    OCR1B = XX;
}

void set_ref_delta(){

}