#include "MidiParse.h"
#include <SoftwareSerial.h>

#define MIDIRX 2
#define MIDICH 9

#define AUXTIP 4
#define AUXRING 3
#define MAINSWITCH 0
#define MIDISTATUS 11

SoftwareSerial midiSerial(MIDIRX, -1);

void setup() {
    midiSerial.begin (31250); // MIDI Baud rate
    initializePin(MIDISTATUS);
    initializePin(AUXTIP);
    initializePin(AUXRING);
    initializePin(MAINSWITCH);
}
void loop() {}

void initializePin(uint8_t pin) {
    pinMode (pin, OUTPUT);
    digitalWrite (pin, LOW);
}