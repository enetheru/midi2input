#ifndef MIDI2INPUT_JACK_H
#define MIDI2INPUT_JACK_H

//header to initialise jack interface and keep it clean
#include <jack/jack.h>
#include <jack/midiport.h>
#include "midi.h"

typedef int32_t (*EventProcessor)( const midi_event &);

class jack_singleton{
public:
    static jack_singleton &getInstance( bool init = false );
    int32_t set_eventProcessor( EventProcessor );
    int32_t midi_send( const midi_event &event );

    bool valid = false;

    ~jack_singleton();
private:
    jack_singleton() = default;

    static int jack_process( jack_nframes_t, void *);

    jack_client_t *client = nullptr;
    jack_port_t *input_port = nullptr;
    jack_port_t *output_port = nullptr;
    EventProcessor eventProcessor = nullptr;
};

#endif // MIDI2INPUT_JACK_H
