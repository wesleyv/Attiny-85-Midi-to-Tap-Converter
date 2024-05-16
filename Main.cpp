#include "Main.h"
#include <Arduino.h>
#include "MidiParse.h"
#include <SoftwareSerial.h>


#define MIDIRX 2
#define MIDICH 9

#define AUXTIP 4
#define AUXRING 3
#define MAINSWITCH 0
#define MIDISTATUS 11

SoftwareSerial midiSerial(MIDIRX, -1);
MidiParser parser;

void onControlChange(uint8_t channel, uint8_t data0, uint8_t data1) {

}

void initializePin(uint8_t pin) {
    pinMode (pin, OUTPUT);
    digitalWrite (pin, LOW);
}

void MainSetup() {
    midiSerial.begin (31250); // MIDI Baud rate
    initializePin(MIDISTATUS);
    initializePin(AUXTIP);
    initializePin(AUXRING);
    initializePin(MAINSWITCH);

    parser = MidiParserInit(&onControlChange);

}

void MainLoop() {
    if (!midiSerial.available()) {
        return;
    }
    uint8_t b = midiSerial.read();

    MidiParserParse(&parser, b);
}