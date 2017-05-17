#include "color_utils.h"

/*******************************************************************************
 *   r, g, b values are from 0..255
 *   h = [0,360], s = [0,255], v = [0,255]
 *   modified from: https://www.ruinelli.ch/rgb-to-hsv
 ******************************************************************************/
void HSV2RGB(uint16_t hi, uint8_t si, uint8_t vi, uint8_t *ro, uint8_t *go,
             uint8_t *bo) {
  int i;
  float f, p, q, t, h, s, v;

  h = (float)hi;
  s = (float)si;
  v = (float)vi;

  s /= 255;

  if (s == 0) { // achromatic (grey)
    *ro = v;
    *go = v;
    *bo = v;
    return;
  }

  h /= 60; // sector 0 to 5
  i = (int)h;
  f = h - i; // factorial part of h
  p = (unsigned char)(v * (1 - s));
  q = (unsigned char)(v * (1 - s * f));
  t = (unsigned char)(v * (1 - s * (1 - f)));

  switch (i) {
  case 0:
    *ro = v;
    *go = t;
    *bo = p;
    break;
  case 1:
    *ro = q;
    *go = v;
    *bo = p;
    break;
  case 2:
    *ro = p;
    *go = v;
    *bo = t;
    break;
  case 3:
    *ro = p;
    *go = q;
    *bo = v;
    break;
  case 4:
    *ro = t;
    *go = p;
    *bo = v;
    break;
  default: // case 5:
    *ro = v;
    *go = p;
    *bo = q;
    break;
  }
}

float color_map(uint16_t in, uint16_t in_min, uint16_t in_max, uint16_t out_min,
                uint16_t out_max) {
  float i = in;
  return (i - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
