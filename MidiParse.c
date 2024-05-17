#import "MidiParse.h"
#import <stdbool.h>
#define MIDICH 9

uint8_t MidiParserGetChannel(uint8_t b) {
uint8_t ByteChannel = b & 0b00001111;
}
// I'm pretty sure I'm not doing this right - how do I return a uint_8 value?


bool MidiParserStatusByteIsControlChangeByte(uint8_t b)
{
    if (b == 0b10110000){
        return true;
    }
    else {
        return false;
    }
}

bool MidiParserStatusCCByteIsOnDesiredChannel(uint8_t b)
{
    if (b & 0b00001111 == (MIDICH - 1)){
        return true;
    }
    else {
        return false;
    }
}

bool MidiParserStatusByteIsSysexByte(uint8_t b)
{
    if (b & 0b11110000  == 0b11110000){
        return true;
    }
    else {
        return false;
    }
}

// checks if incoming byte is Program change or Channel Aftertouch, each of which has 1 data byte which must be ignored
bool MidiParserStatusByteIsIgnored1DataByte(uint8_t b)
{
    if (b & 0b11110000 == 0b11000000 || b & 0b11110000 == 0b11010000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// checks if incoming byte is Note off, Note on, Poly Aftertouch, or Pitch Bend message, each of which has 2 data bytes which must be ignored
bool MidiParserStatusByteIsIgnored2DataBytes(uint8_t b)
{
    if (b & 0b11110000 == 0b10000000 || b & 0b11110000 == 0b10010000 || b & 0b11110000 == 0b10100000 || b & 0b11110000 == 0b11100000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ByteIsStatusByte(uint8_t b)
{
    if (b & 0b10000000 == 0b10000000)
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

void MidiParserParse(MidiParser *self, uint8_t b) {
    if (self->state == MidiParserStateExpectingStatusByte) {
        if (MidiParserStatusByteIsControlChangeByte(b)) {
            self->channel = MidiParserGetChannel(b);
            self->state = MidiParserStateControlMessage1;
        }
        else if (MidiParserStatusByteIsIgnored1DataByte(b)) {
            self->state = MidiParserStateIgnoring1ByteMessage;
        }
        else if (MidiParserStatusByteIsIgnored2DataBytes(b)) {
            self->state = MidiParserStateIgnoring2ByteMessage1;
        }
        else {
            // halt and catch fire
            // emit "not expecting byte b in state self->state"
        }
    }
    else if (self->state == MidiParserStateControlMessage1) {
        self->data0 = b;
        self->state = MidiParserStateControlMessage2;
    }
    else if (self->state == MidiParserStateControlMessage2) {
        self->delegate(self->channel, self->data0, b);
        self->channel = 0;
        self->data0 = 0;
        self->state = MidiParserStateExpectingStatusByte;
    }
    else if (self->state == MidiParserStateIgnoring1ByteMessage) {
        self->state = MidiParserStateExpectingStatusByte;
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage1) {
        self->state = MidiParserStateIgnoring2ByteMessage2;
    }
    else if (self->state == MidiParserStateIgnoring2ByteMessage2) {
        self->state = MidiParserStateExpectingStatusByte;
    }
}