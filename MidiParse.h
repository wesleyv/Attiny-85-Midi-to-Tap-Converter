#ifndef MIDIPARSE_H
#define MIDIPARSE_H
#import <stdint.h>
typedef void (*MidiParseControlChange)(uint8_t channel, uint8_t data0, uint8_t data1);
void MidiParse(uint8_t b, MidiParseControlChange delegate); // this is a function prototype. It describes the name of the function, the argument, and the return type. But does not contain implementation
#endif