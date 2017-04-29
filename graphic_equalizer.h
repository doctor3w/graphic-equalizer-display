//
//  graphic_equalizer.h
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef graphic_equalizer_h
#define graphic_equalizer_h

#include <stdio.h>
#include "led_matrix.h"

/** BAR DISPLAY EXAMPLE
 
 --***-***-***-****-***-***-***--
 01234567890123456789012345678901 //32 LEDS
 
 */

// Height and width of matrix display
const uint8_t WIDTH = 32;
const uint8_t HEIGHT = 16;

const uint8_t MAX_HEIGHT = 15; // Max height of display value
const size_t BARS_SIZE = 7; // Number of bars

// Bar x-value bounds (inclusive)
const coord_t MIN_BOUNDS[] = {2, 6, 10, 14, 19, 23, 27}
const coord_t MAX_BOUNDS[] = {4, 8, 12, 16, 21, 25, 29}

// Clear color (off) for matrix
const MatrixColor_t CLEAR = {0, 0, 0};

/**
 * Represents the 7 bar graphic equalizer.
 * Contains the height of the bar and the matrix display state.
 * Each bar is an integer from 0-15, 15 being the max height
 */
typedef struct GraphicEqualizer {
    coord_t bars[7];
    MatrixState_t *matrixState;
} GraphicEq_t;

/**
 * Initializes the bars to height 0 and inits the matrix state. 
 */
void initGraphicEq(GraphicEq_t *geq;);

/**
 * Sets [bars[bar]] to the height of [height] in [geq].
 */
void setBarHeight(GraphicEq_t *geq, size_t bar, coord_t height);

#endif /* graphic_equalizer_h */
