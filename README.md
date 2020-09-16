![m2i](https://gitlab.com/enetheru/midi2input/raw/master/res/icons/m2i-black.png "m2i")
=======
m2i(midi to input) is a small service like application that runs scripted
actions in response to to midi events. Actions can be mouse, keyboard events,
commands and more midi events. m2i can receive midi events from either the ALSA
and or Jack midi sequencer.

m2i dependencies
----------------
* Linux
* [argh](https://github.com/adishavit/argh) - command line processor
* [libfmt](https://github.com/fmtlib/fmt) - simple formatting
* [spdlog](https://github.com/gabime/spdlog) - fast logging
* lua 5.3 - the script engine
* libx11 & libxtst - for x11 keybord and mouse input and window WM_CLASS detection
* either alsa-lib or jack/jack2 - for midi sequencer input
* qt5-base - for system tray icon

Installation Instructions
=========================
Check INSTALL[.OS].md for build and install instructions

Features
========
* lua script reactions to midi events
* sending of midi events
* routing via ALSA or jack using standard tools
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
m2i --config cfg/config --script cfg/basic.lua
m2i --config cfg/config --script cfg/basic.lua --verbose
```

If you are setting up a new device and using ALSA, you have to connect the MIDI 
device to ALSA :

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

You should now see  `ALSA: Port Subscription from: 'ProductName':'ProductName MIDI 1'` 
in the logs.

Why?
====
Because I had a midi controller, and I thought it was a shame that I could not
control my pc using it. I did a little research online and it didn't seem that
there was an existing solution that fit my needs, indeed I didn't find a
solution at all that was FLOSS.

| Intro Vid |
| --------- |
| [![Intro Vid](https://img.youtube.com/vi/wr1AqlDXnYI/0.jpg)](http://www.youtube.com/watch?v=wr1AqlDXnYI) |

| Adding a new Mapping |
| -------------------- |
| [![Adding a new mapping](https://i9.ytimg.com/vi/T_s2V2JzHwo/mq3.jpg?sqp=CJSb5ekF&rs=AOn4CLAi1M4P4ZJUNhatsiuYcRG60_Qakw)](https://www.youtube.com/embed/T_s2V2JzHwo) |

Examples
========
* Using my midi controller play/pause, jogwheel, volume, tempo buttons to
  control media player
* Using the jogwheel and buttons to have a custom controller for video editing
  software
* A sound and video board and controlling cameras for video streaming
* Jogwheel as mouse scroll
* Shop demo for turning on and off lights
* Turn a keboard/piano into a puzzle for escape rooms

Man page dump
=============

```
M2I(1)                          m2i Reference                          M2I(1)

NAME
       m2i - midi input -> keyboard|mouse|midi|command output

SYNOPSIS
       m2i [OPTIONS]

DESCRIPTION
       m2i(midi  to  input)  is  a  small  service  like applicatin that runs
       scripted actions in response to to midi events. Actions can be  mouse,
       keyboard  events,  commands and more midi events. m2i can receive midi
       events from either the ALSA and or Jack midi sequenser.

OPTIONS
       -h, --help
              Print help text and exit

       -v, --verbose
              Output more information

       -q, --quiet
              Output less information

       -c, --config
              Specify the configuration script to use

       -s, --script
              Specify the script to use

       -a, --alsa
              Enable ALSA midi sequencer

       -j, --jack
              Enable jack midi sequencer

FILES
       m2i uses a config file and a script file. Examples  are  available  in
       /usr/share/m2i

   $XDG_CONFIG_HOME/m2i/config.lua
       Specifies  the  default  configuration,  command line options override
       these values if given.  These values are not guaranteed for the  life‐
       time  of  the  program,  as they are only loaded once on start up, and
       will disappear if a call to reload is received from script file  moni‐
       toring.

       eg.
           config = {
               key = value,
               ...
           }

       verbose = true|false
              More information sent to stdout

       quiet = true|false
              Less information send to stdout

       script = path
              Path  of the script file to load, if relative will look in cur‐
              rent directory, and then $XDG_CONFIG_HOME/m2i

       use_alsa = true|false
              Enable the ALSA sequencer back end

       use_jack = true|false
              Enable the jack sequencer back end

       reconnect = true|false
              Will attempt to reconnect to jack during the watch loop  cycle.
              This  option  will  be removed when a better solution is imple‐
              mented.

       loop_enable = true|false
              Enables evaluation of the loop() function  implemented  in  the
              script.lua,  and will be removed once a decent scheduler is im‐
              plemented.

       main_freq = integer
              Frequency in milliseconds of the main loop.

       loop_freq = integer
              Frequency in milliseconds of the loop() function implemented in
              the script.lua

       watch_freq = integer
              Frequency  in  milliseconds of the file monitoring and jack re‐
              connection checks.

   $XDG_CONFIG_HOME/m2i/script.lua
       The location can be specified on the command line with the -s flag  or
       in the config.lua as written above

       A minimal example:
           function script_init()
               print( "Nothing to init" )
           end

           function midi_recv( status, data1, data2 )
               print( status, data1, data2 )
           end

       Look in /usr/share/m2i/ for more examples

       The  script is watched using inotify and when changed the lua environ‐
       ment is reset and the script reloaded. This allows you to make changes
       and not have to reload m2i manually.

   User defined lua functions:
       midi_recv( status, data1, data2 )
              This is the only user defined function that you are required to
              implement in your script.  midi_recv( ... ) is called for every
              lua  event  that  is  received by m2i and is where you react to
              events.

       script_init()
              Runs once immediately after loading the script.

       loop() Runs at a frequency of loop_freq as specified in the config.lua

   m2i Inbuilt lua functions:
       These functions are implemented by m2i to use in your script.

       print( 'string' ... )
              An overload of the lua print functionality to  tie  it  to  the
              main application.

       midi_send( { status, data1, data2 } )
              Sends midi events to the output port to connected devices.

       loop_enabled()
              Enables the loop() function

       exec( 'command' )
              Runs  an  arbitrary  command  with  whatever privileges the m2i
              utility has, probably a bad idea really, but  so  long  as  its
              just a user application this is quite useful.

       quit() Causes the main loop to exit, ending the application

   X11 functions:
       These  lua  functions  to interface with the X11 windowing system, for
       keyboard and mouse input.

       keypress( XK_keycode )

       keydown( XK_keycode )

       keyup( XK_keycode )

       buttonpress( n )

       buttondown( n )

       buttonup( n )

       mousemove( x, y )

       mousepos( x, y )

   ALSA and Jack functions:
       These lua functions to interface with ALSA and Jack sequencers

       alsaconnect( 'client', 'port' )

       jackconnect( 'client', 'port' )

   Global variables:
       WM_CLASS
              Is provided for differentiating between applications. It is up‐
              dated  during  the main loop with the contents of the currently
              focused window's X11 property WM_CLASS. Using this you can pro‐
              vide a different control mechanism per application.

AUTHORS
        Samuel Nicholas <nicholas.samuel@gmail.com>
        Urs Liska
        Arthur Lutz
        Marcin Świgoń
        Jarrod Whittaker

SEE ALSO
       lua(1), jackd(1), <X11/keysymdef.h>

                                  2020-09-16                           M2I(1)
```
