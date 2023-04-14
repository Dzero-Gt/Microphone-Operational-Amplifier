/*
 * File:   interrupt_based_button.c
 * Author: Adan,Cooper,Dillon
 *
 * Created on March 15, 2023, 8:26 PM
 */

/*
 * currently this is a basic program that can be adapted later
 * initializes button to pin rb8
 * it uses IC1 interrupt to recognize button input
 */


#include "xc.h"
#include <stdlib.h>
#include "button.h"
#include "ADC_config.h"
#include "dac.h"
//
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
//

void setup();



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

// delays in one ms increments
void delay(unsigned int ms) {
    int i;
    for (i = 0; i < ms; i++) {
        asm("repeat #15993");
        asm("nop");
    }
    return;
}



void setup(){
    
    AD1PCFG = 0xffff;     //sets all pins to digital I/O
    CLKDIVbits.RCDIV=0;   // set frequency to 16 MHz 
}

int main(void) {
    setup();
    init_button();
    init_DAC();
    

    while (1);
    
    return -1;
}

