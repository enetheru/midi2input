Notes
=====

Compile-time Options
--------------------
-DWITH_XORG - enabled xorg related functionality like window class detection and input functions.
-DWITH_JACK - enables jack midi backend
-DWITH_ALSA - enables alsa midi backend

Run-time Options
----------------------
having a cmd line options for 
* -a --alsa = enable alsa backend
* -j --jack = enable jack backend
* [BROKEN] -q --quiet = supress all stdout and stderr
* [BROKEN] -v --verbose = more stdout
* -c --config = specify the configuration file to use eg. '-c $HOME/config.lua'

TODO lua functions
==================
* jack_connect( direction, named port )
* alsa_connect( direction, named port )
* reset lua state
* loop function for things like flashing lights, timed effects etc.
* enable and disable loop

Future Features
================
* system tray
* alsa midi
* jack midi
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
* inotify support
* multiple config with override if possible.

things i would like to see is to actually turn a keyboard into a midi
controller not just map the keys, but use an alternative driver to talk to the
kernel to make it think it its a device, xy controls from trackpad or
touchscreen or mouse etc.

System Tray
-----------
using the qt framework, the system tray would make a visible indicator for the
service, and provide manipulation options, initially it will be built into the
application because its easier. but eventually it should be split out and then
use dbus to controll the service
* visible indicator of the application running
* control of things like
    * connected ports
    * auto-reconnect
    * reload configuration
    * quit

But why? I guess the primary requirement I have is that midi2input stays alive
even even after an alsa or jack failure, and will reconnect automatically. that
way i can just have it sit in the background on startup. what happens now?

alsa backend is kinda always there, so it just stays alive
jack backend will fail to load if there is no jack server, but the application
will continue to run.
if the jack server quits then there is a jack failure during running but it
doesnt quit either. so at some point i will need to check for jack validity and
re-initialise the jack thingo.
