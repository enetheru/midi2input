#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>

#include <wordexp.h>
#include <unistd.h>

extern "C" {
	#include <lua5.2/lua.h>
	#include <lua5.2/lauxlib.h>
	#include <lua5.2/lualib.h>
}

#include <jack/jack.h>
#include <jack/midiport.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "options.h"

#include "elog/elog.h"

lua_State *L;
Display* xdp;
jack_client_t *client;
jack_port_t *input_port;
jack_port_t *output_port;
std::string wm_class;

/* options */
enum optionsIndex
{
    UNKNOWN,
    HELP,
    CONFIG
};

const option::Descriptor usage[] = {
    { UNKNOWN, 0, "", "", Arg::Nope,
        "USAGE: ./midi2input [options]\n"
        "\nGENERAL OPTIONS:"
    },

    { HELP, 0, "h", "help", Arg::Nope,
    	"  -h  \t--help "
        "\tPrint usage and exit."
    },

    { CONFIG, 0, "c", "config", Arg::Required,
    	"  -c  \t--config "
    	"\tSpecify config file, default = ~/.config/midi2input.lua"
    },
    {0,0,0,0,0,0}
};

static int
lua_keypress( lua_State *L )
{
	int keysym = luaL_checknumber( L, 1 );
	int keycode = XKeysymToKeycode( xdp, keysym );
	XTestFakeKeyEvent( xdp, keycode, 1, CurrentTime );
	XTestFakeKeyEvent( xdp, keycode, 0, CurrentTime );
	LOG(INFO) << "keypress: " << XKeysymToString( keysym );
	return 0;
}

static int
lua_keydown( lua_State *L )
{
	int keysym = luaL_checknumber( L, 1 );
	int keycode = XKeysymToKeycode( xdp, keysym );
	XTestFakeKeyEvent( xdp, keycode, 1, CurrentTime );
	LOG(INFO) << "keydown: " << XKeysymToString( keysym );
	return 0;
}

static int
lua_keyup( lua_State *L )
{
	int keysym = luaL_checknumber( L, 1 );
	int keycode = XKeysymToKeycode( xdp, keysym );
	XTestFakeKeyEvent( xdp, keycode, 0, CurrentTime );
	LOG(INFO) << "keyup: " << XKeysymToString( keysym );
	return 0;
}

bool
load_config( std::string name )
{
	// load configuration from a priority list of locations
	// * specified from the command line
	// * configuration folder $HOME/.config/
	// * home directory $HOME/
	std::string filepath;
	std::queue<std::string> paths;

	if(! name.empty() ) paths.push( name );

	// configuration folder ~/.config/
	filepath = std::string( getenv( "HOME" ) ) + "/.config/midi2input.lua";
	paths.push( filepath );

	// configuration folder ~/.config/
	filepath = std::string( getenv( "HOME" ) ) + "/.midi2input.lua";
	paths.push( filepath );

	std::ifstream tFile;
	while(! paths.empty() ){
		tFile.open( paths.front().c_str(), std::ifstream::in );
		if( tFile.is_open() ){
			tFile.close();
			break;
		}
		paths.pop();
	}
	if( paths.empty() ) return false;

    if( luaL_loadfile( L, paths.front().c_str() ) || lua_pcall( L, 0, 0, 0 ) ){
		LOG( ERROR ) << "cannot run configuration file: " << lua_tostring( L, -1 );
        return false;
    }
	LOG( INFO ) << "Using: " << paths.front();
	return true;
}

