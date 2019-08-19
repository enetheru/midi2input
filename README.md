![m2i](https://gitlab.com/enetheru/midi2input/raw/master/res/icons/m2i-black.png "m2i")
=======
m2i(midi to input) is a small service like applicatin that runs scripted actions in response to to midi events. Actions can be mouse, keyboard events, commands and more midi events. m2i can receive midi events from either the ALSA and or Jack midi sequenser.

m2i Uses
--------
* Linux
* adishavit/argh - command line processor
* libfmt/fmt - simple formatting
* lua 5.3 - the script engine
* libx11 & libxtst - for x11 keybord and mouse input and window WM_CLASS detection
* either alsa-lib or jack/jack2 - for midi sequensor input
* qt5-base - for system tray icon

Build Instructions
==================

Compile 
-------

* install dependencies:
    * build time: git cmake openimageio gtk-update-icon-cache
    * required: lua-5.3, alsa-lib, libx11, libxtst
    * Optional: jack | jack2, qt5-core
* git clone https://gitlab.com/enetheru/midi2input.git
* cd midi2input
* git submodule update --init --recursive
* mkdir build
* cd build
* cmake ../midi2input/
* make

Install 
-------

After the above compile instruction

* `make install`
* `gtk-update-icon-cache -q -t -f usr/share/icons/hicolor`

Arch
----
* mkdir m2i
* curl https://gitlab.com/enetheru/midi2input/raw/master/pkg/PKGBUILD > m2i/PKGBUILD
* cd m2i
* makepkg -si

Debian/Ubuntu
-------------

After the above compile instruction

* `make package`
* `dpkg -i m2i-*-Linux.deb`

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

Running
=======

```
m2i --help
m2i --config cfg/configua --script cfg/basic.lua
m2i --config cfg/configua --script cfg/basic.lua --verbose
```

If you are setting up a new device and using alsa, you have to connect the MIDI 
device to alsa : 

* get the device name with  

```
aconnect -l
client 24: 'ProductName' [type=kernel,card=2]
    0 'ProductName MIDI 1'
client 128: 'midi2input_alsa' [type=user,pid=22962]
    0 'in              '
    1 'out             '

```

* connect it 

```
aconnect 'ProductName' 'midi2input_alsa'
aconnect 'midi2input_alsa':1 'ProductName'

```

You should now see  `[ALSA]Port Subscription from: 'ProductName':'ProductName MIDI 1'` 
in the logs. 

Why?
====
Because I had a midi controller, and I thought it was a shame that I could not control my pc using it. I did a little research online and it didn't seem that there was an existing solution that fit my needs, indeed I didn't find a solution at all that was FLOSS.

| Intro Vid |
| --------- |
| [![Intro Vid](https://img.youtube.com/vi/wr1AqlDXnYI/0.jpg)](http://www.youtube.com/watch?v=wr1AqlDXnYI) |

| Adding a new Mapping |
| -------------------- |
| [![Adding a new mapping](https://i9.ytimg.com/vi/T_s2V2JzHwo/mq3.jpg?sqp=CJSb5ekF&rs=AOn4CLAi1M4P4ZJUNhatsiuYcRG60_Qakw)](https://www.youtube.com/embed/T_s2V2JzHwo) |

examples
========
* using my controller play/pause, jogwheel, volume, tempo etc to dbus-send to currently running media player
* using the jobwheel and buttons to have a custom controller for video editing software
* A sound and video board and controlling cameras for video streaming
* jogwheel as mouse scroll
* shop demo for turning on and off lights


Excerpt from man-page
=====================

OPTIONS
-------

**-h, --help** 
:   print help text and exit

**-v, --verbose** 
:   output more information

**-q, --quiet** 
:   output less information

**-c, --config** 
:   specify the configuration script to use

**-s, --script** 
:   specify the script to use

**-a, --alsa** 
:   enable alsa midi sequensor

**-j, --jack** 
:   enable jack midi sequensor

FILES
-----
m2i uses a config file and a script file. examples are available in
/usr/local/share/m2i [ ]{#lbAG}

### \$XDG\_CONFIG\_HOME/m2i/config.lua

Specifies the default configuration, command line options override these
values if given. These values are not guaranteed for the lifetime of the
program, as they are only loaded once on startup, and will dissapear if
a call to reload is received from script file monitoring.

eg.
```
    config = {
        key = value,
        ...
    }
```

**verbose, bool** 
:   more information sent to stdout

**quiet, bool** 
:   less information send to stdout

**script, string** 
:   path of the script file to load, if relative will look in current directory, and then \$XDG\_CONFIG\_HOME/m2i

**use\_alsa, bool** 
:   enable the alsa sequensor backend

**use\_jack, bool** 
:   enable the jack sequensor backend

**reconnect, bool** 
:   will attempt to reconnect to jack during the watch loop cycle. this option will be removed when a better solution is implemented.

**loop\_enable, bool** 
:   enables evaluation of the loop() function implemented in the script.lua, and will be removed once a decent scheduler is implemented.

**main\_freq, int** 
:   frequency in milliseconds of the main loop.

**loop\_freq, int** 
:   frequency in milliseconds of the loop() function implemented in the script.lua

**watch\_freq, int** 
:   frequency in milliseconds of the file monitoring and jack reconnection checks.

[ ]{#lbAH}

### \$XDG\_CONFIG\_HOME/m2i/script.lua

The location can be specified on the command line with the -s flag. or
in the config.lua as written above

a minimal example:
```
    function script_init()
        print( "nothing to init" )
    end
    function midi_recv( status, data1, data2 )
        print( status, data1, data2 )
    end
```
Look in /usr/local/share/m2i/ for more examples

The script is watched using inotify and when changed the lua environment
is reset and the script reloaded. This allows you to make changes and
not have to reload m2i manually.

### User defined lua functions:

These are the functions that the main application expect to find in your
script, and will call them as needed.

The only user defined function that is required in the script.lua is
midi_recv( ... ), this function is called for every lua event that is
received by m2i and is where you react to events. In the future I
believe i may want more functions like this for connect/disconnect of
ports. quit etc.

**script\_init()** 
:   runs once immediately after loading the script.

**loop()** 
:   runs at a frequency of loop\_freq as specified in the config.lua

**midi\_recv( status, data1, data2 )** 
:   Is the only actually required function that all events processed
    through.

### m2i lua functions:

m2i provides

**print( 'string' ... )** 
:   an overload of the lua print functionality to tie it to the main
    application.

**midi\_send( { status, data1, data2 } )** 
:   sends midi events to the output port, useful for turning on and off
    controller lights

**loop\_enabled()** 
:   enables the loop() function

**exec( 'command' )** 
:   runs an arbitrary command with whatever privilages the command was
    run with, probably a bad idea really, but so long as its just a user
    application this is quite useful.

**quit()** 
:   causes the main loop to exit, ending the application

### X11 functions:

these are functions which relate the X11 windowing system, for keyboard
and mouse input, and also for window detection

**keypress( XK\_keycode )** 
:   equivalent to a keydown+keyup event

**keydown( XK\_keycode )** 

:   

**keyup( XK\_keycode )** 

:   

**buttonpress( n )** 

:   

**buttondown( n )** 

:   

**buttonup( n )** 

:   

**mousemove( x, y )** 

:   

**mousepos( x, y )** 

:   

### Alsa and Jack functions:

additional commands for alsa and jack subsystems

**alsaconnect( 'client', 'port' )** 

:   

**jackconnect( 'client', 'port' )** 

:   

### Global variables:

globals

**WM\_CLASS** 
:   is provided for differentiating between applications at the moment, a more robust solution would be preferable but this works well enough for now.

AUTHOR
------
Samuel Nicholas &lt;nicholas.samuel@gmail.com&gt;

SEE ALSO
--------
lua(1), jackd(1), &lt;X11/keysymdef.h&gt;
