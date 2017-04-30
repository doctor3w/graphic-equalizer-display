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
  initAnalogA0();
}

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip) {
  // This enable might cause Hard Fault if second line has been called somewhere else
  NVIC_EnableIRQ(PIT1_IRQn);
  SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
  
  // Need to load the timer
  PIT->CHANNEL[1].LDVAL = 20971520u; // Load the timer
  PIT->CHANNEL[1].TCTRL |= 0x3; // Enable interrupt and timer
  PIT->MCR &= !(1 << 1); // Disable MDIS
  
  // RESET chip
}

void PIT1_IRQHandler(void) {
  // Strobe the strobe,
  // Wait and turn off the strobe,
  // Wait for stable and then readin
}

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip);
