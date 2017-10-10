${DOC_TITLE}
=======
[![Build Status](https://travis-ci.org/enetheru/midi2input.svg?branch=master)](https://travis-ci.org/enetheru/midi2input)

${DOC_DESCRIPTION}

**Requires:**
  * lua

**Compile-Time Optional**
  * X11 with the XTest extension
  * Jack audio connection it

**Run-Time Optional**
  * Alsa midi backend
  * Jack midi backend

**Features:**
  * Lua scriptable
  * window detection with WM_CLASS property
  * keyboard and mouse vent simulation using the xtst lib
  * midi send and receive from jack and/or alsa

the above allows things like:
  * conditional per application event processing
  * manipulation of applications via their hotkey configurations
  * 

**Why?:**<br>
Because I had a midi controller, and I thought it was a shame that I could not
control my pc using it. I did a little research online and it didn't seem that
there was an existing solution that fit my needs, indeed I didn't find a
solution at all that was FLOSS.

I use my controller for things like:
  * custom controller in video editing applications, jogging, key commands
  * system wide audio control via dbus-send and alsactl
  * play/pause, jogwheel in vlc nd other media players

Things I could see it used for:
  * creating demo's for audio shops to manipulate the controllers lights
  * custom controller for a video game in specific scenario's

| Intro Vid |
| --------- |
| [![Intro Vid](https://img.youtube.com/vi/wr1AqlDXnYI/0.jpg)](http://www.youtube.com/watch?v=wr1AqlDXnYI) | 
