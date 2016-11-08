#include <BasicSerial3.h>

#define STATE_PIN 0
#define SEND_PIN 1
#define RECEIVE_PIN 2
#define SAMPLES 60
#define THRESHOLD 1800

#define MODE_INPUT(pin) DDRB &= ~_BV(pin)
#define MODE_OUTPUT(pin) DDRB |= _BV(pin)
#define READ(pin) ((PINB & _BV(pin)) ? 1 : 0)
#define WRITE_LOW(pin) (PORTB &= ~_BV(pin))
#define WRITE_HIGH(pin) (PORTB |= _BV(pin))


int cnt;

void setup() {
  noInterrupts();
  MODE_OUTPUT(STATE_PIN);   // state pin : OUTPUT LOW
  MODE_OUTPUT(SEND_PIN);    // send pin : OUTPUT LOW
  WRITE_HIGH(3);            // PB3 : INPUT_PULLUP
  WRITE_HIGH(4);            // PB4 : INPUT_PULLUP
  WRITE_HIGH(5);            // PB5 : INPUT_PULLUP
  interrupts();
}

void loop() {
  cnt = 0;
  for(int i = 0; i < SAMPLES; i++) {
    sense();
  }

  if(cnt >= THRESHOLD) {
    WRITE_HIGH(STATE_PIN);  // state pin : OUTPUT HIGH
  }
  else {
    WRITE_LOW(STATE_PIN);   // state pin : OUTPUT LOW
  }
}

void sense() {
  noInterrupts();
  WRITE_LOW(RECEIVE_PIN);   // receive pin : INPUT
  MODE_OUTPUT(RECEIVE_PIN); // receive pin : OUTPUT LOW
  MODE_INPUT(RECEIVE_PIN);  // receive pin : INPUT
  WRITE_HIGH(SEND_PIN);     // send pin : OUTPUT HIGH
  interrupts();

  while(!READ(RECEIVE_PIN)) {
    cnt++;
  }

  noInterrupts();
  WRITE_HIGH(RECEIVE_PIN);  // receive pin : INPUT_PULLUP
  MODE_OUTPUT(RECEIVE_PIN); // receive pin : OUTPUT HIGH
  MODE_INPUT(RECEIVE_PIN);  // receive pin : INPUT_PULLUP
  WRITE_LOW(RECEIVE_PIN);   // receive pin : INPUT
  WRITE_LOW(SEND_PIN);      // send pin : OUTPUT LOW
  interrupts();

  while(READ(RECEIVE_PIN)) {
    cnt++;
  }
}

