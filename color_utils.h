#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <stdint.h>

/*******************************************************************************
 *   r, g, b values are from 0..255
 *   h = [0,360], s = [0,255], v = [0,255]
 *   modified from: https://www.ruinelli.ch/rgb-to-hsv
 ******************************************************************************/
void HSV2RGB(uint16_t hi, uint8_t si, uint8_t vi, uint8_t *ro, uint8_t *go,
             uint8_t *bo);

float color_map(uint16_t in, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif
