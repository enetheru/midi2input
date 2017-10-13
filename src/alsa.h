#ifndef MIDI2INPUT_ALSA_H
#define MIDI2INPUT_ALSA_H

//header to initialise jack interface and keep it clean
#include <alsa/asoundlib.h>
#include "midi.h"

class AlsaSeq{
public:
    AlsaSeq() = default;
    void init();

    midi_event event_receive();
    int event_pending();
    void event_send( const midi_event &event );

    const bool &valid = valid_;

    ~AlsaSeq();
private:
    bool valid_ = false;
    int out_port = -1;
    int in_port = -1;
    snd_seq_t *seq = nullptr;
};

#endif // MIDI2INPUT_JACK_H
