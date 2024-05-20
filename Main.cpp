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
#define MIDISTATUS 11 // 11 for teensy, 1 for tiny

#define AUXTIPCC 88
#define AUXRINGCC 89
#define MAINSWITCHCC 90

uint32_t now = 0;

SoftwareSerial midiSerial(MIDIRX, -1);
MidiParser parser;

void onControlChange(uint8_t channel, uint8_t data0, uint8_t data1)
{
    // Serial.println("Control Change Detected ");
    if (channel != MIDICH)
    {
        return;
    }
    if (data0 == AUXTIPCC && data1 == 0)
    {
        PinOn(AUXTIP, 1, now);
        PinOn(MIDISTATUS, 0, now);
        Serial.println("=-=-=-=-=Aux Tip Pulsed-=-=-=-=-");
        // Serial.print(now);
    }
    else if (data0 == AUXRINGCC && data1 == 0)
    {
        PinOn(AUXRING, 2, now);
        PinOn(MIDISTATUS, 0, now);
        Serial.println("-=-=-=-=-Aux Ring Pulsed-=-=-=-=");
        // Serial.print(now);
    }
    else if (data0 == MAINSWITCHCC && data1 == 0)
    {
        PinOn(MAINSWITCH, 3, now);
        PinOn(MIDISTATUS, 0, now);
        Serial.println("-=-=-=-=-=Main Switch Pulsed=-=-=-=--=-");
        // Serial.print(now);
    }
}

void printBin(uint8_t aByte)
{
    for (int8_t aBit = 7; aBit >= 0; aBit--)
        Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

void onError(MidiParserState state, uint8_t b)
{
    Serial.print("!!!!!!!!!!!!!!!!!!!!Failed to parse byte: ");
    // printBin(b);
    // Serial.print(" in state: ");
    // Serial.println(state);
    // No Op
}

void MainSetup()
{
    // Serial.print("Main Setup ");
    midiSerial.begin(31250); // MIDI Baud rate
    initializePin(MIDISTATUS);
    initializePin(AUXTIP);
    initializePin(AUXRING);
    initializePin(MAINSWITCH);

    parser = MidiParserInit(&onControlChange, &onError);
}

void MainLoop()
{
    now = micros();
    PinOff(MIDISTATUS, 0, now);
    PinOff(AUXTIP, 1, now);
    PinOff(AUXRING, 2, now);
    PinOff(MAINSWITCH, 3, now);
    if (!midiSerial.available())
    {
        return;
    }
    uint8_t b = midiSerial.read();
//    Serial.println("midi serial detected ");
    printBin(b);
    Serial.println("");
    MidiParserParse(&parser, b);
}
