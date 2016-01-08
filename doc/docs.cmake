# NOTE: cant cache multi line strings in cmake, so just include this file
# whenever you need these strings

set( DOC_TITLE "midi2input" )
set( DOC_DESCRIPTION_SHORT
"Turns a midi device into a keyboard/mouse controller" )

set( DOC_DESCRIPTION
"This program takes midi events from jackd and translates them to input events.
Configuration is written in Lua it doesnt matter if i break spaces with this so
long as it gets done right" )

set( DOC_AUTHORS "Samuel Nicholas <nicholas.samuel@gmail.com>" )

