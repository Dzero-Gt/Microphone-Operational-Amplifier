#include <p24FJ64GA002.h>
#include "ADC_config.h"

void ADC_init(){
    
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    
    _PCFG0 = 0;
    _PCFG1 = 0;
    
    AD1CHS = 0x0000;
    
    AD1CON2bits.VCFG = 0b000;
    AD1CON3bits.ADCS = 1; //for 44.1KHz sampling
    AD1CON1bits.SSRC = 0b010;
    AD1CON3bits.SAMC = 1;
    AD1CON1bits.FORM = 0b01;  //signed integer
    AD1CON1bits.ASAM = 1;
    AD1CON2bits.SMPI = 0;
    AD1CON1bits.ADON = 1;
    
    _AD1IF = 0;
    _AD1IE = 1;
    
    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b00;
    PR3 = 362;
    T3CONbits.TON = 1;
}


