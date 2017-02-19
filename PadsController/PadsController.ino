#include <MIDIUSB.h>

#define DEBUG 0
#define PIN_NUM 24
#define SENSOR_NUM 9
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80


char pins[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
char notes[] = { 49, 42, 46, 38, 50, 47, 43, 57, 51 };
char *names[] = { "LC", "HC", "HO", "SD", "HT", "LT", "FT", "RC", "RD" };
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
#endif
}

void loop() {
  for(int i = 0; i < SENSOR_NUM; i++) {
    bool state = digitalRead(pins[i]);

#if DEBUG
    if(!states[i] && state) {
      Serial.write(names[i]);
      Serial.write(" : ON\n");
    }
    else if(states[i] && !state) {
      Serial.write(names[i]);
      Serial.write(" : OFF\n");
    }
#else
    if(!states[i] && state) {
      sendMIDI(MIDI_NOTE_ON, notes[i]);
    }
    else if(states[i] && !state) {
      sendMIDI(MIDI_NOTE_OFF, notes[i]);
    }
#endif

    states[i] = state;
  }
}

void sendMIDI(byte type, byte pitch) {
  midiEventPacket_t event = {type >> 4, type | 0, pitch, 64};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

