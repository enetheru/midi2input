![m2i](https://gitlab.com/enetheru/midi2input/raw/master/res/icons/m2i-black.png "m2i")
=======
m2i(short for midi 2 input) takes midi events and translates them to keyboard and mouse input events.
Configuration files and scripts are written in Lua

m2i Uses
--------
* Linux
* adishavit/argh - command line processor
* libfmt/fmt - simple formatting
* lua 5.2 - the script engine
* libx11 & libxtst - for x11 keybord and mouse input and window WM_CLASS detection
* either alsa-lib or jack/jack2 - for midi sequensor input
* qt5-base - for system tray icon

Build Instructions
==================
* install dependencies:
    * build time: git cmake openimageio gtk-update-icon-cache
    * required: lua-5.2, alsa-lib, libx11, libxtst
    * Optional: jack | jack2, qt5-core
* git clone https://gitlab.com/enetheru/midi2input.git
* cd midi2input
* git submodule update --init --recursive
* mkdir build
* cd build
* cmake ../midi2input/
* make && make install
* gtk-update-icon-cache -q -t -f usr/share/icons/hicolor

Arch
----
* mkdir m2i
* curl https://gitlab.com/enetheru/midi2input/raw/master/pkg/PKGBUILD > m2i/PKGBUILD
* cd m2i
* makepkg -si

Features
========
* lua script reactions to midi events
* sending of midi events
* routing via alsa or jack using standard tools
* sending keyboard and mouse events from scripts
* executing commands from scripts
* per application control by switching on WM_CLASS attribute
* Qt5 based system tray icon
* detection and reloading of changed script file
* reconnect to device after unplug/plug

Why?
====
Because I had a midi controller, and I thought it was a shame that I could not control my pc using it. I did a little research online and it didn't seem that there was an existing solution that fit my needs, indeed I didn't find a solution at all that was FLOSS.

| Intro Vid |
| --------- |
| [![Intro Vid](https://img.youtube.com/vi/wr1AqlDXnYI/0.jpg)](http://www.youtube.com/watch?v=wr1AqlDXnYI) |

```
USAGE: ./m2i [options]
OPTIONS:
   -h  --help                  Print usage and exit
   -v  --verbose               Output more information
   -c  --config <config.lua>   Specify config file
   -s  --script <script.lua>   Specify script file
   -a  --alsa                  Use ALSA midi backend
   -j  --jack                  Use Jack midi backend
```

examples
========
* using my controller play/pause, jogwheel, volume, tempo etc to dbus-send to currently running media player
* using the jobwheel and buttons to have a custom controller for video editing software
* A sound and video board and controlling cameras for video streaming
* jogwheel as mouse scroll
* shop demo for turning on and off lights

Configuration
=============
default: $XDG_CONFIG_HOME/m2i/config.lua

The location can be specified on the command line with the -c flag.

It sets up the initial behaviour of the m2i program.

```
config = {
    verbose = false,
    quiet = false,
    script = 'basic.lua',
    use_alsa = true,
    use_jack = false,
    reconnect = true,
    loop_enable = true,
    main_freq = 25,
    loop_freq = 250,
    watch_freq = 2500,
}
```
* *verbose* - increase verbosity.
* *quiet* - decrease verbosity.
* *script* - filename and path of script to use. if path is relative will look in current directory first, then $XDG_CONFIG_HOME/m2i/ and can be overridden with cmd line options.
* *use_alsa* - enable alsa sequensor subsystem.
* *use_jack* - enable jack sequensor subsystem.
* *reconnect* - currently only used for jack reconnection after disconnect but it doesnt work properly at the moment. and will go away in the future with better lua handling.
* *loop_enabled* - is true by default, but will set itself to false if the loop function fails.
* *main_freq* - in milliseconds; minimum frequency of the main loop. I believe there is a better way to do this, but I havent figured it out yet.
* *loop_freq* - in milliseconds; how often to run loop() function in the script.lua. I want this to go away in favour of being able to register looping, timed, etc functions into a scheduler.
* *watch_freq* - in milliseconds; how often to check for filesystem changes to script.lua so as to reload. also how often to check for jack disconnection. Again I want this to go away in favour of a scheduler.

Script
======
default: $XDG_CONFIG_HOME/m2i/script.lua

The location can be specified on the command line with the -s flag.

look in /usr/share/m2i/ for examples

m2i watches the file for changes and reloads the lua environment and script, allowing you to make changes and not have to reload manually.

The only user defined function that is required in the script.lua is midi_recv( ... ), this function is called for every lua event that is received by m2i and is where you react to events. In the future I believe i may want more functions like this for connect/disconnect of ports. quit etc..

User Defined functions:
* script_init()
* loop()
* midi_recv( status, data1, data2 )

Registered c functions:
* print( 'string' ... )
* midi_send( { status, data1, data2 } )
* loop_enabled()
* exec( 'command' )
* quit()

X11 c functions:
* keypress( XK_keycode )
* keydown( XK_keycode )
* keyup( XK_keycode )
* buttonpress( n )
* buttondown( n )
* buttonup( n )
* mousemove( x, y )
* mousepos( x, y )

Alsa and Jack c functions:
* alsaconnect( 'client', 'port' )
* jackconnect( 'client', 'port' )

Global Variables:
* WM_CLASS
