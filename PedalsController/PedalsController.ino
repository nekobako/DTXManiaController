#include <Keyboard.h>

#define DEBUG 0
#define PIN_NUM 24
#define SWITCH_NUM 2
#define SWITCH_THRESHOLD 4


char pins[] = { 2, 3 };
char keys[] = { 'v', 'b' };
int counts[SWITCH_NUM];


void setup() {
  for(int i = 0; i < PIN_NUM; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  for(int i = 0; i < SWITCH_NUM; i++) {
    pinMode(pins[i], INPUT);
  }

  Keyboard.begin();
}


void loop() {
  for(int i = 0; i < SWITCH_NUM; i++) {
    counts[i] = digitalRead(pins[i]) ? min(counts[i], SWITCH_THRESHOLD) + 1 : 0;

    if(counts[i] == SWITCH_THRESHOLD) {
      Keyboard.write(keys[i]);
    }
  }
}

