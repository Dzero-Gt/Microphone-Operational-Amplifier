#include "xc.h"
#include <stdlib.h>

/* FUNCTION: init_dac()
 * ====================
 * Initializes the MCP4821 DAC to communicate using SPI 2.
 * PINS USED:
 *              SDO2 (pin6) - 
 *              SCL2 (pin7) - 
 *              LDAC (pin15) -
 *              SS   (pin16) -  
 */
void init_DAC(void) {
    
    PORTBbits.RB2 = 1;      // These pins
    PORTBbits.RB3 = 1;      // are
    PORTBbits.RB6 = 1;      // all
    PORTBbits.RB7 = 1;      // digital
    
    TRISBbits.TRISB2 = 0; // SCK
    TRISBbits.TRISB3 = 0; // SDO
    TRISBbits.TRISB6 = 0; // this will be the DAC /LDAC. I assume /CS is hardwired to GND
    TRISBbits.TRISB7 = 0; // SS (Slave Select Bit) Pulled low when transmitting data.

    // Peripheral Pin Select 
    // Ver 1.25 or later
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR1bits.RP3R = 10;   //RB5->SPI1:SDO1; See Table 10-3 on P109 of the datasheet
    RPOR1bits.RP2R = 11;   //RB4->SPI1:SCK1OUT;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    SPI2CON1 = 0;
    SPI2CON1bits.MSTEN = 1;  // master mode
    SPI2CON1bits.MODE16 = 1; // sixteen bits
    SPI2CON1bits.CKE = 1;
    SPI2CON1bits.CKP = 0;
    SPI2CON1bits.SPRE = 0b110; // secondary prescaler = 2
    SPI2CON1bits.PPRE = 0b11;  // primary prescaler = 1;
    //SPI1CON1bits.PPRE = 0b01;  // primary prescaler = 16;
    SPI2CON2 = 0;
    SPI2STAT = 0;
    SPI2STATbits.SISEL = 0b101; // IF set when last bit is shifted out
                                // That means the SPI xfer is complete.
    SPI2STATbits.SPIEN = 1;

    // Enabling SS
    SPI2CON1bits.SSEN = 1;
    _RB7 = 1;

    IPC1bits.T2IP = 5;
    IEC0bits.T2IE = 1;
    _SPI2IF = 0;
    _SPI2IE = 1;
}

/* FUNCTION: write_DAC()
 * =====================
 * Writes a 
 * **NOT CURRENTLY FINISHED**
 * INPUTS:
 *          int data - 
 *          int gain - 0 or 1
 */
void write_DAC(int data,char gain) {
    // Make !CS pin low when transmitting data, high when not transmitting data.
    
    // Protocol:
    //      -Grab data from circular buffer.
    //      -Add data to SPIxBuffer
    //      -On interrupt update circular buffer.
        
    // DAC has 4 configuration bits, and then 12 data bits.
    // 0XGS dddd dddd dd00
    // G = gain, (0 = 2.048V, 1 = 4.096V)
    // S = Shutdown (1 = Active, 0 = Inactive)
    
    _RB7 = 0; // SS is low.
    int DACBITS = 0b0001000000000000;
    int DACMASK_high = 0b0001111111111111;
    int DACMASK_low = 0b0011111111111111;
    int output = data;
    if(gain == 1) {
        output &= DACMASK_low;
    }
    else {
        output &= DACMASK_high;
    }
    output |= DACBITS;
    SPI2BUF = output;
    return;
}
