//
//  graphic_equalizer.c
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#include "graphic_equalizer.h"

const uint8_t MAX_HEIGHT = 16;
const size_t BARS_SIZE = 7;

void graphicEqInit(GraphicEq_t *geq;) {
  for (size_t i = 0; i < BARS_SIZE) {
    geq->bars[i] = 0;
  }
  
  geq->MatrixState_t
}

void graphicEqSetBarHeight(GraphicEq_t *geq, uint8_t bar, coord_t height);
