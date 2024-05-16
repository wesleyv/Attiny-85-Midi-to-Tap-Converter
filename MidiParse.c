#import "MidiParse.h"

MidiParser MidiParserInit(MidiParserOnControlChange delegate) {
    MidiParser parser;
    parser.state = MidiParserStateInitial;
    parser.delegate = delegate;
    return parser;
}
void MidiParserParse(MidiParser *self, uint8_t b) {

}
