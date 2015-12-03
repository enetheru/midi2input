#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>
#include <string>

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

lua_State *L;
Display* xdp;
jack_client_t *client;
jack_port_t *input_port;

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

// Fake keypress, sends a keypress to the xwindows system
void
fake_keypress( const char *keysym )
{
	KeyCode keycode = XStringToKeysym( keysym );
	keycode = XKeysymToKeycode( xdp, keycode );
	std::cout << "sent keypress with code: " << keycode << std::endl;
	XTestFakeKeyEvent( xdp, (int)keycode, 1, CurrentTime );
	XTestFakeKeyEvent( xdp, (int)keycode, 0, CurrentTime );
	XFlush( xdp );
	return;
}
/*
void fake_keydown
void fake_keyup
void fake_mousemove

*/
void
handle_jack_midi_event( jack_midi_event_t &in_event )
{
	unsigned char event[ 4 ];
	event[ 0 ] = in_event.buffer[ 0 ] >> 4;
	event[ 1 ] = (in_event.buffer[ 0 ] & 0x0F) + 1;
	event[ 2 ] = in_event.buffer[ 1 ];
	event[ 3 ] = in_event.buffer[ 2 ];

	std::cout << "type: ";
	switch( event[ 0 ] ){
		case 0x9:
			std::cout << "note on"; break;
		case 0x8:
			std::cout << "note off"; break;
		case 0xA:
			std::cout << "polyphonic aftertouch"; break;
		case 0xB:
			std::cout << "control/mode change"; break;
		case 0xC:
			std::cout << "program change"; break;
		case 0xD:
			std::cout << "channel aftertouch"; break;
		case 0xE:
			std::cout << "pitch bend change"; break;
		case 0xF:
			std::cout << "system"; break;
	}
	std::cout << " | channel: " << std::setw(2) << (int)event[ 1 ]
		<< " | Note: " << std::setw(3) << (int)event[ 2 ]
		<< " | Velocity: "
		<< std::setw(3) << (int)event[ 3 ]
		<< std::endl;

	/* table is in the stack at index 't' */
	lua_getglobal( L, "map" );
	lua_pushnil( L );  /* first key */
	while( lua_next(L, -2) != 0 ){
		/* uses 'key' (at index -2) and 'value' (at index -1) */

		// get midi table
		lua_pushnumber( L, 1 );
		lua_gettable( L, -2 );

		// check values
		int i = 0;
		while( i < 4 ){
			lua_pushnumber( L, (i + 1) );
			lua_gettable( L, -2 );
			int temp = lua_tonumber( L, -1 );
			lua_pop( L, 1 );
			if( temp == -1 ){ ++i; continue; }
			std::cout << " test " << i << "(" << temp << " ?= " << (int)event[ i ] << ")";
			if( temp != event[ i ] )break;
			++i;
		}
		if( i != 4 ){
			lua_pop( L, 2 ); // pops back to map table
			std::cout << std::endl;
			continue;
		}

		// exit midi table
		lua_pop( L, 1);

		// get action table
		lua_pushnumber( L, 2 );
		lua_gettable( L, -2 );
		unsigned int mask = 0;
		const char *keysym = NULL;
		// first value(modifyer mask);
			lua_pushnumber( L, 1 );
			lua_gettable( L, -2 );
			mask = lua_tonumber( L, -1 );
			lua_pop( L, 1 );
		// second value(keycode)
			lua_pushnumber( L, 2 );
			lua_gettable( L, -2 );
			keysym = lua_tostring( L, -1 );
			lua_pop( L, 1 );
		std::cout << "(" << mask << ", " << keysym << ")\n";
		fake_keypress( keysym );

		lua_pop( L, 1 ); //pop action table

		/* removes 'value'; keeps 'key' for next iteration */
		lua_pop( L, 1 );
    }

	lua_pop( L, 1 ); //pop 'map'
	return;
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
		std::cerr << "ERROR: cannot run configuration file: " << lua_tostring( L, -1 )
			<< std::endl;
        return false;
    }
	std::cout << "INFO: Using: " << paths.front() << std::endl;

	return true;
}

