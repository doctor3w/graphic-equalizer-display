//
//  msgeq7.h
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef msgeq7_h
#define msgeq7_h

#include <stdio.h>
#include "analog.h"
#include "gpio.h"

#define CHIP_LETTER A
#define CHIP_STROBE_PIN 1
#define CHIP_RESET_PIN 2

/**
 * BANDS: 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz, 16kHz
 */

// These are in micro seconds
const uint8_t RESET_WIDTH_MIN = 100;
const uint8_t RESET_TO_STROBE_DELAY_MIN = 72;
const uint8_t STROBE_WIDTH_MIN = 18;
const uint8_t STROBE_TO_STROBE_DELAY_MIN = 72;
const uint8_t OUTPUT_STABILIZE_TIME = 36;

// These need to be micro seconds
const uint16_t RESET_WIDTH = 150;
const uint16_t RESET_TO_STROBE = 100;
const uint16_t STROBE_ON_DELAY = 100;
const uint16_t STROBE_WIDTH = 30;
const uint16_t OUTPUT_DELAY = 50;
const uint16_t READ_TO_STROBE = 72;
const uint16_t READ_TO_RESET = 72;

typedef enum MSGEQ7_state {RESET_ON, RESET_OFF, STROBE_ON, STROBE_OFF, READ} ReadState;

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
