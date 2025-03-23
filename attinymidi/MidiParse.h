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

#ifndef MIDIPARSE_H
#define MIDIPARSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// private
typedef enum {
    MidiParserStateExpectingStatusByte,
    MidiParserStateControlMessage1,
    MidiParserStateControlMessage2,
    MidiParserStateIgnoring1ByteMessage,
    MidiParserStateIgnoring2ByteMessage1,
    MidiParserStateIgnoring2ByteMessage2,
    MidiParserStateSysexIgnore,
} MidiParserState;

typedef void (*MidiParserOnControlChange)(uint8_t channel, uint8_t data0, uint8_t data1);
typedef void (*MidiParserOnError)(MidiParserState state, uint8_t b);

// private
typedef struct {
    MidiParserState state;
    MidiParserOnControlChange delegate;
    MidiParserOnError errorDelegate;
    uint8_t channel;
    uint8_t data0;
} MidiParser;

MidiParser MidiParserInit(MidiParserOnControlChange delegate, MidiParserOnError errorDelegate);
void MidiParserParse(MidiParser *self, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif
