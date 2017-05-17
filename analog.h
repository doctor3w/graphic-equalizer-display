//
//  analog.h
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef analog_h
#define analog_h

#include <fsl_device_registers.h>
#include <stdio.h>

/**
 * Inits the proper ADCH and SC1A values to read from pin A0
 */
void initAnalogA0(void);

/**
 * Reads in the value from A0 on the K64F board 
 * (busy-waits until finished reading)
 * Modified from the examples in K64F examples
 */
uint16_t analogReadA0(void);

#endif /* analog_h */
