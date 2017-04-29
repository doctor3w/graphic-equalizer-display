#ifndef GPIO_H
#define GPIO_H

typedef uint8_t pin_count_t;
typedef uint16_t pin_id_t;

#define DIGITAL_OUT(LETTER, PIN)               \
  SIM->SCGC5 |= SIM_SCGC5_PORT##LETTER##_MASK; \
  PORT##LETTER->PCR[PIN] = PORT_PCR_MUX(0x01); \
  PT##LETTER->PDDR |= (1 << PIN);

#define DIGITAL_WRITE_HIGH(LETTER, PIN) PT##LETTER->PTOR = (1 << PIN);
#define DIGITAL_WRITE_LOW(LETTER, PIN) PT##LETTER->PCOR = (1 << PIN);
#define DIGITAL_WRITE(LETTER, PIN, STATE)   \
  if (STATE)                                \
    DIGITAL_WRITE_HIGH(LETTER, PIN, STATE); \
  else                                      \
    DIGITAL_WRITE_LOW(LETTER, PIN, STATE);

#endif
