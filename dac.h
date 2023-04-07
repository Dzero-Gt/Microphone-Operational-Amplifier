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

void init_DAC(void);
void write_DAC(int data);

#ifdef	__cplusplus
}
#endif

#endif	/* DAC_H */

