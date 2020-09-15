#ifndef MIDI2INPUT_JACK_H
#define MIDI2INPUT_JACK_H

//header to initialise jack interface and keep it clean
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>
#include "midi.h"

class JackSeq{
public:
    JackSeq() = default;
    void init();
    void fina();

    int do_process(jack_nframes_t num_frames);
    void event_send( const midi_event &event );
    unsigned long event_pending();
    midi_event event_receive();

    const bool &valid = valid_;

    ~JackSeq();
private:
    bool valid_ = false;

    jack_client_t *client = nullptr;
    jack_port_t *input_port = nullptr;
    jack_port_t *output_port = nullptr;

    jack_ringbuffer_t *event_buf = nullptr;

};

#endif // MIDI2INPUT_JACK_H
