m2i
=======
This program takes midi events and translates them to input events.
Configuration files and scripts are written in Lua

Requires
--------
* lua
* adishavit argh
* jack OR alsa
* [optional] X11 with the XTest extension

Features
--------
* lua script reactions to midi events
* sending of midi events
* routing via alsa or jack using standard tools
* sending keyboard and mouse events from scripts
* executing commands from scripts
* per application control by switching on WM_CLASS attribute

Why?
====
Because I had a midi controller, and I thought it was a shame that I could not
control my pc using it. I did a little research online and it didn't seem that
there was an existing solution that fit my needs, indeed I didn't find a solution
at all that was FLOSS.

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
* using my controller play/pause, jogwheel, volume, tempo etc to dbus-send to 
  currently running media player
* using the jobwheel and buttons to have a custom controller for video editing
  software
* A sound and video board and controlling cameras for video streaming
* jogwheel as mouse scroll
