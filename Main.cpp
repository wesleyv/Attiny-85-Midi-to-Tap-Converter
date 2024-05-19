#include "Main.h"
#include "MidiParse.h"

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

#define TRIGPULSE 10000

uint32_t MicrosArray[4];

SoftwareSerial midiSerial(MIDIRX, -1);
MidiParser parser;

void PinOn(uint8_t pin, uint8_t microsarrayaddress)
{
    digitalWrite(pin, HIGH);
    MicrosArray[microsarrayaddress] = micros() + TRIGPULSE;
}

void PinOff(uint8_t pin, uint8_t microsarrayaddress)
{
    if (MicrosArray[microsarrayaddress] < micros())
    {
        digitalWrite(pin, LOW);
    }
}

void onControlChange(uint8_t channel, uint8_t data0, uint8_t data1)
{
    if (channel == MIDICH)
    {
        if (data0 == AUXTIPCC && data1 == 0)
        {

            PinOn(AUXTIP, 1);
            PinOn(MIDISTATUS, 0);
        }
        if (data0 == AUXRINGCC && data1 == 0)
        {
            PinOn(AUXRING, 2);
            PinOn(MIDISTATUS, 0);
        }
        if (data0 == MAINSWITCHCC && data1 == 0)
        {
            PinOn(MAINSWITCH, 3);
            PinOn(MIDISTATUS, 0);
        }
    }
}

void initializePin(uint8_t pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void MainSetup()
{
    midiSerial.begin(31250); // MIDI Baud rate
    initializePin(MIDISTATUS);
    initializePin(AUXTIP);
    initializePin(AUXRING);
    initializePin(MAINSWITCH);

    parser = MidiParserInit(&onControlChange);
}

void MainLoop()
{
    if (!midiSerial.available())
    {
        return;
    }

    uint8_t b = midiSerial.read();
    MidiParserParse(&parser, b);
    PinOff(MIDISTATUS, 0);
    PinOff(AUXTIP, 1);
    PinOff(AUXRING, 2);
    PinOff(MAINSWITCH, 3);
}
