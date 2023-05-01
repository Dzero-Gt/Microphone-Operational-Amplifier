/*
 * File:   interrupt_based_button.c
 * Author: Adan,Cooper,Dillon
 *
 * Created on March 15, 2023, 8:26 PM
 */

#include "xc.h"
#include <stdlib.h>
#include "button.h"
#include "dac.h"
#include "ADC_config.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))



//IC button  toggle
volatile int State = 0; // state:0  use raw input 
                        // state:1  use filtered data

volatile unsigned int overflow=0; // overflow occurs after one second
volatile unsigned long int time_current_click=0;
volatile unsigned long int time_preivious_click=0;

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0; // turn the interrupt flag back to 0
    overflow++; // and increase the overflow by one
}

void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void){

    
    _IC1IF = 0; // reset the IC interrupt flag
    time_preivious_click=time_current_click;
    time_current_click = (unsigned long int)((unsigned long int)IC1BUF + (unsigned long int)overflow*(PR2+1));
    if((time_current_click - time_preivious_click) >125){ //to achieve a 2mS debounce delay must find (2/1000)(PR2+1)=125
        State = 1-State; // we intend to use the button as a toggle between the raw and filtered input
    }
    if (State){
        AD1CHS = 0x0001; //switch to filtered input
        
            
    }
    if (!State){
          AD1CHS = 0x0000; //switch to unfiltered input
    }
        
    
}

//SPI DAC interrupt
void __attribute__((__interrupt__,__auto_psv__)) _SPI2Interrupt(void)
{
    _RB7 = 1;
    int temp;
    _SPI2IF = 0;
    temp = SPI2BUF; // just to clear the buffer and avoid setting off the 
                    // overflow bit
    LATBbits.LATB6 = 0;  // SPI transaction complete, give a low pulse on LDAC'
        // to load the value from the input latch register to the DAC register.
    // The DAC datasheet says that the pulse has to remain low for at least
    // 100ns (p6 of the manual, the second table)
    asm("nop");
    asm("nop");
    LATBbits.LATB6 = 1;
}

//ADC Interrupt
void __attribute__ ((__interrupt__, __auto_psv__)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    int data = ADC1BUF0;
    write_DAC(data, 10, 1);
    //write_DAC(0b1111111111, 2, 1);
    // include conversion data here
}

//Pic24 setup
void setup(){
    AD1PCFG = 0xffff;     //sets all pins to digital I/O
    CLKDIVbits.RCDIV=0;   // set frequency to 16 MHz 
}

// delays in one ms increments
void delay(unsigned int ms) {
    int i;
    for (i = 0; i < ms; i++) {
        asm("repeat #15993");
        asm("nop");
    }
    return;
}

int main(void) {
    
    setup();
    init_button();
    init_DAC();
    ADC_init();

    //sets start led state
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9=0;
    
    while (1){
        /*
         * Create a new setup function for the LED
         * hook the LED up to a LATB pin and the positive terminal of the breadboard
         * If state is 1 turn light on, LATB register=0
         * If state is 0 turn external light off. LATB register=1
         */
        if(State){
            LATBbits.LATB9=0;
        }
        if(!State){
            LATBbits.LATB9=1;
        }

    }
    
    return -1;
}



