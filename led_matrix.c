#include "led_matrix.h"
#include "gpio.h"
#include <string.h>

void setPixel(MatrixState_t *matrix, coord_t row, coord_t col,
              MatrixColor_t *color) {
  matrix->buffer[row][col] = *color;
}

void setRect(MatrixState_t *matrix, coord_t row_min, coord_t col_min,
             coord_t row_max, coord_t col_max, MatrixColor_t *color) {
  for (size_t row = row_min; row < row_max; row++) {
    for (size_t col = col_min; col < col_max; col++) {
      setPixel(matrix, row, col, color);
    }
  }
}

void initMatrixState(MatrixState_t *state) {
  setRect(state, 0, MATRIX_ROW_COUNT - 1, 0, MATRIX_COL_COUNT - 1, {0, 0, 0});
}

MatrixState_t *matrix_current_state = NULL :

    void
    beginMatrix(MatrixState_t * matrix) {
  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts */

  SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
  PIT->MCR = 0;                    // Enables timers
  PIT->CHANNEL[0].LDVAL =
      DEFAULT_SYSTEM_CLOCK / MATRIX_ISR_FREQ; // Set load value of zeroth PIT
  PIT->CHANNEL[0].TCTRL = 3;                  // Enable Timer and Interrupts
  matrix_current_state = matrix;
  matrix_current_state->row_pair = 0;
  matrix_current_state->cycle_index = 0;
}

/**
 * Effect: stops the timer and the updates
 */
void endMatrix(MatrixState_t *) {
  PIT->CHANNEL[0].TCTRL = 1; // Disable Timer and Interrupts
}

void PIT0_IRQHandler(void) {
  // Lower the output enable
  DIGITAL_WRITE_LOW(MATRIX_OE_LETTER, MATRIX_OE_PIN);
  // Lower the latch
  DIGITAL_WRITE_LOW(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);

  // Lower the clock
  DIGITAL_WRITE_LOW(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);

  // Iterate through the buffer
  DIGITAL_WRITE(MATRIX_A_LETTER, MATRIX_A_PIN,
                (matrix_current_state->row_pair >> 2) & 0x01);
  DIGITAL_WRITE(MATRIX_B_LETTER, MATRIX_B_PIN,
                (matrix_current_state->row_pair >> 1) & 0x01);
  DIGITAL_WRITE(MATRIX_C_LETTER, MATRIX_C_PIN,
                (matrix_current_state->row_pair >> 0) & 0x01);

  for (size_t col = 0; col < MATRIX_COL_COUNT; col++) {
    DIGITAL_WRITE_LOW(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);

    MatrixColor_t *top_color =
        matrix_current_state->buffer[matrix_current_state->row_pair][col];
    MatrixColor_t *bottom_color =
        matrix_current_state
            ->buffer[matrix_current_state->row_pair + ROW_PAIR_OFFSET][col];
    DIGITAL_WRITE(MATRIX_R1_LETTER, MATRIX_B1_PIN,
                  top_color->r / (256 / MATRIX_RESOLUTION) < cycle_index)
    DIGITAL_WRITE(MATRIX_G1_LETTER, MATRIX_B1_PIN,
                  top_color->g / (256 / MATRIX_RESOLUTION) < cycle_index)
    DIGITAL_WRITE(MATRIX_G1_LETTER, MATRIX_B1_PIN,
                  top_color->b / (256 / MATRIX_RESOLUTION) < cycle_index)
    DIGITAL_WRITE(MATRIX_R1_LETTER, MATRIX_B1_PIN,
                  bottom_color->r / (256 / MATRIX_RESOLUTION) < cycle_index)
    DIGITAL_WRITE(MATRIX_G1_LETTER, MATRIX_B1_PIN,
                  bottom_color->g / (256 / MATRIX_RESOLUTION) < cycle_index)
    DIGITAL_WRITE(MATRIX_G1_LETTER, MATRIX_B1_PIN,
                  bottom_color->b / (256 / MATRIX_RESOLUTION) < cycle_index)

    DIGITAL_WRITE_HIGH(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);
  }

  // Lower the output enable
  DIGITAL_WRITE_HIGH(MATRIX_OE_LETTER, MATRIX_OE_PIN);
  // Lower the latch
  DIGITAL_WRITE_HIGH(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);

  matrix_current_state->row_pair =
      (matrix_current_state->row_pair + 1) % ROW_PAIR_OFFSET;
  matrix_current_state->cycle_index =
      (matrix_current_state->cycle_index + 1) % MATRIX_RESOLUTION;
}
