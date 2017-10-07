#include "alsa.h"
#include "log.h"

alsa_singleton *
alsa_singleton::getInstance( const bool init )
{
    static alsa_singleton alsa;

    if( init ){
        LOG( INFO ) << "Initialising ALSA\n";

        int err;
        err = snd_seq_open( &alsa.handle, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK );
        if( err < 0 ){
            LOG( FATAL ) << "ALSA: Problem creating midi sequencer client\n";
            return &alsa;
        }
        snd_seq_set_client_name( alsa.handle, "midi2input" );

        alsa.in_port = snd_seq_create_simple_port(
            alsa.handle,
            "in",
            SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_MIDI_GENERIC
        );
        if( alsa.in_port < 0 ){
            LOG( FATAL ) << "ALSA: Problem creating input midi port\n";
            return &alsa;
        }

        alsa.out_port = snd_seq_create_simple_port(
            alsa.handle,
            "out",
            SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
            SND_SEQ_PORT_TYPE_MIDI_GENERIC
        );
        if( alsa.out_port < 0 ){
            LOG( FATAL ) << "ALSA: Problem creating output midi port\n";
            return &alsa;
        }
        alsa.valid_ = true;
    }
    return &alsa;
}

int32_t
alsa_singleton::set_eventProcessor( EventProcessor evproc )
{
    eventProcessor = evproc;
    return 0;
}

int32_t
alsa_singleton::midi_send( const midi_event &event )
{

    snd_seq_event_t ev;
    snd_seq_ev_clear( &ev );
    snd_seq_ev_set_source( &ev, out_port );
    snd_seq_ev_set_subs( &ev );
    snd_seq_ev_set_direct( &ev );
    snd_seq_ev_set_noteon( &ev, event[0] & 0x0F, event[1], event[2] );

    snd_seq_event_output( handle, &ev);
    snd_seq_drain_output( handle );  // if necessary

    LOG( INFO ) << "alsa-midi-send: "
                << std::hex << std::setfill( '0' ) << std::uppercase
                << "0x" << std::setw( 2 ) << (int)event[0] << ", "
                << "0x" << std::setw( 2 ) << (int)event[1] << ", "
                << std::dec << std::setfill( ' ' ) << std::setw( 3 ) << (int)event[2]
                << "\n";
    return 0;
}

void
alsa_singleton::midi_recv()
{
    // this requires some form of while loop to eat all the events currently sitting
    // in the queue

    midi_event result = {};
    snd_seq_event_t *ev = nullptr;
    int status = 0;
    while( snd_seq_event_input( handle, &ev ) > 0 ){
        if( ev->type == SND_SEQ_EVENT_NOTEON ) {
            status = 0x90;
            result[0] = ev->data.note.channel + status;
            result[1] = ev->data.note.note;
            result[2] = ev->data.note.velocity;
        } else if(ev->type == SND_SEQ_EVENT_NOTEOFF ) {
            status = 0x80;
            result[0] = ev->data.note.channel + status;
            result[1] = ev->data.note.note;
            result[2] = ev->data.note.velocity;
        } else if( ev->type == SND_SEQ_EVENT_CONTROLLER ) {
            status = 0xB0;
            result[0] = ev->data.control.channel + status;
            result[1] = ev->data.control.param;
            result[2] = ev->data.control.value;
        } else {
            LOG( INFO ) << "ALSA: Unhandled Event Received\n";
            continue;
        }
        eventProcessor( result );
        LOG( INFO ) << "alsa-midi-recv: "
                    << std::hex << std::setfill( '0' ) << std::uppercase
                    << "0x" << std::setw( 2 ) << (int)result[0] << ", "
                    << "0x" << std::setw( 2 ) << (int)result[1] << ", "
                    << std::dec << std::setfill( ' ' ) << std::setw( 3 ) << (int)result[2]
                    << "\n";
    }
}

alsa_singleton::~alsa_singleton() {
    LOG(INFO) << "destroying alsa singleton\n";

    snd_seq_delete_simple_port( handle, out_port );
    snd_seq_delete_simple_port( handle, in_port );

}
