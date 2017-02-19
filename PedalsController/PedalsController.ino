#include <MIDIUSB.h>

#define DEBUG 0
#define PIN_NUM 24
#define SWITCH_NUM 2
#define SWITCH_THRESHOLD 4
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80


char pins[] = { 2, 3 };
char notes[] = { 44, 36 };
char *names[] = { "LP", "RP" };
int counts[SWITCH_NUM];

void setup() {
  for(int i = 0; i < PIN_NUM; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for(int i = 0; i < SWITCH_NUM; i++) {
    pinMode(pins[i], INPUT);
  }

#if DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  for(int i = 0; i < SWITCH_NUM; i++) {
    int count = digitalRead(pins[i]) ? min(counts[i] + 1, SWITCH_THRESHOLD) : 0;

#if DEBUG
    if(counts[i] != SWITCH_THRESHOLD && count == SWITCH_THRESHOLD) {
      Serial.write(names[i]);
      Serial.write(" : ON\n");
    }
    else if(counts[i] == SWITCH_THRESHOLD && count != SWITCH_THRESHOLD) {
      Serial.write(names[i]);
      Serial.write(" : OFF\n");
    }
#else
    if(counts[i] != SWITCH_THRESHOLD && count == SWITCH_THRESHOLD) {
      sendMIDI(MIDI_NOTE_ON, notes[i]);
    }
    else if(counts[i] == SWITCH_THRESHOLD && count != SWITCH_THRESHOLD) {
      sendMIDI(MIDI_NOTE_OFF, notes[i]);
    }
#endif

    counts[i] = count;
  }
}

void sendMIDI(byte type, byte pitch) {
  midiEventPacket_t event = {type >> 4, type | 0, pitch, 64};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

