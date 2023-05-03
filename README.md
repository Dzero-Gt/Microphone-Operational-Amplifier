# Microphone Filter Controller

## Introduction
Microphone Filter Controller is a C library that provides the functionality to take analog audio input from an Electret Microphone amplifier, filters it based on selector/s for varying bandwidths perform Analog to Digital conversion (ADC), process, and amplify, perform Digital to Analog conversion (DAC), and output to a speaker or other device.

## Hardware Description
- PIC24FJ64GA002: <a href="https://ww1.microchip.com/downloads/en/devicedoc/39881e.pdf">Datasheet<a>
- Electret MAX9814 Microphone: <a href="https://cdn-shop.adafruit.com/datasheets/MAX9814.pdf">Datasheet<a>
- MCP4821 DAC: <a href="https://datasheet.lcsc.com/lcsc/2109221453_Microchip-Tech-MCP4821-E-MS_C636207.pdf">Datasheet<a>

## Full Documentation

### ADC
- `ADC_init():`
  - ADC_init() initializes and enables the Analog to Digital Converter (ADC) module for the microcontroller. No parameters or inputs are involved
  - <b>Internal Functionality:</b>
    - Sets TRISAbits.TRISA0 and TRISAbits.TRISA1 to input mode.
    - Enables analog band gap voltage reference for both input pins.
    - Configures the ADC to use Vdd as the maximum voltage and Vss as the minimum voltage reference.
    - Configures TAD to allow for maximum conversion speed at 16MHz.
    - Configures the ADC to output signed integer format.
    - Set the sample timer to startimmediately after the conversion is complete.
    - Configures the ADC to generate an interrupt after every conversion.
    - Initializes Timer3 for the interrupt-based conversions.
    - Clears the ADC interrupt flag and enables the ADC interrupt.
 -  `ADC1Interrupt():`
    - Interrupt service routine that handles ADC1 interrupts. All signal processing and SPI output to the DAC is handled within this interrupt.

### DAC
- `void init_DAC(void);`
  - The init_DAC() function initializes the MCP4821 DAC to communicate using the SPI protocol. It uses the following pins for communication:
    - <b>SDO2</b> (pin6) - SPI Data Output Line
    - <b>SCL2</b> (pin7) - SPI Data Clock Line
    - <b>!LDAC</b> (pin15) - DAC Latch. Pulse low to tell MCP4821 to update its output.
    - <b>!SS</b> (pin16) - Slave Select Bit. Pulled low when transmitting new data. Same as !CS on MCP4821 datasheet.
- `void write_DAC(int data, char dataSize, char lowGain)`
  - Writes a value to the MCP4821 DAC using the SPI protocol.

  - <u>Inputs:</u>
    - <b>data:</b> An integer value between 1 and 12 bits representing the data to be written. If the input data has more than 12 bits, it will be truncated to 12 bits.
    - <b>dataSize:</b> An integer value representing the number of bits the input data is. If the input data has more than 12 bits, this argument should be set to 12.
    - <b>lowGain:</b> An integer value representing the gain level of the DAC. Set to 0 for 100% gain, and 1 for 50% gain.

### Button
- `void init_button()`
  - `init_button()` Initializes a button whose primary purpose is that of a toggle.
  - Sets up an interrupt service routine `(ISR)` using an Input Capture `(IC)` module to capture the falling edge of the button press. 
  - Additionally, it is configuring a Timer 2 `(TMR2)` module to generate a periodic interrupt at 1 second intervals.
  
  - Set RB8 as an input pin `(TRISBbits.TRISB8 = 1)`
  - Enable the internal pull-up resistor on RB8 `(CNPU2bits.CN22PUE=1)`
  - Configure IC1 to use Pin RP8/RB8 as an input, set the interrupt on every falling edge and enable its interrupt 
  - `(IC1CON, RPINR7bits.IC1R, IEC0bits.IC1IE, IC1CONbits.ICM)`
- Interrupts
  ```c
  
  // volatile variables for use in ISR's
  volatile int State = 0; // state:0  use raw input 
                        // state:1  use filtered data

  volatile unsigned int overflow=0; // overflow occurs after one second
  volatile unsigned long int time_current_click=0;
  volatile unsigned long int time_preivious_click=0;
  
  //T2 interrupt we use overflow in IC interrupt for debouncing
  void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
      _T2IF = 0; // turn the interrupt flag back to 0
      overflow++; // and increase the overflow by one
  }
  //IC1 Interrupt is used for button debouncing as well as ADC mux bit selection
  void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void){

    _IC1IF = 0; // reset the IC interrupt flag
    time_preivious_click=time_current_click;
    time_current_click = (unsigned long int)((unsigned long int)IC1BUF + (unsigned long int)overflow*(PR2+1));
    if((time_current_click - time_preivious_click) >125){ //to achieve a 2mS debounce delay must find (2/1000)(PR2+1)=125
        State = 1-State; // we intend to use the button as a toggle between the raw and filtered input
    }
    if (State){
        AD1CHS = 0x0001; //switch to filtered input (pin AN1)     
    }
    if (!State){
          AD1CHS = 0x0000; //switch to unfiltered input (pin AN0)
    } 
  }
  ```
  
### Misc
  - `void delay(unsigned int ms):`
    - Delay function takes an unsigned integer "ms" as an input parameter, representing the number of milliseconds to delay the program execution. Function uses       looping assembly code to achieve 1ms delay. Code only works properly at 16MHz clock speed.
    - Inputs:
      - "ms": an unsigned integer that represents the number of milliseconds to delay.
    - Output:
      - This function does not return any value.
  
## Basic Usage Example
Runs all of the basic initialization functions required to set up usage of the DAC, the ADC, and the button. Pin requirements are detailed under "Full Documentation"

```c
#include <stdio.h>
#include "dac.h"
#include "ADC_config.h"
#include "button.h"

void setup(){
  AD1PCFG = 0xffff;     //sets all pins to digital I/O
  CLKDIVbits.RCDIV=0;   // set frequency to 16 MHz 
}
  
//ADC Interrupt
void __attribute__ ((__interrupt__, __auto_psv__)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    int data = ADC1BUF0;
    write_DAC(data, 10, 1);
    // include any signal additional signal processing here
  
  
int main() {
  
  setup();
  init_button();
  init_DAC();
  ADC_init();
  
  while (1){}
  
  return(-1);
}

```
## Advanced Usage Example
Sets up the DAC, ADC, and Button, including the LED state tracking for button filter control.

```c
#include "xc.h"
#include <stdio.h>
#include "dac.h"
#include "ADC_config.h"
#include "button.h"

  
  
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
  
 void __attribute__ ((__interrupt__, __auto_psv__)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    int data = ADC1BUF0;
    write_DAC(data, 10, 1);
    // include any signal additional signal processing here
  
  
  void setup(){
    AD1PCFG = 0xffff;     //sets all pins to digital I/O
    CLKDIVbits.RCDIV=0;   // set frequency to 16 MHz 
  }
  
  int main() {
  
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
    return 0;
}

```
