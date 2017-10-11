//
// Created by enetheru on 9/26/17.
//

#ifndef M2I_MIDI_H
#define M2I_MIDI_H

#include <string>

//simple midi event structure.
typedef unsigned char midi_event[3];

std::string midi2string( const midi_event &event );

#endif //M2I_MIDI_H
