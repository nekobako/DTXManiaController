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
    int count = digitalRead(pins[i]) ? min(counts[i] + 1, SWITCH_THRESHOLD) : 0;

    if(counts[i] != SWITCH_THRESHOLD && count == SWITCH_THRESHOLD) {
      Keyboard.press(keys[i]);
    }
    else if(counts[i] == SWITCH_THRESHOLD && count != SWITCH_THRESHOLD) {
      Keyboard.release(keys[i]);
    }

    counts[i] = count;
  }
}

