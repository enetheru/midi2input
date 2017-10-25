Notes
=====

Brainstorm Future Features
================
* expose dbus interface
* avahi connection host
* connection dialog
* quick event mapping tool
* network midi receive
* phone application
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

Lua
----
* jackconnect( clientname, portname )
* reset lua state
* load config file
* [DONE] enable and disable loop
* [DONE] alsaconnect( clientname, portname )
* [DONE] loop function for things like flashing lights, timed effects etc.

ALSA
----
* trigger lua from specific event inputs like connection and disconnection

Jack
----
* carla patchbay interface

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
* basic configuration and script
* proper man pages
* desktop shortcut
* packages
    * rpm
    * arch
    * deb
    * gzip
    * source

Project
-------
* Tests
* CI/CD
* Fuzzing
