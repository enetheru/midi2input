#ifndef MIDI2INPUT_JACK_H
#define MIDI2INPUT_JACK_H

//header to initialise jack interface and keep it clean
#include <jack/jack.h>
#include <jack/midiport.h>
#include "midi.h"

typedef int (*EventProcessor)( const midi_event &);

class jack_singleton{
public:
    static jack_singleton *getInstance( const bool init = false );
    void event_send( const midi_event &event );
    int event_pending();
    midi_event event_receive();

    const bool &valid = valid_;

    ~jack_singleton();
private:
    jack_singleton() = default;

    static void error_func( const char *msg );

    bool valid_ = false;
    jack_client_t *client = nullptr;
    jack_port_t *input_port = nullptr;
    void *in_portbuf = nullptr;
    jack_port_t *output_port = nullptr;
    EventProcessor eventProcessor = nullptr;
};

#endif // MIDI2INPUT_JACK_H
