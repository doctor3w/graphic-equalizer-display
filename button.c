#include "button.h"

void initButtonState(ButtonState_t *state) { state->callback = NULL; }

void setButtonCallback(ButtonState_t *state, void (*callback)(void)) {
  state->callback = callback;
}

volatile ButtonState_t *current_button_state = NULL;

size_t pin = 18;

void beginButton(ButtonState_t *state) {
  // configure the pin as an input and set up the interrupt
  NVIC_EnableIRQ(BUTTON_ISR_ENABLE(BUTTON_LETTER));
  DIGITAL_IN_ISR(BUTTON_LETTER, BUTTON_PIN)
  current_button_state = state;
}

void BUTTON_ISR(BUTTON_LETTER) {
  if (DIGITAL_IN_ISR_TRIGGERED(BUTTON_LETTER, BUTTON_PIN)) {
    DIGITAL_IN_CLEAR_ISR(BUTTON_LETTER, BUTTON_PIN);
    if (current_button_state != NULL &&
        current_button_state->callback != NULL) {
      current_button_state->callback();
    }
  }
}
