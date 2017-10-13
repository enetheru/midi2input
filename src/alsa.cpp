#include "alsa.h"
#include "util.h"

void
AlsaSeq::init()
{
    LOG( INFO ) << "Initialising ALSA\n";

    if( snd_seq_open( &seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK ) < 0 )
    {
        LOG( FATAL ) << "ALSA: Problem creating midi sequencer client\n";
        return;
    }
    snd_seq_set_client_name( seq, "midi2input_alsa" );

    in_port = snd_seq_create_simple_port(
        seq,
        "in",
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC
    );
    if( in_port < 0 ){
        LOG( FATAL ) << "ALSA: Problem creating input midi port\n";
        return;
    }

    out_port = snd_seq_create_simple_port(
        seq,
        "out",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC
    );
    if( out_port < 0 ){
        LOG( FATAL ) << "ALSA: Problem creating output midi port\n";
        return;
    }
    valid_ = true;
}

void
AlsaSeq::event_send( const midi_event &event )
{
    snd_seq_event_t ev;
    snd_seq_ev_clear( &ev );
    snd_seq_ev_set_source( &ev, out_port );
    snd_seq_ev_set_subs( &ev );
    snd_seq_ev_set_direct( &ev );
    snd_seq_ev_set_noteon( &ev, event.status & 0x0F, event.data1, event.data2 );

    snd_seq_event_output( seq, &ev);
    snd_seq_drain_output( seq );

    LOG( INFO ) << "alsa-midi-send: " << midi2string( event ) << "\n";
}

int
AlsaSeq::event_pending(){
    return snd_seq_event_input_pending( seq, 1 );
}

midi_event
AlsaSeq::event_receive()
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
    return result;
}

AlsaSeq::~AlsaSeq() {
    LOG(INFO) << "destroying alsa singleton\n";
    snd_seq_delete_simple_port( seq, out_port );
    snd_seq_delete_simple_port( seq, in_port );
}
