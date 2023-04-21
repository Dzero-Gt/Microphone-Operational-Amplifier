/* 
 * File:   dac.h
 * Author: coopb
 *
 * Created on April 7, 2023, 3:58 PM
 */

#ifndef DAC_H
#define	DAC_H

#ifdef	__cplusplus
extern "C" {
#endif

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
    void init_DAC(void);
    
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
    void write_DAC(int data, char dataSize, char lowGain);

#ifdef	__cplusplus
}
#endif

#endif	/* DAC_H */

