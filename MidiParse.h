#ifndef MIDIPARSE_H
#define MIDIPARSE_H

#ifdef __cplusplus
extern "C" {
#endif

#import <stdint.h>


typedef enum {
	MidiParserStateInitial
} MidiParserState;

typedef void (*MidiParserOnControlChange)(uint8_t channel, uint8_t data0, uint8_t data1);

typedef struct {
	MidiParserState state;
	MidiParserOnControlChange delegate;
} MidiParser;

MidiParser MidiParserInit(MidiParserOnControlChange delegate);
void MidiParserParse(MidiParser *self, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif
