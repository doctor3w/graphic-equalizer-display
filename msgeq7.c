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
  chip->readState = RESET_ON;
}

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip) {
  NVIC_EnableIRQ(PIT1_IRQn); // Enables the PIT timer
  
  chip_state = chip; // Sets the current chip state
	
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enables clock to PIT timer

  // Configures the digital out pins
  DIGITAL_OUT(CHIP_LETTER, CHIP_RESET_PIN);
  DIGITAL_OUT(CHIP_LETTER, CHIP_STROBE_PIN);

  // Need to load the timer
  PIT->CHANNEL[1].LDVAL = DEFAULT_SYSTEM_CLOCK * 30 /
                          1000; // Load the timer with frequency of reads
  PIT->CHANNEL[1].TCTRL |= 0x3; // Enable interrupt and timer
  PIT->MCR = 0;        // Disable MDIS
}

// Sets reset to HIGH value and resets the band index
void resetOnChip() {
  chip_state->index = 0;
  DIGITAL_WRITE_HIGH(CHIP_LETTER, CHIP_RESET_PIN);
}

// Sets reset to LOW
void resetOffChip() { DIGITAL_WRITE_LOW(CHIP_LETTER, CHIP_RESET_PIN); }

// Sets strobe to HIGH
void strobeOn() { DIGITAL_WRITE_HIGH(CHIP_LETTER, CHIP_STROBE_PIN); }

// Sets strobe to LOW
void strobeOff() { DIGITAL_WRITE_LOW(CHIP_LETTER, CHIP_STROBE_PIN); }

// Handles the interrupt
void PIT1_IRQHandler(void) {
  uint32_t read; // Declaration of read value
  // RESET chip, STROBE chip, waits for chip output to stabilize,
  // reads analog input, high pass filter the value
  switch (chip_state->readState) {
  case RESET_ON:
    resetOnChip();
    chip_state->readState = RESET_OFF;
    PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() * RESET_WIDTH / 1000000; 
    break;
  case RESET_OFF:
    resetOffChip();
    chip_state->readState = STROBE_ON;
    PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() * RESET_TO_STROBE / 1000000;
    break;
  case STROBE_ON:
    strobeOn();
    chip_state->readState = STROBE_OFF;
    PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() * STROBE_WIDTH / 1000000;
    break;
  case STROBE_OFF:
    strobeOff();
    chip_state->readState = READ;
    PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() * OUTPUT_DELAY / 1000000;
    break;
  case READ:
    // READ in value
	read = analogReadA0();
	// Apply simple high pass filter
	read = read * FREQ_ALPHA / 100 + (uint32_t)(chip_state->data[chip_state->index]) * (100 - FREQ_ALPHA) / 100;
    chip_state->data[chip_state->index] = read; // Store
    chip_state->index++; // increment to next band
    if (chip_state->index > 6) {
      chip_state->readState = RESET_ON;
      PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() / 1000;
    } else {
      chip_state->readState = STROBE_ON;
      PIT->CHANNEL[1].LDVAL = CLOCK_SYS_GetSystemClockFreq() * READ_TO_STROBE / 1000000;
    }
    break;
  }
  PIT->CHANNEL[1].TFLG = 1; // Clear flag
}

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip) {
  PIT->CHANNEL[1].TCTRL = 1; // Disable interrupt and timer
}