int
process( jack_nframes_t nframes, void *arg )
{
	//detect window
	Window w;
	static Window w_current;
	int revert_to;
	XGetInputFocus( xdp, &w, &revert_to );
	if( w != None && w != w_current){
		w_current = w;
		std::cout << "window ID: " << w_current;

		int format;
		unsigned long remain, len;
		unsigned char *list;

		//TODO Xfree() ?
		Atom prop = XInternAtom( xdp, "WM_CLASS", False ), type;
		if( XGetWindowProperty( xdp, w, prop, 0, 1024, False, AnyPropertyType,
					&type, &format, &len, &remain, &list ) == Success ){
			if( format == 8 ) std::cout << "\tWM_CLASS: " << list;
		}
		prop = XInternAtom( xdp, "WM_NAME", False );
		if( XGetWindowProperty( xdp, w, prop, 0, 1024, False, AnyPropertyType,
					&type, &format, &len, &remain, &list ) == Success ){
			if( format == 8 ) std::cout << "\tWM_NAME: " << list;
		}
		std::cout << std::endl;

	}

	//process midi events
	//KeyCode temp;
	void* port_buf = jack_port_get_buffer( input_port, nframes );
	jack_midi_event_t in_event;
	jack_nframes_t event_count = jack_midi_get_event_count( port_buf );

	if( event_count > 0 ){
		for(uint32_t i = 0; i < event_count; i++ ){
			jack_midi_event_get( &in_event, port_buf, i );
			handle_jack_midi_event( in_event );
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

    if( options[ HELP ] ){
        int columns = getenv( "COLUMNS" ) ? atoi( getenv( "COLUMNS" ) ) : 80;
        option::printUsage( std::cout, usage, columns );
        exit( 0 );
    }

    // unknown options
    for( option::Option* opt = options[ UNKNOWN ]; opt; opt = opt->next() ){
        std::cout << "Unknown option: " << std::string( opt->name, opt->namelen );
        fail = true;
    }

    if( fail ) exit( 1 );

    // --config
    std::string luaScript;
    if( options[ CONFIG ] ) luaScript = options[ CONFIG ].arg;
    else luaScript = "~/.config/midi2input.lua";
    std::cout << luaScript << std::endl;

	/*lua*/
	L = luaL_newstate();   /* opens Lua */
    luaL_openlibs( L );

	if(! load_config( luaScript.c_str() ) ){
		std::cerr << "ERROR: Unable to open configuration file\n";
		exit(0);
	}

	/*display*/
	if(! (xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
		std::cerr << "Unable to open X display\n";
		return 2;
	}

	/*jack*/
	if( (client = jack_client_open( "midi2input", JackNullOption, NULL )) == 0 ){
		std::cerr << "jack server not running?\n";
		return 1;
	}

	jack_set_process_callback( client, process, 0 );
	jack_on_shutdown( client, jack_shutdown, 0 );

	input_port = jack_port_register( client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );

	if( jack_activate( client ) ){
		std::cerr << "cannot activate client\n";
		return 1;
	}

	// auto connect to all midi ports available
	std::string autoconnect;
	lua_getglobal( L, "autoconnect" );
	if(! lua_isnil( L, -1 ) ){
		if( lua_isboolean( L, -1 ) ){
			if(! lua_toboolean( L, -1 ) ){
				autoconnect = "None";
			}
		}
		else if( lua_isnumber( L, -1 ) ){
			std::cerr << "ERROR: parse error for 'autoconnect'\n";
		}
		else if( lua_isstring( L, -1 ) ){
			autoconnect = lua_tostring( L, -1);
		} else {
			std::cerr << "ERROR: parse error for 'autoconnect'\n";
		}
	}
	if( autoconnect.empty() ){
		autoconnect = "All";
	}
	std::cout << "autoconnect: " << autoconnect << std::endl;
	lua_pop( L, 1 );

	int i = 0;
	const char ** portnames = jack_get_ports( client, ".midi.", NULL, JackPortIsOutput );
	if(! portnames ){
		std::cerr << "ERROR: no ports available." << std::endl;
		exit( 0 );
	}

	while( *(portnames + i) ){
		std::cout << "found: " << *(portnames+i) << std::endl;
		i++;
	}

	if( autoconnect.compare( "None" ) ){
		if(! autoconnect.compare( "All" ) ){
			if(! portnames ) return 1;
			i = 0;
			while( *(portnames + i) ){
				std::cout << "Connecting to: " << *(portnames+i);
				if( jack_connect( client, *(portnames+i), "midi2input:midi_in" ) ){
					std::cout << " - FAILED\n";
				} else {
					std::cout << " - SUCCESS\n";
				}
				i++;
			}
		} else {
			std::cout << "Connecting to: " << autoconnect;
			if( jack_connect( client, autoconnect.c_str(), "midi2input:midi_in" ) ){
					std::cout << " - FAILED\n";
			} else {
					std::cout << " - SUCCESS\n";
			}

		}
	}

	while( 1 ) sleep( 1 );

	jack_client_close( client );
	lua_close( L );
	exit( 0 );
}