Window
XGetTopLevelParent( Display *xdp, Window w )
{
	std::string wm_name;
	wm_class.clear();

	Atom property;
	Atom actual_type_return;
	int actual_format_return;
	unsigned long nitems_return;
	unsigned long bytes_after_return;
	unsigned char *prop_return;

	property = XInternAtom( xdp, "WM_CLASS", False );
	if( XGetWindowProperty( xdp, w, property, 0, 1024, False, AnyPropertyType,
				&actual_type_return, &actual_format_return,
				&nitems_return, &bytes_after_return, &prop_return ) == Success ){
		if( actual_format_return == 8 )
			wm_class = reinterpret_cast<const char *>(prop_return);
		XFree( prop_return );
	}

	property = XInternAtom( xdp, "WM_NAME", False );
	if( XGetWindowProperty( xdp, w, property, 0, 1024, False, AnyPropertyType,
				&actual_type_return, &actual_format_return,
				&nitems_return, &bytes_after_return, &prop_return ) == Success ){
		if( actual_format_return == 8 )
			wm_name = reinterpret_cast<const char *>(prop_return);
		XFree( prop_return );
	}

	Window root_return;
	Window parent_return;
	Window *children_return;
	unsigned int nchildren_return;

	if( wm_class.empty() ){
		if( XQueryTree( xdp, w, &root_return, &parent_return,
				&children_return, &nchildren_return ) ){
			if( children_return ) XFree( children_return );
			w = XGetTopLevelParent( xdp, parent_return );
		}
	}
	else {
		lua_pushstring( L , wm_class.c_str() );
		lua_setglobal( L, "wm_class" );
		LOG( INFO ) << "WM_CLASS: " << wm_class << " | WM_NAME: " << wm_name;
	}

	return w;
}

int
process( jack_nframes_t nframes, void *arg )
{
	//detect window
	static Window w_current;
	Window w;
	int revert_to;

	XGetInputFocus( xdp, &w, &revert_to );
	if( w != None && w != w_current ){
		w_current = w;
		XGetTopLevelParent( xdp, w );
	}

	//process midi events
	void* port_buf = jack_port_get_buffer( input_port, nframes );
	jack_midi_event_t in_event;
	static jack_midi_event_t last_event;
	static unsigned char last_buffer[4];
	last_event.buffer = last_buffer;

	unsigned char status, type = 0, channel = 0;
	unsigned char control, note;
	unsigned char velocity;
	unsigned int value = 0;

	jack_nframes_t event_count = jack_midi_get_event_count( port_buf );
	if( event_count > 0 ){
		LOG( INFO ) << event_count << " Events in queue.";
		for(uint32_t i = 0; i < event_count; i++ ){
			jack_midi_event_get( &in_event, port_buf, i );
			status = in_event.buffer[ 0 ];

			//note on/off
			if( (status & 0xF0) == 0x80
			 || (status & 0xF0) == 0x90 ){
				if( in_event.size != 3 ){
					LOG (WARN ) << "midi event size unexpected";
					continue;
				}
				type = status & 0xF0;
				channel = (status & 0x0F) + 1;
				note = in_event.buffer[ 1 ];
				velocity = in_event.buffer[ 2 ];
				LOG( INFO ) << "time=" <<  in_event.time
					<< " size=" << in_event.size
					<< " Note On/Off, Channel=" << channel
					<< " note=" << note
					<< " velocity=" << velocity;

				//push values to lua
				lua_getglobal( L, "event_in" );
				lua_pushnumber( L, type );
				lua_pushnumber( L, channel );
				lua_pushnumber( L, note );
				lua_pushnumber( L, velocity );
				if( lua_pcall( L, 4, 0, 0 ) != 0 )
					LOG( ERROR ) << "call to function 'event_in' failed"
						<< lua_tostring( L, -1 );
				
			}
			
			// control/mode change
			if( (status & 0xF0) == 0xB0 ){
				type = status & 0xF0;
				channel = (status & 0x0F) + 1;
				control = in_event.buffer[ 1 ];
				value = in_event.buffer[ 2 ];

				//MSB and LSB for first 32/64 controls sort of works.
				// look at the previous event
				if( control < 64
				 && status == last_event.buffer[ 0 ] 
				 && control == last_event.buffer[ 1 ] + 32 ){
					value = last_event.buffer[ 2 ];
					value = value << 8;
					value += in_event.buffer[ 2 ];
				}

				LOG( INFO ) << "time=" << in_event.time
					<< " size=" << in_event.size
					<< " Control/Mode Change, channel=" << channel
					<< " control=" << control
					<< " value=" << value;

				//push values to lua
				lua_getglobal( L, "event_in" );
				lua_pushnumber( L, type );
				lua_pushnumber( L, channel );
				lua_pushnumber( L, control );
				lua_pushnumber( L, value );
				if( lua_pcall( L, 4, 0, 0 ) != 0 )
					LOG( ERROR ) << "call to function 'event_in' failed"
						<< lua_tostring( L, -1 );
			}

			// hold onto last event so we can check it against next event for
			// LSB/MSB events.
			last_event.size = in_event.size;
			last_event.time = in_event.time;
			memcpy( last_buffer, in_event.buffer, in_event.size );
		}
	}
	return 0;
}

