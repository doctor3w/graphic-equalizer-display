#ifndef BUTTON_H
#define BUTTON_H

#include <fsl_device_registers.h>
#include <fsl_debug_console.h>

// The pin on the board that the button is plugged into
#define BUTTON_LETTER B
#define BUTTON_PIN 18

// Generates the name of the ISR function
#define BUTTON_ISR_ENABLE(LETTER) BUTTON_ISR_ENABLE_IMPL(LETTER)
#define BUTTON_ISR_ENABLE_IMPL(LETTER) PORT##LETTER##_IRQn
// Generates a function signature for ISR function
#define BUTTON_ISR(LETTER) BUTTON_ISR_IMPL(LETTER)
#define BUTTON_ISR_IMPL(LETTER) PORT##LETTER##_IRQHandler(void)
// Configures a pin as an input
#define DIGITAL_IN_ISR(LETTER, PIN) DIGITAL_IN_ISR_IMPL(LETTER, PIN)
#define DIGITAL_IN_ISR_IMPL(LETTER, PIN)                                       \
  SIM->SCGC5 |= SIM_SCGC5_PORT##LETTER##_MASK;                                 \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_MUX(1);                                   \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_PE(1);                                    \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_PS(0);                                    \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_IRQC(9);                                  \
  GPIO##LETTER##_PDDR &= ~(1 << PIN);
// Returns true if an ISR for the specified pin is triggered
#define DIGITAL_IN_ISR_TRIGGERED(LETTER, PIN)                                  \
  DIGITAL_IN_ISR_TRIGGERED_IMPL(LETTER, PIN)
#define DIGITAL_IN_ISR_TRIGGERED_IMPL(LETTER, PIN)                             \
  (PORT##LETTER->PCR[PIN] & PORT_PCR_ISF_MASK)
// Clears the ISR flag
#define DIGITAL_IN_CLEAR_ISR(LETTER, PIN) DIGITAL_IN_CLEAR_ISR_IMPL(LETTER, PIN)
#define DIGITAL_IN_CLEAR_ISR_IMPL(LETTER, PIN)                                 \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_ISF(1)

// A struct which holds the callback
typedef struct ButtonState { void (*callback)(void); } ButtonState_t;

// Initializes a button state struct
void initButtonState(ButtonState_t *state);

// Sets the callback for a button state
void setButtonCallback(ButtonState_t *state, void (*callback)(void));

// Starts the interrupts for the button
void beginButton(ButtonState_t *state);

// The ISR
void BUTTON_ISR(BUTTON_LETTER);

#endif
