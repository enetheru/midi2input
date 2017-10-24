Notes
=====

TODO lua functions
==================
* jack_connect( direction, named port )
* alsa_connect( direction, named port )
* reset lua state
* loop function for things like flashing lights, timed effects etc.
* enable and disable loop

Future Features
================
* native dbus
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

System Tray
-----------
using the qt framework, the system tray makes a visible indicator for the service, and provide manipulation options, initially it will be built into the application because its easier. but eventually it should be split out and then use dbus to controll the service

* visible indicator of the application running
* control of things like
    * connected ports
    * auto-reconnect
    * reload configuration
    * quit

The primary requirement I have is that m2i stays alive
even even after an alsa or jack failure, and will reconnect automatically. that way i can just have it sit in the background on startup, when I connect my device it makes its connection by itself.

Configuration
=============
firstly cmd line options
secondly XDG_CONFIG_HOME/m2i/config
separation between m2i behaviour and action and response.

there is a design choice i am having to make in regards to reloading configuration

on the one hand, command line options should reign supreme for loading the state of the application. so in this way reloading the config either needs to be aware of the cmd line, or dissallow it because it cant tell what was overridden, plus you might want to override the initial values anyway, so i've come to a compromise i think is sane.

configuration and cmd line options only specify the initial state of the application it will not be reloaded, nor referenced further than the very first setup.

the script can be reloaded and the state changed in the script, but not from configuration.

system tray
===========
kind of thinking of things i want to have added to the system tray
* menu's for connecting to ports
* help dialog
* configuration dialog
* temporarily disable/enable
* quit
* bubbles as demonstrated from the qt example for things like auto connection events
* changing icon for when no midi ports are connected
* changing icon or bubbles for when frequency threshold is reached

packaging
=========
missing things
* basic configuration script
* proper man pages
* desktop shortcut
