# NOTE: cant cache multi line strings in cmake, so just include this file
# whenever you need these strings

set( DOC_TITLE "M2I" )
set( DOC_NAME "m2i" )
string( TIMESTAMP DOC_DATE %Y-%m-%d )

set( DOC_DESCRIPTION_SHORT
"midi input -> keyboard|mouse|midi|command output" )

set( DOC_DESCRIPTION
    "${DOC_NAME}(midi to input) is a small service like applicatin that runs
scripted actions in response to to midi events. Actions can be mouse,
keyboard events, commands and more midi events. ${DOC_NAME} can receive midi
events from either the ALSA and or Jack midi sequenser." )

set( DOC_AUTHOR "Samuel Nicholas <nicholas.samuel@gmail.com> and contributors" )
