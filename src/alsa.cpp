#include <spdlog/spdlog.h>

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
        spdlog::error("ALSA: Sequencer not initialised" );
        return -1;
    }

    iport_id = snd_seq_create_simple_port(
        seq,
        "in",
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( iport_id < 0 ){
        spdlog::error( "ALSA: Problem creating input midi port" );
        return -1;
    }

    oport_id = snd_seq_create_simple_port(
        seq,
        "out",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION
    );
    if( oport_id < 0 ){
        spdlog::error( "ALSA: Problem creating output midi port" );
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
            spdlog::info( "ALSA: client wildcard match: {}", snd_seq_client_info_get_name( cinfo ) );
        }
        else if( client_name == snd_seq_client_info_get_name( cinfo ) ){
            spdlog::info( "ALSA: client name match: {}",  snd_seq_client_info_get_name( cinfo ) );
        }
        else continue;

        int client = snd_seq_client_info_get_client( cinfo );
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while( snd_seq_query_next_port( seq, pinfo ) >= 0 ){
            if( port_name == "*" )
                spdlog::info( "ALSA: port wildcard match: {}", snd_seq_port_info_get_name( pinfo ) );
            else if( port_name == snd_seq_port_info_get_name( pinfo ) )
                spdlog::info( "ALSA: port name match: {}", snd_seq_port_info_get_name( pinfo ) );
            else continue;

            auto capabilities = snd_seq_port_info_get_capability(pinfo);
            if( capabilities & (SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ) ){
                spdlog::info( "ALSA: port connect from: {}", snd_seq_port_info_get_name( pinfo ) );
                snd_seq_connect_from( seq, iport_id, client, snd_seq_port_info_get_port( pinfo ) );
                connections++;
            }
            if( capabilities & (SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE) ){
                spdlog::info( "ALSA: port connect to: {}", snd_seq_port_info_get_name( pinfo ) );
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
        spdlog::info( "ALSA: send_noteoff: {}\n", event.str() );
        break;
    case 0x90:
        snd_seq_ev_set_noteon( &ev, event.status & 0x0F, event.data1, event.data2 );
        spdlog::info("ALSA: send_noteon: {}\n", event.str() );
        break;
    case 0xB0:
        snd_seq_ev_set_controller( &ev, event.status & 0x0F, event.data1, event.data2 );
        spdlog::info("ALSA: send_control: {}\n", event.str() );
        break;
    default:
        spdlog::info("ALSA: send type({}) not supported", event.status & 0xF0 );
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
        spdlog::info("ALSA: Note On: {:#04x}, {:#04x}, {:3d}",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x90;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_NOTEOFF:
        spdlog::info("ALSA: Note Off: {:#04x}, {:#04x}, {:3d}",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        result.status = ev->data.note.channel + 0x80;
        result.data1 = ev->data.note.note;
        result.data2 = ev->data.note.velocity;
        break;
    case SND_SEQ_EVENT_KEYPRESS:
        spdlog::info("ALSA: Polyphonic aftertouch: {}, {}, {}",
            ev->data.note.channel, ev->data.note.note, ev->data.note.velocity );
        break;
    case SND_SEQ_EVENT_CONTROLLER:
        spdlog::info("ALSA: Control Change: {:#04x}, {:#04x}, {:3d}",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        result.status = ev->data.control.channel + 0xB0;
        result.data1 = ev->data.control.param;
        result.data2 = ev->data.control.value;
        break;
    case SND_SEQ_EVENT_PGMCHANGE:
        spdlog::info("ALSA: Program change: {}, {}",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_CHANPRESS:
        spdlog::info("ALSA: Channel aftertouch: {}, {}",
            ev->data.control.channel, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_PITCHBEND:
        spdlog::info("ALSA: Pitch bend: {}, {}",
            ev->data.control.channel, ev->data.control.value );
        result.status = ev->data.control.channel + 0xE0;
        //FIXME this really sucks i'm throwing away so much detail it breaks my heart.
        //also its just shitty shitty code
        result.data2 = (ev->data.control.value  + 8192) / 64;
        break;
    case SND_SEQ_EVENT_CONTROL14:
        spdlog::info("ALSA: Control change: {}, {}, {}",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_NONREGPARAM:
        spdlog::info("ALSA: Non-reg. parameter: {}, {}, {}",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_REGPARAM:
        spdlog::info("ALSA: Reg. parameter {}, {}, {}",
            ev->data.control.channel, ev->data.control.param, ev->data.control.value );
        break;
    case SND_SEQ_EVENT_SONGPOS:
        spdlog::info("ALSA: Song position pointer: {}", ev->data.control.value );
        break;
    case SND_SEQ_EVENT_SONGSEL:
        spdlog::info("ALSA: Song select: {}", ev->data.control.value );
        break;
    case SND_SEQ_EVENT_QFRAME:
        spdlog::info("ALSA: MTC quarter frame: {}", ev->data.control.value );
        break;
    case SND_SEQ_EVENT_TIMESIGN:
        spdlog::info("ALSA: SMF time signature: {}", ev->data.control.value );
        break;
    case SND_SEQ_EVENT_KEYSIGN:
        spdlog::info("ALSA: SMF key signature: {}", ev->data.control.value );
        break;
    case SND_SEQ_EVENT_START:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            spdlog::info("ALSA: Queue start: {}", ev->data.queue.queue );
        else
            spdlog::info( "ALSA: Start" );
        break;
    case SND_SEQ_EVENT_CONTINUE:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            spdlog::info("ALSA: Queue continue: {}", ev->data.queue.queue );
        else
            spdlog::info("ALSA: Continue" );
        break;
    case SND_SEQ_EVENT_STOP:
        if( ev->source.client == SND_SEQ_CLIENT_SYSTEM &&
            ev->source.port == SND_SEQ_PORT_SYSTEM_TIMER )
            spdlog::info("ALSA: Queue stop: {}", ev->data.queue.queue );
        else
            spdlog::info("ALSA: Stop" );
        break;
    case SND_SEQ_EVENT_SETPOS_TICK:
        spdlog::info("ALSA: Set tick queue pos: {}", ev->data.queue.queue );
        break;
    case SND_SEQ_EVENT_SETPOS_TIME:
        spdlog::info("ALSA: Set rt queue pos: {}", ev->data.queue.queue );
        break;
    case SND_SEQ_EVENT_TEMPO:
        spdlog::info("ALSA: Set queue tempo: {}", ev->data.queue.queue );
        break;
    case SND_SEQ_EVENT_CLOCK:
        spdlog::info("ALSA: Clock" );
        break;
    case SND_SEQ_EVENT_TICK:
        spdlog::info("ALSA: Tick" );
        break;
    case SND_SEQ_EVENT_QUEUE_SKEW:
        spdlog::info("ALSA: Queue timer skew: {}", ev->data.queue.queue );
        break;
    case SND_SEQ_EVENT_TUNE_REQUEST:
        spdlog::info("ALSA: Tune request" );
        break;
    case SND_SEQ_EVENT_RESET:
        spdlog::info("ALSA: Reset" );
        break;
    case SND_SEQ_EVENT_SENSING:
        spdlog::info( "ALSA: Active Sensing" );
        break;
    case SND_SEQ_EVENT_CLIENT_START:
        spdlog::info( "ALSA: Client start: {}", ev->data.addr.client );
        break;
    case SND_SEQ_EVENT_CLIENT_EXIT:
        spdlog::info( "ALSA: Client exit: {}", ev->data.addr.client );
        break;
    case SND_SEQ_EVENT_CLIENT_CHANGE:
        spdlog::info( "ALSA: Client changed: {}", ev->data.addr.client );
        break;
    case SND_SEQ_EVENT_PORT_START:
        spdlog::info("ALSA: Port start: {},{}", ev->data.addr.client, ev->data.addr.port );
        break;
    case SND_SEQ_EVENT_PORT_EXIT:
        spdlog::info( "ALSA: Port exit: {},{}", ev->data.addr.client, ev->data.addr.port );
        break;
    case SND_SEQ_EVENT_PORT_CHANGE:
        spdlog::info( "ALSA: Port changed: {},{}", ev->data.addr.client, ev->data.addr.port );
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

            spdlog::info( "ALSA: Port Subscription from: '{}':'{}'",
                snd_seq_client_info_get_name( cinfo ),
                snd_seq_port_info_get_name( pinfo )
            );
            spdlog::info( "ALSA: Port Subscribed: {:d}:{:d} -> {:d}:{:d}",
                ev->data.connect.sender.client, ev->data.connect.sender.port,
                ev->data.connect.dest.client, ev->data.connect.dest.port );
        }
        break;
    case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
        spdlog::info("ALSA: Port unsubscribed: {:d}:{:d} -> {:d}:{:d}",
            ev->data.connect.sender.client, ev->data.connect.sender.port,
            ev->data.connect.dest.client, ev->data.connect.dest.port );
        break;
    case SND_SEQ_EVENT_SYSEX:
        {
            unsigned int i;
            spdlog::info( "ALSA:System exclusive: ");
            for (i = 0; i < ev->data.ext.len; ++i)
                fmt::format( "{}", static_cast<unsigned char *>(ev->data.ext.ptr)[i] );
            fmt::format("\n");
        }
        break;
    default:
        spdlog::info("ALSA: Event type: {}", ev->type );
    }
    return result;
}

Seq::~Seq() {
    close();
}

}//end namespace snd
