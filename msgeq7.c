//
//  msgeq7.c
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "msgeq7.h"
//#include "fsl_debug_consol.h"

// These are in micro seconds
const uint8_t RESET_WIDTH_MIN = 100;
const uint8_t RESET_TO_STROBE_DELAY_MIN = 72;
const uint8_t STROBE_WIDTH_MIN = 18;
const uint8_t STROBE_TO_STROBE_DELAY_MIN = 72;
const uint8_t OUTPUT_STABILIZE_TIME = 36;

// These need to be micro seconds
const uint16_t RESET_WIDTH = 10;
const uint16_t RESET_TO_STROBE = 200;
const uint16_t STROBE_ON_DELAY = 100;
const uint16_t STROBE_WIDTH = 100;
const uint16_t OUTPUT_DELAY = 100;
const uint16_t READ_TO_STROBE = 100;
const uint16_t READ_TO_RESET = 10000;

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
  chip_state->readState = RESET_ON;
}

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip) {
  // This enable might cause Hard Fault if second line has been called somewhere else
  NVIC_EnableIRQ(PIT1_IRQn);
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
  DIGITAL_OUT(CHIP_LETTER, CHIP_RESET_PIN);
  DIGITAL_OUT(CHIP_LETTER, CHIP_STROBE_PIN);
	
  // Need to load the timer
  PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * 30 / 1000; // Load the timer with frequency of reads
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
      PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * RESET_WIDTH / 1000000;
      break;
    case RESET_OFF:
      resetOffChip();
      chip_state->readState = STROBE_ON;
      PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * RESET_TO_STROBE / 1000000;
      break;
    case STROBE_ON:
      strobeOn();
      chip_state->readState = STROBE_OFF;
      PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * STROBE_WIDTH / 1000000;
      break;
    case STROBE_OFF:
      strobeOff();
      chip_state->readState = READ;
      PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * OUTPUT_DELAY / 1000000;
      break;
    case READ:
      // READ in
      chip_state->data[chip_state->index] = analogReadA0();
      chip_state->index++;
      if (chip_state->index > 6) {
        chip_state->readState = RESET_ON;
        PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK / 1000;
      } else {
        chip_state->readState = STROBE_ON;
		//PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK / 10;
        PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * READ_TO_STROBE / 1000000;
      }
      break;
  }
  PIT->CHANNEL[1].TFLG = 1;
}

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip) {

}
