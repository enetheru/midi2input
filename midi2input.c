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

lua_State *L;
Display* xdp;
jack_client_t *client;
jack_port_t *input_port;
KeyCode map[ 128 ];

std::string
load_config(  std::string name )
{
	//TODO use a queue of filepaths and c++ streams
	// load configuration from a priority list of locations
	// * specified from the command line
	// * configuration folder $HOME/.config/
	// * home directory $HOME/
	std::string filepath;
	std::queue<std::string> paths;

	paths.push( name );

	// configuration folder ~/.config/
	filepath = std::string( getenv( "HOME" ) ) + "/.config/midi2key.cfg";
	paths.push( filepath );

	// configuration folder ~/.config/
	filepath = std::string( getenv( "HOME" ) ) + "/.midi2key.cfg";
	paths.push( filepath );

	std::ifstream tFile;
	while(! paths.empty() ){
		tFile.open( paths.front().c_str(), std::ifstream::in );
		if( tFile.is_open() )
		   break;

		paths.pop();
	}

	tFile.close();
	std::cout << paths.front() << std::endl;
	return paths.front();
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
	KeyCode temp;
	void* port_buf = jack_port_get_buffer( input_port, nframes );
	jack_midi_event_t in_event;
	jack_nframes_t event_count = jack_midi_get_event_count( port_buf );

	if( event_count > 0 ){
		for(uint32_t i = 0; i < event_count; i++ ){
			jack_midi_event_get( &in_event, port_buf, i );

			//if( (in_event.buffer[1] <= 127) && (in_event.buffer[2]) ){
			//	if( (temp = map[ in_event.buffer[ 1 ] ]) ){
			//		XTestFakeKeyEvent( xdp, temp, 1, CurrentTime );
			//		XTestFakeKeyEvent( xdp, temp, 0, CurrentTime );
			//		XFlush( xdp );
			//	}
			std::cout << "channel: "
				<< std::setw(2)	<< (in_event.buffer[0] & 0x0F)+1;

			if( (in_event.buffer[0] >> 4) == 0x9 ){
				std::cout << " | on ";
			}
			if( (in_event.buffer[0] >> 4) == 0x8 ){
				std::cout << " | off";
			}
			std::cout << " | Note: "
				<< std::setw(3) << (int)in_event.buffer[ 1 ]
				<< " | Velocity: "
				<< std::setw(3) << (int)in_event.buffer[ 2 ]
				<< " >> " << (char)map[ in_event.buffer[ 1 ] ]
				<< std::endl;
			//}

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
	/*usage*/
	if( argc == 2 && std::string( argv[ 1 ] ).compare( "-h" ) == 0 ){
		std::cerr << "Usage: " << *argv << " [config]\n"
			"Defaults to ~/.config/midi2input.cfg if config is unspecified.\n"
			"Configuration example:\n"
			"#set note #32 to spacebar\n"
			"32=space\n"
			"#set note #33 to keycode 67\n"
			"33=67\n"
			"#leave note #34 unset\n"
			"34=\n"
			"#also set note 35 to space\n"
			"35=space\n";

		exit( 1 );
	}

	/*lua*/
	std::string autoconnect;
	L = luaL_newstate();   /* opens Lua */
    luaL_openlibs( L );

    if( luaL_loadfile( L, argv[ 1 ] ) || lua_pcall( L, 0, 0, 0 ) ){
		std::cout << "cannot run configuration file: " << lua_tostring( L, -1 )
			<< std::endl;
        exit(0);
    }

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
	int i = 0;
	const char ** portnames = jack_get_ports( client, ".midi.", NULL, JackPortIsOutput );
	if( true ){
		if(! portnames ) return 1;
		while( *(portnames + i) ){
			std::cout << "found: " << *(portnames+i) << std::endl;
			i++;
		}
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

	/*defines*/
	std::ifstream config;
	std::string cfg_fn;
	std::string cell;
	std::stringstream line;
	std::vector<std::string> tokens;
	//int temp;
//	int count = 0;

	/*config
	cfg_fn = load_config( argv[ 1 ] );
	if( cfg_fn.empty() ){
		std::cerr << "Unable to open configuration file\n"
			"Using null mapping.\n";
	}
	else {
		config.open( cfg_fn.c_str(), std::ifstream::in );

		while( std::getline( config, cell ) ){
			++count;
			if( cell[ 0 ] == '#' ) continue;
			line.clear();
			line.str( cell );

			tokens.clear();
			while( std::getline( line, cell, '=' ) ) tokens.push_back( cell );
			if( tokens.size() != 2 ) {
				std::cerr << "parse error on line " << count << std::endl;
				continue;
			}

			temp = std::stoi( tokens[ 0 ] );

			map[temp] = *tokens[1].c_str();
			// types of things
			// * simple buttons
			// * buttons with state, On or Off
			// * sliders & knobs with single prescision
			// * sliders & knobs with double precision
			// * continuous wheels

		}

		config.close();
	}*/

	while(1)
	{
		sleep(1);
	}
	jack_client_close(client);
	lua_close(L);
	exit (0);
}


