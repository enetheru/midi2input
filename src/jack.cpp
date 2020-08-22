#include "jack.h"
#include "util.h"

using namespace m2i;

#define EVENT_BUF_SIZE 64

void
JackSeq::init()
{
    LOG( JACK ) << "Initialising\n";

    event_buf = jack_ringbuffer_create(EVENT_BUF_SIZE * sizeof(midi_event));

    jack_set_error_function( [](const char *msg){
        LOG( JACK ) << "Error: " << msg << "\n";
            } );

    jack_set_info_function( [](const char *msg){
        LOG( JACK ) << "Info: " << msg << "\n";
            } );

    client = jack_client_open( "midi2input_jack", JackNoStartServer, nullptr );
    if(! client ){
        LOG( JACK ) << "unable to open client on server\n";
        return;
    }

    jack_set_process_callback( client, [](jack_nframes_t nframes, void *arg){
        auto jackSeq = (JackSeq*) arg;
        return jackSeq->do_process(nframes);
        }, this);

    LOG( JACK ) << "registering ports\n";
    input_port = jack_port_register( client, "in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );
    output_port = jack_port_register( client, "out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0 );

    LOG( JACK ) << "Activating client\n";
    if( jack_activate( client ) ){
        LOG( JACK ) << "cannot activate client\n";
        return;
    }
    valid_ = true;
}

void
JackSeq::fina()
{
    if( valid ){
        valid_ = false;
        jack_client_close( client );
        jack_ringbuffer_free(event_buf);
    }
    return;
}


void
JackSeq::event_send( const midi_event &event )
{
    //FIXME there is a bug in here somewhere that makes sending a midi event to the
    //controller fail.
    void *port_buf = jack_port_get_buffer( output_port, 0 );
    if(! port_buf ){
        LOG( JACK ) << "Cannot send events with no connected ports\n";
        return;
    }
    jack_midi_clear_buffer( port_buf );

    jack_midi_data_t *mdata = jack_midi_event_reserve( port_buf, 0, 3 );

    mdata[0] = event.status;
    mdata[1] = event.data1;
    mdata[2] = event.data2;
    jack_midi_event_write( port_buf, 0, mdata, 3 );

    LOG( JACK ) << "midi-send: " << event.str() << "\n";
    return;
}

// this is a callback passed to jack to read events into our buffer
int
JackSeq::do_process(jack_nframes_t process_nframes)
{
    void *port_buf = jack_port_get_buffer( input_port, process_nframes );
    uint32_t jack_event_count = jack_midi_get_event_count( port_buf );

    if (jack_event_count > 0) {
        LOG( JACK ) << "Event Count: " << jack_event_count << "\n";
    }

    uint32_t i;
    jack_midi_event_t jack_event;
    midi_event  m2i_event;
    for (i = 0; i < jack_event_count; i++) {
        int ret = jack_midi_event_get(&jack_event, port_buf, i);

        if (ret != 0) { continue; }

        if (jack_ringbuffer_write_space(event_buf) < sizeof(midi_event)) {
            LOG( JACK ) << "Unable to read event from jack, ringbuffer was full. Skipping event.";
            continue;
        }

        m2i_event.status = jack_event.buffer[0];
        m2i_event.data1  = jack_event.buffer[1];
        m2i_event.data2  = jack_event.buffer[2];

        jack_ringbuffer_write(event_buf, (const char*) &m2i_event, sizeof(m2i_event));
    }

    return 0;
}

int
JackSeq::event_pending()
{
    const int events_waiting_in_buffer = jack_ringbuffer_read_space(event_buf) / sizeof(midi_event);
    return events_waiting_in_buffer;
}

midi_event
JackSeq::event_receive()
{
    midi_event result;
    auto bytes_read = jack_ringbuffer_read(event_buf, (char*) &result, sizeof(result));
    if (bytes_read == 0) {
        LOG( JACK ) << "Ringbuffer read didn't read.";
        result = {};
        return result;
    } else if (bytes_read < sizeof(result)) {
        LOG( JACK ) << "Ringbuffer read read an incomplete event, uh oh";
        result = {};
        return result;
    }

    LOG( JACK ) << "midi-recv: " << result.str() << "\n";
    return result;
}

JackSeq::~JackSeq(){
    fina();
}
