#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <wordexp.h>
#include <unistd.h>

#include <jack/jack.h> 
#include <jack/midiport.h> 

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

Display* xdp;
jack_client_t *client;
jack_port_t *input_port;
KeyCode map[ 128 ];

FILE *
load_config( char *filename )
{
	//TODO use a queue of filepaths and c++ streams
	// load configuration from a priority list of locations
	// * specified from the command line
	// * current directory ./.midi2input.cfg
	// * configuration folder $HOME/.config/
	// * home directory $HOME/
	FILE *tFile;
	char filepath[ 1024 ];

	// cmd line
	tFile = fopen( filename, "r" );
	if(  tFile != NULL ){
		printf( "cfg: %s\n", filename );
		return tFile;
	}

	// current working directory
	char cwd[ 1024 ];
	getcwd( cwd, 1024 );
	
	sprintf( filepath, "%s/.midi2input.cfg", cwd );

	tFile = fopen( filepath, "r" );
	if(  tFile != NULL ){
		printf( "cfg: %s\n", filepath );
		return tFile;
	}
	// configuration folder ~/.config/
	sprintf( filepath, "%s/.config/midi2key.cfg", getenv( "HOME" ) );

	tFile = fopen( filepath, "r" );
	if(  tFile != NULL ){
		printf( "cfg: %s\n", filepath );
		return tFile;
	}
	
	// configuration folder ~/.config/
	sprintf( filepath, "%s/.midi2key.cfg", getenv( "HOME" ) );

	tFile = fopen( filepath, "r" );
	if(  tFile != NULL ){
		printf( "cfg: %s\n", filepath );
		return tFile;
	}

	return NULL;
}

int
process( jack_nframes_t nframes, void *arg )
{
    int temp;
    void* port_buf = jack_port_get_buffer( input_port, nframes );
    jack_midi_event_t in_event;
    jack_nframes_t event_count = jack_midi_get_event_count( port_buf );
    
    if( event_count > 0 ){
        for(uint32_t i = 0; i < event_count; i++ ){
            jack_midi_event_get( &in_event, port_buf, i );

            if( in_event.buffer[ 0 ] == 0x90 ){
                printf("note on\n");
                if( (in_event.buffer[1] <= 127) && (in_event.buffer[2]) ){
                    if( (temp = map[ in_event.buffer[ 1 ] ]) ){
                        XTestFakeKeyEvent( xdp, temp, 1, CurrentTime );
                        XTestFakeKeyEvent( xdp, temp, 0, CurrentTime );
                        XFlush( xdp );
                    }
                    printf( "    size: %i | 0x%02X | 0x%02X | 0x%02X >> %i\n",
                        (int)in_event.size,
                        in_event.buffer[ 0 ],
                        in_event.buffer[ 1 ],
                        in_event.buffer[ 2 ],
                        map[ in_event.buffer[ 1 ] ]
                    );
                }
            }

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
    if( argc == 2 && strcmp(argv[1], "-h") == 0 ){
        fprintf( stderr,
            "Usage: %s [config]\n"
            "Defaults to ~/.config/midi2input.cfg if config is unspecified.\n"
            "Configuration example:\n"
            "#set note #32 to spacebar\n"
            "32=space\n"
            "#set note #33 to keycode 67\n"
            "33=67\n"
            "#leave note #34 unset\n"
            "34=\n"
            "#also set note 35 to space\n"
            "35=space\n",
            *argv
        );
        return 1;
    }

    /*display*/
    if(! (xdp = XOpenDisplay( getenv( "DISPLAY" ) )) ){
        fputs( "Unable to open X display.", stderr );
        return 2;
    }

    /*jack*/
    if( (client = jack_client_open( "midi2input", JackNullOption, NULL )) == 0 ){
        fprintf( stderr, "jack server not running?\n" );
        return 1;
    }

    jack_set_process_callback( client, process, 0 );
    jack_on_shutdown( client, jack_shutdown, 0 );

    input_port = jack_port_register( client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0 );

    if( jack_activate( client ) ){
        fprintf( stderr, "cannot activate client" );
        return 1;
    }

    /*defines*/
    FILE* config;
    char buffer[128];
    char* keysym;
    int temp;

    /*config*/
    memset( map, 0, sizeof( map ) );
	
    if(! (config = load_config( argv[ 1 ] )) ){
        fprintf( stderr, "Unable to open configuration file\nUsing null mapping.\n" );
    }
    else {
        while( fgets( buffer, 128, config ) ){
            if( *buffer != '#' ){
                if( (keysym = strchr( buffer, '=' )) ){
                    temp = atoi( buffer );
                    if( (temp >= 0) && (temp <= 127) ){
                        ++keysym;
                        if( strchr( keysym, '\n' ) ) *strchr( keysym, '\n' ) = '\0';
                        if( (map[ temp ] = XStringToKeysym( keysym )) ){
                            if(! (map[ temp ] = XKeysymToKeycode( xdp, map[ temp ] )) )
                                fprintf( stderr, "No keycode for keysym '%s'\n", keysym );
                        }
                        else if( *keysym ){
                            fprintf( stderr, "Undefined keysym '%s'\n", keysym );
                        }
                    }
                }
            }
        }
        fclose( config );
    }
    
    while(1)
    {
        sleep(1);
    }
    jack_client_close(client);
    exit (0);

    return 0;
}


