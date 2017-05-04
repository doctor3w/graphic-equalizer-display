//
//  msgeq7.c
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "msgeq7.h"

MSGEQ7_t *chip_state = NULL;

/**
 * Inits the chip data to 0.
 */
void initMSGEQ7(MSGEQ7_t *chip) {
  for (size_t i = 0; i < 7; i++) {
    chip->data[i] = 0;
  }
  initAnalogA0();
  chip_state = chip;
  chip_state->readState = RESET;
}

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip) {
  // This enable might cause Hard Fault if second line has been called somewhere else
  NVIC_EnableIRQ(PIT1_IRQn);
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

  // Need to load the timer
  PIT->CHANNEL[1].LDVAL = 100; // Load the timer with frequency of reads
  PIT->CHANNEL[1].TCTRL |= 0x3; // Enable interrupt and timer
  PIT->MCR &= !(1 << 1); // Disable MDIS
}

void resetOnChip() {
  chip_state->index = 0;
  DIGITAL_WRITE_HIGH(CHIP_LETTER, CHIP_RESET_PIN);
}

void resetOffChip() {
  DIGITAL_WRITE_LOW(CHIP_LETTER, CHIP_RESET_PIN);
}

void strobeOn() {
  DIGITAL_WRITE_HIGH(CHIP_LETTER, CHIP_STROBE_PIN);
}

void strobeOff() {
  DIGITAL_WRITE_LOW(CHIP_LETTER, CHIP_STROBE_PIN);
}

void PIT1_IRQHandler(void) {
  // Strobe the strobe,
  // Wait and turn off the strobe,
  // Wait for stable and then reading
  switch (chip_state->readState) {
    case RESET_ON:
      resetOnChip();
      chip_state->readState = RESET_OFF;
      PIT->CHANNEL[1].LDVAL = RESET_WIDTH;
      break;
    case RESET_OFF:
      resetOffChip();
      chip_state->readState = STROBE_ON;
      PIT->CHANNEL[1].LDVAL = RESET_TO_STROBE;
      break;
    case STROBE_ON:
      strobeOn();
      chip_state->readState = STROBE_OFF;
      PIT->CHANNEL[1].LDVAL = STROBE_WIDTH;
      break;
    case STROBE_OFF:
      strobeOff();
      chip_state->readState = READ;
      PIT->CHANNEL[1].LDVAL = OUTPUT_DELAY;
      break;
    case READ:
      // READ in
      chip_state->data[chip_state->index] = analogReadA0();
      chip_state->index++;
      if (chip_state->index >= 7) {
        chip_state->readState = RESET_ON;
        PIT->CHANNEL[1].LDVAL = READ_TO_STROBE;
      } else {
        chip_state->readState = STROBE_ON;
        PIT->CHANNEL[1].LDVAL = READ_TO_RESET;
      }
      break;
  }
  PIT->CHANNEL[1].TFLAG = 1;
}

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip) {

}
