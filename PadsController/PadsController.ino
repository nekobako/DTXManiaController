#include <Keyboard.h>
#include <CapacitiveSensor.h>

#define DEBUG 0
#define PIN_NUM 24
#define SEND_PIN 2
#define SENSOR_NUM 9
#define SENSOR_TIMEOUT 10
#define SENSOR_AUTOCAL 100
#define SENSOR_SAMPLES 30
#define SENSOR_THRESHOLD_TOUCH_DOWN 100
#define SENSOR_THRESHOLD_TOUCH_UP 50


char pins[] = { 3, 4, 5, 6, 7, 8, 9, 10, 14 };
char keys[] = { 'a', 's', 'w', 'f', 'r', 'y', 'h', 'i', 'k' };
CapacitiveSensor *sensors[SENSOR_NUM];
bool states[SENSOR_NUM];

void setup() {
  for(int i = 0; i < PIN_NUM; i++) {
    pinMode(i, INPUT_PULLUP);
  }

#if DEBUG
  Serial.begin(9600);
#else
  Keyboard.begin();
#endif

  for(int i = 0; i < SENSOR_NUM; i++) {
    sensors[i] = new CapacitiveSensor(SEND_PIN, pins[i]);
    sensors[i]->set_CS_Timeout_Millis(SENSOR_TIMEOUT);
    sensors[i]->set_CS_AutocaL_Millis(SENSOR_AUTOCAL);
  }
}


void loop() {
  for(int i = 0; i < SENSOR_NUM; i++) {
    long value = sensors[i]->capacitiveSensor(SENSOR_SAMPLES);
    bool state = states[i] ? value > SENSOR_THRESHOLD_TOUCH_UP : value >= SENSOR_THRESHOLD_TOUCH_DOWN;

#if DEBUG
    char buf[9];
    sprintf(buf, state ? "[%4ld]\t\t" : " %4ld \t\t", value);
    Serial.print(buf);
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

#if DEBUG
    Serial.print("\n");
#endif
}

