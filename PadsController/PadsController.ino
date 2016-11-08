#include <Keyboard.h>

#define DEBUG 0
#define PIN_NUM 24
#define SENSOR_NUM 9


char pins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
char keys[] = { 'a', 's', 'w', 'f', 'r', 'y', 'h', 'i', 'k' };
bool states[SENSOR_NUM];

void setup() {
  for(int i = 0; i < PIN_NUM; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for(int i = 0; i < SENSOR_NUM; i++) {
    pinMode(pins[i], INPUT);
  }

#if DEBUG
  Serial.begin(9600);
#else
  Keyboard.begin();
#endif
}


void loop() {
  for(int i = 0; i < SENSOR_NUM; i++) {
    bool state = digitalRead(pins[i]);

#if DEBUG
    if(!states[i] && state) {
      Serial.write('[');
      Serial.write(keys[i]);
    }
    else if(states[i] && !state) {
      Serial.write(keys[i]);
      Serial.write(']');
    }
#else
    if(!states[i] && state) {
      Keyboard.press(keys[i]);
    }
    else if(states[i] && !state) {
      Keyboard.release(keys[i]);
    }
#endif

    states[i] = state;
  }
}

