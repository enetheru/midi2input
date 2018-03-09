#include "alsa.h"
#include "util.h"

using namespace m2i;

namespace snd {

int
Seq::open()
{
    if( snd_seq_open( &seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK ) < 0 )
        return -1;
    client_id = snd_seq_client_id( seq );
    snd_seq_set_client_name( seq, "midi2input_alsa" );

    if( !seq ){
        LOG( ALSA ) << "ERROR: Sequencer not initialised\n";
        return -1;
    }

    iport_id = snd_seq_create_simple_port(
        seq,
        "in",
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( iport_id < 0 ){
        LOG( ALSA ) << "ERROR: Problem creating input midi port\n";
        return -1;
    }

    oport_id = snd_seq_create_simple_port(
        seq,
        "out",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( oport_id < 0 ){
        LOG( ALSA ) << "ERROR: Problem creating output midi port\n";
        return -1;
    }
    return client_id;
}

void
Seq::close()
{
    if( seq ){
        snd_seq_delete_simple_port( seq, iport_id );
        snd_seq_delete_simple_port( seq, oport_id );
        snd_seq_close( seq );
    }
    iport_id = -1;
    oport_id = -1;
    client_id = -1;
    seq = nullptr;
}

int
Seq::connect( const std::string &client_name, const std::string &port_name )
{
    int connections = 0;
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
            continue;
        }
        if( client_name == "*" ){
            LOG( ALSA ) << "client wildcard match: " << snd_seq_client_info_get_name( cinfo ) << "\n";
        }
        else if( client_name == snd_seq_client_info_get_name( cinfo ) ){
            LOG( ALSA ) << "client name match: " << snd_seq_client_info_get_name( cinfo ) << "\n";
        }
        else continue;

        int client = snd_seq_client_info_get_client( cinfo );
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while( snd_seq_query_next_port( seq, pinfo ) >= 0 ){
            if( port_name == "*" )
                LOG( ALSA ) << "port wildcard match: " << snd_seq_port_info_get_name( pinfo ) << "\n";
            else if( port_name == snd_seq_port_info_get_name( pinfo ) )
                LOG( ALSA ) << "port name match: " << snd_seq_port_info_get_name( pinfo ) << "\n";
            else continue;

            auto capabilities = snd_seq_port_info_get_capability(pinfo);
            if( capabilities & (SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ) ){
                LOG( ALSA ) << "port connect from: " << snd_seq_port_info_get_name( pinfo ) << "\n";
                snd_seq_connect_from( seq, iport_id, client, snd_seq_port_info_get_port( pinfo ) );
                connections++;
            }
            if( capabilities & (SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE) ){
                LOG( ALSA ) << "port connect to: " << snd_seq_port_info_get_name( pinfo ) << "\n";
                snd_seq_connect_to( seq, oport_id, client, snd_seq_port_info_get_port( pinfo ) );
                connections++;
            }
        }
    }
    return connections;
}

void
Seq::event_send( const midi_event &event )
{
    snd_seq_event_t ev;
    snd_seq_ev_clear( &ev );
    snd_seq_ev_set_source( &ev, oport_id );
    snd_seq_ev_set_subs( &ev );
    snd_seq_ev_set_direct( &ev );

    switch( event.status & 0xF0 ){
    case 0x80:
        snd_seq_ev_set_noteoff( &ev, event.status & 0x0F, event.data1, event.data2 );
        LOG( ALSA ) << fmt::format( "send_noteoff: {}\n", event.str() );
        break;
    case 0x90:
        snd_seq_ev_set_noteon( &ev, event.status & 0x0F, event.data1, event.data2 );
        LOG( ALSA ) << fmt::format( "send_noteon: {}\n", event.str() );
        break;
    case 0xB0:
        snd_seq_ev_set_controller( &ev, event.status & 0x0F, event.data1, event.data2 );
        LOG( ALSA ) << fmt::format( "send_control: {}\n", event.str() );
        break;
    default:
        LOG( ALSA ) << fmt::format( "send type({}) not supported", event.status & 0xF0 );
        return;
    }

    snd_seq_event_output( seq, &ev);
    snd_seq_drain_output( seq );
}

int
Seq::event_pending(){
    return snd_seq_event_input_pending( seq, 1 );
}

midi_event
Seq::event_receive()
{
    midi_event result = { 0, 0, 0 };
    snd_seq_event_t *ev = nullptr;
    if( snd_seq_event_input( seq, &ev ) < 0)
        return result;

    switch( ev->type ){
    case SND_SEQ_EVENT_NOTEON:
        LOG( ALSA ) << fmt::format( "Note On: {:#04x}, {:#04x}, {:3d}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x90;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_NOTEOFF:
        LOG( ALSA ) << fmt::format( "Note Off: {:#04x}, {:#04x}, {:3d}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x80;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_KEYPRESS:
        LOG( ALSA ) << fmt::format( "Polyphonic aftertouch: {}, {}, {}\n",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        break;
    case SND_SEQ_EVENT_CONTROLLER:
        LOG( ALSA ) << fmt::format( "Control Change: {:#04x}, {:#04x}, {:3d}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        result.status = ev->data.control.channel + 0xB0;
        result.data1 = ev->data.control.param;
        result.data2 = ev->data.control.value;
        break;
    case SND_SEQ_EVENT_PGMCHANGE:
        LOG( ALSA ) << fmt::format( "Program change: {}, {}\n",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_CHANPRESS:
        LOG( ALSA ) << fmt::format( "Channel aftertouch: {}, {}\n",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_PITCHBEND:
        LOG( ALSA ) << fmt::format( "Pitch bend: {}, {}\n",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_CONTROL14:
        LOG( ALSA ) << fmt::format( "Control change: {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_NONREGPARAM:
        LOG( ALSA ) << fmt::format( "Non-reg. parameter: {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_REGPARAM:
        LOG( ALSA ) << fmt::format( "Reg. parameter {}, {}, {}\n",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_SONGPOS:
        LOG( ALSA ) << "Song position pointer: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_SONGSEL:
        LOG( ALSA ) << "Song select: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_QFRAME:
        LOG( ALSA ) << "MTC quarter frame: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_TIMESIGN:
        LOG( ALSA ) << "SMF time signature: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_KEYSIGN:
        LOG( ALSA ) << "SMF key signature: " << ev->data.control.value << "\n";
        break;
    case SND_SEQ_EVENT_START:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( ALSA ) << "Queue start: " << ev->data.queue.queue << "\n";
        else
            LOG( ALSA ) << "Start\n";
        break;
    case SND_SEQ_EVENT_CONTINUE:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( ALSA ) << "Queue continue: " << ev->data.queue.queue << "\n";
        else
            LOG( ALSA ) << "Continue\n";
        break;
    case SND_SEQ_EVENT_STOP:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            LOG( ALSA ) << "Queue stop: " << ev->data.queue.queue << "\n";
        else
            LOG( ALSA ) << "Stop\n";
        break;
    case SND_SEQ_EVENT_SETPOS_TICK:
        LOG( ALSA ) << "Set tick queue pos: " << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_SETPOS_TIME:
        LOG( ALSA ) << "Set rt queue pos: " <<  ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_TEMPO:
        LOG( ALSA ) << "Set queue tempo: " << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_CLOCK:
        LOG( ALSA ) << "Clock\n";
        break;
    case SND_SEQ_EVENT_TICK:
        LOG( ALSA ) << "Tick\n";
        break;
    case SND_SEQ_EVENT_QUEUE_SKEW:
        LOG( ALSA ) << "Queue timer skew:" << ev->data.queue.queue << "\n";
        break;
    case SND_SEQ_EVENT_TUNE_REQUEST:
        LOG( ALSA ) << "Tune request\n";
        break;
    case SND_SEQ_EVENT_RESET:
        LOG( ALSA ) << "Reset\n";
        break;
    case SND_SEQ_EVENT_SENSING:
        LOG( ALSA ) << "Active Sensing\n";
        break;
    case SND_SEQ_EVENT_CLIENT_START:
        LOG( ALSA ) << "Client start: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_CLIENT_EXIT:
        LOG( ALSA ) << "Client exit: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_CLIENT_CHANGE:
        LOG( ALSA ) << "Client changed: " << ev->data.addr.client << "\n";
        break;
    case SND_SEQ_EVENT_PORT_START:
        LOG( ALSA ) << "Port start: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_EXIT:
        LOG( ALSA ) << "Port exit: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_CHANGE:
        LOG( ALSA ) << "Port changed: "
            << ev->data.addr.client << ","
            << ev->data.addr.port << "\n";
        break;
    case SND_SEQ_EVENT_PORT_SUBSCRIBED:
        {
            snd_seq_client_info_t *cinfo;
            snd_seq_client_info_alloca( &cinfo );
            snd_seq_port_info_t *pinfo;
            snd_seq_port_info_alloca( &pinfo );

            snd_seq_get_any_client_info( seq,
                ev->data.connect.sender.client ,
                cinfo );
            snd_seq_get_any_port_info( seq,
                ev->data.connect.sender.client,
                ev->data.connect.sender.port,
                pinfo );

            LOG( ALSA ) << fmt::format( "Port Subscription from: '{}':'{}'\n",
                snd_seq_client_info_get_name( cinfo ),
                snd_seq_port_info_get_name( pinfo )
            );
            LOG( ALSA ) << fmt::format( "Port Subscribed: {:d}:{:d} -> {:d}:{:d}\n",
                ev->data.connect.sender.client, ev->data.connect.sender.port,
                ev->data.connect.dest.client, ev->data.connect.dest.port );
        }
        break;
    case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
        LOG( ALSA ) << fmt::format( "Port unsubscribed: {:d}:{:d} -> {:d}:{:d}\n",
            ev->data.connect.sender.client, ev->data.connect.sender.port,
            ev->data.connect.dest.client, ev->data.connect.dest.port );
        break;
    case SND_SEQ_EVENT_SYSEX:
        {
            unsigned int i;
            LOG( ALSA ) << "System exclusive: ";
            for (i = 0; i < ev->data.ext.len; ++i)
                LOG( NONE ) << static_cast<unsigned char *>(ev->data.ext.ptr)[i];
            LOG( ALSA ) << "\n";
        }
        break;
    default:
        LOG( ALSA ) << "Event type: " << ev->type << "\n";
    }
    return result;
}

Seq::~Seq() {
    close();
}

}//end namespace snd
