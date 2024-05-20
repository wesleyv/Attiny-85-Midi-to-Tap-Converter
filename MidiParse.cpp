
#include "MidiParse.h"
#include <stdbool.h>
// we can delete this after serial print is removed:
#include <Arduino.h>

uint8_t MidiParserGetChannel(uint8_t b)
{
    return (b & 0b00001111) + 1;
}

bool MidiParserStatusByteIsControlChangeByte(uint8_t b)
{
    return (b & 0b11110000) == 0b10110000;
}

// list of values for tune request and undefined skippable sysex:
bool MidiParserStatusByteIsSkippableByte(uint8_t b)
{
    return b == 0b11110100 || b == 0b11110101 || b == 0b11110110;
}

// rules out multiple undefined + tune request + all system real time messages
bool MidiParserStatusByteisSystemRealTime(uint8_t b)
{
    return b >= 0xF8 && b <= 0xFF;
}

// checks if incoming byte is Program change or Channel Aftertouch, or sysex time code quarter frame or Sysex Song select - each of which has 1 data byte which must be ignored
bool MidiParserStatusByteIsIgnored1DataByte(uint8_t b)
{
    return (b & 0b11110000) == 0b11000000 || (b & 0b11110000) == 0b11010000 || b == 0b11110001 || b == 0b11110011;
}

// checks if incoming byte is Note off, Note on, Poly Aftertouch, or Pitch Bend message, or Sysex Song Position Pointer each of which has 2 data bytes which must be ignored
bool MidiParserStatusByteIsIgnored2DataBytes(uint8_t b)
{
    return (b & 0b11110000) == 0b10000000 || (b & 0b11110000) == 0b10010000 || (b & 0b11110000) == 0b10100000 || (b & 0b11110000) == 0b11100000 || b == 0b11110010;
}

MidiParser MidiParserInit(MidiParserOnControlChange delegate, MidiParserOnError errorDelegate)
{
    MidiParser parser;
    parser.state = MidiParserStateExpectingStatusByte;
    parser.delegate = delegate;
    parser.errorDelegate = errorDelegate;
    parser.channel = 0;
    parser.data0 = 0;
    return parser;
}

bool MidiParserIsEndSysex(uint8_t b)
{
    return b == 0b11110111;
}

bool MidiParserIsBeginSysex(uint8_t b)
{
    return b == 0b11110000;
}

void MidiParserParse(MidiParser *self, uint8_t b)
{
    if (self->state == MidiParserStateExpectingStatusByte)
    {
        if (MidiParserStatusByteIsControlChangeByte(b))
        {
            self->channel = MidiParserGetChannel(b);
            self->state = MidiParserStateControlMessage1;
            // Serial.println("State is: MidiParserStateControlMessage1 ");
        }
        else if (MidiParserStatusByteIsIgnored1DataByte(b))
        {
            self->state = MidiParserStateIgnoring1ByteMessage;
            // Serial.println("State is: MidiParserStateIgnoring1ByteMessage ");
        }
        else if (MidiParserStatusByteIsIgnored2DataBytes(b))
        {
            self->state = MidiParserStateIgnoring2ByteMessage1;

            // Serial.println("State is: MidiParserStateIgnoring2ByteMessage1 ");
        }
        else if (MidiParserIsBeginSysex(b))
        {
            self->state = MidiParserStateSysexIgnore;
            // Serial.println("Sysex Ignore START");
        }
        else if (MidiParserStatusByteIsSkippableByte(b))
        {
            // No Op
            // Serial.println("Byte Skipped ");
        }
        else if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
            // Serial.println("Byte Skipped ");
        }
        else
        {
            self->errorDelegate(self->state, b);
        }
    }
    else if (self->state == MidiParserStateControlMessage1)
    {
        if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
            // Serial.println("Byte is real time ");
        }
        else
        {
            self->data0 = b;
            // Serial.println("Byte Recorded to data0 - state set to Expecting byte 2");
            self->state = MidiParserStateControlMessage2;
        }
    }
    else if (self->state == MidiParserStateControlMessage2)
    {
        if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
        }
        else
        {
            // Serial.println("Byte Recorded to data1");
            self->delegate(self->channel, self->data0, b);
            self->channel = 0;
            self->data0 = 0;
            self->state = MidiParserStateExpectingStatusByte;
        }
    }
    else if (self->state == MidiParserStateIgnoring1ByteMessage)
    {
        if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
        }
        else
        {
            self->state = MidiParserStateExpectingStatusByte;
            // Serial.println("State is: MidiParserStateExpectingStatusByte ");
        }
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage1)
    {
        if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
        }
        else
        {
            self->state = MidiParserStateIgnoring2ByteMessage2;
            // Serial.println("State is: MidiParserStateIgnoring2ByteMessage2 ");
        }
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage2)
    {
        if (MidiParserStatusByteisSystemRealTime(b))
        {
            // No Op
        }
        else
        {
            self->state = MidiParserStateExpectingStatusByte;
            // Serial.println("State is: MidiParserStateExpectingStatusByte ");
        }
    }
    else if (self->state == MidiParserStateSysexIgnore)
    {
        if (MidiParserIsEndSysex(b))
        {
            self->state = MidiParserStateExpectingStatusByte;
            // Serial.println("Sysex Ignore STOP");
        }
    }
}