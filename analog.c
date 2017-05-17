//
//  analog.c
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "analog.h"

/**
 * Inits the proper ADCH and SC1A values to read from pin A0
 */
void initAnalogA0(void) {
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
  ADC0_CFG1 |= 0x4;  /* 16-bit ADC*/
  ADC0_SC1A |= 0x1F; /* Disable the module, ADCH = 11111 */
}

// Read and wait until conversion is finished from pin A0
uint16_t analogReadA0(void) {
  // Sets up ADC mux to read only from pin A0
  // ANDS with the channel mask to make the value the channel AD12
  ADC0_SC1A = 12 & ADC_SC1_ADCH_MASK; // Write to SC1A to start conversion
  // ANDS SC2 with active mask to get active value
  while (ADC0_SC2 & ADC_SC2_ADACT_MASK)
    ; // Conversion in progress
  // ANDS SC1A with COCO_MASK (bit 7) to get value of COCO
  while (!(ADC0_SC1A & ADC_SC1_COCO_MASK))
    ; // Wait until conversion complete
  return ADC0_RA;
}
