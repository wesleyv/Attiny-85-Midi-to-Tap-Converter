#include "Main.h"
#include "MidiParse.h"
#include "Pins.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MIDIRX 2

#define MIDICH 9

#define AUXTIP 4
#define AUXRING 3
#define MAINSWITCH 0
#define MIDISTATUS 11

#define AUXTIPCC 88
#define AUXRINGCC 89
#define MAINSWITCHCC 90

uint32_t now = 0;

SoftwareSerial midiSerial(MIDIRX, -1);
MidiParser parser;

void onControlChange(uint8_t channel, uint8_t data0, uint8_t data1)
{
    Serial.println("Control Change Detected ");
    if (channel != MIDICH)
    {
        return;
    }
    if (data0 == AUXTIPCC && data1 == 0)
    {
        PinOn(AUXTIP, 1, now);
        PinOn(MIDISTATUS, 0, now);
    }
    else if (data0 == AUXRINGCC && data1 == 0)
    {
        PinOn(AUXRING, 2, now);
        PinOn(MIDISTATUS, 0, now);
    }
    else if (data0 == MAINSWITCHCC && data1 == 0)
    {
        PinOn(MAINSWITCH, 3, now);
        PinOn(MIDISTATUS, 0, now);
    }
}

void onError(MidiParserState state, uint8_t b)
{
    Serial.print("Failed to parse byte: ");
    Serial.print(b);
    Serial.print(" in state: ");
    Serial.println(state);
}

void MainSetup()
{
    Serial.print("Main Setup ");
    midiSerial.begin(31250); // MIDI Baud rate
    initializePin(MIDISTATUS);
    initializePin(AUXTIP);
    initializePin(AUXRING);
    initializePin(MAINSWITCH);

    parser = MidiParserInit(&onControlChange, &onError);
}

void MainLoop()
{
    if (!midiSerial.available())
    {
        return;
    }
    uint8_t b = midiSerial.read();
    Serial.print("midi serial detected ");
    Serial.println(b);
    MidiParserParse(&parser, b);
    now = micros();
    PinOff(MIDISTATUS, 0, now);
    PinOff(AUXTIP, 1, now);
    PinOff(AUXRING, 2, now);
    PinOff(MAINSWITCH, 3, now);
}
