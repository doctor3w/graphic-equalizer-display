//
//  LEDMatrix.h
//  
//
//  Created by Drew Dunne on 4/29/17.
//
//

#ifndef LEDMatrix_h
#define LEDMatrix_h

#include <stdio.h>

#endif /* LEDMatrix_h */

typedef struct MatrixColor {
    int r;
    int g;
    int b;
} MatrixColor_t;

void clearScreen();
void fillRect(int x1, int y1, int x2, int y2, MatrixColor_t col);


// This method uses a set 7 bars

void setBarHeight(int barID, int height);
