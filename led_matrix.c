#include "led_matrix.h"
#include "gpio.h"
#include <string.h>

uint8_t extractR(MatrixColor_t *thing) {
  // return ((thing->thing >> 0) & 0x3) << 6;
  return thing->r;
}

uint8_t extractG(MatrixColor_t *thing) {
  // return ((thing->thing >> 2) & 0x3) << 6;
  return thing->g;
}

uint8_t extractB(MatrixColor_t *thing) {
  // return ((thing->thing >> 4) & 0x3) << 6;
  return thing->b;
}

void createColor(uint8_t r, uint8_t g, uint8_t b, MatrixColor_t *thing) {
  // thing->thing = ((r >> 6) << 0) | ((g >> 6) << 2) | ((b >> 6) << 4);
  thing->r = r;
  thing->g = g;
  thing->b = b;
}

void setPixel(MatrixState_t *matrix, coord_t row, coord_t col,
              const MatrixColor_t *color) {
  matrix->buffer[MATRIX_ROW_COUNT - 1 - row][col] = *color;
}

void setRect(MatrixState_t *matrix, coord_t row_min, coord_t col_min,
             coord_t row_max, coord_t col_max, const MatrixColor_t *color) {
  for (size_t row = row_min; row <= row_max; row++) {
    for (size_t col = col_min; col <= col_max; col++) {
      setPixel(matrix, row, col, color);
    }
  }
}

void initMatrixState(MatrixState_t *state) {
  MatrixColor_t foo;
  createColor(0, 0, 0, &foo);
  setRect(state, 0, 0, MATRIX_ROW_COUNT - 1, MATRIX_COL_COUNT - 1, &foo);
}

volatile MatrixState_t *matrix_current_state = NULL;

void beginMatrix(MatrixState_t *matrix) {
  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts */

  DIGITAL_OUT(MATRIX_A_LETTER, MATRIX_A_PIN);
  DIGITAL_OUT(MATRIX_B_LETTER, MATRIX_B_PIN);
  DIGITAL_OUT(MATRIX_C_LETTER, MATRIX_C_PIN);
  DIGITAL_OUT(MATRIX_R1_LETTER, MATRIX_R1_PIN);
  DIGITAL_OUT(MATRIX_R2_LETTER, MATRIX_R2_PIN);
  DIGITAL_OUT(MATRIX_G1_LETTER, MATRIX_G1_PIN);
  DIGITAL_OUT(MATRIX_G2_LETTER, MATRIX_G2_PIN);
  DIGITAL_OUT(MATRIX_B1_LETTER, MATRIX_B1_PIN);
  DIGITAL_OUT(MATRIX_B2_LETTER, MATRIX_B2_PIN);
  DIGITAL_OUT(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);
  DIGITAL_OUT(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);
  DIGITAL_OUT(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);
  DIGITAL_OUT(MATRIX_OE_LETTER, MATRIX_OE_PIN);

  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
  PIT->CHANNEL[0].LDVAL =
      DEFAULT_SYSTEM_CLOCK / MATRIX_ISR_FREQ; // Set load value of zeroth PIT
  PIT->CHANNEL[0].TCTRL = 3;                  // Enable Timer and Interrupts
  PIT->MCR = 0;                               // Enables timers
  matrix_current_state = matrix;
  matrix_current_state->row_pair = 0;
  matrix_current_state->cycle_index = 0;
}

/**
 * Effect: stops the timer and the updates
 */
void endMatrix(MatrixState_t *state) {
  PIT->CHANNEL[0].TCTRL = 1; // Disable Timer and Interrupts
}

uint32_t foobarcow = 0;

