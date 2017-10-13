#include "alsa.h"
#include "util.h"

alsa_singleton *
alsa_singleton::getInstance( const bool init )
{
    static alsa_singleton alsa;

    if( init ){
        LOG( INFO ) << "Initialising ALSA\n";

        int err;
        err = snd_seq_open( &alsa.seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK );
        if( err < 0 ){
            LOG( FATAL ) << "ALSA: Problem creating midi sequencer client\n";
            return &alsa;
        }
        snd_seq_set_client_name( alsa.seq, "midi2input_alsa" );

        alsa.in_port = snd_seq_create_simple_port(
            alsa.seq,
            "in",
            SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_MIDI_GENERIC
        );
        if( alsa.in_port < 0 ){
            LOG( FATAL ) << "ALSA: Problem creating input midi port\n";
            return &alsa;
        }

        alsa.out_port = snd_seq_create_simple_port(
            alsa.seq,
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
    snd_seq_ev_set_noteon( &ev, event.status & 0x0F, event.data1, event.data2 );

    snd_seq_event_output( seq, &ev);
    snd_seq_drain_output( seq );  // if necessary

    LOG( INFO ) << "alsa-midi-send: " << midi2string( event ) << "\n";
    return 0;
}

void
alsa_singleton::midi_recv()
{
    // this requires some form of while loop to eat all the events currently sitting
    // in the queue

    midi_event result;
    snd_seq_event_t *ev = nullptr;
    int status = 0;
    while( snd_seq_event_input( seq, &ev ) > 0 ){
        if( ev->type == SND_SEQ_EVENT_NOTEON ) {
            status = 0x90;
            result.status = ev->data.note.channel + status;
            result.data1 = ev->data.note.note;
            result.data2 = ev->data.note.velocity;
        } else if(ev->type == SND_SEQ_EVENT_NOTEOFF ) {
            status = 0x80;
            result.status = ev->data.note.channel + status;
            result.data1 = ev->data.note.note;
            result.data2 = ev->data.note.velocity;
        } else if( ev->type == SND_SEQ_EVENT_CONTROLLER ) {
            status = 0xB0;
            result.status = ev->data.control.channel + status;
            result.data1 = ev->data.control.param;
            result.data2 = ev->data.control.value;
        } else {
            LOG( INFO ) << "ALSA: Unhandled Event Received\n";
            continue;
        }
        eventProcessor( result );
        LOG( INFO ) << "alsa-midi-recv: " << midi2string( result ) << "\n";
    }
}

int
alsa_singleton::event_pending()//FIXME snd_seq_t * seq )
{
    return snd_seq_event_input_pending( seq, 0 );
}

midi_event
alsa_singleton::event_get()//FIXME snd_seq_t *seq )
{
    midi_event result;
    snd_seq_event_t *ev = nullptr;
    int status = 0;
    if( snd_seq_event_input( seq, &ev ) > 0 ){
        if( ev->type == SND_SEQ_EVENT_NOTEON ) {
            status = 0x90;
            result.status = ev->data.note.channel + status;
            result.data1 = ev->data.note.note;
            result.data2 = ev->data.note.velocity;
        } else if(ev->type == SND_SEQ_EVENT_NOTEOFF ) {
            status = 0x80;
            result.status = ev->data.note.channel + status;
            result.data1 = ev->data.note.note;
            result.data2 = ev->data.note.velocity;
        } else if( ev->type == SND_SEQ_EVENT_CONTROLLER ) {
            status = 0xB0;
            result.status = ev->data.control.channel + status;
            result.data1 = ev->data.control.param;
            result.data2 = ev->data.control.value;
        } else {
            LOG( INFO ) << "ALSA: Unhandled Event Received\n";
        }
    }
    free(ev);//FIXME unsure about this;
    return result;
}

alsa_singleton::~alsa_singleton() {
    LOG(INFO) << "destroying alsa singleton\n";
    snd_seq_delete_simple_port( seq, out_port );
    snd_seq_delete_simple_port( seq, in_port );
}
