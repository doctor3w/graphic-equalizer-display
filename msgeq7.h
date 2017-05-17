//
//  msgeq7.h
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef msgeq7_h
#define msgeq7_h

#include "analog.h"
#include "gpio.h"
#include <stdio.h>
#include <fsl_clock_manager.h>

#define CHIP_LETTER C
#define CHIP_STROBE_PIN 17
#define CHIP_RESET_PIN 16

#define FREQ_ALPHA 10

/**
 * BANDS: 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz, 16kHz
 */

/** 
 * State machine states for reading from the MSGEQ7 chip
 */
typedef enum MSGEQ7_state {
  RESET_ON,
  RESET_OFF,
  STROBE_ON,
  STROBE_OFF,
  READ
} ReadState;

/**
 * State of the MSGEQ7 chip
 * Holds the previously read data for each band
 * Holds the next state in the state machine
 * And holds the next band to read
 */
typedef struct MSGEQ7 {
  uint16_t data[7];
  ReadState readState;
  uint8_t index;
} MSGEQ7_t;

/**
 * Inits the chip data to 0.
 */
void initMSGEQ7(MSGEQ7_t *chip);

/**
 * Sets up and starts the timers to begin analog read in of data
 */
void beginMSGEQ7(MSGEQ7_t *chip);

/**
 * Stops the timers and data reads.
 */
void endMSGEQ7(MSGEQ7_t *chip);

#endif /* msgeq7_h */