void
jack_shutdown( void *arg )
{
	exit( 1 );
}

int
main( int argc, char** argv )
{

	// Options Parsing
    // ===============
    bool fail = false;
    argc -= (argc > 0); argv += (argc > 0);
    option::Stats stats( usage, argc, argv );
    option::Option* options = new option::Option[ stats.options_max ];
    option::Option* buffer = new option::Option[ stats.buffer_max ];
    option::Parser parse( usage, argc, argv, options, buffer );
    
    // setup logging level.
    LOG::SetDefaultLoggerLevel( LOG::INFO );
    //if( options[ VERBOSE ] )
    //    LOG::SetDefaultLoggerLevel( LOG::INFO );
    //if( options[ QUIET ] )
    //    LOG::SetDefaultLoggerLevel( LOG::CHECK );

    if( options[ HELP ] ){
        int columns = getenv( "COLUMNS" ) ? atoi( getenv( "COLUMNS" ) ) : 80;
        option::printUsage( std::cout, usage, columns );
        exit( 0 );
    }

    // unknown options
    for( option::Option* opt = options[ UNKNOWN ]; opt; opt = opt->next() ){
        LOG( WARN ) << "Unknown option: " << std::string( opt->name, opt->namelen );
        fail = true;
    }

    if( fail ) exit( 1 );

    // --config
	LOG( INFO ) << "Parsing cmd line options";
    std::string luaScript;
    if( options[ CONFIG ] ) luaScript = options[ CONFIG ].arg;
    else luaScript = "~/.config/midi2input.lua";

	/* X11 */
	LOG( INFO ) << "Getting X11 Display";
	if(! (xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
		LOG( FATAL ) << "Unable to open X display";
	}

	/* Lua */
	LOG( INFO ) << "Initialising Lua";
	L = luaL_newstate();
    luaL_openlibs( L );

	LOG( INFO ) << "Lua: pushing c functions";
	lua_pushcfunction( L, lua_keypress );
	lua_setglobal( L, "keypress" );

	lua_pushcfunction( L, lua_keydown );
	lua_setglobal( L, "keydown" );

	lua_pushcfunction( L, lua_keyup );
	lua_setglobal( L, "keyup" );

	LOG( INFO ) << "Lua: Loading configuration file";
	if(! load_config( luaScript.c_str() ) ){
		LOG( FATAL ) << "Unable to open configuration file";
	}

	/* Jack */
	LOG( INFO ) << "Initialising Jack";
	if( (client = jack_client_open( "midi2input", JackNullOption, NULL )) == 0 ){
		LOG( FATAL ) << "jack server not running?";
	}

	LOG( INFO ) << "Jack: setting callbacks";
	jack_set_process_callback( client, process, 0 );
	jack_on_shutdown( client, jack_shutdown, 0 );

	LOG( INFO ) << "Jack: registering ports";
	input_port = jack_port_register( client, "midi_in",
			JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );
	output_port = jack_port_register( client, "midi_out",
			JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0 );

	LOG( INFO ) << "Jack: Activating client";
	if( jack_activate( client ) ){
		LOG( FATAL ) << "cannot activate client";
	}

	/* Lua: get port connection configuration */
	LOG( INFO ) << "Lua: Getting autoconnection setting";
	std::string autoconnect;
	lua_getglobal( L, "autoconnect" );
	if(! lua_isnil( L, -1 ) ){
		if( lua_isboolean( L, -1 ) ){
			if(! lua_toboolean( L, -1 ) ){
				autoconnect = "None";
			}
		}
		else if( lua_isnumber( L, -1 ) ){
			LOG( WARN ) << "parse error for 'autoconnect'";
		}
		else if( lua_isstring( L, -1 ) ){
			autoconnect = lua_tostring( L, -1);
		} else {
			LOG( WARN ) << "parse error for 'autoconnect'\n";
		}
	}
	if( autoconnect.empty() ){
		autoconnect = "All";
	}
	LOG( INFO ) << "Lua: autoconnect = " << autoconnect;
	lua_pop( L, 1 );

	/* Jack: get list of output ports */
	LOG( INFO ) << "Jack: Looking up output ports";
	int i = 0;
	const char ** portnames = jack_get_ports( client, ".midi.", NULL, JackPortIsOutput );
	if(! portnames ){
		LOG( FATAL ) << "ERROR: no ports available";
	}

	while( *(portnames + i) ){
		LOG( INFO ) << "Jack: Found: " << *(portnames+i);
		i++;
	}

	/* Jack: Connect to output ports */
	LOG( INFO ) << "Jack: Connecting to output ports";
	std::stringstream message;
	std::string portname;
	if( autoconnect.compare( "None" ) ){
		if(! autoconnect.compare( "All" ) ){
			if(! portnames ) LOG( FATAL ) << "No ports to connect to";
			i = 0;
			while( *(portnames + i) ){
				portname = *(portnames + i);
				if(! portname.compare("midi2input:midi_out") ){
					++i;
					continue;
				}
				message.str( std::string() );
				message << "Connecting to: " << portname;
				if( jack_connect( client, portname.c_str(), "midi2input:midi_in" ) ){
					message << " - FAILED";
				} else {
					message << " - SUCCESS";
				}
				LOG( INFO ) << message.str();
				++i;
			}
		} else {
			message << "Connecting to: " << autoconnect;
			if( jack_connect( client, autoconnect.c_str(), "midi2input:midi_in" ) ){
					message << " - FAILED\n";
			} else {
					message << " - SUCCESS\n";
			}
			LOG( INFO ) << message;
		}
	}
	jack_free( portnames );

	/* Jack: get list of input ports */
	LOG( INFO ) << "Jack: Looking up input ports";
	i = 0;
	portnames = jack_get_ports( client, ".midi.", NULL, JackPortIsInput );
	if(! portnames ){
		LOG( FATAL ) << "ERROR: no ports available";
	}
	while( *(portnames + i) ){
		LOG( INFO ) << "Jack: Found: " << *(portnames+i);
		i++;
	}

	/* Jack: Connect to input ports */
	LOG( INFO ) << "Jack: Connecting to input ports";
	if(! portnames ) LOG( FATAL ) << "No ports to connect to";
	i = 0;
	while( *(portnames + i) ){
		portname = *(portnames + i);
		if(! portname.compare("midi2input:midi_in") ){
			++i;
			continue;
		}
		message.str( std::string() );
		message << "Connecting to: " << *(portnames + i);
		if( jack_connect( client, *(portnames + i), "midi2input:midi_out" ) ){
			message << " - FAILED";
		} else {
			message << " - SUCCESS";
		}
		LOG( INFO ) << message.str();
		i++;
	}
	jack_free( portnames );


	/* main loop */
	LOG( INFO ) << "Main: Entering sleep, waiting for jack events";
	while( 1 ) sleep( 1 );

	jack_client_close( client );
	lua_close( L );
	exit( 0 );
}


