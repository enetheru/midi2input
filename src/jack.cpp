#include "jack.h"
#include "log.h"

jack_singleton *
jack_singleton::getInstance( const bool init )
{
    static jack_singleton jack;

    if( init ){
        LOG( INFO ) << "Initialising Jack\n";
        jack.client = jack_client_open( "midi2input_jack", JackNullOption, nullptr );
        if(! jack.client ){
            LOG( ERROR ) << "unable to open client on jack server\n";
            return &jack;
        }

        LOG( INFO ) << "Jack: registering ports\n";
        jack.input_port = jack_port_register( jack.client, "in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );
        jack.output_port = jack_port_register( jack.client, "out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0 );

        LOG( INFO ) << "Jack: setting event callback\n";
        jack_set_process_callback( jack.client, jack_singleton::jack_process, 0 );

        LOG( INFO ) << "Jack: Activating client\n";
        if( jack_activate( jack.client ) ){
            LOG( ERROR ) << "cannot activate client\n";
            return &jack;
        }
        jack.valid_ = true;
    }
    return &jack;
}

int32_t
jack_singleton::set_eventProcessor( EventProcessor evproc )
{
    eventProcessor = evproc;
    return 0;
}

int32_t
jack_singleton::midi_send( const midi_event &event )
{
    void *port_buf = jack_port_get_buffer( output_port, 0 );
    if(! port_buf ){
        LOG( ERROR ) << "Cannot send events with no connected ports\n";
        return -1;
    }
    jack_midi_clear_buffer( port_buf );

    jack_midi_data_t *mdata = jack_midi_event_reserve( port_buf, 0, 3 );

    mdata[0] = event[0];
    mdata[1] = event[1];
    mdata[2] = event[2];

    LOG( INFO ) << "jack-midi-send: "
                << std::hex << std::setfill( '0' ) << std::uppercase
                << "0x" << std::setw( 2 ) << (int)event[0] << ", "
                << "0x" << std::setw( 2 ) << (int)event[1] << ", "
                << std::dec << std::setfill( ' ' ) << std::setw( 3 ) << (int)event[2] << "\n";

    jack_midi_event_write( port_buf, 0, mdata, 3 );
    return 0;
}

int
jack_singleton::jack_process( jack_nframes_t nframes, void *unused )
{
    (void)unused;
    auto &jack = *jack_singleton::getInstance();
    if(! jack.valid )return -1;

    void* port_buf = jack_port_get_buffer( jack.input_port, nframes );
    auto event_count = jack_midi_get_event_count( port_buf );


    if( event_count > 0 )
    {
        jack_midi_event_t event;
        midi_event result;
        LOG( INFO ) << event_count << " Events in queue\n";
        for( uint32_t i = 0; i < event_count; i++ )
        {
            jack_midi_event_get( &event, port_buf, i );

            LOG( INFO ) << "jack-midi-recv: "
                << std::hex << std::setfill( '0' ) << std::uppercase
                << "0x" << std::setw( 2 ) << (int)event.buffer[ 0 ] << ", "
                << "0x" << std::setw( 2 ) << (int)event.buffer[ 1 ] << ", "
                << std::dec << std::setfill( ' ' ) << std::setw( 3 ) << (int)event.buffer[ 2 ] << "\n";

            result[0] = event.buffer[0];
            result[1] = event.buffer[1];
            result[2] = event.buffer[2];
            jack.eventProcessor( result );

        }
    }
    return 0;
}

jack_singleton::~jack_singleton() {
    LOG(INFO) << "destroying jack singleton\n";
    jack_client_close( client );
}