void PIT0_IRQHandler(void) {
  PIT->CHANNEL[0].TFLG = 1;

  DIGITAL_WRITE_LOW(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);

  coord_t row = matrix_current_state->row_pair;
  DIGITAL_WRITE(MATRIX_A_LETTER, MATRIX_A_PIN, ((row + 6) % 8 >> 0) & 0x01);
  DIGITAL_WRITE(MATRIX_B_LETTER, MATRIX_B_PIN, ((row + 6) % 8 >> 1) & 0x01);
  DIGITAL_WRITE(MATRIX_C_LETTER, MATRIX_C_PIN, ((row + 6) % 8 >> 2) & 0x01);
  DIGITAL_WRITE_LOW(MATRIX_OE_LETTER, MATRIX_OE_PIN);
  DIGITAL_WRITE(MATRIX_A_LETTER, MATRIX_A_PIN, ((row + 7) % 8 >> 0) & 0x01);
  DIGITAL_WRITE(MATRIX_B_LETTER, MATRIX_B_PIN, ((row + 7) % 8 >> 1) & 0x01);
  DIGITAL_WRITE(MATRIX_C_LETTER, MATRIX_C_PIN, ((row + 7) % 8 >> 2) & 0x01);
  DIGITAL_WRITE_LOW(MATRIX_OE_LETTER, MATRIX_OE_PIN);

  coord_t cycle_index = matrix_current_state->cycle_index / ROW_PAIR_OFFSET;

  for (size_t col = 0; col < MATRIX_COL_COUNT; col++) {
    DIGITAL_WRITE_LOW(MATRIX_R1_LETTER, MATRIX_R1_PIN);
    DIGITAL_WRITE_LOW(MATRIX_G1_LETTER, MATRIX_G1_PIN);
    DIGITAL_WRITE_LOW(MATRIX_B1_LETTER, MATRIX_B1_PIN);
    DIGITAL_WRITE_LOW(MATRIX_R2_LETTER, MATRIX_R2_PIN);
    DIGITAL_WRITE_LOW(MATRIX_G2_LETTER, MATRIX_G2_PIN);
    DIGITAL_WRITE_LOW(MATRIX_B2_LETTER, MATRIX_B2_PIN);

    MatrixColor_t top_color = matrix_current_state->buffer[row][col];
    MatrixColor_t bottom_color =
        matrix_current_state->buffer[row + ROW_PAIR_OFFSET][col];

    uint8_t tr = extractR(&top_color);
    uint8_t tg = extractG(&top_color);
    uint8_t tb = extractB(&top_color);
    uint8_t br = extractR(&bottom_color);
    uint8_t bg = extractG(&bottom_color);
    uint8_t bb = extractB(&bottom_color);

    DIGITAL_WRITE(MATRIX_R1_LETTER, MATRIX_R1_PIN,
                  (tr / (MATRIX_RESOLUTION - 1) > cycle_index));
    DIGITAL_WRITE(MATRIX_G1_LETTER, MATRIX_G1_PIN,
                  (tg / (MATRIX_RESOLUTION - 1) > cycle_index));
    DIGITAL_WRITE(MATRIX_B1_LETTER, MATRIX_B1_PIN,
                  (tb / (MATRIX_RESOLUTION - 1) > cycle_index));
    DIGITAL_WRITE(MATRIX_R2_LETTER, MATRIX_R2_PIN,
                  (br / (MATRIX_RESOLUTION - 1) > cycle_index));
    DIGITAL_WRITE(MATRIX_G2_LETTER, MATRIX_G2_PIN,
                  (bg / (MATRIX_RESOLUTION - 1) > cycle_index));
    DIGITAL_WRITE(MATRIX_B2_LETTER, MATRIX_B2_PIN,
                  (bb / (MATRIX_RESOLUTION - 1) > cycle_index));

    DIGITAL_WRITE_HIGH(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);
    DIGITAL_WRITE_LOW(MATRIX_CLK_LETTER, MATRIX_CLK_PIN);
  }

  DIGITAL_WRITE_HIGH(MATRIX_OE_LETTER, MATRIX_OE_PIN);
  DIGITAL_WRITE_HIGH(MATRIX_LAT_LETTER, MATRIX_LAT_PIN);

  matrix_current_state->row_pair =
      (matrix_current_state->row_pair + 1) % ROW_PAIR_OFFSET;
  matrix_current_state->cycle_index =
      (matrix_current_state->cycle_index + 1) %
      (256 / MATRIX_RESOLUTION * ROW_PAIR_OFFSET);
}
