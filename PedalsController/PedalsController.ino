#include <Keyboard.h>
#include <CapacitiveSensor.h>

#define DEBUG 0
#define PIN_NUM 24
#define SEND_PIN 2
#define SENSOR_NUM 2
#define SENSOR_TIMEOUT 10
#define SENSOR_AUTOCAL 100
#define SENSOR_SAMPLES 30
#define SENSOR_THRESHOLD 100


char pins[] = { 3, 4 };
char keys[] = { 'v', 'b' };
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
    char buf[9];
    sprintf(buf, value >= SENSOR_THRESHOLD ? "[%4ld]\t\t" : " %4ld \t\t", value);
    Serial.print(buf);
#else
    if(values[i] < SENSOR_THRESHOLD && value >= SENSOR_THRESHOLD) {
      Keyboard.write(keys[i]);
    }
#endif

    values[i] = value;
  }

#if DEBUG
    Serial.print("\n");
#endif
}

