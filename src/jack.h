#ifndef MIDI2INPUT_JACK_H
#define MIDI2INPUT_JACK_H

//header to initialise jack interface and keep it clean
#include <jack/jack.h>
#include <jack/midiport.h>
#include "midi.h"

class JackSeq{
public:
    JackSeq() = default;
    void init();
    void fina();

    void event_send( const midi_event &event );
    int event_pending();
    midi_event event_receive();

    const bool &valid = valid_;

    ~JackSeq();
private:
    bool valid_ = false;

    jack_client_t *client = nullptr;
    jack_port_t *input_port = nullptr;
    jack_port_t *output_port = nullptr;

    static void error_callback( const char * );
    static void info_callback( const char *  );


};

#endif // MIDI2INPUT_JACK_H
