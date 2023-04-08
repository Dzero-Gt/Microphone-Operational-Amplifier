
#include "xc.h"


void init_button();

volatile unsigned int overflow = 0;



/*
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
    
    T2CON = 0x0010;
    T2CONbits.TCKPS=0b11; // prescaler of 1:256 //
    PR2=62499; // 1 second period
    TMR2=0;
    _T2IE=1; // enable interupt 
    _T2IF=0; // set the interrupt flag to 0
    T2CONbits.TON=1;
    
    IC1CON = 0; 
    IC1CONbits.ICTMR = 1;  // ic1 uses tmr2
    IEC0bits.IC1IE = 1;    // enable interrupt on ic1, turn off if using polling, keep on if using interupt
    IC1CONbits.ICI = 0b00; // interrupt on every capture
    IC1CONbits.ICM = 0b010;// falling edge mode
    _IC1IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0; // turn the interrupt flag back to 0
    overflow++; // and increase the overflow by one
}

void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void){

    static unsigned int last = 0;
    unsigned int current;
    _IC1IF = 0;
    current = IC1BUF;
}