#import "MidiParse.h"
#import <stdbool.h>
#define MIDICH 9

//uint8_t MidiParserGetChannel(uint8_t b) {
//uint8_t ByteChannel = b & 0b00001111;
//}


//uint8_t MidiParserGetStatusType(uint8_t b) {
//uint8_t ByteStatus = b & 0b11110000;
//}

bool MidiParserStatusByteIsControlChangeByte(uint8_t b) {

}

bool MidiParserStatusByteIsIgnored1DataByte(uint8_t b) {

}

bool MidiParserStatusByteIsIgnored2DataBytes(uint8_t b) {

}

bool ByteIsStatusByte(uint8_t b) {
    if (b & 0b10000000 == 0b10000000){
        return true;
    }
    else {
        return false;
    }
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
if (ByteIsStatusByte(b)){
    uint8_t ByteStatus = b & 0b11110000;
    if (ByteStatus == 0b10000000 || ByteStatus == 0b10010000 || ByteStatus == 0b10100000 || ByteStatus == 0b11100000){ 
        //checks if incoming byte is Note off, Note on, Poly Aftertouch, or Pitch Bend message, each of which has 2 data bytes which must be ignored
    self->state = MidiParserStateIgnoring2ByteMessage1;
    }
    else if (ByteStatus == 0b11000000 || ByteStatus == 0b11010000){
    //checks if incoming byte is Program change or Channel Aftertouch, each of which has 1 data byte which must be ignored
    self->state = MidiParserStateIgnoring1ByteMessage;
    }
    else if (ByteStatus == 0b11110000){ //Sysex
    // send it down the sysex tubes. Mapped out in my notes. Let's worry about it later
    }
    else if (ByteStatus == 0b10110000){ // checks if control change
        if (b & 0b00001111 == (MIDICH - 1)){ // checks if control change is on desired channel. -1 accounts for 0 indexing
        self->state = MidiParserStateControlMessage1;
        }
        else {
        self->state = MidiParserStateIgnoring2ByteMessage1;
        }
    }
}
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