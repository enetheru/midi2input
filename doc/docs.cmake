# NOTE: cant cache multi line strings in cmake, so just include this file
# whenever you need these strings

set( DOC_TITLE "midi2input" )

set( DOC_DESCRIPTION_SHORT
"Turns a midi device into a keyboard/mouse controller" )

set( DOC_DESCRIPTION
"This program takes midi events from jackd and translates them to input events.
Configuration files and scripts are written in Lua" )

set( DOC_AUTHORS "Samuel Nicholas <nicholas.samuel@gmail.com>" )
