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
    client_id = snd_seq_client_id( seq );
    snd_seq_set_client_name( seq, "midi2input_alsa" );

    iport_id = snd_seq_create_simple_port(
        seq,
        "in",
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( iport_id < 0 ){
        LOG( FATAL ) << "ALSA: Problem creating input midi port\n";
        return;
    }

    oport_id = snd_seq_create_simple_port(
        seq,
        "out",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( oport_id < 0 ){
        LOG( FATAL ) << "ALSA: Problem creating output midi port\n";
        return;
    }
    valid_ = true;
}

void
AlsaSeq::fina()
{
    valid_ = false;
    snd_seq_delete_simple_port( seq, iport_id );
    snd_seq_delete_simple_port( seq, oport_id );
    snd_seq_close( seq );

    client_id = -1;
    iport_id = -1;
    oport_id = -1;
    seq = nullptr;
}


m2i::ECODE
AlsaSeq::connect( const std::string &client_name, const std::string &port_name )
{
    //client info
    snd_seq_client_info_t *cinfo;
    snd_seq_client_info_alloca( &cinfo );

     //port info
    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca( &pinfo );

    snd_seq_client_info_set_client( cinfo, -1 );
    while( snd_seq_query_next_client( seq, cinfo ) >= 0 ){
        //refuse to self connect
        if( client_id == snd_seq_client_info_get_client( cinfo ) ){
            LOG( INFO ) << "client: refusing to self connect\n";
            continue;
        }
        if( client_name == "*" ){
            LOG( INFO ) << "client: wildcard match: " << snd_seq_client_info_get_name( cinfo ) << "\n";
        }
        else if( client_name == snd_seq_client_info_get_name( cinfo ) ){
            LOG( INFO ) << "client: name match: " << snd_seq_client_info_get_name( cinfo ) << "n";
        }
        else continue;

        int client = snd_seq_client_info_get_client( cinfo );
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while( snd_seq_query_next_port( seq, pinfo ) >= 0 ){
            if( port_name == "*" )
                LOG( INFO ) << "port: wildcard match: " << snd_seq_port_info_get_name( pinfo ) << "\n";
            else if( port_name == snd_seq_port_info_get_name( pinfo ) )
                LOG( INFO ) << "port: name match: " << snd_seq_port_info_get_name( pinfo ) << "\n";
            else continue;

            auto capabilities = snd_seq_port_info_get_capability(pinfo);
            if( capabilities & (SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ) ){
                LOG( INFO ) << "port: connect from: " << snd_seq_port_info_get_name( pinfo ) << "\n";
                snd_seq_connect_from( seq, iport_id, client, snd_seq_port_info_get_port( pinfo ) );
            }
            if( capabilities & (SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE) ){
                LOG( INFO ) << "port: connect to: " << snd_seq_port_info_get_name( pinfo ) << "\n";
                snd_seq_connect_to( seq, oport_id, client, snd_seq_port_info_get_port( pinfo ) );
            }
            //return m2i::ECODE::SUCCESS;
        }
    }
    return m2i::ECODE::FAILURE;
}

void
AlsaSeq::event_send( const midi_event &event )
{
    snd_seq_event_t ev;
    snd_seq_ev_clear( &ev );
    snd_seq_ev_set_source( &ev, oport_id );
    snd_seq_ev_set_subs( &ev );
    snd_seq_ev_set_direct( &ev );
    snd_seq_ev_set_noteon( &ev, event.status & 0x0F, event.data1, event.data2 );

    snd_seq_event_output( seq, &ev);
    snd_seq_drain_output( seq );

    LOG( INFO ) << fmt::format( "alsa-midi-send: {}\n", event.str() );
}

int
AlsaSeq::event_pending(){
    return snd_seq_event_input_pending( seq, 1 );
}

midi_event
AlsaSeq::event_receive()
{
    midi_event result = { 0, 0, 0 };
    snd_seq_event_t *ev = nullptr;
    if( snd_seq_event_input( seq, &ev ) < 0)
        return result;

    switch( ev->type ){
    case SND_SEQ_EVENT_NOTEON:
        LOG( INFO ) << fmt::format( "Note On: {}, {}, {}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x90;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_NOTEOFF:
        LOG( INFO ) << fmt::format( "Note Off: {}, {}, {}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x80;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_KEYPRESS:
        LOG( INFO ) << fmt::format( "Keypress: {}, {}, {}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        break;
    case SND_SEQ_EVENT_CONTROLLER:
        result.status = ev->data.control.channel + 0xB0;
        result.data1 = ev->data.control.param;
        result.data2 = ev->data.control.value;
        break;
    case SND_SEQ_EVENT_PGMCHANGE:
        LOG( INFO ) << fmt::format( "Program change: {}, {}\n",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_CHANPRESS:
        LOG( INFO ) << fmt::format( "Channel aftertouch: {}, {}\n", 
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_PITCHBEND:
        LOG( INFO ) << fmt::format( "Pitch bend: {}, {}\n",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_CONTROL14:
        LOG( INFO ) << fmt::format( "Control change: {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_NONREGPARAM:
        LOG( INFO ) << fmt::format( "Non-reg. parameter: {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_REGPARAM:
        LOG( INFO ) << fmt::format( "Reg. parameter {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_SONGPOS:
        LOG( INFO ) << "Song position pointer: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_SONGSEL:
        LOG( INFO ) << "Song select: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_QFRAME:
        LOG( INFO ) << "MTC quarter frame: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_TIMESIGN:
        LOG( INFO ) << "SMF time signature: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_KEYSIGN:
        LOG( INFO ) << "SMF key signature: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_START:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( INFO ) << "Queue start: " << ev->data.queue.queue << "\n";
        else
            LOG( INFO ) << "Start\n";
        break;
    case SND_SEQ_EVENT_CONTINUE:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( INFO ) << "Queue continue: " << ev->data.queue.queue << "\n";
        else
            LOG( INFO ) << "Continue\n";
        break;
    case SND_SEQ_EVENT_STOP:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( INFO ) << "Queue stop: " << ev->data.queue.queue << "\n";
        else
            LOG( INFO ) << "Stop\n";
        break;
    case SND_SEQ_EVENT_SETPOS_TICK:
        LOG( INFO ) << "Set tick queue pos: " << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_SETPOS_TIME:
        LOG( INFO ) << "Set rt queue pos: " <<  ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_TEMPO:
        LOG( INFO ) << "Set queue tempo: " << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_CLOCK:
        LOG( INFO ) << "Clock\n";
        break;
    case SND_SEQ_EVENT_TICK:
        LOG( INFO ) << "Tick\n";
        break;
    case SND_SEQ_EVENT_QUEUE_SKEW:
        LOG( INFO ) << "Queue timer skew:" << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_TUNE_REQUEST:
        LOG( INFO ) << "Tune request\n";
        break;
    case SND_SEQ_EVENT_RESET:
        LOG( INFO ) << "Reset\n";
        break;
    case SND_SEQ_EVENT_SENSING:
        LOG( INFO ) << "Active Sensing\n";
        break;
    case SND_SEQ_EVENT_CLIENT_START:
        LOG( INFO ) << "Client start: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_CLIENT_EXIT:
        LOG( INFO ) << "Client exit: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_CLIENT_CHANGE:
        LOG( INFO ) << "Client changed: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_PORT_START:
        LOG( INFO ) << "Port start: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_EXIT:
        LOG( INFO ) << "Port exit: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_CHANGE:
        LOG( INFO ) << "Port changed: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_SUBSCRIBED:
        LOG( INFO ) << fmt::format( "Port subscribed: {:d}:{:d} -> {:d}:{:d}\n",
            ev->data.connect.sender.client, ev->data.connect.sender.port,
            ev->data.connect.dest.client, ev->data.connect.dest.port );
        break;
    case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
        LOG( INFO ) << fmt::format( "Port unsubscribed: {:d}:{:d} -> {:d}:{:d}\n",
            ev->data.connect.sender.client, ev->data.connect.sender.port,
            ev->data.connect.dest.client, ev->data.connect.dest.port );
        break;
    case SND_SEQ_EVENT_SYSEX:
        {
            unsigned int i;
            LOG( INFO ) << "System exclusive: ";
            for (i = 0; i < ev->data.ext.len; ++i)
                LOG( NONE ) << static_cast<unsigned char *>(ev->data.ext.ptr)[i];
            LOG( INFO ) << "\n";
        }
        break;
    default:
        LOG( INFO ) << "Event type: " << ev->type << "\n";
    }
    return result;
}

AlsaSeq::~AlsaSeq() {
    fina();
}
