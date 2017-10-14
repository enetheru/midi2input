//
// Created by enetheru on 9/26/17.
//

#ifndef M2I_MIDI_H
#define M2I_MIDI_H

#include <string>

//simple midi event structure.
struct midi_event {
    unsigned char status;
    unsigned char data1;
    unsigned char data2;

    std::string str() const;
private:
};

#endif //M2I_MIDI_H
