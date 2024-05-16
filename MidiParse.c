#import "MidiParse.h"
#import <stdbool.h>

uint8_t MidiParserGetChannel(uint8_t b) {

}

bool MidiParserStatusByteIsControlChangeByte(uint8_t b) {

}

bool MidiParserStatusByteIsIgnored1DataByte(uint8_t b) {

}

bool MidiParserStatusByteIsIgnored2DataBytes(uint8_t b) {

}

MidiParser MidiParserInit(MidiParserOnControlChange delegate) {
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