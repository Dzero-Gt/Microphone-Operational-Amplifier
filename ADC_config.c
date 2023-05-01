#include <p24FJ64GA002.h>
#include "ADC_config.h"

void ADC_init(){
    
    //sets needed pints to input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    
    //Analog band gap voltage reference enabled for both pins
    _PCFG0 = 0; 
    _PCFG1 = 0;
    

    
    AD1CON2bits.VCFG = 0b000; //Vdd st to max voltage and Vss to min voltage
    AD1CON3bits.ADCS = 1; // min speed for 16MHz
    AD1CON1bits.SSRC = 0b010; //Timer 3 used to triggger interuppt for conversion
    AD1CON3bits.SAMC = 1; //auto-sample time set to fastest at 1.
    AD1CON1bits.FORM = 0b01;  //signed integer
    AD1CON1bits.ASAM = 1; //starts sample timer immediatly after conversion completes
    AD1CON2bits.SMPI = 0; //interrupts after every conversion
    AD1CON1bits.ADON = 1; //turn on
    
    //timer interrupt configuration
    _AD1IF = 0; 
    _AD1IE = 1;
    
    
    //sample interrupt timer
    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b00;
    PR3 = 362; //for 44.1KHz sampling, gives roughly 300 cycles per sample for processing
    T3CONbits.TON = 1;
}


