#ifndef MIDI2INPUT_ALSA_H
#define MIDI2INPUT_ALSA_H

//header to initialise jack interface and keep it clean
#include <alsa/asoundlib.h>
#include "midi.h"

typedef int32_t (*EventProcessor)( const midi_event &);

class alsa_singleton{
public:
    static alsa_singleton *getInstance( const bool init = false );
    int32_t set_eventProcessor( EventProcessor );
    int32_t midi_send( const midi_event &event );
    void midi_recv();

    const bool &valid = valid_;

    ~alsa_singleton();
private:
    alsa_singleton() = default;

    bool valid_ = false;
    int out_port = -1;
    int in_port = -1;
    snd_seq_t *handle = nullptr;
    EventProcessor eventProcessor = nullptr;
};

#endif // MIDI2INPUT_JACK_H
