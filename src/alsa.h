#ifndef MIDI2INPUT_ALSA_H
#define MIDI2INPUT_ALSA_H

//header to initialise jack interface and keep it clean
#include <alsa/asoundlib.h>
#include "midi.h"

typedef int32_t (*EventProcessor)( const midi_event &);

class alsa_singleton{
public:
    static alsa_singleton *getInstance( const bool init = false );
    midi_event event_receive();// snd_seq_t *seq );
    int event_pending();// snd_seq_t *seq );
    void event_send( const midi_event &event );

    const bool &valid = valid_;

    ~alsa_singleton();
private:
    alsa_singleton() = default;

    bool valid_ = false;
    int out_port = -1;
    int in_port = -1;
    snd_seq_t *seq = nullptr;
    EventProcessor eventProcessor = nullptr;
};

#endif // MIDI2INPUT_JACK_H
