#include "xc.h"
#include <stdlib.h>

/* FUNCTION: init_dac()
 * ====================
 * Initializes the MCP4821 DAC to communicate using the SPI protocol.
 * PINS USED:
 *              SDO2 (pin6)     - SPI Data Output Line 
 *              SCL2 (pin7)     - SPI Data Clock Line
 *              !LDAC (pin15)   - DAC Latch.
 *                                  Pulse low to tell MCP4821 to update its 
 *                                  output. 
 *              !SS (pin16)     - Slave Select Bit
 *                                  Pulled low when transmitting new data.
 *                                  Same as !CS on MCP4821 datasheet.
 */
void init_DAC(void) {
    // Set pins to digital
    PORTBbits.RB2 = 1;      // SDO2
    PORTBbits.RB3 = 1;      // SCL2
    PORTBbits.RB6 = 1;      // !LDAC
    PORTBbits.RB7 = 1;      // !SS
    // Set Pins to Output.
    TRISBbits.TRISB2 = 0; // SCK
    TRISBbits.TRISB3 = 0; // SDO
    TRISBbits.TRISB6 = 0; // !LDAC
    TRISBbits.TRISB7 = 0; // SS

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR1bits.RP3R = 10;   //RB5->SPI2:SDO2; See Table 10-3 on P109 of the datasheet
    RPOR1bits.RP2R = 11;   //RB4->SPI2:SCK2OUT;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    // SPI2 Configuration
    SPI2CON1 = 0;
    SPI2CON1bits.MSTEN = 1;  // master mode
    SPI2CON1bits.MODE16 = 1; // sixteen bits
    SPI2CON1bits.CKE = 1;       // Rising Edge 
    SPI2CON1bits.CKP = 0;       //
    SPI2CON1bits.SPRE = 0b110; // secondary prescaler = 2
    SPI2CON1bits.PPRE = 0b11;  // primary prescaler = 1;
    SPI2CON2 = 0;
    SPI2STAT = 0;
    SPI2STATbits.SISEL = 0b101; // IF set when last bit is shifted out
                                // That means the SPI transfer is complete.
    SPI2STATbits.SPIEN = 1;

    // Enabling SS
    SPI2CON1bits.SSEN = 1;
    _RB7 = 1;   // SS is high initially --> MCP4821 not currently selected.

    // Enable SPI2 Interrupt.
    _SPI2IF = 0;
    _SPI2IE = 1;
}

/* FUNCTION: write_DAC()
 * =====================
 * Writes a ....
 * INPUTS:
 *          int data        - between 1 and 12 bit data input. Anything else
 *                              longer will be truncated to 12 bits.
 *          int dataSize    - number of bits the data is. if data has more than
 *                              12 bits, just put 12 here.
 *          int lowGain     - Set to 0 for 100% gain, 1 for 50% gain.
 */
void write_DAC(int data, char dataSize, char lowGain) {
    // Make !CS pin low when transmitting data, high when not transmitting data.
        
    // DAC has 4 configuration bits, and then 12 data bits.
    // 0XGS dddd dddd dd00
    // G = gain, (0 = 2.048V, 1 = 4.096V)
    // S = Shutdown (1 = Active, 0 = Inactive)
    
    _RB7 = 0; // SS is low.
    int DACBITS = 0b0001000000000000;   // set S = 0 
    int DACMASK_high = 0b0001111111111111;  // Gain high
    int DACMASK_low = 0b0011111111111111;   // Gain low
    int output = (data << (12 - dataSize));
    if(lowGain == 1) {
        output &= DACMASK_low;
    }
    else {
        output &= DACMASK_high;
    }
    output |= DACBITS;
    SPI2BUF = output;
    return;
}
