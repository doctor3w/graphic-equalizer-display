//
//  msgeq7.c
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "msgeq7.h"

/**
 * Inits the chip data to 0.
 */
void initMSGEQ7(MSGEQ7_t *chip) {
  for (size_t i = 0; i < 7; i++) {
    chip->data[i] = 0;
  }
}

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip) {
  
  //ADC0_SC1A |= 0x40; /* Enable interrupts and DIFF = 0 */
  
  
}

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip);
