//
//  graphic_equalizer.c
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "graphic_equalizer.h"

void initGraphicEq(GraphicEq_t *geq;) {
  for (size_t i = 0; i < BARS_SIZE; i++) {
    geq->bars[i] = 0;
  }
  
  initMatrixState(geq->matrixState);
}

void setBarHeight(GraphicEq_t *geq, size_t bar, coord_t height, MatrixColor_t *color) {
  height = (height > MAX_HEIGHT) ? MAX_HEIGHT : height; // Maxes height with max
  coord_t xMin = MIN_BOUNDS[bar];
  coord_t xMax = MAX_BOUNDS[bar];
  coord_t prevHeight = geq->bars[bar];
  
  // If bar is shrinking, clear top parts
  if (height < prevHeight) {
    setRect(geq->matrixState, height+1, xMin, prevHeight, xMax, CLEAR);
  }
  setRect(geq->matrixState, 0, xMin, height, xMax, color); // Now set new rectangle
}
