#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_LETTER A
#define BUTTON_PIN 1

#define BUTTON_ISR(LETTER) PORT##LETTER##_IRQHandler()
#define DIGITAL_IN_ISR(LETTER, PIN) DIGITAL_IN_ISR_IMPL(LETTER, PIN)
#define DIGITAL_IN_ISR_IMPL(LETTER, PIN)                                       \
  SIM->SCGC5 |= SIM_SCGC5_PORT##LETTER##_MASK;                                 \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_MUX(0x01);                                \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_PE(1);                                    \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_PS(0);                                    \
  PORT##LETTER->PCR[PIN] |= PORT_PCR_IRQC(0x0001);                             \
  PT##LETTER->PDDR &= ~(1 << PIN);
#define DIGITAL_IN_ISR_TRIGGERED(LETTER, PIN)                                  \
  DIGITAL_IN_ISR_TRIGGERED_IMPL(LETTER, PIN)
#define DIGITAL_IN_ISR_TRIGGERED_IMPL(LETTER, PIN)                             \
  ((PORT##LETTER->PCR[PIN] >> 24) & 0x1)
#define DIGITAL_IN_CLEAR_ISR(LETTER, PIN) DIGITAL_IN_CLEAR_ISR_IMPL(LETTER, PIN)
#define DIGITAL_IN_CLEAR_ISR_IMPL(LETTER, PIN)                                 \
  PORT##LETTER->PCR[PIN] &= ~(1 << 24)

typedef struct ButtonState { void (*callback)(void); } ButtonState_t;

void initButtonState(ButtonState_t *state);

void setButtonCallback(ButtonState_t *state, void (*callback)(void));

void beginButton(ButtonState_t *state);

void BUTTON_ISR(BUTTON_LETTER);

#endif
