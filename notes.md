Notes
=====

Brainstorm Future Features
================
* expose dbus interface
* avahi connection host
* connection dialog
* quick event mapping tool
* network midi receive
* mobile app
* proper support of midi
* support for OSC(open sound control)
* support for regular input devices
* systemd service
* multiple config with override if possible.
* [DONE]inotify support
* [DONE]system tray
* [DONE]alsa midi
* [DONE]jack midi

things i would like to see is to actually turn a keyboard into a midi
controller not just map the keys, but use an alternative driver to talk to the
kernel to make it think it its a device, xy controls from trackpad or
touchscreen or mouse etc.

C++
----
* better main loop
* multi-threaded main loop
* scheduler that can have loops and timed events registered with associated c or lua functions.

Lua
----
* jackconnect( clientname, portname )
* reset lua state
* load config file
* [DONE] enable and disable loop
* [DONE] alsaconnect( clientname, portname )
* [DONE] loop function for things like flashing lights, timed effects etc.
* change signature of midi_recv to give a table with midi data
* rename all registered functions to have a consistent naming scheme

ALSA
----
* trigger lua from specific event inputs like connection and disconnection

Jack
----
* carla patchbay interface
* connect lua interface
* remove from watch loop
* triggers on disconnect

QT System Tray
-----------
* connect/disconnect/auto ports
* save configuration
* about/help dialog
* configuration dialog
* temporarily enable/disable
* changing icon indicating status:
    * for when no midi ports are connected
    * for when frequency threshold is reached

Packaging
---------
* [DONE]basic configuration and script
* proper man pages
* [DONE]desktop shortcut
* packages
    * rpm
    * gzip - requires release
    * [DONE]source - from git README has instructions
    * [DONE]arch
    * [DONE]deb

Project
-------
* Tests
* CI/CD
* Fuzzing

Icons
-----
preferably in svg but inkscape crashes on editing text atm :(
* disabled
* Info
* Warning/Error
* [DONE]default, neutral icon
* [DONE]light
* [DONE]dark
