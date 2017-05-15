#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include <stdio.h>

// Row select
#define MATRIX_A_LETTER C
#define MATRIX_A_PIN 3
#define MATRIX_B_LETTER C
#define MATRIX_B_PIN 2
#define MATRIX_C_LETTER A
#define MATRIX_C_PIN 2
// Red
#define MATRIX_R1_LETTER D
#define MATRIX_R1_PIN 1
#define MATRIX_R2_LETTER D
#define MATRIX_R2_PIN 3
// Green
#define MATRIX_G1_LETTER C
#define MATRIX_G1_PIN 4
#define MATRIX_G2_LETTER C
#define MATRIX_G2_PIN 12
// Blue
#define MATRIX_B1_LETTER D
#define MATRIX_B1_PIN 2
#define MATRIX_B2_LETTER D
#define MATRIX_B2_PIN 0
// Latching, output enable, clk
#define MATRIX_LAT_LETTER B
#define MATRIX_LAT_PIN 23
#define MATRIX_CLK_LETTER A
#define MATRIX_CLK_PIN 1
#define MATRIX_OE_LETTER B
#define MATRIX_OE_PIN 9

// Dimension constants
#define MATRIX_ROW_COUNT 16
#define MATRIX_COL_COUNT 32
#define MATRIX_ISR_FREQ 1000
#define MATRIX_RESOLUTION 128
#define ROW_PAIR_OFFSET 8

/**
 * Reprsents the type of a row or column index for the display
 */
typedef uint8_t coord_t;

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

inline uint8_t extractR(MatrixColor_t *thing);

inline uint8_t extractG(MatrixColor_t *thing);

inline uint8_t extractB(MatrixColor_t *thing);

inline void createColor(uint8_t r, uint8_t g, uint8_t b, MatrixColor_t *thing);

typedef struct MatrixState {
  MatrixColor_t buffer[MATRIX_ROW_COUNT][MATRIX_COL_COUNT];
  coord_t row_pair;
  coord_t cycle_index;
} MatrixState_t;

/**
 * Effect: sets the pixel at (row, col) to be color
 * The bottom left is (0,0)
 */
void setPixel(MatrixState_t *matrix, coord_t row, coord_t col,
              const MatrixColor_t *color);

/**
 * Effect: sets all pixels (row, col) such that row_min <= row <= row_max &&
 * col_min <= col <= col_max to color.
 */
void setRect(MatrixState_t *matrix, coord_t row_min, coord_t col_min,
             coord_t row_max, coord_t col_max, const MatrixColor_t *color);

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
void endMatrix(MatrixState_t *);

// ISR for updating display
void PIT0_IRQHandler(void);

#endif
