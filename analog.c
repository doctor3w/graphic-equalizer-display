//
//  analog.c
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "analog.h"

void initAnalogA0() {
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
  ADC0_CFG1 |= 0x4; /* 16bits ADC*/
  ADC0_SC1A |= 0x1F; /* Disable the module, ADCH = 1111 */
}

uint16_t analogReadA0() {
  ADC0_SC1A = 12 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
  while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
  while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
  return ADC0_RA;
}
