#include "button.h"
#include "graphic_equalizer.h"
#include "msgeq7.h"

void barTest(void) {
  GraphicEq_t state;
  initGraphicEq(&state);
  beginGraphicEq(&state);
  MatrixColor_t purple;
  createColor(127, 0, 127, &purple);
  setBarHeight(&state, 0, 5, &purple);
  while (1)
    ;
}

void pixelTest(void) {
  MatrixState_t state;
  initMatrixState(&state);
  beginMatrix(&state);
  MatrixColor_t purple;
  createColor(255, 0, 255, &purple);
  setPixel(&state, 0, 0, &purple);
  setPixel(&state, 15, 31, &purple);
  while (1)
    ;
}

void rectTest(void) {
  MatrixState_t state;
  initMatrixState(&state);
  beginMatrix(&state);
  MatrixColor_t purple;
  createColor(255, 0, 255, &purple);
  setRect(&state, 0, 10, 10, 20, &purple);
  createColor(127, 0, 127, &purple);
  setRect(&state, 0, 0, 10, 10, &purple);
  while (1)
    ;
}

void shadingTest(void) {
  MatrixState_t state;
  initMatrixState(&state);
  beginMatrix(&state);
  MatrixColor_t purple;
  createColor(255, 0, 255, &purple);
  setRect(&state, 0, 0, 10, 8, &purple);
  createColor(191, 0, 191, &purple);
  setRect(&state, 0, 7, 10, 15, &purple);
  createColor(127, 0, 127, &purple);
  setRect(&state, 0, 15, 10, 23, &purple);
  createColor(63, 0, 63, &purple);
  setRect(&state, 0, 23, 10, 31, &purple);
  while (1)
    ;
}

void nothingTest(void) {
  MatrixState_t state;
  initMatrixState(&state);
  beginMatrix(&state);
  while (1)
    ;
}

void audioTest(void) {
  MSGEQ7_t chipState;
  initMSGEQ7(&chipState);
  beginMSGEQ7(&chipState);
  while (1)
    ;
}

void bigBarTest(void) {
  GraphicEq_t state;
  initGraphicEq(&state);
  beginGraphicEq(&state);
  MatrixColor_t purple;
  createColor(255, 0, 255, &purple);
  setBarHeight(&state, 0, 5, &purple);
  createColor(255, 0, 127, &purple);
  setBarHeight(&state, 1, 10, &purple);
  createColor(0, 0, 255, &purple);
  setBarHeight(&state, 2, 7, &purple);
  createColor(255, 0, 0, &purple);
  setBarHeight(&state, 3, 15, &purple);
  createColor(0, 255, 255, &purple);
  setBarHeight(&state, 4, 9, &purple);
  createColor(0, 255, 0, &purple);
  setBarHeight(&state, 5, 3, &purple);
  createColor(0, 127, 0, &purple);
  setBarHeight(&state, 6, 6, &purple);
  while (1)
    ;
}

void bigBarChangeTest(void) {
  GraphicEq_t state;
  initGraphicEq(&state);
  beginGraphicEq(&state);
  MatrixColor_t purple;
  int offset = 0;
  int dir = 1;
  while (1) {
    createColor(255, 0, 255, &purple);
    setBarHeight(&state, 0, 5 + offset, &purple);
    createColor(255, 0, 127, &purple);
    setBarHeight(&state, 1, 10 + offset, &purple);
    createColor(0, 0, 255, &purple);
    setBarHeight(&state, 2, 7 + offset, &purple);
    createColor(255, 0, 0, &purple);
    setBarHeight(&state, 3, 15 + offset, &purple);
    createColor(0, 255, 255, &purple);
    setBarHeight(&state, 4, 9 + offset, &purple);
    createColor(0, 255, 0, &purple);
    setBarHeight(&state, 5, 3 + offset, &purple);
    createColor(0, 127, 0, &purple);
    setBarHeight(&state, 6, 6 + offset, &purple);
    for (int x = 0; x < 1000000; x++)
      ;
    offset += dir;
    if (offset == 10) {
      dir = -1;
    }
    if (offset == 0) {
      dir = 1;
    }
  }
}

volatile uint8_t color_scheme = 0;
const uint8_t COLOR_SCHEME_COUNT = 2;

void buttonCallback() {
  color_scheme++;
  color_scheme %= COLOR_SCHEME_COUNT;
}

void realMain() {
  MSGEQ7_t chipState;
  initMSGEQ7(&chipState);
  beginMSGEQ7(&chipState);

  GraphicEq_t state;
  initGraphicEq(&state);
  beginGraphicEq(&state);

  ButtonState_t button;
  initButtonState(&button);
  setButtonCallback(&button, &buttonCallback);
  beginButton(&button);

  MatrixColor_t red;
  createColor(255, 0, 0, &red);
  MatrixColor_t green;
  createColor(0, 255, 0, &green);
  MatrixColor_t blue;
  createColor(0, 0, 255, &blue);

  MatrixColor_t purple;
  createColor(255, 0, 255, &purple);
  MatrixColor_t yellow;
  createColor(255, 255, 0, &yellow);
  MatrixColor_t cyan;
  createColor(0, 255, 255, &cyan);

  MatrixColor_t white;
  createColor(255, 255, 255, &white);

  MatrixColor_t *color_schemes[][] = {
      {&blue, &blue, &blue, &blue, &blue, &blue, &blue},
      {&red, &green, &blue, &purple, &yellow, &cyan, &white}};

  while (1) {
    for (size_t i = 0; i < 7; i++) {
      uint16_t data = chipState.data[i];
      coord_t height = 15 * data / 0xFFF;
      setBarHeight(&state, i, height, color_schemes[color_scheme][i]);
    }
  }
}

int main(int count, char **args) {
  // bigBarChangeTest();
  realMain();
  return 0;
}
