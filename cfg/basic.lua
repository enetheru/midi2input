--[[ Imported c++ Functions ]]--
--initialise()
--keypress( XK_keycode )
--keydown( XK_keycode )
--keyup( XK_keycode )
--buttonpress( n )
--buttondown( n )
--buttonup( n )
--mousemove( x, y )
--mousepos( x, y )
--midi_send( { status, data1, data2 } )
--exec( 'command' )
--
--[[ Imported Global Variables ]]--
--wm_class
--autoconnect
--
--[[ Functions you must create ]]
--midi_recv( status, data1, data2 )
--loop()

------------------------

--[[ Simple Example ]]--

--[[ Defines ]]--
-- look to X11\keysymdef.h for the full list
XK_space                       = 0x0020  --/* U+0020 SPACE */

controller = {
    deck = {
        A = {
            play = {0x90, 0x0B, 127}
        }
    }
}

--[[ global settings ]]--
-- autoconnect: can be true, false, or a named jack port. default = true
autoconnect = false

--[[ Pattern matcher for messages ]]--
--
-- Both pattern and message share the same structure: {status, data1, data2}.
-- For any element of the pattern is equal -1, corresponding element of the
-- message is ignored / considered equal.
function message_matches( pattern, message )
    if pattern[1] ~= -1 then
        if pattern[1] ~= message[1] then return false end
    end
    if pattern[2] ~= -1 then
        if pattern[2] ~= message[2] then return false end
    end
    if pattern[3] ~= -1 then
        if pattern[3] ~= message[3] then return false end
    end
    return true
end

--[[ initialisation function ]]--
-- run immediately after the application launches and connects to the device
function script_init()
    print( "nothing to initialise" )
end

function loop()
    detectwindow()
end

--[[ Input Event Handler ]]--
function midi_recv( status, data1, data2 )
    local message = { status, data1, data2 }

    if( message_matches( controller.deck['A'].play, message ) ) then
        keypress( XK_space )
    end
end
