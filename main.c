#include "button.h"
#include "graphic_equalizer.h"
#include "msgeq7.h"
#include <board.h>
#include <fsl_debug_console.h>
#include "color_utils.h"

const uint16_t MAX_ITERS = 2000;
volatile uint16_t loop_iters = 0;
volatile uint8_t color_scheme = 0;
const uint8_t COLOR_SCHEME_COUNT = 8;
const uint8_t GRAD_COUNT = 5;

void buttonCallback() {
  if (loop_iters == MAX_ITERS) {
	color_scheme++;
    color_scheme %= (COLOR_SCHEME_COUNT + GRAD_COUNT);
	loop_iters = 0;
  }
}

void run_eq_display() {
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

  MatrixColor_t *color_schemes[][7] = {
      {&blue, &blue, &blue, &blue, &blue, &blue, &blue},
	  {&red, &red, &red, &red, &red, &red, &red},
	  {&green, &green, &green, &green, &green, &green, &green},
	  {&purple, &purple, &purple, &purple, &purple, &purple, &purple},
	  {&yellow, &yellow, &yellow, &yellow, &yellow, &yellow, &yellow},
	  {&cyan, &cyan, &cyan, &cyan, &cyan, &cyan, &cyan},
	  {&white, &white, &white, &white, &white, &white, &white},
      {&red, &green, &blue, &purple, &yellow, &cyan, &white}};
  
  // {a, b, c, d} remaps 0-360 to a-b, then multiplies it by -1 if d==1 and 1 if d==0. Then adds c.
  uint16_t grad_schemes[][6] = {
	  {0, 240, 230, 1}, // really cool
	  {240, 360, 0, 0}, // Blue spice
	  {0, 360, 0, 0}, // rainbow
	  {220, 320, 0, 0}, // coral
	  {0, 70, 60, 1} // extra spicy
  };

  while (1) {
    for (size_t i = 0; i < 7; i++) {
      uint16_t data = chipState.data[i];
      coord_t height = 15 * data / 0xFFF;
	  MatrixColor_t barColor;
	  if (color_scheme < GRAD_COUNT) {
		uint16_t hue = height * 360 / 15;
		  float conv = grad_schemes[color_scheme][2] + color_map(hue, 0, 360, grad_schemes[color_scheme][0], grad_schemes[color_scheme][1]) * (grad_schemes[color_scheme][3] == 1 ? -1 : 1);
		if (conv < 0) conv += 360; 
		hue = (uint16_t)conv;
		uint8_t r, g, b;
		HSV2RGB(hue, 255, 255, &r, &g, &b);
		createColor(r, g, b, &barColor);
	  } else {
	    barColor = *color_schemes[color_scheme - GRAD_COUNT][i];
	  }
      setBarHeight(&state, i, height, &barColor);
    }
	if (loop_iters < MAX_ITERS)
      loop_iters++;
  }
}

int main(int count, char **args) {
  hardware_init();
  run_eq_display();
  return 0;
}
