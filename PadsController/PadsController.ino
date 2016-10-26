#include <Keyboard.h>
#include <CapacitiveSensor.h>

#define DEBUG 1
#define PIN_NUM 24
#define SEND_PIN 2
#define SENSOR_NUM 9
#define SENSOR_TIMEOUT 10
#define SENSOR_AUTOCAL 0xFFFFFFFF
#define SENSOR_SAMPLES 30
#define SENSOR_THRESHOLD 150


char pins[] = { 3, 4, 5, 6, 7, 8, 9, 10, 14 };
char keys[] = { 'a', 's', 'w', 'f', 'r', 'y', 'h', 'i', 'k' };
CapacitiveSensor *sensors[SENSOR_NUM];
long values[SENSOR_NUM];


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

#if DEBUG
    char buf[10];
    bool pressed = value >= SENSOR_THRESHOLD;
    sprintf(buf, "%c%4ld%c\t\t", pressed ? '[' : ' ', value, pressed ? ']' : ' ');
    Serial.print(buf);
#else
    if(values[i] < SENSOR_THRESHOLD && value >= SENSOR_THRESHOLD) {
      Keyboard.write(keys[i]);
    }
    values[i] = value;
#endif
  }

#if DEBUG
    Serial.print("\n");
#endif
}

