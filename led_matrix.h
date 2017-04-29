#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdio.h>

typedef struct MatrixColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} MatrixColor_t;

void clearScreen();
void fillRect(int x1, int y1, int x2, int y2, MatrixColor_t col);


// This method uses a set 7 bars

void setBarHeight(int barID, int height);

#endif
