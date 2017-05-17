//
//  graphic_equalizer.c
//
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "graphic_equalizer.h"

// Height and width of matrix display
const uint8_t WIDTH = 32;
const uint8_t HEIGHT = 16;

const uint8_t MAX_HEIGHT = 15; // Max height of display value
const size_t BARS_SIZE = 7;    // Number of bars

// Bar x-value bounds (inclusive)
const coord_t MIN_BOUNDS[] = {2, 6, 10, 14, 19, 23, 27};
const coord_t MAX_BOUNDS[] = {4, 8, 12, 17, 21, 25, 29};

/**
 * Initializes the bars to height 0 and inits the matrix state.
 */
void initGraphicEq(GraphicEq_t *geq) {
  for (size_t i = 0; i < BARS_SIZE; i++) {
    geq->bars[i] = 0;
  }

  initMatrixState(&geq->matrixState);
}

MatrixColor_t GEQ_CLEAR; // Clear color so it's not inited every time

/**
 * Begins running the display and updating the bar heights
 * Must be called before calling setBarHeight
 */
void beginGraphicEq(GraphicEq_t *geq) {
  createColor(0, 0, 0, &GEQ_CLEAR); // Creates a clear color to lower bar height
  beginMatrix(&geq->matrixState);
}

/**
 * Sets [bars[bar]] to the height of [height] in [geq].
 */
void setBarHeight(GraphicEq_t *geq, size_t bar, coord_t height,
                  MatrixColor_t *color) {
  height = (height > MAX_HEIGHT) ? MAX_HEIGHT : height; // Maxes height with max

  // Retrieves the bounds for the bar
  coord_t xMin = MIN_BOUNDS[bar];
  coord_t xMax = MAX_BOUNDS[bar];
  coord_t prevHeight = geq->bars[bar];

  // If bar is shrinking, clear top parts
  if (height < prevHeight) {
    setRect(&geq->matrixState, height + 1, xMin, prevHeight, xMax, &GEQ_CLEAR);
  }
  setRect(&geq->matrixState, 0, xMin, height, xMax,
          color);          // Now set new rectangle
  geq->bars[bar] = height; // Set new height
}
