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

Documentation
-------------
so on inspection of wanting to update the documentation i have come to the
conclusion that:
* README.md - is primarily an advertisement and depth of description and not used for
reference
* man page - is the primary reference inforamtion and it might be possible to manually update the README.md by tacking on the man page at the bottom.

so README.md = advertisement + deep description + man page
that way it becomes easier to manage.

so an idea i had which was to convert the man page to markdown and tack it onto
the end of README.md turned out to be harder than i expected.

it appears as though nobody is really converting man to markdown, they do it
the other way which IMHO seems weird.. anyway.

I needed to get http://www.kylheku.com/cgit/man/
so that i had a better man2html converter, beacuse the one that comes with arch
is shit.

then i used pandoc to convert that to markdown and i still have to clean it up.
the commands are thus:
* build project first
* man2html m2i.1 > temp.html
* pandoc -f html -t markdown temp.html temp.md

or all in one go

cmake ../ && ~/bin/man2html < m2i.1 > test.html && pandoc -f html -t markdown test.html > test.md

then manually edit.. its notas good as automatic but it isnt totally shit.

this helps:
https://liw.fi/manpages/
