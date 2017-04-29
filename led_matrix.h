#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdio.h>

/**
 * Reprsents the type of a row or column index for the display
 */
typedef uint8_t coord_t;

// Dimension constants
const coord_t matrix_row_count = 16;
const coord_t matrix_col_count = 32;

/**
 * Represents the RGB color (r, g, b)
 * 24 bit color, 0-255 per component
 * 255, 255, 255 -> white
 * 0, 0, 0 -> black (off)
 */
typedef struct MatrixColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} MatrixColor_t;

typedef struct MatrixState {
  MatrixColor_t buffer[matrix_row_count][matrix_col_count];
} MatrixState_t;

/**
 * Effect: sets the pixel at (row, col) to be color
 * The bottom left is (0,0)
 */
void setPixel(MatrixState_t *matrix, coord_t row, coord_t col,
              MatrixColor_t *color);

/**
 * Effect: sets all pixels (row, col) such that row_min <= row <= row_max &&
 * col_min <= col <= col_max to color.
 */
void setRect(MatrixState_t *matrix, coord_t row_min, coord_t col_min,
             coord_t row_max, coord_t col_max, MatrixColor_t *color);

/**
 * Effect: sets all the colors to 0
 */
void initMatrixState(MatrixState_t *state);

/**
 * Effect: initializes the led matrix specified by matrix.
 * Starts in interrupt handler to continually write the buffer stored in matrix.
 * Assumes that the buffer is valid until endMatrix is called.
 */
void beginMatrix(MatrixState_t *matrix);

/**
 * Effect: stops the timer and the updates
 */
void endMatrix(MatrixState_t *)

#endif
