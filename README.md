midi2input
=======
This program takes midi events and translates them to input events.
Configuration files and scripts are written in Lua

**Requires:**
  * X11 with the XTest extension
  * jack or alsa
  * lua

**depends**
additional to the above we use:
* adishavit argh

**Features:**
  * ability to map events from midi device to keyboard and mouse
  * some knowledge about currently focused application
  * scriptable with lua
  * ability to send midi events to the controller, for control of led buttons, etc

**Why?:**<br>
Because I had a midi controller, and I thought it was a shame that I could not
control my pc using it. I did a little research online and it didn't seem that
there was an existing solution that fit my needs, indeed I didn't find a solution
at all that was FLOSS.

| Intro Vid |
| --------- |
| [![Intro Vid](https://img.youtube.com/vi/wr1AqlDXnYI/0.jpg)](http://www.youtube.com/watch?v=wr1AqlDXnYI) | 

```
USAGE: ./midi2input [options]

GENERAL OPTIONS:
  -h  --help   Print usage and exit.
  -c  --config Specify config file, default = ~/.config/midi2input.lua
```
