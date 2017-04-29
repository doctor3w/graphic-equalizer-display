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

// Struct with 7 bars' heights and a matrix display state
typedef struct GraphicEqualizer {
    coord_t bars[7];
    MatrixState_t *state;
} GraphicEq_t;

void graphicEqInit(GraphicEq_t *geq;);
void graphicEqSetBarHeight(GraphicEq_t *geq, uint8_t bar, coord_t height);

#endif /* graphic_equalizer_h */
