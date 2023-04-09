
#include "xc.h"
#include <stdbool.h>

void init_button();
volatile bool State = 0; // state:0  use raw input 
                         // state:1  use filtered data



/* FUNCTION: init_button()
 * ====================
 * initialize pin connected to the button as an input and adds a pull up resistor 
 * to provide a default read value. then map that pin to the input capture peripheral 
 * to trigger a section of code immediately when a button is pressed 
 * 
 * also initialize timer2 for input capture
 * all values for timer2 settings were gotten from the prelab video 
 */

void init_button(){
    
    TRISBbits.TRISB8 = 1; // pin input for our button;
    
    TRISBbits.TRISB8 =1; // rb8 is input pin for button
    CNPU2bits.CN22PUE=1; //Pull up resistor on RB8 (button)
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // (unlock)
    RPINR7bits.IC1R = 8;  // Use Pin RP8/RB for input Compare 1 
    __builtin_write_OSCCONL(OSCCON | 0x40); // (lock)
    
    
    IC1CON = 0; 
    IC1CONbits.ICTMR = 1;  // ic1 uses tmr2
    IEC0bits.IC1IE = 1;    // enable interrupt on ic1, turn off if using polling, keep on if using interupt
    IC1CONbits.ICI = 0b00; // interrupt on every capture
    IC1CONbits.ICM = 0b010;// falling edge mode
    _IC1IF = 0;
}


void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void){

    
    _IC1IF = 0; // reset the IC interupt flag
    State != State; // we intent to use the button as a toggle between the raw and filtered input
}
