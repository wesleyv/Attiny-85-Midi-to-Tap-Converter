#ifndef MIDIPARSE_H
#define MIDIPARSE_H

#ifdef __cplusplus
extern "C" {
#endif

#import <stdint.h>

// private
typedef enum {
    MidiParserStateExpectingStatusByte,
    MidiParserStateControlMessage1,
    MidiParserStateControlMessage2,
    MidiParserStateIgnoring1ByteMessage,
    MidiParserStateIgnoring2ByteMessage1,
    MidiParserStateIgnoring2ByteMessage2,
} MidiParserState;

typedef void (*MidiParserOnControlChange)(uint8_t channel, uint8_t data0, uint8_t data1);

// private
typedef struct {
    MidiParserState state;
    MidiParserOnControlChange delegate;
    uint8_t channel;
    uint8_t data0;
} MidiParser;

MidiParser MidiParserInit(MidiParserOnControlChange delegate);
void MidiParserParse(MidiParser *self, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif
