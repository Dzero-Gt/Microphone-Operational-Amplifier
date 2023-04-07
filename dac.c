#include "xc.h"
#include <stdlib.h>

/* FUNCTION: init_dac()
 * ====================
 * Initializes the MCP4821 DAC to communicate using SPI 2.
 * PINS USED:
 *              SDA2 (pin6) - 
 *              SCL2 (pin7) - 
 * **NOT CURRENTLY FINISHED**
 */
void init_DAC(void) {
    // TODO:
    //  -Setup SPIEN
    //  -SISEL
    //  -SPIxCON1
    //  -MSTEN
    //  -SPIROV
    //  -SPIxBUF
    
    // Make sure !LDAC pin is low.
    
    _SPI2IF = 0;    // Clear SPI interrupt flag
    _SPI2IE = 1;    // Enable SPI interrupt.
      
    SPI2CON1 = 0;   // Clear all previous SPI settings.
    SPI2CON2 = 0;
    
    SPI2CON1bits.MODE16 = 1;    // 16-bit communcation.
    SPI2CON1bits.MSTEN = 1;     // Enable Master Mode
    SPI2CON1bits.CKP = 0;   // Clock is Idle on low, active on high.
    
    SPI2STATbits.SPIROV = 0;    // Clear SPI recieve overflow bit.
    SPI2STATbits.SPIEN = 1;     // Enable SPI
    
}

/* FUNCTION: write_DAC()
 * =====================
 * Writes a 
 * **NOT CURRENTLY FINISHED**
 */
void write_DAC(void) {
    // Make !CS pin low when transmitting data, high when not transmitting data.
    
    // Protocol:
    //      -Grab data from circular buffer.
    //      -Add data to SPIxBuffer
    //      -On interrupt update circular buffer.
    
    // DAC has 4 configuration bits, and then 12 data bits.
    // 0XGS dddd dddd ddXX
    // G = gain, (0 = 2.048V, 1 = 4.096V)
    // S = Shutdown (1 = Active, 0 = Inactive)
    
}
