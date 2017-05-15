//
//  graphic_equalizer.h
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef graphic_equalizer_h
#define graphic_equalizer_h

#include "led_matrix.h"
#include <stdio.h>

/** BAR DISPLAY EXAMPLE

 --***-***-***-****-***-***-***--
 01234567890123456789012345678901 //32 LEDS

 */

/**
 * Represents the 7 bar graphic equalizer.
 * Contains the height of the bar and the matrix display state.
 * Each bar is an integer from 0-15, 15 being the max height
 */
typedef struct GraphicEqualizer {
  coord_t bars[7];
  MatrixState_t matrixState;
} GraphicEq_t;

/**
 * Initializes the bars to height 0 and inits the matrix state.
 */
void initGraphicEq(GraphicEq_t *geq);

void beginGraphicEq(GraphicEq_t *geq);

/**
 * Sets [bars[bar]] to the height of [height] in [geq].
 */
void setBarHeight(GraphicEq_t *geq, size_t bar, coord_t height,
                  MatrixColor_t *color);

#endif /* graphic_equalizer_h */
