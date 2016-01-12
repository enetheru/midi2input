midi2input
=======
[![Build Status](https://travis-ci.org/enetheru/midi2input.svg?branch=master)](https://travis-ci.org/enetheru/midi2input)

This program takes midi events from jackd and translates them to input events.
Configuration is written in Lua it doesnt matter if i break spaces with this so
long as it gets done right

Requires:
  * X11 with the XTest extension
  * jack audio connection it
  * lua

https://youtu.be/wr1AqlDXnYI

```
USAGE: ./midi2input [options]

GENERAL OPTIONS:
  -h  --help   Print usage and exit.
  -c  --config Specify config file, default = ~/.config/midi2input.lua
```
