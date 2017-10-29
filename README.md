m2i
=======
This program takes midi events and translates them to input events.
Configuration files and scripts are written in Lua

Requires
--------
* Linux
* lua 5.2
* adishavit/argh - command line processor
* libfmt/fmt - simple formatting

Optional dependencies
---------------------
* alsa-lib - for ALSA midi sequensor
* libx11 & libxtst - for x11 keybord and mouse input and window WM_CLASS detection
* qt5-base - for system tray icon
* jack or jack2 - for Jack midi sequensor

BUild Instructions
------------------
* sudo pacman -S git cmake lua52
    * sudo pacman -S [Optional Depdencies]
* git clone https://gitlab.com/enetheru/midi2input.git
* cd midi2input
* git submodule update --init --recursive
* mkdir build
* cd build
* cmake ../midi2input/
* make && make install

Features
--------
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
Configuration is reserved for the initial state of m2i, its values are pulled
directly by the c code and cannot be changed from the script file, as such
reloading the lua environment by modifying the script will have no effect on
these values. any script based configuration should be added to a script_init()
function.

priority:
1. cmd line options
2. $XDG_CONFIG_HOME/m2i/config.lua
