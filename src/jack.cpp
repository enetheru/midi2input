#include "jack.h"
#include "util.h"

using namespace m2i;

void
JackSeq::init()
{
    LOG( JACK ) << "Initialising\n";

    jack_set_error_function( JackSeq::error_callback );
    jack_set_info_function( JackSeq::info_callback );

    client = jack_client_open( "midi2input_jack", JackNoStartServer, nullptr );
    if(! client ){
        LOG( JACK ) << "unable to open client on server\n";
        return;
    }

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

int
JackSeq::event_pending()
{
    //FIXME there is a bug here somewhere, because sometimes the count differs
    // from this function to the one below it if called one after the other
    void* port_buf = jack_port_get_buffer( input_port, 0 );
    return jack_midi_get_event_count( port_buf );
}

midi_event
JackSeq::event_receive()
{
    void* port_buf = jack_port_get_buffer( input_port, 0 );

    jack_midi_event_t event;
    jack_midi_event_get( &event, port_buf, 0 );
    auto event_count = jack_midi_get_event_count( port_buf );
    LOG( JACK ) << "Event Count: " << event_count << "\n";

    midi_event result;
    if( event_count < 1 ) return result;
    result.status = event.buffer[0];
    result.data1 = event.buffer[1];
    result.data2 = event.buffer[2];

    LOG( JACK ) << "midi-recv: " << result.str() << "\n";
    return result ;
}

JackSeq::~JackSeq(){
    fina();
}

void
JackSeq::error_callback( const char *msg )
{
    LOG( JACK ) << "Error: " << msg << "\n";
}
void
JackSeq::info_callback( const char *msg )
{
    LOG( JACK ) << "Info: " << msg << "\n";
}
