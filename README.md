# Microphone Filter Controller

## Introduction
Microphone Filter Controller is a C library that provides the functionality to take analog audio input from an Electret Microphone amplifier, filters it based on selector/s for varying bandwidths perform Analog to Digital conversion (ADC), process, and amplify, perform Digital to Analog conversion (DAC), and output to a speaker or other device.

## Hardware Description
[Describe the hardware that the library interacts with. Include information such as device(s), part numbers, links to datasheets or product pages, and any other relevant details.]

## Full Documentation
[Provide a comprehensive list of all public functions in the library, including their parameters, return types, and any other relevant information. Organize the functions into logical groups and provide links to more detailed documentation for each function if necessary.]

### ADC
- `return_type function1(arg1: type, arg2: type)`
  - [Description of function1]
- `return_type function2(arg1: type, arg2: type)`
  - [Description of function2]

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
- `return_type function5(arg1: type, arg2: type)`
  - [Description of function5]
- `return type function6(arg1: type, arg2: type)`
  - [Description of function6]

## Basic Usage Example
[Provide a brief code snippet that demonstrates the most basic usage of the library. This should be a simple example that allows users to quickly test the functionality of the library.]

```c
#include <stdio.h>
#include "dac.h"
#include "ADC_config.h"
#include "button.h"

int main() {
    // [Call a simple function from the library and print the output]
    return 0;
}

```
##Advanced Usage Example
[Provide a more complex example that covers all the major features and functions of the library. This should be a more realistic use case that demonstrates the power and flexibility of the library.]

```c
#include "xc.h"
#include <stdio.h>
#include "dac.h"
#include "ADC_config.h"
#include "button.h"

int main() {
    // [Call a simple function from the library and print the output]
    return 0;
}

```