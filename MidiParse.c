#import "MidiParse.h"
#import <stdbool.h>

bool SysExIgnore = false;

uint8_t MidiParserGetChannel(uint8_t b)
{
    return (b & 0b00001111) - 1;
}

bool MidiParserStatusByteIsControlChangeByte(uint8_t b)
{
    if (b == 0b10110000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MidiParserStatusByteisKnownSkippableByte(uint8_t b)
{ // rules out multiple undefined + tune request + all system real time messages
    if (b == 0b11110100 || b == 0b11110101 || b == 0b11110110 || b == 0b11111000 || b == 0b11111001 || b == 0b11111010 || b == 0b11111011 || b == 0b11111100 || b == 0b11111101 || b == 0b11111110 || b == 0b11111111)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// checks if incoming byte is Program change or Channel Aftertouch, or sysex time code quarter frame or Sysex Song select - each of which has 1 data byte which must be ignored
bool MidiParserStatusByteIsIgnored1DataByte(uint8_t b)
{
    if (b & 0b11110000 == 0b11000000 || b & 0b11110000 == 0b11010000 || b == 0b11010001 || b == 0b11010011)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// checks if incoming byte is Note off, Note on, Poly Aftertouch, or Pitch Bend message, or Sysex Song Position Pointer each of which has 2 data bytes which must be ignored
bool MidiParserStatusByteIsIgnored2DataBytes(uint8_t b)
{
    if (b & 0b11110000 == 0b10000000 || b & 0b11110000 == 0b10010000 || b & 0b11110000 == 0b10100000 || b & 0b11110000 == 0b11100000 || b == 0b11110010)
    {
        return true;
    }
    else
    {
        return false;
    }
}

MidiParser MidiParserInit(MidiParserOnControlChange delegate)
{
    MidiParser parser;
    parser.state = MidiParserStateExpectingStatusByte;
    parser.delegate = delegate;
    parser.channel = 0;
    parser.data0 = 0;
    return parser;
}

void MidiParserParse(MidiParser *self, uint8_t b)
{
    if (b == 0b11110000)
    {
        bool SysExIgnore = true;
    }
    else if (b == 0b11110111)
    {
        bool SysExIgnore = false;
    }
    ///----
    if (SysExIgnore == true || MidiParserStatusByteisKnownSkippableByte(b))
    {
        return;
    }
    else if (MidiParserStatusByteIsControlChangeByte(b))
    {
        self->channel = MidiParserGetChannel(b);
        self->state = MidiParserStateControlMessage1;
    }
    else if (MidiParserStatusByteIsIgnored1DataByte(b))
    {
        self->state = MidiParserStateIgnoring1ByteMessage;
    }
    else if (MidiParserStatusByteIsIgnored2DataBytes(b))
    {
        self->state = MidiParserStateIgnoring2ByteMessage1;
    }
    ///---
    else if (self->state == MidiParserStateControlMessage1)
    {
        self->data0 = b;
        self->state = MidiParserStateControlMessage2;
    }
    else if (self->state == MidiParserStateControlMessage2)
    {
        self->delegate(self->channel, self->data0, b);
        self->channel = 0;
        self->data0 = 0;
        self->state = MidiParserStateExpectingStatusByte;
    }
    ///---
    else if (self->state == MidiParserStateIgnoring1ByteMessage)
    {
        //    self->state = MidiParserStateExpectingStatusByte;
        return;
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage1)
    {
        self->state = MidiParserStateIgnoring2ByteMessage2;
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage2)
    {
        //    self->state = MidiParserStateExpectingStatusByte;
        return;
    }
}