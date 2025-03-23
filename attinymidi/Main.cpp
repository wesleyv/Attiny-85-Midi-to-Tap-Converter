/*
 * Project Name: Attiny-85-Midi-to-Tap-Converter
 * Description: This is firmware for upgrading the MIDI functionality of the Boss-RC5 looper station, or adding midi functionality to any device that actuates buttons by shorting them to ground.
 * designed to run on ATTiny85.
 * 
 * Copyright (c) 2025 Wesley Van Der Veen
 * 
 * This software is released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE AND HARDWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Main.h"
#include "MidiParse.h"
#include "Pins.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MIDIRX 2

#define MIDICH 9
#define AUXTIP 3
#define AUXRING 0
#define MAINSWITCH 4
#define MIDISTATUS 1

#define AUXTIPCC 88
#define AUXRINGCC 89
#define MAINSWITCHCC 90

uint32_t now = 0;

SoftwareSerial midiSerial(MIDIRX, -1);
MidiParser parser;

void onControlChange(uint8_t channel, uint8_t data0, uint8_t data1)
{
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
    // No Op
}

void MainSetup()
{
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
    MidiParserParse(&parser, b);
}
